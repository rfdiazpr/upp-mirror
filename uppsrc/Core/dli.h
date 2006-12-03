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

#if !defined(DLI_HEADER) && !defined(DLI_SOURCE)
#define DLI_HEADER
#define DLI_SOURCE
#endif

#ifdef DLI_HEADER
#undef DLI_HEADER

#define FN_CN(retval, call, fn, args, name)\
	typedef retval (call fn##_type) args; fn##_type *fn;

struct DLLTYPE
{
	DLLTYPE();

	void              SetLibName(const char *n) { if(handle) Free(); libname = n; checked = false; }

	bool              Load();
	bool              Load(const char *n)       { SetLibName(n); return Load(); }
	void              Force();
	void              Free();

	operator          bool ()      { return handle; }

#include DLIHEADER
private:
	String            libname;
	DLLHANDLE         handle;
	bool              checked;
};

#undef FN_CN

// ---------

DLLTYPE& DLIMODULE();
DLLTYPE& COMBINE(DLIMODULE, _)();

#endif

#ifdef DLI_SOURCE
#undef DLI_SOURCE

DLLTYPE& DLIMODULE()
{
	static DLLTYPE out;
	out.Load();
	return out;
}

DLLTYPE& COMBINE(DLIMODULE, _)()
{
	static DLLTYPE out;
	return out;
}

DLLTYPE::DLLTYPE()
{
	checked = false;
	handle = 0;
	libname =
#ifdef DLLFILENAME
		DLLFILENAME;
#else
		DLISTR(DLLSTRUCT);
#endif
}

DLLHANDLE LoadDll(String& fn, const char *const *names, void *const *procs);
void      FreeDll(DLLHANDLE dllhandle);

bool DLLTYPE::Load()
{
	if(!checked)
	{
		checked = true;

#define FN_CN(retval, call, fn, args, name)  name,

		const char *name[] =
		{
		#include DLIHEADER
			0
		};

#undef FN_CN

// --- FnPtr map


#define FN_CN(retval, call, fn, args, name)  &fn,

		void *proc[] =
		{
		#include DLIHEADER
			0
		};

		#undef FN_CN

		// --- Proc pointers


//		if(handle = LoadDll(file, name, proc))
//			atexit(&Free);
		handle = LoadDll(libname, name, proc);
	}
	return handle;
}

void DLLTYPE::Force()
{
	if(!DLLTYPE::Load()) {
//		RLOG("Failed to load required dynamic library '" << LibName() << "'");
		exit(1);
	}
}

void DLLTYPE::Free() {
	if(handle) {
		FreeDll(handle);
		handle = 0;
	}
	checked = false;
}
#endif

#undef DLIMODULE

#undef DLIHEADER
#undef DLLTYPE
#undef DLLCALL

#undef FN
#undef FN_C
#undef FN_N

#undef DLISTR__
#undef DLISTR_
#undef DLISTR

#ifdef DLLFILENAME
#undef DLLFILENAME
#endif

#ifdef DLLAUTOLOAD
#undef DLLAUTOLOAD
#endif


#undef FN
#undef FN_C
#undef FN_N

#undef DLLTYPE

#undef DLISTR__
#undef DLISTR_
#undef DLISTR

#undef DLIMODULE

#undef DLIHEADER
#undef DLLCALL

	#ifdef DLLFILENAME
	#undef DLLFILENAME
	#endif

	#ifdef DLLAUTOLOAD
	#undef DLLAUTOLOAD
	#endif
