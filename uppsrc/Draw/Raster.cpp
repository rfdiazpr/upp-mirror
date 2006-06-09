#include "Draw.h"

int Raster::GetPageCount()
{
	return 1;
}

void Raster::SeekPage(int ii)
{
	ASSERT(ii == 0);
}

Raster::Line::Line(pick_ Line& b)
{
	data = b.data;
	free = b.free;
	const_cast<Line *>(&b)->free = false;
#ifdef _DEBUG
	const_cast<Line *>(&b)->data = NULL;
#endif
}

Raster::Info Raster::GetInfo()
{
	Info f;
	f.bpp = 32;
	f.colors = 256*256*256;
	f.dots = Size(0, 0);
	f.hotspot = Point(0, 0);
	f.kind = IMAGE_ALPHA;
	return f;
}

bool Raster::Create() { return true; }

bool Raster::IsError() { return false; }

int   Raster::GetPaletteCount() { return 0; }

RGBA *Raster::GetPalette() { return NULL; }

Image Raster::GetImage(int x, int y, int cx, int cy)
{
	Size size = GetSize();
	y = minmax(y, 0, size.cy);
	int yy = minmax(y + cy, y, size.cy);
	x = minmax(x, 0, size.cx);
	cx = minmax(x + cx, x, size.cx) - x;
	ImageBuffer b(cx, yy - y);
	RGBA* t = b;
	while(y < yy) {
		memcpy(t, ~GetLine(y) + x, cx * sizeof(RGBA));
		t += cx;
		y++;
	}
	Info f = GetInfo();
	b.SetHotSpot(f.hotspot - Point(x, y));
	if(size.cx && size.cy)
		b.SetDots(Size(f.dots.cx * cx / size.cx, f.dots.cy * cy / size.cy));
	return IsError() ? Image() : Image(b);
}

Image Raster::GetImage()
{
	Size sz = GetSize();
	return GetImage(0, 0, sz.cx, sz.cy);
}

Raster::~Raster() {}

Raster::Line ImageRaster::GetLine(int line)
{
	return Line(img[line], false);
}

Size ImageRaster::GetSize()
{
	return img.GetSize();
}

Raster::Info ImageRaster::GetInfo()
{
	Raster::Info f = Raster::GetInfo();
	f.dots = img.GetDots();
	f.hotspot = img.GetHotSpot();
	f.kind = img.GetKind();
	return f;
}

bool StreamRaster::Open(Stream& _s)
{
	s = &_s;
	error = !Create();
	return !error;
}

bool StreamRaster::IsError()
{
	return error || !s || s->IsError();
}

Image StreamRaster::Load(Stream& s)
{
	if(Open(s)) {
		Image img = GetImage();
		if(!IsError())
			return img;
	}
	return Image();
}

Image StreamRaster::LoadFile(const char *fn)
{
	FileIn in(fn);
	return in ? Load(in) : Image();
}

Image StreamRaster::LoadString(const String& s)
{
	StringStream ss(s);
	return Load(ss);
}

static StaticCriticalSection sAnyRaster;

Vector<void *>& StreamRaster::Map()
{
	static Vector<void *> x;
	return x;
}

void StreamRaster::AddFormat(RasterFactory factory)
{
	INTERLOCKED_(sAnyRaster)
		Map().Add((void *)factory);
}

One<StreamRaster> StreamRaster::OpenAny(Stream& s)
{
	INTERLOCKED_(sAnyRaster)
		for(int i = 0; i < Map().GetCount(); i++) {
			int64 p = s.GetPos();
			One<StreamRaster> raster = (*RasterFactory(Map()[i]))();
			s.ClearError();
			if(raster->Open(s))
				return raster;
			s.Seek(p);
		}
	return NULL;
}

Image StreamRaster::LoadAny(Stream& s)
{
	One<StreamRaster> r = OpenAny(s);
	return r ? r->GetImage() : Image();
}

Image StreamRaster::LoadFileAny(const char *fn)
{
	FileIn in(fn);
	return LoadAny(in);
}

Image StreamRaster::LoadStringAny(const String& s)
{
	StringStream ss(s);
	return LoadAny(ss);
}
