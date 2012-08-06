#include <Core/Core.h>

using namespace Upp;

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

struct Node {
	int   value;
	Node *next;
};

Node *RevertList(Node *list, int n)
{
	if(n <= 0)
		return;
	Node *reverted = NULL;
	Node *last = list;
	while(n > 0) {
		Node *h = list->next;
		list->next = reverted;
		reverted = list;
		list = h;
		n--;
	}
	last->next = h;
	return reverted;
}

void DumpList(Node *list)
{
	while(list) {
		LOG(list->value);
		list = list->next;
	}
}

struct Stack {
	struct Node {
		int   value;
		Node *next;
	};
	
	Node *stack;
	
	void push(int value) {
		Node *n = new Node;
		n->value = value;
		n->next = stack;
		stack = n;
	}

	void push(int value) {
		Node *n = new Node;
		n->value = value;
		do
			n->next = stack;
		while(CAS(stack, n->next, n) != n->next);
	}
	
	int  pop() {
		Node *n;
		do
			n = stack;
		while(CAS(stack, n, n->next) != n);
		int v = n->value;
		do_safe_reclamation(n);
//		delete n;
		return v;
	}
	
	Stack() {
		stack = NULL;
	}
};

CONSOLE_APP_MAIN
{
	SeedRandom();
	
	Stack stack;
	stack.push(1);
	stack.push(2);

	LOG(stack.pop());
	LOG(stack.pop());
	return;
	
	Node *list = 0;
	for(int i = 0; i < 10; i++) {
		Node *n = new Node;
		n->value = Random(20);
		n->next = list;
		list = n;
	}
	DumpList(list);
	list = RevertList(list);
	LOG("-------");
	DumpList(list);
	
	while(list) {
		Node *h = list->next;
		delete list;
		list = h;
	}
	return;
	
	Vector<int> heap;
	for(int i = 0; i < 20; i++)
		HeapAdd(heap, (int)Random(100));
	DDUMPC(heap);
	while(heap.GetCount()) {
		DDUMP(heap[0]);
		HeapRemove(heap, 0);
	}
}
