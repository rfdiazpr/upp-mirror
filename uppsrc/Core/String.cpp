#include <Core/Core.h>

NAMESPACE_UPP

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

#ifdef STRING_EXPERIMENTAL
#define EQ1(i)  (a[i] ^ b[i])
#define EQ2(i)  (*(word *)(a + i) ^ *(word *)(b + i))
#define EQ4(i)  (*(dword *)(a + i) ^ *(dword *)(b + i))
#endif

bool String::IsEqual(const String& s) const
{
#ifdef STRING_EXPERIMENTAL
	int l = GetLength();
	if(s.GetLength() != l) return false;
	const char *a = ptr;
	const char *b = s;
	switch(l) {
	case  0: return true;
	case  1: return a[0] == b[0];
	case  2: return *(word *)a == *(word *)b;
	case  3: return (EQ2(0) | EQ1(2)) == 0;
	case  4: return *(dword *)a == *(dword *)b;
	case  5: return (EQ4(0) | EQ1(4)) == 0;
	case  6: return (EQ4(0) | EQ2(4)) == 0;
	case  7: return (EQ4(0) | EQ2(4) | EQ1(7)) == 0;
	case  8: return (EQ4(0) | EQ4(4)) == 0;
	case  9: return (EQ4(0) | EQ4(4) | EQ1(8)) == 0;
	case 10: return (EQ4(0) | EQ4(4) | EQ2(8) | EQ1(10)) == 0;
	case 11: return (EQ4(0) | EQ4(4) | EQ2(8) | EQ1(10)) == 0;
	case 12: return (EQ4(0) | EQ4(4) | EQ4(8)) == 0;
	case 13: return (EQ4(0) | EQ4(4) | EQ4(8) | EQ1(12)) == 0;
	case 14: return (EQ4(0) | EQ4(4) | EQ4(8) | EQ2(12)) == 0;
	}
#endif
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

WString String::ToWString() const
{
	return ToUnicode(*this, CHARSET_DEFAULT);
}

String::String() {}
String::String(const Nuller&) {}

String::String(const String& s) : B(s) {}
String::String(const char *s) : B(s) {}
String::String(int chr, int count) : B(chr, count) {}

#ifdef STRING_EXPERIMENTAL
#define CREATE(a, l) \
	n = (Data *) new byte[sizeof(Data) + a]; \
	n->alloc = a - 1; \
	n->refcount = 1; \
	t = ptr = (char *)(n + 1); \
	t[n->length = l] = 0;

#define COPY1         t[0]  = s[0];
#define COPY2  COPY1  t[1]  = s[1];
#define COPY3  COPY2  t[2]  = s[2];
#define COPY4  COPY3  t[3]  = s[3];
#define COPY5  COPY4  t[4]  = s[4];
#define COPY6  COPY5  t[5]  = s[5];
#define COPY7  COPY6  t[6]  = s[6];
#define COPY8  COPY7  t[7]  = s[7];
#define COPY9  COPY8  t[8]  = s[8];
#define COPY10 COPY9  t[9]  = s[9];
#define COPY11 COPY10 t[10] = s[10];
#define COPY12 COPY11 t[11] = s[11];
#define COPY13 COPY12 t[12] = s[12];
#define COPY14 COPY13 t[13] = s[13];
#define COPY15 COPY14 t[14] = s[14];
#define COPY16 COPY15 t[15] = s[15];

void String::Slice(const char *s, int len)
{
	Data *n;
	char *t;
	switch(len) {
	case 0:  ptr = CreateNull(); return;
	case 1:  CREATE(8, 1);   COPY1; return;
	case 2:  CREATE(8, 2);   COPY2; return;
	case 3:  CREATE(8, 3);   COPY3; return;
	case 4:  CREATE(8, 4);   COPY4; return;
	case 5:  CREATE(8, 5);   COPY5; return;
	case 6:  CREATE(8, 6);   COPY6; return;
	case 7:  CREATE(8, 7);   COPY7; return;
	case 8:  CREATE(16, 8);  COPY8; return;
	case 9:  CREATE(16, 9);  COPY9; return;
	case 10: CREATE(16, 10); COPY10; return;
	case 11: CREATE(16, 11); COPY11; return;
	case 12: CREATE(16, 12); COPY12; return;
	case 13: CREATE(16, 13); COPY13; return;
	case 14: CREATE(16, 14); COPY14; return;
	case 15: CREATE(16, 15); COPY15; return;
	}
	memcpy(ptr = Create(len), s, len);
}
#endif

String::String(const String& s, int n) : B(~s, n) { ASSERT(n <= s.GetLength() + 1); }
String::String(const char *s, int n) : B(s, n) {}
String::String(const byte *s, int n) : B((char *)s, n) {}

#ifndef STRING_EXPERIMENTAL
String::String(const char *s, const char *lim) : B(s, lim - s) {}
#endif

String::String(const std::string& s) : B(s.c_str(), s.length()) {}
String::operator std::string() const { return std::string(Begin(), End()); }

String::~String() {}

char *String::CreateNull()
{
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
WString::WString(const WString& s, int n) : B(~s, n) { ASSERT(n <= s.GetLength() + 1); }
WString::WString(const wchar *s, int n) : B(s, n) {}
WString::WString(const wchar *s, const wchar *lim) : B(s, lim - s) {}
WString::WString(const char *s) : B(ToUnicode(s, s ? strlen(s) : 0, CHARSET_DEFAULT)) {}
WString::WString(const char *s, int n) : B(ToUnicode(s, n, CHARSET_DEFAULT)) {}
WString::WString(const char *s, const char *lim) : B(ToUnicode(s, s ? lim - s : 0, CHARSET_DEFAULT)) {}
//WString::WString(char *s) : B(ToUnicode(s, CHARSET_DEFAULT)) {}
WString::WString(int chr, int count) : B(chr, count) {}

WString::WString(const std::wstring& s)
{
	if(sizeof(std::wstring::value_type) == sizeof(wchar)) {
		Assign((wchar *)s.c_str(), s.length());
	}
	else {
		std::wstring::const_iterator i = s.begin();
		while(i < s.end())
			Cat(*i++);
	}
}

WString::operator std::wstring() const
{
	if(sizeof(std::wstring::value_type) == sizeof(wchar))
		return std::wstring((std::wstring::value_type *)Begin(),
		                    (std::wstring::value_type *)End());
	else {
		std::wstring r;
		const wchar *s = Begin();
		while(s < End())
			r += *s++;
		return r;
	}
}

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

END_UPP_NAMESPACE
