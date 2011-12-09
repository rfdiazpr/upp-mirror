#include <Core/Core.h>

using namespace Upp;

void FastFormat(StringBuffer& r, unsigned number, int digits)
{
	ASSERT(digits > 0 && digits < 16);
	char h[17];
	for(int i = 0; i < digits; i++) {
		h[digits - i - 1] = number % 10 + '0';
		number = number / 10;
	}
	r.Cat(h, digits);
}

void FastFormat4(char *h, unsigned number)
{
	h[3] = number % 10 + '0'; number /= 10;
	h[2] = number % 10 + '0'; number /= 10;
	h[1] = number % 10 + '0'; number /= 10;
	h[0] = number % 10 + '0'; number /= 10;	
}

void FastFormat2(char *h, unsigned number)
{
	h[1] = number % 10 + '0'; number /= 10;
	h[0] = number % 10 + '0'; number /= 10;	
}

void FastFormatDate(StringBuffer& r, Date d, char sep)
{
	char h[10];
	// 0123456789
	// 2011/11/25
	h[4] = h[7] = sep;
	FastFormat4(h, d.year);
	FastFormat2(h + 5, d.month);
	FastFormat2(h + 8, d.day);
	r.Cat(h, 10);
}
/*
void FastFormat8(char *h, unsigned number)
{
	char h[8];
	h[7] = number % 10 + '0'; number /= 10;
	h[6] = number % 10 + '0'; number /= 10;
	h[5] = number % 10 + '0'; number /= 10;
	h[4] = number % 10 + '0'; number /= 10;	
	h[3] = number % 10 + '0'; number /= 10;
	h[2] = number % 10 + '0'; number /= 10;
	h[1] = number % 10 + '0'; number /= 10;
	h[0] = number % 10 + '0'; number /= 10;	
}
*/
void FastFormat4(StringBuffer& r, unsigned number)
{
	char h[4];
	FastFormat4(h, number);
	r.Cat(h, 4);
}

void FastFormatB(StringBuffer& r, unsigned number, int digits)
{
	ASSERT(digits > 0 && digits < 16);
	int off = r.GetLength();
	r.SetLength(off + digits);
	char *h = ~r + off;
	for(int i = 0; i < digits; i++) {
		h[digits - i - 1] = number % 10 + '0';
		number = number / 10;
	}
}

inline void fastmemcpy(char *t, const char *s, int len)
{
	switch(len) {
	default:
		memcpy(t, s, len);
		break;
	case 10: t[9] = s[9];
	case  9: t[8] = s[8];
	case  8: t[7] = s[7];
	case  7: t[6] = s[6];
	case  6: t[5] = s[5];
	case  5: t[4] = s[4];
	case  4: t[3] = s[3];
	case  3: t[2] = s[2];
	case  2: t[1] = s[1];
	case  1: t[0] = s[0];
	}
}

int N;
String SN;

void FastFormat_(StringBuffer& b, int ii)
{
	char h[12];
	char *s = h + 11;
	bool neg = false;
	unsigned n = ii;
	if(ii < 0) {
		n = -ii;
		neg = true;
	}
	do {
		*s-- = n % 10 + '0';
		n = n / 10;
	}
	while(n);
	if(neg)
		*s-- = '-';
	b.Cat(s + 1, h + 12);
}

String FastFormat1_(int ii)
{
	StringBuffer b;
	FastFormat_(b, ii);
	return b;
}

String FastFormat_(int ii)
{
	char h[12];
	char *s = h + 11;
	bool neg = false;
	unsigned n = ii;
	if(ii < 0) {
		n = -ii;
		neg = true;
	}
	do {
		*s-- = n % 10 + '0';
		n = n / 10;
	}
	while(n);
	if(neg)
		*s-- = '-';
	return String(s + 1, h + 12);	
}

String FastFormat(int ii)
{
	char h[12];
	bool neg = false;
	unsigned n = ii;
	if(ii < 0) {
		n = -ii;
		neg = true;
	}

#define DIGIT(I) \
	h[I] = n % 10 + '0'; n /= 10; \
	if(n == 0) \
		if(neg) { \
			h[I - 1] = '-'; \
			return String(h + I - 1, h + 12); \
		} \
		else \
			return String(h + I, h + 12);
	
	DIGIT(11);
	DIGIT(10);
	DIGIT(9);
	DIGIT(8);
	DIGIT(7);
	DIGIT(6);
	DIGIT(5);
	DIGIT(4);
	DIGIT(3);
	DIGIT(2);
	h[1] = n % 10 + '0'; n /= 10; \
	if(neg) {
		h[0] = '-';
		return String(h, h + 12);
	}
	else
		return String(h + 1, h + 12);

#undef DIGIT
}

CONSOLE_APP_MAIN
{
	RDUMP(FastFormat(0));
	RDUMP(FastFormat(123));
	RDUMP(FastFormat(-123));
	RDUMP(FastFormat(0x7fffffff));
	RDUMP(FastFormat(0x80000000));
	RDUMP(FastFormat(0xffffffff));
	LOG(0x7fffffff);
#ifndef _DEBUG
	int sm = 0;
	for(int i = 0; i < 10000000; i++) {
		RTIMING("FormatInt");
		sm += FormatInt(i).GetCount();
	}
	for(int i = 0; i < 50000000; i++) {
		RTIMING("FastFormat");
		sm += FastFormat(i).GetCount();
	}
	for(int i = 0; i < 50000000; i++) {
		RTIMING("FastFormat_");
		sm += FastFormat_(i).GetCount();
	}
	for(int i = 0; i < 50000000; i++) {
		RTIMING("FastFormat1_");
		sm += FastFormat_(i).GetCount();
	}
#endif
/*	
	for(int i = 0; i < 100000; i++) {
		StringBuffer b;
		FastFormat(b, i, 10);
		ASSERT((String)b == Format("%010d", i));
	}
*/	
}
