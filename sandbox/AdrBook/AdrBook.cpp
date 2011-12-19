#include "AdrBook.h"

#include <Sql/sch_schema.h>

#include <Sql/sch_source.h>

#ifdef PLATFORM_WIN32
#include <wincon.h>
#endif

int number;

struct CtrlCExc : Exc {};

bool Exit;

int Port = 8001;

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

CONSOLE_APP_MAIN
{
	SetTemplatePath("/home/cxl/sandbox;u:/sandbox");

	Sqlite3Session sqlite3;

	sqlite3.LogErrors();
	sqlite3.SetTrace();
	if(!sqlite3.Open(ConfigFile("person.db"))) {
		LOG("Can't create or open database file\n");
		return;
	}
	SQL = sqlite3;

	SqlSchema sch(SQLITE3);
	StdStatementExecutor se(sqlite3);
	All_Tables(sch);
	Sqlite3PerformScript(sch.Upgrade(), se);
	Sqlite3PerformScript(sch.Attributes(), se);
	sch.SaveNormal();

	sqlite3.ThrowOnError();

	LOG("About to start");
#ifdef PLATFORM_WIN32
	SetConsoleCtrlHandler(CtrlCHandlerRoutine, true);
#endif
	Socket server;
	if(!ServerSocket(server, Port, true, 5)) {
		Cout() << "Cannot open server socket!\n";
		return;
	}
	Cout() << "Server socket opened\n";
	while(!Exit) {
		Cout() << "Exit: " << Exit << "\n";
		Socket request;
		if(server.Accept(request)) {
			if(Exit)
				break;
			Cout() << " request accepted\n";
			Http http;
			http.Dispatch(request);
		}
		else
			Cout() << " ...tick...\n";
	}
	LOG("EXIT");
	Cout() << "Exit\n";
}
