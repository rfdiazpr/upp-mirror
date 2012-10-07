#include "Client.h"

namespace Upp { namespace Ini {
	INI_STRING(host, "http://localhost:8001/api", "Watchdog API URL");
	INI_INT(client_id, 1, "Watchdog client ID");
	INI_STRING(password, "", "Watchdog client password");
	INI_INT(max_age, -1, "Only commits this many days old can be build (negative value means any age)");
	INI_INT(log_level, 1, "Verbosity (0=errors only, 1=normal, 2=verbose)");
	INI_STRING(session_cookie, "__watchdog_cookie__", "Skylark session cookie ID");
}}

bool WatchdogClient::Auth(HttpRequest& req){
	HttpRequest auth(Ini::host+String("/auth/")+IntStr(Ini::client_id));
	auth.Execute();
	if(!auth.IsSuccess()){
		RLOG("Error during authentication");
		RLOG((auth.IsError() ? auth.GetErrorDesc() : AsString(auth.GetStatusCode())+':'+auth.GetReasonPhrase()));
		return false;
	}
	req.Header("WD-Nonce", auth.GetHeader("wd-nonce"));
	req.Header("WD-Auth", MD5String(auth.GetHeader("wd-nonce")+MD5String(auth.GetHeader("wd-salt")+Ini::password)));
	req.Header("Cookie", auth.GetHeader("set-cookie").Left(52));
	req.Post("__post_identity__", auth.GetHeader("wd-id"));
	req.Post("client_id", IntStr(Ini::client_id));
	return true;
}

bool WatchdogClient::GetWork(int max_age){
	HttpRequest req(Ini::host+String("/getwork"));
	if(!Auth(req))
		return false;
	
	if(max_age >= 0)
		req.Post("max_age", IntStr(max_age));
	else if (Ini::max_age >= 0)
		req.Post("max_age", IntStr(Ini::max_age));
	String resp = req.Execute();
	if(!req.IsSuccess()){
		RLOG("Error during get work phase");
		RLOG((req.IsError() ? req.GetErrorDesc() : AsString(req.GetStatusCode())+':'+req.GetReasonPhrase()));
		return false;
	}
	todo = Split(resp,",");
	return true;
}

bool WatchdogClient::AcceptWork(int revision, Time start){
	HttpRequest req(Ini::host+String("/acceptwork"));
	if(!Auth(req)) return false;
	req.Post("revision", IntStr(revision));
	if(!IsNull(start)){
		req.Post("start", IntStr64(start.Get()));
	}
	req.Execute();
	if(!req.IsSuccess()){
		RLOG("Error during accept work phase");
		RLOG((req.IsError() ? req.GetErrorDesc() : AsString(req.GetStatusCode())+':'+req.GetReasonPhrase()));
		return false;
	}
	return true;
}

bool WatchdogClient::SubmitWork(const int revision, const int result, const int time, const String& output, Time start, Time end){
	if(Ini::log_level > 0) 
		Cout() << "Sending result '" << status(result) << "'\n";
	HttpRequest req(Ini::host+String("/submitwork"));
	if(!Auth(req)) return false;
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
	req.Execute();
	if(!req.IsSuccess()){
		RLOG("Error during submit work phase");
		RLOG((req.IsError() ? req.GetErrorDesc() : AsString(req.GetStatusCode())+':'+req.GetReasonPhrase()));
		return false;
	}
	return true;
}

WatchdogClient::WatchdogClient(){}
