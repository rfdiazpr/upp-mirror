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
