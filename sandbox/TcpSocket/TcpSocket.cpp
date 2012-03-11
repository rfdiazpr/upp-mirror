#include <Core/Core.h>
#include "TcpSocket.h"

#ifdef PLATFORM_WIN32
#include <winsock2.h>
#endif

NAMESPACE_UPP

#define FAKEERROR 0
#define FAKESLOWLINE 0 // 57600 // Bd fake line speed, 0 = off

#ifdef PLATFORM_WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

enum {
	NB_TIMEOUT  = 30000,
	SOCKBUFSIZE = 65536,

#ifdef PLATFORM_WIN32
	IS_BLOCKED = SOCKERR(EWOULDBLOCK),
#else
	IS_BLOCKED = SOCKERR(EWOULDBLOCK),
#endif
};

static bool LogTcpSocketFlag = false;

void LogTcpSockets(bool ls) { LogTcpSocketFlag = ls; }
bool IsLogTcpSockets()      { return LogTcpSocketFlag; }

#define SLOG(x)  if(!LogTcpSocketFlag) ; else RLOG("[" << GetSysTime() << " @ " << int(GetTickCount() % 10000u) << "] " << x)
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
	switch(code) {
#define ERRINFO(code, desc) case code: return desc " (" #code ")";

ERRINFO(WSAEINTR,                 "Interrupted function call.")
ERRINFO(WSAEACCES,                "Permission denied.")
ERRINFO(WSAEFAULT,                "Bad address.")
ERRINFO(WSAEINVAL,                "Invalid argument.")
ERRINFO(WSAEMFILE,                "Too many open files.")
ERRINFO(WSAEWOULDBLOCK,           "Resource temporarily unavailable.")
ERRINFO(WSAEINPROGRESS,           "Operation now in progress.")
ERRINFO(WSAEALREADY,              "Operation already in progress.")
ERRINFO(WSAENOTSOCK,              "TcpSocket operation on nonsocket.")
ERRINFO(WSAEDESTADDRREQ,          "Destination address required.")
ERRINFO(WSAEMSGSIZE,              "Message too long.")
ERRINFO(WSAEPROTOTYPE,            "Protocol wrong type for socket.")
ERRINFO(WSAENOPROTOOPT,           "Bad protocol option.")
ERRINFO(WSAEPROTONOSUPPORT,       "Protocol not supported.")
ERRINFO(WSAESOCKTNOSUPPORT,       "TcpSocket type not supported.")
ERRINFO(WSAEOPNOTSUPP,            "Operation not supported.")
ERRINFO(WSAEPFNOSUPPORT,          "Protocol family not supported.")
ERRINFO(WSAEAFNOSUPPORT,          "Address family not supported by protocol family.")
ERRINFO(WSAEADDRINUSE,            "Address already in use.")
ERRINFO(WSAEADDRNOTAVAIL,         "Cannot assign requested address.")
ERRINFO(WSAENETDOWN,              "Network is down.")
ERRINFO(WSAENETUNREACH,           "Network is unreachable.")
ERRINFO(WSAENETRESET,             "Network dropped connection on reset.")
ERRINFO(WSAECONNABORTED,          "Software caused connection abort.")
ERRINFO(WSAECONNRESET,            "Connection reset by peer.")
ERRINFO(WSAENOBUFS,               "No buffer space available.")
ERRINFO(WSAEISCONN,               "TcpSocket is already connected.")
ERRINFO(WSAENOTCONN,              "TcpSocket is not connected.")
ERRINFO(WSAESHUTDOWN,             "Cannot send after socket shutdown.")
ERRINFO(WSAETIMEDOUT,             "Connection timed out.")
ERRINFO(WSAECONNREFUSED,          "Connection refused.")
ERRINFO(WSAEHOSTDOWN,             "Host is down.")
ERRINFO(WSAEHOSTUNREACH,          "No route to host.")
ERRINFO(WSAEPROCLIM,              "Too many processes.")
ERRINFO(WSASYSNOTREADY,           "Network subsystem is unavailable.")
ERRINFO(WSAVERNOTSUPPORTED,       "Winsock.dll version out of range.")
ERRINFO(WSANOTINITIALISED,        "Successful WSAStartup not yet performed.")
ERRINFO(WSAEDISCON,               "Graceful shutdown in progress.")
ERRINFO(WSATYPE_NOT_FOUND,        "Class type not found.")
ERRINFO(WSAHOST_NOT_FOUND,        "Host not found.")
ERRINFO(WSATRY_AGAIN,             "Nonauthoritative host not found.")
ERRINFO(WSANO_RECOVERY,           "This is a nonrecoverable error.")
ERRINFO(WSANO_DATA,               "Valid name, no data record of requested type.")
//ERRINFO(WSA_INVALID_HANDLE,       "Specified event object handle is invalid.")
//ERRINFO(WSA_INVALID_PARAMETER,    "One or more parameters are invalid.")
//ERRINFO(WSA_IO_INCOMPLETE,        "Overlapped I/O event object not in signaled state.")
//ERRINFO(WSA_IO_PENDING,           "Overlapped operations will complete later.")
//ERRINFO(WSA_NOT_ENOUGH_MEMORY,    "Insufficient memory available.")
//ERRINFO(WSA_OPERATION_ABORTED,    "Overlapped operation aborted.")
//ERRINFO(WSAINVALIDPROCTABLE,      "Invalid procedure table from service provider.")
//ERRINFO(WSAINVALIDPROVIDER,       "Invalid service provider version number.")
//ERRINFO(WSAPROVIDERFAILEDINIT,    "Unable to initialize a service provider.")
ERRINFO(WSASYSCALLFAILURE,        "System call failure.")

#undef ERRINFO
	default: return "Unknown error code.";
	}
}

#endif

TcpSocket::Data::Data()
: socket(INVALID_SOCKET)
, is_blocking(true)
//, is_error(false)
, is_eof(false)
, fake_error(0)
{
	sock = NULL;
}

Value TcpSocket::Data::GetInfo(String info) const
{
	return ErrorValue(info);
}

bool TcpSocket::Data::Open(bool block)
{
	Init();
	Close(0);
	is_blocking = block;
	ClearError();
	if((socket = ::socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		return false;
	SLOG("TcpSocket::Data::Open() -> " << (int)socket);
	if(!block)
		Block(false);
	return true;
}

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

Mutex& GetHostByNameMutex()
{
	static StaticCriticalSection m;
	return m;
}

String TcpSocket::Data::GetPeerAddr() const
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

bool TcpSocket::Data::OpenClient(const char *host, int port, bool nodelay, dword *my_addr, int timeout, bool block)
{
	SLOG("TcpSocket::Data::OpenClient(" << host << ':' << port << ", timeout " << timeout << ", block " << block << ')');

	int ticks = msecs();
	sockaddr_in sin;
	sockaddr_in addr;

	{
		CriticalSection::Lock __(GetHostByNameMutex());
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

#if FAKEERROR
	fake_error = (rand() * 11111) % 100000;
	SLOG("TcpSocket::OpenClient -> fake error = " << fake_error);
#else
	fake_error = 0;
#endif

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
		SLOG("TcpSocket::Data::OpenClient -> connect error, returning false");
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

bool TcpSocket::Data::Close(int msecs_timeout)
{
	SLOG("TcpSocket::Data::Close(" << (int)socket << ", timeout = " << msecs_timeout << ")");
	bool ok = CloseRaw(msecs_timeout);
	SLOG("//TcpSocket::Data::Close, ok = " << ok);
	return ok;
}

bool TcpSocket::Data::CloseRaw(int msecs_timeout)
{
	if(socket == INVALID_SOCKET)
		return false;
	bool ok = !IsError() && Peek(msecs_timeout, true);
	SOCKET old_socket = socket;
	socket = INVALID_SOCKET;
	if(old_socket != INVALID_SOCKET) {
		SLOG("TcpSocket::Data::CloseRaw(" << (int)old_socket << ")");
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
	SLOG("//TcpSocket::Data::CloseRaw, ok = " << ok);
	return ok;
}

int TcpSocket::Data::Read(void *buf, int amount)
{
	int res = recv(socket, (char *)buf, amount, 0);
#if FAKESLOWLINE
	if(res > 0) {
		int end = msecs() + iscale(res, 10000, FAKESLOWLINE) + 10;
		for(int delta; (delta = end - msecs()) > 0; Sleep(delta))
			;
	}
#endif
#ifndef NOFAKEERROR
	if(fake_error && res > 0) {
		if((fake_error -= res) <= 0) {
			fake_error = 0;
			if(sock)
				sock->SetSockError(socket, "recv", 1, "fake error");
			return -1;
		}
		else
			LLOG("TcpSocket::Data::Read: fake error after " << fake_error);
	}
#endif
	if(res == 0)
		is_eof = true;
	else if(res < 0 && TcpSocket::GetErrorCode() != IS_BLOCKED)
		SetSockError("recv");
	return res;
}

int TcpSocket::Data::Write(const void *buf, int amount)
{
	int res = send(socket, (const char *)buf, amount, 0);
	if(res == 0 || res < 0 && TcpSocket::GetErrorCode() != IS_BLOCKED)
		SetSockError("send");
	return res;
}

bool TcpSocket::Data::Accept(TcpSocket& socket, dword *ipaddr, bool nodelay, int timeout_msec)
{
	SOCKET connection = AcceptRaw(ipaddr, timeout_msec);
	if(connection == INVALID_SOCKET)
		return false;
	One<TcpSocket::Data> data = new Data;
	data->Attach(connection, nodelay, is_blocking);
	socket.Attach(data);
	return true;
}

SOCKET TcpSocket::Data::AcceptRaw(dword *ipaddr, int timeout_msec)
{
	ASSERT(IsOpen());
	if(!IsNull(timeout_msec) && !Peek(timeout_msec, false))
		return INVALID_SOCKET;
	sockaddr_in addr;
	Zero(addr);
	socklen_t addr_len = sizeof(addr);
//	puts("TcpSocket::Accept: accepting socket...");
	SOCKET connection = accept(socket, (sockaddr *)&addr, &addr_len);
	if(connection == INVALID_SOCKET) {
		SetSockError("accept");
		return INVALID_SOCKET;
	}
//	puts("TcpSocket::Accept: socket accepted...");
	dword ip = ntohl(addr.sin_addr.s_addr);
	if(ipaddr)
		*ipaddr = ip;
	SLOG("TcpSocket::Accept() -> " << (int)connection << " &" << FormatIP(ip));
	return connection;
}

void TcpSocket::Data::AttachRaw(SOCKET s, bool blocking)
{
	CloseRaw(0);
	socket = s;
	is_blocking = blocking;
}

void TcpSocket::Data::Attach(SOCKET s, bool nodelay, bool blocking)
{
	AttachRaw(s, blocking);
	if(nodelay)
		NoDelay();
	if(!is_blocking)
		Block(false);
}

bool TcpSocket::Data::Peek(int timeout_msec, bool write)
{
	if(!write && !leftover.IsEmpty())
		return true;
	if(timeout_msec < 0 || socket == INVALID_SOCKET)
		return false;
	fd_set set[1];
	FD_ZERO(set);
	int maxindex = (int)socket;
	FD_SET(socket, set);
	timeval tval;
	tval.tv_sec = timeout_msec / 1000;
	tval.tv_usec = 1000 * (timeout_msec % 1000);
	int avail;
	if(write) {
		avail = select(maxindex + 1, 0, set, 0, &tval);
	}
	else {
		avail = select(maxindex + 1, set, 0, 0, &tval);
	}
	return avail > 0;
}

void TcpSocket::Data::SetSockError(const String& context)
{
	int errorcode = TcpSocket::GetErrorCode();
	SetSockError(context, errorcode, TcpSocketErrorDesc(errorcode));
}

void TcpSocket::Data::SetSockError(const String& context, int errorcode, const String& errordesc)
{
	if(sock)
		sock->SetSockError(socket, context, errorcode, errordesc);
}

void TcpSocket::Data::NoDelay()
{
	ASSERT(IsOpen());
	int __true = 1;
	SLOG("NoDelay(" << (int)socket << ")");
	if(setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (const char *)&__true, sizeof(__true)))
		SetSockError("setsockopt(TCP_NODELAY)");
}

void TcpSocket::Data::Linger(int msecs)
{
	ASSERT(IsOpen());
	linger ls;
	ls.l_onoff = !IsNull(msecs) ? 1 : 0;
	ls.l_linger = !IsNull(msecs) ? (msecs + 999) / 1000 : 0;
	if(setsockopt(socket, SOL_SOCKET, SO_LINGER,
		reinterpret_cast<const char *>(&ls), sizeof(ls)))
		SetSockError("setsockopt(SO_LINGER)");
}

void TcpSocket::Data::Block(bool b)
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
		SLOG("msecs = " << msecs);
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

void TcpSocket::Data::StopWrite()
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

bool TcpSocket::Close(int msecs_timeout)
{
	if(!data)
		return false;
	bool res = data->Close(msecs_timeout);
	data = NULL;
	return res;
}

int TcpSocket::WriteWait(const char *s, int length, int timeout_msec)
{
	SLOG("WriteWait(@ " << GetNumber() << ": " << length << ", Tmax = " << timeout_msec << ")");
	ASSERT(IsOpen());
	if(length < 0 && s)
		length = (int)strlen(s);
	if(!s || length <= 0 || IsError())
		return 0;
/*
	if(data->is_blocking) {
//		data->WriteTimeout(timeout_msec);
		int count = data->Write(s, length);
		SLOG("WriteWait(blocking, " << length << ", " << timeout_msec << ") -> " << count);
		if(count == 0) {
			SetSockError("WriteWait(blocking)->broken line");
			return 0;
		}
		if(count < 0 / * && TcpSocket::GetErrorCode() != SOCKERR(ETIMEDOUT)* /)
			SetSockError("WriteWait(blocking)");
		SLOG("//WriteWait(blocking) -> " << count);
		return max(count, 0);
	}
*/
	int done = 0;
	int end_ticks = msecs() + timeout_msec;
	bool peek = data->is_blocking;
	while(done < length) {
		if(peek) {
			int delta = IsNull(timeout_msec) ? NB_TIMEOUT : max(end_ticks - msecs(), 0);
			if(!PeekWrite(delta))
				return done;
			peek = data->is_blocking;
		}
		int count = data->Write(s + done, length - done);
		if(IsError())
			return done;
		if(count > 0)
			done += count;
		else
			peek = true;
	}
	SLOG("//WriteWait() -> " << done);
	return done;
}

bool TcpSocket::Wait(const Vector<SOCKET>& read, const Vector<SOCKET>& write, int timeout_msec)
{
	if(timeout_msec < 0 || read.IsEmpty() && write.IsEmpty())
		return false;
	fd_set rdset[1], wrset[1];
	FD_ZERO(rdset);
	FD_ZERO(wrset);
	SLOG("TcpSocket::Wait(" << read.GetCount() << " SOCKETs for read, " << write.GetCount() << " SOCKETs for write, " << timeout_msec << " msecs)");
	int maxindex = -1;
	int i;
	for(i = 0; i < read.GetCount(); i++)
		if(read[i] != INVALID_SOCKET) {
			SLOG("-> add read socket " << (int)read[i]);
			FD_SET(read[i], rdset);
			maxindex = max<int>(maxindex, (int)read[i]);
		}
	for(i = 0; i < write.GetCount(); i++)
		if(write[i] != INVALID_SOCKET) {
			SLOG("-> add write socket " << (int)write[i]);
			FD_SET(write[i], wrset);
			maxindex = max<int>(maxindex, (int)write[i]);
		}
	if(maxindex < 0)
		return false;
//	LOGHEXDUMP(set, sizeof(set));
	timeval tval;
	tval.tv_sec = timeout_msec / 1000;
	tval.tv_usec = 1000 * (timeout_msec % 1000);
	int avail = select(maxindex + 1, read.IsEmpty() ? 0 : rdset, write.IsEmpty() ? 0 : wrset, 0, &tval);
	SLOG("//TcpSocket::Wait -> avail = " << avail);
//	if(avail) { LOG("select() -> " << avail); }
//	puts(STR "TcpSocket::Peek: timeout = " << timeout_msec << ", avail = " << avail);
	return avail > 0;
}

bool TcpSocket::Wait(const Vector<TcpSocket *>& read, const Vector<TcpSocket *>& write, int timeout_msec)
{
	SLOG("TcpSocket::Wait(" << read.GetCount() << " TcpSockets for read, " << write.GetCount() << " TcpSockets for write, " << timeout_msec << " msecs)");
	Vector<SOCKET> readfds, writefds;
	int i;
	readfds.Reserve(read.GetCount());
	for(i = 0; i < read.GetCount(); i++)
		if(read[i]->IsOpen()) {
			if(read[i]->data->Peek(0, false))
				return true;
			readfds.Add(read[i]->GetTcpSocket());
		}
	writefds.Reserve(write.GetCount());
	for(i = 0; i < write.GetCount(); i++)
		if(write[i]->IsOpen()) {
			if(write[i]->data->Peek(0, true))
				return true;
			writefds.Add(write[i]->GetTcpSocket());
		}
	return Wait(readfds, writefds, timeout_msec);
}

bool TcpSocket::PeekAbort(int timeout_msec)
{
	if(!data)
		return true;
	String left = data->leftover;
	data->leftover = Null;
	char buffer;
	int count = -1;
	if(data->Peek(timeout_msec, false))
		count = data->Read(&buffer, 1);
	left.Cat(data->leftover);
	if(count > 0)
		left.Cat(buffer);
	data->leftover = left;
	return count == 0;
}

String TcpSocket::Read(int timeout, int maxlen)
{
	if(!data->leftover.IsEmpty()) {
		String out = data->leftover;
		data->leftover = Null;
		return out;
	}
	if(IsError() || IsEof())
		return String::GetVoid();
	int ticks = GetTickCount(), end_ticks = ticks + timeout;
	if(data->is_blocking && !IsNull(timeout)
	|| !data->is_blocking && timeout != 0)
		if(!Peek(IsNull(timeout) ? NB_TIMEOUT : end_ticks - ticks))
			return Null;
	char buffer[SOCKBUFSIZE];
	int count = data->Read(buffer, min<int>(maxlen, sizeof(buffer)));
	SLOG("recv(" << GetNumber() << ") -> " << count << ": " << String(buffer, max(count, 64)));
	if(data->IsError())
		return String::GetVoid();
	if(count <= 0) {
		if(!data->IsEof()) {
			SLOG("TcpSocket::Read(" << GetNumber() << ") -> ewouldblock");
		}
		return Null;
	}
	return String(buffer, count);
}

int TcpSocket::ReadCount(void *buffer, int count, int timeout_msec)
{
	SLOG("ReadCount(@" << GetNumber() << ": " << count << ", Tmax = " << timeout_msec << ")");
	int done = 0;
	if(!data->leftover.IsEmpty()) {
//		SLOG("-> leftover = <" << BinHexEncode(data->leftover) << ">");
		done = min(data->leftover.GetLength(), count);
		memcpy(buffer, data->leftover, done);
		if(done < data->leftover.GetLength()) {
			data->leftover.Remove(0, done);
			return done;
		}
		data->leftover.Clear();
	}
	int end_ticks = msecs() + timeout_msec;
	while(done < count && !IsError() && !IsEof()) {
//		SLOG("ReadCount(@" << GetNumber() << ": " << count << ", done " << done << ") -> peek");
		if(!IsNull(timeout_msec) && !Peek(max(end_ticks - msecs(), 0)))
			break;
//		SLOG("ReadCount(@" << GetNumber() << ": " << count << ", done " << done << ") -> read");
		int part = ReadRaw((char *)buffer + done, count - done);
		if(part > 0)
			done += part;
	}
	SLOG("//ReadCount -> " << done);
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
	SLOG("TcpSocket::RecvUntil(term = " << (int)term << ", maxlen = " << maxlen << ", timeout = " << timeout << ")");
	ASSERT(IsOpen() && maxlen != 0);
	int ticks = GetTickCount(), end_ticks = IsNull(timeout) ? int(Null) : ticks + timeout, seek = 0;
	String out = Read(timeout, maxlen);
	if(out.IsVoid())
		return out;

	for(;;) {
		int f = out.Find((byte)term, seek);
		if(f >= 0) {
			data->leftover = String(out.Begin() + f + 1, out.GetLength() - f - 1) + data->leftover;
			return out.Left(f);
		}
		seek = out.GetLength();
		ticks = GetTickCount();
		if(!IsNull(timeout)) timeout = end_ticks - ticks;
		if(!IsNull(timeout) && timeout <= 0 || out.GetLength() >= maxlen)
			return out;
		String part = Read(timeout, maxlen - out.GetLength());
		if(part.IsVoid()) {
			SLOG("term " << (int)term << " not found in: " << out);
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
	SLOG("TcpSocket::RecvUntil(term = " << (int)term << ", maxlen = " << maxlen << ", timeout = " << timeout << ")");
	ASSERT(IsOpen() && maxlen != 0);
	int ticks = GetTickCount(), end_ticks = IsNull(timeout) ? int(Null) : ticks + timeout, seek = 0;
	String out = Read(timeout, maxlen);
	if(out.IsVoid())
		return out;

	for(;;) {
		int f = Find(out, term, seek);
		if(f >= 0) {
			termchar = out[f];
			data->leftover = String(out.Begin() + f + 1, out.GetLength() - f - 1) + data->leftover;
			return out.Left(f);
		}
		seek = out.GetLength();
		ticks = GetTickCount();
		if(!IsNull(timeout)) timeout = end_ticks - ticks;
		if(!IsNull(timeout) && timeout <= 0 || out.GetLength() >= maxlen)
			return out;
		String part = Read(timeout, maxlen - out.GetLength());
		if(part.IsVoid()) {
			SLOG("term " << (int)term << " not found in: " << out);
			return out;
		}
		out.Cat(part);
	}
}

void TcpSocket::UnRead(const void *buffer, int len)
{
	ASSERT(len >= 0);
	ASSERT(!!data);
	if(len > 0)
		data->leftover.Insert(0, (const char *)buffer, len);
}

String TcpSocket::PeekCount(int count, int timeout_msec)
{
	String s = ReadCount(count, timeout_msec);
	data->leftover.Insert(0, s);
	return s;
}

String TcpSocket::PeekUntil(char term, int timeout_msec, int maxlen)
{
	String s = ReadUntil(term, timeout_msec, maxlen);
	data->leftover.Insert(0, s);
	return s;
}

static thread__ char s_errortext[201];
static thread__ int  s_errortextlen;

String TcpSocket::GetErrorText()
{
	return String(s_errortext, s_errortextlen);
}

void TcpSocket::SetErrorText(String text)
{
	SLOG("TcpSocket::SetLastErrorText = " << text);
	s_errortextlen = min(text.GetLength(), 200);
	memcpy(s_errortext, ~text, s_errortextlen + 1);
}

void TcpSocket::SetSockError(SOCKET socket, const char *context, int code, const char *errdesc)
{
	String err;
	errorcode = code;
	if(socket != INVALID_SOCKET)
		err << "socket(" << (int)socket << ") / ";
	err << context << ": " << errdesc;
	errordesc = err;
	is_error = true;
	SetErrorText(err);
}

void TcpSocket::SetSockError(SOCKET socket, const char *context)
{
	errorcode = GetErrorCode();
	SetSockError(socket, context, errorcode, TcpSocketErrorDesc(GetErrorCode()));
}

bool TcpSocket::OpenClient(const char *host, int port, bool nodelay, dword *my_addr, int timeout, bool is_blocking)
{
	TcpSocket::Data *data = new TcpSocket::Data;
	Attach(data);
	if(data->OpenClient(host, port, nodelay, my_addr, timeout, blocking))
		return true;
	socket.Clear();
	return false;
}

bool ServerTcpSocket(TcpSocket& socket, int port, bool nodelay, int listen_count, bool blocking, bool reuse)
{
	TcpSocket::Data *data = new TcpSocket::Data;
	socket.Attach(data);
	if(data->OpenServer(port, nodelay, listen_count, blocking, reuse))
		return true;
	socket.Clear();
	return false;
}

void AttachTcpSocket(TcpSocket& socket, SOCKET s, bool blocking)
{
	One<TcpSocket::Data> data = new TcpSocket::Data;
	data->AttachRaw(s, blocking);
	socket.Attach(data);
}

END_UPP_NAMESPACE
