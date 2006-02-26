void DumpStringStats();

template <class T, class S>
typename AString<T, S>::Data *AString<T, S>::Alloc(int alloc)
{
//	LOG("Alloc " << alloc);
	alloc = (HeapRoundUp((max(alloc, 5) + 1) * sizeof(T) + sizeof(Data)) - sizeof(Data)) / sizeof(T) - 1;
#ifdef STRING_STATS
	StringAlloc[min(dword(sizeof(Data) + sizeof(T) * (alloc + 1)), (dword)63)]++;
#endif
//	LOG("Rounded Alloc " << alloc << ": " << (alloc + 1) * sizeof(T) + sizeof(Data));
	Data *newdata = (Data *) new byte[sizeof(Data) + sizeof(T) * (alloc + 1)];
	newdata->alloc = alloc;
	newdata->length = 0;
	AtomicWrite(newdata->refcount, 1);
	return newdata;
}

template <class T, class S>
typename AString<T, S>::Data *AString<T, S>::Clone(const typename AString<T, S>::Data *data, int newalloc)
{
	Data *newdata = Alloc(newalloc);
	newdata->length = min(data->length, newalloc);
	memcpy(newdata->GetPtr(), data->GetPtr(), sizeof(T) * newdata->length);
	newdata->GetPtr()[newdata->length] = 0;
	return newdata;
}

template <class T, class S>
T *AString<T, S>::CreateEmpty(typename AString<T, S>::Data *data)
{
	AtomicInc(data->refcount);
	return (T *)(data + 1);
}

template <class T, class S>
T *AString<T, S>::Create(int len)
{
	if(!len)
		return S::CreateNull();
	int al = len;
	Data *n = Alloc(len);
	n->length = len;
	T *p = (T *)(n + 1);
	*(p + len) = 0;
	return p;
}

template <class T, class S>
T *AString<T, S>::Create(const T *data, int len)
{
	if(!len)
		return S::CreateNull();
	T *ptr = Create(len);
	memcpy(ptr, data, sizeof(T) * len);
	ptr[len] = 0;
	return ptr;
}

template <class T, class S>
void AString<T, S>::Cat(const T *s, int len)
{
	STRING_STAT_CODE(StringClone);
	STRING_STAT_CODER(StringCatN);
	Chk();
	Data *d = GetData(ptr);
	if(AtomicRead(d->refcount) == 1 && d->length + len <= d->alloc) {
		memcpy(ptr + d->length, s, sizeof(T) * len);
		d->length += len;
		ptr[d->length] = 0;
	}
	else {
		Data *nd = Clone(d, d->length + max(d->length, len));
		ptr = nd->GetPtr();
		memcpy(ptr + d->length, s, sizeof(T) * len);
		d->Release();
		nd->length += len;
		ptr[nd->length] = 0;
	}
}

template <class T, class S>
void AString<T, S>::Cat(int c)
{
	STRING_STAT_CODE(StringClone);
	STRING_STAT_CODER(StringCat1);
	Chk();
	Data *d = GetData(ptr);
	if(AtomicRead(d->refcount) != 1 || d->length >= d->alloc) {
		Data *nd = Clone(d, d->length + max(d->length, 1));
		ptr = nd->GetPtr();
		d->Release();
		d = nd;
	}
	ptr[d->length++] = c;
	ptr[d->length] = 0;
}

template <class T, class S>
void AString<T, S>::Cat(int c, int count)
{
	STRING_STAT_CODE(StringClone);
	STRING_STAT_CODER(StringCatN);
	Chk();
	Data *d = GetData(ptr);
	if(AtomicRead(d->refcount) != 1 || d->length + count > d->alloc) {
		Data *nd = Clone(d, d->length + max(d->length, count));
		ptr = nd->GetPtr();
		d->Release();
		d = nd;
	}
	Fill(ptr + d->length, ptr + d->length + count, (T)c);
	d->length += count;
	ptr[d->length] = 0;
}

template <class T, class S>
void AString<T, S>::Insert(int at, const T *s, int len)
{
	Chk();
	ASSERT(at >= 0 && at <= GetLength());
	Data *d = GetData(ptr);
	if(AtomicRead(d->refcount) == 1 && d->length + len <= d->alloc &&
	   !(s >= ptr && s < ptr + d->length)) {
		memmove(ptr + at + len, ptr + at, sizeof(T) * (d->length - at + 1));
		memcpy(ptr + at, s, sizeof(T) * len);
		d->length += len;
	}
	else {
		Data *nd = Alloc(d->length + max(d->length, len));
		T *nptr = nd->GetPtr();
		memcpy(nptr, ptr, sizeof(T) * at);
		memcpy(nptr + at, s, sizeof(T) * len);
		memcpy(nptr + at + len, ptr + at, sizeof(T) * (d->length - at + 1));
		ptr = nd->GetPtr();
		nd->length += d->length + len;
		d->Release();
	}
}

template <class T, class S>
void AString<T, S>::Insert(int at, int c)
{
	T x = c;
	Insert(at, &x, 1);
}

template <class T, class S>
void AString<T, S>::Remove(int at, int count)
{
	Chk();
	ASSERT(at >= 0 && at + count <= GetLength() && count >= 0);
	Data *d = GetData(ptr);
	if(AtomicRead(d->refcount) != 1) {
		Data *nd = Clone(d, d->length + max(d->length, count));
		ptr = nd->GetPtr();
		d->Release();
		d = nd;
	}
	memmove(ptr + at, ptr + at + count, sizeof(T) * (d->length - at - count + 1));
	d->length -= count;
}

template <class T, class S>
void AString<T, S>::Trim(int at)
{
	Chk();
	ASSERT(at <= GetLength());
	if(at >= GetLength()) return;
	Data *d = GetData(ptr);
	if(AtomicRead(d->refcount) != 1) {
		Data *nd = Clone(d, at);
		ptr = nd->GetPtr();
		d->Release();
		d = nd;
	}
	d->length = at;
	ptr[d->length] = 0;
}

template <class T, class S>
void AString<T, S>::Clear()
{
	Chk();
	Release();
	ptr = S::CreateNull();
}

template <class T, class S>
void AString<T, S>::Shrink()
{
	Chk();
	Data *d = GetData(ptr);
	Data *nd = Clone(d, d->length);
	ptr = nd->GetPtr();
	d->Release();
}

template <class T, class S>
void AString<T, S>::Reserve(int len)
{
	Chk();
	Data *d = GetData(ptr);
	if(len < d->alloc) return;
	Data *nd = Clone(d, len);
	ptr = nd->GetPtr();
	d->Release();
}

template <class T, class S>
T *AString<T, S>::GetBuffer(int len)
{
	ASSERT(len >= 0);
	Data *d = GetData(ptr);
	if(AtomicRead(d->refcount) != 1 || len >= d->alloc) {
		Data *nd = Clone(d, len);
		d->Release();
		d = nd;
		ptr = d->GetPtr();
	}
#ifdef _DEBUG
	AtomicWrite(d->refcount, 0);
#endif
	return ptr;
}

template <class T, class S>
void AString<T, S>::ReleaseBuffer(int len)
{
	Data *d = GetData(ptr);
	if(len < 0) len = strlen__(ptr);
	ASSERT(len <= d->alloc && d->refcount == 0);
	ptr[len] = 0;
	d->length = len;
#ifdef _DEBUG
	AtomicWrite(d->refcount, 1);
#endif
}

template <class T, class S>
void AString<T, S>::Assign(const AString<T, S>& s)
{
	STRING_STAT_CODE(StringAssign);
	Chk();
	s.Retain();
	Release();
	ptr = s.ptr;
}

template <class T, class S>
void AString<T, S>::Assign(const T *s, int len)
{
	Chk();
	T *q = Create(s, len);
	Release();
	ptr = q;
}

template <class T, class S>
void AString<T, S>::DeepAssign(const AString<T, S>& s)
{
	Chk();
	T *q = Create(s.ptr, s.GetLength());
	Release();
	ptr = q;
}

template <class T, class S>
void AString<T, S>::Set(int i, int chr)
{
	ASSERT(i >= 0 && i < GetLength());
	Data *d = GetData(ptr);
	if(AtomicRead(d->refcount) != 1) {
		Data *nd = Clone(d, d->length + max(d->length, GetCount()));
		ptr = nd->GetPtr();
		d->Release();
		d = nd;
	}
	ptr[i] = chr;
}

template <class T, class S>
T *AString<T, S>::Mid(int from, int count) const
{
	Chk();
	int l = GetLength();
	if(from > l) from = l;
	if(from < 0) from = 0;
	if(count < 0)
		count = 0;
	if(from + count > l)
		count = l - from;
	if(from == 0 && count == l) {
		Retain();
		return ptr;
	}
	else
		return Create(ptr + from, count);
}

template <class T, class S>
T *AString<T, S>::Mid(int from) const
{
	Chk();
	return Mid(from, GetLength() - from);
}

template <class T, class S>
T *AString<T, S>::Right(int count) const
{
	Chk();
	return Mid(GetLength() - count);
}

template <class T, class S>
T *AString<T, S>::Left(int count) const
{
	Chk();
	return Mid(0, count);
}

template <class T, class S>
int AString<T, S>::Find(int chr, int from) const
{
	ASSERT(from >= 0 && from <= GetLength());
	const T *e = End();
	for(const T *s = ptr + from; s < e; s++)
		if(*s == chr)
			return s - ptr;
	return -1;
}

template <class T, class S>
int AString<T, S>::ReverseFind(int chr, int from) const
{
	const T *s = ptr + from;
	while(s >= ptr)
		if(*--s == chr)
			return s - ptr;
	return -1;
}
/*
int AString::Find(const char *s, int len, int from)
{
	const char *p = ptr + from;
	int l = GetLength() - len;
	if(l < 0)
		return -1;
	const char *e = ptr + l;
	len *= sizeof(T);
	while(p <= e) {
		if(memcmp(s, p, len) == 0)
			return p - ptr;
		p++;
	}
	return -1;
}
*/
template <class T, class S>
bool AString<T, S>::IsEqual(const T *p, int len) const
{
	if(len != GetLength()) return false;
	if(len == 0 || p == ptr) return true;
	return memcmp(ptr, p, len * sizeof(T)) == 0;
}

template <class T, class S>
AString<T, S>::AString(int chr, int count)
{
	ptr = Create(count);
	Fill(ptr, ptr + count, (T)chr);
}

template <class T, class S>
void AStringBuffer<T, S>::Init(int len)
{
	begin = S::Create(len);
	Reads();
}

template <class T, class S>
void AStringBuffer<T, S>::Init(S& text, int alloc)
{
	typename S::Data *d = S::GetData(text.ptr);
	if(AtomicRead(d->refcount) != 1 || d->alloc < alloc) {
		typename S::Data *nd = S::Clone(d, alloc);
		d->Release();
		begin = nd->GetPtr();
	}
	else
		begin = text.ptr;
	text.ptr = S::CreateNull();
	Reads();
}

template <class T, class S>
AStringBuffer<T, S>::AStringBuffer()
{
	begin = S::CreateNull();
	Reads();
}

template <class T, class S>
void AStringBuffer<T, S>::Clear()
{
	Free();
	begin = S::CreateNull();
	Reads();
}

template <class T, class S>
void AStringBuffer<T, S>::Realloc(int newalloc)
{
	typename S::Data *d = S::GetData(begin);
	d->length = end - begin;
	typename S::Data *nd = S::Clone(d, newalloc);
	d->Release();
	begin = nd->GetPtr();
	Reads();
}

template <class T, class S>
void AStringBuffer<T, S>::Expand()
{
	typename S::Data *d = S::GetData(begin);
	Realloc(max(8, 2 * (int)(intptr_t)(alloc - begin)));
}

template <class T, class S>
void AStringBuffer<T, S>::Expand(int len)
{
	typename S::Data *d = S::GetData(begin);
	Realloc(max(8, max((int)(intptr_t)(alloc - begin + len), 2 * (int)(intptr_t)(alloc - begin))));
}

template <class T, class S>
T *AStringBuffer<T, S>::SetLength(int len)
{
	if(alloc - begin < len)
		Realloc(len);
	end = begin + len;
	return begin;
}

template <class T, class S>
T *AStringBuffer<T, S>::Get()
{
	int l = GetLength();
	begin[l] = 0;
	S::GetData(begin)->length = l;
	T *p = begin;
	begin = S::CreateNull();
	Reads();
	return p;
}

template <class T, class S>
void AStringBuffer<T, S>::Set(S& s)
{
	Free();
	Init(s, s.GetLength());
}

template <class T, class S>
AStringBuffer<T, S>::~AStringBuffer()
{
	Free();
}
