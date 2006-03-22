#ifndef _nImage_Image_h_
#define _nImage_Image_h_

#include <CtrlLib/CtrlLib.h>

#define Image NewImage
#define ImageDraw NewImageDraw

struct RGBA {
	byte b, g, r, a;
};

void AlphaBlend(RGBA *t, RGBA *s, int len, byte const_alpha = 255, Color c = Null);

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
	Buffer<RGBA> pixels;
	Point        hotspot;

	void         Init(Size sz);
	void         Set(Image& img);

	RGBA*        Line(int i) const      { ASSERT(i >= 0 && i < size.cy);
	                                      return (RGBA *)~pixels + size.cx * (size.cy - i - 1); }

	friend class Image;

public:
	void  SetKind(int k)                { kind = k; }
	int   GetKind();

	void  SetHotSpot(Point p)           { hotspot = p; }
	Point GetHotSpot() const            { return hotspot; }

	Size  GetSize() const               { return size; }
	RGBA *operator[](int i)             { return Line(i); }
	const RGBA *operator[](int i) const { return Line(i); }
	int   GetLineDelta() const          { return -size.cx; }
	RGBA *operator~()                   { return pixels; }
	operator RGBA*()                    { return pixels; }
	const RGBA *operator~() const       { return pixels; }
	operator const RGBA*() const        { return pixels; }

//	ImageBuffer()                       { size = Size(0, 0); kind = IMAGE_UNKNOWN; }
	ImageBuffer(int cx, int cy);
	ImageBuffer(Size sz);
	ImageBuffer(Image& img);
	ImageBuffer(ImageDraw& w);
	ImageBuffer(ImageBuffer& b)         { kind = b.kind; size = b.size; pixels = b.pixels; hotspot = b.hotspot; }
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

		ImageBuffer buffer;

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
	void Paint(Draw& w, int x, int y, byte const_alpha = 255, Color c = Null);

	const RGBA*    operator~() const;
	operator const RGBA*() const;

	Size  GetSize() const;
	Point GetHotSpot() const;

	void Clear();

	Image& operator=(const Image& img);
	Image& operator=(ImageBuffer& img);

	Image()              { data = NULL; }
	Image(const Image& img);
	Image(ImageBuffer& b);
	~Image();
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

#ifdef PLATFORM_WIN32

Image Win32Icon(LPCSTR id, bool cursor = false);
HICON IconWin32(Image img, bool cursor = false);

#endif

#endif
