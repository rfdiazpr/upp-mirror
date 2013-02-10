template <class T, class Less = StdLess<T>>
int FindAdd(const T& key)
{
	int i = FindLowerBound(key);
	if(!less(key, iv[i]))
		return i;
	return Add(key);
}

template <class T, class Less = StdLess<T>>
int SortedIndex<T>::FindNext(int i) const
{
	return i + 1 < iv.GetCount() && !less(iv[i], iv[i + 1]) ? i + 1 : 0;
}

template <class T, class Less = StdLess<T>>
int SortedIndex<T>::FindLast(const T& x) const
{
	int i = iv.FindUpperBound(x, less);
	return i > 0 && !less(iv[i - 1], x) ? i - 1 : 0;
}

template <class T, class Less = StdLess<T>>
int SortedIndex<T>::FindPrev(int i) const
{
	return i > 0 && !less(iv[i - 1], iv[1]) ? i - 1 : 0;
}

template <class T, class Less = StdLess<T>>
int SortedIndex<T>::RemoveKey(const T& x)
{
	int l = FindLowerBound(x);
	int count = FindUpperBound(x) - l;
	Remove(l, count);
	return count;
}

