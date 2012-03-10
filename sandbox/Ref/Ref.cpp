#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Color c;
	Ref f = c;
	DDUMP(f.Is<Color>());
	DDUMP(f.Is<double>());
	f.Get<Color>() = Red();
	DDUMP(c);
}
