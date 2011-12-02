#include <Core/Core.h>

using namespace Upp;

inline int svo_strlen0(const char *s)
{
	const char *p = s;
	p += !!*p;
	p += !!*p;
	p += !!*p;
	p += !!*p;
	p += !!*p;
	p += !!*p;
	p += !!*p;
	p += !!*p;
	return *p ? 8 + strlen(p) : p - s;
}

inline int svo_strlen1(const char *s)
{
	int len = 0;
	int b = 1;
	do {
		b = b & !!s[0];
		len += b;
		b = b & !!s[1];
		len += b;
		b = b & !!s[2];
		len += b;
		b = b & !!s[3];
		len += b;
		s += 4;
	}
	while(b);
	return len;
}

inline int svo_strlen(const char *s)
{
	const char *b = s;
	while(!!s[0] & !!s[1] & !!s[2] & !!s[3])
		s += 4;
	s += !!*s;
	s += !!*s;
	s += !!*s;
	s += !!*s;
	return s - b;
}

int tester;

String tester2;
StringBuffer hh;

CONSOLE_APP_MAIN
{
	tester = strlen("hello");
	
	tester2 = "hello";
	tester2 << "world";
	
	hh << "test";
	
	tester = strlen(~tester2);
	
	int sum = 0;
	String h('?', 6);
	for(int i = 0; i < 10000000; i++) {
		RTIMING("strlen");
		sum += strlen(h);
	}
	for(int i = 0; i < 10000000; i++) {
		RTIMING("svo_strlen");
		sum += svo_strlen(h);
	}
	RLOG(sum);
	for(int i = 0; i < 20; i++)
		RLOG(svo_strlen(String('.', i)));
}
