#ifndef _TcpSocket2_HttpRequest_h_
#define _TcpSocket2_HttpRequest_h_

struct HttpHeader {
	String                    first_line;
	VectorMap<String, String> fields;
	
	String operator[](const char *id) { return fields.Get(id, Null); }

	bool   Response(String& protocol, int& code, String& reason);
	bool   Request(String& method, String& uri, String& version);

	void Clear();	
	bool Parse(const String& hdrs);
};

class HttpRequest : public TcpSocket {
	int          phase;
	String       data;
	int          count;

	HttpHeader   header;

	String       error;
	String       body;

	enum {
		DEFAULT_HTTP_PORT        = 80,
		DEFAULT_MAX_HEADER_SIZE  = 1000000,
		DEFAULT_MAX_CONTENT_SIZE = 10000000,
		DEFAULT_MAX_REDIRECTS    = 5,
		DEFAULT_MAX_RETRIES      = 3,
	};

	enum {
		METHOD_GET,
		METHOD_POST,
		METHOD_HEAD,
		METHOD_PUT,
	};

	int          max_header_size;
	int          max_content_size;
	int          max_redirects;
	int          max_retries;

	String       host;
	int          port;
	String       proxy_host;
	int          proxy_port;
	String       proxy_username;
	String       proxy_password;
	String       path;

	int          method;
	String       accept;
	String       agent;
	bool         force_digest;
	bool         is_post;
	bool         std_headers;
	bool         hasurlvar;
	String       contenttype;
	String       username;
	String       password;
	String       digest;
	String       request_headers;
	String       postdata;

	String       protocol;
	int          status_code;
	String       response_phrase;
	
	int          retry_count;
	int          redirect_count;

	void         Init();

	void         StartPhase(int s);
	void         StartBody();
	bool         SendingData();
	bool         ReadingHeader();
	bool         ReadingBody();
	void         StartRequest();
	void         ReadingChunkHeader();
	void         Finish();

	void         HttpError(const char *s);
	String       Execute0();

	String       CalculateDigest(const String& authenticate) const;

public:
	HttpRequest&  MaxHeaderSize(int m)                   { max_header_size = m; return *this; }
	HttpRequest&  MaxContentSize(int m)                  { max_content_size = m; return *this; }
	HttpRequest&  MaxRedirect(int n)                     { max_redirects = n; return *this; }
	HttpRequest&  MaxRetries(int n)                      { max_retries = n; return *this; }

	HttpRequest&  Method(int m)                          { method = m; return *this; }
	HttpRequest&  GET()                                  { return Method(METHOD_GET); }
	HttpRequest&  POST()                                 { return Method(METHOD_POST); }
	HttpRequest&  HEAD()                                 { return Method(METHOD_HEAD); }
	HttpRequest&  PUT()                                  { return Method(METHOD_PUT); }

	HttpRequest&  Host(const String& h)                  { host = h; return *this; }
	HttpRequest&  Port(int p)                            { port = p; return *this; }
	HttpRequest&  Path(const String& p)                  { path = p; return *this; }
	HttpRequest&  User(const String& u, const String& p) { username = u; password = p; return *this; }
	HttpRequest&  Digest()                               { force_digest = true; return *this; }
	HttpRequest&  Digest(const String& d)                { digest = d; return *this; }
	HttpRequest&  URL(const char *url);
	HttpRequest&  Url(const char *id, const String& data);
	HttpRequest&  UrlVar(const char *id, const String& data);
	HttpRequest&  operator()(const char *id, const String& data) { return UrlVar(id, data); }
	HttpRequest&  PostData(const String& pd)              { postdata = pd; return *this; }
	HttpRequest&  PostUData(const String& pd)             { return PostData(UrlEncode(pd)); }
	HttpRequest&  Post(const String& data)                { POST(); return PostData(data); }
	HttpRequest&  Post(const char *id, const String& data);

	HttpRequest&  Headers(const String& h)                { request_headers = h; return *this; }
	HttpRequest&  ClearHeaders()                          { return Headers(Null); }
	HttpRequest&  AddHeaders(const String& h)             { request_headers.Cat(h); return *this; }
	HttpRequest&  Header(const char *id, const String& data);

	HttpRequest&  StdHeaders(bool sh)                     { std_headers = sh; return *this; }
	HttpRequest&  NoStdHeaders()                          { return StdHeaders(false); }
	HttpRequest&  Accept(const String& a)                 { accept = a; return *this; }
	HttpRequest&  Agent(const String& a)                  { agent = a; return *this; }
	HttpRequest&  ContentType(const String& a)            { contenttype = a; return *this; }

	HttpRequest&  Proxy(const String& host, int port)     { proxy_host = host; proxy_port = port; return *this; }
	HttpRequest&  Proxy(const char *url);
	HttpRequest&  ProxyAuth(const String& u, const String& p) {  proxy_username = u; proxy_password = p; return *this; }

	bool         IsSocketError() const            { return TcpSocket::IsError(); }
	bool         IsHttpError() const              { return !IsNull(error) ; }
	bool         IsError() const                  { return IsSocketError() || IsHttpError(); }
	String       GetErrorDesc() const             { return IsSocketError() ? TcpSocket::GetErrorDesc() : error; }
	void         ClearError()                     { TcpSocket::ClearError(); error.Clear(); }

	String       GetHeader(const char *s)         { return header[s]; }
	String       operator[](const char *s)        { return GetHeader(s); }
	String       GetRedirectUrl();
	int          GetContentLength();
	int          GetStatusCode() const            { return status_code; }
	String       GetResponsePhrase() const        { return response_phrase; }
	String       GetContent() const               { return body; }

	enum Phase {
		START, REQUEST, HEADER, BODY, CHUNK_HEADER, CHUNK_BODY, TRAILER, FINISHED, FAILED
	};

	bool    Do();
	int     GetPhase() const                      { return phase; }
	bool    InProgress() const                    { return phase != FAILED && phase != FINISHED; }
	bool    IsFailure() const                     { return phase == FAILED; }
	bool    IsSuccess() const                     { return phase == FINISHED && status_code >= 200 && status_code < 300; }

	String  Execute();
	
	HttpRequest();
	HttpRequest(const char *url);

	
	static void  Trace(bool b = true);
};

#endif
