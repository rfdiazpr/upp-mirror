#include "Algo.h"

template <class C>
C MergeSort1(const C& src, int l, int h)
{
	C result;
	if(h == l + 1)
		result.Add(src[l]);
	else
	if(h > l) {
		int m = (l + h) / 2;
		C a = MergeSort(src, l, m);
		C b = MergeSort(src, m, h);
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
	return result;
}

template <class T>
void MergeSort2(I array, int count) {
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
