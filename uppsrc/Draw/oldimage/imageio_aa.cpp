//////////////////////////////////////////////////////////////////////
// graphics/imageio_aa.cpp: antialiased image scaling.

#include "Draw.h"

#if defined(PLATFORM_WIN32) && defined(QLIB2IMAGE)

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
};

#define AA_TIMING(name) // RTIMING(name) // comment off to turn on routine timing

//////////////////////////////////////////////////////////////////////

void MMXDump();

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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
			cache[i] = minmax<int>((i <= HALF ? o : 2 - o) * 128, 0, 255);
		}
	}
	return cache;
}

//////////////////////////////////////////////////////////////////////

Vector<unsigned> AAGetMap(int& dmin, int& dmax, int dclipmin, int dclipmax, int smin, int smax, int sclipmin, int sclipmax, int times, int avail)
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
	int segment = min(tabs(sw) % dw ? idivceil(tabs(sw), dw) + 1 : tabs(sw) / dw, smax - smin);
	map.SetCount(2 + count * (1 + segment));
	map[0] = unsigned(count);
	map[1] = unsigned(segment);
	unsigned *out = map.Begin() + 2;
	int sendoffset = (smax - segment) * times;
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
				int i = segment - 1;
				while(--i >= 0)
					*out++ = 0;
				*out++ = left;
			}
			else if(end <= start)
			{ // 1 source pixel only
				int scomp = minmax(start + segment - smax, 0, start - smin);
				last += *out++ = (start - scomp) * times - last;
				int i = scomp;
				while(--i >= 0)
					*out++ = 0;
				*out++ = unsigned(left);
				i = segment - scomp - 1;
				while(--i >= 0)
					*out++ = 0;
			}
			else
			{
				int delta = (dw - rem) * left / total;
				if(!delta)
					start++;
				int scomp = minmax(start + segment - smax, 0, start - smin);
				last += *out++ = (start - scomp) * times - last;
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
//			LOG("-> " << map[rec] << " + " << map[rec + 1]);
		}
	}
	else
	{ // size inflation
		static const byte *curve = GetStretchCurve();
		ASSERT(segment == 2);
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
	int offs = 0, step = map[1] + 1;
	for(int t = 0; t < map[0]; t++)
	{
		offs += map[2 + t * step];
		ASSERT(offs >= times * smin && offs + times * map[1] <= times * smax);
	}
#endif

	return map;
}

//////////////////////////////////////////////////////////////////////

#define BLT_AA_MAP_REP4(BPROC, o) \
	BPROC((o) + 0) \
	BPROC((o) + 1) \
	BPROC((o) + 2) \
	BPROC((o) + 3)

#define BLT_AA_MAP_PROC(SUFFIX, SCOUNT, BPROC) \
static void COMBINE(BltAAMap, SUFFIX)(unsigned *dest, const byte *s, const unsigned *map, const byte *pal) \
{ \
	AA_TIMING("BltAAMap" #SUFFIX); \
	unsigned count = map[0]; \
	unsigned offset = 0; \
	map += 2; \
	enum { STEP = SCOUNT + 1 }; \
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

#define BLT_AA_MAP_RGB_1(o) s += map[2 * (o) + 0]; \
	dest[3 * (o) + 0] = s[0] * 0x100; \
	dest[3 * (o) + 1] = s[1] * 0x100; \
	dest[3 * (o) + 2] = s[2] * 0x100; \

#define BLT_AA_MAP_RGB_2(o) s += map[3 * (o) + 0]; \
	dest[3 * (o) + 0] = s[0] * map[3 * (o) + 1] + s[3] * map[3 * (o) + 2]; \
	dest[3 * (o) + 1] = s[1] * map[3 * (o) + 1] + s[4] * map[3 * (o) + 2]; \
	dest[3 * (o) + 2] = s[2] * map[3 * (o) + 1] + s[5] * map[3 * (o) + 2]; \

/*
#define BLT_AA_MAP_RGB_3(o) \
	dest[3 * (o) + 0] = s[0] * map[4 * (o) + 1] + s[3] * map[4 * (o) + 2] + s[6] * map[4 * (o) + 3]; \
	dest[3 * (o) + 1] = s[1] * map[4 * (o) + 1] + s[4] * map[4 * (o) + 2] + s[7] * map[4 * (o) + 3]; \
	dest[3 * (o) + 2] = s[2] * map[4 * (o) + 1] + s[5] * map[4 * (o) + 2] + s[8] * map[4 * (o) + 3]; \
*/
#define BLT_AA_MAP_RGB_3(o) s += map[4 * (o) + 0]; \
	dest[3 * (o) + 0] = (s[0] - s[6]) * map[4 * (o) + 1] + (s[3] - s[6]) * map[4 * (o) + 2] + s[6] * 0x100; \
	dest[3 * (o) + 1] = (s[1] - s[7]) * map[4 * (o) + 1] + (s[4] - s[7]) * map[4 * (o) + 2] + s[7] * 0x100; \
	dest[3 * (o) + 2] = (s[2] - s[8]) * map[4 * (o) + 1] + (s[5] - s[8]) * map[4 * (o) + 2] + s[8] * 0x100; \

#define BLT_AA_MAP_RGB_4(o) s += map[5 * (o) + 0]; \
	dest[3 * (o) + 0] = s[0] * map[5 * (o) + 1] + s[3] * map[5 * (o) + 2] + s[6] * map[5 * (o) + 3] + s[ 9] * map[5 * (o) + 4]; \
	dest[3 * (o) + 1] = s[1] * map[5 * (o) + 1] + s[4] * map[5 * (o) + 2] + s[7] * map[5 * (o) + 3] + s[10] * map[5 * (o) + 4]; \
	dest[3 * (o) + 2] = s[2] * map[5 * (o) + 1] + s[5] * map[5 * (o) + 2] + s[8] * map[5 * (o) + 3] + s[11] * map[5 * (o) + 4]; \

#define BLT_AA_MAP_PAL_1(o) s += map[2 * (o) + 0]; \
	dest[3 * (o) + 0] = pal[s[0] + 0x200] * 0x100; \
	dest[3 * (o) + 1] = pal[s[0] + 0x100] * 0x100; \
	dest[3 * (o) + 2] = pal[s[0] + 0x000] * 0x100; \

#define BLT_AA_MAP_PAL_2(o) s += map[3 * (o) + 0]; \
	dest[3 * (o) + 0] = pal[s[0] + 0x200] * map[3 * (o) + 1] + pal[s[1] + 0x200] * map[3 * (o) + 2]; \
	dest[3 * (o) + 1] = pal[s[0] + 0x100] * map[3 * (o) + 1] + pal[s[1] + 0x100] * map[3 * (o) + 2]; \
	dest[3 * (o) + 2] = pal[s[0] + 0x000] * map[3 * (o) + 1] + pal[s[1] + 0x000] * map[3 * (o) + 2]; \

#define BLT_AA_MAP_PAL_3(o) s += map[4 * (o) + 0]; \
	dest[3 * (o) + 0] = pal[s[0] + 0x200] * map[4 * (o) + 1] + pal[s[1] + 0x200] * map[4 * (o) + 2] + pal[s[2] + 0x200] * map[4 * (o) + 3]; \
	dest[3 * (o) + 1] = pal[s[0] + 0x100] * map[4 * (o) + 1] + pal[s[1] + 0x100] * map[4 * (o) + 2] + pal[s[2] + 0x100] * map[4 * (o) + 3]; \
	dest[3 * (o) + 2] = pal[s[0] + 0x000] * map[4 * (o) + 1] + pal[s[1] + 0x000] * map[4 * (o) + 2] + pal[s[2] + 0x000] * map[4 * (o) + 3]; \

#define BLT_AA_MAP_PAL_4(o) s += map[5 * (o) + 0]; \
	dest[3 * (o) + 0] = pal[s[0] + 0x200] * map[5 * (o) + 1] + pal[s[1] + 0x200] * map[5 * (o) + 2] + pal[s[2] + 0x200] * map[5 * (o) + 3] + pal[s[3] + 0x200] * map[5 * (o) + 4]; \
	dest[3 * (o) + 1] = pal[s[0] + 0x100] * map[5 * (o) + 1] + pal[s[1] + 0x100] * map[5 * (o) + 2] + pal[s[2] + 0x100] * map[5 * (o) + 3] + pal[s[3] + 0x100] * map[5 * (o) + 4]; \
	dest[3 * (o) + 2] = pal[s[0] + 0x000] * map[5 * (o) + 1] + pal[s[1] + 0x000] * map[5 * (o) + 2] + pal[s[2] + 0x000] * map[5 * (o) + 3] + pal[s[3] + 0x000] * map[5 * (o) + 4]; \

#define BLT_AA_MAP_BIT_1(o) offset += map[2 * (o) + 0]; \
	{ \
		const byte *p = s + (offset >> 3); \
		const byte *p1 = &pal[(*p >> (~offset & 7)) & 1]; \
		dest[3 * (o) + 0] = p1[0x200] * 0x100; \
		dest[3 * (o) + 1] = p1[0x100] * 0x100; \
		dest[3 * (o) + 2] = p1[0x000] * 0x100; \
	}
	
#define BLT_AA_MAP_BIT_2(o) offset += map[3 * (o) + 0]; \
	{ \
		const byte *p = s + (offset >> 3); \
		int s = ~offset & 7; \
		const byte *p1 = &pal[(*p >> s) & 1]; \
		const byte *p2 = &pal[s ? (*p >> (s - 1)) & 1 : p[1] >> 7]; \
		dest[3 * (o) + 0] = p1[0x200] * map[3 * (o) + 1] + p2[0x200] * map[3 * (o) + 2]; \
		dest[3 * (o) + 1] = p1[0x100] * map[3 * (o) + 1] + p2[0x100] * map[3 * (o) + 2]; \
		dest[3 * (o) + 2] = p1[0x000] * map[3 * (o) + 1] + p2[0x000] * map[3 * (o) + 2]; \
	}
	
#define BLT_AA_MAP_BIT_3(o) offset += map[4 * (o) + 0]; \
	{ \
		const byte *p = s + (offset >> 3); \
		int sh = (offset & 7) - 5; \
		byte v = (sh <= 0 ? p[0] >> -sh : (p[0] << sh) | (p[1] >> (8 - sh))); \
		const byte *p1 = &pal[(v >> 2) & 1], *p2 = &pal[(v >> 1) & 1], *p3 = &pal[(v >> 0) & 1]; \
		dest[3 * (o) + 0] = p1[0x200] * map[4 * (o) + 1] + p2[0x200] * map[4 * (o) + 2] + p3[0x200] * map[4 * (o) + 3]; \
		dest[3 * (o) + 1] = p1[0x100] * map[4 * (o) + 1] + p2[0x100] * map[4 * (o) + 2] + p3[0x100] * map[4 * (o) + 3]; \
		dest[3 * (o) + 2] = p1[0x000] * map[4 * (o) + 1] + p2[0x000] * map[4 * (o) + 2] + p3[0x000] * map[4 * (o) + 3]; \
	}
	
#define BLT_AA_MAP_BIT_4(o) offset += map[5 * (o) + 0]; \
	{ \
		const byte *p = s + (offset >> 3); \
		int sh = (offset & 7) - 4; \
		byte v = (sh <= 0 ? p[0] >> -sh : (p[0] << sh) | (p[1] >> (8 - sh))); \
		const byte *p1 = &pal[(v >> 3) & 1], *p2 = &pal[(v >> 2) & 1], *p3 = &pal[(v >> 1) & 1], *p4 = &pal[(v >> 0) & 1]; \
		dest[3 * (o) + 0] = p1[0x200] * map[5 * (o) + 1] + p2[0x200] * map[5 * (o) + 2] + p3[0x200] * map[5 * (o) + 3] + p4[0x200] * map[5 * (o) + 4]; \
		dest[3 * (o) + 1] = p1[0x100] * map[5 * (o) + 1] + p2[0x100] * map[5 * (o) + 2] + p3[0x100] * map[5 * (o) + 3] + p4[0x100] * map[5 * (o) + 4]; \
		dest[3 * (o) + 2] = p1[0x000] * map[5 * (o) + 1] + p2[0x000] * map[5 * (o) + 2] + p3[0x000] * map[5 * (o) + 3] + p4[0x000] * map[5 * (o) + 4]; \
	}

BLT_AA_MAP_PROC(Rgb1, 1, BLT_AA_MAP_RGB_1)
//BLT_AA_MAP_PROC(Rgb2, 2, BLT_AA_MAP_RGB_2)
BLT_AA_MAP_PROC(Rgb3, 3, BLT_AA_MAP_RGB_3)
BLT_AA_MAP_PROC(Rgb4, 4, BLT_AA_MAP_RGB_4)
BLT_AA_MAP_PROC(Pal1, 1, BLT_AA_MAP_PAL_1)
BLT_AA_MAP_PROC(Pal2, 2, BLT_AA_MAP_PAL_2)
BLT_AA_MAP_PROC(Pal3, 3, BLT_AA_MAP_PAL_3)
BLT_AA_MAP_PROC(Pal4, 4, BLT_AA_MAP_PAL_4)
BLT_AA_MAP_PROC(Bit1, 1, BLT_AA_MAP_BIT_1)
BLT_AA_MAP_PROC(Bit2, 2, BLT_AA_MAP_BIT_2)
BLT_AA_MAP_PROC(Bit3, 3, BLT_AA_MAP_BIT_3)
BLT_AA_MAP_PROC(Bit4, 4, BLT_AA_MAP_BIT_4)

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

//*
static void BltAAMapRgb2(unsigned *dest, const byte *s, const unsigned *map, const byte *pal)
{
	AA_TIMING("BltAAMapRgb2");
	unsigned count = map[0];
	map += 2;

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

//////////////////////////////////////////////////////////////////////

static void BltAAMapRgbAny(unsigned *dest, const byte *s, const unsigned *map, const byte *pal)
{
	AA_TIMING("BltAAMapRgbAny");
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

//////////////////////////////////////////////////////////////////////

static void BltAAMapPalAny(unsigned *dest, const byte *s, const unsigned *map, const byte *pal)
{
	AA_TIMING("BltAAMapPalAny");
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

//////////////////////////////////////////////////////////////////////

static void BltAAMapBitAny(unsigned *dest, const byte *s, const unsigned *map, const byte *pal)
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
		const byte *p = s + (offset >> 3);
		unsigned b = 0, g = 0, r = 0;
		const byte *pp;
		byte v;
		for(unsigned n = seg8; n; n--)
		{
			v = (sh1 ? (p[0] << sh1) | (p[1] >> sh2) : p[0]);

#define BLT_AA_MAP_ANY_BIT_1(o) pp = &pal[(v >> (7 - (o))) & 1]; \
			b += pp[0x200] * map[(o)]; \
			g += pp[0x100] * map[(o)]; \
			r += pp[0x000] * map[(o)]; \

			BLT_AA_MAP_ANY_BIT_1(0) BLT_AA_MAP_ANY_BIT_1(1) BLT_AA_MAP_ANY_BIT_1(2) BLT_AA_MAP_ANY_BIT_1(3)
			BLT_AA_MAP_ANY_BIT_1(4) BLT_AA_MAP_ANY_BIT_1(5) BLT_AA_MAP_ANY_BIT_1(6) BLT_AA_MAP_ANY_BIT_1(7)
			map += 8;
			p++;
		}
		v = ((segment & 7) + sh1 <= 8 ? p[0] << sh1 : (p[0] << sh1) | (p[1] >> sh2));
		if(segment & 4)
		{
			BLT_AA_MAP_ANY_BIT_1(0)
			BLT_AA_MAP_ANY_BIT_1(1)
			BLT_AA_MAP_ANY_BIT_1(2)
			BLT_AA_MAP_ANY_BIT_1(3)
			map += 4;
			v <<= 4;
		}
		if(segment & 2)
		{
			BLT_AA_MAP_ANY_BIT_1(0);
			BLT_AA_MAP_ANY_BIT_1(1);
			map += 2;
			v <<= 2;
		}
		if(segment & 1)
		{
			BLT_AA_MAP_ANY_BIT_1(0);
			map++;
		}
		dest[0] = b;
		dest[1] = g;
		dest[2] = r;
		dest += 3;
	}
	while(--count);
}

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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
	unsigned offset = 0; \
	map += 2; \
	enum { STEP = SCOUNT + 1 }; \
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

#define BLT_AA_MAP_MASK_BYTE_1(o) s += map[2 * (o)]; dest[(o)] = ((s[0] - 1) & 0x100);
#define BLT_AA_MAP_MASK_BYTE_2(o) s += map[3 * (o)]; dest[(o)] = ((s[0] - 1) & map[3 * (o) + 1]) + ((s[1] - 1) & map[3 * (o) + 2]);
#define BLT_AA_MAP_MASK_BYTE_3(o) s += map[4 * (o)]; dest[(o)] = ((s[0] - 1) & map[4 * (o) + 1]) + ((s[1] - 1) & map[4 * (o) + 2]) + ((s[2] - 1) & map[4 * (o) + 3]);
#define BLT_AA_MAP_MASK_BYTE_4(o) s += map[5 * (o)]; dest[(o)] = ((s[0] - 1) & map[5 * (o) + 1]) + ((s[1] - 1) & map[5 * (o) + 2]) + ((s[2] - 1) & map[5 * (o) + 3]) + ((s[3] - 1) & map[5 * (o) + 4]);

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
	
#define BLT_AA_MAP_MASK_BIT_4(o) offset += map[5 * (o) + 0]; \
	{ \
		const byte *p = s + (offset >> 3); \
		int sh = (offset & 7) - 4; \
		byte v = (sh <= 0 ? p[0] >> -sh : (p[0] << sh) | (p[1] >> (8 - sh))) ^ xor1; \
		dest[(o)] = (map[5 * (o) + 1] & -((v >> 3) & 1)) \
		+ (map[5 * (o) + 2] & -((v >> 2) & 1)) + (map[5 * (o) + 3] & -((v >> 1) & 1)) + (map[5 * (o) + 4] & -((v >> 0) & 1)); \
	}

BLT_AA_MAP_MASK_PROC(Bit1, 1, BLT_AA_MAP_MASK_BIT_1)
BLT_AA_MAP_MASK_PROC(Bit2, 2, BLT_AA_MAP_MASK_BIT_2)
BLT_AA_MAP_MASK_PROC(Bit3, 3, BLT_AA_MAP_MASK_BIT_3)
BLT_AA_MAP_MASK_PROC(Bit4, 4, BLT_AA_MAP_MASK_BIT_4)

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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
	else \
	{ \
		dpixel[3 * (o) + 0] = dpixel[3 * (o) + 1] = dpixel[3 * (o) + 2] = 0; \
		dmask[(o)] = 1; \
	} \

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

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

enum SOURCE_FORMAT { SRGB, SPAL, SBIT };

static void (*GetPixelProc(int rep, SOURCE_FORMAT sfmt))(unsigned *dest, const byte *src, const unsigned *map, const byte *pal)
{
	switch(sfmt)
	{
	case SRGB:
		switch(rep)
		{
		case 1:  return BltAAMapRgb1;
		case 2:  return BltAAMapRgb2;
		case 3:  return BltAAMapRgb3;
		case 4:  return BltAAMapRgb4;
		default: return BltAAMapRgbAny;
		}
	
	case SPAL:
		switch(rep)
		{
		case 1:  return BltAAMapPal1;
		case 2:  return BltAAMapPal2;
		case 3:  return BltAAMapPal3;
		case 4:  return BltAAMapPal4;
		default: return BltAAMapPalAny;
		}
		
	case SBIT:
		switch(rep)
		{
		case 1:  return BltAAMapBit1;
		case 2:  return BltAAMapBit2;
		case 3:  return BltAAMapBit3;
		case 4:  return BltAAMapBit4;
		default: return BltAAMapBitAny;
		}
		
	default:
		NEVER();
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////

static void (*GetMaskProc(int rep, SOURCE_FORMAT sfmt))(unsigned *dest, const byte *src, const unsigned *map, byte xor1)
{
	if(sfmt == SBIT)
		switch(rep)
		{
		case 1:  return BltAAMapMaskBit1;
		case 2:  return BltAAMapMaskBit2;
		case 3:  return BltAAMapMaskBit3;
		case 4:  return BltAAMapMaskBit4;
		default: return BltAAMapMaskBitAny;
		}
	else
		switch(rep)
		{
		case 1:  return BltAAMapMaskByte1;
		case 2:  return BltAAMapMaskByte2;
		case 3:  return BltAAMapMaskByte3;
		case 4:  return BltAAMapMaskByte4;
		default: return BltAAMapMaskByteAny;
		}
}

//////////////////////////////////////////////////////////////////////

void CopyAntiAliasRaw(RawImage& dest, const Rect& dest_rc, const RawImage& src, const Vector<Color>& palette, const Rect& src_rc, const Rect& clip)
{
	AA_TIMING("CopyAntiAlias (new)");
	
//	LOG("CopyAntiAlias: " << dest_rc << " <- " << src_rc);
	ASSERT(dest.bpp == 24 && (src.bpp == 24 || src.bpp == 8 || src.bpp == 1));
	SOURCE_FORMAT sfmt = (src.bpp == 24 ? SRGB : src.bpp == 8 ? SPAL : SBIT);
	
	Rect cl = Nvl(clip, dest.GetRect());
	Rect dr = dest_rc;
	Vector<unsigned> row = AAGetMap(dr.left, dr.right, cl.left, cl.right, src_rc.left, src_rc.right, 0, src.size.cx, sfmt == SRGB ? 3 : 1, 0x100);
	if(row.IsEmpty())
		return;
	Vector<unsigned> col = AAGetMap(dr.top, dr.bottom, cl.top, cl.bottom, src_rc.top, src_rc.bottom, 0, src.size.cy, 1, 0x100);
	if(col.IsEmpty())
		return;
	Buffer<byte> pal(sfmt == SRGB ? 1 : 0x300);
	if(sfmt != SRGB)
		CvtPalette(pal, palette);
	void (*row_proc)(unsigned *dest, const byte *src, const unsigned *map, const byte *pal) = GetPixelProc(row[1], sfmt);
	int wd = dr.Width(), w3 = 3 * wd, dsh = 3 * dr.left;
	Buffer<unsigned> sum_buffer(w3);
	int count = col[0];
	int segment = col[1];
	Buffer<unsigned> row_buffers(w3 * segment);
	int first = col[2], full = 0;
	const unsigned *offsets = col.GetIter(2);
	int offset = 0;
	for(int y = dr.top; --count >= 0; y++)
	{
		int endoff = (offset += *offsets++) + segment;
		ASSERT(offset >= 0 && endoff <= src.size.cy);
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
		byte *dp = dest.GetDownScan(y) + dsh;
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
		}
	}
//	MMXClear();
}

//////////////////////////////////////////////////////////////////////

void CopyAntiAlias(RawImage& dest, const Rect& dest_rc, const RawImage& src, const Rect& src_rc, const Rect& clip)
{
	CopyAntiAliasRaw(dest, dest_rc, src, src.palette, src_rc, clip);
}

//////////////////////////////////////////////////////////////////////

void CopyAntiAliasMask(RawImage& dest, const Rect& dest_rc, const RawImage& src, const RawImage& smask, const Rect& src_rc, const Rect& clip)
{
	AA_TIMING("CopyAntiAliasMask");
	
//	LOG("CopyAntiAlias: " << dest_rc << " <- " << src_rc);
	ASSERT(dest.bpp == 24 && (src.bpp == 24 || src.bpp == 8 || src.bpp == 1) && (src.bpp == 1 || smask.bpp == 8));
	SOURCE_FORMAT sfmt = (src.bpp == 24 ? SRGB : src.bpp == 8 ? SPAL : SBIT);
	byte xor1 = 0;
	const RawImage *smptr = &smask;
	if(sfmt == SBIT)
	{
		smptr = &src;
		Color c0 = smask.palette.GetCount() >= 1 ? smask.palette[0] : Black;
		Color c1 = smask.palette.GetCount() >= 2 ? smask.palette[1] : White;
		if(IsNull(c0) && IsNull(c1))
			return; // nothing to do
		if(!IsNull(c0) && !IsNull(c1))
		{ // no mask
			CopyAntiAliasRaw(dest, dest_rc, src, smask.palette, src_rc, clip);
			return;
		}
		xor1 = IsNull(c0) ? 0 : 255;
	}
	Rect cl = Nvl(clip, dest.GetRect());
	Rect dr = dest_rc;
	Vector<unsigned> row = AAGetMap(dr.left, dr.right, cl.left, cl.right, src_rc.left, src_rc.right, 0, min(src.size.cx, smptr -> size.cx), 1, 0x100);
	if(row.IsEmpty())
		return;
	Vector<unsigned> col = AAGetMap(dr.top, dr.bottom, cl.top, cl.bottom, src_rc.top, src_rc.bottom, 0, min(src.size.cy, smptr -> size.cy), 1, 0x100);
	if(col.IsEmpty())
		return;

	Vector<unsigned> pix_row;
	pix_row <<= row;
	Buffer<byte> pal(sfmt == SRGB ? 1 : 0x300);
	if(sfmt == SRGB)
	{ // multiply pixel offsets by 3
		unsigned c = pix_row[0], s = pix_row[1] + 1;
		for(unsigned *p = pix_row.GetIter(2); c; c--, p += s)
			*p *= 3;
	}
	else
		CvtPalette(pal, sfmt == SBIT ? smask.palette : src.palette);

	void (*pixel)(unsigned *dest, const byte *src, const unsigned *map, const byte *pal) = GetPixelProc(row[1], sfmt);
	void (*mask)(unsigned *dest, const byte *src, const unsigned *map, byte xor1) = GetMaskProc(row[1], sfmt);
	int wd = dr.Width(), w3 = 3 * wd, w4 = 4 * wd, dsh = 3 * dr.left;
	Buffer<unsigned> sum_buffer(w4);
	int count = col[0];
	int segment = col[1];
	Buffer<unsigned> row_buffers(w4 * segment);
	int first = col[2], full = 0;
	const unsigned *offsets = col.GetIter(2);
	int offset = 0;
	for(int y = dr.top; --count >= 0; y++)
	{
		int endoff = (offset += *offsets++) + segment;
		for(int next = first + full; next < endoff; next++)
		{
			if(full >= segment)
				first++;
			else
				full++;
			unsigned *out = &row_buffers[next % segment * w4];
			mask(out, smptr -> GetDownScan(next), row.Begin(), xor1);
			pixel(out + wd, src.GetDownScan(next), pix_row.Begin(), pal);
		}
		while(first > offset)
		{
			if(full < segment)
				full++;
			unsigned *out = &row_buffers[--first % segment * w4];
			mask(out, smptr -> GetDownScan(first), row.Begin(), xor1);
			pixel(out + wd, src.GetDownScan(first), pix_row.Begin(), pal);
		}
		ASSERT(offset >= first && endoff <= first + full);
		byte *dp = dest.GetDownScan(y) + dsh;
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
	}
}

//////////////////////////////////////////////////////////////////////

void CopyAntiAliasMaskOut(RawImage& dest, RawImage& dmask, const Rect& dest_rc, const RawImage& src, const RawImage& smask, const Rect& src_rc, bool merge, const Rect& clip)
{
	AA_TIMING("CopyAntiAliasMaskOut");
	
//	LOG("CopyAntiAlias: " << dest_rc << " <- " << src_rc);
	ASSERT(dest.bpp == 24 && (src.bpp == 24 || src.bpp == 8 || src.bpp == 1) && (src.bpp == 1 || smask.bpp == 8));
	SOURCE_FORMAT sfmt = (src.bpp == 24 ? SRGB : src.bpp == 8 ? SPAL : SBIT);
	byte xor1 = 0;
	const RawImage *smptr = &smask;
	if(sfmt == SBIT)
	{
		smptr = &src;
		Color c0 = smask.palette.GetCount() >= 1 ? smask.palette[0] : Black;
		Color c1 = smask.palette.GetCount() >= 2 ? smask.palette[1] : White;
		Rect dnorm = dest_rc.Normalized();
		if(IsNull(c0) && IsNull(c1))
		{
			if(!merge)
			{
				dest.Set(dnorm, Color(0, 0, 0));
				dmask.Set(dnorm, 1);
			}
			return;
		}
		if(!IsNull(c0) && !IsNull(c1))
		{ // no mask
			CopyAntiAliasRaw(dest, dest_rc, src, smask.palette, src_rc, clip);
			dmask.Set(dnorm, 0);
			return;
		}
		xor1 = IsNull(c0) ? 0 : 255;
	}
	Rect cl = Nvl(clip, dest.GetRect());
	Rect dr = dest_rc;
	Vector<unsigned> row = AAGetMap(dr.left, dr.right, cl.left, cl.right, src_rc.left, src_rc.right, 0, min(src.size.cx, smptr -> size.cx), 1, 0x100);
	if(row.IsEmpty())
		return;
	Vector<unsigned> col = AAGetMap(dr.top, dr.bottom, cl.top, cl.bottom, src_rc.top, src_rc.bottom, 0, min(src.size.cy, smptr -> size.cy), 1, 0x100);
	if(col.IsEmpty())
		return;
	
	Buffer<byte> pal(sfmt == SRGB ? 1 : 0x300);
	Vector<unsigned> pix_row;
	pix_row <<= row;

	if(sfmt == SRGB)
	{ // multiply pixel offsets by 3
		unsigned c = pix_row[0], s = pix_row[1] + 1;
		for(unsigned *p = pix_row.GetIter(2); c; c--, p += s)
			*p *= 3;
	}
	else
		CvtPalette(pal, sfmt == SBIT ? smask.palette : src.palette);

	void (*mrow)(byte *dpixel, byte *dmask, const unsigned *pixels, const unsigned *weights, unsigned count)
		= merge ? BltAAFixMaskOutMergeRow : BltAAFixMaskOutRow;
	void (*many)(byte *dpixel, byte *dmask, const unsigned *pixels, const unsigned *weights, unsigned count)
		= merge ? BltAAFixMaskOutMergeAny : BltAAFixMaskOutAny;
	
	void (*pixel)(unsigned *dest, const byte *src, const unsigned *map, const byte *pal) = GetPixelProc(row[1], sfmt);
	void (*mask)(unsigned *dest, const byte *src, const unsigned *map, byte xor1) = GetMaskProc(row[1], sfmt);
	int wd = dr.Width(), w3 = 3 * wd, w4 = 4 * wd, dsh = 3 * dr.left;
	Buffer<unsigned> sum_buffer(w4);
	int count = col[0];
	int segment = col[1];
	Buffer<unsigned> row_buffers(w4 * segment);
	int first = col[2], full = 0;
	const unsigned *offsets = col.GetIter(2);
	int offset = 0;
	for(int y = dr.top; --count >= 0; y++)
	{
		int endoff = (offset += *offsets++) + segment;
		for(int next = first + full; next < endoff; next++)
		{
			if(full >= segment)
				first++;
			else
				full++;
			unsigned *out = &row_buffers[next % segment * w4];
			mask(out, smptr -> GetDownScan(next), row.Begin(), xor1);
			pixel(out + wd, src.GetDownScan(next), pix_row.Begin(), pal);
		}
		while(first > offset)
		{
			if(full < segment)
				full++;
			unsigned *out = &row_buffers[--first % segment * w4];
			mask(out, smptr -> GetDownScan(first), row.Begin(), xor1);
			pixel(out + wd, src.GetDownScan(first), pix_row.Begin(), pal);
		}
		ASSERT(offset >= first && endoff <= first + full);
		byte *dp = dest.GetDownScan(y) + dsh;
		byte *dm = dmask.GetDownScan(y) + dr.left;
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
		}
	}
}

//////////////////////////////////////////////////////////////////////


#endif
