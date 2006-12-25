#include "IconDes.h"

NAMESPACE_UPP

struct sFloodFill {
	Rect         rc;
	Size         sz;
	Buffer<byte> flag;
	ImageBuffer& ib;
	RGBA         scolor;
	RGBA         color;
	bool         done;

	RGBA& At(int x, int y)         { return ib[y + rc.top][x + rc.left]; }
	bool  Eq(int x, int y, RGBA c) { RGBA& q = At(x, y); return (q.a | c.a) == 0 || c == q; }
	byte& Flag(int x, int y) { return flag[y * sz.cx + x]; }
	void  Fill(RGBA color, Point pt, const Rect& rc);
	void  Try(int x, int y);

	sFloodFill(ImageBuffer& ib) : ib(ib) {}
};

void sFloodFill::Try(int x, int y)
{
	if(x >= 0 && x < sz.cx && y >= 0 && y < sz.cy && Flag(x, y) == 0 && Eq(x, y, scolor)) {
		Flag(x, y) = 1;
		At(x, y) = color;
		done = false;
	}
}

void sFloodFill::Fill(RGBA _color, Point pt, const Rect& _rc)
{
	rc = _rc & ib.GetSize();
	if(!rc.Contains(pt))
		return;
	scolor = ib[pt.y][pt.x];
	color = _color;
	sz = rc.GetSize();
	flag.Alloc(sz.cx * sz.cy, 0);
	pt -= rc.TopLeft();
	Flag(pt.x, pt.y) = 1;
	At(pt.x, pt.y) = color;
	do {
		done = true;
		for(int y = 0; y < sz.cy; y++)
			for(int x = 0; x < sz.cx; x++) {
				if(Flag(x, y) == 1) {
					Flag(x, y) = 2;
					Try(x + 1, y);
					Try(x - 1, y);
					Try(x, y + 1);
					Try(x, y - 1);
				}
			}
	}
	while(!done);
}

void FloodFill(ImageBuffer& img, RGBA color, Point pt, const Rect& rc)
{
	sFloodFill(img).Fill(color, pt, rc);
}

struct InterpolateFilter : ImageFilter9 {
	int todo;

	virtual RGBA operator()(const RGBA **mx);
};

RGBA InterpolateFilter::operator()(const RGBA **mx)
{
	RGBA t = mx[1][1];
	int na = mx[0][1].a, wa = mx[1][0].a, ea = mx[1][2].a, sa = mx[2][1].a;
	int suma = na + wa + ea + sa, half = suma >> 1;
	if(suma > 0) {
		t.r = (mx[0][1].r * na + mx[1][0].r * wa + mx[1][2].r * ea + mx[2][1].r * sa + half) / suma;
		t.g = (mx[0][1].g * na + mx[1][0].g * wa + mx[1][2].g * ea + mx[2][1].g * sa + half) / suma;
		t.b = (mx[0][1].b * na + mx[1][0].b * wa + mx[1][2].b * ea + mx[2][1].b * sa + half) / suma;
		t.a = max(max(na, sa), max(wa, ea));
		if(t != mx[1][1])
			todo++;
	}
	return t;
}

void InterpolateImage(Image& img, const Rect& _rc)
{
	Rect rc = _rc & img.GetSize();
	Image m = Crop(img, rc);
	TimeStop tm;
	Image imp = CreateImage(rc.GetSize(), Null);
	Over(imp, Point(0, 0), m, m.GetSize());
	Progress pi("Interpolating...");
	int n = rc.GetWidth() * rc.GetHeight();
	for(int qq = 0; qq < 2000; qq++) {
		InterpolateFilter f;
		f.todo = 0;
		imp = Filter(imp, f);
		Over(imp, Point(0, 0), m, m.GetSize());
		if(f.todo == 0)
			break;
		if(pi.SetCanceled(qq, 2000))
			break;
	}
	Copy(img, rc.TopLeft(), imp, imp.GetSize());
}

void MirrorHorz(Image& img, const Rect& rect)
{
	ImageBuffer ib(img);
	Rect rc = rect & ib.GetSize();
	for(int y = rc.top; y < rc.bottom; y++) {
		RGBA *b = ib[y] + rc.left;
		RGBA *e = ib[y] + rc.right - 1;
		while(b < e) {
			Swap(*b, *e);
			b++;
			e--;
		}
	}
	img = ib;
}

void MirrorVert(Image& img, const Rect& rect)
{
	ImageBuffer ib(img);
	Rect rc = rect & ib.GetSize();
	int b = rc.top;
	int e = rc.bottom - 1;
	int n = rc.right - rc.left;
	if(n > 0) {
		Buffer<RGBA> h(n);
		n *= sizeof(RGBA);
		while(b < e) {
			memcpy(h, ib[b] + rc.left, n);
			memcpy(ib[b] + rc.left, ib[e] + rc.left, n);
			memcpy(ib[e] + rc.left, h, n);
			b++;
			e--;
		}
	}
	img = ib;
}

Image Magnify(const Image& img, int nx, int ny)
{
	if(nx == 1 && ny == 1)
		return img;
	if(nx == 0 || ny == 0)
		return Image();
	Size sz = img.GetSize();
	bool xdown = nx < 0;
	nx = abs(nx);
	int ncx = xdown ? sz.cx / nx : sz.cx * nx;
	ImageBuffer b(ncx, sz.cy * ny);
	const RGBA *s = ~img;
	const RGBA *e = s + img.GetLength();
	RGBA *t = ~b;
	while(s < e) {
		RGBA *q = t;
		const RGBA *le = s + sz.cx;
		while(s < le) {
			Fill(q, *s, nx);
			q += nx;
			s++;
		}
		for(int n = ny - 1; n--;) {
			memcpy(q, t, ncx * sizeof(RGBA));
			q += ncx;
		}
		t = q;
	}
	return b;
}

String PackImlData(const Vector<Image>& image)
{
	StringBuffer block;
	int bl = 0;
	for(int i = 0; i < image.GetCount(); i++) {
		Image img = image[i];
		StringStream ss;
		ss.Put(0);
		Size sz = img.GetSize();
		ss.Put16le(sz.cx);
		ss.Put16le(sz.cy);
		Point p = img.GetHotSpot();
		ss.Put16le(p.x);
		ss.Put16le(p.y);
		p = img.Get2ndSpot();
		ss.Put16le(p.x);
		ss.Put16le(p.y);
		block.Cat(ss.GetResult());
		const RGBA *s = img;
		const RGBA *e = s + img.GetLength();
		while(s < e) {
			block.Cat(s->r);
			block.Cat(s->g);
			block.Cat(s->b);
			block.Cat(s->a);
			s++;
		}
		bl += img.GetLength();
	}
	return ZCompress(block);
}

END_UPP_NAMESPACE
