int64 NewInVectorSerial();

template <class T>
InVector<T>::InVector()
{
	serial = NewInVectorSerial();
	Reset();
}

template <class T>
void InVector<T>::Reset()
{
	hcount = count = 0;
	blk_high = 4000 / sizeof(T);
	blk_low = 800 / sizeof(T);
}

template <class T>
void InVector<T>::Clear()
{
	data.Clear();
	Reset();
}

extern thread__ int64 invector_cache_serial_;
extern thread__ int   invector_cache_blki_;
extern thread__ int   invector_cache_offset_;
extern thread__ int   invector_cache_end_;

template <class T>
force_inline void InVector<T>::SetCache(int blki, int offset) const
{
	invector_cache_serial_ = serial;
	invector_cache_blki_ = blki;
	invector_cache_offset_ = offset;
	invector_cache_end_ = offset + data[blki].GetCount();
}

template <class T>
force_inline void InVector<T>::ClearCache() const
{
	invector_cache_serial_ = 0;
}


template <class T>
force_inline int InVector<T>::FindBlock(int& pos, int& off) const
{
	if(invector_cache_serial_ == serial && pos >= invector_cache_offset_ &&
	   pos < invector_cache_end_) {
		LLOG("Found in cache, serial: " << r.serial << ", offset: " << r.offset << ", end: " << r.end);
		off = invector_cache_offset_;
		pos -= off;
		return invector_cache_blki_;
	}
	return FindBlock0(pos, off);
}

template <class T>
int InVector<T>::FindBlock0(int& pos, int& off) const
{
	LLOG("FindBlock " << pos);
	ASSERT(pos >= 0 && pos <= count);
	if(pos == count) {
		LLOG("Found last");
		pos = data.Top().GetCount();
		off = count - pos;
		return data.GetCount() - 1;
	}
	int blki = 0;
	int offset = 0;
	for(int i = index.GetCount(); --i >= 0;) {
		int n = index[i][blki];
		if(pos >= n) {
			blki++;
			pos -= n;
			offset += n;
		}
		blki += blki;
	}
	int n = data[blki].GetCount();
	if(pos >= n) {
		blki++;
		pos -= n;
		offset += n;
	}

	SetCache(blki, offset);

	off = offset;
	return blki;
}

template <class T>
force_inline int InVector<T>::FindBlock(int& pos) const
{
	int h;
	return FindBlock(pos, h);
}

template <class T>
const T& InVector<T>::operator[](int i) const
{
	LLOG("operator[] " << i);
	ASSERT(i >= 0 && i < count);
	int blki = FindBlock(i);
	return data[blki][i];
}

template <class T>
T& InVector<T>::operator[](int i)
{
	LLOG("operator[] " << i);
	ASSERT(i >= 0 && i < count);
	int blki = FindBlock(i);
	return data[blki][i];
}

#ifdef _DEBUG
template <class T>
void InVector<T>::DumpIndex()
{
	String h;
	DLOG("------- InVector dump, " << index.GetCount());
	for(int i = 0; i < data.GetCount(); i++) {
		if(i)
			h << ", ";
		h << data[i].GetCount();
	}
	DLOG(h);
	for(int j = 0; j < index.GetCount(); j++) {
		h.Clear();
		for(int k = 0; k < index[j].GetCount(); k++) {
			if(k)
				h << ", ";
			h << index[j][k];
		}
		DLOG(h);
	}
	DLOG(".");
}
#endif

template <class T>
void InVector<T>::Reindex()
{
	RTIMING("Reindex");
	LLOG("--- Reindexing");
	index.Clear();
	hcount = 0;
	Vector<T> *ds = data.Begin();
	Vector<T> *dend = data.End();
	int n = data.GetCount();
	if(n <= 2)
		return;
	Vector<int>& w = index.Add();
	hcount = 2;
	w.SetCount((n + 1) >> 1);
	int *t = w.Begin();
	while(ds != dend) {
		*t = (ds++)->GetCount();
		if(ds == dend)
			break;
		*t++ += (ds++)->GetCount();
	}
	int *s = w.Begin();
	int *end = w.End();
	n = w.GetCount();
	while(n > 2) {
		Vector<int>& w = index.Add();
		hcount += hcount;
		w.SetCount((n + 1) >> 1);
		t = w.Begin();
		while(s != end) {
			*t = *s++;
			if(s == end)
				break;
			*t++ += *s++;
		}
		s = w.Begin();
		end = w.End();
		n = w.GetCount();
	}
	ClearCache();
#ifdef ADAPTIVE
	n = 2500 + data.GetCount() / 4;
	blk_high = max(n / (int)sizeof(T), 16);
	blk_low = max(n / 3 / (int)sizeof(T), 16);
#endif
}

template <class T>
T *InVector<T>::Insert0(int ii, int blki, int pos, const T *val)
{
	if(data[blki].GetCount() > blk_high) {
		Vector<T>& b2 = data.Insert(blki + 1);
		b2.InsertSplit(0, data[blki], data[blki].GetCount() / 2);
		data[blki].Shrink();
		Reindex();
		ClearCache();
		pos = ii;
		blki = FindBlock(pos);
	}
	LLOG("blki: " << blki << ", pos: " << pos);
	count++;
	ClearCache(); // TODO: Immediately after inserting, update cache for the next retrieval
	int q = blki;
	for(int i = 0; i < index.GetCount(); i++)
		index[i].At(q >>= 1, 0)++;
	if(val) {
		data[blki].Insert(pos, *val);
		return &data[blki][pos];
	}
	else
		return &data[blki].Insert(pos);
}

template <class T>
force_inline bool InVector<T>::JoinSmall(int blki)
{
	if(blki < data.GetCount()) {
		int n = data[blki].GetCount();
		if(n == 0) {
			data.Remove(blki);
			return true;
		}
		if(n < blk_low) {
			if(blki > 0 && data[blki - 1].GetCount() + n <= blk_high) {
				data[blki - 1].AppendPick(data[blki]);
				data.Remove(blki);
				return true;
			}
			if(blki + 1 < data.GetCount() && n + data[blki + 1].GetCount() <= blk_high) {
				data[blki].AppendPick(data[blki + 1]);
				data.Remove(blki + 1);
				return true;
			}
		}
	}
	return false;
}

template <class T>
T *InVector<T>::Insert(int ii, const T *val)
{
	ASSERT(ii >= 0 && ii <= GetCount());
	if(ii == 0 && data.GetCount() == 0) {
		count++;
		ClearCache();
		if(val) {
			data.Add().Insert(0, *val);
			return &data[0][0];
		}
		return &data.Add().Insert(0);
	}
	int pos = ii;
	int blki = FindBlock(pos);
	return Insert0(ii, blki, pos, val);
}

/*
template <class T>
void InVector<T>::InsertN(int ii, int n)
{
	ASSERT(ii >= 0 && ii <= GetCount());
	if(data[blki].GetCount() > blk_high) {
		Vector<T>& b2 = data.Insert(blki + 1);
		b2.InsertSplit(0, data[blki], data[blki].GetCount() / 2);
		data[blki].Shrink();
		Reindex();
		invector_cache.serial = 0;
		pos = ii;
		blki = FindBlock(pos);
	}
	count++;
	invector_cache.serial = 0; // TODO: Immediately after inserting, update cache for the next retrieval
	int q = blki;
	for(int i = 0; i < index.GetCount(); i++)
		index[i].At(q >>= 1, 0)++;
	if(val) {
		data[blki].Insert(pos, *val);
		return &data[blki][pos];
	}
	else
		return &data[blki].Insert(pos);
}
*/

template <class T>
void InVector<T>::Remove(int pos, int n)
{
	ASSERT(pos + n <= GetCount());
	count -= n;
	int blki = FindBlock(pos);
	if(pos + n < data[blki].GetCount()) {
		data[blki].Remove(pos, n);
		if(JoinSmall(blki))
			Reindex();
		else {
			int q = blki;
			for(int i = 0; i < index.GetCount(); i++)
				index[i].At(q >>= 1, 0) -= n;
		}
	}
	else {
		int b1 = blki;
		int nn = min(n, data[b1].GetCount() - pos);
		data[b1++].Remove(pos, nn);
		n -= nn;
		int b2 = b1;
		while(n >= data[b2].GetCount()) {
			n -= min(n, data[b2].GetCount());
			b2++;
		}
		data.Remove(b1, b2 - b1);
		data[b1].Remove(0, n);
		JoinSmall(blki + 1);
		JoinSmall(blki);
		Reindex();
	}
	ClearCache(); // TODO: Immediately after removing, update cache for the next retrieval
}
