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
	String content;
	
	VectorMap<String, String> field;
	
	bool Read(Socket& http);
	
	String GetHeader(const char *s) const  { return field.Get(s, Null); }
	int    GetLength() const               { return atoi(GetHeader("content-length")); }
};

struct Http : HttpHeader {
	VectorMap<String, String> request; 
	Vector<String>            arg;
	
	int    code;
	String code_text;
	String response;
	String content_type;
	
	void  ParseRequest(const char *s);
	
	String operator[](const char *id) const           { return request.Get(id, String()); }
	String operator[](int i)                          { return i >= 0 && i < arg.GetCount() ? arg[i] : String(); }
	
	Http& ContentType(const char *s)                  { content_type = s; return *this; }
	Http& Content(const char *s, const String& data)  { content_type = s; response = data; return *this; }
	Http& operator<<(const String& s)                 { response << s; return *this; }
	
	Http() { code = 200; content_type = "text/html; charset=UTF-8"; }
};

String HttpResponse(int code, const char *phrase, const String& data, const char *content_type = NULL);

void RegisterView(const char *path, Callback1<Http&> view);
void RegisterView(const char *path, void (*view)(Http&));

void Dispatch(Socket& http);

#include "Templates.h"

#endif
