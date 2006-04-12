#ifndef _nRaster_Raster_h_
#define _nRaster_Raster_h_

enum {
	RASTER_1,
	RASTER_2,
	RASTER_4,
	RASTER_8,
	RASTER_16,
	RASTER_24,
	RASTER_32,
	RASTER_32ALPHA,

	RASTER_MSBFIRST = 0x80,

	RASTER_MAP_R = 32,
	RASTER_SHIFT_R = 3,
	RASTER_MAP_G = 64,
	RASTER_SHIFT_G = 2,
	RASTER_MAP_B = 16,
	RASTER_SHIFT_B = 4,

	RASTER_MAP_MAX = 64
};

struct PaletteCv {
	Buffer<byte> cv;

	byte *At(int r, int b)           { return cv + (r << 10) + (b << 6); }
	byte  Get(const RGBA& b) const   { return cv[(int(b.r >> RASTER_SHIFT_R) << 10) +
	                                             (int(b.g >> RASTER_SHIFT_G)) +
	                                             (int(b.b >> RASTER_SHIFT_B) << 6)]; }
	PaletteCv()                      { cv.Alloc(RASTER_MAP_R * RASTER_MAP_G * RASTER_MAP_B); }
};

struct RasterFormat {
	byte  type;
	dword rmask, gmask, bmask, amask;

	void Set1lf()                    { type = RASTER_1; }
	void Set1mf()                    { type = RASTER_1|RASTER_MSBFIRST; }
	void Set2lf()                    { type = RASTER_2; }
	void Set2mf()                    { type = RASTER_2|RASTER_MSBFIRST; }
	void Set4lf()                    { type = RASTER_4; }
	void Set4mf()                    { type = RASTER_4|RASTER_MSBFIRST; }
	void Set8()                      { type = RASTER_8; }
	void Set16le(dword rmask, dword gmask, dword bmask);
	void Set16be(dword rmask, dword gmask, dword bmask);
	void Set24le(dword rmask, dword gmask, dword bmask);
	void Set24be(dword rmask, dword gmask, dword bmask);
	void Set32le(dword rmask, dword gmask, dword bmask, dword amask = 0);
	void Set32be(dword rmask, dword gmask, dword bmask, dword amask = 0);
	void SetRGBA();

	int  IsRGBA() const;
	int  GetByteCount(int cx) const;
	int  GetColors() const;

	void Read(RGBA *t, const byte *s, int cx, const RGBA *palette) const;
	void Write(byte *t, const RGBA *s, int cx, const PaletteCv *palcv) const;

private:
	byte  rpos, gpos, bpos, apos;
	static void TailBits(RGBA *t, const byte *src, int cx, byte and, byte shift, const RGBA *palette);
	static void TailBitsMSB1st(RGBA *t, const byte *src, int cx, byte shift1, byte and, byte shift, const RGBA *palette);
};

class Raster {
public:
	class Line {
		const RGBA *data;
		bool        free;

		friend class Raster;
	public:
		operator const RGBA *()                                    { return data; }
		const RGBA *operator~()                                    { return data; }

		Line(const RGBA *data, bool free) : data(data), free(free) {}
		Line(pick_ Line& b);
		~Line()                                                    { if(free) delete[] data; }
	};

public:
	virtual bool    Create();
	virtual Size    GetSize() = 0;
	virtual Line    GetLine(int line) = 0;
	virtual bool    IsError();
	virtual int     GetPaletteCount();
	virtual RGBA   *GetPalette();

	Line   operator[](int i)                                      { return GetLine(i); }

	Image  GetImage(int x, int y, int cx, int cy);
	Image  GetImage();

	virtual ~Raster();
};

void CreatePaletteCv(const RGBA *palette, int ncolors, PaletteCv& cv_pal);
void CreatePalette(Raster& raster, RGBA *palette, int ncolors);
void CreatePalette(Raster& raster, RGBA *palette, int ncolors, PaletteCv& cv);

class ImageRaster : public Raster {
	Image img;

public:
	virtual Size    GetSize();
	virtual Line    GetLine(int line);

	ImageRaster(const Image& img) : img(img) {}
};

class StreamRaster : public Raster {
	Stream *s;
	bool    error;

public:
	Stream& GetStream()                 { return *s; }
	bool    Open(Stream& _s);
	bool    IsError();

	void    SetError()                  { error = true; }

	Image Load(Stream& s);
	Image LoadFile(const char *fn);
	Image LoadString(const String& s);

	StreamRaster()                      { error = true; }
};

class BMPRaster : public StreamRaster {
	Size         size;
	int          row_bytes;
	Buffer<RGBA> palette;
	Buffer<byte> scanline;
	bool         topdown;
	int64        soff;
	RasterFormat fmt;

public:
	virtual bool    Create();
	virtual Size    GetSize();
	virtual Line    GetLine(int line);
};

class RasterEncoder {
	Size           size;
	RGBA          *line;
	Buffer<RGBA>   h;
	Buffer<RGBA>   palette;
	One<PaletteCv> palette_cv;

protected:
	void SetLine(RGBA *_line);

public:
	virtual int  GetPaletteCount();
	virtual void Start(Size sz) = 0;
	virtual void WriteLine();
	virtual void WriteLine(const RGBA *s);

	RGBA       *GetPalette();
	PaletteCv&  GetPaletteCv();

	operator RGBA *()                            { return line; }
	RGBA *operator~()                            { return line; }
	Size GetSize() const                         { return size; }

	void Create(Size sz);
	void Create(int cx, int cy)                  { Create(Size(cx, cy)); }
	void CreatePalette(Raster& raster, bool cv_all = true);
	void Create(Size sz, Raster& pal_raster, bool cv_all = true);
	void Create(int cx, int cy, Raster& pal_raster, bool cv_all = true);

	virtual ~RasterEncoder();
};

class ImageEncoder : public RasterEncoder {
	int         ii;
	ImageBuffer ib;

public:
	virtual void Start(Size sz);
	virtual void WriteLine();

	Size GetSize() const                { return ib.GetSize(); }
	int  GetWidth() const               { return GetSize().cx; }
	int  GetHeight() const              { return GetSize().cy; }

	operator Image()                    { return Image(ib); }
};

class StreamRasterEncoder : public RasterEncoder {
	Stream *s;

public:
	Stream& GetStream()                 { return *s; }
	void    SetStream(Stream& _s)       { s = &_s; }

	void    Save(Stream& s, Raster& raster);
	void    SaveFile(const char *fn, Raster& raster);
	String  SaveString(Raster& raster);

	void    Save(Stream& s, const Image& img);
	void    SaveFile(const char *fn, const Image& img);
	String  SaveString(const Image& img);
};

class BMPEncoder : public StreamRasterEncoder {
	int          row_bytes;
	int          linei;
	int64        soff;
	RasterFormat fmt;
	Buffer<byte> scanline;

public:
	virtual int  GetPaletteCount();
	virtual void Start(Size sz);
	virtual void WriteLine(const RGBA *s);

	int       bpp;
	bool      grayscale;
	Size      dot_size;

	BMPEncoder()                        { bpp = 24; grayscale = false; dot_size = Size(0, 0); }
};

#endif
