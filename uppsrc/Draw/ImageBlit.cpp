#include "Draw.h"

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

const byte *UnpackRLE(RGBA *t, const byte *s, int len)
{
	RGBA *e = t + len;
	while(t < e)
		if(*s & 0x80) {
			if(*s == 0x80)
				break;
			int count = min<int>(*s & 0x3F, e - t);
			RGBA h;
			if(*s++ & 0x40)
				Zero(h);
			else {
				h.b = s[0];
				h.g = s[1];
				h.r = s[2];
				h.a = 255;
				s += 3;
			}
			count = min<int>(count, e - t);
			memsetex(t, &h, sizeof(RGBA), count);
			t += count;
		}
		else {
			if(*s == 0)
				break;
			int count = *s++;
			while(count-- && t < e) {
				RGBA h;
				h.b = s[0];
				h.g = s[1];
				h.r = s[2];
				h.a = 255;
				*t++ = h;
				s += 3;
			}
		}
	while(t < e)
		Zero(*t++);
	return s;
}

String PackRLE(const RGBA *s, int len)
{
	StringBuffer r;
	const RGBA *e = s + len;
	while(s < e) {
		const RGBA *q = s;
		if(s->a == 0) {
			s++;
			while(s->a == 0 && s < e && s - q < 0x3f)
				s++;
			r.Cat((0x80|0x40) + (s - q));
		}
		else
		if(s + 1 < e && s[0] == s[1]) {
			s++;
			while(s + 1 < e && s[0] == s[1] && s - q < 0x3e)
				s++;
			s++;
			r.Cat(0x80 + (s - q));
			r.Cat(q->b);
			r.Cat(q->g);
			r.Cat(q->r);
		}
		else {
			s++;
			while(s + 1 < e && s->a && s[0] != s[1] && s - q < 0x3f)
				s++;
			r.Cat(s - q);
			while(q < s) {
				r.Cat(q->b);
				r.Cat(q->g);
				r.Cat(q->r);
				q++;
			}
		}
	}
	return r;
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
				t->a += alpha * (s->a - t->a) >> 8;
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
				t->a += alpha * (s->a - t->a) >> 8;
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
				t->a += alpha * (const_alpha - t->a) >> 8;
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
				t->a += alpha * (const_alpha - t->a) >> 8;
				s++;
				t++;
			}
		}
	}
}

int GetChMaskPos32(dword mask)
{
	if(mask == 0xff000000)
		return 3;
	if(mask == 0xff0000)
		return 2;
	if(mask == 0xff00)
		return 1;
	return 0;
}
