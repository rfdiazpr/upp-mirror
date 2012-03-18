#include <Core/Core.h>
#include "TcpSocket.h"

#ifdef PLATFORM_WIN32
#include <winsock2.h>
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
		WSAStartup(0x101, &wsadata);
	}
#endif
}

void TcpSocket::Reset()
{
	is_eof = false;
	socket = INVALID_SOCKET;
}

/*
bool TcpSocket::Data::OpenServer(int port, bool nodelay, int listen_count, bool block, bool reuse)
{
	if(!Open(block))
		return false;
	if(nodelay)
		NoDelay();
	sockaddr_in sin;
	Zero(sin);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if(reuse) {
		int optval = 1;
		setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, sizeof(optval));
	}
	if(bind(socket, (const sockaddr *)&sin, sizeof(sin))) {
		SetSockError(NFormat("bind(port=%d)", port));
		return false;
	}
	if(listen(socket, listen_count)) {
		SetSockError(NFormat("listen(port=%d, count=%d)", port, listen_count));
		return false;
	}
	return true;
}
*/
bool TcpSocket::Accept(TcpSocket& socket, dword *ipaddr, int timeout_msec)
{
	SOCKET connection = socket.AcceptRaw(ipaddr, timeout_msec);
	if(connection == INVALID_SOCKET)
		return false;
	socket.Attach(connection);
	return true;
}

SOCKET TcpSocket::AcceptRaw(dword *ipaddr, int timeout_msec)
{
	ASSERT(IsOpen());
	if(!IsNull(timeout_msec) && !Peek(timeout_msec, false)) // !! Why peek?
		return INVALID_SOCKET;
	sockaddr_in addr;
	Zero(addr);
	socklen_t addr_len = sizeof(addr);
	SOCKET connection = accept(socket, (sockaddr *)&addr, &addr_len);
	if(connection == INVALID_SOCKET) {
		SetSockError("accept");
		return INVALID_SOCKET;
	}
	dword ip = ntohl(addr.sin_addr.s_addr);
	if(ipaddr)
		*ipaddr = ip;
	LLOG("TcpSocket::Accept() -> " << (int)connection << " &" << FormatIP(ip));
	return connection;
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

bool TcpSocket::Open(int family, int type, int protocol)
{
	Init();
	Close(0);
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
	FD_ZERO(fdset);
	FD_SET(socket, fdset);
	return true;
}

void TcpSocket::Attach(SOCKET s)
{
	Close(0);
	socket = s;
}

bool TcpSocket::OpenClient(const char *host, int port)
{
	LLOG("TcpSocket::Data::OpenClient(" << host << ':' << port << ')');

	Init();

	addrinfo hints;
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	addrinfo *result;
	if(getaddrinfo(host, ~AsString(port), &hints, &result) || !result) {
		SetSockError(NFormat("getaddrinfo(%s) failed", host));
		return false;
	}
	
	addrinfo *rp = result;
	for(;;) {
		if(Open(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) {
			if(connect(socket, rp->ai_addr, rp->ai_addrlen) == 0 || GetErrorCode() == SOCKERR(EINPROGRESS))
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

	int err = TcpSocket::GetErrorCode();
#ifdef PLATFORM_WIN32
	if(err != SOCKERR(EWOULDBLOCK))
#else
	if(err != SOCKERR(EINPROGRESS))
#endif
	{
		SetSockError(NFormat("connect(%s:%d)", host, port));
		LLOG("TcpSocket::Data::OpenClient -> connect error, returning false");
		return false;
	}
	return true;
}

bool TcpSocket::CloseRaw()
{
	SOCKET old_socket = socket;
	socket = INVALID_SOCKET;
	if(old_socket != INVALID_SOCKET) {
		LLOG("TcpSocket::Close(" << (int)old_socket << ")");
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

bool TcpSocket::Close(int msecs_timeout)
{
	if(socket == INVALID_SOCKET)
		return false;
	return !IsError() && Peek(msecs_timeout, true) && CloseRaw();
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

bool TcpSocket::Peek(int timeout_msec, bool write) // !! Redo to bitmask options
{
	if(!write && !leftover.IsEmpty())
		return true;
	if(timeout_msec < 0 && !IsNull(timeout_msec) || socket == INVALID_SOCKET)
		return false;
	timeval *tvalp = NULL;
	timeval tval;
	if(!IsNull(timeout_msec)) {
		tval.tv_sec = timeout_msec / 1000;
		tval.tv_usec = 1000 * (timeout_msec % 1000);
		tvalp = &tval;
	}
	int avail = write ? select((int)socket + 1, NULL, fdset, NULL, &tval)
	                  : select((int)socket + 1, fdset, NULL, NULL, &tval);
	LLOG("Peek(" << timeout_msec << ", " << write << ") avail: " << avail);
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

String TcpSocket::Read(int timeout, int maxlen)
{
	if(!IsOpen())
		return String::GetVoid();
	if(leftover.GetCount()) {
		String out;
		if(leftover.GetCount() < maxlen) {
			out = leftover;
			leftover = Null;
		}
		else {
			out = leftover.Mid(0, maxlen);
			leftover = leftover.Mid(maxlen);
		}
		return out;
	}
	if(IsError() || IsEof())
		return String::GetVoid();
	if(timeout && !Peek(timeout))
		return Null;
	char buffer[4096];
	int count = Recv(buffer, min<int>(maxlen, sizeof(buffer)));
	if(IsError())
		return String::GetVoid();
	return String(buffer, count);
}

int TcpSocket::ReadCount(void *buffer, int count, int timeout_msec)
{
	LLOG("ReadCount(@" << socket << ": " << count << ", Tmax = " << timeout_msec << ")");
	int done = 0;
	if(leftover.GetCount()) {
//		LLOG("-> leftover = <" << BinHexEncode(data->leftover) << ">");
		done = min(leftover.GetCount(), count);
		memcpy(buffer, ~leftover, done);
		if(done <= leftover.GetCount()) {
			leftover.Remove(0, done);
			return done;
		}
		leftover.Clear();
	}
	int end_ticks = msecs() + timeout_msec;
	while(done < count && !IsError() && !IsEof()) { // Hard waiting if IsNull, should block here!
		if(!IsNull(timeout_msec) && !Peek(max(end_ticks - msecs(), 0)))
			break;
		int part = Recv((char *)buffer + done, count - done);
		if(part > 0)
			done += part;
	}
	LLOG("//ReadCount -> " << done);
	return done;
}

String TcpSocket::ReadCount(int count, int timeout_msec)
{
	// int end_ticks = msecs() + timeout_msec; Mirek:unused
	if(count == 0)
		return Null;
	StringBuffer out(count);
	int done = ReadCount(out, count, timeout_msec);
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

String TcpSocket::ReadUntil(Gate1<int> term, int& termchar, int timeout, int maxlen)
{
	LLOG("TcpSocket::RecvUntil(term = " << (int)term << ", maxlen = " << maxlen << ", timeout = " << timeout << ")");
	ASSERT(IsOpen() && maxlen != 0);
	int ticks = GetTickCount(), end_ticks = IsNull(timeout) ? int(Null) : ticks + timeout, seek = 0;
	String out = Read(timeout, maxlen);
	if(out.IsVoid())
		return out;

	for(;;) {
		for(int f = seek; f < out.GetCount(); f++) {
			if(term((byte)out[f])) {
				termchar = out[f];
				leftover = String(out.Begin() + f + 1, out.GetLength() - f - 1) + leftover;
				return out.Left(f);
			}
		}
		seek = out.GetLength();
		ticks = GetTickCount();
		if(!IsNull(timeout)) timeout = end_ticks - ticks;
		if(!IsNull(timeout) && timeout <= 0 || out.GetLength() >= maxlen)
			return out;
		String part = Read(timeout, maxlen - out.GetLength());
		if(part.IsVoid()) {
			LLOG("term " << (int)term << " not found in: " << out);
			return out;
		}
		out.Cat(part);
	}
}

String TcpSocket::ReadUntil(int term, int timeout, int maxlen)
{
	Chr h;
	h.chr = term;
	return ReadUntil(callback(&h, &Chr::Check), timeout, maxlen);
}

String TcpSocket::ReadUntil(int (*term)(int c), int timeout, int maxlen)
{
	Filter h;
	h.filter = term;
	return ReadUntil(callback(&h, &Filter::Check), timeout, maxlen);
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
