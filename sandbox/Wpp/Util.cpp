#include "Wpp.h"

#define LLOG(x) LOG(x)

static String ReadLine(Socket& s)
{
	String t = s.ReadUntil('\n');
	if(*t.Last() == '\r')
		t.Trim(t.GetCount() - 1);
	LLOG(t);
	return t;
}

bool Http::Read(Socket& http)
{
	String s = ReadLine(http);
	Vector<String> h = Split(s, ' '); // Optimize!
	if(h.GetCount() != 3)
		return false;
	method = h[0];
	uri = h[1];
	version = h[2];
	hdrfield.Clear();
	for(;;) {
		s = ReadLine(http);
		if(s.IsEmpty()) break;
		int q = s.Find(':');
		if(q >= 0)
			hdrfield.GetAdd(ToLower(s.Mid(0, q))) = TrimLeft(s.Mid(q + 1));
	}
	return !http.IsError();
}

void Http::ParseRequest(const char *p)
{
	while(*p) {
		const char *last = p;
		while(*p && *p != '=' && *p != '&')
			p++;
		String key = UrlDecode(last, p);
		if(*p == '=')
			p++;
		last = p;
		while(*p && *p != '&')
			p++;
		DUMP(key);
		DUMP(UrlDecode(last, p));
		request.GetAdd(key) = UrlDecode(last, p);
		if(*p)
			p++;
	}
}

String HttpResponse(int code, const char *phrase, const String& data, const char *content_type)
{
	String r;
	r <<
		"HTTP/1.0 " << code << ' ' << phrase << "\r\n"
		"Date: " <<  WwwFormat(GetUtcTime()) << "\r\n"
		"Server: U++\r\n"
		"Content-Length: " << data.GetCount() << "\r\n"
		"Connection: close\r\n";
	if(content_type)
		r << "Content-Type: " << content_type << "\r\n";
	r << "\r\n" << data;
	return r;
}
