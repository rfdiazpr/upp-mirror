#include <Draw/Draw.h>

#ifdef COMPILER_MSC
#pragma pack(push, 1)
#endif

struct ICONDIR
{
	word           idReserved;   // Reserved (must be 0)
	word           idType;       // Resource Type (1 for icons)
	word           idCount;      // How many images?
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

struct ICONDIRENTRY
{
	byte        bWidth;          // Width, in pixels, of the image
	byte        bHeight;         // Height, in pixels, of the image
	byte        bColorCount;
	byte        bReserved;
//	short       wPlanes;
//	short       wBitCount;
	short       wHotSpotX;
	short       wHotSpotY;
	dword       dwBytesInRes;    // How many bytes in this resource?
	dword       dwImageOffset;   // Where in the file is this image?
}
#ifdef COMPILER_GCC
__attribute__((packed))
#endif
;

#ifdef COMPILER_MSC
#pragma pack(pop)
#endif

BmpEncoder::BmpEncoder(int save_bpp, BMPFORMAT bmpformat)
: ImageEncoder(save_bpp)
, bmpformat(bmpformat)
{
}

BmpEncoder::~BmpEncoder() {}

void BmpEncoder::SaveRaw(Stream& stream, const Vector<const AlphaArray *>& image)
{
	ASSERT(stream.IsOpen() && stream.IsStoring());
	int ico_fixup = -1;
	if(bmpformat == BF_ICOFILE)
	{
		ico_fixup = (int)stream.GetPos();
		ICONDIR id;
		Zero(id);
		id.idType = 1;
		id.idCount = image.GetCount();
		stream.Put(&id, sizeof(id));
		stream.Put0(sizeof(ICONDIRENTRY) * image.GetCount());
	}

	int count = (bmpformat == BF_ICOFILE ? image.GetCount() : 1);
	for(int i = 0; i < count; i++)
	{
		int fpos = (int)stream.GetPos();
		Header header;

		PixelArray pcvt;
		const PixelArray *pix = &image[i] -> pixel;
		if(save_bpp && pix -> GetRawBPP() != save_bpp)
		{
			pcvt = PixelConvert(*pix, save_bpp);
			pix = &pcvt;
		}

		Size size = pix -> GetSize();
		ASSERT(pix -> alignment == 4); // WIN32 doesn't support other alignments
		Zero(header);
		header.biSize = sizeof(BITMAPINFOHEADER);
		header.biWidth = size.cx;
		header.biHeight = size.cy;
		if(bmpformat == BF_ICOFILE)
			header.biHeight <<= 1;
		header.biBitCount = pix -> bpp;
		header.biPlanes = 1;
		header.biCompression = 0; /* BI_RGB */
		int nclr = (pix -> bpp <= 8 ? 1 << pix -> bpp : 0);
		if(bmpformat == BF_BMPFILE && nclr && pix -> palette.GetCount() < nclr)
			header.biClrUsed = nclr = max(2, pix -> palette.GetCount());
		if(nclr)
		{
			const Color *c = pix -> palette.Begin();
			for(RGBQUAD *p = header.palette, *e = p + pix -> palette.GetCount(); p < e; p++, c++)
			{
				p -> rgbRed   = c -> GetR();
				p -> rgbGreen = c -> GetG();
				p -> rgbBlue  = c -> GetB();
			}
		}
		if(pix -> bpp == 16 || pix -> bpp == 32)
		{
			dword *bitfields = reinterpret_cast<dword *>(header.palette);
			bitfields[0] = pix -> cmask[0];
			bitfields[1] = pix -> cmask[1];
			bitfields[2] = pix -> cmask[2];
			header.biCompression = 3; /* BI_BITFIELDS */
		}
		header.biSizeImage = pix -> GetBytes();
		header.biXPelsPerMeter = dot_size.cx ? fround(header.biWidth  * (1000.0 / 25.4 * 600.0) / dot_size.cx) : 0;
		header.biYPelsPerMeter = dot_size.cy ? fround(header.biHeight * (1000.0 / 25.4 * 600.0) / dot_size.cy) : 0;
		int pal_bytes = (pix -> bpp <= 8 ? sizeof(RGBQUAD) * (header.biClrUsed ? header.biClrUsed : (1 << pix -> bpp)) : 0);
		if(bmpformat == BF_BMPFILE)
		{
			BITMAPFILEHEADER bmfh;
			Zero(bmfh);
			bmfh.bfType = 'B' + 256 * 'M';
			bmfh.bfOffBits = sizeof(bmfh) + sizeof(BITMAPINFOHEADER) + pal_bytes;
			bmfh.bfSize = sizeof(bmfh) + sizeof(BITMAPINFOHEADER) + pal_bytes + header.biSizeImage;
			stream.Put(&bmfh, sizeof(bmfh)); // NUXI
		}
		stream.Put(&header, sizeof(BITMAPINFOHEADER) + pal_bytes);
#if IMAGE_ORIGIN_BOTTOM
		stream.Put(pix -> Begin(), pix -> GetBytes());
#else
		for(int y = 0; y < pix -> size.cy; i++)
			stream.Put(pix -> GetUpScan(y), pix -> GetRowBytes());
#endif
		if(bmpformat == BF_ICOFILE)
		{
			const PixelArray *alp = &image[i] -> alpha;
			if(alp -> IsEmpty())
				stream.Put(0, PixelArray::GetRowBytes(1, size.cx, 4) * size.cy);
			else
			{
				PixelArray tmp;
				if(alp -> GetBPP() != 1)
				{
					tmp = PixelConvert(*alp, 1, 4);
					alp = &tmp;
				}
#if IMAGE_ORIGIN_BOTTOM
				stream.Put(alp -> Begin(), alp -> GetBytes());
#else
				for(int y = 0; i < size.cy; i++)
					stream.Put(alp -> GetUpScan(i), alp -> GetRowBytes());
#endif
			}

			int newfpos = (int)stream.GetPos();
			ASSERT(ico_fixup >= 0);
			ICONDIRENTRY ide;
			Zero(ide);
			ide.bWidth = pix -> size.cx;
			ide.bHeight = pix -> size.cy;
			ide.bColorCount = (byte)header.biClrUsed;
//			ide.wPlanes = header.biPlanes;
//			ide.wBitCount = header.biBitCount;
//			ide.bReserved = 0;
			ide.wHotSpotX = pix -> hotspot.x;
			ide.wHotSpotY = pix -> hotspot.y;
			ide.dwBytesInRes = newfpos - fpos;
			ide.dwImageOffset = fpos - ico_fixup;
			stream.Seek(ico_fixup + sizeof(ICONDIR) + i * sizeof(ICONDIRENTRY));
			stream.Put(&ide, sizeof(ide));
			stream.Seek(newfpos);
		}
	}
}

static Size GetDotSize(Size pixel_size, int xpm, int ypm)
{
	if(!xpm || !ypm)
		return Size(0, 0);
	static const double DOTS_PER_METER = 60000 / 2.54;
	return Size(fround(pixel_size.cx * DOTS_PER_METER / xpm), fround(pixel_size.cy * DOTS_PER_METER / ypm));
}

Array<AlphaArray> BmpEncoder::LoadRaw(Stream& stream, const Vector<int>& page_index)
{
	Array<AlphaArray> out;
	if(!stream.IsOpen() || page_index.IsEmpty())
		return out;
	ASSERT(stream.IsLoading());
	int count = 1;
	if(bmpformat == BF_ICOFILE)
	{
		ICONDIR id;
		if(!stream.GetAll(&id, sizeof(id)))
			return out;
		if(id.idReserved != 0 || (id.idType != 1 && id.idType != 2) || id.idCount == 0)
			return out;
		count = id.idCount;
	}
	int dirpos = (int)stream.GetPos();
	for(int i = 0; i < page_index.GetCount(); i++)
	{
		int pg = page_index[i];
		if(bmpformat == BF_ICOFILE)
		{
			stream.Seek(dirpos + minmax(pg, 0, count - 1) * sizeof(ICONDIRENTRY));
			ICONDIRENTRY ide;
			if(!stream.GetAll(&ide, sizeof(ide)))
				return out;
			if(ide.dwBytesInRes < sizeof(BITMAPINFOHEADER) || ide.dwBytesInRes > stream.GetSize()
			|| ide.dwImageOffset > (unsigned)(stream.GetSize() - ide.dwBytesInRes))
				return Array<AlphaArray>();
			stream.Seek(ide.dwImageOffset + dirpos - sizeof(ICONDIR));
		}
		else
			stream.Seek(dirpos);
		Header header;
		if(!InfoRaw(stream, header))
			return out;
		dword cmasks[3];
		const dword *cmptr = NULL;
		Vector<Color> palette;
		if(header.biBitCount <= 8)
		{
			palette.SetCount(1 << header.biBitCount);
			const RGBQUAD *q = header.palette;
			for(int i = 0; i < palette.GetCount(); i++, q++)
				palette[i] = Color(q -> rgbRed, q -> rgbGreen, q -> rgbBlue);
		}
		if(header.biCompression == 3 /* BI_BITFIELD */)
		{
			cmptr = cmasks;
			memcpy(cmasks, header.palette, sizeof(cmasks));
		}
		Size size(tabs(header.biWidth), tabs(header.biHeight));
		if(bmpformat == BF_ICOFILE)
			size.cy >>= 1;
		AlphaArray image(size, header.biBitCount == 24 ? -3 : header.biBitCount, 4, cmptr,
				bmpformat == BF_ICOFILE ? 1 : 0, 4, palette, 0);
		image.SetDotSize(GetDotSize(image.GetSize(), header.biXPelsPerMeter, header.biYPelsPerMeter));
		for(int i = 0; i < size.cy; i++)
			if(!stream.GetAll(header.biHeight >= 0 ? image.pixel.GetUpScan(i)
				: image.pixel.GetDownScan(i), image.pixel.GetRowBytes()))
				return out;
		if(bmpformat == BF_ICOFILE)
			for(int i = 0; i < size.cy; i++)
				if(!stream.GetAll(header.biHeight >= 0 ? image.alpha.GetUpScan(i)
					: image.alpha.GetDownScan(i), image.alpha.GetRowBytes()))
					return out;
		out.Add() = image;
	}
	return out;
}

Array<ImageInfo> BmpEncoder::InfoRaw(Stream& stream)
{
	Array<ImageInfo> out;
	if(bmpformat == BF_ICOFILE)
	{
		ICONDIR id;
		if(!stream.GetAll(&id, sizeof(id)))
			return out;
		if(id.idReserved != 0 || (id.idType != 1 && id.idType != 2) || id.idCount == 0)
			return out;
		int dirpos = (int)stream.GetPos();
		unsigned dirend = dirpos + id.idCount * sizeof(ICONDIRENTRY);
		if(stream.GetSize() < dirend)
			return out;
		for(int i = 0; i < id.idCount; i++)
		{
			ICONDIRENTRY ide;
			stream.Seek(dirpos + i * sizeof(ide));
			if(!stream.GetAll(&ide, sizeof(ide)))
				return out;
			unsigned imgoff = ide.dwImageOffset + dirpos - sizeof(ICONDIR);
			if(ide.dwBytesInRes < sizeof(BITMAPINFOHEADER)
			|| imgoff < dirend || imgoff > (unsigned)(stream.GetSize() - ide.dwBytesInRes))
				return out;
			stream.Seek(imgoff);
			Header header;
			if(!InfoRaw(stream, header))
				return out;
			ImageInfo& info = out.Add();
			info.size = Size(tabs(header.biWidth), tabs(header.biHeight) >> 1);
			info.bits_per_pixel = header.biBitCount;
			info.dots = GetDotSize(info.size, header.biXPelsPerMeter, header.biYPelsPerMeter);
		}
	}
	else
	{
		Header header;
		if(!InfoRaw(stream, header))
			return out;
		out.SetCount(1);
		ImageInfo& info = out[0];
		info.size = Size(tabs(header.biWidth), tabs(header.biHeight));
		info.bits_per_pixel = header.biBitCount;
		info.dots = GetDotSize(info.size, header.biXPelsPerMeter, header.biYPelsPerMeter);
	}
	return out;
}

bool BmpEncoder::InfoRaw(Stream& stream, Header& header)
{
	if(!stream.IsOpen())
		return false;
	ASSERT(stream.IsLoading());
	if(bmpformat == BF_BMPFILE)
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
		if(header.biCompression != 0 /* BI_RGB */)
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
		if(header.biCompression == 0 /* BI_RGB */)
			;
		else if(header.biCompression == 3 /* BI_BITFIELDS */)
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
