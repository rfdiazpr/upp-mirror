#include "Draw.h"
#pragma hdrstop

#include "PixelUtil.h"

#if defined(PLATFORM_WIN32) || defined(PLATFORM_X11)
	#define PLATFORM_GDI
#endif

#if defined(CPU_IA32) && defined(COMPILER_MSC)
	#define I386_ASSEMBLY
#endif

#ifdef I386_ASSEMBLY
	#define BLT_TIMING(routine) //RTIMING(#routine " (asm)")
#else
	#define BLT_TIMING(routine) //RTIMING(#routine " (cpp)")
#endif

#define X11DUMP(x) puts(String() << #x << " = " << (x))

static void RleDecode(byte *dest, int unit, int items, const char *src)
{
	byte *dend = dest + unit * items;
	while(items > 0)
	{
		if(*src & 0x80)
		{
			int count = min<int>(*src++ & 0x7F, items);
			memsetex(dest, src, unit, count);
			dest += unit * count;
			items -= count;
			src += unit;
		}
		else
		{ // enum row / end of row
			if(*src == 0)
				return;
			const char *new_src = src + 1 + unit * *src;
			int count = min<int>((byte)*src++, items);
			int uc = unit * count;
			memcpy(dest, src, uc);
			dest += uc;
			items -= count;
			src = new_src;
		}
	}
}

#define COMPARE1(a, b) (*(a) == *(b))
#define COMPARE2(a, b) ((a)[0] == (b)[0] && (a)[1] == (b)[1])
#define COMPARE3(a, b) (COMPARE2(a, b) && (a)[2] == (b)[2])
#define COMPARE4(a, b) (COMPARE3(a, b) && (a)[3] == (b)[3])

#define RLE_ENCODE(EQ, UNIT) \
static String RleEncode##UNIT(const byte *src, int items) \
{ \
	String out; \
	int last = items; \
	while(items > (UNIT == 1 ? 2 : 1)) \
		if(EQ(src, src + UNIT) && (UNIT > 1 || EQ(src, src + 2 * UNIT))) \
		{ /* group */ \
			if(items != last) \
			{ /* enum record pending */ \
				int count = last - items; \
				const byte *pos = src - count * UNIT; \
				while(count > 127) \
				{ \
					out.Cat(0x7F); \
					out.Cat(pos, 127 * UNIT); \
					pos += 127 * UNIT; \
					count -= 127; \
				} \
				out.Cat(count); \
				out.Cat(pos, count * UNIT); \
			} \
			last = items; \
			src += (UNIT == 1 ? 3 : 2) * UNIT; \
			items -= (UNIT == 1 ? 3 : 2); \
			while(items > 0 && EQ(src, src - UNIT)) \
			{ \
				items--; \
				src += UNIT; \
			} \
			int count = last - items; \
			while(count > 127) \
			{ \
				out.Cat(0xFF); \
				out.Cat(src - UNIT, UNIT); \
				count -= 127; \
			} \
			out.Cat(0x80 + count); \
			out.Cat(src - UNIT, UNIT); \
			last = items; \
		} \
		else \
		{ \
			items--; \
			src += UNIT; \
		} \
	if(last) \
	{ \
		src -= (last - items) * UNIT; \
		while(last > 127) \
		{ \
			out.Cat(0x7F); \
			out.Cat(src, 127 * UNIT); \
			src += 127 * UNIT; \
			last -= 127; \
		} \
		out.Cat(last); \
		out.Cat(src, last * UNIT); \
	} \
	return out; \
}

RLE_ENCODE(COMPARE1, 1)
RLE_ENCODE(COMPARE2, 2)
RLE_ENCODE(COMPARE3, 3)
RLE_ENCODE(COMPARE4, 4)

static String (*RleEncodeTable[])(const byte *src, int items)
= { RleEncode1, RleEncode2, RleEncode3, RleEncode4 };

#ifdef PLATFORM_X11
int   Xpixel_raw_bpp;
int   Xpixel_bpp;
int   Xpixel_class;
int   Xpixel_format_code;
dword Xpixel_cmask[3];
int   Xpixel_cshift24[3];
int   Xpixel_cbits[3];

static void sInitPixelDefaults()
{
	if(Xpixel_bpp)
		return;
	Visual *v = DefaultVisual(Xdisplay, Xscreenno);
//	XVisualInfo vtempl;
//	vtempl.visualid = XVisualIDFromVisual(vf);
//	int nfound = 0;
//	XVisualInfo *v = XGetVisualInfo(Xdisplay, VisualIDMask, &vtempl, &nfound);
//	if(!nfound || !v)
//	{
//		Panic("Error initializing visual info defaults.");
//	}
	Xpixel_raw_bpp = DefaultDepth(Xdisplay, Xscreenno);
	Xpixel_bpp = (Xpixel_raw_bpp <= 8 ? Xpixel_raw_bpp : Xpixel_raw_bpp <= 16 ? 16 : 32);
	Xpixel_class = v -> c_class;
	const dword in_mask[] = { v -> red_mask, v -> green_mask, v -> blue_mask };
	CalcBitMasks(Xpixel_raw_bpp, in_mask, Xpixel_cmask, Xpixel_cshift24, Xpixel_cbits);
	Xpixel_format_code = CalcFormatCode(Xpixel_raw_bpp, Xpixel_cshift24, Xpixel_cbits);
//	XFree(vf);
/*
	LOG("Pixel defaults:");
	DUMP(Xpixel_raw_bpp);
	DUMP(Xpixel_bpp);
	DUMP(Xpixel_class);
	DUMP(Xpixel_format_code);
	DUMP(Xpixel_cshift24[2]);
	DUMP(Xpixel_cbits[2]);
	DUMP(Xpixel_cshift24[1]);
	DUMP(Xpixel_cbits[1]);
	DUMP(Xpixel_cshift24[0]);
	DUMP(Xpixel_cbits[0]);
*/
}
#endif

void CalcBitMasks(int bpp, const dword *in_cmasks, dword cmask[], int cshift24[], int cbits[])
{
	static const dword cmasks16[] = { 0xF800, 0x07E0, 0x001F };
	static const dword cmasks32[] = { 0xFF0000, 0x00FF00, 0x0000FF };
	if(!in_cmasks)
		in_cmasks = (bpp <= 16 ? cmasks16 : cmasks32);
	else if((in_cmasks[0] | in_cmasks[1] | in_cmasks[2]) == 0)
	{
#ifdef PLATFORM_X11
		in_cmasks = Xpixel_cmask;
#else
		in_cmasks = (bpp <= 16 ? cmasks16 : cmasks32);
#endif
	}
	for(int i = 0; i < 3; i++)
	{
		dword cm = in_cmasks[i];
		cmask[i] = cm;
		int shift = 0;
		if(!(cm & 0xFFFF0000))
		{
			cm <<= 16;
			shift += 16;
		}
		if(!(cm & 0xFF000000))
		{
			cm <<= 8;
			shift += 8;
		}
		if(!(cm & 0xF0000000))
		{
			cm <<= 4;
			shift += 4;
		}
		if(!(cm & 0xC0000000))
		{
			cm <<= 2;
			shift += 2;
		}
		if(!(cm & 0x80000000))
		{
			cm <<= 1;
			shift += 1;
		}
		int width = 0;
		dword mask = 0x80000000;
		while((cm & mask) && width < 8)
		{
			mask >>= 1;
			width++;
		}
		cshift24[i] = shift;
		cbits[i] = width;
	}
}

int CalcFormatCode(int bpp, const int cshift24[], const int cbits[])
{
	int fmtcode = (bpp < 0 ? 12 : bpp < 8 ? bpp : bpp == 16 ? 9 : bpp == 24 ? 10 : bpp == 32 ? 11 : 0);
//	fmtcode |= align << 4;
	if(bpp == 16 || bpp == 32)
		fmtcode |= ((cshift24[0] | ((cbits[0] - 1) << 5)) << 8)
			|      ((cshift24[1] | ((cbits[1] - 1) << 5)) << 16)
			|      ((cshift24[2] | ((cbits[2] - 1) << 5)) << 24);
	return fmtcode;
}

#ifdef PLATFORM_WIN32
#define WXROP(w, x) w
#endif
#ifdef PLATFORM_X11
#define WXROP(w, x) x
#endif

#ifdef PLATFORM_GDI

int GetPixelROP(PixelOp op)
{
	static bool inited = false;
	static int  rop[PIXEL_OP_COUNT];
	if(!inited)
	{
		inited = true;
		rop[PIXEL_NOP]      = WXROP(0x00AA0029, X11_ROP2_NOP);
		rop[PIXEL_BLACK]    = WXROP(BLACKNESS,  X11_ROP2_ZERO);
		rop[PIXEL_WHITE]    = WXROP(WHITENESS,  X11_ROP2_ONE);
		rop[PIXEL_COPY]     = WXROP(SRCCOPY,    X11_ROP2_COPY);
		rop[PIXEL_NOT_COPY] = WXROP(NOTSRCCOPY, X11_ROP2_NOT_COPY);
		rop[PIXEL_AND]      = WXROP(SRCAND,     X11_ROP2_AND);
		rop[PIXEL_NOT_AND]  = WXROP(0x00220326, X11_ROP2_NOT_AND);
		rop[PIXEL_OR]       = WXROP(SRCPAINT,   X11_ROP2_OR);
		rop[PIXEL_NOT_OR]   = WXROP(MERGEPAINT, X11_ROP2_NOT_OR);
		rop[PIXEL_XOR]      = WXROP(SRCINVERT,  X11_ROP2_XOR);
		rop[PIXEL_NOT_XOR]  = WXROP(0x00990066, X11_ROP2_NOT_XOR);
	}
	ASSERT(op >= PIXEL_NOP && op < PIXEL_OP_COUNT);
	return rop[op];
}

#endif//PLATFORM_GDI

enum
{
	GAMMA_BITS  = 10,
	GAMMA_COUNT = 1 << GAMMA_BITS,
	GAMMA_TOP   = GAMMA_COUNT - 1,
};

static const byte *GetGammaTable()
{
	static byte gamma_table[GAMMA_COUNT];
	if(gamma_table[GAMMA_TOP] == 0)
	{ // initialize table
		for(int i = 0; i < GAMMA_COUNT; i++)
			gamma_table[i]
			= (byte)minmax(fround(255 * pow(i / (double)GAMMA_TOP, 0.4)), 0, 255);
	}
	return gamma_table;
}

const Vector<Color>& MonoPalette()
{
	static Vector<Color> mp;
	if(mp.IsEmpty())
	{
		mp.Add(Black());
		mp.Add(White());
	}
	return mp;
}

static void BltConvert(byte *dest, const byte *src, int dest_row_bytes, int src_row_bytes, int cx, int cy,
	void (*proc)(byte *, const byte *, unsigned))
{
	while(--cy >= 0)
	{
		proc(dest, src, cx);
		dest += dest_row_bytes;
		src += src_row_bytes;
	}
}

static void Convert1to4(const PixelArray& src, byte *dest, int alignment)
{
	ASSERT(!src.IsEmpty() && src.GetBPP() == 1);
	BltConvert(dest, src.Begin(),
		PixelArray::GetRowBytes(4, src.GetWidth(), alignment),
		src.GetRowBytes(), src.GetWidth(), src.GetHeight(), Blt1to4);
}

static void Convert1to8(const PixelArray& src, byte *dest, int alignment)
{
	ASSERT(!src.IsEmpty() && src.GetBPP() == 1);
	BltConvert(dest, src.Begin(),
		PixelArray::GetRowBytes(8, src.GetWidth(), alignment),
		src.GetRowBytes(), src.GetWidth(), src.GetHeight(), Blt1to8);
}

static void Convert2to4(const PixelArray& src, byte *dest, int alignment)
{
	ASSERT(!src.IsEmpty() && src.GetBPP() == 2);
	BltConvert(dest, src.Begin(),
		PixelArray::GetRowBytes(4, src.GetWidth(), alignment),
		src.GetRowBytes(), src.GetWidth(), src.GetHeight(), Blt2to4);
}

static void Convert4to1(const PixelArray& src, byte *dest, int alignment)
{
	ASSERT(!src.IsEmpty() && src.GetBPP() == 4);
	BltConvert(dest, src.Begin(),
		PixelArray::GetRowBytes(1, src.GetWidth(), alignment),
		src.GetRowBytes(), src.GetWidth(), src.GetHeight(), Blt4to1);
}

static void Convert4to8(const PixelArray& src, byte *dest, int alignment)
{
	ASSERT(!src.IsEmpty() && src.GetBPP() == 4);
	BltConvert(dest, src.Begin(),
		PixelArray::GetRowBytes(8, src.GetWidth(), alignment),
		src.GetRowBytes(), src.GetWidth(), src.GetHeight(), Blt4to8);
}

static void Convert8to1(const PixelArray& src, byte *dest, int alignment)
{
	ASSERT(!src.IsEmpty() && src.GetBPP() == 8);
	BltConvert(dest, src.Begin(),
		PixelArray::GetRowBytes(1, src.GetWidth(), alignment),
		src.GetRowBytes(), src.GetWidth(), src.GetHeight(), Blt8to1);
}

static void Convert8to4(const PixelArray& src, byte *dest, int alignment)
{
	ASSERT(!src.IsEmpty() && src.GetBPP() == 8);
	BltConvert(dest, src.Begin(),
		PixelArray::GetRowBytes(4, src.GetWidth(), alignment),
		src.GetRowBytes(), src.GetWidth(), src.GetHeight(), Blt8to4);
}

static void Convert8to24(const PixelArray& simg, byte *dest, int alignment, const Vector<Color>& pal)
{
	ASSERT(!simg.IsEmpty() && simg.GetBPP() == 8);
	const byte *src = simg.End();
	Size size = simg.GetSize();
	int in_bytes = simg.GetRowBytes();
	int out_bytes = PixelArray::GetRowBytes(24, size.cx, alignment);
	dest += out_bytes * size.cy;
	byte rgb[256][3];
	ZeroArray(rgb);
	for(int t = 0; t < pal.GetCount(); t++)
	{
		Color c = pal[t];
		byte *p = rgb[t];
		p[0] = c.GetB();
		p[1] = c.GetG();
		p[2] = c.GetR();
	}
	const byte *r;
	for(int i = size.cy; --i >= 0;)
	{
		const byte *s = (src -= in_bytes) + size.cx;
		byte *d = (dest -= out_bytes) + 3 * size.cx;
		int n = size.cx;
		while(n >= 4)
		{
			r = rgb[s[-1]]; d[- 1] = r[2]; d[- 2] = r[1]; d[- 3] = r[0];
			r = rgb[s[-2]]; d[- 4] = r[2]; d[- 5] = r[1]; d[- 6] = r[0];
			r = rgb[s[-3]]; d[- 7] = r[2]; d[- 8] = r[1]; d[- 9] = r[0];
			r = rgb[s[-4]]; d[-10] = r[2]; d[-11] = r[1]; d[-12] = r[0];
			d -= 12;
			s -= 4;
			n -= 4;
		}
		if(n & 2)
		{
			r = rgb[s[-1]]; d[- 1] = r[2]; d[- 2] = r[1]; d[- 3] = r[0];
			r = rgb[s[-2]]; d[- 4] = r[2]; d[- 5] = r[1]; d[- 6] = r[0];
			d -= 6;
			s -= 2;
		}
		if(n & 1)
		{
			r = rgb[s[-1]]; d[- 1] = r[2]; d[- 2] = r[1]; d[- 3] = r[0];
		}
	}
}

void PixelScanToRLE(String& result, const byte *dp, int width)
{
	const byte *de = dp + 3 * width;
	const byte *dr = dp;
	while(dp <= de)
		if(dp < de && dp[0] == dp[3] && dp[1] == dp[4] && dp[2] == dp[5])
		{ // flush last enum segment, generate repeat segment
			if(dp > dr)
			{ // enum segment
				int count = (dp - dr) / 3;
				while(count > 0)
				{
					int part = min(count, 0x7F);
					result.Cat(part);
					result.Cat((const char *)dr, 3 * part);
					dr += 3 * part;
					count -= part;
				}
			}

			const byte *ds = dp;
			while((dp += 3) < de && dp[0] == dp[3] && dp[1] == dp[4] && dp[2] == dp[5])
				;
			dr = dp += 3;
			int count = (dp - ds) / 3;
			while(count > 0)
			{
				int part = min(count, 0x3F);
				result.Cat(0x80 + part);
				result.Cat((const char *)dp - 3, 3);
				count -= part;
			}
		}
		else
			dp += 3;
	if(dp > dr)
	{ // last enum segment
		int count = (dp - dr) / 3;
		while(count > 0)
		{
			int part = min(count, 0x7F);
			result.Cat(part);
			result.Cat((const char *)dr, 3 * part);
			dr += 3 * part;
			count -= part;
		}
	}
}

String PixelToRLE(const PixelArray& image)
{
	if(image.GetRawBPP() != -3)
		return PixelToRLE(PixelConvert(image, -3, 4));
	String result;
	int last = 0;
	for(int y = 0; y < image.GetHeight(); y++)
	{
		int prev = result.GetLength();
		PixelScanToRLE(result, image.GetDownScan(y), image.GetWidth());
		if(result.GetLength() > prev)
			last = result.GetLength();
		result.Cat(0x80); // row end
	}
	result.Trim(last);
	return result;
}

const byte *RLEToPixelScan(const byte *src, byte *dp, int width)
{
	byte *de = dp + 3 * width;
//	int $done = 0;
	for(;;)
	{
//		ASSERT($done == (mp - Mask(0, y)));
		if(*src & 0x80)
		{
//			$done += *src & 0x3F;
			if(*src == 0x80) // end of row
				return src;
			int count = min<int>(*src & 0x3F, (de - dp) / 3);
			if(*src++ & 0x40)
			{ // skip segment
				dp += 3 * count;
			}
			else
			{ // repeat segment
				memsetex(dp, src, 3, count);
				dp += 3 * count;
				src += 3;
			}
		}
		else
		{ // enum row / end of row
//			$done += *src & 0x7F;
			if(*src == 0)
				return src; // end of image
			const byte *new_src = src + 1 + 3 * *src;
			int count = min<int>(*src++, (de - dp) / 3);
			memcpy(dp, src, 3 * count);
			dp += 3 * count;
			src = new_src;
		}
	}
}

void RLEToPixel(PixelArray& dest, const byte *src)
{
	for(int y = 0; y < dest.GetHeight() && *(src = RLEToPixelScan(src, dest.GetDownScan(y), dest.GetWidth())); y++, src++)
		;
}

PixelArrayDeep::PixelArrayDeep()
: size(0, 0)
, dot_size(0, 0)
, hotspot(0, 0)
, raw_bpp(0)
, bpp(0)
, row_bytes(0)
, alignment(0)
, mono(false)
{
}

int PixelArray::SizeOfInstance() const
{
	return sizeof(*this) + sizeof(Color) * palette.GetCount() + GetBytes();
}

void PixelArray::Clear()
{
	pixels.Clear();
	pixels.Shrink();
	palette.Clear();
	palette.Shrink();
	size = Size(0, 0);
	raw_bpp = bpp = alignment = 0;
	mono = false;
}

void PixelArray::Create(int cx_, int cy_, int bpp_, int alignment_, const dword *cmasks_,
	const Vector<Color>& pal_, int deep)
{
	Create(cx_, cy_, bpp_, alignment_, cmasks_, Vector<Color>(pal_, 0));
}

void PixelArray::Create(int cx_, int cy_, int bpp_, int alignment_, const dword *cmasks_, pick_ Vector<Color>& pal_)
{
	size.cx = cx_;
	size.cy = cy_;
	mono = false;
	SetFormat(bpp_, alignment_);
	Shrink();
	if(bpp_ > 8)
		SetMasks(cmasks_);
	else
		palette = pal_;
}

int PixelArray::SerializeInfo(Stream& stream,
	Size& size, bool& mono, int& raw_bpp, int& alignment,
	dword cmasks[3], Vector<Color>& palette)
{
	int version = 1;
	stream / version % size % mono / raw_bpp / alignment;
	if(!mono)
		stream % cmasks[0] % cmasks[1] % cmasks[2] % palette;
	return version;
}

void PixelArray::Serialize(Stream& stream)
{
	bool nm = mono;
	Size nsz = size;
	int nbpp = raw_bpp, nali = alignment;
	dword ncmask[3] = { cmask[0], cmask[1], cmask[2] };
	Vector<Color> npal(palette, 0);
	int version = SerializeInfo(stream, nsz, nm, nbpp, nali, ncmask, npal);
	if(stream.IsLoading())
		if(nm)
			CreateMono(nsz, nbpp, nali);
		else
			Create(nsz, nbpp, nali, ncmask, npal);
	if(size.cx <= 0 || size.cy <= 0)
		return;
	if(stream.IsLoading())
		memset(Begin(), 0, GetBytes());
	String (*RleEncode)(const byte *pos, int items);
	if(nm)
	{
		int unit = (size.cx + 7) >> 3;
		Buffer<byte> temp(unit);
		RleEncode = RleEncodeTable[0];
		for(int i = 0; i < size.cy; i++)
		{
			String s;
			byte *p = GetDownScan(i);
			if(stream.IsStoring())
			{
				if(nbpp == 1)
					s = RleEncode(p, unit);
				else if(nbpp == 8)
				{
					Blt8to1(temp, p, size.cx);
					s = RleEncode(temp, unit);
				}
				else
					NEVER();
			}
			stream % s;
			if(stream.IsLoading())
			{
				if(nbpp == 1)
					RleDecode(p, 1, unit, s);
				else
				{
					RleDecode(temp, 1, unit, s);
					Blt1to8(p, temp, size.cx);
				}
			}
		}
	}
	else
	{
		int unit = ((bpp - 1) >> 3) + 1;
		ASSERT(unit >= 1 && unit <= 4);
		int items = ((size.cx * bpp - 1) >> 3) / unit + 1;
		String (*RleEncode)(const byte *pos, int items) = RleEncodeTable[unit - 1];
		for(int i = 0; i < size.cy; i++)
		{
			String s;
			byte *p = GetDownScan(i);
			if(stream.IsStoring())
				s = RleEncode(p, items);
			stream % s;
			if(stream.IsLoading())
				RleDecode(p, unit, items, s);
		}
	}
}

void PixelArray::CreateMono(int cx, int cy, int bpp, int alignment)
{
	Create(cx, cy, bpp, alignment);
	mono = true;
	palette.SetCount(2);
	palette[0] = Black;
	palette[1] = White;
}

void PixelArray::GetMaskInfo(byte BsmGsmRsm[6]) const
{
	BsmGsmRsm[0] = cshift24[2];
	BsmGsmRsm[1] = (-256 >> cbits[2]) & 0xFF;
	BsmGsmRsm[2] = cshift24[1];
	BsmGsmRsm[3] = (-256 >> cbits[1]) & 0xFF;
	BsmGsmRsm[4] = cshift24[0];
	BsmGsmRsm[5] = (-256 >> cbits[0]) & 0xFF;
}

void PixelArray::SetSize(Size sz)
{
	size = sz;
	row_bytes = GetRowBytes(bpp, size.cx, alignment);
	int total = row_bytes * size.cy;
	if(pixels.GetCount() < total)
		pixels.SetCount(total);
}

void PixelArray::SetFormat(int new_bpp, int new_alignment)
{
	raw_bpp = new_bpp;
	bpp = (raw_bpp == -3 ? 24 : raw_bpp <= 8 ? raw_bpp : raw_bpp <= 16 ? 16 : 32);
	row_bytes = GetRowBytes(bpp, size.cx, alignment = new_alignment);
	format_code = CalcFormatCode(raw_bpp, cshift24, cbits);
}

void PixelArray::Shrink()
{
	pixels.SetCount(row_bytes * size.cy);
	pixels.Shrink();
}

void PixelArray::SetMasks(const dword *in_cmasks)
{
	if(bpp == 16 || bpp == 32)
		CalcBitMasks(bpp, in_cmasks, cmask, cshift24, cbits);
	format_code = CalcFormatCode(raw_bpp, cshift24, cbits);
}

void PixelArray::Paint(Draw& draw, const Rect& src, const Rect& dest, PixelOp op) const
{
	ASSERT(!pixels.IsPicked());
	if(!IsEmpty())
	{
#ifdef PLATFORM_WIN32
		PaintDC(draw, src, dest, GetPixelROP(op));
#endif
#ifdef PLATFORM_X11
		PaintGC(draw.GetGC(), draw.GetDrawable(), draw.GetClipList(), src, dest + draw.GetOffset(), GetPixelROP(op));
#endif
	}
}

void PixelArray::Paint(Draw& draw, const Rect& src, const Rect& dest, Color black, Color white, Color doxor) const
{
	ASSERT(!pixels.IsPicked());
#ifdef PLATFORM_WIN32
	PaintDC(draw, src, dest, black, white, doxor);
#endif
#ifdef PLATFORM_X11
	PaintGC(draw.GetGC(), draw.GetDrawable(), draw.GetClipList(), src, dest + draw.GetOffset(), black, white, doxor);
#endif
}

#ifdef PLATFORM_WIN32
void PixelArray::PaintDC(HDC hdc, const Rect& src, const Rect& dest, Color fore, Color back, Color doxor) const
{
	ASSERT(!pixels.IsPicked());
	ASSERT(GetBPP() <= 8);
	if(IsEmpty() || IsNull(fore) && IsNull(back))
		return;
	STRETCH_MODE mode = (GetDeviceCaps(hdc, BITSPIXEL) * GetDeviceCaps(hdc, PLANES) <= 8
		? STRETCH_PAL_MONO : STRETCH_RGB_MONO);
	Color pal[2] = { fore, back };
	if(!IsNull(doxor))
	{
		Color c0 = Nvl(fore, doxor), c1 = Nvl(back, doxor);
		pal[0] = Color(c0.GetR() ^ doxor.GetR(), c0.GetG() ^ doxor.GetG(), c0.GetB() ^ doxor.GetB());
		pal[1] = Color(c1.GetR() ^ doxor.GetR(), c1.GetG() ^ doxor.GetG(), c1.GetB() ^ doxor.GetB());
	}
	PaintDCRaw(hdc, src, dest, SRCCOPY, mode, pal);
}
#endif

#ifdef PLATFORM_WIN32
void PixelArray::PaintDC(HDC hdc, const Rect& src, const Rect& dest, int rop) const
{
	STRETCH_MODE mode = (GetDeviceCaps(hdc, BITSPIXEL) * GetDeviceCaps(hdc, PLANES) <= 8
		? STRETCH_PAL_COLOR : STRETCH_RGB_COLOR);
	PaintDCRaw(hdc, src, dest, rop, mode, NULL);
}
#endif

#ifdef PLATFORM_WIN32
static inline RGBQUAD GetRGBQuad(Color c)
{
	RGBQUAD quad;
	quad.rgbRed = c.GetR();
	quad.rgbGreen = c.GetG();
	quad.rgbBlue = c.GetB();
	return quad;
}

static void StretchBlock(HDC hdc, const Rect& src, const Rect& dest, int total_cy,
	PixelArray::Header& header, const void *data, int rop, PixelArray::STRETCH_MODE mode, const Color *pal)
{
	Size size = src.Size();
	if(mode == PixelArray::STRETCH_PAL_MONO || mode == PixelArray::STRETCH_PAL_COLOR)
	{
		HBITMAP aux_bmp = (mode == PixelArray::STRETCH_PAL_MONO
			? CreateBitmap(size.cx, size.cy, 1, 1, NULL)
			: CreateCompatibleBitmap(hdc, size.cx, size.cy));
		HDC mdc = CreateCompatibleDC(hdc);
		HGDIOBJ old_bmp = SelectObject(mdc, aux_bmp);
		StretchDIBits(mdc, 0, 0, size.cx, size.cy, src.left, src.top, size.cx, size.cy,
			data, header, DIB_RGB_COLORS, SRCCOPY);
		if(mode == PixelArray::STRETCH_PAL_COLOR)
		{
			StretchDIBits(hdc, dest.left, dest.top, dest.Width(), dest.Height(),
				src.left, total_cy - src.bottom, src.Width(), src.Height(),
				data, header, DIB_RGB_COLORS, rop);
		}
		else
		{
			COLORREF old_text, old_back;
			ASSERT(pal);
			if(!IsNull(pal[0]) && !IsNull(pal[1]))
			{
				old_text = SetTextColor(hdc, pal[0]);
				old_back = SetBkColor(hdc, pal[1]);
				StretchBlt(hdc, dest.left, dest.top, dest.Width(), dest.Height(),
					mdc, 0, 0, size.cx, size.cy, rop);
			}
			else if(!IsNull(pal[0]))
			{
				old_text = SetTextColor(hdc, 0);
				old_back = SetBkColor(hdc, 0xFFFFFF);
				StretchBlt(hdc, dest.left, dest.top, dest.Width(), dest.Height(),
					mdc, 0, 0, size.cx, size.cy, SRCAND);
				if(pal[0] != Black)
				{
					SetTextColor(hdc, pal[0]);
					SetBkColor(hdc, 0);
					StretchBlt(hdc, dest.left, dest.top, dest.Width(), dest.Height(),
						mdc, 0, 0, size.cx, size.cy, SRCPAINT);
				}
			}
			else if(!IsNull(pal[1]))
			{
				old_text = SetTextColor(hdc, 0xFFFFFF);
				old_back = SetBkColor(hdc, 0);
				StretchBlt(hdc, dest.left, dest.top, dest.Width(), dest.Height(),
					mdc, 0, 0, size.cx, size.cy, SRCAND);
				if(pal[1] != Black)
				{
					SetTextColor(hdc, 0);
					SetBkColor(hdc, pal[1]);
					StretchBlt(hdc, dest.left, dest.top, dest.Width(), dest.Height(),
						mdc, 0, 0, size.cx, size.cy, SRCPAINT);
				}
			}
			SetTextColor(hdc, old_text);
			SetBkColor(hdc, old_back);
		}
		SelectObject(mdc, old_bmp);
		DeleteObject(aux_bmp);
		DeleteDC(mdc);
	}
	else if(mode == PixelArray::STRETCH_RGB_MONO)
	{
		if(!IsNull(pal[0]) && !IsNull(pal[1]))
		{
			header.palette[0] = GetRGBQuad(pal[0]);
			header.palette[1] = GetRGBQuad(pal[1]);
			StretchDIBits(hdc, dest.left, dest.top, dest.Width(), dest.Height(),
				src.left, total_cy - src.bottom, src.Width(), src.Height(),
				data, header, DIB_RGB_COLORS, rop);
		}
		else if(!IsNull(pal[0]))
		{
			header.palette[0] = GetRGBQuad(Black);
			header.palette[1] = GetRGBQuad(White);
			StretchDIBits(hdc, dest.left, dest.top, dest.Width(), dest.Height(),
				src.left, total_cy - src.bottom, src.Width(), src.Height(),
				data, header, DIB_RGB_COLORS, SRCAND);
			if(pal[0] != Black)
			{
				header.palette[0] = GetRGBQuad(pal[0]);
				header.palette[1] = GetRGBQuad(Black);
				StretchDIBits(hdc, dest.left, dest.top, dest.Width(), dest.Height(),
					src.left, total_cy - src.bottom, src.Width(), src.Height(),
					data, header, DIB_RGB_COLORS, SRCPAINT);
			}
		}
		else if(!IsNull(pal[1]))
		{
			header.palette[0] = GetRGBQuad(White);
			header.palette[1] = GetRGBQuad(Black);
			StretchDIBits(hdc, dest.left, dest.top, dest.Width(), dest.Height(),
				src.left, total_cy - src.bottom, src.Width(), src.Height(),
				data, header, DIB_RGB_COLORS, SRCAND);
			if(pal[1] != Black)
			{
				header.palette[0] = GetRGBQuad(Black);
				header.palette[1] = GetRGBQuad(pal[1]);
				StretchDIBits(hdc, dest.left, dest.top, dest.Width(), dest.Height(),
					src.left, total_cy - src.bottom, src.Width(), src.Height(),
					data, header, DIB_RGB_COLORS, SRCPAINT);
			}
		}
	}
	else if(mode == PixelArray::STRETCH_RGB_COLOR)
	{
		StretchDIBits(hdc, dest.left, dest.top, dest.Width(), dest.Height(),
			src.left, total_cy - src.bottom, src.Width(), src.Height(),
			data, header, DIB_RGB_COLORS, rop);
	}
	else
		NEVER();
}
#endif

#ifdef PLATFORM_WIN32
void PixelArray::PaintDCRaw(HDC hdc, const Rect& src, const Rect& dest, int rop, STRETCH_MODE mode, const Color *pal) const
{
	Header header;
	if(!SetHeader(header, NULL) || src.left == src.right || src.top == src.bottom
		|| dest.left == dest.right || dest.top == dest.bottom)
		return;
	int old_mode = SetStretchBltMode(hdc, COLORONCOLOR);
	Rect s0 = src, d0 = dest;
	if(s0.right < s0.left)
	{
		Swap(s0.left, s0.right);
		Swap(d0.left, d0.right);
	}
	if(s0.bottom < s0.top)
	{
		Swap(s0.top, s0.bottom);
		Swap(d0.top, d0.bottom);
	}
	Size bsize = s0.Size();
	enum { SPLIT = 1024 };
	if(bsize.cx <= SPLIT && bsize.cy <= SPLIT && tabs(d0.Width()) <= SPLIT && tabs(d0.Height()) <= SPLIT)
		StretchBlock(hdc, src, dest, size.cy, header, Begin(), rop, mode, pal);
	else
	{
		bool rx = false, ry = false;
		Point plus(d0.left + d0.right, d0.top + d0.bottom);
		if(d0.right < d0.left)
		{
			rx = true;
			Swap(d0.left, d0.right);
		}
		if(d0.bottom < d0.top)
		{
			ry = true;
			Swap(d0.top, d0.bottom);
		}
		Rect rc;
		GetClipBox(hdc, rc);
		for(SmartStretch sms(d0, s0, rc, SPLIT); sms.Get();)
		{
			s0 = sms.GetSrc(), d0 = sms.GetDest();
			if(rx)
			{
				d0.left = plus.x - d0.left;
				d0.right = plus.x - d0.right;
			}
			if(ry)
			{
				d0.top = plus.y - d0.top;
				d0.bottom = plus.y - d0.bottom;
			}
			header.biHeight = s0.Height();
			StretchBlock(hdc, s0, d0, s0.bottom, header, GetDownScan(s0.bottom - 1), rop, mode, pal);
		}
	}
	SetStretchBltMode(hdc, old_mode);
}
#endif

#ifdef PLATFORM_X11
void PixelArray::PaintGC(GC gc, Drawable dw, const Vector<Rect>& clip, const Rect& src, const Rect& dest, int rop) const
{
/*
	{
		String out;
		out << "PixelArray::PaintGC(color) & " << dest << "; clip =";
		for(int i = 0; i < clip.GetCount(); i++)
			out << " " << clip[i];
		puts(out);
		fflush(stdout);
	}
*/
	sInitPixelDefaults();

	XGCValues gcv;
	int gcv_mask = GCFunction | GCPlaneMask | (mono ? GCForeground | GCBackground : 0);
	XGetGCValues(Xdisplay, gc, gcv_mask, &gcv);
	int old_fn = gcv.function;
	unsigned old_mask = gcv.plane_mask;
	gcv.function = rop;
	gcv.plane_mask = -1u;
	if(mono)
	{
		gcv.foreground = GetXPixel(palette[1]);
		gcv.background = GetXPixel(palette[0]);
	}
	XChangeGC(Xdisplay, gc, gcv_mask, &gcv);

	bool format = (format_code != Xpixel_format_code || mono && bpp != 1);
	bool resize = (src.Size() != dest.Size());

	if(!resize)
	{
		PixelArray cvt;
		const PixelArray *psrc = (format ? &cvt : this);
		if(format)
			cvt = PixelConvertScreen(*this);
		XImage ximage;
		psrc -> SetXImage(ximage);
		for(int c = 0; c < clip.GetCount(); c++)
		{
			Rect dclip = dest & clip[c];
			if(!dclip.IsEmpty())
			{
				Point dsrc = dclip.TopLeft() - dest.TopLeft() + src.TopLeft();
				XPutImage(Xdisplay, dw, gc, &ximage, dsrc.x, dsrc.y,
					dclip.left, dclip.top, dclip.Width(), dclip.Height());
			}
		}
	}
	else
	{
		int maxbytes = 0;
		int fmtbytes = 0;
		Vector<Rect> raw_clip;
		for(int c = 0; c < clip.GetCount(); c++)
		{
			Rect dclip = clip[c] & dest;
			if(!dclip.IsEmpty())
			{
				raw_clip.Add(dclip);
				fmtbytes = max(fmtbytes, GetRowBytes(bpp, dclip.Width(), alignment) * dclip.Height());
				maxbytes = max(maxbytes, GetRowBytes(Xpixel_bpp, dclip.Width(), alignment) * dclip.Height());
			}
		}
		if(!raw_clip.IsEmpty())
		{
//			ASSERT(Xpixel_raw_bpp > 8); // todo: palette conversions
			int cv_bytes = GetRowBytes(Xpixel_bpp, src.Width(), alignment) * src.Height();
			PixelArray precvt;
			bool precpy = resize, cpyout = false;
			if(format && cv_bytes <= fmtbytes && !mono)
			{
				precvt = PixelConvertScreen(*this);
				precpy = false;
			}
			else if(resize && mono && raw_bpp != 8)
			{
				precvt = PixelConvert(*this, 8, alignment);
				cpyout = true;
			}
			const PixelArray& cpysrc = (precvt.IsEmpty() ? *this : precvt);
//			LOG("Image to paint: " << PixelDump(cpysrc));
			int prebpp = cpyout ? precvt.GetRawBPP() : GetRawBPP();
			for(int c = 0; c < raw_clip.GetCount(); c++)
			{
				Rect dclip = raw_clip[c];
				Size dsize = dclip.Size();
				Rect cdrect(Point(dest.TopLeft() - dclip.TopLeft()), dest.Size());
				PixelArray out(dsize, cpysrc.raw_bpp, alignment, cpysrc.cmask);
				PixelCopy(out, cdrect, cpysrc, src, Rect(dsize));
				PixelSetConvertScreen(out);
//				XImage *in_image = XGetImage(Xdisplay, dw, dclip.left, dclip.top, dclip.Width(), dclip.Height(), -1u, ZPixmap);
//				XFree(in_image);
//				LOG("Image to put: " << PixelDump(out));
				XImage ximage;
				out.SetXImage(ximage);
//				if(mono)
//					ximage.format = XYBitmap;
//				memset(out.Begin(), 0x0F, out.GetBytes());
				XPutImage(Xdisplay, dw, gc, &ximage, 0, 0, dclip.left, dclip.top, dclip.Width(), dclip.Height());
			}
		}
	}

	gcv.function = old_fn;
	gcv.plane_mask = old_mask;
	XChangeGC(Xdisplay, gc, gcv_mask, &gcv);
}
#endif

#ifdef PLATFORM_X11
void PixelArray::PaintGC(GC gc, Drawable dw, const Vector<Rect>& clip, const Rect& src, const Rect& dest, Color fore, Color back, Color doxor) const
{
/*
	{
		String out;
		out << "PixelArray::PaintGC(mono) & " << dest << "; clip =";
		for(int i = 0; i < clip.GetCount(); i++)
			out << " " << clip[i];
		puts(out);
		fflush(stdout);
	}
*/
	sInitPixelDefaults();

	XGCValues gcv;
	int gcv_mask = GCFunction | (mono ? GCForeground | GCBackground : 0);
	XGetGCValues(Xdisplay, gc, gcv_mask, &gcv);
	int old_fn = gcv.function;
	int old_fore = gcv.foreground, old_back = gcv.background;

	bool format = (format_code != Xpixel_format_code || mono && bpp != 1);
	bool resize = (src.Size() != dest.Size());

	XGCValues gcv2;
	GC gc2;
	bool full = (!IsNull(fore) && !IsNull(back)) || !IsNull(doxor);
	if(full)
	{
		if(!IsNull(doxor))
		{
			unsigned xor_pixel = GetXPixel(doxor);
			gcv.background = GetXPixel(Nvl(fore, Black)) ^ xor_pixel;
			gcv.foreground = GetXPixel(Nvl(back, Black)) ^ xor_pixel;
			gcv.function = X11_ROP2_XOR;
		}
		else
		{
			gcv.background = GetXPixel(fore);
			gcv.foreground = GetXPixel(back);
			gcv.function = X11_ROP2_COPY;
		}
	}
	else
	{
		if(IsNull(fore))
		{
			gcv.foreground = ~0;
			gcv2.foreground = gcv.background = 0;
			gcv2.background = GetXPixel(back);
		}
		else
		{
			gcv.background = ~0;
			gcv2.background = gcv.foreground = 0;
			gcv2.foreground = GetXPixel(fore);
		}
		gcv.function = X11_ROP2_AND;
		gcv2.function = X11_ROP2_OR;
		gc2 = XCreateGC(Xdisplay, dw, GCFunction | GCBackground | GCForeground, &gcv2);
	}
	XChangeGC(Xdisplay, gc, gcv_mask, &gcv);

	if(!resize)
	{
		PixelArray cvt;
		const PixelArray *psrc = (format ? &cvt : this);
		if(format)
			cvt = PixelConvertScreen(*this);
		XImage ximage;
		psrc -> SetXImage(ximage);
		for(int c = 0; c < clip.GetCount(); c++)
		{
			Rect dclip = dest & clip[c];
			if(!dclip.IsEmpty())
			{
				Point dsrc = dclip.TopLeft() - dest.TopLeft() + src.TopLeft();
				Size sz = dclip.Size();
				if(full)
					XPutImage(Xdisplay, dw, gc, &ximage, dsrc.x, dsrc.y,
						dclip.left, dclip.top, sz.cx, sz.cy);
				else
				{
					Pixmap px = XCreatePixmap(Xdisplay, Xroot, sz.cx, sz.cy, 1);
					GC tgc = XCreateGC(Xdisplay, px, 0, NULL);
					XPutImage(Xdisplay, px, tgc, &ximage, dsrc.x, dsrc.y, 0, 0, sz.cx, sz.cy);
					XFreeGC(Xdisplay, tgc);
					XCopyPlane(Xdisplay, px, dw, gc, 0, 0, sz.cx, sz.cy, dclip.left, dclip.top, 1);
					XCopyPlane(Xdisplay, px, dw, gc2, 0, 0, sz.cx, sz.cy, dclip.left, dclip.top, 1);
					XFreePixmap(Xdisplay, px);
				}
			}
		}
	}
	else
	{
		int maxbytes = 0;
		int fmtbytes = 0;
		Vector<Rect> raw_clip;
		for(int c = 0; c < clip.GetCount(); c++)
		{
			Rect dclip = clip[c] & dest;
			if(!dclip.IsEmpty())
			{
				raw_clip.Add(dclip);
				fmtbytes = max(fmtbytes, GetRowBytes(bpp, dclip.Width(), alignment) * dclip.Height());
				maxbytes = max(maxbytes, GetRowBytes(Xpixel_bpp, dclip.Width(), alignment) * dclip.Height());
			}
		}
		if(!raw_clip.IsEmpty())
		{
//			ASSERT(Xpixel_raw_bpp > 8); // todo: palette conversions
			int cv_bytes = GetRowBytes(Xpixel_bpp, src.Width(), alignment) * src.Height();
			PixelArray precvt;
			bool precpy = resize, cpyout = false;
			if(format && cv_bytes <= fmtbytes && !mono)
			{
				precvt = PixelConvertScreen(*this);
				precpy = false;
			}
			else if(resize && mono && raw_bpp != 8)
			{
				precvt = PixelConvert(*this, 8, alignment);
				cpyout = true;
			}
			const PixelArray& cpysrc = (precvt.IsEmpty() ? *this : precvt);
			int prebpp = cpyout ? precvt.GetRawBPP() : GetRawBPP();
			for(int c = 0; c < raw_clip.GetCount(); c++)
			{
				Rect dclip = raw_clip[c];
				Size dsize = dclip.Size();
				Rect drect(Point(dest.TopLeft() - dclip.TopLeft()), dest.Size());
				PixelArray out;
				if(mono)
					out.CreateMono(dsize, 8);
				else
					out.Create(dsize, precpy ? prebpp : Xpixel_raw_bpp, alignment, precpy ? cmask : Xpixel_cmask);
				PixelCopy(out, drect, cpysrc, src, Rect(dsize));
				if(format)
					PixelSetConvertScreen(out);
//				XImage *in_image = XGetImage(Xdisplay, dw, dclip.left, dclip.top, dclip.Width(), dclip.Height(), -1u, ZPixmap);
//				XFree(in_image);
				XImage ximage;
				out.SetXImage(ximage);
//				if(mono)
//					ximage.format = XYBitmap;
//				memset(out.Begin(), 0x0F, out.GetBytes());
				if(full)
					XPutImage(Xdisplay, dw, gc, &ximage, 0, 0, dclip.left, dclip.top, dsize.cx, dsize.cy);
				else
				{
					Pixmap px = XCreatePixmap(Xdisplay, Xroot, dsize.cx, dsize.cy, 1);
					GC tgc = XCreateGC(Xdisplay, px, 0, NULL);
					XPutImage(Xdisplay, px, tgc, &ximage, 0, 0, 0, 0, dsize.cx, dsize.cy);
					XFreeGC(Xdisplay, tgc);
					XCopyPlane(Xdisplay, px, dw, gc, 0, 0, dsize.cx, dsize.cy, dclip.left, dclip.top, 1);
					XCopyPlane(Xdisplay, px, dw, gc2, 0, 0, dsize.cx, dsize.cy, dclip.left, dclip.top, 1);
					XFreePixmap(Xdisplay, px);
				}
			}
		}
	}

	if(!full)
		XFreeGC(Xdisplay, gc2);

	gcv.function = old_fn;
	gcv.foreground = old_fore;
	gcv.background = old_back;
	XChangeGC(Xdisplay, gc, gcv_mask, &gcv);
}
#endif

#ifdef PLATFORM_X11
void PixelArray::SetXImage(XImage& ximage) const
{
	ximage.width            = size.cx;
	ximage.height           = size.cy;
	ximage.xoffset          = 0;
	ximage.format           = (mono && raw_bpp == 1 ? XYBitmap : ZPixmap);
	ximage.data             = (char *)Begin();
#ifdef CPU_LITTLE_ENDIAN
	ximage.byte_order       = LSBFirst;
#else
	ximage.byte_order       = MSBFirst;
#endif
	ximage.bitmap_unit      = max(bpp, 8);
	ximage.bitmap_bit_order = MSBFirst;
	ximage.bitmap_pad       = alignment << 3;
	ASSERT(raw_bpp > 0);
	ximage.depth            = raw_bpp;
	ximage.bytes_per_line   = GetRowBytes();
	ximage.bits_per_pixel   = bpp;
	ximage.blue_mask        = cmask[0];
	ximage.green_mask       = cmask[1];
	ximage.red_mask         = cmask[2];
	ximage.obdata           = NULL;
	XInitImage(&ximage);
}
#endif

#ifdef PLATFORM_WIN32
bool PixelArray::SetHeader(Header& header, const Color *c) const
{
	if(IsEmpty())
		return false;
	ASSERT(alignment == 4); // WIN32 doesn't support other alignments
	Zero(header);
	header.biSize = sizeof(BITMAPINFOHEADER);
	header.biWidth = size.cx;
	header.biHeight = size.cy;
	header.biBitCount = bpp;
	header.biPlanes = 1;
	header.biCompression = BI_RGB;
//	header.biClrUsed = bpp <= 8 ? max(2, palette.GetCount()) : 0;
	if(bpp <= 8)
	{
		if(!c)
			c = palette.Begin();
		for(RGBQUAD *p = header.palette, *e = p + (bpp <= 8 ? palette.GetCount() : 0); p < e; p++, c++)
		{
			COLORREF cr = *c;
			p -> rgbRed   = GetRValue(cr);
			p -> rgbGreen = GetGValue(cr);
			p -> rgbBlue  = GetBValue(cr);
		}
	}
	else if(bpp == 16 || bpp == 32)
	{
		dword *bitfields = reinterpret_cast<dword *>(header.palette);
		bitfields[0] = cmask[0];
		bitfields[1] = cmask[1];
		bitfields[2] = cmask[2];
		header.biCompression = BI_BITFIELDS;
	}
	return true;
}
#endif

PixelArray PixelConvert(const PixelArray& src, int to_bpp, int alignment, const dword *cmasks, const Vector<Color>& palette)
{
	ASSERT(to_bpp == -3 || to_bpp == 1 || to_bpp == 4 || to_bpp >= 8 && to_bpp <= 32);
	if(src.IsEmpty())
		return PixelArray();
	int sbpp = src.GetRawBPP();
	if((to_bpp == sbpp) && (to_bpp <= 8 || to_bpp == -3))
		return PixelArray(src, 0);
	if(to_bpp > 8 || to_bpp == -3)
	{
		PixelArray tmp(src.GetSize(), to_bpp, alignment, cmasks);
		PixelReader24 reader(src);
		PixelWriter24 writer(tmp);
		for(int y = 0; y < src.GetHeight(); y++)
		{
			memcpy(writer[y], reader[y], src.GetWidth() * 3);
			writer.Write();
		}
		return tmp;
	}
	else
	{
		if(sbpp > 8 || sbpp == -3 || src.GetPalette().GetCount() > (1 << to_bpp))
		{ // map palette
			if(sbpp == -3 || sbpp <= 8)
				return CreatePalette(src, to_bpp, -1, palette);
			else
				return CreatePalette(PixelConvert(src, -3), to_bpp, -1, palette);
		}

		PixelArray result;
		if(src.IsMono())
			result.CreateMono(src.GetSize(), to_bpp, alignment);
		else
			result.Create(src.GetSize(), to_bpp, alignment, NULL, palette, 0);

		if(sbpp == 8)
		{
			switch(to_bpp)
			{
			case 1:  Convert8to1(src, result.Begin(), alignment); break;
			case 4:  Convert8to4(src, result.Begin(), alignment); break;
			case 8:  break;
			default: NEVER(); break;
			}
		}
		else if(sbpp == 4)
		{
			switch(to_bpp)
			{
			case 1:  Convert4to1(src, result.Begin(), alignment); break;
			case 4:  break;
			case 8:  Convert4to8(src, result.Begin(), alignment); break;
			default: NEVER(); break;
			}
		}
		else if(sbpp == 1)
		{
			switch(to_bpp)
			{
			case 1:  break;
			case 4:  Convert1to4(src, result.Begin(), alignment); break;
			case 8:  Convert1to8(src, result.Begin(), alignment); break;
			default: NEVER(); break;
			}
		}
		else
			NEVER();
		if(result.palette.IsEmpty())
			result.palette.Append(src.palette, 0, min(src.palette.GetCount(), 1 << to_bpp));
		return result;
	}
}

void PixelSetConvert(PixelArray& image, int to_bpp, int alignment,
	const dword *cmasks, const Vector<Color>& palette)
{
	ASSERT(to_bpp == -3 || to_bpp == 1 || to_bpp == 4 || to_bpp >= 8 && to_bpp <= 32);
	if(image.IsEmpty())
		return;
	Size size = image.GetSize();
	int bpp = image.GetRawBPP();
	if((to_bpp == bpp) && (to_bpp <= 8 || to_bpp == -3))
		return;
	if(to_bpp > 8 || to_bpp == -3)
	{
		image = PixelConvert(image, to_bpp, alignment, cmasks);
		return;
	}
	else if(bpp > 8 || bpp == -3 || palette.GetCount() > (1 << to_bpp))
	{ // map palette
		image = CreatePalette(image, to_bpp, -1, palette);
	}
	else if(bpp == 8)
	{
		switch(to_bpp)
		{
		case 1: Convert8to1(image, image.Begin(), alignment); break;
		case 4: Convert8to4(image, image.Begin(), alignment); break;
		case 8: break;
		default: NEVER(); break;
		}
	}
	else if(bpp == 4)
	{
		switch(to_bpp)
		{
		case 1:   Convert4to1(image, image.Begin(), alignment); break;
		case 8:
			{
				Vector<byte> aux;
				aux.SetCount(size.cy * PixelArray::GetRowBytes(8, size.cx, alignment));
				Convert4to8(image, aux.Begin(), alignment);
				image.pixels = aux;
			}
			break;

		case 4:   break;
		default:  NEVER(); break;
		}
	}
	else if(bpp == 2)
	{
		Vector<byte> aux;
		if(to_bpp == 4 || to_bpp == 8)
		{
			int row = PixelArray::GetRowBytes(to_bpp, size.cx, alignment);
			aux.SetCount(size.cy * row);
			if(to_bpp == 4)
				Convert2to4(image, aux.Begin(), alignment);
			else
			{
				int row4 = PixelArray::GetRowBytes(4, size.cx, alignment);
				byte *end = aux.End() - row4 * size.cy;
				byte *out = aux.Begin();
				Convert2to4(image, end, alignment);
				for(int i = 0; i < size.cy; i++, end += row4, out += row)
					Blt4to8(out, end, size.cx);
			}
			image.pixels = aux;
		}
		else
			NEVER();
	}
	else if(bpp == 1)
	{
		Vector<byte> aux;
		switch(to_bpp)
		{
		case 1:
			break;

		case 4:
			aux.SetCount(size.cy * PixelArray::GetRowBytes(4, size.cx, alignment));
			Convert1to4(image, aux.Begin(), alignment);
			image.pixels = aux;
			break;

		case 8:
			aux.SetCount(size.cy * PixelArray::GetRowBytes(8, size.cx, alignment));
			Convert1to8(image, aux.Begin(), alignment);
			image.pixels = aux;
			break;

		default:
			NEVER();
			break;
		}
	}
	else
		NEVER();
	image.SetFormat(to_bpp, alignment);
	if(to_bpp > 8)
		image.palette.Clear();
	else if(image.palette.GetCount() > 1 << bpp)
		image.palette.SetCount(1 << bpp);
	if(to_bpp == 16 || to_bpp == 32)
		image.SetMasks(cmasks);
	image.pixels.SetCount(image.GetRowBytes() * size.cy);
	image.pixels.Shrink();
}

void PixelGetRGBScan(const PixelArray& image, int y, byte *dest)
{
	const byte *src = image.GetDownScan(y);
	int count = image.GetWidth();
	byte *top = dest + 2 * count;
	int bpp = image.GetRawBPP();
	if(bpp == -3)
	{
		memcpy(dest, src, 3 * count);
		return;
	}
	if(bpp <= 8)
	{
		const byte *pos = src;
		if(bpp < 8)
		{
			pos = top;
			switch(bpp)
			{
			case 1: Blt1to8(top, src, count); break;
			case 4: Blt4to8(top, src, count); break;
			default:
				NEVER();
			}
		}
		if(image.palette.IsEmpty())
		{ // system palette
#ifdef PLATFORM_X11
			extern byte *Xunmapcolor;
			while(--count >= 0)
			{
				const byte *s = Xunmapcolor + 3 * *pos++;
				dest[0] = s[0];
				dest[1] = s[1];
				dest[2] = s[2];
				dest += 3;
			}
#endif
		}
		else
		{ // image-specific palette
			while(--count >= 0)
			{
				Color c = (*pos < image.palette.GetCount() ? image.palette[(int)*pos] : Black);
				dest[0] = c.GetB();
				dest[1] = c.GetG();
				dest[2] = c.GetR();
				dest += 3;
				pos++;
			}
		}
	}
	else if(bpp == 16 || bpp == 32)
	{
		byte sbits[6];
		image.GetMaskInfo(sbits);
		if(bpp == 16)
			Blt16to24(dest, src, count, sbits);
		else
			Blt32to24(dest, src, count, sbits);
	}
	else
	{
		NEVER();
	}
}

PixelArray PixelConvertScreen(const PixelArray& image)
{
	if(image.IsEmpty())
		return Null;
#ifdef PLATFORM_X11
	if(image.IsMono())
		return PixelConvert(image, 1);
	if(Xpixel_raw_bpp > 8)
		return PixelConvert(image, Xpixel_raw_bpp, 4, Xpixel_cmask);
	Size size = image.GetSize();
	PixelArray out(size, 8, 4);
	extern byte *Xmapcolor;
	ASSERT(Xmapcolor);
	Buffer<byte> rowbuf(3 * size.cx);
	for(int i = 0; i < size.cy; i++)
	{
		PixelGetRGBScan(image, i, rowbuf);
		const byte *src = rowbuf;
		for(byte *dest = out.GetDownScan(i), *end = dest + size.cx; dest < end; dest++, src += 3)
			*dest = Xmapcolor[(((src[2] * 12) >> 8) * 24 + ((src[1] * 24) >> 8)) * 12 + ((src[0] * 12) >> 8)];
	}
	return out;
#else
	return PixelArray(image, 0);
#endif
}

#ifdef PLATFORM_GDI
void PixelSetConvertScreen(PixelArray& image)
{
#ifdef PLATFORM_WIN32
	return;
#else
	if(image.IsMono() && image.raw_bpp == 1
	|| !image.IsMono() && image.raw_bpp == Xpixel_raw_bpp)
		return;
	image = PixelConvertScreen(image);
#endif
}
#endif

void PixelSet(PixelArray& image, const Rect& dest, Color color)
{
	BLT_TIMING("PixelSet(Color)");
	Rect r = dest & image.GetRect();
	if(r.IsEmpty())
		return;
	ASSERT(image.GetRawBPP() == -3);
	byte temp[12];
	temp[0] = temp[3] = temp[6] = temp[9] = color.GetB();
	temp[1] = temp[4] = temp[7] = temp[10] = color.GetG();
	temp[2] = temp[5] = temp[8] = temp[11] = color.GetR();
	byte *d = image.GetDownScan(r.top) + 3 * r.left, *rw = d;
	int wd = r.Width();
#ifdef I386_ASSEMBLY
	int row_count = r.Height();
__3:
	__asm mov edi, [d]
	__asm mov ecx, [wd]
	__asm mov eax, dword ptr [temp]
	__asm mov ebx, dword ptr [temp + 4]
	__asm mov edx, dword ptr [temp + 8]
	__asm shr ecx, 4
	__asm jcxz __2
__7:
	__asm mov [edi], eax
	__asm mov [edi + 4], ebx
	__asm mov [edi + 8], edx
	__asm mov [edi + 12], eax
	__asm mov [edi + 16], ebx
	__asm mov [edi + 20], edx
	__asm mov [edi + 24], eax
	__asm mov [edi + 28], ebx
	__asm mov [edi + 32], edx
	__asm mov [edi + 36], eax
	__asm mov [edi + 40], ebx
	__asm mov [edi + 44], edx
	__asm add edi, 48
	__asm loop __7
__2:
	__asm mov ecx, [wd]
	__asm test cl, 8
	__asm je __1
	__asm mov [edi], eax
	__asm mov [edi + 4], ebx
	__asm mov [edi + 8], edx
	__asm mov [edi + 12], eax
	__asm mov [edi + 16], ebx
	__asm mov [edi + 20], edx
	__asm add edi, 24
__1:
	__asm test cl, 4
	__asm je __6
	__asm mov [edi], eax
	__asm mov [edi + 4], ebx
	__asm mov [edi + 8], edx
	__asm add edi, 12
__6:
	__asm test cl, 2
	__asm je __4
	__asm mov [edi], eax
	__asm mov [edi + 4], bx
	__asm add edi, 6
__4:
	__asm test cl, 1
	__asm je __5
	__asm mov [edi], ax
	__asm mov [edi + 2], dl
__5:
	d += image.GetDownRowBytes();
	if(--row_count > 0) goto __3;
#else
	while(wd >= 4)
	{
		memcpy(d, temp, 12);
		d += 12;
		wd -= 4;
	}
	if(wd & 2)
	{
		memcpy(d, temp, 6);
		d += 6;
	}
	if(wd & 1)
		memcpy(d, temp, 3);
	wd = 3 * r.Width();
	while(++r.top < r.bottom)
	{
		const byte *from = rw;
		memcpy(rw += image.GetDownRowBytes(), from, wd);
	}
#endif
}

void PixelSet(PixelArray& image, const Rect& dest, byte value)
{
	BLT_TIMING("PixelSet(byte)");
	if(image.GetBPP() > 8)
	{
		PixelSet(image, dest, Color(value, value, value));
		return;
	}
	ASSERT(image.GetBPP() == 8); // ? todo
	Rect r = dest & image.GetRect();
	if(r.IsEmpty())
		return;
	byte *d = image.GetDownScan(r.top) + r.left; // 8 bit only
	int wd = r.Width();
	for(int i = r.Height(); --i >= 0; d += image.GetDownRowBytes())
		memset(d, value, wd);
}

int AlphaArray::SizeOfInstance() const
{
	return pixel.SizeOfInstance() + alpha.SizeOfInstance();
}

void AlphaArray::Create(int cx, int cy,
	int bpp, int alignment, const dword *cmasks, pick_ Vector<Color>& palette,
	int mask_bpp, int mask_align)
{
	pixel.Create(cx, cy, bpp, alignment, cmasks, palette);
	if(mask_bpp)
		alpha.CreateMono(cx, cy, mask_bpp, mask_align);
	else
		alpha.Clear();
}

void AlphaArray::CreateAlpha(int mask_bpp, int mask_align, bool init0)
{
	if(!IsEmpty())
	{
		alpha.CreateMono(pixel.GetWidth(), pixel.GetHeight(), mask_bpp, mask_align);
		if(init0)
			memset(alpha.Begin(), 0, alpha.GetBytes());
	}
}

void AlphaArray::Serialize(Stream& stream)
{
	int version = 1;
	stream / version % pixel % alpha;
//	LOG((stream.IsLoading() ? "LOAD " : "SAVE ") << AlphaDump(*this));
}

void AlphaArray::Paint(Draw& draw, const Rect& src, const Rect& dest) const
{
	if(!HasAlpha())
		pixel.Paint(draw, src, dest, PIXEL_COPY);
	else
	{
		pixel.Paint(draw, src, dest, PIXEL_XOR);
		alpha.Paint(draw, src, dest, Black, Null);
		pixel.Paint(draw, src, dest, PIXEL_XOR);
	}
}

AlphaArray AlphaConvert(const AlphaArray& image,
	int pixel_bpp, int pixel_align, const dword *cmasks, const Vector<Color>& palette,
	int alpha_bpp, int alpha_align)
{
	PixelArray pixel, alpha;
	pixel = PixelConvert(image.pixel, pixel_bpp, pixel_align, cmasks, palette);
	alpha = PixelConvert(image.alpha, alpha_bpp, alpha_align, NULL, MonoPalette());
	return AlphaArray(pixel, alpha);
}

void AlphaScanToRLE(String& result, const byte *dp, const byte *mp, int width)
{
	const byte *me = mp + width;
	while(mp < me)
	{
		if(*mp)
		{ // skip run
			const byte *ms = mp;
			do
			{
				mp++;
				dp += 3;
			}
			while(mp < me && *mp);
			if(mp == me)
				break;
			int count = mp - ms;
			while(count > 0)
			{
				int part = min(count, 0x3F);
				result.Cat(0xC0 + part);
				count -= part;
			}
		}
		if(mp < me)
		{
			const byte *ms = mp;
			while(++mp < me && !*mp)
				;
			const byte *de = dp + 3 * (mp - ms - 1);
			const byte *dr = dp;
			while(dp <= de)
				if(dp < de && dp[0] == dp[3] && dp[1] == dp[4] && dp[2] == dp[5])
				{ // flush last enum segment, generate repeat segment
					if(dp > dr)
					{ // enum segment
						int count = (dp - dr) / 3;
						while(count > 0)
						{
							int part = min(count, 0x7F);
							result.Cat(part);
							result.Cat((const char *)dr, 3 * part);
							dr += 3 * part;
							count -= part;
						}
					}

					const byte *ds = dp;
					while((dp += 3) < de && dp[0] == dp[3] && dp[1] == dp[4] && dp[2] == dp[5])
						;
					dr = dp += 3;
					int count = (dp - ds) / 3;
					while(count > 0)
					{
						int part = min(count, 0x3F);
						result.Cat(0x80 + part);
						result.Cat((const char *)dp - 3, 3);
						count -= part;
					}
				}
				else
					dp += 3;
			if(dp > dr)
			{ // last enum segment
				int count = (dp - dr) / 3;
				while(count > 0)
				{
					int part = min(count, 0x7F);
					result.Cat(part);
					result.Cat((const char *)dr, 3 * part);
					dr += 3 * part;
					count -= part;
				}
			}
		}
	}
}

String AlphaToRLE(const AlphaArray& aa)
{
	ASSERT(aa.GetPixelBPP() == 24 && aa.GetAlphaBPP() == 8);
	String result;
	int last = 0;
	for(int y = 0; y < aa.GetHeight(); y++)
	{
		int prev = result.GetLength();
		AlphaScanToRLE(result, aa.GetPixelDownScan(y), aa.GetAlphaDownScan(y), aa.GetWidth());
		if(result.GetLength() > prev)
			last = result.GetLength();
		result.Cat(0x80); // row end
	}
	result.Trim(last);
	return result;
}

const byte *RLEToAlphaScan(const byte *src, byte *dp, byte *mp, int width)
{
	byte *me = mp + width;
//	int $done = 0;
	for(;;)
	{
//		ASSERT($done == (mp - Mask(0, y)));
		if(*src & 0x80)
		{
//			$done += *src & 0x3F;
			if(*src == 0x80) // end of row
				return src;
			int count = min<int>(*src & 0x3F, me - mp);
			if(*src++ & 0x40)
			{ // skip segment
				dp += 3 * count;
				mp += count;
			}
			else
			{ // repeat segment
				memset(mp, 0, count);
				memsetex(dp, src, 3, count);
				dp += 3 * count;
				mp += count;
				src += 3;
			}
		}
		else
		{ // enum row / end of row
//			$done += *src & 0x7F;
			if(*src == 0)
				return src; // end of image
			const byte *new_src = src + 1 + 3 * *src;
			int count = min<int>(*src++, me - mp);
			memset(mp, 0, count);
			memcpy(dp, src, 3 * count);
			dp += 3 * count;
			mp += count;
			src = new_src;
		}
	}
}

void RLEToAlpha(AlphaArray& aa, const byte *src)
{
	AlphaSet(aa, aa.GetRect(), Color(Null));
	for(int y = 0; y < aa.GetHeight()
		&& *(src = RLEToAlphaScan(src, aa.GetPixelDownScan(y), aa.GetAlphaDownScan(y), aa.GetWidth())); y++, src++)
		;
}

void AlphaSet(AlphaArray& dest, const Rect& drect, Color color)
{
	PixelSet(dest.pixel, drect, Nvl(color, Black));
	if(dest.HasAlpha())
		PixelSet(dest.alpha, drect, !IsNull(color) ? 0 : 1);
}

void AlphaSet(AlphaArray& dest, const Rect& drect, byte value)
{
	PixelSet(dest.pixel, drect, value);
	if(dest.HasAlpha())
		PixelSet(dest.alpha, drect, 1);
}

void AlphaCopy(PixelArray& dest, const Rect& drect, const AlphaArray& src, const Rect& srect, const Rect& clip)
{
	if(src.HasAlpha())
	{
		PixelCopy(dest, drect, src.alpha, srect, clip, BltOp2And);
		PixelCopy(dest, drect, src.pixel, srect, clip, BltOp2Xor);
	}
	else
		PixelCopy(dest, drect, src.pixel, srect, clip);
}

void AlphaCopy(AlphaArray& dest, const Rect& drect, const AlphaArray& src, const Rect& srect, bool merge, const Rect& clip)
{
	if(merge && src.HasAlpha()) {
		AlphaCopy(dest.pixel, drect, src, srect, clip);
		if(dest.HasAlpha())
			PixelCopy(dest.alpha, drect, src.alpha, srect, clip, BltOp2And);
	}
	else {
		if(!src.pixel.IsEmpty() && !dest.pixel.IsEmpty())
			PixelCopy(dest.pixel, drect, src.pixel, srect, clip);
		if(dest.HasAlpha()) {
			if(src.HasAlpha())
				PixelCopy(dest.alpha, drect, src.alpha, srect, clip);
			else
				PixelSet(dest.alpha, drect, 0);
		}
		return;
	}
}

AlphaArray AlphaCrop(const AlphaArray& src, const Rect& rc)
{
	AlphaArray out;
	out.pixel = PixelCrop(src.pixel, rc);
	if(src.HasAlpha())
		out.alpha = PixelCrop(src.alpha, rc);
	return out;
}

void AlphaGray(AlphaArray& aa, const Rect& drect)
{
	ASSERT(aa.GetPixelBPP() == 24);
	Rect r = drect & aa.GetRect();
	if(r.IsEmpty())
		return;
	const byte *gamma = GetGammaTable();
	int w3 = 3 * r.Width();
	for(int y = r.top; y < r.bottom; y++)
		for(byte *b = aa.GetPixelDownScan(y) + 3 * r.left, *e = b + w3; b < e; b += 3)
			b[0] = b[1] = b[2] = gamma[CalcGray(b) >> (16 - GAMMA_BITS)];
}

void AlphaEtch(AlphaArray& aa, const Rect& drect)
{
	ASSERT(aa.GetPixelBPP() == 24 && aa.GetAlphaBPP() == 8);
	Rect r = drect & aa.GetRect();
	if(r.IsEmpty())
		return;
	int w3 = 3 * r.Width();
	for(int y = r.bottom; --y > r.top;)
	{
		byte *db = aa.GetPixelDownScan(y) + 3 * r.left, *de = db + w3, *mb = aa.GetAlphaDownScan(y) + r.left;
		const byte *ldb = aa.GetPixelDownScan(y - 1) + 3 * r.left, *lmb = aa.GetAlphaDownScan(y - 1) + r.left;
		if(!*mb && CalcGray(db) < 0x8000)
			db[0] = db[1] = db[2] = 0x80;
		else
		{
			db[0] = db[1] = db[2] = 0;
			*mb = 1;
		}
		while(++mb, (db += 3) < de)
		{
			if(!*mb && CalcGray(db) < 0x8000)
				db[0] = db[1] = db[2] = 0x80;
			else if(!*lmb && CalcGray(ldb) < 0x8000)
			{
				db[0] = db[1] = db[2] = 0xFF;
				*mb = 0;
			}
			else
			{
				db[0] = db[1] = db[2] = 0;
				*mb = 1;
			}
			lmb++;
			ldb += 3;
		}
	}

	{ // 1st row must be handled in a special way
		for(byte *db = aa.GetPixelDownScan(r.top) + 3 * r.left, *de = db + w3, *mb = aa.GetAlphaDownScan(r.top) + r.left; db < de; db += 3, mb++)
			if(!*mb && CalcGray(db) < 0x8000)
				db[0] = db[1] = db[2] = 0x80;
			else
			{
				db[0] = db[1] = db[2] = 0;
				*mb = 1;
			}
	}
}

static void BltMono24(byte *dest, const byte *src, unsigned count, unsigned threshold, bool inv)
{
	byte ii = (inv ? 1 : 0);
#define BLT_MONO(o) dest[(o)] = ii ^ (CalcGray(src + 3 * (o)) >= threshold ? 1 : 0);
#define BLT_MONO_4(o) BLT_MONO((o) + 0) BLT_MONO((o) + 1) BLT_MONO((o) + 2) BLT_MONO((o) + 3)

	for(unsigned rep = count >> 4; rep; rep--)
	{
		BLT_MONO_4(0)
		BLT_MONO_4(4)
		BLT_MONO_4(8)
		BLT_MONO_4(12)
		src += 16 * 3;
		dest += 16;
	}
	if(count & 8)
	{
		BLT_MONO_4(0)
		BLT_MONO_4(4)
		src += 8 * 3;
		dest += 8;
	}
	if(count & 4)
	{
		BLT_MONO_4(0)
		src += 4 * 3;
		dest += 4;
	}
	if(count & 2)
	{
		BLT_MONO(0)
		BLT_MONO(1)
		src += 2 * 3;
		dest += 2;
	}
	if(count & 1)
	{
		BLT_MONO(0)
	}
}

PixelArray PixelMono(const PixelArray& aa, byte threshold, bool inv, int outbpp)
{
	ASSERT(outbpp == 1 || outbpp == 4 || outbpp == 8);
	PixelArray out;
	if(aa.IsEmpty())
		return out;
	if(aa.IsMono())
		return PixelConvert(aa, outbpp);
	if(aa.GetBPP() == 1)
	{
		out = PixelConvert(aa, outbpp);
		out.SetMono(true);
		if(out.palette.GetCount() >= 2 && CalcGray(out.palette[0]) > CalcGray(out.palette[1]))
			inv = !inv;
		if(inv)
			BltOp1Xor(out.Begin(), outbpp == 8 ? 1 : 255, out.GetBytes());
		return out;
	}
	out.CreateMono(aa.GetSize(), outbpp);
	unsigned thr8 = threshold << 8;
	void (*store)(byte *dest, const byte *src, unsigned count);
	switch(outbpp)
	{
	case 1: store = Blt8to1; break;
	case 4: store = Blt8to4; break;
	case 8: store = NULL; break;
	default: NEVER(); return Null;
	}
	Buffer<byte> scanbuf(store ? out.GetWidth() : 0);
	if(aa.GetBPP() <= 8)
	{
		byte xlat[256];
		memset(xlat, 0, sizeof(xlat));
#ifdef PLATFORM_X11
		if(aa.palette.IsEmpty())
		{
			extern byte *Xunmapcolor;
			const byte *p = Xunmapcolor;
			for(int x = 0; x < 256; x++, p += 3)
				xlat[x] = (CalcGray(p[2], p[1], p[0]) >= thr8 ? 1 : 0) ^ (inv ? 1 : 0);
		}
#endif
		for(int x = 0; x < aa.palette.GetCount(); x++)
			xlat[x] = (CalcGray(aa.palette[x]) >= thr8 ? 1 : 0) ^ (inv ? 1 : 0);
		for(int y = 0; y < out.GetHeight(); y++)
		{
			byte *imgscan = out.GetDownScan(y), *scan = (store ? ~scanbuf : imgscan);
			BltXlatB(scan, PixelGetPixel8(aa, 0, y, out.GetWidth()), out.GetWidth(), xlat);
			if(store)
				store(imgscan, scan, out.GetWidth());
		}
	}
	else
	{
		for(int y = 0; y < out.GetHeight(); y++)
		{
			byte *imgscan = out.GetDownScan(y), *scan = (store ? ~scanbuf : imgscan);
			BltMono24(scan, PixelGetPixel24(aa, 0, y, out.GetWidth()), out.GetWidth(), thr8, inv);
			if(store)
				store(imgscan, scan, out.GetWidth());
		}
	}
	return out;
}

void AlphaImageInfo::Serialize(Stream& stream)
{
	int version = 1;
	stream / version;
	if(version >= 1)
		stream % size % hotspot % encoding;
}
