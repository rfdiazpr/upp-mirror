#include "Image.h"

void AlphaBlend(RGBA *t, RGBA *s, int len, byte const_alpha, Color color)
{
	if(IsNull(color)) {
		if(const_alpha == 255) {
			RGBA *e = s + len;
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
			RGBA *e = s + len;
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
			RGBA *e = s + len;
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
			RGBA *e = s + len;
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
