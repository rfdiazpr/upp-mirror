#include <Core/Core.h>
#include <Wpp/Wpp.h>

using namespace Upp;

VectorMap<String, Value> var;

Value Eval(const char *s)
{
	One<Exe> exe = Compile(s, var.GetIndex());
	return Run(exe, var.GetValues());
}

void Test(const char *s, Value val)
{
	ASSERT(Eval(s) == val);
}

CONSOLE_APP_MAIN
{
	var.Add("n", 12);
	var.Add("s", "Hello world!");

	DDUMP(FormatIntHex(Eval("1 && 0")));
	
	DDUMP(Eval("123"));
	DDUMP(Eval("\"Hello\""));
	DDUMP(Eval("4 * n + s"));
	DDUMP(Eval("n - 1 + 1"));
	DDUMP(Eval("n * 10 / 10"));
	DDUMP(Eval("n % 10"));
	DDUMP(Eval("1 << 3"));
	DDUMP(FormatIntHex(Eval("0x80000000 >> 2")));
	DDUMP(FormatIntHex(Eval("0x80000000 >>> 2")));
	DDUMP(Eval("1 < n"));
	DDUMP(Eval("12 < n"));
	DDUMP(Eval("20 < n"));
	DDUMP(Eval("1 <= n"));
	DDUMP(Eval("12 <= n"));
	DDUMP(Eval("20 <= n"));
	DDUMP(Eval("1 > n"));
	DDUMP(Eval("12 > n"));
	DDUMP(Eval("20 > n"));
	DDUMP(Eval("1 >= n"));
	DDUMP(Eval("12 >= n"));
	DDUMP(Eval("20 >= n"));
	DDUMP(Eval("1 == n"));
	DDUMP(Eval("12 == n"));
	DDUMP(Eval("20 == n"));
	DDUMP(Eval("1 != n"));
	DDUMP(Eval("12 != n"));
	DDUMP(Eval("20 != n"));

	DDUMP(FormatIntHex(Eval("0x101 & 0x11")));
	DDUMP(FormatIntHex(Eval("0x101 ^ 0x11")));
	DDUMP(FormatIntHex(Eval("0x101 | 0x11")));
	DDUMP(Eval("1 && 0"));
	DDUMP(Eval("1 && 1"));
	DDUMP(Eval("0 && 1"));
	DDUMP(Eval("0 && 0"));
	DDUMP(Eval("1 || 0"));
	DDUMP(Eval("1 || 1"));
	DDUMP(Eval("0 || 1"));
	DDUMP(Eval("0 || 0"));
	      
	DDUMP(Eval("1 > 10 ? \"ano\" : \"ne\""));
	DDUMP(Eval("5 * 5 > 10 ? \"ano\" : \"ne\""));
}
