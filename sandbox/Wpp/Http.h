void MakeLink(StringBuffer& out, const Vector<String>& part, const Vector<Value>& arg);

struct Http {
	String method;
	String uri;
	String version;
	String content;
	
	VectorMap<String, String> hdrfield;

	Vector<String>            arg;
	
	VectorMap<String, Value>  var;
	
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

	String operator[](const char *id) const           { return var.Get(id, Null); }
	String operator[](int i)                          { return i >= 0 && i < arg.GetCount() ? arg[i] : String(); }
	int    GetParamCount() const                      { return arg.GetCount(); }
	
	Http& ContentType(const char *s)                  { content_type = s; return *this; }
	Http& Content(const char *s, const String& data)  { content_type = s; response = data; return *this; }
	Http& operator<<(const String& s)                 { response << s; return *this; }
	
	Http& operator()(const char *id, const Value& v)  { var.Add(id, v); return *this; }
	Http& Render(const String& template_name);
	
	Http& SetRawCookie(const char *id, const String& value,
	                   Time expires = Null, const char *path = NULL,
	                   const char *domain = NULL, bool secure = false, bool httponly = false);
	Http& SetCookie(const char *id, const String& value,
	                Time expires = Null, const char *path = NULL,
	                const char *domain = NULL, bool secure = false, bool httponly = false);

	Http& Redirect(const char *url, int code_ = 302)      { code = code_; redirect = url; return *this; }
	Http& Redirect(void (*view)(Http&), const Vector<Value>& arg);
	Http& Redirect(void (*view)(Http&));
	Http& Redirect(void (*view)(Http&), const Value& v1);
	Http& Redirect(void (*view)(Http&), const Value& v1, const Value& v2);
	
	String GetResponse() const                        { return response; }
	
	Http() { code = 200; content_type = "text/html; charset=UTF-8"; }
};

String HttpResponse(int code, const char *phrase, const String& data, const char *content_type = NULL);

void RegisterView(void (*view)(Http&), const char *id, const char *path);

#define URL_VIEW(name, path) void name(Http& http); INITBLOCK { RegisterView(name, #name, path); } void name(Http& http)

Vector<String> *GetUrlViewLinkParts(const String& id);

String MakeLink(void (*view)(Http&), const Vector<Value>& arg);
