#include <Core/Core.h>

using namespace Upp;

/*
template <class C>
C MergeSort(const C& src, int l, int h)
{
	C result;
	if(h == l + 1)
		result.Add(src[l]);
	else
	if(h > l) {
		int m = (l + h) / 2;
		C a = MergeSort(src, l, m);
		DDUMPC(a);
		C b = MergeSort(src, m, h);
		DDUMPC(b);
		int ai = 0;
		int bi = 0;
		while(ai < a.GetCount() && bi < b.GetCount())
			if(a[ai] < b[bi])
				result.Add(a[ai++]);
			else
				result.Add(b[bi++]);
		while(ai < a.GetCount())
			result.Add(a[ai++]);
		while(bi < b.GetCount())
			result.Add(b[bi++]);
	}
	DDUMP(l);
	DDUMP(h);
	DDUMPC(result);
	return result;
}
*/

template <class I>
void MergeSort(I array, int count) {
	int stride = 1;
	while(stride < count) {
		Buffer<int> result(min(2 * stride, count));
		for(int pos = 0; pos < count; pos += 2 * stride) {
			I a = array + pos;
			I ae = min(a + stride, array + count);
			I b = ae;
			I be = min(b + stride, array + count);
			I r = result;
			while(a < ae && b < be)
				if(*a < *b)
					*r++ = *a++;
				else
					*r++ = *b++;
			while(a < ae)
				*r++ = *a++;
			while(b < be)
				*r++ = *b++;
			I t = array + pos;
			for(I s = result; s < r; s++)
				*t++ = *s;
		}
		stride += stride;
	}
}

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

CONSOLE_APP_MAIN
{
	SeedRandom();
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
