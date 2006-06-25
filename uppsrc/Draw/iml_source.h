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

/*

//////////////////////////////////////////////////////////////////////
// graphics/Iml_source: image list source generator

//$-

#define IMAGE_OLDINIT(n, x, y) \
	static Image::OldInit COMBINE(n, _desc) = { x, y, __countof(IMAGE_SCANS(n)), IMAGE_SCANS(n) };
#define IMAGE_NEWINIT(n, d) \
	static const char COMBINE(n, _init)[] = d; \
	static Image::Init COMBINE(n, _desc) = \
	{ \
		IMAGE_SCANS(n), COMBINE(n, _init), __countof(IMAGE_SCANS(n)), sizeof(COMBINE(n, _init)) \
	};
#define IMAGE_TAIL(n, x, y) \
	n() \
	{ \
		IMAGE_OLDINIT(n, x, y); \
		static Image i = COMBINE(n, _desc); \
		return i; \
	} \
	IMAGE_REGISTER(n)
#define IMAGE_PTAIL(n, d) \
	n() \
	{ \
		IMAGE_NEWINIT(n, d); \
		static Image i = COMBINE(n, _desc); \
		return i; \
	} \
	IMAGE_REGISTER(n)
#ifdef  IMAGENONAMES
	#define IMAGE_REGISTER(n)
#else//IMAGENONAMES
	#define IMAGE_REGISTER(n) \
		INITBLOCK { RegisterImageName__(IMAGE_NAME(n), IMAGE_REF(n)); } // Improve (once per iml, use vector)
#endif//IMAGENONAMES

#define IMAGE_NAMEOF(n)    IMAGE_NAMEOF_(n)
#define IMAGE_NAMEOF_(n)   #n
#define IMAGE_RNAME_(m, n) IMAGE_RNAME__(m, n)
#define IMAGE_RNAME__(m, n) reg_##m##_##n
#if defined(IMAGECLASS)
	#define IMAGE_SCANS(sn)    COMBINE(COMBINE(IMAGECLASS, _), COMBINE(sn, _scans))
	#define IMAGE_END(n, x, y) }; const Image& IMAGECLASS::IMAGE_TAIL(n, x, y);
	#define IMAGE_PACKED(n, d) }; const Image& IMAGECLASS::IMAGE_PTAIL(n, d);
	#define IMAGE_NAME(n)      IMAGE_NAMEOF(IMAGECLASS::n)
	#define IMAGE_REF(n)       IMAGECLASS::n
	#define IMAGE_RNAME(n)     IMAGE_RNAME_(IMAGECLASS, n)
#elif defined(IMAGEOBJECT)
	#define IMAGE_SCANS(sn)    COMBINE(__, COMBINE(sn, _scans))
	#define IMAGE_END(n, x, y) }; IMAGE_OLDINIT(n, x, y); o.n = COMBINE(n, _desc);
	#define IMAGE_PACKED(n, d) }; IMAGE_NEWINIT(n, d);    o.n = COMBINE(n, _desc);
	#define IMAGE_NAME(n)      IMAGE_NAMEOF(IMAGEOBJECT::n)
	#define IMAGE_REF(n)       IMAGEOBJECT().n
	#define IMAGE_RNAME(n)     IMAGE_RNAME_(IMAGEOBJECT, n)
	const COMBINE(IMAGEOBJECT, _cls)& IMAGEOBJECT()
	{
		static bool inited = false;
		static StaticCriticalSection sct;
		static COMBINE(IMAGEOBJECT, _cls) o;
		if(!inited) {
			CriticalSection::Lock cs(sct);
			if(!inited) {
				inited = true;
#elif defined(IMAGESPACE)
	#define IMAGE_SCANS(sn)    COMBINE(COMBINE(IMAGESPACE, _), COMBINE(sn, _scans))
	#define IMAGE_END(n, x, y) }; const Image& IMAGE_TAIL(n, x, y);
	#define IMAGE_PACKED(n, d) }; const Image& IMAGE_PTAIL(n, d);
	#define IMAGE_NAME(n)      IMAGE_NAMEOF(IMAGESPACE::n)
	#define IMAGE_REF(n)       IMAGESPACE::n
	#define IMAGE_RNAME(n)     IMAGE_RNAME_(IMAGESPACE, n)
	namespace IMAGESPACE
	{
#else
	#define IMAGE_SCANS(sn)    COMBINE(__, COMBINE(sn, _scans))
	#define IMAGE_NAME(n)      IMAGE_NAMEOF(n)
	#define IMAGE_RNAME(n)     IMAGE_RNAME_(_, n)
	#if defined(IMAGESTATIC)
		#define IMAGE_REF(n)       n
		#define IMAGE_END(n, x, y) }; static const Image& IMAGE_TAIL(n, x, y);
		#define IMAGE_PACKED(n, d) }; static const Image& IMAGE_PTAIL(n, d);
	#else
		#define IMAGE_REF(n)       n
		#define IMAGE_END(n, x, y) }; const Image& IMAGE_TAIL(n, x, y);
		#define IMAGE_PACKED(n, d) }; const Image& IMAGE_PTAIL(n, d);
	#endif//IMAGESTATIC
#endif//CLASS switch

#define IMAGE_META(k, v)
#define IMAGE_BEGIN(n) static const char *const IMAGE_SCANS(n)[] = {
#define IMAGE_SCAN(s) s,

#include IMAGEFILE

#ifdef IMAGEOBJECT
			}
		}
		return o;
	}
#endif//IMAGEOBJECT

#ifdef IMAGEVECTOR
	#undef IMAGE_BEGIN
	#undef IMAGE_SCAN
	#undef IMAGE_END
	#undef IMAGE_PACKED
	#define IMAGE_BEGIN(n) IMAGE_REF(n)(),
	#define IMAGE_SCAN(s)
	#define IMAGE_END(n, x, y)
	#define IMAGE_PACKED(n, d)

	#ifdef IMAGESTATIC
		static
	#endif//IMAGESTATIC
	Image IMAGE_REF(IMAGEVECTOR)[] =
	{
	#include IMAGEFILE
		Image()
	};

#endif//IMAGEVECTOR

#ifdef IMAGESPACE
	} // end of namespace
#endif//IMAGESPACE

#undef IMAGE_META
#undef IMAGE_BEGIN
#undef IMAGE_SCAN
#undef IMAGE_END
#undef IMAGE_PACKED
#undef IMAGE_TAIL
#undef IMAGE_PTEXT
#undef IMAGE_PTAIL
#undef IMAGE_NAMEOF
#undef IMAGE_NAMEOF_
#undef IMAGE_RNAME
#undef IMAGE_RNAME_
#undef IMAGE_RNAME__
#undef IMAGE_NAME
#undef IMAGE_REF
#undef IMAGE_REGISTER
#undef IMAGE_SCANS

// kill modifier macros when asked to
#ifndef IMAGE_KEEP
	#undef IMAGEFILE

	#ifdef IMAGECLASS
		#undef IMAGECLASS
	#endif//IMAGECLASS

	#ifdef IMAGEOBJECT
		#undef IMAGEOBJECT
	#endif//IMAGEOBJECT

	#ifdef IMAGESPACE
		#undef IMAGESPACE
	#endif//IMAGESPACE

	#ifdef IMAGENONAMES
		#undef IMAGENONAMES
	#endif//IMAGENONAMES

	#ifdef IMAGEVECTOR
		#undef IMAGEVECTOR
	#endif//IMAGEVECTOR

	#ifdef IMAGESTATIC
		#undef IMAGESTATIC
	#endif//IMAGESTATIC
#endif//IMAGE_KEEP
*/
