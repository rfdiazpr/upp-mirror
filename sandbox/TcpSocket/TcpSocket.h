//#define NOFAKEERROR

#if defined(PLATFORM_WIN32)

#define W_P(w, p) w
#if !defined(PLATFORM_CYGWIN)
#include <winsock2.h>
#endif
typedef int socklen_t;

#elif defined(PLATFORM_POSIX)

#define W_P(w, p) p
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
//#include <libiberty.h>
enum
{
	INVALID_SOCKET = -1,
	TCP_NODELAY    = 1,
	SD_RECEIVE     = 0,
	SD_SEND        = 1,
	SD_BOTH        = 2,
};
typedef int SOCKET;

#else
	#error Unsupported platform
#endif//PLATFORM switch

NAMESPACE_UPP

String        FormatIP(dword _ip);
String        UrlEncode(const String& s);
String        UrlEncode(const String& s, const char *specials);
String        UrlDecode(const char *b, const char *e);
inline String UrlDecode(const String& s)          { return UrlDecode(s.Begin(), s.End() ); }
String        Base64Encode(const char *b, const char *e);
inline String Base64Encode(const String& data)    { return Base64Encode(data.Begin(), data.End()); }
String        Base64Decode(const char *b, const char *e);
inline String Base64Decode(const String& data)    { return Base64Decode(data.Begin(), data.End()); }


static const int DEFAULT_CONNECT_TIMEOUT = 5000;

static const int SOCKKIND_STD = 1; // GetKind() for ordinary socket

class TcpSocket : Moveable<TcpSocket>
{
public:
#if 0
	class Data
	{
	public:
		Data();
		virtual ~Data() { CloseRaw(0); }

		bool                    Open(bool is_blocking);
		bool                    OpenServer(int port, bool nodelay, int listen_count, bool is_blocking, bool reuse = true);
		bool                    OpenClient(const char *host, int port, bool nodelay, dword *my_addr, int timeout, bool is_blocking);

//		void                    WriteTimeout(int msecs);
//		void                    ReadTimeout(int msecs);

		virtual int             GetKind() const        { return SOCKKIND_STD; }

		virtual int             Write(const void *buf, int amount);
		virtual bool            Accept(TcpSocket& socket, dword *ipaddr, bool nodelay, int timeout_msec);

	public:
	protected:
		void                    SetSockResError(const String& context);
		void                    SetSockError(const String& context);
		void                    SetSockError(const String& context, int code, const String& errortext);

		friend void AttachTcpSocket(TcpSocket& socket, SOCKET hsocket, bool blocking);
	};
#endif
	SOCKET                  socket;
	String                  leftover;
	bool                    is_blocking;
	bool                    is_eof;
#ifndef NOFAKEERROR
	int                     fake_error;
#endif
	bool            is_error;
	int             errorcode;
	String          errordesc;

	SOCKET                  AcceptRaw(dword *ipaddr, int timeout_msec);
	void                    Attach(SOCKET socket, bool nodelay, bool is_blocking);
	void                    AttachRaw(SOCKET s, bool blocking);

protected:
	void     SetSockError(const char *context)        { SetSockError(INVALID_SOCKET, context); }
	void     SetSockError(SOCKET socket, const char *context);
	void     SetSockError(SOCKET socket, const char *context, int code, const char *errordesc);
	
	void     Reset();

private:
	
	friend bool SSLSecureTcpSocket(TcpSocket& socket);

public:
	TcpSocket()                                              { ClearError(); socket = INVALID_SOCKET; }
	~TcpSocket()                                             { Close(); }

	static void     Init();

	void            Clear()                                  { Close(); }

	bool            IsOpen() const                           { return socket != INVALID_SOCKET; }

	bool            IsEof() const                            { return is_eof && leftover.IsEmpty();; }
	bool            IsError() const                          { return is_error; }
	void            ClearError()                             { is_error = false; errorcode = 0; errordesc.Clear(); }
	int             GetError() const                         { return errorcode; }
	String          GetErrorDesc() const                     { return errordesc; }

	SOCKET          GetSocket() const                        { return socket; }
	int             GetNumber() const                        { return (int)GetSocket(); }

	String          GetPeerAddr() const;

	bool            Accept(TcpSocket& socket, dword *ipaddr = 0, bool nodelay = true, int timeout_msec = DEFAULT_CONNECT_TIMEOUT);
	bool            Close(int msecs_timeout = 0);

	void            NoDelay();
	void            Linger(int msecs);
	void            NoLinger()                               { Linger(Null); }
	void            Block(bool b = true);
	void            NoBlock()                                { Block(false); }

	static bool     Wait(const Vector<SOCKET>& read, const Vector<SOCKET>& write, int timeout_msec);
	static bool     Wait(const Vector<TcpSocket *>& read, const Vector<TcpSocket *>& write, int timeout_msec);

	bool            Peek(int timeout_msec = 0, bool write = false);
	bool            PeekWrite(int timeout_msec = 0)          { return Peek(timeout_msec, true); }
	bool            PeekAbort(int timeout_msec = 0);
	String          PeekCount(int count, int timeout_msec = Null);
	String          PeekUntil(char term, int timeout_msec = Null, int maxlen = 1000000);

	int             ReadRaw(void *buffer, int maxlen);
	String          Read(int timeout_msec = Null, int maxlen = 1000000);
	int             ReadCount(void *buffer, int count, int timeout_msec = Null);
	String          ReadCount(int count, int timeout_msec = Null);
	String          ReadUntil(char term, int timeout_msec = Null, int maxlen = 1000000);
	String          ReadUntil(Gate1<int> term, int& termchar, int timeout = Null, int maxlen = 1000000);
	void            UnRead(const void *buffer, int len);
	void            UnRead(String data)                      { UnRead(data.Begin(), data.GetLength()); }

	int             WriteRaw(const void *buffer, int maxlen);
	int             WriteWait(const char *s, int length, int timeout_msec);
	void            Write(const char *s, int length)         { WriteWait(s, length, Null); }
	void            Write(String s)                          { Write(s.Begin(), s.GetLength()); }

	void            StopWrite()                              { StopWrite(); }

	static String   GetHostName();

	static String   GetErrorText();
	static void     SetErrorText(String text);
	static void     ClearErrorText()                         { SetErrorText(Null); }

#if defined(PLATFORM_WIN32)
	static int      GetErrorCode()                           { return WSAGetLastError(); }
#define SOCKERR(x) WSA##x
#elif defined(PLATFORM_POSIX)
	static int      GetErrorCode()                           { return errno; }
#define SOCKERR(x) x
#else
#error Unsupported platform
#endif

public:
	bool            OpenClient(const char *host, int port, bool nodelay = true, dword *my_addr = NULL, int timeout = DEFAULT_CONNECT_TIMEOUT, bool is_blocking = true);
};

bool ServerTcpSocket(TcpSocket& socket, int port, bool nodelay = true, int listen_count = 5, bool is_blocking = true, bool reuse = true);
bool ClientTcpSocket(TcpSocket& socket, const char *host, int port, bool nodelay = true, dword *my_addr = NULL, int timeout = DEFAULT_CONNECT_TIMEOUT, bool is_blocking = true);
void AttachTcpSocket(TcpSocket& socket, SOCKET s, bool blocking);

#include "HttpRequest.h"

END_UPP_NAMESPACE