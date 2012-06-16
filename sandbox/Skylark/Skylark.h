#ifndef _Wpp_Wpp_h
#define _Wpp_Wpp_h

#include <Draw/Draw.h>
#include <plugin/png/png.h>
#include <plugin/jpg/jpg.h>
#include <Sql/Sql.h>

using namespace Upp;

class Renderer;
class Http;

struct SessionConfig {
	String cookie;
	String dir;
	int    format;
	SqlId  table, id_column, data_column, lastwrite_column;
	int    max_stored_count;
	
	SessionConfig();
};

struct AuthExc : Exc {
	AuthExc(const String& s) : Exc(s) {}
};

class SkylarkApp {
	TcpSocket    server;
	Mutex        accept_mutex;
	int          main_pid;
	Vector<int>  child_pid;
	bool         quit;
	

	void         ThreadRun();
	void         Broadcast(int signal);
	void         Signal(int signal);
	static void  SignalHandler(int signal);
	void         Main();


	void      FinalizeViews();

	static SkylarkApp *app;

	typedef SkylarkApp CLASSNAME;

public: // Skylark config - should be protected
	String                    root;
	VectorMap<String, String> view_var;
	String                    template_path;
	SessionConfig             session;
	int                       threads;
	int                       post_identities;
	int                       port;
	int                       prefork;
	int                       timeout;

public:
	virtual void SqlError(Http& http);
	virtual void InternalError(Http& http);
	virtual void NotFound(Http& http);
	virtual void Unauthorized(Http& http);

	virtual void WorkThread() = 0;
	
	void RunThread();

	void Run();
	
	static SkylarkApp& TheApp();

	SkylarkApp();
	virtual ~SkylarkApp();
};


#include "Witz.h"
#include "Http.h"

#endif
