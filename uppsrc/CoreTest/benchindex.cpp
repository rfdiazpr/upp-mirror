#include <Core/Core.h>

static int mask = 255;
static int nb = 257;

int Test1(int q)
{
	return q % nb;
}

int xorfold5(int h)
{
	return int(((h >> 7) ^ (h >> 22) ^ /*(h >> 15) ^ (h >> 20) ^ (h >> 25) ^*/ h) & mask);
}

int (*xorfold)(int) = xorfold5;

int Test2(int h)
{
	return xorfold(h);
}

int TestX(unsigned h)
{
	int r = 0;
	for(int i = 0; i < 5; i++) {
		r = r ^ (h & 255);
		h >>= 8;
	}
	return r & mask;
}

static String h = "Hello";

int Test3(const String& s)
{
	return s == h;
}



/*
#include <Core/Core.h>

#include <vector>
#include <map>
#include <set>
#include <algorithm>

#define NUM 2000000
#define LN  15000
#define HN  100

Vector<int> rnd;

template <class T>
void Bench(const char *name, T& ndx)
{
	int i;
	RLOG(name << " -------");
	RDUMP(sizeof(ndx));
	TimeStop();
	for(i = 0; i < NUM; i++)
		ndx.Add(i);
	RLOG("  Add: " << TimeStop());
	ndx.Find(123);
	RLOG("  First Find: " << TimeStop());
	int s = 0;
	for(i = 0; i < NUM; i++)
		s += ndx.Find(i);
	RLOG("  Finding: " << TimeStop());
	for(i = NUM; i < NUM + 3 * NUM / 4; i++)
		ndx.Add(i);
	RLOG("  2nd Add: " << TimeStop());
	ndx.Find(123);
	RLOG("  2nd First Find: " << TimeStop());
	for(i = 0; i < NUM; i++) {
		ASSERT(ndx.Find(i) == i);
		s += ndx.Find(i);
	}
	RLOG("  2nd Finding: " << TimeStop());
	for(i = 0; i < NUM; i++) {
		s += ndx.Find(i);
		ndx.Add(i);
	}
	RLOG("  Add & Find: " << TimeStop());
	ndx.Clear();
	for(i = 0; i < NUM; i++) {
		ndx.Add(i);
		ndx.Add(i);
		s += ndx.Find(i);
	}
	RLOG("  2x Add & Find: " << TimeStop());
	
	if(rnd.GetCount() == 0)
		for(i = 0; i < LN; i++)
			rnd.Add(rand() & 8191);
	
	TimeStop();
	int w;
	for(w = 0; w < HN; w++) {
		ndx.Clear();
		for(i = 0; i < LN; i++) {
			s += ndx.Find(rnd[i]);
			ndx.Add(rnd[i]);
		}
	}
	RLOG("  L Add & Find: " << TimeStop());
	for(w = 0; w < HN; w++) {
		ndx.Clear();
		for(i = 0; i < LN; i++) {
			s += ndx.Find(rnd[i]);
			ndx.Add(rnd[i]);
			ndx.Add(rnd[i]);
		}
	}
	RLOG("  L 2x Add & Find: " << TimeStop());
	RLOG("  Result: " << s);
	RLOG("");
}

#define XL 1000 * 1000
//#define XL 10000

int Rand()
{
	return rand() ^ (rand() << 14);
}

void Test(int mask)
{
	int i;
	RLOG("------- " << mask);
	Vector<int> rnd1, rnd2;
	for(i = 0; i < XL; i++) {
		rnd1.Add(Rand() & mask);
		rnd2.Add(Rand() & mask);
	}
	{
		RLOG("--- STL");
		std::multiset<int> b;
		TimeStop();
		int n = 0;
		for(i = 0; i < XL; i++) {
			b.insert(rnd1[i]);
			n += b.erase(rnd2[i]);
		}
		RDUMP(TimeStop());
		RDUMP(n);
	}
	RLOG("");
}

void MainC()
{
	Index<int> alfa;
	alfa.Add(1);
	alfa.Find(1);
	alfa.Find(1);
	int i;
	int n = 0;
	Vector<int> rnd1, rnd2;
	for(i = 0; i < XL; i++) {
		rnd1.Add(rand() & 15);
		rnd2.Add(rand() & 15);
	}
	
	RDUMP(sizeof(VectorMap<int, int>));

	Bench("Index", Index<int>());
	RLOG("--- DeleteKey");
}

void Search(Index<int>& ndx, int n)
{
	LOG("----- Search for " << n);
	int q = ndx.Find(n);
	while(q >= 0) {
		LOG(q << " = " << ndx[q]);
		q = ndx.FindNext(q);
	}
}

void MainB()
{
	int i, j;
	Index<int> ndx;
	for(i = 0; i < 100; i++)
		ndx.Add(i % 9);
	ndx.RemoveKey(2);
	DUMPC(ndx);
	for(i = 10; i < 20; i++)
		ndx.Set(i, 33);
	DUMPC(ndx);
	Search(ndx, 33);
	for(i = 20; i >= 10; i--)
		ndx.Set(i, 44);
	DUMPC(ndx);
	Search(ndx, 33);
	Search(ndx, 44);
	Search(ndx, 4);
	ndx.Unlink(4);
	Search(ndx, 4);
	for(i = 30; i < 70; i++)
		ndx.Unlink(i);
	Search(ndx, 4);
	for(i = 0; i < ndx.GetCount(); i++)
		if(!ndx.IsUnlinked(i))
			ndx.Unlink(i);
	Search(ndx, 4);
	for(i = 20; i < 30; i++)
		ndx.Set(i, 5);
	Search(ndx, 5);
	DUMPC(ndx);
	ndx.Sweep();
	DUMPC(ndx);
}

template <class T>
void TestVector(T& v)
{
	int i;
	for(i = 0; i < 100; i++)
		v.Add(i);
	T help;
	help <<= v;
	for(i = 0; i < 10000; i++)
		v.Add(i);
	for(i = 0; i < v.GetCount(); i += 10)
		v.Insert(i, 3, 3);
	for(i = 0; i < v.GetCount(); i += 137)
		v.Insert(i, help);
	int q = 10;
	for(i = 0; i < v.GetCount() - q; i += q) {
		v.Remove(i, q);
		q++;
	}
	v.InsertPick(30, help);
}

	struct Alfa {
		int a;
		byte q[33];
	};

template class Moveable<Alfa>;

template <class T, class V>
void Benchmark(const char *name, T& v, V val)
{
	TimeStop();
	{
		for(int j = 0; j < N; j++) {
			v.Clear();
			for(int i = 0; i < 1000 * 1000; i++)
				v.Add(val);
		}
	}
	RLOG(name << "adding " << TimeStop());
	{
		for(int j = 0; j < N; j++) {
			v.Clear();
			for(int i = 0; i < 1000 * 1000; i++)
				v.Add() = val;
		}
	}
	RLOG(name << "dc adding " << TimeStop());
	int s = 0;
	for(int j = 0; j < N; j++) {
		for(int i = 0; i < v.GetCount(); i++)
			if(v[i] == val)
				s++;
	}
	RLOG(name << "iterating " << TimeStop());
	for(j = 0; j < N; j++) {
		T::Iterator q = v.Begin();
		T::Iterator end = v.End();
		while(q != end) {
			if(*q == val)
				s++;
			++q;
		}
	}
	RLOG(name << "Iterator iterating " << TimeStop());
	RLOG("");
}


class SBuffer {
	struct Data {
		int   refcount;
		int   length;
		int   alloc;
	};

	char  *ptr;
	char  *end;
	char  *lim;

	static char *Alloc(int len);
	void  Expand();
	void  Free()          { delete[] (byte *) ((Data *)ptr - 1); }
	Data *GetData()       { return (Data *)ptr - 1; }

public:
	void Cat(int c)       { if(end >= lim) Expand(); *end++ = c; }
	int  GetLength()      { return end - ptr; }

	SBuffer()             { ptr = end = Alloc(8); lim = ptr + 8; }
	~SBuffer()            { Free(); }
};

char *SBuffer::Alloc(int len)
{
	Data *d = (Data *) new byte[sizeof(Data) + len + 1];
	return (char *)(d + 1);
}

void SBuffer::Expand()
{
	int nl = max(2 * (lim - ptr), 8);
	char *p = Alloc(nl);
	memcpy(p, ptr, GetLength());
	int pos = end - ptr;
	Free();
	ptr = p;
	end = ptr + pos;
	lim = ptr + nl;
}
*/
