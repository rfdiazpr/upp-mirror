#include "Image.h"

int ImageBuffer::GetKind()
{
	if(kind == KIND_UNKNOWN) {
		bool a255 = false;
		bool a0 = false;
		const RGBA *b = pixels;
		for(int i = 0; i < sz.cy; i++) {
			for(const RGBA *q = b, const RGBA *e = b + sz.cx; q < e; q++)
				if(q->a == 0)
					a0 = true;
				else
				if(q->a == 255)
					a255 = true;
				else {
					kind = IMAGE_ALPHA;
					return kind;
				}
			b += sz.cy;
		}
		kind = a255 ? a0 ? IMAGE_MASK : IMAGE_OPAUE : IMAGE_EMPTY;
	}
	return kind;
}

ImageBuffer::ImageBuffer(Size sz, int _kind)
{
	pixels.Alloc(sz.cx * sz.cy);
	kind = _kind;
}

ImageBuffer::ImageBuffer(pick_ ImageBuffer& b, int kind)
{
	pixels = b.pixels;
	size = b.size;
	kind = newkind;
}

ImageBuffer::ImageBuffer(Image& img, int kind)
{

	img = Image();
}

Image& Image::operator=(const Image& img)
{
	Data *d = data;
	data = img.data;
	data->Retain();
	d->R
}

Image::Image()
{

}

Image::Image(pick_ ImageBuffer& b)
{

}
