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
{ // Add length sanity check here...
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
			hdrfield.Add(ToLower(s.Mid(0, q))) = TrimLeft(s.Mid(q + 1));
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

String HttpResponse(int code, const char *phrase, const String& data, const char *content_type,
                    const char *cookies)
{
	String r;
	r <<
		"HTTP/1.0 " << code << ' ' << phrase << "\r\n"
		"Date: " <<  WwwFormat(GetUtcTime()) << "\r\n"
		"Server: U++\r\n"
		"Content-Length: " << data.GetCount() << "\r\n"
		"Connection: close\r\n"
		<< cookies;
	if(content_type)
		r << "Content-Type: " << content_type << "\r\n";
	r << "\r\n" << data;
	return r;
}

Http& Http::SetRawCookie(const char *id, const String& value, Time expires,
                         const char *path, const char *domain, bool secure,
                         bool httponly)
{
	cookies << "Set-Cookie:" << ' ' << id << '=' << value;
	if(!IsNull(expires))
		cookies << "; " << WwwFormat(expires);
	if(path && *path)
		cookies << "; Path=" << path;
	if(domain && *domain)
		cookies << "; Domain=" << domain;
	if(secure)
		cookies << "; Secure";
	if(httponly)
		cookies << "; HttpOnly";
	cookies << "\r\n";
	return *this;
}

Http& Http::SetCookie(const char *id, const String& value, Time expires,
                      const char *path, const char *domain, bool secure, bool httponly)
{
	return SetRawCookie(id, UrlEncode(value), expires, path, domain, secure);
}
