#include <Core/Core.h>

using namespace Upp;


CONSOLE_APP_MAIN
{
	ValueMap t;
	t("test") = 123;
	DUMP(t);
	DDUMP(t.At(0));
	
	ValueMap m;
	m("key1", "value1")("key2", "value2");
	
	Value vm = m;
	Value vm2 = m;	
	vm(1) = "123";
	
	DDUMP(vm);
	DDUMP(vm2);
	
	Value json;
	Value& h = json("monday");
	h("morning")(12) = GetSysDate();
	h("monday")("evening")(12) = GetSysTime();
	DDUMP(json);	
	
	ValueArray va;
	va.Add("Node");

	Value v = va;

	Value& v1 = va.At(1);

	v1 = 123;
	
//	v1 = va;
//	v1 = va.GetClone();

	DDUMP(va);
	
	Value v2;
	v2.At(0) = 1;
	Value v3 = v2;
	v2.At(1) = 2;
	DUMP(v2);
	DDUMP(v3);

	{	
		Value va;
		DDUMP(va);
		va << 1;
		DDUMP(va);
		va << 1 << "dva" << 3.0;
		DDUMP(va);
	}
}
