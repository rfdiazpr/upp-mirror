#include "Draw.h"

struct cDrawRasterData : DataDrawer {
	int                cx;
	StringStream       ss;
	One<StreamRaster>  raster;
	RescaleImage       si;

	virtual void Open(const String& data, int cx, int cy);
	virtual Image Render(int lines);
};

void cDrawRasterData::Open(const String& data, int _cx, int cy)
{
	cx = _cx;
	ss.Open(data);
	raster = StreamRaster::OpenAny(ss);
	if(raster)
		si.Create(Size(cx, cy), *raster, raster->GetSize());
}

Image cDrawRasterData::Render(int lines)
{
	RTIMING("Render");
	ImageBuffer ib(cx, lines);
	for(int y = 0; y < lines; y++)
		si.Get(ib[y]);
	return ib;
}

INITBLOCK
{
	DataDrawer::Register<cDrawRasterData>("image_data");
};

void DrawRasterData(Draw& w, int x, int y, int cx, int cy, const String& data)
{
	w.DrawData(x, y, cx, cy, data, "image_data");
}
