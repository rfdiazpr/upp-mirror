template <class K, class T, class Less, class Data>
SortedAMap<K, T, Less, Data>::SortedAMap(const SortedAMap& s, int)
:	key(s.key, 0), value(s.value, 0)
{
	key.iv.slave = &value;
}

template <class T>
void Slaved_InVector__<T>::Insert(int blki, int pos)
{
	if(ptr)
		data.data[blki].Insert(pos, *ptr);
	else
		data.data[blki].Insert(pos);
	ptr = &data.data[blki][pos];
}

template <class T>
void Slaved_InVector__<T>::Split(int blki, int nextsize)
{
	Vector<T>& x = data.data.Insert(blki + 1);
	x.InsertSplit(0, data.data[blki], nextsize);
}

template <class T>
void Slaved_InVector__<T>::AddFirst()
{
	if(ptr)
		data.data.Add().Add(*ptr);
	else
		data.data.Add().Add();
	ptr = &data.data[0][0];
}

template <class T>
void Slaved_InArray__<T>::Insert(int blki, int pos)
{
	T *x = ptr ? new T(*ptr) : new T();
	data.iv.data[blki].Insert(pos, x);
	ptr = x;
}

template <class T>
void Slaved_InArray__<T>::Split(int blki, int nextsize)
{
	Vector< typename InArray<T>::PointerType >& x = data.iv.data.Insert(blki + 1);
	x.InsertSplit(0, data.iv.data[blki], nextsize);
}

template <class T>
void Slaved_InArray__<T>::AddFirst()
{
	T *x = ptr ? new T(*ptr) : new T();
	data.iv.data.Add().Add(x);
	ptr = x;
}
