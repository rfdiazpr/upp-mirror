template <class K, class T, class Less>
void SortedVectorMap<K, T, Less>::Data::Clear()
{
	data.Clear();
}

template <class K, class T, class Less>
void SortedVectorMap<K, T, Less>::Data::Insert(int blki, int pos)
{
	if(ptr)
		data[blki].Insert(pos, *ptr);
	else
		data[blki].Insert(pos);
	ptr = &data[blki][pos];
}

template <class K, class T, class Less>
void SortedVectorMap<K, T, Less>::Data::Split(int blki, int nextsize)
{
	Vector<T>& x = data.Insert(blki + 1);
	x.InsertSplit(0, data[blki], nextsize);
}

template <class K, class T, class Less>
void SortedVectorMap<K, T, Less>::Data::AddFirst()
{
	if(ptr)
		data.Add().Add(*ptr);
	else
		data.Add().Add();
	ptr = &data[0][0];
}

template <class K, class T, class Less>
void SortedVectorMap<K, T, Less>::Data::RemoveBlk(int blki, int n)
{
	data.Remove(blki, n);
}

template <class K, class T, class Less>
void SortedVectorMap<K, T, Less>::Data::Join(int blki)
{
	data[blki].AppendPick(data[blki + 1]);
	data.Remove(blki + 1);
}

template <class K, class T, class Less>
void SortedVectorMap<K, T, Less>::Data::Remove(int blki, int pos, int n)
{
	data[blki].Remove(pos, n);
}

template <class K, class T, class Less>
void SortedVectorMap<K, T, Less>::Shrink()
{
	key.Shrink();
	for(int i = 0; i < value.data.GetCount(); i++)
		value.data[i].Shrink();
}

