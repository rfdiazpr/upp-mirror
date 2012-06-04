#ifndef _Wpp_Wpp_h
#define _Wpp_Wpp_h

#include <Draw/Draw.h>
#include <plugin/png/png.h>
#include <plugin/jpg/jpg.h>
#include <Sql/Sql.h>

using namespace Upp;

class Http;

struct SessionConfig {
	String cookie;
	String dir;
	int    format;
	SqlId  table, id_column, data_column, lastwrite_column;
	int    max_stored_count;
	
	SessionConfig();
};

class SkylarkApp {
	TcpSocket server;
	Mutex     accept_mutex;
	void      ThreadRun();

	void      FinalizeViews();
	
	static SkylarkApp *app;

	typedef SkylarkApp CLASSNAME;

public: // should be protected
	String                    root;
	VectorMap<String, String> view_var;
	String                    template_path;
	SessionConfig             session;

public:
	virtual void WorkThread() = 0;
	virtual void SqlError(Http& http);
	virtual void InternalError(Http& http);
	virtual void NotFound(Http& http);
	
	void RunThread();

	void Run(int threads = Null);
	
	static SkylarkApp& TheApp();

	SkylarkApp();
};


#include "Witz.h"
#include "Http.h"

#endif
