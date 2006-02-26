#include "Core.h"

#ifdef _MULTITHREADED

CriticalSection& sCriticalSectionLock();

Thread::Thread()
{
	sCriticalSectionLock();
#ifdef PLATFORM_WIN32
	handle = 0;
#endif
#ifdef PLATFORM_POSIX
	handle = 0;
#endif
}

void Thread::Detach()
{
#if defined(PLATFORM_WIN32)
	if(handle) {
		CloseHandle(handle);
		handle = 0;
	}
#elif defined(PLATFORM_POSIX)
	if(handle) {
		CHECK(!pthread_detach(handle));
		handle = 0;
	}
#endif
}

#if defined(PLATFORM_WIN32) || defined(PLATFORM_POSIX)
static
#ifdef PLATFORM_WIN32
dword __stdcall
#else
void *
#endif
sThreadRoutine(void *arg)
{
	Callback *cb = (Callback *)arg;
	(*cb)();
	delete cb;
	return 0;
}
#endif

bool Thread::Run(Callback _cb)
{
	Detach();
	Callback *cb = new Callback(_cb);
#ifdef PLATFORM_WIN32
	dword thread_id;
	handle = CreateThread(0, 0, sThreadRoutine, cb, 0, &thread_id);
#endif
#ifdef PLATFORM_POSIX
	if(pthread_create(&handle, 0, sThreadRoutine, cb))
		handle = 0;
#endif
	return handle;
}

int Thread::Wait(int timeout_msec)
{
	if(!IsOpen())
		return -1;
	int out;
#ifdef PLATFORM_WIN32
	dword exit;
	if(!GetExitCodeThread(handle, &exit))
		return -1;
	if(exit != STILL_ACTIVE)
		out = (int)exit;
	else
	{
		if(WaitForSingleObject(handle, IsNull(timeout_msec) ? INFINITE : timeout_msec) != WAIT_OBJECT_0)
			return Null;
		out = GetExitCodeThread(handle, &exit) ? int(exit) : int(Null);
	}
	Detach();
#endif
#ifdef PLATFORM_POSIX
	void *thread_return;
	//!! todo: timed wait
	if(pthread_join(handle, &thread_return))
		out = Null;
	else
		out = (int)(intptr_t)thread_return;
	handle = 0;
#endif
	return out;
}

void Thread::Priority(int percent)
{
	ASSERT(IsOpen());
#ifdef PLATFORM_WIN32
	int prior;
	if(percent <= 25)
		prior = THREAD_PRIORITY_LOWEST;
	else if(percent <= 75)
		prior = THREAD_PRIORITY_BELOW_NORMAL;
	else if(percent <= 125)
		prior = THREAD_PRIORITY_NORMAL;
	else if(percent <= 175)
		prior = THREAD_PRIORITY_ABOVE_NORMAL;
	else
		prior = THREAD_PRIORITY_HIGHEST;
	SetThreadPriority(handle, prior);
#endif
#ifdef PLATFORM_POSIX
	//!! todo
#endif
}

void Thread::Sleep(int msec)
{
#ifdef PLATFORM_WIN32
	::Sleep(msec);
#endif
#ifdef PLATFORM_POSIX
	timespec tval;
	tval.tv_sec = msec / 1000;
	tval.tv_nsec = (msec % 1000) * 1000000;
	nanosleep(&tval, NULL);
#endif
}

#endif // _MT

#ifdef PLATFORM_WIN32

bool SyncObject::Wait(int ms)
{
	return WaitForSingleObject(handle, ms);
}

bool SyncObject::Wait()
{
	return Wait(INFINITE);
}

SyncObject::SyncObject()
{
	handle = NULL;
}

SyncObject::~SyncObject()
{
	if(handle) CloseHandle(handle);
}

Event::Event()
{
	handle = CreateEvent(NULL, FALSE, FALSE, NULL);
}

void Event::Set()
{
	SetEvent(handle);
}

#endif
