#ifndef _nImage_Image_h_
#define _nImage_Image_h_

#include <CtrlLib/CtrlLib.h>

#define Image NewImage

struct RGBA {
	byte b, g, r, a;
};

enum ImageKind {
	IMAGE_UNKNOWN,
	IMAGE_EMPTY,
	IMAGE_ALPHA,
	IMAGE_MASK,
	IMAGE_OPAQUE,
};

class Image;

class ImageBuffer {
	int          kind;
	Size         size;
	Buffer<RGBA> pixels;

	friend class Image;

public:
	RGBA *operator[](int i)             { ASSERT(i >= 0 && i < size.cy); return ~data + size.cx * (size.cy - i - 1); }
	const RGBA *operator[](int i) const { ASSERT(i >= 0 && i < size.cy); return ~data + size.cx * (size.cy - i - 1); }
	int   GetLineDelta()() const        { return size.cx; }
	int   Kind(int k)                   { kind = k; }
	int   GetKind();

	ImageBuffer(Size sz, int kind = IMAGE_UNKNOWN);
	ImageBuffer(pick_ ImageBuffer& b, int newkind = IMAGE_UNKNOWN);
	ImageBuffer(Image& img, int newkind = IMAGE_UNKNOWN);
};

class Image {
	struct Data {
		Atomic refcount;
		void   Retain()  { AtomicInc(refcount); }
		void   Release() { if(AtomicDec(refcount) == 0) delete this; }

		HBITMAP     hbmp;
		HBITMAP     hmask;
		RGBA       *section;
		int         linedelta;
		ImageBuffer buffer;
		int         paintcount;

		void        SysRelease();
		void        Paint(Draw& w, int x, int y);

		Data(pick_ ImageBuffer& b);
		~Data();
	};

	Data *data;

	friend class ImageBuffer;

public:
	void Paint(Draw& w, int x, int y);

	void Clear();

	Image& operator=(const Image& img);

	Image();
	Image(pick_ ImageBuffer& b);
}

#endif
