//////////////////////////////////////////////////////////////////////
// PixelUtil.cpp: pixel image utility functions. To be moved somewhere else.

#include "Draw.h"
#pragma hdrstop
#include "PixelUtil.h"

#if defined(CPU_IA32) && defined(COMPILER_MSC)
#define I386_ASSEMBLY
#endif

#ifdef I386_ASSEMBLY
#define BLT_TIMING(routine) //RTIMING(#routine " (asm)")
#else
#define BLT_TIMING(routine) //RTIMING(#routine " (cpp)")
#endif

#ifdef PLATFORM_WIN32
void PixelArrayDraw::Open(Size sz, int b)
{
	Close();
	size = sz;
	bpp = b;
	if(size.cx > 0 && size.cy > 0 && bpp != 0) {
		PixelArray::Header header;
		Zero(header);
		header.biSize = sizeof(BITMAPINFOHEADER);
		header.biWidth = size.cx;
		header.biHeight = size.cy;
		header.biBitCount = (bpp > 0 ? bpp : 24);
		header.biPlanes = 1;
		header.biCompression = BI_RGB;
		bitmap = CreateDIBSection(ScreenInfo(), header, DIB_RGB_COLORS, &bits, NULL, 0);
		handle = ::CreateCompatibleDC(ScreenInfo());
		oldbmp = (HBITMAP) ::SelectObject(handle, bitmap);
		Init();
		backdraw = true;
	}
}

void PixelArrayDraw::Close()
{
	RDUMP(GdiGetFreeSpace());
	if(handle) {
		HDC hdc = Detach();
		SelectObject(handle, oldbmp);
		DeleteObject(bitmap);
		bitmap = oldbmp = NULL;
		DeleteDC(hdc);
	}
}

PixelArray PixelArrayDraw::GetResult() const
{
	if(size.cx <= 0 || size.cy <= 0 || !bpp || !bitmap || !bits)
		return Null;
	PixelArray out(size, bpp);
	memcpy(out.Begin(), bits, out.GetBytes());
	return out;
}
#endif

PixelReader8::PixelReader8(const PixelArray& array)
: array(array)
, data(NULL)
, last_y(-1)
{
	switch(array.GetBPP())
	{
	case 1: decode = &Blt1to8; break;
	case 4: decode = &Blt4to8; break;
	case 8: decode = NULL; break;
	default: NEVER(); break;
	}
	if(decode)
	{
		buffer.SetCount(array.GetWidth());
		data = buffer.Begin();
	}
}

const byte *PixelReader8::Get(int y) const
{
	if(y != last_y)
	{
		if(decode)
			decode(buffer.Begin(), array.GetDownScan(y), array.GetWidth());
		else
			data = array.GetDownScan(y);
		last_y = y;
	}
	return data;
}

PixelReader24::PixelReader24(const PixelArray& array)
: array(array)
, decode(NULL)
, data(NULL)
, last_y(-1)
{
	palette = (array.GetBPP() <= 8);
	switch(array.GetBPP())
	{
	case 16: decode = &Blt16to24; array.GetMaskInfo(mask_info); break;
	case 24: decode = NULL; break;
	case 32: decode = &Blt32to24; array.GetMaskInfo(mask_info); break;
	default: ASSERT(palette); break;
	}
	if(array.GetBPP() != 24)
		buffer.SetCount(3 * array.GetWidth());
	data = buffer.Begin();
}

const byte *PixelReader24::Get(int y) const
{
	if(y == last_y) return data;
	if(palette)
	{
		buffer = PixelGetPixel24(array, 0, y, array.GetWidth());
		data = buffer.Begin();
	}
	else if(decode)
		decode(buffer.Begin(), array.GetDownScan(y), array.GetWidth(), mask_info);
	else
		data = array.GetDownScan(y);
	last_y = y;
	return data;
}

PixelWriter8::PixelWriter8(PixelArray& array)
: array(array)
, data(NULL)
, last_y(-1)
{
	switch(array.GetBPP())
	{
	case 1: encode = &Blt8to1; break;
	case 4: encode = &Blt8to4; break;
	case 8: encode = NULL; break;
	default: NEVER(); break;
	}
	if(encode)
	{
		buffer.SetCount(array.GetWidth());
		data = buffer;
	}
}

byte *PixelWriter8::Get(int y)
{
	ASSERT(last_y < 0);
	if(!encode)
		data = array.GetDownScan(y);
	last_y = y;
	return data;
}

void PixelWriter8::Write()
{
	ASSERT(last_y >= 0);
	if(encode)
		encode(array.GetDownScan(last_y), data, array.GetWidth());
	last_y = -1;
}

PixelWriter24::PixelWriter24(PixelArray& array)
: array(array)
, data(NULL)
, last_y(-1)
{
	switch(array.GetBPP())
	{
	case 16: encode = &Blt24to16; break;
	case 24: encode = NULL; break;
	case 32: encode = &Blt24to32; break;
	}
	if(encode)
	{
		array.GetMaskInfo(mask_info);
		buffer.SetCount(3 * array.GetWidth());
		data = buffer.Begin();
	}
}

byte *PixelWriter24::Get(int y)
{
	ASSERT(last_y < 0);
	if(!encode)
		data = array.GetDownScan(y);
	last_y = y;
	return data;
}

void PixelWriter24::Write()
{
	ASSERT(last_y >= 0);
	if(encode)
		encode(array.GetDownScan(last_y), data, array.GetWidth(), mask_info);
	last_y = -1;
}

String PixelDump(const PixelArray& image, const Rect& rc)
{
	String out;
	out << "Image(" << image.GetWidth() << " x " << image.GetHeight()
	<< ", bpp = " << image.GetBPP() << ", alignment = " << image.GetAlign()
	<< (image.IsMono() ? ", mono" : ", color") << ")\n";
	if(image.GetBPP() == 16 || image.GetBPP() == 32)
		out << "shift24 / width: R(" << (int)image.cshift24[2] << " / " << (int)image.cbits[2]
		<< "), G(" << (int)image.cshift24[1] << " / " << (int)image.cbits[1]
		<< "), B(" << (int)image.cshift24[0] << " / " << (int)image.cbits[0] << ")\n";
	if(image.GetWidth() <= 0 || image.GetHeight() <= 0 || image.GetBPP() == 0)
		return out;
	Rect do_rc = Nvl(rc, image.GetRect());
	for(int y = do_rc.top; y < do_rc.bottom; y++)
	{
		String row;
		for(int x = do_rc.left; x < do_rc.right; x++)
		{
			Color c = PixelGetPixel(image, x, y);
			if(IsNull(c))
				row << "|...";
			else
			{
				static const char hex[] = "0123456789abcdef";
				row << "|" << hex[c.GetR() >> 4] << hex[c.GetG() >> 4] << hex[c.GetB() >> 4];
			}
		}
		out << "[" << Format("%2d", y) << "]: " << row << "\n";
	}
	return out;
}

String AlphaDump(const AlphaArray& image, const Rect& rc)
{
	String out;
	out << "Pixels: " << PixelDump(image.pixel, rc)
	<< "Alpha channel: " << PixelDump(image.alpha, rc);
	return out;
}

/*
static PixelArray Transform8(const PixelArray& src, int flags)
{
	if(!src)
		return Null;
#if !IMAGE_ORIGIN_BOTTOM
	if(flags & IMAGE_TD)
		flags ^= IMAGE_TX | IMAGE_TY;
#endif//IMAGE_ORIGIN_BOTTOM
	Size in_size = src.GetSize(), out_size = in_size;
	ASSERT(in_size.cx > 0 && in_size.cy > 0 && src.GetBPP() == 8);
	if(flags & IMAGE_TD)
		Swap(out_size.cx, out_size.cy);
	PixelArray out(out_size, 8);
	int out_row = out.GetRowBytes();
	out.palette <<= src.palette;
	for(int i = 0; i < in_size.cy; i++)
	{
		const byte *s = src.GetUpScan(i), *e = s + in_size.cx;
		if(flags & IMAGE_TD)
		{
			byte *d = out.Begin() + (flags & IMAGE_TY ? i : (in_size.cy - 1 - i));
			int delta = out.GetRowBytes();
			if(!(flags & IMAGE_TX))
			{
				d += out_row * (in_size.cx - 1);
				delta = -out_row;
			}
			for(; s < e; s++, d += delta)
				*d = *s;
		}
		else
		{
			byte *d = out.GetUpScan(flags & IMAGE_TY ? (in_size.cy - 1 - i) : i);
			if(flags & IMAGE_TX)
			{
				d += in_size.cx;
				for(; s < e; *--d = *s++)
					;
			}
			else
				memcpy(d, s, in_size.cx);
		}
	}
	return out;
}
*/

#define STRAIGHT_COPY_1(d, s) d[0] = s[0]
#define STRAIGHT_COPY_2(d, s) d[0] = s[0], d[1] = s[1]
#define STRAIGHT_COPY_3(d, s) d[0] = s[0], d[1] = s[1], d[2] = s[2]
#define STRAIGHT_COPY_4(d, s) *(dword *)d = *(const dword *)s

#define PALETTE_COPY_2(d, s) const byte *pp = colormap + s[0]; d[0] = pp[0]; d[1] = pp[256];
#define PALETTE_COPY_3(d, s) const byte *pp = colormap + s[0]; d[0] = pp[0]; d[1] = pp[256]; d[2] = pp[512];
#define PALETTE_COPY_4(d, s) const byte *pp = colormap + s[0]; d[0] = pp[0]; d[1] = pp[256]; d[2] = pp[512]; d[3] = pp[768];

enum
{
#if !IMAGE_ORIGIN_BOTTOM
	TRANSFORM_TD_XOR = IMAGE_TX | IMAGE_TY
#else
	TRANSFORM_TD_XOR = 0
#endif//IMAGE_ORIGIN_BOTTOM
};

#define PIXEL_TRANSFORM(bypp) \
static PixelArray COMBINE(PixelTransform, bypp)(const PixelArray& src, int flags) \
{ \
	if(!src) \
		return Null; \
	if(flags & IMAGE_TD) \
		flags ^= TRANSFORM_TD_XOR; \
	Size in_size = src.GetSize(), out_size = in_size; \
	ASSERT(in_size.cx > 0 && in_size.cy > 0 && src.GetBPP() == 8 * bypp); \
	if(flags & IMAGE_TD) \
		Swap(out_size.cx, out_size.cy); \
	PixelArray out(out_size, bypp == 3 ? -3 : 8 * bypp, src.GetAlign()); \
	out.palette <<= src.palette; \
	int out_row = out.GetRowBytes(); \
	ASSERT(out_row > 0); \
	for(int i = 0; i < in_size.cy; i++) \
	{ \
		const byte *s = src.GetUpScan(i), *e = s + bypp * in_size.cx; \
		if(flags & IMAGE_TD) \
		{ \
			byte *d = out.Begin() + (bypp * (flags & IMAGE_TY ? i : (in_size.cy - 1 - i))); \
			int delta = out_row; \
			if(!(flags & IMAGE_TX)) \
			{ \
				d += out_row * (in_size.cx - 1); \
				delta = -out_row; \
			} \
			for(; s < e; s += bypp, d += delta) \
			{ COMBINE(STRAIGHT_COPY_, bypp)(d, s); } \
		} \
		else \
		{ \
			byte *d = out.GetUpScan(flags & IMAGE_TY ? (in_size.cy - 1 - i) : i); \
			if(flags & IMAGE_TX) \
			{ \
				d += bypp * (in_size.cx - 1); \
				for(; s < e; d -= bypp, s += bypp) \
				{ COMBINE(STRAIGHT_COPY_, bypp)(d, s); } \
			} \
			else \
				memcpy(d, s, bypp * in_size.cx); \
		} \
	} \
	return out; \
} \

PIXEL_TRANSFORM(1)
PIXEL_TRANSFORM(2)
PIXEL_TRANSFORM(3)
PIXEL_TRANSFORM(4)

#define PIXEL_STRAIGHT_COPY(sbypp, dbypp, palinsert, copyproc) \
static void COMBINE(COMBINE(PixelStraightCopy, sbypp), COMBINE(to, dbypp))(PixelArray& dest, const Rect& drect, \
	const PixelArray& src, const Rect& srect, const Rect& clip, BltOp blt) \
{ \
	BLT_TIMING("PixelStraightCopy" #sbypp "To" #dbypp); \
	ASSERT(dest.GetBPP() == 8 * dbypp && src.GetBPP() == 8 * sbypp); \
	Rect raw_clip = Nvl(clip, dest.GetRect()) & dest.GetRect() & drect; \
	if(raw_clip.IsEmpty()) \
		return; \
	Size ssize = src.GetSize(), dsize = dest.GetSize(); \
	Size ds = max(drect.Size(), Size(1, 1)), ss = max(srect.Size(), Size(1, 1)); \
	Size skip = ss / ds, add = ss - ds * skip; \
	int skipp = sbypp * skip.cx; \
	Point dp = drect.TopLeft(), sp = srect.TopLeft(); \
	Size counter = add >> 1, items = drect.Size(); \
	if(items.cx <= 0 || items.cy <= 0) \
		return; \
 \
	/* clip destination */ \
	if(dp.x < raw_clip.left) \
	{ \
		int ld = dp.x - raw_clip.left; \
		int d = (counter.cx -= ld * ss.cx) / ds.cx; \
		counter.cx -= d * ds.cx; \
		if((items.cx += ld) <= 0 || (sp.x += d) >= ssize.cx) \
			return; \
		dp.x = raw_clip.left; \
	} \
	if(dp.y < raw_clip.top) \
	{ \
		int td = dp.y - raw_clip.top; \
		int d = (counter.cy -= td * ss.cy) / ds.cy; \
		counter.cy -= d * ds.cy; \
		if((items.cy += td) <= 0 || (sp.y += d) >= ssize.cy) \
			return; \
		dp.y = raw_clip.top; \
	} \
	 \
	/* clip source */ \
	if(sp.x < 0) \
	{ \
		int tm = -sp.x * ds.cx; \
		int g = idivceil(tm - counter.cx, ss.cx); \
		counter.cx = (counter.cx + tm) % ss.cx; \
		sp.x = 0; \
		dp.x += g; \
		if((items.cx -= g) <= 0) \
			return; \
	} \
	if(sp.y < 0) \
	{ \
		int tm = -sp.y * ds.cy; \
		int g = idivceil(tm - counter.cy, ss.cy); \
		counter.cy = (counter.cy + tm) % ss.cy; \
		sp.y = 0; \
		dp.y += g; \
		if((items.cy -= g) <= 0) \
			return; \
	} \
 \
	/* clip source size */ \
	int full = counter.cx + items.cx * ss.cx; \
	int fc = full / ds.cx; \
	if(fc + sp.x >= ssize.cx) \
		items.cx = idivceil((ssize.cx - sp.x) * ds.cx - counter.cx, ss.cx); \
	if(dp.x + items.cx > raw_clip.right) \
		items.cx = raw_clip.right - dp.x; \
 \
	full = counter.cy + items.cy * ss.cy; \
	fc = full / ds.cy; \
	if(fc + sp.y >= ssize.cy) \
		items.cy = idivceil((ssize.cy - sp.y) * ds.cy - counter.cy, ss.cy); \
	if(dp.y + items.cy > raw_clip.bottom) \
		items.cy = raw_clip.bottom - dp.y; \
 \
	if(items.cx <= 0 || items.cy <= 0) \
		return; \
 \
	Buffer<byte> scan(dbypp * items.cx); \
	palinsert \
 \
	for(; --items.cy >= 0; dp.y++) \
	{ \
		byte *dd = scan, *de = scan + dbypp * items.cx; \
		const byte *sd = src.GetDownScan(sp.y) + sbypp * sp.x; \
		int c = counter.cx; \
		for(; dd < de; dd += dbypp) \
		{ \
			copyproc(dd, sd); \
			sd += skipp; \
			if((c += add.cx) >= ds.cx) \
			{ \
				c -= ds.cx; \
				sd += sbypp; \
			} \
		} \
		sp.y += skip.cy; \
		if((counter.cy += add.cy) >= ds.cy) \
		{ \
			counter.cy -= ds.cy; \
			sp.y++; \
		} \
		blt(dest.GetDownScan(dp.y) + dbypp * dp.x, scan, dbypp * items.cx); \
	} \
} \

PIXEL_STRAIGHT_COPY(1, 1, {}, STRAIGHT_COPY_1)
PIXEL_STRAIGHT_COPY(2, 2, {}, STRAIGHT_COPY_2)
PIXEL_STRAIGHT_COPY(3, 3, {}, STRAIGHT_COPY_3)
PIXEL_STRAIGHT_COPY(4, 4, {}, STRAIGHT_COPY_4)

PIXEL_STRAIGHT_COPY(1, 3,
	Buffer<byte> colormap(3 * 256);
	for(int i = 0; i < src.palette.GetCount(); i++)
	{
		Color c = src.palette[i];
		colormap[i +   0] = c.GetB();
		colormap[i + 256] = c.GetG();
		colormap[i + 512] = c.GetR();
	}, PALETTE_COPY_3)

void PixelCopy(PixelArray& dest, const Rect& drect, const PixelArray& src,
	const Rect& srect, const Rect& clip, BltOp blt)
{
	if(src.GetBPP() > 8)
	{
		ASSERT(dest.GetBPP() == src.GetBPP());
		switch(dest.GetBPP())
		{
		case 16: PixelStraightCopy2to2(dest, drect, src, srect, clip, blt); break;
		case 24: PixelStraightCopy3to3(dest, drect, src, srect, clip, blt); break;
		case 32: PixelStraightCopy4to4(dest, drect, src, srect, clip, blt); break;
		default: NEVER(); break;
		}
	}
	else
	{
		ASSERT(src.GetBPP() == 8);
		switch(dest.GetBPP())
		{
		case  8: PixelStraightCopy1to1(dest, drect, src, srect, clip, blt); break;
		case 24: PixelStraightCopy1to3(dest, drect, src, srect, clip, blt); break;
		default: NEVER(); break;
		}
	}
}

PixelArray PixelCrop(const PixelArray& src, const Rect& rc)
{
	Rect r = rc & src.GetRect();
	if(r.IsEmpty())
		return Null;
	PixelArray out;
	if(src.IsMono())
		out.CreateMono(r.Size());
	else
		out.Create(r.Size(), src.GetRawBPP(), src.GetAlign(), src.cmask, src.palette, 0);
	PixelCopy(out, r.Size(), src, rc);
	return out;
}

PixelArray DrawToPixelArray(Draw& draw, const Rect& rc, bool mono, int bpp, int alignment,
	const dword *cmasks, const Vector<Color>& palette)
{
	if(rc.IsEmpty())
		return PixelArray();
#ifdef PLATFORM_WIN32
	ASSERT(alignment == 4);
	HBITMAP hbmp = CreateCompatibleBitmap(draw, rc.Width(), rc.Height());
	HDC hdc = CreateCompatibleDC(draw);
	HGDIOBJ oldbmp = SelectObject(hdc, hbmp);
	BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), draw, rc.left, rc.top, SRCCOPY);
	SelectObject(hdc, oldbmp);
	PixelArray image = HBITMAPToPixelArray(hbmp, mono, bpp, hdc);
	DeleteDC(hdc);
	DeleteObject(hbmp);
	return image;
#endif
#ifdef PLATFORM_X11
	return DrawableToPixelArray(draw.GetDrawable(), rc, mono, bpp, alignment, cmasks, palette);
#endif
}

#ifdef PLATFORM_WIN32
PixelArray HBITMAPToPixelArray(HBITMAP hbmp, bool mono, int bpp, HDC hdc)
{
	PixelArray result;
	if(!hbmp)
		return result;

	PixelArray::Header header;
	Zero(header);
	header.biSize = sizeof(BITMAPINFOHEADER);
	GetDIBits(hdc, hbmp, 0, 0, 0, header, DIB_RGB_COLORS);
	result.Create(header.biWidth, header.biHeight, header.biBitCount, 4);
	result.SetHeader(header, NULL);
	GetDIBits(hdc, hbmp, 0, result.GetHeight(), result.Begin(), header, DIB_RGB_COLORS);
//	int rgbmode = DIB_RGB_COLORS;
	if(result.GetBPP() <= 8) { // retrieve palette
		if(mono) {
			result.palette.SetCount(2);
			result.palette[0] = Black;
			result.palette[1] = White;
			if(result.GetBPP() == 8 || result.GetBPP() == 4)
				BltOp1And(result.Begin(), result.GetBPP() == 8 ? 1 : 0x11, result.GetBytes());
		}
		else {
			result.palette.SetCount(1 << result.GetBPP());
//			Buffer<PALETTEENTRY> palent(result.palette.GetCount());
//			if(Draw::AutoPalette())
//				GetPaletteEntries(GetQlibPalette(), 0, result.palette.GetCount(), palent);
//			else
//				GetSystemPaletteEntries(hdc, 0, result.palette.GetCount(), palent);
//			for(int i = 0; i < result.palette.GetCount(); i++)
//				result.palette[i] = Color(palent[i].peRed, palent[i].peGreen, palent[i].peBlue);
			const RGBQUAD *q = header.palette;
			for(Color *p = result.palette.Begin(), *e = result.palette.End(); p < e; p++, q++)
				*p = Color(q->rgbRed, q->rgbGreen, q->rgbBlue);
		}
//		rgbmode = DIB_PAL_COLORS;
	}

/*
	BITMAP info;
	GetObject(hbmp, sizeof(info), &info);
	int get_bpp = bpp;
	int src_bpp = info.bmBitsPixel * info.bmPlanes;
	if(bpp == -1)
		bpp = get_bpp = (src_bpp > 8 ? -3 : 8);
	else if(bpp == 0)
		get_bpp = bpp = (src_bpp > 8 ? -3 : src_bpp > 4 ? 8 : src_bpp > 1 ? 4 : 1);
	get_bpp = (bpp >= 4 && bpp <= 8 && src_bpp > 8 ? -3 : bpp);
	result.Create(info.bmWidth, info.bmHeight, get_bpp);
	result.SetHeader(header, NULL);
*/

	if(bpp == -1)
		bpp = (header.biBitCount > 8 ? -3 : 8);
	else if(bpp == 0)
		bpp = (header.biBitCount > 8 ? -3 : header.biBitCount > 4 ? 8 : header.biBitCount > 1 ? 4 : 1);

	PixelSetConvert(result, bpp);
	return result;
}
#endif

#ifdef PLATFORM_X11
Size GetDrawableSize(Drawable drawable)
{
	Window root;
	int x, y;
	unsigned width, height, border_width, depth;
	XGetGeometry(Xdisplay, drawable, &root, &x, &y, &width, &height, &border_width, &depth);
	return Size(width, height);
}
#endif

#ifdef PLATFORM_X11
PixelArray DrawableToPixelArray(Drawable drawable, const Rect& rc, bool mono, int bpp, int alignment, const dword *cmasks, const Vector<Color>& palette)
{
	if(rc.IsEmpty())
		return Null;
	XImage *image = XGetImage(Xdisplay, drawable, rc.left, rc.top, rc.Width(), rc.Height(),
		mono ? 1u : -1u, mono ? XYPixmap : ZPixmap);
//	DUMP((int)image->red_mask);
//	DUMP((int)image->green_m ask);
//	DUMP((int)image->blue_mask);
	PixelArray out;
//	dword in_cmasks[] = { image->blue_mask, image->green_mask, image->red_mask };
	dword in_cmasks[] = { 0, 0, 0 };
	out.Create(rc.Size(), mono ? 1 : image->depth, image->bitmap_pad >> 3, in_cmasks);
	if(mono)
	{
		out.SetMono(mono);
		out.palette.SetCount(2);
		out.palette[0] = Black;
		out.palette[1] = White;
	}
	byte *dest_ptr = out.Begin();
	const byte *src_ptr = (const byte *)image->data;
	int dest_bytes = out.GetBytes();
//	memcpy(dest_ptr, image->data, dest_bytes);
	if(image->byte_order == MSBFirst && (image->bits_per_pixel == 16 || image->bits_per_pixel == 32))
	{
		if(image->bits_per_pixel == 16)
			BltSwapEndian2(dest_ptr, src_ptr, dest_bytes >> 1);
		else
			BltSwapEndian4(dest_ptr, src_ptr, dest_bytes >> 2);
		if(image->bitmap_bit_order == LSBFirst)
			BltSwapBitOrder(dest_ptr, dest_ptr, dest_bytes);
	}
	else if(mono && image->bitmap_bit_order == LSBFirst)
		BltSwapBitOrder(dest_ptr, src_ptr, dest_bytes);
	else
		memcpy(dest_ptr, src_ptr, dest_bytes);
	XDestroyImage(image);
//	LOG("Pixel dumpout: " << PixelDump(out));
	if(bpp)
	{
		if(bpp == -1)
			bpp = (out.GetBPP() <= 8 ? 8 : -3);
		PixelSetConvert(out, bpp, alignment, cmasks);
	}
	return out;
}
#endif

PixelArray PixelTransform(const PixelArray& src, int flags)
{
	if(src.IsEmpty())
		return PixelArray();
	int sb = src.GetBPP();
	if(sb < 8)
		return PixelTransform(PixelConvert(src, 8, src.GetAlign()), flags);
	switch(sb)
	{
	case  8: return PixelTransform1(src, flags);
	case 16: return PixelTransform2(src, flags);
	case 24: return PixelTransform3(src, flags);
	case 32: return PixelTransform4(src, flags);
	default: NEVER(); return Null;
	}
}

void PixelTransform(PixelArray& pixel, const Rect& rc, int flags)
{
	Rect r = rc & pixel.GetRect();
	if(r.IsEmpty())
		return;
	PixelArray tmp = PixelTransform(PixelCrop(pixel, rc), flags);
	PixelCopy(pixel, Rect(r.TopLeft(), tmp.GetSize()), tmp, tmp.GetRect());
}

PixelArray PixelMask(PixelArray& src, Color color, int tolerance, int outbpp, Color kill_mask_color)
{
	BLT_TIMING("PixelMask");
	if(src.IsEmpty())
		return PixelArray();
	ASSERT(outbpp == 1 || outbpp == 4 || outbpp == 8);
	PixelArray mask = PixelArray::Mono(src.GetSize(), outbpp);
	int r = color.GetR(), g = color.GetG(), b = color.GetB();
	Size ssize = src.GetSize();
	void (*store)(byte *dest, const byte *src, unsigned count);
	switch(outbpp)
	{
	case 1: store = Blt8to1; break;
	case 4: store = Blt8to4; break;
	case 8: store = NULL; break;
	default: NEVER(); return Null;
	}
	Buffer<byte> scanbuf(store ? ssize.cx : 0);
	if(src.GetBPP() <= 8)
	{ // simple xlat
		byte table[256];
		unsigned tt = 2 * tolerance;
		for(int i = 0; i < src.palette.GetCount(); i++)
		{
			Color c = src.palette[i];
			table[i] = ((unsigned)(c.GetR() - r + tolerance) <= tt
				&&      (unsigned)(c.GetG() - g + tolerance) <= tt
				&&      (unsigned)(c.GetB() - b + tolerance) <= tt ? 1 : 0);
			if(table[i] && !IsNull(kill_mask_color))
				src.palette[i] = kill_mask_color;
		}
		void (*decode)(byte *, const byte *, unsigned);
		switch(src.GetBPP())
		{
		case 1:  decode = Blt1to8; break;
		case 4:  decode = Blt4to8; break;
		case 8:  decode = NULL; break;
		default: NEVER(); return mask;
		}
		Buffer<byte> tsrc(decode ? ssize.cx : 0);
		for(int w = 0; w < ssize.cy; w++)
		{
			byte *orgscan = src.GetDownScan(w), *srcscan = orgscan;
			if(decode)
				decode(srcscan = tsrc, orgscan, ssize.cx);
			byte *imgscan = mask.GetDownScan(w), *scan = (store ? ~scanbuf : imgscan);
			BltXlatB(scan, srcscan, mask.GetWidth(), table);
			if(store)
				store(imgscan, scan, mask.GetWidth());
		}
	}
	else
	{ // full stepping
		byte kill[3] = { kill_mask_color.GetB(), kill_mask_color.GetG(), kill_mask_color.GetR() };
		byte rRgGbB[6];
		rRgGbB[0] = max(r - tolerance, 0);
		rRgGbB[1] = min(r + tolerance, 255);
		rRgGbB[2] = max(g - tolerance, 0);
		rRgGbB[3] = min(g + tolerance, 255);
		rRgGbB[4] = max(b - tolerance, 0);
		rRgGbB[5] = min(b + tolerance, 255);
		byte RsmGsmBsm[6];
		void (*decode)(byte *, const byte *, unsigned, const byte *);
		void (*encode)(byte *, const byte *, unsigned, const byte *);
		switch(src.GetBPP())
		{
		case 16: decode = Blt16to24; encode = Blt24to16; src.GetMaskInfo(RsmGsmBsm); break;
		case 24: decode = NULL; encode = NULL; break;
		case 32: decode = Blt32to24; encode = Blt24to16; src.GetMaskInfo(RsmGsmBsm); break;
		default: NEVER(); return mask;
		}

		Buffer<byte> tsrc(decode ? 3 * ssize.cx : 0);
		for(int r = 0; r < ssize.cy; r++)
		{
			byte *orgscan = src.GetDownScan(r), *srcscan = orgscan;
			if(decode)
				decode(srcscan = tsrc, src.GetDownScan(r), ssize.cx, RsmGsmBsm);
			byte *imgscan = mask.GetDownScan(r), *scan = (store ? ~scanbuf : imgscan);
			BltBuildMaskRgb(scan, srcscan, ssize.cx, rRgGbB);
			if(store)
				store(imgscan, scan, ssize.cx);
			if(!IsNull(kill_mask_color))
			{
				BltKillMask3(srcscan, scan, false, kill, ssize.cx);
				if(encode)
					encode(orgscan, srcscan, ssize.cx, RsmGsmBsm);
			}
		}
	}
	return mask;
}

void PixelInvertMask(PixelArray& image)
{
	switch(image.GetBPP())
	{
	case 1:  BltOp1Xor(image.Begin(), 0xFF, image.GetBytes()); break;
	case 4:  BltOp1Xor(image.Begin(), 0x11, image.GetBytes()); break;
	case 8:  BltOp1Xor(image.Begin(), 0x01, image.GetBytes()); break;
	default: NEVER();
	}
}

PixelArray PixelDiffMask(const PixelArray& a, const PixelArray& b, int out_bpp)
{
	Size sz = a.GetSize();
	ASSERT(b.GetSize() == sz);
	if(sz.cx <= 0 || sz.cy <= 0)
		return Null;
	PixelArray out = PixelArray::Mono(sz, out_bpp);
	PixelWriter8 writer(out);
	PixelReader24 rda(a), rdb(b);
	for(int i = 0; i < sz.cy; i++, writer.Write())
		BltDiffMask3(writer[i], rda[i], rdb[i], sz.cx);
	return out;
}

void PixelKillMask(PixelArray& image, const PixelArray& alpha, byte color, bool inv)
{
	if(image.IsEmpty() || alpha.IsEmpty())
		return;
	ASSERT(image.GetSize() == alpha.GetSize() && image.GetBPP() <= 8 && alpha.GetBPP() <= 8);
	Vector<byte> tscan, tmask;
	void (*store)(byte *dest, const byte *src, unsigned count);
	switch(image.GetBPP())
	{
	case 1:  store = Blt8to1; break;
	case 4:  store = Blt8to4; break;
	case 8:  store = NULL; break;
	default: NEVER(); return;
	}
	for(int i = 0; i < image.GetHeight(); i++)
	{
		byte *scan;
		if(image.GetBPP() == 8)
			scan = image.GetDownScan(i);
		else
		{
			tscan = PixelGetPixel8(image, 0, i, image.GetWidth());
			scan = tscan;
		}
		const byte *mask;
		if(alpha.GetBPP() == 8)
			mask = alpha.GetDownScan(i);
		else
		{
			tmask = PixelGetPixel8(alpha, 0, i, image.GetWidth());
			mask = tmask;
		}
		BltKillMask1(scan, mask, inv, color, image.GetWidth());
		if(store)
			store(image.GetDownScan(i), scan, image.GetWidth());
	}
}

void PixelKillMask(PixelArray& image, const PixelArray& alpha, Color color, bool inv)
{
	if(image.IsEmpty() || alpha.IsEmpty())
		return;
	ASSERT(image.GetSize() == alpha.GetSize() && image.GetBPP() > 8 && alpha.GetBPP() <= 8);
	byte rgb[3] = { color.GetB(), color.GetG(), color.GetR() };
	byte BsmGsmRsm[6];
	if(image.GetBPP() == 16 || image.GetBPP() == 32)
		image.GetMaskInfo(BsmGsmRsm);
	void (*decode)(byte *, const byte *, unsigned, const byte *);
	void (*encode)(byte *, const byte*, unsigned, const byte *);
	switch(image.GetBPP())
	{
	case 16: decode = Blt16to24; encode = Blt24to16; image.GetMaskInfo(BsmGsmRsm); break;
	case 24: decode = NULL; encode = NULL; break;
	case 32: decode = Blt32to24; encode = Blt24to32; image.GetMaskInfo(BsmGsmRsm); break;
	default: NEVER(); return;
	}
	void (*decode_mask)(byte *, const byte *, unsigned);
	switch(alpha.GetBPP())
	{
	case 1:  decode_mask = Blt1to8; break;
	case 8:  decode_mask = NULL; break;
	default: NEVER(); return;
	}

	Buffer<byte> tscan(decode ? 3 * image.GetWidth() : 0);
	Buffer<byte> tmask(decode_mask ? image.GetWidth() : 0);
	for(int i = 0; i < image.GetHeight(); i++)
	{
		byte *img_scan = image.GetDownScan(i), *scan = img_scan;
		if(decode)
			decode(scan = tscan, img_scan, image.GetWidth(), BsmGsmRsm);
		const byte *mask;
		if(decode_mask)
		{
			mask = tmask;
			decode_mask(tmask, alpha.GetDownScan(i), image.GetWidth());
		}
		else
			mask = alpha.GetDownScan(i);
		BltKillMask3(scan, mask, inv, rgb, image.GetWidth());
		if(encode)
			encode(img_scan, scan, image.GetWidth(), BsmGsmRsm);
	}
}

void PixelSetPixel(PixelArray& pa, const Vector<Point>& list, Color color)
{
	ASSERT(pa.GetBPP() == 24);
	color = Nvl(color, Black);
	byte rgb[3] = { color.GetB(), color.GetG(), color.GetR() };
	Rect rc = pa.GetRect();
	for(int i = 0; i < list.GetCount(); i++)
	{
		Point pt = list[i];
		if(rc.Contains(pt))
		{
			byte *dest = pa.GetDownScan(pt.y) + 3 * pt.x;
			dest[0] = rgb[0];
			dest[1] = rgb[1];
			dest[2] = rgb[2];
		}
	}
}

void PixelSetPixel(PixelArray& pa, const Vector<Point>& list, byte color)
{
	ASSERT(pa.GetBPP() == 8);
	Rect rc = pa.GetRect();
	for(int i = 0; i < list.GetCount(); i++)
	{
		Point pt = list[i];
		if(rc.Contains(pt))
			pa.GetDownScan(pt.y)[pt.x] = color;
	}
}

void PixelSetPixel(PixelArray& pa, int x, int y, Color color)
{
	ASSERT(pa.GetBPP() == 24);
	color = Nvl(color, Black);
	byte rgb[3] = { color.GetB(), color.GetG(), color.GetR() };
	if((unsigned)x < (unsigned)pa.GetWidth() && (unsigned)y < (unsigned)pa.GetHeight())
	{
		byte *dest = pa.GetDownScan(y) + 3 * x;
		dest[0] = rgb[0];
		dest[1] = rgb[1];
		dest[2] = rgb[2];
	}
}

void PixelSetPixel(PixelArray& pa, int x, int y, byte color)
{
	ASSERT(pa.GetBPP() == 8);
	if((unsigned)x < (unsigned)pa.GetWidth() && (unsigned)y < (unsigned)pa.GetHeight())
		pa.GetDownScan(y)[x] = color;
}

static inline Color GetColorPal(byte index, const Vector<Color>& palette)
{
	if(palette.IsEmpty())
	{
#ifdef PLATFORM_X11
		extern byte *Xunmapcolor;
		const byte *bgr = Xunmapcolor + 3 * index;
		return Color(bgr[2], bgr[1], bgr[0]);
#endif
	}
	else if(index < palette.GetCount())
		return palette[(int)index];
	return Null;
}

static inline void GetBGRPal(byte *dest, byte index, const Vector<Color>& palette)
{
	if(palette.IsEmpty())
	{
#ifdef PLATFORM_X11
		extern byte *Xunmapcolor;
		const byte *bgr = Xunmapcolor + 3 * index;
		dest[0] = bgr[0];
		dest[1] = bgr[1];
		dest[2] = bgr[2];
		return;
#endif
	}
	else if(index < palette.GetCount())
	{
		Color c = palette[(int)index];
		dest[0] = c.GetB();
		dest[1] = c.GetG();
		dest[2] = c.GetR();
		return;
	}
	dest[0] = dest[1] = dest[2] = 0;
}

static inline Color GetColorBit(dword value, const int cshift24[3], const int bitmask[3])
{
	return Color(
		((value << cshift24[2]) >> 24) & bitmask[2],
		((value << cshift24[1]) >> 24) & bitmask[1],
		((value << cshift24[0]) >> 24) & bitmask[0]);
}

static inline void GetBGRBit(byte *dest, dword value, const int cshift24[3], const int bitmask[3])
{
	dest[0] = (byte)(((value << cshift24[0]) >> 24) & bitmask[0]);
	dest[1] = (byte)(((value << cshift24[1]) >> 24) & bitmask[1]);
	dest[2] = (byte)(((value << cshift24[2]) >> 24) & bitmask[2]);
}

Vector<Color> PixelGetPixel(const PixelArray& pa, const Vector<Point>& list)
{
	Vector<Color> out;
	out.SetCount(list.GetCount(), Null);
	Rect rc = pa.GetRect();
	int bitmask[3];
	int bpp = pa.GetBPP();
	if(bpp == 16 || bpp == 32)
	{
		bitmask[0] = -256 >> pa.cbits[0];
		bitmask[1] = -256 >> pa.cbits[1];
		bitmask[2] = -256 >> pa.cbits[2];
	}
	if(bpp == 1 || bpp == 4 || bpp == 8)
	{
		for(int i = 0; i < list.GetCount(); i++)
		{
			Point pt = list[i];
			if(rc.Contains(pt))
			{
				const byte *ptr = pa.GetDownScan(pt.y);
				byte bvalue;
				switch(bpp)
				{
				case 1: bvalue = (ptr[pt.x >> 3] >> (~pt.x & 7)) & 1; break;
				case 4: bvalue = (ptr[pt.x >> 1] >> (pt.x & 1 ? 0 : 4)) & 15; break;
				case 8: bvalue = ptr[pt.x]; break;
				}
				out[i] = GetColorPal(bvalue, pa.palette);
			}
		}
	}
	else if(bpp == 24)
	{
		for(int i = 0; i < list.GetCount(); i++)
		{
			Point pt = list[i];
			if(rc.Contains(pt))
			{
				const byte *ptr = pa.GetDownScan(pt.y) + 3 * pt.x;
				out[i] = Color(ptr[2], ptr[1], ptr[0]);
			}
		}
	}
	else if(bpp == 16 || bpp == 32)
	{
		for(int i = 0; i < list.GetCount(); i++)
		{
			Point pt = list[i];
			if(rc.Contains(pt))
			{
				const byte *ptr = pa.GetDownScan(pt.y);
				dword value;
				switch(bpp)
				{
				case 16: value = *(const word *)(ptr + 2 * pt.x); break;
				case 32: value = *(const dword *)(ptr + 4 * pt.x); break;
				}
				out[i] = GetColorBit(value, pa.cshift24, bitmask);
			}
		}
	}
	else
		NEVER();
	return out;
}

Vector<byte> PixelGetPixel8(const PixelArray& pa, int x, int y, int width)
{
	ASSERT(x >= 0 && x + width <= pa.GetWidth() && y >= 0 && y < pa.GetHeight());
	Vector<byte> out;
	if(width == 0)
		return out;
	out.SetCount(width, 0);
	Rect rc = pa.GetRect();
	int bpp = pa.GetBPP();
	ASSERT(bpp <= 8);
	int bitpos = x * bpp;
	const byte *ptr = pa.GetDownScan(y) + (bitpos >> 3);
	byte *dest = out.Begin();
	int c = width;
	switch(bpp)
	{
	case 1:
		{
			if(bitpos &= 7)
			{
				byte b = *ptr++ << bitpos;
				int leftover = min(c, 8 - bitpos);
				c -= leftover;
				while(--leftover >= 0)
				{
					*dest++ = b >> 7;
					b <<= 1;
				}
			}
			Blt1to8(dest, ptr, c);
		}
		break;

	case 4:
		{
			if(bitpos & 7)
			{
				c--;
				*dest++ = *ptr++ >> 4;
			}
			Blt4to8(dest, ptr, c);
		}
		break;

	case 8:
		memcpy(dest, ptr, c);
		break;

	default:
		NEVER();
		break;
	}

	ASSERT(dest <= out + width);
	return out;
}

Vector<byte> PixelGetPixel24(const PixelArray& pa, int x, int y, int width)
{
	ASSERT(x >= 0 && x + width <= pa.GetWidth() && y >= 0 && y < pa.GetHeight());
	Vector<byte> out;
	if(width == 0)
		return out;
	out.SetCount(3 * width, 0);
	Rect rc = pa.GetRect();
	int bitmask[3];
	int bpp = pa.GetBPP();
	if(bpp == 16 || bpp == 32)
	{
		bitmask[0] = -256 >> pa.cbits[0];
		bitmask[1] = -256 >> pa.cbits[1];
		bitmask[2] = -256 >> pa.cbits[2];
	}
	int bitpos = x * bpp;
	const byte *ptr = pa.GetDownScan(y) + (bitpos >> 3);
	byte *dest = out.Begin();
	int c = width;
	switch(bpp)
	{
	case 1:
		{
			if(bitpos &= 7)
			{
				byte b = *ptr++ << bitpos;
				int leftover = min(c, 8 - bitpos);
				c -= leftover;
				while(--leftover >= 0)
				{
					GetBGRPal(dest, b >> 7, pa.palette);
					dest += 3;
					b <<= 1;
				}
			}
			while(c >= 8)
			{
				byte b = *ptr++;
				GetBGRPal(dest +  0, byte(b << 0) >> 7, pa.palette);
				GetBGRPal(dest +  3, byte(b << 1) >> 7, pa.palette);
				GetBGRPal(dest +  6, byte(b << 2) >> 7, pa.palette);
				GetBGRPal(dest +  9, byte(b << 3) >> 7, pa.palette);
				GetBGRPal(dest + 12, byte(b << 4) >> 7, pa.palette);
				GetBGRPal(dest + 15, byte(b << 5) >> 7, pa.palette);
				GetBGRPal(dest + 18, byte(b << 6) >> 7, pa.palette);
				GetBGRPal(dest + 21, byte(b << 7) >> 7, pa.palette);
				dest += 24;
				c -= 8;
			}
			if(c > 0)
				for(byte b = *ptr; --c >= 0; b <<= 1, dest += 3)
					GetBGRPal(dest, b >> 7, pa.palette);
		}
		break;

	case 4:
		{
			if(bitpos & 7)
			{
				c--;
				GetBGRPal(dest, *ptr++ >> 4, pa.palette);
				dest += 3;
			}
			while(c >= 2)
			{
				byte b = *ptr++;
				GetBGRPal(dest + 0, b >> 4, pa.palette);
				GetBGRPal(dest + 3, b & 15, pa.palette);
				dest += 6;
				c -= 2;
			}
			if(c & 1)
			{
				GetBGRPal(dest, *ptr >> 4, pa.palette);
				dest += 3;
			}
		}
		break;

	case 8:
		for(; --c >= 0; ptr += 1, dest += 3)
			GetBGRPal(dest, *ptr, pa.palette);
		break;

	case 16:
		for(; --c >= 0; ptr += 2, dest += 3)
			GetBGRBit(dest, *(const word *)ptr, pa.cshift24, bitmask);
		break;

	case 24:
		memcpy(dest, ptr, c * 3);
		break;

	case 32:
		for(; --c >= 0; ptr += 4, dest += 3)
			GetBGRBit(dest, *(const dword *)ptr, pa.cshift24, bitmask);
		break;

	default:
		NEVER();
	}

	ASSERT(dest <= out + 3*width);
	return out;
}

Vector<Color> PixelGetPixel(const PixelArray& pa, int x, int y, int width)
{
	Vector<byte> bgr = PixelGetPixel24(pa, x, y, width);
	Vector<Color> out;
	out.SetCount(bgr.GetCount());
	Color *dest = out.Begin();
	const byte *s = bgr.Begin(), *e = bgr.End();
	for(; s < e; s += 3)
		*dest++ = Color(s[2], s[1], s[0]);
	return out;
}

Color PixelGetPixel(const PixelArray& pa, int x, int y)
{
	if(x < 0 || x >= pa.GetWidth() || y < 0 || y >= pa.GetHeight())
		return Null;
	const byte *ptr = pa.GetDownScan(y);
	int bitmask[3];
	int bpp = pa.GetBPP();
	switch(bpp)
	{
	case 1:  return GetColorPal((ptr[x >> 3] >> (~x & 7)) & 1, pa.palette);
	case 4:  return GetColorPal((ptr[x >> 1] >> (x & 1 ? 0 : 4)) & 15, pa.palette);
	case 8:  return GetColorPal(ptr[x], pa.palette);
	case 24: return Color(ptr[3 * x + 2], ptr[3 * x + 1], ptr[3 * x]);
	case 16:
	case 32:
		bitmask[0] = -256 >> pa.cbits[0];
		bitmask[1] = -256 >> pa.cbits[1];
		bitmask[2] = -256 >> pa.cbits[2];
		return GetColorBit(bpp == 16 ? *(const word *)(ptr + 2 * x)
			: *(const dword *)(ptr + 4 * x), pa.cshift24, bitmask);
	default: NEVER(); return Null;
	}
}

Color PixelGetAvgColor(const PixelArray& pa, const Rect& rc)
{
	Size size = rc.Size();
	if(pa.IsEmpty() || size.cx <= 0 || size.cy <= 0)
		return Null;
	int sr = 0, sg = 0, sb = 0;
	int bypp = (pa.GetBPP() + 7) >> 3;
	if(bypp > 1)
	{
		dword counts[3 * 256];
		ZeroArray(counts);
		Buffer<byte> temp(bypp == 3 ? 0 : 3 * size.cx);
		byte BsmGsmRsm[6];
		void (*cfn)(byte *, const byte *, unsigned, const byte[]) = NULL;
		int offset = bypp * rc.left;
		if(bypp == 2 || bypp == 4)
		{
			pa.GetMaskInfo(BsmGsmRsm);
			cfn = (bypp == 2 ? &Blt16to24 : &Blt32to24);
		}
		else if(bypp != 3)
		{
			NEVER();
		}
		int i;
		for(i = rc.top; i < rc.bottom; i++)
		{
			const byte *scan = pa.GetDownScan(i);
			if(cfn)
			{
				cfn(temp, scan + offset, size.cx, BsmGsmRsm);
				scan = temp;
			}
			else
				scan += offset;
			BltStatRGB(scan, size.cx, counts);
		}
		for(i = 0; i < 256; i++)
		{
			sb += counts[0x000 + i] * i;
			sg += counts[0x100 + i] * i;
			sr += counts[0x200 + i] * i;
		}
	}
	else
	{
		dword counts[256];
		ZeroArray(counts);
		Buffer<byte> temp(size.cx + 8);
		void (*cfn)(byte *, const byte *, unsigned) = NULL;
		int soff = 0, doff = 0, lmask = 0;
		switch(pa.GetBPP())
		{
		case 1:  lmask = 7; soff = rc.left >> 3; doff = rc.left & 7; cfn = &Blt1to8; break;
		case 4:  lmask = 1; soff = rc.left >> 1; doff = rc.left & 1; cfn = &Blt4to8; break;
		case 8:  soff = rc.left; break;
		default: NEVER(); break;
		}
		int ccnt = rc.right - (rc.left & ~lmask);
		int i;
		for(i = rc.top; i < rc.bottom; i++)
		{
			const byte *scan = pa.GetDownScan(i) + soff;
			if(cfn)
			{
				cfn(temp, scan, ccnt);
				scan = temp + doff;
			}
			BltStatB(scan, size.cx, counts);
		}
		for(i = 0; i < pa.palette.GetCount(); i++)
		{
			Color pal = pa.palette[i];
			sr += pal.GetR() * counts[i];
			sg += pal.GetG() * counts[i];
			sb += pal.GetB() * counts[i];
		}
	}
	int den = size.cx * size.cy;
	return Color(sr / den, sg / den, sb / den);
}

/*
void KillMask(PixelArray& image, const PixelArray& mask)
{
	ASSERT(mask.size == image.size && mask.bpp == 8);
	void (*kill)(byte *, const byte *, unsigned);
	switch(image.bpp)
	{
	case 8:  kill = BltKillMask1; break;
	case 24: kill = BltKillMask3; break;
	default: NEVER(); return;
	}
	byte *p = image.data.Begin();
	const byte *m = mask.data.Begin();
	for(int y = image.size.cy; --y >= 0; p += image.row_bytes, m += mask.row_bytes)
		kill(p, m, image.size.cx);
}
*/

void PixelAlphaBlend(PixelArray& image, Color color, int percent, const Rect& min_area, double gamma)
{
	if(image.IsEmpty() || percent >= 100 || percent < 0)
		return;
	Rect raw = min_area & image.GetRect();
	if(raw.IsEmpty())
		return;
	enum { CACHE = 32 };
	static byte transform[CACHE][256];
	static Index<int> cache;
	static double old_gamma;
	if(cache.GetCount() > CACHE - 3 || gamma != old_gamma)
		cache.Clear();
	old_gamma = gamma;
	int rgb[3] = { color.GetR(), color.GetG(), color.GetB() };
	const byte *pc[3];
	int i;
	for(i = 0; i < 3; i++)
	{
		int k = rgb[i] | (percent << 8);
		int x = cache.Find(k);
		if(x < 0)
		{ // generate map
			x = cache.GetCount();
			cache.Add(k);
			byte *p = transform[x];
			double rgamma = 1 / gamma;
			double mt = percent / 100.0, mu = (1 - mt) * pow((double)rgb[i], gamma);
			for(int t = 0; t < 256; t++)
				*p++ = fround(minmax<double>(pow(mt * pow((double)t, gamma) + mu, rgamma), 0, 255));
		}
		pc[i] = transform[x];
	}

	if(image.GetBPP() <= 8)
	{ // convert palette
		for(int i = 0; i < image.palette.GetCount(); i++)
		{
			Color c = image.palette[i];
			image.palette[i] = Color(pc[0][c.GetR()], pc[1][c.GetG()], pc[2][c.GetB()]);
		}
	}
	else
	{
		PixelSetConvert(image, -3, image.GetAlign());
		int offset = 3 * raw.left, wd = raw.Width();
		for(i = raw.bottom; --i >= raw.top;)
			BltAverage(image.GetDownScan(i) + offset, wd, pc[0], pc[1], pc[2]);
	}
}

void PixelAlphaBlend(PixelArray& dest, const Rect& drc, const PixelArray& image, Point src_pos, int alpha)
{
	BLT_TIMING("PixelAlphaBlend(RawImage, RawImage)");
	alpha = minmax(alpha * 256 / 100, 0, 256);
	if(alpha == 256)
		return;
	if(alpha == 0)
	{
		PixelCopy(dest, drc, image, Rect(src_pos, drc.Size()));
		return;
	}
	Rect r = drc & dest.GetRect();
	src_pos = r.TopLeft() - (src_pos - drc.TopLeft());
	if(src_pos.x < 0) { r.left -= src_pos.x; src_pos.x = 0; }
	if(src_pos.y < 0) { r.top  -= src_pos.y; src_pos.y = 0; }
	if(src_pos.x + r.Width()  > image.GetWidth())  { r.right  = r.left + image.GetWidth()  - src_pos.x; }
	if(src_pos.y + r.Height() > image.GetHeight()) { r.bottom = r.top  + image.GetHeight() - src_pos.y; }
	if(r.IsEmpty())
		return;
	PixelSetConvert(dest, -3);
	const PixelArray *src = &image;
	PixelArray temp;
	if(image.GetBPP() != 24)
	{
		temp = PixelConvert(image, -3);
		src = &temp;
	}
	byte *d = dest.GetDownScan(r.top) + 3 * r.left;
	const byte *s = src->GetDownScan(src_pos.y) + 3 * src_pos.x;
	int wd = r.Width();
	for(; r.top < r.bottom; r.top++, d -= dest.row_bytes, s -= src->row_bytes)
		BltAlphaBlend24(d, s, wd, alpha);
}

AlphaArray AlphaTransform(const AlphaArray& image, int flags)
{
	AlphaArray out;
	out.pixel = PixelTransform(image.pixel, flags);
	if(image.HasAlpha())
		out.alpha = PixelTransform(image.alpha, flags);
	return out;
}

void AlphaTransform(AlphaArray& image, const Rect& rc, int flags)
{
	PixelTransform(image.pixel, rc, flags);
	if(image.HasAlpha())
		PixelTransform(image.alpha, rc, flags);
}

void AlphaKillMask(AlphaArray& image, byte color)
{
	PixelKillMask(image.pixel, image.alpha, color);
	image.alpha = PixelArray(Null);
}

void AlphaKillMask(AlphaArray& image, Color color)
{
	PixelKillMask(image.pixel, image.alpha, color);
	image.alpha = PixelArray(Null);
}

void AlphaChangeColor(AlphaArray& aa, const Rect& rc, Color src, Color dest, int tolerance)
{
	ASSERT(aa.GetPixelBPP() == 24 && aa.GetAlphaBPP() == 8);

	Rect r = rc & aa.GetRect();
	if(r.IsEmpty())
		return;
	int rw = r.Width();
	byte rRgGbB[6];
	byte get_r = src.GetR(), get_g = src.GetG(), get_b = src.GetB();
	bool src_null = IsNull(src);
	Buffer<byte> maskbuf;
	if(!src_null)
	{
		rRgGbB[0] = max(get_r - tolerance, 0); rRgGbB[1] = min(get_r + tolerance, 255);
		rRgGbB[2] = max(get_g - tolerance, 0); rRgGbB[3] = min(get_g + tolerance, 255);
		rRgGbB[4] = max(get_b - tolerance, 0); rRgGbB[5] = min(get_b + tolerance, 255);
		maskbuf.Alloc(rw);
	}
	byte kill[3] = { dest.GetB(), dest.GetG(), dest.GetR() };
	bool dest_null = IsNull(dest);
	for(int y = r.top; y < r.bottom; y++)
	{
		byte *d = aa.pixel.GetDownScan(y) + 3 * r.left;
		byte *m = aa.alpha.GetDownScan(y) + r.left, *mp = m;
		if(!src_null)
		{
			BltBuildMaskRgb(mp = maskbuf, d, rw, rRgGbB);
			BltOp2NotAnd(mp, m, rw);
		}
		BltKillMask3(d, mp, false, kill, rw);
		BltKillMask1(m, mp, false, dest_null ? 1 : 0, rw);
	}
}

void AlphaFloodFill(AlphaArray& aa, Color color, Point pt, const Rect& rc, bool diagonal)
{
	ASSERT(aa.GetPixelBPP() == 24 && aa.GetAlphaBPP() == 8);

	Rect r = rc & aa.GetRect();
	if(!r.Contains(pt))
		return;

	Color src = AlphaGetPixel(aa, pt);
	byte get[3] = { src.GetB(), src.GetG(), src.GetR() };

	{ // prepare mask
		for(int y = r.top; y < r.bottom; y++)
		{
			byte *m = aa.alpha.GetDownScan(y) + r.left, *e = m + r.Width();
			if(IsNull(src))
			{
				while(m < e)
					if(*m++ == 1)
						m[-1] = 2;
			}
			else
			{
				const byte *d = aa.pixel.GetDownScan(y) + 3 * r.left;
				for(; m < e; d += 3)
					if(*m++ == 0 && d[0] == get[0] && d[1] == get[1] && d[2] == get[2])
						m[-1] = 2;
			}
		}
	}
	Rect flood = Rect(pt, Size(1, 1));
	aa.alpha.GetDownScan(pt.y)[pt.x] = 3;

	bool done;
	do
	{
		done = false;
		int mask_scan = aa.alpha.GetDownRowBytes();
		for(int y = flood.top; y < flood.bottom; y++)
		{
			byte *b = aa.alpha.GetDownScan(y), *m = b + flood.left, *e = b + flood.right;
			for(; m < e; m++)
				if(*m == 3)
				{ // change to 3 and disperse
					*m = 4;
					done = true;
					byte *t = m;
					int i;

					for(i = y, t = m; --i >= r.top && *(t -= mask_scan) == 2; *t = 3)
						;
					if(++i < flood.top) flood.top = i;

					for(i = m - b, t = m; --i >= r.left && *--t == 2; *t = 3)
						;
					if(++i < flood.left) flood.left = i;

					for(i = y, t = m; ++i < r.bottom && *(t += mask_scan) == 2; *t = 3)
						;
					if(i > flood.bottom) flood.bottom = i;

					for(i = m - b, t = m; ++i < r.right && *++t == 2; *t = 3)
						;
					if(i > flood.right) flood.right = i;

					if(diagonal)
					{
						int x = m - b;
						if(y > r.top)
						{
							if(x > r.left && m[-mask_scan - 1] == 2)
							{
								m[-mask_scan - 1] = 3;
								if(x - 0 <= flood.left) flood.left = x - 1;
								if(y - 0 <= flood.top)  flood.top = y - 1;
							}
							if(x + 1 < r.right && m[-mask_scan + 1] == 2)
							{
								m[-mask_scan + 1] = 3;
								if(x + 1 >= flood.right) flood.right = x + 2;
								if(y - 0 <= flood.top)   flood.top = y - 1;
							}
						}
						if(y + 1 < r.bottom)
						{
							if(x > r.left && m[+mask_scan - 1] == 2)
							{
								m[+mask_scan - 1] = 3;
								if(x - 0 <= flood.left)   flood.left = x - 1;
								if(y + 1 >= flood.bottom) flood.bottom = y + 2;
							}
							if(x + 1 < r.right && m[+mask_scan + 1] == 2)
							{
								m[+mask_scan + 1] = 3;
								if(x + 1 >= flood.right)  flood.right = x + 2;
								if(y + 1 >= flood.bottom) flood.bottom = y + 2;
							}
						}
					}
				}
		}
	}
	while(done);

	byte set[3] = { GetBValue(color), GetGValue(color), GetRValue(color) };
	byte set_mask = IsNull(color) ? 1 : 0;
	for(int y = r.top; y < r.bottom; y++)
	{
		byte *d = aa.pixel.GetDownScan(y) + 3 * r.left;
		byte *m = aa.alpha.GetDownScan(y) + r.left, *e = m + r.Width();
		while(m < e)
		{
			byte *r = m;
			if(*m >= 3)
			{
				while(++m < e && *m >= 3)
					;
				if(!set_mask)
					memsetex(d, set, 3, m - r);
				memset(r, set_mask, m - r);
			}
			else if(*m == 2)
			{ // restore to previous value
				while(++m < e && *m == 2)
					;
				memset(r, IsNull(src) ? 1 : 0, m - r);
			}
			else
				while(++m < e && *m < 2)
					;
			d += 3 * (m - r);
		}
	}
}

void AlphaSetPixel(AlphaArray& aa, const Vector<Point>& list, Color color)
{
	ASSERT(aa.GetPixelBPP() == 24 && (!aa.HasAlpha() || aa.GetAlphaBPP() == 8));
	byte mask = IsNull(color) ? 1 : 0;
	color = Nvl(color, Black);
	byte rgb[3] = { color.GetB(), color.GetG(), color.GetR() };
	int i;
	Rect rc = aa.GetRect();
	for(i = 0; i < list.GetCount(); i++)
	{
		Point pt = list[i];
		if(rc.Contains(pt))
		{
			byte *dest = aa.pixel.GetDownScan(pt.y) + 3 * pt.x;
			dest[0] = rgb[0];
			dest[1] = rgb[1];
			dest[2] = rgb[2];
			if(aa.HasAlpha())
				aa.alpha.GetDownScan(pt.y)[pt.x] = mask;
		}
	}
}

void AlphaSetPixel(AlphaArray& aa, const Vector<Point>& list, byte color, byte mask)
{
	ASSERT(aa.GetPixelBPP() == 8 && (!aa.HasAlpha() || aa.GetAlphaBPP() == 8));
	int i;
	Rect rc = aa.GetRect();
	for(i = 0; i < list.GetCount(); i++)
	{
		Point pt = list[i];
		if(rc.Contains(pt))
		{
			aa.pixel.GetDownScan(pt.y)[pt.x] = color;
			if(aa.HasAlpha())
				aa.alpha.GetDownScan(pt.y)[pt.x] = mask;
		}
	}
}

void AlphaSetPixel(AlphaArray& aa, int x, int y, Color color)
{
	ASSERT(aa.GetPixelBPP() == 24 && (!aa.HasAlpha() || aa.GetAlphaBPP() == 8));
	byte mask = IsNull(color) ? 1 : 0;
	color = Nvl(color, Black);
	byte rgb[3] = { color.GetB(), color.GetG(), color.GetR() };
	if((unsigned)x < (unsigned)aa.GetWidth() && (unsigned)y < (unsigned)aa.GetHeight())
	{
		byte *dest = aa.pixel.GetDownScan(y) + 3 * x;
		dest[0] = rgb[0];
		dest[1] = rgb[1];
		dest[2] = rgb[2];
		if(aa.HasAlpha())
			aa.alpha.GetDownScan(y)[x] = mask;
	}
}

void AlphaSetPixel(AlphaArray& aa, int x, int y, byte color, byte mask)
{
	ASSERT(aa.GetPixelBPP() == 8 && (!aa.HasAlpha() || aa.GetAlphaBPP() == 8));
	if((unsigned)x < (unsigned)aa.GetWidth() && (unsigned)y < (unsigned)aa.GetHeight())
	{
		aa.pixel.GetDownScan(y)[x] = color;
		if(aa.HasAlpha())
			aa.alpha.GetDownScan(y)[x] = mask;
	}
}

Vector<Color> AlphaGetPixel(const AlphaArray& aa, const Vector<Point>& list)
{
	Vector<Color> out = PixelGetPixel(aa.pixel, list);
	if(!aa.HasAlpha() || out.IsEmpty())
		return out;
	Rect rc = aa.GetRect();
	int i;
	switch(aa.GetAlphaBPP())
	{
	case 1:
		for(i = 0; i < list.GetCount(); i++)
		{
			Point pt = list[i];
			if(rc.Contains(pt) && aa.alpha.GetDownScan(pt.y)[pt.x >> 3] & (0x80 >> (pt.x & 7)))
				out[i] = Null;
		}
		break;

	case 8:
		for(i = 0; i < list.GetCount(); i++)
		{
			Point pt = list[i];
			if(rc.Contains(pt) && aa.alpha.GetDownScan(pt.y)[pt.x])
				out[i] = Null;
		}
		break;

	default:
		NEVER();
	}
	return out;
}

Vector<Color> AlphaGetPixel(const AlphaArray& aa, int x, int y, int width)
{
	Vector<Color> out = PixelGetPixel(aa.pixel, x, y, width);
	if(!aa.HasAlpha() || out.IsEmpty())
		return out;
	const byte *scan = aa.alpha.GetDownScan(y);
	Color *d = out.Begin();
	switch(aa.GetAlphaBPP())
	{
	case 1:
		{
			scan += x >> 3;
			if(x & 7)
			{
				int leftover = min(8 - (x & 7), width);
				byte b = *scan++ << (x & 7);
				width -= leftover;
				do
				{
					if(b & 0x80)
						*d = Null;
					d++;
					b <<= 1;
				}
				while(--leftover);
			}
			while(width >= 8)
			{
				byte b = *scan++;
				if(b & 0x80) d[0] = Null;
				if(b & 0x40) d[1] = Null;
				if(b & 0x20) d[2] = Null;
				if(b & 0x10) d[3] = Null;
				if(b & 0x08) d[4] = Null;
				if(b & 0x04) d[5] = Null;
				if(b & 0x02) d[6] = Null;
				if(b & 0x01) d[7] = Null;
				d += 8;
				width -= 8;
			}
			if(width)
			{
				byte b = *scan;
				do
				{
					if(b & 0x80) *d = Null;
					d++;
					b <<= 1;
				}
				while(--width);
			}
		}
		break;

	case 8:
		while(width >= 4)
		{
			if(scan[0]) d[0] = Null;
			if(scan[1]) d[1] = Null;
			if(scan[2]) d[2] = Null;
			if(scan[3]) d[3] = Null;
			scan += 4;
			d += 4;
			width -= 4;
		}
		if(width & 2)
		{
			if(scan[0]) d[0] = Null;
			if(scan[1]) d[1] = Null;
			scan += 2;
			d += 2;
		}
		if((width & 1) && scan[0])
			d[0] = Null;
		break;

	default:
		NEVER();
	}
	return out;
}

Color AlphaGetPixel(const AlphaArray& aa, int x, int y)
{
	if(x < 0 || x >= aa.GetWidth() || y < 0 || y >= aa.GetHeight())
		return Null;
	switch(aa.GetAlphaBPP())
	{
	case 0: break;
	case 1: if(aa.alpha.GetDownScan(y)[x >> 3] & (0x80 >> (x & 7))) return Null; break;
	case 8: if(aa.alpha.GetDownScan(y)[x]) return Null; break;
	}
	return PixelGetPixel(aa.pixel, x, y);
}

void OptimizePalette(PixelArray& image)
{
	if(image.IsEmpty() || image.GetBPP() > 8)
		return;

	ASSERT(image.GetBPP() == 8);

	dword usage[256];
	ZeroArray(usage);
	int y;

	// calculate pixel usage
	Size size = image.GetSize();
	for(y = 0; y < size.cy; y++)
		BltStatB(image.GetFastScan(y), size.cx, usage);

	Vector<int> index = GetSortOrder(usage, usage + __countof(usage), StdGreater<int>());
	Vector<Color> new_palette;
	byte conversion_table[256];
	for(int i = 0; i < index.GetCount(); i++)
	{
		int x = index[i];
		if(usage[x] == 0)
			break;

		new_palette.Add(image.palette[x]);
		conversion_table[x] = i;
	}
	image.palette = new_palette;

	// convert bitmap bits
	byte *p = image.Begin();
	int row = image.GetRowBytes();
	for(y = 0; y < size.cy; y++, p += row)
		BltXlatB(p, p, size.cx, conversion_table);
}

#ifdef PLATFORM_WIN32
bool PixelArrayToClipboard(const PixelArray& array, bool clear)
{
	if(array.IsEmpty() || !OpenClipboard(0))
		return false;
	if(clear)
		EmptyClipboard();
	PixelArray::Header header;
	array.SetHeader(header, NULL);
	String out;
	int palsize = 0;
	if(array.GetBPP() <= 8)
		palsize = sizeof(RGBQUAD) * (1 << array.GetBPP());
	else if(array.GetBPP() == 16 || array.GetBPP() == 32)
		palsize = 4 * 3;
	out.Cat((const char *)&header, sizeof(BITMAPINFOHEADER) + palsize);
	out.Cat(array.Begin(), array.GetBytes());
//	WriteClipboardData(CF_DIB, out);

	HGLOBAL hmem = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, out.GetLength());
	if(!hmem)
	{
		CloseClipboard();
		return false;
	}
	memcpy(GlobalLock(hmem), out, out.GetLength());
	GlobalUnlock(hmem);
	SetClipboardData(CF_DIB, hmem);
	CloseClipboard();
	return true;
}
#endif

static String GetUnicodeScan(const char *scanner, const char **end)
{
	WString rawdata;
	while(*scanner == '\\' && scanner[1] == 'x'
	&& IsXDigit(scanner[2]) && IsXDigit(scanner[3]) && IsXDigit(scanner[4]) && IsXDigit(scanner[5]))
		rawdata.Cat(strtol(scanner + 2, (char **)&scanner, 16));
	String scandata;
	const wchar *p = rawdata.Begin(), *e = rawdata.End();
	while(p < e)
	{
		wchar ctl = *p++;
		byte skip = ctl & 0x7F, full = ctl >> 8;
		while(skip)
		{
			byte part = min<byte>(skip, 0x3F);
			scandata.Cat(0xc0 + part);
			skip -= part;
		}
		if(ctl & 0x80)
		{ // repeat segment
			if(p >= e) return scandata; // overflow
			ctl = *p++;
			byte b = (ctl << 3) & 0xF8, g = (ctl >> 2) & 0xF8, r = (ctl >> 7) & 0xF8;
			while(full)
			{
				byte part = min<byte>(full, 0x3F);
				full -= part;
				scandata.Cat(0x80 + part);
				scandata.Cat(b);
				scandata.Cat(g);
				scandata.Cat(r);
			}
		}
		else
		{ // enum segment
			if(p + full > e) // overflow
				return scandata;
			while(full)
			{
				byte part = min<byte>(full, 0x7F);
				scandata.Cat(part);
				full -= part;
				do
				{
					ctl = *p++;
					byte b = (ctl << 3) & 0xF8, g = (ctl >> 2) & 0xF8, r = (ctl >> 7) & 0xF8;
					scandata.Cat(b);
					scandata.Cat(g);
					scandata.Cat(r);
				}
				while(--part);
			}
		}
	}
	if(end)
		*end = scanner;
	return scandata;
}

void AlphaScanIML(CParser& parser, Array<IMLImageInfo>& out_images, VectorMap<String, String>& out_settings)
{
	String name, bid;
	while(!parser.IsEof())
	{
		if((bid = parser.ReadId()) == "IMAGE_META" && parser.Char('(') && !IsNull(name = parser.ReadId()))
		{
			parser.PassChar(',');
			String value = parser.ReadString();
			parser.PassChar(')');
			out_settings.Add(name, value);
		}
		else if(bid == "IMAGE_BEGIN" && parser.Char('(') && !IsNull(name = parser.ReadId()) && parser.Char(')'))
		{
			String encoded_data;
			out_settings.GetAdd("wince_16bit", "0");
			String id;
			bool first = true;
			while((id = parser.ReadId()) == "IMAGE_SCAN" && parser.Char('('))
			{
				bool first_in_row = true;
				while(parser.IsChar('\"'))
				{
					String scan = parser.ReadOneString();
					if(!first && first_in_row)
						encoded_data.Cat('\x80');
					first_in_row = first = false;
					encoded_data.Cat(scan);
				}
				if(!parser.Char(')'))
					break;
			}
			AlphaImageInfo image;
			bool accepted = false;
			if(parser.Char('(') && parser.ReadId() == name && parser.Char(','))
				if(id == "IMAGE_END"
				&& (image.size.cx = parser.ReadInt()) > 0 && parser.Char(',')
				&& (image.size.cy = parser.ReadInt()) > 0 && parser.Char(')'))
				{
					accepted = true;
				}
				else if(id == "IMAGE_PACKED" && parser.IsChar('\"'))
				{
					String d = parser.ReadOneString();
					if(parser.Char(')'))
					{
						StringStream ss(d);
						ss % image;
						if(!ss.IsError())
							accepted = true;
					}
				}

			if(name.GetLength() >= 6 && !memcmp(name, "_java_", 6))
				accepted = false;

			if(accepted)
			{
				if(name.GetLength() >= 4 && !memcmp(name, "im__", 4))
					name = Null;

				IMLImageInfo& info = out_images.Add();
				info.AlphaImageInfo::operator = (image);
				info.name = name;
				info.encoded_data = encoded_data;
			}
		}
		else if(bid == "IMAGE_BEGIN16" && parser.Char('(') && !IsNull(name = parser.ReadId()) && parser.Char(')'))
		{
			out_settings.GetAdd("wince_16bit", "1");
			String encoded_data;
			String id;
			bool first = true;
			while((id = parser.ReadId()) == "IMAGE_SCAN16" && parser.Char('(') && parser.Char('L'))
			{
				bool first_in_row = true;
				while(parser.Char('\"'))
				{
					CParser::Pos pos = parser.GetPos();
					const char *end;
					String scan = GetUnicodeScan(pos.ptr, &end);
					pos.ptr = end;
					parser.SetPos(pos);
					if(!parser.Char('\"'))
						break;
					if(!first && first_in_row)
						encoded_data.Cat('\x80');
					first_in_row = first = false;
					encoded_data.Cat(scan);
				}
				if(!parser.Char(')'))
					break;
			}
			AlphaImageInfo idata;
			bool accepted = false;
			if(id == "IMAGE_END16" && parser.Char('(') && parser.ReadId() == name && parser.Char(',')
			&& (idata.size.cx = parser.ReadInt()) > 0 && parser.Char(',')
			&& (idata.size.cy = parser.ReadInt()) > 0 && parser.Char(',')
			&& !IsNull(idata.hotspot.x = parser.ReadInt()) && parser.Char(',')
			&& !IsNull(idata.hotspot.y = parser.ReadInt()) && parser.Char(')'))
			{
				accepted = true;
			}

			if(accepted)
			{
				if(name.GetLength() >= 4 && !memcmp(name, "im__", 4))
					name = Null;

				IMLImageInfo& info = out_images.Add();
				info.AlphaImageInfo::operator = (idata);
				info.name = name;
				info.encoded_data = encoded_data;
			}
		}
		else
			break;
	}
}

AlphaArray IMLToAlphaArray(const IMLImageInfo& iml)
{
	AlphaArray out;
	out.Create(iml.size, -3);
	out.pixel.hotspot = iml.hotspot;
	RLEToAlpha(out, iml.encoded_data);
	return out;
}
