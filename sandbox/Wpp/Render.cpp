#include "Wpp.h"

#define LLOG(x) LOG(x)

StaticMutex                 template_cache_lock;
ArrayMap<String, One<Exe> > template_cache;

const One<Exe>& Http::GetTemplate(const String& template_name)
{
	String sgn = signature;
	sgn << ':' << template_name;
	LLOG("Trying to retrieve " << sgn << " from cache");
	Mutex::Lock __(template_cache_lock);
	int q = template_cache.Find(sgn);
	if(q >= 0)
		return template_cache[q];
	LLOG("About to compile: " << sgn);
	One<Exe>& exe = template_cache.Add(sgn);
	exe = Compile(GetPreprocessedTemplate(template_name), var.GetIndex());
	return exe;
}

Http& Http::Render(const String& template_name)
{
	response << ::Render(GetTemplate(template_name), var.GetValues());
	return *this;
}

Http& Http::Redirect(void (*view)(Http&), const Vector<Value>& arg)
{
	Redirect(MakeLink(view, arg));
}

Http& Http::Redirect(void (*view)(Http&))
{
	Vector<Value> arg;
	Redirect(view, arg);
}

Http& Http::Redirect(void (*view)(Http&), const Value& v1)
{
	Vector<Value> arg;
	arg.Add(v1);
	Redirect(view, arg);
}

Http& Http::Redirect(void (*view)(Http&), const Value& v1, const Value& v2)
{
	Vector<Value> arg;
	arg.Add(v1);
	arg.Add(v2);
	Redirect(view, arg);
}
