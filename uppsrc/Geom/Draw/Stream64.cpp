#include <Core/Core.h>
#pragma hdrstop

#include "Stream64.h"

#ifdef PLATFORM_POSIX
#include <sys/mman.h>
#endif

#define LLOG(x) // RLOG(x)
#define LDUMP(x) // RDUMP(x)
#define LLOGHEXDUMP(x, y) // RLOGHEXDUMP(x, y)

void Stream64::_Put(const void *data, dword size) {
	const byte *s = (const byte *) data;
	while(size--)
		Put(*s++);
}

dword Stream64::_Get(void *data, dword size) {
	int c;
	byte *s = (byte *) data;
	dword sz;
	for(sz = 0; sz < size && (c = Get()) >= 0; sz++)
		*s++ = c;
	return sz;
}

void Stream64::_Put(int w) {
	SetError(ERROR_NOT_ENOUGH_SPACE);
}

int  Stream64::_Get() {
	return -1;
}

int  Stream64::_Term() {
	return -1;
}

void Stream64::Seek(int64) {
	NEVER();
}

int64 Stream64::GetSize() const {
	return 0;
}

void Stream64::SetSize(int64) {
	NEVER();
}

bool Stream64::IsOpen() const { return false; }

void Stream64::Close() {}

void Stream64::Flush() {}

Stream64::Stream64() {
	ASSERT(sizeof(short int) == 2 && sizeof(int) == 4 &&
		   *(dword *) "\x12\x34\x56\x78" == 0x78563412);
	pos = style = 0;
	buffer = ptr = rdlim = wrlim = NULL;
	beginofline = true;
	radix = usedefault;
	depth = 0;
}

Stream64::~Stream64() {}

bool Stream64::_IsEof() const
{
	return GetPos() >= GetSize();
}

bool Stream64::GetAll(void *data, dword size) {
	return Get(data, size) == size;
}

int  Stream64::_GetW() {
	word w;//#
	return Get(&w, 2) == 2 ? w : -1;
}

long Stream64::_GetL() {
	long l;//#
	return Get(&l, 4) == 4 ? l : -1;
}

int  Stream64::GetMW() {
	byte h[2];
	int q;
	h[1] = Get();
	h[0] = q = Get();
	return q < 0 ? -1 : *(word *)h;
}

long Stream64::GetML() {
	byte h[4];
	int q;
	h[3] = Get();
	h[2] = Get();
	h[1] = Get();
	h[0] = q = Get();
	return q < 0 ? -1 : *(long *)h;
}

String Stream64::GetLine() {
	String result;
	for(;;) {
		int c = Get();
		if(c < 0)
			return result.IsEmpty() ? String::GetVoid() : result;
		if(c == '\n')
			return result;
		if(c != '\r')
			result.Cat(c);
	}
}

int Stream64::GetUtf8()
{
	word code = Get();
	if(code <= 0x7F)
		return code;
	else
	if(code <= 0xDF)
		return IsEof() ? -1 : ((code - 0xC0) << 6) + Get() - 0x80;
	else
	if(code <= 0xEF) {
		int c0 = Get();
		int c1 = Get();
		return c1 < 0 ? -1 : ((code - 0xE0) << 12) + ((c0 - 0x80) << 6) + c1 - 0x80;
	}
	else //!!!!! add complete Utf here !!!!
	if(code <= 0xF7) {
		Get(); Get(); Get();
		return 32;
	}
	else
	if(code <= 0xFB) {
		Get(); Get(); Get(); Get();
		return 32;
	}
	else {
		Get(); Get(); Get(); Get(); Get();
		return 32;
	}
}

#ifndef CPU_LE
void Stream64::PutIW(word q) {
	byte *h = (byte *) &q;
	Put(h[0]);
	Put(h[1]);
}
#endif

#ifndef CPU_LE
void Stream64::PutIL(long q) {
	byte *h = (byte *) &q;
	Put(h[0]);
	Put(h[1]);
	Put(h[2]);
	Put(h[3]);
}
#endif

void Stream64::PutMW(word q) {
	byte *h = (byte *) &q;
	Put(h[1]);
	Put(h[0]);
}

void Stream64::PutML(long q) {
	byte *h = (byte *) &q;
	Put(h[3]);
	Put(h[2]);
	Put(h[1]);
	Put(h[0]);
}

void Stream64::Put(const char *s) {
	Put(s, strlen(s));
}

void Stream64::Put(const String& s) {
	Put((const char *) s, s.GetLength());
}

void Stream64::Put(int c, int count) {
	while(count) {
		int n = min(count, wrlim - ptr);
		if(n > 0) {
			memset(ptr, c, n);
			ptr += n;
			count -= n;
		}
		else {
			Put(c);
			count--;
		}
	}
}

void Stream64::PutLine(const char *s) {
	Put(s);
	PutEol();
}

void Stream64::PutLine(const String& s) {
	Put(s);
	PutEol();
}

#ifndef EXPERIMENTAL
void Stream64::PutFormat(const char *s, ...) {
	va_list argptr;
	va_start(argptr, s);
	Put(VFormat(s, argptr));
}
#endif

void  Stream64::Put(Stream64& s, int64 size, dword click) {
	Buffer<byte> buffer(click);
	while(size) {
		dword n = s.Get(buffer, (int)min<int64>(click, size));
		Put(buffer.operator const byte *(), click);
		size -= n;
	}
}

void  Stream64::Putf(int c) {
	if(beginofline) {
		beginofline = false;
		for(int a = depth; a--;)
			Put('\t');
	}
	if(c == '\n') {
	#ifdef PLATFORM_WIN32
		Put('\r');
	#endif
		Put('\n');
		beginofline = true;
		radix = usedefault;
	}
	else
	if(c != '\r')
		Put(c);
}

void  Stream64::Putf(const char *s) {
	while(*s) Putf(*s++);
}

#ifndef EXPERIMENTAL
void  Stream64::Format(const char *fmt, ...) {
	char buffer[1024];//!!! not ideal...
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(buffer, fmt, (char *)argptr);
	va_end(argptr);
	Putf(buffer);
}
#endif

void  Stream64::PutHex(uint32 a) {
	Putf(Sprintf("%lX", a));
}

void  Stream64::PutDec(uint32 a) {
	Putf(Sprintf("%lu", a));
}

void  Stream64::PutBin(uint32 a) {
	for(int i = 0; i < 32; i++)
		if((0x80000000 >> i) & a) {
			for(; i < 32; i++)
				Put((0x80000000 >> i) & a ? '1' : '0');
			return;
		}
	Put('0');
}

void  Stream64::PutInt(int32 a) {
	if(radix == usehex)
		PutHex(a);
	else
	if(radix == usebin)
		PutBin(a);
	else
		Putf(Sprintf("%ld", a));
}

void  Stream64::PutUnsigned(uint32 a) {
	if(radix == usehex)
		PutHex(a);
	else
	if(radix == usebin)
		PutBin(a);
	else
		PutDec(a);
}

void  Stream64::PutDouble(double a) {
	Putf(Sprintf("%.6g", a));
}

Stream64& Stream64::operator<<(bool a) {
	Putf(a ? "true" : "false");
	return *this;
}

void Stream64::LdError() {
	SetError(ERROR_LOADING_FAILED);
	if(style & STRM_THROW)
		throw LoadingError();
}

void Stream64::SerializeRLE(byte *data, dword size)
{
	if(IsError()) return;
	byte *s =   (byte *)data;
	byte *lim = s + size;
	if(IsLoading())
		while(s != lim) {
			if(IsEof() || s > lim) {
				LdError();
				return;
			}
			byte c = Get();
			if(c == 0xcb) {
				c = Get();
				int n = Get();
				if(s + n > lim) {
					LdError();
					return;
				}
				memset(s, c, n);
				s += n;
			}
			else
				*s++ = c;
		}
	else
		while(s < lim) {
			byte  c = *s;
			byte *t = s + 1;
			byte *lm = min(s + 250, lim);
			while(*t == c && t < lm)
				t++;
			if(t >= s + 3 || c == 0xcb) {
				Put(0xcb);
				Put(c);
				Put(byte(t - s));
			}
			else {
				Put(*s);

				if(t == s + 2)
					Put(*s);
			}
			if(IsError()) break;
			s = t;
		}
}

void Stream64::SerializeRaw(byte *data, dword size) {
	if(IsError()) return;
	if(IsLoading()) {
		if(!GetAll(data, size))
			LdError();
	}
	else
		Put(data, size);
}

void Stream64::SerializeRaw(word *data, dword count) {
	SerializeRaw((byte *)data, 2 * count);
}

void Stream64::SerializeRaw(dword *data, dword count) {
	SerializeRaw((byte *)data, 4 * count);
}

void Stream64::Pack(dword& w) {
	if(IsError()) return;
	if(IsLoading()) {
		int q = Get();
		if(q < 0)
			LdError();
		else {
			if(q != 255)
				w = q;
			else
				SerializeRaw(&w, 1);
		}
	}
	else {
		if(w < 255)
			Put(w);
		else {
			Put(255);
			SerializeRaw(&w, 1);
		}
	}
}

void    Stream64::Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f, bool& g, bool& h) {
	if(IsError()) return;
	if(IsLoading()) {
		int f = Get();
		if(f < 0) LdError();
		else {
			a = !!(f & 0x80);
			b = !!(f & 0x40);
			c = !!(f & 0x20);
			d = !!(f & 0x10);
			e = !!(f & 0x08);
			f = !!(f & 0x04);
			g = !!(f & 0x02);
			h = !!(f & 0x01);
		}
	}
	else {
		int f = 0;
		if(a) f |= 0x80;
		if(b) f |= 0x40;
		if(c) f |= 0x20;
		if(d) f |= 0x10;
		if(e) f |= 0x08;
		if(f) f |= 0x04;
		if(g) f |= 0x02;
		if(h) f |= 0x01;
		Put(f);
	}
}

void  Stream64::Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f, bool& g) {
	bool h = false; Pack(a, b, c, d, e, f, g, h);
}

void  Stream64::Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f) {
	bool h = false; Pack(a, b, c, d, e, f, h, h);
}

void  Stream64::Pack(bool& a, bool& b, bool& c, bool& d, bool& e) {
	bool h = false; Pack(a, b, c, d, e, h, h, h);
}

void  Stream64::Pack(bool& a, bool& b, bool& c, bool& d) {
	bool h = false; Pack(a, b, c, d, h, h, h, h);
}

void  Stream64::Pack(bool& a, bool& b, bool& c) {
	bool h = false; Pack(a, b, c, h, h, h, h, h);
}

void  Stream64::Pack(bool& a, bool& b) {
	bool h = false; Pack(a, b, h, h, h, h, h, h);
}

//#must be changed for nonIA32....
Stream64& Stream64::operator%(bool& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream64& Stream64::operator%(char& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream64& Stream64::operator%(signed char& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream64& Stream64::operator%(unsigned char& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream64& Stream64::operator%(short& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream64& Stream64::operator%(unsigned short& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream64& Stream64::operator%(int& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream64& Stream64::operator%(unsigned int& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream64& Stream64::operator%(long& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream64& Stream64::operator%(unsigned long& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream64& Stream64::operator%(float& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream64& Stream64::operator%(double& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream64& Stream64::operator%(int64& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream64& Stream64::operator%(uint64& d)
{
	SerializeRaw((byte *)&d, sizeof(d));
	return *this;
}

Stream64& Stream64::operator%(String& s) {
	if(IsError()) return *this;
	if(IsLoading()) {
		dword len;
		len = Get();
		if(len != 0xff) {
			if(len & 0x80) {
				len &= 0x7f;
				Get(); // reserved for future use... or removal
			}
		}
		else {
			len = GetL();
			if(len & 0x80000000) {
				len &= 0x7fffffff;
				Get(); // reserved for future use... or removal
			}
		}
		if(IsError() || len + GetPos() > GetSize())
			LdError();
		else {
			StringBuffer sb(len);
			SerializeRaw((byte*)~sb, len);
			s = sb;
		}
	}
	else {
		dword len = s.GetLength();
		if(len < 127)
			Put(len);
		else {
			Put(0xff);
			PutL(len);
		}
		SerializeRaw((byte *)~s, len);
	}
	return *this;
}

Stream64& Stream64::operator/(String& s) {
	if(IsError()) return *this;
	dword len = s.GetLength();
	Pack(len);
	if(IsLoading()) {
		SerializeRLE((byte *)s.GetBuffer(len), len);
		s.ReleaseBuffer(len);
	}
	else
		SerializeRLE((byte *)~s, len);
	return *this;
}

Stream64& Stream64::operator%(WString& s) {
	if(IsError()) return *this;
	if(IsLoading()) {
		dword len;
		Pack(len);
		if(IsError() || len + GetPos() > GetSize())
			LdError();
		else {
			WStringBuffer sb(len);
			SerializeRaw((byte*)~sb, len * sizeof(wchar));
			s = sb;
		}
	}
	else {
		dword len = s.GetLength();
		if(len < 0xff)
			Put(len);
		else {
			Put(0xff);
			PutL(len);
		}
		SerializeRaw((byte*)~s, len * sizeof(wchar));
	}
	return *this;
}

Stream64& Stream64::operator/(WString& s) {
	if(IsError()) return *this;
	String h = ToUtf8(s);
	*this / h;
	s = FromUtf8(h);
	return *this;
}

void Stream64::Magic(dword magic) {
	dword a = magic;
	*this % a;
	if(magic != a) LdError();
}

// -------------------------- String stream -----------------------------

void  StringStream64::_Put(const void *d, dword sz) {
	if(ptr + sz >= wrlim)
	{
		dword p = ptr - buffer;
		if(p + sz > (dword)data.GetAlloc())
		{
			data.ReleaseBuffer((int)GetSize());
			buffer = (byte *) data.GetBuffer(p + sz + abs(chunk));
			if(chunk < 0 && chunk > -100000000) chunk += chunk;
		}
		ptr = buffer + p;
	}
	wrlim = buffer + data.GetAlloc();
	rdlim = buffer;
	memcpy(ptr, d, sz);
	ptr += sz;
}

void  StringStream64::_Put(int w) {
	byte h = w;
	_Put(&h, 1);
}

dword StringStream64::_Get(void *data, dword sz) {
	if(dword(ptr - buffer) > size)
		size = ptr - buffer;
	wrlim = buffer;
	rdlim = buffer + size;
	dword read = min(dword(rdlim - ptr), sz);
	memcpy(data, ptr, read);
	ptr += read;
	return read;
}

int  StringStream64::_Get() {
	byte h;
	return _Get(&h, 1) == 1 ? h : -1;
}

int  StringStream64::_Term() {
	byte h;
	_Get(&h, 0);
	return ptr < rdlim ? *ptr : -1;
}

void  StringStream64::Seek(int64 pos) {
	if(int64(ptr - buffer) > size)
		size = ptr - buffer;
	ptr = buffer + min(GetSize(), pos);
}

int64 StringStream64::GetSize() const {
	return max<int64>(size, ptr - buffer);
}

void  StringStream64::SetSize(int64 asize) {
	ASSERT(asize <= 0x7fffffff);
	if(asize > data.GetAlloc()) {
		data.ReleaseBuffer((dword)GetSize());
		dword p = ptr - buffer;
		buffer = (byte *) data.GetBuffer((dword)(asize + chunk));
		ptr = buffer + p;
	}
	size = asize;
	ptr = min(ptr, buffer + size);
	wrlim = rdlim = buffer;
}

void  StringStream64::Close() {
	if(isOpen) data.ReleaseBuffer((dword)GetSize());
	isOpen = false;
	if(data.GetAlloc() - data.GetLength() > 256)
		data.Shrink();
}

String StringStream64::GetResult() const {
	return isOpen ? String(buffer, (dword)GetSize()) : data;
}

bool  StringStream64::IsOpen() const {
	return isOpen;
}

void  StringStream64::Open(const String& adata) {
	if(isOpen) Close();
	data = adata;
	style = STRM_READ|STRM_WRITE|STRM_SEEK|STRM_LOADING;
	ClearError();
	size = data.GetLength();
	buffer = (byte *) data.GetBuffer((dword)size);
	pos = 0;
	ptr = rdlim = wrlim = buffer;
	chunk = -1024;
	isOpen = true;
}

void  StringStream64::Create() {
	Open(String());
	SetStoring();
}

StringStream64::StringStream64() {
	isOpen = false;
	Create();
}

StringStream64::StringStream64(const String& data)
{
	isOpen = false;
	Open(data);
}

StringStream64::~StringStream64()
{
	Close();
}


// -------------------- Memory read-write stream ------------------------

void MemStream64::Seek(int64 pos) {
	ptr = buffer + min<int64>(pos, rdlim - buffer);
}

int64 MemStream64::GetSize() const {
	return rdlim - buffer;
}

dword MemStream64::_Get(void *data, dword size) {
	if(size > dword(rdlim - ptr))
		size = rdlim - ptr;
	memcpy(data, ptr, size);
	ptr += size;
	return size;
}

void  MemStream64::_Put(const void *data, dword size) {
	if(size > dword(wrlim - ptr)) {
		SetError(ERROR_NOT_ENOUGH_SPACE);
		return;
	}
	memcpy(ptr, data, size);
	ptr += size;
}

bool  MemStream64::IsOpen() const {
	return true;
}

MemStream64::MemStream64(void *data, int size) {
	style = STRM_WRITE|STRM_READ|STRM_SEEK|STRM_LOADING;
	ptr = buffer = (byte *) data;
	wrlim = rdlim = buffer + size;
	pos = 0;
}

#ifdef flagSO
MemStream64::~MemStream64() {}
#endif

// ----------------------- Memory read streamer -------------------------

MemReadStream64::MemReadStream64(const void *data, int size) : MemStream64((void *)data, size) {
	style = STRM_READ|STRM_SEEK|STRM_LOADING;
	wrlim = buffer;
}

// ------------------------ Stream64 with Buffer --------------------------

void BufferStream64::Init(dword size) {
	buffer = new byte[buffersize = size];
	ptr = rdlim = wrlim = buffer;
}

void BufferStream64::SetBufferSize(dword size) {
	ASSERT(!IsOpen());
	delete[] buffer;
	Init(size);
}

BufferStream64::BufferStream64(dword bsize) {
	Init(bsize);
}

BufferStream64::~BufferStream64() {
	delete[] buffer;
}

// ------------- Generic read/write block based stream ------------------

BlockStream64::BlockStream64() {}
BlockStream64::~BlockStream64() {}

void BlockStream64::SetWramount() {
	if(wrlim > buffer && dword(ptr - buffer) > wramount) {
		wramount = ptr - buffer;
		if(pos + wramount > file_size)
			file_size = pos + wramount;
	}
}

void BlockStream64::Flush() {
	SetWramount();
	LLOG("Flush file: " << pos << ", " << wramount);
	LLOGHEXDUMP(buffer, min(wramount, (dword)64));
	if(wramount) {
		Write(pos, buffer, wramount);
		wramount = 0;
	}
	pos += ptr - buffer;
	ptr = rdlim = buffer;
}

void BlockStream64::_Put(int c) {
	if(IsError() || !IsOpen()) return;
	SetWramount();
	wrlim = buffer + buffersize;
	if(ptr >= wrlim)
		Flush();
	*ptr++ = c;
}

int BlockStream64::_Term() {
	if(IsError() || !IsOpen()) return -1;
	SetWramount();
	if(rdlim < wrlim)
		rdlim = buffer + wramount;
	wrlim = buffer;
	if(ptr < rdlim)
		return *ptr;
	Flush();
	rdlim = buffer + Read(pos, buffer, buffersize);
	return ptr < rdlim ? *ptr : -1;
}

int  BlockStream64::_Get() {
	int c = _Term();
	if(c >= 0) ptr++;
	return c;
}

void BlockStream64::_Put(const void *data, dword size) {
	if(IsError() || !IsOpen()) return;
	SetWramount();
	wrlim = buffer + buffersize;
	if(ptr + size < wrlim) {
		memcpy(ptr, data, size);
		ptr += size;
		return;
	}
	Flush();
	if(ptr + size < wrlim) {
		memcpy(ptr, data, size);
		ptr += size;
		return;
	}
	Write(pos, data, size);
	pos += size;
	if(pos > file_size)
		file_size = pos;
	rdlim = wrlim = buffer;
}

dword BlockStream64::_Get(void *data, dword size) {
	if(IsError()) return 0;
	dword read = min(size, (dword)(rdlim - ptr));
	memcpy(data, ptr, read);
	ptr += read;
	size -= read;
	if(size)
		if(size < buffersize / 2) {
			_Term();
			int n = min(size, (dword)(rdlim - ptr));
			memcpy((byte *)data + read, ptr, n);
			read += n;
			ptr += n;
		}
		else {
			Flush();
			wrlim = buffer;
			int n = Read(pos, (byte *) data + read, size);
			read += n;
			pos += n;
		}
	return read;
}

void BlockStream64::Seek(int64 apos) {
	if(IsError()) return;
	if(apos > GetSize())
		apos = GetSize();
	SetWramount();
	if(rdlim < buffer + wramount)
		rdlim = buffer + wramount;
	if(apos >= pos && apos < (rdlim - buffer) + pos) {
		ptr = buffer + (apos - pos);
		wrlim = buffer;
	}
	else {
		Flush();
		pos = apos;
	}
}

int64 BlockStream64::GetSize() const {
	if(IsError()) return 0;
	return wrlim > buffer ? max(file_size, ptr - buffer + pos) : file_size;
}

void BlockStream64::SetSize(int64 apos) {
	if(IsError()) return;
	Flush();
	rdlim = wrlim = buffer;
	SetStreamSize(apos);
	if(IsError()) return;
	file_size = apos;
	if(pos > file_size)
		pos = file_size;
}

dword BlockStream64::Read(int64 at, void *ptr, dword size) {
	NEVER();
	return 0;
}

void  BlockStream64::Write(int64 at, const void *data, dword size) {
	NEVER();
}

void  BlockStream64::SetStreamSize(int64 pos) {
	NEVER();
}

void BlockStream64::OpenInit(BlockStream64::OpenMode mode) {
	style = STRM_READ|STRM_SEEK;
	SetLoading();
	mode &= ~SHAREMASK;
	if(mode != READ) {
		style |= STRM_WRITE;
		SetStoring();
	}
	rdlim = wrlim = ptr = buffer;
	wramount = 0;
	ClearError();
	pos = 0;
	if(mode == APPEND) SeekEnd();
}

// ---------------------------- File stream -----------------------------

#ifdef PLATFORM_WIN32

void FileStream64::SetStreamSize(int64 pos) {
	long lo = (dword)pos, hi = (dword)(pos >> 32);
	if(SetFilePointer(handle, lo, &hi, FILE_BEGIN) == 0xffffffff && GetLastError() != NO_ERROR ||
	   !SetEndOfFile(handle)) {
		SetLastError();
	}
}

void FileStream64::SetPos(int64 pos) {
	ASSERT(IsOpen());
	long lo = (dword)pos, hi = (dword)(pos >> 32);
	if(SetFilePointer(handle, lo, &hi, FILE_BEGIN) == 0xffffffff && GetLastError() != NO_ERROR)
		SetLastError();
}

dword FileStream64::Read(int64 at, void *ptr, dword size) {
	ASSERT(IsOpen() && (style & STRM_READ));
	dword n;
	SetPos(at);
	if(IsError()) return 0;
	if(!ReadFile(handle, ptr, size, &n, NULL)) {
		SetLastError();
		return 0;
	}
	return n;
}

void FileStream64::Write(int64 at, const void *ptr, dword size) {
	ASSERT(IsOpen() && (style & STRM_WRITE));
	dword n;
	SetPos(at);
	if(IsError()) return;
	if(!WriteFile(handle, ptr, size, &n, NULL)) {
		SetLastError();
		return;
	}
	if(n != size)
		SetError(ERROR_NOT_ENOUGH_SPACE);
}

FileTime FileStream64::GetTime() const {
	ASSERT(IsOpen());
	FileTime tm;
	GetFileTime(handle, NULL, NULL, &tm);
	return tm;
}

void     FileStream64::SetTime(const FileTime& tm) {
	ASSERT(IsOpen());
	Flush();
	if(!SetFileTime(handle, NULL, NULL, &tm))
		SetLastError();
}

bool FileStream64::Open(const char *name, FileStream64::OpenMode mode) {
	LLOG("Open " << name << " mode: " << mode);
	Close();
	int iomode = mode & ~SHAREMASK;
	handle = CreateFile(name,
		iomode == READ ? GENERIC_READ : GENERIC_READ|GENERIC_WRITE,
		(mode & NOREADSHARE ? 0 : FILE_SHARE_READ)
		| (mode & NOWRITESHARE ? 0 : FILE_SHARE_WRITE)
		| (mode & DELETESHARE ? FILE_SHARE_DELETE : 0),
		NULL,
		iomode == READ ? OPEN_EXISTING : iomode == CREATE ? CREATE_ALWAYS : OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,
		NULL
	);
	if(handle == INVALID_HANDLE_VALUE) {
		LDUMP(GetLastErrorMessage());
		SetError();
		return FALSE;
	}
	dword fsz_lo, fsz_hi;
	fsz_lo = GetFileSize(handle, &fsz_hi);
	if(fsz_lo == 0xffffffff && GetLastError() != NO_ERROR)
		file_size = 0;
	else
		file_size = fsz_lo | (int64(fsz_hi) << 32);
	OpenInit(iomode);
	LLOG("OPEN " << handle);
	return TRUE;
}

void FileStream64::Close() {
	if(!IsOpen()) return;
	Flush();
	LLOG("CLOSE " << handle);
	if(!CloseHandle(handle)) {
		LLOG("CLOSE ERROR");
		LDUMP(GetLastErrorMessage());
		SetLastError();
	}
	handle = INVALID_HANDLE_VALUE;
}

bool FileStream64::IsOpen() const {
	return handle != INVALID_HANDLE_VALUE;
}

FileStream64::FileStream64(const char *filename, FileStream64::OpenMode mode) {
	handle = INVALID_HANDLE_VALUE;
	Open(filename, mode);
}

FileStream64::FileStream64() {
	handle = INVALID_HANDLE_VALUE;
}

FileStream64::~FileStream64() {
	Close();
}

#endif

#ifdef PLATFORM_POSIX

void FileStream64::SetStreamSize(int64 pos)
{
	if(handle < 0) return;
	loff_t cur = lseek64(handle, 0, SEEK_CUR);
	if(cur < 0) {
		SetLastError();
		return;
	}
	loff_t len = lseek64(handle, 0, SEEK_END);
	if(len < 0) {
		SetLastError();
		lseek64(handle, cur, SEEK_SET);
		return;
	}
	while(pos > len) {
		static char buffer[1024];
		int64 diff = pos - len;
		int chunk = (diff > sizeof(buffer) ? sizeof(buffer) : (int)diff);
		if(write(handle, buffer, chunk) != chunk) {
			SetLastError();
			lseek64(handle, cur, SEEK_SET);
			return;
		}
		len += chunk;
	}
	if(pos < len) {
		if(cur > pos)
			lseek64(handle, cur = pos, SEEK_SET);
		if(ftruncate64(handle, pos) < 0)
			SetLastError();
	}
	if(lseek64(handle, cur, SEEK_SET) < 0)
		SetLastError();
}

void FileStream64::SetPos(int64 pos) {
	ASSERT(IsOpen());
	if(lseek64(handle, pos, SEEK_SET) < 0)
		SetLastError();
}

dword FileStream64::Read(int64 at, void *ptr, dword size) {
	ASSERT(IsOpen() && (style & STRM_READ));
	SetPos(at);
	if(IsError()) return 0;
	int n = read(handle, ptr, size);
	if(n < 0) {
		SetLastError();
		return 0;
	}
	return n;
}

void FileStream64::Write(int64 at, const void *ptr, dword size) {
	ASSERT(IsOpen() && (style & STRM_WRITE));
	SetPos(at);
	if(IsError()) return;
	int n = write(handle, ptr, size);
	if(n < 0) {
		SetLastError();
		return;
	}
	if((dword)n != size)
		SetError(ERROR_NOT_ENOUGH_SPACE);
}

FileTime FileStream64::GetTime() const {
	ASSERT(IsOpen());
	struct stat fst;
	fstat(handle, &fst);
	return fst.st_mtime;
}

bool FileStream64::Open(const char *name, FileStream64::OpenMode mode, mode_t tmode) {
	Close();
	int iomode = mode & ~SHAREMASK;
	handle = open(name, iomode == READ ? O_RDONLY :
	                    iomode == CREATE ? O_CREAT|O_RDWR|O_TRUNC :
	                    O_RDWR,
	              tmode);
	if(handle >= 0) {
		file_size = lseek(handle, 0, SEEK_END);
		if(file_size >= 0) {
			OpenInit(mode);
			return true;
		}
	}
	SetLastError();
	return false;
}

void FileStream64::Close() {
	if(!IsOpen()) return;
	Flush();
	if(close(handle) < 0)
		SetLastError();
	handle = -1;
}

bool FileStream64::IsOpen() const {
	return handle != -1;
}

FileStream64::FileStream64(const char *filename, FileStream64::OpenMode mode, mode_t acm) {
	handle = -1;
	Open(filename, mode, acm);
}

FileStream64::FileStream64() {
	handle = -1;
}

FileStream64::~FileStream64() {
	Close();
}

#endif

// --------------------------- Size stream -----------------------

void SizeStream64::Seek(int64 apos) {
	if(ptr - buffer + pos > size)
		size = ptr - buffer + pos;
	pos = apos;
	if(pos > GetSize())
		pos = GetSize();
	ptr = buffer;
}

int64 SizeStream64::GetSize() const {
	return max(ptr - buffer + pos, size);
}

void SizeStream64::SetSize(int64 asize) {
	size = asize;
	if(ptr - buffer + pos > size) {
		ptr = buffer;
		pos = size;
	}
}

void SizeStream64::_Put(const void *, dword sz) {
	wrlim = buffer + buffersize;
	pos += ptr - buffer + sz;
	ptr = buffer;
}

void SizeStream64::_Put(int w) {
	_Put(NULL, 1);
}

bool SizeStream64::IsOpen() const {
	return true;
}

SizeStream64::SizeStream64() : BufferStream64(256) {
	size = 0;
	style = STRM_WRITE|STRM_SEEK;
}

// ------------------------------ Compare stream ----------------------------

CompareStream64::CompareStream64() {
	stream = NULL;
	equal = false;
	size = 0;
}

CompareStream64::CompareStream64(Stream64& astream) {
	stream = NULL;
	Open(astream);
}

void CompareStream64::Open(Stream64& astream) {
	ASSERT(astream.IsOpen());
	Close();
	style = STRM_WRITE|STRM_SEEK;
	stream = &astream;
	size = pos = 0;
	wrlim = buffer + buffersize;
	ptr = buffer;
	equal = true;
}

bool CompareStream64::IsOpen() const {
	return !!stream;
}

int64 CompareStream64::GetSize() const {
	return max(ptr - buffer + pos, size);
}

void CompareStream64::Close() {
	if(!stream) return;
	if(GetPos() > size)
		size = GetPos();
	Flush();
	if(stream->GetSize() != GetSize())
		equal = false;
	stream = NULL;
}

void CompareStream64::SetSize(int64 asize) {
	Flush();
	pos += ptr - buffer;
	ptr = buffer;
	size = asize;
	if(pos > size)
		pos = size;
}

void CompareStream64::Seek(int64 apos) {
	Flush();
	int64 sz = ptr - buffer + pos;
	if(sz > size)
		size = sz;
	pos = apos;
	ptr = buffer;
}

void CompareStream64::Compare(int64 pos, const void *data, dword size) {
	ASSERT(stream);
	if(!size) return;
	Buffer<byte> b(size);
	if(stream->GetPos() != pos)
		stream->Seek(pos);
	if(stream->Get(b, size) != size || memcmp(b.operator const byte *(), data, size))
		equal = false;
}

void CompareStream64::Flush() {
	Compare(pos, buffer, ptr - buffer);
}

void CompareStream64::_Put(const void *data, dword size) {
	wrlim = buffer + buffersize;
	ASSERT(ptr <= wrlim);
	Flush();
	pos += ptr - buffer;
	ptr = buffer;
	byte *b = (byte *) data;
	while(size && equal) {
		int sz = min(size, (dword)DEFAULT_STREAM_BUFFER_SIZE);
		Compare(pos, b, sz);
		pos += sz;
		b += sz;
		size -= sz;
	}
}

void CompareStream64::_Put(int w) {
	byte b = w;
	_Put(&b, 1);
}

Stream64Stream::Stream64Stream(Stream64& stream64, int64 offset64)
: stream64(stream64), offset64(offset64)
{
	style = stream64.GetStyle();
	rdlim = wrlim = ptr = buffer;
	wramount = 0;
	ClearError();
	if(stream64.IsError())
		SetError(stream64.GetError());
	file_size = (dword)min<int64>(stream64.GetSize() - offset64, 0x7ffffffe);
	pos = 0;
}

Stream64Stream::~Stream64Stream()
{
	if(stream64.IsOpen())
		Flush();
}

dword Stream64Stream::Read(dword at, void *ptr, dword size)
{
	stream64.Seek(at + offset64);
	dword count = stream64.Get(ptr, size);
	if(stream64.IsError())
		SetError(stream64.GetError());
	return count;
}

void Stream64Stream::Write(dword at, const void *data, dword size)
{
	int64 at64 = at + offset64;
	if(at64 > stream64.GetSize())
		stream64.SetSize(at64);
	stream64.Seek(at64);
	stream64.Put(data, size);
	if(stream64.IsError())
		SetError(stream64.GetError());
}

void Stream64Stream::SetStreamSize(dword size)
{
	stream64.SetSize(size + offset64);
	if(stream64.IsError())
		SetError(stream64.GetError());
}

bool Stream64Stream::IsOpen() const
{
	return stream64.IsOpen();
}

String LoadStream64(Stream64& in) {
	String s;
	if(in.IsOpen()) {
		int size = (int)min<int64>(in.GetLeft(), 0x7ffffffe); //?? temporary limit
		if(size > 0) {
			in.Get(s.GetBuffer(size), size);
			s.ReleaseBuffer(size);
		}
		if(!in.IsError())
			return s;
	}
	return String::GetVoid();
}

bool SaveStream64(Stream64& out, const String& data) {
	if(!out.IsOpen() || out.IsError()) return false;
	out.Put((const char *)data, data.GetLength());
	out.Close();
	return out.IsOK();
}

int CopyStream64(Stream64& dest, Stream64& src, int count) {
	int block = min(count, 65536);
	Buffer<byte> temp(block);
	int loaded;
	int done = 0;
	while(count > 0 && (loaded = src.Get(temp, min(count, block))) > 0) {
		dest.Put(temp.operator const byte *(), loaded);
		count -= loaded;
		done += loaded;
	}
	return done;
}

void CheckedSerialize(const Callback1<Stream64&> serialize, Stream64& stream)
{
	int64 pos = stream.GetPos();
	stream.Magic(0x61746164);
	serialize(stream);
	stream.Magic(0x00646e65);
	pos = stream.GetPos() - pos;
	stream.Magic((int)pos);
	stream.Magic((int)(pos >> 32));
}

bool Load(Callback1<Stream64&> serialize, Stream64& stream) {
	StringStream64 backup;
	backup.SetStoring();
	serialize(backup);
	ASSERT(!backup.IsError());
	stream.SetLoading();
	stream.LoadThrowing();
	try {
		CheckedSerialize(serialize, stream);
	}
	catch(LoadingError) {
		backup.Seek(0);
		backup.SetLoading();
		serialize(backup);
		ASSERT(!backup.IsError());
		return false;
	}
	return true;
}

bool Store(Callback1<Stream64&> serialize, Stream64& stream) {
	stream.SetStoring();
	CheckedSerialize(serialize, stream);
	return !stream.IsError();
}

String Cfgname(const char *file);

bool LoadFromFile(Callback1<Stream64&> serialize, const char *file) {
	FileIn64 f(Cfgname(file));
	return f ? Load(serialize, f) : false;
}

bool StoreToFile(Callback1<Stream64&> serialize, const char *file) {
	FileOut64 f(Cfgname(file));
	return f ? Store(serialize, f) : false;
}

Stream64& Pack16(Stream64& s, int& i) {
	if(s.IsLoading()) {
		i = (int16) s.GetIW();
		if(i == -32768)
			i = s.GetIL();
	}
	else
		if(i < -32767 || i > 32767) {
			s.PutIW((word)-32768);
			s.PutIL(i);
		}
		else
			s.PutIW((word)i);
	return s;
}

Stream64& Pack16(Stream64& s, int& i1, int& i2) {
	Pack16(s, i1);
	Pack16(s, i2);
	return s;
}

Stream64& Pack16(Stream64& s, int& i1, int& i2, int& i3) {
	Pack16(s, i1, i2);
	Pack16(s, i3);
	return s;
}

Stream64& Pack16(Stream64& s, int& i1, int& i2, int& i3, int& i4) {
	Pack16(s, i1, i2, i3);
	Pack16(s, i4);
	return s;
}

Stream64& Pack16(Stream64& s, int& i1, int& i2, int& i3, int& i4, int& i5) {
	Pack16(s, i1, i2, i3, i4);
	Pack16(s, i5);
	return s;
}

int Stream64Heading(Stream64& stream, int ver, int minver, int maxver, const char* tag)
{
	if(stream.IsLoading() && stream.IsEof() || stream.IsError())
		return Null;
	String text = tag;
	dword len = text.GetLength();
	stream.Pack(len);
	if(stream.IsLoading()) {
		if(stream.IsError() || len != text.GetLength()) {
			stream.SetError();
			return Null;
		}
		String in;
		stream.SerializeRaw((byte *)in.GetBuffer(len), len);
		in.ReleaseBuffer(len);
		if(stream.IsError() || in != text) {
			stream.SetError();
			return Null;
		}
	}
	else
		stream.SerializeRaw((byte *)(const char*)text, len);
	stream / ver;
	if(ver < minver || ver > maxver) {
		stream.SetError();
		return Null;
	}
	return ver;
}
