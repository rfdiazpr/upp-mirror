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
	MySqlSession mysql;
	OpenSQL(mysql);
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
	root = "root";
	template_path = "/home/cxl/sandbox;u:/sandbox";
	view_var.Add("base", "asdfasdf");
	session.format = SESSION_FORMAT_XML;
	prefork = 5;
}

CONSOLE_APP_MAIN
{
	InitModel();
	AdrBook().Run();	
}
