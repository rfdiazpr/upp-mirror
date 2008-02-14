#ifndef _Docking_SplitterTree_h_
#define _Docking_SplitterTree_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

class SplitterTree : public Ctrl
{
public:	
	typedef SplitterTree CLASSNAME;

	virtual void Paint(Draw &w);
	virtual void Layout()							{ LayoutNode(root, GetView()); }
	virtual void LeftDown(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual Image CursorImage(Point p, dword keyflags);
	
	virtual void ChildAdded(Ctrl *child) 			{ if (!internal_block && child->GetParent() == this) AddRoot0(*child, false, Size(Null)); }
	virtual void ChildRemoved(Ctrl *child) 			{ if (!internal_block && child->GetParent() == this) RemoveFromTree(*child); }
protected:	
	struct Node
	{
		Node *	parent;
		Node *	first, *last;
		Node *	next, *prev;
		bool 	vert:1;
		int  	sz;
		bool 			IsEmpty()	{ return first == NULL; }
		virtual bool 	IsLeaf()	{ return false; }		
		virtual Ctrl *	GetCtrl()	{ return NULL; }
		Node() : parent(NULL), first(NULL), last(NULL), next(NULL), prev(NULL), vert(false), sz(0)
			{ }		
	};
	
	struct Leaf : public Node {
		Ctrl *ctrl;
		
		virtual bool 	IsLeaf()	{ return true; }
		virtual Ctrl *	GetCtrl()	{ return ctrl; }
		Leaf(Ctrl *c = NULL) : ctrl(c)	{ }
	};
	
	Node *	root;
	Node *	dragprev, *dragnext;
	Point 	lastdragpos;
	bool 	internal_block:1;
	
	Rect 	PaintNode(Draw &w, Node *p);
	void	LayoutNode(Node *p, Rect r);
	Rect 	GetNodeRect(Node *n);
	Node *	MouseInBar(const Point &pt, Node *p, Rect r, bool store_nodes);
	Rect	DragHlRect();

	Size 	HVSize(bool vert, const Size &a, const Size &b);
	Size 	GetNodeMinSize(Node *n);
	Size 	GetNodeMaxSize(Node *n);
	void	ReconcileChildSizes(Node *p);
	void	SetNodeSize(Node *n, const Size &sz);
	void 	Relayout();

	Leaf *	NewLeaf(Ctrl &c)					  	{ Leaf *l = new Leaf(&c); internal_block = true; Add(c); internal_block = false; return l; }
	Node *	NewNode()								{ return new Node(); }
	void	DeleteNode(Node *n)						{ if (n->GetCtrl()) { internal_block = true; RemoveChild(n->GetCtrl()); internal_block = false; } delete n; }

	int 	ChildCount(Node *n)					  	{ int ix = 0; for (Node *t = n->first; t; t = t->next) { ix++; } return ix; }	
	Leaf *	FindCtrl(Node *r, Ctrl *c) const;

	Node * 	AddBA(bool before, Node *p, Node *n)	{ return (before ? AddBefore(p, n) : AddAfter(p, n)); }
	Node * 	AddFirst(Node *p, Node *n);
	Node * 	AddLast(Node *p, Node *n);
	Node * 	AddBefore(Node *s, Node *n);
	Node * 	AddAfter(Node *s, Node *n);
	void	AddRoot0(Ctrl &ctrl, bool first, const Size &sz);
	
	void 	DestroyTree(Node *p);
	void 	RemoveND(Node *l);
	void 	RemoveFromTree(Ctrl &ctrl);
	void	CleanUpTree(Node *n);
	
	void 	RefreshNode(Node *n)					{ Refresh(GetNodeRect(n)); }
	int 	GetDepth(Node *p, int depth) const		{ int d = depth; for (Node *n = p->first; n; n = n->next) { depth = max(depth, GetDepth(n, d)); } return depth+1; }
public:
	Ctrl &	AddSibling(bool before, Ctrl &sibling, Ctrl &c, Size sz = Null);
	Ctrl & 	AddSiblingOver(bool vert, bool before, Ctrl &sibling, Ctrl &c, Size sz = Null);
	Ctrl & 	AddSiblingUnder(bool vert, bool before, Ctrl &sibling, Ctrl &c, Size sz = Null);
	Ctrl & 	AddRoot(Ctrl &ctrl, bool first, Size sz = Null) 					{ if (!HasChild(&ctrl)) AddRoot0(ctrl, first, sz); return ctrl; }
	
	Ctrl &	Swap(Ctrl &oldctrl, Ctrl &newctrl);
	
	void 	Clear();
	
	void 	RootVert()								{ RootVertHorz(true); }
	void 	RootHorz()								{ RootVertHorz(false); }
	void 	RootVertHorz(bool isvert)				{ root->vert = isvert; Layout(); }
	bool 	IsRootVert() const	  					{ return root->vert; }
	bool	IsCtrlVert(Ctrl &c) const				{ Node *n = FindCtrl(root, &c); ASSERT(n); return n->parent->vert; }
	bool	IsCtrlRoot(Ctrl &c) const; 
	bool	IsCtrlNearRoot(Ctrl &c) const; 			// Will return true only if Ctrl is a child of roots only child
	int 	GetDepth() const						{ return GetDepth(root, 0); }
	
	Ctrl *	GetNextSibling(Ctrl &c);
	Ctrl *	GetPrevSibling(Ctrl &c);
	
	bool IsEmpty()									{ return (!root || !root->first); }
	
	~SplitterTree();
	SplitterTree();
};



#endif
