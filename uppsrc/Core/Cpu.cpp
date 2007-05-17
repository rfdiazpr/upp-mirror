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
	dword info1;
	dword info2;
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
	__asm {
		mov eax, 1
		cpuid
		mov info1, edx
		mov info2, ecx
	}
#else
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

#endif

END_UPP_NAMESPACE
