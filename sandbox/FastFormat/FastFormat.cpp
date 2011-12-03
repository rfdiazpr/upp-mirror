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

String FastFormat(unsigned n)
{
	char h[11];
	char *s = h + 9;
	*s = n % 10 + '0'; s--; n = n / 10;
	*s = n % 10 + '0'; s -= !!n; n = n / 10;
	*s = n % 10 + '0'; s -= !!n; n = n / 10;
	*s = n % 10 + '0'; s -= !!n; n = n / 10;
	*s = n % 10 + '0'; s -= !!n; n = n / 10;
	*s = n % 10 + '0'; s -= !!n; n = n / 10;
	*s = n % 10 + '0'; s -= !!n; n = n / 10;
	*s = n % 10 + '0'; s -= !!n; n = n / 10;
	*s = n % 10 + '0'; s -= !!n;
	return String(s + 1, h);
}

CONSOLE_APP_MAIN
{
	DDUMP(FastFormat(0));
	DDUMP(FastFormat(123));
	DDUMP(FastFormat(0xffffffff));
	LOG(0xffffffff);
/*	
	for(int i = 0; i < 100000; i++) {
		StringBuffer b;
		FastFormat(b, i, 10);
		ASSERT((String)b == Format("%010d", i));
	}
*/	
}
