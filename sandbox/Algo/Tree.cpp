#include "Algo.h"

#include <vector>

void Inorder(Node *n)
{
	if(n) {
		Inorder(~n->left);
		LOG(n->value);
		Inorder(~n->right);
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
