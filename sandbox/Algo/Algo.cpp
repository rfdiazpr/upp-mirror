#include "Algo.h"

using namespace Upp;

template <class I, class T>
force_inline
void InsertSortIns__(I l, I i, T& x)
{
	T temp = x;
	while(i > l && temp < *(i - 1)) {
		*i = *(i - 1);
		--i;
	}
	*i = temp;
}

template <class I>
void InsertSort(I l, I h)
{
	for(I i = l + 1; i < h; ++i)
		InsertSortIns__(l, i, *i);		
}

template <class I>
void ISort(I l, I h)
{
	for(;;) {
		int count = h - l;
		if(count < 2)
			return;
		if(count == 2) {
			if(l[0] < l[1])
				IterSwap(l, l + 1);
		}
		if(count == 3) {
			if(l[0] < l[1])
				IterSwap(l, l + 1);
			if(l[1] < l[2])
				IterSwap(l + 1, l + 2);
			if(l[0] < l[1])
				IterSwap(l, l + 1);
		}
/*		if(count < 8) {
			ForwardSort(l, h);
			return;
		}
*/		I t = l;
		IterSwap(l, l + count / 2);
		for(I i = l + 1; i < h; ++i)
			if(*i < *l)
				IterSwap(++t, i);
		IterSwap(t, l);
		if(t - l < h - t) {
			ISort(l, t);
			l = t + 1;
		}
		else {
			ISort(t + 1, h);
			h = t;
		}
	}
}

void SortTest()
{
		Vector<String> sdata;
	for(int i = 0; i < 20000; i++)
		sdata.Add() = AsString(Random(500));
#ifdef _DEBUG
//	ISort(sdata.Begin(), sdata.End());
//	DDUMPC(sdata);
//	for(int i = 0; i < sdata.GetCount() - 1; i++)
//		ASSERT(sdata[i] <= sdata[i + 1]);
	return;
#endif
	for(int i = 0; i < 100; i++) {
		Vector<String> data;
		data <<= sdata;
		{
			RTIMING("MergeSort");
//			MergeSort((int *)data, data.GetCount());
		}
		data <<= sdata;
		{
			RTIMING("TrcSort");
			Sort(data);
		}
		data <<= sdata;
		{
			RTIMING("ISort");
			ISort(data.Begin(), data.End());
		}
		data <<= sdata;
		{
			RTIMING("InsertSort");
	//		InsertSort(data.Begin(), data.End());
		}
		data <<= sdata;
		{
			RTIMING("std::sort");
			std::sort(data.Begin(), data.End());
		}
	}
}

CONSOLE_APP_MAIN
{
	SeedRandom();
//	TestTree();
	TestReplace();
}
