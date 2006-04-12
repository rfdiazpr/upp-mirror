#include "Image.h"

int ImageBuffer::GetKind()
{
	if(kind == IMAGE_UNKNOWN) {
		bool a255 = false;
		bool a0 = false;
		const RGBA *s = pixels;
		const RGBA *e = s + GetLength();
		while(s < e) {
			if(s->a == 0)
				a0 = true;
			else
			if(s->a == 255)
				a255 = true;
			else {
				kind = IMAGE_ALPHA;
				return IMAGE_ALPHA;
			}
			s++;
		}
		kind = a255 ? a0 ? IMAGE_MASK : IMAGE_OPAQUE : IMAGE_EMPTY;
	}
	return kind;
}

void ImageBuffer::Create(int cx, int cy)
{
	size.cx = cx;
	size.cy = cy;
	pixels.Alloc(GetLength());
	kind = IMAGE_UNKNOWN;
	hotspot = Point(0, 0);
}

void ImageBuffer::Set(Image& img)
{
	if(img.data->refcount == 1) {
		size = img.GetSize();
		kind = IMAGE_UNKNOWN;
		hotspot = img.GetHotSpot();
		pixels = img.data->buffer.pixels;
		img.Clear();
	}
	else {
		Create(img.GetSize());
		memcpy(pixels, img.data->buffer.pixels, GetLength() * sizeof(RGBA));
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

ImageBuffer::ImageBuffer(ImageBuffer& b)
{
	kind = b.kind;
	size = b.size;
	pixels = b.pixels;
	hotspot = b.hotspot;
}

void Image::Set(ImageBuffer& b)
{
	data = new Data(b);
}

void Image::Clear()
{
	if(data)
		data->Release();
	data = NULL;
}

Image& Image::operator=(ImageBuffer& img)
{
	if(data)
		data->Release();
	Set(img);
	return *this;
}

Image& Image::operator=(const Image& img)
{
	Data *d = data;
	data = img.data;
	if(data)
		data->Retain();
	if(d)
		d->Release();
	return *this;
}

const RGBA* Image::operator~() const
{
	return data ? ~data->buffer : NULL;
}

Image::operator const RGBA*() const
{
	return data ? ~data->buffer : NULL;
}

const RGBA* Image::operator[](int i) const
{
	ASSERT(data);
	return data->buffer[i];
}

Size Image::GetSize() const
{
	return data ? data->buffer.GetSize() : Size(0, 0);
}

int Image::GetLength() const
{
	return data ? data->buffer.GetLength() : 0;
}

Point Image::GetHotSpot() const
{
	return data ? data->buffer.GetHotSpot() : Null;
}

void Image::Paint(Draw& w, int x, int y, byte const_alpha, Color c) const
{
	if(data)
		data->Paint(w, x, y, const_alpha, c);
}

Image::Image(const Image& img)
{
	data = img.data;
	if(data)
		data->Retain();
}

Image::Image(ImageBuffer& b)
{
	Set(b);
}

Image::~Image()
{
	if(data)
		data->Release();
}

Image::Image(const Init& init)
{
	ASSERT(init.info[0] >= 1);
	Size sz;
	sz.cx = PeekIW(init.info + 1);
	sz.cy = PeekIW(init.info + 5);
	ImageBuffer b(sz);
	int i = 0;
	while(i < init.scan_count) {
		UnpackRLE(b[i], (const byte *)init.scans[i], sz.cx);
		i++;
	}
	while(i < sz.cy)
		memset(b[i++], 0, sizeof(RGBA) * sz.cx);
	Set(b);
}
