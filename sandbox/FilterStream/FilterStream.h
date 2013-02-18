#ifndef _FilterStream_FilterStream_h_
#define _FilterStream_FilterStream_h_

#include <Core/Core.h>

using namespace Upp;

class InputFilterStream : public Stream {
public:
	virtual   bool  IsOpen() const;

protected:
	virtual   int   _Term();
	virtual   int   _Get();
	virtual   dword _Get(void *data, dword size);

	Vector<byte> buffer;

	void   Fetch(int size);
	void   Init();
	dword  Avail();

public:
	Stream                      *in;
	Callback2<const void *, int> Filter;
	Callback                     End;
	void                         Out(const void *ptr, int size);
	
	InputFilterStream();
};

#endif
