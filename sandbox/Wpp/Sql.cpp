#include "Skylark.h"

struct sFieldsToHttp : public FieldOperator {
	Http& http;

	void Field(const char *name, Ref f) {
		http(name, f);
	}
	
	sFieldsToHttp(Http& http) : http(http) {}
};

Http& Http::operator()(Fields rec)
{
	sFieldsToHttp x(*this);
	rec(x);
	return *this;
}

Http& Http::operator()(const Sql& sql)
{
	int n = sql.GetColumns();
	for(int i = 0; i < n; i++)
		(*this)(sql.GetColumnInfo(i).name, sql[i]);
	return *this;
}

SqlUpdate Http::Update(SqlId table)
{
	Vector<String> col = GetSchColumns(~table);
	SqlUpdate u(table);
	for(int i = 0; i < col.GetCount(); i++) {
		String c = col[i];
		int q = var.Find(c);
		if(q < 0)
			q = var.Find(ToLower(c));
		if(q >= 0)
			u(c, var[q]);
	}
	return u;
}

SqlInsert Http::Insert(SqlId table)
{
	Vector<String> col = GetSchColumns(~table);
	SqlInsert y(table);
	for(int i = 0; i < col.GetCount(); i++) {
		String c = col[i];
		int q = var.Find(c);
		if(q < 0)
			q = var.Find(ToLower(c));
		if(q >= 0)
			y(c, var[q]);
	}
	return y;
}
