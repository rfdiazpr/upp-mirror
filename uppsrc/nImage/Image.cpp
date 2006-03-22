#include "Image.h"

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

Size Image::GetSize() const
{
	return data ? data->buffer.GetSize() : Size(0, 0);
}

Point Image::GetHotSpot() const
{
	return data ? data->buffer.GetHotSpot() : Null;
}

void Image::Paint(Draw& w, int x, int y, byte const_alpha, Color c)
{
	if(data)
		data->Paint(w, x, y, const_alpha, c);
}

Image::Image(const Image& img)
{
	data = img.data;
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
