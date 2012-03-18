template <class T>
class Order2 {
	Flex<T> data1, data2;

public:
	int      Find(const T& x);
	int      FindAdd(const T& x);
	void     Insert(int i, const T& x);
	const    T& operator[](int i) const  { return i < data1.GetCount() ? data1[i] : data2[i - data1.GetCount()]; }
	int      GetCount() const            { return data1.GetCount() + data2.GetCount(); }
	String   GetInfo() const             { return data1.GetInfo() + " " + data2.GetInfo(); }
};

template <class T>
void Order2<T>::Insert(int i, const T& x)
{
	if(GetCount() == 0)
		data2.Insert(i, x);
	else
	if(i < data1.GetCount() || i == 0)
		data1.Insert(i, x);
	else
		data2.Insert(i - data1.GetCount(), x);
}






/*
template <class T>
int Order2<T>::Find(const T& x)
{
	if(data2.GetCount() == 0 && x < 
	int i = data.FindLowerBound(x);
	return i < GetCount() && data[i] == x ? i : -1;
}

template <class T>
int Order2<T>::FindAdd(const T& x)
{
	int i = data.FindLowerBound(x);
//	LOG("LowerBoind: " << i);
	if(i < GetCount() && data[i] == x) return i;
	data.Insert(i, x);
	return i;
}
*/
