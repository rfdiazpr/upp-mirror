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

void BenchNTL(const char *file, Stream& out) {
	FileIn in(file);
	if (!in) {
		out << "Cannot open input file.\n";
		return;
	}

	SortedVectorMap<String, int> map;
	
	int line = 1;

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
		if(c == '\n')
			++line;
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
	
	int line = 1;

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
		if(c == '\n')
			++line;
	}

	Vector<int> order = GetSortOrder(map.GetKeys());
	for(int i = 0; i < order.GetCount(); i++)
		out << ~map.GetKey(order[i]) << ": " << map[i] << '\n';
}

void BenchSTL(const char *file, Stream& out) {
	FileIn in(file);
	if (!in) {
		out << "Cannot open input file.\n";
		return;
	}

	int line = 1;
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
		if(c == '\n')
			++line;
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

	BenchNTL(fn, NilStream()); // first run to cache the file

	{
		FileOut out(GetHomeDirFile("stl.txt"));
		BenchSTL(fn, out);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchSTL(fn, NilStream());
		Cout() << "std::map<std::string, int> time: " << tm.Elapsed() << " ms \n";
	}

	{
		FileOut out(GetHomeDirFile("stl.txt"));
		BenchSTL2(fn, out);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchSTL2(fn, NilStream());
		Cout() << "std::map<String, int> time: " << tm.Elapsed() << " ms \n";
	}

	{
		FileOut out(GetHomeDirFile("ntl.txt"));
		BenchNTL(fn, out);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchNTL(fn, NilStream());
		Cout() << "SortedVectorMap<String, int> time: " << tm.Elapsed() << " ms\n";
	}

	{
		FileOut out(GetHomeDirFile("ntl2.txt"));
		BenchNTL2(fn, out);
		TimeStop tm;
		for(int n = 0; n < N; n++)
			BenchNTL2(fn, NilStream());
		Cout() << "VectorMap<String, int> time: " << tm.Elapsed() << " ms\n";
	}
	
	ASSERT(LoadFile(GetHomeDirFile("stl.txt")) == LoadFile(GetHomeDirFile("ntl.txt")));
	
	if(LoadFile(GetHomeDirFile("stl.txt")) == LoadFile(GetHomeDirFile("ntl.txt")))
		LOG("MATCH!");
	else
		LOG("ERROR!!!!!");
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	BenchmarkMap();

#if 1
	SortedVectorMap<int, int> map;
	
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
	ASSERT(map.Get(10) == 10);

	DUMPM(map);
#endif
	RDUMP(sizeof(Vector<int>));
	RDUMP(sizeof(InVector<int>));
	RDUMP(sizeof(VectorMap<int, int>));
	RDUMP(sizeof(SortedVectorMap<int, int>));
}
