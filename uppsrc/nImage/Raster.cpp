#include "Image.h"

Raster::Line::Line(pick_ Line& b)
{
	data = b.data;
	free = b.free;
	const_cast<Line *>(&b)->free = false;
#ifdef _DEBUG
	const_cast<Line *>(&b)->data = NULL;
#endif
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
	return b;
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

void RasterEncoder::SetLine(RGBA *_line)
{
	line = _line;
	h.Clear();
}

void RasterEncoder::WriteLine()
{
	WriteLine(line);
}

void RasterEncoder::WriteLine(const RGBA *s)
{
	memcpy(line, s, size.cx * sizeof(s));
	WriteLine();
}

void RasterEncoder::Create(Size sz)
{
	size = sz;
	h.Alloc(sz.cx);
	line = h;
	Start(sz);
}

void RasterEncoder::CreatePalette(Raster& raster, bool cv_all)
{
	int n = GetPaletteCount();
	if(n) {
		int nn = abs(n);
		if(raster.GetPaletteCount() == nn) {
			memcpy(GetPalette(), raster.GetPalette(), nn * sizeof(RGBA));
			CreatePaletteCv(GetPalette(), nn, GetPaletteCv());
		}
		else {
			if(n > 0 && !cv_all)
				::CreatePalette(raster, GetPalette(), nn, GetPaletteCv());
			else {
				::CreatePalette(raster, GetPalette(), nn);
				CreatePaletteCv(GetPalette(), nn, GetPaletteCv());
			}
		}
	}
}

void RasterEncoder::Create(Size sz, Raster& pal_raster, bool cv_all)
{
	CreatePalette(pal_raster, cv_all);
	Create(sz);
}

void RasterEncoder::Create(int cx, int cy, Raster& pal_raster, bool cv_all)
{
	CreatePalette(pal_raster, cv_all);
	Create(cx, cy);
}

int RasterEncoder::GetPaletteCount()
{
	return 0;
}

RGBA *RasterEncoder::GetPalette()
{
	if(!palette)
		palette.Alloc(256);
	return palette;
}

PaletteCv& RasterEncoder::GetPaletteCv()
{
	if(!palette_cv)
		palette_cv.Create<PaletteCv>();
	return *palette_cv;
}

RasterEncoder::~RasterEncoder() {}

void ImageEncoder::Start(Size sz)
{
	ii = 0;
	ib.Create(sz);
	if(sz.cy)
		SetLine(ib[0]);
}

void ImageEncoder::WriteLine()
{
	if(++ii < GetHeight())
		SetLine(ib[ii]);
}

void StreamRasterEncoder::Save(Stream& s, Raster& raster)
{
	SetStream(s);
	Size sz = raster.GetSize();
	Create(sz, raster, false);
	for(int i = 0; i < sz.cy; i++)
		WriteLine(raster[i]);
}

void StreamRasterEncoder::SaveFile(const char *fn, Raster& raster)
{
	FileOut out(fn);
	Save(out, raster);
}

String StreamRasterEncoder::SaveString(Raster& raster)
{
	StringStream ss;
	Save(ss, raster);
	return ss;
}

void StreamRasterEncoder::Save(Stream& s, const Image& img)
{
	ImageRaster r(img);
	Save(s, r);
}

void StreamRasterEncoder::SaveFile(const char *fn, const Image& img)
{
	ImageRaster r(img);
	SaveFile(fn, r);
}

String StreamRasterEncoder::SaveString(const Image& img)
{
	ImageRaster r(img);
	return SaveString(r);
}
