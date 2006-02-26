#include <Core/Core.h>
#include <algorithm>

template <class T>
void benchmark_VectorMap(const Vector<T>& key, int count)
{
	TimeStop();
	RLOG(count << " items");

		int i;
		VectorMap<T, int> map;
		TimeStop();
		for(i = 0; i < count; i++)
			map.Add(key[i], i);
		RLOG("Adding done in " << TimeStop() << " s");
		for(int j = 0; j < 2; j++) {
			for(i = 0; i < count; i++)
				if(map.Get(key[i]) != i) {
					RLOG("ERROR !\n");
					abort();
				}
			RLOG("Search done in " << TimeStop() << " s");
		}
		for(i = 0; i < count; i++)
			map.UnlinkKey(key[i]);
		RLOG("Unlinking done in " << TimeStop() << " s");
		for(i = 0; i < count; i++)
			map.Put(key[i], i);
		RLOG("Putting done in " << TimeStop() << " s");
	
	RLOG("---------------------------------------------");
	RLOG("");
}

void Main()
{
	RDUMP(sizeof(Vector<int>()));
	RDUMP(sizeof(BiVector<int>()));
	RDUMP(sizeof(BiArray<int>()));
	RDUMP(sizeof(Array<int>()));
	RDUMP(sizeof(Index<int>()));
	RDUMP(sizeof(ArrayIndex<int>()));
	RDUMP(sizeof(VectorMap<int, int>()));
	RDUMP(sizeof(ArrayMap<int, int>()));
	RDUMP(sizeof(Point));
	
	DUMP(sizeof(String));
	
	String a;
	a = "Ahoj";
	String b = a;
	b =a;
	a = b + a;
	RDUMP(a);
	String x = "Bejby";
	RDUMP(x);
	a += x;
	b << a;
	RDUMP(a);
	RDUMP(b);
	
	Value q;
	q = a;


	for(int i = 1; i <= 12; i++) {
		int m = i < 4 ? 10 : (i - 4) / 3 * 3 + 1;
		LOG(i << ": " << Date(2000, m, 1) << " - " << LastDayOfMonth(Date(2000, m + 2, 1)));
	}

/*
#ifdef INDEX_MOD
	RLOG("INDEX_MOD version");
#else
	RLOG("static xor-folding version");
#endif
	RLOG("---------------------------------------------");
	RLOG("");

	String h = "tst";

	Vector<String> key;
	for(int i = 0; i < 1000000; i++)
		key.Add(FormatIntAlpha(i));
	std::random_shuffle(key.Begin(), key.End());
	
	for(i = 0; i < 5; i++)
		RLOG(key[i]);

	Vector<String> skey, skey2;
	for(i = 0; i < 1000000; i++) {
		skey.Add(key[i]);
		skey2.Add(key[i]);
	}

	{
		Vector<int> key;
		for(int i = 0; i < 1000000; i++)
			key.Add(i);
		std::random_shuffle(key.Begin(), key.End());
		
		RLOG("VectorMap<int, int> benchmark ");
		benchmark_VectorMap(key, 1000000);
		RLOG("VectorMap<int, int> benchmark ");
		benchmark_VectorMap(key, 100000);
	}

	{
		RLOG("VectorMap<String, int> benchmark");
		benchmark_VectorMap(skey,  1000000);

		RLOG("VectorMap<String, int> benchmark ");
		benchmark_VectorMap(skey, 100000);
	}

	FileIn in("v:\\ntl\\detail\\Algo.h");;
	Vector<String> line;
	while(!in.IsEof()) {
		String ln = in.GetLine();
		line.Add(ln);
		line.Add(ln);
		line.Add(ln);
	}
	
	std::random_shuffle(line.Begin(), line.End());
	std::random_shuffle(line.Begin(), line.End());
	std::random_shuffle(line.Begin(), line.End());

	TimeStop();
	for(i = 0; i < 1000; i++) {
		Index<String> x;
		for(int j = 0; j < line.GetCount(); j++)
			x.FindAdd(line[j]);
	}
	RLOG("Creating set of inputfile lines using Index done in " << TimeStop() << " ms");
*/
}
