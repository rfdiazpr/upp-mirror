#include "Core.h"
#pragma hdrstop

#ifdef PLATFORM_WIN32
typedef HMODULE DLLHANDLE;
#else
typedef void   *DLLHANDLE;
#endif

#ifdef PLATFORM_WIN32

#include <winnt.h>

class PeFile
{
public:
	PeFile(const char *data = 0) { Open(data); }

	bool                        Open(const char *data);

	int                         GetExportCount() const;
	const char                 *GetExport(int index) const;
	const char                 *FindExportRaw(const char *name, bool case_sensitive = true) const;

	int                         GetSectionIndex(const char *name) const;

private:
	const char                 *data;
	const IMAGE_NT_HEADERS     *headers;
	const IMAGE_SECTION_HEADER *sections;
	const IMAGE_EXPORT_DIRECTORY *exports;
};

bool PeFile::Open(const char *_data)
{
	data = _data;
	if(!data || data[0] != 'M' || data[1] != 'Z')
		return false;
	int pe = PeekIL(data + 0x3C);
	if(IsBadReadPtr(data + pe, sizeof(IMAGE_NT_HEADERS)))
		return false;
	if(memcmp(data + pe, "PE\0\0", 4))
		return false;
	headers = (const IMAGE_NT_HEADERS *)(data + pe);
	sections = (const IMAGE_SECTION_HEADER *)(headers + 1);
	exports = (const IMAGE_EXPORT_DIRECTORY *)(data
		+ headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	return true;
}

int PeFile::GetSectionIndex(const char *name) const
{
	for(int i = 0, n = headers->FileHeader.NumberOfSections; i < n; i++)
		if(!strcmp((const char *)sections[i].Name, name))
			return i;
	return -1;
}

int PeFile::GetExportCount() const
{
	return exports ? exports->NumberOfNames : 0;
}

const char *PeFile::GetExport(int index) const
{
	if(!exports || index < 0 || index >= (int) exports->NumberOfNames)
		return 0;
	return data + ((const dword *)(data + (dword)exports->AddressOfNames))[index];
}

static bool EqualMem(const char *a, const char *b, int len, bool case_sensitive)
{
	if(case_sensitive)
		return !memcmp(a, b, len);
	else
		return !MemICmp(a, b, len);
}

const char *PeFile::FindExportRaw(const char *name, bool case_sensitive) const
{
	if(!exports || !name || !*name)
		return 0;
	int len = strlen(name);
	const dword *pnames = (const dword *)(data + (dword)exports->AddressOfNames);
	for(int i = 0; i < (int) exports->NumberOfNames; i++) {
		const char *exp = data + pnames[i];
		int elen = strlen(exp);
		if(elen < len)
			continue;
		if(elen == len && EqualMem(exp, name, len, case_sensitive))
			return exp;
		if(EqualMem(exp, name, len, case_sensitive) && exp[len] == '@')
			return exp;
		if(exp[0] == '_' && EqualMem(exp + 1, name, len, case_sensitive)
			&& (exp[len + 1] == '@' || exp[len + 1] == 0))
			return exp;
	}
	return 0;
}

HMODULE CheckDll(const char *fn, const char *const *names, Vector<void *>& plist)
{
#ifdef PLATFORM_WINCE
	HMODULE hmod = LoadLibrary(ToSysChrSet(fn));
#else
	HMODULE hmod = LoadLibrary(fn);
#endif
	if(!hmod)
		return 0;

	PeFile pe;
	if(!pe.Open((const char *)hmod)) {
		FreeLibrary(hmod);
		return 0;
	}

	int missing = 0;
	for(const char *const *p = names; *p; p++) {
		const char *name = pe.FindExportRaw(*p);
		void *proc = 0;
#ifdef PLATFORM_WINCE
		if(!name || !(proc = (void *)GetProcAddress(hmod, ToSysChrSet(name))))
#else
		if(!name || !(proc = (void *)GetProcAddress(hmod, name)))
#endif
			missing++;
		plist.Add(proc);
	}
	if(missing) {
		FreeLibrary(hmod);
		return 0;
	}
	return hmod;
}

void FreeDll(HMODULE hmod)
{
	FreeLibrary(hmod);
}

#endif

#ifdef PLATFORM_POSIX

#include <dlfcn.h>

void *CheckDll(const char *fn, const char *const *names, Vector<void *>& plist)
{
	void *hmod = dlopen(fn, RTLD_LAZY);
	if(!hmod) {
		fputs(NFormat("Failed to open DLL %s.\n", fn), stderr);
		fflush(stderr);
		return 0;
	}

	for(const char *const *p = names; *p; p++) {
		void *proc = dlsym(hmod, *p);
		if(!proc) {
			fputs(NFormat("Symbol %s not found in %s.\n", *p, fn), stderr);
			fflush(stderr);
			dlclose(hmod);
			return 0;
		}
		plist.Add(proc);
	}
//	fputs(NFormat("All %d symbols found in library %s.\n", plist.GetCount(), fn), stderr);
//	fflush(stderr);
	return hmod;
}

void FreeDll(void *hmod)
{
	if(hmod)
		dlclose(hmod);
}

#endif//PLATFORM_POSIX

DLLHANDLE LoadDll(String& inoutfn, const char *const *names, void *const *procs)
{
	const char *fn = inoutfn;
	while(*fn) {
		const char *b = fn;
		while(*fn && *fn != ';'
#ifndef PLATFORM_WIN32
			&& *fn != ':'
#endif
		)
			fn++;
		String libname(b, fn);
		if(*fn)
			fn++;
		Vector<void *> plist;
		if(DLLHANDLE hmod = CheckDll(libname, names, plist)) {
			for(int i = 0; i < plist.GetCount(); i++)
				*(void **)*procs++ = plist[i];
			inoutfn = libname;
			return hmod;
		}
	}
	return 0;
}
