#include <Core/Core.h>
#include "TcpSocket.h"

#ifdef PLATFORM_WIN32
#include <winsock2.h>
#endif

NAMESPACE_UPP

#ifdef PLATFORM_WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

enum {
	NB_TIMEOUT  = 30000,
	SOCKBUFSIZE = 65536, // Too big!!!

#ifdef PLATFORM_WIN32
	IS_BLOCKED = SOCKERR(EWOULDBLOCK),
#else
	IS_BLOCKED = SOCKERR(EWOULDBLOCK),
#endif
};

#define LLOG(x)  // LOG(x)

#ifdef PLATFORM_POSIX

#include <arpa/inet.h>

const char *TcpSocketErrorDesc(int code)
{
	return strerror(code);
}

#else

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

#endif

void TcpSocket::Reset()
{
	is_blocking = true;
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


static StaticCriticalSection s_hostbyname_lock;

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

bool TcpSocket::Open(bool block)
{
	Init();
	Close(0);
	is_blocking = block;
	ClearError();
	if((socket = ::socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		return false;
	LLOG("TcpSocket::Data::Open() -> " << (int)socket);
	if(!block)
		Block(false);
	return true;
}

bool TcpSocket::OpenClient(const char *host, int port, bool nodelay, dword *my_addr, int timeout, bool block)
{
	LLOG("TcpSocket::Data::OpenClient(" << host << ':' << port << ", timeout " << timeout << ", block " << block << ')');

	int ticks = msecs();
	sockaddr_in sin;
	sockaddr_in addr;

	{
		CriticalSection::Lock __(s_hostbyname_lock);
		TcpSocket::Init();
		hostent *he = gethostbyname(host);
		if(!he) {
			SetSockError(NFormat("gethostbyname(%s) failed", host));
			return false;
		}

		Zero(sin);
		sin.sin_family = AF_INET;
		sin.sin_port = htons(0);
		sin.sin_addr.s_addr = htonl(INADDR_ANY);

		Zero(addr);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr = *(in_addr *)(he -> h_addr_list[0]);
	}

	if(!Open(block))
		return false;

	if(nodelay)
		NoDelay();

	while(bind(socket, (const sockaddr *)&sin, sizeof(sin))) {
		if(TcpSocket::GetErrorCode() != SOCKERR(EINPROGRESS) || !IsNull(timeout) && msecs(ticks) >= timeout) {
			SetSockError(NFormat("bind(host=%s, port=%d)", FormatIP(Peek32be(&addr.sin_addr)), port));
			return false;
		}
		Sleep(500);
	}
	if(my_addr)
		*my_addr = sin.sin_addr.s_addr;

	if(!connect(socket, (sockaddr *)&addr, sizeof(addr)))
		return true;

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
/*
	if(!Peek(IsNull(timeout) ? NB_TIMEOUT : max<int>(0, timeout - msecs(ticks)), true)) {
		SetSockError("connect timeout expired");
		return false;
	}
*/
	return true;
}

bool TcpSocket::Close(int msecs_timeout)
{
	LLOG("TcpSocket::Data::Close(" << (int)socket << ", timeout = " << msecs_timeout << ")");
	bool ok = CloseRaw(msecs_timeout);
	if(ok)
		socket = INVALID_SOCKET;
	LLOG("//TcpSocket::Data::Close, ok = " << ok);
	return ok;
}

bool TcpSocket::CloseRaw(int msecs_timeout)
{
	if(socket == INVALID_SOCKET)
		return false;
	bool ok = !IsError() && Peek(msecs_timeout, true);
	SOCKET old_socket = socket;
	socket = INVALID_SOCKET;
	if(old_socket != INVALID_SOCKET) {
		LLOG("TcpSocket::Data::CloseRaw(" << (int)old_socket << ")");
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
			ok = false;
		}
	}
	LLOG("//TcpSocket::Data::CloseRaw, ok = " << ok);
	return ok;
}

int TcpSocket::WriteRaw(const void *buf, int amount)
{
	int res = send(socket, (const char *)buf, amount, 0);
	if(res == 0 || res < 0 && TcpSocket::GetErrorCode() != IS_BLOCKED)
		SetSockError("send");
	return res;
}

bool TcpSocket::Accept(TcpSocket& socket, dword *ipaddr, bool nodelay, int timeout_msec)
{
	SOCKET connection = AcceptRaw(ipaddr, timeout_msec);
	if(connection == INVALID_SOCKET)
		return false;
	socket.Attach(connection, nodelay, is_blocking);
	return true;
}

SOCKET TcpSocket::AcceptRaw(dword *ipaddr, int timeout_msec)
{
	ASSERT(IsOpen());
	if(!IsNull(timeout_msec) && !Peek(timeout_msec, false))
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

void TcpSocket::AttachRaw(SOCKET s, bool blocking)
{
	CloseRaw(0);
	socket = s;
	is_blocking = blocking;
}

void TcpSocket::Attach(SOCKET s, bool nodelay, bool blocking)
{
	AttachRaw(s, blocking);
	if(nodelay)
		NoDelay();
	if(!is_blocking)
		Block(false);
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

void TcpSocket::Block(bool b)
{
	ASSERT(IsOpen());
#ifdef PLATFORM_WIN32
	u_long arg = b ? 0 : 1;
	if(ioctlsocket(socket, FIONBIO, &arg))
		SetSockError("ioctlsocket(FIO[N]BIO)");
#else
	if(fcntl(socket, F_SETFL, (fcntl(socket, F_GETFL, 0) & ~O_NONBLOCK) | (b ? 0 : O_NONBLOCK)))
		SetSockError("fcntl(O_[NON]BLOCK)");
#endif
	is_blocking = b;
}

/*
void TcpSocket::Data::WriteTimeout(int msecs)
{
	ASSERT(IsOpen());
	if(IsNull(msecs)) msecs = 0;
	if(setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&msecs, sizeof(msecs))) {
		SetSockError("setsockopt(SO_SNDTIMEO)");
		LLOG("msecs = " << msecs);
	}
}
*/

/*
void TcpSocket::Data::ReadTimeout(int msecs)
{
	ASSERT(IsOpen());
	if(IsNull(msecs)) msecs = 0;
	if(setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&msecs, sizeof(msecs)))
		SetSockError("setsockopt(SO_RCVTIMEO)");
}
*/

void TcpSocket::StopWrite()
{
	ASSERT(IsOpen());
	if(shutdown(socket, SD_SEND))
		SetSockError("shutdown(SD_SEND)");
}

void TcpSocket::Init()
{
#if defined(PLATFORM_WIN32)
	ONCELOCK {
		WSADATA wsadata;
		WSAStartup(0x101, &wsadata);
	}
#elif defined(PLATFORM_POSIX)
	// no initialization necessary
#else
#error Unsupported platform
#endif
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
	LLOG("WriteWait(@ " << GetNumber() << ": " << length << ", Tmax = " << timeout_msec << ")");
	ASSERT(IsOpen());
	if(length < 0 && s)
		length = (int)strlen(s);
	if(!s || length <= 0 || IsError())
		return 0;
/*
	if(data->is_blocking) {
//		data->WriteTimeout(timeout_msec);
		int count = data->Write(s, length);
		LLOG("WriteWait(blocking, " << length << ", " << timeout_msec << ") -> " << count);
		if(count == 0) {
			SetSockError("WriteWait(blocking)->broken line");
			return 0;
		}
		if(count < 0 / * && TcpSocket::GetErrorCode() != SOCKERR(ETIMEDOUT)* /)
			SetSockError("WriteWait(blocking)");
		LLOG("//WriteWait(blocking) -> " << count);
		return max(count, 0);
	}
*/
	int done = 0;
	int end_ticks = msecs() + timeout_msec;
	bool peek = is_blocking;
	while(done < length) {
		if(peek) {
			int delta = IsNull(timeout_msec) ? NB_TIMEOUT : max(end_ticks - msecs(), 0);
			if(!PeekWrite(delta))
				return done;
			peek = is_blocking;
		}
		int count = WriteRaw(s + done, length - done);
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

bool TcpSocket::Peek(int timeout_msec, bool write) // Redo to bitmask options
{
	if(!write && !leftover.IsEmpty())
		return true;
	if(timeout_msec < 0 && !IsNull(timeout_msec) || socket == INVALID_SOCKET)
		return false;
	fd_set set[1];
	FD_ZERO(set);
	int maxindex = (int)socket;
	FD_SET(socket, set);
	timeval *tvalp = NULL;
	timeval tval;
	if(!IsNull(timeout_msec)) {
		tval.tv_sec = timeout_msec / 1000;
		tval.tv_usec = 1000 * (timeout_msec % 1000);
		tvalp = &tval;
	}
	int avail;
	if(write)
		avail = write ? select(maxindex + 1, NULL, set, NULL, &tval) : select(maxindex + 1, set, NULL, NULL, &tval);
	return avail > 0;
}

bool TcpSocket::PeekAbort(int timeout_msec) // ????
{
	if(!IsOpen())
		return true;
	String left = leftover;
	leftover.Clear();
	char buffer;
	int count = -1;
	if(Peek(timeout_msec, false))
		count = ReadRaw(&buffer, 1);
	left.Cat(leftover);
	if(count > 0)
		left.Cat(buffer);
	leftover = left;
	return count == 0;
}

int TcpSocket::ReadRaw(void *buf, int amount)
{
	int res = recv(socket, (char *)buf, amount, 0);
	if(res == 0)
		is_eof = true;
	else
	if(res < 0 && GetErrorCode() != IS_BLOCKED)
		SetSockError("recv");
	return res;
}

String TcpSocket::Read(int timeout, int maxlen)
{
	if(!IsOpen())
		return String::GetVoid();
	if(leftover.GetCount()) {
		String out = leftover;
		leftover = Null;
		return out;
	}
	if(IsError() || IsEof())
		return String::GetVoid();
	int ticks = GetTickCount(), end_ticks = ticks + timeout;
	if(is_blocking && !IsNull(timeout) || !is_blocking && timeout != 0)
		if(!Peek(IsNull(timeout) ? NB_TIMEOUT : end_ticks - ticks))
			return Null;
	char buffer[SOCKBUFSIZE]; // Too big!!!
	int count = ReadRaw(buffer, min<int>(maxlen, sizeof(buffer)));
	LLOG("recv(" << GetNumber() << ") -> " << count << ": " << String(buffer, max(count, 64)));
	if(IsError())
		return String::GetVoid();
	if(count <= 0) {
		if(!IsEof()) {
			LLOG("TcpSocket::Read(" << GetNumber() << ") -> ewouldblock");
		}
		return Null;
	}
	return String(buffer, count);
}

int TcpSocket::ReadCount(void *buffer, int count, int timeout_msec)
{
	LLOG("ReadCount(@" << GetNumber() << ": " << count << ", Tmax = " << timeout_msec << ")");
	int done = 0;
	if(leftover.GetCount()) {
//		LLOG("-> leftover = <" << BinHexEncode(data->leftover) << ">");
		done = min(leftover.GetLength(), count);
		memcpy(buffer, ~leftover, done);
		if(done < leftover.GetCount()) {
			leftover.Remove(0, done);
			return done;
		}
		leftover.Clear();
	}
	int end_ticks = msecs() + timeout_msec;
	while(done < count && !IsError() && !IsEof()) { // Hard waiting if IsNull, should block here!
		if(!IsNull(timeout_msec) && !Peek(max(end_ticks - msecs(), 0)))
			break;
		int part = ReadRaw((char *)buffer + done, count - done);
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
/*
	do {
		String part = Read(IsNull(timeout_msec) ? int(Null) : end_ticks - msecs(), count - out.GetLength());
		if(part.IsVoid())
			return !IsNull(out) ? out : String::GetVoid();
		out.Cat(part);
	}
	while(out.GetLength() < count && (IsNull(timeout_msec) || msecs(end_ticks) <= 0));
	return out;
*/
}

String TcpSocket::ReadUntil(char term, int timeout, int maxlen)
{
	LLOG("TcpSocket::RecvUntil(term = " << (int)term << ", maxlen = " << maxlen << ", timeout = " << timeout << ")");
	ASSERT(IsOpen() && maxlen != 0);
	int ticks = GetTickCount(), end_ticks = IsNull(timeout) ? int(Null) : ticks + timeout, seek = 0;
	String out = Read(timeout, maxlen);
	if(out.IsVoid())
		return out;

	for(;;) {
		int f = out.Find((byte)term, seek);
		if(f >= 0) {
			leftover = String(out.Begin() + f + 1, out.GetLength() - f - 1) + leftover;
			return out.Left(f);
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
		int f = Find(out, term, seek);
		if(f >= 0) {
			termchar = out[f];
			leftover = String(out.Begin() + f + 1, out.GetLength() - f - 1) + leftover;
			return out.Left(f);
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

void TcpSocket::UnRead(const void *buffer, int len)
{
	ASSERT(len >= 0);
	ASSERT(IsOpen());
	if(len > 0)
		leftover.Insert(0, (const char *)buffer, len);
}

String TcpSocket::PeekCount(int count, int timeout_msec)
{
	String s = ReadCount(count, timeout_msec);
	leftover.Insert(0, s);
	return s;
}

String TcpSocket::PeekUntil(char term, int timeout_msec, int maxlen)
{
	String s = ReadUntil(term, timeout_msec, maxlen);
	leftover.Insert(0, s);
	return s;
}

// This part should be completely removed, replaced by new error handling
static thread__ char s_errortext[201];
static thread__ int  s_errortextlen;

String TcpSocket::GetErrorText()
{
	return String(s_errortext, s_errortextlen);
}

void TcpSocket::SetErrorText(String text)
{
	LLOG("TcpSocket::SetLastErrorText = " << text);
	s_errortextlen = min(text.GetLength(), 200);
	memcpy(s_errortext, ~text, s_errortextlen + 1);
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
	SetErrorText(err);
}

void TcpSocket::SetSockError(const char *context)
{
	SetSockError(context, TcpSocketErrorDesc(GetErrorCode()));
}

END_UPP_NAMESPACE
