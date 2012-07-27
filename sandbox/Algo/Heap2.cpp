template <class C, class T>
void SiftUp(C& heap, int pos, const T& x)
{
	while(pos) {
		int parent = (pos - 1) / 2;
		if(x >= heap[parent])
			break;
		heap[pos] = heap[parent];
		pos = parent;
	}
	heap[pos] = x;
}

template <class C, class T>
void HeapAdd(C& heap, const T& x)
{
	heap.Add();
	SiftUp(heap, heap.GetCount() - 1, x);
}

template <class C>
void HeapRemove(C& heap, int i)
{
	if(heap.GetCount() == 1) {
		heap.Clear();
		return;
	}
	for(;;) {
		int q = 2 * i + 1;
		if(q >= heap.GetCount()) {
			SiftUp(heap, i, heap.Top());
			heap.Drop();
			return;
		}
		q += q + 1 < heap.GetCount() && heap[q + 1] < heap[q];
		heap[i] = heap[q];
		i = q;
	}
}
