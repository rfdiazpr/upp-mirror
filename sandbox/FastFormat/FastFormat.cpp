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

CONSOLE_APP_MAIN
{
	N = strlen("Hello!");
	SN = "Hello ";
	SN << "world!";
	Date d = GetSysDate();
	StringBuffer b;
	b.Reserve(100);
	FastFormatDate(b, d, '/');
	RDUMP((String)b);
/*	
	for(int i = 0; i < 100000; i++) {
		StringBuffer b;
		FastFormat(b, i, 10);
		ASSERT((String)b == Format("%010d", i));
	}
*/	

#ifdef _DEBUG
	return;
#endif

	String str;
	for(int i = 0; i < 10000000; i++) {
		str.Clear();
		RTIMING("Cat 18");
		str.Cat("Hello", 5);
		str.Cat("Hello", 5);
		str.Cat("Hello", 5);
	}
	for(int i = 0; i < 10000000; i++) {
		str.Clear();
		RTIMING("Cat 40");
		str.Cat("Hello", 5);
		str.Cat("Hello", 5);
		str.Cat("Hello", 5);
		str.Cat("Hello", 5);
		str.Cat("Hello", 5);
		str.Cat("Hello", 5);
		str.Cat("Hello", 5);
		str.Cat("Hello", 5);
	}
	return;

	char h[8];
	char j[8] = "memcpy";
	for(int i = 0; i < 10000000; i++) {
		j[3] = i;
		RTIMING("memcpy");
		memcpy(h, j, 7);
	}
	for(int i = 0; i < 10000000; i++) {
		j[3] = i;
		RTIMING("fast_memcpy");
		fastmemcpy(h, j, 7);
	}
	
	RDUMP(h);
	

	return;
	for(int i = 0; i < 1000000; i++) {
		RTIMING("FastFormat");
		StringBuffer b;
		FastFormat(b, i, 10);
	}
	for(int i = 0; i < 1000000; i++) {
		RTIMING("FastFormat4");
		StringBuffer b;
		FastFormat4(b, i);
	}

	for(int i = 0; i < 1000000; i++) {
		RTIMING("FastFormat4h");
		char h[4];
		FastFormat4(b, i);
	}

	for(int i = 0; i < 100000; i++) {
		RTIMING("Format");
		Format("%010d", i);
	}
	
	for(int i = 0; i < 100000; i++) {
		RTIMING("FormatDate");
		Format("%04d/%02d/%02d", (int)d.year, (int)d.month, (int)d.day);
	}
	for(int i = 0; i < 100000; i++) {
		RTIMING("FatFormatDate");
		StringBuffer h;
		FastFormatDate(h, d, '/');
	}

}
