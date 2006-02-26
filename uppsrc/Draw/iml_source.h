#pragma BLITZ_APPROVE

//////////////////////////////////////////////////////////////////////
// graphics/iml_source: image list source generator

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
	#define IMAGE_REGISTER(n) static ImageCache::RegisterName IMAGE_RNAME(n)(IMAGE_NAME(n), IMAGE_REF(n)());
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
