#pragma BLITZ_APPROVE

//$-

#define IMAGE_META(k, v)
#define IMAGE_SCAN(s)
#define IMAGE_PACKED(n, d)

class IMAGECLASS {
public:
#define IMAGE_BEGIN(n) I_##n,
	enum {
#include IMAGEFILE
		COUNT
	};
#undef  IMAGE_BEGIN

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
#include IMAGEFILE
#undef  IMAGE_BEGIN

};

#undef  IMAGE_SCAN
#undef  IMAGE_PACKED
#undef  IMAGE_META

#ifndef IMAGE_KEEP
#undef  IMAGECLASS
#undef  IMAGEFILE
#endif
