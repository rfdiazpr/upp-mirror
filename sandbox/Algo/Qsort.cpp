#include <Core/Core.h>

using namespace Upp;

template <class I, class Less>
force_inline
void OrderIter2(I a, I b, Less less)
{
	if(!less(*a, *b))
		IterSwap(a, b);
}

template <class I, class Less>
void ForwardSort(I begin, I end, const Less& less)
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
			if(less(*best, *--ptr)) { // best holds, scan for better candidate
				do
					if(ptr == begin) { // best is least
						IterSwap(begin, best);
						++begin;
						goto NEXT_ITEM;
					}
				while(less(*best, *--ptr));
				if(ptr == begin) { // begin is least, best is 2nd least
					IterSwap(++begin, best);
					++begin;
					break;
				}
				next = ptr; // mark position after new best as the new end of sorted array
				++next;     // it will hold only if all subseqent iterations define new best (descending order)
			}
			else
			if(ptr == begin) { // begin is least
				begin = next; // everything is sorted up to next
				break;
			}
			best = ptr;
		}
	NEXT_ITEM:
		;
	}
}

template <class I, class Less>
void Isort0(I l, I h, Less less)
{
	for(;;) {
		int count = h - l;
		if(count < 2)
			return;
/*		if(count == 3) {
			OrderIter2(l, l + 1, less);
			OrderIter2(l + 1, l + 2, less);
			OrderIter2(l, l + 1, less);
			return;
		}
		if(count == 2) {
			OrderIter2(l, l + 1, less);
			return;
		}
*/
		if(count < 7) {
			ForwardSort(l, h, less);
			return;
		}
		bool tryagain = true;
		LOG("-----------------");
		for(;;) {
			I pi = l + ((h - l) >> 1);
			if(count > 30) {
				OrderIter2(l, pi, less);
				OrderIter2(pi, h, less);
			}
			IterSwap(l, pi);
			I ii = l;
			for(I i = l + 1; i != h; i++)
				if(less(*i, *l))
					IterSwap(++ii, i);
			IterSwap(ii, l);
			DUMP(ii - l);
			DUMP(h - ii);
			if(min(ii - l, h - ii) > count / 16 || !tryagain) {
				if(ii - l < h - ii - 1) {
					Isort(l, ii, less);
					l = ii + 1;
				}
				else {
					Isort(ii + 1, h, less);
					h = ii;
				}
				break;
			}
			IterSwap(l, l + Random(count));
			IterSwap(pi, l + Random(count));
			IterSwap(h - 1, l + Random(count));
			tryagain = false;
			LOG("AGAIN!");
			DUMP(min(ii - l, h - ii));
			DUMP(count / 16);
			DUMP(count);			
		}
	}
}

Vector<String> GetData()
{
	SeedRandom();
	Vector<String> data;
	for(int i = 0; i < 5000; i++)
		data.Add(AsString(Random(20000)));
	return data;
}

#ifdef _DEBUG
#define N 1
#else
#define N 10000
#endif

void IterSortA(String *a)
{
	if(!(a[0] < a[1]))
		IterSwap(a, a + 1);
}

void IterSortB(String *a)
{
	IterSwap(a, a + !(a[0] < a[1]));
}

CONSOLE_APP_MAIN
{
	Vector<String> sdata = GetData();
	Vector<String> data;
	for(int i = 0; i < N; i++) {
		data <<= sdata;
		{
			RTIMING("ISort");
			Isort(data.Begin(), data.End(), StdLess<String>());
		}
		for(int i = 0; i < data.GetCount() - 1; i++) {
			if(!(data[i] <= data[i + 1])) {
				RDUMPC(data);
				RLOG("ERROR!");
				Exit(1);
			}
		}
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
