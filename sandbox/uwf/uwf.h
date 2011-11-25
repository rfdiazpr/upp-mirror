#include <Web/Web.h>

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
	
	String response;
	
	Http& operator<<(const String& s) { response << s; return *this; }
};

String HttpResponse(int code, const char *phrase, const String& data, const char *content_type = NULL);

void RegisterView(const char *path, Callback1<Http&> view);
void RegisterView(const char *path, void (*view)(Http&));

void Dispatch(Socket& http);
