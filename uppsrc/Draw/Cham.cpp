#include "Draw.h"

struct ChVar : Moveable<ChVar> {
	int         count;
	byte       *state;
	Value     (*get)(int i);
	void      (*set)(int i, const Value& v);
};

static VectorMap<String, ChVar>& chVar()
{
	static VectorMap<String, ChVar> var;
	return var;
};

void ChRegisterVar__(const char *name, int n,
                     byte *state, Value (*get)(int i), void (*set)(int i, const Value& v))
{
	VectorMap<String, ChVar>& var = chVar();
	ASSERT(var.Find(name) < 0);
	ChVar& ch = var.Add(name);
	ch.count = n;
	ch.state = state;
	ch.get = get;
	ch.set = set;
}

void ChReset()
{
	VectorMap<String, ChVar>& var = chVar();
	for(int i = 0; i < var.GetCount(); i++)
		*var[i].state = 0;
}

void ChSet(const char *name, int i, const Value& val)
{
	VectorMap<String, ChVar>& var = chVar();
	ChVar& v = var.Get(name);
	*v.state = 2;
	(*v.set)(i, val);
	for(int i = 0; i < var.GetCount(); i++)
		if(*var[i].state == 1)
			*var[i].state = 0;
}

void ChSet(const char *name, const Value& val)
{
	VectorMap<String, ChVar>& var = chVar();
	ChVar& v = var.Get(name);
	for(int i = 0; i < v.count; i++)
		ChSet(name, i, val);
}

Value ChGet(const char *name, int i)
{
	VectorMap<String, ChVar>& var = chVar();
	return (*var.Get(name).get)(i);
}

Value ChGet(const char *name)
{
	return ChGet(name, 0);
}

struct sChRoutine : Moveable<sChRoutine> {
	String style;
	void  (*fn)();
};

static Vector<sChRoutine>& s_r()
{
	static Vector<sChRoutine> x;
	return x;
}

void ChRegister(const char *style, void (*fn)())
{
	sChRoutine& r = s_r().Add();
	r.style = style;
	r.fn = fn;
}

void ChRegister(void (*fn)())
{
	ChRegister(NULL, fn);
}

void StdChPainter(Draw& w, const Rect& r, const Value& v)
{
	if(IsType<Color>(v)) {
		w.DrawRect(r, Color(v));
	}
	if(IsType<Image>(v)) {
		Image b = v;
		Size isz = b.GetSize();
		Size sz = r.GetSize();
		Point p = b.GetHotSpot();
		w.Clipoff(r);
		w.DrawImage(0, 0, b, RectC(0, 0, p.x, p.y));
		w.DrawImage(0, sz.cy - p.y, b, RectC(0, isz.cy - p.y, p.x, p.y));
		w.DrawImage(sz.cx - p.x, 0, b, RectC(isz.cx - p.x, 0, p.x, p.y));
		w.DrawImage(sz.cx - p.x, sz.cy - p.y, b, RectC(isz.cx - p.x, isz.cy - p.y, p.x, p.y));
		w.DrawImage(p.x, 0, sz.cx - 2 * p.x, p.y, b, RectC(p.x, 0, isz.cx - 2 * p.x, p.y));
		w.DrawImage(p.x, sz.cy - p.y, sz.cx - 2 * p.x, p.y, b, RectC(p.x, isz.cy - p.y, isz.cx - 2 * p.x, p.y));
		w.DrawImage(0, p.y, p.x, sz.cy - 2 * p.y, b, RectC(0, p.y, p.y, isz.cy - 2 * p.y));
		w.DrawImage(sz.cx - p.x, p.y, p.x, sz.cy - 2 * p.y, b, RectC(isz.cx - p.x, p.y, p.y, isz.cy - 2 * p.y));
		w.DrawImage(p.x, p.y, sz.cx - 2 * p.x, sz.cy - 2 * p.y, b,
		            RectC(p.x, p.y, isz.cx - 2 * p.x, isz.cy - 2 * p.y));
		w.End();
	}
}

static void sChSync(const String& s)
{
	for(int i = 0; i < s_r().GetCount(); i++) {
		sChRoutine& r = s_r()[i];
		if(s == r.style)
			(*r.fn)();
	}
}

void ChSetStyle(const char *style)
{
	ChReset();
	Vector<String> s = Split(style, ';');
	ChPainter(StdChPainter);
	sChSync("!");
	sChSync("");
	for(int i = 0; i < s.GetCount(); i++)
		sChSync(s[i]);
	sChSync(".");
}

typedef void (*ChPainterFn)(Draw& w, const Rect& r, const Value& v);

Vector<ChPainterFn>& sChps()
{
	static Vector<ChPainterFn> x;
	return x;
}

void ChPainter(void (*fn)(Draw& w, const Rect& r, const Value& v))
{
	sChps().Add(fn);
}

void ChPaint(Draw& w, const Rect& r, const Value& element)
{
	for(int i = sChps().GetCount() - 1; i >= 0; i--)
		(*sChps()[i])(w, r, element);
}

void ChPaint(Draw& w, int x, int y, int cx, int cy, const Value& element)
{
	ChPaint(w, RectC(x, y, cx, cy), element);
}

Image AdjustColors(const Image& img) {
	const RGBA *s = ~img;
	const RGBA *e = s + img.GetLength();
	ImageBuffer w(img.GetSize());
	RGBA *t = w;
	Color black = SColorText();
	Color shadow = SColorShadow();
	Color face = SColorFace();
	Color light = SColorLight();
	Color highlight = SColorHighlight();
	Color hot = highlight;
	Color paper = SColorPaper();
	while(s < e) {
		*t = *s;
		if(s->r == s->g && s->g == s->b)
			if(s->r >= 0 && s->r < 128)
				*t = Blend(black, shadow, 2 * s->r);
			else
			if(s->r >= 128 && s->r < 192)
				*t = Blend(shadow, face, 4 * (s->r - 128));
			else
				*t = Blend(face, light, 4 * (s->r - 192));
		else
		if(s->r == 0 && s->g == 0)
			*t = Blend(black, highlight, 2 * (s->b - 128));
		else
		if(s->b == 0 && s->g == 0)
			*t = Blend(black, hot, 2 * (s->r - 128));
		else
		if(s->r == s->g && s->b == 0)
			*t = Blend(black, paper, s->r);
		t->a = s->a;
		t++;
		s++;
	}
	w.SetHotSpot(img.GetHotSpot());
	return w;
}

void  Override(Iml& target, const char *prefix, Iml& source, bool colored)
{
	Vector<String> p = Split(prefix, ';');
	for(int q = 0; q < p.GetCount(); q++)
		for(int i = 0; i < target.GetCount(); i++) {
			String n = p[q] + target.GetId(i);
			int q = source.Find(n);
			if(q >= 0) {
				DUMP(n);
				Image m = source.Get(q);
				if(colored)
					m = AdjustColors(m);
				target.Set(i, m);
			}
		}
}
