#include "Core.h"

NAMESPACE_UPP

int sMemDiagInitCount = 0;

#if defined(_DEBUG) && defined(UPP_HEAP)

static StaticCriticalSection sHeapLock2;

struct DbgBlkLink {
	DbgBlkLink *prev;
	DbgBlkLink *next;

	void LinkSelf() {
		next = prev = this;
	}
	void Unlink() {
		prev->next = next;
		next->prev = prev;
	}
	void Insert(DbgBlkLink *lnk) {
		lnk->prev = this;
		lnk->next = next;
		next->prev = lnk;
		next = lnk;
	}
};

static void *l_leak[8];
static int   l_count;

void  MemoryWatch(void *leak)
{
	if(l_count < 8)
		l_leak[l_count++] = leak;
}

void _PLACE_DEBUGER_BREAK_HERE_()
{
	LOG("Memory watch...");
}

static void *l_wfree[8];
static int   l_wfcount;

void  MemoryWatchFree(void *ptr)
{
	if(l_wfcount < 8)
		l_wfree[l_wfcount++] = ptr;
}

void _PLACE_DEBUGER_FREE_BREAK_HERE_()
{
	LOG("Memory watch free...");
}

static DbgBlkLink dbg_live = { &dbg_live, &dbg_live };

extern bool PanicMode;

void *MemoryAllocDebug(size_t size)
{
	if(PanicMode)
		return malloc(size);
#ifdef _MULTITHREADED
	sHeapLock2.Enter();
#endif
//	if(!dbg_live.next)
//		dbg_live.LinkSelf();
	size = (size + 7) & 0xfffffff8;
	DbgBlkLink *p = (DbgBlkLink *)MemoryAlloc(size + 2 * sizeof(DbgBlkLink));
	DbgBlkLink *e = (DbgBlkLink *)((byte *)(p + 1) + size);
	dbg_live.Insert(e);
	dbg_live.Insert(p);
	p++;
#ifdef _MULTITHREADED
	sHeapLock2.Leave();
#endif
	for(int i = 0; i < l_count; i++)
		if(l_leak[i] == p) {
			fprintf(stderr, "Allocating leak: %p\n", p);
			fflush(stderr);
			LOG("Leak spot allocation: " << FormatIntHex(p));
			_PLACE_DEBUGER_BREAK_HERE_();
		}
	return p;
}

void HeapPanic(const char *text, void *pos, int size);

void MemoryFreeDebug(void *ptr)
{
	if(PanicMode)
		return;
	if(!ptr) return;
//	LOG("T");
#ifdef _MULTITHREADED
	CriticalSection::Lock __(sHeapLock2);
#endif
	for(int i = 0; i < l_wfcount; i++)
		if(l_wfree[i] == ptr)
			_PLACE_DEBUGER_FREE_BREAK_HERE_();
	DbgBlkLink *p = (DbgBlkLink *)ptr - 1;
	DbgBlkLink *e = p->next;
	if(p != e->prev) {
		sHeapLock2.Leave();
		char h[256];
		sprintf(h, "Heap is corrupted p:%p e->prev:%p", p, e->prev);
		HeapPanic(h, p + 1, (int)((uintptr_t)p->next - (uintptr_t)(p + 1)));
	}
	e->Unlink();
	p->Unlink();
	MemoryFree(p);
}

void MemoryCheckDebug()
{
	MemoryCheck();
	CriticalSection::Lock __(sHeapLock2);
	DbgBlkLink *p = &dbg_live;
	do {
		if(p->prev->next != p || p->next->prev != p) {
			sHeapLock2.Leave();
			HeapPanic("HEAP CHECK: Heap is corrupted", p + 1, (int)((uintptr_t)p->next - (uintptr_t)(p + 1)));
		}
		p = p->next;
	}
	while(p != &dbg_live);
}

void MemoryDumpLeaks()
{
	if(!dbg_live.next || dbg_live.next == dbg_live.prev)
		return;
	BugLog() << "\n\nHeap leaks detected:\n";
	DbgBlkLink *p = dbg_live.next;
	for(;;) {
		BugLog() << Sprintf("MemoryWatch(0x%lX);\n", p + 1);
		HexDump(VppLog(), p + 1, (dword)((uintptr_t)p->next - (uintptr_t)(p + 1)), 64);
		p = p->next;
		if(p == &dbg_live) break;
		p = p->next;
		if(p == &dbg_live) break;
	}
#ifdef PLATFORM_WIN32
#ifdef PLATFORM_WINCE
	MessageBox(::GetActiveWindow(),
	           L"Heap leaks detected !",
	           L"Warning",
	           MB_ICONSTOP|MB_OK|MB_APPLMODAL);
#else
	MessageBox(::GetActiveWindow(),
	           "Heap leaks detected !",
	           "Warning",
	           MB_ICONSTOP|MB_OK|MB_APPLMODAL);
#endif
#else
	static const char leaks[] = "Heap leaks detected!\n";
	write(2, leaks, sizeof(leaks) - 1);
#endif
}

#ifdef COMPILER_MSC

#pragma warning(disable: 4074)
#pragma init_seg(compiler)

EXITBLOCK { MemoryDumpLeaks(); }

#endif

#ifdef COMPILER_GCC
/*
struct MemoryInitDiagCls {
	~MemoryInitDiagCls() { MemoryDumpLeaks(); }
};
*/

void MemoryInitDiagnostics()
{
//	static MemoryInitDiagCls diagcls;
/*
	static bool b;
	if(!b) {
		static
		atexit(MemoryDumpLeaks);
		b = true;
		MemoryWatch(0x417FDE60);
	}
*/
}
#endif

#endif

END_UPP_NAMESPACE
