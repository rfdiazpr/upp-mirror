#ifdef _MULTITHREADED

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
	int        Wait(int timeout_msec = Null);

	bool       IsOpen() const     { return handle; }

#ifdef PLATFORM_WIN32
	HANDLE      GetHandle() const { return handle; }
#endif
#ifdef PLATFORM_POSIX
	pthread_t   GetHandle() const { return handle; }
#endif

	void        Priority(int percent); // 0 = lowest, 100 = normal

	static void Sleep(int ms);

	static int  GetCount();
	static void ShutdownThreads();
	static bool IsShutdownThreads();


	Thread();
	~Thread()   { Detach(); }

private:
	void operator=(const Thread&);
	Thread(const Thread&);
};

#endif

#ifdef PLATFORM_WIN32

class SyncObject {
protected:
	HANDLE     handle;

public:
	bool       Wait(int time_ms);
	bool       Wait();

	HANDLE     GetHandle() const { return handle; }

	SyncObject();
	~SyncObject();
};

class Event : public SyncObject {
public:
	void       Set();

	Event();
};

class Semaphore : public SyncObject {
public:
	void       Release(int add);

	Semaphore();
};

#endif
