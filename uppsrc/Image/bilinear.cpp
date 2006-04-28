#include "Image.h"
#pragma hdrstop

#ifndef NEWIMAGE

#include <Draw/PixelUtil.h>

struct SideSegment
{
	Point64 from16;
	Size64  dfrom16;
	int64   to16;
	int64   dto16;

	void  operator ++ ()      { from16 += dfrom16; to16 += dto16; }
	void  operator += (int64 i) { from16 += i * dfrom16; to16 += i * dto16; }
};

static SideSegment CalcSideSegment(Point d1, Point d2, Point s1, Point s2, int y1)
{
	if(d2.y < d1.y)
	{
		Swap(d1, d2);
		Swap(s1, s2);
	}
	SideSegment sgm;
	int ht = max<int>(d2.y - d1.y, 1);
	sgm.to16 = ((int64)d1.x << 16) + 0x8000;
	sgm.dto16 = (int64)(d2.x - d1.x) * 0x10000 / ht;
	sgm.from16 = ((Point64)s1 << 16) + (int64)0x8000;
	sgm.dfrom16 = (Size64)(s2 - s1) * (int64)0x10000 / (int64)ht;
	if((y1 -= d1.y) > 0)
	{
		sgm.to16 += y1 * sgm.dto16;
		sgm.from16 += (int64)y1 * sgm.dfrom16;
	}
	return sgm;
}

static void PixelMapSet(PixelArray& pixel, PixelArray& alpha, Color color, SideSegment& left, SideSegment& right, Rect clip)
{
	if(IsNull(color)) color = Black();
	char bgr[3] = { color.GetB(), color.GetG(), color.GetR() };
	char mask = IsNull(color) ? 1 : 0;
	for(int y = clip.top; y < clip.bottom; ++y, ++left, ++right) {
		int dl = (int)(left.to16 >> 16), n = (int)((right.to16 >> 16) - dl + 1);
		if(n <= 0)
			continue;
		if(dl < clip.left) {
			n += dl - clip.left;
			dl = clip.left;
		}
		if(dl + n > clip.right)
			n = clip.right - dl;
		if(!!pixel) memsetex(pixel.GetDownScan(y) + 3 * dl, bgr, 3, n);
		if(!!alpha) memset(alpha.GetDownScan(y) + dl, mask, n);
	}
}

#define BuildSegmentProc(NAME, INITPAL, PIXELPROC, MASKPROC, DESTBYPP) \
static void NAME(PixelArray& dpixel, PixelArray& dalpha, const PixelArray& pixel, const PixelArray& alpha, SideSegment& left, SideSegment& right, Rect clip) \
{ \
	int prw = pixel.GetDownRowBytes(), arw = alpha.GetDownRowBytes(); \
	const byte *p0 = pixel.GetDownScan(0); \
	const byte *a0 = alpha.GetDownScan(0); \
	int xmax = (pixel.GetWidth() << 16) - 0x10001; \
	int ymax = (pixel.GetHeight() << 16) - 0x10001; \
	INITPAL() \
	for(int y = clip.top; y < clip.bottom; ++y, ++left, ++right) \
	{ \
		int dl = (int)(left.to16 >> 16), n = (int)((right.to16 >> 16) - dl + 1); \
		if(n <= 0) \
			continue; \
		Point64 sl16 = left.from16, sd16 = (right.from16 - left.from16) / (int64)n; \
		if(dl < clip.left) \
		{ \
			n += dl - clip.left; \
			sl16 += sd16 * (int64)(clip.left - dl); \
			dl = clip.left; \
		} \
		if(dl + n > clip.right) \
			n = clip.right - dl; \
		for(byte *d = dpixel.GetDownScan(y) + DESTBYPP * dl, *a = dalpha.GetDownScan(y) + dl; \
			--n >= 0; d += DESTBYPP, a++, sl16 += sd16) \
		{ \
			int x = (int)minmax<int64>(sl16.x, 0, xmax); \
			int y = (int)minmax<int64>(sl16.y, 0, ymax); \
			MASKPROC() \
			PIXELPROC \
		} \
	} \
}

#define CALCSHIFT(O, S) \
	row1 = byte(bgr00 >> S) * 256 + (byte(bgr01 >> S) - byte(bgr00 >> S)) * byte(x >> 8); \
	row2 = byte(bgr10 >> S) * 256 + (byte(bgr11 >> S) - byte(bgr10 >> S)) * byte(x >> 8); \
	d[O] = (row1 * 256 + ((row2 - row1) * byte(y >> 8))) >> 16;

#define CALCSHIFTMASK(O, S) \
	row1 = byte(bgr00 >> S) * mask00 + byte(bgr01 >> S) * mask01; \
	row2 = byte(bgr10 >> S) * mask10 + byte(bgr11 >> S) * mask11; \
	d[O] = (row1 + row2) / mask;

#define CALCHIGH(O, M, S) \
	row1 = byte(((bgr00 & M) << S) >> 24) * 256 + (byte(((bgr01 & M) << S) >> 24) - byte(((bgr00 & M) << S) >> 24)) * byte(x >> 8); \
	row2 = byte(((bgr10 & M) << S) >> 24) * 256 + (byte(((bgr11 & M) << S) >> 24) - byte(((bgr10 & M) << S) >> 24)) * byte(x >> 8); \
	d[O] = (row1 * 256 + ((row2 - row1) * byte(y >> 8))) >> 16;

#define CALCHIGHMASK(O, M, S) \
	row1 = byte(((bgr00 & M) << S) >> 24) * mask00 + byte(((bgr01 & M) << S) >> 24) * mask01; \
	row2 = byte(((bgr10 & M) << S) >> 24) * mask10 + byte(((bgr11 & M) << S) >> 24) * mask11; \
	d[O] = (row1 + row2) / mask;

#define CALCPART(o) \
	row1 = sa[o + 0] * 256 + ((sa[o + 3] - sa[o + 0]) * byte(x >> 8)); \
	row2 = sb[o + 0] * 256 + ((sb[o + 3] - sb[o + 0]) * byte(x >> 8)); \
	d[o] = (row1 * 256 + ((row2 - row1) * byte(y >> 8))) >> 16;

#define CALCPARTMASK(o) \
	row1 = sa[o + 0] * mask00 + sa[o + 3] * mask01; \
	row2 = sb[o + 0] * mask10 + sb[o + 3] * mask11; \
	d[o] = (row1 + row2) / mask;

#define PixelProc32(MASKOUT) \
	dword val = PeekIL(p0 + prw * (y >> 16) + 4 * (x >> 16)); \
	d[0] = (byte)(((val & bmask) << bshift24) >> 24); \
	d[1] = (byte)(((val & gmask) << gshift24) >> 24); \
	d[2] = (byte)(((val & rmask) << rshift24) >> 24); \
	MASKOUT()

#define PixelProc32Mask(MASKOUT) \
	if(!mask) { \
		dword val = PeekIL(p0 + prw * (y >> 16) + 4 * (x >> 16)); \
		d[0] = (byte)(((val & bmask) << bshift24) >> 24); \
		d[1] = (byte)(((val & gmask) << gshift24) >> 24); \
		d[2] = (byte)(((val & rmask) << rshift24) >> 24); \
		MASKOUT() \
	}

#define PixelProc32AA(MASKOUT) \
	const byte *pa = p0 + prw * (y >> 16) + 4 * (x >> 16), *pb = pa + prw; \
	dword bgr00 = PeekIL(pa), bgr01 = PeekIL(pa + 4); \
	dword bgr10 = PeekIL(pb), bgr11 = PeekIL(pb + 4); \
	int row1, row2; \
	CALCHIGH(0, bmask, bshift24) \
	CALCHIGH(1, gmask, gshift24) \
	CALCHIGH(2, rmask, rshift24) \
	MASKOUT()

#define PixelProc32AAMask(MASKOUT) \
	if(mask >= 0x8000) { \
		const byte *pa = p0 + prw * (y >> 16) + 4 * (x >> 16), *pb = pa + prw; \
		dword bgr00 = PeekIL(pa), bgr01 = PeekIL(pa + 4); \
		dword bgr10 = PeekIL(pb), bgr11 = PeekIL(pb + 4); \
		int row1, row2; \
		CALCHIGHMASK(0, bmask, bshift24) \
		CALCHIGHMASK(1, gmask, gshift24) \
		CALCHIGHMASK(2, rmask, rshift24) \
		MASKOUT() \
	}

#define PixelProc24(MASKOUT) \
	const byte *s = p0 + prw * (y >> 16) + 3 * (x >> 16); \
	d[0] = s[0]; \
	d[1] = s[1]; \
	d[2] = s[2]; \
	MASKOUT()

#define PixelProc24Mask(MASKOUT) \
	if(!mask) { \
		const byte *s = p0 + prw * (y >> 16) + 3 * (x >> 16); \
		d[0] = s[0]; \
		d[1] = s[1]; \
		d[2] = s[2]; \
		MASKOUT() \
	}

#define PixelProc24AA(MASKOUT) \
	const byte *sa = p0 + prw * (y >> 16) + 3 * (x >> 16), *sb = sa + prw; \
	int row1, row2; CALCPART(0) CALCPART(1) CALCPART(2) MASKOUT()

#define PixelProc24AAMask(MASKOUT) \
	if(mask >= 0x8000) { \
		const byte *sa = p0 + prw * (y >> 16) + 3 * (x >> 16), *sb = sa + prw; \
		int row1, row2; CALCPARTMASK(0) CALCPARTMASK(1) CALCPARTMASK(2) MASKOUT() \
	}

#define PixelProc16(MASKOUT) \
	dword val = (word)PeekIW(p0 + prw * (y >> 16) + 2 * (x >> 16)); \
	d[0] = (byte)(((val & bmask) << bshift24) >> 24); \
	d[1] = (byte)(((val & gmask) << gshift24) >> 24); \
	d[2] = (byte)(((val & rmask) << rshift24) >> 24); \
	MASKOUT()

#define PixelProc16Mask(MASKOUT) \
	if(!mask) { \
		dword val = (word)PeekIW(p0 + prw * (y >> 16) + 2 * (x >> 16)); \
		d[0] = (byte)(((val & bmask) << bshift24) >> 24); \
		d[1] = (byte)(((val & gmask) << gshift24) >> 24); \
		d[2] = (byte)(((val & rmask) << rshift24) >> 24); \
		MASKOUT() \
	}

#define PixelProc16AA(MASKOUT) \
	const byte *pa = p0 + prw * (y >> 16) + 2 * (x >> 16), *pb = pa + prw; \
	dword bgr00 = (word)PeekIW(pa), bgr01 = (word)PeekIW(pa + 2); \
	dword bgr10 = (word)PeekIW(pb), bgr11 = (word)PeekIW(pb + 2); \
	int row1, row2; \
	CALCHIGH(0, bmask, bshift24) \
	CALCHIGH(1, gmask, gshift24) \
	CALCHIGH(2, rmask, rshift24) \
	MASKOUT()

#define PixelProc16AAMask(MASKOUT) \
	if(mask >= 0x8000) { \
		const byte *pa = p0 + prw * (y >> 16) + 2 * (x >> 16), *pb = pa + prw; \
		dword bgr00 = (word)PeekIW(pa), bgr01 = (word)PeekIW(pa + 2); \
		dword bgr10 = (word)PeekIW(pb), bgr11 = (word)PeekIW(pb + 2); \
		int row1, row2; \
		CALCHIGHMASK(0, bmask, bshift24) \
		CALCHIGHMASK(1, gmask, gshift24) \
		CALCHIGHMASK(2, rmask, rshift24) \
		MASKOUT() \
	}

#define PixelProc8(MASKOUT) \
	unsigned bgr = pal[(int)p0[prw * (y >> 16) + (x >> 16)]].GetRaw(); \
	d[0] = bgr >> 16; \
	d[1] = bgr >> 8; \
	d[2] = bgr >> 0; \
	MASKOUT()

#define PixelProc8Mask(MASKOUT) \
	if(!mask) { \
		unsigned bgr = pal[(int)p0[prw * (y >> 16) + (x >> 16)]].GetRaw(); \
		d[0] = bgr >> 16; \
		d[1] = bgr >> 8; \
		d[2] = bgr >> 0; \
		MASKOUT() \
	}

#define PixelProc8AA(MASKOUT) \
	const byte *sa = p0 + prw * (y >> 16) + (x >> 16); \
	unsigned bgr00 = pal[(int)sa[0]].GetRaw(); \
	unsigned bgr01 = pal[(int)sa[1]].GetRaw(); \
	unsigned bgr10 = pal[(int)sa[prw + 0]].GetRaw(); \
	unsigned bgr11 = pal[(int)sa[prw + 1]].GetRaw(); \
	int row1, row2; CALCSHIFT(0, 16) CALCSHIFT(1, 8) CALCSHIFT(2, 0) MASKOUT()

#define PixelProc8AAMask(MASKOUT) \
	if(mask >= 0x8000) { \
		const byte *sa = p0 + prw * (y >> 16) + (x >> 16); \
		unsigned bgr00 = pal[(int)sa[0]].GetRaw(); \
		unsigned bgr01 = pal[(int)sa[1]].GetRaw(); \
		unsigned bgr10 = pal[(int)sa[prw + 0]].GetRaw(); \
		unsigned bgr11 = pal[(int)sa[prw + 1]].GetRaw(); \
		int row1, row2; CALCSHIFTMASK(0, 16) CALCSHIFTMASK(1, 8) CALCSHIFTMASK(2, 0) MASKOUT() \
	}

#define PixelProc4(MASKOUT) \
	unsigned bgr = pal[15 & (p0[prw * (y >> 16) + (x >> 17)] >> (x & 0x10000 ? 0 : 4))].GetRaw(); \
	d[0] = bgr >> 16; \
	d[1] = bgr >> 8; \
	d[2] = bgr >> 0; \
	MASKOUT()

#define PixelProc4Mask(MASKOUT) \
	if(!mask) { \
		unsigned bgr = pal[15 & (p0[prw * (y >> 16) + (x >> 17)] >> (x & 0x10000 ? 0 : 4))].GetRaw(); \
		d[0] = bgr >> 16; \
		d[1] = bgr >> 8; \
		d[2] = bgr >> 0; \
		MASKOUT() \
	}

#define PixelProc4AA(MASKOUT) \
	const byte *sa = p0 + prw * (y >> 16) + (x >> 17); \
	byte b00, b01, b10, b11; \
	if(x & 0x10000) \
	{ b00 = sa[0] & 15; b01 = sa[1] >> 4; b10 = sa[prw] & 15; b11 = sa[prw + 1] >> 4; } \
	else \
	{ b00 = sa[0] >> 4; b01 = sa[0] & 15; b10 = sa[prw] >> 4; b11 = sa[prw] & 15; } \
	unsigned bgr00 = pal[int(b00)].GetRaw(); \
	unsigned bgr01 = pal[int(b01)].GetRaw(); \
	unsigned bgr10 = pal[int(b10)].GetRaw(); \
	unsigned bgr11 = pal[int(b11)].GetRaw(); \
	int row1, row2; CALCSHIFT(0, 16) CALCSHIFT(1, 8) CALCSHIFT(2, 0) MASKOUT()

#define PixelProc4AAMask(MASKOUT) \
	if(mask >= 0x8000) { \
		const byte *sa = p0 + prw * (y >> 16) + (x >> 17); \
		byte b00, b01, b10, b11; \
		if(x & 0x10000) \
		{ b00 = sa[0] & 15; b01 = sa[1] >> 4; b10 = sa[prw] & 15; b11 = sa[prw + 1] >> 4; } \
		else \
		{ b00 = sa[0] >> 4; b01 = sa[0] & 15; b10 = sa[prw] >> 4; b11 = sa[prw] & 15; } \
		unsigned bgr00 = pal[int(b00)].GetRaw(); \
		unsigned bgr01 = pal[int(b01)].GetRaw(); \
		unsigned bgr10 = pal[int(b10)].GetRaw(); \
		unsigned bgr11 = pal[int(b11)].GetRaw(); \
		int row1, row2; CALCSHIFTMASK(0, 16) CALCSHIFTMASK(1, 8) CALCSHIFTMASK(2, 0) MASKOUT() \
	}

#define PixelProc1(MASKOUT) \
	unsigned bgr = pal[1 & (p0[prw * (y >> 16) + (x >> 19)] >> ((~x >> 16) & 7))].GetRaw(); \
	d[0] = bgr >> 16; \
	d[1] = bgr >> 8; \
	d[2] = bgr >> 0; \
	MASKOUT()

#define PixelProc1Mask(MASKOUT) \
	if(!mask) { \
		unsigned bgr = pal[1 & (p0[prw * (y >> 16) + (x >> 19)] >> ((~x >> 16) & 7))].GetRaw(); \
		d[0] = bgr >> 16; \
		d[1] = bgr >> 8; \
		d[2] = bgr >> 0; \
		MASKOUT() \
	}

#define PixelProc1AA(MASKOUT) \
	const byte *pa = p0 + prw * (y >> 16) + (x >> 19); \
	byte b00, b01, b10, b11; \
	int shift = (~x >> 16) & 7; \
	if(shift) \
	{ b00 = (pa[0] >> shift) & 1; b01 = (pa[0] >> (shift - 1)) & 1; b10 = (pa[prw] >> shift) & 1; b11 = (pa[prw] >> (shift - 1)) & 1; } \
	else \
	{ b00 = pa[0] & 1; b01 = pa[1] >> 7; b10 = pa[prw] & 1; b11 = pa[prw + 1] >> 7; } \
	unsigned bgr00 = pal[int(b00)].GetRaw(); \
	unsigned bgr01 = pal[int(b01)].GetRaw(); \
	unsigned bgr10 = pal[int(b10)].GetRaw(); \
	unsigned bgr11 = pal[int(b11)].GetRaw(); \
	int row1, row2; CALCSHIFT(0, 16) CALCSHIFT(1, 8) CALCSHIFT(2, 0) MASKOUT()

#define PixelProc1AAMask(MASKOUT) \
	if(mask >= 0x8000) { \
		const byte *pa = p0 + prw * (y >> 16) + (x >> 19); \
		byte b00, b01, b10, b11; \
		int shift = (~x >> 16) & 7; \
		if(shift) \
		{ b00 = (pa[0] >> shift) & 1; b01 = (pa[0] >> (shift - 1)) & 1; b10 = (pa[prw] >> shift) & 1; b11 = (pa[prw] >> (shift - 1)) & 1; } \
		else \
		{ b00 = pa[0] & 1; b01 = pa[1] >> 7; b10 = pa[prw] & 1; b11 = pa[prw + 1] >> 7; } \
		unsigned bgr00 = pal[int(b00)].GetRaw(); \
		unsigned bgr01 = pal[int(b01)].GetRaw(); \
		unsigned bgr10 = pal[int(b10)].GetRaw(); \
		unsigned bgr11 = pal[int(b11)].GetRaw(); \
		int row1, row2; CALCSHIFTMASK(0, 16) CALCSHIFTMASK(1, 8) CALCSHIFTMASK(2, 0) MASKOUT() \
	}

#define MaskProc8() byte mask = a0[arw * (y >> 16) + (x >> 16)];

#define MaskProc8AA() \
	const byte *aa = &a0[arw * (y >> 16) + (x >> 16)], *ab = aa + arw; \
	int lx = byte(x >> 8), rx = 0x100 - lx, ly = byte(y >> 8), ry = 0x100 - ly; \
	int mask00 = aa[0] ? 0 : rx * ry; \
	int mask01 = aa[1] ? 0 : lx * ry; \
	int mask10 = ab[0] ? 0 : rx * ly; \
	int mask11 = ab[1] ? 0 : lx * ly; \
	int mask = mask00 + mask01 + mask10 + mask11;

#define MaskProc1() byte mask = (a0[arw * (y >> 16) + (x >> 19)] >> ((~x >> 16) & 7)) & 1; \

#define MaskProc1AA() \
	const byte *aa = a0 + arw * (y >> 16) + (x >> 19); \
	int mask00, mask01, mask10, mask11; \
	int shift = (~x >> 16) & 7; \
	if(shift) \
	{ mask00 = (aa[0] >> shift) & 1; mask01 = (aa[0] >> (shift - 1)) & 1; mask10 = (aa[arw] >> shift) & 1; mask11 = (aa[arw] >> (shift - 1)) & 1; } \
	else \
	{ mask00 = aa[0] & 1; mask01 = aa[1] >> 7; mask10 = aa[arw] & 1; mask11 = aa[arw + 1] >> 7; } \
	int lx = byte(x >> 8), rx = 0x100 - lx, ly = byte(y >> 8), ry = 0x100 - ly; \
	mask00 = (mask00 ? 0 : rx * ry); \
	mask01 = (mask01 ? 0 : lx * ry); \
	mask10 = (mask10 ? 0 : rx * ly); \
	mask11 = (mask11 ? 0 : lx * ly); \
	int mask = mask00 + mask01 + mask10 + mask11;

#define NopProc() ;

#define MaskOutProc() *a = 0;

#define InitPal24() ;

#define InitPal32() \
	dword rmask = pixel.cmask[0], gmask = pixel.cmask[1], bmask = pixel.cmask[2]; \
	byte rshift24 = pixel.cshift24[0], gshift24 = pixel.cshift24[1], bshift24 = pixel.cshift24[2];

#define InitPal8() const Color *pal = pixel.palette.Begin();

BuildSegmentProc(PixelMapSegment32,   InitPal32, PixelProc32(NopProc),   NopProc, 3)
BuildSegmentProc(PixelMapSegment32AA, InitPal32, PixelProc32AA(NopProc), NopProc, 3)
BuildSegmentProc(PixelMapSegment24,   InitPal24, PixelProc24(NopProc),   NopProc, 3)
BuildSegmentProc(PixelMapSegment24AA, InitPal24, PixelProc24AA(NopProc), NopProc, 3)
BuildSegmentProc(PixelMapSegment16,   InitPal32, PixelProc16(NopProc),   NopProc, 3)
BuildSegmentProc(PixelMapSegment16AA, InitPal32, PixelProc16AA(NopProc), NopProc, 3)
BuildSegmentProc(PixelMapSegment8,    InitPal8,  PixelProc8(NopProc),    NopProc, 3)
BuildSegmentProc(PixelMapSegment8AA,  InitPal8,  PixelProc8AA(NopProc),  NopProc, 3)
BuildSegmentProc(PixelMapSegment4,    InitPal8,  PixelProc4(NopProc),    NopProc, 3)
BuildSegmentProc(PixelMapSegment4AA,  InitPal8,  PixelProc4AA(NopProc),  NopProc, 3)
BuildSegmentProc(PixelMapSegment1,    InitPal8,  PixelProc1(NopProc),    NopProc, 3)
BuildSegmentProc(PixelMapSegment1AA,  InitPal8,  PixelProc1AA(NopProc),  NopProc, 3)

BuildSegmentProc(PixelMapSegment32Mask0Out,   InitPal32, PixelProc32(MaskOutProc),   NopProc, 3)
BuildSegmentProc(PixelMapSegment32AAMask0Out, InitPal32, PixelProc32AA(MaskOutProc), NopProc, 3)
BuildSegmentProc(PixelMapSegment24Mask0Out,   InitPal24, PixelProc24(MaskOutProc),   NopProc, 3)
BuildSegmentProc(PixelMapSegment24AAMask0Out, InitPal24, PixelProc24AA(MaskOutProc), NopProc, 3)
BuildSegmentProc(PixelMapSegment16Mask0Out,   InitPal32, PixelProc16(MaskOutProc),   NopProc, 3)
BuildSegmentProc(PixelMapSegment16AAMask0Out, InitPal32, PixelProc16AA(MaskOutProc), NopProc, 3)
BuildSegmentProc(PixelMapSegment8Mask0Out,    InitPal8,  PixelProc8(MaskOutProc),    NopProc, 3)
BuildSegmentProc(PixelMapSegment8AAMask0Out,  InitPal8,  PixelProc8AA(MaskOutProc),  NopProc, 3)
BuildSegmentProc(PixelMapSegment4Mask0Out,    InitPal8,  PixelProc4(MaskOutProc),    NopProc, 3)
BuildSegmentProc(PixelMapSegment4AAMask0Out,  InitPal8,  PixelProc4AA(MaskOutProc),  NopProc, 3)
BuildSegmentProc(PixelMapSegment1Mask0Out,    InitPal8,  PixelProc1(MaskOutProc),    NopProc, 3)
BuildSegmentProc(PixelMapSegment1AAMask0Out,  InitPal8,  PixelProc1AA(MaskOutProc),  NopProc, 3)

BuildSegmentProc(PixelMapSegment32Mask8,   InitPal32, PixelProc32Mask(NopProc),   MaskProc8,   3)
BuildSegmentProc(PixelMapSegment32AAMask8, InitPal32, PixelProc32AAMask(NopProc), MaskProc8AA, 3)
BuildSegmentProc(PixelMapSegment24Mask8,   InitPal24, PixelProc24Mask(NopProc),   MaskProc8,   3)
BuildSegmentProc(PixelMapSegment24AAMask8, InitPal24, PixelProc24AAMask(NopProc), MaskProc8AA, 3)
BuildSegmentProc(PixelMapSegment16Mask8,   InitPal32, PixelProc16Mask(NopProc),   MaskProc8,   3)
BuildSegmentProc(PixelMapSegment16AAMask8, InitPal32, PixelProc16AAMask(NopProc), MaskProc8AA, 3)
BuildSegmentProc(PixelMapSegment8Mask8,    InitPal8,  PixelProc8Mask(NopProc),    MaskProc8,   3)
BuildSegmentProc(PixelMapSegment8AAMask8,  InitPal8,  PixelProc8AAMask(NopProc),  MaskProc8AA, 3)
BuildSegmentProc(PixelMapSegment4Mask8,    InitPal8,  PixelProc4Mask(NopProc),    MaskProc8,   3)
BuildSegmentProc(PixelMapSegment4AAMask8,  InitPal8,  PixelProc4AAMask(NopProc),  MaskProc8AA, 3)
BuildSegmentProc(PixelMapSegment1Mask8,    InitPal8,  PixelProc1Mask(NopProc),    MaskProc8,   3)
BuildSegmentProc(PixelMapSegment1AAMask8,  InitPal8,  PixelProc1AAMask(NopProc),  MaskProc8AA, 3)

BuildSegmentProc(PixelMapSegment32Mask1,   InitPal32, PixelProc32Mask(NopProc),   MaskProc1,   3)
BuildSegmentProc(PixelMapSegment32AAMask1, InitPal32, PixelProc32AAMask(NopProc), MaskProc1AA, 3)
BuildSegmentProc(PixelMapSegment24Mask1,   InitPal24, PixelProc24Mask(NopProc),   MaskProc1,   3)
BuildSegmentProc(PixelMapSegment24AAMask1, InitPal24, PixelProc24AAMask(NopProc), MaskProc1AA, 3)
BuildSegmentProc(PixelMapSegment16Mask1,   InitPal32, PixelProc16Mask(NopProc),   MaskProc1,   3)
BuildSegmentProc(PixelMapSegment16AAMask1, InitPal32, PixelProc16AAMask(NopProc), MaskProc1AA, 3)
BuildSegmentProc(PixelMapSegment8Mask1,    InitPal8,  PixelProc8Mask(NopProc),    MaskProc1,   3)
BuildSegmentProc(PixelMapSegment8AAMask1,  InitPal8,  PixelProc8AAMask(NopProc),  MaskProc1AA, 3)
BuildSegmentProc(PixelMapSegment4Mask1,    InitPal8,  PixelProc4Mask(NopProc),    MaskProc1,   3)
BuildSegmentProc(PixelMapSegment4AAMask1,  InitPal8,  PixelProc4AAMask(NopProc),  MaskProc1AA, 3)
BuildSegmentProc(PixelMapSegment1Mask1,    InitPal8,  PixelProc1Mask(NopProc),    MaskProc1,   3)
BuildSegmentProc(PixelMapSegment1AAMask1,  InitPal8,  PixelProc1AAMask(NopProc),  MaskProc1AA, 3)

BuildSegmentProc(PixelMapSegment32Mask8Out,   InitPal32, PixelProc32Mask(MaskOutProc),   MaskProc8,   3)
BuildSegmentProc(PixelMapSegment32AAMask8Out, InitPal32, PixelProc32AAMask(MaskOutProc), MaskProc8AA, 3)
BuildSegmentProc(PixelMapSegment24Mask8Out,   InitPal24, PixelProc24Mask(MaskOutProc),   MaskProc8,   3)
BuildSegmentProc(PixelMapSegment24AAMask8Out, InitPal24, PixelProc24AAMask(MaskOutProc), MaskProc8AA, 3)
BuildSegmentProc(PixelMapSegment16Mask8Out,   InitPal32, PixelProc16Mask(MaskOutProc),   MaskProc8,   3)
BuildSegmentProc(PixelMapSegment16AAMask8Out, InitPal32, PixelProc16AAMask(MaskOutProc), MaskProc8AA, 3)
BuildSegmentProc(PixelMapSegment8Mask8Out,    InitPal8,  PixelProc8Mask(MaskOutProc),    MaskProc8,   3)
BuildSegmentProc(PixelMapSegment8AAMask8Out,  InitPal8,  PixelProc8AAMask(MaskOutProc),  MaskProc8AA, 3)
BuildSegmentProc(PixelMapSegment4Mask8Out,    InitPal8,  PixelProc4Mask(MaskOutProc),    MaskProc8,   3)
BuildSegmentProc(PixelMapSegment4AAMask8Out,  InitPal8,  PixelProc4AAMask(MaskOutProc),  MaskProc8AA, 3)
BuildSegmentProc(PixelMapSegment1Mask8Out,    InitPal8,  PixelProc1Mask(MaskOutProc),    MaskProc8,   3)
BuildSegmentProc(PixelMapSegment1AAMask8Out,  InitPal8,  PixelProc1AAMask(MaskOutProc),  MaskProc8AA, 3)

BuildSegmentProc(PixelMapSegment32Mask1Out,   InitPal32, PixelProc32Mask(MaskOutProc),   MaskProc1,   3)
BuildSegmentProc(PixelMapSegment32AAMask1Out, InitPal32, PixelProc32AAMask(MaskOutProc), MaskProc1AA, 3)
BuildSegmentProc(PixelMapSegment24Mask1Out,   InitPal24, PixelProc24Mask(MaskOutProc),   MaskProc1,   3)
BuildSegmentProc(PixelMapSegment24AAMask1Out, InitPal24, PixelProc24AAMask(MaskOutProc), MaskProc1AA, 3)
BuildSegmentProc(PixelMapSegment16Mask1Out,   InitPal32, PixelProc16Mask(MaskOutProc),   MaskProc1,   3)
BuildSegmentProc(PixelMapSegment16AAMask1Out, InitPal32, PixelProc16AAMask(MaskOutProc), MaskProc1AA, 3)
BuildSegmentProc(PixelMapSegment8Mask1Out,    InitPal8,  PixelProc8Mask(MaskOutProc),    MaskProc1,   3)
BuildSegmentProc(PixelMapSegment8AAMask1Out,  InitPal8,  PixelProc8AAMask(MaskOutProc),  MaskProc1AA, 3)
BuildSegmentProc(PixelMapSegment4Mask1Out,    InitPal8,  PixelProc4Mask(MaskOutProc),    MaskProc1,   3)
BuildSegmentProc(PixelMapSegment4AAMask1Out,  InitPal8,  PixelProc4AAMask(MaskOutProc),  MaskProc1AA, 3)
BuildSegmentProc(PixelMapSegment1Mask1Out,    InitPal8,  PixelProc1Mask(MaskOutProc),    MaskProc1,   3)
BuildSegmentProc(PixelMapSegment1AAMask1Out,  InitPal8,  PixelProc1AAMask(MaskOutProc),  MaskProc1AA, 3)

#define PixelProcMask() d[0] &= mask;

#define PixelProcMaskAA() if(mask >= 0x8000) d[0] = 0;

BuildSegmentProc(PixelMapSegmentMask8,   NopProc, PixelProcMask(),   MaskProc8,   1)
BuildSegmentProc(PixelMapSegmentMask8AA, NopProc, PixelProcMaskAA(), MaskProc8AA, 1)
BuildSegmentProc(PixelMapSegmentMask1,   NopProc, PixelProcMask(),   MaskProc1,   1)
BuildSegmentProc(PixelMapSegmentMask1AA, NopProc, PixelProcMaskAA(), MaskProc1AA, 1)

/*
static void PixelMapSegmentAntiAlias(PixelArray& dest, const PixelArray& src, SideSegment& left, SideSegment& right, Rect clip)
{
	const byte *simg = src.GetDownScan(0);
	int srw = src.GetDownRowBytes();
	int xmax = (src.size.cx << 16) - 0x10001;
	int ymax = (src.size.cy << 16) - 0x10001;
	for(int ry = clip.top; ry < clip.bottom; ++ry, ++left, ++right)
	{
		int dl = left.to16 >> 16, n = (right.to16 >> 16) - dl + 1;
		if(n <= 0)
			continue;
		Point sl16 = left.from16, sd16 = (right.from16 - left.from16) / n;
		if(dl < clip.left)
		{
			n += dl - clip.left;
			sl16 += sd16 * (clip.left - dl);
			dl = clip.left;
		}
		if(dl + n > clip.right)
			n = clip.right - dl;
		for(byte *d = dest.GetDownScan(ry) + 3 * dl; --n >= 0; d += 3, sl16 += sd16)
		{
			int x = min(sl16.x, xmax);
			int y = min(sl16.y, ymax);
		}
	}
}
*/

typedef void (*SegmentProc)(PixelArray& dpixel, PixelArray& dalpha,
	const PixelArray& spixel, const PixelArray& salpha, SideSegment& left, SideSegment& right, Rect clip);

enum MASKSTYLE { PIXELONLY, MASKONLY, MASKIN, MASKOUT };
#define PAS(p, a, s, aa) (p) * 1024 + (a) * 16 + (s) * 2 + (aa ? 1 : 0)

static SegmentProc GetSegmentProc(int pxbpp, int albpp, bool antialias, MASKSTYLE style)
{
	switch(PAS(pxbpp, albpp, style, antialias)) {
	case PAS(32, 0, PIXELONLY, false): return &PixelMapSegment32;
	case PAS(24, 0, PIXELONLY, false): return &PixelMapSegment24;
	case PAS(16, 0, PIXELONLY, false): return &PixelMapSegment16;
	case PAS( 8, 0, PIXELONLY, false): return &PixelMapSegment8;
	case PAS( 4, 0, PIXELONLY, false): return &PixelMapSegment4;
	case PAS( 1, 0, PIXELONLY, false): return &PixelMapSegment1;

	case PAS(32, 0, PIXELONLY, true): return &PixelMapSegment32AA;
	case PAS(24, 0, PIXELONLY, true): return &PixelMapSegment24AA;
	case PAS(16, 0, PIXELONLY, true): return &PixelMapSegment16AA;
	case PAS( 8, 0, PIXELONLY, true): return &PixelMapSegment8AA;
	case PAS( 4, 0, PIXELONLY, true): return &PixelMapSegment4AA;
	case PAS( 1, 0, PIXELONLY, true): return &PixelMapSegment1AA;

	case PAS( 0, 8, MASKONLY, false): return &PixelMapSegmentMask8;
	case PAS( 0, 1, MASKONLY, false): return &PixelMapSegmentMask1;
	case PAS( 0, 8, MASKONLY, true): return &PixelMapSegmentMask8AA;
	case PAS( 0, 1, MASKONLY, true): return &PixelMapSegmentMask1AA;

	case PAS(32, 8, MASKIN, false): return &PixelMapSegment32Mask8;
	case PAS(24, 8, MASKIN, false): return &PixelMapSegment24Mask8;
	case PAS(16, 8, MASKIN, false): return &PixelMapSegment16Mask8;
	case PAS( 8, 8, MASKIN, false): return &PixelMapSegment8Mask8;
	case PAS( 4, 8, MASKIN, false): return &PixelMapSegment4Mask8;
	case PAS( 1, 8, MASKIN, false): return &PixelMapSegment1Mask8;

	case PAS(32, 8, MASKIN, true): return &PixelMapSegment32AAMask8;
	case PAS(24, 8, MASKIN, true): return &PixelMapSegment24AAMask8;
	case PAS(16, 8, MASKIN, true): return &PixelMapSegment16AAMask8;
	case PAS( 8, 8, MASKIN, true): return &PixelMapSegment8AAMask8;
	case PAS( 4, 8, MASKIN, true): return &PixelMapSegment4AAMask8;
	case PAS( 1, 8, MASKIN, true): return &PixelMapSegment1AAMask8;

	case PAS(32, 1, MASKIN, false): return &PixelMapSegment32Mask1;
	case PAS(24, 1, MASKIN, false): return &PixelMapSegment24Mask1;
	case PAS(16, 1, MASKIN, false): return &PixelMapSegment16Mask1;
	case PAS( 8, 1, MASKIN, false): return &PixelMapSegment8Mask1;
	case PAS( 4, 1, MASKIN, false): return &PixelMapSegment4Mask1;
	case PAS( 1, 1, MASKIN, false): return &PixelMapSegment1Mask1;

	case PAS(32, 1, MASKIN, true): return &PixelMapSegment32AAMask1;
	case PAS(24, 1, MASKIN, true): return &PixelMapSegment24AAMask1;
	case PAS(16, 1, MASKIN, true): return &PixelMapSegment16AAMask1;
	case PAS( 8, 1, MASKIN, true): return &PixelMapSegment8AAMask1;
	case PAS( 4, 1, MASKIN, true): return &PixelMapSegment4AAMask1;
	case PAS( 1, 1, MASKIN, true): return &PixelMapSegment1AAMask1;

	case PAS(32, 8, MASKOUT, false): return &PixelMapSegment32Mask8Out;
	case PAS(24, 8, MASKOUT, false): return &PixelMapSegment24Mask8Out;
	case PAS(16, 8, MASKOUT, false): return &PixelMapSegment16Mask8Out;
	case PAS( 8, 8, MASKOUT, false): return &PixelMapSegment8Mask8Out;
	case PAS( 4, 8, MASKOUT, false): return &PixelMapSegment4Mask8Out;
	case PAS( 1, 8, MASKOUT, false): return &PixelMapSegment1Mask8Out;

	case PAS(32, 8, MASKOUT, true): return &PixelMapSegment32AAMask8Out;
	case PAS(24, 8, MASKOUT, true): return &PixelMapSegment24AAMask8Out;
	case PAS(16, 8, MASKOUT, true): return &PixelMapSegment16AAMask8Out;
	case PAS( 8, 8, MASKOUT, true): return &PixelMapSegment8AAMask8Out;
	case PAS( 4, 8, MASKOUT, true): return &PixelMapSegment4AAMask8Out;
	case PAS( 1, 8, MASKOUT, true): return &PixelMapSegment1AAMask8Out;

	case PAS(32, 1, MASKOUT, false): return &PixelMapSegment32Mask1Out;
	case PAS(24, 1, MASKOUT, false): return &PixelMapSegment24Mask1Out;
	case PAS(16, 1, MASKOUT, false): return &PixelMapSegment16Mask1Out;
	case PAS( 8, 1, MASKOUT, false): return &PixelMapSegment8Mask1Out;
	case PAS( 4, 1, MASKOUT, false): return &PixelMapSegment4Mask1Out;
	case PAS( 1, 1, MASKOUT, false): return &PixelMapSegment1Mask1Out;

	case PAS(32, 1, MASKOUT, true): return &PixelMapSegment32AAMask1Out;
	case PAS(24, 1, MASKOUT, true): return &PixelMapSegment24AAMask1Out;
	case PAS(16, 1, MASKOUT, true): return &PixelMapSegment16AAMask1Out;
	case PAS( 8, 1, MASKOUT, true): return &PixelMapSegment8AAMask1Out;
	case PAS( 4, 1, MASKOUT, true): return &PixelMapSegment4AAMask1Out;
	case PAS( 1, 1, MASKOUT, true): return &PixelMapSegment1AAMask1Out;

	case PAS(32, 0, MASKOUT, false): return &PixelMapSegment32Mask0Out;
	case PAS(24, 0, MASKOUT, false): return &PixelMapSegment24Mask0Out;
	case PAS(16, 0, MASKOUT, false): return &PixelMapSegment16Mask0Out;
	case PAS( 8, 0, MASKOUT, false): return &PixelMapSegment8Mask0Out;
	case PAS( 4, 0, MASKOUT, false): return &PixelMapSegment4Mask0Out;
	case PAS( 1, 0, MASKOUT, false): return &PixelMapSegment1Mask0Out;

	case PAS(32, 0, MASKOUT, true): return &PixelMapSegment32AAMask0Out;
	case PAS(24, 0, MASKOUT, true): return &PixelMapSegment24AAMask0Out;
	case PAS(16, 0, MASKOUT, true): return &PixelMapSegment16AAMask0Out;
	case PAS( 8, 0, MASKOUT, true): return &PixelMapSegment8AAMask0Out;
	case PAS( 4, 0, MASKOUT, true): return &PixelMapSegment4AAMask0Out;
	case PAS( 1, 0, MASKOUT, true): return &PixelMapSegment1AAMask0Out;
	}

	return NULL;
}

void PixelMapLinearSet(PixelArray& dpixel, PixelArray& dalpha, Point d0, Point d1, Point d2, Color color, Rect clip)
{
	if(IsNull(clip))
		clip = dpixel.GetSize();
	clip &= dpixel.GetSize();
	if(clip.IsEmpty())
		return;
	if(d0.y > d1.y) Swap(d0, d1);
	if(d1.y > d2.y) { Swap(d1, d2); if(d0.y > d1.y) Swap(d0, d1); }
	if(d0.y > clip.top) clip.top = d0.y;
	if(d2.y < clip.bottom) clip.bottom = d2.y + 1;
	Rect clip01, clip12;
	clip01 = clip12 = clip;
	clip01.bottom = clip12.top = minmax(d1.y, clip.top, clip.bottom);
	SideSegment sg01 = CalcSideSegment(d0, d1, Point(0, 0), Point(0, 0), clip01.top);
	SideSegment sg12 = CalcSideSegment(d1, d2, Point(0, 0), Point(0, 0), clip12.top);
	SideSegment sg02 = CalcSideSegment(d0, d2, Point(0, 0), Point(0, 0), clip.top);
	if(sg01.dto16 < sg02.dto16) {
		PixelMapSet(dpixel, dalpha, color, sg01, sg02, clip01);
		PixelMapSet(dpixel, dalpha, color, sg12, sg02, clip12);
	}
	else {
		PixelMapSet(dpixel, dalpha, color, sg02, sg01, clip01);
		PixelMapSet(dpixel, dalpha, color, sg02, sg12, clip12);
	}
}

void PixelMapLinearRaw(PixelArray& dpixel, PixelArray& dalpha, Point d0, Point d1, Point d2,
	const PixelArray& spixel, const PixelArray& salpha, Point s0, Point s1, Point s2, Rect clip, SegmentProc segment)
{
	if(IsNull(clip))
		clip = dpixel.GetSize();
	clip &= dpixel.GetSize();
	if(clip.IsEmpty())
		return;
	if(d0.y > d1.y) { Swap(d0, d1); Swap(s0, s1); }
	if(d1.y > d2.y) { Swap(d1, d2); Swap(s1, s2); if(d0.y > d1.y) { Swap(d0, d1); Swap(s0, s1); } }
	if(d0.y > clip.top) clip.top = d0.y;
	if(d2.y < clip.bottom) clip.bottom = d2.y + 1;
	Rect clip01, clip12;
	clip01 = clip12 = clip;
	clip01.bottom = clip12.top = minmax(d1.y, clip.top, clip.bottom);
	SideSegment sg01 = CalcSideSegment(d0, d1, s0, s1, clip01.top);
	SideSegment sg12 = CalcSideSegment(d1, d2, s1, s2, clip12.top);
	SideSegment sg02 = CalcSideSegment(d0, d2, s0, s2, clip.top);
	if(sg01.dto16 < sg02.dto16) {
		segment(dpixel, dalpha, spixel, salpha, sg01, sg02, clip01);
		segment(dpixel, dalpha, spixel, salpha, sg12, sg02, clip12);
	}
	else {
		segment(dpixel, dalpha, spixel, salpha, sg02, sg01, clip01);
		segment(dpixel, dalpha, spixel, salpha, sg02, sg12, clip12);
	}
}

void PixelMapLinear(PixelArray& dest, Point d0, Point d1, Point d2,
	const PixelArray& src, Point s0, Point s1, Point s2, Rect clip, bool antialias)
{
	if(!src)
		return;
	ASSERT(dest.GetRawBPP() == -3);
	SegmentProc segment = GetSegmentProc(src.GetBPP(), 0, antialias, PIXELONLY);
	if(segment)
		PixelMapLinearRaw(dest, dest, d0, d1, d2, src, src, s0, s1, s2, clip, segment);
	else
		NEVER();
}

void PixelMapLinearMaskOnly(PixelArray& dest, Point d0, Point d1, Point d2,
	const PixelArray& src, Point s0, Point s1, Point s2, Rect clip, bool antialias)
{
	if(!src)
		return;
	ASSERT(dest.GetRawBPP() == 8);
	SegmentProc segment = GetSegmentProc(0, src.GetBPP(), antialias, MASKONLY);
	if(segment)
		PixelMapLinearRaw(dest, dest, d0, d1, d2, src, src, s0, s1, s2, clip, segment);
	else
		NEVER();
}

void PixelMapLinearMask(PixelArray& dest, Point d0, Point d1, Point d2,
	const PixelArray& src_pixel, const PixelArray& src_alpha, Point s0, Point s1, Point s2,
	Rect clip, bool antialias)
{
	if(!src_alpha) {
		PixelMapLinear(dest, d0, d1, d2, src_pixel, s0, s1, s2, clip, antialias);
		return;
	}
	if(!src_pixel)
		return;
	ASSERT(dest.GetRawBPP() == -3);
	SegmentProc segment = GetSegmentProc(src_pixel.GetBPP(), src_alpha.GetBPP(), antialias, MASKIN);
	if(segment)
		PixelMapLinearRaw(dest, dest, d0, d1, d2, src_pixel, src_alpha, s0, s1, s2, clip, segment);
	else
		NEVER();
}

void PixelMapLinearMaskOut(PixelArray& dest_pixel, PixelArray& dest_alpha, Point d0, Point d1, Point d2,
	const PixelArray& src_pixel, const PixelArray& src_alpha,
	Point s0, Point s1, Point s2, Rect clip, bool antialias)
{
	if(!dest_alpha)
		PixelMapLinearMask(dest_pixel, d0, d1, d2, src_pixel, src_alpha, s0, s1, s2, clip, antialias);
	else if(!dest_pixel)
		PixelMapLinearMaskOnly(dest_alpha, d0, d1, d2, src_alpha, s0, s1, s2, clip, antialias);
	else if(!!src_pixel) {
		ASSERT(dest_pixel.GetRawBPP() == -3 && dest_alpha.GetRawBPP() == 8);
		SegmentProc segment = GetSegmentProc(src_pixel.GetBPP(), src_alpha.GetBPP(), antialias, MASKOUT);
		if(segment)
			PixelMapLinearRaw(dest_pixel, dest_alpha, d0, d1, d2, src_pixel, !!src_alpha ? src_alpha : src_pixel, s0, s1, s2, clip, segment);
		else
			NEVER();
	}
}

void PixelMapBilinearSet(PixelArray& dest_pixel, PixelArray& dest_alpha, const Point destpos[4], Color color, Rect clip)
{
	if(IsNull(clip))
		clip = dest_pixel.GetSize();
	clip &= dest_pixel.GetSize();
	if(clip.IsEmpty())
		return;

	int ybrk[4];
	SideSegment sides[4];
	int i;
	for(i = 0; i < 4; i++) {
//		ASSERT(src_pixel.GetRect().Contains(srcpos[i]));
		sides[i].to16 = Null;
		ybrk[i] = destpos[i].y;
		for(int j = i; --j >= 0 && ybrk[j] > ybrk[j + 1]; Swap(ybrk[j], ybrk[j + 1]))
			;
	}
	ybrk[3]++;

	for(i = 0; i < 3; i++) {
		int top = ybrk[i], bottom = ybrk[i + 1];
		if(top >= clip.bottom)
			return;
		if(bottom <= clip.top)
			continue;
		Rect subclip(clip.left, max(top, clip.top), clip.right, min(bottom, clip.bottom));
		int half = subclip.Height() >> 1;
		int active[4];
		int64 mid[4];
		int acount = 0;
		Point prev = destpos[3];
		for(int s = 0; s < 4; s++) {
			Point next = destpos[s];
			if(next.y < prev.y)
				Swap(prev, next);
			if(prev.y < subclip.bottom && next.y > subclip.top) {
				if(IsNull(sides[s].to16))
					sides[s] = CalcSideSegment(prev, next, Point(0, 0), Point(0, 0), subclip.top);
				mid[acount] = sides[s].to16 + sides[s].dto16 * half;
				active[acount] = s;
				for(int t = acount++; --t >= 0 && mid[t] > mid[t + 1]; Swap(mid[t], mid[t + 1]), Swap(active[t], active[t + 1]))
					;
			}
			prev = destpos[s];
		}
		if(acount != 2 && acount != 4)
			continue;
		SideSegment *sal = &sides[active[(acount >> 1) - 1]];
		SideSegment *sar = &sides[active[(acount >> 1) - 0]];
		int64 dt = sar->to16 - sal->to16, dw = sar->dto16 - sal->dto16, db = dt + dw * (subclip.Height() - 1);
		int ycross = Null;
		Rect crossclip;
		if(dt >= +0x10000 && db <= -0x10000 || dt <= -0x10000 && db >= +0x10000) {
			ycross = (int)(subclip.top - dt / dw);
			crossclip = subclip;
			crossclip.top = subclip.bottom = ycross;
			if(dt < 0)
				Swap(sal, sar);
		}
		if(acount == 4) {
			PixelMapSet(dest_pixel, dest_alpha, color, sides[active[0]], *sal, subclip);
			PixelMapSet(dest_pixel, dest_alpha, color, *sar, sides[active[3]], subclip);
			if(!IsNull(ycross)) {
				PixelMapSet(dest_pixel, dest_alpha, color, sides[active[0]], *sar, crossclip);
				PixelMapSet(dest_pixel, dest_alpha, color, *sal, sides[active[3]], crossclip);
			}
		}
		else {
			PixelMapSet(dest_pixel, dest_alpha, color, *sal, *sar, subclip);
			if(!IsNull(ycross))
				PixelMapSet(dest_pixel, dest_alpha, color, *sar, *sal, crossclip);
		}
	}
}

void PixelMapBilinearRaw(PixelArray& dest_pixel, PixelArray& dest_alpha, const Point destpos[4],
	const PixelArray& src_pixel, const PixelArray& src_alpha, const Point srcpos[4], Rect clip, SegmentProc segment)
{
	if(IsNull(clip))
		clip = dest_pixel.GetSize();
	clip &= dest_pixel.GetSize();
	if(clip.IsEmpty())
		return;

	int ybrk[4];
	SideSegment sides[4];
	int i;
	for(i = 0; i < 4; i++) {
//		ASSERT(src_pixel.GetRect().Contains(srcpos[i]));
		sides[i].to16 = Null;
		ybrk[i] = destpos[i].y;
		for(int j = i; --j >= 0 && ybrk[j] > ybrk[j + 1]; Swap(ybrk[j], ybrk[j + 1]))
			;
	}
	ybrk[3]++;

	for(i = 0; i < 3; i++) {
		int top = ybrk[i], bottom = ybrk[i + 1];
		if(top >= clip.bottom)
			return;
		if(bottom <= clip.top)
			continue;
		Rect subclip(clip.left, max(top, clip.top), clip.right, min(bottom, clip.bottom));
		int half = subclip.Height() >> 1;
		int active[4];
		int64 mid[4];
		int acount = 0;
		Point prev = destpos[3], sprev = srcpos[3];
		for(int s = 0; s < 4; s++) {
			Point next = destpos[s], snext = srcpos[s];
			if(next.y < prev.y) {
				Swap(prev, next);
				Swap(sprev, snext);
			}
			if(prev.y < subclip.bottom && next.y > subclip.top) {
				if(IsNull(sides[s].to16))
					sides[s] = CalcSideSegment(prev, next, sprev, snext, subclip.top);
				mid[acount] = sides[s].to16 + sides[s].dto16 * half;
				active[acount] = s;
				for(int t = acount++; --t >= 0 && mid[t] > mid[t + 1]; Swap(mid[t], mid[t + 1]), Swap(active[t], active[t + 1]))
					;
			}
			prev = destpos[s];
			sprev = srcpos[s];
		}
		if(acount != 2 && acount != 4)
			continue;
		SideSegment *sal = &sides[active[(acount >> 1) - 1]];
		SideSegment *sar = &sides[active[(acount >> 1) - 0]];
		int64 dt = sar->to16 - sal->to16, dw = sar->dto16 - sal->dto16, db = dt + dw * (subclip.Height() - 1);
		int ycross = Null;
		Rect crossclip;
		if(dt >= +0x10000 && db <= -0x10000 || dt <= -0x10000 && db >= +0x10000) {
			ycross = (int)(subclip.top - dt / dw);
			crossclip = subclip;
			crossclip.top = subclip.bottom = ycross;
			if(dt < 0)
				Swap(sal, sar);
		}
		if(acount == 4) {
			segment(dest_pixel, dest_alpha, src_pixel, src_alpha, sides[active[0]], *sal, subclip);
			segment(dest_pixel, dest_alpha, src_pixel, src_alpha, *sar, sides[active[3]], subclip);
			if(!IsNull(ycross)) {
				segment(dest_pixel, dest_alpha, src_pixel, src_alpha, sides[active[0]], *sar, crossclip);
				segment(dest_pixel, dest_alpha, src_pixel, src_alpha, *sal, sides[active[3]], crossclip);
			}
		}
		else {
			segment(dest_pixel, dest_alpha, src_pixel, src_alpha, *sal, *sar, subclip);
			if(!IsNull(ycross))
				segment(dest_pixel, dest_alpha, src_pixel, src_alpha, *sar, *sal, crossclip);
		}
	}
}

static inline void ExpandRect(Point *out, const Rect& rc)
{
	out[0].x = out[3].x = rc.left;
	out[0].y = out[1].y = rc.top;
	out[1].x = out[2].x = rc.right;
	out[2].y = out[3].y = rc.bottom;
}

void PixelMapBilinear(PixelArray& dest, const Point destpos[4], const PixelArray& src, const Point srcpos[4], Rect clip, bool antialias)
{
	if(!src)
		return;
	ASSERT(dest.GetRawBPP() == -3);
	SegmentProc segment = GetSegmentProc(src.GetBPP(), 0, antialias, PIXELONLY);
	if(segment)
		PixelMapBilinearRaw(dest, dest, destpos, src, src, srcpos, clip, segment);
	else
		NEVER();
}

void PixelMapBilinear(PixelArray& dest, const Point destpos[4], const PixelArray& src, Rect srcrc, Rect clip, bool antialias)
{
	Point sp[4];
	ExpandRect(sp, srcrc);
	PixelMapBilinear(dest, destpos, src, sp, clip, antialias);
}

void PixelMapBilinearMaskOnly(PixelArray& dest, const Point destpos[4], const PixelArray& src, const Point srcpos[4], Rect clip, bool antialias)
{
	if(!src)
		return;
	ASSERT(dest.GetRawBPP() == 8);
	SegmentProc segment = GetSegmentProc(0, src.GetBPP(), antialias, MASKONLY);
	if(segment)
		PixelMapBilinearRaw(dest, dest, destpos, src, src, srcpos, clip, segment);
	else
		NEVER();
}

void PixelMapBilinearMaskOnly(PixelArray& dest, const Point destpos[4], const PixelArray& src, Rect srcrc, Rect clip, bool antialias)
{
	Point sp[4];
	ExpandRect(sp, srcrc);
	PixelMapBilinearMaskOnly(dest, destpos, src, sp, clip, antialias);
}

void PixelMapBilinearMask(PixelArray& dest, const Point destpos[4],
	const PixelArray& src_pixel, const PixelArray& src_alpha, const Point srcpos[4], Rect clip, bool antialias)
{
	if(!src_alpha) {
		PixelMapBilinear(dest, destpos, src_pixel, srcpos, clip, antialias);
		return;
	}
	if(!src_pixel)
		return;
	ASSERT(dest.GetRawBPP() == -3);
	SegmentProc segment = GetSegmentProc(src_pixel.GetBPP(), src_alpha.GetBPP(), antialias, MASKIN);
	if(segment)
		PixelMapBilinearRaw(dest, dest, destpos, src_pixel, src_alpha, srcpos, clip, segment);
	else
		NEVER();
}

void PixelMapBilinearMask(PixelArray& dest, const Point destpos[4],
	const PixelArray& src_pixel, const PixelArray& src_alpha, Rect srcrc, Rect clip, bool antialias)
{
	Point sp[4];
	ExpandRect(sp, srcrc);
	PixelMapBilinearMask(dest, destpos, src_pixel, src_alpha, sp, clip, antialias);
}

void PixelMapBilinearMaskOut(PixelArray& dest_pixel, PixelArray& dest_alpha, const Point destpos[4],
	const PixelArray& src_pixel, const PixelArray& src_alpha, const Point srcpos[4], Rect clip, bool antialias)
{
	if(!dest_alpha)
		PixelMapBilinearMask(dest_pixel, destpos, src_pixel, src_alpha, srcpos, clip, antialias);
	else if(!dest_pixel)
		PixelMapBilinearMaskOnly(dest_alpha, destpos, src_alpha, srcpos, clip, antialias);
	else if(!!src_pixel) {
		ASSERT(dest_pixel.GetRawBPP() == -3);
		SegmentProc segment = GetSegmentProc(src_pixel.GetBPP(), src_alpha.GetBPP(), antialias, MASKOUT);
		if(segment)
			PixelMapBilinearRaw(dest_pixel, dest_alpha, destpos, src_pixel, !!src_alpha ? src_alpha : src_pixel, srcpos, clip, segment);
		else
			NEVER();
	}
}

void PixelMapBilinearMaskOut(PixelArray& dest_pixel, PixelArray& dest_alpha, const Point destpos[4],
	const PixelArray& src_pixel, const PixelArray& src_alpha, Rect srcrc, Rect clip, bool antialias)
{
	Point sp[4];
	ExpandRect(sp, srcrc);
	PixelMapBilinearMaskOut(dest_pixel, dest_alpha, destpos, src_pixel, src_alpha, sp, clip, antialias);
}

#endif
