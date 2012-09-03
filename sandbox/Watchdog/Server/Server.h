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

struct Watchdog : SkylarkApp {
	virtual void WorkThread();
	Watchdog();
};

 
#endif
