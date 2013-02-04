#include "InVector.h"

int64 NewInVectorSerial()
{
	static int64 x;
	INTERLOCKED {
		++x;
	}
	return x;
}

thread__ InVectorCacheRecord invector_cache;
