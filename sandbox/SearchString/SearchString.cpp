#include <Core/Core.h>

using namespace Upp;

int Search(const char *s, int len, const char *ss, int slen)
{
	int step = slen - 1 / 2 * 2;
	if(step < 2) {
		for(int i = 0; i < len - slen + 1; i++)
			if(memcmp(s + i, ss, slen) == 0)
				return i;
		return -1;
	}

	Index<word> p;
	for(int i = 0; i < slen - 1; i++)
		p.Add(Peek16le(ss + i), i);
	
	for(int i = 0; i < len; i += step) {
		word h = Peek16le(s + i);
		int q = p.Find(h);
		while(q >= 0) {
			int pos = i - q;
			if(pos >= 0 && memcmp(s + pos, ss, slen) == 0)
				return pos;
			q = p.FindNext(q);
		}
	}
	return -1;
}

int Search(const String& s, const String& ss)
{
	return Search(s, s.GetLength(), ss, ss.GetLength());
}

CONSOLE_APP_MAIN
{
	DDUMP(Search("Ahojx", "Ahoj"));

	String text = "Ahoj kamarade, jak se mas vole?";
	for(int i = 0; i < 20; i++)
		for(int j = 0; j < 20; j++)
			for(int h = 1; h < text.GetCount(); h++) {
				String ss = String('x', i) + text.Mid(0, h) + String('x', j);
				DDUMP(ss);
				ASSERT(Search(ss, text.Mid(0, h)) == i);
			}
	DDUMP(Search("01234Ahoj34", "Ahoj"));
	DDUMP(Search("01234Ahoj", "Ahoj"));
	DDUMP(Search("0Ahoj124", "Ahoj"));
	DDUMP(Search("Ahoj124", "Ahoj"));
}
