#include "AdrBook.h"

#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>

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

#if 0
	Sqlite3Session sqlite3;

#ifdef _DEBUG
	sqlite3.LogErrors();
	sqlite3.SetTrace();
#endif
	if(!sqlite3.Open(ConfigFile("person.db"))) {
		LOG("Can't create or open database file\n");
		return;
	}
	SQL = sqlite3;
	sqlite3.ThrowOnError();

	SqlSchema sch(SQLITE3);
#else
	MySqlSession mysql;
#ifdef _DEBUG
	mysql.LogErrors();
	mysql.SetTrace();
#endif
	SqlId::UseQuotes();
	if(!mysql.Connect("root", "Passw0rd", "test")) {
		LOG("Can't create or open database file\n");
		return;
	}
	SQL = mysql;
	
	SqlSchema sch(MY_SQL);
#endif

	All_Tables(sch);
	SqlPerformScript(sch.Upgrade());
	SqlPerformScript(sch.Attributes());
	sch.SaveNormal();
	SQL.ClearError();
	SQLR.ClearError();
	
	SetViewRoot("root");
	SetViewVar("base", "asdfasdf");
	
	SessionConfig cfg;
	cfg.table = SES;
	SetSessionConfig(cfg);
	
	FinalizeViews();

	SQL.GetSession().ThrowOnError();

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
		#if 0
		#ifndef _DEBUG
			http.Benchmark(4000);
		#endif
		#endif
			http.Dispatch(request);
//			break;
		}
		else
			Cout() << " ...tick...\n";
	}
	LOG("EXIT");
	Cout() << "Exit\n";
}
