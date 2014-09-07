#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	ValueArray va;
	va.Add("Node");
	Value v = va;

	Value& v1 = va.At(1);

	v1 = va;
//	v1 = va.GetClone();

	DDUMP(va);
}
