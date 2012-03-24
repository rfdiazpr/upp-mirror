#include <Core/Core.h>
#include "TcpSocket.h"

#ifdef PLATFORM_WIN32
#include <winsock2.h>
#include <Ws2ipdef.h>
#include <Ws2tcpip.h>
#endif

NAMESPACE_UPP

#ifdef PLATFORM_WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

#define LLOG(x)   DLOG(msecs() << " " << x)

#ifdef PLATFORM_POSIX

#define SOCKERR(x) x

#include <arpa/inet.h>

const char *TcpSocketErrorDesc(int code)
{
	return strerror(code);
}

int TcpSocket::GetErrorCode()
{
	return errno;
}

#else

#define SOCKERR(x) WSA##x

const char *TcpSocketErrorDesc(int code)
{
	static Tuple2<int, const char *> err[] = {
		{ WSAEINTR,                 "Interrupted function call." },
		{ WSAEACCES,                "Permission denied." },
		{ WSAEFAULT,                "Bad address." },
		{ WSAEINVAL,                "Invalid argument." },
		{ WSAEMFILE,                "Too many open files." },
		{ WSAEWOULDBLOCK,           "Resource temporarily unavailable." },
		{ WSAEINPROGRESS,           "Operation now in progress." },
		{ WSAEALREADY,              "Operation already in progress." },
		{ WSAENOTSOCK,              "TcpSocket operation on nonsocket." },
		{ WSAEDESTADDRREQ,          "Destination address required." },
		{ WSAEMSGSIZE,              "Message too long." },
		{ WSAEPROTOTYPE,            "Protocol wrong type for socket." },
		{ WSAENOPROTOOPT,           "Bad protocol option." },
		{ WSAEPROTONOSUPPORT,       "Protocol not supported." },
		{ WSAESOCKTNOSUPPORT,       "TcpSocket type not supported." },
		{ WSAEOPNOTSUPP,            "Operation not supported." },
		{ WSAEPFNOSUPPORT,          "Protocol family not supported." },
		{ WSAEAFNOSUPPORT,          "Address family not supported by protocol family." },
		{ WSAEADDRINUSE,            "Address already in use." },
		{ WSAEADDRNOTAVAIL,         "Cannot assign requested address." },
		{ WSAENETDOWN,              "Network is down." },
		{ WSAENETUNREACH,           "Network is unreachable." },
		{ WSAENETRESET,             "Network dropped connection on reset." },
		{ WSAECONNABORTED,          "Software caused connection abort." },
		{ WSAECONNRESET,            "Connection reset by peer." },
		{ WSAENOBUFS,               "No buffer space available." },
		{ WSAEISCONN,               "TcpSocket is already connected." },
		{ WSAENOTCONN,              "TcpSocket is not connected." },
		{ WSAESHUTDOWN,             "Cannot send after socket shutdown." },
		{ WSAETIMEDOUT,             "Connection timed out." },
		{ WSAECONNREFUSED,          "Connection refused." },
		{ WSAEHOSTDOWN,             "Host is down." },
		{ WSAEHOSTUNREACH,          "No route to host." },
		{ WSAEPROCLIM,              "Too many processes." },
		{ WSASYSNOTREADY,           "Network subsystem is unavailable." },
		{ WSAVERNOTSUPPORTED,       "Winsock.dll version out of range." },
		{ WSANOTINITIALISED,        "Successful WSAStartup not yet performed." },
		{ WSAEDISCON,               "Graceful shutdown in progress." },
		{ WSATYPE_NOT_FOUND,        "Class type not found." },
		{ WSAHOST_NOT_FOUND,        "Host not found." },
		{ WSATRY_AGAIN,             "Nonauthoritative host not found." },
		{ WSANO_RECOVERY,           "This is a nonrecoverable error." },
		{ WSANO_DATA,               "Valid name, no data record of requested type." },
		{ WSASYSCALLFAILURE,        "System call failure." },
	};
	const Tuple2<int, const char *> *x = FindTuple(err, __countof(err), code);
	return x ? x->b : "Unknown error code.";
}

int TcpSocket::GetErrorCode()
{
	return WSAGetLastError();
}

#endif


void TcpSocket::Init()
{
#if defined(PLATFORM_WIN32)
	ONCELOCK {
		WSADATA wsadata;
		WSAStartup(MAKEWORD(2, 2), &wsadata);
	}
#endif
}

void TcpSocket::Reset()
{
	is_eof = false;
	socket = INVALID_SOCKET;
	ipv6 = false;
	ptr = end = buffer;
}

bool TcpSocket::Open(int family, int type, int protocol)
{
	Init();
	CloseRaw();
	ClearError();
	if((socket = ::socket(family, type, protocol)) == INVALID_SOCKET)
		return false;
	LLOG("TcpSocket::Data::Open() -> " << (int)socket);
#ifdef PLATFORM_WIN32
	u_long arg = 1;
	if(ioctlsocket(socket, FIONBIO, &arg))
		SetSockError("ioctlsocket(FIO[N]BIO)");
#else
	if(fcntl(socket, F_SETFL, (fcntl(socket, F_GETFL, 0) | O_NONBLOCK)))
		SetSockError("fcntl(O_[NON]BLOCK)");
#endif
	return true;
}

bool TcpSocket::Listen(int port, int listen_count, bool ipv6_, bool reuse)
{
	ipv6 = ipv6_;
	if(!Open(ipv6 ? AF_INET6 : AF_INET, SOCK_STREAM, 0))
		return false;
	sockaddr_in sin;
#ifdef PLATFORM_WIN32
	SOCKADDR_IN6 sin6;
	if(ipv6)
#else
	sockaddr_in6 sin6;
	if(ipv6 && IsWinVista())
#endif
	{
		Zero(sin6);
		sin.sin_family = AF_INET6;
		sin.sin_port = htons(port);
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else {
		Zero(sin);
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	if(reuse) {
		int optval = 1;
		setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, sizeof(optval));
	}
	if(bind(socket, ipv6 ? (const sockaddr *)&sin6 : (const sockaddr *)&sin,
	        ipv6 ? sizeof(sin6) : sizeof(sin))) {
		SetSockError(NFormat("bind(port=%d)", port));
		return false;
	}
	if(listen(socket, listen_count)) {
		SetSockError(NFormat("listen(port=%d, count=%d)", port, listen_count));
		return false;
	}
	return true;
}

bool TcpSocket::Accept(TcpSocket& ls, int timeout)
{
	CloseRaw();
	if(timeout && !ls.Peek(timeout))
		return false;
	if(!Open(ls.ipv6 ? AF_INET6 : AF_INET, SOCK_STREAM, 0))
		return false;
	socket = accept(ls.GetSOCKET(), NULL, NULL);
	if(socket == INVALID_SOCKET) {
		SetSockError("accept");
		return false;
	}
	return true;
}

String TcpSocket::GetPeerAddr() const
{
	if(!IsOpen())
		return Null;
	sockaddr_in addr;
	socklen_t l = sizeof(addr);
	if(getpeername(socket, (sockaddr *)&addr, &l) != 0)
		return Null;
	if(l > sizeof(addr))
		return Null;
#ifdef PLATFORM_WIN32
	return inet_ntoa(addr.sin_addr);
#else
	char h[200];
	return inet_ntop(AF_INET, &addr.sin_addr, h, 200);
#endif
}

void TcpSocket::NoDelay()
{
	ASSERT(IsOpen());
	int __true = 1;
	LLOG("NoDelay(" << (int)socket << ")");
	if(setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (const char *)&__true, sizeof(__true)))
		SetSockError("setsockopt(TCP_NODELAY)");
}

void TcpSocket::Linger(int msecs)
{
	ASSERT(IsOpen());
	linger ls;
	ls.l_onoff = !IsNull(msecs) ? 1 : 0;
	ls.l_linger = !IsNull(msecs) ? (msecs + 999) / 1000 : 0;
	if(setsockopt(socket, SOL_SOCKET, SO_LINGER,
		reinterpret_cast<const char *>(&ls), sizeof(ls)))
		SetSockError("setsockopt(SO_LINGER)");
}

void TcpSocket::Attach(SOCKET s)
{
	CloseRaw();
	socket = s;
}

bool TcpSocket::Connect(const char *host, int port)
{
	LLOG("TcpSocket::Data::OpenClient(" << host << ':' << port << ')');

	Init();

	addrinfo hints;
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	addrinfo *result;
	if(getaddrinfo(host, ~AsString(port), &hints, &result) || !result) {
		SetSockError(NFormat("getaddrinfo(%s) failed", host));
		return false;
	}
	
	addrinfo *rp = result;
	for(;;) {
		if(Open(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) {
			if(connect(socket, rp->ai_addr, rp->ai_addrlen) == 0 ||
			   GetErrorCode() == SOCKERR(EINPROGRESS) || GetErrorCode() == SOCKERR(EWOULDBLOCK))
				break;
			CloseRaw();
		}
		rp = rp->ai_next;
		if(!rp) {
			SetSockError(NFormat("unable to open or bind socket for %s", host));
			freeaddrinfo(result);
			return false;
		}
    }

	freeaddrinfo(result);
	return true;
}

bool TcpSocket::CloseRaw()
{
	SOCKET old_socket = socket;
	socket = INVALID_SOCKET;
	if(old_socket != INVALID_SOCKET) {
		LLOG("TcpSocket::CloseRaw(" << (int)old_socket << ")");
		int res;
#if defined(PLATFORM_WIN32)
		res = closesocket(old_socket);
#elif defined(PLATFORM_POSIX)
		res = close(old_socket);
#else
	#error Unsupported platform
#endif
		if(res && !IsError()) {
			SetSockError("close");
			return false;
		}
	}
	return true;
}

bool TcpSocket::Close(int timeout)
{
	if(socket == INVALID_SOCKET)
		return false;
	return !IsError() && Peek(timeout, true) && CloseRaw();
}

bool TcpSocket::WouldBlock()
{
	int c = GetErrorCode();
#ifdef PLATFORM_POSIX
	return c == SOCKERR(EWOULDBLOCK) || c == SOCKERR(EAGAIN);
#endif
#ifdef PLATFORM_WIN32
	return c == SOCKERR(EWOULDBLOCK);
#endif
}

bool TcpSocket::Peek(int timeout, bool write)
{
	LLOG("Peek(" << timeout << ", " << write << ")");
	if(!write && !leftover.IsEmpty())
		return true;
	if(timeout < 0 && !IsNull(timeout) || socket == INVALID_SOCKET)
		return false;
	timeval *tvalp = NULL;
	timeval tval;
	if(!IsNull(timeout)) {
		tval.tv_sec = timeout / 1000;
		tval.tv_usec = 1000 * (timeout % 1000);
		tvalp = &tval;
	}
	fd_set fdset[1];
	FD_ZERO(fdset);
	FD_SET(socket, fdset);
	int avail = select((int)socket + 1, write ? NULL : fdset, write ? fdset : NULL, NULL, tvalp);
	LLOG("//Peek(" << timeout << ", " << write << ") avail: " << avail);
	return avail > 0;
}

int TcpSocket::Send(const void *buf, int amount)
{
	int res = send(socket, (const char *)buf, amount, 0);
	if(res < 0 && WouldBlock())
		res = 0;
	else
	if(res == 0 || res < 0)
		SetSockError("send");
	return res;
}

void TcpSocket::StopWrite()
{
	ASSERT(IsOpen());
	if(shutdown(socket, SD_SEND))
		SetSockError("shutdown(SD_SEND)");
}

String TcpSocket::GetHostName()
{
	Init();
	char buffer[256];
	gethostname(buffer, __countof(buffer));
	return buffer;
}

int TcpSocket::WriteWait(const char *s, int length, int timeout_msec)
{
	LLOG("WriteWait(@ " << socket << ": " << length << ", Tmax = " << timeout_msec << ")");
	ASSERT(IsOpen());
	if(length < 0 && s)
		length = (int)strlen(s);
	if(!s || length <= 0 || IsError())
		return 0;
	int done = 0;
	int end_ticks = msecs() + timeout_msec;
	bool peek = false;
	while(done < length) {
		if(peek && !PeekWrite(IsNull(timeout_msec) ? (int)Null : max(end_ticks - msecs(), 0)))
			return done;
		peek = false;
		int count = Send(s + done, length - done);
		if(IsError())
			return done;
		if(count > 0)
			done += count;
		else
			peek = true;
	}
	LLOG("//WriteWait() -> " << done);
	return done;
}

int TcpSocket::Recv(void *buf, int amount)
{
	int res = recv(socket, (char *)buf, amount, 0);
	if(res == 0)
		is_eof = true;
	else
	if(res < 0 && WouldBlock())
		res = 0;
	else
	if(res < 0)
		SetSockError("recv");
	LLOG("recv(" << socket << "): " << res << " bytes: "
	     << AsCString((char *)buf, (char *)buf + min(res, 16))
	     << (res ? "" : IsEof() ? ", EOF" : ", WOULDBLOCK"));
	return res;
}

void TcpSocket::ReadBuffer()
{
	ptr = buffer;
	end = buffer + Recv(buffer, BUFFERSIZE);	
}

int TcpSocket::Get0()
{
	ReadBuffer();
	return ptr < end ? *ptr++ : -1;
}

int TcpSocket::Peek0()
{
	ReadBuffer();
	return ptr < end ? *ptr : -1;
}

int TcpSocket::Read(void *buffer, int count, int timeout, Gate abort, int abortstep)
{
	LLOG("Read " << maxlen << ", timeout: " << timeout);

	if(!IsOpen() || IsError() || IsEof())
		return String::GetVoid();
	
	String out;
	int l = end - ptr;
	if(l > 0)
		if(l < count) {
			out = String(ptr, end);
			ptr = end;
			count -= l;
		}
		else {
			out = String(ptr, maxlen);
			ptr += maxlen;
			return out;
		}
	if(timeout && !Peek(timeout))
		return Null;
	int endtime = msecs() + Nvl(timeout);
	int done = Recv((char *)buffer, count);
	while(done < count && !IsError() && !IsEof()) {
		int time = msecs();
		if(time > endtime ||
		   !PeekRead(abort ? abortstep : IsNull(timeout) ? (int)Null : max(endtime - msecs(), 0)))
			break;
		int part = Recv((char *)buffer + done, count - done);
		if(part > 0)
			done += part;
	}
	return done;
}

String TcpSocket::Read(int len, int timeout, Gate abort, int steptime)
{
}

String TcpSocket::ReadCount(int count, int timeout)
{
	if(count == 0)
		return Null;
	StringBuffer out(count);
	int done = ReadCount(out, count, timeout);
	if(!done && IsEof())
		return String::GetVoid();
	out.SetLength(done);
	return out;
}

int Find(const String& s, Gate1<int> term, int seek)
{
	for(int i = seek; i < s.GetCount(); i++)
		if(term(s[i]))
			return i;
	return -1;
}

String TcpSocket::ReadUntil(int term, int timeout, int maxlen, Gate abort, int steptime)
{
	LLOG("Socket::ReadUntil(term = " << (int)term << ", maxlen = " << maxlen << ", timeout = " << timeout << ")");
	ASSERT(IsOpen() && maxlen != 0);
	int end_time = IsNull(timeout) ? INT_MAX : msecs() + timeout;
	String out = leftover;
	leftover.Clear();
	int from = 0;
	
//	_DBG_; abort.Clear();
	
	while(!IsEof() && !IsError()) {
		int f = out.Find(term, from);
		if(f >= 0) {
			leftover = out.Mid(f + 1);
			DDUMP(leftover.GetCount());
			out.Trim(f);
			return out;
		}
		if(msecs() >= end_time) {
			SetSockError("timeout");
			break;
		}
		if(abort())
			break;
		from = leftover.GetCount();
		int n = maxlen - from;
		if(n <= 0)
			break;
		out.Cat(Read(abort ? steptime : end_time - msecs(), n));
	}
	return String::GetVoid();
}

String TcpSocket::ReadLine(int timeout, int maxlen, Gate step, int steptime)
{
	String ln = ReadUntil('\n', timeout, maxlen, step, steptime);
	if(*ln.Last() == '\r')
		ln.Trim(ln.GetCount() - 1);
	return ln;
}

void TcpSocket::UnRead(const void *buffer, int len)
{
	ASSERT(len >= 0);
	ASSERT(IsOpen());
	if(len > 0)
		leftover.Insert(0, (const char *)buffer, len);
}

void TcpSocket::SetSockError(const char *context, const char *errdesc)
{
	String err;
	errorcode = GetErrorCode();
	if(socket != INVALID_SOCKET)
		err << "socket(" << (int)socket << ") / ";
	err << context << ": " << errdesc;
	errordesc = err;
	is_error = true;
}

void TcpSocket::SetSockError(const char *context)
{
	SetSockError(context, TcpSocketErrorDesc(GetErrorCode()));
}

END_UPP_NAMESPACE
