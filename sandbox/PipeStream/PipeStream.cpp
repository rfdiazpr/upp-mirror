#include "PipeStream.h"

NAMESPACE_UPP

void PipeStream::SetState(bool b){
	if(state != b) {
		Swap(ptr,pptr);
		state = b;
	}
}

void PipeStream::Reserve(int n){
	if (n == bufsize)
		return;
	SetState(READING);
	int len = GetAvailable();
	ASSERT(n >= len && n > 0);

	byte* tmp = new byte[n];
	if(len>0)
		_Get(tmp ,len);
	delete[] buffer;

	bufsize = n;
	ptr = wrlim = buffer = tmp;
	pptr = rdlim = buffer + len;
}

dword PipeStream::_Get(void *data, dword size) {
	if (size <= 0) return 0;
	SetState(READING);
	int len = GetAvailable();
	
	if(size > len)
		size = len;
	
	dword cont = (dword)(intptr_t)(buffer + bufsize - ptr);
	if(size > cont){
		memcpy(data, ptr, cont);
		memcpy((byte*)data + cont, buffer, size - cont);
		ptr = buffer + size - cont;
	} else {
		memcpy(data, ptr, size);
		ptr += size;
	}
	rdlim = ptr>pptr ? buffer+bufsize : pptr;
	wrlim = buffer;
	return size;
}

void  PipeStream::_Put(const void *data, dword size) {
	int len = GetAvailable();
	
	if((int)size > bufsize - len - 1) {
		if(autoresize){
			Reserve(2*(len + size)); //TODO: do we want nicer sizes?
		} else {
			SetError(ERROR_NOT_ENOUGH_SPACE);
			return;
		}
	}
	
	SetState(WRITING);
	dword cont = (dword)(uintptr_t)(buffer + bufsize - ptr);
	if(size > cont) {
		memcpy(ptr, data, cont);
		memcpy(buffer, (byte*)data + cont, size - cont);
		ptr = buffer + size - cont;
	} else {
		memcpy(ptr, data, size);
		ptr += size;
	}
	wrlim = ptr>pptr ? buffer+bufsize : pptr-1;
	rdlim = buffer;
}

void PipeStream::_Put(int w) {
	byte b = w;
	_Put(&b, 1);
}

int PipeStream::_Get() {
	byte b;
	_Get(&b, 1);
	return b;
}

int PipeStream::_Term() {
	if(GetAvailable()==0)
		return -1;
	SetState(READING);
	return *ptr;
}

bool  PipeStream::IsOpen() const {
	return true;
}

void PipeStream::Clear() {
	pos = 0;
	ptr = pptr = rdlim = buffer;
	wrlim = buffer + bufsize;
	SetState(WRITING);
}

int PipeStream::GetAvailable() const {
	if(state==READING)
		return ptr<=pptr ? pptr-ptr : bufsize-(ptr-pptr);
	else
		return ptr<pptr ? bufsize-(pptr-ptr) : ptr-pptr;
}

PipeStream::PipeStream(int buffersize, bool resize) : bufsize(buffersize), autoresize(resize) {
	rdlim = ptr = pptr = buffer = new byte[bufsize];
	wrlim = buffer + bufsize;
	state = WRITING;
	style = STRM_WRITE|STRM_READ;
	pos = 0;
}

PipeStream::~PipeStream() {
	delete[] buffer;
}

END_UPP_NAMESPACE

