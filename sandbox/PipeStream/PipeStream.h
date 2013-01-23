#ifndef _PipeStream_PipeStream_h_
#define _PipeStream_PipeStream_h_

#include <Core/Core.h>

NAMESPACE_UPP

class PipeStream : public Stream {
protected:
	virtual void  _Put(const void *data, dword size);
	virtual dword _Get(void *data, dword size);
	virtual void  _Put(int w);
	virtual int   _Get();
	virtual int   _Term();
	
public:
	virtual bool  IsOpen() const;
	
	void SetLoading();
	void SetStoring();
	
	void Reserve(int n);
	int  GetReserved() const            { return bufsize; }
	void AutoResize(bool resize = true) { autoresize = resize; }
	void NoAutoResize()                 { autoresize = false; }
	bool IsAutoResize() const           { return autoresize; }
	
	int64 GetLeft();
	void Clear();
	
protected:
	int bufsize;
	byte *pptr;
	int64 len;
	bool autoresize;
	
public:
	PipeStream(int buffersize = 4096, bool resize = true);
	virtual ~PipeStream();
};

END_UPP_NAMESPACE

#endif
