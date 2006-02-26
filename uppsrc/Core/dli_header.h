#pragma BLITZ_APPROVE

#define DLISTR__(s) #s
#define DLISTR_(s)  DLISTR__(s)
#define DLISTR(s)   DLISTR_(s)

#define FN(retval, fn, args)          FN_CN(retval, DLLCALL, fn, args, DLISTR(fn))
#define FN_C(retval, call, fn, args)  FN_CN(retval, call, fn, args, DLISTR(fn))
#define FN_N(retval, fn, args, name)  FN_CN(retval, DLLCALL, fn, args, name)

#ifndef DLLTYPE
#define DLLTYPE   COMBINE(T_, DLIMODULE)
#endif

#ifndef DLLCALL
#ifdef PLATFORM_WIN32
#define DLLCALL //__cdecl
#else
#define DLLCALL
#endif
#endif

#ifdef PLATFORM_WIN32
typedef HMODULE DLLHANDLE;
#else
typedef void   *DLLHANDLE;
#endif

#ifndef DLLSTRUCT
#define DLLSTRUCT DLIMODULE
#endif

// ---------

#define FN_CN(retval, call, fn, args, name)\
	typedef retval (call fn##_type) args; fn##_type *fn;

#define HEADER

struct DLLTYPE
{
	DLLTYPE();

	String&           LibName() { return libname; }

	bool              Load();
	void              Force();
	void              Free();

	operator          bool () { return Load(); }

#include DLIHEADER

	String            libname;
	DLLHANDLE         handle;
	bool              checked;
};

#undef HEADER
#undef FN_CN

// ---------

DLLTYPE& DLIMODULE();

#ifndef DLLKEEP

#undef FN
#undef FN_C
#undef FN_N

#undef DLISTR__
#undef DLISTR_
#undef DLISTR

#undef DLIMODULE

#undef DLIHEADER
#undef DLLTYPE
#undef DLLCALL

	#ifdef DLLFILENAME
	#undef DLLFILENAME
	#endif

	#ifdef DLLAUTOLOAD
	#undef DLLAUTOLOAD
	#endif

#endif
