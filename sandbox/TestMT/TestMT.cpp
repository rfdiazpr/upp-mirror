#include <Core/Core.h>

using namespace Upp;

int n;

void ThreadDo()
{
	LOG(++n);
	for(;;) {
		Sleep(Random(1000));
	}
}

CONSOLE_APP_MAIN
{
	for(int i = 0; i < 100000; i++) {
		Thread::Start(callback(ThreadDo));
	}
	ThreadDo();
}
