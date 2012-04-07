#include "HttpRequest2.h"

#ifdef PLATFORM_WIN32
#include <winsock2.h>
#include <Ws2ipdef.h>
#include <Ws2tcpip.h>
#endif

#ifdef PLATFORM_POSIX
#include <arpa/inet.h>
#endif


namespace Upp {
	const char *TcpSocketErrorDesc(int code);
};

