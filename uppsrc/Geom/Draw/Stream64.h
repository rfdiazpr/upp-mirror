#ifndef _Geom_Draw_Stream6464_h_
#define _Geom_Draw_Stream6464_h_

class Stream64 {
protected:
	int64  pos;
	byte  *buffer;
	byte  *ptr;
	byte  *rdlim;
	byte  *wrlim;

	unsigned style:6;
	unsigned beginofline:1;
	unsigned radix:2;
	unsigned depth:7;
	unsigned errorcode:16;

	enum {
		BEGINOFLINE = 0x02,
	};

	virtual   void  _Put(int w);
	virtual   int   _Term();
	virtual   int   _Get();
	virtual   void  _Put(const void *data, dword size);
	virtual   dword _Get(void *data, dword size);

	int       _GetW();
	long      _GetL();
	bool      _IsEof() const;

	void      LdError();

public:
	virtual   void  Seek(int64 pos);
	virtual   int64 GetSize() const;
	virtual   void  SetSize(int64 size);
	virtual   void  Flush();
	virtual   void  Close();
	virtual   bool  IsOpen() const = 0;

	Stream64();
	virtual  ~Stream64();

	word      GetStyle() const     { return style; }

	bool      IsError() const      { return style & STRM_ERROR; }
	bool      IsOK() const         { return !IsError(); }
	void      SetError(int c = 0)  { style |= STRM_ERROR; errorcode = c; }
#ifdef PLATFORM_WIN32
	void      SetLastError()       { SetError(GetLastError()); }
#endif
#ifdef PLATFORM_POSIX
	void      SetLastError()       { SetError(errno); }
#endif
	int       GetError() const     { return errorcode; }
	void      ClearError()         { style &= ~STRM_ERROR; errorcode = 0; }

	int64     GetPos() const       { return dword(ptr - buffer) + pos; }
	bool      IsEof() const        { return ptr >= rdlim && _IsEof(); }
	int64     GetLeft() const      { return GetSize() - GetPos(); }
	void      SeekEnd(int rel = 0) { Seek(GetSize() + rel); }
	void      SeekCur(int rel)     { Seek(GetPos() + rel); }

	void      Put(int c)           { if(ptr < wrlim) *ptr++ = c; else _Put(c); }
	int       Term()               { return ptr < rdlim ? *ptr : _Term(); }
	int       Get()                { return ptr < rdlim ? *ptr++ : _Get(); }

	void      Put(const void *data, dword size)  { _Put(data, size); }
	dword     Get(void *data, dword size)        { return _Get(data, size); }

#if defined(CPU_LE) && defined(CPU_IA32)
	int       GetW()               { if(ptr >= rdlim - 1) return _GetW(); int q = *(word *)ptr; ptr += 2; return q; }
	long      GetL()               { if(ptr >= rdlim - 3) return _GetL(); long q = *(long *)ptr; ptr += 4; return q; }
#else
	int       GetW()               { return _GetW(); }
	long      GetL()               { return _GetL(); }
#endif

	bool      GetAll(void *data, dword size);

	int       GetIW()              { return GetW(); }
	long      GetIL()              { return GetL(); }
	int       GetMW();
	long      GetML();

	String    GetLine();

#if defined(CPU_LE) && defined(CPU_IA32)
	void      PutW(word q)         { if(ptr < wrlim - 1) { *(word *)ptr = q; ptr += 2; } else Put(&q, 2); }
	void      PutL(long q)         { if(ptr < wrlim - 3) { *(long *)ptr = q; ptr += 4; } else Put(&q, 4); }
#else
	void      PutW(word q)         { Put(&q, 2); }
	void      PutL(long q)         { Put(&q, 4); }
#endif

#ifdef CPU_LE
	void      PutIW(word q)        { PutW(q); }
	void      PutIL(long q)        { PutL(q); }
#else
	void      PutIW(word q);
	void      PutIL(long q);
#endif
	void      PutMW(word q);
	void      PutML(long q);

	void      Put(const char *s);
	void      Put(const String& s);
	void      Put(int c, int count);
	void      Put0(int count)      { Put(0, count); }

#ifdef CPU_LE
	void      PutW(const wchar *s, int count)       { Put(s, count * 2); }
	dword     GetW(wchar *s, int count)             { return Get(s, count * 2) / 2; }
	bool      GetAllW(wchar *s, int count)          { return GetAll(s, count * 2); }
#endif

	void      PutCrLf()            { Put('\r'); Put('\n'); }
#ifdef PLATFORM_WIN32
	void      PutEol()             { PutCrLf(); }
#else
	void      PutEol()             { Put('\n'); }
#endif
	void      PutLine(const char *s);
	void      PutLine(const String& s);
#ifndef EXPERIMENTAL
	void      PutFormat(const char *s, ...);
#endif
	void      Put(Stream64& s, int64 size = INT64_MAX, dword click = 4096);

	int       GetUtf8();

//  Stream64 as formatted text output or log, slower but with begin/end justification

	void      Putf(int c);
	void      Putf(const char *s);
#ifndef EXPERIMENTAL
	void      Format(const char *s, ...);
#endif
	void      PutHex(uint32 i);
	void      PutDec(uint32 i);
	void      PutBin(uint32 i);
	void      PutUnsigned(uint32 i);
	void      PutInt(int32 i);
	void      PutDouble(double a);

	void      Begin()                      { depth++; }
	void      End()                        { depth--; }

	Stream64&   operator<<(BeginIndentEnum)  { Begin(); return *this; }
	Stream64&   operator<<(EndIndentEnum)    { End(); return *this; }

	void      NewLine()                    { Putf('\n'); }

	Stream64&   operator<<(RadixEnum i)      { radix = i; return *this; }
/*
	Stream64&   operator<<(const String& s)  { Putf(s); return *this; }
	Stream64&   operator<<(const char *s)    { Putf(s); return *this; }
	Stream64&   operator<<(char *s)          { Putf(s); return *this; }
	Stream64&   operator<<(char a)           { Putf(a);  return *this; }
	Stream64&   operator<<(signed char a)    { Putf(a);  return *this; }
	Stream64&   operator<<(unsigned char a)  { PutUnsigned(a);  return *this; }
	Stream64&   operator<<(short a)          { PutInt(a); return *this; }
	Stream64&   operator<<(unsigned short a) { PutUnsigned(a); return *this; }
	Stream64&   operator<<(int a)            { PutInt(a); return *this; }
	Stream64&   operator<<(unsigned int a)   { PutUnsigned(a); return *this; }
	Stream64&   operator<<(long a)           { PutInt(a); return *this; }
	Stream64&   operator<<(unsigned long a)  { PutUnsigned(a); return *this; }
	Stream64&   operator<<(float a)          { PutDouble(a); return *this; }
	Stream64&   operator<<(double a)         { PutDouble(a); return *this; }
	Stream64&   operator<<(const void *a)    { PutHex((dword)a); return *this; }*/
	Stream64&   operator<<(bool a);

//  Stream64 as serialization streamer
	void      SetLoading()                 { ASSERT(style & STRM_READ); style |= STRM_LOADING; }
	void      SetStoring()                 { ASSERT(style & STRM_WRITE); style &= ~STRM_LOADING; }
	bool      IsLoading()                  { return style & STRM_LOADING; }
	bool      IsStoring()                  { return !IsLoading(); }

	void      LoadThrowing()               { style |= STRM_THROW; }
	void      LoadError()                  { LdError(); }

	void      SerializeRaw(byte *data, dword count);
	void      SerializeRaw(word *data, dword count);
	void      SerializeRaw(dword *data, dword count);

	void      SerializeRLE(byte *data, dword count);

	Stream64&   operator%(bool& d);
	Stream64&   operator%(char& d);
	Stream64&   operator%(signed char& d);
	Stream64&   operator%(unsigned char& d);
	Stream64&   operator%(short& d);
	Stream64&   operator%(unsigned short& d);
	Stream64&   operator%(int& d);
	Stream64&   operator%(unsigned int& d);
	Stream64&   operator%(long& d);
	Stream64&   operator%(unsigned long& d);
	Stream64&   operator%(float& d);
	Stream64&   operator%(double& d);
	Stream64&   operator%(int64& d);
	Stream64&   operator%(uint64& d);

	Stream64&   operator%(String& s);
	Stream64&   operator/(String& s);

	Stream64&   operator%(WString& s);
	Stream64&   operator/(WString& s);

	void      Pack(dword& i);
	Stream64&   operator/(int& i)            { dword w = i + 1; Pack(w); i = w - 1; return *this; }
	Stream64&   operator/(unsigned int& i)   { Pack(*(dword *)&i); return *this; }
	Stream64&   operator/(long& i)           { dword w = i + 1; Pack(w); i = w - 1; return *this; }
	Stream64&   operator/(unsigned long& i)  { Pack(*(dword *)&i); return *this; }

	void      Magic(dword magic = 0x7d674d7b);

	void      Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f, bool& g, bool& h);
	void      Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f, bool& g);
	void      Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f);
	void      Pack(bool& a, bool& b, bool& c, bool& d, bool& e);
	void      Pack(bool& a, bool& b, bool& c, bool& d);
	void      Pack(bool& a, bool& b, bool& c);
	void      Pack(bool& a, bool& b);

private: // No copy
	Stream64(const Stream64& s);
	void operator=(const Stream64& s);
};

String LoadStream64(Stream64& in);
bool   SaveStream64(Stream64& out, const String& data);

class StringStream64 : public Stream64 {
protected:
	virtual  void  _Put(int w);
	virtual  int   _Term();
	virtual  int   _Get();
	virtual  void  _Put(const void *data, dword size);
	virtual  dword _Get(void *data, dword size);

public:
	virtual  void  Seek(int64 pos);
	virtual  int64 GetSize() const;
	virtual  void  SetSize(int64 size);
	virtual  void  Close();
	virtual  bool  IsOpen() const;

protected:
	bool        isOpen;
	String      data;
	int64       size;
	int         chunk;

public:
	void        Open(const String& data);
	void        Create();

	void        SetChunk(int size)           { chunk = size; }
	operator    bool() const                 { return IsOpen(); }
	String      GetResult() const;
	operator    String() const               { return GetResult(); }

	StringStream64();
	StringStream64(const String& data);
	~StringStream64();
};

class MemStream64 : public Stream64 {
protected:
	virtual   void  _Put(const void *data, dword size);
	virtual   dword _Get(void *data, dword size);

public:
	virtual   void  Seek(int64 pos);
	virtual   int64 GetSize() const;
	virtual   bool  IsOpen() const;

public:
	MemStream64(void *data, int size);
#ifdef flagSO
	virtual ~MemStream64();
#endif
};

class MemReadStream64 : public MemStream64 {
public:
	MemReadStream64(const void *data, int size);
};

class StrStream64 : public MemReadStream64 {
public:
	StrStream64(const char *s) : MemReadStream64(s, strlen(s)) {}
	StrStream64(const String& s) : MemReadStream64(s, s.GetLength()) {}
#ifdef PLATFORM_MFC
	StrStream64(const CString& s) : MemReadStream64(s, s.GetLength()) {}
#endif
};

class BufferStream64 : public Stream64 {
protected:
	dword     buffersize;

	void      Init(dword size);

public:
	void      SetBufferSize(dword newsize);

	BufferStream64(dword buffersize = DEFAULT_STREAM_BUFFER_SIZE);
	~BufferStream64();
};

class BlockStream64 : public BufferStream64 {
protected:
	virtual  void  _Put(int w);
	virtual  int   _Term();
	virtual  int   _Get();
	virtual  void  _Put(const void *data, dword size);
	virtual  dword _Get(void *data, dword size);

public:
	virtual  void  Seek(int64 pos);
	virtual  int64 GetSize() const;
	virtual  void  SetSize(int64 size);
	virtual  void  Flush();

	BlockStream64();
	virtual ~BlockStream64();

protected:
	virtual  dword Read(int64 at, void *ptr, dword size);
	virtual  void  Write(int64 at, const void *data, dword size);
	virtual  void  SetStreamSize(int64 size);

	int64    file_size;
	dword    wramount;

	void     SetWramount();

public:
	enum {
		READ, CREATE, APPEND, READWRITE,

		NOWRITESHARE = 0x10,
		DELETESHARE = 0x20,
		NOREADSHARE = 0x40,
		SHAREMASK = 0x70,
	};
	typedef int OpenMode;

protected:
	void     OpenInit(OpenMode mode);
};

class FileStream64 : public BlockStream64 {
protected:
	virtual  void  SetStreamSize(int64 size);
	virtual  dword Read(int64 at, void *ptr, dword size);
	virtual  void  Write(int64 at, const void *data, dword size);

public:
	virtual  void  Close();
	virtual  bool  IsOpen() const;

protected:
#ifdef PLATFORM_WIN32
	HANDLE    handle;
#endif
#ifdef PLATFORM_POSIX
	int       handle;
#endif

	void      SetPos(int64 pos);
	void      Init(int64 size);

public:
	operator  bool() const                 { return IsOpen(); }
	FileTime  GetTime() const;

#ifdef PLATFORM_WIN32
	void      SetTime(const FileTime& tm);
	bool      Open(const char *filename, OpenMode mode);

	FileStream64(const char *filename, OpenMode mode);
#endif

#ifdef PLATFORM_POSIX
	bool      Open(const char *filename, OpenMode mode, mode_t acm = 0644);
	FileStream64(const char *filename, OpenMode mode, mode_t acm = 0644);
	FileStream64(int std_handle);
#endif

	FileStream64();
	~FileStream64();

#ifdef PLATFORM_WIN32
	HANDLE    GetHandle() const            { return handle; }
#endif
#ifdef PLATFORM_POSIX
	int       GetHandle() const            { return handle; }
#endif
};

class FileOut64 : public FileStream64 {
public:
#ifdef PLATFORM_POSIX
	bool Open(const char *fn, mode_t acm = 0644)
	{ return FileStream64::Open(fn, FileStream64::CREATE, acm); }
#endif
#ifdef PLATFORM_WIN32
	bool Open(const char *fn)              { return FileStream64::Open(fn, FileStream64::CREATE); }
#endif


	FileOut64(const char *fn)                { Open(fn); }
	FileOut64()                              {}
};

class FileAppend64 : public FileStream64 {
public:
	bool Open(const char *fn)         { return FileStream64::Open(fn, FileStream64::APPEND); }

	FileAppend64(const char *fn)        { Open(fn); }
	FileAppend64()                      {}
};

class FileIn64 : public FileStream64 {
public:
	bool Open(const char *fn)         { return FileStream64::Open(fn, FileStream64::READ); }

	FileIn64(const char *fn)            { Open(fn); }
	FileIn64()                          {}
};

class SizeStream64 : public BufferStream64 {
protected:
	virtual void  _Put(int w);
	virtual void  _Put(const void *data, dword size);

public:
	virtual void  Seek(int64 pos);
	virtual int64 GetSize() const;
	virtual void  SetSize(int64 size);
	virtual bool  IsOpen() const;

protected:
	int64   size;

public:
	operator int64() const            { return GetSize(); }

	void     Open()                   { ptr = buffer; size = 0; }

	SizeStream64();
};

class CompareStream64 : public BufferStream64 {
protected:
	virtual  void  _Put(int w);
	virtual  void  _Put(const void *data, dword size);

public:
	virtual  void  Seek(int64 pos);
	virtual  int64 GetSize() const;
	virtual  void  SetSize(int64 size);
	virtual  void  Close();
	virtual  bool  IsOpen() const;
	virtual  void  Flush();

protected:
	Stream64  *stream;
	bool     equal;
	int64    size;

	void     Compare(int64 pos, const void *data, dword size);

public:
	void     Open(Stream64& aStream64);

	bool     IsEqual()                         { Flush(); return equal; }
	operator bool()                            { Flush(); return equal; }

	CompareStream64();
	CompareStream64(Stream64& aStream64);
};

class Stream64Stream : public BlockStream {
public:
	Stream64Stream(Stream64& stream64, int64 offset);
	virtual ~Stream64Stream();

	virtual   bool  IsOpen() const;

protected:
	virtual  dword Read(dword at, void *ptr, dword size);
	virtual  void  Write(dword at, const void *data, dword size);
	virtual  void  SetStreamSize(dword size);

protected:
	Stream64& stream64;
	int64     offset64;
};

int64        CopyStream64(Stream64& dest, Stream64& src, int64 count);
inline int64 CopyStream64(Stream64& dest, Stream64& src) { return CopyStream64(dest, src, src.GetLeft()); }

// Stream64ing templates

template <class T>
inline Stream64& operator%(Stream64& s, T& x)
{
	x.Serialize(s);
	return s;
}

inline Stream64& operator<<(Stream64& s, const char *x)
{
	s.Putf(x);
	return s;
}

inline Stream64& operator<<(Stream64& s, char *x)
{
	s.Putf(x);
	return s;
}

inline Stream64& operator<<(Stream64& s, const String &x)
{
	s.Putf(x);
	return s;
}

inline Stream64& operator<<(Stream64& s, char x)
{
	s.Putf((int) x);
	return s;
}

inline Stream64& operator<<(Stream64& s, const void *x)
{
	s << FormatPtr(x);
	return s;
}

inline Stream64& operator<<(Stream64& s, void *x)
{
	s << FormatPtr(x);
	return s;
}

template <class T>
inline Stream64& operator<<(Stream64& s, const T& x) {
	s << AsString(x);
	return s;
}

// ------

Stream64& Pack16(Stream64& s, int& i);
Stream64& Pack16(Stream64& s, int& i1, int& i2);
Stream64& Pack16(Stream64& s, int& i1, int& i2, int& i3);
Stream64& Pack16(Stream64& s, int& i1, int& i2, int& i3, int& i4);
Stream64& Pack16(Stream64& s, int& i1, int& i2, int& i3, int& i4, int& i5);

int     Stream64Heading(Stream64& stream, int ver, int minver, int maxver, const char* tag);

#endif
