#include "Draw.h"

struct ImageOp : Moveable<ImageOp> {
	void  (*op)(Vector<Image>& m, CParser& p, void *fn);
	void  *fn;
};

static StaticCriticalSection sMakeImageOp;

VectorMap<String, ImageOp>& ImageOpMap()
{
	static VectorMap<String, ImageOp> x;
	return x;
}

void RegisterImageOp(const char *id, void  (*op)(Vector<Image>& m, CParser& p, void *fn), void *fn)
{
	INTERLOCKED_(sMakeImageOp) {
		ImageOp& p = ImageOpMap().Add(id);
		p.op = op;
		p.fn = fn;
	}
}

template <class T>
void AutoCast(T& x, void *ptr) { x = (T)(ptr); }

static void sBaseOp(Vector<Image>& m, CParser& p, void *fn)
{
	void  (*op)(Vector<Image>& m, CParser& p);
	AutoCast(op, fn);
	(*op)(m, p);
}

void RegisterImageOp(const char *id, void  (*op)(Vector<Image>& m, CParser& p))
{
	RegisterImageOp(id, sBaseOp, (void *)op);
}

static void sOp0(Vector<Image>& m, CParser& p, void *fn)
{
	Image (*op0)(CParser& p);
	AutoCast(op0, fn);
	if(m.GetCount() == 0)
		m.Add();
	m.Top() = (*op0)(p);
}

void RegisterImageOp(const char *id, Image (*op0)(CParser& p))
{
	RegisterImageOp(id, sOp0, (void *)op0);
}

static void sOpn(Vector<Image>& m, CParser& p, void *fn)
{
	Image (*op1)(const Image& img);
	AutoCast(op1, fn);
	if(m.GetCount() == 0)
		m.Add();
	m.Top() = (*op1)(m.Top());
}

void RegisterImageOp(const char *id, Image (*opn)(const Image& img))
{
	RegisterImageOp(id, sOpn, (void *)opn);
}

static void sOp1(Vector<Image>& m, CParser& p, void *fn)
{
	Image (*op1)(const Image& img, CParser& p);
	AutoCast(op1, fn);
	if(m.GetCount() == 0)
		m.Add();
	m.Top() = (*op1)(m.Top(), p);
}

void RegisterImageOp(const char *id, Image (*op1)(const Image& img, CParser& p))
{
	RegisterImageOp(id, sOp1, (void *)op1);
}

static void sOp2(Vector<Image>& m, CParser& p, void *fn)
{
	Image (*op2)(const Image& img1, const Image& img2, CParser& p);
	AutoCast(op2, fn);
	while(m.GetCount() < 2)
		m.Add();
	m.Top() = (*op2)(m[m.GetCount() - 2], m[m.GetCount() - 1], p);
}

void RegisterImageOp(const char *id, Image (*op2)(const Image& img1, const Image& img2, CParser& p))
{
	RegisterImageOp(id, sOp2, (void *)op2);
}

struct sImageMaker : ImageMaker {
	const Vector<Image>& in;
	const char          *code;

	virtual String Key() const;
	virtual Image Make() const;

	sImageMaker(const Vector<Image>& in, const char *code) : in(in), code(code) {}
};

String sImageMaker::Key() const
{
	StringBuffer kb;
	int l = strlen(code);
	kb.SetLength(in.GetCount() * sizeof(int64) + l + 1);
	char *p = kb;
	for(int i = 0; i < in.GetCount(); i++) {
		int64 x = in[i].GetSerialId();
		memcpy(p, &x, sizeof(int64));
		p += sizeof(int64);
	}
	*p++ = in.GetCount();
	memcpy(p, code, l);
	return kb;
}

Image sImageMaker::Make() const
{
	Image img;
	try {
		Vector<Image> stack(in, 1);
		CParser p(code);
		while(!p.IsEof()) {
			String fn = p.ReadId();
			ImageOp op;
			op.fn = NULL;
			INTERLOCKED_(sMakeImageOp) {
				op = ImageOpMap().Get(fn, op);
			}
			if(op.fn)
				(*op.op)(stack, p, op.fn);
			p.Char(';');
		}
		if(stack.GetCount())
			img = stack.Top();
	}
	catch(CParser::Error e) {}
	return img;
}

Image MakeImage(const Vector<Image>& in, const char *code)
{
	sImageMaker m(in, code);
	return MakeImage(m);
}

Image MakeImage(const Image& in, const char *code)
{
	return MakeImage(Vector<Image>() << in, code);
}

Image MakeImage(const char *code)
{
	return MakeImage(Vector<Image>(), code);
}

static Image sRescale(const Image& img, CParser& p)
{
	int cx = p.ReadInt();
	int cy = p.ReadInt();
	return Rescale(img, cx, cy);
}

static Image sSharpen(const Image& img, CParser& p)
{
	return Sharpen(img, p.IsInt() ? p.ReadInt() : 300);
}

static Image sGrayscale(const Image& img, CParser& p)
{
	if(p.IsInt())
		return Grayscale(img, p.ReadInt());
	else
		return Grayscale(img);
}

static Image sContrast(const Image& img, CParser& p)
{
	return Contrast(img, p.IsInt() ? p.ReadInt() : 256);
}

static Image sCrop(const Image& img, CParser& p)
{
	Rect r;
	r.left = p.ReadInt();
	r.top = p.ReadInt();
	r.right = r.left + p.ReadInt();
	r.bottom = r.top + p.ReadInt();
	return Crop(img, r);
}

INITBLOCK {
	RegisterImageOp("rescale", sRescale);
	RegisterImageOp("sharpen", sSharpen);
	RegisterImageOp("grayscale", sGrayscale);
	RegisterImageOp("etched", Etched);
	RegisterImageOp("crop", sCrop);
	RegisterImageOp("contrast", sContrast);
}
