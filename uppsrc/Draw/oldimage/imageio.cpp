//////////////////////////////////////////////////////////////////////
// imageio.cpp: image input/output.

#include "Draw.h"

#if defined(PLATFORM_WIN32) && defined(QLIB2IMAGE)

#ifdef CPU_IA32
#define I386_ASSEMBLY
#endif

#ifdef I386_ASSEMBLY
#define BLT_TIMING(routine) //RTIMING(#routine " (asm)")
#else
#define BLT_TIMING(routine) //RTIMING(#routine " (cpp)")
#endif

//////////////////////////////////////////////////////////////////////

static RawImage Transform8(const RawImage& src, int flags)
{
	ASSERT(src.size.cx > 0 && src.size.cy > 0);
	Size out_size = src.size;
	if(flags & IMAGE_TD)
		Swap(out_size.cx, out_size.cy);
	RawImage out(out_size, 8);
	out.palette <<= src.palette;
	for(int i = 0; i < src.size.cy; i++)
	{
		const byte *s = src.GetUpScan(i), *e = s + src.size.cx;
		if(flags & IMAGE_TD)
		{
			byte *d = out.data.GetIter(flags & IMAGE_TY ? i : (src.size.cy - 1 - i));
			int delta = out.row_bytes;
			if(!(flags & IMAGE_TX))
			{
				d += out.row_bytes * (src.size.cx - 1);
				delta = -out.row_bytes;
			}
			for(; s < e; s++, d += delta)
				*d = *s;
		}
		else
		{
			byte *d = out.GetUpScan(flags & IMAGE_TY ? (src.size.cy - 1 - i) : i);
			if(flags & IMAGE_TX)
			{
				d += src.size.cx;
				for(; s < e; *--d = *s++)
					;
			}
			else
				memcpy(d, s, src.size.cx);
		}
	}
	return out;
}

//////////////////////////////////////////////////////////////////////

static RawImage Transform24(const RawImage& src, int flags)
{
	ASSERT(src.size.cx > 0 && src.size.cy > 0);
	Size out_size = src.size;
	if(flags & IMAGE_TD)
		Swap(out_size.cx, out_size.cy);
	RawImage out(out_size, 24);
	for(int i = 0; i < src.size.cy; i++)
	{
		const byte *s = src.GetUpScan(i), *e = s + 3 * src.size.cx;
		if(flags & IMAGE_TD)
		{
			byte *d = out.data.GetIter(3 * (flags & IMAGE_TY ? i : (src.size.cy - 1 - i)));
			int delta = out.row_bytes;
			if(!(flags & IMAGE_TX))
			{
				d += out.row_bytes * (src.size.cx - 1);
				delta = -out.row_bytes;
			}
			for(; s < e; s += 3, d += delta)
				d[0] = s[0], d[1] = s[1], d[2] = s[2];
		}
		else
		{
			byte *d = out.GetUpScan(flags & IMAGE_TY ? (src.size.cy - 1 - i) : i);
			if(flags & IMAGE_TX)
			{
				d += 3 * src.size.cx;
				for(; s < e; d -= 3, s += 3)
					d[-3] = s[0], d[-2] = s[1], d[-1] = s[2];
			}
			else
				memcpy(d, s, 3 * src.size.cx);
		}
	}
	return out;
}

//////////////////////////////////////////////////////////////////////

static void Copy24(RawImage& dest, const Rect& drect, const RawImage& src, const Rect& srect, BltRop rop)
{
	BLT_TIMING("RawImage::Copy24");
	ASSERT(dest.bpp == 24 && src.bpp == 24);
	Size ds = max(drect.Size() - 1, Size(1, 1)), ss = max(srect.Size() - 1, Size(1, 1));
	Size skip = ss / ds, add = ss - ds * skip;
	int skip3 = 3 * skip.cx;
	Point dp = drect.TopLeft(), sp = srect.TopLeft();
	Size counter = ds >> 1, items = drect.Size();
	if(items.cx <= 0 || items.cy <= 0)
		return;

	// clip destination
	if(dp.x < 0)
	{
		int d = (counter.cx -= dp.x * ss.cx) / ds.cx;
		counter.cx -= d * ds.cx;
		if((items.cx += dp.x) <= 0 || (sp.x += d) >= src.size.cx)
			return;
		dp.x = 0;
	}
	if(dp.y < 0)
	{
		int d = (counter.cy -= dp.y * ss.cy) / ds.cy;
		counter.cy -= d * ds.cy;
		if((items.cy += dp.y) <= 0 || (sp.y += d) >= src.size.cy)
			return;
		dp.y = 0;
	}

	// clip source
	if(sp.x < 0)
	{
		int tm = -sp.x * ds.cx;
		int g = idivceil(tm - counter.cx, ss.cx);
		counter.cx = (counter.cx + tm) % ss.cx;
		sp.x = 0;
		dp.x += g;
		if((items.cx -= g) <= 0)
			return;
	}
	if(sp.y < 0)
	{
		int tm = -sp.y * ds.cy;
		int g = idivceil(tm - counter.cy, ss.cy);
		counter.cy = (counter.cy + tm) % ss.cy;
		sp.y = 0;
		dp.y += g;
		if((items.cy -= g) <= 0)
			return;
	}

	// clip source size
	int full = counter.cx + items.cx * ss.cx;
	int fc = full / ds.cx;
	if(fc + sp.x >= src.size.cx)
		items.cx = idivceil((src.size.cx - sp.x) * ds.cx - counter.cx, ss.cx);
	if(dp.x + items.cx > dest.size.cx)
		items.cx = dest.size.cx - dp.x;

	full = counter.cy + items.cy * ss.cy;
	fc = full / ds.cy;
	if(fc + sp.y >= src.size.cy)
		items.cy = idivceil((src.size.cy - sp.y) * ds.cy - counter.cy, ss.cy);
	if(dp.y + items.cy > dest.size.cy)
		items.cy = dest.size.cy - dp.y;

	if(items.cx <= 0 || items.cy <= 0)
		return;

	Buffer<byte> scan(3 * items.cx);

	for(; --items.cy >= 0; dp.y++)
	{
		byte *dd = scan, *de = scan + 3 * items.cx;
		const byte *sd = src.GetDownScan(sp.y) + 3 * sp.x;
		int c = counter.cx;
		for(; dd < de; dd += 3)
		{
			dd[0] = sd[0];
			dd[1] = sd[1];
			dd[2] = sd[2];
			sd += skip3;
			if((c += add.cx) >= ds.cx)
			{
				c -= ds.cx;
				sd += 3;
			}
		}
		sp.y += skip.cy;
		if((counter.cy += add.cy) >= ds.cy)
		{
			counter.cy -= ds.cy;
			sp.y++;
		}
		rop(dest.GetDownScan(dp.y) + 3 * dp.x, scan, 3 * items.cx);
	}
}

//////////////////////////////////////////////////////////////////////

static void Copy8(RawImage& dest, const Rect& drect, const RawImage& src, const Rect& srect, BltRop rop)
{
	BLT_TIMING("RawImage::Copy8");

	ASSERT(dest.bpp == 8 && src.bpp == 8);
	Size ds = max(drect.Size() - 1, Size(1, 1)), ss = max(srect.Size() - 1, Size(1, 1));
	Size skip = ss / ds, add = ss - ds * skip;
	Point dp = drect.TopLeft(), sp = srect.TopLeft();
	Size counter = ds >> 1, items = drect.Size();
	if(items.cx <= 0 || items.cy <= 0)
		return;

	// clip destination
	if(dp.x < 0)
	{
		int d = (counter.cx -= dp.x * ss.cx) / ds.cx;
		counter.cx -= d * ds.cx;
		if((items.cx += dp.x) <= 0 || (sp.x += d) >= src.size.cx)
			return;
		dp.x = 0;
	}
	if(dp.y < 0)
	{
		int d = (counter.cy -= dp.y * ss.cy) / ds.cy;
		counter.cy -= d * ds.cy;
		if((items.cy += dp.y) <= 0 || (sp.y += d) >= src.size.cy)
			return;
		dp.y = 0;
	}

	// clip source
	if(sp.x < 0)
	{
		int tm = -sp.x * ds.cx;
		int g = idivceil(tm - counter.cx, ss.cx);
		counter.cx = (counter.cx + tm) % ss.cx;
		sp.x = 0;
		dp.x += g;
		if((items.cx -= g) <= 0)
			return;
	}
	if(sp.y < 0)
	{
		int tm = -sp.y * ds.cy;
		int g = idivceil(tm - counter.cy, ss.cy);
		counter.cy = (counter.cy + tm) % ss.cy;
		sp.y = 0;
		dp.y += g;
		if((items.cy -= g) <= 0)
			return;
	}

	// clip source size
	int full = counter.cx + items.cx * ss.cx;
	int fc = full / ds.cx;
	if(fc + sp.x >= src.size.cx)
		items.cx = idivceil((src.size.cx - sp.x) * ds.cx - counter.cx, ss.cx);
	if(dp.x + items.cx > dest.size.cx)
		items.cx = dest.size.cx - dp.x;

	full = counter.cy + items.cy * ss.cy;
	fc = full / ds.cy;
	if(fc + sp.y >= src.size.cy)
		items.cy = idivceil((src.size.cy - sp.y) * ds.cy - counter.cy, ss.cy);
	if(dp.y + items.cy > dest.size.cy)
		items.cy = dest.size.cy - dp.y;

	if(items.cx <= 0 || items.cy <= 0)
		return;

	Buffer<byte> scan(items.cx);

	for(; --items.cy >= 0; dp.y++)
	{
		byte *dd = scan, *de = dd + items.cx;
		const byte *sd = src.GetDownScan(sp.y) + sp.x;
		int c = counter.cx;
		for(; dd < de; dd++)
		{
			dd[0] = sd[0];
			sd += skip.cx;
			if((c += add.cx) >= ds.cx)
			{
				c -= ds.cx;
				sd++;
			}
		}
		sp.y += skip.cy;
		if((counter.cy += add.cy) >= ds.cy)
		{
			counter.cy -= ds.cy;
			sp.y++;
		}
		rop(dest.GetDownScan(dp.y) + dp.x, scan, items.cx);
	}
}

//////////////////////////////////////////////////////////////////////

static void Copy8to24(RawImage& dest, const Rect& drect, const RawImage& src, const Rect& srect, BltRop rop)
{
	BLT_TIMING("RawImage::Copy8to24");

	ASSERT(dest.bpp == 24 && src.bpp == 8);
	Size ds = max(drect.Size() - 1, Size(1, 1)), ss = max(srect.Size() - 1, Size(1, 1));
	Size skip = ss / ds, add = ss - ds * skip;
	Point dp = drect.TopLeft(), sp = srect.TopLeft();
	Size counter = ds >> 1, items = drect.Size();
	if(items.cx <= 0 || items.cy <= 0)
		return;

	// clip destination
	if(dp.x < 0)
	{
		int d = (counter.cx -= dp.x * ss.cx) / ds.cx;
		counter.cx -= d * ds.cx;
		if((items.cx += dp.x) <= 0 || (sp.x += d) >= src.size.cx)
			return;
		dp.x = 0;
	}
	if(dp.y < 0)
	{
		int d = (counter.cy -= dp.y * ss.cy) / ds.cy;
		counter.cy -= d * ds.cy;
		if((items.cy += dp.y) <= 0 || (sp.y += d) >= src.size.cy)
			return;
		dp.y = 0;
	}

	// clip source
	if(sp.x < 0)
	{
		int tm = -sp.x * ds.cx;
		int g = idivceil(tm - counter.cx, ss.cx);
		counter.cx = (counter.cx + tm) % ss.cx;
		sp.x = 0;
		dp.x += g;
		if((items.cx -= g) <= 0)
			return;
	}
	if(sp.y < 0)
	{
		int tm = -sp.y * ds.cy;
		int g = idivceil(tm - counter.cy, ss.cy);
		counter.cy = (counter.cy + tm) % ss.cy;
		sp.y = 0;
		dp.y += g;
		if((items.cy -= g) <= 0)
			return;
	}

	// clip source size
	int full = counter.cx + items.cx * ss.cx;
	int fc = full / ds.cx;
	if(fc + sp.x >= src.size.cx)
		items.cx = idivceil((src.size.cx - sp.x) * ds.cx - counter.cx, ss.cx);
	if(dp.x + items.cx > dest.size.cx)
		items.cx = dest.size.cx - dp.x;

	full = counter.cy + items.cy * ss.cy;
	fc = full / ds.cy;
	if(fc + sp.y >= src.size.cy)
		items.cy = idivceil((src.size.cy - sp.y) * ds.cy - counter.cy, ss.cy);
	if(dp.y + items.cy > dest.size.cy)
		items.cy = dest.size.cy - dp.y;

	if(items.cx <= 0 || items.cy <= 0)
		return;

	Buffer<byte> scan(3 * items.cx);
	Buffer<byte> colormap(3 * 256);
	for(int i = 0; i < src.palette.GetCount(); i++)
	{
		Color c = src.palette[i];
		colormap[i + 0] = c.GetB();
		colormap[i + 256] = c.GetG();
		colormap[i + 512] = c.GetR();
	}

	for(; --items.cy >= 0; dp.y++)
	{
		byte *dd = scan, *de = scan + 3 * items.cx;
		const byte *sd = src.GetDownScan(sp.y) + sp.x;
		int c = counter.cx;
		for(; dd < de; dd += 3)
		{
			const byte *s = colormap + sd[0];
			dd[0] = s[0];
			dd[1] = s[256];
			dd[2] = s[512];
			sd += skip.cx;
			if((c += add.cx) >= ds.cx)
			{
				c -= ds.cx;
				sd++;
			}
		}
		sp.y += skip.cy;
		if((counter.cy += add.cy) >= ds.cy)
		{
			counter.cy -= ds.cy;
			sp.y++;
		}
		rop(dest.GetDownScan(dp.y) + 3 * dp.x, scan, 3 * items.cx);
	}
}

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

static void Convert1to4(const RawImage& src, byte *dest)
{
	ASSERT(!src.IsEmpty() && src.bpp == 1);
	BltConvert(dest, src.data.Begin(), src.GetRowBytes(4), src.row_bytes, src.size.cx, src.size.cy, Blt1to4);
}

//////////////////////////////////////////////////////////////////////

static void Convert1to8(const RawImage& src, byte *dest)
{
	ASSERT(!src.IsEmpty() && src.bpp == 1);
	BltConvert(dest, src.data.Begin(), src.GetRowBytes(8), src.row_bytes, src.size.cx, src.size.cy, Blt1to8);
}

//////////////////////////////////////////////////////////////////////

static void Convert2to4(const RawImage& src, byte *dest)
{
	ASSERT(!src.IsEmpty() && src.bpp == 2);
	BltConvert(dest, src.data.Begin(), src.GetRowBytes(4), src.row_bytes, src.size.cx, src.size.cy, Blt2to4);
}

//////////////////////////////////////////////////////////////////////

static void Convert4to1(const RawImage& src, byte *dest)
{
	ASSERT(!src.IsEmpty() && src.bpp == 4);
	BltConvert(dest, src.data.Begin(), src.GetRowBytes(1), src.row_bytes, src.size.cx, src.size.cy, Blt4to1);
}

//////////////////////////////////////////////////////////////////////

static void Convert4to8(const RawImage& src, byte *dest)
{
	ASSERT(!src.IsEmpty() && src.bpp == 4);
	BltConvert(dest, src.data.Begin(), src.GetRowBytes(8), src.row_bytes, src.size.cx, src.size.cy, Blt4to8);
}

//////////////////////////////////////////////////////////////////////

static void Convert8to1(const RawImage& src, byte *dest)
{
	ASSERT(!src.IsEmpty() && src.bpp == 8);
	BltConvert(dest, src.data.Begin(), src.GetRowBytes(1), src.row_bytes, src.size.cx, src.size.cy, Blt8to1);
}

//////////////////////////////////////////////////////////////////////

static void Convert8to4(const RawImage& src, byte *dest)
{
	ASSERT(!src.IsEmpty() && src.bpp == 8);
	BltConvert(dest, src.data.Begin(), src.GetRowBytes(4), src.row_bytes, src.size.cx, src.size.cy, Blt8to4);
}

//////////////////////////////////////////////////////////////////////

static void Convert8to24(const RawImage& simg, byte *dest, const Vector<Color>& pal)
{
	ASSERT(!simg.IsEmpty() && simg.bpp == 8);
	const byte *src = simg.GetUpScan(simg.size.cy);
	int out_bytes = simg.GetRowBytes(24);
	dest += out_bytes * simg.size.cy;
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
	for(int i = simg.size.cy; --i >= 0;)
	{
		const byte *s = (src -= simg.row_bytes) + simg.size.cx;
		byte *d = (dest -= out_bytes) + 3 * simg.size.cx;
		int n = simg.size.cx;
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

//////////////////////////////////////////////////////////////////////

RawImage::RawImage()
: size(0, 0)
, bpp(0)
, row_bytes(0)
{
}

//////////////////////////////////////////////////////////////////////

RawImage::RawImage(const RawImage& pm, int)
: data(pm.data, 0)
, palette(pm.palette, 0)
, bpp(pm.bpp)
, size(pm.size)
, row_bytes(pm.row_bytes)
{
}

//////////////////////////////////////////////////////////////////////

RawImage RawImage::Mono(int cx, int cy)
{
	RawImage result;
	result.Create(cx, cy, 8);
	result.palette.SetCount(2);
	result.palette[0] = Black;
	result.palette[1] = White;
	return result;
}

//////////////////////////////////////////////////////////////////////

void RawImage::Clear()
{
	data.Clear();
	palette.Clear();
	bpp = 0;
	size.cx = size.cy = 0;
	row_bytes = 0;
}

//////////////////////////////////////////////////////////////////////

void RawImage::Create(int cx, int cy, int _bpp)
{
	Clear();
	size = Size(cx, cy);
	bpp = _bpp;
	row_bytes = ((size.cx * bpp + 31) >> 3) & -4;
	if(!IsEmpty())
		data.SetCount(row_bytes * cy);
}

//////////////////////////////////////////////////////////////////////

bool RawImage::SetHeader(Header& header) const
{
	if(IsEmpty())
		return false;
	Zero(header);
	header.biSize = sizeof(BITMAPINFOHEADER);
	header.biWidth = size.cx;
	header.biHeight = size.cy;
	header.biBitCount = bpp;
	header.biPlanes = 1;
	header.biCompression = BI_RGB;
//	header.biClrUsed = bpp <= 8 ? max(2, palette.GetCount()) : 0;
	const Color *c = palette.Begin();
	for(RGBQUAD *p = header.palette, *e = p + (bpp <= 8 ? palette.GetCount() : 0); p < e; p++, c++)
	{
		COLORREF cr = *c;
		p -> rgbRed   = GetRValue(cr);
		p -> rgbGreen = GetGValue(cr);
		p -> rgbBlue  = GetBValue(cr);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////

static int GetMaxBitmapSize()
{
	static int mbs = 0;
	if(mbs < 0)
	{
#ifdef PLATFORM_WIN32
		OSVERSIONINFO osvi;
		osvi.dwOSVersionInfoSize = sizeof(osvi);
		VERIFY(GetVersionEx(&osvi));
		mbs = (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT ? 0 : 1 << 24); // 16M in W95, no limit in NT
#else
#error Needs to be reimplemented for other platforms
#endif
	}
	return mbs;
}

//////////////////////////////////////////////////////////////////////

void RawImage::Paint(HDC hdc, const Rect& src, const Rect& dest, int rop) const
{
	Header header;
	if(!SetHeader(header) || src.left == src.right || src.top == src.bottom
		|| dest.left == dest.right || dest.top == dest.bottom)
		return;
	int mode = SetStretchBltMode(hdc, COLORONCOLOR);
	enum { SPLIT = 1024 };
	if(src.Width() <= SPLIT && src.Height() <= SPLIT && dest.Width() <= SPLIT && dest.Height() <= SPLIT)
		StretchDIBits(hdc, dest.left, dest.top, dest.Width(), dest.Height(),
			src.left, size.cy - src.bottom, src.Width(), src.Height(), 
			data.Begin(), header, DIB_RGB_COLORS, rop);
	else
	{
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
			StretchDIBits(hdc, d0.left, d0.top, d0.Width(), d0.Height(),
				s0.left, 0, s0.Width(), s0.Height(), GetDownScan(s0.bottom - 1),
				header, DIB_RGB_COLORS, rop);
		}
	}
	SetStretchBltMode(hdc, mode);
}

//////////////////////////////////////////////////////////////////////

RawImage ImageToRawImage(const Image& im, int bpp, bool convert, Draw& draw)
{
	return HBITMAPToRawImage(im.GetHBITMAP(draw), bpp, convert, draw);
}

//////////////////////////////////////////////////////////////////////

RawImage MaskToRawImage(const Image& im, bool onebit)
{
	return HBITMAPToRawImage(im.GetMaskHBITMAP(), onebit ? 1 : 8, false);
}

//////////////////////////////////////////////////////////////////////

RawImage DrawToRawImage(Draw& draw, const Rect& rc, int bpp, bool convert)
{
	if(rc.IsEmpty())
		return RawImage();
	HBITMAP hbmp = CreateCompatibleBitmap(draw, rc.Width(), rc.Height());
	HDC hdc = CreateCompatibleDC(draw);
	HGDIOBJ oldbmp = SelectObject(hdc, hbmp);
	BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), draw, rc.left, rc.top, SRCCOPY);
	SelectObject(hdc, oldbmp);
	RawImage image = HBITMAPToRawImage(hbmp, bpp, convert, hdc);
	DeleteDC(hdc);
	DeleteObject(hbmp);
	return image;
}

//////////////////////////////////////////////////////////////////////

RawImage HBITMAPToRawImage(HBITMAP hbmp, int bpp, bool convert, HDC hdc)
{
	RawImage result;
	if(!hbmp)
		return result;
	BITMAP info;
	GetObject(hbmp, sizeof(info), &info);
	int get_bpp = bpp;
	int src_bpp = info.bmBitsPixel * info.bmPlanes;
	if(bpp == 0)
		bpp = get_bpp = (src_bpp > 8 ? 24 : 8);
	if(bpp < 0 || convert)
	{
		if(bpp <= 0)
			get_bpp = bpp = (src_bpp > 8 ? 24 : src_bpp > 4 ? 8 : src_bpp > 1 ? 4 : 1);
		if(bpp >= 4 && bpp <= 8 && src_bpp > 8)
			get_bpp = 24;
	}
	result.Create(info.bmWidth, info.bmHeight, get_bpp);
	RawImage::Header header;
	result.SetHeader(header);
	GetDIBits(hdc, hbmp, 0, result.size.cy, result.data.Begin(), header, DIB_RGB_COLORS);
	if(get_bpp <= 8)
	{ // retrieve palette
		result.palette.SetCount(1 << get_bpp);
		const RGBQUAD *q = header.palette;
		for(Color *p = result.palette.Begin(), *e = result.palette.End(); p < e; p++, q++)
			*p = Color(q -> rgbRed, q -> rgbGreen, q -> rgbBlue);
	}
	if(bpp != get_bpp)
		result.Convert(bpp);
	return result;
}

//////////////////////////////////////////////////////////////////////

Image RawImageToImage(const RawImage& src, Draw& draw)
{
	if(src.IsEmpty())
		return Image();
	Image image(src.size);
	if(!draw.IsDrawing() && GetDeviceCaps(draw, BITSPIXEL) > 8 && src.bpp <= 8)
		GetConvert(src, 24).Paint(ImageDraw(draw, image));
	else
		src.Paint(ImageDraw(draw, image));
	return image;
}

//////////////////////////////////////////////////////////////////////

void Transform(RawImage& image, int flags)
{
	if(!image.IsEmpty() && flags)
		image = GetTransform(image, flags);
}

//////////////////////////////////////////////////////////////////////

RawImage GetTransform(const RawImage& src, int flags)
{
	if(src.IsEmpty())
		return RawImage();
	if(src.bpp == 8)
		return Transform8(src, flags);
	else if(src.bpp == 24)
		return Transform24(src, flags);
	else
	{
		NEVER(); // invalid bpp
		return RawImage();
	}
}

//////////////////////////////////////////////////////////////////////

RawImage GetConvert(const RawImage& src, int to_bpp)
{
	ASSERT(to_bpp == 1 || to_bpp == 4 || to_bpp == 8 || to_bpp == 24);
	if(src.IsEmpty())
		return RawImage();
	if(to_bpp == src.bpp || src.bpp > 8 && to_bpp > 8)
		return RawImage(src, 0);
	if(to_bpp > 8)
	{
		ASSERT(src.bpp <= 8);
		RawImage tmp(src.size, 24);
		switch(src.bpp)
		{
		case 1:
			Convert1to8(src, tmp.data.Begin());
			tmp.row_bytes = tmp.GetRowBytes(tmp.bpp = 8);
			Convert8to24(tmp, tmp.data.Begin(), src.palette);
			tmp.row_bytes = tmp.GetRowBytes(tmp.bpp = 24);
			break;

		case 4:
			Convert4to8(src, tmp.data.Begin());
			tmp.row_bytes = tmp.GetRowBytes(tmp.bpp = 8);
			Convert8to24(tmp, tmp.data.Begin(), src.palette);
			tmp.row_bytes = tmp.GetRowBytes(tmp.bpp = 24);
			break;

		case 8:
			Convert8to24(src, tmp.data.Begin(), src.palette);
			break;

		default:
			NEVER();
		}
		return tmp;
	}
	else
	{
		RawImage result(src.size, to_bpp);
		if(src.bpp > 8 || src.palette.GetCount() > (1 << to_bpp)) // map palette
			CreatePalette(result, src, to_bpp, 1 << to_bpp);
		else if(src.bpp == 8)
		{
			switch(to_bpp)
			{
			case 1:  Convert8to1(src, result.data.Begin()); break;
			case 4:  Convert8to4(src, result.data.Begin()); break;
			case 8:  break;
			default: NEVER(); break;
			}
		}
		else if(src.bpp == 4)
		{
			switch(to_bpp)
			{
			case 1:  Convert4to1(src, result.data.Begin()); break;
			case 4:  break;
			case 8:  Convert4to8(src, result.data.Begin()); break;
			default: NEVER(); break;
			}
		}
		else if(src.bpp == 1)
		{
			switch(to_bpp)
			{
			case 1:  break;
			case 4:  Convert1to4(src, result.data.Begin()); break;
			case 8:  Convert1to8(src, result.data.Begin()); break;
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

//////////////////////////////////////////////////////////////////////

void RawImage::Convert(int to_bpp)
{
	ASSERT(to_bpp == 1 || to_bpp == 4 || to_bpp == 8 || to_bpp == 24);
	if(IsEmpty())
		return;
	if(to_bpp == bpp || bpp > 8 && to_bpp > 8)
		return;
	if(to_bpp > 8)
	{ // expand palette
		*this = GetConvert(*this, 24);
	}
	else if(bpp > 8 || palette.GetCount() > (1 << to_bpp)) // map palette
		CreatePalette(*this, *this, to_bpp, 1 << to_bpp);
	else if(bpp == 8)
	{
		switch(to_bpp)
		{
		case 1: Convert8to1(*this, data.Begin()); break;
		case 4: Convert8to4(*this, data.Begin()); break;
		case 8: break;
		default: NEVER(); break;
		}
	}
	else if(bpp == 4)
	{
		switch(to_bpp)
		{
		case 1:   Convert4to1(*this, data.Begin()); break;
		case 8:
			{
				Vector<byte> aux;
				aux.SetCount(size.cy * GetRowBytes(8, size.cx));
				Convert4to8(*this, aux.Begin());
				data = aux;
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
			int row = GetRowBytes(to_bpp, size.cx);
			aux.SetCount(size.cy * row);
			if(to_bpp == 4)
				Convert2to4(*this, aux.Begin());
			else
			{
				int row4 = GetRowBytes(4, size.cx);
				byte *end = aux.End() - row4 * size.cy;
				byte *out = aux.Begin();
				Convert2to4(*this, end);
				for(int i = 0; i < size.cy; i++, end += row4, out += row)
					Blt4to8(out, end, size.cx);
			}
			data = aux;
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
			aux.SetCount(size.cy * GetRowBytes(4, size.cx));
			Convert1to4(*this, aux.Begin());
			data = aux;
			break;
			
		case 8:
			aux.SetCount(size.cy * GetRowBytes(8, size.cx));
			Convert1to8(*this, aux.Begin());
			data = aux;
			break;
			
		default:
			NEVER();
			break;
		}
	}
	else
		NEVER();
	bpp = to_bpp;
	row_bytes = GetRowBytes(bpp);
	if(bpp > 8)
		palette.Clear();
	else if(palette.GetCount() > 1 << bpp)
		palette.SetCount(1 << bpp);
	data.SetCount(row_bytes * size.cy);
	data.Shrink();
}

//////////////////////////////////////////////////////////////////////

void RawImage::OptimizePalette()
{
	if(IsEmpty() || bpp > 8)
		return;

	ASSERT(bpp == 8);

	dword usage[256];
	ZeroArray(usage);
	int y;
	
	// calculate pixel usage
	for(y = 0; y < size.cy; y++)
		BltStatB(GetUpScan(y), size.cx, usage);

	Vector<int> index = GetSortOrder<int>(usage, usage + __countof(usage), StdGreater<int>());
	Vector<Color> new_palette;
	byte conversion_table[256];
	for(int i = 0; i < index.GetCount(); i++)
	{
		int x = index[i];
		if(usage[x] == 0)
			break;

		new_palette.Add(palette[x]);
		conversion_table[x] = i;
	}
	palette = new_palette;

	// convert bitmap bits
	byte *p = data.Begin();
	for(y = 0; y < size.cy; y++, p += row_bytes)
		BltXlatB(p, p, size.cx, conversion_table);
}

//////////////////////////////////////////////////////////////////////

void AlphaBlend(RawImage& image, Color color, int percent, const Rect& min_area, double gamma)
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
			double mt = percent / 100.0, mu = (1 - mt) * pow(rgb[i], gamma);
			for(int t = 0; t < 256; t++)
				*p++ = fround(minmax<double>(pow(mt * pow(t, gamma) + mu, rgamma), 0, 255));
		}
		pc[i] = transform[x];
	}

	if(image.bpp <= 8)
	{ // convert palette
		for(int i = 0; i < image.palette.GetCount(); i++)
		{
			Color c = image.palette[i];
			image.palette[i] = Color(pc[0][c.GetR()], pc[1][c.GetG()], pc[2][c.GetB()]);
		}
	}
	else
	{
		ASSERT(image.bpp == 24);
		int offset = 3 * raw.left, wd = raw.Width();
		int most = image.size.cy - 1;
		for(i = raw.bottom; --i >= raw.top;)
			BltAverage(image.GetUpScan(most - i) + offset, wd, pc[0], pc[1], pc[2]);
	}
}

//////////////////////////////////////////////////////////////////////

Point RawImage::ClipMove(Rect& dest, Point src, Size src_size) const
{
	Point pos = dest.TopLeft();
	dest &= GetRect();
	src += dest.TopLeft() - pos;
	if(src.x < 0)
	{
		dest.left -= src.x;
		src.x = 0;
	}
	if(src.y < 0)
	{
		dest.top -= src.y;
		src.y = 0;
	}
	if(src.x + dest.Width() > src_size.cx)
		dest.right = dest.left + src_size.cx - src.x;
	if(src.y + dest.Height() > src_size.cy)
		dest.bottom = dest.top + src_size.cy - src.y;
	return src;
}

//////////////////////////////////////////////////////////////////////

void RawImage::Set(const Rect& dest, byte value)
{
	BLT_TIMING("RawImage::Set(byte)");
	if(bpp > 8)
	{
		RawImage::Set(dest, Color(value, value, value));
		return;
	}
	ASSERT(bpp == 8); // ? todo
	Rect r = dest & GetRect();
	if(r.IsEmpty())
		return;
	byte *d = GetDownScan(r.top) + r.left; // 8 bit only
	int wd = r.Width();
	for(int i = r.Height(); --i >= 0; d -= GetRowBytes())
		memset(d, value, wd);
}

//////////////////////////////////////////////////////////////////////

void RawImage::Set(const Rect& dest, Color color)
{
	BLT_TIMING("RawImage::Set(Color)");
	Rect r = dest & GetRect();
	if(r.IsEmpty())
		return;
	ASSERT(bpp == 24);
	byte temp[12];
	temp[0] = temp[3] = temp[6] = temp[9] = color.GetB();
	temp[1] = temp[4] = temp[7] = temp[10] = color.GetG();
	temp[2] = temp[5] = temp[8] = temp[11] = color.GetR();
	byte *d = GetDownScan(r.top) + 3 * r.left, *rw = d;
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
	d -= GetRowBytes();
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
		memcpy(rw -= row_bytes, from, wd);
	}
#endif
}

//////////////////////////////////////////////////////////////////////

RawImage GetCrop(const RawImage& src, const Rect& rc)
{
	BLT_TIMING("GetCrop(RawImage)");
	Rect r = rc & src.GetRect();
	RawImage result(r.Size(), src.bpp);
	result.palette <<= src.palette;
	Copy(result, result.GetRect(), src, r.TopLeft());
	return result;
}

//////////////////////////////////////////////////////////////////////

void Copy(RawImage& dest, const Rect& drect, const RawImage& src, Point src_pos, BltRop rop)
{
	BLT_TIMING("Copy(RawImage) (simple)");
	if(dest.bpp != src.bpp)
	{
		Copy(dest, drect, src, Rect(src_pos, drect.Size()), rop);
		return;
	}
	Rect r = drect;
	src_pos = dest.ClipMove(r, src_pos, src.GetSize());
	if(r.IsEmpty())
		return;
	ASSERT(dest.bpp == src.bpp && dest.bpp >= 8); // todo ?
	int bypp = dest.bpp >> 3;
	byte *d = dest.GetDownScan(r.top) + bypp * r.left;
	const byte *s = src.GetDownScan(src_pos.y) + bypp * src_pos.x;
	int count = r.Width() * bypp;
	for(; r.top < r.bottom; r.top++, d -= dest.row_bytes, s -= src.row_bytes)
		rop(d, s, count);
}

//////////////////////////////////////////////////////////////////////

void Copy(RawImage& dest, const Rect& drect, const RawImage& src, const Rect& src_rect, BltRop rop)
{
	if(drect.IsEmpty() || src_rect.IsEmpty())
		return;
	if(drect.Size() == src_rect.Size() && dest.bpp == src.bpp)
	{
		Copy(dest, drect, src, src_rect.TopLeft());
		return;
	}
	if(dest.bpp == 24 && src.bpp == 24)
		Copy24(dest, drect, src, src_rect, rop);
	else if(dest.bpp == 8 && src.bpp == 8)
		Copy8(dest, drect, src, src_rect, rop);
	else if(dest.bpp == 24 && src.bpp == 8)
		Copy8to24(dest, drect, src, src_rect, rop);
	else
		NEVER();
}

//////////////////////////////////////////////////////////////////////

void AlphaBlend(RawImage& dest, const Rect& drc, const RawImage& image, Point src_pos, int alpha)
{
	BLT_TIMING("AlphaBlend(RawImage, RawImage)");
	alpha = minmax(alpha * 256 / 100, 0, 256);
	if(alpha == 256)
		return;
	if(alpha == 0)
	{
		Copy(dest, drc, image, src_pos);
		return;
	}
	Rect r = drc;
	src_pos = dest.ClipMove(r, src_pos, image.GetSize());
	if(r.IsEmpty())
		return;
	dest.Convert(24);
	const RawImage *src = &image;
	RawImage temp;
	if(image.bpp != 24)
	{
		temp = GetConvert(image, 24);
		src = &temp;
	}
	byte *d = dest.GetDownScan(r.top) + 3 * r.left;
	const byte *s = src -> GetDownScan(src_pos.y) + 3 * src_pos.x;
	int wd = r.Width();
	for(; r.top < r.bottom; r.top++, d -= dest.row_bytes, s -= src -> row_bytes)
		BltAlphaBlend24(d, s, wd, alpha);
}

//////////////////////////////////////////////////////////////////////

RawImage GetMask(const RawImage& src, Color color, int tolerance)
{
	BLT_TIMING("RawImage::Mask");
	if(src.IsEmpty())
		return RawImage();
	if(src.bpp != 8 && src.bpp != 24)
		return GetMask(GetConvert(src, src.bpp <= 8 ? 8 : 24), color, tolerance);
	RawImage mask = RawImage::Mono(src.GetSize());
	int r = color.GetR(), g = color.GetG(), b = color.GetB();
	if(src.bpp == 8)
	{ // simple xlat
		byte table[256];
		unsigned tt = 2 * tolerance;
		for(int i = 0; i < src.palette.GetCount(); i++)
		{
			Color c = src.palette[i];
			table[i] = ((unsigned)(c.GetR() - r + tolerance) <= tt
				&&      (unsigned)(c.GetG() - g + tolerance) <= tt
				&&      (unsigned)(c.GetB() - b + tolerance) <= tt ? 1 : 0);
		}
		for(int r = 0; r < src.size.cy; r++)
			BltXlatB(mask.GetUpScan(r), src.GetUpScan(r), src.size.cx, table);
	}
	else
	{ // full stepping
		byte rRgGbB[6];
		rRgGbB[0] = max(r - tolerance, 0);
		rRgGbB[1] = min(r + tolerance, 255);
		rRgGbB[2] = max(g - tolerance, 0);
		rRgGbB[3] = min(g + tolerance, 255);
		rRgGbB[4] = max(b - tolerance, 0);
		rRgGbB[5] = min(b + tolerance, 255);
		for(int r = 0; r < src.size.cy; r++)
			BltBuildMaskRgb(mask.GetUpScan(r), src.GetUpScan(r), src.size.cx, rRgGbB);
	}
	return mask;
}

//////////////////////////////////////////////////////////////////////

void InvertMask(RawImage& image)
{
	ASSERT(image.GetBPP() == 1);
	int row = image.GetRowBytes();
	for(int i = 0, n = image.GetSize().cy; i < n; i++)
		BltInvert(image.GetUpScan(i), row);
}

//////////////////////////////////////////////////////////////////////

void KillMask(RawImage& image, const RawImage& mask)
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

//////////////////////////////////////////////////////////////////////

Image Transform(const Image& src, int flags, Draw& draw)
{
	BLT_TIMING("Image::Transform");
	if(src.IsEmpty())
		return src;
	Image image(src);
	Point hotspot = src.GetHotSpot();
	if(flags & IMAGE_TX) hotspot.x = src.GetSize().cx - 1 - hotspot.x;
	if(flags & IMAGE_TY) hotspot.y = src.GetSize().cy - 1 - hotspot.y;
	if(flags & IMAGE_TD) Swap(hotspot.x, hotspot.y);
	image.SetHotSpot(hotspot);
	Size size = image.GetSize();
	Size out_size = (flags & IMAGE_TD ? Size(size.cy, size.cx) : size);
	Image out_image(draw, out_size);
	HBITMAP sbmp = image.GetHBITMAP(draw);
	BITMAP info;
	GetObject(sbmp, sizeof(info), &info);
	int bpp = info.bmPlanes * info.bmBitsPixel;
	GetTransform(HBITMAPToRawImage(sbmp, bpp > 8 ? 24 : 8), flags).Paint(ImageDraw(draw, out_image), Point(0, 0));
	if(image.HasMask())
		GetTransform(MaskToRawImage(image), flags).Paint(ImageMaskDraw(out_image), Point(0, 0));
	return out_image;
}

//////////////////////////////////////////////////////////////////////

void AlphaBlend(Image& dest, Color color, int percent, const Rect& at_least_this_part, int gamma)
{
	BLT_TIMING("AlphaBlend (Image, color)");
	RawImage image = ImageToRawImage(dest);
	if(!image)
		return;
	::AlphaBlend(image, color, percent, at_least_this_part, gamma);
//	__asm int 3
	image.Paint(ImageDraw(dest), Point(0, 0));
}

//////////////////////////////////////////////////////////////////////
// ImageInfo::

bool ImageInfo::FromClipboard()
{
	if(!OpenClipboard(0))
		return false;
	HGLOBAL hmem = (HGLOBAL)GetClipboardData(CF_DIB);
	if(!hmem)
	{
		CloseClipboard();
		return false;
	}
	;
	*this = BmpEncoder().Info(MemReadStream(GlobalLock(hmem), GlobalSize(hmem)));
	GlobalUnlock(hmem);
	CloseClipboard();
	return true;
}

//////////////////////////////////////////////////////////////////////
// ImageEncoder::

String ImageEncoder::SaveRawString(const RawImage& image)
{
	StringStream ss;
	SaveRaw(ss, image);
	return ss;
}

//////////////////////////////////////////////////////////////////////

bool ImageEncoder::SaveRawFile(const char *fn, const RawImage& image)
{
	FileOut fo(fn);
	if(!fo.IsOpen())
		return false;
	SaveRaw(fo, image);
	fo.Close();
	return !fo.IsError();
}

//////////////////////////////////////////////////////////////////////

void ImageEncoder::Save(Stream& stream, const Image& image)
{
	ASSERT(stream.IsOpen() && stream.IsStoring());
	SaveRaw(stream, ImageToRawImage(image, save_bpp));
}

//////////////////////////////////////////////////////////////////////

String ImageEncoder::SaveString(const Image& image)
{
	StringStream ss;
	Save(ss, image);
	return ss;
}

//////////////////////////////////////////////////////////////////////

bool ImageEncoder::SaveFile(const char *fn, const Image& image)
{
	FileOut fo;
	if(!fo.Open(fn))
		return false;
	Save(fo, image);
	fo.Close();
	return !fo.IsError();
}

//////////////////////////////////////////////////////////////////////

RawImage ImageEncoder::LoadRawString(const String& s, int page_index)
{
	return LoadRaw(StringStream(s), page_index);
}

//////////////////////////////////////////////////////////////////////

RawImage ImageEncoder::LoadRawFile(const char *fn, int page_index)
{
	return LoadRaw(FileIn(fn), page_index);
}

//////////////////////////////////////////////////////////////////////

Image ImageEncoder::Load(Stream& stream, int page_index)
{
	return RawImageToImage(LoadRaw(stream, page_index));
}

//////////////////////////////////////////////////////////////////////

Image ImageEncoder::LoadString(const String& s, int page_index)
{
	return Load(StringStream(s), page_index);
}

//////////////////////////////////////////////////////////////////////

Image ImageEncoder::LoadFile(const char *fn, int page_index)
{
	return Load(FileIn(fn), page_index);
}

//////////////////////////////////////////////////////////////////////

ImageInfo ImageEncoder::Info(Stream& stream)
{
	Array<ImageInfo> out = InfoMulti(stream);
	if(out.IsEmpty())
		return ImageInfo();
	return out[0];
}

//////////////////////////////////////////////////////////////////////

Array<ImageInfo> ImageEncoder::InfoStringMulti(const String& s)
{
	return InfoMulti(StringStream(s));
}

//////////////////////////////////////////////////////////////////////

ImageInfo ImageEncoder::InfoString(const String& s)
{
	return Info(StringStream(s));
}

//////////////////////////////////////////////////////////////////////

Array<ImageInfo> ImageEncoder::InfoFileMulti(const char *fn)
{
	return InfoMulti(FileIn(fn));
}

//////////////////////////////////////////////////////////////////////

ImageInfo ImageEncoder::InfoFile(const char *fn)
{
	return Info(FileIn(fn));
}

//////////////////////////////////////////////////////////////////////

void ImageEncoder::SerializeRaw(Stream& stream, RawImage& image)
{
	if(stream.IsLoading())
		image = LoadRaw(stream);
	else
		SaveRaw(stream, image);
}

//////////////////////////////////////////////////////////////////////

void ImageEncoder::Serialize(Stream& stream, Image& image)
{
	if(stream.IsLoading())
		image = Load(stream);
	else
		Save(stream, image);
}

//////////////////////////////////////////////////////////////////////

One<ImageEncoder> MapInfo(Array<ImageInfo>& info, const EncoderMap& map, Stream& stream)
{
	if(!stream.IsOpen())
		return One<ImageEncoder>();
	ASSERT(stream.IsLoading());
	int pos = stream.GetPos();
	for(int i = 0; i < map.GetCount(); i++)
	{
		One<ImageEncoder> encoder = map[i]();
		if(encoder)
		{
			info = encoder -> InfoMulti(stream);
			if(!info.IsEmpty())
				return encoder;
		}
		stream.ClearError();
		stream.Seek(pos);
	}
	stream.ClearError();
	stream.Seek(pos);
	return One<ImageEncoder>();
}

//////////////////////////////////////////////////////////////////////

One<ImageEncoder> MapInfo(ImageInfo& info, const EncoderMap& map, Stream& stream)
{
	Array<ImageInfo> info_list;
	One<ImageEncoder> encoder = MapInfo(info_list, map, stream);
	if(info_list.IsEmpty())
		info = ImageInfo();
	else
		info = info_list[0];
	return encoder;
}

//////////////////////////////////////////////////////////////////////

Image MapLoad(const EncoderMap& map, Stream& stream, int page_index)
{
	if(!stream.IsOpen())
		return Image();
	Array<ImageInfo> info;
	int pos = stream.GetPos();
	One<ImageEncoder> encoder = MapInfo(info, map, stream);
	stream.Seek(pos);
	if(!encoder)
		return Image();
	return encoder -> Load(stream, page_index);
}

//////////////////////////////////////////////////////////////////////

RawImage MapLoadRaw(const EncoderMap& map, Stream& stream, int page_index)
{
	if(!stream.IsOpen())
		return RawImage();
	Array<ImageInfo> info;
	int pos = stream.GetPos();
	One<ImageEncoder> encoder = MapInfo(info, map, stream);
	stream.Seek(pos);
	if(!encoder)
		return RawImage();
	return encoder -> LoadRaw(stream, page_index);
}

//////////////////////////////////////////////////////////////////////

/*
void ImageTest()
{
	String src_path = "i:\\graph\\pic\\pics";
	String dest_path = GetTempPath();
	static const char *images[] =
	{
		"gandalf.bmp",
		"flower3.jpg",
		"San Pedro.gif",
	};
	try
	{
		ProgressMeter progress("Provádím testy...", 1, 14 * __countof(images));
		for(int i = 0; i < __countof(images); i++)
		{
			String in_path = AppendFileName(src_path, images[i]);
			String out_path = ForceExt(AppendFileName(dest_path, images[i]), "");
			dword to, t;
			t = GetTickCount();
			ImageInfo info;
			if(!StdInfoFile(info, in_path))
				throw Exc("Nelze naèíst obrázek '%s'.", in_path);
			RawImage image = StdLoadRawFile(in_path);
			if(!image)
				throw Exc("Nelze naèíst obrázek '%s'.", in_path);
			LOG(in_path << ": size = " << info.size << ", bpp = " << info.bits_per_pixel << ", dots = " << info.dots);
			to = t; t = GetTickCount(); LOG("load time: " << int(t - to));
			RawImage tr;
			Image im = image.ToImage();
			progress.StepCancelled(); GifEncoder()   .SaveFile(out_path + ".gif",     im);
			to = t; t = GetTickCount(); LOG("gif export time: " << int(t - to));
			progress.StepCancelled(); BmpEncoder(1)  .SaveFile(out_path + "_1.bmp",   im);
			to = t; t = GetTickCount(); LOG("bmp(1) export time: " << int(t - to));
			progress.StepCancelled(); BmpEncoder(4)  .SaveFile(out_path + "_4.bmp",   im);
			to = t; t = GetTickCount(); LOG("bmp(4) export time: " << int(t - to));
			progress.StepCancelled(); BmpEncoder(8)  .SaveFile(out_path + "_8.bmp",   im);
			to = t; t = GetTickCount(); LOG("bmp(8) export time: " << int(t - to));
			progress.StepCancelled(); BmpEncoder(24) .SaveFile(out_path + "_24.bmp",  im);
			to = t; t = GetTickCount(); LOG("bmp(24) export time: " << int(t - to));
			progress.StepCancelled(); JpgEncoder(100).SaveFile(out_path + "_100.jpg", im);
			to = t; t = GetTickCount(); LOG("jpg(100) export time: " << int(t - to));
			progress.StepCancelled(); JpgEncoder(60) .SaveFile(out_path + "_60.jpg",  im);
			to = t; t = GetTickCount(); LOG("jpg(60) export time: " << int(t - to));
			progress.StepCancelled(); JpgEncoder(20) .SaveFile(out_path + "_20.jpg",  im);
			to = t; t = GetTickCount(); LOG("jpg(20) export time: " << int(t - to));
			t = GetTickCount(); tr = image.Transformed(Image::TR_CLOCKWISE); t = GetTickCount() - t; LOG("clockwise rotation time: " << int(t));
			progress.StepCancelled(); GifEncoder().SaveRawFile(out_path + "_rc.gif", tr);
			t = GetTickCount(); tr = image.Transformed(Image::TR_ANTICLOCKWISE); t = GetTickCount() - t; LOG("anticlockwise rotation time: " << int(t));
			progress.StepCancelled(); GifEncoder().SaveRawFile(out_path + "_ra.gif", tr);
			t = GetTickCount(); tr = image.Transformed(Image::TR_X); t = GetTickCount() - t; LOG("x mirror time: " << int(t));
			progress.StepCancelled(); GifEncoder().SaveRawFile(out_path + "_x.gif", tr);
			t = GetTickCount(); tr = image.Transformed(Image::TR_Y); t = GetTickCount() - t; LOG("y mirror time: " << int(t));
			progress.StepCancelled(); GifEncoder().SaveRawFile(out_path + "_y.gif", tr);
			t = GetTickCount(); tr = image.Transformed(Image::TR_XY); t = GetTickCount() - t; LOG("y mirror time: " << int(t));
			progress.StepCancelled(); GifEncoder().SaveRawFile(out_path + "_xy.gif", tr);
			t = GetTickCount(); image.AlphaBlend(LtRed, 50); t = GetTickCount() - t; LOG("alpha blend time: " << int(t));
			progress.StepCancelled(); GifEncoder().SaveRawFile(out_path + "_alpha.gif", image);
		}
	}
	catch(Exc e)
	{
		e.Show();
	}
}
//*/

//////////////////////////////////////////////////////////////////////
#endif
