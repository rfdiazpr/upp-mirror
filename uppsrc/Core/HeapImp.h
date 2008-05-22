#ifndef sHEAPIMP
#define sHEAPIMP

struct FreeLink {
	FreeLink *next;
};

struct MPage {
	byte         klass;
	byte         freecount;
	byte         count;
	byte         filler;
	int          free;
	FreeLink    *freelist;
	MPage       *next;
	MPage       *prev;
#ifdef CPU_32
	byte         align32[12];
#endif

	void         LinkSelf()        { prev = next = this; }
	void         Unlink()          { prev->next = next; next->prev = prev; }
	void         Link(MPage *lnk)  { prev = lnk; next = lnk->next; next->prev = this; lnk->next = this;  }

	void         Format(int k);
};

struct MCache {
	FreeLink  *list;
	int32      count;
};

extern
#ifdef flagMT
#ifdef COMPILER_MSC
__declspec(thread)
#else
__thread
#endif
#endif
MCache mcache[16];

void *MAlloc_Get(MCache& m, int k);
void  MFree_Reduce(MCache& m, int k);

#define NKLASS      16

#define CACHEMAX    31
#define CACHERES    15

#define CACHETH     9

void FreeFill(dword *ptr, int count);
void FreeCheck(dword *ptr, int count);

void  *SysAllocRaw(size_t size);
void   SysFreeRaw(void *ptr, size_t size);

void  *AllocRaw64KB();

extern MemoryProfile *sPeak;
extern bool sWasPeak;

inline void sDoPeakProfile()
{
	if(sWasPeak) {
		*sPeak = MemoryProfile();
		sWasPeak = false;
	}
}

inline void sHeapStat(int k)
{
#ifdef flagHEAPSTAT
	extern int sHeapStats[18];
	INTERLOCKED
		sHeapStats[k]++;
#endif
}

void  *LAlloc(size_t& size);
void   LFree(void *ptr);
void   LMake(MemoryProfile& p);
void   LMemoryShrink();

#endif
