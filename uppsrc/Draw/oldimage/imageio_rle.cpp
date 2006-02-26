//////////////////////////////////////////////////////////////////////
// imageio_rle.cpp: RLE image file format.

#include "Draw.h"

#if defined(PLATFORM_WIN32) && defined(QLIB2IMAGE)

//////////////////////////////////////////////////////////////////////

static bool RleStreamHeading(Stream& stream, Size& size, int& bpp, Vector<Color>& palette)
{
	int version = StreamHeading(stream, 1, 1, 1, "RleRawImage");
	if(version < 0)
		return false;
	stream % size;
	if(size.cx == 0 || size.cy == 0)
		return false;
	stream / bpp % palette;
	if(stream.IsError())
		return false;
	if(stream.IsLoading())
	{
		if(bpp != 1 && bpp != 4 && bpp != 8 && bpp != 16 && bpp != 24 && bpp != 32)
		{
			stream.SetError();
			return false;
		}
		if(size.cx < 0 || size.cy < 0)
		{
			stream.SetError();
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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
		if(EQ(src, src + UNIT) && (UNIT > 1 || EQ(src, src + 2))) \
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
			src += (UNIT == 1 ? 3 : 2 * UNIT); \
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

//////////////////////////////////////////////////////////////////////

RleEncoder::RleEncoder()
{
}

//////////////////////////////////////////////////////////////////////

RawImage RleEncoder::LoadRaw(Stream& stream, int page_index)
{
	Size size;
	int bpp;
	ASSERT(stream.IsLoading());
	Vector<Color> palette;
	if(!RleStreamHeading(stream, size, bpp, palette))
		return RawImage();
	RawImage image(size, bpp);
	image.palette = palette;
	if(size.cx == 0 || size.cy == 0)
		return image;
	int unit = ((bpp - 1) >> 3) + 1;
	ASSERT(unit >= 1 && unit <= 4);
	int items = ((size.cx * bpp - 1) >> 3) / unit + 1;
	for(int i = 0; i < size.cy; i++)
	{
		String s;
		stream % s;
		RleDecode(image.GetUpScan(i), unit, items, s);
	}
	return image;
}

//////////////////////////////////////////////////////////////////////

static bool RleStreamImageHeading(Stream& stream, Size& dot_size, int& bpp, int& flags)
{
	int version = StreamHeading(stream, 1, 1, 1, "RleImage");
	if(version < 0)
		return false;
	stream % dot_size / bpp / flags;
	return true;
}

//////////////////////////////////////////////////////////////////////

Image RleEncoder::Load(Stream& stream, int page_index)
{
	ASSERT(stream.IsLoading());
	Size size, dot_size;
	int bpp, flags;
	Pack16(stream, size);
	if(size.cx <= 0 || size.cy <= 0)
		return Image();
	if(!RleStreamImageHeading(stream, dot_size, bpp, flags))
		return Image();
	Image image(size);
	LoadRaw(stream, page_index).Paint(ImageDraw(image));
	if(flags & 1)
		LoadRaw(stream, page_index).Paint(ImageMaskDraw(image));
	return image;
}

//////////////////////////////////////////////////////////////////////

void RleEncoder::SaveRaw(Stream& stream, const RawImage& image)
{
	Size size = image.GetSize();
	int bpp = image.bpp;
	ASSERT(stream.IsStoring());
	if(!RleStreamHeading(stream, size, bpp, const_cast<Vector<Color>&>(image.palette)))
		return;
	int unit = ((bpp - 1) >> 3) + 1;
	ASSERT(unit >= 1 && unit <= 4);
	int items = ((size.cx * bpp - 1) >> 3) / unit + 1;
	String (*RleEncode)(const byte *pos, int items) = RleEncodeTable[unit - 1];
	for(int i = 0; i < size.cy; i++)
		stream % RleEncode(image.GetUpScan(i), items);
}

//////////////////////////////////////////////////////////////////////

void RleEncoder::Save(Stream& stream, const Image& image)
{
	ASSERT(stream.IsStoring());
	Size size, dots;
	size = image.GetSize();
	Pack16(stream, size);
	if(size.cx <= 0 || size.cy <= 0)
		return;
	dots = dot_size;
	int bpp = 24;
	int flags = image.HasMask() ? 1 : 0;
	BITMAP info;
	if(GetObject(image.GetHBITMAP(), sizeof(BITMAP), &info))
		bpp = info.bmBitsPixel * info.bmPlanes;
	bpp = (bpp > 24 ? 32 : bpp > 16 ? 24 : bpp > 8 ? 16 : bpp > 4 ? 8 : bpp > 1 ? 4 : 1);
	RleStreamImageHeading(stream, dots, bpp, flags);
	SaveRaw(stream, ImageToRawImage(image, bpp));
	if(flags & 1)
		SaveRaw(stream, MaskToRawImage(image, true));
}

//////////////////////////////////////////////////////////////////////

Array<ImageInfo> RleEncoder::InfoMulti(Stream& stream)
{
	ASSERT(stream.IsLoading());
	Array<ImageInfo> out;
	out.SetCount(1);
	ImageInfo& info = out[0];
	int flags;
	Pack16(stream, info.size);
	if(info.size.cx <= 0 || info.size.cy <= 0
	|| !RleStreamImageHeading(stream, info.dots, info.bits_per_pixel, flags))
		return Array<ImageInfo>();
	return out;
}

//////////////////////////////////////////////////////////////////////
#endif
