#include <Core/Core.h>
#include <set>

using namespace Upp;

#define LLOG(x) // LOG(x)

#define USECACHE

template <class T>
class InVector {
	Vector< Vector<T> > data;
	Vector< Vector<int> > index;
	int   count;
	int64 serial1, serial2;
	
#ifdef _DEBUG
	enum { BLKSIZE = 10 };
#else
	enum { BLKSIZE = 12000 / sizeof(T) };
#endif

	int FindBlock(int& pos) const;
	void Reindex();

public:
	InVector();
	void Dump();
	T&   Insert(int ii);
	const T& operator[](int i) const;
	T& operator[](int i);
	int  GetCount() const      { return count; }

	typedef ConstIIterator<InVector> ConstIterator;
	typedef IIterator<InVector>      Iterator;

	typedef T        ValueType;

	ConstIterator    Begin() const              { return ConstIterator(*this, 0); }
	ConstIterator    End() const                { return ConstIterator(*this, GetCount()); }
	ConstIterator    GetIter(int pos) const     { return ConstIterator(*this, pos); }
	Iterator         Begin()                    { return Iterator(*this, 0); }
	Iterator         End()                      { return Iterator(*this, GetCount()); }
	Iterator         GetIter(int pos)           { return Iterator(*this, pos); }
};

int64 NewInVectorSerial()
{
	static int64 x;
	INTERLOCKED {
		++x;
	}
	return x;
}

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

static thread__ InVectorCacheRecord invector_cache[6];
static thread__ int invector_cachei;

template <class T>
int InVector<T>::FindBlock(int& pos) const
{
	LLOG("FindBlock " << pos);
	ASSERT(pos >= 0 && pos <= count);
#ifdef USECACHE
	invector_cachei = invector_cachei == 5 ? 0 : invector_cachei + 1;
	InVectorCacheRecord& r = invector_cache[invector_cachei];
	if(r.serial1 == serial1 && r.serial2 == serial2 && pos >= r.offset && pos < r.end) {
		LLOG("Found in cache, serial1: " << r.serial1 << ", serial2: " << r.serial2 << ", offset: " << r.offset << ", end: " << r.end);
		pos -= r.offset;
		return r.blki;
	}
#endif
	if(pos == count) {
		LLOG("Found last");
		pos = data.Top().GetCount();
		return data.GetCount() - 1;
	}
	int ii = 0;
	int offset = 0;
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
	return ii;
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

template <class T>
T& InVector<T>::Insert(int ii)
{
	LLOG("*** Insert " << ii << ", count " << count);
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

#define N 10000000

void InVectorBenchmark()
{
	std::set<int> s;
	SeedRandom();
	{
		RTIMING("std::set INSERT");
		for(int i = 0; i < N; i++)
			s.insert(Random());
	}

	InVector<int> a;
	SeedRandom();
	{
		RTIMING("InVector INSERT");
		a.Insert(0) = 0;
		for(int i = 0; i < N; i++) {
			a.Insert(Random(a.GetCount())) = Random(100);
		}
	}
	{
		RTIMING("InVector SCAN");
		int sum = 0;
		for(int i = 0; i < a.GetCount(); i++)
			sum += a[i];
		RDUMP(sum);
	}
	{
		RTIMING("InVector Sort");
		Sort(a);
	}
	RDUMP(a.GetCount());
	return;

	SeedRandom();
	Vector<int> b;
	{
		RTIMING("Vector INSERT");
		b.Insert(0) = 0;
		for(int i = 0; i < N; i++) {
			b.Add(Random(100));
//			b.Insert(Random(b.GetCount())) = Random(100);
		}
	}
	{
		RTIMING("Vector SCAN");
		int sum = 0;
		for(int i = 0; i < b.GetCount(); i++)
			sum += b[i];
		RDUMP(sum);
	}
	{
		RTIMING("Vector Sort");
		Sort(b);
	}
	SeedRandom();
	Array<int> c;
	{
		RTIMING("Array INSERT");
		c.Insert(0) = 0;
		for(int i = 0; i < N; i++) {
			c.Add(Random(100));
//			c.Insert(Random(c.GetCount())) = Random(100);
		}
	}
	{
		RTIMING("Array SCAN");
		int sum = 0;
		for(int i = 0; i < c.GetCount(); i++)
			sum += c[i];
		RDUMP(sum);
	}
	{
		RTIMING("Array Sort");
		Sort(c);
	}

	SeedRandom();
	BiVector<int> d;
	{
		RTIMING("BiVector INSERT");
		d.AddTail() = 0;
		for(int i = 0; i < N; i++) {
			d.AddTail(Random(100));
//			d.Insert(Random(d.GetCount())) = Random(100);
		}
	}
	{
		RTIMING("BiVector SCAN");
		int sum = 0;
		for(int i = 0; i < d.GetCount(); i++)
			sum += d[i];
		RDUMP(sum);
	}
	{
		RTIMING("BiVector Sort");
		Sort(d);
	}
}

void InVectorTest()
{
	Vector<int> q;
	InVector<int> iv;
	iv.Insert(0) = 0;
	q.Insert(0) = 0;
	iv.Insert(1) = -1;
	q.Insert(1) = -1;
	for(int i = 0; i < 10000; i++) {
		if(i % 1000 == 0)
			LOG(i);
		int i = Random(iv.GetCount());
		{ RTIMING("InVector Insert");
			iv.Insert(i) = i;
		}
		{ RTIMING("Vector Insert");
			q.Insert(i) = i;
		}
		ASSERT(q.GetCount() == iv.GetCount());
		for(int i = 0; i < q.GetCount(); i++)
			ASSERT(q[i] == iv[i]);
	}
}
	
CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
#ifdef _DEBUG
	InVectorTest();
#else
	InVectorBenchmark();
#endif
}
