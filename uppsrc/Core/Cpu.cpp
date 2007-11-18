#include "Core.h"

NAMESPACE_UPP

#ifdef CPU_X86

static bool sHasMMX;
static bool sHasSSE;
static bool sHasSSE2;
static bool sHasSSE3;

static void sCheckCPU()
{
	static bool done;
	if(done) return;
	done = true;
#ifdef PLATFORM_OSX11
//	__asm__("pushl %%ebx\n\tmovl $1, %%eax\n\tcpuid\n\tpopl %%ebx" : "=d" (info1), "=c" (info2) : : "%eax");
	sHasMMX = true;
	sHasSSE = true;
	sHasSSE2 = true;
#else
#ifdef CPU_AMD64
	sHasMMX = true;
	sHasSSE = true;
	sHasSSE2 = true;
#else
#ifdef COMPILER_MSC
	dword info1;
	dword info2;
	__asm {
		mov eax, 1
		cpuid
		mov info1, edx
		mov info2, ecx
	}
#else
	dword info1;
	dword info2;
	__asm__("movl $1, %%eax\n\tcpuid" : "=d" (info1), "=c" (info2) : : "%eax", "%ebx");
#endif
	sHasMMX = ((info1 >> 23) & 0x1);
	sHasSSE = ((info1 >> 25) & 0x1);
	sHasSSE2 = ((info1 >> 26) & 0x1);
	sHasSSE3 = ((info2) & 0x1);
#endif
#endif
}

INITBLOCK {
//	sCheckCPU();
}

bool CPU_MMX()  { sCheckCPU(); return sHasMMX; }
bool CPU_SSE()  { sCheckCPU(); return sHasSSE; }
bool CPU_SSE2() { sCheckCPU(); return sHasSSE2; }
bool CPU_SSE3() { sCheckCPU(); return sHasSSE3; }

#ifdef PLATFORM_POSIX
#ifdef PLATFORM_FREEBSD
#include <unistd.h>
#else
#include <sys/sysinfo.h>
#endif
#endif

int CPU_Cores()
{
	static int n;
	ONCELOCK {
#ifdef PLATFORM_WIN32
#ifdef CPU_64
		uint64 pa, sa;
		GetProcessAffinityMask(GetCurrentProcess(), &pa, &sa);
		for(int i = 0; i < 64; i++)
			n += !!(sa & ((uint64)1 << i));
#else
		DWORD pa, sa;
		GetProcessAffinityMask(GetCurrentProcess(), &pa, &sa);
		for(int i = 0; i < 64; i++)
			n += !!(sa & (1 << i));
#endif
#elif defined(PLATFORM_POSIX)
#ifdef PLATFORM_FREEBSD
		n = minmax((int)sysconf(_SC_NPROCESSORS_ONLN), 1, 256);
#else
		n = minmax(get_nprocs(), 1, 256);
#endif
#else
		n = 1;
#endif
	}
	return n;
}

#endif

#ifdef PLATFORM_WIN32
bool IsDecentMachine()
{
	if(!IsWin2K())
		return false;
	MEMORYSTATUS m;
	GlobalMemoryStatus(&m);
	return m.dwTotalPhys > 500 * 1024 * 1024;

}
#else
bool IsDecentMachine()
{
	return true;
}
#endif

END_UPP_NAMESPACE
