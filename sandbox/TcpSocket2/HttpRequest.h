#ifndef _TcpSocket2_HttpRequest_h_
#define _TcpSocket2_HttpRequest_h_

class HttpRequest : public TcpSocket {
	bool         Problem();
	void         HttpError(const char *s);
	String       Execute0();
	
public:
	bool         keepalive;
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

public:
	enum Phase {
		NONE,
		REQUEST, HEADER, BODY, CHUNK_HEADER, CHUNK_BODY,
	};
	
	HttpRequest&  TimeoutMsecs(int t)              { timeout_msecs = t; return *this; }
	HttpRequest&  MaxHeaderSize(int m)             { max_header_size = m; return *this; }
	HttpRequest&  MaxContentSize(int m)            { max_content_size = m; return *this; }

	HttpRequest&  Host(String h)                   { host = h; return *this; }
	HttpRequest&  Port(int p)                      { port = p; return *this; }
	HttpRequest&  Path(String p)                   { path = p; return *this; }
	HttpRequest&  User(String u, String p)         { username = u; password = p; return *this; }
	HttpRequest&  Digest()                         { force_digest = true; return *this; }
	HttpRequest&  Digest(String d)                 { digest = d; return *this; }
	HttpRequest&  URL(const char *url);
	HttpRequest&  Url(const char *id, const String& data);
	HttpRequest&  KeepAlive(bool k)                { keepalive = k; return *this; }
	HttpRequest&  Proxy(String host, int port)     { proxy_host = host; proxy_port = port; return *this; }
	HttpRequest&  Proxy(const char *url);
	HttpRequest&  ProxyAuth(String usr, String pwd){  proxy_username = usr; proxy_password = pwd; return *this; }

	HttpRequest&  Headers(String h)                { client_headers = h; return *this; }
	HttpRequest&  ClearHeaders()                   { return Headers(Null); }
	HttpRequest&  AddHeaders(String h)             { client_headers.Cat(h); return *this; }
	HttpRequest&  Header(const char *id, const String& data);

	HttpRequest&  StdHeaders(bool sh)              { std_headers = sh; return *this; }
	HttpRequest&  NoStdHeaders()                   { return StdHeaders(false); }
	HttpRequest&  Accept(String a)                 { accept = a; return *this; }
	HttpRequest&  Agent(String a)                  { agent = a; return *this; }
	HttpRequest&  ContentType(String a)            { contenttype = a; return *this; }

	HttpRequest&  Method(int m)                    { method = m; return *this; }
	HttpRequest&  Get()                            { return Method(METHOD_GET); }
	HttpRequest&  Post()                           { return Method(METHOD_POST); }
	HttpRequest&  Head()                           { return Method(METHOD_HEAD); }
	HttpRequest&  Put()                            { return Method(METHOD_PUT); }

	HttpRequest&  PostData(String pd)              { postdata = pd; return *this; }
	HttpRequest&  PostUData(String pd)             { return PostData(UrlEncode(pd)); }
	HttpRequest&  Post(const String& data)         { Post(); return PostData(data); }
	HttpRequest&  Post(const char *id, const String& data);

	HttpRequest&  UrlVar(const char *id, const String& data);
	HttpRequest&  operator()(const char *id, const String& data) { return UrlVar(id, data); }

	bool         IsSocketError() const            { return TcpSocket::IsError(); }
	bool         IsHttpError() const              { return !IsNull(error) ; }
	bool         IsError() const                  { return IsSocketError() || IsHttpError(); }
	String       GetErrorDesc() const             { return IsSocketError() ? TcpSocket::GetErrorDesc() : error; }

	int          GetStatusCode() const            { return status_code; }
	String       GetStatusLine() const            { return status_line; }
	String       GetHeaders() const               { return server_headers; }
	String       GetBody() const                  { return body; }

	bool         IsRedirect() const               { return is_redirect; }
	String       GetRedirectURL() const           { return redirect_url; }

	String       CalculateDigest(String authenticate) const;
	
	static void  Trace(bool b = true);

	String       Execute(int max_redirect = DEFAULT_MAX_REDIRECT, int retries = DEFAULT_RETRIES);

	HttpRequest();
	HttpRequest(const char *url);
};

#endif
