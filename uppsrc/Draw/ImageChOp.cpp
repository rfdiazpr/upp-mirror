#include "Draw.h"

NAMESPACE_UPP

int Diff(RGBA a, RGBA b)
{
	return max(abs(a.a - b.a), max(abs(a.r - b.r), max(abs(a.b - b.b), abs(a.g - b.g))));
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

void Interpolate(ImageBuffer& b, Vector< Vector<bool> >& map, const Rect& rc)
{
	Unmultiply(b);
	int todo;
	do {
		todo = 0;
		for(int x = rc.left; x < rc.right; x++)
			for(int y = rc.top; y < rc.bottom; y++) {
				RGBA& p = b[y][x];
				if(map[y][x]) {
					map[y][x] = 0;
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
	Premultiply(b);
}

struct ButtonDecomposer {
	Image src;
	Image dst;
	int   aa[8];
	int   maxdiff;
	RGBA  color;
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
		color = SColorText();
		maxdiff = gdiff = gcount = 0;
		Vector< Vector<bool> > map;
		for(int y = a; y < sz.cy - a; y++) {
			map.At(y).SetCount(sz.cx, false);
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
			map[y].Set(x, true, xx - x);
		}
		Interpolate(b, map, Rect(a, a, sz.cx - a, sz.cy - a));
		if(a < 2) a = 2;
		b.SetHotSpot(Point(a, a));
		dst = b;
	}
};

Image Unglyph(const Image& m, Color& c, double& gfactor)
{
	ButtonDecomposer b;
	b.src = Unmultiply(m);
	b.Do();
	c = b.color;
	gfactor = (double)b.gdiff / b.gcount;
	return Premultiply(b.dst);
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
		else
		if(y >= y0 && y1 > y0) {
			int alpha = 256 * (y - y0) / (y1 - y0);
			RGBA *t = b[y];
			const RGBA *s1 = img1[y];
			const RGBA *s2 = img2[y];
			const RGBA *e = s1 + sz.cx;
			while(s1 < e) {
				t->r = s1->r + ((alpha * (s2->r - s1->r)) >> 8);
				t->g = s1->g + ((alpha * (s2->g - s1->g)) >> 8);
				t->b = s1->b + ((alpha * (s2->b - s1->b)) >> 8);
				t->a = s1->a + ((alpha * (s2->a - s1->a)) >> 8);
				s1++;
				s2++;
				t++;
			}
		}
		else
			memcpy(b[y], img1[y], sz.cx * sizeof(RGBA));
	b.SetHotSpot(img1.GetHotSpot());
	b.Set2ndSpot(img1.Get2ndSpot());
	return b;
}

Image HorzBlend(Image img1, Image img2, int x0, int x1)
{
	Image m = RotateAntiClockwise(VertBlend(RotateClockwise(img1), RotateClockwise(img2), x0, x1));
	ImageBuffer b(m);
	b.SetHotSpot(img1.GetHotSpot());
	b.Set2ndSpot(img1.Get2ndSpot());
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
	b.SetHotSpot(src.GetHotSpot());
	b.Set2ndSpot(src.Get2ndSpot());
	return b;
}

bool EqLine(const Image& m, int l1, int l2, int x, int width)
{
	return !memcmp(m[l1] + x, m[l2] + x, width * sizeof(RGBA));
}

bool EqLine(const Image& m, int l1, int l2)
{
	return EqLine(m, l1, l2, 0, m.GetWidth());
}

bool EqColumn(const Image& m, int c1, int c2, int y, int height)
{
	int cx = m.GetWidth();
	const RGBA *a = m[y] + c1;
	const RGBA *b = m[y] + c2;
	for(int w = 0; w < height; w++) {
		if(*a != *b)
			return false;
		a += cx;
		b += cx;
	}
	return true;
}

bool EqColumn(const Image& m, int c1, int c2)
{
	return EqColumn(m, c1, c2, 0, m.GetHeight());
}

int ClassifyContent(const Image& m, const Rect& rect)
{
	if(IsNull(rect))
		return 0;
	bool vdup = true;
	for(int q = rect.top + 1; q < rect.bottom; q++)
		if(!EqLine(m, q, rect.top, rect.left, rect.GetWidth())) {
			vdup = false;
			break;
		}
	int cx = m.GetSize().cx;
	for(int q = rect.left + 1; q < rect.right; q++)
		if(!EqColumn(m, rect.left, q, rect.top, rect.GetHeight()))
			return vdup;
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
	Premultiply(r);
	return r;
}

int ImageMargin(const Image& _m, int p, int dist)
{
	Image m = Unmultiply(_m);
	Color c = m[p][p];
	int d;
	Size sz = m.GetSize();
	for(d = p; d >= 0; d--)
		if(Diff(m[d][d], c) > dist || Diff(m[sz.cx - d - 1][sz.cy - d - 1], c) > dist)
			break;
	return d + 1;
}

ChPartMaker::ChPartMaker(const Image& m)
{
	image = m;
	border = SColorShadow();
	bg = Null;
	ResetShape();
}

void ChPartMaker::ResetShape()
{
	t = b = l = r = true;
	tl = tr = bl = br = 0;
}

Image ChPartMaker::Make() const
{
	Size sz = image.GetSize();
	ASSERT(sz.cx >= 6 && sz.cy >= 6);
	Image h = image;
	ImageBuffer ib(h);
	for(int x = 0; x < sz.cx; x++) {
		if(t)
			ib[0][x] = x >= tl && x < sz.cx - tr ? border : bg;
		if(b)
			ib[sz.cy - 1][x] = x >= bl && x < sz.cx - br ? border : bg;
	}
	for(int y = 0; y < sz.cy; y++) {
		if(l)
			ib[y][0] = y >= tl && y < sz.cy - bl ? border : bg;
		if(r)
			ib[y][sz.cx - 1] = y >= tr && y < sz.cy - br ? border : bg;
	}
	if(tl == 2)
		ib[1][1] = border;
	if(tr == 2)
		ib[1][sz.cx - 2] = border;
	if(bl == 2)
		ib[sz.cy - 2][1] = border;
	if(br == 2)
		ib[sz.cy - 2][sz.cx - 2] = border;
	int q = max(max(tl, tr), max(br, bl));
	ib.SetHotSpot(Point(q, q));
	ib.Set2ndSpot(Point(sz.cx - q - 1, sz.cy - q - 1));
	return ib;
}

END_UPP_NAMESPACE
