#ifndef _plugin_jpg_jpg_h_
#define _plugin_jpg_jpg_h_

#include <Draw/Draw.h>

class JPGRaster : public StreamRaster {
public:
	class Data;
	One<Data> data;

public:
	JPGRaster();
	~JPGRaster();

	virtual bool    Create();
	virtual Size    GetSize();
	virtual Info    GetInfo();
	virtual Line    GetLine(int line);
};

class JPGEncoder : public StreamRasterEncoder {
	class Data;
	One<Data> data;

public:
	JPGEncoder(int quality = 50);
	~JPGEncoder();

	JPGEncoder&  Quality(int q) { quality = q; return *this; }

	virtual int  GetPaletteCount();
	virtual void Start(Size sz);
	virtual void WriteLine(const RGBA *s);

private:
	int          quality;
};

#endif
