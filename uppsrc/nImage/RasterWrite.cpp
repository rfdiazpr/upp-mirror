#include "Image.h"

void RasterFormat::Write(byte *t, const RGBA *s, int cx, const PaletteCv *palcv) const
{
	const RGBA *e = s + cx;
	switch(type) {
	case RASTER_1:
		if(palcv) {
			const byte *e = t + (cx >> 3);
			while(t < e) {
				*t++ =
					  ((palcv->Get(s[0]) & 1) << 0)
					| ((palcv->Get(s[1]) & 1) << 1)
					| ((palcv->Get(s[2]) & 1) << 2)
					| ((palcv->Get(s[3]) & 1) << 3)
					| ((palcv->Get(s[4]) & 1) << 4)
					| ((palcv->Get(s[5]) & 1) << 5)
					| ((palcv->Get(s[6]) & 1) << 6)
					| ((palcv->Get(s[7]) & 1) << 7)
				;
				s += 8;
			}
			cx &= 7;
			if(cx) {
				byte c = 0;
				byte sh = 0;
				while(cx > 0) {
					c |= (palcv->Get(*s)) << sh;
					sh++;
					cx--;
				}
				*t = c;
			}
		}
		else {
			const byte *e = t + (cx >> 3);
			while(t < e) {
				*t++ =
					  ((s[0].g & 0x80) >> 7)
					| ((s[1].g & 0x80) >> 6)
					| ((s[2].g & 0x80) >> 5)
					| ((s[3].g & 0x80) >> 4)
					| ((s[4].g & 0x80) >> 3)
					| ((s[5].g & 0x80) >> 2)
					| ((s[6].g & 0x80) >> 1)
					| ((s[7].g & 0x80) >> 0)
				;
				s += 8;
			}
			cx &= 7;
			if(cx) {
				byte c = 0;
				byte sh = 7;
				while(cx > 0) {
					c |= (s->r & 0x80) >> sh;
					sh--;
					cx--;
				}
				*t = c;
			}
		}
		break;
	case RASTER_1|RASTER_MSBFIRST:
		if(palcv) {
			const byte *e = t + (cx >> 3);
			while(t < e) {
				*t++ =
					  ((palcv->Get(s[7]) & 1) << 0)
					| ((palcv->Get(s[6]) & 1) << 1)
					| ((palcv->Get(s[5]) & 1) << 2)
					| ((palcv->Get(s[4]) & 1) << 3)
					| ((palcv->Get(s[3]) & 1) << 4)
					| ((palcv->Get(s[2]) & 1) << 5)
					| ((palcv->Get(s[1]) & 1) << 6)
					| ((palcv->Get(s[0]) & 1) << 7)
				;
				s += 8;
			}
			cx &= 7;
			if(cx) {
				byte c = 0;
				byte sh = 7;
				while(cx > 0) {
					c |= (palcv->Get(*s)) << sh;
					sh--;
					cx--;
				}
				*t = c;
			}
		}
		else {
			const byte *e = t + (cx >> 3);
			while(t < e) {
				*t++ =
					  ((s[7].g & 0x80) >> 7)
					| ((s[6].g & 0x80) >> 6)
					| ((s[5].g & 0x80) >> 5)
					| ((s[4].g & 0x80) >> 4)
					| ((s[3].g & 0x80) >> 3)
					| ((s[2].g & 0x80) >> 2)
					| ((s[1].g & 0x80) >> 1)
					| ((s[0].g & 0x80) >> 0)
				;
				s += 8;
			}
			cx &= 7;
			if(cx) {
				byte c = 0;
				byte sh = 0;
				while(cx > 0) {
					c |= (s->r & 0x80) >> sh;
					sh++;
					cx--;
				}
				*t = c;
			}
		}
		break;
	case RASTER_4:
		if(palcv) {
			const byte *e = t + (cx >> 1);
			while(t < e) {
				*t++ = (palcv->Get(s[0]) & 15) | ((palcv->Get(s[1]) & 15) << 4);
				s += 2;
			}
			if(cx & 1)
				*t = palcv->Get(*s) & 15;
		}
		else {
			const byte *e = t + (cx >> 1);
			while(t < e) {
				*t++ = ((GrayScale(s[0]) & 0xf0) >> 4) | ((GrayScale(s[1]) & 0xf0));
				s += 2;
			}
			if(cx & 1)
				*t = (GrayScale(s[0]) & 0xf0) >> 4;
		}
		break;
	case RASTER_4|RASTER_MSBFIRST:
		if(palcv) {
			const byte *e = t + (cx >> 1);
			while(t < e) {
				*t++ = (palcv->Get(s[1]) & 15) | ((palcv->Get(s[0]) & 15) << 4);
				s += 2;
			}
			if(cx & 1)
				*t = (palcv->Get(*s) & 15) << 4;
		}
		else {
			const byte *e = t + (cx >> 1);
			while(t < e) {
				*t++ = ((GrayScale(s[1]) & 0xf0) >> 4) | ((GrayScale(s[0]) & 0xf0));
				s += 2;
			}
			if(cx & 1)
				*t = GrayScale(s[0]) & 0xf0;
		}
		break;
	case RASTER_8:
	case RASTER_8|RASTER_MSBFIRST:
		if(palcv)
			while(s < e) {
				*t++ = palcv->Get(*s);
				s++;
			}
		else
			while(s < e)
				*t++ = GrayScale(*s++);
		break;
	case RASTER_16:
		while(s < e) {
			Poke16le(t, ((s->r << 8 >> rpos) & rmask) |
			            ((s->g << 8 >> gpos) & gmask) |
			            ((s->b << 8 >> bpos) & bmask));
			t += 2;
			s++;
		}
		break;
	case RASTER_16|RASTER_MSBFIRST:
		while(s < e) {
			Poke16be(t, ((s->r << 8 >> rpos) & rmask) |
			            ((s->g << 8 >> gpos) & gmask) |
			            ((s->b << 8 >> bpos) & bmask));
			t += 2;
			s++;
		}
		break;
	case RASTER_24:
	case RASTER_24|RASTER_MSBFIRST:
		while(s < e) {
			t[rpos] = s->r;
			t[gpos] = s->g;
			t[bpos] = s->b;
			t += 3;
			s++;
		}
		break;
	case RASTER_32:
	case RASTER_32|RASTER_MSBFIRST:
		while(s < e) {
			t[rpos] = s->r;
			t[gpos] = s->g;
			t[bpos] = s->b;
			t += 4;
			s++;
		}
		break;
	case RASTER_32ALPHA:
	case RASTER_32ALPHA|RASTER_MSBFIRST:
	#ifdef PLATFORM_WIN32
		if(bpos == 0 && gpos == 1 && rpos == 2 && apos == 3)
			memcpy(t, s, cx * sizeof(RGBA));
		else
	#endif
		{
			while(s < e) {
				t[rpos] = s->r;
				t[gpos] = s->g;
				t[bpos] = s->b;
				t[apos] = s->a;
				t += 4;
				s++;
			}
		}
		break;
	default:
		NEVER();
	}
}
