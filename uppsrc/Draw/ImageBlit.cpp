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

void AlphaBlendOpaque(RGBA *t, const RGBA *s, int len)
{
	RTIMING("Opaque");
	const RGBA *e = s + len;
	while(s < e) {
		int alpha = s->a + (s->a >> 7);
		t->r += alpha * (s->r - t->r) >> 8;
		t->g += alpha * (s->g - t->g) >> 8;
		t->b += alpha * (s->b - t->b) >> 8;
		t->a = 255;
		s++;
		t++;
	}
}

void AlphaBlendOpaque(RGBA *t, const RGBA *s, int len, int alpha)
{
	const RGBA *e = s + len;
	alpha = alpha + (alpha >> 7);
	while(s < e) {
		t->r += alpha * (s->r - t->r) >> 8;
		t->g += alpha * (s->g - t->g) >> 8;
		t->b += alpha * (s->b - t->b) >> 8;
		t->a = 255;
		s++;
		t++;
	}
}

void AlphaBlendOpaque(RGBA *t, const RGBA *s, int len, Color color)
{
	const RGBA *e = s + len;
	int r = color.GetR();
	int g = color.GetG();
	int b = color.GetB();
	while(s < e) {
		int alpha = s->a + (s->a >> 7);
		t->r += alpha * (r - t->r) >> 8;
		t->g += alpha * (g - t->g) >> 8;
		t->b += alpha * (b - t->b) >> 8;
		t->a = 255;
		s++;
		t++;
	}
}

struct sBlends {
	int16 m;
	byte  a;
};

sBlends *sblends;

void sOnceInitBlends()
{
	ONCELOCK {
		RTIMING("InitBlends");
		sblends = (sBlends *)MemoryAllocPermanent(256 * 256 * sizeof(sBlends));
		for(int Fa = 0; Fa <= 255; Fa++)
			for(int Ba = 0; Ba <= 255; Ba++) {
				double A = (Fa / 255.0 + Ba / 255.0 - Fa / 255.0 * Ba / 255.0);
				double dDa = 255 * A;
				sblends[(Ba << 8) + Fa].a = minmax((int)(255 * A + 0.5), 0, 255);
				sblends[(Ba << 8) + Fa].m = A > 0.001 ? int(256 * (Fa / 255.0) / A + 0.5) : 0;
			}
	}
}

inline void sInitBlends()
{
	if(!sblends)
		sOnceInitBlends();
}

void AlphaBlend(RGBA *b, const RGBA *f, int len)
{
	RTIMING("Full");
	sInitBlends();
	const RGBA *e = f + len;
	while(f < e) {
		sBlends& x = sblends[(b->a << 8) + f->a];
		int m = x.m;
		b->a = x.a;
		b->r += m * (f->r - b->r) >> 8;
		b->g += m * (f->g - b->g) >> 8;
		b->b += m * (f->b - b->b) >> 8;
		b++;
		f++;
	}
}

void AlphaBlend(RGBA *b, const RGBA *f, int len, Color color)
{
	sInitBlends();
	const RGBA *e = f + len;
	int cr = color.GetR();
	int cg = color.GetG();
	int cb = color.GetB();
	while(f < e) {
		sBlends& x = sblends[(b->a << 8) + f->a];
		int m = x.m;
		b->a = x.a;
		b->r += m * (cr - b->r) >> 8;
		b->g += m * (cg - b->g) >> 8;
		b->b += m * (cb - b->b) >> 8;
		b++;
		f++;
	}
}

/* TODO - remove after uvs2 sync....
static int sArev[256] = {
	0, 32895, 16447, 10965, 8223, 6579, 5482, 4699, 4111, 3655, 3289, 2990, 2741,
	2530, 2349, 2193, 2055, 1935, 1827, 1731, 1644, 1566, 1495, 1430, 1370, 1315,
	1265, 1218, 1174, 1134, 1096, 1061, 1027, 996, 967, 939, 913, 889, 865, 843,
	822, 802, 783, 765, 747, 731, 715, 699, 685, 671, 657, 645, 632, 620, 609, 598,
	587, 577, 567, 557, 548, 539, 530, 522, 513, 506, 498, 490, 483, 476, 469, 463,
	456, 450, 444, 438, 432, 427, 421, 416, 411, 406, 401, 396, 391, 387, 382, 378,
	373, 369, 365, 361, 357, 353, 349, 346, 342, 339, 335, 332, 328, 325, 322, 319,
	316, 313, 310, 307, 304, 301, 299, 296, 293, 291, 288, 286, 283, 281, 278, 276,
	274, 271, 269, 267, 265, 263, 261, 259, 256, 255, 253, 251, 249, 247, 245, 243,
	241, 240, 238, 236, 234, 233, 231, 230, 228, 226, 225, 223, 222, 220, 219, 217,
	216, 215, 213, 212, 210, 209, 208, 206, 205, 204, 203, 201, 200, 199, 198, 196,
	195, 194, 193, 192, 191, 190, 189, 187, 186, 185, 184, 183, 182, 181, 180, 179,
	178, 177, 176, 175, 174, 174, 173, 172, 171, 170, 169, 168, 167, 166, 166, 165,
	164, 163, 162, 162, 161, 160, 159, 158, 158, 157, 156, 155, 155, 154, 153, 153,
	152, 151, 150, 150, 149, 148, 148, 147, 146, 146, 145, 144, 144, 143, 143, 142,
	141, 141, 140, 139, 139, 138, 138, 137, 137, 136, 135, 135, 134, 134, 133, 133,
	132, 132, 131, 131, 130, 130, 129, 129
};

//*/

struct Blends {

};


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
