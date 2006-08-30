#pragma BLITZ_APPROVE

//$

#define IMAGE_META(k, v)
#define IMAGE_BEGIN(n) const char *COMBINE(COMBINE(IMAGECLASS, _), n##__scans__)[] = {
#define IMAGE_SCAN(s)  s,
#define IMAGE_PACKED(n, d) };
#include IMAGEFILE
#undef  IMAGE_BEGIN
#undef  IMAGE_SCAN
#undef  IMAGE_PACKED

// -----------------------

#define IMAGE_SCAN(s)
#define IMAGE_BEGIN(n)

// -----------------------


Iml& IMAGECLASS::Iml() {
	static Image::Init init[IMAGECLASS::COUNT] = {
	#define IMAGE_PACKED(n, d) { COMBINE(COMBINE(IMAGECLASS, _), n##__scans__), __countof(COMBINE(COMBINE(IMAGECLASS, _), n##__scans__)), d },
	#include IMAGEFILE
	#undef  IMAGE_PACKED
	};

	static const char *name[IMAGECLASS::COUNT] = {
	#define IMAGE_PACKED(n, d) #n,
	#include IMAGEFILE
	#undef  IMAGE_PACKED
	};
	static ::Iml iml(init, name, COUNT);
	return iml;
}

Image IMAGECLASS::Get(int i)
{
	return Iml().Get(i);
}

Image IMAGECLASS::Get(const char *s)
{
	return Iml().Get(Find(s));
}

int   IMAGECLASS::Find(const String& s)
{
	return Iml().Find(s);
}

int   IMAGECLASS::Find(const char *s)
{
	return Iml().Find(s);
}

void  IMAGECLASS::Set(int i, const Image& m)
{
	Iml().Set(i, m);
}

void  IMAGECLASS::Set(const char *s, const Image& m)
{
	Iml().Set(Find(s), m);
}

struct COMBINE(IMAGECLASS, __Reg) {
	COMBINE(IMAGECLASS, __Reg()) {
		IMAGECLASS::Register__();
	}
};

static COMBINE(IMAGECLASS, __Reg) COMBINE(IMAGECLASS, ___Reg);

#ifndef IMAGE_KEEP
#undef  CLASSNAME
#undef  IMAGEFILE
#endif

#undef  IMAGE_META
