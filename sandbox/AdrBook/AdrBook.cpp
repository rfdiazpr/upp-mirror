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
	void WorkThread();

	AdrBook();
};

void OpenSQL(MySqlSession& mysql)
{
	if(!mysql.Connect("root", "Passw0rd", "test")) {
		LOG("Can't create or open database file\n");
		Exit(1);
	}
	mysql.AutoReconnect();
#ifdef _DEBUG
	mysql.LogErrors();
	mysql.SetTrace();
#endif
	SQL = mysql;
}

void InitModel()
{
#ifdef _DEBUG
	SqlSchema sch(MY_SQL);
	All_Tables(sch);
	SqlPerformScript(sch.Upgrade());
	SqlPerformScript(sch.Attributes());
	sch.SaveNormal();
#endif
}

void AdrBook::WorkThread()
{
	MySqlSession mysql;
	OpenSQL(mysql);
	RunThread();
}

AdrBook::AdrBook()
{
	root = "adrbook";
	path = "/home/cxl/sandbox;u:/sandbox";
	session.format = SESSION_FORMAT_JSON;
	session.expire = 5 * 60;
#ifdef _DEBUG
	prefork = 0;
	use_caching = false;
#else
	prefork = 5;
	use_cachong = true;
#endif
	
}

CONSOLE_APP_MAIN
{
	MySqlSession mysql;
	OpenSQL(mysql);
	InitModel();
	AdrBook().Run();	
}
