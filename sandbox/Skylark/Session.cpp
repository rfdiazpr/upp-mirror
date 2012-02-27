#include "Skylark.h"

#define LLOG(x)   DLOG(x)
#define LDUMPC(x) DDUMPC(x)
#define LDUMPM(x) DDUMPM(x)

SessionConfig::SessionConfig()
{
	cookie = "__skylark_session_cookie__";
	dir = ConfigFile("session");
	id_column = "ID";
	data_column = "DATA";
	lastwrite_column = "LASTWRITE";
}

static SessionConfig sCfg;

void SetSessionConfig(const SessionConfig& scfg)
{
	sCfg = scfg;
}

void SerializeSession(Stream& s, VectorMap<String, Value> *map)
{
	int version = 0;
	s.Magic(12341234);
	s / version;
	s % *map;
}

String SessionFile(const String& sid)
{
	ONCELOCK
		RealizeDirectory(sCfg.dir);
	return AppendFileName(sCfg.dir, sid);
}

void Http::LoadSession()
{
	session_var.Clear();
	session_id = (*this)[sCfg.cookie];
	if(IsNull(session_id))
		return;
	if(sCfg.table.IsNull())
		LoadFromFile(callback1(SerializeSession, &session_var), SessionFile(session_id));
	else {
		String data = SQLR % Select(sCfg.data_column).From(sCfg.table)
		                     .Where(sCfg.id_column == session_id);
		LOGHEXDUMP(data, data.GetCount());
		StringStream ss(data);
		Load(callback1(SerializeSession, &session_var), ss);
	}
	LLOG("Loaded session: " << session_id);
	LDUMPM(session_var);
	for(int i = 0; i < session_var.GetCount(); i++)
		var.Add(session_var.GetKey(i), session_var[i]);
}

void Http::SaveSession()
{
	SetCookie(sCfg.cookie, session_id);
	if(IsNull(session_id))
		return;
	if(sCfg.table.IsNull())
		StoreToFile(callback1(SerializeSession, &session_var), SessionFile(session_id));
	else {
		StringStream ss;
		Store(callback1(SerializeSession, &session_var), ss);
		SqlVal data = SqlBinary(ss.GetResult());
		SQL * Update(sCfg.table)
				(sCfg.data_column, data)
				(sCfg.lastwrite_column, GetSysTime())
		      .Where(sCfg.id_column == session_id);
		if(SQL.GetRowsProcessed() == 0)
			SQL * Insert(sCfg.table)
			        (sCfg.id_column, session_id)
					(sCfg.data_column, data)
					(sCfg.lastwrite_column, GetSysTime());
	}
	LLOG("Stored session: " << session_id);
	LDUMPM(session_var);
}

// ToDo: Limit number of session - remove oldest ones

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
