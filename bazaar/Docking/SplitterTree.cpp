#include "Docking.h"

void SplitterTree::Paint(Draw &w)
{
	if (!IsTransparent())
		w.DrawRect(GetSize(), SColorFace());
	if (HasCapture() && dragprev)
		w.DrawRect(DragHlRect(), GUI_GlobalStyle() >= GUISTYLE_XP ? Blend(SColorHighlight, SColorFace) : SColorShadow);
}

void SplitterTree::LeftDown(Point p, dword keyflags)
{
	if (MouseInBar(p, root, GetView(), true)) {
		SetCapture();
		lastdragpos = p;
		Refresh(DragHlRect());
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
	Node *n = dragprev ? dragprev->parent : MouseInBar(p, root, GetView(), false);
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

Rect SplitterTree::DragHlRect()
{
	Rect r = GetNodeRect(dragprev);
	if (dragprev->parent->vert) {
		r.top = r.bottom;
		r.bottom += 4;	
	}
	else {
		r.left = r.right;
		r.right += 4;	
	}
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
	int bsz = 4;
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

Size SplitterTree::GetMinSize(Node *p)
{
	if (Ctrl *c = p->GetCtrl())
		return c->GetMinSize();
	Size sz(0, 0);
	for (Node *n = p->first; n; n = n->next) {
		sz += GetMinSize(n);
		p->vert ? sz.cy += 4 : sz.cx += 4;
	}		
	return sz;
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

void SplitterTree::SetNodeSize(Node *n, const Size &sz)
{
	Node *p = n->parent;
	ASSERT(p->first);
	if ((p->first == p->last))
		n->sz = 10000;
	else if (sz.IsNullInstance())
		n->sz = 10000/(ChildCount(p) - 1);	
	else {
		int newsz = p->vert ? sz.cy : sz.cx;
		if (newsz == 0) {
			n->sz = 0;
			return ReconcileChildSizes(p);	
		}
		int psz = p->vert ? GetNodeRect(p).Height() : GetNodeRect(p).Width(); // Get total size of parent
/*		int msz = p->vert ? GetMinSize(p).cy : GetMinSize(p).cx;
		if (newsz > psz-msz)
			newsz = max(p->vert ? GetMinSize(n).cy : GetMinSize(n).cx, psz-msz);
*/		newsz = (newsz * 10000) / max(psz, 1); // Get new size in internal ratio units
		newsz = min(8000, newsz); // Clamp
		int dif = (newsz - n->sz)/(ChildCount(p)-1); // Calc. how much to take off other nodes
		n->sz = newsz; // Set node to new size
		psz = 0; // Used for summing new sizes
		for (Node *q = p->first; q; q = q->next) {
			if (q != n)			
				q->sz = max(q->sz - dif, 1500);
			psz += q->sz;
		}		
		p->last->sz	+= 10000 - psz;
		return;
	}
	ReconcileChildSizes(p);	
}
/* Failed attempt at propertional resizing
void SplitterTree::SetNodeSize(Node *n, const Size &sz)
{
	Node *p = n->parent;
	ASSERT(p->first);
	if ((p->first == p->last))
		n->sz = 10000;
	else if (sz.IsNullInstance())
		n->sz = 10000/(ChildCount(p) - 1);	
	else {
		int newsz = p->vert ? sz.cy : sz.cx;
		if (newsz == 0) {
			n->sz = 0;
			return ReconcileChildSizes(p);	
		}
		int psz = 0;
		for (Node *c = p->first; c; c = c->next) {
			if (c != n || n->sz != 0) {
				c->sz = (p->vert ? GetNodeRect(c).GetSize().cy : GetNodeRect(c).GetSize().cx);
				psz += c->sz;
			}
		}
		newsz = min(8000, (newsz * 10000) / max(psz, 1));
		psz -= n->sz;
		n->sz = newsz;
		int spare = 10000 - n->sz;
		int sum = 0;
		for (Node *q = p->first; q; q = q->next) {
			if (q != n)
				q->sz = (q->sz * spare) / max(psz, 1);				
			sum += q->sz;
		}		
		p->last->sz	+= 10000 - sum;
		return;
	}
	ReconcileChildSizes(p);	
}
*/
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

SplitterTree::Leaf * SplitterTree::FindCtrl(Node *r, Ctrl *c) const
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

Ctrl & SplitterTree::AddSibling(bool before, Ctrl &sibling, Ctrl &c, Size sz)
{
	if (HasChild(&c)) return c;
	Leaf *l = FindCtrl(root, &sibling);
	ASSERT(l);
	SetNodeSize(AddBA(before, l, NewLeaf(c)), sz);
	Layout();
	return c;
}

Ctrl & SplitterTree::AddSiblingOver(bool vert, bool before, Ctrl &sibling, Ctrl &c, Size sz)
{
	if (HasChild(&c)) return c;
	Leaf *l = FindCtrl(root, &sibling);
	ASSERT(l);
	Node *p = l->parent;
	if (p->vert == vert || (p != root && p->first == p->last)) { // Don't change orientation of root node
		p->vert = vert;
		SetNodeSize(AddBA(before, l, NewLeaf(c)), sz);
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
			SetNodeSize(AddBA(before, p, NewLeaf(c)), sz);
		}
	}
	Layout();
	return c;
}

Ctrl & SplitterTree::AddSiblingUnder(bool vert, bool before, Ctrl &sibling, Ctrl &c, Size sz)
{
	if (HasChild(&c)) return c;
	Leaf *l = FindCtrl(root, &sibling);
	ASSERT(l);
	Node *p = l->parent;
	if (p->vert == vert || (p != root && p->first == p->last)) { // Don't change orientation of root node
		p->vert = vert;
		SetNodeSize(AddBA(before, l, NewLeaf(c)), sz);
	}
	else {
		Node *n = NewNode();
		AddBefore(l, n);
		n->sz = l->sz;
		RemoveND(l);
		n->vert = vert;
		l->sz = 10000;
		AddFirst(n, l);
		SetNodeSize(AddBA(before, l, NewLeaf(c)), sz);
	}
	Layout();
	return c;
}

void SplitterTree::AddRoot0(Ctrl &ctrl, bool first, const Size &sz)
{
	int cnt = ChildCount(root);
	Node *n = first ? AddFirst(root, NewLeaf(ctrl)) : AddLast(root, NewLeaf(ctrl));
	SetNodeSize(n, sz);
	Layout();
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

bool SplitterTree::IsCtrlRoot(Ctrl &c) const
{
	for (Node *n = root->first; n; n = n->next)
		if (n->GetCtrl() == &c) return true;
	return false;
}

bool SplitterTree::IsCtrlNearRoot(Ctrl &c) const
{
	if (root->first != NULL && root->first == root->last) {
		for (Node *n = root->first->first; n; n = n->next)
			if (n->GetCtrl() == &c) return true;
	}
	return false;
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

