void MakeLink(StringBuffer& out, const Vector<String>& part, const Vector<Value>& arg);

struct Http;

struct Renderer {
	VectorMap<String, Value>  var;

	Renderer& Link(const char *id, void (*view)(Http&), const Vector<Value>& arg);
	const One<Exe>& GetTemplate(const String& template_name);

public:	
	Renderer& operator()(const char *id, const Value& v)  { var.Add(id, v); return *this; }
	Renderer& operator()(const ValueMap& map);
	Renderer& operator()(const char *id, void (*view)(Http&));
	Renderer& operator()(const char *id, void (*view)(Http&), const Value& arg1);
	Renderer& operator()(const char *id, void (*view)(Http&), const Value& arg1, const Value& arg2);

	Renderer& operator()(const Sql& sql);
	Renderer& operator()(Fields rec);
	SqlUpdate Update(SqlId table);
	SqlInsert Insert(SqlId table);
	
	Value     operator[](const char *id) const           { return var.Get(id, Null); }

	String    RenderString(const String& template_name);
	Value     Render(const String& template_name)        { return Raw(RenderString(template_name)); }
	
	Renderer& Render(const char *id, const String& template_name);
};

struct Http : Renderer {
	String method;
	String uri;
	String version;
	String content;
	
	VectorMap<String, String> hdrfield;
	Vector<String>            arg;
	String                    session_id;
	VectorMap<String, Value>  session_var;
	
	String redirect;
	int    code;
	String code_text;
	String response;
	String content_type;
	String request_content_type;
	
	String cookies;
	
	int    benchmark;
	
	bool   Read(Socket& http);
	
	void   ParseRequest(const char *s);
	void   ReadMultiPart(const String& content);
	
	void   LoadSession();
	void   SaveSession();

public:
	Http&  operator()(const char *id, const Value& v)  { var.Add(id, v); return *this; }
	Http&  operator()(const ValueMap& map)             { Renderer::operator()(map); return *this; }
	Http&  operator()(const char *id, void (*view)(Http&)) { Renderer::operator()(id, view); return *this; }
	Http&  operator()(const char *id, void (*view)(Http&), const Value& arg1) { Renderer::operator()(id, view, arg1); return *this; }
	Http&  operator()(const char *id, void (*view)(Http&), const Value& arg1, const Value& arg2) { Renderer::operator()(id, view, arg1, arg2); return *this; }

	Http&  operator()(const Sql& sql)                  { Renderer::operator()(sql); return *this; }
	Http&  operator()(Fields rec)                      { Renderer::operator()(rec); return *this; }
	Http&  Render(const char *id, const String& template_name) { Renderer::Render(id, template_name); return *this; }
	Value  Render(const String& template_name)         { return Renderer::Render(template_name); }

	void   Dispatch(Socket& socket);

	String GetHeader(const char *s) const              { return hdrfield.Get(s, Null); }
	int    GetLength() const                           { return atoi(GetHeader("content-length")); }

	Value  operator[](const char *id) const            { return Renderer::operator[](id); }
	String operator[](int i)                           { return i >= 0 && i < arg.GetCount() ? arg[i] : String(); }
	int    GetParamCount() const                       { return arg.GetCount(); }
	
	Http&  ContentType(const char *s)                  { content_type = s; return *this; }
	Http&  Content(const char *s, const String& data)  { content_type = s; response = data; return *this; }
	Http&  operator<<(const String& s)                 { response << s; return *this; }

	Http&  SetRawCookie(const char *id, const String& value,
	                    Time expires = Null, const char *path = NULL,
	                    const char *domain = NULL, bool secure = false, bool httponly = false);
	Http&  SetCookie(const char *id, const String& value,
	                 Time expires = Null, const char *path = NULL,
	                 const char *domain = NULL, bool secure = false, bool httponly = false);

	Http&  ClearSession();
	Http&  SetSession(const char *id, const Value& value);

	Http&  RenderResult(const String& template_name);
	Http&  Redirect(const char *url, int code_ = 302) { code = code_; redirect = url; return *this; }
	Http&  Redirect(void (*view)(Http&), const Vector<Value>& arg);
	Http&  Redirect(void (*view)(Http&));
	Http&  Redirect(void (*view)(Http&), const Value& v1);
	Http&  Redirect(void (*view)(Http&), const Value& v1, const Value& v2);
	
	String GetResponse() const                        { return response; }
	
	void   Benchmark(int n = 10)                      { benchmark = n; }

	Http() { code = 200; content_type = "text/html; charset=UTF-8"; benchmark = 1; }
};

String HttpResponse(int code, const char *phrase, const String& data, const char *content_type = NULL);

void RegisterView(void (*view)(Http&), const char *id, const char *path);

#define URL_VIEW(name, path) void name(Http& http); INITBLOCK { RegisterView(name, #name, path); } void name(Http& http)

Vector<String> *GetUrlViewLinkParts(const String& id);

String MakeLink(void (*view)(Http&), const Vector<Value>& arg);

void FinalizeViews(); // Remove later

void SetViewRoot(const char *root);
void SetViewVar(const char *id, const char *value);

