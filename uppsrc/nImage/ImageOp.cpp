#include "Image.h"

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

Image Adjust(const Image& img,
             int mulr, int addr, int mulg, int addg, int mulb, int addb, int mula, int adda)
{
	ImageBuffer b(img.GetSize());
	const RGBA *s = ~img;
	const RGBA *e = s + img.GetLength();
	RGBA *t = b;
	while(s < e) {
		t->r = Saturate255(((s->r * mulr) >> 8) + addr);
		t->g = Saturate255(((s->g * mulg) >> 8) + addg);
		t->b = Saturate255(((s->b * mulb) >> 8) + addb);
		t->a = Saturate255(((s->a * mula) >> 8) + adda);
		t++;
		s++;
	}
	return b;
}

Image Adjust(const Image& img, int mul, int add)
{
	return Adjust(img, mul, add, mul, add, mul, add, 256, 0);
}

struct RGBAI {
	int r, g, b, a;

	RGBAI() {}
	RGBAI(int c) { r = g = b = a =c; }
};

void GetPx(const Image& img, int x, int y, RGBAI& p, int mul)
{
	Size sz = img.GetSize();
	RGBA q = img[minmax(y, 0, sz.cy - 1)][minmax(x, 0, sz.cx - 1)];
	p.r += mul * q.r;
	p.g += mul * q.g;
	p.b += mul * q.b;
	p.a += mul * q.a;
}

Image Sharpen(const Image& img, int amount)
{
	Size sz = img.GetSize();
	ImageBuffer b(sz);
	int na = 256 + amount;
	for(int y = 0; y < sz.cy; y++)
		for(int x = 0; x < sz.cx; x++) {
			RGBAI q(0);
			GetPx(img, x - 1, y - 1, q, 7);
			GetPx(img, x - 1, y + 1, q, 7);
			GetPx(img, x + 1, y - 1, q, 7);
			GetPx(img, x + 1, y + 1, q, 7);
			GetPx(img, x - 1, y, q, 9);
			GetPx(img, x + 1, y, q, 9);
			GetPx(img, x, y - 1, q, 9);
			GetPx(img, x, y + 1, q, 9);
			RGBA s = img[y][x];
			RGBA& t = b[y][x];
			t.b = Saturate255((na * (s.b << 6) - amount * q.b) >> 14);
			t.g = Saturate255((na * (s.g << 6) - amount * q.g) >> 14);
			t.r = Saturate255((na * (s.r << 6) - amount * q.r) >> 14);
			t.a = Saturate255((na * (s.a << 6) - amount * q.a) >> 14);
		}
	return b;
}

Image SetColor(const Image& img, Color c)
{
	byte r = c.GetR();
	byte b = c.GetB();
	byte g = c.GetG();
	const RGBA *s = ~img;
	const RGBA *e = s + img.GetLength();
	ImageBuffer w(img.GetSize());
	RGBA *t = w;
	while(s < e) {
		t->r = r;
		t->g = g;
		t->b = b;
		t->a = s->a;
		t++;
		s++;
	}
	return w;
}

Image Over(const Image& img, const Image& over, byte alpha)
{//Improve!
	Size sz = img.GetSize();
	if(sz != over.GetSize())
		return Image();
	int n = img.GetLength();
	ImageBuffer b(img.GetSize());
	memcpy(~b, img, sizeof(RGBA) * n);
	AlphaBlend(b, ~over, n, alpha);
	return b;
}

Image Grayscale(const Image& img)
{
	const RGBA *s = ~img;
	const RGBA *e = s + img.GetLength();
	ImageBuffer w(img.GetSize());
	RGBA *t = w;
	while(s < e) {
		int q = GrayScale(*s);
		t->r = q;
		t->g = q;
		t->b = q;
		t->a = s->a;
		t++;
		s++;
	}
	return w;

}

Image Emboss(const Image& img, Color color, int amount)
{
	Size sz = img.GetSize();
	ImageBuffer b(sz);
	RGBA c = RGBAColor(color);
	Fill(~b, RGBAZero(), sz.cx);
	int na = 256 - amount;
	for(int y = 1; y < sz.cy; y++) {
		RGBA *t = b[y];
		const RGBA *s = img[y];
		const RGBA *e = s + sz.cx;
		*t = c;
		t->a = s->a;
		t++;
		s++;
		while(s < e) {
			int q = amount * (GrayScale(*s) - GrayScale(*(s - sz.cx - 1)));
			t->a = s->a;
			t->r = Saturate255(c.r + ((q * c.r) >> 16));
			t->g = Saturate255(c.g + ((q * c.g) >> 16));
			t->b = Saturate255(c.b + ((q * c.b) >> 16));
			t++;
			s++;
		}
	}
	return b;
}

Image EmbossTh(const Image& img, Color color, Color colorl, Color colord, int thold)
{
	Size sz = img.GetSize();
	ImageBuffer b(sz);
	RGBA c = RGBAColor(color);
	RGBA cl = RGBAColor(colorl);
	RGBA cd = RGBAColor(colord);
	FillColor(~b, c, sz.cx);
	for(int y = 1; y < sz.cy; y++) {
		RGBA *t = b[y];
		const RGBA *s = img[y];
		const RGBA *e = s + sz.cx;
		*t = c;
		t->a = s->a;
		t++;
		s++;
		while(s < e) {
			int q = GrayScale(*s) - GrayScale(*(s - sz.cx - 1));
			byte a = s->a;
			*t = q > thold ? cl : q < -thold ? cd : c;
			t->a = a;
			t++;
			s++;
		}
	}
	return b;
}
