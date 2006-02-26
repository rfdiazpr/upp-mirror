#ifndef DRAW_PIXEL_UTIL_H
#define DRAW_PIXEL_UTIL_H

#ifdef PLATFORM_WIN32
class PixelArrayDraw : public Draw
{
public:
	PixelArrayDraw()                           { bitmap = oldbmp = NULL; bits = NULL; }
	PixelArrayDraw(Size size, int bpp = -3)    { Open(size, bpp); }
	~PixelArrayDraw()                          { Close(); }

	void       Open(Size size, int bpp = -3);
	void      *GetBits() const                 { return bits; }
	PixelArray GetResult() const;
	void       Close();

private:
	Size       size;
	int        bpp;
	void      *bits;
	HBITMAP    bitmap;
	HBITMAP    oldbmp;
};
#endif

class PixelReader8
{
public:
	PixelReader8(const PixelArray& array);

	const byte          *Get(int scan) const;
	const byte          *operator [] (int scan) const { return Get(scan); }
	const byte          *Get() const                  { return data; }

private:
	const PixelArray&    array;
	void               (*decode)(byte *dest, const byte *src, unsigned count);
	mutable const byte  *data;
	mutable int          last_y;
	mutable Vector<byte> buffer;
};

class PixelReader24
{
public:
	PixelReader24(const PixelArray& array);

	const byte          *Get(int scan) const;
	const byte          *operator [] (int scan) const { return Get(scan); }
	const byte          *Get() const                  { return data; }

private:
	const PixelArray&    array;
	void               (*decode)(byte *dest, const byte *src, unsigned count, const byte mask_info[6]);
	mutable const byte  *data;
	mutable int          last_y;
	mutable Vector<byte> buffer;
	byte                 mask_info[6];
	bool                 palette;
};

class PixelWriter8
{
public:
	PixelWriter8(PixelArray& array);

	byte                *Get(int scan);
	byte                *operator [] (int scan)       { return Get(scan); }
	byte                *Get() const                  { return data; }
	void                 Write();

private:
	PixelArray&          array;
	void               (*encode)(byte *dest, const byte *src, unsigned count);
	byte                *data;
	int                  last_y;
	Vector<byte>         buffer;
};

class PixelWriter24
{
public:
	PixelWriter24(PixelArray& array);

	byte                *Get(int scan);
	byte                *operator [] (int scan)       { return Get(scan); }
	byte                *Get() const                  { return data; }
	void                 Write();

private:
	PixelArray&          array;
	void               (*encode)(byte *dest, const byte *src, unsigned count, const byte mask_info[6]);
	byte                *data;
	int                  last_y;
	Vector<byte>         buffer;
	byte                 mask_info[6];
};

enum
{
	IMAGE_TX = 1, IMAGE_TY = 2, IMAGE_TD = 4,
	IMAGE_TXY            = IMAGE_TX | IMAGE_TY,
	IMAGE_TANTICLOCKWISE = IMAGE_TX | IMAGE_TD,
	IMAGE_TCLOCKWISE     = IMAGE_TY | IMAGE_TD,
};

PixelArray          PixelTransform(const PixelArray& image, int flags);
void                PixelTransform(PixelArray& image, const Rect& rc, int flags);
PixelArray          PixelMask(PixelArray& image, Color color, int tolerance = 0, int bpp = 8, Color kill_mask_color = Null);
void                PixelInvertMask(PixelArray& image);
PixelArray          PixelDiffMask(const PixelArray& a, const PixelArray& b, int out_bpp = 8);
void                PixelKillMask(PixelArray& image, const PixelArray& mask, byte color, bool inv = false);
void                PixelKillMask(PixelArray& image, const PixelArray& mask, Color color, bool inv = false);
void                PixelAlphaBlend(PixelArray& image, Color color, int percent, const Rect& min_area, double gamma = 2.5);
inline void         PixelAlphaBlend(PixelArray& image, Color color, int percent, double gamma = 2.5) { PixelAlphaBlend(image, color, percent, image.GetRect(), gamma); }
void                PixelAlphaBlend(PixelArray& image, const Rect& dest, const PixelArray& with_image, Point src_pos, int alpha = 50);
void                PixelSetPixel(PixelArray& pa, const Vector<Point>& list, Color color);
void                PixelSetPixel(PixelArray& pa, const Vector<Point>& list, byte color);
void                PixelSetPixel(PixelArray& pa, int x, int y, Color color);
void                PixelSetPixel(PixelArray& pa, int x, int y, byte color);
Vector<Color>       PixelGetPixel(const PixelArray& pa, const Vector<Point>& list);
Vector<Color>       PixelGetPixel(const PixelArray& pa, int x, int y, int width);
Vector<byte>        PixelGetPixel8(const PixelArray& pa, int x, int y, int width);
Vector<byte>        PixelGetPixel24(const PixelArray& pa, int x, int y, int width);
Color               PixelGetPixel(const PixelArray& pa, int x, int y);
inline Color        PixelGetPixel(const PixelArray& pa, Point pt) { return PixelGetPixel(pa, pt.x, pt.y); }
Color               PixelGetAvgColor(const PixelArray& pa, const Rect& rc);

PixelArray          DrawToPixelArray(Draw& draw, const Rect& rc,
	bool mono = false, int bpp = 0, int alignment = 4,
	const dword *cmasks = NULL, const Vector<Color>& palette = Vector<Color>());

#ifdef PLATFORM_WIN32
PixelArray          HBITMAPToPixelArray(HBITMAP hbmp, bool mono, int bpp, HDC hdc = ScreenInfo());
bool                PixelArrayToClipboard(const PixelArray& array, bool clear);
PixelArray          ClipboardToPixelArray();
AlphaArray          ClipboardToAlphaArray();
#endif
#ifdef PLATFORM_X11
Size                GetDrawableSize(Drawable drawable);
PixelArray          DrawableToPixelArray(Drawable drawable, const Rect& rc,
	bool mono, int bpp, int alignment,
	const dword *cmasks = NULL, const Vector<Color>& palette = Vector<Color>());
#endif

AlphaArray          AlphaTransform(const AlphaArray& image, int flags);
void                AlphaTransform(AlphaArray& image, const Rect& rc, int flags);
void                AlphaKillMask(AlphaArray& image, byte color);
void                AlphaKillMask(AlphaArray& image, Color color);
void                AlphaChangeColor(AlphaArray& aa, const Rect& rc, Color src, Color dest, int tolerance = 0);
void                AlphaFloodFill(AlphaArray& aa, Color color, Point pt, const Rect& rc, bool diagonal = false);
void                AlphaSetPixel(AlphaArray& aa, const Vector<Point>& list, Color color);
void                AlphaSetPixel(AlphaArray& aa, const Vector<Point>& list, byte color, byte mask);
void                AlphaSetPixel(AlphaArray& aa, int x, int y, Color color);
void                AlphaSetPixel(AlphaArray& aa, int x, int y, byte color, byte mask);
Vector<Color>       AlphaGetPixel(const AlphaArray& aa, const Vector<Point>& list);
Vector<Color>       AlphaGetPixel(const AlphaArray& aa, int x, int y, int width);
Color               AlphaGetPixel(const AlphaArray& aa, int x, int y);
inline Color        AlphaGetPixel(const AlphaArray& aa, Point pt) { return AlphaGetPixel(aa, pt.x, pt.y); }

struct IMLImageInfo : AlphaImageInfo
{
	IMLImageInfo() {}
	IMLImageInfo(const AlphaImageInfo& ai, String name = Null, String encoded_data = Null)
	: AlphaImageInfo(ai), name(name), encoded_data(encoded_data) {}

	String name;
	String encoded_data;
};

void                AlphaScanIML(CParser& parser, Array<IMLImageInfo>& out_images, VectorMap<String, String>& out_settings);
AlphaArray          IMLToAlphaArray(const IMLImageInfo& iml);

#endif
