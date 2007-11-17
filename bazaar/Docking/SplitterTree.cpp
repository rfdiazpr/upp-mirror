#include "Docking.h"

void SplitterTree::Paint(Draw &w)
{
	if (!root->first)
		w.DrawRect(GetSize(), SColorFace());
	else
		PaintNode(w, root);
}

void SplitterTree::LeftDown(Point p, dword keyflags)
{
	if (MouseInBar(p, root, GetView(), true)) {
		SetCapture();
		lastdragpos = p;
	}
}

void SplitterTree::LeftUp(Point p, dword keyflags)
{
	if (HasCapture() && dragprev) {
		ReleaseCapture();
		RefreshNode(dragprev->parent);
		dragprev = dragnext = NULL;
	}
}

void SplitterTree::MouseMove(Point p, dword keyflags)
{
	if (HasCapture() && dragprev && dragnext) {
		int dif = 0;
		Rect prect = GetNodeRect(dragprev->parent);
		if (dragprev->parent->vert) {
			int h = prect.Height();
			if (!h) return;
			dif = p.y - lastdragpos.y;
			dif = minmax((dif * 10000) / h, -dragprev->sz, dragnext->sz);	
		}
		else {
			int w = prect.Width();
			if (!w) return;
			dif = p.x - lastdragpos.x;
			dif = minmax((dif * 10000) / w, -dragprev->sz, dragnext->sz);	
		}
		dragprev->sz += dif;
		dragnext->sz -= dif;
		lastdragpos = p;
		LayoutNode(dragprev->parent, prect);
		Refresh(prect);
	}
}

Image SplitterTree::CursorImage(Point p, dword keyflags)
{
	Node *n = MouseInBar(p, root, GetView(), false);
	if (n) return n->vert ? Image::SizeVert() : Image::SizeHorz();	
	return Image::Arrow();
}

SplitterTree::Node *SplitterTree::MouseInBar(const Point &pt, Node *p, Rect r, bool store_nodes)
{
	if (!p->first) return false;
	Node *n;
	for (n = p->first; n; n = n->next) {
		Rect r = GetNodeRect(n);
		if (p->vert) {
			if (pt.y < r.top)
				break;
			else if (pt.y < r.bottom && n->first)
				return MouseInBar(pt, n, r, store_nodes);				
		}
		else {
			if (pt.x < r.left)
				break;
			else if (pt.x < r.right)
				return MouseInBar(pt, n, r, store_nodes);					
		}
		
	}
	if (!n || !n->prev) return false;
	
	if (store_nodes) {
		dragprev = n->prev;
		dragnext = n;	
	}
	return p;
}

Rect SplitterTree::PaintNode(Draw &w, Node *p)
{
	if (!p->first) {
		ASSERT(p->GetCtrl());
		return p->GetCtrl()->GetRect();
	}
	int initialc1 = 0;
	Rect r(0, 0, 0, 0);
	int *c1 = p->vert ? &r.top : &r.left;
	int *c2 = p->vert ? &r.bottom : &r.right;
	for (Node *n = p->first; n; n = n->next) {
		r = PaintNode(w, n);
		if (!n->prev) initialc1 = *c1;
		if (n->next) {
			*c1 = *c2;
			*c2 += BarSize();
			if (n == dragprev)
				w.DrawRect(r, GUI_GlobalStyle() >= GUISTYLE_XP ? Blend(SColorHighlight, SColorFace) : SColorShadow);
			else
				//InsetFrame().FramePaint(w, r);
				w.DrawRect(r, SColorFace());
		}
	}
	*c1 = initialc1;
	return r;
}

void SplitterTree::LayoutNode(Node *p, Rect r)
{
	int cnt = ChildCount(p);
	
	if (!cnt && p->IsLeaf()) {
		Ctrl *c = p->GetCtrl();
		ASSERT(c);
		c->SetRect(r);
	}
	int rsz = p->vert ? r.Height() : r.Width();
	int bsz = BarSize();
	rsz -= (cnt-1) * bsz;
	if (p->vert) {
		int bt = r.bottom;
		for (Node *n = p->first; n; n = n->next) {
			r.bottom = r.top + (rsz*n->sz)/10000;
			if (!n->next) r.bottom = bt;
			LayoutNode(n, r);
			r.top = r.bottom + bsz;
		}
	}
	else {
		int rt = r.right;
		for (Node *n = p->first; n; n = n->next) {
			r.right = r.left + (rsz*n->sz)/10000;
			if (!n->next) r.right = rt;
			LayoutNode(n, r);
			r.left = r.right + bsz;
		}
	}
}

void SplitterTree::ReconcileChildSizes(Node *p)
{
	ASSERT(p->first);
	int sum = 0;
	int cnt = 0;
	for (Node *n = p->first; n; n = n->next) {
		sum += n->sz;	
		cnt++;
	}
	if (cnt == 1) { p->first->sz = 10000; return; }
	sum -= 10000;
	int rem = sum % cnt;
	sum /= cnt;
	for (Node *n = p->first; n; n = n->next)
		n->sz -= sum;
	p->first->sz += rem;
}


Rect SplitterTree::GetNodeRect(Node *n)
{
	if (!n->first) {
		ASSERT(n->GetCtrl());
		return n->GetCtrl()->GetRect();	
	}
	else if (n->first == n->last)
		return GetNodeRect(n->first);
	return Rect(GetNodeRect(n->first).TopLeft(), GetNodeRect(n->last).BottomRight()); 
}

SplitterTree::Leaf * SplitterTree::FindCtrl(Node *r, Ctrl *c)
{
	Leaf *l;
	for (Node *n = r->first; n; n = n->next) {
		l = FindCtrl(n, c);
		if (l) return l;		
	}
	if (r->IsLeaf()) {
		if (r->GetCtrl() == c)
			return dynamic_cast<Leaf *>(r);	
	}
	return NULL;
}

SplitterTree::Node *SplitterTree::AddFirst(Node *p, Node *n)
{
	ASSERT(n && p);
	if (!p->IsEmpty()) return AddBefore(p->first, n);
	p->first = n;
	p->last = n;
	n->parent = p;
	n->next = n->prev = NULL;
	return n;
}

SplitterTree::Node *SplitterTree::AddLast(Node *p, Node *n)
{
	ASSERT(n && p);
	if (!p->IsEmpty()) return AddAfter(p->last, n);
	p->first = n;
	p->last = n;
	n->parent = p;
	n->next = n->prev = NULL;
	return n;
}

SplitterTree::Node *SplitterTree::AddBefore(Node *s, Node *n)
{
	ASSERT(s && n);
	n->prev = s->prev;
	if (n->prev) n->prev->next = n;
	n->next = s;
	s->prev = n;
	n->parent = s->parent;
	if (s->parent && s->parent->first == s) s->parent->first = n;
	return n;
}

SplitterTree::Node *SplitterTree::AddAfter(Node *s, Node *n)
{
	ASSERT(s && n);
	n->next = s->next;
	if (n->next) n->next->prev = n;
	n->prev = s;
	s->next = n;
	n->parent = s->parent;
	if (s->parent && s->parent->last == s) s->parent->last = n;
	return n;
}

void SplitterTree::DestroyTree(Node *p)
{
	Node *nn = NULL;
	for (Node *n = p->first; n; n = nn) {
		nn = n->next;
		DestroyTree(n);
	}
	DeleteNode(p);
}

void SplitterTree::RemoveND(Node *n)
{
	ASSERT(n && n->parent);
	if (n->next) n->next->prev = n->prev;
	if (n->prev) n->prev->next = n->next;
	if (n->parent->first == n) n->parent->first = n->next;
	if (n->parent->last == n) n->parent->last = n->prev;
	n->next = NULL;
	n->prev = NULL;
	n->parent = NULL;
}

void SplitterTree::CleanUpTree(Node *n)
{
	if (n->parent && n->first == NULL && !n->IsLeaf()) {
		Node *p = n->parent;
		RemoveND(n);
		DeleteNode(n);
		if (p->first)
			ReconcileChildSizes(p);
		else
			CleanUpTree(p);
	}
	// If only one node left we should merge up
	else if (n->first == n->last && n != root) {
		Node *c = n->first;
		RemoveND(n->first);
		AddAfter(n, c);
		c->sz = n->sz;
		RemoveND(n);
		DeleteNode(n); 
		CleanUpTree(c->parent);
	}
}

Ctrl & SplitterTree::AddSibling(bool before, Ctrl &sibling, Ctrl &c)
{
	if (HasChild(&c)) return c;
	Leaf *l = FindCtrl(root, &sibling);
	ASSERT(l);
	Node *n = AddBA(before, l, NewLeaf(c));
	n->sz = 10000/max(ChildCount(l->parent)-1, 1);
	Layout();
	return c;
}

Ctrl & SplitterTree::AddSiblingOver(bool vert, bool before, Ctrl &sibling, Ctrl &c)
{
	if (HasChild(&c)) return c;
	Leaf *l = FindCtrl(root, &sibling);
	ASSERT(l);
	Node *p = l->parent;
	if (p->vert == vert) { //  || p->first == p->last
		p->vert = vert;
		AddBA(before, l, NewLeaf(c));
		ReconcileChildSizes(p);
	}
	else {
		Node *pp = p->parent;
		if (pp) {
			ASSERT(vert == pp->vert);
			Node *n = AddBA(before, p, NewLeaf(c));
			n->sz = 10000/max(ChildCount(p)-1, 1);
			ReconcileChildSizes(pp);
		}
		else { // p must be root
			root = NewNode();
			root->vert = vert;
			AddFirst(root, p);
			Node *n = AddBA(before, p, NewLeaf(c));
			n->sz = 10000/max(ChildCount(p)-1, 1);
			ReconcileChildSizes(root);
		}
	}
	Layout();
	return c;
}

Ctrl & SplitterTree::AddSiblingUnder(bool vert, bool before, Ctrl &sibling, Ctrl &c)
{
	if (HasChild(&c)) return c;
	Leaf *l = FindCtrl(root, &sibling);
	ASSERT(l);
	Node *p = l->parent;
	if (p->vert == vert) { //  || p->first == p->last
		p->vert = vert;
		Node *t = AddBA(before, l, NewLeaf(c));
		t->sz = 10000/max(ChildCount(p)-1, 1);
		ReconcileChildSizes(p);
	}
	else {
		Node *n = NewNode();
		AddBefore(l, n);
		n->sz = l->sz;
		RemoveND(l);
		n->vert = vert;
		l->sz = 10000;
		AddFirst(n, l);
		Node *t = AddBA(before, l, NewLeaf(c));
		t->sz = 10000/max(ChildCount(n)-1, 1);
		ReconcileChildSizes(n);
	}
	Layout();
	return c;
}

Ctrl & SplitterTree::AddRoot(Ctrl &ctrl, bool first)
{
	if (HasChild(&ctrl)) return ctrl;
	int cnt = ChildCount(root);
	Node *n = first ? AddFirst(root, NewLeaf(ctrl)) : AddLast(root, NewLeaf(ctrl));
	n->sz = 10000/max(cnt, 1);
	ReconcileChildSizes(root);
	Layout();
	return ctrl;
}

Ctrl & SplitterTree::Swap(Ctrl &oldctrl, Ctrl &newctrl)
{
	Leaf *l = FindCtrl(root, &oldctrl);
	ASSERT(l);	
	internal_block = true;
	l->ctrl->Remove();
	l->ctrl = &newctrl;
	Add(newctrl);
	internal_block = false;	
	newctrl.SetRect(oldctrl.GetRect());
	return newctrl;
}

void SplitterTree::RemoveFromTree(Ctrl &ctrl)
{
	int cnt = 0;
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext())
		cnt++;
	Leaf *l = FindCtrl(root, &ctrl);
	ASSERT(l);	
	Node *n = l->parent;
	RemoveND(l);
	if (!n->first || (n->first == n->last && n != root))
		CleanUpTree(n);
	else
		ReconcileChildSizes(n);
	DeleteNode(l);
}

void SplitterTree::Clear()
{
	Node *nn = NULL;
	for (Node *n = root->first; n; n = nn) {
		nn = n->next;
		DestroyTree(n);
	}
	root->first = root->last = NULL;		
}

SplitterTree::~SplitterTree()
{
	Clear();
	DeleteNode(root);
	root = NULL;
}

SplitterTree::SplitterTree()
{
	root = NewNode();
	internal_block = false;
	dragnext = dragprev = NULL;
}

