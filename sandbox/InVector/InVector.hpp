#include "InVector.h"

int64 NewInVectorSerial();

template <class T>
InVector<T>::InVector()
{
	hcount = count = 0;
#ifdef USECACHE
	serial1 = NewInVectorSerial();
	serial2 = 1;
#endif
#ifdef USECACHE2
	serial = NewInVectorSerial();
#endif
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

#ifdef USECACHE
struct InVectorCacheRecord {
	int64 serial1;
	int64 serial2;
	int   blki;
	int   offset;
	int   end;
	int   filler;
};

extern thread__ InVectorCacheRecord invector_cache[6];
extern thread__ int invector_cachei;
#endif

#ifdef USECACHE2
struct InVectorCacheRecord {
	int64 serial;
	int   blki;
	int   offset;
	int   end;
};

extern thread__ InVectorCacheRecord invector_cache;
#endif

#ifdef USECACHE2
template <class T>
force_inline int InVector<T>::FindBlock(int& pos, int& off) const
{
	InVectorCacheRecord& r = invector_cache;
	if(r.serial == serial && pos >= r.offset && pos < r.end) {
		LLOG("Found in cache, serial1: " << r.serial << ", offset: " << r.offset << ", end: " << r.end);
		pos -= r.offset;
		off = r.offset;
		return r.blki;
	}
	return FindBlock0(pos, off);
}

template <class T>
int InVector<T>::FindBlock0(int& pos, int& off) const

#else

template <class T>
int InVector<T>::FindBlock(int& pos, int& off) const
#endif
{
//	RTIMING("FindBlock");
	LLOG("FindBlock " << pos);
	ASSERT(pos >= 0 && pos <= count);
#ifdef USECACHE
	invector_cachei = invector_cachei == 5 ? 0 : invector_cachei + 1;
	InVectorCacheRecord& r = invector_cache[invector_cachei];
	if(r.serial1 == serial1 && r.serial2 == serial2 && pos >= r.offset && pos < r.end) {
		LLOG("Found in cache, serial1: " << r.serial1 << ", serial2: " << r.serial2 << ", offset: " << r.offset << ", end: " << r.end);
		pos -= r.offset;
		off = r.offset;
		return r.blki;
	}
#endif
//	RTIMING("FindBlock no cache");
	if(pos == count) {
		LLOG("Found last");
		pos = data.Top().GetCount();
		off = count - pos;
	//	DLOG("Last off: " << off);
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
#ifdef USECACHE
	r.serial1 = serial1;
	r.serial2 = serial2;
	r.blki = blki;
	r.offset = offset;
	r.end = offset + data[blki].GetCount();
#endif
#ifdef USECACHE2
	InVectorCacheRecord& r = invector_cache;
	r.serial = serial;
	r.blki = blki;
	r.offset = offset;
	r.end = offset + data[blki].GetCount();
#endif
	off = offset;
	return blki;
}

template <class T>
template <class L>
int InVector<T>::FindUpperBound(const T& val, const L& less, int& off, int& pos)
{
	if(data.GetCount() == 0) {
		pos = off = 0;
		return 0;
	}
	int blki = 0;
	int ii = 0;
	int offset = 0;
	int half = hcount;
	for(int i = index.GetCount(); --i >= 0;) {
		int m = blki + half;
		if(m - 1 < data.GetCount() && !less(val, data[m - 1].Top())) {
			blki = m;
			offset += index[i][ii];
			ii++;
		}
		ii += ii;
		half >>= 1;
	}
	if(blki < data.GetCount()) {
		if(!less(val, data[blki].Top()))
			offset += data[blki++].GetCount();
		if(blki < data.GetCount()) {
			pos = Upp::FindUpperBound(data[blki], val, less);
			off = offset;
			return blki;
		}
	}
	pos = data.Top().GetCount();
	off = count - pos;
	blki--;
	return blki;
}

template <class T>
template <class L>
int InVector<T>::FindLowerBound(const T& val, const L& less, int& off, int& pos)
{
	if(data.GetCount() == 0) {
		pos = off = 0;
		return 0;
	}
	int blki = 0;
	int ii = 0;
	int offset = 0;
	int half = hcount;
	for(int i = index.GetCount(); --i >= 0;) {
		int m = blki + half;
		if(m - 1 < data.GetCount() && less(data[m - 1][0], val)) {
			blki = m;
			offset += index[i][ii];
			ii++;
		}
		ii += ii;
		half >>= 1;
	}
	if(blki < data.GetCount()) {
		if(less(data[blki][0], val))
			offset += data[blki++].GetCount();
		if(blki < data.GetCount()) {
			pos = Upp::FindLowerBound(data[blki], val, less);
			off = offset;
			return blki;
		}
	}
	pos = data.Top().GetCount();
	off = count - pos;
	blki--;
	return blki;
}

#ifndef IITERATOR

template <class T>
void InVector<T>::SetIter(ConstIterator& it, int ii)
{
	it.v = this;
	it.blki = FindBlock(ii, it.offset);
	it.begin = data[it.blki].Begin();
	it.end = data[it.blki].End();
	it.ptr = it.begin + ii;
}

template <class T>
void InVector<T>::SetBegin(ConstIterator& it)
{
	if(count) {
		it.v = this;
		it.blki = 0;
		it.ptr = it.begin = data[0].Begin();
		it.end = data[0].End();
		it.offset = 0;
	}
	else
		SetEnd(it);
}

template <class T>
void InVector<T>::SetEnd(ConstIterator& it)
{
	if(count) {
		it.v = this;
		it.blki = data.GetCount() - 1;
		it.begin = data.Top().Begin();
		it.ptr = it.end = data.Top().End();
		it.offset = count - data.Top().GetCount();
	}
	else {
		it.v = this;
		it.blki = 0;
		it.ptr = it.begin = it.end = NULL;
		it.offset = 0;		
	}
}

template <typename T>
force_inline typename InVector<T>::ConstIterator& InVector<T>::ConstIterator::operator+=(int d)
{
	if(d >= 0 ? d < end - ptr : -d < ptr - begin)
		ptr += d;
	else
		v->SetIter(*this, GetIndex() + d);
	ASSERT(end - begin == v->data[blki].GetCount());
	return *this;
}

template <typename T>
void InVector<T>::ConstIterator::NextBlk()
{
	ASSERT(end - begin == v->data[blki].GetCount());
	if(blki + 1 < v->data.GetCount()) {
		offset += v->data[blki].GetCount();
		++blki;
		ptr = begin = v->data[blki].Begin();
		end = v->data[blki].End();
	}
}

template <typename T>
void InVector<T>::ConstIterator::PrevBlk()
{
	--blki;
	begin = v->data[blki].Begin();
	ptr = end = v->data[blki].End();
	offset -= v->data[blki].GetCount();
}

#endif

template <class T>
force_inline int InVector<T>::FindBlock(int& pos) const
{
	int h;
	return FindBlock(pos, h);
}

template <class T>
void InVector<T>::Dump()
{
	String h;
	LLOG("------- InVector dump, " << index.GetCount());
	for(int i = 0; i < data.GetCount(); i++) {
		if(i)
			h << ", ";
		h << data[i].GetCount();
	}
	LLOG(h);
	for(int j = 0; j < index.GetCount(); j++) {
		h.Clear();
		for(int k = 0; k < index[j].GetCount(); k++) {
			if(k)
				h << ", ";
			h << index[j][k];
		}
		LLOG(h);
	}
	LLOG(".");
}

template <class T>
void InVector<T>::Reindex()
{
	RTIMING("Reindex");
	LLOG("--- Reindexing");
	index.Clear();
	hcount = 0;
	Vector<int> *ds = data.Begin();
	Vector<int> *dend = data.End();
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
#ifdef _DEBUG
	Dump();
#endif
}

template <class T>
T& InVector<T>::Insert(int ii)
{
	LLOG("*** Insert " << ii << ", count " << count << ", index: " << index.GetCount());
	if(ii == 0 && data.GetCount() == 0) {
		count++;
#ifdef USECACHE
		serial2++;
#endif
#ifdef USECACHE2
		invector_cache.serial = 0;
#endif
		return data.Add().Insert(0);
	}
	int ii0 = ii;
	int blki = FindBlock(ii);
	if(data[blki].GetCount() > BLKSIZE) {
		Vector<T>& b2 = data.Insert(blki + 1);
		b2.InsertSplit(0, data[blki], data[blki].GetCount() / 2);
		data[blki].Shrink();
		Reindex();
#ifdef USECACHE
		serial2++;
#endif
#ifdef USECACHE2
		invector_cache.serial = 0;
#endif
		ii = ii0;
		blki = FindBlock(ii);
	}
	LLOG("blki: " << blki << ", ii: " << ii);
	count++;
	invector_cache.serial = 0;
#ifdef USECACHE
		serial2++;
#endif
#ifdef USECACHE2
		invector_cache.serial = 0;
#endif
	// TODO: Immediately after inserting, update cache for the next retrieval
	int q = blki;
	for(int i = 0; i < index.GetCount(); i++)
		index[i].At(q >>= 1, 0)++;
	T& x = data[blki].Insert(ii);
#ifdef _DEBUG
	Dump();
#endif
	return x;
}

