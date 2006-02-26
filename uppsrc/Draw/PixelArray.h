inline unsigned CalcGray(byte r, byte g, byte b) { return b * 26 + g * 153 + r * 77; }
inline unsigned CalcGray(const byte *p)          { return p[0] * 26 + p[1] * 153 + p[2] * 77; }
inline unsigned CalcGray(Color c)                { return c.GetB() * 26 + c.GetG() * 153 + c.GetR() * 77; }

void BltXlatB(byte *dest, const byte *src, unsigned count, const byte *table);
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
void Blt8to24(byte *dest, const byte *src, unsigned count, const byte bgr[3][256]);
void Blt8to16(byte *dest, const byte *src, unsigned count, const byte bgr[3][256], const byte BsmGsmRsm[]);
void Blt8to32(byte *dest, const byte *src, unsigned count, const byte bgr[3][256], const byte BsmGsmRsm[]);
void Blt24to16(byte *dest, const byte *src, unsigned count, const byte BsmGsmRsm[]);
void Blt24to32(byte *dest, const byte *src, unsigned count, const byte BsmGsmRsm[]);
void Blt16to24(byte *dest, const byte *src, unsigned count, const byte BsmGsmRsm[]);
void Blt32to24(byte *dest, const byte *src, unsigned count, const byte BsmGsmRsm[]);
void Blt16to16(byte *dest, const byte *src, unsigned count, const byte dest_BsmGsmRsm[], const byte src_BsmGsmRsm[]);
void Blt32to16(byte *dest, const byte *src, unsigned count, const byte dest_BsmGsmRsm[], const byte src_BsmGsmRsm[]);
void Blt16to32(byte *dest, const byte *src, unsigned count, const byte dest_BsmGsmRsm[], const byte src_BsmGsmRsm[]);
void Blt32to32(byte *dest, const byte *src, unsigned count, const byte dest_BsmGsmRsm[], const byte src_BsmGsmRsm[]);
void BltSwapBitOrder(byte *dest, const byte *src, unsigned count);
void BltSwapEndian2(byte *dest, const byte *src, unsigned count2);
void BltSwapEndian4(byte *dest, const byte *src, unsigned count4);

void BltMask8(byte *dest, const byte *mask, unsigned count, byte mask_value);
void BltBuildMaskRgb(byte *dest, const byte *src, unsigned count, const byte rRgGbB[6]);
void BltKillMask1(byte *dest, const byte *mask, bool inv, byte color, unsigned count);
void BltKillMask3(byte *dest, const byte *mask, bool inv, const byte rgb[3], unsigned count);
void BltDiffMask3(byte *out, const byte *sa, const byte *sb, unsigned count);
void BltOp1And(byte *dest, byte value, unsigned count);
void BltOp1Or(byte *dest, byte value, unsigned count);
void BltOp1Xor(byte *dest, byte value, unsigned count);

typedef void (*BltOp)(byte *dest, const byte *src, unsigned bytes);

void BltOp2Nop(byte *dest, const byte *src, unsigned bytes);
void BltOp2Copy(byte *dest, const byte *src, unsigned bytes);
void BltOp2NotCopy(byte *dest, const byte *src, unsigned bytes);
void BltOp2And(byte *dest, const byte *src, unsigned bytes);
void BltOp2Or(byte *dest, const byte *src, unsigned bytes);
void BltOp2Xor(byte *dest, const byte *src, unsigned bytes);
void BltOp2NotAnd(byte *dest, const byte *src, unsigned bytes);
void BltOp2NotOr(byte *dest, const byte *src, unsigned bytes);
void BltOp2NotXor(byte *dest, const byte *src, unsigned bytes);
void BltOp2Black(byte *dest, const byte *src, unsigned bytes);
void BltOp2White(byte *dest, const byte *src, unsigned bytes);

#ifdef PLATFORM_X11
enum
{
	X11_ROP2_ZERO,
	X11_ROP2_AND,
	X11_ROP2_AND_NOT,
	X11_ROP2_COPY,
	X11_ROP2_NOT_AND,
	X11_ROP2_NOP,
	X11_ROP2_XOR,
	X11_ROP2_OR,
	X11_ROP2_NOT_AND_NOT,
	X11_ROP2_NOT_XOR,
	X11_ROP2_INVERT,
	X11_ROP2_OR_NOT,
	X11_ROP2_NOT_COPY,
	X11_ROP2_NOT_OR,
	X11_ROP2_NOT_OR_NOT,
	X11_ROP2_ONE,
};
#endif

class Draw;

#ifdef PLATFORM_WIN32
	#define IMAGE_ORIGIN_BOTTOM 1
#else
	#define IMAGE_ORIGIN_BOTTOM 0
#endif

enum PixelOp
{
	PIXEL_NOP,
	PIXEL_BLACK,
	PIXEL_WHITE,
	PIXEL_COPY,
	PIXEL_NOT_COPY,
	PIXEL_AND,
	PIXEL_NOT_AND,
	PIXEL_OR,
	PIXEL_NOT_OR,
	PIXEL_XOR,
	PIXEL_NOT_XOR,
	PIXEL_OP_COUNT,
};

int GetPixelROP(PixelOp op);

class PixelArrayDeep
{
protected:
	PixelArrayDeep();

public:
	Size          size;
	Size          dot_size;
	Point         hotspot;
	int           raw_bpp;
	int           bpp;
	int           format_code;
	int           row_bytes;
	int           alignment;
	bool          mono;
	dword         cmask[3]; // r, g, b
	int           cshift24[3];
	int           cbits[3];
};

class PixelArray : public PixelArrayDeep, DeepCopyOption<PixelArray>
{
public:
	PixelArray() {}
	PixelArray(const Nuller&) {}
	PixelArray(const PixelArray& pa, int deep)
	: PixelArrayDeep(pa), pixels(pa.pixels, deep), palette(pa.palette, deep) {}

	PixelArray(Size size, int bpp = -3, int alignment = 4,
		const dword *cmasks = NULL, pick_ Vector<Color>& palette = Vector<Color>())
	{ Create(size.cx, size.cy, bpp, alignment, cmasks, palette); }
	PixelArray(int cx, int cy, int bpp = -3, int alignment = 4,
		const dword *cmasks = NULL, pick_ Vector<Color>& palette = Vector<Color>())
	{ Create(cx, cy, bpp, alignment, cmasks, palette); }
	PixelArray(Size size, int bpp, int alignment,
		const dword *cmasks, const Vector<Color>& palette, int deep)
	{ Create(size.cx, size.cy, bpp, alignment, cmasks, palette, deep); }
	PixelArray(int cx, int cy, int bpp, int alignment,
		const dword *cmasks, const Vector<Color>& palette, int deep)
	{ Create(cx, cy, bpp, alignment, cmasks, palette, deep); }

	static PixelArray    Mono(Size size, int bpp = 8, int alignment = 4)       { return Mono(size.cx, size.cy, bpp, alignment); }
	static PixelArray    Mono(int cx, int cy, int bpp = 8, int alignment = 4)  { PixelArray p; p.CreateMono(cx, cy, bpp, alignment); return p; }

	bool                 IsEmpty() const                                       { return bpp == 0; }
	bool                 operator ! () const                                   { return IsEmpty(); }

	int                  SizeOfInstance() const;

	void                 Clear();
	void                 Reserve(int size)                                     { pixels.Reserve(size); }

	void                 Serialize(Stream& stream);
	static int           SerializeInfo(Stream& stream,
		Size& size, bool& mono, int& raw_bpp, int& alignment,
		dword cmasks[3], Vector<Color>& palette);

	void                 Create(int cx, int cy, int bpp = -3, int alignment = 4,
		const dword *cmasks = NULL, pick_ Vector<Color>& palette = Vector<Color>());
	void                 Create(Size size, int bpp = -3, int alignment = 4,
		const dword *cmasks = NULL, pick_ Vector<Color>& palette = Vector<Color>())
	{ Create(size.cx, size.cy, bpp, alignment, cmasks, palette); }
	void                 Create(int cx, int cy, int bpp, int alignment,
		const dword *cmasks, const Vector<Color>& palette, int deep);
	void                 Create(Size size, int bpp, int alignment,
		const dword *cmasks, const Vector<Color>& palette, int deep)
	{ Create(size.cx, size.cy, bpp, alignment, cmasks, palette, deep); }

	void                 CreateMono(Size size, int bpp = 8, int alignment = 4) { CreateMono(size.cx, size.cy, bpp, alignment); }
	void                 CreateMono(int cx, int cy, int bpp = 8, int alignment = 4);

	Size                 GetSize() const                                       { return size; }
	Size                 GetDotSize() const                                    { return dot_size; }
	int                  GetWidth() const                                      { return size.cx; }
	int                  GetHeight() const                                     { return size.cy; }
	Point                GetHotSpot() const                                    { return hotspot; }
	int                  GetRawBPP() const                                     { return raw_bpp; }
	int                  GetBPP() const                                        { return bpp; }
	bool                 IsMono() const                                        { return mono; }
	int                  GetFormatCode() const                                 { return format_code; }
	int                  GetAlign() const                                      { return alignment; }
	Rect                 GetRect() const                                       { return size; }
	int                  GetRowBytes() const                                   { return row_bytes; }
	int                  GetRowBytes(int bpp) const                            { return GetRowBytes(bpp, size.cx, alignment); }
	static int           GetRowBytes(int bpp, int cx, int align)               { return ((bpp * cx + 8 * align - 1) >> 3) & -align; }
	int                  GetBytes() const                                      { return row_bytes * size.cy; }
	void                 GetMaskInfo(byte BsmGsmRsm[6]) const;
	void                 SetMono(bool m)                                       { mono = m; }
	void                 SetSize(Size size);
	void                 SetDotSize(Size dots)                                 { dot_size = dots; }
	void                 SetHotSpot(Point pt)                                  { hotspot = pt; }
	void                 SetFormat(int bpp, int alignment);
	void                 SetMasks(const dword *cmasks);
	void                 Shrink();
	const Vector<Color>& GetPalette() const                                    { return palette; }

#if IMAGE_ORIGIN_BOTTOM
	int                  GetUpRowBytes() const                                 { return +row_bytes; }
	int                  GetDownRowBytes() const                               { return -row_bytes; }

	const byte          *GetUpScan(int y) const                                { return pixels.GetIter(row_bytes * y); }
	byte                *GetUpScan(int y)                                      { return pixels.GetIter(row_bytes * y); }

	const byte          *GetDownScan(int y) const                              { return pixels.GetIter(row_bytes * (size.cy - 1 - y)); }
	byte                *GetDownScan(int y)                                    { return pixels.GetIter(row_bytes * (size.cy - 1 - y)); }
#else//IMAGE_ORIGIN_BOTTOM
	int                  GetUpRowBytes() const                                 { return -row_bytes; }
	int                  GetDownRowBytes() const                               { return +row_bytes; }

	const byte          *GetDownScan(int y) const                              { return pixels.GetIter(row_bytes * y); }
	byte                *GetDownScan(int y)                                    { return pixels.GetIter(row_bytes * y); }

	const byte          *GetUpScan(int y) const                                { return pixels.GetIter(row_bytes * (size.cy - 1 - y)); }
	byte                *GetUpScan(int y)                                      { return pixels.GetIter(row_bytes * (size.cy - 1 - y)); }
#endif//IMAGE_ORIGIN_BOTTOM
	const byte          *GetFastScan(int y) const                              { return pixels.GetIter(row_bytes * y); }
	byte                *GetFastScan(int y)                                    { return pixels.GetIter(row_bytes * y); }

	const byte          *Begin() const                                         { return pixels.Begin(); }
	byte                *Begin()                                               { return pixels.Begin(); }

	const byte          *End() const                                           { return pixels.Begin() + GetBytes(); }
	byte                *End()                                                 { return pixels.Begin() + GetBytes(); }

	void                 Paint(Draw& draw, PixelOp op = PIXEL_COPY) const                   { Paint(draw, GetRect(), GetRect(), op); }
	void                 Paint(Draw& draw, Point pos, PixelOp op = PIXEL_COPY) const        { Paint(draw, GetRect(), Rect(pos, size), op); }
	void                 Paint(Draw& draw, const Rect& dest, PixelOp op = PIXEL_COPY) const { Paint(draw, GetRect(), dest, op); }
	void                 Paint(Draw& draw, const Rect& src, const Rect& dest, PixelOp op = PIXEL_COPY) const;

	void                 Paint(Draw& draw, Color black, Color white, Color doxor = Null) const                   { Paint(draw, GetRect(), GetRect(), black, white, doxor); }
	void                 Paint(Draw& draw, Point pos, Color black, Color white, Color doxor = Null) const        { Paint(draw, GetRect(), Rect(pos, size), black, white, doxor); }
	void                 Paint(Draw& draw, const Rect& dest, Color black, Color white, Color doxor = Null) const { Paint(draw, GetRect(), dest, black, white, doxor); }
	void                 Paint(Draw& draw, const Rect& src, const Rect& dest, Color black, Color white, Color doxor = Null) const;

#ifdef PLATFORM_WIN32
	enum STRETCH_MODE { STRETCH_PAL_MONO, STRETCH_RGB_MONO, STRETCH_PAL_COLOR, STRETCH_RGB_COLOR };

	void                 PaintDC(HDC hdc, int rop) const                                    { PaintDC(hdc, GetRect(), GetRect(), rop); }
	void                 PaintDC(HDC hdc, Point pos, int rop) const                         { PaintDC(hdc, GetRect(), Rect(pos, size), rop); }
	void                 PaintDC(HDC hdc, const Rect& dest, int rop) const                  { PaintDC(hdc, GetRect(), dest, rop); }
	void                 PaintDC(HDC hdc, const Rect& src, const Rect& dest, int rop) const;

	void                 PaintDCRaw(HDC hdc, const Rect& src, const Rect& dest, int rop, STRETCH_MODE mode, const Color *pal) const;

	void                 PaintDC(HDC hdc, Color fore, Color back, Color doxor = Null) const                                    { PaintDC(hdc, GetRect(), GetRect(), fore, back, doxor); }
	void                 PaintDC(HDC hdc, Point pos, Color fore, Color back, Color doxor = Null) const                         { PaintDC(hdc, GetRect(), Rect(pos, size), fore, back, doxor); }
	void                 PaintDC(HDC hdc, const Rect& dest, Color fore, Color back, Color doxor = Null) const                  { PaintDC(hdc, GetRect(), dest, fore, back, doxor); }
	void                 PaintDC(HDC hdc, const Rect& src, const Rect& dest, Color fore, Color back, Color doxor = Null) const;

	struct Header : public BITMAPINFOHEADER
	{
		RGBQUAD palette[256];

		operator const BITMAPINFO * () const { return reinterpret_cast<const BITMAPINFO *>(this); }
		operator BITMAPINFO * ()             { return reinterpret_cast<BITMAPINFO *>(this); }
	};

	bool                 SetHeader(Header& header, const Color *pal_mono) const;
#endif
#ifdef PLATFORM_X11
	void                 PaintGC(GC gc, Drawable dw, const Vector<Rect>& clip, int rop) const                                    { PaintGC(gc, dw, clip, GetRect(), GetRect(), rop); }
	void                 PaintGC(GC gc, Drawable dw, const Vector<Rect>& clip, Point pos, int rop) const                         { PaintGC(gc, dw, clip, GetRect(), Rect(pos, size), rop); }
	void                 PaintGC(GC gc, Drawable dw, const Vector<Rect>& clip, const Rect& dest, int rop) const                  { PaintGC(gc, dw, clip, GetRect(), dest, rop); }
	void                 PaintGC(GC gc, Drawable dw, const Vector<Rect>& clip, const Rect& src, const Rect& dest, int rop) const;

	void                 PaintGC(GC gc, Drawable dw, const Vector<Rect>& clip, Color fore, Color back, Color doxor = Null) const                                    { PaintGC(gc, dw, clip, GetRect(), GetRect(), fore, back, doxor); }
	void                 PaintGC(GC gc, Drawable dw, const Vector<Rect>& clip, Point pos, Color fore, Color back, Color doxor = Null) const                         { PaintGC(gc, dw, clip, GetRect(), Rect(pos, size), fore, back, doxor); }
	void                 PaintGC(GC gc, Drawable dw, const Vector<Rect>& clip, const Rect& dest, Color fore, Color back, Color doxor = Null) const                  { PaintGC(gc, dw, clip, GetRect(), dest, fore, back, doxor); }
	void                 PaintGC(GC gc, Drawable dw, const Vector<Rect>& clip, const Rect& src, const Rect& dest, Color fore, Color back, Color doxor = Null) const;

	void                 SetXImage(XImage& ximage) const;
#endif

public:
	Vector<byte>         pixels;
	mutable Vector<Color> palette;
};

const Vector<Color>&  MonoPalette();

String                PixelDump(const PixelArray& pixel, const Rect& area = Null);

void                  CalcBitMasks(int bpp, const dword *in_cmasks, dword cmask[], int cshift24[], int cbits[]);
int                   CalcFormatCode(int bpp, const int cshift24[], const int cbits[]);

PixelArray            CreatePalette(const PixelArray& image, int out_bpp = 8,
	int colors = -1, const Vector<Color>& out_pal = Vector<Color>());
void                  OptimizePalette(PixelArray& image);

PixelArray            PixelConvert(const PixelArray& image, int to_bpp, int alignment = 4,
	const dword *cmasks = NULL, const Vector<Color>& palette = Vector<Color>());
void                  PixelSetConvert(PixelArray& image, int to_bpp, int alignment = 4,
	const dword *cmasks = NULL, const Vector<Color>& palette = Vector<Color>());

PixelArray            PixelConvertScreen(const PixelArray& image);
void                  PixelSetConvertScreen(PixelArray& image);

void                  PixelScanToRLE(String& result, const byte *dp, int width);
const byte           *RLEToPixelScan(const byte *src, byte *pixel, int width);

String                PixelToRLE(const PixelArray& pa);
void                  RLEToPixel(PixelArray& pa, const byte *src);

void                  PixelSet(PixelArray& dest, const Rect& drect, Color color);
void                  PixelSet(PixelArray& dest, const Rect& drect, byte value);
void                  PixelCopy(PixelArray& dest, const Rect& drect,
	const PixelArray& src, const Rect& srect, const Rect& clip = Null, BltOp op = &BltOp2Copy);
PixelArray            PixelCrop(const PixelArray& src, const Rect& rc);
PixelArray            PixelMono(const PixelArray& aa, byte threshold = 0xc0, bool inv = false, int bpp = 8);

class AlphaArray : DeepCopyOption<AlphaArray>
{
public:
	AlphaArray() {}
	AlphaArray(const Nuller&) {}
	AlphaArray(const AlphaArray& aa, int deep)
	: pixel(aa.pixel, 0), alpha(aa.alpha, 0) {}
	AlphaArray(pick_ PixelArray& pixel, pick_ PixelArray& alpha) : pixel(pixel), alpha(alpha) {}
	AlphaArray(const PixelArray& pixel, const PixelArray& alpha, int deep) : pixel(pixel, 0), alpha(alpha, 0) {}

	AlphaArray(Size size, int bpp = -3, int alignment = 4, const dword *cmasks = NULL,
		pick_ Vector<Color>& palette = Vector<Color>(), int mask_bpp = 8, int mask_align = 4)
	{ Create(size.cx, size.cy, bpp, alignment, cmasks, palette, mask_bpp, mask_align); }
	AlphaArray(int cx, int cy, int bpp = -3, int alignment = 4, const dword *cmasks = NULL,
		pick_ Vector<Color>& palette = Vector<Color>(), int mask_bpp = 8, int mask_align = 4)
	{ Create(cx, cy, bpp, alignment, cmasks, palette, mask_bpp, mask_align); }
	AlphaArray(Size size, int bpp, int alignment, const dword *cmasks,
		int mask_bpp, int mask_align, const Vector<Color>& palette, int)
	{ Create(size.cx, size.cy, bpp, alignment, cmasks, Vector<Color>(palette, 0), mask_bpp, mask_align); }
	AlphaArray(int cx, int cy, int bpp, int alignment, const dword *cmasks,
		int mask_bpp, int mask_align, const Vector<Color>& palette, int)
	{ Create(cx, cy, bpp, alignment, cmasks, Vector<Color>(palette, 0), mask_bpp, mask_align); }

	void              Create(int cx, int cy, int bpp = -3, int alignment = 4, const dword *cmasks = NULL,
		pick_ Vector<Color>& palette = Vector<Color>(), int mask_bpp = 8, int mask_align = 4);
	void              Create(Size size, int bpp = -3, int alignment = 4, const dword *cmasks = NULL,
		pick_ Vector<Color>& palette = Vector<Color>(), int mask_bpp = 8, int mask_align = 4)
	{ Create(size.cx, size.cy, bpp, alignment, cmasks, palette, mask_bpp, mask_align); }
	void              Create(int cx, int cy, int bpp, int alignment, const dword *cmasks,
		int mask_bpp, int mask_align, const Vector<Color>& palette, int)
	{ Create(cx, cy, bpp, alignment, cmasks, Vector<Color>(palette, 0), mask_bpp, mask_align); }
	void              Create(Size size, int bpp, int alignment, const dword *cmasks,
		int mask_bpp, int mask_align, const Vector<Color>& palette, int)
	{ Create(size.cx, size.cy, bpp, alignment, cmasks, Vector<Color>(palette, 0), mask_bpp, mask_align); }

	void              CreateAlpha(int mask_bpp = 8, int mask_align = 4, bool init0 = true);

	void              Clear()                                   { pixel.Clear(); alpha.Clear(); }

	int               SizeOfInstance() const;

	void              Serialize(Stream& stream);

	bool              IsEmpty() const                           { return pixel.IsEmpty(); }
	Size              GetSize() const                           { return pixel.GetSize(); }
	Size              GetDotSize() const                        { return pixel.GetDotSize(); }
	Point             GetHotSpot() const                        { return pixel.GetHotSpot(); }
	int               GetWidth() const                          { return pixel.GetWidth(); }
	int               GetHeight() const                         { return pixel.GetHeight(); }
	int               GetPixelBPP() const                       { return pixel.GetBPP(); }
	int               GetAlphaBPP() const                       { return alpha.GetBPP(); }
	bool              HasAlpha() const                          { return !alpha.IsEmpty(); }
	Rect              GetRect() const                           { return pixel.GetRect(); }
	int               GetPixelRowBytes() const                  { return pixel.GetRowBytes(); }
	int               GetAlphaRowBytes() const                  { return alpha.GetRowBytes(); }
	int               GetRowBytes(int bpp) const                { return pixel.GetRowBytes(bpp); }
	int               GetPixelBytes() const                     { return pixel.GetBytes(); }
	int               GetAlphaBytes() const                     { return alpha.GetBytes(); }
	int               GetBytes() const                          { return GetPixelBytes() + GetAlphaBytes(); }

	void              SetDotSize(Size dots)                     { pixel.SetDotSize(dots); }
	void              SetHotSpot(Point pt)                      { pixel.SetHotSpot(pt); }

	const byte       *GetPixelUpScan(int y) const               { return pixel.GetUpScan(y); }
	byte             *GetPixelUpScan(int y)                     { return pixel.GetUpScan(y); }
	const byte       *GetAlphaUpScan(int y) const               { return alpha.GetUpScan(y); }
	byte             *GetAlphaUpScan(int y)                     { return alpha.GetUpScan(y); }

	const byte       *GetPixelDownScan(int y) const             { return pixel.GetDownScan(y); }
	byte             *GetPixelDownScan(int y)                   { return pixel.GetDownScan(y); }
	const byte       *GetAlphaDownScan(int y) const             { return alpha.GetDownScan(y); }
	byte             *GetAlphaDownScan(int y)                   { return alpha.GetDownScan(y); }

	const byte       *GetPixelFastScan(int y) const             { return pixel.GetFastScan(y); }
	byte             *GetPixelFastScan(int y)                   { return pixel.GetFastScan(y); }
	const byte       *GetAlphaFastScan(int y) const             { return alpha.GetFastScan(y); }
	byte             *GetAlphaFastScan(int y)                   { return alpha.GetFastScan(y); }

	void              Paint(Draw& draw) const                   { Paint(draw, GetRect(), GetRect()); }
	void              Paint(Draw& draw, Point pos) const        { Paint(draw, GetRect(), Rect(pos, GetSize())); }
	void              Paint(Draw& draw, const Rect& dest) const { Paint(draw, GetRect(), dest); }
	void              Paint(Draw& draw, const Rect& src, const Rect& dest) const;

public:
	PixelArray        pixel;
	PixelArray        alpha;
};

String                AlphaDump(const AlphaArray& image, const Rect& area = Null);

AlphaArray            AlphaConvert(const AlphaArray& image,
	int pixel_bpp, int pixel_align = 4, const dword *cmasks = NULL, const Vector<Color>& palette = Vector<Color>(),
	int alpha_bpp = 8, int alpha_align = 4);

void                  AlphaScanToRLE(String& result, const byte *dp, const byte *mp, int width);
const byte           *RLEToAlphaScan(const byte *src, byte *pixel, byte *alpha, int width);

String                AlphaToRLE(const AlphaArray& aa);
void                  RLEToAlpha(AlphaArray& aa, const byte *src);

void                  AlphaSet(AlphaArray& dest, const Rect& drect, Color color);
void                  AlphaSet(AlphaArray& dest, const Rect& drect, byte value);

void                  AlphaCopy(PixelArray& dest, const Rect& drect, const AlphaArray& src, const Rect& srect, const Rect& clip = Null);
void                  AlphaCopy(AlphaArray& dest, const Rect& drect, const AlphaArray& src, const Rect& srect, bool merge, const Rect& clip = Null);
AlphaArray            AlphaCrop(const AlphaArray& src, const Rect& rc);

void                  AlphaGray(AlphaArray& aa, const Rect& drect);
void                  AlphaEtch(AlphaArray& aa, const Rect& drect);

class AlphaImageInfo
{
public:
	enum ENCODING { COLOR_RLE, MONO_RLE, MONO_PACKED };

	AlphaImageInfo(Size size = Size(0, 0), int encoding = COLOR_RLE, Point hotspot = Point(0, 0))
		: size(size), encoding(encoding), hotspot(hotspot) {}

	void               Serialize(Stream& stream);

public:
	int                encoding;
	Size               size;
	Point              hotspot;
};
