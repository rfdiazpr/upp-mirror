#include "Draw.h"

NAMESPACE_UPP

int Diff(RGBA a, RGBA b)
{
	return max(abs(a.r - b.r), max(abs(a.b - b.b), abs(a.g - b.g)));
}

struct InterPoint {
	ImageBuffer &b;
	Rect         rc;
	int          sr, sg, sb;
	int          sn, in;

	InterPoint(ImageBuffer& b) : b(b) {
		sr = sg = sb = sn = in = 0;
	}
	void Add(int x, int y);
};

void InterPoint::Add(int y, int x) {
	if(rc.Contains(x, y)) {
		RGBA ba = b[y][x];
		if(ba.a == 0)
			return;
		in++;
		sn++;
		sr += ba.r;
		sg += ba.g;
		sb += ba.b;
	}
	else
		in++;
}

void Interpolate(ImageBuffer& b, const Rect& rc)
{
	int todo;
	do {
		todo = 0;
		for(int x = rc.left; x < rc.right; x++)
			for(int y = rc.top; y < rc.bottom; y++) {
				RGBA& p = b[y][x];
				if(p.a == 0) {
					InterPoint ip(b);
					ip.rc = rc;
					ip.Add(y - 1, x - 1);
					ip.Add(y - 1, x);
					ip.Add(y - 1, x + 1);
					ip.Add(y, x - 1);
					ip.Add(y, x + 1);
					ip.Add(y + 1, x - 1);
					ip.Add(y + 1, x);
					ip.Add(y + 1, x + 1);
					if(ip.in >= 2) {
						if(ip.sn) {
							p.r = ip.sr / ip.sn;
							p.g = ip.sg / ip.sn;
							p.b = ip.sb / ip.sn;
						}
						else
							p = SColorFace();
						p.a = 255;
					}
					else
						todo++;
				}
			}
	}
	while(todo);
}

struct ButtonDecomposer {
	Image src;
	Image dst;
	int   aa[8];
	int   maxdiff;
	Color color;
	int   gdiff;
	int   gcount;

	RGBA GetC(int p) {
		Size sz = src.GetSize();
		int xx[3];
		int yy[3];
		yy[0] = xx[0] = aa[p];
		xx[1] = sz.cx - aa[p] - 1;
		xx[2] = sz.cx / 2;
		yy[1] = sz.cy - aa[p] - 1;
		yy[2] = sz.cy / 2;
		return src[xx[p / 3]][xx[p % 3]];
	}

	void Do() {
		Size sz = src.GetSize();
		int qa = min(4, min(sz.cy / 4, sz.cx / 4));
		if(qa == 0) {
			dst = src;
			return;
		}
		int a = 0;
		for(int p = 0; p < 8; p++) {
			aa[p] = qa;
			Color c = GetC(p);
			while(aa[p] > 0) {
				Color c1 = GetC(p);
				if(Diff(c, c1) > 30)
					break;
				c = c1;
				aa[p]--;
			}
			if(aa[p] > a)
				a = aa[p];
		}
		if(a < min(sz.cx / 3, sz.cy / 3))
			a++;
		dst = src;
		ImageBuffer b(dst);
		color = SColorText;
		maxdiff = gdiff = gcount = 0;
		for(int y = a; y < sz.cy - a; y++) {
			RGBA *p = b[y];
			int x = a;
			Color c = p[x];
			while(x < sz.cx - a) {
				if(Diff(p[x], c) > 30)
					break;
				c = p[x++];
			}
			int xx = sz.cx - a;
			while(xx > x) {
				if(Diff(p[xx - 1], c) > 30)
					break;
				c = p[--xx];
			}
			for(int q = x; q < xx; q++) {
				int d = Diff(p[q], c);
				gcount++;
				gdiff += d;
				if(d >= maxdiff) {
					maxdiff = d;
					color = p[q];
				}
			}
			Fill(p + x, RGBAZero(), xx - x);
		}
		Interpolate(b, Rect(a, a, sz.cx - a, sz.cy - a));
		b.SetHotSpot(Point(a, a));
		dst = b;
	}
};

Image Unglyph(const Image& m, Color& c, double& gfactor)
{
	ButtonDecomposer b;
	b.src = m;
	b.Do();
	c = b.color;
	gfactor = (double)b.gdiff / b.gcount;
	return b.dst;
}

Image Unglyph(const Image& m, Color& c)
{
	double dummy;
	return Unglyph(m, c, dummy);
}

Image Unglyph(const Image& m)
{
	Color dummy;
	return Unglyph(m, dummy);
}

Image VertBlend(Image img1, Image img2, int y0, int y1)
{
	Size sz = img1.GetSize();
	Size sz2 = img2.GetSize();
	sz.cx = min(sz.cx, sz2.cx);
	sz.cy = min(sz.cy, sz2.cy);
	ImageBuffer b(sz);
	for(int y = 0; y < sz.cy; y++)
		if(y >= y1)
			memcpy(b[y], img2[y], sz.cx * sizeof(RGBA));
		else {
			memcpy(b[y], img1[y], sz.cx * sizeof(RGBA));
			if(y >= y0 && y1 > y0)
				AlphaBlendOpaque(b[y], img2[y], sz.cx, 255 * (y - y0) / (y1 - y0));
		}
	return b;
}

Image HorzSymm(Image src) {
	ImageBuffer b(src);
	Size sz = b.GetSize();
	for(int y = 0; y < sz.cy; y++) {
		RGBA *l = b[y];
		for(int x = 0; x < sz.cx / 2; x++)
			l[sz.cx - x - 1] = l[x];
	}
	return b;
}

int ClassifyContent(const Image& m, const Rect& rect)
{
	if(IsNull(rect))
		return 0;
	bool vdup = true;
	for(int q = rect.top + 1; q < rect.bottom; q++)
		if(memcmp(m[q] + rect.left, m[rect.top] + rect.left, rect.GetWidth() * sizeof(RGBA))) {
			vdup = false;
			break;
		}
	bool hdup = true;
	int cx = m.GetSize().cx;
	for(int q = rect.left + 1; q < rect.right; q++) {
		const RGBA *a = m[rect.top] + rect.left;
		const RGBA *b = m[rect.top] + q;
		for(int w = 1; w < rect.GetHeight(); w++) {
			if(*a != *b)
				return vdup;
			a += cx;
			b += cx;
		}
	}
	return 2 + vdup;
}

Image RecreateAlpha(const Image& overwhite, const Image& overblack)
{
	Size sz = overwhite.GetSize();
	ASSERT(overblack.GetSize() == sz);
	ImageBuffer r(sz);
	const RGBA *ws = overwhite;
	const RGBA *bs = overblack;
	RGBA *t = r;
	RGBA *e = t + r.GetLength();
	while(t < e) {
		t->a = bs->r - ws->r + 255;
		if(t->a) {
			t->r = bs->r * 255 / t->a;
			t->g = bs->g * 255 / t->a;
			t->b = bs->b * 255 / t->a;
		}
		else
			t->r = t->g = t->b = 0;
		t++;
		bs++;
		ws++;
	}
	return r;
}

int ImageMargin(const Image& m, int p, int dist)
{
	Color c = m[4][4];
	int d;
	for(d = 4; d > 0; d--)
		if(Diff(m[d][d], c) > 10)
			break;
	return d + 1;
}

END_UPP_NAMESPACE
