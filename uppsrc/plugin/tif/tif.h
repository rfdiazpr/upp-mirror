#ifndef _plugin_tif_tif_h_
#define _plugin_tif_tif_h_

#include <Draw/Draw.h>

class TIFRaster : public StreamRaster {
public:
	class Data;
	One<Data> data;

public:
	TIFRaster();
	~TIFRaster();

	virtual bool    Create();
	virtual Size    GetSize();
	virtual Info    GetInfo();
	virtual Line    GetLine(int line);
	virtual const RGBA *GetPalette();
	virtual const RasterFormat *GetFormat();

private:
	bool            Init();
};

class TIFEncoder : public StreamRasterEncoder {
public:
	class Data;
	One<Data> data;

public:
	TIFEncoder(int bpp = 24);
	~TIFEncoder();

	TIFEncoder&  Bpp(int b)       { bpp = b; return *this; }

	virtual int  GetPaletteCount();
	virtual void Start(Size sz);
	virtual void WriteLineRaw(const byte *s);

private:
	int bpp;
};

// StreamRasterEncoder::SaveFile by melo vracet true / false podle toho, jestli se mu podari ulozit soubor
// RasterEncoder by mel umoznit nastavit fyzickou velikost obrazku pro ulozeni do souboru
// v Raster::Info bych zmenil flag alpha na ImageKind
// dotaz: resi se nejak zrychleni toho generatoru palety, kdyz je ten obrazek mrnavy
//        (typicky ikony 16 x 16 nebo 32 x 32 pixelu) ?

#endif
