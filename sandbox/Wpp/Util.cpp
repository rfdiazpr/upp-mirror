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

bool HttpHeader::Read(Socket& http)
{
	String s = ReadLine(http);
	Vector<String> h = Split(s, ' '); // Optimize!
	if(h.GetCount() != 3)
		return false;
	method = h[0];
	uri = h[1];
	version = h[2];
	field.Clear();
	for(;;) {
		s = ReadLine(http);
		if(s.IsEmpty()) break;
		int q = s.Find(':');
		if(q >= 0)
			field.GetAdd(ToLower(s.Mid(0, q))) = TrimLeft(s.Mid(q + 1));
	}
	return !http.IsError();
}

String HttpResponse(int code, const char *phrase, const String& data, const char *content_type)
{
	String r;
	r <<
		"HTTP/1.0 " << code << ' ' << phrase << "\r\n"
		"Date: " <<  WwwFormat(GetUtcTime()) << "\r\n"
		"Server: Centrum-Nos SaleCrm\r\n"
		"Content-Length: " << data.GetCount() << "\r\n"
		"Connection: close\r\n";
	if(content_type)
		r << "Content-Type: " << content_type << "\r\n";
	r << "\r\n" << data;
	return r;
}
