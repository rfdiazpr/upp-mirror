#include <Core/Core.h>

using namespace Upp;

struct Foo;

template <class T>
struct pick {
	T& foo;
	
	T& operator~()             { return foo; }
	operator T&()              { LOG("operator T&"); return foo; }
	
	pick(T& foo) : foo(foo)    { LOG("pick(T&)"); }
};

struct Foo {
	int x;
	
	Foo(Foo& a)                { LOG("Foo(Foo& a)"); }
	Foo(pick<Foo> a)           { LOG("Foo(pick<Foo>& a)"); }
	Foo()                      { LOG("Foo();"); }

	void operator=(Foo&)       { LOG("opreator=(Foo&)"); }
	void operator=(pick<Foo>)  { LOG("operator=(pick<Foo>)"); }
	
	operator pick<Foo>()       { LOG("operator pick()"); return pick<Foo>(*this); }
};

struct Bar {
	Foo a;
	Foo b;
	int c;

	Bar() {}
	Bar(pick<Bar> a)            { LOG("Bar(pick<Bar>)"); *this = ~a; }
	void operator=(pick<Bar> b) { LOG("operator=(pick<Bar>)"); *this = ~b; }
	operator pick<Bar>()        { LOG("operator Bar::pick()"); return pick<Bar>(*this); }
};

Foo MakeFoo()
{
	Foo foo;
	return foo;
}

Bar MakeBar()
{
	Bar bar;
	return bar;
}

CONSOLE_APP_MAIN
{
	Foo x;
	
	LOG("=====");
	
	Foo y(x);

	LOG("=====");
	
	Foo yy = x;

	LOG("=====");

	Foo y3 = Foo();

	LOG("=====");

	y3 = Foo();

	LOG("=====");
	
	y3 = MakeFoo();
	
	LOG("===== Foo y5 = MakeFoo()");

	Foo y5 = MakeFoo();		
	
	
	LOG("============");
	Bar b = Bar();
	LOG("============ b = Bar()");
	b = Bar();
	LOG("============ b = MakeBar()");
	b = MakeBar();

	return;

	const Foo y4;
//	y3 = y4;
}
