#include <Core/Core.h>
#pragma BLITZ_APPROVE

#ifdef COMPILER_MSC
#include <malloc.h>
#else
#include <stdlib.h>
#endif

NAMESPACE_UPP

#ifdef UPP_HEAP

//#define _DEBUG

#define LLOG(x)    // LOG(x)
#define LTIMING(x) // RTIMING(x)

#ifdef CPU_32

#ifdef PLATFORM_WIN32
#define MB_MAP_SIZE 2048
#else
#define MB_MAP_SIZE 4096
#endif

#endif

#ifdef CPU_64
#define MB_MAP_SIZE 16384
#define MB_MAP_MASK (MB_MAP_SIZE - 1)
#endif

#define RESERVE_CHUNK 4 * 1024 * 1024

static const char *sDumpPtr(void *ptr)
{
	static char h[200];
	sprintf(h, "%p", ptr);
	return h;
}

static const char *sDump(dword w)
{
	static char h[4][200];
	static int q;
	q = (q + 1) & 3;
	sprintf(h[q], "%d", w);
	return h[q];
}

int  sSmallKb;

#ifdef CPU_64
int64 sLarge;
#else
int   sLarge;
#endif

MemoryProfile *sPeak;

int MemoryUsedKb()
{
	return sSmallKb + (int) (sLarge >> 10);
}

MemoryProfile *PeakMemoryProfile()
{
	if(sPeak)
		return sPeak;
	sPeak = (MemoryProfile *)MemoryAllocPermanent(sizeof(MemoryProfile));
	memset(sPeak, 0, sizeof(MemoryProfile));
	return NULL;
}

#ifdef PLATFORM_WIN32

static byte **free_raw_page;
static int    free_raw_alloc;
static int    free_raw_count;

byte *Alloc4KBRaw() {
	if(sPeak) *sPeak = MemoryProfile();
	sSmallKb += 4;
	if(free_raw_count) {
		byte *ptr = free_raw_page[--free_raw_count];
		VirtualAlloc(ptr, 4096, MEM_COMMIT, PAGE_READWRITE);
		LLOG("Recommiting page " << sDumpPtr(ptr));
		return ptr;
	}
	static byte *ptr     = NULL;
	static byte *limit   = NULL;
	if(ptr >= limit) {
		ptr = (byte *) VirtualAlloc(NULL, RESERVE_CHUNK, MEM_RESERVE, PAGE_READWRITE);
		limit = ptr + RESERVE_CHUNK;
	}
	VirtualAlloc(ptr, 4096, MEM_COMMIT, PAGE_READWRITE);
	byte *p = ptr;
	ptr += 4096;
	return p;
}

void Free4KBRaw(byte *ptr)
{
	sSmallKb -= 4;
	LLOG("Free4KBRaw " << sDumpPtr(ptr));
	if(free_raw_count >= free_raw_alloc) {
		byte **prev = free_raw_page;
		if(free_raw_alloc)
			free_raw_alloc *= 2;
		else
			free_raw_alloc = 100000;
		free_raw_page = (byte **)malloc(sizeof(byte *) * free_raw_alloc);
		if(prev) {
			memcpy(free_raw_page, prev, free_raw_count * sizeof(byte *));
			free(prev);
		}
	}
	free_raw_page[free_raw_count++] = ptr;
	VirtualFree(ptr, 4096, MEM_DECOMMIT);
}


#else

byte *Alloc4KBRaw() {
	if(sPeak) *sPeak = MemoryProfile();
	sSmallKb += 4;
	static unsigned chunk = 64 * 1024;
	static byte *ptr   = NULL;
	static byte *limit = NULL;
	if(ptr >= limit) {
		size_t a = (size_t)malloc(chunk);
		limit = (byte *)((a + chunk) & ~4095);
		ptr = (byte *)((a | 4095) + 1);
		chunk = min(2 * chunk, 512 * 1024U);
	}

	byte *p = ptr;
	ptr += 4096;
	return p;
}

#endif

//----------------------------------------------------------------

static StaticCriticalSection sHeapLock;

void *MemoryAllocPermanentRaw(size_t size)
{
	if(size >= 256)
		return malloc(size);
	static byte *ptr = NULL;
	static byte *limit = NULL;
	if(ptr + size >= limit) {
		ptr = Alloc4KBRaw();
		limit = ptr + 4096;
	}
	void *p = ptr;
	ptr += size;
	return p;
}


void *MemoryAllocPermanent(size_t size)
{
#ifdef _MULTITHREADED
	CriticalSection::Lock __(sHeapLock);
#endif
	return MemoryAllocPermanentRaw(size);
}

static int page_count;
static int small_count;
static int recycled;
static int allocated;
static int large;

struct BlockInfo {
	BlockInfo *link;
};

struct PageLink {
	PageLink  *bucket_next;
	PageLink  *bucket_prev;
	PageLink  *free_next;
	PageLink  *free_prev;

	void LinkSelf() {
		bucket_next = bucket_prev = free_next = free_prev = this;
	}
	void UnlinkBucket() {
		bucket_prev->bucket_next = bucket_next;
		bucket_next->bucket_prev = bucket_prev;
		bucket_next = bucket_prev = this;
	}
	void InsertBucket(PageLink *lnk) {
		lnk->bucket_prev = this;
		lnk->bucket_next = bucket_next;
		bucket_next->bucket_prev = lnk;
		bucket_next = lnk;
	}
	void UnlinkFree() {
		free_prev->free_next = free_next;
		free_next->free_prev = free_prev;
		free_next = free_prev = this;
	}
	void InsertFree(PageLink *lnk) {
		lnk->free_prev = this;
		lnk->free_next = free_next;
		free_next->free_prev = lnk;
		free_next = lnk;
	}
};

struct PageInfo : public PageLink {
	BlockInfo *freelist;
	byte      *memory;
	word       freecount;
	word       blockcount;
	word       magnitude;
	word       size;

	#ifdef CPU_64
	void      *filler; // fill to 64 bytes
	#endif

	void Format(int m);
};

struct MegaByte {
#ifdef CPU_64
	int64     mbi;
	MegaByte *next;
#endif
	PageInfo *page[256];
};

MegaByte    unknown_megabyte[1]
#ifdef CPU_64
 = { -1 }
#endif
;

MegaByte   *memory_map[MB_MAP_SIZE];

void HeapPanic(const char *text, void *pos, int size)
{
	RLOG("\n\n" << text << "\n");
	char h[200];
	sprintf(h, "MemoryWatchFree(%p);\n", pos);
	RLOG(h);
	HexDump(VppLog(), pos, size, 64);
	Panic(text);
}

#ifdef _DEBUG

void FreeFill(void *ptr, int count)
{
	dword *w = (dword *)ptr + 1;
#ifdef CPU_64
	w++;
	count--;
#endif
	while(--count)
		*w++ = 0x65657246;
}

void FreeCheck(PageInfo *page)
{
	uintptr_t pos = (uintptr_t)page->freelist - (uintptr_t)page->memory;
	if(pos >= 4096 || pos % page->size) {
		VppLog() << "\n\nHeap is corrupted !\n";
		Panic("Heap is corrupted !");
	}
	dword *w = (dword *)page->freelist + 1;
	int count = page->magnitude;
#ifdef CPU_64
	w++;
	count--;
#endif
	while(--count)
		if(*w++ != 0x65657246)
			HeapPanic("Writes to freed blocks detected", page->freelist, page->size);
}

#endif

void PageInfo::Format(int m) {
	LTIMING("Format");
	ASSERT(m > 0 && m <= 256);
	magnitude = m;
	size = 4 * m;
#ifdef _DEBUG
	FreeFill(memory, 1024);
#endif
	byte *p = memory;
	byte *e = memory + 4096 - size;
	blockcount = 0;
	freelist = NULL;
	while(p <= e) {
		((BlockInfo *)p)->link = freelist;
		freelist = (BlockInfo *)p;
		p += size;
		blockcount++;
	}
	freecount = blockcount;
}

static PageLink *free_info = NULL;

PageInfo *NewPage(int magnitude)
{
	LTIMING("NewPage");
	LLOG("Acquiring new page");
	allocated++;

	if(!free_info) {
		page_count++;
		PageInfo *ptr = (PageInfo *) Alloc4KBRaw();
		PageInfo *limit = 4096 / sizeof(PageInfo) + ptr;
		while(ptr < limit) {
			ptr->free_next = free_info;
			free_info = ptr;
			ptr++;
		}
	}

	PageInfo *p = (PageInfo *)free_info;
	free_info = free_info->free_next;
	p->memory = Alloc4KBRaw();
	intptr_t pgi = ((uintptr_t)p->memory >> 12);
	intptr_t mbi = pgi >> 8;
	pgi &= 255;
#ifdef CPU_32
	MegaByte *m = memory_map[mbi];
	if(m == unknown_megabyte) {
		memory_map[mbi] = m = (MegaByte *)MemoryAllocPermanentRaw(sizeof(MegaByte));
		memset(m, 0, sizeof(MegaByte));
	}
#endif
#ifdef CPU_64
	int64 mi = mbi & MB_MAP_MASK;
	MegaByte *m = memory_map[mi];
	for(;;) {
		if(m->mbi == mbi)
			break;
		if(m->mbi == -1) {
			m = (MegaByte *)MemoryAllocPermanentRaw(sizeof(MegaByte));
			memset(m, 0, sizeof(MegaByte));
			m->mbi = mbi;
			m->next = memory_map[mi];
			memory_map[mi] = m;
			break;
		}
		m = m->next;
	}
#endif
	m->page[pgi] = p;
	page_count++;
	return p;
}

const char *Dump(PageInfo *page)
{
	static char h[1024];
	sprintf(h, "%p at %p size %3d, freecount/blockcount: %d/%d",
	        page, page->memory, page->size, page->freecount, page->blockcount);
	return h;
}

#ifdef PLATFORM_WIN32
void ReleasePage(PageInfo *p)
{
	LLOG("Releasing page " << Dump(p));
	Free4KBRaw(p->memory);
	p->free_next = free_info;
	free_info = p;
	intptr_t pgi = ((uintptr_t)p->memory >> 12);
	intptr_t mbi = pgi >> 8;
	pgi &= 255;
#ifdef CPU_32
	memory_map[mbi]->page[pgi] = NULL;
#endif
#ifdef CPU_64
	MegaByte *m = memory_map[mbi];
	for(;;) {
		ASSERT(m->mbi >= 0);
		if(m->mbi == mbi)
			m->page[pgi] = NULL;
		m = m->next;
	}
#endif
	page_count--;
}
#endif

#define InitLink2(a, n)         { a + n, a + n, a + n, a + n }, \
                                { a + n + 1, a + n + 1, a + n + 1, a + n + 1 }
#define InitLink4(a, n)         InitLink2(a, n),   InitLink2(a,   n + 2)
#define InitLink8(a, n)         InitLink4(a, n),   InitLink4(a,   n + 4)
#define InitLink16(a, n)        InitLink8(a, n),   InitLink8(a,   n + 8)
#define InitLink32(a, n)        InitLink16(a, n),  InitLink16(a,  n + 16)
#define InitLink64(a, n)        InitLink32(a, n),  InitLink32(a,  n + 32)
#define InitLink128(a, n)       InitLink64(a, n),  InitLink64(a,  n + 64)

PageLink bucket[257] = {
	InitLink128(bucket, 0),
	InitLink128(bucket, 128),
	{ bucket + 256, bucket + 256, bucket + 256, bucket + 256 }
};

PageLink free_bucket[257] = {
	InitLink128(free_bucket, 0),
	InitLink128(free_bucket, 128),
	{ free_bucket + 256, free_bucket + 256, free_bucket + 256, free_bucket + 256 }
};

PageLink emptylist[1] = {
	{ emptylist, emptylist, emptylist, emptylist }
};

word size_magnitude[1025];

void GenerateMagnitudeTable(int *s)
{
	int size = 0;
	int magnitude = *s / 4;
	while(size <= 1024) {
		ASSERT(*s);
		if(size > *s) {
			s++;
			magnitude = *s / 4;
		}
		size_magnitude[size] = magnitude;
		size++;
	}
	ASSERT(*s == 1024);
}

static int sPgc;

void MemoryProbeRaw(const char *name, dword flags);

static inline void sInitTables()
{
	if(page_count < sPgc) return;
	LTIMING("sInitTables");
	int *mtab;
#ifdef CPU_32
	static int coarse_map[] = {
		4, 8, 16, 32, 64, 96, 128, 192, 256, 512, 1024
	};
	static int medium_map[] = {
		4, 8, 16, 24, 32, 48, 64, 80, 96, 112, 128, 144, 176, 192, 256, 336, 448, 512, 672, 1024
	};
	static int fine_map[] = {
		4, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128, 144, 160, 176,
		192, 224, 256, 288, 304, 336, 368, 408, 448, 512, 672, 816, 1024
	};
#endif
#ifdef CPU_64
	static int coarse_map[] = {
		8, 16, 32, 64, 96, 128, 192, 256, 512, 1024
	};
	static int medium_map[] = {
		8, 16, 24, 32, 48, 64, 80, 96, 112, 128, 144, 176, 192, 256, 336, 448, 512, 672, 1024
	};
	static int fine_map[] = {
		8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128, 144, 160, 176,
		192, 224, 256, 288, 312, 336, 368, 408, 448, 512, 672, 816, 1024
	};
#endif
	if(page_count < 20) {
		sPgc = 20;
		mtab = coarse_map;
	}
	else
	if(page_count < 200) {
		sPgc = 200;
		mtab = medium_map;
	}
	else {
		sPgc = INT_MAX;
		mtab = fine_map;
	}
	GenerateMagnitudeTable(mtab);
}

size_t HeapRoundUp(size_t size) {
	LTIMING("HeapRoundUp");
	if(size <= 1024) {
		ONCELOCK {
			CriticalSection::Lock __(sHeapLock);
			sInitTables();
		}
		return size_magnitude[size] * 4;
	}
	return size;
}

void *MemoryAlloc(size_t size)
{
#ifdef _MULTITHREADED
	CriticalSection::Lock __(sHeapLock);
#endif
	static bool init;
	LTIMING("Alloc");
	LLOG("Alloc(" << sDump(size) << ")");

	if(size <= 1024) {
#ifdef _DEBUG
		small_count++;
#endif
		int m = size_magnitude[size];
		PageLink& bm = bucket[m];
		PageInfo *page = (PageInfo *)bm.bucket_next;
		BlockInfo *q;
		if(page != &bm) {
			ASSERT(page->freecount > 0 && page->freecount < page->blockcount);
			LLOG("Allocating from page " << Dump(page));
#ifdef _DEBUG
			FreeCheck(page);
#endif
			q = page->freelist;
			page->freelist = q->link;
			if(--page->freecount == 0) {
				LLOG("Page exhausted " << Dump(page));
				page->UnlinkBucket();
			}
			return q;
		}
		page = (PageInfo *)free_bucket[m].bucket_next;
		if(page != &free_bucket[m]) {
			LLOG("Next empty bucket page " << Dump(page));
			ASSERT(page->freecount == page->blockcount);
			page->UnlinkFree();
			page->UnlinkBucket();
			bm.InsertBucket(page);
#ifdef _DEBUG
			FreeCheck(page);
#endif
			q = page->freelist;
			page->freelist = q->link;
			if(--page->freecount == 0)
				page->UnlinkBucket();
			return q;
		}
		page = (PageInfo *)emptylist->free_next;
		if(page != emptylist) {
			ASSERT(page->freecount == page->blockcount);
			LLOG("Recycling page " << Dump(page));
			page->UnlinkFree();
			page->UnlinkBucket();
			recycled++;
		}
		else {
			sInitTables();
			if(!init) {
				for(int i = 0; i < MB_MAP_SIZE; i++)
					memory_map[i] = unknown_megabyte;
				init = true;
				LLOG("PageInfo size: " << sDump(sizeof(PageInfo)));
				LLOG("MegaByte size: " << sDump(sizeof(MegaByte)));
			}
			m = size_magnitude[size];
			page = NewPage(m);
		}
		page->Format(m);
		LLOG("Formated page " << Dump(page));
		bucket[m].InsertBucket(page);
#ifdef _DEBUG
		FreeCheck(page);
#endif
		q = page->freelist;
		page->freelist = q->link;
		page->freecount--;
		return q;
	}
	void *q = malloc(size);
	sInitTables();
	if(!init) {
		for(int i = 0; i < MB_MAP_SIZE; i++)
			memory_map[i] = unknown_megabyte;
		init = true;
		LLOG("PageInfo size: " << sDump(sizeof(PageInfo)));
		LLOG("MegaByte size: " << sDump(sizeof(MegaByte)));
	}
	large++;
#ifdef COMPILER_MSC
	sLarge += _msize(q);
#endif
	if(!q)
		Panic("Out of memory");
	return q;
}

void MemoryFree(void *ptr)
{
	LTIMING("Free");
	if(!ptr) return;
#ifdef _MULTITHREADED
	CriticalSection::Lock __(sHeapLock);
#endif

	LLOG("Free(" << sDumpPtr(ptr) << ")");

#ifdef CPU_32
	int pgi = ((dword)ptr >> 12);
	PageInfo *page = memory_map[pgi >> 8]->page[pgi & 255];
	if(!page) {
	#ifdef COMPILER_MSC
		sLarge -= _msize(ptr);
	#endif
		free(ptr);
		large--;
		return;
	}
#endif

#ifdef CPU_64
	int64 pgi = ((uint64)ptr >> 12);
	int64 mbi = pgi >> 8;
	MegaByte *m = memory_map[mbi & MB_MAP_MASK];
	PageInfo *page;
	for(;;) {
		if(m->mbi == mbi) {
			page = m->page[pgi & 255];
			if(!page) {
				free(ptr);
				large--;
				return;
			}
			break;
		}
		if(m->mbi == -1) {
		#ifdef PLATFORM_WIN32
			sLarge -= _msize(ptr);
		#endif
			free(ptr);
			large--;
			return;
		}
		m = m->next;
	}
#endif

#ifdef _DEBUG
	small_count--;
	if(((uintptr_t)ptr & 4095) % page->size) {
		VppLog() << "\n\rInvalid ptr " << ptr << " deallocated !\r\n";
		Panic("Invalid ptr deallocated !");
	}
	FreeFill(ptr, page->magnitude);
#endif
	BlockInfo *b = (BlockInfo *)ptr;
	b->link = page->freelist;
	page->freelist = b;
	if(page->bucket_next == page) {
		LLOG("Putting to bucket " << Dump(page));
		bucket[page->magnitude].InsertBucket(page);
	}
	LLOG("Freeing in page " << Dump(page));
	if(++page->freecount == page->blockcount) {
		page->UnlinkBucket();
		free_bucket[page->magnitude].InsertBucket(page);
		emptylist->InsertFree(page);
		LLOG("Moving to empty " << Dump(page));
	}
	ASSERT(page->freecount <= page->blockcount);
}

void MemoryShrink()
{
#ifdef PLATFORM_WIN32
#ifdef _MULTITHREADED
	CriticalSection::Lock __(sHeapLock);
#endif
	for(int i = 0; i < MB_MAP_SIZE; i++) {
		MegaByte *m = memory_map[i];
	#ifdef CPU_32
		if(memory_map[i] != unknown_megabyte)
	#endif
	#ifdef CPU_64
		for(; m != unknown_megabyte; m = m->next)
	#endif
			for(int j = 0; j < 256; j++) {
				PageInfo *page = memory_map[i]->page[j];
				if(page && page->freecount == page->blockcount) {
					page->UnlinkFree();
					page->UnlinkBucket();
					ReleasePage(page);
				}
			}
	}
#endif
}

void MemoryCheck()
{
	MemoryFree(MemoryAlloc(5));
#ifdef _MULTITHREADED
	CriticalSection::Lock __(sHeapLock);
#endif
	for(int i = 0; i < MB_MAP_SIZE; i++) {
		MegaByte *m = memory_map[i];
	#ifdef CPU_32
		if(memory_map[i] != unknown_megabyte)
	#endif
	#ifdef CPU_64
		for(; m != unknown_megabyte; m = m->next)
	#endif
			for(int j = 0; j < 256; j++) {
				PageInfo *page = memory_map[i]->page[j];
				if(page) {
					BlockInfo *l = page->freelist;
					while(l) {
						uintptr_t pos = (uintptr_t)l - (uintptr_t)page->memory;
						if(pos >= 4096 || pos % page->size) {
							VppLog() << "\n\nHeap is corrupted !\n";
							Panic("Heap is corrupted !");
						}
#ifdef _DEBUG
						dword *w = (dword *)page->freelist + 1;
						int count = page->magnitude;
					#ifdef CPU_64
						count--;
						w++;
					#endif
						while(--count)
							if(*w++ != 0x65657246)
								HeapPanic("Writes to freed blocks detected",
								          page->freelist, page->size);
						l = l->link;
#endif
					}
				}
			}
	}
}

MemoryProfile::MemoryProfile()
{
	memset(this, 0, sizeof(MemoryProfile));
#ifdef CPU_32
	for(int i = 0; i < MB_MAP_SIZE; i++)
		if(memory_map[i] != unknown_megabyte)
			for(int j = 0; j < 256; j++) {
				PageInfo *page = memory_map[i]->page[j];
				if(page)
					if(page->freecount == page->blockcount)
						freepages++;
					else {
						fragmented[page->size / 4] += page->freecount;
						allocated[page->size / 4] += page->blockcount - page->freecount;
					}
			}

#if defined(COMPILER_MSC) && !defined(PLATFORM_WINCE) //!!MINGW
	_HEAPINFO hinfo;
	int heapstatus;
	hinfo._pentry = NULL;
	while( ( heapstatus = _heapwalk( &hinfo ) ) == _HEAPOK ) {
		if(hinfo._useflag != _USEDENTRY) {
			large_free_total += hinfo._size;
			if(large_free_count < 1024)
				large_free_size[large_free_count] = hinfo._size;
			large_free_count++;
		}
		else {
			large_total += hinfo._size;
			if(large_count < 1024)
				large_size[large_count] = hinfo._size;
			large_count++;
		}
	}
#endif
#endif
}

#endif

END_UPP_NAMESPACE
