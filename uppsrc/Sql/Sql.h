#ifndef SQL_H
#define SQL_H

#include <Core/Core.h>

#ifdef flagNOAPPSQL
	#define NOAPPSQL
	#define APPSQLCURSOR
	#define APPSQLSESSION
#else
	#define APPSQLCURSOR = SQL
	#define APPSQLSESSION = SQL.GetSession()
#endif

#include <Sql/Sqlexp.h>
#include <Sql/Sqls.h>
#include <Sql/SqlSchema.h>

#endif
