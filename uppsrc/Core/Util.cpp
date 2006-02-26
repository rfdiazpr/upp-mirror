#include "Core.h"

void    Panic(const char *msg)
{
	LOG(msg);
	BugLog() << "PANIC: " << msg << "\n";
	UsrLogT("===== PANIC ================================================");
	UsrLogT(msg);
#ifdef PLATFORM_WIN32
	MessageBox(::GetActiveWindow(), msg, "Panic", MB_ICONSTOP | MB_OK | MB_APPLMODAL);
#ifdef _DEBUG
#ifdef COMPILER_MSC
	_asm int 3
#endif
#ifdef COMPILER_GCC
	asm("int $3");
#endif
#endif
#else
	write(2, msg, strlen(msg));
	write(2, "\n", 1);
#endif
	abort();
}

static void (*s_assert_hook)(const char *);

void    SetAssertFailedHook(void (*h)(const char *))
{
	s_assert_hook = h;
}

void    AssertFailed(const char *file, int line, const char *cond)
{
	char s[2048];
	sprintf(s, "Assertion failed in %s, line %d\n%s\n", file, line, cond);
	LOG(s);
	if(s_assert_hook)
		(*s_assert_hook)(s);
	BugLog() << "ASSERT FAILED: " << s << "\n";
	UsrLogT("===== ASSERT FAILED ================================================");
	UsrLogT(s);
#ifdef PLATFORM_WIN32
	MessageBox(::GetActiveWindow(), s, "Assertion failed", MB_ICONSTOP | MB_OK | MB_APPLMODAL);
#ifdef COMPILER_MSC
	_asm int 3
#endif
#ifdef COMPILER_GCC
	asm("int $3");
#endif
#else
	write(2, s, strlen(s));
#endif
	abort();
}

#ifdef PLATFORM_POSIX
int GetTickCount() {
	struct timeval tv[1];
	struct timezone tz[1];
	memset(tz, 0, sizeof(tz));
	gettimeofday(tv, tz);
	return tv->tv_sec % 1000000 * 1000 + tv->tv_usec / 1000;
}
#endif

TimeStop::TimeStop()
{
	starttime = GetTickCount();
}

String TimeStop::ToString() const
{
	dword time = Elapsed();
	return Format("%d.%03d", int(time / 1000), int(time % 1000));
}

int RegisterTypeNo__(const char *type)
{
	INTERLOCKED {
		static Index<String> types;
		return types.FindAdd(type);
	}
	return -1;
}

char *PermanentCopy(const char *s)
{
	char *t = (char *)MemoryAllocPermanent(strlen(s) + 1);
	strcpy(t, s);
	return t;
}

#ifndef CPU_X86
int PeekIW(const void *ptr) {
	const byte *p = (const byte *)ptr;
	return (p[0] << 8 * 0)
		|  (p[1] << 8 * 1);
}
#endif

#ifndef CPU_X86
int PeekIL(const void *ptr) {
	const byte *p = (const byte *)ptr;
	return (p[0] << 8 * 0)
		|  (p[1] << 8 * 1)
		|  (p[2] << 8 * 2)
		|  (p[3] << 8 * 3);
}
#endif

#ifndef CPU_X86
int PeekI64(const void *ptr) {
	const byte *p = (const byte *)ptr;
	dword a = p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
	dword b = p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24);
	return (int64)a | ((int64)b << 32);
}
#endif

int PeekMW(const void *ptr) {
	const byte *p = (const byte *)ptr;
	return (p[1] << 8 * 0)
		|  (p[0] << 8 * 1);
}

int PeekML(const void *ptr) {
	const byte *p = (const byte *)ptr;
	return (p[3] << 8 * 0)
		|  (p[2] << 8 * 1)
		|  (p[1] << 8 * 2)
		|  (p[0] << 8 * 3);
}

#ifndef CPU_X86
void PokeIW(void *ptr, int value) {
	byte *p = (byte *)ptr;
	p[0] = (byte)(value >> 0);
	p[1] = (byte)(value >> 8);
}
#endif

#ifndef CPU_X86
void PokeIL(void *ptr, int value) {
	byte *p = (byte *)ptr;
	p[0] = (byte)(value >> 0);
	p[1] = (byte)(value >> 8);
	p[2] = (byte)(value >> 16);
	p[3] = (byte)(value >> 24);
}
#endif

#ifndef CPU_X86
void PokeI64(void *ptr, int64 value) {
	byte *p = (byte *)ptr;
	p[0] = (byte)(value >> 8 * 0);
	p[1] = (byte)(value >> 8 * 1);
	p[2] = (byte)(value >> 8 * 2);
	p[3] = (byte)(value >> 8 * 3);
	p[4] = (byte)(value >> 8 * 4);
	p[5] = (byte)(value >> 8 * 5);
	p[6] = (byte)(value >> 8 * 6);
	p[7] = (byte)(value >> 8 * 7);
}
#endif

#ifndef CPU_X86
void PokeIF4(void *ptr, float value)
{
	ASSERT(sizeof(float) == 4);
	union {
		float f;
		byte b[4];
	};
	f = value;
#ifdef CPU_LE
	memcpy(ptr, b, 4);
#else
	((byte *)ptr)[0] = b[3];
	((byte *)ptr)[1] = b[2];
	((byte *)ptr)[2] = b[1];
	((byte *)ptr)[3] = b[0];
#endif
}
#endif

#ifndef CPU_X86
float PeekIF4(const void *ptr)
{
	ASSERT(sizeof(float) == 4);
	union {
		float f;
		byte b[4];
	};
#ifdef CPU_LE
	memcpy(b, ptr, 4);
#else
	b[3] = ((byte *)ptr)[0];
	b[2] = ((byte *)ptr)[1];
	b[1] = ((byte *)ptr)[2];
	b[0] = ((byte *)ptr)[3];
#endif
	return f;
}
#endif

#ifndef CPU_X86
void PokeID8(void *ptr, double value)
{
	ASSERT(sizeof(double) == 8);
	union {
		double d;
		byte b[8];
	};
	d = value;
#ifdef CPU_LE
	memcpy(ptr, b, 8);
#else
	((byte *)ptr)[0] = b[7];
	((byte *)ptr)[1] = b[6];
	((byte *)ptr)[2] = b[5];
	((byte *)ptr)[3] = b[4];
	((byte *)ptr)[0] = b[3];
	((byte *)ptr)[1] = b[2];
	((byte *)ptr)[2] = b[1];
	((byte *)ptr)[3] = b[0];
#endif
}
#endif

#ifndef CPU_X86
double PeekID8(const void *ptr)
{
	ASSERT(sizeof(double) == 8);
	union {
		double d;
		byte b[8];
	};
#ifdef CPU_LE
	memcpy(b, ptr, 8);
#else
	b[7] = ((byte *)ptr)[0];
	b[6] = ((byte *)ptr)[1];
	b[5] = ((byte *)ptr)[2];
	b[4] = ((byte *)ptr)[3];
	b[3] = ((byte *)ptr)[4];
	b[2] = ((byte *)ptr)[5];
	b[1] = ((byte *)ptr)[6];
	b[0] = ((byte *)ptr)[7];
#endif
	return d;
}
#endif

void PokeMW(void *ptr, int value) {
	byte *p = (byte *)ptr;
	p[1] = (byte)(value >> 0);
	p[0] = (byte)(value >> 8);
}

void PokeML(void *ptr, int value) {
	byte *p = (byte *)ptr;
	p[3] = (byte)(value >> 0);
	p[2] = (byte)(value >> 8);
	p[1] = (byte)(value >> 16);
	p[0] = (byte)(value >> 24);
}

#ifndef PLATFORM_WIN32
void Sleep(int msec)
{
	timespec tval;
	tval.tv_sec = msec / 1000;
	tval.tv_nsec = (msec % 1000) * 1000000;
	nanosleep(&tval, NULL);
}
#endif

int MemICmp(const void *dest, const void *src, int count)
{

	const byte *a = (const byte *)dest;
	const byte *b = (const byte *)src;
	const byte *l = a + count;
	while(a < l) {
		if(ToUpper(*a) != ToUpper(*b))
			return ToUpper(*a) - ToUpper(*b);
		a++;
		b++;
	}
	return 0;
}

Stream& Pack16(Stream& s, Point& p) {
	return Pack16(s, p.x, p.y);
}

Stream& Pack16(Stream& s, Size& sz) {
	return Pack16(s, sz.cx, sz.cy);
}

Stream& Pack16(Stream& s, Rect& r) {
	return Pack16(s, r.left, r.top, r.right, r.bottom);
}

void StringC::Free() {
	if(IsString()) delete (String *) bap.GetPtr();
}

StringC::~StringC() {
	Free();
}

StringC::operator const char *() const {
	if(IsEmpty()) return NULL;
	if(IsString()) return *(String *) bap.GetPtr();
	return (const char *)bap.GetPtr();
}

StringC::operator String() const {
	if(IsEmpty()) return (const char *)NULL;
	if(IsString()) return *(String *) bap.GetPtr();
	return (const char *)bap.GetPtr();
}

bool StringC::IsEmpty() const {
	if(IsString()) return (*(String *) bap.GetPtr()).IsEmpty();
	if(!bap.GetPtr()) return true;
	return !*(const char *)bap.GetPtr();
}

void StringC::SetString(const String& s) {
	Free();
	String *ptr = new String;
	*ptr = s;
	bap.Set1(ptr);
}

void StringC::SetCharPtr(const char *s) {
	Free();
	bap.Set0((void *)s);
}

CharFilterTextTest::CharFilterTextTest(int (*filter)(int)) : filter(filter) {}
CharFilterTextTest::~CharFilterTextTest() {}

const char *CharFilterTextTest::Accept(const char *s) const {
	if(!(*filter)((byte)*s++)) return NULL;
	while((*filter)((byte)*s)) s++;
	return s;
}

Vector<String> Split(const char *s, const TextTest& delim, bool ignoreempty) {
	Vector<String> r;
	const char *t = s;
	while(*t) {
		const char *q = delim.Accept(t);
		if(q) {
			if(!ignoreempty || t > s)
				r.Add(String(s, t));
			t = s = q;
		}
		else
			t++;
	}
	if(!ignoreempty || t > s)
		r.Add(String(s, t));
	return r;
}

Vector<String> Split(const char *s, int (*filter)(int), bool ignoreempty) {
	return Split(s, CharFilterTextTest(filter), ignoreempty);
}

struct chrTextTest : public TextTest {
	int chr;
	virtual const char *Accept(const char *s) const { return chr == *s ? s + 1 : NULL; }
};

Vector<String> Split(const char *s, int chr, bool ignoreempty) {
	chrTextTest ct;
	ct.chr = chr;
	return Split(s, ct, ignoreempty);
}

String Join(const Vector<String>& im, const String& delim) {
	String r;
	for(int i = 0; i < im.GetCount(); i++) {
		if(i) r.Cat(delim);
		r.Cat(im[i]);
	}
	return r;
}

// ---------------------------

VectorMap<String, String> LoadIniFile(const char *filename) {
	VectorMap<String, String> key;
	FileIn in(filename);
	if(!in) return key;
	int c;
	if((c = in.Get()) < 0) return key;
	for(;;) {
		String k, v;
		for(;;) {
			if(IsAlNum(c) || c == '_')
				k.Cat(c);
			else
				break;
			if((c = in.Get()) < 0) return key;
		}
		for(;;) {
			if(c != '=' && c != ' ') break;
			if((c = in.Get()) < 0) return key;
		}
		for(;;) {
			if(c < ' ') break;
			v.Cat(c);
			if((c = in.Get()) < 0) break;
		}
		if(!k.IsEmpty())
			key.Add(k, v);
		if(k == "LINK") {
			in.Close();
			if(!in.Open(v) || (c = in.Get()) < 0) return key;
		}
		else
			for(;;) {
				if(IsAlNum(c)) break;
				if((c = in.Get()) < 0) return key;
			}
	}
}

const char *sIniFile;

void SetIniFile(const char *name) {
	sIniFile = name;
}

#ifdef PLATFORM_WIN32

String GetIniKey(const char *name) {
	static bool loaded;
	static VectorMap<String, String> key;
	if(!loaded) {
		loaded = true;
		key = LoadIniFile(sIniFile ? sIniFile : ~GetExeDirFile("Q.INI"));
		if(key.GetCount() == 0)
			key = LoadIniFile("C:\\Q.INI");
	}
	int i = key.Find(name);
	if(i < 0) return String();
	return key[i];
}

#endif

#ifdef PLATFORM_POSIX

String GetIniKey(const char *name) {
	static bool loaded;
	static VectorMap<String, String> key;
	if(!loaded) {
		loaded = true;
		key = LoadIniFile(GetHomeDirFile("Q.INI"));
	}
	int i = key.Find(name);
	if(i < 0) return String();
	return key[i];
}

#endif

void TextSettings::Load(const char *filename)
{
	FileIn in(filename);
	int themei = 0;
	settings.Add("");
	while(!in.IsEof()) {
		String ln = in.GetLine();
		const char *s = ln;
		if(*s == '[') {
			s++;
			String theme;
			while(*s && *s != ']')
				theme.Cat(*s++);
			themei = settings.FindAdd(theme);
		}
		else {
			if(themei >= 0) {
				String key;
				while(*s && *s != '=') {
					key.Cat(*s++);
				}
				if(*s == '=') s++;
				String value;
				while(*s) {
					value.Cat(*s++);
				}
				if(!IsEmpty(key))
					settings[themei].GetAdd(key) = value;
			}
		}
	}
}

String TextSettings::Get(const char *group, const char *key) const
{
	int itemi = settings.Find(group);
	return itemi < 0 ? Null : settings.Get(group).Get(key, Null);

}

// --------------------------------------------------------------

String timeFormat(double s) {
	if(s < 0.001) return Sprintf("%5.1f us", s * 1.0e6);
	if(s < 1) return Sprintf("%5.1f ms", s * 1.0e3);
	return Sprintf("%5.1f s ", s);
}

String Garble(const char *s, const char *e)
{
	int c = 0xAA;
	String result;
	if(!e)
		e = s + strlen(s);
	while(s != e)
	{
		result.Cat(*s++ ^ (char)c);
		if((c <<= 1) & 0x100)
			c ^= 0x137;
	}
	return result;
}

String Garble(const String& s)
{
	return Garble(~s, ~s + s.GetLength());
}

String Encode64(const String& s)
{
	String enc;
	int l = s.GetLength();
	enc << l << ':';
	for(int i = 0; i < l;)
	{
		char a = 0, b = 0, c = 0;
		if(i < l) a = s[i++];
		if(i < l) b = s[i++];
		if(i < l) c = s[i++];
		enc.Cat(' ' + 1 + ((a >> 2) & 0x3F));
		enc.Cat(' ' + 1 + ((a << 4) & 0x30) + ((b >> 4) & 0x0F));
		enc.Cat(' ' + 1 + ((b << 2) & 0x3C) + ((c >> 6) & 0x03));
		enc.Cat(' ' + 1 + (c & 0x3F));
	}
	return enc;
}

String Decode64(const String& s)
{
	if(!IsDigit(*s))
		return s;
	const char *p = s;
	char *h;
	int len = strtol(p, &h, 10);
	p = h;
	if(*p++ != ':' || len < 0 || (len + 2) / 3 * 4 > (s.End() - p))
		return s; // invalid encoding
	if(len == 0)
		return Null;
	String dec;
	for(;;)
	{
		byte ea = *p++ - ' ' - 1, eb = *p++ - ' ' - 1, ec = *p++ - ' ' - 1, ed = *p++ - ' ' - 1;
		byte out[3] = { (ea << 2) | (eb >> 4), (eb << 4) | (ec >> 2), (ec << 6) | (ed >> 0) };
		switch(len)
		{
		case 1:  dec.Cat(out[0]); return dec;
		case 2:  dec.Cat(out, 2); return dec;
		case 3:  dec.Cat(out, 3); return dec;
		default: dec.Cat(out, 3); len -= 3; break;
		}
	}
}

String NormalizeSpaces(const char *s)
{
	StringBuffer r;
	while(*s && (byte)*s <= ' ')
		s++;
	while(*s) {
		if((byte)*s <= ' ') {
			while(*s && (byte)*s <= ' ')
				s++;
			if(*s)
				r.Cat(' ');
		}
		else
			r.Cat(*s++);
	}
	return r;
}

String NormalizeSpaces(const char *s, const char *end)
{
	StringBuffer r;
	while(*s && (byte)*s <= ' ')
		s++;
	while(s < end) {
		if((byte)*s <= ' ') {
			while(s < end && (byte)*s <= ' ')
				s++;
			if(*s)
				r.Cat(' ');
		}
		else
			r.Cat(*s++);
	}
	return r;
}

int ChNoInvalid(int c)
{
	return c == DEFAULTCHAR ? '_' : c;
}

String ToSystemCharset(const String& src)
{
	return IsMainRunning() ? Filter(ToCharset(GetLNGCharset(GetSystemLNG()), src), ChNoInvalid)
	                       : src;
}

String FromSystemCharset(const String& src)
{
	return IsMainRunning() ? Filter(ToCharset(CHARSET_DEFAULT, src, GetLNGCharset(GetSystemLNG())), ChNoInvalid) : src;
}


static VectorMap<String, String>& sGCfg()
{
	static VectorMap<String, String> m;
	return m;
}

static StaticCriticalSection sGCfgLock;

static Vector<Callback>& sGFlush()
{
	static Vector<Callback> m;
	return m;
}

static StaticCriticalSection sGFlushLock;

void    RegisterGlobalConfig(const char *name)
{
	INTERLOCKED_(sGCfgLock) {
		ASSERT(sGCfg().Find(name) < 0);
		sGCfg().Add(name);
	}
}

void    RegisterGlobalConfig(const char *name, Callback WhenFlush)
{
	RegisterGlobalConfig(name);
	INTERLOCKED_(sGFlushLock) {
		sGFlush().Add(WhenFlush);
	}
}

String GetGlobalConfigData(const char *name)
{
	INTERLOCKED_(sGCfgLock) {
		return sGCfg().GetAdd(name);
	}
	return String();
}

void SetGlobalConfigData(const char *name, const String& data)
{
	INTERLOCKED_(sGCfgLock) {
		sGCfg().GetAdd(name) = data;
	}
}

void  SerializeGlobalConfigs(Stream& s)
{
	INTERLOCKED_(sGFlushLock) {
		for(int i = 0; i < sGFlush().GetCount(); i++)
			sGFlush()[i]();
	}
	INTERLOCKED_(sGCfgLock) {
		VectorMap<String, String>& cfg = sGCfg();
		int version = 0;
		s / version;
		int count = cfg.GetCount();
		s / count;
		for(int i = 0; i < count; i++) {
			String name;
			if(s.IsStoring())
				name = cfg.GetKey(i);
			s % name;
			int q = cfg.Find(name);
			if(q >= 0)
				s % cfg[q];
			else
			{
				String dummy;
				s % dummy;
			}
		}
		s.Magic();
	}
}

Exc::Exc() : String(GetLastErrorMessage()) {}
/*
#ifdef PLATFORM_WIN32
void Exc::Show() const {
	if(IsEmpty()) return;
	MessageBox(GetActiveWindow(), *this, "Chyba", MB_OK);
}
#endif

#ifdef PLATFORM_POSIX
void Exc::Show() const {
	printf(*this);
}
#endif
*/
AbortExc::AbortExc() :
	Exc(t_("Aborted by user.")) {}

#ifdef PLATFORM_WIN32

String GetErrorMessage(DWORD dwError) {
	char h[2048];
	sprintf(h, "%08x", dwError);
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dwError, 0, h, 2048, NULL);
	String result = h;
	String modf;
	const char* s = result;
	BYTE c;
	while((c = *s++) != 0)
		if(c <= ' ') {
			if(!modf.IsEmpty() && modf[modf.GetLength() - 1] != ' ')
				modf += ' ';
		}
		else if(c == '%' && *s >= '0' && *s <= '9') {
			s++;
			modf += "<###>";
		}
		else
			modf += (char)c;
	const char* p = modf;
	for(s = p + modf.GetLength(); s > p && s[-1] == ' '; s--);
	return FromSystemCharset(modf.Left(s - p));
}

String GetLastErrorMessage() {
	return GetErrorMessage(GetLastError());
}

#endif

#ifdef PLATFORM_POSIX

String GetErrorMessage(int errorno) {
	enum { MAX_ERROR_LEN = 1000000 };
	int len = 1024;
	Buffer<char> buffer(len);
	while(strerror_r(errorno, buffer, len))
		if(errno == ERANGE && len < MAX_ERROR_LEN)
			buffer.Alloc(len <<= 1);
		else
			return NFormat("strerror_r(%d, %d) -> errno = %d", errorno, len, errno);
	return FromSystemCharset(~buffer);
/*
	struct {
		int code;
		const char *text;
	}
	em[] = {
		{ 1, "Operation not permitted" },
		{ 2, "No such file or directory" },
		{ 3, "No such process" },
		{ 4, "Interrupted system call" },
		{ 5, "I/O error" },
		{ 6, "No such device or address" },
		{ 7, "Arg list too long" },
		{ 8, "Exec format error" },
		{ 9, "Bad file number" },
		{ 10, "No child processes" },
		{ 11, "Try again" },
		{ 12, "Out of memory" },
		{ 13, "Permission denied" },
		{ 14, "Bad address" },
		{ 15, "Block device required" },
		{ 16, "Device or resource busy" },
		{ 17, "File exists" },
		{ 18, "Cross-device link" },
		{ 19, "No such device" },
		{ 20, "Not a directory" },
		{ 21, "Is a directory" },
		{ 22, "Invalid argument" },
		{ 23, "File table overflow" },
		{ 24, "Too many open files" },
		{ 25, "Not a typewriter" },
		{ 26, "Text file busy" },
		{ 27, "File too large" },
		{ 28, "No space left on device" },
		{ 29, "Illegal seek" },
		{ 30, "Read-only file system" },
		{ 31, "Too many links" },
		{ 32, "Broken pipe" },
		{ 33, "Math argument out of domain of func" },
		{ 34, "Math result not representable" },
		{ 35, "Resource deadlock would occur" },
		{ 36, "File name too long" },
		{ 37, "No record locks available" },
		{ 38, "Function not implemented" },
		{ 39, "Directory not empty" },
		{ 40, "Too many symbolic links encountered" },
		{ 41, "Operation would block" },
		{ 42, "No message of desired type" },
		{ 43, "Identifier removed" },
		{ 44, "Channel number out of range" },
		{ 45, "Level { 2 not synchronized" },
		{ 46, "Level { 3 halted" },
		{ 47, "Level { 3 reset" },
		{ 48, "Link number out of range" },
		{ 49, "Protocol driver not attached" },
		{ 50, "No CSI structure available" },
		{ 51, "Level { 2 halted" },
		{ 52, "Invalid exchange" },
		{ 53, "Invalid request descriptor" },
		{ 54, "Exchange full" },
		{ 55, "No anode" },
		{ 56, "Invalid request code" },
		{ 57, "Invalid slot" },
		{ 59, "Bad font file format" },
		{ 60, "Device not a stream" },
		{ 61, "No data available" },
		{ 62, "Timer expired" },
		{ 63, "Out of streams resources" },
		{ 64, "Machine is not on the network" },
		{ 65, "Package not installed" },
		{ 66, "Object is remote" },
		{ 67, "Link has been severed" },
		{ 68, "Advertise error" },
		{ 69, "Srmount error" },
		{ 70, "Communication error on send" },
		{ 71, "Protocol error" },
		{ 72, "Multihop attempted" },
		{ 73, "RFS specific error" },
		{ 74, "Not a data message" },
		{ 75, "Value too large for defined data type" },
		{ 76, "Name not unique on network" },
		{ 77, "File descriptor in bad state" },
		{ 78, "Remote address changed" },
		{ 79, "Can not access a needed shared library" },
		{ 80, "Accessing a corrupted shared library" },
		{ 81, ".lib section in a.out corrupted" },
		{ 82, "Attempting to link in too many shared libraries" },
		{ 83, "Cannot exec a shared library directly" },
		{ 84, "Illegal byte sequence" },
		{ 85, "Interrupted system call should be restarted" },
		{ 86, "Streams pipe error" },
		{ 87, "Too many users" },
		{ 88, "Socket operation on non-socket" },
		{ 89, "Destination address required" },
		{ 90, "Message too long" },
		{ 91, "Protocol wrong type for socket" },
		{ 92, "Protocol not available" },
		{ 93, "Protocol not supported" },
		{ 94, "Socket type not supported" },
		{ 95, "Operation not supported on transport endpoint" },
		{ 96, "Protocol family not supported" },
		{ 97, "Address family not supported by protocol" },
		{ 98, "Address already in use" },
		{ 99, "Cannot assign requested address" },
		{ 100, "Network is down" },
		{ 101, "Network is unreachable" },
		{ 102, "Network dropped connection because of reset" },
		{ 103, "Software caused connection abort" },
		{ 104, "Connection reset by peer" },
		{ 105, "No buffer space available" },
		{ 106, "Transport endpoint is already connected" },
		{ 107, "Transport endpoint is not connected" },
		{ 108, "Cannot send after transport endpoint shutdown" },
		{ 109, "Too many references: cannot splice" },
		{ 110, "Connection timed out" },
		{ 111, "Connection refused" },
		{ 112, "Host is down" },
		{ 113, "No route to host" },
		{ 114, "Operation already in progress" },
		{ 115, "Operation now in progress" },
		{ 116, "Stale NFS file handle" },
		{ 117, "Structure needs cleaning" },
		{ 118, "Not a XENIX named type file" },
		{ 119, "No XENIX semaphores available" },
		{ 120, "Is a named type file" },
		{ 121, "Remote I/O error" },
		{ 122, "Quota exceeded" },
		{ 123, "No medium found" },
		{ 124, "Wrong medium type" },
	};
	for(int i = 0; i < __countof(em); i++)
		if(em[i].code == errorno)
			return em[i].text;
	return "Unknown error";
*/
}

String GetLastErrorMessage() {
	return GetErrorMessage(errno);
}

#endif

void BeepInformation()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONINFORMATION);
#else
	write(1, "\a", 1); //??
#endif
}

void BeepExclamation()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONEXCLAMATION);
#else
	write(1, "\a", 1); //??
#endif
}

void BeepQuestion()
{
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONQUESTION);
#else
	write(1, "\a", 1); //??
#endif
}

#if defined(COMPILER_MSC) && (_MSC_VER < 1300)
//hack for linking libraries built using VC7 with VC6 standard lib's
extern "C" long _ftol( double );
extern "C" long _ftol2( double dblSource ) { return _ftol( dblSource ); }
#endif
