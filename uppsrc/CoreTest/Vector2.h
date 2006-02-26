#define RawVector RawVector2
#define Vector    Vector2

template <int size>
class RawVector {
protected:
	struct Data {
		byte __[size];
	};

	Data       *vector;
	Data       *lim;
	Data       *end;
	int         items;

	void     RawFree();
	Data    *Alloc(int n)            { return (Data *) new byte[n * sizeof(Data)]; }
	void     Realloc(int newalloc);
	void     Expand();

	void     Pick(pick_ RawVector& v);
	void     SetPicked(pick_ RawVector& v) {
		RawVector& pick = (RawVector&)(v);
		pick.items = -1;
		pick.vector = NULL;
	}
	void     Chk() const             { ASSERT(items >= 0); }

	void    *RawAdd()                { Chk(); if(end >= lim) Expand();
		                               items++; return end++; }
	void     RawInsert(int q, int count);
	void     RawInsertPick(int i, pick_ RawVector<size>& v);

	int      RawGetIndex(const void *item) const;

	RawVector()                      { vector = lim = end = NULL; items = 0; }

	void     RawReserve(int xtra);
	void     RawSwap(RawVector& b);

public:
	MoveTemplate(RawVector);
};

template <int size>
void RawVector<size>::Realloc(int newalloc)
{
	ASSERT(newalloc >= items);
	Chk();
	size_t alloc = lim - vector;
	if(newalloc == alloc) return;
	Data *newvector = newalloc ? Alloc(newalloc) : NULL;
	if(vector) {
		memcpy(newvector, vector, items * sizeof(Data));
		delete[] (byte *) vector;
	}
	vector = newvector;
	lim = vector + newalloc;
	end = vector + items;
}

template <int size>
void RawVector<size>::Expand()
{
	Chk();
	Realloc(max(2 * (lim - vector), 8));
}

template <int size>
void RawVector<size>::RawFree() {
	if(vector)
		delete[] (byte *) vector;
}

template <int size>
void RawVector<size>::Pick(pick_ RawVector& v)
{
	vector = v.vector;
	items = v.items;
	alloc = v.alloc;
	SetPicked(v);
}

template <int size>
int  RawVector<size>::RawGetIndex(const void *item) const
{
	Chk();
	if(vector == NULL) return -1;
	int n = (const Data *)item - vector;
	return n >= 0 && n < items ? n : -1;
}

template <int size>
void RawVector<size>::RawSwap(RawVector<size>& b)
{
	::Swap(vector, b.vector);
	::Swap(lim, b.lim);
	::Swap(end, b.end);
	::Swap(items, b.items);
}

template <int size>
void RawVector<size>::RawReserve(int n)
{
	if(n > lim - vector)
		Realloc(n);
}

template <int size>
void RawVector<size>::RawInsert(int q, int count)
{
	Chk();
	ASSERT(q >= 0 && q <= items);
	if(!count) return;
	if(end + count > lim) {
		size_t alloc = lim - vector + max(lim - vector, count);
		Data *newvector = Alloc(alloc);
		if(vector) {
			memcpy(newvector, vector, q * sizeof(Data));
			memcpy(newvector + q + count, vector + q, (items - q) * sizeof(Data));
		    delete[] (byte *) vector;
		}
		vector = newvector;
		lim = newvector + alloc;
	}
	else
		memmove(vector + q + count, vector + q, (items - q) * sizeof(Data));
	items += count;
	end = vector + items;
}

template <int size>
void RawVector<size>::RawInsertPick(int i, pick_ RawVector<size>& v) {
	Chk();
	v.Chk();
	if(v.items) {
		RawInsert(i, v.items);
		memcpy(vector + i, v.vector, size * v.items);
	}
	const_cast< RawVector<size>& >(v).RawFree();
	SetPicked(v);
}

template <class T>
class Vector : public RawVector<sizeof(T)> {
protected:
	void     Free();

	void     __DeepCopy(const Vector& src);
	
	T&       Get(int i) const        { ASSERT(i >= 0 && i < items); return (T&)vector[i]; }

public:
	T&       Add()                   { return *new(RawAdd()) T; }
	void     Add(const T& x)         { DeepCopyConstruct(RawAdd(), x); }
	void     AddPick(pick_ T& x)     { PickConstruct(*(T*)RawAdd(), x); }
	const T& operator[](int i) const { return Get(i); }
	T&       operator[](int i)       { return Get(i); }
	int      GetCount() const        { Chk(); return items; }
	bool     IsEmpty() const         { Chk(); return items == 0; }
	void     SetCount(int i);
	void     SetCount(int i, const T& init);
	void     SetCountR(int i);
	void     SetCountR(int i, const T& init);
	void     Clear();
	T&       DoIndex(int i)             { if(i >= items) SetCountR(i + 1); return Get(i); }
	T&       DoIndex(int i, const T& x) { if(i >= items) SetCountR(i + 1, x); return Get(i); }

	void     Shrink()                   { Realloc(items); }
	void     Reserve(int n)             { RawReserve(n); }
	int      GetAlloc() const           { return lim - vector; }

	void     Set(int i, const T& x, int count = 1);
	void     Remove(int i, int count = 1);
	void     InsertN(int i, int count = 1);
	T&       Insert(int i)                         { InsertN(i); return Get(i); }
	void     Insert(int i, const T& x, int count = 1);
	void     Insert(int i, const Vector& x);
	void     Insert(int i, const Vector& x, int offset, int count);
	void     InsertPick(int i, pick_ Vector& x)    { RawInsertPick(i, x); }
	void     Append(const Vector& x)               { Insert(GetCount(), x); }
	void     Append(const Vector& x, int o, int c) { Insert(GetCount(), x, o, c); }
	void     AppendPick(pick_ Vector& x)           { RawInsertPick(GetCount(), x); }
	int      GetIndex(const T& item) const         { return RawGetIndex(&item); }

	void     Drop(int n = 1)         { ASSERT(n <= GetCount()); end -= n; items -= n; }
	T&       Top()                   { ASSERT(GetCount()); return *(T *)(end - 1); }
	const T& Top() const             { ASSERT(GetCount()); return *(T *)(end - 1); }
	T        Pop()                   { T h = Top(); Drop(); return h; }

	operator T*()                    { return (T*)vector; }
	operator const T*() const        { return (T*)vector; }

	Vector&  operator<<(const T& x)  { Add(x); return *this; }
	Vector&  operator|(pick_ T& x)   { AddPick(x); return *this; }

	void     Serialize(Stream& s)    { StreamContainer(s, *this); }

	Vector()                         {}
	~Vector()                        { AssertMoveable<T>(); Free(); }

// Pick assignment & copy. Picked source can only Clear(), ~Vector(), operator=, operator <<=
	Vector(pick_ Vector& v)          { Pick(v); }
	void operator=(pick_ Vector& v)  { Free(); Pick(v); }
	bool IsPicked() const            { return items < 0; }

// Deep copy
	Vector(const Vector& v, int)     { __DeepCopy(v); }

// Standard container interface
	typedef T        ValueType;
	typedef T       *Iterator;
	typedef const T *ConstIterator;

	ConstIterator    Begin() const          { return (T*)vector; }
	ConstIterator    End() const            { return (T*)end; }
	ConstIterator    GetIter(int i) const   { ASSERT(i >= 0 && i <= items); return (T*)vector + i; }
	Iterator         Begin()                { return (T*)vector; }
	Iterator         End()                  { return (T*)end; }
	Iterator         GetIter(int i)         { ASSERT(i >= 0 && i <= items); return (T*)vector + i; }

// Optimizations
	friend int  GetCount(ConstIterator ptr, ConstIterator end) { return end - ptr; }
	friend int  GetCount(Iterator ptr, Iterator end)           { return end - ptr; }
	friend void Swap(Vector& a, Vector& b)                     { a.RawSwap(b); }
	friend void Append(Vector& dst, const Vector& src)         { dst.Append(src); }

	DeepCopyOptionTemplate(Vector);
	MoveableTemplate(Vector);
};

template <class T>
void Vector<T>::Free() {
	if(vector)
		DestroyArray((T *)vector, (T *)end);
	RawFree();
}

template <class T>
void Vector<T>::Clear() {
	if(vector)
		SetCount(0);
	else {
		items = 0;
		vector = end = lim = NULL;
	}
}

template <class T>
void Vector<T>::__DeepCopy(const Vector& src) {
	src.Chk();
	items = src.items;
	if(src.vector) {
		vector = Alloc(items);
		end = lim = vector + items;
		DeepCopyConstructArray((T *)vector, (T *)src.vector, (T *)src.end);
	}
	else {
		vector = end = lim = NULL;
	}
}

template <class T>
void Vector<T>::SetCount(int n) {
	Chk();
	ASSERT(n >= 0);
	if(n == items) return;
	if(n < items)
		DestroyArray((T*)vector + n, (T*)end);
	else {
		if(n > lim - vector) Realloc(n);
		ConstructArray((T*)end, (T*)vector + n);
	}
	items = n;
	end = vector + items;
}

template <class T>
void Vector<T>::SetCount(int n, const T& init) {
	Chk();
	ASSERT(n >= 0);
	if(n == items) return;
	if(n < items)
		DestroyArray((T*)vector + n, (T*)end);
	else {
		if(n > alloc) Realloc(n);
		DeepCopyConstructFill((T*)end, (T*)vector + n, init);
	}
	items = n;
	end = vector + items;
}

template <class T>
void Vector<T>::SetCountR(int n) {
	Chk();
	if(end + n > lim)
		Realloc(lim - vector + max(lim - vector, n));
	SetCount(n);
}

template <class T>
void Vector<T>::SetCountR(int n, const T& init) {
	Chk();
	if(end + n > lim)
		Realloc(lim - vector + max(lim - vector, n));
	SetCount(n, init);
}

template <class T>
void Vector<T>::Remove(int q, int count) {
	Chk();
	ASSERT(q >= 0 && q <= items - count && count >= 0);
	if(count == 0) return;
	DestroyArray((T*)vector + q, (T*)vector + q + count);
	memmove((T*)vector + q, (T*)vector + q + count, (items - q - count) * sizeof(T));
	items -= count;
	end -= count;
}

template <class T>
void Vector<T>::InsertN(int q, int count) {
	ASSERT(count >= 0);
	RawInsert(q, count);
	ConstructArray((T*)vector + q, (T*)vector + q + count);
}

template <class T>
void Vector<T>::Insert(int q, const T& x, int count) {
	if(!count) return;
	RawInsert(q, count);
	DeepCopyConstructFill((T*)vector + q, (T*)vector + q + count, x);
}

template <class T>
void Vector<T>::Insert(int q, const Vector& x, int offset, int count) {
	if(!count) return;
	ASSERT(offset >= 0 && count >= 0 && offset + count <= x.GetCount());
	RawInsert(q, count);
	DeepCopyConstructArray((T*)vector + q, (T*)x.vector + offset,
	                       (T*)x.vector + offset + count);
}

template <class T>
void Vector<T>::Insert(int q, const Vector& x) {
	if(!x.GetCount()) return;
	Insert(q, x, 0, x.GetCount());
}

template <class T>
void Vector<T>::Set(int i, const T& x, int count) {
	Chk();
	ASSERT(i >= 0 && count >= 0);
	if(count == 0) return;
	DoIndex(i + count - 1);
	Fill((T*)vector + i, (T*)vector + i + count, x);
}

#undef Vector
#undef RawVector
