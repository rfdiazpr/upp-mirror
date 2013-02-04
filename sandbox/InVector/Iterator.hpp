
template <class T>
void InVector<T>::SetIter(ConstIterator& it, int ii)
{
	it.v = this;
	it.blki = FindBlock(ii, it.offset);
	it.begin = data[it.blki].Begin();
	it.end = data[it.blki].End();
	it.ptr = it.begin + ii;
}

template <class T>
void InVector<T>::SetBegin(ConstIterator& it)
{
	if(count) {
		it.v = this;
		it.blki = 0;
		it.ptr = it.begin = data[0].Begin();
		it.end = data[0].End();
		it.offset = 0;
	}
	else
		SetEnd(it);
}

template <class T>
void InVector<T>::SetEnd(ConstIterator& it)
{
	if(count) {
		it.v = this;
		it.blki = data.GetCount() - 1;
		it.begin = data.Top().Begin();
		it.ptr = it.end = data.Top().End();
		it.offset = count - data.Top().GetCount();
	}
	else {
		it.v = this;
		it.blki = 0;
		it.ptr = it.begin = it.end = NULL;
		it.offset = 0;		
	}
}

template <typename T>
force_inline typename InVector<T>::ConstIterator& InVector<T>::ConstIterator::operator+=(int d)
{
	if(d >= 0 ? d < end - ptr : -d < ptr - begin)
		ptr += d;
	else
		v->SetIter(*this, GetIndex() + d);
	ASSERT(end - begin == v->data[blki].GetCount());
	return *this;
}

template <typename T>
void InVector<T>::ConstIterator::NextBlk()
{
	ASSERT(end - begin == v->data[blki].GetCount());
	if(blki + 1 < v->data.GetCount()) {
		offset += v->data[blki].GetCount();
		++blki;
		ptr = begin = v->data[blki].Begin();
		end = v->data[blki].End();
	}
}

template <typename T>
void InVector<T>::ConstIterator::PrevBlk()
{
	--blki;
	begin = v->data[blki].Begin();
	ptr = end = v->data[blki].End();
	offset -= v->data[blki].GetCount();
}
