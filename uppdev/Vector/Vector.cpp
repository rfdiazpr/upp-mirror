#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Vector<String> test;
	test.Add("1");
	test.Add("2");
	test.Add("3");
	test.Add("5");
	test.Add("4");
	DUMPC(test);
	
	Vector<String> b = test;
	
	for(auto x: b)
		DLOG(x);
	
	DDUMP(__cplusplus);
	
#ifdef CPP_11
	LOG("C++ 11 active");
#endif
}
