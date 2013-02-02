#include "InVector.h"

int64 NewInVectorSerial();

template <class T>
InVector<T>::InVector()
{
	count = 0;
	serial1 = NewInVectorSerial();
	serial2 = 1;
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

template <class T>
int InVector<T>::FindBlock(int& pos, int& off) const
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
	off = offset;
	DDUMP(off);
	ASSERT(BlkToOffset(blki) == offset); _DBG_
	return blki;
}

template <class T>
int InVector<T>::BlkToOffset(int blki) const
{
	int ii = 0;
	int offset = 0;
	int count = data.GetCount();
	DLOG("BlkToOffset");
	DDUMP(blki);
	DDUMP(index.GetCount());
	for(int i = index.GetCount(); --i >= 0;) {
		int n = index[i][ii];
		count >>= 1;
		if(blki > count) {
			ii++;
			blki -= count;
			offset += n;
		}
		ii += ii;
	}
	DDUMP(blki);
	DDUMP(ii);
	if(blki > ii)
		offset += data[ii].GetCount();
	DDUMP(offset);
	return offset;
}

/*
int InVector::FindUpperBound(const T& val, const L& less, int& off, int& pos)
{
	int count = data.GetCount();
	int blki = 0;
	while(count > 0) {
		int half = count >> 1;
		int m = blki + half;
		if(less(val, data[m].Top()))
			count = half;
		else {
			blki = m + 1;
			count = count - half - 1;
		}
	}
	
	off = BlkToOffset(blki);
	return pos;
	
	
	
	
	LLOG("FindUpperBound " << pos);
	int ii = 0;
	int offt = 0;
	for(int i = index.GetCount(); --i >= 0;) {
		int n = index[i][ii];
		if(pos >= n) {
			ii++;
			pos -= n;
			offset += n;
		}
		ii += ii;
	}
	int n = data[ii].GetCount();
	if(pos >= n) {
		ii++;
		pos -= n;
		offset += n;
	}
#ifdef USECACHE
	r.serial1 = serial1;
	r.serial2 = serial2;
	r.blki = ii;
	r.offset = offset;
	r.end = offset + data[ii].GetCount();
#endif
	off = offset;
//	DDUMP(off);
	return ii;
}
*/

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
int InVector<T>::FindBlock(int& pos) const
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

#ifdef REINDEX2
template <class T>
void InVector<T>::Reindex()
{
	RTIMING("Reindex");
	LLOG("--- Reindexing");
	index.Clear();
	Vector<int> *ds = data.Begin();
	Vector<int> *dend = data.End();
	int n = data.GetCount();
	if(n <= 2)
		return;
	Vector<int>& w = index.Add();
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
#else
template <class T>
void InVector<T>::Reindex()
{
	RTIMING("Reindex");
	index.Clear();
	int q = data.GetCount();
	int n = 0;
	while((q >>= 1) > 0)
		n++;
	LLOG("Reindexing " << n);
	index.SetCount(n);
	for(int i = 0; i < data.GetCount(); i++) {
		int n = data[i].GetCount();
		int q = i;
		for(int j = 0; j < index.GetCount(); j++)
			index[j].At(q >>= 1, 0) += n;
	}
#ifdef _DEBUG
	Dump();
#endif
}
#endif

template <class T>
T& InVector<T>::Insert(int ii)
{
	LLOG("*** Insert " << ii << ", count " << count << ", index: " << index.GetCount());
	if(ii == 0 && data.GetCount() == 0) {
		count++;
		serial2++;
		return data.Add().Insert(0);
	}
	int ii0 = ii;
	int blki = FindBlock(ii);
	if(data[blki].GetCount() > BLKSIZE) {
		Vector<T>& b2 = data.Insert(blki + 1);
		b2.InsertSplit(0, data[blki], data[blki].GetCount() / 2);
		data[blki].Shrink();
		Reindex();
		serial2++;
		ii = ii0;
		blki = FindBlock(ii);
	}
	LLOG("blki: " << blki << ", ii: " << ii << ", serial2: " << serial2);
	count++;
	serial2++;
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

