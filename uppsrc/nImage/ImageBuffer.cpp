#include "Image.h"

int ImageBuffer::GetKind()
{
	if(kind == IMAGE_UNKNOWN) {
		bool a255 = false;
		bool a0 = false;
		const RGBA *b = pixels;
		for(int i = 0; i < size.cy; i++) {
			for(const RGBA *q = b, *e = b + size.cx; q < e; q++)
				if(q->a == 0)
					a0 = true;
				else
				if(q->a == 255)
					a255 = true;
				else {
					kind = IMAGE_ALPHA;
					return kind;
				}
			b += size.cy;
		}
		kind = a255 ? a0 ? IMAGE_MASK : IMAGE_OPAQUE : IMAGE_EMPTY;
	}
	return kind;
}

void ImageBuffer::Init(Size sz)
{
	size = sz;
	pixels.Alloc(sz.cx * sz.cy);
	kind = IMAGE_UNKNOWN;
	hotspot = Point(0, 0);
}

ImageBuffer::ImageBuffer(Size sz)
{
	Init(sz);
}


ImageBuffer::ImageBuffer(int cx, int cy)
{
	Init(Size(cx, cy));
}

void ImageBuffer::Set(Image& img)
{
	size = img.GetSize();
	if(img.data->refcount == 1) {
		pixels = img.data->buffer.pixels;
		img.Clear();
	}
	else {
		Init(img.GetSize());
		memcpy(pixels, img.data->buffer.pixels, size.cx * size.cy * sizeof(RGBA));
		img.Clear();
	}
}

ImageBuffer::ImageBuffer(Image& img)
{
	Set(img);
}

ImageBuffer::ImageBuffer(ImageDraw& w)
{
	Image img = w;
	Set(img);
}
