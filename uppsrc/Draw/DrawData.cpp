#include "Draw.h"

VectorMap<String, void *>& DataDrawer::Map()
{
	static VectorMap<String, void *> x;
	return x;
}

static StaticCriticalSection sDataDrawer;

void DataDrawer::AddFormat(const char *id, Factory factory)
{
	INTERLOCKED_(sDataDrawer)
		Map().Add(id, (void *)factory);
}

One<DataDrawer> DataDrawer::Create(const String& id)
{
	INTERLOCKED_(sDataDrawer) {
		Factory q = (Factory) Map().Get(id, NULL);
		if(q)
			return (*q)();
	}
	return NULL;
}

bool IsEqColumn(const Image& m, int x, RGBA c)
{
	RTIMING("IsEqColumn");
	Size sz = m.GetSize();
	const RGBA *s = ~m + x;
	if(c.a != 255)
		return false;
	while(sz.cy > 1) {
		s += sz.cx;
		if(*s != c)
			return false;
		sz.cy--;
	}
	return true;
}

void Draw::DrawDataOp(int x, int y, int cx, int cy, const String& data, const char *id)
{
	One<DataDrawer> dd = DataDrawer::Create(id);
	if(dd) {
		dd->Open(data, cx, cy);
		if(cx * cy > 2048*2048) {
			int yy = 0;
			int rcd = 0;
			int imd = 0;
			while(yy < cy) {
				int ccy = min(cy - yy, 16);
				ImageBuffer ib(cx, ccy);
				dd->Render(ib);
				Image m = ib;
				int xi = 0;
				while(xi < cx) {
					int xi0 = xi;
					RGBA c = m[0][xi0];
					while(IsEqColumn(m, xi, c) && xi < cx)
						xi++;
					if(xi - xi0 >= 16) {
						DrawRect(x + xi0, y + yy, xi - xi0, ccy, Color(c.r, c.g, c.b)); //TODO - Color RGBA conversion
						rcd += xi - xi0;
						xi0 = xi;
					}
					while(!IsEqColumn(m, xi, m[0][xi]) && xi < cx)
						xi++;
					if(xi - xi0 > 0) {
						DrawImage(x + xi0, y + yy, Crop(m, xi0, 0, xi - xi0, ccy));
						imd += xi - xi0;
					}
				}
				yy += ccy;
			}
			// TODO - maybe "fallback" for case where there are not much "rects" ?
		}
		else {
			ImageBuffer m(cx, cy);
			dd->Render(m);
			DrawImage(x, y, m);
		}
	}
}

DataDrawer::~DataDrawer() {}
