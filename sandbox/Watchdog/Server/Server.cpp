#include "Server.h"

#define  MODEL <Watchdog/Server/table.sch>
#define  SCHEMADIALECT <MySql/MySqlSchema.h>
#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>

namespace Upp{ namespace Ini {
	INI_STRING(sql_user, "root", "MySql user");
	INI_STRING(sql_password, "", "MySql password");
	INI_STRING(sql_database, "watchdog", "MySql database name");
	INI_STRING(sql_host, "localhost", "MySql hostname");
	INI_INT(sql_port, MYSQL_PORT, "MySql port number");
	INI_STRING(sql_socket, "/var/run/mysqld/mysqld.sock", "MySql socket path");
	INI_BOOL(sql_log, true, "Activates logging of MySql queries");
	INI_STRING(log_file, GetExeDirFile("log")+"/"+GetExeTitle()+".log", "Path for the log file");
}}

void OpenSQL(MySqlSession& session)
{
	if(!session.Connect((String)Ini::sql_user,
                        (String)Ini::sql_password,
                        (String)Ini::sql_database,
                        (String)Ini::sql_host,
                        Ini::sql_port,
                        (String)Ini::sql_socket)) {
		RLOG("Can't connect to database");
		RLOG(session.GetLastError());
		Exit(1);
	}
	SQL = session;
}

void Watchdog::WorkThread()
{
	SetDateFormat("%1:4d/%2:02d/%3:02d");
	MySqlSession mysql;
	if(Ini::sql_log){
		mysql.LogErrors();
		mysql.SetTrace();
	}
	OpenSQL(mysql);
	RunThread();
}

void InitDB()
{
	MySqlSession sqlsession;
	OpenSQL(sqlsession);
	SqlSchema sch(MY_SQL);
	All_Tables(sch);
	SqlPerformScript(sch.Upgrade());
	SqlPerformScript(sch.Attributes());
	Sql sql;
	sql * Insert(CLIENT)(NAME, "test")
	                    (PASSWORD, "aaa")
	                    (DESCR, "Testing client")
	                    (SRC, "/trunk/uppsrc");
	sql * Insert(CLIENT)(NAME, "test2")
	                    (PASSWORD, "bbb")
	                    (DESCR, "Another testing client")
	                    (SRC, "/trunk/uppsrc");
}

Value Duration(const Vector<Value>& arg, const Renderer *)
{
	int t = arg[0];
	if (t<=0)
		return "";
	else if (t<60)
		return Format("%d`s", t);
	else if (t<3600)
		return Format("%d`m %d`s", t/60, t%60);
	else
		return Format("%d`h %d`m %d`s", t/3600, (t%3600)/60, (t%3600)%60);
}

INITBLOCK {
	Compiler::Register("Duration", Duration);
};

Watchdog::Watchdog() {
	root = "";
#ifdef _DEBUG
	use_caching = false;
#endif
	MySqlSession sqlsession;
	OpenSQL(sqlsession);
	SigUsr1 = callback(&ReopenLog);
}

#ifdef flagMAIN

CONSOLE_APP_MAIN{
	SetIniFile(GetDataFile("Server.ini"));
	
	SetDateFormat("%1:4d/%2:02d/%3:02d");
	
	StdLogSetup(LOG_FILE
	           |LOG_TIMESTAMP
	           |LOG_APPEND
#ifdef _DEBUG
	           |LOG_CERR
#endif
	           , (String)Ini::log_file);
	
//	InitDB(); //only for development phase
	
	RLOG(" === STARTING WATCHDOG === ");
	RLOG(GetIniInfoFormatted());
	
	Watchdog().Run();
}

#endif
