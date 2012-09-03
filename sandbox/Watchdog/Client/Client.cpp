#include "Client.h"

namespace Upp { namespace Ini {
	INI_STRING(host, "http://localhost:8001/api", "Watchdog API URL");
	INI_INT(client_id, 1, "Watchdog client ID");
	INI_STRING(password, "", "Watchdog client password");
	INI_INT(max_age, -1, "Only commits this many days old can be build (negative value means any age)");
	INI_STRING(session_cookie, "__watchdog_cookie__", "Skylark session cookie ID");
}}

bool WatchdogClient::Auth(){
	HttpClient req;
	req.URL(Ini::host+String("/auth/")+IntStr(Ini::client_id));
	String resp = req.ExecuteRedirect();
	if(req.status_code != 200){
		RLOG("Authentication error (" << req.status_code << "): " << req.error << "\n" << resp);
		return false;
	}
	salt = resp.Left(4);
	String headers = req.GetHeaders();
	cookie = headers.Mid(headers.Find(cookiename)+cookiename.GetLength()+1,32);
	identity = resp.Mid(resp.Find("value=")+7,32);
	return true;
}

bool WatchdogClient::GetWork(int max_age){
	if(!Auth()) return false;
	HttpClient req;
	req.URL(Ini::host+String("/getwork"));
	req.Post("__post_identity__", identity);
	req.Post("client_id", IntStr(Ini::client_id));
	req.Post("passphrase", MD5String(salt+Ini::password));
	if(max_age >= 0)
		req.Post("max_age", IntStr(max_age));
	else if (Ini::max_age >= 0)
		req.Post("max_age", IntStr(max_age));
	req.Header("Cookie",cookiename+"="+cookie);
	String resp = req.ExecuteRedirect();
	todo = Split(resp,",");
	ClearAuth();
	if(req.status_code != 200){
		DUMP(req.error);
		return false;
	}
	return true;
}

bool WatchdogClient::AcceptWork(int revision, Time start){
	if(!Auth()) return false;
	HttpClient req;
	req.URL(Ini::host+String("/acceptwork"));
	req.Post("__post_identity__", identity);
	req.Post("client_id", IntStr(Ini::client_id));
	req.Post("passphrase", MD5String(salt+Ini::password));
	req.Post("revision", IntStr(revision));
	if(!IsNull(start)){
		req.Post("start", IntStr64(start.Get()));
	}
	req.Header("Cookie",cookiename+"="+cookie);
	req.ExecuteRedirect();
	ClearAuth();
	if(req.status_code != 200){
		DUMP(req.error);
		return false;
	}
	return true;
}

bool WatchdogClient::SubmitWork(const int revision, const int result, const int time, const String& output, Time start, Time end){
	if(!Auth()) return false;
	HttpClient req;
	req.URL(Ini::host+String("/submitwork"));
	req.Post("__post_identity__", identity);
	req.Post("client_id", IntStr(Ini::client_id));
	req.Post("passphrase", MD5String(salt+Ini::password));
	req.Post("result", IntStr(result));
	req.Post("time", IntStr(time));
	req.Post("revision", IntStr(revision));
	req.Post("output", output);
	if(!IsNull(start)){
		req.Post("start", IntStr64(start.Get()));
	}
	if(!IsNull(end)){
		req.Post("end", IntStr64(end.Get()));
	}
	req.Header("Cookie",cookiename+"="+cookie);
	req.ExecuteRedirect();
	ClearAuth();
	if(req.status_code != 200){
		DUMP(req.error);
		return false;
	}
	return true;
}

void WatchdogClient::ClearAuth() {
	salt = cookie = identity = "";
}

WatchdogClient::WatchdogClient() : cookiename(Ini::session_cookie) {}
