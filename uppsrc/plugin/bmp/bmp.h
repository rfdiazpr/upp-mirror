#ifndef _plugin_bmp_bmp_h_
#define _plugin_bmp_bmp_h_

#include <Draw/Draw.h>

class BMPRaster : public StreamRaster {
	Size             size;
	int              row_bytes;
	Buffer<RGBA>     palette;
	Buffer<byte>     scanline;
	bool             topdown;
	int64            soff;
	RasterFormat     fmt;
	Raster::Info     info;
	One<ImageRaster> rle;
	bool             file;

public:
	virtual bool    Create();
	virtual Info    GetInfo();
	virtual Size    GetSize();
	virtual Line    GetLine(int line);
	virtual int     GetPaletteCount();
	virtual RGBA   *GetPalette();
	virtual const RasterFormat *GetFormat();

	BMPRaster()     { file = true; }
	~BMPRaster();
};

class BMPEncoder : public StreamRasterEncoder {
	int          row_bytes;
	int          linei;
	int64        soff;
	Buffer<byte> scanline;
	int          linebytes;

	int       bpp;
	bool      grayscale;

public:
	virtual int  GetPaletteCount();
	virtual void Start(Size sz);
	virtual void WriteLineRaw(const byte *data);

	BMPEncoder& Bpp(int _bpp)                  { bpp = _bpp; return *this; }
	BMPEncoder& Mono(bool gs = true)           { grayscale = gs; return *this; }

	BMPEncoder(int _bpp = 24, bool gs = false) { bpp = _bpp; grayscale = gs; }
};

#endif
