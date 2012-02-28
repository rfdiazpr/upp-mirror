#include "Skylark.h"

#ifdef PLATFORM_WIN32
#include <wincon.h>
#endif

bool Exit;
int  Port = 8001;

#ifdef PLATFORM_WIN32
BOOL WINAPI CtrlCHandlerRoutine(__in  DWORD dwCtrlType)
{
	LOG("Ctrl+C handler");
	Exit = true;
	Cout() << "Ctrl + C\n";
	Socket h;
	ClientSocket(h, "127.0.0.1", Port);
	h.Write("quit");
	return TRUE;
}
#endif

void SkylarkApp::ThreadRun()
{
	WorkThread();
}

void SkylarkApp::RunThread()
{
	SQL.ClearError();
	SQLR.ClearError();
	SQL.GetSession().ThrowOnError();
	SQLR.GetSession().ThrowOnError();

	for(;;) {
		Socket request;
		accept_mutex.Enter();
		if(Exit) {
			accept_mutex.Leave();
			break;
		}
		bool b = server.Accept(request);
		accept_mutex.Leave();
		if(Exit)
			break;
		if(b) {
			Cout() << "Accepted " << Thread::GetCurrentId() << "\n";
			Http http;
			http.Dispatch(request);
			Cout() << "Finished " << Thread::GetCurrentId() << "\n";
		}
		else
			Cout() << "Waiting " << Thread::GetCurrentId() << "\n";
	}
}

void SkylarkApp::Run(int threads)
{
	SqlSession::PerThread();
	SqlId::UseQuotes();
	FinalizeViews();

#ifdef PLATFORM_WIN32
	SetConsoleCtrlHandler(CtrlCHandlerRoutine, true);
#endif

	if(!ServerSocket(server, Port, true, 5)) {
		Cout() << "Cannot open server socket!\n";
		return;
	}

	if(IsNull(threads))
		threads = CPU_Cores() + 1;
	Buffer<Thread> uwt(threads);
	for(int i = 0; i < threads; i++)
		Thread::Start(THISBACK(ThreadRun));
	ThreadRun();
	while(Thread::GetCount())
		Sleep(10);
	Cout() << "Exit\n";
}
