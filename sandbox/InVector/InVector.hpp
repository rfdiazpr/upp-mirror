template <class K, class T, class Less>
int SortedVectorMap<K, T, Less>::FindAdd(const K& k)
{
	int i = FindLowerBound(k);
	if(i == GetCount() || less(k, key[i])) {
		key.iv.Insert(i, k);
		value.Insert(i);
	}
	return i;
}

template <class K, class T, class Less>
int SortedVectorMap<K, T, Less>::FindAdd(const K& k, const T& init)
{
	int i = FindLowerBound(k);
	if(i == GetCount() || less(k, key[i])) {
		key.iv.Insert(i, k);
		value.Insert(i, init);
	}
	return i;
}

template <class K, class T, class Less>
int SortedVectorMap<K, T, Less>::RemoveKey(const K& k)
{
	int l = FindLowerBound(k);
	int count = FindUpperBound(k) - l;
	Remove(l, count);
	return count;
}
