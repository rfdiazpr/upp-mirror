//////////////////////////////////////////////////////////////////////
// designer/ImageBuffer: image canvas.


#include "Draw.h"

#if defined(PLATFORM_WIN32) && defined(QLIB2IMAGE)
	
//////////////////////////////////////////////////////////////////////

inline int CalcGray(const byte *p) { return p[0] * 26 + p[1] * 153 + p[2] * 77; }

//////////////////////////////////////////////////////////////////////

static void memswap(void *_a, void *_b, unsigned count)
{
	byte *a = reinterpret_cast<byte *>(_a);
	byte *b = reinterpret_cast<byte *>(_b);
	while(count >= 4)
	{
		byte t;
		t = a[0]; a[0] = b[0]; b[0] = t;
		t = a[1]; a[1] = b[1]; b[1] = t;
		t = a[2]; a[2] = b[2]; b[2] = t;
		t = a[3]; a[3] = b[3]; b[3] = t;
		a += 4;
		b += 4;
		count -= 4;
	}
	if(count > 0)
		do
		{
			byte t = *a; *a++ = *b; *b++ = t;
		}
		while(--count);
}

//////////////////////////////////////////////////////////////////////
// ImageBuffer::

ImageBuffer::ImageBuffer()
{
	SetSize(Size(0, 0));
}

//////////////////////////////////////////////////////////////////////

ImageBuffer::ImageBuffer(Size size)
{
	SetSize(size);
}

//////////////////////////////////////////////////////////////////////

ImageBuffer::ImageBuffer(int cx, int cy)
{
	SetSize(cx, cy);
}

//////////////////////////////////////////////////////////////////////

ImageBuffer::ImageBuffer(const ImageBuffer& buffer, int)
: ImageBufferBase(buffer), data(buffer.data, 0), mask(buffer.mask, 0)
{
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::Serialize(Stream& stream)
{
	Size s = size;
	stream % s;
	String rle;
	if(stream.IsStoring())
		rle = GetRLE();
	stream % rle;
	if(stream.IsLoading())
	{
		SetSize(s);
		SetRLE(rle);
	}
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::SetSize(Size _size)
{
	size = _size;
	top = size.cy - 1;
	data_scan = (3 * size.cx + 3) & -4;
	mask_scan = (size.cx + 3) & -4;
	data.SetCount(data_scan * size.cy);
	mask.SetCount(mask_scan * size.cy);
	Clear();
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::Clear()
{
	memset(mask.Begin(), 0, mask.GetCount());
	memset(data.Begin(), 0, data.GetCount());
}

//////////////////////////////////////////////////////////////////////

bool ImageBuffer::HasMask() const
{
	typedef byte int1;
	typedef word int2;
	typedef dword int4;
	ASSERT(sizeof(int1) == 1 && sizeof(int2) == 2 && sizeof(int4) == 4); // update algorithm or change type when not
	for(int i = 0; i < size.cy; i++)
	{
		const int4 *p = reinterpret_cast<const int4 *>(Mask(0, i));
		int count = size.cx;
		while(count >= 32)
		{
			if((p[0] & p[1] & p[2] & p[3] & p[4] & p[5] & p[6] & p[7]) != 0x01010101)
				return true;
			p += 8;
			count -= 32;
		}
		if(count & 16)
			if((p[0] & p[1] & p[2] & p[3]) != 0x01010101)
				return true;
			else
				p += 4;
		if(count & 8)
			if((p[0] & p[1]) != 0x01010101)
				return true;
			else
				p += 2;
		if(count & 4)
			if(p[0] != 0x01010101)
				return true;
			else
				p++;
		const byte *bp = reinterpret_cast<const byte *>(p);
		if(count >= 2)
			if(*reinterpret_cast<const int2 *>(bp) != 0x0101)
				return true;
			else
				bp += 2;
		if(count & 1 && *bp == 0)
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::SetPixel(int x, int y, Color color)
{
	if(Contains(x, y))
		if(IsNull(color))
			*Mask(x, y) = 0;
		else
		{
			*Mask(x, y) = 1;
			byte *ptr = Data(x, y);
			ptr[0] = color.GetB();
			ptr[1] = color.GetG();
			ptr[2] = color.GetR();
		}
}

//////////////////////////////////////////////////////////////////////

Color ImageBuffer::GetPixel(Point pt) const
{
	if(Contains(pt))
		if(*Mask(pt))
		{
			const byte *p = Data(pt);
			return Color(p[2], p[1], p[0]);
		}
	return Null;
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::PaintData(Draw& draw, const Rect& src, const Rect& dest, int rop) const
{
	if(src.IsEmpty() || dest.IsEmpty())
		return;

	BITMAPINFOHEADER bmih;
	SetBitmapHeader(bmih);

	StretchDIBits(draw, dest.left, dest.top, dest.Width(), dest.Height(),
		src.left, size.cy - src.bottom, src.Width(), src.Height(),
		data.Begin(), (const BITMAPINFO *)&bmih, DIB_RGB_COLORS, rop);
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::PaintMask(Draw& draw, const Rect& src, const Rect& dest, Color color0, Color color1, int rop) const
{
	if(src.IsEmpty() || dest.IsEmpty())
		return;

	struct : BITMAPINFOHEADER
	{
		RGBQUAD palette[256];
	}
	bmih;

	memset(&bmih, 0, sizeof(bmih));
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = size.cx;
	bmih.biHeight = size.cy;
	bmih.biPlanes = 1;
	bmih.biBitCount = 8;
	bmih.palette[0].rgbRed   = GetRValue(color0);
	bmih.palette[0].rgbGreen = GetGValue(color0);
	bmih.palette[0].rgbBlue  = GetBValue(color0);
	bmih.palette[1].rgbRed   = GetRValue(color1);
	bmih.palette[1].rgbGreen = GetGValue(color1);
	bmih.palette[1].rgbBlue  = GetBValue(color1);

//	DUMP(*Mask(src.TopLeft()));

/*	draw.BeginGdi();
	draw.Select(Stock.GetSolidBrush());
	Size s = src.Size(), d = dest.Size();
	for(int y = src.top; y < src.bottom; y++)
		for(int x = src.left; x < src.right; x++)
		{
			Rect d(
				(x + 0 - src.left) * d.cx / s.cx + dest.left,
				(y + 0 - src.top)  * d.cy / s.cy + dest.top,
				(x + 1 - src.left) * d.cx / s.cx + dest.left,
				(y + 1 - src.top)  * d.cy / s.cy + dest.top);
			draw.SetBkColor(*Mask(x, y) ? color1 : color0);
			draw.PatBlt(d, rop);
		}
	draw.EndGdi();
*/
	StretchDIBits(draw, dest.left, dest.top, dest.Width(), dest.Height(),
		src.left, size.cy - src.bottom, src.Width(), src.Height(),
		mask.Begin(), (const BITMAPINFO *)&bmih, DIB_RGB_COLORS, rop);
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::Paint(Draw& draw, const Rect& src, const Rect& dest, Color color) const
{
	if(src.IsEmpty() || dest.IsEmpty())
		return;
	if(!IsNull(color))
	{ // paint opaque
		PaintData(draw, src, dest, SRCCOPY);
		PaintMask(draw, src, dest);
		PaintMask(draw, src, dest, color, Black, SRCPAINT);
	}
	else
	{ // paint transparent
		PaintData(draw, src, dest, SRCINVERT);
		PaintMask(draw, src, dest, White, Black, SRCAND);
		PaintData(draw, src, dest, SRCINVERT);
	}
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::Paint(Draw& draw, const Rect& dest, Color color) const
{
	Paint(draw, Rect(size), dest, color);
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::Paint(Draw& draw, Point pos, Color color) const
{
	Paint(draw, Rect(size), Rect(pos, size), color);
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::GetScanRLE(String& result, int y) const
{
	const byte *dp = Data(0, y);
	const byte *mp = Mask(0, y);
	const byte *me = mp + size.cx;
	while(mp < me)
	{
		if(*mp == 0)
		{ // skip run
			const byte *ms = mp;
			do
			{
				mp++;
				dp += 3;
			}
			while(mp < me && *mp == 0);
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
			while(++mp < me && *mp)
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

//////////////////////////////////////////////////////////////////////

String ImageBuffer::GetRLE() const
{
	String result;
	int last = 0;
	for(int y = 0; y < size.cy; y++)
	{
		int prev = result.GetLength();
		GetScanRLE(result, y);
		if(result.GetLength() > prev)
			last = result.GetLength();
		result.Cat(0x80); // row end
	}
	result.Trim(last);
	return result;
}

//////////////////////////////////////////////////////////////////////

const byte *ImageBuffer::SetScanRLE(const byte *src, int y)
{
	byte *dp = Data(0, y), *mp = Mask(0, y), *me = mp + size.cx;
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
				memset(mp, 1, count);
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
			memset(mp, 1, count);
			memcpy(dp, src, 3 * count);
			dp += 3 * count;
			mp += count;
			src = new_src;
		}
	}
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::SetRLE(const byte *src)
{
	Clear();
	for(int y = 0; y < size.cy && *(src = SetScanRLE(src, y)); y++, src++)
		;
}

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

void ImageBuffer::SetGrayed(const Rect& rc)
{
	Rect r = rc & Rect(size);
	if(r.IsEmpty())
		return;
	const byte *gamma = GetGammaTable();
	int w3 = 3 * r.Width();
	for(int y = r.top; y < r.bottom; y++)
		for(byte *b = Data(r.left, y), *e = b + w3; b < e; b += 3)
			b[0] = b[1] = b[2] = gamma[CalcGray(b) >> (16 - GAMMA_BITS)];
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::SetEtched(const Rect& rc)
{
	Rect r = rc & Rect(size);
	if(r.IsEmpty())
		return;
	int w3 = 3 * r.Width();
	for(int y = r.bottom; --y > r.top;)
	{
		byte *db = Data(r.left, y), *de = db + w3, *mb = Mask(r.left, y);
		const byte *ldb = Data(r.left, y - 1), *lmb = Mask(r.left, y - 1);
		if(*mb && CalcGray(db) < 0x8000)
			db[0] = db[1] = db[2] = 0x80;
		else
			*mb = 0;
		while(++mb, (db += 3) < de)
		{
			if(*mb && CalcGray(db) < 0x8000)
				db[0] = db[1] = db[2] = 0x80;
			else if(*lmb && CalcGray(ldb) < 0x8000)
			{
				db[0] = db[1] = db[2] = 0xFF;
				*mb = 1;
			}
			else
				*mb = 0;
			lmb++;
			ldb += 3;
		}
	}

	{ // 1st row must be handled in a special way
		for(byte *db = Data(r.TopLeft()), *de = db + w3, *mb = Mask(r.TopLeft()); db < de; db += 3, mb++)
			if(*mb && CalcGray(db) < 0x8000)
				db[0] = db[1] = db[2] = 0x80;
			else
				*mb = 0;
	}
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::Copy(Point pos, const ImageBuffer& src, const Rect& src_rect, bool transparent)
{
	Rect clipped = src_rect & Rect(src.GetSize());
	if(clipped.IsEmpty())
		return;
	pos += clipped.TopLeft() - src_rect.TopLeft();
	if(pos.x >= size.cx || pos.y >= size.cy)
		return;
	if(pos.x < 0)
	{
		if(clipped.Width() + pos.x <= 0)
			return;
		clipped.left -= pos.x;
		pos.x = 0;
	}
	if(pos.y < 0)
	{
		if(clipped.Height() + pos.y <= 0)
			return;
		clipped.top -= pos.y;
		pos.y = 0;
	}
	int above;
	if((above = pos.x + clipped.Width() - size.cx) > 0)
		clipped.right -= above;
	if((above = pos.y + clipped.Height() - size.cy) > 0)
		clipped.bottom -= above;

	int cm = clipped.Width();
	for(int y = clipped.top; y < clipped.bottom; y++, pos.y++)
		SetPixel(pos.x, pos.y, cm, src.Data(clipped.left, y), src.Mask(clipped.left, y), transparent);
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::Copy(const Rect& dest, const ImageBuffer& src, const Rect& src_rect, bool transparent)
{
	if(dest.IsEmpty() || src_rect.IsEmpty())
		return;
	if(dest.Size() == src_rect.Size())
	{
		Copy(dest.TopLeft(), src, src_rect, transparent);
		return;
	}
	Size ds = max(dest.Size() - 1, Size(1, 1)), ss = max(src_rect.Size() - 1, Size(1, 1));
	Size skip = ss / ds, add = ss - ds * skip;
	int skip3 = 3 * skip.cx;
	Point dp = dest.TopLeft(), sp = src_rect.TopLeft();
	Size counter = ds >> 1, items = min(dest.Size(), Size(size - max(dp, Point(0, 0))));
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

	full = counter.cy + items.cy * ss.cy;
	fc = full / ds.cy;
	if(fc + sp.y >= src.size.cy)
		items.cy = idivceil((src.size.cy - sp.y) * ds.cy - counter.cy, ss.cy);

	if(items.cx <= 0 || items.cy <= 0)
		return;

	for(; --items.cy >= 0; dp.y++)
	{
		byte *dd = Data(dp), *dm = Mask(dp), *de = dm + items.cx;
		const byte *sd = src.Data(sp), *sm = src.Mask(sp);
		int c = counter.cx;
		if(transparent)
			for(; dm < de; dm++, dd += 3)
			{
				if(*sm)
				{
					dd[0] = sd[0];
					dd[1] = sd[1];
					dd[2] = sd[2];
					*dm = 1;
				}
				sd += skip3;
				sm += skip.cx;
				if((c += add.cx) >= ds.cx)
				{
					c -= ds.cx;
					sd += 3;
					sm++;
				}
			}
		else
			for(; dm < de; dm++, dd += 3)
			{
				dd[0] = sd[0];
				dd[1] = sd[1];
				dd[2] = sd[2];
				*dm = *sm;
				sd += skip3;
				sm += skip.cx;
				if((c += add.cx) >= ds.cx)
				{
					c -= ds.cx;
					sd += 3;
					sm++;
				}
			}
		sp.y += skip.cy;
		if((counter.cy += add.cy) >= ds.cy)
		{
			counter.cy -= ds.cy;
			sp.y++;
		}
	}
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::SetPixel(int x, int y, int count, const byte *data, const byte *mask, bool transparent)
{
	byte *d = Data(x, y), *m = Mask(x, y);
	if(transparent)
	{
		const byte *e = mask + count;
		while(mask < e)
			if(*mask)
			{
				const byte *s = mask;
				while(++mask < e && *mask)
					;
				int n = mask - s, tn = 3 * n;
				memcpy(d, data, tn);
				memcpy(m, s, n);
				d += tn;
				data += tn;
				m += n;
			}
			else
			{
				const byte *s = mask;
				while(++mask < e && !*mask)
					;
				int n = mask - s, tn = 3 * n;
				d += tn;
				data += tn;
				m += n;
			}
	}
	else
	{
		memcpy(d, data, 3 * count);
		memcpy(m, mask, count);
	}
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::Fill(const Rect& rc, Color color)
{
	Rect r = rc & Rect(size);
	if(r.IsEmpty())
		return;
	int w = r.Width();
	if(IsNull(color))
		for(int y = r.top; y < r.bottom; y++)
			memset(Mask(r.left, y), 0, w);
	else
	{
		byte temp[3] = { color.GetB(), color.GetG(), color.GetR() };
		for(int y = r.top; y < r.bottom; y++)
		{
			memsetex(Data(r.left, y), temp, 3, w);
			memset(Mask(r.left, y), 1, w);
		}
	}
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::WriteDib(Stream& stream, const Rect& rc, Color bgcolor) const
{
	Rect r = rc & Rect(size);
	BITMAPINFOHEADER bmih;
	SetBitmapHeader(bmih, r.Size());
	stream.Put(&bmih, sizeof(bmih));
	byte rgb[] = { bgcolor.GetB(), bgcolor.GetG(), bgcolor.GetR() };
	int skip = (-3 * size.cx) & 3;
	for(int y = r.bottom; --y >= r.top;)
	{
		const byte *d = Data(r.left, y), *m = Mask(r.left, y), *e = m + bmih.biWidth;
		while(m < e)
			if(*m)
			{
				const byte *s = m;
				while(++m < e && *m)
					;
				int count = 3 * (m - s);
				stream.Put(d, count);
				d += count;
			}
			else
				do
				{
					stream.Put(rgb, 3);
					d += 3;
				}
				while(++m < e && !*m);
		if(skip)
			stream.Put(0, skip);
	}
}

//////////////////////////////////////////////////////////////////////

int ImageBuffer::GetDibSize(Size size)
{
	return sizeof(BITMAPINFOHEADER) + ((3 * size.cx + 3) & -4) * size.cy;
}

//////////////////////////////////////////////////////////////////////

ImageBuffer& ImageBuffer::From(Draw& draw, const Rect& rc, Color bgcolor)
{
	SetSize(rc.Size());
	Image image(size);
	BitBlt(ImageDraw(draw, image), Point(0, 0), draw, rc);
	BITMAPINFOHEADER bmih;
	SetBitmapHeader(bmih);
	GetDIBits(draw, image.GetHBITMAP(), 0, size.cy, data.Begin(), 
		reinterpret_cast<BITMAPINFO *>(&bmih), DIB_RGB_COLORS);
	if(IsNull(bgcolor))
		memset(mask.Begin(), 1, mask.GetCount());
	else
	{
		byte r = bgcolor.GetR(), g = bgcolor.GetG(), b = bgcolor.GetB();
		for(int y = 0; y < size.cy; y++)
		{
			const byte *d = Data(0, y);
			for(byte *m = Mask(0, y), *e = m + size.cx; m < e; d += 3)
				*m++ = (d[0] != b || d[1] != g || d[2] != r);
		}
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////

ImageBuffer& ImageBuffer::From(Draw& ddata, const Rect& rc, Draw& dmask, Point pos_mask)
{
	SetSize(rc.Size());
	Image image(size);
	BITMAPINFOHEADER bmih;
	SetBitmapHeader(bmih);
	BitBlt(ImageDraw(ddata, image), Point(0, 0), dmask, Rect(pos_mask, size));
	GetDIBits(dmask, image.GetHBITMAP(), 0, size.cy, data.Begin(), 
		reinterpret_cast<BITMAPINFO *>(&bmih), DIB_RGB_COLORS);
	{ // generate mask
		for(int y = 0; y < size.cy; y++)
		{
			byte *d = Data(0, y), *m = Mask(0, y), *e = m + size.cx;
			for(; m < e; d += 3)
				*m++ = (~CalcGray(d) >> 15) & 1;
		}
	}
	BitBlt(ImageDraw(ddata, image), Point(0, 0), ddata, rc);
	GetDIBits(ddata, image.GetHBITMAP(), 0, size.cy, data.Begin(), 
		reinterpret_cast<BITMAPINFO *>(&bmih), DIB_RGB_COLORS);
	return *this;
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::SetBitmapHeader(BITMAPINFOHEADER& bmih, Size size)
{
	Zero(bmih);
	bmih.biSize = sizeof(bmih);
	bmih.biWidth = size.cx;
	bmih.biHeight = size.cy;
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::Curve(const Vector<Point>& list, Color color)
{
	for(int i = 0; i < list.GetCount(); i++)
		SetPixel(list[i], color);
}

//////////////////////////////////////////////////////////////////////

ImageBuffer ImageBuffer::GetPart(const Rect& rc) const
{
	Rect r = rc & Rect(size);
	ImageBuffer dest(r.Size());
	dest.Copy(Point(0, 0), *this, r);
	return dest;
}

//////////////////////////////////////////////////////////////////////

ImageBuffer ImageBuffer::GetPart(const Rect& rc, Size size) const
{
	if(rc.Size() == size)
		return GetPart(rc);
	ImageBuffer dest(size);
	Rect r = rc & Rect(size);
	if(!r.IsEmpty())
		dest.Copy(Rect(size), *this, r);
	return dest;
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::ChangeColor(const Rect& rc, Color src, Color dest, int tolerance)
{
	Rect r = rc & Rect(size);
	if(r.IsEmpty())
		return;
	byte get_r = src.GetR(), get_g = src.GetG(), get_b = src.GetB();
	byte set_r = dest.GetR(), set_g = dest.GetG(), set_b = dest.GetB();
	byte set_mask = IsNull(dest) ? 0 : 1;
	tolerance = minmax(tolerance, 0, 255);
	tolerance = tolerance * tolerance;
	for(int y = r.top; y < r.bottom; y++)
	{
		byte *d = Data(r.left, y), *m = Mask(r.left, y), *e = m + r.Width();
		if(IsNull(src))
		{
			for(; m < e; m++, d += 3)
				if(*m == 0)
					*m = set_mask, d[0] = set_b, d[1] = set_g, d[2] = set_r;
		}
		else if(tolerance == 0)
		{
			for(; m < e; m++, d += 3)
				if(*m && d[0] == get_b && d[1] == get_g && d[2] == get_r)
					*m = set_mask, d[0] = set_b, d[1] = set_g, d[2] = set_r;
		}
		else
		{ // slow version for tolerance test
			for(; m < e; m++, d += 3)
				if(*m)
				{
					int dr = d[2] - get_r, dg = d[1] - get_g, db = d[0] - get_b;
					if(dr * dr + dg * dg + db * db <= tolerance)
						*m = set_mask, d[0] = set_b, d[1] = set_g, d[2] = set_r;
				}
		}
	}
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::FloodFill(Color color, Point pt, const Rect& rc, bool diagonal)
{
	Rect r = rc & Rect(size);
	if(!r.Contains(pt))
		return;

	Color src = GetPixel(pt);
	byte get[3] = { src.GetB(), src.GetG(), src.GetR() };

	{ // prepare mask
		for(int y = r.top; y < r.bottom; y++)
		{
			byte *m = Mask(r.left, y), *e = m + r.Width();
			if(IsNull(src))
			{
				while(m < e)
					if(*m++ == 0)
						m[-1] = 2;
			}
			else
			{
				const byte *d = Data(r.left, y);
				for(; m < e; d += 3)
					if(*m++ && d[0] == get[0] && d[1] == get[1] && d[2] == get[2])
						m[-1] = 2;
			}
		}
	}
	Rect flood = Rect(pt, Size(1, 1));
	*Mask(pt) = 3;

	bool done;
	do
	{
		done = false;
		for(int y = flood.top; y < flood.bottom; y++)
		{
			byte *b = Mask(0, y), *m = b + flood.left, *e = b + flood.right;
			for(; m < e; m++)
				if(*m == 3)
				{ // change to 3 and disperse
					*m = 4;
					done = true;
					byte *t = m;
					int i;

					for(i = y, t = m; --i >= r.top && *(t += mask_scan) == 2; *t = 3)
						;
					if(++i < flood.top) flood.top = i;

					for(i = m - b, t = m; --i >= r.left && *--t == 2; *t = 3)
						;
					if(++i < flood.left) flood.left = i;

					for(i = y, t = m; ++i < r.bottom && *(t -= mask_scan) == 2; *t = 3)
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
							if(x > r.left && m[mask_scan - 1] == 2)
							{
								m[mask_scan - 1] = 3;
								if(x - 0 <= flood.left) flood.left = x - 1;
								if(y - 0 <= flood.top)  flood.top = y - 1;
							}
							if(x + 1 < r.right && m[mask_scan + 1] == 2)
							{
								m[mask_scan + 1] = 3;
								if(x + 1 >= flood.right) flood.right = x + 2;
								if(y - 0 <= flood.top)   flood.top = y - 1;
							}
						}
						if(y + 1 < r.bottom)
						{
							if(x > r.left && m[-mask_scan - 1] == 2)
							{
								m[-mask_scan - 1] = 3;
								if(x - 0 <= flood.left)   flood.left = x - 1;
								if(y + 1 >= flood.bottom) flood.bottom = y + 2;
							}
							if(x + 1 < r.right && m[-mask_scan + 1] == 2)
							{
								m[-mask_scan + 1] = 3;
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
	byte set_mask = IsNull(color) ? 0 : 1;
	for(int y = r.top; y < r.bottom; y++)
	{
		byte *d = Data(r.left, y), *m = Mask(r.left, y), *e = m + r.Width();
		while(m < e)
		{
			byte *r = m;
			if(*m >= 3)
			{
				while(++m < e && *m >= 3)
					;
				if(set_mask)
					memsetex(d, set, 3, m - r);
				memset(r, set_mask, m - r);
			}
			else if(*m == 2)
			{ // restore to previous value
				while(++m < e && *m == 2)
					;
				memset(r, IsNull(src) ? 0 : 1, m - r);
			}
			else
				while(++m < e && *m < 2)
					;
			d += 3 * (m - r);
		}
	}
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::MirrorX(const Rect& rc)
{
	Rect r = rc & Rect(size);
	if(r.IsEmpty() || r.Width() <= 1)
		return;
	int w = r.Width() - 1, w3 = 3 * w;
	for(int y = r.top; y < r.bottom; y++)
	{
		byte *db = Data(r.left, y), *de = db + w3;
		byte *mb = Mask(r.left, y), *me = mb + w;
		while(mb < me)
		{
			byte t;
			t = db[0]; db[0] = de[0]; de[0] = t;
			t = db[1]; db[1] = de[1]; de[1] = t;
			t = db[2]; db[2] = de[2]; de[2] = t;
			t = mb[0]; mb[0] = me[0]; me[0] = t;
			mb++;
			me--;
			db += 3;
			de -= 3;
		}
	}
}

//////////////////////////////////////////////////////////////////////

void ImageBuffer::MirrorY(const Rect& rc)
{
	Rect r = rc & Rect(size);
	if(r.IsEmpty() || r.Height() <= 1)
		return;
	int a = r.top + r.bottom - 1;
	int w = r.Width(), w3 = 3 * w;
	for(int y = r.top; y < a - y; y++)
	{
		memswap(Data(r.left, y), Data(r.left, a - y), w3);
		memswap(Mask(r.left, y), Mask(r.left, a - y), w);
	}
}

//////////////////////////////////////////////////////////////////////

ImageBuffer ImageBuffer::GetRotated(const Rect& rc) const
{
	Rect r = rc & Rect(size);
	if(r.IsEmpty() || r.Width() == 1 && r.Height() == 1)
		return ImageBuffer(*this, 0);
	int wd = r.Width();
	ImageBuffer im(r.Height(), wd);
	for(int y = r.top; y < r.bottom; y++)
	{
		const byte *sd = Data(r.left, y), *sm = Mask(r.left, y), *se = sm + wd;
		byte *dd = im.Data(y - r.top, wd - 1), *dm = im.Mask(y - r.top, wd - 1);
		while(sm < se)
		{
			dd[0] = sd[0];
			dd[1] = sd[1];
			dd[2] = sd[2];
			*dm = *sm;
			sd += 3;
			sm++;
			dd += im.data_scan;
			dm += im.mask_scan;
		}
	}
	return im;
}

//////////////////////////////////////////////////////////////////////

ImageBuffer& ImageBuffer::FromClipboard(Color bgcolor)
{
	if(ReadClipboardFormat(*this))
		return *this;

	HGLOBAL hmem;
	if(!OpenClipboard(NULL))
	{
		Reset();
		return *this;
	}
	if((hmem = GetClipboardData(CF_DIB)) == NULL)
	{
		CloseClipboard();
		Reset();
		return *this;
	}
	const BITMAPINFO *bmi = (const BITMAPINFO *)GlobalLock(hmem);
	int hdrsize = bmi -> bmiHeader.biSize;
	if(bmi -> bmiHeader.biBitCount <= 8)
		hdrsize += 4 << bmi -> bmiHeader.biBitCount;
	else if(bmi -> bmiHeader.biBitCount == 16 || bmi -> bmiHeader.biBitCount == 32)
		hdrsize += 12;
	const void *data = (const byte *)bmi + hdrsize;
	Image image(bmi -> bmiHeader.biWidth, bmi -> bmiHeader.biHeight);
	Rect rc = Rect(image.GetSize());
	ImageDraw draw(image);
	draw.DrawRect(draw.GetClip(), White);
	StretchDIBits(draw, 0, 0, rc.Width(), rc.Height(),
		rc.left, rc.top, rc.right, rc.bottom, 
		data, bmi, DIB_RGB_COLORS, SRCCOPY);
	From(draw, Rect(image.GetSize()), bgcolor);
	GlobalUnlock(hmem);
	CloseClipboard();
	return *this;
}

//////////////////////////////////////////////////////////////////////
#endif
