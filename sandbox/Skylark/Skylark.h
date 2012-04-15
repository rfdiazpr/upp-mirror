#ifndef _Wpp_Wpp_h
#define _Wpp_Wpp_h

#include <Draw/Draw.h>
#include <plugin/png/png.h>
#include <plugin/jpg/jpg.h>
#include <Sql/Sql.h>

using namespace Upp;

#include "Witz.h"
#include "Http.h"

class SkylarkApp {
	TcpSocket server;
	Mutex     accept_mutex;
	void      ThreadRun();
	
	typedef SkylarkApp CLASSNAME;

public:
	virtual void WorkThread() = 0;
	
	void RunThread();
	void Run(int threads = Null);
};

#endif
