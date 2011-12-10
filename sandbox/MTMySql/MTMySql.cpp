#include <MySql/MySql.h>

using namespace Upp;

#define SCHEMADIALECT <MySql/MySqlSchema.h>
#define MODEL <MTMySql/Test.sch>
#include <Sql/sch_header.h>
#include <Sql/sch_source.h>
#include <Sql/sch_schema.h>

void DoWork()
{
	MySqlSession session;
	// edit the connection parameters if necessary
	if(!session.Connect("root", "Passw0rd", "adrbook")) {
		LOG("Can't create or open database file\n");
		return;
	}

	session.SetTrace();
	SQL = session;

	MySqlSession sessionr;
	// edit the connection parameters if necessary
	if(!sessionr.Connect("root", "Passw0rd", "adrbook")) {
		LOG("Can't create or open database file\n");
		return;
	}
	
	sessionr.SetTrace();
	SQLR = sessionr;

	DUMP((void *)&SQL);
	DUMP((void *)&SQLR);
	SqlR sql1;
	DUMP((void *)&sql1.GetSession());
	for(int i = 0; i < 1000; i++) {
		RTIMING("Insert");
		SQL * Insert(ADDRESSBOOK)(ID, (int)Random())(FIRSTNAME, AsString(Random()));
	}
	String h;
	for(int i = 0; i < 100; i++) {
		RTIMING("Select");
		SqlR sql;
		SQLR * Select(FIRSTNAME).From(ADDRESSBOOK).Limit(10);
		while(SQLR.Fetch())
			h << SQLR[FIRSTNAME] << '\n';
	}
	RLOG((void *)&SQL << '\n' << h);
}

CONSOLE_APP_MAIN
{
	MySqlSession session;
	// edit the connection parameters if necessary
	if(session.Connect("root", "Passw0rd", "adrbook")) {
		Cout() << "Connected\n";
//		session.SetTrace();
		SQL = session;
		
		SqlSchema sch(MY_SQL);
		All_Tables(sch);
		// create the table if necessary
		SqlPerformScript(sch.Upgrade());
		SqlPerformScript(sch.Attributes());
		SQL.ClearError();

		DoWork();
/*
		Thread t1, t2;
		
		t1.Run(callback(DoWork));
		t2.Run(callback(DoWork));
		
		t1.Wait();
		t2.Wait();*/
	}
	else {
		Cerr() <<"ERROR: Unable to connect to database\n";
		SetExitCode(1);
	}
}
