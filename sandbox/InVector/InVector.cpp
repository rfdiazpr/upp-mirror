#include "InVector.h"

int64 NewInVectorSerial()
{
	static int64 x;
	INTERLOCKED {
		++x;
	}
	return x;
}


#ifdef USECACHE
thread__ InVectorCacheRecord invector_cache[6];
thread__ int invector_cachei;
#endif

#ifdef USECACHE2
thread__ InVectorCacheRecord invector_cache;
#endif