//////////////////////////////////////////////////////////////////////
// image.h: rastrovy obrazek, 2. iterace

#include "Draw.h"
#pragma hdrstop
#include "PixelUtil.h"

#ifdef CPU_IA32
#define I386_ASSEMBLY
#endif

#ifdef I386_ASSEMBLY
#define BLT_TIMING(routine) //RTIMING(#routine " (asm)")
#else
#define BLT_TIMING(routine) //RTIMING(#routine " (cpp)")
#endif

Image ImageTransform(const Image& src, int flags, Draw& draw)
{
	BLT_TIMING("ImageTransform");
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
	{
		PixelArray px;
#ifdef PLATFORM_WIN32
		HBITMAP sbmp = image.GetHBITMAP(draw);
		px = HBITMAPToPixelArray(sbmp, false, 0);
#endif
#ifdef PLATFORM_X11
		px = ImageToPixelArray(image);
#endif
		ImageDraw idraw(draw, out_image);
		PixelTransform(px, flags).Paint(idraw, Point(0, 0));
	}
	if(image.HasMask())
	{
		ImageMaskDraw imdraw(out_image);
		PixelTransform(MaskToPixelArray(image), flags).Paint(imdraw, Point(0, 0));
	}
	return out_image;
}

Image  ImageMirrorX(const Image& src, Draw& draw)              { return ImageTransform(src, IMAGE_TX, draw); }
Image  ImageMirrorY(const Image& src, Draw& draw)              { return ImageTransform(src, IMAGE_TY, draw); }
Image  ImageMirrorXY(const Image& src, Draw& draw)             { return ImageTransform(src, IMAGE_TXY, draw); }
Image  ImageRotateAnticlockwise(const Image& src, Draw& draw)  { return ImageTransform(src, IMAGE_TANTICLOCKWISE, draw); }
Image  ImageRotateClockwise(const Image& src, Draw& draw)      { return ImageTransform(src, IMAGE_TCLOCKWISE, draw); }

#ifdef PLATFORM_WIN32
HMETAFILE ImageToMetafile(const Image& image)
{
	if(image.IsEmpty())
		return 0;
	HDC hdc = CreateMetaFile(0);
	if(hdc == NULL)
		return 0;
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetWindowExtEx(hdc, image.GetWidth(), image.GetHeight(), 0);
	{
		Draw draw(hdc);
		Rect rc = image.GetRect();
		image.PaintMask(draw, rc, rc, Black, Null);
		image.PaintData(draw, rc, rc, image.HasMask() ? SRCINVERT : SRCCOPY);
	}
	return CloseMetaFile(hdc);
}
#endif

#ifdef PLATFORM_WIN32
HENHMETAFILE ImageToEnhMetafile(const Image& image)
{
	if(image.IsEmpty())
		return 0;
	HDC hdc = CreateEnhMetaFile(0, 0, image.GetRect(), 0);
	if(hdc == NULL)
		return 0;
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetWindowExtEx(hdc, image.GetWidth(), image.GetHeight(), 0);
	{
		Draw draw(hdc);
		Rect rc = image.GetRect();
		image.PaintMask(draw, rc, rc, Black, Null);
		image.PaintData(draw, rc, rc, image.HasMask() ? SRCINVERT : SRCCOPY);
	}
	return CloseEnhMetaFile(hdc);
}
#endif

#ifdef PLATFORM_WIN32
String ImageToMetafileData(const Image& image, bool enhanced)
{
	StringBuffer data;
	if(enhanced)
	{
		HENHMETAFILE hemf = ImageToEnhMetafile(image);
		if(hemf == 0)
			return Null; // error getting metafile
		unsigned size = GetEnhMetaFileBits(hemf, 0, 0);
		if(size > 0)
			GetEnhMetaFileBits(hemf, size, (byte *)data.SetLength(size));
		DeleteEnhMetaFile(hemf);
	}
	else
	{ // old-style meta
		HMETAFILE hmf = ImageToMetafile(image);
		if(!hmf)
			return Null; // error getting metafile
		unsigned size = GetMetaFileBitsEx(hmf, 0, 0);
		if(size > 0)
			GetMetaFileBitsEx(hmf, size, (byte *)data.SetLength(size));
		DeleteMetaFile(hmf);
	}
	return data;
}
#endif

PixelArray ImageToPixelArray(const Image& im, Draw& draw, int bpp, int alignment, const dword *cmasks, const Vector<Color>& palette)
{
	ASSERT(alignment == 4);
#ifdef PLATFORM_WIN32
	return HBITMAPToPixelArray(im.GetHBITMAP(draw), false, bpp, draw);
#endif
#ifdef PLATFORM_X11
	return DrawableToPixelArray(im.GetPixmap(), im.GetRect(), false, bpp, alignment, cmasks);
#endif
}

PixelArray MaskToPixelArray(const Image& im, int bpp, int alignment)
{
	ASSERT(alignment == 4);
#ifdef PLATFORM_WIN32
	return HBITMAPToPixelArray(im.GetMaskHBITMAP(), true, bpp);
#endif
#ifdef PLATFORM_X11
	return DrawableToPixelArray(im.GetMaskPixmap(), im.GetRect(), true, bpp, alignment);
#endif
}

/////////////////////////////////////////////////////////////////////

AlphaArray ImageToAlphaArray(const Image& im, Draw& draw,
	int bpp, int alignment, const dword *cmasks, const Vector<Color>& palette,
	int mask_bpp, int mask_align)
{
	if(im.IsEmpty())
		return Null;
	PixelArray pixel = ImageToPixelArray(im, draw, bpp, alignment, cmasks, palette), alpha;
	if(im.HasMask())
		alpha = MaskToPixelArray(im, mask_bpp, mask_align);
	return AlphaArray(pixel, alpha);
}

Image PixelArrayToImage(const PixelArray& src, Draw& draw)
{
	if(src.IsEmpty())
		return Null;
	Image image(src.GetSize());
	{
		ImageDraw idraw(draw, image);
#ifdef PLATFORM_WIN32
		if(!draw.IsDrawing() && GetDeviceCaps(draw, BITSPIXEL) > 8 && src.GetBPP() <= 8)
			PixelConvert(src, -3).Paint(idraw);
		else
			src.Paint(idraw);
#else
		src.Paint(idraw);
#endif
	}
	return image;
}

Image AlphaArrayToImage(const AlphaArray& image, Draw& draw)
{
	if(image.IsEmpty())
		return Null;
	Image out(image.GetSize());
	if(!image.IsEmpty())
	{
		{
			ImageDraw idraw(out);
			image.pixel.Paint(idraw, Point(0, 0));
		}
		if(image.HasAlpha())
		{
			ImageMaskDraw imdraw(out);
			image.alpha.Paint(imdraw, Point(0, 0));
		}
	}
	return out;
}

void ImageAlphaBlend(Image& dest, Color color, int percent, const Rect& at_least_this_part, int gamma)
{
	BLT_TIMING("ImageAlphaBlend (Image, color)");
	PixelArray image = ImageToPixelArray(dest);
	if(!image)
		return;
	PixelAlphaBlend(image, color, percent, at_least_this_part, gamma);
//	__asm int 3
	ImageDraw idraw(dest);
	image.Paint(idraw, Point(0, 0));
}
