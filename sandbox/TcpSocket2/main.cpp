#include <Core/Core.h>
#include "TcpSocket.h"

using namespace Upp;

CONSOLE_APP_MAIN
{
	DDUMP(sizeof(fd_set));
//	DDUMP(RequestHttpGet("www.ultimatepp.com"));

#if 0
	DDUMP(HttpRequest("www.idnes.cz").Execute());
	DLOG("=======================");
	DDUMP(HttpRequest("www.ultimatepp.org").Execute());
#endif

//	HttpRequest h("www.idnes.cz"); // Chunked
//	HttpRequest h("www.ultimatepp.org"); // Normal
	HttpRequest h("www.google.com"); // Redirect to google.cz

	while(h.Do());
	
	DDUMP(h.GetContent());
}
