#define __BREAK__        (*(int *)0 = 0)

#ifdef COMPILER_MSC
	#pragma warning(disable : 4800)
	#pragma warning(disable : 4129)
	#pragma warning(disable : 4290)
	#pragma warning(disable : 4068)
	#pragma warning(disable : 4005)
	#pragma warning(disable : 4675)
	#pragma warning(disable : 4996)

	#pragma setlocale("C")
#endif

void    Panic(const char *msg);

void    AssertFailed(const char *file, int line, const char *cond);

#define VERIFY(x)        ((x) ? (void)0 : AssertFailed(__FILE__, __LINE__, #x))

#ifdef _DEBUG

#define ASSERT(x)        ((x) ? (void)0 : AssertFailed(__FILE__, __LINE__, #x))

#else

#define ASSERT(x)

#endif

#define _cm_  ,

#define __countof(a)          int(sizeof(a) / sizeof(a[0]))

#define __Expand1(x) x(1)
#define __Expand2(x)  __Expand1(x) x(2)
#define __Expand3(x)  __Expand2(x) x(3)
#define __Expand4(x)  __Expand3(x) x(4)
#define __Expand5(x)  __Expand4(x) x(5)
#define __Expand6(x)  __Expand5(x) x(6)
#define __Expand7(x)  __Expand6(x) x(7)
#define __Expand8(x)  __Expand7(x) x(8)
#define __Expand9(x)  __Expand8(x) x(9)
#define __Expand10(x) __Expand9(x) x(10)
#define __Expand11(x) __Expand10(x) x(11)
#define __Expand12(x) __Expand11(x) x(12)
#define __Expand13(x) __Expand12(x) x(13)
#define __Expand14(x) __Expand13(x) x(14)
#define __Expand15(x) __Expand14(x) x(15)
#define __Expand16(x) __Expand15(x) x(16)
#define __Expand17(x) __Expand16(x) x(17)
#define __Expand18(x) __Expand17(x) x(18)
#define __Expand19(x) __Expand18(x) x(19)
#define __Expand20(x) __Expand19(x) x(20)

#define __Expand(x)   __Expand20(x)

#define __List1(x) x(1)
#define __List2(x)  __List1(x), x(2)
#define __List3(x)  __List2(x), x(3)
#define __List4(x)  __List3(x), x(4)
#define __List5(x)  __List4(x), x(5)
#define __List6(x)  __List5(x), x(6)
#define __List7(x)  __List6(x), x(7)
#define __List8(x)  __List7(x), x(8)
#define __List9(x)  __List8(x), x(9)
#define __List10(x) __List9(x), x(10)
#define __List11(x) __List10(x), x(11)
#define __List12(x) __List11(x), x(12)
#define __List13(x) __List12(x), x(13)
#define __List14(x) __List13(x), x(14)
#define __List15(x) __List14(x), x(15)
#define __List16(x) __List15(x), x(16)
#define __List17(x) __List16(x), x(17)
#define __List18(x) __List17(x), x(18)
#define __List19(x) __List18(x), x(19)
#define __List20(x) __List19(x), x(20)

#define E__p(I)       p##I

#define ASSTRING_(x)  #x
#define ASSTRING(x)   ASSTRING_(x)

#define COMBINE__(a, b)         a##b
#define COMBINE(a, b)           COMBINE__(a, b)

#define COMBINE3__(a, b, c)     a##b##c
#define COMBINE3(a, b, c)       COMBINE3__(a, b, c)

#define COMBINE4__(a, b, c, d)  a##b##c##d
#define COMBINE4(a, b, c, d)    COMBINE4__(a, b, c, d)

#define MK__s__(x)       s__s##x
#define MK__s_(x)        MK__s__(x)

#ifdef  flagBLITZ
#define MK__s            MK__s_(COMBINE(BLITZ_INDEX__, __LINE__))
#else
#define MK__s            MK__s_(__LINE__)
#endif

struct Callinit {
	Callinit(void (*fn)()) { fn(); }
};

struct Callexit {
	Callexit(void (*fn)()) { atexit(fn); }
};


// deprecated, use INITBLOCK
#define INITCODE(x) \
static void COMBINE(MK__s, _fn)() { x } static Callinit MK__s(COMBINE(MK__s, _fn));


#define INITBLOCK \
static void COMBINE(MK__s, _fn)(); static Callinit MK__s(COMBINE(MK__s, _fn)); \
static void COMBINE(MK__s, _fn)()

#define INITBLOCK_(x) \
static void COMBINE(x, _fn)(); static Callinit x(COMBINE(x, _fn)); \
static void COMBINE(x, _fn)()


// deprecated, use EXITBLOCK
#define EXITCODE(x) \
static void COMBINE(MK__s, _fn)() { x } static Callexit MK__s(COMBINE(MK__s, _fn));


#define EXITBLOCK \
static void COMBINE(MK__s, _fn)(); static Callexit MK__s(COMBINE(MK__s, _fn)); \
static void COMBINE(MK__s, _fn)()

#define EXITBLOCK_(x) \
static void COMBINE(x, _fn)(); static Callexit x(COMBINE(x, _fn)); \
static void COMBINE(x, _fn)()

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1300)
	template <class T> inline const T& min(const T& a, const T& b) { return a < b ? a : b; }
	template <class T> inline const T& max(const T& a, const T& b) { return a > b ? a : b; }
#else
	#include <algorithm>
	using std::max;
	using std::min;
#endif

template <class T>
inline T minmax(T x, T _min, T _max)                           { return min(max(x, _min), _max); }

typedef unsigned char      byte;
typedef unsigned char      uint8;
typedef signed char        int8;

typedef short unsigned     word;
typedef short int          int16;
typedef short unsigned     uint16;

#ifdef PLATFORM_WIN32
typedef unsigned long      dword;
typedef long               int32;
typedef unsigned long      uint32;
#else
typedef unsigned int       dword;
typedef int                int32;
typedef unsigned int       uint32;
#endif

typedef word               wchar;

#ifdef COMPILER_MSC
typedef __int64            int64;
typedef unsigned __int64   uint64;
#else
typedef long long int      int64;
typedef long long unsigned uint64;
#endif

typedef uint64             qword;

#ifdef COMPILER_MSC
	#define INT64(c) ((int64)COMBINE(c, i64))
#else
	#define INT64(c) ((int64)COMBINE(c, LL))
#endif

#ifndef INT64_MIN
#define INT64_MIN          INT64(-0x8000000000000000)
#endif
#ifndef INT64_MAX
#define INT64_MAX          INT64(+0x7FFFFFFFFFFFFFFF)
#endif

#if !defined(PLATFORM_WIN32)

#define HIBYTE(a)        (byte)((a) >> 8)
#define LOBYTE(a)        byte(a)
#define HIWORD(a)        (word)((a) >> 16)
#define LOWORD(a)        word(a)

#define MAKEWORD(a, b)   ((word)  (((byte) (a)) | ((word) ((byte) (b))) << 8))
#define MAKELONG(a, b)   ((dword) (((word) (a)) | ((dword) ((word) (b))) << 16))

#endif

#ifdef COMPILER_MSC
#define pick_
#else
#define pick_ const
#endif

// fix MSC8 beta problem....
#ifdef COMPILER_MSC
namespace std {
	inline void __cdecl _Debug_message(const wchar_t *, const wchar_t *, unsigned int line) {}
};
#endif

#define BINARY(i, f) \
extern "C" byte *i; \
extern "C" int COMBINE(i, _length);

#define BINARY_ARRAY(i, x, f) \
extern "C" byte *i[]; \
extern "C" int COMBINE(i, _length)[]; \
extern "C" int COMBINE(i, _count);

#define BINARY_MASK(i, m) \
extern "C" byte *i[]; \
extern "C" int COMBINE(i, _length)[]; \
extern "C" int COMBINE(i, _count); \
extern "C" char *COMBINE(i, _files)[];

template <class T>
int StaticTypeNo() {
	int RegisterTypeNo__(const char *type);
	static int typeno = -1;
	if(typeno < 0)
		typeno = RegisterTypeNo__(typeid(T).name());
	return typeno;
}

size_t HeapRoundUp(size_t size);

#ifndef  flagUSEMALLOC
#define UPP_HEAP
#endif

#ifdef UPP_HEAP

void *MemoryAllocPermanent(size_t size);

void *MemoryAlloc(size_t size);
void  MemoryFree(void *ptr);

void  MemoryShrink();


void *MemoryAllocDebug(size_t size);
void  MemoryFreeDebug(void *ptr);

void  MemoryWatch(void *leak);
inline void  MemoryWatch(uintptr_t leak)           { MemoryWatch((void *)leak); }
void  MemoryWatchFree(void *ptr);
inline void  MemoryWatchFree(uintptr_t leak)       { MemoryWatchFree((void *)leak); }

void  MemoryInitDiagnostics();
void  MemoryDumpLeaks();
void  MemoryCheck();

enum MemoryProbeFlags {
	MEMORY_PROBE_FULL    = 1,
	MEMORY_PROBE_FREE    = 2,
	MEMORY_PROBE_MIXED   = 4,
	MEMORY_PROBE_LARGE   = 8,
	MEMORY_PROBE_SUMMARY = 16,
};

void  MemoryProbe(const char *name = NULL, dword flags = 0);
const char *MemoryCounters();

#include <new>

#ifdef _DEBUG

inline void *operator new(size_t size) throw(std::bad_alloc) { void *ptr = MemoryAllocDebug(size); return ptr; }
inline void operator  delete(void *ptr) throw()              { MemoryFreeDebug(ptr); }

inline void *operator new[](size_t size) throw(std::bad_alloc) { void *ptr = MemoryAllocDebug(size); return ptr; }
inline void operator  delete[](void *ptr) throw()              { MemoryFreeDebug(ptr); }

#else

inline void *operator new(size_t size) throw(std::bad_alloc) { void *ptr = MemoryAlloc(size); return ptr; }
inline void operator  delete(void *ptr) throw()              { MemoryFree(ptr); }

inline void *operator new[](size_t size) throw(std::bad_alloc) { void *ptr = MemoryAlloc(size); return ptr; }
inline void operator  delete[](void *ptr) throw()              { MemoryFree(ptr); }

#endif

#else

inline void  *MemoryAllocPermanent(size_t size)                { return malloc(size); }
inline void  *MemoryAlloc(size_t size) { return new byte[size]; }
inline void   MemoryFree(void *p)      { delete[] (byte *) p; }
inline size_t HeapRoundUp(size_t size) { return (size + 3) & -4; }
inline void   MemoryShrink()           {}
inline void   MemoryProbe(const char *name, dword flags) {}
inline const char *MemoryCounters() { return ""; }
inline void   MemoryInitDiagnostics() {}
inline void  *MemoryAllocDebug(dword size) {}
inline void   MemoryFreeDebug(void *p) {}
inline void   MemoryCheck() {}
inline void   MemoryInitDiagnostics() {}

#endif
