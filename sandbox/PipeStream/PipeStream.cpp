#include "PipeStream.h"

NAMESPACE_UPP

void PipeStream::SetLoading(){
	if(style & STRM_LOADING) return;
	Stream::SetLoading();
	Swap(ptr,pptr);
}

void PipeStream::SetStoring(){
	if(!(style & STRM_LOADING)) return;
	Stream::SetStoring();
	Swap(ptr,pptr);
}

void PipeStream::Reserve(int n){
	ASSERT(n>=len);
	byte* tmp = new byte[n];
	if(buffer){
		if(len>0){
			len = _Get(tmp ,len);
			ptr = tmp;
			pptr = tmp + len;
		}
		delete[] buffer;
	}
	bufsize = n;
	buffer = tmp;
}

dword PipeStream::_Get(void *data, dword size) {
	SetLoading();
	
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
	len -= size;
	return size;
}

void  PipeStream::_Put(const void *data, dword size) {
	if(size > bufsize - len) {
		if(autoresize){
			Reserve(2*(len + size)); //TODO: nicer sizes
		} else {
			SetError(ERROR_NOT_ENOUGH_SPACE);
			return;
		}
	}

	SetStoring();
	dword cont = (dword)(uintptr_t)(buffer + bufsize - ptr);
	if(size > cont) {
		memcpy(ptr, data, cont);
		memcpy(buffer, (byte*)data + cont, size - cont);
		ptr = buffer + size - cont;
	} else {
		memcpy(ptr, data, size);
		ptr += size;
	}
	len += size;
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
	SetLoading();
	if(len)
		return *ptr;
	else
		return -1;
}

bool  PipeStream::IsOpen() const {
	return true;
}

void PipeStream::Clear() {
	len = pos = 0;
	ptr = pptr = buffer;
}

int64 PipeStream::GetLeft() {
	return len;
}

PipeStream::PipeStream(int buffersize, bool resize) : bufsize(buffersize), autoresize(resize) {
	ptr = pptr = buffer = new byte[bufsize];
	style = STRM_WRITE|STRM_READ;
	wrlim = rdlim = NULL;
	len = pos = 0;
}

PipeStream::~PipeStream() {
	delete[] buffer;
}

END_UPP_NAMESPACE

