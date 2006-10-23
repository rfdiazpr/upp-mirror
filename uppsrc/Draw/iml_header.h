#pragma BLITZ_APPROVE

//$-

#define IMAGE_META(k, v)
#define IMAGE_SCAN(s)
#define IMAGE_PACKED(n, d)

#define IMAGE_BEGIN_DATA
#define IMAGE_DATA(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,aa,ab,ac,ad,ae,af,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,ba,bb,bc,bd,be,bf)
#define IMAGE_END_DATA(n, c)

class IMAGECLASS {
public:
#define IMAGE_BEGIN(n) I_##n,
#define IMAGE_ID(n) I_##n,
	enum {
#include IMAGEFILE
		COUNT
	};
#undef  IMAGE_BEGIN
#undef  IMAGE_ID

public:
	static ::Iml& Iml();

	static void   Register__()                { Register(ASSTRING(IMAGECLASS), Iml()); }

	static int    Find(const String& s);
	static int    Find(const char *s);
	static int    GetCount()                  { return Iml().GetCount(); }
	static String GetId(int i)                { return Iml().GetId(i); }

	static Image  Get(int i);
	static Image  Get(const char *s);
	static Image  Get(const String& s);

	static void   Set(int i, const Image& m);
	static void   Set(const char *s, const Image& m);

	static void   Reset()                     { Iml().Reset(); }

#define IMAGE_BEGIN(n) static Image n() { return Get(I_##n); }
#define IMAGE_ID(n) static Image n() { return Get(I_##n); }
#include IMAGEFILE
#undef  IMAGE_BEGIN
#undef  IMAGE_ID

};

#undef  IMAGE_SCAN
#undef  IMAGE_PACKED
#undef  IMAGE_META

#undef  IMAGE_BEGIN_DATA
#undef  IMAGE_END_DATA
#undef  IMAGE_DATA

#ifndef IMAGE_KEEP
#undef  IMAGECLASS
#undef  IMAGEFILE
#endif
