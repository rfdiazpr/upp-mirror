#include "Image.h"

void Fill(RGBA *t, const RGBA& src, int n)
{
	while(n--)
		*t++ = src;
}

void FillColor(RGBA *t, const RGBA& src, int n)
{
	while(n--) {
		t->r = src.r;
		t->g = src.g;
		t->b = src.b;
		t++;
	}
}

void UnpackRLE(RGBA *t, const byte *s, int cx)
{
	RGBA *e = t + cx;
	RGBA zero;
	zero.r = zero.g = zero.b = zero.a = 0;
	for(;;) {
		if(*s & 0x80) {
			if(*s == 0x80)
				break;
			int count = min<int>(*s & 0x3F, e - t);
			RGBA h;
			if(*s++ & 0x40)
				h = zero;
			else {
				h.b = s[0];
				h.g = s[1];
				h.r = s[2];
				h.a = 255;
				s += 3;
			}
			ASSERT(t + count <= e);
			memsetex(t, &h, sizeof(RGBA), count);
			t += count;
		}
		else {
			if(*s == 0)
				break;
			int count = *s++;
			while(count--) {
				RGBA h;
				h.b = s[0];
				h.g = s[1];
				h.r = s[2];
				h.a = 255;
				ASSERT(t < e);
				*t++ = h;
				s += 3;
			}
		}
	}
	while(t < e)
		*t++ = zero;
}

void PreMultiplyAlpha(RGBA *t, const RGBA *s, int len)
{
	const RGBA *e = s + len;
	while(s < e) {
		int alpha = s->a + (s->a >> 7);
		t->r = alpha * (s->r) >> 8;
		t->g = alpha * (s->g) >> 8;
		t->b = alpha * (s->b) >> 8;
		t->a = s->a;
		s++;
		t++;
	}
}

void AlphaBlend(RGBA *t, const RGBA *s, int len, byte const_alpha, Color color)
{
	const RGBA *e = s + len;
	if(IsNull(color)) {
		if(const_alpha == 255) {
			while(s < e) {
				int alpha = s->a + (s->a >> 7);
				t->r += alpha * (s->r - t->r) >> 8;
				t->g += alpha * (s->g - t->g) >> 8;
				t->b += alpha * (s->b - t->b) >> 8;
				s++;
				t++;
			}
		}
		else {
			int ca = const_alpha + (const_alpha >> 7);
			while(s < e) {
				int alpha = (ca * s->a) >> 8;
				alpha = alpha + (alpha >> 7);
				t->r += alpha * (s->r - t->r) >> 8;
				t->g += alpha * (s->g - t->g) >> 8;
				t->b += alpha * (s->b - t->b) >> 8;
				s++;
				t++;
			}
		}
	}
	else {
		byte r = color.GetR();
		byte g = color.GetG();
		byte b = color.GetB();
		if(const_alpha == 255) {
			while(s < e) {
				int alpha = s->a + (s->a >> 7);
				t->r += alpha * (r - t->r) >> 8;
				t->g += alpha * (g - t->g) >> 8;
				t->b += alpha * (b - t->b) >> 8;
				s++;
				t++;
			}
		}
		else {
			int ca = const_alpha + (const_alpha >> 7);
			while(s < e) {
				int alpha = (ca * s->a) >> 8;
				alpha = alpha + (alpha >> 7);
				t->r += alpha * (r - t->r) >> 8;
				t->g += alpha * (g - t->g) >> 8;
				t->b += alpha * (b - t->b) >> 8;
				s++;
				t++;
			}
		}
	}
}
