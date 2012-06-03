#ifndef _Wpp_Wpp_h
#define _Wpp_Wpp_h

#include <Draw/Draw.h>
#include <plugin/png/png.h>
#include <plugin/jpg/jpg.h>
#include <Sql/Sql.h>

using namespace Upp;

class Http;

class SkylarkApp {
	TcpSocket server;
	Mutex     accept_mutex;
	void      ThreadRun();
	
	typedef SkylarkApp CLASSNAME;

public:
	virtual void WorkThread() = 0;
	virtual void SqlError(Http& http);
	virtual void InternalError(Http& http);
	virtual void NotFound(Http& http);
	
	void RunThread();
	void Run(int threads = Null);
};

#include "Witz.h"
#include "Http.h"

#endif
