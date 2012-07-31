#include "Algo.h"

#include <vector>

void Inorder(Node *n)
{
	if(n) {
		Inorder(n->left);
		LOG(n);
		Inorder(n->right);
	}
}

void Inorder(Node *n)
{
start:
	if(n) {
	start1:
		SE& x = stack.Add();
		x.n = n;
		x.ret = 1;
		n = n->left;
		goto start;
	ret1:
		LOG(n);
		SE& x = stack.Add();
		x.n = n;
		n = n->right;
		goto start;
	ret2:
	}
	SE x = stack.Pop();
	n = x.n;
	if(x.ret == 1)
		goto ret1;
	else
		goto ret2;
}

void Inorder(Node *n)
{
	
	switch(ip) {
	case 0:
	if(n) {
		Inorder(~n->left);
	case 1:
		LOG(n->value);
		Inorder(~n->right);
	case 2:
	}
}


void Inorder(Node *n)
{
	switch(ip) {
	case 0:
	if(n) {
		Inorder(~n->left);
	case 1:
		LOG(n->value);
		Inorder(~n->right);
	case 2:
		break;
	}
	}

void {
	struct SE {
		Node *n;
		int   next;
	};
	
	Vector<Stack> stack;
	
	SE& e = stack.Add();
	e.n = n;
	e.next = 0;
	
	while(stack) {
		SE x = stack.Pop();
		switch(x.next) {
		case 0:
			x.next = 1;
			x.n = ~x.n->left;
			stack.Add(x);
			break;
		case 1:
			LOG(n->value);
			x.next = 0;
			x.n = ~x.n->right;
			stack.Add(x);
			break;
		}
	}
}

void InorderStack(Node *n)
{
	std::vector<Node *> stack;

	if(n)
		for(;;) {
			while(n->left) {
				stack.push_back(n);
				n = ~n->left;
			}
			LOG(n->value);
			for(;;) {
				if(n->right) {
					n = ~n->right;
					break;
				}
				if(stack.size() == 0)
					return;
				n = stack.back();
				stack.pop_back();
				LOG(n->value);
			}
		}
}

void CreateTree(One<Node>& node, int n)
{
	if(n == 0)
		return;
	node.Create();
	node->value = Random(20);
	if(Random(20) < 15)
		CreateTree(node->left, n - 1);
	if(Random(20) < 15)
		CreateTree(node->right, n - 1);
}

void TestTree()
{
	One<Node> node;
	CreateTree(node, 5);
	Inorder(~node);
	LOG("-----------");
	InorderStack(~node);
}
