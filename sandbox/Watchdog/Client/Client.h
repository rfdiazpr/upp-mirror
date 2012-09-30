#ifndef _Watchdog_Client_Client_h_
#define _Watchdog_Client_Client_h_

#include <Watchdog/Watchdog.h>

struct WatchdogClient {
	Vector<String> todo;
	virtual bool GetWork(int max_age = -1);
	virtual bool AcceptWork(int revision, Time start=Null);
	virtual bool SubmitWork(const int revision, const int result, const int time, const String& output, Time start=Null, Time end=Null);
	WatchdogClient();
protected:
	virtual bool Auth(HttpRequest& req);
};

#endif
