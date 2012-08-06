#include <Core/Core.h>

using namespace Upp;

template <class I, class Less>
void IForwardSort2(I begin, I end, const Less& less)
{
	while(begin != end) {
		I best = end;
		I next = end;
		--best;
		if(best == begin)
			break;
		I ptr = best;
		while(ptr != begin) {
			--ptr;
			if(less(*ptr, *best))
				best = ptr;
			else
				next = ptr;
		}
		if(begin == best)
			begin = next;
		else {
			IterSwap(begin, best);
			begin++;
		}
	}
}


template <class I, class Less>
void IForwardSort1(I begin, I end, const Less& less)
{
	if(begin == end)
		return;
	I last = end;
	--last;
	while(!(begin == last)) {
		I best = last;
		I next = last;
		I ptr = last;
		for(;;) {
			if(less(*best, *--ptr)) {  // best holds, scan for better candidate
				do
					if(ptr == begin) { // best is final minimum
						IterSwap(begin, best);
						++begin;
						goto NEXT_ITEM;
					}
				while(less(*best, *--ptr));
				if(ptr == begin) { // begin is final minimum, best is 2nd least
					IterSwap(++begin, best);
					++begin;
					break;
				}
				next = ptr; // mark position after new best as the new end of sorted array
				++next;     // it will hold only if all subseqent iterations define new best (descending order)
			}
			else
			if(ptr == begin) { // begin is least
				begin = next;  // everything is sorted up to next
				break;
			}
			best = ptr;
		}
	NEXT_ITEM:
		;
	}
}

template <class I, class Less>
force_inline
void OrderIter2(I a, I b, Less less)
{
	if(!less(*a, *b))
		IterSwap(a, b);
}

template <class I, class T, class Less>
force_inline
void InsertSortIns__(I l, I i, T& x, Less less)
{
	T temp = x;
	while(i > l && less(temp, *(i - 1))) {
		*i = *(i - 1);
		--i;
	}
	*i = temp;
}

template <class I, class Less>
void Isort(I l, I h, Less less)
{
	for(;;) {
		int count = h - l;
		if(count < 2)
			return;
		if(count < 16) { // Final Insertsort
			for(I i = l + 1; i != h; ++i) 
				InsertSortIns__(l, i, *i, less);		
//			IForwardSort1(l, h, less);
			return;
		}
		int pass = 5;
		LOG("-----------------");
		for(;;) {
			I middle = l + (count >> 1);      // get the middle element
			OrderIter2(l, middle, less);      // sort elements l, middle, h-1 to get median at middle
			OrderIter2(middle, h - 1, less);
			IterSwap(l, middle);              // replace median-pivot with the first element
			I ii = l;
			for(I i = l; i != h; i++)
				if(less(*i, *l))
					IterSwap(++ii, i);
			if(pass > 6 || min(ii - l, h - ii) > (count >> pass)) { // partitions size ok or we done max passes
				IterSwap(ii, l);              // put pivot back in between partitions
				if(ii - l < h - ii - 1) {     // recurse on smaller partition, tail on larger
					Isort(l, ii, less); 
					l = ii + 1;
				}
				else {
					Isort(ii + 1, h, less);
					h = ii;
				}
				break;
			}
			IterSwap(l + 1, l + Random(count)); // try some other random elements for median-pivot
			IterSwap(middle, l + Random(count));
			IterSwap(h - 1, l + Random(count));
			pass++;
		}
	}
}

template <class T, class I, class Less>
void MergeSort__(T&, I array, int count, Less less) {
	int stride = 1;
	while(stride < count) {
		Buffer<T> result(min(2 * stride, count));
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

template <class I, class Less>
void MergeSort(I begin, I end, Less less)
{
	int count = end - begin;
	if(count > 1)
		MergeSort__(*begin, begin, count, less);
}

Vector<String> GetData()
{
//	SeedRandom();
	Vector<String> data;
	int n = Random(500000);
	for(int i = 0; i < n; i++)
		data.Add(AsString(Random(n)));
	return data;
}

template <class C>
void CheckSorted(C& data)
{
	for(int i = 0; i < data.GetCount() - 1; i++) {
		if(!(data[i] <= data[i + 1])) {
			RDUMPC(data);
			RLOG("ERROR! " << i);				
			Exit(1);
		}
	}
}

#ifdef _DEBUG
#define N 1
#else
#define N 30
#endif

CONSOLE_APP_MAIN
{
	SeedRandom();
	Vector<String> sdata = GetData();
	Vector<String> data;
	for(int i = 0; i < N; i++) {
		sdata = GetData();
		data <<= sdata;
		{
			RTIMING("ISort");
			Isort(data.Begin(), data.End(), StdLess<String>());
		}
		CheckSorted(data);
		data <<= sdata;
		{
			RTIMING("MergeSort");
			MergeSort(data.Begin(), data.End(), StdLess<String>());
		}
		CheckSorted(data);
		data <<= sdata;
		{
			RTIMING("StableSort");
			StableSort(data);
		}
		{
			RTIMING("GetSortOrder");
			GetSortOrder(data);
		}
		CheckSorted(data);
		data <<= sdata;
		{
			RTIMING("TrcSort");
			Sort(data);
		}
		data <<= sdata;
		{
			RTIMING("std::sort");
			std::sort(data.Begin(), data.End());
		}
	}
//	RDUMP(nswap);
}
