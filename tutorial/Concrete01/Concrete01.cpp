#include <Core/Core.h>

using namespace Upp;

template <class T>
void Print(const T& a)
{
	Cout() << a << '\n';
}

CONSOLE_APP_MAIN
{
	String a;
	a = "Hello";
	Print(a);
	a = a + " world";
	Print(a);
	a.Cat('!');
	Print(a);
	a.Cat("ABCDEFGHIJKLM", 3);
	Print(a);
	a.Clear();
	for(int i = 0; i < 10; i++)
		a << i << ", ";
	Print(a);
	Print(String().Cat() << "Number is " << 123 << ".");
	a = "0123456789";
	Print(a.GetLength());
	a.Insert(6, "<inserted>");
	Print(a);
	a.Remove(2, 2);
	Print(a);
	Print(a.Find('e'));
	Print(a.ReverseFind('e'));
	Print(a.Find("ted"));
	Print(a.Mid(3, 3));
	Print(a.Mid(3));
	a.Trim(4);
	Print(a);
	Print(a[0]);
	Print(*a);
	Print(a);
}
