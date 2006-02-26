#ifndef __TSql__
#define __TSql__

#include <Sql/Sql.h>
#include <TCore/TCore.h>

#define FromSchema(tbl) From(SchemaAlias((tbl)))
#define FromSchema2(t1, t2) From(SqlSet(SchemaAlias((t1)), SchemaAlias((t2))))
#define FromSchema3(t1, t2, t3) From(SqlSet(SchemaAlias((t1)), SchemaAlias((t2)), SchemaAlias((t3))))
#define FromSet(set)    From((set))

#include "util.h"
#include "template.h"
#include "dict.h"

#endif//__TSql__
