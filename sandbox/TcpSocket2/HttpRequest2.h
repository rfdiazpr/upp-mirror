#ifndef _Web_httpcli_h_
#define _Web_httpcli_h_

class RequestHttp
{
public:
	RequestHttp();
	RequestHttp(const char *url);
	virtual ~RequestHttp();

	RequestHttp&  TimeoutMsecs(int t)              { timeout_msecs = t; return *this; }
	RequestHttp&  MaxHeaderSize(int m)             { max_header_size = m; return *this; }
	RequestHttp&  MaxContentSize(int m)            { max_content_size = m; return *this; }

	RequestHttp&  Host(String h)                   { host = h; return *this; }
	RequestHttp&  Port(int p)                      { port = p; return *this; }
	RequestHttp&  Path(String p)                   { path = p; return *this; }
	RequestHttp&  User(String u, String p)         { username = u; password = p; return *this; }
	RequestHttp&  Digest()                         { force_digest = true; return *this; }
	RequestHttp&  Digest(String d)                 { digest = d; return *this; }
	RequestHttp&  URL(const char *url);
	RequestHttp&  Url(const char *id, const String& data);
	RequestHttp&  KeepAlive(bool k)                { keepalive = k; return *this; }
	RequestHttp&  Proxy(String host, int port)     { proxy_host = host; proxy_port = port; return *this; }
	RequestHttp&  Proxy(const char *url);
	RequestHttp&  ProxyAuth(String usr, String pwd){  proxy_username = usr; proxy_password = pwd; return *this; }

	RequestHttp&  Headers(String h)                { client_headers = h; return *this; }
	RequestHttp&  ClearHeaders()                   { return Headers(Null); }
	RequestHttp&  AddHeaders(String h)             { client_headers.Cat(h); return *this; }
	RequestHttp&  Header(const char *id, const String& data);

	RequestHttp&  StdHeaders(bool sh)              { std_headers = sh; return *this; }
	RequestHttp&  NoStdHeaders()                   { return StdHeaders(false); }
	RequestHttp&  Accept(String a)                 { accept = a; return *this; }
	RequestHttp&  Agent(String a)                  { agent = a; return *this; }
	RequestHttp&  ContentType(String a)            { contenttype = a; return *this; }

	RequestHttp&  Method(int m)                    { method = m; return *this; }
	RequestHttp&  Get()                            { return Method(METHOD_GET); }
	RequestHttp&  Post()                           { return Method(METHOD_POST); }
	RequestHttp&  Head()                           { return Method(METHOD_HEAD); }
	RequestHttp&  Put()                            { return Method(METHOD_PUT); }

	RequestHttp&  PostData(String pd)              { postdata = pd; return *this; }
	RequestHttp&  PostUData(String pd)             { return PostData(UrlEncode(pd)); }
	RequestHttp&  Post(const String& data)         { Post(); return PostData(data); }
	RequestHttp&  Post(const char *id, const String& data);

	RequestHttp&  UrlVar(const char *id, const String& data);
	RequestHttp&  operator()(const char *id, const String& data) { return UrlVar(id, data); }

	String       Execute(Gate2<int, int> progress = false);
	String       ExecuteRedirect(int max_redirect = DEFAULT_MAX_REDIRECT,
		int retries = DEFAULT_RETRIES, Gate2<int, int> progress = false);

	bool         IsError() const                  { return !IsNull(error); }
	String       GetError() const                 { return error; }
	bool         IsAborted() const                { return aborted; }

	int          GetStatusCode() const            { return status_code; }
	String       GetStatusLine() const            { return status_line; }
	String       GetHeaders() const               { return server_headers; }
	String       GetBody() const                  { return body; }

	bool         IsRedirect() const               { return is_redirect; }
	String       GetRedirectURL() const           { return redirect_url; }

	String       CalculateDigest(String authenticate) const;

	void         Close()                          { socket.Close(); }

	static void  Trace(bool b = true);

	
	virtual bool CreateClientSocket();
	virtual bool IsSecure();

public:
	TcpSocket       socket;
	bool         keepalive;
	bool         aborted;
	bool         force_digest;
	String       error;
	String       body;

	int          timeout_msecs;
	int          max_header_size;
	int          max_content_size;

	String       host;
	int          port;
	int          method;
	String       proxy_host;
	int          proxy_port;
	String       proxy_username;
	String       proxy_password;
	String       path;
	String       username;
	String       password;
	String       digest;
	String       client_headers;
	String       accept;
	String       agent;
	String       contenttype;
	String       postdata;

	bool         is_post;
	bool         std_headers;
	bool         hasurlvar;
	bool         is_redirect;

	int          status_code;
	String       status_line;
	String       server_headers;

	String       redirect_url;
	String       authenticate;
	
	String       socket_host;
	int          socket_port;

	enum {
		DEFAULT_HTTP_PORT        = 80,
		DEFAULT_TIMEOUT_MSECS    = 120000,
		DEFAULT_MAX_HEADER_SIZE  = 1000000,
		DEFAULT_MAX_CONTENT_SIZE = 10000000,
		DEFAULT_MAX_REDIRECT     = 5,
		DEFAULT_RETRIES          = 3,
	};

	enum {
		METHOD_GET,
		METHOD_POST,
		METHOD_HEAD,
		METHOD_PUT,
	};

private:
	void         Init();

protected:
	bool         use_proxy;
	String       ReadUntilProgress(char until, int start_time, int end_time, Gate2<int, int> progress);
};

String RequestHttpGet(String url, String *server_headers = NULL, String *error = NULL,
	Gate2<int, int> progress = false, int timeout = RequestHttp::DEFAULT_TIMEOUT_MSECS,
	int max_redirect = RequestHttp::DEFAULT_MAX_REDIRECT, int retries = RequestHttp::DEFAULT_RETRIES);

String RequestHttpGet(String url, String username, String password,
	String *server_headers = NULL, String *error = NULL,
	Gate2<int, int> progress = false, int timeout = RequestHttp::DEFAULT_TIMEOUT_MSECS,
	int max_redirect = RequestHttp::DEFAULT_MAX_REDIRECT, int retries = RequestHttp::DEFAULT_RETRIES);

String RequestHttpGet(String url, String proxy,
	String *server_headers = NULL, String *error = NULL,
	Gate2<int, int> progress = false, int timeout = RequestHttp::DEFAULT_TIMEOUT_MSECS,
	int max_redirect = RequestHttp::DEFAULT_MAX_REDIRECT, int retries = RequestHttp::DEFAULT_RETRIES);

String RequestHttpGet(String url, String proxy, String username, String password,
	String *server_headers = NULL, String *error = NULL,
	Gate2<int, int> progress = false, int timeout = RequestHttp::DEFAULT_TIMEOUT_MSECS,
	int max_redirect = RequestHttp::DEFAULT_MAX_REDIRECT, int retries = RequestHttp::DEFAULT_RETRIES);

#endif
