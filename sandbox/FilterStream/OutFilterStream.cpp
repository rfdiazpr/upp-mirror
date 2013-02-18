#include "FilterStream.h"

OutputFilterStream::OutputFilterStream()
{
	Init();
}

void OutputFilterStream::Init()
{
	buffer.Alloc(4096);
	wrlim = ~buffer + 4096;
	ptr = ~buffer;
}

OutputFilterStream::~OutputFilterStream()
{
	Close();
}

void OutputFilterStream::Close()
{
	if(buffer) {
		FlushOut();
		End();
		buffer.Clear();
	}
}

void OutputFilterStream::FlushOut()
{
	if(ptr != ~buffer) {
		Filter(~buffer, ptr - ~buffer);
		ptr = ~buffer;
	}
}

void OutputFilterStream::_Put(int w)
{
	FlushOut();
	*ptr++ = w;
}

void OutputFilterStream::_Put(const void *data, dword size)
{
	const byte *p = (const byte *)data;
	for(;;) {
		int n = min(Avail(), size);
		memcpy(ptr, p, n);
		size -= n;
		p += n;
		ptr += n;
		if(size == 0)
			return;
		FlushOut();
	}
}

bool OutputFilterStream::IsOpen() const
{
	return buffer;
}

void OutputFilterStream::Out(const void *ptr, int size)
{
	out->Put(ptr, size);
}
