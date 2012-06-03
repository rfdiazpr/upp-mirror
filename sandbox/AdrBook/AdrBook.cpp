#include "AdrBook.h"

#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>

#define IMAGECLASS AdrBookImg
#define IMAGEFILE <AdrBook/AdrBook.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS AdrBookImg
#define IMAGEFILE <AdrBook/AdrBook.iml>
#include <Draw/iml_source.h>

struct AdrBook : SkylarkApp {
	virtual void WorkThread();
};

void AdrBook::WorkThread()
{
	MySqlSession mysql;
	if(!mysql.Connect("root", "Passw0rd", "test")) {
		LOG("Can't create or open database file\n");
		return;
	}
	mysql.AutoReconnect();
#ifdef _DEBUG
	mysql.LogErrors();
	mysql.SetTrace();
#endif
	SQL = mysql;
	SQLR = mysql;
	
	RunThread();
}

CONSOLE_APP_MAIN
{
	SetTemplatePath("/home/cxl/sandbox;u:/sandbox");
	SetViewRoot("root");
	SetViewVar("base", "asdfasdf");
	SessionConfig cfg;
//	cfg.table = SES;
	cfg.format = SESSION_FORMAT_XML;
	SetSessionConfig(cfg);
	
	{
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

		All_Tables(sch);
		SqlPerformScript(sch.Upgrade());
		SqlPerformScript(sch.Attributes());
		sch.SaveNormal();
	}
	
	AdrBook().Run();
}
