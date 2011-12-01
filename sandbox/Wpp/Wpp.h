#ifndef _Wpp_Wpp_h
#define _Wpp_Wpp_h

#include <Web/Web.h>
#include <Draw/Draw.h>
#include <plugin/png/png.h>
#include <plugin/jpg/jpg.h>

using namespace Upp;

struct HttpHeader {
	String method;
	String uri;
	String version;
	
	VectorMap<String, String> field;
	
	bool Read(Socket& http);
	
	int  GetLength() const          { return atoi(field.Get("content-length", Null)); }
};

struct Http : HttpHeader {
	Vector<String> arg;
	
	int    code;
	String code_text;
	String response;
	String content_type;
	
	Http& operator<<(const String& s) { response << s; return *this; }
	
	Http() { code = 200; content_type = "text/html; charset=UTF-8"; }
};

String HttpResponse(int code, const char *phrase, const String& data, const char *content_type = NULL);

void RegisterView(const char *path, Callback1<Http&> view);
void RegisterView(const char *path, void (*view)(Http&));

void Dispatch(Socket& http);

#endif
