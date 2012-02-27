#ifndef _AdrBook_AdrBook_h_
#define _AdrBook_AdrBook_h_

#include <Skylark/Skylark.h>
#include <plugin/sqlite3/Sqlite3.h>
#include <MySql/MySql.h>

#ifdef PLATFORM_WIN32
#include <wincon.h>
#endif

#define  MODEL <AdrBook/Model.sch>

#define  SCHEMADIALECT <MySql/MySqlSchema.h>
#include <Sql/sch_header.h>


#endif
