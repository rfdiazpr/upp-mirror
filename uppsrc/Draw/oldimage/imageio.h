//////////////////////////////////////////////////////////////////////
// imageio.h: image input/output.

#ifndef __Graphics_ImageIO__
#define __Graphics_ImageIO__

//////////////////////////////////////////////////////////////////////
// low-level blitter routines

bool MMXDetect();
void MMXClear();

void BltXlatB(byte *dest, const byte *src, unsigned count, const byte *table);
void BltXlatRgb(byte *dest, const byte *src, unsigned count, const byte *table);
void BltStatB(const byte *p, unsigned count, dword *stat);
void BltStatRGB(const byte *p, unsigned count, dword *stat);
void BltSwapRGB(byte *d, const byte *s, unsigned count);
void BltSwapRGBA(byte *bgr, byte *mask, const byte *src, unsigned count);
void BltSwapRGB4(byte *bgr, const byte *src, unsigned count);
void BltExpand3(byte *dest, const byte *src, unsigned count);
void BltExpand4(byte *dest, const byte *src, unsigned count);
void BltAverage(byte *p, unsigned count, const byte *r, const byte *g, const byte *b);
void BltAlphaBlend24(byte *dest, const byte *src, unsigned count, unsigned alpha);
void Blt2to1(byte *dest, const byte *src, unsigned count);
void Blt4to1(byte *dest, const byte *src, unsigned count);
void Blt8to1(byte *dest, const byte *src, unsigned count);
void Blt1to4(byte *dest, const byte *src, unsigned count);
void Blt2to4(byte *dest, const byte *src, unsigned count);
void Blt8to4(byte *dest, const byte *src, unsigned count);
void Blt1to8(byte *dest, const byte *src, unsigned count);
void Blt2to8(byte *dest, const byte *src, unsigned count);
void Blt3to8(byte *dest, const byte *src, unsigned count);
void Blt4to8(byte *dest, const byte *src, unsigned count);
void Blt5to8(byte *dest, const byte *src, unsigned count);
void Blt6to8(byte *dest, const byte *src, unsigned count);
void Blt7to8(byte *dest, const byte *src, unsigned count);
void BltMask8(byte *dest, const byte *mask, unsigned count, byte mask_value);
void BltBuildMaskRgb(byte *dest, const byte *src, unsigned count, const byte rRgGbB[6]);
void BltKillMask1(byte *dest, const byte *mask, unsigned count);
void BltKillMask3(byte *dest, const byte *mask, unsigned count);
void BltInvert(byte *dest, unsigned count);

typedef void (*BltRop)(byte *dest, const byte *src, unsigned bytes);

void BltRopCopy(byte *dest, const byte *src, unsigned bytes);
void BltRopXor(byte *dest, const byte *src, unsigned bytes);
void BltRopAnd(byte *dest, const byte *src, unsigned bytes);

//////////////////////////////////////////////////////////////////////
// RawImage::

class RawImage
{
public:
	RawImage();
	RawImage(const RawImage& pm, int);
	RawImage(int cx, int cy, int bpp)                         { Create(cx, cy, bpp); }
	RawImage(Size size, int bpp)                              { Create(size, bpp); }

	static RawImage Mono(Size size)                           { return Mono(size.cx, size.cy); }
	static RawImage Mono(int cx, int cy);

	bool            IsEmpty() const                           { return size.cx <= 0 || size.cy <= 0; }
	operator bool () const                                    { return !IsEmpty(); }

	void            Clear();
	void            Create(int cx, int cy, int bpp);
	void            Create(Size size, int bpp)                { Create(size.cx, size.cy, bpp); }

	Size            GetSize() const                           { return size; }
	int             GetBPP() const                            { return bpp; }
	Rect            GetRect() const                           { return size; }
	int             GetRowBytes() const                       { return row_bytes; }
	int             GetRowBytes(int bpp) const                { return GetRowBytes(bpp, size.cx); }
	static int      GetRowBytes(int bpp, int cx)              { return ((bpp * cx + 31) >> 3) & -4; }
	int             GetBytes() const                          { return data.GetCount(); }

	const byte     *GetUpScan(int y) const                    { return data.GetIter(row_bytes * y); }
	byte           *GetUpScan(int y)                          { return data.GetIter(row_bytes * y); }

	const byte     *GetDownScan(int y) const                  { return data.GetIter(row_bytes * (size.cy - 1 - y)); }
	byte           *GetDownScan(int y)                        { return data.GetIter(row_bytes * (size.cy - 1 - y)); }

	void            Paint(Draw& draw) const                   { Paint(draw, GetRect(), GetRect(), SRCCOPY); }
	void            Paint(Draw& draw, Point pos) const        { Paint(draw, GetRect(), Rect(pos, size), SRCCOPY); }
	void            Paint(Draw& draw, const Rect& dest) const { Paint(draw, GetRect(), dest, SRCCOPY); }
	void            Paint(Draw& draw, const Rect& src, const Rect& dest) const { Paint(draw, src, dest, SRCCOPY); }

#ifdef PLATFORM_WIN32
	void            Paint(HDC hdc, int rop) const             { Paint(hdc, GetRect(), GetRect(), rop); }
	void            Paint(HDC hdc, Point pos, int rop) const  { Paint(hdc, GetRect(), Rect(pos, size), rop); }
	void            Paint(HDC hdc, const Rect& dest, int rop) const { Paint(hdc, GetRect(), dest, rop); }
	void            Paint(HDC hdc, const Rect& src, const Rect& dest, int rop) const;
#endif

	void            Convert(int to_bpp);
	void            OptimizePalette();
	void            Set(const Rect& dest, byte value);
	void            Set(byte value)  { Set(GetRect(), value); }
	void            Set(const Rect& dest, Color color);
	void            Set(Color color) { Set(GetRect(), color); }
	Point           ClipMove(Rect& dest, Point src, Size src_size) const;

#ifdef PLATFORM_WIN32
	struct Header : public BITMAPINFOHEADER
	{
		RGBQUAD palette[256];

		operator const BITMAPINFO * () const { return reinterpret_cast<const BITMAPINFO *>(this); }
		operator BITMAPINFO * ()             { return reinterpret_cast<BITMAPINFO *>(this); }
	};

	bool            SetHeader(Header& header) const;
#endif//PLATFORM_WIN32

public:
	Vector<byte>    data;
	Vector<Color>   palette;
	byte            bpp;
	Size            size;
	int             row_bytes;
};

template class DeepCopyOption<RawImage>;
template class Moveable<RawImage>;

RawImage            GetConvert(const RawImage& image, int to_bpp);
RawImage            GetTransform(const RawImage& image, int flags);
RawImage            GetCrop(const RawImage& image, const Rect& rc);
RawImage            GetMask(const RawImage& image, Color color, int tolerance = 0);
void                Transform(RawImage& image, int flags);
void                InvertMask(RawImage& image);
void                KillMask(RawImage& image, const RawImage& mask);
void                AlphaBlend(RawImage& image, Color color, int percent, const Rect& min_area, double gamma = 2.5);
inline void         AlphaBlend(RawImage& image, Color color, int percent, double gamma = 2.5) { AlphaBlend(image, color, percent, image.GetRect(), gamma); }
void                AlphaBlend(RawImage& image, const Rect& dest, const RawImage& with_image, Point src_pos, int alpha = 50);
void                CreatePalette(RawImage& out_image, const RawImage& image, int out_bpp = 8, int colors = 256, const Color *palette = 0);
void                Copy(RawImage& dest, const Rect& drect, const RawImage& src, Point src_pos, BltRop rop = BltRopCopy);
void                Copy(RawImage& dest, const Rect& drect, const RawImage& src, const Rect& srect, BltRop rop = BltRopCopy);
void                CopyAntiAlias(RawImage& dest, const Rect& drect, const RawImage& src, const Rect& srect, const Rect& clip = Null);
void                CopyAntiAliasMask(RawImage& dest, const Rect& drect, const RawImage& src, const RawImage& smask, const Rect& srect, const Rect& clip = Null);
void                CopyAntiAliasMaskOut(RawImage& dest, RawImage& dmask, const Rect& drect, const RawImage& src, const RawImage& smask, const Rect& srect, bool merge, const Rect& clip = Null);

RawImage            ImageToRawImage(const Image& im, int bpp = 0, bool convert = true, Draw& draw = ScreenInfo());
RawImage            MaskToRawImage(const Image& im, bool onebit = false);
RawImage            DrawToRawImage(Draw& draw, const Rect& rc, int bpp = 0, bool convert = true);

#ifdef PLATFORM_WIN32
RawImage            HBITMAPToRawImage(HBITMAP hbmp, int bpp = 8, bool convert = true, HDC hdc = ScreenInfo());
#endif

Image               RawImageToImage(const RawImage& image, Draw& draw = ScreenInfo());


//////////////////////////////////////////////////////////////////////
// raw image transforms

class ImageInfo
{
public:
	ImageInfo() : dots(0, 0), size(0, 0), bits_per_pixel(0), src_bits_per_pixel(0), alpha(false) {}

	operator bool () const { return size.cx > 0 && size.cy > 0; }

	bool                     FromClipboard();

public:
	Size                     size;               // image size in pixels
	Size                     dots;               // image size in dots
	int                      bits_per_pixel;     // bits per pixel in Load()ed image
	int                      src_bits_per_pixel; // bits per pixel in original image
	bool                     alpha;              // alpha channel may be present
	VectorMap<String, Value> extra;              // format-specific information
};

//////////////////////////////////////////////////////////////////////

class ImageEncoder
{
public:
	ImageEncoder(int save_bpp = 0) : save_bpp(save_bpp), dot_size(0, 0) {}

	ImageEncoder&            DotSize(Size _dot_size)  { dot_size = _dot_size; return *this; }

	virtual void             SaveRaw(Stream& stream, const RawImage& image) = 0;
	String                   SaveRawString(const RawImage& image);
	bool                     SaveRawFile(const char *fn, const RawImage& image);

	virtual void             Save(Stream& stream, const Image& image);
	String                   SaveString(const Image& image);
	bool                     SaveFile(const char *fn, const Image& image);

	virtual RawImage         LoadRaw(Stream& stream, int page_index = 0) = 0;
	RawImage                 LoadRawString(const String& s, int page_index = 0);
	RawImage                 LoadRawFile(const char *fn, int page_index = 0);

	virtual Image            Load(Stream& stream, int page_index = 0);
	Image                    LoadString(const String& s, int page_index = 0);
	Image                    LoadFile(const char *fn, int page_index = 0);

	virtual Array<ImageInfo> InfoMulti(Stream& stream) = 0;
	ImageInfo                Info(Stream& stream);
	Array<ImageInfo>         InfoStringMulti(const String& s);
	ImageInfo                InfoString(const String& s);
	Array<ImageInfo>         InfoFileMulti(const char *fn);
	ImageInfo                InfoFile(const char *fn);

	void                     SerializeRaw(Stream& stream, RawImage& image);
	void                     Serialize(Stream& stream, Image& image);

protected:
	Size                     dot_size;
	int                      save_bpp;
};

//////////////////////////////////////////////////////////////////////

typedef  One<ImageEncoder> (*NewEncoderProc)();
template BitWiseType<NewEncoderProc>;
typedef  Vector<NewEncoderProc> EncoderMap;

//////////////////////////////////////////////////////////////////////

class RleEncoder : public ImageEncoder
{
public:
	RleEncoder();

	virtual RawImage         LoadRaw(Stream& stream, int page_index = 0);
	virtual Image            Load(Stream& stream, int page_index = 0);
	virtual void             SaveRaw(Stream& stream, const RawImage& image);
	virtual void             Save(Stream& stream, const Image& image);
	virtual Array<ImageInfo> InfoMulti(Stream& stream);
};

//////////////////////////////////////////////////////////////////////

class BmpEncoder : public ImageEncoder
{
public:
	BmpEncoder(int bpp_out = -1, bool file_header = true);

	virtual void             SaveRaw(Stream& stream, const RawImage& image);
	virtual RawImage         LoadRaw(Stream& stream, int page_index = 0);
	virtual Image            Load(Stream& stream, int page_index = 0);
	virtual Array<ImageInfo> InfoMulti(Stream& stream);

	static One<ImageEncoder> New() { return new BmpEncoder; }

private:
	bool              InfoRaw(Stream& stream, RawImage::Header& header);
	RawImage          LoadRaw(Stream& stream, RawImage::Header& header);

private:
	bool              file_header;
};

//////////////////////////////////////////////////////////////////////

/*  */ One<ImageEncoder> MapInfo(ImageInfo& info, const EncoderMap& map, Stream& stream);
/*  */ One<ImageEncoder> MapInfo(Array<ImageInfo>& info, const EncoderMap& map, Stream& stream);
inline One<ImageEncoder> MapInfoString(ImageInfo& info, const EncoderMap& map, const String& s)        { return MapInfo(info, map, StringStream(s)); }
inline One<ImageEncoder> MapInfoString(Array<ImageInfo>& info, const EncoderMap& map, const String& s) { return MapInfo(info, map, StringStream(s)); }
inline One<ImageEncoder> MapInfoFile(ImageInfo& info, const EncoderMap& map, const char *fn)           { return MapInfo(info, map, FileIn(fn)); }
inline One<ImageEncoder> MapInfoFile(Array<ImageInfo>& info, const EncoderMap& map, const char *fn)    { return MapInfo(info, map, FileIn(fn)); }

Image                    MapLoad(const EncoderMap& map, Stream& stream, int page_index = 0);
inline Image             MapLoadString(const EncoderMap& map, const String& s, int page_index = 0)    { return MapLoad(map, StringStream(s), page_index); }
inline Image             MapLoadFile(const EncoderMap& map, const char *fn, int page_index = 0)       { return MapLoad(map, FileIn(fn), page_index); }

RawImage                 MapLoadRaw(const EncoderMap& map, Stream& stream, int page_index = 0);
inline RawImage          MapLoadRawString(const EncoderMap& map, const String& s, int page_index = 0) { return MapLoadRaw(map, StringStream(s), page_index); }
inline RawImage          MapLoadRawFile(const EncoderMap& map, const char *fn, int page_index = 0)    { return MapLoadRaw(map, FileIn(fn), page_index); }

//////////////////////////////////////////////////////////////////////

const EncoderMap&        StdMap();

inline One<ImageEncoder> StdInfo(ImageInfo& info, Stream& stream)                 { return MapInfo(info, StdMap(), stream); }
inline One<ImageEncoder> StdInfo(Array<ImageInfo>& info, Stream& stream)          { return MapInfo(info, StdMap(), stream); }
inline One<ImageEncoder> StdInfoString(ImageInfo& info, const String& s)          { return MapInfoString(info, StdMap(), s); }
inline One<ImageEncoder> StdInfoString(Array<ImageInfo>& info, const String& s)   { return MapInfoString(info, StdMap(), s); }
inline One<ImageEncoder> StdInfoFile(ImageInfo& info, const char *fn)             { return MapInfoFile(info, StdMap(), fn); }
inline One<ImageEncoder> StdInfoFile(Array<ImageInfo>& info, const char *fn)      { return MapInfoFile(info, StdMap(), fn); }

inline Image             StdLoad(Stream& stream, int page_index = 0)              { return MapLoad(StdMap(), stream, page_index); }
inline Image             StdLoadString(const String& s, int page_index = 0)       { return MapLoadString(StdMap(), s, page_index); }
inline Image             StdLoadFile(const char *fn, int page_index = 0)          { return MapLoadFile(StdMap(), fn, page_index); }

inline RawImage          StdLoadRaw(Stream& stream, int page_index = 0)           { return MapLoadRaw(StdMap(), stream, page_index); }
inline RawImage          StdLoadRawString(const String& s, int page_index = 0)    { return MapLoadRawString(StdMap(), s, page_index); }
inline RawImage          StdLoadRawFile(const char *fn, int page_index = 0)       { return MapLoadRawFile(StdMap(), fn, page_index); }

//////////////////////////////////////////////////////////////////////

#endif//__Graphics_ImageIO__

