#include "HttpRequest2.h"

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

	HttpRequest::Trace();
	
	if(0) {
		IpAddrInfo f;
		f.Start("www.ultimatepp.org", 80);
		while(f.InProgress()) {
			DDUMP(msecs());
			Sleep(1);
		}
		DDUMP((void *)f.GetResult());
		DDUMP(f.Execute("www.google.cz", 80));
		DDUMP((void *)f.GetResult());
		return;
	}

	


	if(1) {
		HttpRequest h(
			"http://www.koupelny-instalace.cz/"
		);
//		h.Accept("text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
//		h.Header("Accept-Language", "cs,en-us;q=0.7,en;q=0.3");
		h.UserAgent("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:11.0) Gecko/20100101 Firefox/11.0");
		DDUMP(h.Execute());
		DDUMP(h.GetContent());
	}

//			"http://www.moto-brzdy.cz/produkty/"
//		HttpRequest h("http://www.drevostavby.cz/cz/");
//		HttpRequest h("http://Copypartner.cz/servis-tiskaren");// http://www.heckl.cz/
//		HttpRequest h("http://url.hurra.com/cgi-local/ProxyUrl.cgi?ad_id=1628942305&kw={Keyword}&GOOGLE_TYPE={ifsearch:SEARCH}{ifcontent:CONTENT}&GOOGLE_PLACEMENT={placement}");
//		HttpRequest h("http://www.abaut.cz/kotevni-technika-3");
//		h.Header("Cookie", "PHPSESSID=Bi7l7Q5opolAjS3tAHtIO4CWxbM9QQOx6sRiVRs_sNC4xLxW1N3f-pQWCMiNqT7b; path=/");
//		HttpRequest h("http://insolvence.jex.cz/");

	if(0) {
		HttpRequest h("http://www.insolvence.jex.cz/");
		DDUMP(h.Execute());
	}

	if(0) {
		HttpRequest h("https://www.google.cz/search"); // Normal
		h.Timeout(Null);
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
