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
	SetBlkPar();
}

template <class T>
void InVector<T>::Clear()
{
	data.Clear();
	index.Clear();
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
	Chk();
	if(invector_cache_serial_ == serial && pos >= invector_cache_offset_ &&
	   pos < invector_cache_end_) {
		LLOG("Found in cache, serial: " << invector_cache_serial_ << ", offset: " << invector_cache_offset_ << ", end: " << invector_cache_end_);
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
	ClearCache();
	SetBlkPar();
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
}

template <class T>
void InVector<T>::SetBlkPar()
{
#if defined(_DEBUG) && defined(flagIVTEST)
	blk_high = 11;
	blk_low = 5;
#else
	n = 2500 + data.GetCount() / 4;
	blk_high = max(n / (int)sizeof(T), 16);
	blk_low = max(n / 3 / (int)sizeof(T), 16);
#endif
}

template <class T>
T *InVector<T>::Insert0(int ii, int blki, int pos, int off, const T *val)
{
	if(data[blki].GetCount() > blk_high) {
		data.Insert(blki + 1).InsertSplit(0, data[blki], data[blki].GetCount() / 2);
		data[blki].Shrink();
		Reindex();
		pos = ii;
		blki = FindBlock(pos, off);
	}
	LLOG("blki: " << blki << ", pos: " << pos);
	count++;
	int q = blki;
	for(int i = 0; i < index.GetCount(); i++)
		index[i].At(q >>= 1, 0)++;
	if(val)
		data[blki].Insert(pos, *val);
	else
		data[blki].Insert(pos);
	SetCache(blki, off);
	return &data[blki][pos];
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
T *InVector<T>::Insert0(int ii, const T *val)
{
	ASSERT(ii >= 0 && ii <= GetCount());
	if(data.GetCount() == 0) {
		count++;
		ClearCache();
		if(val) {
			data.Add().Add(*val);
			return &data[0][0];
		}
		return &data.Add().Insert(0);
	}
	int pos = ii;
	int off;
	int blki = FindBlock(pos, off);
	return Insert0(ii, blki, pos, off, val);
}

template <class T>
void InVector<T>::InsertN(int ii, int n)
{
	ASSERT(ii >= 0 && ii <= GetCount() && n >= 0);
	LLOG("InsertN " << ii << ", " << n);

	if(n == 0)
		return;

	if(data.GetCount() == 0 && n > 0) {
		count++;
		data.Add().Add();
		if(--n == 0)
			return;
	}

	int pos = ii;
	int off;
	int blki = FindBlock(pos, off);
	int bc = data[blki].GetCount();
	
	count += n;

	if(bc + n < blk_high) {
		data[blki].InsertN(pos, n);
		int q = blki;
		for(int i = 0; i < index.GetCount(); i++)
			index[i].At(q >>= 1, 0) += n;
		SetCache(blki, off);
	}
	else
	if(bc - pos + n < blk_high) {
		Vector<T>& t = data.Insert(blki + 1);
		t.InsertN(0, n);
		t.InsertSplit(n, data[blki], pos);
		data[blki].Shrink();
		Reindex();
	}
	else {
		int m = (blk_high + blk_low) / 2;
		int bn = (n + m - 1) / m;
		int ti;
		if(pos) {
			ti = blki + 1;
			data.InsertN(ti, bn + 1);
			data[ti + bn].InsertSplit(0, data[blki], pos);
			data[blki].Shrink();
		}
		else {
			ti = blki;
			data.InsertN(ti, bn);
		}
		for(int i = 0; i < bn; i++) {
			int q = min(m, n);
			data[ti + i].SetCount(q);
			n -= q;
		}
		ASSERT(n == 0);
		Reindex();
	}
}

template <class T>
void InVector<T>::Remove(int pos, int n)
{
	ASSERT(pos >= 0 && pos + n <= GetCount());
	count -= n;
	int off;
	int blki = FindBlock(pos, off);
	if(pos + n < data[blki].GetCount()) {
		data[blki].Remove(pos, n);
		if(JoinSmall(blki))
			Reindex();
		else {
			int q = blki;
			for(int i = 0; i < index.GetCount(); i++)
				index[i].At(q >>= 1, 0) -= n;
			SetCache(blki, off);
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
}

template <class T>
void InVector<T>::SetCount(int n)
{
	if(n < GetCount())
		Trim(n);
	else
		Insert(GetCount(), n - GetCount());
}

template <class T>
void InVector<T>::Shrink()
{
	for(int i = 0; i < data.GetCount(); i++)
		data[i].Shrink();
	data.Shrink();
	for(int i = 0; i < index.GetCount(); i++)
		index[i].Shrink();
	index.Shrink();
}

template <class T>
void InVector<T>::Set(int i, const T& x, int count)
{
	Iterator it = GetIter(i);
	while(count-- > 0)
		*it++ = x;
}

template <class T>
InVector<T>::InVector(const InVector<T>& v, int)
{
	data <<= v.data;
	index <<= v.index;
	count = v.count;
	hcount = v.hcount;
	blk_high = v.blk_high;
	blk_low = v.blk_low;
	serial = NewInVectorSerial();
}
