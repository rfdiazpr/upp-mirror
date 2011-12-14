struct Http {
	String method;
	String uri;
	String version;
	String content;
	
	VectorMap<String, String> hdrfield;

	VectorMap<String, String> request; 
	Vector<String>            arg;
	
	VectorMap<String, Value>  var;
	String                    signature;
	
	String redirect;
	int    code;
	String code_text;
	String response;
	String content_type;
	String request_content_type;
	
	String cookies;
	
	
	bool Read(Socket& http);
	
	void  ParseRequest(const char *s);
	const One<Exe>& GetTemplate(const String& template_name);
	void  ReadMultiPart(const String& content);

public:
	void Dispatch(Socket& socket);

	String GetHeader(const char *s) const  { return hdrfield.Get(s, Null); }
	int    GetLength() const               { return atoi(GetHeader("content-length")); }

	String operator[](const char *id) const           { return request.Get(id, String()); }
	String operator[](int i)                          { return i >= 0 && i < arg.GetCount() ? arg[i] : String(); }
	
	Http& ContentType(const char *s)                  { content_type = s; return *this; }
	Http& Content(const char *s, const String& data)  { content_type = s; response = data; return *this; }
	Http& operator<<(const String& s)                 { response << s; return *this; }
	
	Http& operator()(const char *id, const Value& v)  { signature << id << ';'; var.Add(id, v); return *this; }
	Http& Render(const String& template_name);
	
	Http& SetRawCookie(const char *id, const String& value,
	                   Time expires = Null, const char *path = NULL,
	                   const char *domain = NULL, bool secure = false, bool httponly = false);
	Http& SetCookie(const char *id, const String& value,
	                Time expires = Null, const char *path = NULL,
	                const char *domain = NULL, bool secure = false, bool httponly = false);

	Http& Redirect(const char *url, int code_ = 302)  { code = code_; redirect = url; return *this; }
	
	Http() { code = 200; content_type = "text/html; charset=UTF-8"; }
};

String HttpResponse(int code, const char *phrase, const String& data, const char *content_type = NULL);

void RegisterView(const char *path, Callback1<Http&> view);
void RegisterView(const char *path, void (*view)(Http&));

void Dispatch(Socket& http);
