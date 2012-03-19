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

class TcpSocket {
	SOCKET                  socket;
	String                  leftover;
	bool                    is_eof;
	bool                    is_error;
	bool                    ipv6;
	int                     errorcode;
	String                  errordesc;

	bool                    CloseRaw();
	SOCKET                  AcceptRaw(dword *ipaddr, int timeout_msec);
	bool                    Open(int family, int type, int protocol);
	static int              GetErrorCode();
	static bool             WouldBlock();

	void     Reset();

	struct Chr {
		int chr;
		
		bool Check(int c) const { return c == chr; }
	};

	struct Filter {
		int (*filter)(int c);
		
		bool Check(int c) const { return (*filter)(c); }
	};

public:
	TcpSocket()                                              { ClearError(); Reset(); }
	~TcpSocket()                                             { Close(); }

	static void     Init();

	void            Clear()                                  { Close(); }

	bool            IsOpen() const                           { return socket != INVALID_SOCKET; }
	bool            IsEof() const                            { return is_eof && leftover.IsEmpty(); }

	bool            IsError() const                          { return is_error; }
	void            ClearError()                             { is_error = false; errorcode = 0; errordesc.Clear(); }
	int             GetError() const                         { return errorcode; }
	String          GetErrorDesc() const                     { return errordesc; }

	void            SetSockError(const char *context); // Remove later when new Until is available
	void            SetSockError(const char *context, const char *errordesc);
	

	SOCKET          GetSOCKET() const                        { return socket; }
	String          GetPeerAddr() const;

	void            Attach(SOCKET socket);
	bool            Connect(const char *host, int port);
	bool            Listen(int port, int listen_count, bool ipv6 = false, bool reuse = true);
	bool            Accept(TcpSocket& listen_socket, int timeout = Null);
	bool            Close(int timeout = Null);

	void            NoDelay();
	void            Linger(int msecs);
	void            NoLinger()                               { Linger(Null); }
	void            Reuse(bool reuse = true);

	bool            Peek(int timeout = 0, bool write = false);
	bool            PeekWrite(int timeout = 0)               { return Peek(timeout, true); }

	int             Recv(void *buffer, int maxlen);
	String          Read(int timeout = Null, int maxlen = 4096);
	int             ReadCount(void *buffer, int count, int timeout = Null);
	String          ReadCount(int count, int timeout = Null);
	String          ReadUntil(int term, int timeout = Null, int maxlen = 2000000, Gate step = Gate(), int steptime = 20);
	String          ReadLine(int timeout = Null, int maxlen = 2000000, Gate abort = Gate(), int steptime = 20);
	void            UnRead(const void *buffer, int len);
	void            UnRead(const String& data)               { UnRead(data.Begin(), data.GetLength()); }

	int             Send(const void *buffer, int maxlen);
	int             WriteWait(const char *s, int length, int timeout_msec = 0);
	void            Write(const char *s, int length)         { WriteWait(s, length, Null); }
	void            Write(String s)                          { Write(s.Begin(), s.GetLength()); }

	void            StopWrite();

	static String   GetHostName();
};
#include "HttpRequest.h"

END_UPP_NAMESPACE