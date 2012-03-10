#include <Core/Core.h>
#include "TcpSocket.h"

NAMESPACE_UPP

String FormatIP(dword _ip)
{
	byte ip[4];
	Poke32be(ip, _ip);
	return Format("%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
}

END_UPP_NAMESPACE