#include <Core/Core.h>

void  String::Cat(const char *s)
{
	B::Cat(s, strlen(s));
}

void  String::Cat(const String& s)
{
	B::Cat(~s, s.GetLength());
}

void String::Insert(int at, const String& s)
{
	Insert(at, ~s, s.GetLength());
}

void String::Insert(int at, const char *s)
{
	Insert(at, s, strlen(s));
}

String& String::operator=(const char *s)
{
	if(!s)
		Clear();
	else
		Assign(s, strlen(s));
	return *this;
}

bool String::IsEqual(const String& s) const
{
	return B::IsEqual(s, s.GetCount());
}

bool String::IsEqual(const char *s) const
{
	return B::IsEqual(s, strlen(s));
}

struct StringCompare__
{
	int operator()(char a, char b) const { return byte(a) - byte(b); }
};

int String::Compare(const String& s) const
{
	STRING_STAT_CODE(StringCompare);
	int l1 = GetLength();
	int l2 = s.GetLength();
	int l = min(l1, l2);
	int q = memcmp(ptr, ~s, l);
	return q ? q : l1 - l2;
}

int String::Compare(const char *s) const
{
	int l1 = GetLength();
	int l2 = strlen(s);
	int l = min(l1, l2);
	int q = memcmp(ptr, s, l);
	return q ? q : l1 - l2;
}

/*
String::operator WString() const
{
	return ToUnicode(*this, CHARSET_DEFAULT);
}
*/

WString String::ToWString() const
{
	return ToUnicode(*this, CHARSET_DEFAULT);
}

String::String() {}
String::String(const Nuller&) {}

String::String(const String& s) : B(s)
{
	STRING_STAT_CODE(StringCopy);
}

String::String(const char *s) : B(s) {}
String::String(const String& s, int n) : B(~s, s.GetLength()) {}
String::String(const char *s, int n) : B(s, n) {}
String::String(const byte *s, int n) : B((char *)s, n) {}
String::String(const char *s, const char *lim) : B(s, lim - s) {}
String::String(int chr, int count) : B(chr, count) {}

String::~String() {
	STRING_STAT_CODE(StringDestruct);
}

char *String::CreateNull()
{
#ifdef STRING_STATS
	StringNull++;
#endif
	static Data data[] = { { 1, 0, 0 }, { 0 } };
	data->Retain();
	return (char *)(data + 1);
}

String::Data *String::Void()
{
	static Data d[] = { { 2, 0, 0 }, { 0 } };
	return d;
}

 String::Data *String::Zero()
{
	static Data d[] = { { 3, 0, 0 }, { 0 } };
	return d;
}

bool   String::IsVoid() const
{
	return ptr == Void()->GetPtr();
}

bool   String::IsZero() const
{
	return ptr == Zero()->GetPtr();
}

String String::GetVoid()
{
	return String(Void());
}

String String::GetZero()
{
	return String(Zero());
}

void String::Buffer::Strlen()
{
	int len = strlen(begin);
	ASSERT(len < end - begin);
	end = begin + len;
}

String TrimLeft(const String& str)
{
	const char *s = str;
	if(!IsSpace(*s))
		return str;
	while(IsSpace((byte)*s)) s++;
	return String(s, str.End());
}

String TrimRight(const String& str)
{
	if(str.IsEmpty())
		return str;
	const char *s = str.Last();
	if(!IsSpace(*s))
		return str;
	while(s >= ~str && IsSpace((byte)*s)) s--;
	return String(~str, s + 1);
}

struct StringICompare__
{
	int encoding;
	int operator()(char a, char b) const { return ToUpper(a, encoding) - ToUpper(b, encoding); }

	StringICompare__(int e) : encoding(e) {}
};

int CompareNoCase(const String& a, const String& b, byte encoding)
{
	if(encoding == CHARSET_DEFAULT) encoding = GetDefaultCharset();
	if(encoding == CHARSET_UTF8) return CompareNoCase(FromUtf8(a), FromUtf8(b));
	return IterCompare(a.Begin(), a.End(), b.Begin(), b.End(), StringICompare__(encoding));
}

int CompareNoCase(const String& a, const char *b, byte encoding)
{
	if(encoding == CHARSET_DEFAULT) encoding = GetDefaultCharset();
	if(encoding == CHARSET_UTF8) return CompareNoCase(FromUtf8(a), FromUtf8(b, strlen(b)));
	return IterCompare(a.Begin(), a.End(), b, b + strlen(b), StringICompare__(encoding));
}

// -----------------------------------------------------------------------------------

void  WString::Cat(const wchar *s)
{
	B::Cat(s, wstrlen(s));
}

void  WString::Cat(const WString& s)
{
	B::Cat(~s, s.GetLength());
}

void WString::Insert(int at, const WString& s)
{
	Insert(at, ~s, s.GetLength());
}

void WString::Insert(int at, const wchar *s)
{
	Insert(at, s, wstrlen(s));
}

WString& WString::operator=(const wchar *s)
{
	if(!s)
		Clear();
	else
		Assign(s, wstrlen(s));
	return *this;
}

bool WString::IsEqual(const WString& s) const
{
	return B::IsEqual(s, s.GetCount());
}

bool WString::IsEqual(const wchar *s) const
{
	return B::IsEqual(s, wstrlen(s));
}

struct WStringCompare__
{
	int operator()(wchar a, wchar b) const { return a - b; }
};

int WString::Compare(const WString& s) const
{
	return IterCompare(Begin(), End(), s.Begin(), s.End(), WStringCompare__());
}

int WString::Compare(const wchar *s) const
{
	return IterCompare(Begin(), End(), s, s + wstrlen(s), WStringCompare__());
}

/*
WString::operator String() const
{
	return FromUnicode(*this, CHARSET_DEFAULT);
}
*/

String WString::ToString() const
{
	return FromUnicode(*this, CHARSET_DEFAULT);
}

WString::WString() {}
WString::WString(const Nuller&) {}

WString::WString(const WString& s) : B(s) {}

WString::WString(const wchar *s) : B(s) {}
//WString::WString(wchar *s) : B(s) {}
WString::WString(const WString& s, int n) : B(~s, s.GetLength()) {}
WString::WString(const wchar *s, int n) : B(s, n) {}
WString::WString(const wchar *s, const wchar *lim) : B(s, lim - s) {}
WString::WString(const char *s) : B(ToUnicode(s, s ? strlen(s) : 0, CHARSET_DEFAULT)) {}
WString::WString(const char *s, int n) : B(ToUnicode(s, n, CHARSET_DEFAULT)) {}
WString::WString(const char *s, const char *lim) : B(ToUnicode(s, s ? lim - s : 0, CHARSET_DEFAULT)) {}
//WString::WString(char *s) : B(ToUnicode(s, CHARSET_DEFAULT)) {}
WString::WString(int chr, int count) : B(chr, count) {}

WString::~WString() {}

wchar *WString::CreateNull()
{
	static Data data[] = { { 1, 0, 0 }, { 0 } };
	data->Retain();
	return (wchar *)(data + 1);
}

WString::Data *WString::Void()
{
	static Data d[] = { { 2, 0, 0 }, { 0 } };
	return d;
}

WString::Data *WString::Zero()
{
	static Data d[] = { { 3, 0, 0 }, { 0 } };
	return d;
}

bool   WString::IsVoid() const
{
	return ptr == Void()->GetPtr();
}

bool   WString::IsZero() const
{
	return ptr == Zero()->GetPtr();
}

WString WString::GetVoid()
{
	return WString(Void());
}

WString WString::GetZero()
{
	return WString(Zero());
}

void WString::Buffer::Strlen()
{
	int len = wstrlen(begin);
	ASSERT(len < end - begin);
	end = begin + len;
}

WString TrimLeft(const WString& str)
{
	const wchar *s = str;
	if(!IsSpace(*s))
		return s;
	while(IsSpace(*s)) s++;
	return WString(s, str.End());
}

WString TrimRight(const WString& str)
{
	if(str.IsEmpty())
		return str;
	const wchar *s = str.Last();
	if(!IsSpace(*s))
		return str;
	while(s >= ~str && IsSpace(*s)) s--;
	return WString(~str, s + 1);
}

struct WStringICompare__
{
	int operator()(wchar a, wchar b) const { return ToUpper(a) - ToUpper(b); }
};

int CompareNoCase(const WString& a, const WString& b)
{
	return IterCompare(a.Begin(), a.End(), b.Begin(), b.End(), WStringICompare__());
}

int CompareNoCase(const WString& a, const wchar *b)
{
	return IterCompare(a.Begin(), a.End(), b, b + wstrlen(b), WStringICompare__());
}

#ifdef STRING_STATS

int StringClone[64];
int StringPtr[64];
int StringIndex[64];
int StringLength[64];
int StringRetain[64];
int StringRelease[64];
int StringRetainR[64];
int StringReleaseR[64];
int StringAssign[64];
int StringCopy[64];
int StringDestruct[64];
int StringCompare[64];
int StringAll[64];
int StringAlloc[64];
int StringCat1[64];
int StringCatN[64];
int StringNull;

void DumpStringStats(const char *s, int *d, bool acc = true)
{
	RLOG("--------------------");
	int sum = 0;
	for(int i = 0; i < 64; i++) {
//		StdLog() << Format("%7d|", data[i]);
		sum += d[i];
	}
	RLOG(s << " " << sum);
	int q = 0;
	for(int j = 0; j < 4; j++) {
		const int *data = d + 16 * j;
		for(int i = 0; i < 16; i++)
			StdLog() << Format("%4d|", i + 16 * j);
		StdLog() << '\n';
		for(int i = 0; i < 16; i++) {
			if(acc)
				q += data[i];
			else
				q = data[i];
			StdLog() << Format("%3d%%|", 100.0 * q / sum);
		}
		StdLog() << "\n\n";
	}
	for(int i = 0; i < 64; i++)
		StringAll[i] += d[i];
}

void DumpStringStats()
{
	RLOG("StringNull: " << StringNull);
	DumpStringStats("Cat", StringClone);
	DumpStringStats("Ptr", StringPtr);
	DumpStringStats("Length", StringLength);
	DumpStringStats("Retain", StringRetain);
	DumpStringStats("Release", StringRelease);
	DumpStringStats("Assign", StringAssign);
	DumpStringStats("Copy", StringCopy);
	DumpStringStats("Destruct", StringDestruct);
	DumpStringStats("Index", StringIndex);
	DumpStringStats("Compare", StringCompare);
	DumpStringStats("ALL", StringAll);

	DumpStringStats("RetainCount", StringRetainR, false);
	DumpStringStats("ReleaseCount", StringReleaseR, false);
	DumpStringStats("Cat1Count", StringCat1, false);
	DumpStringStats("CatNCount", StringCatN, false);

	DumpStringStats("Alloc", StringAlloc, false);
}

#else

void DumpStringStats() {}

#endif
