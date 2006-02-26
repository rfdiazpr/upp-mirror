#ifdef _DEBUG
//#define STRING_STATS
#endif

#ifdef STRING_STATS
extern int StringClone[64];
extern int StringPtr[64];
extern int StringIndex[64];
extern int StringLength[64];
extern int StringRetain[64];
extern int StringRetainR[64];
extern int StringRelease[64];
extern int StringReleaseR[64];
extern int StringAssign[64];
extern int StringCopy[64];
extern int StringDestruct[64];
extern int StringCompare[64];
extern int StringAlloc[64];
extern int StringCat1[64];
extern int StringCatN[64];
extern int StringNull;
#define STRING_STAT_CODE(x) { x[min(GetData(ptr)->length, 63)]++; }
#define STRING_STAT_CODER(x) { if(GetData(ptr)->length) x[min((int)GetData(ptr)->refcount, 63)]++; }
#else
#define STRING_STAT_CODE(x)
#define STRING_STAT_CODER(x)
#endif

class Nuller;

int wstrlen(const wchar *s);

#ifdef PLATFORM_POSIX
inline int stricmp(const char *a, const char *b)         { return strcasecmp(a, b); }
inline int strnicmp(const char *a, const char *b, int n) { return strncasecmp(a, b, n); }
#endif

template <class T, class St>
class AStringBuffer;

inline int strlen__(const char *s) { return strlen(s); }
inline int strlen__(const wchar *s) { return wstrlen(s); }

template <class T, class S>
class AString : public Moveable<S> {
protected:
	struct Data {
		Atomic   refcount;
		int      length;
		int      alloc;

		T    *GetPtr() const  { return (T*)(this + 1); }
		void  Release()       { ASSERT(refcount > 0); if(AtomicDec(refcount) == 0) delete[] (byte *)this; }
		void  Retain()        { ASSERT(refcount > 0); AtomicInc(refcount); }
	};

	T *ptr;

	static Data *GetData(const T *ptr)    { return (Data *)ptr - 1; }

	static Data *Alloc(int len);
	static Data *Clone(const Data *data, int newalloc);

	static T *CreateEmpty(Data *kind);
	static T *Create(int len);
	static T *Create(const T *data, int len);

	void  Retain() const               { STRING_STAT_CODER(StringRetainR); STRING_STAT_CODE(StringRetain); GetData(ptr)->Retain(); }
	void  Release()                    { STRING_STAT_CODER(StringReleaseR); STRING_STAT_CODE(StringRelease); GetData(ptr)->Release(); }

	void  Chk()	const                  { ASSERT(ptr && AtomicRead(GetData(ptr)->refcount) > 0); }

	void Cat(int c);
	void Cat(int c, int count);
	void Cat(const T *s, int len);

	void Insert(int at, int c);
	void Insert(int at, const T *s, int len);

	void Assign(const AString& s);
	void Assign(const T *s, int len);
	void DeepAssign(const AString& s);

	T   *Mid(int pos, int length) const;
	T   *Mid(int pos) const;
	T   *Right(int count) const;
	T   *Left(int count) const;

	bool   IsEqual(const T *ptr, int len) const;

	AString(int, T *p)                 { ptr = p; }

public:
	void   Remove(int at, int count = 1);

	void   Set(int at, int chr);

	void   Trim(int at);

	int    GetLength() const                   { STRING_STAT_CODE(StringLength); return GetData(ptr)->length; }
	int    GetCount() const                    { STRING_STAT_CODE(StringLength); return GetData(ptr)->length; }
	bool   IsEmpty() const                     { STRING_STAT_CODE(StringLength); return GetCount() == 0; }

	int    Find(int chr, int from = 0) const;
	int    ReverseFind(int chr, int from) const;
	int    ReverseFind(int chr) const          { return ReverseFind(chr, GetCount()); }

//	int    Find(const T *s, int len);

	void   Clear();

	void   Shrink();
	void   Reserve(int len);
	int    GetAlloc() const                    { return GetData(ptr)->alloc; }


	T     *GetBuffer(int minlen); // deprecated !!!
	void   ReleaseBuffer(int newlen = -1); // deprecated !!!

	AString()                                  { ptr = S::CreateNull(); }
	AString(const AString& a)                  { ptr = a.ptr; Retain(); }
	AString(const T *s, int len)               { ptr = Create(s, len); }
	AString(const T *s)                        { ptr = s ? Create(s, strlen__(s)) : S::CreateNull(); }
	AString(int chr, int count);

	~AString()                                 { Chk(); Release(); }
	typedef T        ValueType;
	typedef const T* ConstIterator;
	const T *Begin() const                     { return ptr; }
	const T *End() const                       { return ptr + GetCount(); }
	const T *Last() const                      { return End() - 1; }
	const T *GetIter(int i) const              { ASSERT(i >= 0 && i <= GetCount()); return ptr + i; }

	friend class AStringBuffer<T, S>;

	friend bool operator<(const S& a, const S& b)   { return a.Compare(b) < 0; }
	friend bool operator<(const S& a, const T *b)   { return a.Compare(b) < 0; }
	friend bool operator<(const T *a, const S& b)   { return b.Compare(a) > 0; }

	friend bool operator<=(const S& a, const S& b)  { return a.Compare(b) <= 0; }
	friend bool operator<=(const S& a, const T *b)  { return a.Compare(b) <= 0; }
	friend bool operator<=(const T *a, const S& b)  { return b.Compare(a) >= 0; }

	friend bool operator>(const S& a, const S& b)   { return a.Compare(b) > 0; }
	friend bool operator>(const S& a, const T *b)   { return a.Compare(b) > 0; }
	friend bool operator>(const T *a, const S& b)   { return b.Compare(a) < 0; }

	friend bool operator>=(const S& a, const S& b)  { return a.Compare(b) >= 0; }
	friend bool operator>=(const S& a, const T *b)  { return a.Compare(b) >= 0; }
	friend bool operator>=(const T *a, const S& b)  { return b.Compare(a) <= 0; }

	friend bool operator==(const S& a, const S& b)  { return a.Compare(b) == 0; }
	friend bool operator==(const S& a, const T *b)  { return a.Compare(b) == 0; }
	friend bool operator==(const T *a, const S& b)  { return b.Compare(a) == 0; }

	friend bool operator!=(const S& a, const S& b)  { return a.Compare(b) != 0; }
	friend bool operator!=(const S& a, const T *b)  { return a.Compare(b) != 0; }
	friend bool operator!=(const T *a, const S& b)  { return b.Compare(a) != 0; }

	friend S    operator+(const S& a, const S& b)   { S c(a); c += b; return c; }
	friend S    operator+(const S& a, const T *b)   { S c(a); c += b; return c; }
	friend S    operator+(const T *a, const S& b)   { S c(a); c += b; return c; }
	friend S    operator+(const S& a, int b)        { S c(a); c += b; return c; }
	friend S    operator+(int a, const S& b)        { S c; c.Cat(a); c += b; return c; }
	friend S    operator+(const S& a, T b)          { S c(a); c += b; return c; }
	friend S    operator+(T a, const S& b)          { S c; c.Cat(a); c += b; return c; }
};

template <class T, class S>
class AStringBuffer {
protected:
	T      *begin;
	T      *end;
	T      *alloc;

	void Reads() { typename S::Data *d = S::GetData(begin); end = begin + d->length; alloc = begin + d->alloc; }
	void Realloc(int newalloc);
	void Expand(int len);
	void Expand();
	void Init(int len);
	void Init(S& text, int minalloc);
	void Free()                            { if(begin) S::GetData(begin)->Release(); }
	T   *Get();
	void Set(S& s);
	T   *Ptr()                             { *end = '\0'; return begin; }

	friend class String;
	friend class WString;

public:
	AStringBuffer();
	AStringBuffer(int length)              { Init(length); }
	AStringBuffer(S& text)                 { Init(text, text.GetCount()); }
	AStringBuffer(S& text, int length)     { Init(text, length); end = begin + length; }

	operator T*()                          { return Ptr(); }
	operator void*()                       { return Ptr(); }
	T *operator~()                         { return Ptr(); }

	void Cat(int c)                        { if(end >= alloc) Expand(); *end++ = c; }
	void Cat(int c, int count)             { if(end + count >= alloc) Expand(count);
	                                         Fill(end, end + count, (T)c); end += count; }
	void Cat(const T *s, int len)          { if(end + len >= alloc) Expand(len);
	                                         memcpy(end, s, sizeof(T) * len); end += len; }

	int  GetLength() const                 { return end - begin; }
	T   *SetLength(int len);
	int  GetCount() const                  { return GetLength(); }
	T   *SetCount(int len)                 { return SetLength(len); }
	T   *Begin()                           { return begin; }
	T   *End()                             { return end; }

	void Clear();
	void Reserve(int alloc);

	~AStringBuffer();

private:
	AStringBuffer(const AStringBuffer&);
	void operator=(const AStringBuffer&);
};

class WString;

class String : public AString<char, String> {
	typedef AString<char, String> B;

	String(Data *data)                         { ptr = CreateEmpty(data); }
	String(int, char *p) : B(0, p)             {}

	static Data *Void();
	static Data *Zero();

	static char *CreateNull();

//	friend class B;
	friend class AString<char, String>;
	friend class AStringBuffer<char, String>;

public:
	class Buffer;
	friend class String::Buffer;

	void  Cat(int c)                           { B::Cat(c); }
	void  Cat(const char *s);
	void  Cat(const String& s);

	void  Cat(int c, int count)                { B::Cat(c, count); }
	void  Cat(const char *s, int len)          { B::Cat(s, len); }
	void  Cat(const char *s, const char *lim)  { ASSERT(s <= lim); Cat(s, lim - s); }
	void  Cat(const String& s, int len)        { B::Cat(~s, len); }
	void  Cat(const byte *s, int len)          { B::Cat((const char *) s, len); }

	String& Cat()                              { return *this; }

	const String& operator+=(char c)           { Cat(c); return *this; }
	const String& operator+=(const char *s)    { Cat(s); return *this; }
	const String& operator+=(const String& s)  { Cat(s); return *this; }

	String& operator=(const char *s);
	String& operator=(const String& s)         { B::Assign(s); return *this; }
	String& operator=(Buffer& b)               { *this = String(b); return *this; }
	String& operator<<=(const String& s)       { B::DeepAssign(s); return *this; }

	void  Insert(int at, int c)                { B::Insert(at, c); }
	void  Insert(int at, const String& s);
	void  Insert(int at, const char *s, int l) { B::Insert(at, s, l); }
	void  Insert(int at, const char *s);

	String Mid(int pos, int length) const      { return String(0, B::Mid(pos, length)); }
	String Mid(int pos) const                  { return String(0, B::Mid(pos)); }
	String Right(int count) const              { return String(0, B::Right(count)); }
	String Left(int count) const               { return String(0, B::Left(count)); }

	bool   IsEqual(const String& s) const;
	bool   IsEqual(const char *s) const;

	int    Compare(const String& s) const;
	int    Compare(const char *s) const;

	const char *operator~() const              { STRING_STAT_CODE(StringPtr); return B::ptr; }
	operator const char*() const               { STRING_STAT_CODE(StringPtr); return B::ptr; }
	operator const void*() const               { STRING_STAT_CODE(StringPtr); return B::ptr; }
	operator const byte*() const               { STRING_STAT_CODE(StringPtr); return (byte *) B::ptr; }

	int operator*() const                      { STRING_STAT_CODE(StringPtr); return *B::ptr; }
	int operator[](int i) const                { STRING_STAT_CODE(StringIndex); ASSERT(i <= B::GetLength());
	                                             return (byte)ptr[i]; }
	bool   IsVoid() const;
	bool   IsZero() const;

	static String GetVoid();
	static String GetZero();

//	operator WString() const;

	WString       ToWString() const;
	const String& ToString() const             { return *this; }

	String();
	String(const Nuller&);
	String(const String& s);
	String(const char *s);
	String(const String& s, int n);
	String(const char *s, int n);
	String(const byte *s, int n);
	String(const char *s, const char *lim);
	String(int chr, int count);
	String(Buffer& b) : B(0, b.Get()) {};
	~String();

	friend void Swap(String& a, String& b)     { char *h = a.ptr; a.ptr = b.ptr; b.ptr = h; }

	class Buffer : public AStringBuffer<char, String> {
		typedef AStringBuffer<char, String> B;

	public:
		Buffer()                                           {}
		Buffer(int length) : B(length)                     {}
		Buffer(String& text) : B(text)                     {}
		Buffer(String& text, int length) : B(text, length) {}

		void  Cat(int c)                                   { B::Cat(c); }
		void  Cat(int c, int count)                        { B::Cat(c, count); }
		void  Cat(const char *s, int len)                  { B::Cat(s, len); }

		void  Cat(const char *s)                           { Cat(s, strlen(s)); }
		void  Cat(const String& s)                         { Cat(s, s.GetLength()); }
		void  Cat(const char *s, const char *lim)          { ASSERT(s <= lim); Cat(s, lim - s); }
		void  Cat(const String& s, int len)                { Cat(~s, len); }
		void  Cat(const byte *s, int len)                  { Cat((const char *) s, len); }

		void operator=(String& s)                          { Set(s); }

		void Strlen();
	};
};

typedef String::Buffer StringBuffer;

inline bool  IsEmpty(const String& s)      { return s.IsEmpty(); }

template <class T> bool IsNull(const T& x) { return x.IsNullInstance(); }

String FormatPtr(const void *p);

template <class T>
inline String AsString(const T& x)
{
	return x.ToString();
}

/*
template <class T>
inline String AsString(T *x)
{
	return FormatPtr(x);
}
*/

#ifdef PLATFORM_MSC
inline String AsString(const void *x)
{
	return FormatPtr(x);
}
#endif

inline String& operator<<(String& s, const char *x)
{
	s.Cat(x);
	return s;
}

inline String& operator<<(String& s, char *x)
{
	s.Cat(x);
	return s;
}

inline String& operator<<(String& s, const String &x)
{
	s.Cat(x);
	return s;
}

inline String& operator<<(String& s, char x)
{
	s.Cat((int) x);
	return s;
}

inline String& operator<<(String& s, const void *x)
{
	s << FormatPtr(x);
	return s;
}

inline String& operator<<(String& s, void *x)
{
	s << FormatPtr(x);
	return s;
}

template <class T>
inline String& operator<<(String& s, const T& x)
{
	s.Cat(AsString(x));
	return s;
}

template<>
inline String& operator<<(String& s, const char * const &x)
{
	s.Cat(x);
	return s;
}

template<>
inline String& operator<<(String& s, const String &x)
{
	s.Cat(x);
	return s;
}

template<>
inline String& operator<<(String& s, const char& x)
{
	s.Cat(x);
	return s;
}

template<>
inline bool  IsNull(const String& s)       { return s.IsEmpty(); }

template<>
inline String AsString(const String& s)     { return s; }

template<>
inline unsigned GetHashValue(const String& s) { return memhash(~s, s.GetLength()); }

int CompareNoCase(const String& a, const String& b, byte encoding = 0);
int CompareNoCase(const String& a, const char *b, byte encoding = 0);

inline
int CompareNoCase(const char *a, const String& b, byte encoding = 0) {
	return -CompareNoCase(b, a, encoding);
}

String TrimLeft(const String& s);
String TrimRight(const String& s);

inline StringBuffer& operator<<(StringBuffer& s, const char *x)
{
	s.Cat(x);
	return s;
}

inline StringBuffer& operator<<(StringBuffer& s, char *x)
{
	s.Cat(x);
	return s;
}

inline StringBuffer& operator<<(StringBuffer& s, const String &x)
{
	s.Cat(x);
	return s;
}

inline StringBuffer& operator<<(StringBuffer& s, char x)
{
	s.Cat((int) x);
	return s;
}

inline StringBuffer& operator<<(StringBuffer& s, const void *x)
{
	s << FormatPtr(x);
	return s;
}

inline StringBuffer& operator<<(StringBuffer& s, void *x)
{
	s << FormatPtr(x);
	return s;
}

template <class T>
inline StringBuffer& operator<<(StringBuffer& s, const T& x)
{
	s.Cat(AsString(x));
	return s;
}

template<>
inline StringBuffer& operator<<(StringBuffer& s, const char * const &x)
{
	s.Cat(x);
	return s;
}

template<>
inline StringBuffer& operator<<(StringBuffer& s, const String &x)
{
	s.Cat(x);
	return s;
}

template<>
inline StringBuffer& operator<<(StringBuffer& s, const char& x)
{
	s.Cat(x);
	return s;
}

class Value;

class WString : public AString<wchar, WString> {
	typedef AString<wchar, WString> B;

	WString(Data *data)                         { ptr = CreateEmpty(data); }
	WString(int, wchar *p) : B(0, p)            {}

	static Data *Void();
	static Data *Zero();

	static wchar *CreateNull();

//	friend class B;
	friend class AString<wchar, WString>;
	friend class AStringBuffer<wchar, WString>;

public:
	class Buffer;
	friend class WString::Buffer;

	void  Cat(int c)                            { B::Cat(c); }
	void  Cat(const wchar *s);
	void  Cat(const WString& s);

	void  Cat(int c, int count)                 { B::Cat(c, count); }
	void  Cat(const wchar *s, int len)          { B::Cat(s, len); }
	void  Cat(const wchar *s, const wchar *lim) { ASSERT(s <= lim); Cat(s, lim - s); }
	void  Cat(const WString& s, int len)        { B::Cat(~s, len); }

	WString& Cat()                              { return *this; }

	const WString& operator+=(wchar c)          { Cat(c); return *this; }
	const WString& operator+=(const wchar *s)   { Cat(s); return *this; }
	const WString& operator+=(const WString& s) { Cat(s); return *this; }

	WString& operator<<(wchar c)                { Cat(c); return *this; }
	WString& operator<<(const WString& s)       { Cat(s); return *this; }
	WString& operator<<(const wchar *s)         { Cat(s); return *this; }

	WString& operator=(const wchar *s);
	WString& operator=(const WString& s)        { B::Assign(s); return *this; }
	WString& operator=(Buffer& b)               { *this = WString(b); return *this; }
	WString& operator<<=(const WString& s)      { B::DeepAssign(s); return *this; }

	void  Insert(int at, int c)                 { B::Insert(at, c); }
	void  Insert(int at, const WString& s);
	void  Insert(int at, const wchar *s, int l) { B::Insert(at, s, l); }
	void  Insert(int at, const wchar *s);

	WString Mid(int pos, int length) const      { return WString(0, B::Mid(pos, length)); }
	WString Mid(int pos) const                  { return WString(0, B::Mid(pos)); }
	WString Right(int count) const              { return WString(0, B::Right(count)); }
	WString Left(int count) const               { return WString(0, B::Left(count)); }

	bool   IsEqual(const WString& s) const;
	bool   IsEqual(const wchar *s) const;

	int    Compare(const WString& s) const;
	int    Compare(const wchar *s) const;

	const wchar *operator~() const              { return B::ptr; }
	operator const wchar*() const               { return B::ptr; }
	operator const void*() const                { return B::ptr; }

	int operator*() const                       { return *B::ptr; }
	int operator[](int i) const                 { ASSERT(i <= B::GetLength()); return ptr[i]; }

	bool   IsVoid() const;
	bool   IsZero() const;

	static WString GetVoid();
	static WString GetZero();

	String ToString() const;

	WString();
	WString(const Nuller&);
	WString(const WString& s);
	WString(const wchar *s);
	WString(const WString& s, int n);
	WString(const wchar *s, int n);
	WString(const wchar *s, const wchar *lim);
	WString(const char *s);
	WString(const char *s, int n);
	WString(const char *s, const char *lim);
	WString(int chr, int count);
	WString(Buffer& b) : B(0, b.Get()) {};
	~WString();

	friend void Swap(WString& a, WString& b)    { wchar *h = a.ptr; a.ptr = b.ptr; b.ptr = h; }

	class Buffer : public AStringBuffer<wchar, WString> {
		typedef AStringBuffer<wchar, WString> B;

	public:
		Buffer()                                            {}
		Buffer(int length) : B(length)                      {}
		Buffer(WString& text) : B(text)                     {}
		Buffer(WString& text, int length) : B(text, length) {}

		void  Cat(int c)                                   { B::Cat(c); }
		void  Cat(int c, int count)                        { B::Cat(c, count); }
		void  Cat(const wchar *s, int len)                 { B::Cat(s, len); }

		void  Cat(const char *s)                           { Cat(WString(s)); }
		void  Cat(const wchar *s)                          { Cat(s, wstrlen(s)); }
		void  Cat(const WString& s)                        { Cat(s, s.GetLength()); }
		void  Cat(const wchar *s, const wchar *lim)        { ASSERT(s <= lim); Cat(s, lim - s); }
		void  Cat(const WString& s, int len)               { Cat(~s, len); }

		void operator=(WString& s)                         { Set(s); }

		void Strlen();
	};
};

typedef WString::Buffer WStringBuffer;

inline bool  IsEmpty(const WString& s)      { return s.IsEmpty(); }

template<>
inline bool  IsNull(const WString& s)       { return s.IsEmpty(); }

//template<>
//inline String AsString(const WString& s)     { return s; }

template<>
inline unsigned GetHashValue(const WString& s) { return memhash(~s, 2 * s.GetLength()); }

WString TrimLeft(const WString& str);
WString TrimRight(const WString& s);

int CompareNoCase(const WString& a, const WString& b);
int CompareNoCase(const WString& a, const wchar *b);

inline
int CompareNoCase(const wchar *a, const WString& b) {
	return -CompareNoCase(b, a);
}

template<> inline String AsString(const char * const &s)    { return s; }
template<> inline String AsString(char * const &s)          { return s; }
template<> inline String AsString(const char& a)            { return String(a, 1); }
template<> inline String AsString(const signed char& a)     { return String(a, 1); }
template<> inline String AsString(const unsigned char& a)   { return String(a, 1); }
template<> inline String AsString(const bool& a)            { return a ? "true" : "false"; }

unsigned ctoi(int c);

typedef int (*CharFilter)(int);

int CharFilterAscii(int c);
int CharFilterAscii128(int c);
int CharFilterUnicode(int c);
int CharFilterDigit(int c);
int CharFilterWhitespace(int c);
int CharFilterAlpha(int c);
int CharFilterAlphaToUpper(int c);
int CharFilterAlphaToLower(int c);
int CharFilterInt(int c);
int CharFilterDouble(int c);
int CharFilterDefaultToUpperAscii(int c);

String Filter(const char *s, int (*filter)(int));
String FilterWhile(const char *s, int (*filter)(int));
