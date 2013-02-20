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
	
	enum {READING, WRITING};
	inline void SetState(bool b);
public:
	virtual bool  IsOpen() const;
	
	void Reserve(int n);
	int  GetReserved() const            { return bufsize; }
	void AutoResize(bool resize = true) { autoresize = resize; }
	void NoAutoResize()                 { autoresize = false; }
	bool IsAutoResize() const           { return autoresize; }
	
	int GetAvailable() const;
	void Clear();
	
protected:
	int bufsize;
	byte *pptr;
	bool autoresize;
	bool state;
	
public:
	PipeStream(int buffersize = 4096, bool resize = true);
	virtual ~PipeStream();
};

END_UPP_NAMESPACE

#endif
