//////////////////////////////////////////////////////////////////////
// imageio_bmp: MS Windows BMP image file format.

#include "Draw.h"

#if defined(PLATFORM_WIN32) && defined(QLIB2IMAGE)

//////////////////////////////////////////////////////////////////////
// BmpEncoder::

BmpEncoder::BmpEncoder(int bpp_out, bool file_header)
: ImageEncoder(bpp_out)
, file_header(file_header)
{
}

//////////////////////////////////////////////////////////////////////

void BmpEncoder::SaveRaw(Stream& stream, const RawImage& image)
{
	ASSERT(stream.IsOpen() && stream.IsStoring());
	RawImage::Header header;
	image.SetHeader(header);
	header.biSizeImage = image.data.GetCount();
	header.biXPelsPerMeter = dot_size.cx ? fround(header.biWidth  * (1000.0 / 25.4 * 600.0) / dot_size.cx) : 0;
	header.biYPelsPerMeter = dot_size.cy ? fround(header.biHeight * (1000.0 / 25.4 * 600.0) / dot_size.cy) : 0;
	int pal_bytes = (image.bpp <= 8 ? sizeof(RGBQUAD) * (header.biClrUsed ? header.biClrUsed : (1 << image.bpp)) : 0);
	if(file_header)
	{
		BITMAPFILEHEADER bmfh;
		Zero(bmfh);
		bmfh.bfType = 'B' + 256 * 'M';
		bmfh.bfSize = sizeof(bmfh) + sizeof(BITMAPINFOHEADER) + pal_bytes + header.biSizeImage;
		stream.Put(&bmfh, sizeof(bmfh)); // NUXI
	}
	stream.Put(&header, sizeof(BITMAPINFOHEADER) + pal_bytes);
	stream.Put(image.data.Begin(), header.biSizeImage);
}

//////////////////////////////////////////////////////////////////////

Image BmpEncoder::Load(Stream& stream, int page_index)
{
	RawImage::Header header;
	RawImage raw_image = LoadRaw(stream, header);
	if(!raw_image)
		return Image();
	Image image(raw_image.GetSize());
	StretchDIBits(ImageDraw(image), 0, 0, header.biWidth, header.biHeight,
		0, 0, header.biWidth, header.biHeight, raw_image.data.Begin(), header, DIB_RGB_COLORS, SRCCOPY);
	return image;
}

//////////////////////////////////////////////////////////////////////

RawImage BmpEncoder::LoadRaw(Stream& stream, int page_index)
{
	RawImage::Header header;
	RawImage image = LoadRaw(stream, header);
	if(!image)
		return image;
	if(image.bpp <= 8)
	{ // retrieve palette
		image.palette.SetCount(1 << image.bpp);
		const RGBQUAD *q = header.palette;
		for(Color *p = image.palette.Begin(), *e = image.palette.End(); p < e; p++, q++)
			*p = Color(q -> rgbRed, q -> rgbGreen, q -> rgbBlue);
	}
	if(header.biCompression != BI_RGB)
	{ // convert image to RGB mode
		Image temp(header.biWidth, header.biHeight);
		StretchDIBits(ImageDraw(temp), 0, 0, header.biWidth, header.biHeight,
			0, 0, header.biWidth, header.biHeight, image.data.Begin(), header, DIB_RGB_COLORS, SRCCOPY);
		image = ImageToRawImage(temp, header.biBitCount);
	}
	return image;
}

//////////////////////////////////////////////////////////////////////

RawImage BmpEncoder::LoadRaw(Stream& stream, RawImage::Header& header)
{
	if(!stream.IsOpen())
		return RawImage();
	ASSERT(stream.IsLoading());
	if(!InfoRaw(stream, header))
		return RawImage();
	RawImage image(header.biWidth, header.biHeight, header.biBitCount);
	image.data.SetCount(header.biSizeImage);
	if(!stream.GetAll(image.data.Begin(), image.data.GetCount()))
		return RawImage(); // sync error
	return image;
}

//////////////////////////////////////////////////////////////////////

Array<ImageInfo> BmpEncoder::InfoMulti(Stream& stream)
{
	Array<ImageInfo> out;
	out.SetCount(1);
	ImageInfo& info = out[0];
	RawImage::Header header;
	if(!InfoRaw(stream, header))
		return Array<ImageInfo>();
	info.size = Size(tabs(header.biWidth), tabs(header.biHeight));
	info.bits_per_pixel = header.biBitCount;
	if(header.biXPelsPerMeter)
		info.dots.cx = fround(header.biWidth * (1000.0 / 25.4 * 600.0) / header.biXPelsPerMeter);
	if(header.biYPelsPerMeter)
		info.dots.cy = fround(header.biHeight * (1000.0 / 25.4 * 600.0) / header.biYPelsPerMeter);
	return out;
}

//////////////////////////////////////////////////////////////////////

bool BmpEncoder::InfoRaw(Stream& stream, RawImage::Header& header)
{
	if(!stream.IsOpen())
		return false;
	ASSERT(stream.IsLoading());
	if(file_header)
	{
		BITMAPFILEHEADER bmfh;
		// NUXI problem
		if(!stream.GetAll(&bmfh, sizeof(bmfh)) || bmfh.bfType != 'B' + 256 * 'M')
			return false;
	}
	Zero(header);
	if(!stream.GetAll(&header, sizeof(BITMAPINFOHEADER)) || header.biSize < sizeof(BITMAPINFOHEADER))
		return false;
	if(header.biBitCount != 1 && header.biBitCount != 4 && header.biBitCount != 8
	&& header.biBitCount != 16 && header.biBitCount != 24 && header.biBitCount != 32)
		return false;
	if(header.biSizeImage == 0)
	{
		if(header.biCompression != BI_RGB)
			return false;
		header.biSizeImage = header.biHeight * (((header.biWidth * header.biBitCount + 31) >> 3) & -4);
	}
	stream.SeekCur(header.biSize - sizeof(BITMAPINFOHEADER));
	if(header.biBitCount <= 8)
	{
		if(!stream.GetAll(header.palette, (header.biClrUsed ? header.biClrUsed : 1 << header.biBitCount) * sizeof(RGBQUAD)))
			return false;
	}
	else if(header.biBitCount == 16 || header.biBitCount == 32)
	{ // prepare 16-bit rgb masks & shifts
		if(header.biCompression == BI_RGB)
			;
		else if(header.biCompression == BI_BITFIELDS)
		{ // read bitfield masks
			if(!stream.GetAll(header.palette, 12))
				return false;
		}
		else
			return false;
	}
	if(header.biBitCount >= 16 && header.biClrUsed != 0)
		stream.SeekCur(header.biClrUsed * sizeof(RGBQUAD));
	return true;
}

//////////////////////////////////////////////////////////////////////
#endif
