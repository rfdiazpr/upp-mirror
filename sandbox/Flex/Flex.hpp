#define FTIMING(x) // RTIMING(x)
#define LLOG(x)    // LOG(x)
#define LDUMP(x)   // DUMP(x)

template <class T>
void Flex<T>::Destroy(int from, int count)
{
	DestroyArray(data, data + count);
}

template <class T>
void Flex<T>::Free()
{
	if(data) {
		DestroyArray(data, data + count);
		delete[] (byte *)data;
		delete offset;
		data = NULL;
	}
}

template <class T>
void Flex<T>::Init()
{
	SHIFT = sizeof(T) < 16 ? 6 : 5;
	NBLK = 1 << SHIFT;
	MASK = NBLK - 1;
	blk_items = blk_count = 0;
	data = NULL;
	static int zero = 0;
	offset = &zero;
	alloc = count = 0;
}

template <class T>
void Flex<T>::Clear()
{
	Free();
	Init();
}

template <class T>
void Flex<T>::Expand(int n)
{
	FTIMING("Expand");
	unsigned ns = count + n;
	ns = ns + (ns >> 1);
	alloc = max(max(1, int(16 / sizeof(T))), (int)ns);
	T *newdata = (T *) new byte[sizeof(T) * alloc];
	int rawsz = count * sizeof(T);
	int *newoffset;
	const unsigned growk = 2;
	if(5 * blk_count > NBLK) {
		T *t = newdata;
		for(int i = 0; i < blk_count; i++) {
			int off = offset[i];
			T *bpos = data + (i << SHIFT);
			int sz = NBLK - off;
			memcpy(t, bpos + off, sz * sizeof(T));
			t += sz;
			memcpy(t, bpos, off * sizeof(T));
			t += off;
		}
		memcpy(t, data + blk_items, (count - blk_items) * sizeof(T));
		do {
			SHIFT++;
			blk_count = (unsigned)count >> SHIFT;
		}
		while(growk * blk_count > NBLK * sizeof(T));
		NBLK = 1 << SHIFT;
		MASK = NBLK - 1;
		blk_items = blk_count << SHIFT;
		newoffset = new int[(alloc >> SHIFT) + 1];
		for(int i = 0; i <= blk_count; i++)
			newoffset[i] = 0;
	}
	else {
		memcpy(newdata, data, rawsz);
		newoffset = new int[(alloc >> SHIFT) + 1];
		memcpy(newoffset, offset, ((count >> SHIFT) + 1) * sizeof(int));
	}
	if(data) {
		delete[] offset;
		delete[] (byte *) data;
	}
	data = newdata;
	offset = newoffset;
}

template <class T>
T *Flex<T>::RawInsert(int i)
{
	Move(i + 1, i, count - i);
	count++;
	return &data[i];
}

template <class T>
void Flex<T>::RawCopy(T& dst, const T& src)
{
	*reinterpret_cast<Data_S_<sizeof(T)> *>(&dst) = *reinterpret_cast<const Data_S_<sizeof(T)> *>(&src);
}

template <class T>
void Flex<T>::Insert(int i, const T& x)
{
	ASSERT(i >= 0 && i <= count);
	if(count >= alloc)
		Expand(count + 1);
	if(i >= blk_items)
		DeepCopyConstruct(RawInsert(i), x);
	else {
		T *p = RawInsert(blk_items);
		int tn = i >> SHIFT;
		int bpos = blk_items - NBLK;
		int ii = blk_count - 1;
		{
			FTIMING("Rotate");
			while(ii > tn) {
				offset[ii] = (offset[ii] + NBLK - 1) & MASK;
				T *q = &data[bpos + offset[ii]];
				RawCopy(*p, *q);
				p = q;
				--ii;
				bpos -= NBLK;
			}
		}
		FTIMING("Insert");
		int ex = (offset[ii] + NBLK - 1) & MASK;
		RawCopy(*p, data[bpos + ex]);
		int boff = offset[ii];
		int bi = (i + boff) & MASK;
#if 1
		if(((bi + NBLK - boff) & MASK) < (NBLK >> 1)) {
			offset[ii] = ex;
			bi = (bi + NBLK - 1) & MASK;
			if(bi < ex) {
				Move(bpos + ex, bpos + ex + 1, NBLK - ex - 1);
				RawCopy(data[bpos + NBLK - 1], data[bpos]);
				Move(bpos, bpos + 1, bi);
				LLOG("Copy count down A " << NBLK - boff - 1 + bi);
			}
			else {
				Move(bpos + ex, bpos + ex + 1, bi - ex);
				LDUMP(NBLK);
				LDUMP(i);
				LDUMP(bpos);
				LDUMP(bi);
				LDUMP(boff);
				LDUMP(bpos + boff);
				LLOG("Copy count down B " << bi - boff - 1);
			}
		}
		else
#endif
		{
			if(bi > ex) {
				Move(bpos + 1, bpos, ex);
				RawCopy(data[bpos], data[bpos + NBLK - 1]);
				Move(bpos + bi + 1, bpos + bi, NBLK - 1 - bi);
				LLOG("Copy count up A " << ex + NBLK - 1 - bi);
			}
			else {
				Move(bpos + bi + 1, bpos + bi, ex - bi);
				LLOG("Copy count up B " << ex - bi);
			}
		}
		DeepCopyConstruct(&data[bpos + bi], x);
	}
	if(count > blk_items + NBLK) {
		blk_items += NBLK;
		blk_count++;
		offset[blk_count] = 0;
	}
}

/*
template <class T>
void Flex<T>::Remove(int i, const T& x)
{
	ASSERT(i >= 0 && i <= count);
	if(i >= blk_items)
		RawRemove(i);
	else {
		int tn = i >> SHIFT;

	}
	if(count < blk_items) {
	}
}
*/

template<class T> template <class Less>
inline int Flex<T>::LBound(const T& x, int l, int h, const Less& less) const
{
	while(l < h) {
		int mid = unsigned(l + h) >> 1;
		if(less(data[mid], x))
			l = mid + 1;
		else
			h = mid;
	}
	return l;
}


template <class T> template<class Less>
int Flex<T>::FindLowerBound0(const T& x, const Less& less) const
{
	int l = 0;
	int h = blk_count + 1;
	while(l < h) {
		int mid = unsigned(l + h) >> 1;
		if(less(data[(mid << SHIFT) + offset[mid]], x))
			l = mid + 1;
		else
			h = mid;
	}
	if(l == blk_count + 1)
		return LBound(x, blk_items, GetCount(), less);
	if(l == 0)
		return 0;
	l--;
	int bpos = l << SHIFT;
	int off = bpos + offset[l];
	if(less(data[bpos + NBLK - 1], x))
		return LBound(x, bpos, off, less) + bpos + NBLK - off;
	else
		return LBound(x, off, bpos + NBLK, less) - off + bpos;
}

#if 0
template <class T> template<class Less>
int Flex<T>::FindLowerBound0(const T& x, const Less& less) const
{
	
	int l = 0;
	int h = blk_count;
	DUMP(x);
	while(l < h) {
		int mid = unsigned(l + h) >> 1;
		DDUMP(l);
		DDUMP(h);
		DUMP(mid);
		DUMP(offset[mid]);
		DUMP(data[(mid << SHIFT) + offset[mid]]);
		if(less(data[(mid << SHIFT) + offset[mid]], x))
			l = mid + 1;
		else
			h = mid;
	}
	DUMP(l);
	if(l == 0)
		return 0;
	l--;
	if(l == blk_count)
		return LBound(x, blk_items, GetCount(), less);
	DUMP((l << SHIFT) + NBLK);
	return BoundL(x, 0, (l << SHIFT) + NBLK);

	
	int bpos = l << SHIFT;
	int off = offset[l];
	if(less(data[bpos + NBLK - 1], x))
		return LBound(x, bpos, off, less) + bpos + NBLK - off;
	else
		return LBound(x, off, bpos + NBLK, less) - off + bpos;
}
#endif
template<class T> template <class Less>
int Flex<T>::UBound(const T& x, int l, int h, const Less& less) const
{
	while(l < h) {
		int mid = unsigned(l + h) >> 1;
		if(less(x, data[mid]))
			h = mid;
		else
			l = mid + 1;
	}
	return h;
}

template <class T> template<class Less>
int Flex<T>::FindUpperBound(const T& x, const Less& less) const
{
	if(GetCount() == 0)
		return 0;
	int l = 0;
	int h = blk_count + 1;
	while(l < h) {
		int mid = unsigned(l + h) >> 1;
		if(less(x, data[offset[mid]]))
			h = mid;
		else
			l = mid + 1;
	}
	if(l == blk_count + 1)
		return UBound(x, blk_items, GetCount(), less);
	if(l == 0)
		return 0;
	int bpos = (l - 1) << SHIFT;
	int off = offset[l];
	if(less(data[bpos], x))
		return bpos + NBLK - off + UBound(x, bpos, off, less);
	else
		return bpos + UBound(x, off, bpos + NBLK, less);
}
