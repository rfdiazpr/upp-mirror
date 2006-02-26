#define  NDEBUG

#include <Core/Core.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <algorithm>
#include <map>
#include <deque>
#include <string>

using namespace std;

void BenchNTL(const char *file) {
	std::ifstream in(file);
	if (!in) {
		std::cout << "Cannot open input file.\n";
		return;
	}
	
	VectorMap<String, Vector<int> > map;
	int line = 1;
	
	for(;;) {
		int c = in.get();
		if(c == EOF) break;
		if(isalpha(c) || c == '_') {
			String id;
			id += c;
			c = in.get();
			while(c != EOF && (isalnum(c) || c == '_')) {
				id.Cat(c);
				c = in.get();
			}
			map.GetAdd(id).Add(line);
		}
		else
		if(isdigit(c))
			do c = in.get();
			while(c != EOF && (isalnum(c) || c == '.'));
		if(c == '\n')
			++line;
	}

	Vector<int> order = GetSortOrder(map.GetKeys());
/* For benchmark purposes, output is omitted
	for(int i = 0; i < order.GetCount(); i++) {
		std::cout << map.GetKey(order[i]) << ": ";
		const Vector<int>& l = map[order[i]];
		for(int i = 0; i < l.GetCount(); i++) {
			if(i) std::cout << ", ";
			std::cout << l[i];
		}
		std::cout << '\n';
	}
*/
}

void BenchSTL(const char *file) {
	std::ifstream in(file);
	if (!in) {
		std::cout << "Cannot open input file.\n";
		return;
	}
	
	map< string, vector<int> > imap;
	int line = 1;
	
	for(;;) {
		int c = in.get();
		if(c == EOF) break;
		if(isalpha(c) || c == '_') {
			string id;
			id += c;
			c = in.get();
			while(c != EOF && (isalnum(c) || c == '_')) {
				id += c;
				c = in.get();
			}
			imap[id].push_back(line);
		}
		else
		if(isdigit(c))
			do c = in.get();
			while(c != EOF && (isalnum(c) || c == '.'));
		if(c == '\n')
			++line;
	}

/* For benchmark purposes, output is omitted
	map< std::string, vector<int> >::const_iterator e = imap.end();
	for(map< std::string, vector<int> >::const_iterator i = imap.begin(); i != e; i++) {
		std::cout << i->first << ": ";
		vector<int>::const_iterator e = i->second.end();
		vector<int>::const_iterator b = i->second.begin();
		for(vector<int>::const_iterator j = b; j != e; j++) {
			if(j != b) std::cout << ", ";
			std::cout << *j;
		}
		std::cout << '\n';
	}
*/
}

CONSOLE_APP_MAIN
{
	String fn;
	int argc = CommandLine().GetCount();
	const Vector<String>& argv = CommandLine();
	if(argc < 1)
		fn = GetDataFile("main.cpp");
	else
		fn = argv[0];

	int n;

	BenchSTL(fn);// One run to cache the file

	{
		BenchSTL(fn);
		TimeStop tm;
		for(n = 0; n < 10000; n++)
			BenchSTL(fn);
		cout << "STL time: " << tm.Elapsed() << " ms \n";
	}

	{
		BenchNTL(fn);
		TimeStop tm;
		for(n = 0; n < 10000; n++)
			BenchNTL(fn);
		cout << "NTL time: " << tm.Elapsed() << " ms\n";
	}
}
