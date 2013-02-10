#include "InVector.h"



CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	SeedRandom();

	SortedIndex<int> si;
	int count = 0;
	for(int i = 0; true /*i < 10000000*/; i++) {
		if(i % 1000 == 0)
			LOG(i);
		if(si.GetCount() > 1000 && Random(1000) == 0) {
			si.Clear();
			count = 0;
		}
		int v = Random(100);
		int h0 = si.FindUpperBound(v);
		int q = si.Add(v);
		count++;
		ASSERT(h0 == q);
		ASSERT(si[q] == v);
		for(int pass = 0; pass < 2; pass++) {
			ASSERT(si.GetCount() == count);
			int l = si.FindLowerBound(v);
			int h = si.FindUpperBound(v);
			ASSERT(l == 0 || si[l - 1] < v);
			ASSERT(l == si.GetCount() || si[l] >= v);
			ASSERT(h == si.GetCount() || si[h] > v);
			ASSERT(h == 0 || si[h - 1] <= v);
			ASSERT(si.GetCount() == 0 || si.FindUpperBound(si.Top()) == si.GetCount());
			for(int i = l; i < h; i++)
				ASSERT(si[i] == v);
			if(si.GetCount() > 500 && Random(40) == 0) {
				count -= h - l;
				si.RemoveKey(v);
				ASSERT(si.FindUpperBound(v) == si.FindLowerBound(v));
			}
			v = Random(100);
		}
	}
	
	DDUMPC(si);
}
