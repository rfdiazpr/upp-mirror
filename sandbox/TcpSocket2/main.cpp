#include <Core/Core.h>
#include "TcpSocket.h"

using namespace Upp;

CONSOLE_APP_MAIN
{
	DDUMP(sizeof(fd_set));
//	DDUMP(RequestHttpGet("www.ultimatepp.com"));
	DDUMP(RequestHttpGet("www.idnes.cz"));
}
