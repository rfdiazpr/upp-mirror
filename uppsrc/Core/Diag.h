class LogStream : public Stream {
#ifdef PLATFORM_WIN32
	HANDLE hfile;
#endif
#ifdef PLATFORM_POSIX
	enum { INVALID_HANDLE_VALUE = -1 };
	int   hfile;
#endif

	char  filename[512];
	char  backup[512];
	byte  buffer[512];
	int   filesize;
	byte *p;
	
	int   sizelimit;
	int   part;
	
	void  Flush();

protected:
	virtual void    _Put(int w);
	virtual void    _Put(const void *data, dword size);
	virtual int64   GetSize() const { return filesize; }

public:
	virtual   bool  IsOpen() const;
	void            Create(const char *path, bool append);
	void            SetLimit(int maxsize)                    { sizelimit = maxsize; }

	bool            Delete();
	void            Close();

	LogStream();
	~LogStream();
};

void     ActivateUsrLog();
void     ActivatePersistentUsrLog();
bool     IsUsrLog();
Stream&  UsrLog();
Stream&  UsrLog(const char *line);
Stream&  UsrLogT(const char *line);
Stream&  UsrLogT(int indent, const char *line);
void     DeleteUsrLog();

Stream&  BugLog();

Stream&  StdLog();

Stream&  VppLog();
void     SetVppLog(Stream& log);
void     SetVppLogName(const String& file);
void     SetVppLogSizeLimit(int filesize);
void     SetVppLogNoDeleteOnStartup();

void     HexDump(Stream& s, const void *ptr, int size, int maxsize = INT_MAX);

int      msecs(int from = 0);

String        GetTypeName(const char *type_name);
inline String GetTypeName(const std::type_info& tinfo)   { return GetTypeName(tinfo.name()); }

void __LOGF__(const char *format, ...);

#ifdef _DEBUG

#define DEBUGCODE(x)     x

#define LOG(a)           VppLog() << a << '\n'
#define LOGF             __LOGF__
#define LOGBEGIN()       VppLog().Begin()
#define LOGEND()         VppLog().End()
#define LOGBLOCK(n)      RLOGBLOCK(n)
#define LOGHEXDUMP(s, a) HexDump(VppLog(), s, a)
#define QUOTE(a)         { LOG(#a); a; }
#define LOGSRCPOS()      VppLog() << __FILE__ << '#' << __LINE__ << '\n'
#define DUMP(a)          VppLog() << #a << " = " << (a) << '\n'
#define DUMPC(c)         DumpContainer(VppLog() << #c << ":\n", (c))
#define DUMPCC(c)        DumpContainer2(VppLog() << #c << ":\n", (c))
#define DUMPCCC(c)       DumpContainer3(VppLog() << #c << ":\n", (c))
#define XASSERT(c, d)    if(!bool(c)) { LOG("XASSERT failed"); LOGSRCPOS(); LOG(d); ASSERT(0); } else
#define NEVER()          ASSERT(0)
#define XNEVER(d)        if(1) { LOG("NEVER failed"); LOGSRCPOS(); LOG(d); ASSERT(0); } else
#define CHECK(c)         if(!bool(c)) { ASSERT(0); } else
#define XCHECK(c, d)     if(!bool(c)) { LOG("XCHECK failed"); LOGSRCPOS(); LOG(d); ASSERT(0); } else

#define TIMING(x)        RTIMING(x)
#define HITCOUNT(x)      RHITCOUNT(x)
#define ACTIVATE_TIMING()    TimingInspector::Activate(true);
#define DEACTIVATE_TIMING()  TimingInspector::Activate(false);

#else

#define ASSERT(x)

#define DEBUGCODE(x)

inline void LOGF(const char *format, ...) {}

#define LOG(a)
#define LOGBEGIN()
#define LOGEND()
#define LOGBLOCK(n)
#define LOGHEXDUMP(s, a)
#define QUOTE(a)         a
#define LOGSRCPOS()
#define DUMP(a)
#define DUMPC(a)
#define DUMPCC(a)
#define DUMPCCC(a)
#define XASSERT(c, d)
#define NEVER()
#define XNEVER(d)
#define CHECK(c)         (c)
#define XCHECK(c, d)     (c)

#define TIMING(x)
#define HITCOUNT(x)
#define ACTIVATE_TIMING()
#define DEACTIVATE_TIMING()

#endif

struct DebugLogBlock
{
	DebugLogBlock(const char *name) : name(name) { VppLog() << name << "\n" << BeginIndent; }
	~DebugLogBlock()                             { VppLog() << EndIndent << "//" << name << "\n"; }
	const char *name;
};

#define RLOG(a)           VppLog() << a << '\n'
#define RLOGBEGIN()       VppLog().Begin()
#define RLOGEND()         VppLog().End()
#define RLOGBLOCK(n)      DebugLogBlock MK__s(n)
#define RLOGHEXDUMP(s, a) HexDump(VppLog(), s, a)
#define RQUOTE(a)         { LOG(#a); a; }
#define RLOGSRCPOS()      VppLog() << __FILE__ << '#' << __LINE__ << '\n'
#define RDUMP(a)          VppLog() << #a << " = " << (a) << '\n'
#define RDUMPC(c)         DumpContainer(VppLog() << #c << ":\n", (c))

// Crash support

void InstallCrashDump(const char *app_info = NULL);

// Dumping templates

template <class T>
void DumpContainer(Stream& s, T ptr, T end) {
	int i = 0;
	s.Begin();
	while(ptr != end)
		s << '[' << i++ << "] = " << *ptr++ << '\n';
	s.End();
}

template <class C>
void DumpContainer(Stream& s, const C& c) {
	DumpContainer(s, c.Begin(), c.End());
}

template <class T>
void DumpContainer2(Stream& s, T ptr, T end) {
	int i = 0;
	s.Begin();
	while(ptr != end) {
		s << '[' << i++ << "] =\n";
		DumpContainer(s, (*ptr).Begin(), (*ptr).End());
		ptr++;
	}
	s.End();
}

template <class C>
void DumpContainer2(Stream& s, const C& c) {
	DumpContainer2(s, c.Begin(), c.End());
}

template <class T>
void DumpContainer3(Stream& s, T ptr, T end) {
	int i = 0;
	s.Begin();
	while(ptr != end) {
		s << '[' << i++ << "] =\n";
		DumpContainer2(s, (*ptr).Begin(), (*ptr).End());
		ptr++;
	}
	s.End();
}

template <class C>
void DumpContainer3(Stream& s, const C& c) {
	DumpContainer3(s, c.Begin(), c.End());
}
