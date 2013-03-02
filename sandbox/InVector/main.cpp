#include "InVector.h"

#define TEST_HASHMAP

#ifdef TEST_HASHMAP

#ifdef COMPILER_GCC
#include <tr1/unordered_map>
#else
#include <hash_map>
#endif

#include <map>
#include <string>
#include <vector>

#endif

using namespace std;
using namespace Upp;

template <class C>
void BenchNTL(const char *file, Stream& out) {
	FileIn in(file);
	if (!in) {
		out << "Cannot open input file.\n";
		return;
	}

	C map;
	
	for(;;) {
		int c = in.Get();
		if(c < 0) break;
		if(IsAlpha(c) || c == '_') {
			String id;
			id.Cat(c);
			c = in.Get();
			while(c >= 0 && (IsAlNum(c) || c == '_')) {
				id.Cat(c);
				c = in.Get();
			}
			map.GetAdd(id, 0)++;
		}
		else
		if(IsDigit(c))
			do c = in.Get();
			while(c >= 0 && (IsAlNum(c) || c == '.'));
	}

	for(int i = 0; i < map.GetCount(); i++)
		out << ~map.GetKey(i) << ": " << map[i] << '\n';
}

void BenchNTL2(const char *file, Stream& out) {
	FileIn in(file);
	if (!in) {
		out << "Cannot open input file.\n";
		return;
	}

	VectorMap<String, int> map;
	
	for(;;) {
		int c = in.Get();
		if(c < 0) break;
		if(IsAlpha(c) || c == '_') {
			String id;
			id.Cat(c);
			c = in.Get();
			while(c >= 0 && (IsAlNum(c) || c == '_')) {
				id.Cat(c);
				c = in.Get();
			}
			map.GetAdd(id, 0)++;
		}
		else
		if(IsDigit(c))
			do c = in.Get();
			while(c >= 0 && (IsAlNum(c) || c == '.'));
	}

	Vector<int> order = GetSortOrder(map.GetKeys());
	for(int i = 0; i < order.GetCount(); i++)
		out << ~map.GetKey(order[i]) << ": " << map[order[i]] << '\n';
}

void BenchSTL(const char *file, Stream& out) {
	FileIn in(file);
	if (!in) {
		out << "Cannot open input file.\n";
		return;
	}

	map<string, int> imap;

	for(;;) {
		int c = in.Get();
		if(c == EOF) break;
		if(isalpha(c) || c == '_') {
			string id;
			id += c;
			c = in.Get();
			while(c != EOF && (isalnum(c) || c == '_')) {
				id += c;
				c = in.Get();
			}
			imap[id]++;
		}
		else
		if(isdigit(c))
			do c = in.Get();
			while(c != EOF && (isalnum(c) || c == '.'));
	}
	map< std::string, int >::const_iterator e = imap.end();
	for(map< std::string, int >::const_iterator i = imap.begin(); i != e; i++)
		out << i->first.c_str() << ": " << i->second << '\n';
}

void BenchSTL2(const char *file, Stream& out) {
	FileIn in(file);
	if (!in) {
		out << "Cannot open input file.\n";
		return;
	}

	int line = 1;
	map<String, int> imap;

	for(;;) {
		int c = in.Get();
		if(c == EOF) break;
		if(isalpha(c) || c == '_') {
			string id;
			id += c;
			c = in.Get();
			while(c != EOF && (isalnum(c) || c == '_')) {
				id += c;
				c = in.Get();
			}
			imap[id]++;
		}
		else
		if(isdigit(c))
			do c = in.Get();
			while(c != EOF && (isalnum(c) || c == '.'));
		if(c == '\n')
			++line;
	}
	map< String, int >::const_iterator e = imap.end();
	for(map< String, int >::const_iterator i = imap.begin(); i != e; i++)
		out << i->first << ": " << i->second << '\n';
}

#ifdef _DEBUG
#define N 0
#else
#define N 10
#endif

void BenchmarkMap()
{
	String fn;
	int argc = CommandLine().GetCount();
	const Vector<String>& argv = CommandLine();
	if(argc < 1)
#ifdef _DEBUG
		fn = GetDataFile("main.cpp");
#else
		fn = GetHomeDirFile("test.txt");
#endif
	else
		fn = argv[0];

	BenchSTL(fn, NilStream()); // first run to cache the file

	{
		FileOut out(GetHomeDirFile("stl.txt"));
		BenchSTL(fn, out);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchSTL(fn, NilStream());
		RLOG("std::map<std::string, int> time: " << tm.Elapsed() << " ms");
	}

	{
		FileOut out(GetHomeDirFile("stl.txt"));
		BenchSTL2(fn, out);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchSTL2(fn, NilStream());
		RLOG("std::map<String, int> time: " << tm.Elapsed() << " ms");
	}

	{
		FileOut out(GetHomeDirFile("ntl.txt"));
		BenchNTL< SortedVectorMap<String, int> >(fn, out);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchNTL< SortedVectorMap<String, int> >(fn, NilStream());
		RLOG("SortedVectorMap<String, int> time: " << tm.Elapsed() << " ms");
	}

	{
		FileOut out(GetHomeDirFile("ntla.txt"));
		BenchNTL< SortedArrayMap<String, int> >(fn, out);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchNTL< SortedArrayMap<String, int> >(fn, NilStream());
		RLOG("SortedArrayMap<String, int> time: " << tm.Elapsed() << " ms");
	}

	{
		FileOut out(GetHomeDirFile("ntl2.txt"));
		BenchNTL2(fn, out);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchNTL2(fn, NilStream());
		RLOG("VectorMap<String, int> time: " << tm.Elapsed() << " ms");
	}
	
	ASSERT(LoadFile(GetHomeDirFile("stl.txt")) == LoadFile(GetHomeDirFile("ntl.txt")));
	
	if(LoadFile(GetHomeDirFile("stl.txt")) == LoadFile(GetHomeDirFile("ntl.txt")) &&
	   LoadFile(GetHomeDirFile("ntl2.txt")) == LoadFile(GetHomeDirFile("ntl.txt")))
		RLOG("MATCH!");
	else {
		RLOG("ERROR!!!!!");
		__BREAK__;
	}
}

template <class T>
void Test(T& map)
{
	map.Add(1, 10);
	map.Add(20, 20);
	map.Add(12, 20);
	map.Add(10, 11);
	map.Add(12) = 21;
	map.Add(1) = 2;
	
	map.GetAdd(12) = 1234;
	
	DUMPM(map);
	
	DUMP(map.FindAdd(4, 2));
	
	DUMPM(map);
	
	map.GetAdd(13) = 1313;
	
	ASSERT(map.Get(13) == 1313);
	ASSERT(map.Get(1) == 10);
	ASSERT(map.Get(10) == 11);
	ASSERT(map.GetAdd(10) == 11);
	ASSERT(map.GetAdd(15, 4321) == 4321);
	
	for(int i = 0; i < 1000; i++)
		map.Add(i, Random(1000));

	for(int i = 0; i < 1000; i++)
		map.FindAdd(i, Random(1000));
	
	typename T::Iterator it = map.Begin();
	typename T::KeyConstIterator ckit = map.KeyBegin();
	typename T::ConstIterator cit = map.Begin();
	for(int i = 0; i < map.GetCount(); i++) {
		ASSERT(map.GetKey(i) == *ckit++);
		ASSERT(map[i] == *it++);
		ASSERT(map[i] == *cit++);

		typename T::Iterator q = map.GetIter(i);
		ASSERT(map[i] == *q);
		typename T::KeyConstIterator kq = map.KeyGetIter(i);
		ASSERT(map.GetKey(i) == *kq);
	}
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	BenchmarkMap();

	LOG("===========================");
	LOG("VectorMap");
	SortedVectorMap<int, int> map;
	Test(map);

	LOG("===========================");
	LOG("ArrayMap");
	SortedArrayMap<int, int> amap;
	Test(amap);

	LOG("------------");
	
	SortedArrayMap<int, int, StdGreater<int> > am;
	am.Add(1, 11);
	am.Add(2) = 12;
	am.Add(3, new int(13));
	am.Create<int>(4) = 14;
	am.GetAdd(5) = 15;
	am.GetAdd(6, 0);
	am.GetAdd(6, 1) = 16;
	am.GetAdd(7, 17);
	
	DDUMPM(am);

	ASSERT(am.GetCount() == 7);	
	for(int i = 0; i < am.GetCount(); i++) {
		ASSERT(am.GetKey(i) == am[i] - 10);
	}

	am.Remove(0);

	DDUMPM(am);
	
	ASSERT(am.GetCount() == 6);	
	for(int i = 0; i < am.GetCount(); i++)
		ASSERT(am.GetKey(i) + 1 == am[i] - 10 + 1);
	
	delete am.Detach(0);

	DDUMPM(am);

	ASSERT(am.GetCount() == 5);	
	for(int i = 0; i < am.GetCount(); i++)
		ASSERT(am.GetKey(i) + 2 == am[i] - 10 + 2);
	
	RDUMP(sizeof(Vector<int>));
	RDUMP(sizeof(Array<int>));
	RDUMP(sizeof(Index<int>));
	RDUMP(sizeof(VectorMap<int, int>));
	RDUMP(sizeof(InVector<int>));
	RDUMP(sizeof(InArray<int>));
	RDUMP(sizeof(SortedIndex<int>));
	RDUMP(sizeof(SortedVectorMap<int, int>));
}
