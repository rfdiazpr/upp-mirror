#pragma BLITZ_APPROVE

// --- Names

/*
#ifdef DLLAUTOLOAD

#define FN_CN(retval, call, fn, args, name) \
	static void fn##__stub() \
	{ \
		DLLSTRUCT.Force(); \
		__asm mov esp, ebp \
		__asm pop ebp \
		__asm jmp [DLLSTRUCT]T_##DLLSTRUCT.fn \
	} \
	DLLTYPE::fn##_type *DLLTYPE::fn = reinterpret_cast<retval (*) args>(fn##_stub);

#endif
*/

// --- Data init

DLLTYPE& DLIMODULE()
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
		handle = LoadDll(LibName(), name, proc);
	}
	return handle;
}

void DLLTYPE::Force()
{
	if(!DLLTYPE::Load()) {
		RLOG("Failed to load required dynamic library '" << LibName() << "'");
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
