#include <Core/Core.h>

using namespace Upp;



struct Test {
	int a, b;
	
	void Jsonize(JsonIO& io) {
		io("a", a)
		  ("b", b);
	}
	
	void Serialize(Stream& s) {
		s % a % b;
	}
};


CONSOLE_APP_MAIN
{
	Array<Test> test;
	for(int i = 0; i < 10; i++) {
		Test t;
		t.a = 1 + i;
		t.b = 23 + i;
		test.Add(t);
	}
	
	String json = StoreAsJson(test);
	RDUMP(json);
	Array<Test> test2;
	LoadFromJson(test2, json);
	RDUMP(StoreAsJson(test2));

#ifdef _DEBUG
	return;
#endif
	
	const int N = 100000;
	for(int i = 0; i < N; i++) {
		Array<Test> test2;
		RTIMING("JSONIZE");
		LoadFromJson(test2, StoreAsJson(test));
	}
	for(int i = 0; i < N; i++) {
		Array<Test> test2;
		RTIMING("Jsonize tree");
		LoadFromJsonTree(test2, StoreAsJsonTree(test));
	}
	Value tree;
	for(int i = 0; i < N; i++) {
		Array<Test> test2;
		RTIMING("StoreAsJsonTree");
		tree = StoreAsJsonTree(test);
	}
	for(int i = 0; i < N; i++) {
		Array<Test> test2;
		RTIMING("LoadFromJsonTree");
		LoadFromJsonTree(test2, tree);
	}
	for(int i = 0; i < N; i++) {
		Array<Test> test2;
		RTIMING("Serialize");
		LoadFromString(test2, StoreAsString(test));
	}
	
	Test tt;
	tt.a = 1; tt.b = 2;
	for(int i = 0; i < 10 * N; i++) {
		RTIMING("Jsonize one");
		Test tt2;
		LoadFromJson(tt2, StoreAsJson(tt));
	}
	for(int i = 0; i < 10 * N; i++) {
		RTIMING("Serialize one");
		Test tt2;
		LoadFromString(tt2, StoreAsString(tt));
	}
}
