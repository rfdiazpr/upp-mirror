#include "Core.h"

NAMESPACE_UPP

struct Vector_ {
	void *vector;
	int   items;
	int   alloc;
};

void VectorReAlloc_(void *v_, int newalloc, int sizeofT)
{
	Vector_ *v = (Vector_*)v_;
	ASSERT(newalloc >= v->items);
	ASSERT(v->items >= 0); // Pick semantics broken
	void *newvector = newalloc ? new byte[newalloc * sizeofT] : NULL;
	if(v->vector)
		memcpy(newvector, v->vector, v->items * sizeofT);
	v->vector = newvector;
	v->alloc = newalloc;
}

void VectorReAllocF_(void *v_, int newalloc, int sizeofT)
{
	Vector_ *v = (Vector_*)v_;
	void *prev = v->vector;
	VectorReAlloc_(v, newalloc, sizeofT);
	delete (byte *)prev;
}

void VectorGrow_(void *v_, int sizeofT)
{
	Vector_ *v = (Vector_*)v_;
#ifdef _DEBUG
	VectorReAlloc_(v, max(v->alloc + 1, 2 * v->alloc), sizeofT);
#else
//	VectorReAlloc_(v, max(v->alloc + 1, max(3 * v->alloc / 2, 1 / sizeofT)), sizeofT);
	VectorReAlloc_(v, max(v->alloc + 1, max(2 * v->alloc, 16 / sizeofT)), sizeofT);
#endif
}

void VectorGrowF_(void *v_, int sizeofT)
{
	Vector_ *v = (Vector_*)v_;
	void *prev = v->vector;
	VectorGrow_(v, sizeofT);
	delete (byte *)prev;
}

END_UPP_NAMESPACE
