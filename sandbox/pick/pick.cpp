#include <Core/Core.h>

using namespace Upp;

struct Foo;

template <class T>
struct pick {
	T& src;
	
	T& operator~()             { return src; }
	operator T&() const        { return src; }
	
	pick(T& src) : src(src)    {}
};

struct PickType {
	PickType(PickType& t)       { LOG("PickType(PickType&)"); }
	void operator=(PickType& t) { LOG("operator=(PickType&)"); }
	PickType()                  { LOG("PickType"); }
};

struct Foo : PickType {
	int x;

	Foo()                      { LOG("Foo();"); }
	
	Foo(Foo& a)                { LOG("Foo(Foo&);"); }
	void operator=(Foo& a)     { LOG("operator=(Foo&a)"); }
	
//	Foo(pick<Foo> a)           { LOG("Foo(pick<Foo>& a)"); }
//	void operator=(pick<Foo>)  { LOG("operator=(pick<Foo>)"); }
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

void Another(pick<

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
	
	
	LOG("============ Bar b = Bar()");
	Bar b = Bar();
	LOG("============ b = Bar()");
	b = Bar();
	LOG("============ b = MakeBar()");
	b = MakeBar();

	return;

	const Foo y4;
//	y3 = y4;
}
