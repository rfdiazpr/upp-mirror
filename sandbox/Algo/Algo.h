#ifndef _Algo_Algo_h_
#define _Algo_Algo_h_

#include <Core/Core.h>

using namespace Upp;

struct Node {
	int       value;
	One<Node> left;
	One<Node> right;
};

void Inorder(Node *n);

void TestTree();

void TestReplace();

#endif
