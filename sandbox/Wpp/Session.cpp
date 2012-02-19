#include "Skylark.h"

#define LLOG(x)   DLOG(x)
#define LDUMPC(x) DDUMPC(x)
#define LDUMPM(x) DDUMPM(x)

void SerializeSession(Stream& s, VectorMap<String, Value> *map)
{
	int version = 0;
	s.Magic(12341234);
	s / version;
	s % *map;
}

String SessionFile(const String& sid)
{
	String dir = ConfigFile("session");
	ONCELOCK {
		RealizeDirectory(dir);
	}
	return AppendFileName(dir, sid);
}

void Http::LoadSession()
{
	session_var.Clear();
	session_id = (*this)["__skylark_session__"];
	if(IsNull(session_id))
		return;
	LoadFromFile(callback1(SerializeSession, &session_var), SessionFile(session_id));
	LLOG("Loaded session: " << session_id);
	LDUMPM(session_var);
	for(int i = 0; i < session_var.GetCount(); i++)
		var.Add(session_var.GetKey(i), session_var[i]);
}

void Http::SaveSession()
{
	SetCookie("__skylark_session__", session_id);
	if(IsNull(session_id))
		return;
	StoreToFile(callback1(SerializeSession, &session_var), SessionFile(session_id));
	LLOG("Stored session: " << session_id);
	LDUMPM(session_var);
}

Http& Http::ClearSession()
{
	session_var.Clear();
	session_id.Clear();
	return *this;
}

Http& Http::SetSession(const char *id, const Value& value)
{
	if(IsNull(session_id))
		session_id = AsString(Uuid::Create());
	session_var.GetAdd(id) = value;
	var.GetAdd(id) = value;
	return *this;
}
