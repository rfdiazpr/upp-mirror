#include "InVector.h"

#define SN 10000000
#define IN 100

int sSum = 0;

template <class T>
void ScanBenchmark()
{
	T x;
	for(int i = 0; i < SN; i++)
		x.Insert(x.GetCount()) = i;
	
	{
		TimeStop tm;
		for(int j = 0; j < IN; j++)
			for(int i = 0; i < SN; i++)
				sSum += x[i];
		RLOG(typeid(T).name() << " index scan " << tm);
	}
	{
		TimeStop tm;
		typename T::Iterator end = x.End();
		for(int j = 0; j < IN; j++)
			for(typename T::Iterator it = x.Begin(); it != end; ++it)
				sSum += *it;
		RLOG(typeid(T).name() << " iterator scan " << tm);
	}
}

void InVectorScanBenchmark()
{
	ScanBenchmark< Vector<int> >();
	ScanBenchmark< Array<int> >();
	ScanBenchmark< InVector<int> >();
}

#define N 10000000


void InVectorBenchmark()
{

	std::multiset<int> s;
	SeedRandom();
	{
		RTIMING("std::set INSERT");
		for(int i = 0; i < N; i++) {
			int x = Random();
			s.insert(x);
		}
	}

	SeedRandom();
	{
		InVector<int> a;
		RTIMING("InVector INSERT UB");
		for(int i = 0; i < N; i++) {
			int x = Random();
			a.Insert(FindLowerBound(a, x)) = x;
		}
	}
	InVector<int> a;
	{
		RTIMING("InVector INSERT");
		a.Insert(0) = 0;
		for(int i = 0; i < N; i++) {
			a.Insert(Random(a.GetCount())) = i;
		}
	}
	{
		InVector<int> a;
		RTIMING("InVector INSERT 0");
		a.Insert(0) = 0;
		for(int i = 0; i < N; i++) {
			a.Insert(0) = Random(100);
		}
	}
	{
		InVector<int> a;
		RTIMING("InVector INSERT N/2");
		a.Insert(0) = 0;
		for(int i = 0; i < N; i++) {
			a.Insert(i / 2) = Random(100);
		}
	}
	return;
	{
		RTIMING("InVector SCAN");
		int sum = 0;
		for(int i = 0; i < a.GetCount(); i++)
			sum += a[i];
		RDUMP(sum);
	}
	{
		RTIMING("InVector iterator SCAN");
		int sum = 0;
		for(InVector<int>::Iterator i = a.Begin(); i != a.End(); i++)
			sum += *i;
		RDUMP(sum);
	}
	{
		RTIMING("InVector Sort");
		Sort(a);
	}
	RDUMP(a.GetCount());

	SeedRandom();
	Vector<int> b;
	{
		RTIMING("Vector INSERT");
		b.Insert(0) = 0;
		for(int i = 0; i < N; i++) {
			b.Add(Random());
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
		RTIMING("Vector iterator SCAN");
		int sum = 0;
		for(Vector<int>::Iterator i = b.Begin(); i != b.End(); i++)
			sum += *i;
		RDUMP(sum);
	}
	{
		RTIMING("Vector Sort");
		Sort(b);
	}
	return;
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

template <class C1, class C2>
void Compare(C1& a, C2& b)
{
	ASSERT(a.GetCount() == b.GetCount());

	for(int i = 0; i < a.GetCount(); i++)
		ASSERT(a[i] == b[i]);
	
	typename C1::Iterator ia = a.Begin();
	typename C2::Iterator ib = b.Begin();
	while(ib != b.End()) {
		ASSERT(*ia == *ib);
		ia++;
		ib++;
	}
}

void InVectorTest()
{
	SeedRandom();
	Vector<int> q;
	InVector<int> iv;
	Compare(q, iv);
	iv.Insert(0) = 0;
	q.Insert(0) = 0;
	iv.Insert(1) = -1;
	q.Insert(1) = -1;
	for(int j = 0; j < 10000; j++) {
		if(j % 1000 == 0)
			LOG(j);
		int i = Random(iv.GetCount());
		iv.Insert(i) = i;
		q.Insert(i) = i;
		Compare(q, iv);
		ASSERT(iv.End() - iv.Begin() == iv.GetCount());
	}

	for(int i = 0; i < 100; i++) {
		int n = Random(100) + 20;
		InVector<int>::Iterator it2, it = iv.Begin();
		it += n;
		ASSERT(it - iv.Begin() == n);
		it2 = it;
		for(int j = 0; j < 10; j++) {
			ASSERT(it2 - iv.Begin() == n + j);
			++it2;
		}
		it2 = it;
		for(int j = 0; j < 10; j++) {
			ASSERT(it2 - iv.Begin() == n - j);
			--it2;
		}
	}

	StableSort(q);
	StableSort(iv);
	Compare(q, iv);
}

void TestUpperBound()
{
	{
		InVector<int> v;
		for(int i = 0; i < 3000; i++) {
			if(i % 1000 == 0)
				LOG(i);
			v.Insert(i) = i;
			ASSERT(v.FindUpperBound(i) == i + 1);
			for(int j = 0; j < i; j++)
				ASSERT(v.FindUpperBound(j) == j + 1);
		}
	}
	{
		InVector<int> v;
		for(int i = 0; i < 3000; i++) {
			if(i % 1000 == 0)
				LOG(i);
			for(int j = 0; j < 7; j++)
				v.Insert(7 * i) = i;
			ASSERT(v.FindUpperBound(i) == 7 * i + 7);
			for(int j = 0; j < i; j++)
				ASSERT(v.FindUpperBound(j) == 7 * j + 7);
		}
	}
}

void TestLowerBound()
{
	{
		InVector<int> v;
		for(int i = 0; i < 3000; i++) {
			if(i % 1000 == 0)
				LOG(i);
			v.Insert(i) = i;
			ASSERT(v.FindLowerBound(i) == i);
			for(int j = 0; j < i; j++)
				ASSERT(v.FindLowerBound(j) == j);
		}
	}
	{
		InVector<int> v;
		for(int i = 0; i < 3000; i++) {
			if(i % 1000 == 0)
				LOG(i);
			for(int j = 0; j < 7; j++)
				v.Insert(7 * i) = i;
			ASSERT(v.FindLowerBound(i) == 7 * i);
			for(int j = 0; j < i; j++)
				ASSERT(v.FindLowerBound(j) == 7 * j);
		}
	}
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
#ifdef _DEBUG
	TestLowerBound();
//	TestUpperBound();
//	InVectorTest();
#else
	InVectorScanBenchmark();
//	InVectorBenchmark();
#endif
}
