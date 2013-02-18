#include "FilterStream.h"

InputFilterStream::InputFilterStream()
{
	Init();
}

void InputFilterStream::Init()
{
	static byte h[1];
	ptr = rdlim = h;
}

dword InputFilterStream::Avail()
{
	return dword(rdlim - ptr);
}

bool InputFilterStream::IsOpen() const
{
	return in->IsOpen();
}

int InputFilterStream::_Term()
{
	if(ptr == rdlim)
		Fetch(1);
	return ptr == rdlim ? -1 : *ptr;
}

int InputFilterStream::_Get()
{
	if(ptr == rdlim)
		Fetch(1);
	return ptr == rdlim ? -1 : *ptr++;
}

dword InputFilterStream::_Get(void *data, dword size)
{
	byte *p = (byte *)data;
	int nn = 0;
	for(int pass = 0; size && pass < 2; pass++) {
		int n = min(size, Avail());
		memcpy(p, ptr, n);
		size -= n;
		p += n;
		ptr += n;
		nn += n;
		if(size)
			Fetch(size);
	}
	return nn;
}

void InputFilterStream::Out(const void *ptr, int size)
{
	int l = buffer.GetCount();
	buffer.SetCount(l + size);
	memcpy(buffer.Begin() + l, ptr, size);
}

void InputFilterStream::Fetch(int size)
{
	buffer.Clear();
	Buffer<byte> b(4096);
	while(buffer.GetCount() < size) {
		int n = in->Get(~b, 4096);
		if(n == 0) {
			End();
			break;
		}
		Filter(~b, n);
	}
	ptr = buffer.Begin();
	rdlim = buffer.End();
}
