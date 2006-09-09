#include "Draw.h"

#define LTIMING(x) // RTIMING(x)

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

void ChSet(const char *name, int i, const Value& val)
{
	VectorMap<String, ChVar>& var = chVar();
	int q = var.Find(name);
	if(q < 0)
		return;
	ChVar& v = var[q];
	*v.state = 2;
	(*v.set)(i, val);
	for(int i = 0; i < var.GetCount(); i++)
		if(*var[i].state == 1)
			*var[i].state = 0;
}

void ChSet(const char *name, const Value& val)
{
	VectorMap<String, ChVar>& var = chVar();
	int q = var.Find(name);
	if(q < 0)
		return;
	ChVar& v = var[q];
	for(int i = 0; i < v.count; i++)
		ChSet(name, i, val);
}

void ChSetf(const char *name, Value (*fn)(int))
{
	VectorMap<String, ChVar>& var = chVar();
	int q = var.Find(name);
	if(q < 0)
		return;
	ChVar& v = var[q];
	for(int i = 0; i < v.count; i++)
		ChSet(name, i, (*fn)(i));
}

Value ChGet(const char *name, int i)
{
	VectorMap<String, ChVar>& var = chVar();
	int q = var.Find(name);
	return q < 0 ? Null : (*var[q].get)(i);
}

Value ChGet(const char *name)
{
	return ChGet(name, 0);
}

struct ChImageMaker : ImageMaker {
	Size       sz;
	Rect       sr;
	Image      img;

	virtual String Key() const;
	virtual Image  Make() const;
};

String ChImageMaker::Key() const
{
	LTIMING("ChImageMaker::Key");
	StringBuffer b;
	int64 id = img.GetSerialId();
	b.Cat((const char *)&id, sizeof(id));
	b.Cat((const char *)&sz.cx, sizeof(sz.cx));
	b.Cat((const char *)&sz.cy, sizeof(sz.cy));
	b.Cat((const char *)&sr.left, sizeof(sr.left));
	b.Cat((const char *)&sr.right, sizeof(sr.right));
	b.Cat((const char *)&sr.top, sizeof(sr.top));
	b.Cat((const char *)&sr.bottom, sizeof(sr.bottom));
	return b;
}

Image ChImageMaker::Make() const
{
	LTIMING("ChImageMaker::Make");
	return Rescale(img, sz, sr);
}

void  ChDraw(Draw& w, int x, int y, int cx, int cy, const Image& img, const Rect& src)
{
	if(cx > 0 && cy > 0) {
		ChImageMaker m;
		m.sz = Size(cx, cy);
		m.sr = src;
		m.img = img;
		w.DrawImage(x, y, MakeImage(m));
	}
}

void  ChDraw(Draw& w, const Rect& r, const Image& img, const Rect& src)
{
	ChDraw(w, r.left, r.top, r.GetWidth(), r.GetHeight(), img, src);
}

struct sChLookWith {
	Value look;
	Image img;
	Color (*colorfn)(int i);
	int   ii;
	Color color;
};

Value ChLookWith(const Value& look, const Image& img)
{
	sChLookWith x;
	x.look = look;
	x.img = img;
	x.colorfn = NULL;
	x.color = Null;
	return RawToValue(x);
}

Value ChLookWith(const Value& look, const Image& img, Color color)
{
	sChLookWith x;
	x.look = look;
	x.img = img;
	x.colorfn = NULL;
	x.color = color;
	return RawToValue(x);
}

Value ChLookWith(const Value& look, const Image& img, Color (*color)(int i), int i)
{
	sChLookWith x;
	x.look = look;
	x.img = img;
	x.colorfn = color;
	x.ii = i;
	return RawToValue(x);
}

Value sChOp(Draw& w, const Rect& r, const Value& v, int op);

Value StdChLookFn(Draw& w, const Rect& r, const Value& v, int op)
{
	if(IsType<sChLookWith>(v)) {
		const sChLookWith& x = ValueTo<sChLookWith>(v);
		if(op == LOOK_PAINT) {
			ChPaint(w, r, x.look);
			Point p = r.CenterPos(x.img.GetSize());
			if(x.colorfn)
				w.DrawImage(p.x, p.y, x.img, (*x.colorfn)(x.ii));
			else
			if(!IsNull(x.color))
				w.DrawImage(p.x, p.y, x.img, x.color);
			else
				w.DrawImage(p.x, p.y, x.img);
			return 1;
		}
		return sChOp(w, r, x.look, op);
	}
	if(IsType<Color>(v)) {
		Color c = v;
		switch(op) {
		case LOOK_PAINT:
			w.DrawRect(r, c);
			return 0;
		case LOOK_PAINTEDGE:
			DrawFrame(w, r, c);
			return 0;
		case LOOK_MARGINS:
			return Rect(1, 1, 1, 1);
		case LOOK_ISOPAQUE:
			return 1;
		}
	}
	if(IsType<Image>(v)) {
		Image b = v;
		Size isz = b.GetSize();
		Size sz = r.GetSize();
		Point p = b.GetHotSpot();
		int tile = 0;
		if(p.x > isz.cx / 2) {
			tile = 1;
			p.x = isz.cx - p.x - 1;
		}
		if(p.y > isz.cy / 2) {
			tile += 2;
			p.y = isz.cy - p.y - 1;
		}
		if(op == LOOK_ISOPAQUE)
			return b.GetKind() == IMAGE_OPAQUE;
		if(op == LOOK_MARGINS)
			return Rect(p.x, p.y, p.x, p.y);
		if(op == LOOK_PAINT || op == LOOK_PAINTEDGE) {
			LTIMING("ChPaint Image");
			w.Clipoff(r);
			if(p.x >= 0 && 2 * p.x < min(isz.cx, sz.cx) &&
			   p.y >= 0 && 2 * p.y < min(isz.cy, sz.cy)) {
				w.DrawImage(0, 0, b, RectC(0, 0, p.x, p.y));
				w.DrawImage(0, sz.cy - p.y, b, RectC(0, isz.cy - p.y, p.x, p.y));
				w.DrawImage(sz.cx - p.x, 0, b, RectC(isz.cx - p.x, 0, p.x, p.y));
				w.DrawImage(sz.cx - p.x, sz.cy - p.y, b, RectC(isz.cx - p.x, isz.cy - p.y, p.x, p.y));
				ChDraw(w, p.x, 0, sz.cx - 2 * p.x, p.y, b, RectC(p.x, 0, isz.cx - 2 * p.x, p.y));
				ChDraw(w, p.x, sz.cy - p.y, sz.cx - 2 * p.x, p.y, b, RectC(p.x, isz.cy - p.y, isz.cx - 2 * p.x, p.y));
				ChDraw(w, 0, p.y, p.x, sz.cy - 2 * p.y, b, RectC(0, p.y, p.x, isz.cy - 2 * p.y));
				ChDraw(w, sz.cx - p.x, p.y, p.x, sz.cy - 2 * p.y, b, RectC(isz.cx - p.x, p.y, p.x, isz.cy - 2 * p.y));
				if(op == LOOK_PAINT) {
					Rect r = RectC(p.x, p.y, sz.cx - 2 * p.x, sz.cy - 2 * p.y);
					Rect sr = RectC(p.x, p.y, isz.cx - 2 * p.x, isz.cy - 2 * p.y);
					if(IsNull(r) || IsNull(sr))
						return 1;
					if(tile) {
						LTIMING("Ch-Tiles");
						b = Rescale(b, Size((tile & 1 ? sr : r).GetWidth(),
						                    (tile & 2 ? sr : r).GetHeight()), sr);
						DrawTiles(w, r, b);
					}
					else {
						static VectorMap<int64, int> btc;
						int64 key = b.GetSerialId();
						int q;
						{
							LTIMING("Find");
							q = btc.Find(key);
						}
						if(q < 0) {
							LTIMING("ClassifyContent");
							q = ClassifyContent(b, sr);
							if(btc.GetCount() > 100)
								btc.Clear();
							btc.Add(key, q);
						}
						else
							q = btc[q];
						switch(q) {
						case IMAGECONTENT_VERTDUP|IMAGECONTENT_HORZDUP:
							{
								LTIMING("Ch-singlecolor");
								RGBA c = b[sr.top][sr.left];
								if(c.a == 255) {
									w.DrawRect(r, c);
									break;
								}
							}
						case 0:
							ChDraw(w, r, b, sr);
							break;
						default:
							LTIMING("Ch-linedup");
							b = Rescale(b, Size(((q & IMAGECONTENT_HORZDUP) ? sr : r).GetWidth(),
							                    ((q & IMAGECONTENT_VERTDUP) ? sr : r).GetHeight()), sr);
							LTIMING("Ch-linedup-drawtiles");
							DrawTiles(w, r, b);
							break;
						}
					}
				}
			}
			else
				ChDraw(w, r, b, b.GetSize());
			w.End();
			return 1;
		}
	}
	return Null;
}

typedef Value (*ChPainterFn)(Draw& w, const Rect& r, const Value& v, int op);

Vector<ChPainterFn>& sChps()
{
	static Vector<ChPainterFn> x;
	return x;
}

void ChLookFn(Value (*fn)(Draw& w, const Rect& r, const Value& v, int op))
{
	if(FindIndex(sChps(), fn) < 0)
		sChps().Add(fn);
}

void ChReset()
{
	VectorMap<String, ChVar>& var = chVar();
	for(int i = 0; i < var.GetCount(); i++)
		*var[i].state = 0;
	ChLookFn(StdChLookFn);
}

Value sChOp(Draw& w, const Rect& r, const Value& v, int op)
{
	Value q;
	for(int i = sChps().GetCount() - 1; i >= 0; i--) {
		q = (*sChps()[i])(w, r, v, op);
		if(!IsNull(q))
			break;
	}
	return q;
}

void ChPaint(Draw& w, const Rect& r, const Value& look)
{
	sChOp(w, r, look, LOOK_PAINT);
}

void ChPaint(Draw& w, int x, int y, int cx, int cy, const Value& look)
{
	sChOp(w, RectC(x, y, cx, cy), look, LOOK_PAINT);
}

void ChPaintEdge(Draw& w, const Rect& r, const Value& look)
{
	sChOp(w, r, look, LOOK_PAINTEDGE);
}

void ChPaintEdge(Draw& w, int x, int y, int cx, int cy, const Value& look)
{
	sChOp(w, RectC(x, y, cx, cy), look, LOOK_PAINTEDGE);
}

Rect ChMargins(const Value& look)
{
	NilDraw w;
	return sChOp(w, Null, look, LOOK_MARGINS);
}

bool ChIsOpaque(const Value& look)
{
	NilDraw w;
	return sChOp(w, Null, look, LOOK_ISOPAQUE);
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
			if(s->r < 128)
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

void  Override(Iml& target, Iml& source, bool colored)
{
	for(int i = 0; i < target.GetCount(); i++) {
		int q = source.Find(target.GetId(i));
		if(q >= 0) {
			Image m = source.Get(q);
			if(colored)
				m = AdjustColors(m);
			target.Set(i, m);
		}
	}
}

void ColoredOverride(Iml& target, Iml& source)
{
	Override(target, source, true);
}
