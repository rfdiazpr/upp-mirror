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

enum { WAIT_READ = 1, WAIT_WRITE = 2, WAIT_EXCEPTION = 3 };

class TcpSocket {
	enum { BUFFERSIZE = 512 };
	SOCKET                  socket;
	char                    buffer[BUFFERSIZE];
	char                   *ptr;
	char                   *end;
	bool                    is_eof;
	bool                    is_error;
	bool                    is_timeout;
	bool                    is_abort;
	bool                    ipv6;

	bool                    global;
	int                     timeout;
	int                     starttime;
	int                     waitstep;
	int                     done;

	int                     errorcode;
	String                  errordesc;


	bool                    CloseRaw();
	SOCKET                  AcceptRaw(dword *ipaddr, int timeout_msec);
	bool                    Open(int family, int type, int protocol);
	int                     Recv(void *buffer, int maxlen);
	int                     Send(const void *buffer, int maxlen);

	void                    ReadBuffer();
	int                     Get_();
	int                     Peek_();

	void                    Reset();

	void SetSockError(const char *context, const char *errdesc);
	void SetSockError(const char *context);

	static int              GetErrorCode();
	static bool             WouldBlock();

public:
	Callback        WhenWait;
	
	TcpSocket&      Timeout(int ms)                          { timeout = ms; global = false; return *this; }
	TcpSocket&      GlobalTimeout(int ms);
	TcpSocket&      Blocking()                               { return Timeout(Null); }
	
	int             GetDone() const                          { return done; }

	TcpSocket();
	~TcpSocket()                                             { Close(); }

	static void     Init();

	bool            IsOpen() const                           { return socket != INVALID_SOCKET; }
	bool            IsEof() const                            { return is_eof && ptr == end; }

	bool            IsError() const                          { return is_error; }
	void            ClearError()                             { is_error = false; errorcode = 0; errordesc.Clear(); }
	int             GetError() const                         { return errorcode; }
	String          GetErrorDesc() const                     { return errordesc; }

	bool            IsTimeout() const                        { return is_timeout; }
	void            ClearTimeout()                           { is_timeout = false; }
	
	void            Abort()                                  { is_abort = true; }
	bool            IsAbort() const                          { return IsAbort(); }
	void            ClearAbort()                             { is_abort = false; }
	
	SOCKET          GetSOCKET() const                        { return socket; }
	String          GetPeerAddr() const;

	void            Attach(SOCKET socket);
	bool            Connect(const char *host, int port);
	bool            Listen(int port, int listen_count, bool ipv6 = false, bool reuse = true);
	bool            Accept(TcpSocket& listen_socket);
	bool            Close();
	void            Shutdown();

	void            NoDelay();
	void            Linger(int msecs);
	void            NoLinger()                               { Linger(Null); }
	void            Reuse(bool reuse = true);
	
	bool            Wait(dword flags);
	bool            WaitRead()                               { return Wait(WAIT_READ); }
	bool            WaitWrite()                              { return Wait(WAIT_WRITE); }

	int             Peek()                                   { return ptr < end ? *ptr : Peek_(); }
	int             Term()                                   { return Peek(); }
	int             Get()                                    { return ptr < end ? *ptr++ : Get_(); }
	int             Get(void *buffer, int len);
	String          Get(int len);
	int             GetAll(void *buffer, int len)            { return Get(buffer, len) == len; }
	String          GetAll(int len)                          { String s = Get(len); return s.GetCount() == len ? s : String::GetVoid(); }
	String          GetLine(int maxlen = 2000000);

	int             Put(const char *s, int len);
	int             Put(const String& s)                     { return Put(s.Begin(), s.GetLength()); }
	bool            PutAll(const char *s, int len)           { return Put(s, len) == len; }
	bool            PutAll(const String& s)                  { return Put(s) == s.GetCount(); }

	static String   GetHostName();
};

#include "HttpRequest.h"
#include "HttpRequest2.h"

END_UPP_NAMESPACE