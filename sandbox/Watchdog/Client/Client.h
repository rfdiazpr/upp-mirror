#ifndef _Watchdog_Client_Client_h_
#define _Watchdog_Client_Client_h_

#include <Watchdog/Watchdog.h>

class WatchdogClient {
	String salt;
	String cookie;
	String identity;
	String cookiename;
public:
	Vector<String> todo;
	virtual bool Auth();
	virtual bool GetWork(int max_age = -1);
	virtual bool AcceptWork(int revision, Time start=Null);
	virtual bool SubmitWork(const int revision, const int result, const int time, const String& output, Time start=Null, Time end=Null);
	WatchdogClient();
protected:
	void ClearAuth();
};

#endif
