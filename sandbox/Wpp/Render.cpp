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
