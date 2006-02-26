#define RawVector RawVector3
#define Vector    Vector3

template <int size>
class RawVector {
protected:
	struct Data {
		byte __[size];
	};

	Data       *vector;
	mutable int items;
	int         alloc;

	int      CalcNewSize(int n)      { return max(alloc + max(alloc, n), 8); }
	void     RawFree();
	Data    *Alloc(int n)            { return (Data *) new byte[n * sizeof(Data)]; }
	void     Realloc(int newalloc);
	void     Expand0(int i);
	void     Expand(int i)           { Chk(); if(i >= alloc) Expand0(i); }
	void     RawInsert(int q, int count);
	void     RawInsertPick(int i, pick_ RawVector<size>& v);

	void     Pick(pick_ RawVector& v);
	void     Chk() const             { ASSERT(items >= 0); }
	void    *RawGet(int i) const     { ASSERT(i >= 0 && i < items); return vector + i; }

	void    *RawAdd()                { Expand(items); return vector + items++; }
	int      RawGetIndex(const void *item) const;

	RawVector()                      { vector = NULL; items = alloc = 0; }

public:
	int      GetCount() const        { Chk(); return items; }
	bool     IsEmpty() const         { Chk(); return items == 0; }
	void     Shrink()                { Realloc(items); }
	void     Reserve(int xtra);
	int      GetAlloc() const        { return alloc; }

	void     Swap(RawVector& b);

	bool     IsPicked() const        { return items < 0; }

	MoveTemplate(RawVector);
};

template <int size>
void RawVector<size>::Realloc(int newalloc)
{
	ASSERT(newalloc >= items);
	Chk();
	if(newalloc == alloc) return;
	Data *newvector = newalloc ? Alloc(newalloc) : NULL;
	if(vector) {
		memcpy(newvector, vector, items * sizeof(Data));
		delete[] (byte *) vector;
	}
	vector = newvector;
	alloc = newalloc;
}

template <int size>
void RawVector<size>::Expand0(int i)
{
	Chk();
	Realloc(CalcNewSize(i + 1));
}

template <int size>
void RawVector<size>::RawFree() {
	if(vector && !IsPicked())
		delete[] (byte *) vector;
}

template <int size>
void RawVector<size>::Pick(pick_ RawVector& v)
{
	vector = v.vector;
	items = v.items;
	alloc = v.alloc;
	v.items = -1;
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
void RawVector<size>::Swap(RawVector<size>& b)
{
	::Swap(items, b.items);
	::Swap(alloc, b.alloc);
	::Swap(vector, b.vector);
}

template <int size>
void RawVector<size>::Reserve(int n)
{
	if(n > alloc)
		Realloc(n);
}

template <int size>
void RawVector<size>::RawInsert(int q, int count)
{
	Chk();
	ASSERT(q >= 0 && q <= items);
	if(!count) return;
	if(items + count > alloc) {
		Data *newvector = Alloc(alloc = CalcNewSize(items + count));
		if(vector) {
			memcpy(newvector, vector, q * sizeof(Data));
			memcpy(newvector + q + count, vector + q, (items - q) * sizeof(Data));
		    delete[] (byte *) vector;
		}
		vector = newvector;
	}
	else
		memmove(vector + q + count, vector + q, (items - q) * sizeof(Data));
	items += count;
}

template <int size>
void RawVector<size>::RawInsertPick(int i, pick_ RawVector<size>& v) {
	Chk();
	v.Chk();
	if(!v.IsEmpty()) {
		RawInsert(i, v.GetCount());
		memcpy(RawGet(i), v.RawGet(0), size * v.GetCount());
	}
	const_cast< RawVector<size>& >(v).RawFree();
	v.items = -1;
}

template <class T>
class Vector : public RawVector<sizeof(T)> {
protected:
	void     Free();

	void     __DeepCopy(const Vector& src);
	
	T&       Get(int i) const        { return *(T*)RawGet(i); }

public:
	T&       Add()                   { return *new(RawAdd()) T; }
	void     Add(const T& x)         { DeepCopyConstruct(RawAdd(), x); }
	void     AddPick(pick_ T& x)     { new(RawAdd()) T(x); }
	const T& operator[](int i) const { return Get(i); }
	T&       operator[](int i)       { return Get(i); }
	int      GetCount() const        { Chk(); return items; }
	bool     IsEmpty() const         { Chk(); return items == 0; }
	void     SetCount(int i);
	void     SetCount(int i, const T& init);
	void     SetCountR(int i);
	void     SetCountR(int i, const T& init);
	void     Clear();
	T&       DoIndex(int i)             { if(i >= items) SetCountR(i + 1); return (*this)[i]; }
	T&       DoIndex(int i, const T& x) { if(i >= items) SetCountR(i + 1, x); return (*this)[i]; }
	void     Shrink()                   { Realloc(items); }
	void     Reserve(int xtra);
	int      GetAlloc() const           { return alloc; }

	void     Set(int i, const T& x, int count = 1);
	void     Remove(int i, int count = 1);
	void     InsertN(int i, int count = 1);
	T&       Insert(int i)           { InsertN(i); return Get(i); }
	void     Insert(int i, const T& x, int count = 1);
	void     Insert(int i, const Vector& x);
	void     Insert(int i, const Vector& x, int offset, int count);
	void     InsertPick(int i, pick_ Vector& x)    { RawInsertPick(i, x); }
	void     Append(const Vector& x)               { Insert(GetCount(), x); }
	void     Append(const Vector& x, int o, int c) { Insert(GetCount(), x, o, c); }
	void     AppendPick(pick_ Vector& x)           { RawInsertPick(GetCount(), x); }
	int      GetIndex(const T& item) const         { return RawGetIndex(&item); }

	void     Drop(int n = 1)         { ASSERT(n <= GetCount()); SetCount(GetCount() - n); }
	T&       Top()                   { ASSERT(GetCount()); return Get(GetCount() - 1); }
	const T& Top() const             { ASSERT(GetCount()); return Get(GetCount() - 1); }
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

// Deep copy
	Vector(const Vector& v, int)     { __DeepCopy(v); }

// Standard container interface
	typedef T        ValueType;
	typedef T       *Iterator;
	typedef const T *ConstIterator;

	ConstIterator    Begin() const          { return (T*)vector; }
	ConstIterator    End() const            { return (T*)vector + items; }
	ConstIterator    GetIter(int i) const   { ASSERT(i >= 0 && i <= items); return (T*)vector + i; }
	Iterator         Begin()                { return (T*)vector; }
	Iterator         End()                  { return (T*)vector + items; }
	Iterator         GetIter(int i)         { ASSERT(i >= 0 && i <= items); return (T*)vector + i; }

// Optimalizations
	friend int  GetCount(ConstIterator ptr, ConstIterator end) { return end - ptr; }
	friend int  GetCount(Iterator ptr, Iterator end)           { return end - ptr; }
	friend void Swap(Vector& a, Vector& b)                     { a.Swap(b); }
	friend void Append(Vector& dst, const Vector& src)         { dst.Append(src); }

	DeepCopyOptionTemplate(Vector);
	MoveableTemplate(Vector);
};

template <class T>
void Vector<T>::Free() {
	if(vector && items >= 0)
		DestroyArray((T *)vector, (T *)vector + items);
	RawFree();
}

template <class T>
void Vector<T>::Clear() {
	if(vector && items >= 0)
		SetCount(0);
	else {
		alloc = items = 0;
		vector = NULL;
	}
}

template <class T>
void Vector<T>::__DeepCopy(const Vector& src) {
	src.Chk();
	items = alloc = src.items;
	if(src.vector) {
		vector = Alloc(alloc);
		DeepCopyConstructArray((T *)vector, (T *)src.vector, (T *)src.vector + items);
	}
	else
		vector = NULL;
}

template <class T>
void Vector<T>::SetCount(int n) {
	Chk();
	ASSERT(n >= 0);
	if(n == items) return;
	if(n < items)
		DestroyArray((T*)vector + n, (T*)vector + items);
	else {
		if(n > alloc) Realloc(n);
		ConstructArray((T*)vector + items, (T*)vector + n);
	}
	items = n;
}

template <class T>
void Vector<T>::SetCount(int n, const T& init) {
	Chk();
	ASSERT(n >= 0);
	if(n == items) return;
	if(n < items)
		DestroyArray((T*)vector + n, (T*)vector + items);
	else {
		if(n > alloc) Realloc(n);
		DeepCopyConstructFill((T*)vector + items, (T*)vector + n, init);
	}
	items = n;
}

template <class T>
void Vector<T>::Reserve(int n) {
	if(n + items > alloc)
		Realloc(n + items);
}

template <class T>
void Vector<T>::SetCountR(int n) {
	Chk();
	if(n + items > alloc)
		Realloc(CalcNewSize(n));
	SetCount(n);
}

template <class T>
void Vector<T>::SetCountR(int n, const T& init) {
	Chk();
	if(n + items > alloc)
		Realloc(CalcNewSize(n));	
	SetCount(n, init);
}

template <class T>
void Vector<T>::Remove(int q, int count) {
	Chk();
	ASSERT(q >= 0 && q <= items - count && count >= 0);
	if(count == 0) return;
	DestroyArray((T*)vector + q, (T*)vector + q + count);
//G++
	memmove((T*)vector + q, (T*)vector + q + count, (items - q - count) * sizeof(T));
//	memmove((T*)vector + q, (T*)vector + q + count, (items - q - count) * sizeof(Data));
	items -= count;
}

template <class T>
void Vector<T>::InsertN(int q, int count) {
	ASSERT(count >= 0);
	RawInsert(q, count);
	ConstructArray((T*) vector + q, (T*)vector + q + count);
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
