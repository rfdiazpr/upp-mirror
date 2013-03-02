#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct Foo {
	ArrayMap<String, String> map;
};

GUI_APP_MAIN
{
	One<int> tst = One<int>(new int);

	PromptOK("Hello!");
	DDUMP(sizeof(RichStyle));
	Foo foo, foo2;
	foo.map.Add("Hello", "World");
	foo2.map.Add("Hello", "World");
	foo2 = foo;
	return;
	
	RichText x = ParseQTF("H&H&H&H");
	RichText a;
	a = x;
	return;

	RichTextCtrl txt;
	
	
	txt.Pick(ParseQTF("Hello&Hello"));
	return;

	TopWindow win;
	Label lbl;
	lbl = "\1Hello!";
	win.Add(lbl.SizePos());

	
//	
	win.Run();
//	PromptOK("Hello world!");
//	PromptOK("Hello world!");

	Any any;
	any.Create<String>();
	any.Get<String>() = String(' ', 200);
	return;
}
