#include <Core/Core.h>

using namespace Upp;

String content;

void Content(const void *ptr, dword size)
{
	DLOG("CONTENT " << size);
	content.Cat((const char *)ptr, size);
}

CONSOLE_APP_MAIN
{
	DDUMP(sizeof(fd_set));
//	DDUMP(RequestHttpGet("www.ultimatepp.com"));

#if 0
	DDUMP(HttpRequest("www.idnes.cz").Execute());
	DLOG("=======================");
	DDUMP(HttpRequest("www.ultimatepp.org").Execute());
#endif

//	HttpRequest h("www.ultimatepp.org"); // Normal

	DDUMP(Encode64("hello world!"));
	DDUMP(Base64Encode("hello world!"));

	HttpRequest::Trace();

	if(1) {
		HttpRequest h("www.google.cz/search"); // Normal
		h.Timeout(0);
		DDUMP(h.Execute());
	}
	if(0) {
		HttpRequest h("www.ultimatepp.org"); // Normal
		h.Timeout(0);
		DDUMP(h.Execute());
	}
	if(0) {
		HttpRequest h("http://www.ultimatepp.org/forum"); // Normal
		h.Timeout(0);
		DDUMP(h.Execute());
	}
	if(0) {
		HttpRequest h("www.idnes.cz"); // Chunked
		h.WhenContent = callback(Content);
		h.MaxContentSize(10000);
		DDUMP(h.Execute());
		DDUMP(content);
	}
	if(0) {
		HttpRequest h("www.google.com"); // Redirect to google.cz
		DDUMP(h.Execute());
	}
}
