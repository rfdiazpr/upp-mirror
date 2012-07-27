#include "Algo.h"

void Replace(Stream& out, Stream& in, const String& search, const String& replace)
{
	String data;
	while(!in.IsEof()) {
		data.Cat(in.Get());
		if(data.GetCount() > search.GetCount()) {
			out.Put(data[0]);
			data.Remove(0);
		}
		if(data == search) {
			out.Put(replace);
			data.Clear();
		}
	}
	out.Put(data);
}

void ReplaceCC(Stream& out, Stream& in, const String& search, const String& replace)
{
	int bound = 1;
	while(bound < search.GetCount())
		bound += bound;
	int mask = bound - 1;
	Buffer<char> cb(bound);
	int start = 0;
	int count = 0;
	while(!in.IsEof()) {
		cb[(start + count++) & mask] = in.Get();
		if(count == search.GetCount()) {
			int n = bound - start;
			if(memcmp(~search, ~cb + start, n) == 0 &&
			   memcmp(~search + n, ~cb, start) == 0) {
				out.Put(replace);
				start = count = 0;
			}
			else {
				out.Put(cb[start]);
				start = (start + 1) & mask;
				count--;
			}
		}
	}
	out.Put(~cb + start, min(bound - start, count));
	out.Put(~cb, max(start + count - bound, 0));
}

void TestReplace(const char *text)
{
	StringStream ss(text);
	StringStream out;
	ReplaceCC(out, ss, "abc", "X");
	DDUMP(out.GetResult());
}

void TestReplace()
{
	TestReplace("abc12345abc1abc12abc123abc1234abc123abc");
	TestReplace("1abc12345abc1abc12abc123abc1234abc123abc1");
	TestReplace("12abc12345abc1abc12abc123abc1234abc123abc12");
	TestReplace("123abc12345abc1abc12abc123abc1234abc123abc123");
}
