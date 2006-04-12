#ifndef _nImage_Image_h_
#define _nImage_Image_h_

#include <CtrlLib/CtrlLib.h>

// #include "file.h"

#define Image NewImage
#define ImageDraw NewImageDraw
#define ImageCache NewImageCache
#define ImageEncoder NewImageEncoder

struct RGBA : Moveable<RGBA> {
	byte b, g, r, a;
};

inline RGBA RGBAColor(Color c, byte alpha = 255) {
	RGBA b; b.a = alpha; b.r = c.GetR(); b.g = c.GetG(); b.b = c.GetB(); return b;
}

inline RGBA RGBAZero() {
	RGBA b; b.a = 0; b.r = 0; b.g = 0; b.b = 0; return b;
}

void Fill(RGBA *t, const RGBA& src, int n);
void FillColor(RGBA *t, const RGBA& src, int n);
void PreMultiplyAlpha(RGBA *t, const RGBA *s, int len);
void AlphaBlend(RGBA *t, const RGBA *s, int len, byte const_alpha = 255, Color c = Null);
void UnpackRLE(RGBA *t, const byte *src, int cx);

inline int  GrayScale(int r, int g, int b) { return (77 * r + 151 * b + 28 * r) >> 8; }
inline int  GrayScale(const RGBA& c)       { return GrayScale(c.r, c.g, c.b); }
inline byte Saturate255(int x)             { return byte(~(x >> 24) & (x | (-(x >> 8) >> 24)) & 0xff); }

enum ImageKind {
	IMAGE_UNKNOWN,
	IMAGE_EMPTY,
	IMAGE_ALPHA,
	IMAGE_MASK,
	IMAGE_OPAQUE,
};

class Image;

class ImageDraw;

class ImageBuffer : NoCopy {
	int          kind;
	Size         size;
	Size         dots;
	Buffer<RGBA> pixels;
	Point        hotspot;

	void         Set(Image& img);

	RGBA*        Line(int i) const      { ASSERT(i >= 0 && i < size.cy); return (RGBA *)~pixels + i * size.cx; }

	friend class Image;

public:
	void  SetKind(int k)                { kind = k; }
	int   GetKind();

	void  SetHotSpot(Point p)           { hotspot = p; }
	Point GetHotSpot() const            { return hotspot; }

	Size  GetSize() const               { return size; }
	int   GetWidth() const              { return size.cx; }
	int   GetHeight() const             { return size.cy; }
	int   GetLength() const             { return size.cx * size.cy; }

	RGBA *operator[](int i)             { return Line(i); }
	const RGBA *operator[](int i) const { return Line(i); }
	RGBA *operator~()                   { return pixels; }
	operator RGBA*()                    { return pixels; }
	const RGBA *operator~() const       { return pixels; }
	operator const RGBA*() const        { return pixels; }

	void  Create(int cx, int cy);
	void  Create(Size sz)               { Create(sz.cx, sz.cy); }
	bool  IsEmpty()                     { return (size.cx | size.cy) == 0; }
	void  Clear()                       { Create(0, 0); }

	ImageBuffer()                       { Create(0, 0); }
	ImageBuffer(int cx, int cy)         { Create(cx, cy); }
	ImageBuffer(Size sz)                { Create(sz.cx, sz.cy); }
	ImageBuffer(Image& img);
	ImageBuffer(ImageDraw& w);
	ImageBuffer(ImageBuffer& b);
};

struct ImageCache {
	struct RegisterName {
		RegisterName(const char *name, const Image& image) { /*DUMP(name);*/ }
	};
};

class DrawSurface : NoCopy {
	int          x, y;
	Size         size;
	RGBA        *pixels;
	HDC          dc, dcMem;
	HBITMAP      hbmp, hbmpOld;

	void  Init(Draw& w, int x, int y, int cx, int cy);
	RGBA* Line(int i) const { ASSERT(i >= 0 && i < size.cy); return (RGBA *)pixels + size.cx * (size.cy - i - 1); }

public:
	operator RGBA *()                   { return pixels; }
	Size        GetSize() const         { return size; }
	RGBA       *operator[](int i)       { return Line(i); }
	const RGBA *operator[](int i) const { return Line(i); }
	int         GetLineDelta() const    { return -size.cx; }

	DrawSurface(Draw& w, const Rect& r);
	DrawSurface(Draw& w, int x, int y, int cx, int cy);
	~DrawSurface();
};

void SetSurface(Draw& w, int x, int y, int cx, int cy, RGBA *pixels);

class Image : Moveable<Image> {
private:
	struct Data : Link<Data> {
		Atomic refcount;
		void   Retain()  { AtomicInc(refcount); }
		void   Release() { if(AtomicDec(refcount) == 0) delete this; }

#ifdef PLATFORM_WIN32
		HBITMAP     hbmp;
		HBITMAP     hmask;
		HBITMAP     himg;
		RGBA       *section;
		int         paintcount;

		static StaticCriticalSection ResLock;
		static Link<Data>            ResData[1];
		static int                   ResCount;
#endif

		ImageBuffer                  buffer;

		void        SysRelease();
		void        Paint(Draw& w, int x, int y, byte const_alpha, Color c);

		Data(ImageBuffer& b);
		~Data();
	};

	Data *data;

	static StaticCriticalSection ResLock;
	static Link<Image::Data>     ResData[1];
	static int                   ResCount;

	void Set(ImageBuffer& b);

	friend class ImageBuffer;
	friend struct Data;

public:
	void Paint(Draw& w, int x, int y, byte const_alpha = 255, Color c = Null) const;

	const RGBA*    operator~() const;
	operator const RGBA*() const;
	const RGBA* operator[](int i) const;

	Size  GetSize() const;
	int   GetWidth() const              { return GetSize().cx; }
	int   GetHeight() const             { return GetSize().cy; }
	int   GetLength() const;
	Point GetHotSpot() const;

	void Clear();

	Image& operator=(const Image& img);
	Image& operator=(ImageBuffer& img);

	operator bool()                                         { return !IsNullInstance(); }

	bool IsNullInstance() const                             { Size sz = GetSize(); return (sz.cx|sz.cy) == 0; }

	Image()                                                 { data = NULL; }
	Image(const Nuller&)                                    { data = NULL; }
	Image(const Image& img);
	Image(ImageBuffer& b);
	~Image();


// IML support ("private")
	struct Init {
		const char *const *scans;
		const char *info;
		short scan_count, info_count;
	};
	Image(const Init& init);
};

class ImageDraw : public Draw, NoCopy {
	Size    size;

	struct  Section {
		HDC     dc;
		HBITMAP hbmp, hbmpOld;
		RGBA   *pixels;

		void Init(int cx, int cy);
		~Section();
	};

	Section rgb;
	Section a;

	void Init();

public:
	Draw alpha;

	operator Image() const;

	ImageDraw(Size sz);
	ImageDraw(int cx, int cy);
	~ImageDraw();
};


#include "Raster.h"

Image Magnify(const Image& img, int cx, int cy);
Image Adjust(const Image& img,
             int mulr, int addr, int mulg, int addg, int mullb, int addb, int mula, int adda);
Image Adjust(const Image& img, int mul, int add);
Image Sharpen(const Image& img, int amount = 100);
Image SetColor(const Image& img, Color c);
Image Over(const Image& img, const Image& over, byte alpha = 255);
Image Grayscale(const Image& img);
Image Emboss(const Image& img, Color color = SColorFace, int amount = 256);
Image EmbossTh(const Image& img, Color color, Color colorl, Color colord, int thold = 10);

void  Rescale(RasterEncoder& tgt, Raster& src, const Rect& src_rc);
Image Rescale(const Image& src, int cx, int cy);

#ifdef PLATFORM_WIN32

Image Win32Icon(LPCSTR id, bool cursor = false);
HICON IconWin32(const Image& img, bool cursor = false);

#endif

#endif
