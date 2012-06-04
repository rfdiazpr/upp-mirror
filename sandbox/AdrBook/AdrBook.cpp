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
	
	AdrBook();
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

AdrBook::AdrBook()
{
	root = "root";
	template_path = "/home/cxl/sandbox;u:/sandbox";
	view_var.Add("base", "asdfasdf");
	session.format = SESSION_FORMAT_XML;
}

CONSOLE_APP_MAIN
{
//	cfg.table = SES;
	
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
