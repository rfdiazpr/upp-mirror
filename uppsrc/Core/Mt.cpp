#include "Core.h"

#ifdef _MULTITHREADED

#if defined(_DEBUG) && defined(PLATFORM_WIN32)
void CriticalSection::Enter()
{
	dword tid = GetCurrentThreadId();
	ASSERT(threadid != tid); // The same thread reentrancy not allowed (deadlock in linux)
	EnterCriticalSection(&section);
	threadid = tid;
}

void CriticalSection::Leave()
{
	threadid = ~0;
	LeaveCriticalSection(&section);
}

CriticalSection::CriticalSection()
{
	threadid = ~0;
	InitializeCriticalSection(&section);
}
#endif

#endif

CriticalSection& sCriticalSectionLock()
{
	static CriticalSection *section;
	if(!section) {
		static byte b[sizeof(CriticalSection)];
		section = new(b) CriticalSection;
	}
	return *section;
}

#ifdef _MULTITHREADED
void StaticCriticalSection::Initialize()
{
	CriticalSection::Lock __(sCriticalSectionLock());
	if(!section)
		section = new(buffer) CriticalSection;
}
#endif
