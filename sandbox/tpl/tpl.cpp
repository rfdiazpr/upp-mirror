#include <Core/Core.h>
#include <Wpp/Wpp.h>

using namespace Upp;

VectorMap<String, Value> var;

Value Eval(const char *s)
{
	One<Exe> exe = Compile(s, var.GetIndex());
	return Render(exe, var.GetValues());
}

void Test(const char *s, Value val)
{
	ASSERT(Eval(s) == val);
}

Value MakeMap(const Vector<Value>& arg)
{
	ValueMap m;
	m.Add("NAME", "John");
	m.Add("SURNAME", "Smith");
	m.Add("EMAIL", "smith@earth.org");
	m.Add("NESTED", m);	
	return m;
}

CONSOLE_APP_MAIN
{
	Compiler::Register("cycle", Cycle);
	Compiler::Register("MakeMap", MakeMap);
	Compiler::Register("raw", Raw);

	var.Add("n", 12);
	var.Add("s", "Hello world!");
	ValueArray va;
	va << "One" << "Two" << "Three" << "Four" << "Five" << "Six";
	var.Add("a", va);
	
	ValueMap m;
	m.Add("NAME", "John");
	m.Add("SURNAME", "Smith");
	m.Add("EMAIL", "smith@earth.org");
	m.Add("NESTED", m);

	var.Add("map", m);
/*
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
*/
	One<Exe> exe = Compile(LoadFile(GetDataFile("test.txt")), var.GetIndex());

#ifndef _DEBUG
	for(int i = 0; i < 100000; i++) {
		RTIMING("Benchmark");
		Render(exe, var.GetValues());
	}
#endif

	LOG(Render(exe, var.GetValues()));	
}
