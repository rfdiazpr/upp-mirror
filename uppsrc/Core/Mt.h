#ifndef _DEBUG
inline void AssertST() {}
#endif

#ifdef _MULTITHREADED

class Callback;

class Thread {
#ifdef PLATFORM_WIN32
	HANDLE     handle;
#endif
#ifdef PLATFORM_POSIX
	pthread_t  handle;
#endif

public:
	bool       Run(Callback cb);

	void       Detach();
	int        Wait();

	bool       IsOpen() const     { return handle; }

#ifdef PLATFORM_WIN32
	HANDLE      GetHandle() const { return handle; }
#endif
#ifdef PLATFORM_POSIX
	pthread_t   GetHandle() const { return handle; }
#endif

	void        Priority(int percent); // 0 = lowest, 100 = normal

	static void Start(Callback cb);

	static void Sleep(int ms);

	static bool IsST();
	static int  GetCount();
	static void ShutdownThreads();
	static bool IsShutdownThreads();


	Thread();
	~Thread()   { Detach(); }

private:
	void operator=(const Thread&);
	Thread(const Thread&);
};

#ifdef _DEBUG
inline void AssertST() { ASSERT(Thread::IsST()); }
#endif

void ReadMemoryBarrier();
void WriteMemoryBarrier();

template <class U>
inline bool ReadWithBarrier(const volatile U& b)
{
	volatile bool tmp = b;
	ReadMemoryBarrier();
	return tmp;
}

template <class U, class V>
inline void BarrierWrite(volatile U& ptr, V data)
{
	WriteMemoryBarrier();
	ptr = data;
}


class Semaphore {
#ifdef PLATFORM_WIN32
	HANDLE     handle;
#else
	sem_t      sem;
#endif

public:
	void       Wait();
	void       Release();

	Semaphore();
	~Semaphore();
};

class StaticSemaphore {
	volatile Semaphore *semaphore;
	byte                buffer[sizeof(Semaphore)];

	void Initialize();

public:
	Semaphore& Get()             { if(!ReadWithBarrier(semaphore)) Initialize(); return *const_cast<Semaphore *>(semaphore); }
	operator Semaphore&()        { return Get(); }
	void Wait()                  { Get().Wait(); }
	void Release()               { Get().Release(); }
};

#ifdef PLATFORM_WIN32

typedef LONG Atomic;

inline int  AtomicInc(volatile Atomic& t)             { return InterlockedIncrement((Atomic *)&t); }
inline int  AtomicDec(volatile Atomic& t)             { return InterlockedDecrement((Atomic *)&t); }
inline int  AtomicXAdd(volatile Atomic& t, int incr)  { return InterlockedExchangeAdd((Atomic *)&t, incr); }

class Mutex {
protected:
	CRITICAL_SECTION section;

	Mutex(int)         {}

public:
	void  Enter()                { EnterCriticalSection(&section); }
	void  Leave()                { LeaveCriticalSection(&section); }

	Mutex()                      { InitializeCriticalSection(&section); }
	~Mutex()                     { DeleteCriticalSection(&section); }

	struct Lock;
};

/* Win32 RWMutex implementation by Chris Thomasson, cristom@comcast.net */

class RWMutex {
    LONG   m_count, m_rdwake;
    HANDLE m_wrwset, m_rdwset;
    CRITICAL_SECTION m_wrlock;

public:
	void EnterWrite();
	void LeaveWrite();

	void EnterRead();
	void LeaveRead();

	RWMutex();
	~RWMutex();

	struct ReadLock;
	struct WriteLock;
};

#endif

#ifdef PLATFORM_POSIX

typedef _Atomic_word Atomic;

inline int  AtomicXAdd(volatile Atomic& t, int incr)  { using namespace __gnu_cxx; return __exchange_and_add(&t, incr); }

inline int  AtomicInc(volatile Atomic& t)             { return AtomicXAdd(t, +1) + 1; }
inline int  AtomicDec(volatile Atomic& t)             { return AtomicXAdd(t, -1) - 1; }

/*
inline int  AtomicInc(volatile Atomic& t)             { using namespace __gnu_cxx; __atomic_add(&t, 1); return t; }
inline int  AtomicDec(volatile Atomic& t)             { using namespace __gnu_cxx; __atomic_add(&t, -1); return t; }
*/

class Mutex {
protected:
	pthread_mutex_t  mutex[1];

	Mutex(int)         {}

public:
	void  Enter()      { pthread_mutex_lock(mutex); }
	void  Leave()      { pthread_mutex_unlock(mutex); }

	struct Lock;

	Mutex();
	~Mutex();
};

class RWMutex {
	pthread_rwlock_t rwlock[1];

public:
	void EnterWrite()  { pthread_rwlock_wrlock(rwlock); }
	void LeaveWrite()  { pthread_rwlock_unlock(rwlock); }
	void EnterRead()   { pthread_rwlock_rdlock(rwlock); }
	void LeaveRead()   { pthread_rwlock_unlock(rwlock); }

	RWMutex();
	~RWMutex();

	struct ReadLock;
	struct WriteLock;
};

#endif

inline int  AtomicRead(const volatile Atomic& t)      { return ReadWithBarrier(t); }
inline void AtomicWrite(volatile Atomic& t, int val)  { BarrierWrite(t, val); }

struct Mutex::Lock {
	Mutex& s;
	Lock(Mutex& s) : s(s) { s.Enter(); }
	~Lock()               { s.Leave(); }
};

struct RWMutex::ReadLock {
	RWMutex& s;
	ReadLock(RWMutex& s) : s(s) { s.EnterRead(); }
	~ReadLock()                 { s.LeaveRead(); }
};

struct RWMutex::WriteLock {
	RWMutex& s;
	WriteLock(RWMutex& s) : s(s) { s.EnterWrite(); }
	~WriteLock()                 { s.LeaveWrite(); }
};

class StaticMutex {
	volatile Mutex *section;
	byte            buffer[sizeof(Mutex)];

	void Initialize();

public:
	Mutex& Get()         { if(!Readsection) Initialize(); return *const_cast<Mutex *>(section); }
	operator Mutex&()    { return Get(); }
	void Enter()         { Get().Enter();}
	void Leave()         { Get().Leave(); }
};

class StaticRWMutex {
	volatile RWMutex *rw;
	byte              buffer[sizeof(RWMutex)];

	void Initialize();

public:
	RWMutex& Get()       { if(!rw) Initialize(); return *const_cast<RWMutex *>(rw); }
	operator RWMutex&()  { return Get(); }
	void EnterRead()     { Get().EnterRead();}
	void LeaveRead()     { Get().LeaveRead(); }
	void EnterWrite()    { Get().EnterWrite();}
	void LeaveWrite()    { Get().LeaveWrite(); }
};

#define INTERLOCKED \
for(bool i_b_ = true; i_b_;) \
	for(static UPP::StaticMutex i_ss_; i_b_;) \
		for(UPP::Mutex::Lock i_ss_lock__(i_ss_); i_b_; i_b_ = false)

struct H_l_ : Mutex::Lock {
	bool b;
	H_l_(Mutex& cs) : Mutex::Lock(cs) { b = true; }
};

#define INTERLOCKED_(cs) \
for(UPP::H_l_ i_ss_lock__(cs); i_ss_lock__.b; i_ss_lock__.b = false)

void Set__(volatile bool& b);

#define ONCELOCK \
for(static volatile bool o_b_; !ReadWithBarrier(o_b_);) \
	for(static StaticMutex o_ss_; !o_b_;) \
		for(Mutex::Lock o_ss_lock__(o_ss_); !o_b_; BarrierWrite(o_b_, true))

#define ONCELOCK_(o_b_) \
for(static StaticMutex o_ss_; !ReadWithBarrier(o_b_);) \
	for(Mutex::Lock o_ss_lock__(o_ss_); !o_b_; BarrierWrite(o_b_, true))

#define ONCELOCK_PTR(ptr, init) \
if(!ReadWithBarrier(ptr)) { \
	static StaticMutex cs; \
	cs.Enter(); \
	if(!ptr) \
		BarrierWrite(ptr, init); \
	cs.Leave(); \
}

#else

typedef int Atomic;

inline int  AtomicRead(const volatile Atomic& t)      { return t; }
inline void AtomicWrite(volatile Atomic& t, int data) { t = data; }

inline int  AtomicInc(volatile Atomic& t)             { ++t; return t; }
inline int  AtomicDec(volatile Atomic& t)             { --t; return t; }
inline int  AtomicXAdd(volatile Atomic& t, int incr)  { Atomic x = t; t += incr; return x; }

struct Mutex {
	void  Enter()                {}
	void  Leave()                {}

	struct Lock;
};

typedef Mutex StaticMutex;

struct Mutex::Lock {
	Lock(Mutex&) {}
	~Lock()                {}
};

struct RWMutex {
	void EnterWrite() {}
	void LeaveWrite() {}

	void EnterRead() {}
	void LeaveRead() {}

	struct ReadLock;
	struct WriteLock;
};

struct RWMutex::ReadLock {
	ReadLock(RWMutex&) {}
	~ReadLock()        {}
};

struct RWMutex::WriteLock {
	WriteLock(RWMutex&) {}
	~WriteLock()        {}
};

typedef RWMutex StaticRWMutex;

#define INTERLOCKED
#define INTERLOCKED_(x) { x.Enter(); }

#define ONCELOCK \
for(static bool o_b_; !o_b_; o_b_ = true)

#define ONCELOCK_(o_b_) \
for(; !o_b_; o_b_ = true) \

#define ONCELOCK_PTR(ptr, init) \
if(!ptr) ptr = init;

inline void ReadMemoryBarrier() {}
inline void WriteMemoryBarrier() {}

#ifdef _DEBUG
inline void AssertST() {}
#endif

#endif

typedef Mutex CriticalSection; // deprecated
typedef StaticMutex StaticCriticalSection; // deprecated
