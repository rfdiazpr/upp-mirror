#include "Draw.h"

int ImageBuffer::GetKind() const
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
#ifdef _DEBUG
	RGBA *s = pixels;
	RGBA *e = pixels + GetLength();
	byte  a = 0;
	while(s < e) {
		s->a = a;
		a = ~a;
		s->r = 255;
		s->g = s->b = 0;
		s++;
	}
#endif
	kind = IMAGE_UNKNOWN;
	hotspot = Point(0, 0);
	dots = Size(0, 0);
}

void ImageBuffer::Set(Image& img)
{
	if(img.data->refcount == 1) {
		size = img.GetSize();
		kind = IMAGE_UNKNOWN;
		hotspot = img.GetHotSpot();
		dots = img.GetDots();
		pixels = img.data->buffer.pixels;
		img.Clear();
	}
	else {
		Create(img.GetSize());
		memcpy(pixels, img.data->buffer.pixels, GetLength() * sizeof(RGBA));
		img.Clear();
	}
}

void ImageBuffer::operator=(Image& img)
{
	Clear();
	Set(img);
}

void ImageBuffer::operator=(ImageBuffer& img)
{
	Clear();
	Image m = img;
	Set(m);
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
	dots = b.dots;
	pixels = b.pixels;
	hotspot = b.hotspot;
}

void Image::Set(ImageBuffer& b)
{
	if(b.GetWidth() == 0 || b.GetHeight() == 0)
		data = NULL;
	else
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
	return data ? data->buffer.GetHotSpot() : Point(0, 0);
}

Size Image::GetDots() const
{
	return data ? data->buffer.GetDots() : Size(0, 0);
}

int Image::GetKind() const
{
	return data ? data->buffer.GetKind() : IMAGE_EMPTY;
}

void Image::PaintImage(Draw& w, int x, int y, const Rect& src, Color c) const
{
	if(data)
		data->Paint(w, x, y, src, c);
}

void Image::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	Size sz = GetSize();
	Point p = GetHotSpot();
	Size dots = GetDots();
	s % sz % p % dots;
	int len = sz.cx * sz.cy;
	if(s.IsLoading())
		if(len) {
			ImageBuffer b(sz);
			if(!s.GetAll(~b, len * sizeof(RGBA)))
				s.SetError();
			b.SetDots(dots);
			b.SetHotSpot(p);
			*this = b;
		}
		else
			Clear();
	else
		s.Put(~*this, len * sizeof(RGBA));
}

Image::Image(const Image& img)
{
	data = img.data;
	if(data)
		data->Retain();
}

Image::Image(const Image& (*fn)())
{
	data = NULL;
	*this = (*fn)();
}

Image::Image(const Value& src)
{
	data = NULL;
	if(!IsNull(src))
		*this = RawValue<Image>::Extract(src);
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
	sz.cx = PeekIL(init.info + 1);
	sz.cy = PeekIL(init.info + 5);
	ImageBuffer b(sz);
	int i = 0;
	while(i < init.scan_count) {
		UnpackRLE(b[i], (const byte *)init.scans[i], sz.cx);
		i++;
	}
	while(i < sz.cy)
		memset(b[i++], 0, sizeof(RGBA) * sz.cx);
	b.SetHotSpot(Point(PeekIL(init.info + 9), PeekIL(init.info + 13)));
	Set(b);
}

String Image::ToString() const
{
	return String("Image ").Cat() << GetSize();
}

StaticCriticalSection Image::Data::ResLock;
Link<Image::Data>     Image::Data::ResData[1];
int                   Image::Data::ResCount;

Image::Data::Data(ImageBuffer& b)
:	buffer(b)
{
	paintcount = 0;
	refcount = 1;
	INTERLOCKED {
		static int64 gserial;
		serial = ++gserial;
	}
	SysInit();
}

Image::Data::~Data()
{
	INTERLOCKED_(ResLock) {
		SysRelease();
		Unlink();
	}
}

typedef const Image& (*ImgFn)();

static StaticCriticalSection sImgMapLock;

static VectorMap<String, ImgFn>& sImgMap()
{
	static VectorMap<String, ImgFn> x;
	return x;
}

void RegisterImageName__(const char *name, ImgFn fn)
{
	INTERLOCKED_(sImgMapLock)
		sImgMap().GetAdd(name) = fn;
}

Image GetImlImage(const char *name)
{
	ImgFn fn;
	INTERLOCKED_(sImgMapLock)
		fn = sImgMap().Get(name, NULL);
	return fn ? (*fn)() : Image();
}

int GetImlImageCount()
{
	int q;
	INTERLOCKED_(sImgMapLock)
		q = sImgMap().GetCount();
	return q;
}

Image GetImlImage(int i)
{
	Image m;
	INTERLOCKED_(sImgMapLock)
		m = sImgMap()[i];
	return m;
}

String GetImlImageName(int i)
{
	String s;
	INTERLOCKED_(sImgMapLock)
		s = sImgMap().GetKey(i);
	return s;
}

String StoreImageAsString(const Image& img)
{
	if(img.GetKind() == IMAGE_EMPTY)
		return Null;
	int type = img.GetKind() == IMAGE_OPAQUE ? 3 : 4;
	StringStream ss;
	ss.Put(type);
	Size sz = img.GetSize();
	ss.Put16le(sz.cx);
	ss.Put16le(sz.cy);
	Point p = img.GetHotSpot();
	ss.Put16le(p.x);
	ss.Put16le(p.y);
	Size dots = img.GetDots();
	ss.Put16le(dots.cx);
	ss.Put16le(dots.cy);
	const RGBA *s = img;
	const RGBA *e = s + img.GetLength();
	Buffer<byte> b(type * img.GetLength());
	s = img;
	byte *t = b;
	if(type == 3)
		while(s < e) {
			*t++ = s->r;
			*t++ = s->g;
			*t++ = s->b;
			s++;
		}
	else
		while(s < e) {
			*t++ = s->r;
			*t++ = s->g;
			*t++ = s->b;
			*t++ = s->a;
			s++;
		}
	MemReadStream m(b, type * img.GetLength());
	ZCompress(ss, m);
	return ss;
}

Image  LoadImageFromString(const String& src)
{
	if(src.GetLength() < 13)
		return Null;
	StringStream ss(src);
	int type = ss.Get();
	Size sz;
	sz.cx = ss.Get16le();
	sz.cy = ss.Get16le();
	if(sz.cx < 0 || sz.cy < 0)
		return Null;
	Point p;
	p.x = ss.Get16le();
	p.y = ss.Get16le();
	if(p.x < 0 || p.y < 0)
		return Null;
	Size dots;
	dots.cx = ss.Get16le();
	dots.cy = ss.Get16le();
	if(dots.cx < 0 || dots.cy < 0)
		return Null;
	StringStream out;
	ZDecompress(out, ss);
	String data = out;
	if(data.GetLength() != type * sz.cx * sz.cy)
		return Image();
	ImageBuffer ib(sz);
	ib.SetHotSpot(p);
	ib.SetDots(dots);
	RGBA *t = ib;
	const RGBA *e = t + ib.GetLength();
	const byte *s = data;
	if(type == 3)
		while(t < e) {
			t->r = *s++;
			t->g = *s++;
			t->b = *s++;
			t->a = 255;
			t++;
		}
	else
	if(type == 4)
		while(t < e) {
			t->r = *s++;
			t->g = *s++;
			t->b = *s++;
			t->a = *s++;
			t++;
		}
	else
		return Image();
	return ib;
}

Size GetImageStringSize(const String& src)
{
	if(src.GetLength() < 13)
		return Size(0, 0);
	StringStream ss(src);
	ss.Get();
	Size sz;
	sz.cx = ss.Get16le();
	sz.cy = ss.Get16le();
	return sz;
}

Size GetImageStringDots(const String& src)
{
	if(src.GetLength() < 13)
		return Size(0, 0);
	StringStream ss(src);
	ss.SeekCur(9);
	Size sz;
	sz.cx = ss.Get16le();
	sz.cy = ss.Get16le();
	return sz;
}
