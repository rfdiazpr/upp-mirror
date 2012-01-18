#include "Skylark.h"

#define LLOG(x) // DLOG(x)
#define LTIMING(x) RTIMING(x)

Http& Http::operator()(const ValueMap& map)
{
	ValueArray v = map.GetValues();
	const Index<Value>& k = map.GetKeys();
	for(int i = 0; i < map.GetCount(); i++)
		var.Add(k[i], v[i]);
	return *this;
}

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

Http& Http::Link(const char *id, void (*view)(Http&), const Vector<Value>& arg)
{
	var.Add(id, Raw('\"' + MakeLink(view, arg) + '\"'));
	return *this;
}

Http& Http::operator()(const char *id, void (*view)(Http&))
{
	return Link(id, view, Vector<Value>());
}

Http& Http::operator()(const char *id, void (*view)(Http&), const Value& arg1)
{
	return Link(id, view, Vector<Value>() << arg1);
}

Http& Http::operator()(const char *id, void (*view)(Http&), const Value& arg1, const Value& arg2)
{
	return Link(id, view, Vector<Value>() << arg1 << arg2);
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

StaticMutex                 template_cache_lock;
ArrayMap<String, One<Exe> > template_cache;

const One<Exe>& Http::GetTemplate(const String& template_name)
{
	LTIMING("GetTemplate");
	StringBuffer s;
	{
		LTIMING("MakeSignature");
		for(int i = 0; i < var.GetCount(); i++)
			s << var.GetKey(i) << ';';
		s << ':' << template_name;
	}
	String sgn = s;
	LLOG("Trying to retrieve " << sgn << " from cache");
	Mutex::Lock __(template_cache_lock);
	int q = template_cache.Find(sgn);
	if(q >= 0)
		return template_cache[q];
	LLOG("About to compile: " << sgn);
	LTIMING("Compile");
	One<Exe>& exe = template_cache.Add(sgn);
	exe = Compile(GetPreprocessedTemplate(template_name), var.GetIndex());
	return exe;
}

Http& Http::Render(const String& template_name)
{
	LTIMING("Render");
	response << ::Render(GetTemplate(template_name), var.GetValues());
	return *this;
}

Http& Http::Redirect(void (*view)(Http&), const Vector<Value>& arg)
{
	Redirect(MakeLink(view, arg));
	return *this;
}

Http& Http::Redirect(void (*view)(Http&))
{
	Vector<Value> arg;
	Redirect(view, arg);
	return *this;
}

Http& Http::Redirect(void (*view)(Http&), const Value& v1)
{
	Vector<Value> arg;
	arg.Add(v1);
	Redirect(view, arg);
	return *this;
}

Http& Http::Redirect(void (*view)(Http&), const Value& v1, const Value& v2)
{
	Vector<Value> arg;
	arg.Add(v1);
	arg.Add(v2);
	Redirect(view, arg);
	return *this;
}
