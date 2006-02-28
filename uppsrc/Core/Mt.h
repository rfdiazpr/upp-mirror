#ifdef _MULTITHREADED

#ifdef PLATFORM_WIN32

typedef LONG Atomic;

inline int  AtomicRead(const volatile Atomic& t)      { return t; }
inline void AtomicWrite(volatile Atomic& t, int val)  { t = val; }

inline int  AtomicInc(volatile Atomic& t)             { return InterlockedIncrement((Atomic *)&t); }
inline int  AtomicDec(volatile Atomic& t)             { return InterlockedDecrement((Atomic *)&t); }
inline int  AtomicXAdd(volatile Atomic& t, int incr)  { return InterlockedExchangeAdd((Atomic *)&t, incr); }

class CriticalSection {
protected:
	CRITICAL_SECTION section;
#ifdef _DEBUG
	dword threadid;
#endif

	CriticalSection(int)         {}

public:
	void  Enter();
	void  Leave();

	CriticalSection();
	~CriticalSection()           { DeleteCriticalSection(&section); }

	struct Lock;
};

#ifndef _DEBUG
inline void CriticalSection::Enter()       { EnterCriticalSection(&section); }
inline void CriticalSection::Leave()       { LeaveCriticalSection(&section); }
inline CriticalSection::CriticalSection()  { InitializeCriticalSection(&section); }
#endif

#endif

#ifdef PLATFORM_POSIX

#include <bits/atomicity.h>

typedef _Atomic_word Atomic;

inline int  AtomicRead(const volatile Atomic& t)      { return t; }
inline void AtomicWrite(volatile Atomic& t, int val)  { t = val; }

inline int  AtomicInc(volatile Atomic& t)             { using namespace __gnu_cxx; __atomic_add(&t, 1); return t; }
inline int  AtomicDec(volatile Atomic& t)             { using namespace __gnu_cxx; __atomic_add(&t, -1); return t; }
inline int  AtomicXAdd(volatile Atomic& t, int incr)  { using namespace __gnu_cxx; return __exchange_and_add(&t, incr); }

class CriticalSection {
protected:
	pthread_mutex_t  mutex[1];

	CriticalSection(int)         {}

public:
	void  Enter()             { pthread_mutex_lock(mutex); }
	void  Leave()             { pthread_mutex_unlock(mutex); }

	struct Lock;

	CriticalSection()         { pthread_mutex_init(mutex, NULL); }
	~CriticalSection()        { pthread_mutex_destroy(mutex); }
};

#endif

struct CriticalSection::Lock {
	CriticalSection& s;
	Lock(CriticalSection& s) : s(s) { s.Enter(); }
	~Lock()                         { s.Leave(); }
};

class StaticCriticalSection {
	volatile CriticalSection *section;
	byte                      buffer[sizeof(CriticalSection)];

	void Initialize();

public:
	CriticalSection& Get()         { if(!section) Initialize(); return *const_cast<CriticalSection *>(section); }
	operator CriticalSection&()    { return Get(); }
	void Enter()                   { Get().Enter(); }
	void Leave()                   { Get().Leave(); }
};

#define INTERLOCKED \
for(bool i_b_ = true; i_b_;) \
	for(static StaticCriticalSection i_ss_; i_b_;) \
		for(CriticalSection::Lock i_ss_lock__(i_ss_); i_b_; i_b_ = false)

#define INTERLOCKED_(cs) \
for(bool i_b_ = true; i_b_;) \
	for(CriticalSection::Lock i_ss_lock__(cs); i_b_; i_b_ = false)


#else

typedef int Atomic;

inline int  AtomicRead(const volatile Atomic& t)      { return t; }
inline void AtomicWrite(volatile Atomic& t, int data) { t = data; }

inline int  AtomicInc(volatile Atomic& t)             { ++t; return t; }
inline int  AtomicDec(volatile Atomic& t)             { --t; return t; }
inline int  AtomicXAdd(volatile Atomic& t, int incr)  { Atomic x = t; t += incr; return t; }

class CriticalSection {
public:
	void  Enter()                {}
	void  Leave()                {}

	CriticalSection()            {}
	~CriticalSection()           {}

	struct Lock;
};

typedef CriticalSection StaticCriticalSection;

struct CriticalSection::Lock {
	Lock(CriticalSection&) {}
	~Lock()                {}
};

#define INTERLOCKED
#define INTERLOCKED_(x)

#endif
