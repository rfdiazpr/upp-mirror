#ifndef _Watchdog_Watchdog_h
#define _Watchdog_Watchdog_h

#include <Skylark/Skylark.h>
#include <MySql/MySql.h>
#include <Watchdog/Watchdog.h>
using namespace Upp;

#define  MODEL <Watchdog/Server/table.sch>
#define  SCHEMADIALECT <MySql/MySqlSchema.h>
#include <Sql/sch_header.h>

#include "Util.h"

namespace Upp{ 
	void ReopenLog();
	namespace Ini {
		extern IniString sql_user;
		extern IniString sql_password;
		extern IniString sql_database;
		extern IniString sql_host;
		extern IniInt    sql_port;
		extern IniString sql_socket;
		extern IniBool   sql_log;
		extern IniString log_file;
		extern IniString output_dir;
		extern IniString server_url;
		extern IniString svn;
		extern IniInt    max_build_time;
	}
}


struct Watchdog : SkylarkApp {
	virtual void WorkThread();
	virtual void SigUsr1();
	Watchdog();
};

 
#endif
