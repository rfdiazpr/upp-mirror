#include "InVector.h"

int64 NewInVectorSerial()
{
	static int64 x;
	INTERLOCKED {
		++x;
	}
	return x;
}

thread__ int64 invector_cache_serial_;
thread__ int   invector_cache_blki_;
thread__ int   invector_cache_offset_;
thread__ int   invector_cache_end_;
