template <class T>
template <class L>
int InVector<T>::FindUpperBound(const T& val, const L& less, int& off, int& pos)
{
//	RTIMING("FindUpperBound");
	if(data.GetCount() == 0) {
		pos = off = 0;
		return 0;
	}
	int blki = 0;
	int ii = 0;
	int offset = 0;
	int half = hcount;
	for(int i = index.GetCount(); --i >= 0;) {
		int m = blki + half;
		if(m - 1 < data.GetCount() && !less(val, data[m - 1].Top())) {
			blki = m;
			offset += index[i][ii];
			ii++;
		}
		ii += ii;
		half >>= 1;
	}
	if(blki < data.GetCount()) {
		if(!less(val, data[blki].Top()))
			offset += data[blki++].GetCount();
		if(blki < data.GetCount()) {
			pos = Upp::FindUpperBound(data[blki], val, less);
			off = offset;
			SetCache(blki, offset);
			return blki;
		}
	}
	pos = data.Top().GetCount();
	off = count - pos;
	blki--;
	return blki;
}

template <class T>
template <class L>
int InVector<T>::FindLowerBound(const T& val, const L& less, int& off, int& pos)
{
	if(data.GetCount() == 0) {
		pos = off = 0;
		return 0;
	}
	int blki = 0;
	int ii = 0;
	int offset = 0;
	int half = hcount;
	for(int i = index.GetCount(); --i >= 0;) {
		int m = blki + half;
		if(m < data.GetCount() && less(data[m][0], val)) {
			blki = m;
			offset += index[i][ii];
			ii++;
		}
		ii += ii;
		half >>= 1;
	}
	if(blki < data.GetCount()) {
		if(blki + 1 < data.GetCount() && less(data[blki + 1][0], val))
			offset += data[blki++].GetCount();
		if(blki < data.GetCount()) {
			pos = Upp::FindLowerBound(data[blki], val, less);
			off = offset;
			SetCache(blki, offset);
			return blki;
		}
	}
	pos = data.Top().GetCount();
	off = count - pos;
	blki--;
	return blki;
}

template <class T>
template <class L>
int InVector<T>::InsertUpperBound(const T& val, const L& less)
{
	if(data.GetCount() == 0) {
		count++;
		ClearCache();
		data.Add().Insert(0) = val;
		return 0;
	}
	int off;
	int pos;
	int blki = FindUpperBound(val, less, off, pos);
//	RTIMING("Insert0");
	Insert0(off + pos, blki, pos, off, &val);
	return off + pos;
}

template <class T>
template <class L>
int InVector<T>::Find(const T& val, const L& less)
{
	int i = FindLowerBound(val, less);
	return i < GetCount() && !less(val, (*this)[i]) ? i : -1;
}
