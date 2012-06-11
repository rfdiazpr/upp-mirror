#include "Skylark.h"

#ifdef PLATFORM_WIN32
#include <wincon.h>
#endif

bool ExitSkylark;
int  Port = 8001;

#ifdef PLATFORM_WIN32
BOOL WINAPI CtrlCHandlerRoutine(__in  DWORD dwCtrlType)
{
	LOG("Ctrl+C handler");
	ExitSkylark = true;
	Cout() << "Ctrl + C\n";
	Socket h;
	ClientSocket(h, "127.0.0.1", Port);
	h.Write("quit");
	return TRUE;
}
#endif

void SkylarkApp::WorkThread()
{
	RunThread();
}

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
		TcpSocket request;
		accept_mutex.Enter();
		if(ExitSkylark) {
			accept_mutex.Leave();
			break;
		}
		bool b = request.Timeout(2000).Accept(server);
		accept_mutex.Leave();
		if(ExitSkylark)
			break;
		if(b) {
			Cout() << "Accepted " << Thread::GetCurrentId() << "\n";
			Http http(*this);
			http.Dispatch(request);
			Cout() << "Finished " << Thread::GetCurrentId() << "\n";
		}
		else
			Cout() << "Waiting " << Thread::GetCurrentId() << "\n";
	}
}

void SkylarkApp::Run()
{
	SqlSession::PerThread();
	SqlId::UseQuotes();
	FinalizeViews();

#ifdef PLATFORM_WIN32
	SetConsoleCtrlHandler(CtrlCHandlerRoutine, true);
#endif

	// Add prefork here
	if(!server.Listen(Port, 5)) {
		Cout() << "Cannot open server socket!\n";
		return;
	}

	Buffer<Thread> uwt(threads);
	for(int i = 0; i < threads; i++)
		Thread::Start(THISBACK(ThreadRun));

	while(Thread::GetCount())
		Sleep(100);

	Cout() << "ExitSkylark\n";
}


void SkylarkApp::SqlError(Http& http)
{
}

void SkylarkApp::InternalError(Http& http)
{
}

void SkylarkApp::NotFound(Http& http)
{
}

void SkylarkApp::Unauthorized(Http& http)
{
}

SkylarkApp *SkylarkApp::app;

SkylarkApp& SkylarkApp::TheApp()
{
	ASSERT(app);
	return *app;
}

SkylarkApp::SkylarkApp()
{
	ASSERT(!app);
	app = this;
	threads = 3 * CPU_Cores() + 1;
	post_identities = 60;
}
