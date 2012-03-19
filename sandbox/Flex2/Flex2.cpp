#include <Core/Core.h>

using namespace Upp;

int N = 10000000;

template <class T>
void Test()
{
	{
		T x[128];
		RLOG(sizeof(T) * 127);
		RTIMING(typeid(T).name());
		for(int i = 0; i < N; i++)
			memcpy(x, x + 1, sizeof(T) * 127);
	}
}

CONSOLE_APP_MAIN
{
	Test<byte>();
	Test<int>();
	Test<int64>();
	Test<Rect>();
}
