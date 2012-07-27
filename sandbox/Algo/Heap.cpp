
template <class C>
String HeapAsText(C& heap, int ri = 0, int indent = 0)
{
	String r(' ', 4 * indent);
	if(ri < heap.GetCount()) {
		r << heap[ri] << "\n";
		r << HeapAsText(heap, 2 * ri + 1, indent + 1);
		r << HeapAsText(heap, 2 * ri + 2, indent + 1);
	}
	else
		r << "nil\n";
	return r;
}

template <class C, class T>
void HeapSiftUp(C& heap, T x, int ni)
{
	for(;;) {
		if(ni == 0) {
			heap[0] = x;
			break;
		}
		int pni = (ni - 1) >> 1;
		if(heap[pni] > x)
			heap[ni] = heap[pni];
		else {
			heap[ni] = x;
			break;
		}
		ni = pni;
	}
}

template <class C, class T>
void HeapAdd(C& heap, const T& x)
{
	heap.Add();
	HeapSiftUp(heap, x, heap.GetCount() - 1);
}

template <class C>
void HeapDel(C& heap, int count, int ni)
{
	for(;;) {
		int c1 = 2 * ni + 1;
		int c2 = 2 * ni + 2;
		if(c1 < count) {
			int nni = c2 < count && heap[c2] < heap[c1] ? c2 : c1;
			heap[ni] = heap[nni];
			ni = nni;
		}
		else {
			HeapSiftUp(heap, heap[count - 1], ni);
			break;
		}
	}
}


template <class C>
void HeapRemove(C& heap, int ni = 0)
{
	HeapDel(heap, heap.GetCount(), ni);
	heap.Drop();
}

template <class C>
void HeapSort(C& v)
{
	for(int i = 1; i < v.GetCount(); i++)
		HeapSiftUp(v, v[i], i);
	LOG(HeapAsText(v));
	for(int i = v.GetCount() - 1; i >= 0; i--) {
		C::ValueType x = v[0];
		DDUMP(x);
		HeapDel(v, i + 1, 0);
		v[i] = x;
	}
}

CONSOLE_APP_MAIN
{
	Vector<int> x;
	for(int i = 0; i < 20; i++)
		x.Add(Random(20));
	DDUMPC(x);
	HeapSort(x);
	DDUMPC(x);
	return;

	Vector<int> heap;
	for(int i = 0; i < 20; i++) {
		int n = Random(20);
		DDUMP(n);
		HeapAdd(heap, n);
//		LOG(HeapAsText(heap));
	}
	DDUMPC(heap);
	LOG("============");
	while(heap.GetCount()) {
		DDUMP(heap[0]);
		HeapRemove(heap, 0);
//		LOG(HeapAsText(heap));
	}
}
