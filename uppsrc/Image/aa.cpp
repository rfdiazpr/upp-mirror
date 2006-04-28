//////////////////////////////////////////////////////////////////////
// aa: anti-aliasing.//////////////////////////////////////////////////////////////////////
// graphics/imageio_aa.cpp: antialiased image scaling.

#include "Image.h"
#pragma hdrstop

#ifndef NEWIMAGE

#include <Draw/PixelUtil.h>

#if defined(CPU_IA32) && defined(COMPILER_MSC)
#define MSC_ASSEMBLY
#endif

enum
{
#ifdef CPU_LITTLE_ENDIAN
	OFF0 = 0,
	OFF1 = 1,
	OFF2 = 2,
#else
	OFF0 = sizeof(unsigned) - 1,
	OFF1 = sizeof(unsigned) - 2,
	OFF2 = sizeof(unsigned) - 3,
#endif

	MAXAA = 4,

	MAP_COUNT   = 0,
	MAP_SEGMENT = 1,
	MAP_BLOCK   = 2,
	MAP_STEP    = 3,
	MAP_DATA    = 4,
};

#define AA_TIMING(name) // RTIMING(name) // comment off to turn on routine timing

void MMXDump();

enum { LOG2_STRETCH_CURVE = 10, COUNT_STRETCH_CURVE = 1 << LOG2_STRETCH_CURVE };

static const byte *GetStretchCurve()
{
	static Buffer<byte> cache(COUNT_STRETCH_CURVE);
	static bool inited = false;
	if(!inited)
	{
		inited = true;
		for(int i = 0; i < COUNT_STRETCH_CURVE; i++)
		{
			enum { HALF = COUNT_STRETCH_CURVE >> 1 };
			double a = (i <= HALF ? i / double(HALF) : (COUNT_STRETCH_CURVE - i) / double(HALF));
			double o = pow(a, 0.85);
			cache[i] = minmax<int>((int)((i <= HALF ? o : 2 - o) * 128), 0, 255);
		}
	}
	return cache;
}

Vector<unsigned> AAGetMap(int& dmin, int& dmax, int dclipmin, int dclipmax,
	int smin, int smax, int sclipmin, int sclipmax, int times, int avail)
{
	AA_TIMING("AAGetMap");

	Vector<unsigned> map;
	if(dmax == dmin || smax == smin)
		return map;
	if(dmax < dmin)
	{
		Swap(dmin, dmax);
		Swap(smin, smax);
	}
	int dw = dmax - dmin, sw = smax - smin, spos;
	if(sw > 0)
	{
		int x0 = dmin;
		if(smin < sclipmin)
			x0 += iscalefloor(sclipmin - smin, dw, sw);
		if(x0 < dclipmin)
			x0 = dclipmin;
		spos = smin * dw + (x0 - dmin) * sw;
		dmin = x0;
		if(smax > sclipmax)
			dmax -= iscalefloor(smax - sclipmax, dw, sw);
		if(dmax > dclipmax)
			dmax = dclipmax;
	}
	else
	{
		int x0 = dmin;
		if(smin > sclipmax)
			x0 += iscalefloor(sclipmax - smin, dw, sw);
		if(x0 < dclipmin)
			x0 = dclipmin;
		spos = smin * dw + (x0 - dmin) * sw;
		dmin = x0;
		if(smax < sclipmin)
			dmax -= iscalefloor(smax - sclipmin, dw, sw);
		Swap(smin, smax);
	}
	int count = min(dclipmax, dmax) - dmin;
	if(smin < sclipmin)
		smin = sclipmin;
	if(smax > sclipmax)
		smax = sclipmax;
	if(smax <= smin || count <= 0)
		return map;
	int span = min(tabs(sw) % dw ? idivceil(tabs(sw), dw) + 1 : tabs(sw) / dw, smax - smin);
	bool bigseg = (span >= MAXAA);
	int segment = (bigseg ? MAXAA : span);
	int segstep = span / segment;
	map.SetCount(4 + count * (bigseg ? 1 : 1 + segment));
	map[MAP_COUNT] = unsigned(count);
	map[MAP_SEGMENT] = unsigned(segment);
	map[MAP_BLOCK] = 1 + (bigseg ? 0 : segment);
	map[MAP_STEP] = (span / segment) * times;
	unsigned *out = map.Begin() + MAP_DATA;
	int sendoffset = (smax - (segment - 1) * segstep - 1) * times;
	int last = 0;

	if(smax - smin == 1)
	{
		ASSERT(segment == 1);
		unsigned dval = unsigned(smin * times);
		*out++ = dval;
		*out++ = avail;
		while(--count > 0)
		{
			*out++ = 0;
			*out++ = avail;
		}
	}
	else if(tabs(sw) >= dw)
	{ // size reduction
		int sbegin = smin * dw, send = smax * dw, aw = tabs(sw);
		for(spos += min(sw, 0); --count >= 0; spos += sw)
		{
			int pb = max(spos, sbegin), pe = min(spos + aw, send);
			int total = pe - pb, left = avail;
			int start = idivfloor(pb, dw), end = idivceil(pe, dw) - 1, rem = pb % dw;
//			DUMP(start);
//			DUMP(end);
			if(pb >= send)
			{
				last += *out++ = sendoffset - last;
				if(!bigseg)
				{
					int i = segment - 1;
					while(--i >= 0)
						*out++ = 0;
					*out++ = left;
				}
			}
			else if(end <= start)
			{ // 1 source pixel only
//				ASSERT(!bigseg);
				int scomp = minmax(start + segment - smax, 0, start - smin);
				last += *out++ = (start - scomp) * times - last;
				if(!bigseg)
				{
					int i = scomp;
					while(--i >= 0)
						*out++ = 0;
					*out++ = unsigned(left);
					i = segment - scomp - 1;
					while(--i >= 0)
						*out++ = 0;
				}
			}
			else
			{
				int delta = (dw - rem) * left / total;
				if(!delta)
					start++;
				int scomp = minmax(start + span - smax, 0, start - smin);
				last += *out++ = (start - scomp) * times - last;
				if(!bigseg)
				{
					int i = scomp;
					while(--i >= 0)
						*out++ = 0;
					i = segment - scomp;
					if(delta)
					{
						*out++ = delta;
						left -= delta;
						total -= dw - rem;
						i--;
					}
					while(++start < end)
					{
						ASSERT(i > 0);
						delta = dw * left / total;
						*out++ = delta;
						left -= delta;
						total -= dw;
						--i;
					}
					if(left > 0)
					{
						ASSERT(i > 0);
						*out++ = left;
						--i;
					}
					while(--i >= 0)
						*out++ = 0;
				}
			}
//			LOG("-> " << map[rec] << " + " << map[rec + 1]);
		}
	}
	else
	{ // size inflation
		static const byte *curve = GetStretchCurve();
		ASSERT(segment == 2 && !bigseg);
		int sbegin = smin * dw, send = (smax - 1) * dw;
		for(spos += (sw - dw) >> 1; --count >= 0; spos += sw)
		{
			if(spos <= sbegin)
			{
				last += out[0] = smin * times - last;
				out[1] = avail;
				out[2] = 0;
			}
			else if(spos >= send)
			{
				last += out[0] = sendoffset - last;
				out[1] = 0;
				out[2] = avail;
			}
			else
			{
				int pos = spos / dw;
				int rel = spos % dw;
				last += out[0] = pos * times - last;
				int w1 = rel * avail / dw;
				out[1] = avail - (out[2] = curve[rel * COUNT_STRETCH_CURVE / dw]);
			}
			out += 3;
		}
	}

#ifdef _DEBUG
	ASSERT(out == map.End());
	int offs = 0, step = map[MAP_BLOCK], segspan = (map[MAP_SEGMENT] - 1) * map[MAP_STEP] + 1;
	for(int t = 0; t < (int)map[MAP_COUNT]; t++)
	{
		offs += map[MAP_DATA + t * step];
		ASSERT(offs >= times * smin && offs + segspan <= times * smax);
	}
#endif

	return map;
}

#define BLT_AA_MAP_REP4(BPROC, o) \
	BPROC((o) + 0) \
	BPROC((o) + 1) \
	BPROC((o) + 2) \
	BPROC((o) + 3)

#define BLT_AA_MAP_PROC(SUFFIX, SCOUNT, BPROC, INITPAL) \
static void COMBINE(BltAAMap, SUFFIX)(unsigned *dest, const byte *s, const unsigned *map, const byte *pal) \
{ \
	AA_TIMING("BltAAMap" #SUFFIX); \
	unsigned count = map[MAP_COUNT]; \
	int step = map[MAP_STEP]; \
	unsigned offset = 0; \
	map += 4; \
	enum { STEP = (SCOUNT < MAXAA ? SCOUNT + 1 : 1) }; \
	INITPAL() \
	\
	for(unsigned rep = count >> 4; rep; rep--) \
	{ \
		BLT_AA_MAP_REP4(BPROC, 0) \
		BLT_AA_MAP_REP4(BPROC, 4) \
		BLT_AA_MAP_REP4(BPROC, 8) \
		BLT_AA_MAP_REP4(BPROC, 12) \
		dest += 3 * 16; \
		map += STEP * 16; \
	} \
	if(count & 8) \
	{ \
		BLT_AA_MAP_REP4(BPROC, 0) BLT_AA_MAP_REP4(BPROC, 4) \
		dest += 3 * 8; \
		map += STEP * 8; \
	} \
	if(count & 4) \
	{ \
		BLT_AA_MAP_REP4(BPROC, 0) \
		dest += 3 * 4; \
		map += STEP * 4; \
	} \
	if(count & 2) \
	{ \
		BPROC(0) \
		BPROC(1) \
		dest += 3 * 2; \
		map += STEP * 2; \
	} \
	if(count & 1) \
	{ \
		BPROC(0) \
	} \
}

#define BLT_AA_MAP_32BIT_1(o) s += map[2 * (o) + 0]; \
	c0 = PeekIL(s); \
	dest[3 * (o) + 0] = ((c0 & bmask) << bshift24) >> 16; \
	dest[3 * (o) + 1] = ((c0 & gmask) << gshift24) >> 16; \
	dest[3 * (o) + 2] = ((c0 & rmask) << rshift24) >> 16;

#define BLT_AA_MAP_32BIT_2(o) s += map[3 * (o) + 0]; \
	c0 = PeekIL(s); c1 = PeekIL(s + 4); \
	dest[3 * (o) + 0] = (((c0 & bmask) << bshift24) >> 24) * map[3 * (o) + 1] + (((c1 & bmask) << bshift24) >> 24) * map[3 * (o) + 2]; \
	dest[3 * (o) + 1] = (((c0 & gmask) << gshift24) >> 24) * map[3 * (o) + 1] + (((c1 & gmask) << gshift24) >> 24) * map[3 * (o) + 2]; \
	dest[3 * (o) + 2] = (((c0 & rmask) << rshift24) >> 24) * map[3 * (o) + 1] + (((c1 & rmask) << rshift24) >> 24) * map[3 * (o) + 2];

#define BLT_AA_MAP_32BIT_3(o) s += map[4 * (o) + 0]; \
	c0 = PeekIL(s); c1 = PeekIL(s + 4); c2 = PeekIL(s + 8); \
	dest[3 * (o) + 0] = (((c0 & bmask) << bshift24) >> 24) * map[4 * (o) + 1] + (((c1 & bmask) << bshift24) >> 24) * map[4 * (o) + 2] + (((c2 & bmask) << bshift24) >> 24) * map[4 * (o) + 3]; \
	dest[3 * (o) + 1] = (((c0 & gmask) << gshift24) >> 24) * map[4 * (o) + 1] + (((c1 & gmask) << gshift24) >> 24) * map[4 * (o) + 2] + (((c2 & gmask) << gshift24) >> 24) * map[4 * (o) + 3]; \
	dest[3 * (o) + 2] = (((c0 & rmask) << rshift24) >> 24) * map[4 * (o) + 1] + (((c1 & rmask) << rshift24) >> 24) * map[4 * (o) + 2] + (((c2 & rmask) << rshift24) >> 24) * map[4 * (o) + 3];

#define BLT_AA_MAP_32BIT_4(o) s += map[(o) + 0]; \
	c0 = PeekIL(s); c1 = PeekIL(s + step); c2 = PeekIL(s + 2 * step); c3 = PeekIL(s + 3 * step); \
	dest[3 * (o) + 0] = (((c0 & bmask) << bshift24) >> 18) + (((c1 & bmask) << bshift24) >> 18) + (((c2 & bmask) << bshift24) >> 18) + (((c3 & bmask) << bshift24) >> 18); \
	dest[3 * (o) + 1] = (((c0 & gmask) << gshift24) >> 18) + (((c1 & gmask) << gshift24) >> 18) + (((c2 & gmask) << gshift24) >> 18) + (((c3 & gmask) << gshift24) >> 18); \
	dest[3 * (o) + 2] = (((c0 & rmask) << rshift24) >> 18) + (((c1 & rmask) << rshift24) >> 18) + (((c2 & rmask) << rshift24) >> 18) + (((c3 & rmask) << rshift24) >> 18);

#define BLT_AA_MAP_24BIT_1(o) s += map[2 * (o) + 0]; \
	dest[3 * (o) + 0] = s[0] * 0x100; \
	dest[3 * (o) + 1] = s[1] * 0x100; \
	dest[3 * (o) + 2] = s[2] * 0x100; \

#define BLT_AA_MAP_24BIT_2(o) s += map[3 * (o) + 0]; \
	dest[3 * (o) + 0] = s[0] * map[3 * (o) + 1] + s[3] * map[3 * (o) + 2]; \
	dest[3 * (o) + 1] = s[1] * map[3 * (o) + 1] + s[4] * map[3 * (o) + 2]; \
	dest[3 * (o) + 2] = s[2] * map[3 * (o) + 1] + s[5] * map[3 * (o) + 2]; \

/*
#define BLT_AA_MAP_24BIT_3(o) \
	dest[3 * (o) + 0] = s[0] * map[4 * (o) + 1] + s[3] * map[4 * (o) + 2] + s[6] * map[4 * (o) + 3]; \
	dest[3 * (o) + 1] = s[1] * map[4 * (o) + 1] + s[4] * map[4 * (o) + 2] + s[7] * map[4 * (o) + 3]; \
	dest[3 * (o) + 2] = s[2] * map[4 * (o) + 1] + s[5] * map[4 * (o) + 2] + s[8] * map[4 * (o) + 3]; \
*/
#define BLT_AA_MAP_24BIT_3(o) s += map[4 * (o) + 0]; \
	dest[3 * (o) + 0] = (s[0] - s[6]) * map[4 * (o) + 1] + (s[3] - s[6]) * map[4 * (o) + 2] + s[6] * 0x100; \
	dest[3 * (o) + 1] = (s[1] - s[7]) * map[4 * (o) + 1] + (s[4] - s[7]) * map[4 * (o) + 2] + s[7] * 0x100; \
	dest[3 * (o) + 2] = (s[2] - s[8]) * map[4 * (o) + 1] + (s[5] - s[8]) * map[4 * (o) + 2] + s[8] * 0x100; \

#define BLT_AA_MAP_24BIT_4(o) s += map[(o) + 0]; \
	dest[3 * (o) + 0] = (s[0] + s[0 + step] + s[0 + 2 * step] + s[0 + 3 * step]) << 6; \
	dest[3 * (o) + 1] = (s[1] + s[1 + step] + s[1 + 2 * step] + s[1 + 3 * step]) << 6; \
	dest[3 * (o) + 2] = (s[2] + s[2 + step] + s[2 + 2 * step] + s[2 + 3 * step]) << 6; \

#define BLT_AA_MAP_16BIT_1(o) s += map[2 * (o) + 0]; \
	c0 = PeekIW(s); \
	dest[3 * (o) + 0] = ((c0 & bmask) << bshift24) >> 16; \
	dest[3 * (o) + 1] = ((c0 & gmask) << gshift24) >> 16; \
	dest[3 * (o) + 2] = ((c0 & rmask) << rshift24) >> 16;

#define BLT_AA_MAP_16BIT_2(o) s += map[3 * (o) + 0]; \
	c0 = PeekIW(s); c1 = PeekIW(s + 2); \
	dest[3 * (o) + 0] = (((c0 & bmask) << bshift24) >> 24) * map[3 * (o) + 1] + (((c1 & bmask) << bshift24) >> 24) * map[3 * (o) + 2]; \
	dest[3 * (o) + 1] = (((c0 & gmask) << gshift24) >> 24) * map[3 * (o) + 1] + (((c1 & gmask) << gshift24) >> 24) * map[3 * (o) + 2]; \
	dest[3 * (o) + 2] = (((c0 & rmask) << rshift24) >> 24) * map[3 * (o) + 1] + (((c1 & rmask) << rshift24) >> 24) * map[3 * (o) + 2];

#define BLT_AA_MAP_16BIT_3(o) s += map[4 * (o) + 0]; \
	c0 = PeekIW(s); c1 = PeekIW(s + 2); c2 = PeekIW(s + 4); \
	dest[3 * (o) + 0] = (((c0 & bmask) << bshift24) >> 24) * map[4 * (o) + 1] + (((c1 & bmask) << bshift24) >> 24) * map[4 * (o) + 2] + (((c2 & bmask) << bshift24) >> 24) * map[4 * (o) + 3]; \
	dest[3 * (o) + 1] = (((c0 & gmask) << gshift24) >> 24) * map[4 * (o) + 1] + (((c1 & gmask) << gshift24) >> 24) * map[4 * (o) + 2] + (((c2 & gmask) << gshift24) >> 24) * map[4 * (o) + 3]; \
	dest[3 * (o) + 2] = (((c0 & rmask) << rshift24) >> 24) * map[4 * (o) + 1] + (((c1 & rmask) << rshift24) >> 24) * map[4 * (o) + 2] + (((c2 & rmask) << rshift24) >> 24) * map[4 * (o) + 3];

#define BLT_AA_MAP_16BIT_4(o) s += map[(o) + 0]; \
	c0 = PeekIW(s); c1 = PeekIW(s + step); c2 = PeekIW(s + 2 * step); c3 = PeekIW(s + 3 * step); \
	dest[3 * (o) + 0] = (((c0 & bmask) << bshift24) >> 18) + (((c1 & bmask) << bshift24) >> 18) + (((c2 & bmask) << bshift24) >> 18) + (((c3 & bmask) << bshift24) >> 18); \
	dest[3 * (o) + 1] = (((c0 & gmask) << gshift24) >> 18) + (((c1 & gmask) << gshift24) >> 18) + (((c2 & gmask) << gshift24) >> 18) + (((c3 & gmask) << gshift24) >> 18); \
	dest[3 * (o) + 2] = (((c0 & rmask) << rshift24) >> 18) + (((c1 & rmask) << rshift24) >> 18) + (((c2 & rmask) << rshift24) >> 18) + (((c3 & rmask) << rshift24) >> 18);

#define BLT_AA_MAP_8BIT_1(o) s += map[2 * (o) + 0]; \
	dest[3 * (o) + 0] = pal[s[0] + 0x200] * 0x100; \
	dest[3 * (o) + 1] = pal[s[0] + 0x100] * 0x100; \
	dest[3 * (o) + 2] = pal[s[0] + 0x000] * 0x100; \

#define BLT_AA_MAP_8BIT_2(o) s += map[3 * (o) + 0]; \
	dest[3 * (o) + 0] = pal[s[0] + 0x200] * map[3 * (o) + 1] + pal[s[1] + 0x200] * map[3 * (o) + 2]; \
	dest[3 * (o) + 1] = pal[s[0] + 0x100] * map[3 * (o) + 1] + pal[s[1] + 0x100] * map[3 * (o) + 2]; \
	dest[3 * (o) + 2] = pal[s[0] + 0x000] * map[3 * (o) + 1] + pal[s[1] + 0x000] * map[3 * (o) + 2]; \

#define BLT_AA_MAP_8BIT_3(o) s += map[4 * (o) + 0]; \
	dest[3 * (o) + 0] = pal[s[0] + 0x200] * map[4 * (o) + 1] + pal[s[1] + 0x200] * map[4 * (o) + 2] + pal[s[2] + 0x200] * map[4 * (o) + 3]; \
	dest[3 * (o) + 1] = pal[s[0] + 0x100] * map[4 * (o) + 1] + pal[s[1] + 0x100] * map[4 * (o) + 2] + pal[s[2] + 0x100] * map[4 * (o) + 3]; \
	dest[3 * (o) + 2] = pal[s[0] + 0x000] * map[4 * (o) + 1] + pal[s[1] + 0x000] * map[4 * (o) + 2] + pal[s[2] + 0x000] * map[4 * (o) + 3]; \

#define BLT_AA_MAP_8BIT_4(o) s += map[(o) + 0]; \
	dest[3 * (o) + 0] = (pal[s[0] + 0x200] + pal[s[1 * step] + 0x200] + pal[s[2 * step] + 0x200] + pal[s[3 * step] + 0x200]) << 6; \
	dest[3 * (o) + 1] = (pal[s[0] + 0x100] + pal[s[1 * step] + 0x100] + pal[s[2 * step] + 0x100] + pal[s[3 * step] + 0x100]) << 6; \
	dest[3 * (o) + 2] = (pal[s[0] + 0x000] + pal[s[1 * step] + 0x000] + pal[s[2 * step] + 0x000] + pal[s[3 * step] + 0x000]) << 6; \

#define BLT_AA_MAP_1BIT_1(o) offset += map[2 * (o) + 0]; \
	{ \
		const byte *p = s + (offset >> 3); \
		const byte *p1 = &pal[(*p >> (~offset & 7)) & 1]; \
		dest[3 * (o) + 0] = p1[0x200] * 0x100; \
		dest[3 * (o) + 1] = p1[0x100] * 0x100; \
		dest[3 * (o) + 2] = p1[0x000] * 0x100; \
	}

#define BLT_AA_MAP_1BIT_2(o) offset += map[3 * (o) + 0]; \
	{ \
		const byte *p = s + (offset >> 3); \
		int s = ~offset & 7; \
		const byte *p1 = &pal[(*p >> s) & 1]; \
		const byte *p2 = &pal[s ? (*p >> (s - 1)) & 1 : p[1] >> 7]; \
		dest[3 * (o) + 0] = p1[0x200] * map[3 * (o) + 1] + p2[0x200] * map[3 * (o) + 2]; \
		dest[3 * (o) + 1] = p1[0x100] * map[3 * (o) + 1] + p2[0x100] * map[3 * (o) + 2]; \
		dest[3 * (o) + 2] = p1[0x000] * map[3 * (o) + 1] + p2[0x000] * map[3 * (o) + 2]; \
	}

#define BLT_AA_MAP_1BIT_3(o) offset += map[4 * (o) + 0]; \
	{ \
		const byte *p = s + (offset >> 3); \
		int sh = (offset & 7) - 5; \
		byte v = (sh <= 0 ? p[0] >> -sh : (p[0] << sh) | (p[1] >> (8 - sh))); \
		const byte *p1 = &pal[(v >> 2) & 1], *p2 = &pal[(v >> 1) & 1], *p3 = &pal[(v >> 0) & 1]; \
		dest[3 * (o) + 0] = p1[0x200] * map[4 * (o) + 1] + p2[0x200] * map[4 * (o) + 2] + p3[0x200] * map[4 * (o) + 3]; \
		dest[3 * (o) + 1] = p1[0x100] * map[4 * (o) + 1] + p2[0x100] * map[4 * (o) + 2] + p3[0x100] * map[4 * (o) + 3]; \
		dest[3 * (o) + 2] = p1[0x000] * map[4 * (o) + 1] + p2[0x000] * map[4 * (o) + 2] + p3[0x000] * map[4 * (o) + 3]; \
	}

#define BLT_AA_MAP_1BIT_4(o) offset += map[(o) + 0]; \
	{ \
		int d = offset; \
		const byte *p1 = &pal[byte(s[d >> 3] << (d & 7)) >> 7]; d += step; \
		const byte *p2 = &pal[byte(s[d >> 3] << (d & 7)) >> 7]; d += step; \
		const byte *p3 = &pal[byte(s[d >> 3] << (d & 7)) >> 7]; d += step; \
		const byte *p4 = &pal[byte(s[d >> 3] << (d & 7)) >> 7]; \
		dest[3 * (o) + 0] = (p1[0x200] + p2[0x200] + p3[0x200] + p4[0x200]) << 6; \
		dest[3 * (o) + 1] = (p1[0x100] + p2[0x100] + p3[0x100] + p4[0x100]) << 6; \
		dest[3 * (o) + 2] = (p1[0x000] + p2[0x000] + p3[0x000] + p4[0x000]) << 6; \
	}

#define BLT_AA_MAP_4BIT_1(o) offset += map[2 * (o) + 0]; \
	{ \
		const byte *p = s + (offset >> 1); \
		const byte *p1 = &pal[offset & 1 ? *p & 15 : *p >> 4]; \
		dest[3 * (o) + 0] = p1[0x200] * 0x100; \
		dest[3 * (o) + 1] = p1[0x100] * 0x100; \
		dest[3 * (o) + 2] = p1[0x000] * 0x100; \
	}

#define BLT_AA_MAP_4BIT_2(o) offset += map[3 * (o) + 0]; \
	{ \
		const byte *p = s + (offset >> 1); \
		const byte *p1, *p2; \
		if(offset & 1) \
			p1 = &pal[p[0] & 15], p2 = &pal[p[1] >> 4]; \
		else \
			p1 = &pal[p[0] >> 4], p2 = &pal[p[0] & 15]; \
		dest[3 * (o) + 0] = p1[0x200] * map[3 * (o) + 1] + p2[0x200] * map[3 * (o) + 2]; \
		dest[3 * (o) + 1] = p1[0x100] * map[3 * (o) + 1] + p2[0x100] * map[3 * (o) + 2]; \
		dest[3 * (o) + 2] = p1[0x000] * map[3 * (o) + 1] + p2[0x000] * map[3 * (o) + 2]; \
	}

#define BLT_AA_MAP_4BIT_3(o) offset += map[4 * (o) + 0]; \
	{ \
		const byte *p = s + (offset >> 1); \
		const byte *p1, *p2, *p3; \
		if(offset & 1) \
			p1 = &pal[p[0] & 15], p2 = &pal[p[1] >> 4], p3 = &pal[p[1] & 15]; \
		else \
			p1 = &pal[p[0] >> 4], p2 = &pal[p[0] & 15], p3 = &pal[p[1] >> 4]; \
		dest[3 * (o) + 0] = p1[0x200] * map[4 * (o) + 1] + p2[0x200] * map[4 * (o) + 2] + p3[0x200] * map[4 * (o) + 3]; \
		dest[3 * (o) + 1] = p1[0x100] * map[4 * (o) + 1] + p2[0x100] * map[4 * (o) + 2] + p3[0x100] * map[4 * (o) + 3]; \
		dest[3 * (o) + 2] = p1[0x000] * map[4 * (o) + 1] + p2[0x000] * map[4 * (o) + 2] + p3[0x000] * map[4 * (o) + 3]; \
	}

#define BLT_AA_MAP_4BIT_4(o) offset += map[(o) + 0]; \
	{ \
		int d = offset; \
		const byte *p1 = &pal[byte(s[d >> 1] << ((d & 1) << 2)) >> 4]; d += step; \
		const byte *p2 = &pal[byte(s[d >> 1] << ((d & 1) << 2)) >> 4]; d += step; \
		const byte *p3 = &pal[byte(s[d >> 1] << ((d & 1) << 2)) >> 4]; d += step; \
		const byte *p4 = &pal[byte(s[d >> 1] << ((d & 1) << 2)) >> 4]; \
		dest[3 * (o) + 0] = (p1[0x200] + p2[0x200] + p3[0x200] + p4[0x200]) << 6; \
		dest[3 * (o) + 1] = (p1[0x100] + p2[0x100] + p3[0x100] + p4[0x100]) << 6; \
		dest[3 * (o) + 2] = (p1[0x000] + p2[0x000] + p3[0x000] + p4[0x000]) << 6; \
	}

#define InitNop() ;
#define InitBit0() \
	dword bmask = PeekIL(pal + 0), gmask = PeekIL(pal + 4), rmask = PeekIL(pal + 8); \
	byte bshift24 = pal[12], gshift24 = pal[13], rshift24 = pal[14];

#define InitBit1() InitBit0() dword c0;
#define InitBit2() InitBit1() dword c1;
#define InitBit3() InitBit2() dword c2;
#define InitBit4() InitBit3() dword c3;

BLT_AA_MAP_PROC(32bit1, 1, BLT_AA_MAP_32BIT_1, InitBit1)
BLT_AA_MAP_PROC(32bit2, 2, BLT_AA_MAP_32BIT_2, InitBit2)
BLT_AA_MAP_PROC(32bit3, 3, BLT_AA_MAP_32BIT_3, InitBit3)
BLT_AA_MAP_PROC(32bit4, 4, BLT_AA_MAP_32BIT_4, InitBit4)
BLT_AA_MAP_PROC(24bit1, 1, BLT_AA_MAP_24BIT_1, InitNop)
//BLT_AA_MAP_PROC(24bit2, 2, BLT_AA_MAP_24BIT_2)
BLT_AA_MAP_PROC(24bit3, 3, BLT_AA_MAP_24BIT_3, InitNop)
BLT_AA_MAP_PROC(24bit4, 4, BLT_AA_MAP_24BIT_4, InitNop)
BLT_AA_MAP_PROC(16bit1, 1, BLT_AA_MAP_16BIT_1, InitBit1)
BLT_AA_MAP_PROC(16bit2, 2, BLT_AA_MAP_16BIT_2, InitBit2)
BLT_AA_MAP_PROC(16bit3, 3, BLT_AA_MAP_16BIT_3, InitBit3)
BLT_AA_MAP_PROC(16bit4, 4, BLT_AA_MAP_16BIT_4, InitBit4)
BLT_AA_MAP_PROC(8bit1, 1, BLT_AA_MAP_8BIT_1, InitNop)
BLT_AA_MAP_PROC(8bit2, 2, BLT_AA_MAP_8BIT_2, InitNop)
BLT_AA_MAP_PROC(8bit3, 3, BLT_AA_MAP_8BIT_3, InitNop)
BLT_AA_MAP_PROC(8bit4, 4, BLT_AA_MAP_8BIT_4, InitNop)
BLT_AA_MAP_PROC(4bit1, 1, BLT_AA_MAP_4BIT_1, InitNop)
BLT_AA_MAP_PROC(4bit2, 2, BLT_AA_MAP_4BIT_2, InitNop)
BLT_AA_MAP_PROC(4bit3, 3, BLT_AA_MAP_4BIT_3, InitNop)
BLT_AA_MAP_PROC(4bit4, 4, BLT_AA_MAP_4BIT_4, InitNop)
BLT_AA_MAP_PROC(1bit1, 1, BLT_AA_MAP_1BIT_1, InitNop)
BLT_AA_MAP_PROC(1bit2, 2, BLT_AA_MAP_1BIT_2, InitNop)
BLT_AA_MAP_PROC(1bit3, 3, BLT_AA_MAP_1BIT_3, InitNop)
BLT_AA_MAP_PROC(1bit4, 4, BLT_AA_MAP_1BIT_4, InitNop)

/*
#ifdef MSC_ASSEMBLY
static void BltAAMap2MMX(unsigned *dest, const byte *src, const unsigned *map)
{
	__int64 buffer[6];

	__asm lea edi, [buffer]
	__asm pxor mm0, mm0
	__asm movq [buffer + 0 * 8], mm0
	__asm movq [buffer + 1 * 8], mm0
	__asm movq [buffer + 2 * 8], mm0
	__asm movq [buffer + 3 * 8], mm0
	__asm movq [buffer + 4 * 8], mm0
	__asm mobq [buffer + 5 * 8], mm0
	__asm mov edi, [dest]
	__asm mov esi, [map]
	__asm mov eax, [esi]
	__asm lea ebx, [esi + 4 * eax + 8]
	__asm add esi, 8
	__asm push eax
	__asm shr eax, 2
__rep:
	__asm mov ecx, [esi]
	__asm add ecx, [src]
	__asm mov dl, [ecx + 0]
	__asm mov dh, [ecx + 3]
	__asm mov [buffer + 0 * 8 + 0], dl
	__asm mov [buffer + 0 * 8 + 4], dh
	__asm mov dl, [ecx + 1]
	__asm mov dh, [ecx + 4]
	__asm mov [buffer + 1 * 8 + 0], dl
	__asm mov [buffer + 1 * 8 + 4], dh
	__asm mov dl, [ecx + 2]
	__asm mov dh, [ecx + 5]
	__asm mov [buffer + 2 * 8 + 0], dl
	__asm mov [buffer + 2 * 8 + 4], dh
	__asm mov ecx, [esi + 4]
	__asm add ecx, [src]
	__asm mov dl, [ecx + 0]
	__asm mov dh, [ecx + 3]
	__asm mov [buffer + 3 * 8 + 0], dl
	__asm mov [buffer + 3 * 8 + 4], dh
	__asm mov dl, [ecx + 1]
	__asm mov dh, [ecx + 4]
	__asm mov [buffer + 4 * 8 + 0], dl
	__asm mov [buffer + 4 * 8 + 4], dh
	__asm mov dl, [ecx + 2]
	__asm mov dh, [ecx + 5]
	__asm mov [buffer + 5 * 8 + 0], dl
	__asm mov [buffer + 5 * 8 + 4], dh
	__asm movq mm0, [ebx]
	__asm movq mm1, [ebx + 8]
	__asm pmaddwd

	__asm xor ebx, ebx
	__asm mov [edi
	__asm mov esi, [map]
	__asm mov
}
#endif
*/

//*
static void BltAAMap24bit2(unsigned *dest, const byte *s, const unsigned *map, const byte *pal)
{
	AA_TIMING("BltAAMap24bit2");
	unsigned count = map[MAP_COUNT];
	map += 4;

	unsigned a, b;
//	unsigned w0, w1;

//	w0 = weights[2 * (o) + 0];
//	w1 = weights[2 * (o) + 1];

/*
#define BLT_AA_MAP_RGB_2_1(o) \
	s += map[3 * (o)]; \
	a = s[0] * map[3 * (o) + 1] + s[3] * map[3 * (o) + 2]; \
	b = s[1] * map[3 * (o) + 1] + s[4] * map[3 * (o) + 2]; \
	dest[3 * (o) + 2] = s[2] * map[3 * (o) + 1] + s[5] * map[3 * (o) + 2]; \
	dest[3 * (o) + 1] = b; dest[3 * (o) + 0] = a;
*/
#define BLT_AA_MAP_RGB_2_1(o) \
	s += map[3 * (o)]; \
	a = (s[0] - s[3]) * map[3 * (o) + 1] + s[3] * 0x100; \
	b = (s[1] - s[4]) * map[3 * (o) + 1] + s[4] * 0x100; \
	dest[3 * (o) + 2] = (s[2] - s[5]) * map[3 * (o) + 1] + s[5] * 0x100; \
	dest[3 * (o) + 1] = b; dest[3 * (o) + 0] = a;

#define BLT_AA_MAP_RGB_2_4(o) \
	BLT_AA_MAP_RGB_2_1((o) + 0) BLT_AA_MAP_RGB_2_1((o) + 1) \
	BLT_AA_MAP_RGB_2_1((o) + 2) BLT_AA_MAP_RGB_2_1((o) + 3)

	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_AA_MAP_RGB_2_4(0) BLT_AA_MAP_RGB_2_4(4) BLT_AA_MAP_RGB_2_4(8) BLT_AA_MAP_RGB_2_4(12)
		BLT_AA_MAP_RGB_2_4(16) BLT_AA_MAP_RGB_2_4(20) BLT_AA_MAP_RGB_2_4(24) BLT_AA_MAP_RGB_2_4(28)
		dest += 3 * 32;
		map += 3 * 32;
	}
	if(count & 16)
	{
		BLT_AA_MAP_RGB_2_4(0) BLT_AA_MAP_RGB_2_4(4) BLT_AA_MAP_RGB_2_4(8) BLT_AA_MAP_RGB_2_4(12)
		dest += 3 * 16;
		map += 3 * 16;
	}
	if(count & 8)
	{
		BLT_AA_MAP_RGB_2_4(0) BLT_AA_MAP_RGB_2_4(4)
		dest += 3 * 8;
		map += 3 * 8;
	}
	if(count & 4)
	{
		BLT_AA_MAP_RGB_2_4(0)
		dest += 3 * 4;
		map += 3 * 4;
	}
	if(count & 2)
	{
		BLT_AA_MAP_RGB_2_1(0) BLT_AA_MAP_RGB_2_1(1)
		dest += 3 * 2;
		map += 3 * 2;
	}
	if(count & 1)
	{
		BLT_AA_MAP_RGB_2_1(0)
	}
}
//*/

/*
static void BltAAMap24bitAny(unsigned *dest, const byte *s, const unsigned *map, const byte *pal)
{
	AA_TIMING("BltAAMap24bitAny");
	unsigned count = map[0];
	unsigned segment = map[1];
	unsigned seg4 = segment >> 2;
	unsigned comp = 3 * segment;
	map += 2;
	do
	{
		s += *map++;
		unsigned b = 0, g = 0, r = 0;
		for(unsigned n = seg4; n; n--)
		{
			b += s[0] * map[0] + s[3] * map[1] + s[6] * map[2] + s[ 9] * map[3];
			g += s[1] * map[0] + s[4] * map[1] + s[7] * map[2] + s[10] * map[3];
			r += s[2] * map[0] + s[5] * map[1] + s[8] * map[2] + s[11] * map[3];
			s += 12;
			map += 4;
		}
		if(segment & 2)
		{
			b += s[0] * map[0] + s[3] * map[1];
			g += s[1] * map[0] + s[4] * map[1];
			r += s[2] * map[0] + s[5] * map[1];
			s += 6;
			map += 2;
		}
		if(segment & 1)
		{
			b += s[0] * map[0];
			g += s[1] * map[0];
			r += s[2] * map[0];
			s += 3;
			map++;
		}
		dest[0] = b;
		dest[1] = g;
		dest[2] = r;
		dest += 3;
		s -= comp;
	}
	while(--count);
}
*/

/*
static void BltAAMap8bitAny(unsigned *dest, const byte *s, const unsigned *map, const byte *pal)
{
	AA_TIMING("BltAAMap8bitAny");
	unsigned count = map[0];
	unsigned segment = map[1];
	unsigned seg4 = segment >> 2;
	unsigned comp = segment;
	map += 2;
	do
	{
		s += *map++;
		unsigned b = 0, g = 0, r = 0;
		for(unsigned n = seg4; n; n--)
		{
			b += pal[s[0] + 0x200] * map[0] + pal[s[1] + 0x200] * map[1] + pal[s[2] + 0x200] * map[2] + pal[s[3] + 0x200] * map[3];
			g += pal[s[0] + 0x100] * map[0] + pal[s[1] + 0x100] * map[1] + pal[s[2] + 0x100] * map[2] + pal[s[3] + 0x100] * map[3];
			r += pal[s[0] + 0x000] * map[0] + pal[s[1] + 0x000] * map[1] + pal[s[2] + 0x000] * map[2] + pal[s[3] + 0x000] * map[3];
			s += 4;
			map += 4;
		}
		switch(segment & 3)
		{
		case 0:
			break;

		case 1:
			b += pal[s[0] + 0x200] * map[0];
			g += pal[s[0] + 0x100] * map[0];
			r += pal[s[0] + 0x000] * map[0];
			s++;
			map++;
			break;

		case 2:
			b += pal[s[0] + 0x200] * map[0] + pal[s[1] + 0x200] * map[1];
			g += pal[s[0] + 0x100] * map[0] + pal[s[1] + 0x100] * map[1];
			r += pal[s[0] + 0x000] * map[0] + pal[s[1] + 0x000] * map[1];
			s += 2;
			map += 2;
			break;

		case 3:
			b += pal[s[0] + 0x200] * map[0] + pal[s[1] + 0x200] * map[1] + pal[s[2] + 0x200] * map[2];
			g += pal[s[0] + 0x100] * map[0] + pal[s[1] + 0x100] * map[1] + pal[s[2] + 0x100] * map[2];
			r += pal[s[0] + 0x000] * map[0] + pal[s[1] + 0x000] * map[1] + pal[s[2] + 0x000] * map[2];
			s += 3;
			map += 3;
			break;
		}
		dest[0] = b;
		dest[1] = g;
		dest[2] = r;
		dest += 3;
		s -= comp;
	}
	while(--count);
}
*/

/*
static void BltAAMap1bitAny(unsigned *dest, const byte *s, const unsigned *map, const byte *pal)
{
	AA_TIMING("BltAAMap1bitAny");
	unsigned count = map[0];
	unsigned segment = map[1];
	unsigned seg8 = segment >> 3;
	unsigned offset = 0;
	map += 2;
	do
	{
		offset += *map++;
		int sh1 = offset & 7, sh2 = 8 - sh1;
		const byte *p = s + (offset >> 3);
		unsigned b = 0, g = 0, r = 0;
		const byte *pp;
		byte v;
		for(unsigned n = seg8; n; n--)
		{
			v = (sh1 ? (p[0] << sh1) | (p[1] >> sh2) : p[0]);

#define BLT_AA_MAP_ANY_1BIT_1(o) pp = &pal[(v >> (7 - (o))) & 1]; \
			b += pp[0x200] * map[(o)]; \
			g += pp[0x100] * map[(o)]; \
			r += pp[0x000] * map[(o)]; \

			BLT_AA_MAP_ANY_1BIT_1(0) BLT_AA_MAP_ANY_1BIT_1(1) BLT_AA_MAP_ANY_1BIT_1(2) BLT_AA_MAP_ANY_1BIT_1(3)
			BLT_AA_MAP_ANY_1BIT_1(4) BLT_AA_MAP_ANY_1BIT_1(5) BLT_AA_MAP_ANY_1BIT_1(6) BLT_AA_MAP_ANY_1BIT_1(7)
			map += 8;
			p++;
		}
		v = ((segment & 7) + sh1 <= 8 ? p[0] << sh1 : (p[0] << sh1) | (p[1] >> sh2));
		if(segment & 4)
		{
			BLT_AA_MAP_ANY_1BIT_1(0)
			BLT_AA_MAP_ANY_1BIT_1(1)
			BLT_AA_MAP_ANY_1BIT_1(2)
			BLT_AA_MAP_ANY_1BIT_1(3)
			map += 4;
			v <<= 4;
		}
		if(segment & 2)
		{
			BLT_AA_MAP_ANY_1BIT_1(0);
			BLT_AA_MAP_ANY_1BIT_1(1);
			map += 2;
			v <<= 2;
		}
		if(segment & 1)
		{
			BLT_AA_MAP_ANY_1BIT_1(0);
			map++;
		}
		dest[0] = b;
		dest[1] = g;
		dest[2] = r;
		dest += 3;
	}
	while(--count);
}
*/

/*
static void BltAAMap4bitAny(unsigned *dest, const byte *s, const unsigned *map, const byte *pal)
{
	AA_TIMING("BltAAMap4bitAny");
	unsigned count = map[0];
	unsigned segment = map[1];
	ASSERT(segment >= 2);
	unsigned offset = 0;
	map += 2;
	do
	{
		offset += *map++;
		unsigned b = 0, g = 0, r = 0;
		unsigned seg = segment;
		const byte *p = s + (offset >> 1);
		const byte *pp;
		if(offset & 1)
		{
			pp = &pal[*p & 15];
			b += pp[0x200] * map[0];
			g += pp[0x100] * map[0];
			r += pp[0x000] * map[0];
			map++;
			p++;
			seg--;
		}
		while(seg >= 2)
		{
			pp = &pal[*p >> 4];
			b += pp[0x200] * map[0];
			g += pp[0x100] * map[0];
			r += pp[0x000] * map[0];
			pp = &pal[*p & 15];
			b += pp[0x200] * map[1];
			g += pp[0x100] * map[1];
			r += pp[0x000] * map[1];
			map += 2;
			p++;
			seg -= 2;
		}
		if(seg & 1)
		{
			pp = &pal[*p >> 4];
			b += pp[0x200] * map[0];
			g += pp[0x100] * map[0];
			r += pp[0x000] * map[0];
			map++;
		}
		dest[0] = b;
		dest[1] = g;
		dest[2] = r;
		dest += 3;
	}
	while(--count);
}
*/

#ifdef MSC_ASSEMBLY
void BltAAAdd1I386(unsigned *dest, const unsigned *src, unsigned w, unsigned count16)
{
/*
	union
	{
		unsigned w1[2];
		__int64 w2;
	}
	init;
	init.w1[0] = init.w1[1] = w;
*/
	__asm mov edi, [dest]
	__asm mov esi, [src]
	__asm mov eax, [count16]
	__asm mov ebx, [w]
__rep16:
#define BLT_AA_ADD1_I386_2(o) \
	__asm mov ecx, [esi + 4 * (o)] \
	__asm mov edx, [esi + 4 * (o) + 4] \
	__asm imul ecx, ebx \
	__asm imul edx, ebx \
	__asm add [edi + 4 * (o)], ecx \
	__asm add [edi + 4 * (o) + 4], edx

	BLT_AA_ADD1_I386_2(0) BLT_AA_ADD1_I386_2(2) BLT_AA_ADD1_I386_2(4) BLT_AA_ADD1_I386_2(6)
	BLT_AA_ADD1_I386_2(8) BLT_AA_ADD1_I386_2(10) BLT_AA_ADD1_I386_2(12) BLT_AA_ADD1_I386_2(14)
	__asm add esi, 64
	__asm add edi, 64
	__asm dec eax
	__asm jne __rep16
/*
	__asm mov ecx, [count16]
	__asm movq mm0, [init.w2]
__rep:
	__asm movq mm1, [esi]
	__asm movq mm2, [esi + 8]
	__asm movq mm3, [esi + 16]
	__asm movq mm4, [esi + 24]
	__asm call MMXDump
	__asm pmaddwd mm1, mm0
	__asm pmaddwd mm2, mm0
	__asm pmaddwd mm3, mm0
	__asm pmaddwd mm4, mm0
	__asm call MMXDump
	__asm paddd mm1, [edi]
	__asm paddd mm2, [edi + 8]
	__asm paddd mm3, [edi + 16]
	__asm paddd mm4, [edi + 24]
	__asm call MMXDump
	__asm movq [edi], mm1
	__asm movq [edi + 8], mm2
	__asm movq [edi + 16], mm3
	__asm movq [edi + 24], mm4
	__asm movq mm1, [esi + 32]
	__asm movq mm2, [esi + 40]
	__asm movq mm3, [esi + 48]
	__asm movq mm4, [esi + 56]
	__asm call MMXDump
	__asm pmaddwd mm1, mm0
	__asm pmaddwd mm2, mm0
	__asm pmaddwd mm3, mm0
	__asm pmaddwd mm4, mm0
	__asm call MMXDump
	__asm paddd mm1, [edi + 32]
	__asm paddd mm2, [edi + 40]
	__asm paddd mm3, [edi + 48]
	__asm paddd mm4, [edi + 56]
	__asm call MMXDump
	__asm movq [edi + 32], mm1
	__asm movq [edi + 40], mm2
	__asm movq [edi + 48], mm3
	__asm movq [edi + 56], mm4
	__asm add esi, 64
	__asm add edi, 64
	__asm loop __rep
	__asm emms
*/
}
#endif

void BltAAAdd1(unsigned *dest, const unsigned *src, unsigned w, unsigned count)
{
	AA_TIMING("BltAAAdd1");

#define BLT_AA_ADD1(o) dest[(o)] += src[(o)] * w;
#define BLT_AA_ADD1_4(o) BLT_AA_ADD1((o)) BLT_AA_ADD1((o) + 1) BLT_AA_ADD1((o) + 2) BLT_AA_ADD1((o) + 3)
	if(unsigned rep = count >> 5)
	{
/*
#ifdef MSC_ASSEMBLY
//		if(MMXDetect())
		{
			BltAAAdd1I386(dest, src, w, 2 * rep);
			dest += 32 * rep;
			src += 32 * rep;
			rep = 0;
		}
#endif
*/
		for(; rep; rep--)
		{
			BLT_AA_ADD1_4(0) BLT_AA_ADD1_4(4) BLT_AA_ADD1_4(8) BLT_AA_ADD1_4(12)
			BLT_AA_ADD1_4(16) BLT_AA_ADD1_4(20) BLT_AA_ADD1_4(24) BLT_AA_ADD1_4(28)
			dest += 32;
			src += 32;
		}
	}
	if(count & 16)
	{
		BLT_AA_ADD1_4(0) BLT_AA_ADD1_4(4) BLT_AA_ADD1_4(8) BLT_AA_ADD1_4(12)
		dest += 16;
		src += 16;
	}
	if(count & 8)
	{
		BLT_AA_ADD1_4(0) BLT_AA_ADD1_4(4)
		dest += 8;
		src += 8;
	}
	if(count & 4)
	{
		BLT_AA_ADD1_4(0)
		dest += 4;
		src += 4;
	}
	if(count & 2)
	{
		BLT_AA_ADD1(0) BLT_AA_ADD1(1)
		dest += 2;
		src += 2;
	}
	if(count & 1)
	{
		BLT_AA_ADD1(0)
	}
}

void BltAAAdd2Fix(byte *dest, const unsigned *src1, unsigned w1, const unsigned *src2, unsigned w2, unsigned count)
{
	AA_TIMING("BltAAAdd2Fix");

#define BLT_AA_ADD2FIX(o) dest[(o)] = (src1[(o)] * w1 + src2[(o)] * w2) >> 16;
#define BLT_AA_ADD2FIX_4(o) BLT_AA_ADD2FIX((o)) BLT_AA_ADD2FIX((o) + 1) BLT_AA_ADD2FIX((o) + 2) BLT_AA_ADD2FIX((o) + 3)
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_AA_ADD2FIX_4(0) BLT_AA_ADD2FIX_4(4) BLT_AA_ADD2FIX_4(8) BLT_AA_ADD2FIX_4(12)
		BLT_AA_ADD2FIX_4(16) BLT_AA_ADD2FIX_4(20) BLT_AA_ADD2FIX_4(24) BLT_AA_ADD2FIX_4(28)
		dest += 32;
		src1 += 32;
		src2 += 32;
	}
	if(count & 16)
	{
		BLT_AA_ADD2FIX_4(0) BLT_AA_ADD2FIX_4(4) BLT_AA_ADD2FIX_4(8) BLT_AA_ADD2FIX_4(12)
		dest += 16;
		src1 += 16;
		src2 += 16;
	}
	if(count & 8)
	{
		BLT_AA_ADD2FIX_4(0) BLT_AA_ADD2FIX_4(4)
		dest += 8;
		src1 += 8;
		src2 += 8;
	}
	if(count & 4)
	{
		BLT_AA_ADD2FIX_4(0)
		dest += 4;
		src1 += 4;
		src2 += 4;
	}
	if(count & 2)
	{
		BLT_AA_ADD2FIX(0) BLT_AA_ADD2FIX(1)
		dest += 2;
		src1 += 2;
		src2 += 2;
	}
	if(count & 1)
	{
		BLT_AA_ADD2FIX(0)
	}
}

void BltAAAdd2(unsigned *dest, const unsigned *src1, unsigned w1, const unsigned *src2, unsigned w2, unsigned count)
{
	AA_TIMING("BltAAAdd2");

#define BLT_AA_ADD2(o) dest[(o)] += src1[(o)] * w1 + src2[(o)] * w2;
#define BLT_AA_ADD2_4(o) BLT_AA_ADD2((o)) BLT_AA_ADD2((o) + 1) BLT_AA_ADD2((o) + 2) BLT_AA_ADD2((o) + 3)
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_AA_ADD2_4(0) BLT_AA_ADD2_4(4) BLT_AA_ADD2_4(8) BLT_AA_ADD2_4(12)
		BLT_AA_ADD2_4(16) BLT_AA_ADD2_4(20) BLT_AA_ADD2_4(24) BLT_AA_ADD2_4(28)
		dest += 32;
		src1 += 32;
		src2 += 32;
	}
	if(count & 16)
	{
		BLT_AA_ADD2_4(0) BLT_AA_ADD2_4(4) BLT_AA_ADD2_4(8) BLT_AA_ADD2_4(12)
		dest += 16;
		src1 += 16;
		src2 += 16;
	}
	if(count & 8)
	{
		BLT_AA_ADD2_4(0) BLT_AA_ADD2_4(4)
		dest += 8;
		src1 += 8;
		src2 += 8;
	}
	if(count & 4)
	{
		BLT_AA_ADD2_4(0)
		dest += 4;
		src1 += 4;
		src2 += 4;
	}
	if(count & 2)
	{
		BLT_AA_ADD2(0) BLT_AA_ADD2(1)
		dest += 2;
		src1 += 2;
		src2 += 2;
	}
	if(count & 1)
	{
		BLT_AA_ADD2(0)
	}
}

void BltAASet2(unsigned *dest, const unsigned *src1, unsigned w1, const unsigned *src2, unsigned w2, unsigned count)
{
	AA_TIMING("BltAASet2");

#define BLT_AA_SET2(o) dest[(o)] = src1[(o)] * w1 + src2[(o)] * w2;
#define BLT_AA_SET2_4(o) BLT_AA_SET2((o)) BLT_AA_SET2((o) + 1) BLT_AA_SET2((o) + 2) BLT_AA_SET2((o) + 3)
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_AA_SET2_4(0) BLT_AA_SET2_4(4) BLT_AA_SET2_4(8) BLT_AA_SET2_4(12)
		BLT_AA_SET2_4(16) BLT_AA_SET2_4(20) BLT_AA_SET2_4(24) BLT_AA_SET2_4(28)
		dest += 32;
		src1 += 32;
		src2 += 32;
	}
	if(count & 16)
	{
		BLT_AA_SET2_4(0) BLT_AA_SET2_4(4) BLT_AA_SET2_4(8) BLT_AA_SET2_4(12)
		dest += 16;
		src1 += 16;
		src2 += 16;
	}
	if(count & 8)
	{
		BLT_AA_SET2_4(0) BLT_AA_SET2_4(4)
		dest += 8;
		src1 += 8;
		src2 += 8;
	}
	if(count & 4)
	{
		BLT_AA_SET2_4(0)
		dest += 4;
		src1 += 4;
		src2 += 4;
	}
	if(count & 2)
	{
		BLT_AA_SET2(0) BLT_AA_SET2(1)
		dest += 2;
		src1 += 2;
		src2 += 2;
	}
	if(count & 1)
	{
		BLT_AA_SET2(0)
	}
}

void BltAASet3(unsigned *dest, const unsigned *src1, unsigned w1, const unsigned *src2, unsigned w2, const unsigned *src3, unsigned w3, unsigned count)
{
	AA_TIMING("BltAASet3");

#define BLT_AA_SET3(o) dest[(o)] = src1[(o)] * w1 + src2[(o)] * w2 + src3[(o)] * w3;
#define BLT_AA_SET3_4(o) BLT_AA_SET3((o)) BLT_AA_SET3((o) + 1) BLT_AA_SET3((o) + 2) BLT_AA_SET3((o) + 3)
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_AA_SET3_4(0) BLT_AA_SET3_4(4) BLT_AA_SET3_4(8) BLT_AA_SET3_4(12)
		BLT_AA_SET3_4(16) BLT_AA_SET3_4(20) BLT_AA_SET3_4(24) BLT_AA_SET3_4(28)
		dest += 32;
		src1 += 32;
		src2 += 32;
		src3 += 32;
	}
	if(count & 16)
	{
		BLT_AA_SET3_4(0) BLT_AA_SET3_4(4) BLT_AA_SET3_4(8) BLT_AA_SET3_4(12)
		dest += 16;
		src1 += 16;
		src2 += 16;
		src3 += 16;
	}
	if(count & 8)
	{
		BLT_AA_SET3_4(0) BLT_AA_SET3_4(4)
		dest += 8;
		src1 += 8;
		src2 += 8;
		src3 += 8;
	}
	if(count & 4)
	{
		BLT_AA_SET3_4(0)
		dest += 4;
		src1 += 4;
		src2 += 4;
		src3 += 4;
	}
	if(count & 2)
	{
		BLT_AA_SET3(0) BLT_AA_SET3(1)
		dest += 2;
		src1 += 2;
		src2 += 2;
		src3 += 2;
	}
	if(count & 1)
	{
		BLT_AA_SET3(0)
	}
}

void BltAASet4(unsigned *dest, const unsigned *src1, const unsigned *src2,
	const unsigned *src3, const unsigned *src4, unsigned count)
{
	AA_TIMING("BltAASet4");

#define BLT_AA_SET4(o) dest[(o)] = (src1[(o)] + src2[(o)] + src3[(o)] + src4[(o)]) << 6;
#define BLT_AA_SET4_4(o) BLT_AA_SET4((o)) BLT_AA_SET4((o) + 1) BLT_AA_SET4((o) + 2) BLT_AA_SET4((o) + 3)
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_AA_SET4_4(0) BLT_AA_SET4_4(4) BLT_AA_SET4_4(8) BLT_AA_SET4_4(12)
		BLT_AA_SET4_4(16) BLT_AA_SET4_4(20) BLT_AA_SET4_4(24) BLT_AA_SET4_4(28)
		dest += 32; src1 += 32; src2 += 32; src3 += 32; src4 += 32;
	}
	if(count & 16)
	{
		BLT_AA_SET4_4(0) BLT_AA_SET4_4(4) BLT_AA_SET4_4(8) BLT_AA_SET4_4(12)
		dest += 16; src1 += 16; src2 += 16; src3 += 16; src4 += 16;
	}
	if(count & 8)
	{
		BLT_AA_SET4_4(0) BLT_AA_SET4_4(4)
		dest += 8; src1 += 8; src2 += 8; src3 += 8; src4 += 8;
	}
	if(count & 4)
	{
		BLT_AA_SET4_4(0)
		dest += 4; src1 += 4; src2 += 4; src3 += 4; src4 += 4;
	}
	if(count & 2)
	{
		BLT_AA_SET4(0) BLT_AA_SET4(1)
		dest += 2; src1 += 2; src2 += 2; src3 += 2; src4 += 2;
	}
	if(count & 1)
	{
		BLT_AA_SET4(0)
	}
}

void BltAAFix2(byte *dest, const unsigned *src, unsigned count)
{
	AA_TIMING("BltAAFix2");

#define BLT_AA_FIX2_2(o) \
	a = ((const byte *)src)[4 * (o) + OFF1]; \
	b = ((const byte *)src)[4 * (o) + OFF1 + 4]; \
	dest[(o) + 0] = a; \
	dest[(o) + 1] = b;
#define BLT_AA_FIX2_8(o) BLT_AA_FIX2_2((o)) BLT_AA_FIX2_2((o) + 2) BLT_AA_FIX2_2((o) + 4) BLT_AA_FIX2_2((o) + 6)

	byte a, b;
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_AA_FIX2_8(0) BLT_AA_FIX2_8(8) BLT_AA_FIX2_8(16) BLT_AA_FIX2_8(24)
		dest += 32;
		src += 32;
	}
	if(count & 16)
	{
		BLT_AA_FIX2_8(0) BLT_AA_FIX2_8(8)
		dest += 16;
		src += 16;
	}
	if(count & 8)
	{
		BLT_AA_FIX2_8(0)
		dest += 8;
		src += 8;
	}
	if(count & 4)
	{
		BLT_AA_FIX2_2(0) BLT_AA_FIX2_2(2)
		dest += 4;
		src += 4;
	}
	if(count & 2)
	{
		BLT_AA_FIX2_2(0)
		dest += 2;
		src += 2;
	}
	if(count & 1)
		dest[0] = ((const byte *)src)[OFF1];
}

void BltAAFix3(byte *out, const unsigned *src, unsigned count)
{
	AA_TIMING("BltAAFix3");

#define BLT_FIX_2(o) a = ((const byte *)src)[4 * (o) + OFF2]; b = ((const byte *)src)[4 * (o) + OFF2 + 4]; out[(o) + 0] = a; out[(o) + 1] = b;
#define BLT_FIX_8(o) BLT_FIX_2((o) + 0) BLT_FIX_2((o) + 2) BLT_FIX_2((o) + 4) BLT_FIX_2((o) + 6)
	byte a, b;
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_FIX_8(0)
		BLT_FIX_8(8)
		BLT_FIX_8(16)
		BLT_FIX_8(24)
		src += 32;
		out += 32;
	}
	if(count & 16)
	{
		BLT_FIX_8(0)
		BLT_FIX_8(8)
		src += 16;
		out += 16;
	}
	if(count & 8)
	{
		BLT_FIX_8(0)
		src += 8;
		out += 8;
	}
	if(count & 4)
	{
		BLT_FIX_2(0) BLT_FIX_2(2)
		src += 4;
		out += 4;
	}
	if(count & 2)
	{
		BLT_FIX_2(0)
		src += 2;
		out += 2;
	}
	if(count & 1)
		out[0] = ((const byte *)src)[OFF2];
}

#define BLT_AA_MAP_MASK_REP4(BPROC, o) \
	BPROC((o) + 0) \
	BPROC((o) + 1) \
	BPROC((o) + 2) \
	BPROC((o) + 3)

#define BLT_AA_MAP_MASK_PROC(SUFFIX, SCOUNT, BPROC) \
static void COMBINE(BltAAMapMask, SUFFIX)(unsigned *dest, const byte *s, const unsigned *map, byte xor1) \
{ \
	AA_TIMING("BltAAMapMask" #SUFFIX); \
	unsigned count = map[0]; \
	int step = map[3]; \
	unsigned offset = 0; \
	map += 4; \
	enum { STEP = SCOUNT < MAXAA ? SCOUNT + 1 : 1 }; \
	\
	for(unsigned rep = count >> 4; rep; rep--) \
	{ \
		BLT_AA_MAP_MASK_REP4(BPROC, 0) BLT_AA_MAP_MASK_REP4(BPROC, 4) \
		BLT_AA_MAP_MASK_REP4(BPROC, 8) BLT_AA_MAP_MASK_REP4(BPROC, 12) \
		dest += 16; \
		map += STEP * 16; \
	} \
	if(count & 8) \
	{ \
		BLT_AA_MAP_MASK_REP4(BPROC, 0) BLT_AA_MAP_MASK_REP4(BPROC, 4) \
		dest += 8; \
		map += STEP * 8; \
	} \
	if(count & 4) \
	{ \
		BLT_AA_MAP_MASK_REP4(BPROC, 0) \
		dest += 4; \
		map += STEP * 4; \
	} \
	if(count & 2) \
	{ \
		BPROC(0) \
		BPROC(1) \
		dest += 2; \
		map += STEP * 2; \
	} \
	if(count & 1) \
	{ \
		BPROC(0) \
	} \
}

#define BLT_AA_MAP_MASK_BYTE_1(o) s += map[2 * (o)]; dest[(o)] = (-(s[0] ^ xor1) & 0x100);
#define BLT_AA_MAP_MASK_BYTE_2(o) s += map[3 * (o)]; dest[(o)] = (-(s[0] ^ xor1) & map[3 * (o) + 1]) + (-(s[1] ^ xor1) & map[3 * (o) + 2]);
#define BLT_AA_MAP_MASK_BYTE_3(o) s += map[4 * (o)]; dest[(o)] = (-(s[0] ^ xor1) & map[4 * (o) + 1]) + (-(s[1] ^ xor1) & map[4 * (o) + 2]) + (-(s[2] ^ xor1) & map[4 * (o) + 3]);
#define BLT_AA_MAP_MASK_BYTE_4(o) s += map[(o)]; dest[(o)] = ((s[0] ^ xor1) + (s[1 * step] ^ xor1) + (s[2 * step] ^ xor1) + (s[3 * step] ^ xor1)) << 6;

BLT_AA_MAP_MASK_PROC(Byte1, 1, BLT_AA_MAP_MASK_BYTE_1)
BLT_AA_MAP_MASK_PROC(Byte2, 2, BLT_AA_MAP_MASK_BYTE_2)
BLT_AA_MAP_MASK_PROC(Byte3, 3, BLT_AA_MAP_MASK_BYTE_3)
BLT_AA_MAP_MASK_PROC(Byte4, 4, BLT_AA_MAP_MASK_BYTE_4)

#define BLT_AA_MAP_MASK_BIT_1(o) offset += map[2 * (o) + 0]; \
	{ \
		const byte *p = s + (offset >> 3); \
		dest[3 * (o) + 0] = dest[3 * (o) + 1] = dest[3 * (o) + 2] = (*p ^ xor1) << ((offset & 7) + 1) & 0x100; \
	}

#define BLT_AA_MAP_MASK_BIT_2(o) offset += map[3 * (o) + 0]; \
	{ \
		const byte *p = s + (offset >> 3); \
		int s = ~offset & 7; \
		dest[(o)] = (map[3 * (o) + 1] & -(((*p ^ xor1) >> s) & 1)) \
		+ (map[3 * (o) + 2] & -(s ? ((*p ^ xor1) >> (s - 1)) & 1 : (p[1] ^ xor1) >> 7)); \
	}

#define BLT_AA_MAP_MASK_BIT_3(o) offset += map[4 * (o) + 0]; \
	{ \
		const byte *p = s + (offset >> 3); \
		int sh = (offset & 7) - 5; \
		byte v = (sh <= 0 ? p[0] >> -sh : (p[0] << sh) | (p[1] >> (8 - sh))) ^ xor1; \
		dest[(o)] = (map[4 * (o) + 1] & -((v >> 2) & 1)) \
		+ (map[4 * (o) + 2] & -((v >> 1) & 1)) + (map[4 * (o) + 3] & -((v >> 0) & 1)); \
	}

#define BLT_AA_MAP_MASK_BIT_4(o) offset += map[(o)]; \
	{ \
		int d = offset; \
		byte v = byte((s[d >> 3] ^ xor1) << (d & 7)) >> 7; d += step; \
		v     += byte((s[d >> 3] ^ xor1) << (d & 7)) >> 7; d += step; \
		v     += byte((s[d >> 3] ^ xor1) << (d & 7)) >> 7; d += step; \
		v     += byte((s[d >> 3] ^ xor1) << (d & 7)) >> 7; \
		dest[(o)] = v << 6; \
	}

BLT_AA_MAP_MASK_PROC(Bit1, 1, BLT_AA_MAP_MASK_BIT_1)
BLT_AA_MAP_MASK_PROC(Bit2, 2, BLT_AA_MAP_MASK_BIT_2)
BLT_AA_MAP_MASK_PROC(Bit3, 3, BLT_AA_MAP_MASK_BIT_3)
BLT_AA_MAP_MASK_PROC(Bit4, 4, BLT_AA_MAP_MASK_BIT_4)

/*
static void BltAAMapMaskByteAny(unsigned *dest, const byte *src, const unsigned *map, byte xor1)
{
	AA_TIMING("BltAAMapMaskAny");
	unsigned count = map[0];
	unsigned segment = map[1];
	unsigned seg4 = segment >> 2;
	map += 2;
	do
	{
		const byte *s = src += *map++;
		unsigned m = 0;
		for(unsigned n = seg4; n; n--)
		{
			m += ((s[0] - 1) & map[0]) + ((s[1] - 1) & map[1]) + ((s[2] - 1) & map[2]) + ((s[3] - 1) & map[3]);
			s += 4;
			map += 4;
		}
		if(segment & 2)
		{
			m += ((s[0] - 1) & map[0]) + ((s[1] - 1) & map[1]);
			s += 2;
			map += 2;
		}
		if(segment & 1)
		{
			m += ((s[0] - 1) & map[0]);
			map++;
		}
		*dest++ = m;
	}
	while(--count);
}
*/

/*
static void BltAAMapMaskBitAny(unsigned *dest, const byte *src, const unsigned *map, byte xor1)
{
	AA_TIMING("BltAAMapBitAny");
	unsigned count = map[0];
	unsigned segment = map[1];
	unsigned seg8 = segment >> 3;
	unsigned offset = 0;
	map += 2;
	do
	{
		offset += *map++;
		int sh1 = offset & 7, sh2 = 8 - sh1;
		const byte *p = src + (offset >> 3);
		unsigned m = 0;
		byte v;
		for(unsigned n = seg8; n; n--)
		{
			v = (sh1 ? (p[0] << sh1) | (p[1] >> sh2) : p[0]) ^ xor1;
			if(v & 0x80) m += map[0]; if(v & 0x40) m += map[1];
			if(v & 0x20) m += map[2]; if(v & 0x10) m += map[3];
			if(v & 0x08) m += map[4]; if(v & 0x04) m += map[5];
			if(v & 0x02) m += map[6]; if(v & 0x01) m += map[7];
			map += 8;
			p++;
		}
		v = ((segment & 7) + sh1 <= 8 ? p[0] << sh1 : (p[0] << sh1) | (p[1] >> sh2)) ^ xor1;
		if(segment & 4)
		{
			if(v & 0x80) m += map[0];
			if(v & 0x40) m += map[1];
			if(v & 0x20) m += map[2];
			if(v & 0x10) m += map[3];
			map += 4;
			v <<= 4;
		}
		if(segment & 2)
		{
			if(v & 0x80) m += map[0];
			if(v & 0x40) m += map[1];
			map += 2;
			v <<= 2;
		}
		if(segment & 1)
		{
			if(v & 0x80) m += map[0];
			map++;
		}
		*dest++ = m;
	}
	while(--count);
}
*/

#define BLT_AA_FIX_MASK_PROC(BPROC, SPROC) \
	for(unsigned rep = count >> 4; rep; rep--) \
	{ \
		BPROC(0) BPROC(1) BPROC(2) BPROC(3) BPROC(4) BPROC(5) BPROC(6) BPROC(7) \
		BPROC(8) BPROC(9) BPROC(10) BPROC(11) BPROC(12) BPROC(13) BPROC(14) BPROC(15) \
		SPROC(16) \
	} \
	if(count & 8) \
	{ \
		BPROC(0) BPROC(1) BPROC(2) BPROC(3) BPROC(4) BPROC(5) BPROC(6) BPROC(7) \
		SPROC(8) \
	} \
	if(count & 4) \
	{ \
		BPROC(0) BPROC(1) BPROC(2) BPROC(3) \
		SPROC(4) \
	} \
	if(count & 2) \
	{ \
		BPROC(0) BPROC(1) \
		SPROC(2) \
	} \
	if(count & 1) \
	{ \
		BPROC(0) \
	} \

#define BLT_AA_FIX_MASK_ROW(o) \
	if(((const byte *)&weights[(o)])[OFF1]) \
	{ \
		dest[3 * (o) + 0] = ((const byte *)&pixels[3 * (o) + 0])[OFF1]; \
		dest[3 * (o) + 1] = ((const byte *)&pixels[3 * (o) + 1])[OFF1]; \
		dest[3 * (o) + 2] = ((const byte *)&pixels[3 * (o) + 2])[OFF1]; \
	} \
	else if(((const byte *)&weights[(o)])[OFF0] & 0x80) \
	{ \
		unsigned w = weights[(o)]; \
		dest[3 * (o) + 0] = pixels[3 * (o) + 0] / w; \
		dest[3 * (o) + 1] = pixels[3 * (o) + 1] / w; \
		dest[3 * (o) + 2] = pixels[3 * (o) + 2] / w; \
	} \

#define BLT_AA_FIX_MASK_ANY(o) \
	if(((const byte *)&weights[(o)])[OFF2]) \
	{ \
		dest[3 * (o) + 0] = ((const byte *)&pixels[3 * (o) + 0])[OFF2]; \
		dest[3 * (o) + 1] = ((const byte *)&pixels[3 * (o) + 1])[OFF2]; \
		dest[3 * (o) + 2] = ((const byte *)&pixels[3 * (o) + 2])[OFF2]; \
	} \
	else if(((const byte *)&weights[(o)])[OFF1] & 0x80) \
	{ \
		unsigned w = weights[(o)]; \
		dest[3 * (o) + 0] = pixels[3 * (o) + 0] / w; \
		dest[3 * (o) + 1] = pixels[3 * (o) + 1] / w; \
		dest[3 * (o) + 2] = pixels[3 * (o) + 2] / w; \
	} \

#define BLT_AA_FIX_MASK_SHIFT(c) \
	dest += (c) * 3; \
	pixels += (c) * 3; \
	weights += (c); \

void BltAAFixMaskRow(byte *dest, const unsigned *pixels, const unsigned *weights, unsigned count)
{
	AA_TIMING("BltAAFixMaskRow");
	BLT_AA_FIX_MASK_PROC(BLT_AA_FIX_MASK_ROW, BLT_AA_FIX_MASK_SHIFT)
}

void BltAAFixMaskAny(byte *dest, const unsigned *pixels, const unsigned *weights, unsigned count)
{
	AA_TIMING("BltAAFixMaskAny");
	BLT_AA_FIX_MASK_PROC(BLT_AA_FIX_MASK_ANY, BLT_AA_FIX_MASK_SHIFT)
}

#define BLT_AA_FIX_MASK_OUT_ROW(o) \
	if(((const byte *)&weights[(o)])[OFF1]) \
	{ \
		dpixel[3 * (o) + 0] = ((const byte *)&pixels[3 * (o) + 0])[OFF1]; \
		dpixel[3 * (o) + 1] = ((const byte *)&pixels[3 * (o) + 1])[OFF1]; \
		dpixel[3 * (o) + 2] = ((const byte *)&pixels[3 * (o) + 2])[OFF1]; \
		dmask[(o)] = 0; \
	} \
	else if(((const byte *)&weights[(o)])[OFF0] & 0x80) \
	{ \
		unsigned w = weights[(o)]; \
		dpixel[3 * (o) + 0] = pixels[3 * (o) + 0] / w; \
		dpixel[3 * (o) + 1] = pixels[3 * (o) + 1] / w; \
		dpixel[3 * (o) + 2] = pixels[3 * (o) + 2] / w; \
		dmask[(o)] = 0; \
	} \
	else \
	{ \
		dpixel[3 * (o) + 0] = dpixel[3 * (o) + 1] = dpixel[3 * (o) + 2] = 0; \
		dmask[(o)] = 1; \
	} \

#define BLT_AA_FIX_MASK_OUT_ANY(o) \
	if(((const byte *)&weights[(o)])[OFF2]) \
	{ \
		dpixel[3 * (o) + 0] = ((const byte *)&pixels[3 * (o) + 0])[OFF2]; \
		dpixel[3 * (o) + 1] = ((const byte *)&pixels[3 * (o) + 1])[OFF2]; \
		dpixel[3 * (o) + 2] = ((const byte *)&pixels[3 * (o) + 2])[OFF2]; \
		dmask[(o)] = 0; \
	} \
	else if(((const byte *)&weights[(o)])[OFF1] & 0x80) \
	{ \
		unsigned w = weights[(o)]; \
		dpixel[3 * (o) + 0] = pixels[3 * (o) + 0] / w; \
		dpixel[3 * (o) + 1] = pixels[3 * (o) + 1] / w; \
		dpixel[3 * (o) + 2] = pixels[3 * (o) + 2] / w; \
		dmask[(o)] = 0; \
	} \
	else \
	{ \
		dpixel[3 * (o) + 0] = dpixel[3 * (o) + 1] = dpixel[3 * (o) + 2] = 0; \
		dmask[(o)] = 1; \
	} \

#define BLT_AA_FIX_MASK_OUT_SHIFT(c) \
	dpixel += (c) * 3; \
	dmask += (c); \
	pixels += (c) * 3; \
	weights += (c); \

void BltAAFixMaskOutRow(byte *dpixel, byte *dmask, const unsigned *pixels, const unsigned *weights, unsigned count)
{
	AA_TIMING("BltAAFixMaskOutRow");
//	__asm int 3
	BLT_AA_FIX_MASK_PROC(BLT_AA_FIX_MASK_OUT_ROW, BLT_AA_FIX_MASK_OUT_SHIFT)
}

void BltAAFixMaskOutAny(byte *dpixel, byte *dmask, const unsigned *pixels, const unsigned *weights, unsigned count)
{
	AA_TIMING("BltAAFixMaskOutAny");
//	__asm int 3
	BLT_AA_FIX_MASK_PROC(BLT_AA_FIX_MASK_OUT_ANY, BLT_AA_FIX_MASK_OUT_SHIFT)
}

#define BLT_AA_FIX_MASK_OUT_MERGE_ROW(o) \
	if(((const byte *)&weights[(o)])[OFF1]) \
	{ \
		dpixel[3 * (o) + 0] = ((const byte *)&pixels[3 * (o) + 0])[OFF1]; \
		dpixel[3 * (o) + 1] = ((const byte *)&pixels[3 * (o) + 1])[OFF1]; \
		dpixel[3 * (o) + 2] = ((const byte *)&pixels[3 * (o) + 2])[OFF1]; \
		dmask[(o)] = 0; \
	} \
	else if(((const byte *)&weights[(o)])[OFF0] & 0x80) \
	{ \
		unsigned w = weights[(o)]; \
		dpixel[3 * (o) + 0] = pixels[3 * (o) + 0] / w; \
		dpixel[3 * (o) + 1] = pixels[3 * (o) + 1] / w; \
		dpixel[3 * (o) + 2] = pixels[3 * (o) + 2] / w; \
		dmask[(o)] = 0; \
	} \
//	else \
//	{ \
//		dpixel[3 * (o) + 0] = dpixel[3 * (o) + 1] = dpixel[3 * (o) + 2] = 0; \
//		dmask[(o)] = 1; \
//	} \

#define BLT_AA_FIX_MASK_OUT_MERGE_ANY(o) \
	if(((const byte *)&weights[(o)])[OFF2]) \
	{ \
		dpixel[3 * (o) + 0] = ((const byte *)&pixels[3 * (o) + 0])[OFF2]; \
		dpixel[3 * (o) + 1] = ((const byte *)&pixels[3 * (o) + 1])[OFF2]; \
		dpixel[3 * (o) + 2] = ((const byte *)&pixels[3 * (o) + 2])[OFF2]; \
		dmask[(o)] = 0; \
	} \
	else if(((const byte *)&weights[(o)])[OFF1] & 0x80) \
	{ \
		unsigned w = weights[(o)]; \
		dpixel[3 * (o) + 0] = pixels[3 * (o) + 0] / w; \
		dpixel[3 * (o) + 1] = pixels[3 * (o) + 1] / w; \
		dpixel[3 * (o) + 2] = pixels[3 * (o) + 2] / w; \
		dmask[(o)] = 0; \
	} \

void BltAAFixMaskOutMergeRow(byte *dpixel, byte *dmask, const unsigned *pixels, const unsigned *weights, unsigned count)
{
	AA_TIMING("BltAAFixMaskOutMergeRow");
	BLT_AA_FIX_MASK_PROC(BLT_AA_FIX_MASK_OUT_MERGE_ROW, BLT_AA_FIX_MASK_OUT_SHIFT)
}

void BltAAFixMaskOutMergeAny(byte *dpixel, byte *dmask, const unsigned *pixels, const unsigned *weights, unsigned count)
{
	AA_TIMING("BltAAFixMaskOutMergeAny");
	BLT_AA_FIX_MASK_PROC(BLT_AA_FIX_MASK_OUT_MERGE_ANY, BLT_AA_FIX_MASK_OUT_SHIFT)
}

#define BLT_AA_FIX_MASK_ONLY_ROW(o) dmask[(o)] = (weights[(o)] < 0x80);
#define BLT_AA_FIX_MASK_ONLY_ANY(o) dmask[(o)] = (weights[(o)] < 0x8000);
#define BLT_AA_FIX_MASK_ONLY_MERGE_ROW(o) if(weights[(o)] >= 0x80) dmask[(o)] = 0;
#define BLT_AA_FIX_MASK_ONLY_MERGE_ANY(o) if(weights[(o)] >= 0x8000) dmask[(o)] = 0;
#define BLT_AA_FIX_MASK_ONLY_SHIFT(c) dmask += (c); weights += (c); \

void BltAAFixMaskOnlyRow(byte *dmask, const unsigned *weights, unsigned count)
{
	AA_TIMING("BltAAFixMaskOnlyRow");
	BLT_AA_FIX_MASK_PROC(BLT_AA_FIX_MASK_ONLY_ROW, BLT_AA_FIX_MASK_ONLY_SHIFT)
}

void BltAAFixMaskOnlyAny(byte *dmask, const unsigned *weights, unsigned count)
{
	AA_TIMING("BltAAFixMaskOnlyAny");
	BLT_AA_FIX_MASK_PROC(BLT_AA_FIX_MASK_ONLY_ANY, BLT_AA_FIX_MASK_ONLY_SHIFT)
}

void BltAAFixMaskOnlyMergeRow(byte *dmask, const unsigned *weights, unsigned count)
{
	AA_TIMING("BltAAFixMaskOnlyMergeRow");
	BLT_AA_FIX_MASK_PROC(BLT_AA_FIX_MASK_ONLY_MERGE_ROW, BLT_AA_FIX_MASK_ONLY_SHIFT)
}

void BltAAFixMaskOnlyMergeAny(byte *dmask, const unsigned *weights, unsigned count)
{
	AA_TIMING("BltAAFixMaskOnlyMergeAny");
	BLT_AA_FIX_MASK_PROC(BLT_AA_FIX_MASK_ONLY_MERGE_ANY, BLT_AA_FIX_MASK_ONLY_SHIFT)
}

static void CvtPalette(byte *dest, const Vector<Color>& src)
{
	memset(dest, 0, 0x300);
	for(int i = 0, n = min(src.GetCount(), 256); i < n; i++)
	{
		Color c = Nvl(src[i], Color(0, 0, 0));
		dest[i + 0x000] = c.GetR();
		dest[i + 0x100] = c.GetG();
		dest[i + 0x200] = c.GetB();
	}
}

enum SOURCE_FORMAT { SRC0, SRC1, SRC4, SRC8, SRC16, SRC24, SRC32 };

static void (*GetPixelProc(int rep, SOURCE_FORMAT sfmt))(unsigned *dest, const byte *src, const unsigned *map, const byte *pal)
{
	switch(sfmt)
	{
	case SRC32:
		switch(rep)
		{
		case 1:  return BltAAMap32bit1;
		case 2:  return BltAAMap32bit2;
		case 3:  return BltAAMap32bit3;
		default: NEVER();
		case 4:  return BltAAMap32bit4;
		}

	case SRC24:
		switch(rep)
		{
		case 1:  return BltAAMap24bit1;
		case 2:  return BltAAMap24bit2;
		case 3:  return BltAAMap24bit3;
		default: NEVER();
		case 4:  return BltAAMap24bit4;
		}

	case SRC16:
		switch(rep)
		{
		case 1:  return BltAAMap16bit1;
		case 2:  return BltAAMap16bit2;
		case 3:  return BltAAMap16bit3;
		default: NEVER();
		case 4:  return BltAAMap16bit4;
		}

	case SRC8:
		switch(rep)
		{
		case 1:  return BltAAMap8bit1;
		case 2:  return BltAAMap8bit2;
		case 3:  return BltAAMap8bit3;
		default: NEVER();
		case 4:  return BltAAMap8bit4;
		}

	case SRC4:
		switch(rep)
		{
		case 1:  return BltAAMap4bit1;
		case 2:  return BltAAMap4bit2;
		case 3:  return BltAAMap4bit3;
		default: NEVER();
		case 4:  return BltAAMap4bit4;
		}

	case SRC1:
		switch(rep)
		{
		case 1:  return BltAAMap1bit1;
		case 2:  return BltAAMap1bit2;
		case 3:  return BltAAMap1bit3;
		default: NEVER();
		case 4:  return BltAAMap1bit4;
		}

	default:
		NEVER();
		return 0;
	}
}

static void (*GetMaskProc(int rep, SOURCE_FORMAT sfmt))(unsigned *dest, const byte *src, const unsigned *map, byte xor1)
{
	if(sfmt == SRC1)
		switch(rep)
		{
		case 1:  return BltAAMapMaskBit1;
		case 2:  return BltAAMapMaskBit2;
		case 3:  return BltAAMapMaskBit3;
		default: NEVER();
		case 4:  return BltAAMapMaskBit4;
		}
	else if(sfmt == SRC8)
		switch(rep)
		{
		case 1:  return BltAAMapMaskByte1;
		case 2:  return BltAAMapMaskByte2;
		case 3:  return BltAAMapMaskByte3;
		default: NEVER();
		case 4:  return BltAAMapMaskByte4;
		}
	else
	{
		NEVER();
		return NULL;
	}
}

void PixelCopyAntiAlias(PixelArray& dest, Rect dest_rc, const PixelArray& src, Rect src_rc, Rect clip)
{
	AA_TIMING("PixelCopyAntiAlias (new)");

	if(dest.IsEmpty())
		return;

	ASSERT(dest.GetRawBPP() == -3);

	Rect cl = dest.GetRect();
	if(!IsNull(clip))
		cl &= clip;

	if(cl.IsEmpty())
		return;

	if(src.IsMono() && (IsNull(src.palette[0]) || IsNull(src.palette[1])))
	{
		if(IsNull(src.palette[0]) && IsNull(src.palette[1]))
			return;
		PixelArray temp = PixelArray::Mono(dest.GetSize(), 8);
		PixelSet(temp, temp.GetRect(), 1);
		PixelCopyAntiAliasMaskOnly(temp, dest_rc, src, src_rc, false, !IsNull(src.palette[1]), clip);
		PixelKillMask(dest, temp, Nvl(src.palette[0], src.palette[1]), true);
		return;
	}

//	LOG("CopyAntiAlias: " << dest_rc << " <- " << src_rc);
	SOURCE_FORMAT sfmt = (src.bpp == 32 ? SRC32 : src.bpp == 24 ? SRC24
		: src.bpp == 16 ? SRC16 : src.bpp == 8 ? SRC8 : src.bpp == 4 ? SRC4 : src.bpp == 1 ? SRC1 : SRC0);
	ASSERT(sfmt != SRC0);

	Rect dr = dest_rc;
	Vector<unsigned> row = AAGetMap(dr.left, dr.right, cl.left, cl.right, src_rc.left, src_rc.right, 0, src.size.cx,
		sfmt == SRC32 ? 4 : sfmt == SRC24 ? 3 : sfmt == SRC16 ? 2 : 1, 0x100);
	if(row.IsEmpty())
		return;
	Vector<unsigned> col = AAGetMap(dr.top, dr.bottom, cl.top, cl.bottom, src_rc.top, src_rc.bottom, 0, src.size.cy, 1, 0x100);
	if(col.IsEmpty())
		return;
	byte pal[768];
	if(sfmt == SRC32 || sfmt == SRC16) {
		PokeIL(pal + 0, src.cmask[2]);
		PokeIL(pal + 4, src.cmask[1]);
		PokeIL(pal + 8, src.cmask[0]);
		pal[12] = src.cshift24[2];
		pal[13] = src.cshift24[1];
		pal[14] = src.cshift24[0];
	}
	else if(sfmt != SRC24)
		CvtPalette(pal, src.palette);
	void (*row_proc)(unsigned *dest, const byte *src, const unsigned *map, const byte *pal) = GetPixelProc(row[1], sfmt);
	int wd = dr.Width(), w3 = 3 * wd, dsh = 3 * dr.left;
	Buffer<unsigned> sum_buffer(w3);
	int count = col[MAP_COUNT];
	int segment = col[MAP_SEGMENT];
	int entry = col[MAP_BLOCK];
	int step = col[MAP_STEP];
	int segspan = (segment - 1) * step + 1;
	bool bigseg = (segment == MAXAA);
	Buffer<unsigned> row_buffers(w3 * segment);
	int first = col[4], full = 0;
	const unsigned *offsets = col.GetIter(4);
	int offset = 0;
	for(int y = dr.top; --count >= 0; y++)
	{
		offset += *offsets++;
		ASSERT(offset >= 0 && offset + segspan <= src.size.cy);
		byte *dp = dest.GetDownScan(y) + dsh;
		if(bigseg)
		{
			row_proc(&row_buffers[0 * w3], src.GetDownScan(offset + 0 * step), row.Begin(), pal);
			row_proc(&row_buffers[1 * w3], src.GetDownScan(offset + 1 * step), row.Begin(), pal);
			row_proc(&row_buffers[2 * w3], src.GetDownScan(offset + 2 * step), row.Begin(), pal);
			row_proc(&row_buffers[3 * w3], src.GetDownScan(offset + 3 * step), row.Begin(), pal);
			BltAASet4(sum_buffer, &row_buffers[0 * w3], &row_buffers[1 * w3],
				&row_buffers[2 * w3], &row_buffers[3 * w3], w3);
			BltAAFix3(dp, sum_buffer, w3);
		}
		else
		{
			int endoff = offset + segment;
			for(int next = first + full; next < endoff; next++)
			{
				if(full >= segment)
					first++;
				else
					full++;
				row_proc(&row_buffers[next % segment * w3], src.GetDownScan(next), row.Begin(), pal);
			}
			while(first > offset)
			{
				if(full < segment)
					full++;
				--first;
				row_proc(&row_buffers[first % segment * w3], src.GetDownScan(first), row.Begin(), pal);
			}
			ASSERT(offset >= first && endoff <= first + full);
			switch(segment)
			{
			case 1:
				BltAAFix2(dp, &row_buffers[offset % segment * w3], w3);
				offsets++;
				break;

			case 2:
				if(offsets[0] == 0)
					BltAAFix2(dp, &row_buffers[(offset + 1) % segment * w3], w3);
				else if(offsets[1] == 0)
					BltAAFix2(dp, &row_buffers[offset % segment * w3], w3);
				else
					BltAAAdd2Fix(dp, &row_buffers[(offset + 0) % segment * w3], offsets[0],
						&row_buffers[(offset + 1) % segment * w3], offsets[1], w3);
				offsets += 2;
				break;

			case 3:
				BltAASet3(sum_buffer,
					&row_buffers[(offset + 0) % segment * w3], offsets[0],
					&row_buffers[(offset + 1) % segment * w3], offsets[1],
					&row_buffers[(offset + 2) % segment * w3], offsets[2], w3);
				BltAAFix3(dp, sum_buffer, w3);
				offsets += 3;
				break;

			default:
				NEVER();
				break;

	/*
			default:
				{
	//				memset(sum_buffer, 0, sizeof(unsigned) * w3);
	//				unsigned o = offset, left = segment;
					BltAASet3(sum_buffer,
						&row_buffers[(offset + 0) % segment * w3], offsets[0],
						&row_buffers[(offset + 1) % segment * w3], offsets[1],
						&row_buffers[(offset + 2) % segment * w3], offsets[2], w3);
					offsets += 3;
					unsigned o = offset + 3, left = segment - 3;
					for(unsigned c = left >> 1; c; c--, o += 2, offsets += 2)
						BltAAAdd2(sum_buffer, &row_buffers[o % segment * w3], offsets[0],
							&row_buffers[(o + 1) % segment * w3], offsets[1], w3);
					if(left & 1)
						BltAAAdd1(sum_buffer, &row_buffers[o % segment * w3], *offsets++, w3);
					BltAAFix3(dp, sum_buffer, w3);
				}
				break;
	*/
			}
		}
	}
//	MMXClear();
}

void PixelCopyAntiAliasMask(PixelArray& dest, Rect dest_rc,
	const PixelArray& src_pixel, const PixelArray& src_alpha, Rect src_rc, bool inv, Rect clip)
{
	if(dest.IsEmpty())
		return;

	ASSERT(dest.GetRawBPP() == -3);

	if(src_alpha.IsEmpty()) {
		PixelCopyAntiAlias(dest, dest_rc, src_pixel, src_rc, clip);
		return;
	}

	AA_TIMING("PixelCopyAntiAliasMask");

//	LOG("CopyAntiAlias: " << dest_rc << " <- " << src_rc);
	SOURCE_FORMAT sfmt = (src_pixel.bpp == 32 ? SRC32 : src_pixel.bpp == 24 ? SRC24
		: src_pixel.bpp == 16 ? SRC16 : src_pixel.bpp == 8 ? SRC8
		: src_pixel.bpp == 4 ? SRC4 : src_pixel.bpp == 1 ? SRC1 : SRC0);
	SOURCE_FORMAT mfmt = (src_alpha.bpp == 8 ? SRC8 : src_alpha.bpp == 1 ? SRC1 : SRC0);
	ASSERT(sfmt != SRC0 && mfmt != SRC0);
	Rect cl = dest.GetRect();
	if(!IsNull(clip))
		cl &= clip;
	Rect dr = dest_rc;
	Vector<unsigned> row = AAGetMap(dr.left, dr.right, cl.left, cl.right, src_rc.left, src_rc.right, 0, min(src_pixel.GetWidth(), src_alpha.GetWidth()), 1, 0x100);
	if(row.IsEmpty())
		return;
	Vector<unsigned> col = AAGetMap(dr.top, dr.bottom, cl.top, cl.bottom, src_rc.top, src_rc.bottom, 0, min(src_pixel.GetHeight(), src_alpha.GetHeight()), 1, 0x100);
	if(col.IsEmpty())
		return;

	Vector<unsigned> pix_row;
	pix_row <<= row;
	byte pal[768];
	if(sfmt == SRC32 || sfmt == SRC16 || sfmt == SRC24) {
		if(sfmt != SRC24) {
			PokeIL(pal + 0, src_pixel.cmask[2]);
			PokeIL(pal + 4, src_pixel.cmask[1]);
			PokeIL(pal + 8, src_pixel.cmask[0]);
			pal[12] = src_pixel.cshift24[2];
			pal[13] = src_pixel.cshift24[1];
			pal[14] = src_pixel.cshift24[0];
		}
		int times = (sfmt == SRC32 ? 4 : sfmt == SRC24 ? 3 : 2);
		pix_row[MAP_STEP] *= times;
		unsigned c = pix_row[MAP_COUNT], s = pix_row[MAP_BLOCK];
		for(unsigned *p = pix_row.GetIter(4); c; c--, p += s)
			*p *= times;
	}
	else
		CvtPalette(pal, src_pixel.palette);

	void (*pixel)(unsigned *dest, const byte *src, const unsigned *map, const byte *pal) = GetPixelProc(row[1], sfmt);
	void (*mask)(unsigned *dest, const byte *src, const unsigned *map, byte xor1) = GetMaskProc(row[1], mfmt);
	int wd = dr.Width(), w3 = 3 * wd, w4 = 4 * wd, dsh = 3 * dr.left;
	Buffer<unsigned> sum_buffer(w4);
	int count = col[MAP_COUNT], segment = col[MAP_SEGMENT], entry = col[MAP_BLOCK], step = col[MAP_STEP];
	int segspan = (segment - 1) * step + 1;
	bool bigseg = (segment >= MAXAA);
	byte xor1 = (inv ? 0 : mfmt == SRC8 ? 1 : 255);
	Buffer<unsigned> row_buffers(w4 * segment);
	int first = col[MAP_DATA], full = 0;
	const unsigned *offsets = col.GetIter(MAP_DATA);
	int offset = 0;
	for(int y = dr.top; --count >= 0; y++)
	{
		offset += *offsets++;
		ASSERT(offset >= 0 && offset + segspan <= src_pixel.size.cy);
		byte *dp = dest.GetDownScan(y) + dsh;
		if(bigseg)
		{
			for(int i = 0; i < MAXAA; i++)
			{
				mask(&row_buffers[i * w4], src_alpha.GetDownScan(offset + i * step), row.Begin(), xor1);
				pixel(&row_buffers[i * w4 + wd], src_pixel.GetDownScan(offset + i * step), pix_row.Begin(), pal);
			}
			BltAASet4(sum_buffer, &row_buffers[0 * w4], &row_buffers[1 * w4],
				&row_buffers[2 * w4], &row_buffers[3 * w4], w4);
			BltAAFixMaskAny(dp, sum_buffer + wd, sum_buffer, wd);
		}
		else
		{
			int endoff = offset + segment;
			for(int next = first + full; next < endoff; next++)
			{
				if(full >= segment)
					first++;
				else
					full++;
				unsigned *out = &row_buffers[next % segment * w4];
				mask(out, src_alpha.GetDownScan(next), row.Begin(), xor1);
				pixel(out + wd, src_pixel.GetDownScan(next), pix_row.Begin(), pal);
			}
			while(first > offset)
			{
				if(full < segment)
					full++;
				unsigned *out = &row_buffers[--first % segment * w4];
				mask(out, src_alpha.GetDownScan(first), row.Begin(), xor1);
				pixel(out + wd, src_pixel.GetDownScan(first), pix_row.Begin(), pal);
			}
			ASSERT(offset >= first && endoff <= first + full);
			switch(segment)
			{
			case 1:
				{
					const unsigned *p = &row_buffers[offset % segment * w4];
					BltAAFixMaskRow(dp, p + wd, p, wd);
				}
				offsets++;
				break;

			case 2:
				if(offsets[0] == 0)
				{
					const unsigned *p = &row_buffers[(offset + 1) % segment * w4];
					BltAAFixMaskRow(dp, p + wd, p, wd);
				}
				else if(offsets[1] == 0)
				{
					const unsigned *p = &row_buffers[offset % segment * w4];
					BltAAFixMaskRow(dp, p + wd, p, wd);
				}
				else
				{
	//				memset(sum_buffer, 0, sizeof(unsigned) * w4);
	//				BltAAAdd2(sum_buffer,
	//					&row_buffers[(offset + 0) % segment * w4], offsets[0],
	//					&row_buffers[(offset + 1) % segment * w4], offsets[1], w4);
					BltAASet2(sum_buffer,
						&row_buffers[(offset + 0) % segment * w4], offsets[0],
						&row_buffers[(offset + 1) % segment * w4], offsets[1], w4);
					BltAAFixMaskAny(dp, sum_buffer + wd, sum_buffer, wd);
				}
				offsets += 2;
				break;

			case 3:
				BltAASet3(sum_buffer,
					&row_buffers[(offset + 0) % segment * w4], offsets[0],
					&row_buffers[(offset + 1) % segment * w4], offsets[1],
					&row_buffers[(offset + 2) % segment * w4], offsets[2], w4);
				BltAAFixMaskAny(dp, sum_buffer + wd, sum_buffer, wd);
				offsets += 3;
				break;

			default:
				NEVER();
				break;
			}

/*
			default:
				{
					BltAASet3(sum_buffer,
						&row_buffers[(offset + 0) % segment * w4], offsets[0],
						&row_buffers[(offset + 1) % segment * w4], offsets[1],
						&row_buffers[(offset + 2) % segment * w4], offsets[2], w4);
					offsets += 3;
					unsigned o = offset + 3, left = segment - 3;
					for(unsigned c = left >> 1; c; c--, o += 2, offsets += 2)
						BltAAAdd2(sum_buffer, &row_buffers[o % segment * w4], offsets[0],
							&row_buffers[(o + 1) % segment * w4], offsets[1], w4);
					if(left & 1)
						BltAAAdd1(sum_buffer, &row_buffers[o % segment * w4], *offsets++, w4);
					BltAAFixMaskAny(dp, sum_buffer + wd, sum_buffer, wd);
				}
				break;
			}
*/
		}
	}
}

void PixelCopyAntiAliasMaskOut(PixelArray& dest_pixel, PixelArray& dest_alpha, Rect dest_rc,
	const PixelArray& src_pixel, const PixelArray& src_alpha, Rect src_rc, bool merge, bool inv, Rect clip)
{
	AA_TIMING("PixelCopyAntiAliasMaskOut");

	if(dest_pixel.IsEmpty() && dest_alpha.IsEmpty())
		return;

	ASSERT(dest_pixel.GetRawBPP() == -3);

	if(dest_alpha.IsEmpty())
	{
		if(merge && !src_alpha.IsEmpty())
			PixelCopyAntiAliasMask(dest_pixel, dest_rc, src_pixel, src_alpha, src_rc, inv, clip);
		else
			PixelCopyAntiAlias(dest_pixel, dest_rc, src_pixel, src_rc, clip);
		return;
	}

	Rect cl = dest_alpha.GetRect() & dest_rc;
	if(!dest_pixel.IsEmpty())
		cl &= dest_pixel.GetRect();
	if(!IsNull(clip))
		cl &= clip;
	if(cl.IsEmpty())
		return;

	if(dest_pixel.IsEmpty())
	{
		if(!src_alpha.IsEmpty())
			PixelCopyAntiAliasMaskOnly(dest_alpha, dest_rc, src_alpha, src_rc, merge, inv, clip);
		else
			PixelSet(dest_alpha, cl, 0);
		return;
	}
	if(src_alpha.IsEmpty())
	{
		PixelSet(dest_alpha, cl, 0);
		if(!src_pixel.IsEmpty())
			PixelCopyAntiAlias(dest_pixel, dest_rc, src_pixel, src_rc, clip);
		return;
	}
	if(src_pixel.IsEmpty())
	{
		PixelCopyAntiAliasMaskOnly(dest_alpha, dest_rc, src_alpha, src_rc, merge, inv, clip);
		return;
	}

//	LOG("CopyAntiAlias: " << dest_rc << " <- " << src_rc);
	ASSERT(dest_alpha.GetSize() == dest_pixel.GetSize());
	SOURCE_FORMAT sfmt = (src_pixel.bpp == 32 ? SRC32 : src_pixel.bpp == 24 ? SRC24
		: src_pixel.bpp == 16 ? SRC16 : src_pixel.bpp == 8 ? SRC8
		: src_pixel.bpp == 4 ? SRC4 : src_pixel.bpp == 1 ? SRC1 : SRC0);
	SOURCE_FORMAT mfmt = (src_alpha.bpp == 8 ? SRC8 : src_alpha.bpp == 1 ? SRC1 : SRC0);
	ASSERT(dest_pixel.bpp == 24 && sfmt != SRC0 && mfmt != SRC0);
	Rect dr = dest_rc;
	Vector<unsigned> row = AAGetMap(dr.left, dr.right, cl.left, cl.right, src_rc.left, src_rc.right,
		0, min(src_pixel.GetWidth(), src_alpha.GetWidth()), 1, 0x100);
	if(row.IsEmpty())
		return;
	Vector<unsigned> col = AAGetMap(dr.top, dr.bottom, cl.top, cl.bottom, src_rc.top, src_rc.bottom,
		0, min(src_pixel.GetHeight(), src_alpha.GetHeight()), 1, 0x100);
	if(col.IsEmpty())
		return;

	byte pal[768];
	Vector<unsigned> pix_row;
	pix_row <<= row;

	if(sfmt == SRC32 || sfmt == SRC16 || sfmt == SRC24) {
		if(sfmt != SRC24) {
			PokeIL(pal + 0, src_pixel.cmask[2]);
			PokeIL(pal + 4, src_pixel.cmask[1]);
			PokeIL(pal + 8, src_pixel.cmask[0]);
			pal[12] = src_pixel.cshift24[2];
			pal[13] = src_pixel.cshift24[1];
			pal[14] = src_pixel.cshift24[0];
		}
		int times = (sfmt == SRC32 ? 4 : sfmt == SRC24 ? 3 : 2);
		pix_row[MAP_STEP] *= times;
		unsigned c = pix_row[MAP_COUNT], s = pix_row[MAP_BLOCK];
		for(unsigned *p = pix_row.GetIter(4); c; c--, p += s)
			*p *= times;
	}
	else
		CvtPalette(pal, src_pixel.palette);

	void (*mrow)(byte *dpixel, byte *dmask, const unsigned *pixels, const unsigned *weights, unsigned count)
		= merge ? BltAAFixMaskOutMergeRow : BltAAFixMaskOutRow;
	void (*many)(byte *dpixel, byte *dmask, const unsigned *pixels, const unsigned *weights, unsigned count)
		= merge ? BltAAFixMaskOutMergeAny : BltAAFixMaskOutAny;

	void (*pixel)(unsigned *dest, const byte *src, const unsigned *map, const byte *pal) = GetPixelProc(row[1], sfmt);
	void (*mask)(unsigned *dest, const byte *src, const unsigned *map, byte xor1) = GetMaskProc(row[1], mfmt);
	int wd = dr.Width(), w3 = 3 * wd, w4 = 4 * wd, dsh = 3 * dr.left;
	Buffer<unsigned> sum_buffer(w4);
	int count = col[MAP_COUNT];
	int segment = col[MAP_SEGMENT];
	int entry = col[MAP_BLOCK];
	int step = col[MAP_STEP];
	int segspan = (segment - 1) * step + 1;
	bool bigseg = (segment >= MAXAA);
	byte xor1 = (inv ? 0 : mfmt == SRC8 ? 1 : 255);
	Buffer<unsigned> row_buffers(w4 * segment);
	int first = col[4], full = 0;
	const unsigned *offsets = col.GetIter(4);
	int offset = 0;
	for(int y = dr.top; --count >= 0; y++)
	{
		offset += *offsets++;
		byte *dp = dest_pixel.GetDownScan(y) + dsh;
		byte *dm = dest_alpha.GetDownScan(y) + dr.left;
		if(bigseg)
		{
			for(int i = 0; i < MAXAA; i++)
			{
				mask(&row_buffers[i * w4], src_alpha.GetDownScan(offset + i * step), row.Begin(), xor1);
				pixel(&row_buffers[i * w4 + wd], src_pixel.GetDownScan(offset + i * step), pix_row.Begin(), pal);
			}
			BltAASet4(sum_buffer, &row_buffers[0 * w4], &row_buffers[1 * w4],
				&row_buffers[2 * w4], &row_buffers[3 * w4], w4);
			many(dp, dm, sum_buffer + wd, sum_buffer, wd);
		}
		else
		{
			int endoff = offset + segment;
			for(int next = first + full; next < endoff; next++)
			{
				if(full >= segment)
					first++;
				else
					full++;
				unsigned *out = &row_buffers[next % segment * w4];
				mask(out, src_alpha.GetDownScan(next), row.Begin(), xor1);
				pixel(out + wd, src_pixel.GetDownScan(next), pix_row.Begin(), pal);
			}
			while(first > offset)
			{
				if(full < segment)
					full++;
				unsigned *out = &row_buffers[--first % segment * w4];
				mask(out, src_alpha.GetDownScan(first), row.Begin(), xor1);
				pixel(out + wd, src_pixel.GetDownScan(first), pix_row.Begin(), pal);
			}
			ASSERT(offset >= first && endoff <= first + full);
			switch(segment)
			{
			case 1:
				{
					const unsigned *p = &row_buffers[offset % segment * w4];
					mrow(dp, dm, p + wd, p, wd);
				}
				offsets++;
				break;

			case 2:
				if(offsets[0] == 0)
				{
					const unsigned *p = &row_buffers[(offset + 1) % segment * w4];
					mrow(dp, dm, p + wd, p, wd);
				}
				else if(offsets[1] == 0)
				{
					const unsigned *p = &row_buffers[offset % segment * w4];
					mrow(dp, dm, p + wd, p, wd);
				}
				else
				{
	//				memset(sum_buffer, 0, sizeof(unsigned) * w4);
	//				BltAAAdd2(sum_buffer,
	//					&row_buffers[(offset + 0) % segment * w4], offsets[0],
	//					&row_buffers[(offset + 1) % segment * w4], offsets[1], w4);
					BltAASet2(sum_buffer,
						&row_buffers[(offset + 0) % segment * w4], offsets[0],
						&row_buffers[(offset + 1) % segment * w4], offsets[1], w4);
					many(dp, dm, sum_buffer + wd, sum_buffer, wd);
				}
				offsets += 2;
				break;

			case 3:
				BltAASet3(sum_buffer,
					&row_buffers[(offset + 0) % segment * w4], offsets[0],
					&row_buffers[(offset + 1) % segment * w4], offsets[1],
					&row_buffers[(offset + 2) % segment * w4], offsets[2], w4);
				many(dp, dm, sum_buffer + wd, sum_buffer, wd);
				offsets += 3;
				break;

			default:
				NEVER();
				break;

/*
			default:
				{
	//				memset(sum_buffer, 0, sizeof(unsigned) * w4);
					BltAASet3(sum_buffer,
						&row_buffers[(offset + 0) % segment * w4], offsets[0],
						&row_buffers[(offset + 1) % segment * w4], offsets[1],
						&row_buffers[(offset + 2) % segment * w4], offsets[2], w4);
					offsets += 3;
					unsigned o = offset + 3, left = segment - 3;
					for(unsigned c = left >> 1; c; c--, o += 2, offsets += 2)
						BltAAAdd2(sum_buffer, &row_buffers[o % segment * w4], offsets[0],
							&row_buffers[(o + 1) % segment * w4], offsets[1], w4);
					if(left & 1)
						BltAAAdd1(sum_buffer, &row_buffers[o % segment * w4], *offsets++, w4);
					many(dp, dm, sum_buffer + wd, sum_buffer, wd);
				}
				break;
*/
			}
		}
	}
}

void PixelCopyAntiAliasMaskOnly(PixelArray& dest_alpha, Rect dest_rc,
	const PixelArray& src_alpha, Rect src_rc, bool merge, bool inv, Rect clip)
{
	AA_TIMING("PixelCopyAntiAliasMaskOnly");

//	LOG("CopyAntiAlias: " << dest_rc << " <- " << src_rc);
	SOURCE_FORMAT mfmt = (src_alpha.bpp == 8 ? SRC8 : src_alpha.bpp == 1 ? SRC1 : SRC0);
	ASSERT(dest_alpha.bpp == 8 && mfmt != SRC0);
	Rect cl = dest_alpha.GetRect();
	if(!IsNull(clip))
		cl &= clip;
	Rect dr = dest_rc;
	Vector<unsigned> row = AAGetMap(dr.left, dr.right, cl.left, cl.right, src_rc.left, src_rc.right,
		0, src_alpha.GetWidth(), 1, 0x100);
	if(row.IsEmpty())
		return;
	Vector<unsigned> col = AAGetMap(dr.top, dr.bottom, cl.top, cl.bottom, src_rc.top, src_rc.bottom,
		0, src_alpha.GetHeight(), 1, 0x100);
	if(col.IsEmpty())
		return;

	void (*mrow)(byte *dmask, const unsigned *weights, unsigned count)
		= merge ? BltAAFixMaskOnlyMergeRow : BltAAFixMaskOnlyRow;
	void (*many)(byte *dmask, const unsigned *weights, unsigned count)
		= merge ? BltAAFixMaskOnlyMergeAny : BltAAFixMaskOnlyAny;
	void (*mask)(unsigned *dest, const byte *src, const unsigned *map, byte xor1) = GetMaskProc(row[1], mfmt);

	int wd = dr.Width();
	Buffer<unsigned> sum_buffer(wd);
	int count = col[MAP_COUNT];
	int segment = col[MAP_SEGMENT];
	int entry = col[MAP_BLOCK];
	int step = col[MAP_STEP];
	int segspan = (segment - 1) * step + 1;
	bool bigseg = (segment >= MAXAA);
	byte xor1 = (inv ? 0 : mfmt == SRC8 ? 1 : 255);
	Buffer<unsigned> row_buffers(wd * segment);
	int first = col[4], full = 0;
	const unsigned *offsets = col.GetIter(4);
	int offset = 0;
	for(int y = dr.top; --count >= 0; y++)
	{
		offset += *offsets++;
		byte *dm = dest_alpha.GetDownScan(y) + dr.left;
		if(bigseg)
		{
			mask(&row_buffers[0 * wd], src_alpha.GetDownScan(offset + 0 * step), row.Begin(), xor1);
			mask(&row_buffers[1 * wd], src_alpha.GetDownScan(offset + 1 * step), row.Begin(), xor1);
			mask(&row_buffers[2 * wd], src_alpha.GetDownScan(offset + 2 * step), row.Begin(), xor1);
			mask(&row_buffers[3 * wd], src_alpha.GetDownScan(offset + 3 * step), row.Begin(), xor1);
			BltAASet4(sum_buffer,
				&row_buffers[0 * wd], &row_buffers[1 * wd], &row_buffers[2 * wd], &row_buffers[3 * wd], wd);
			many(dm, sum_buffer, wd);
		}
		else
		{
			int endoff = offset + segment;
			for(int next = first + full; next < endoff; next++)
			{
				if(full >= segment)
					first++;
				else
					full++;
				unsigned *out = &row_buffers[next % segment * wd];
				mask(out, src_alpha.GetDownScan(next), row.Begin(), xor1);
			}
			while(first > offset)
			{
				if(full < segment)
					full++;
				unsigned *out = &row_buffers[--first % segment * wd];
				mask(out, src_alpha.GetDownScan(first), row.Begin(), xor1);
			}
			ASSERT(offset >= first && endoff <= first + full);
			switch(segment)
			{
			case 1:
				{
					const unsigned *p = &row_buffers[offset % segment * wd];
					mrow(dm, p, wd);
				}
				offsets++;
				break;

			case 2:
				if(offsets[0] == 0)
				{
					const unsigned *p = &row_buffers[(offset + 1) % segment * wd];
					mrow(dm, p, wd);
				}
				else if(offsets[1] == 0)
				{
					const unsigned *p = &row_buffers[offset % segment * wd];
					mrow(dm, p, wd);
				}
				else
				{
	//				memset(sum_buffer, 0, sizeof(unsigned) * w4);
	//				BltAAAdd2(sum_buffer,
	//					&row_buffers[(offset + 0) % segment * w4], offsets[0],
	//					&row_buffers[(offset + 1) % segment * w4], offsets[1], w4);
					BltAASet2(sum_buffer,
						&row_buffers[(offset + 0) % segment * wd], offsets[0],
						&row_buffers[(offset + 1) % segment * wd], offsets[1], wd);
					many(dm, sum_buffer, wd);
				}
				offsets += 2;
				break;

			case 3:
				BltAASet3(sum_buffer,
					&row_buffers[(offset + 0) % segment * wd], offsets[0],
					&row_buffers[(offset + 1) % segment * wd], offsets[1],
					&row_buffers[(offset + 2) % segment * wd], offsets[2], wd);
				many(dm, sum_buffer, wd);
				offsets += 3;
				break;

			default:
				NEVER();
				break;

/*
			default:
				{
	//				memset(sum_buffer, 0, sizeof(unsigned) * w4);
					BltAASet3(sum_buffer,
						&row_buffers[(offset + 0) % segment * wd], offsets[0],
						&row_buffers[(offset + 1) % segment * wd], offsets[1],
						&row_buffers[(offset + 2) % segment * wd], offsets[2], wd);
					offsets += 3;
					unsigned o = offset + 3, left = segment - 3;
					for(unsigned c = left >> 1; c; c--, o += 2, offsets += 2)
						BltAAAdd2(sum_buffer, &row_buffers[o % segment * wd], offsets[0],
							&row_buffers[(o + 1) % segment * wd], offsets[1], wd);
					if(left & 1)
						BltAAAdd1(sum_buffer, &row_buffers[o % segment * wd], *offsets++, wd);
					many(dm, sum_buffer, wd);
				}
				break;
*/
			}
		}
	}
}

#endif
