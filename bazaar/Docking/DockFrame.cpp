#include "DockFrame.h"

#define ANIM_TICKS 10
#define ANIM_INTERVAL 20 // 1000/25(fps) = 50

void DockFrame::FrameAdd(Ctrl& parent)
{
	parent.Add(*this);
}

void DockFrame::FrameRemove()
{
	this->Ctrl::Remove();
}

void DockFrame::FrameAddSize(Size& sz)
{
	if (IsShown()) (type == LEFT || type == RIGHT ? sz.cx : sz.cy) += size;
}

int  DockFrame::BoundSize()
{
	int maxsize = max(0, (type == LEFT || type == RIGHT ? parentsize.cx : parentsize.cy) - sizemin);
	return IsShown() ? max(4, minmax(size, minsize - 4, maxsize)) : 0;
}

Rect DockFrame::SplitterRect()
{
	Rect r = Ctrl::GetSize();
	switch(type) {
	case LEFT:
		r.right -= 4;
		break;
	case RIGHT:
		r.left += 4;
		break;
	case TOP:
		r.bottom -= 4;
		break;
	case BOTTOM:
		r.top += 4;
		break;
	}	
	return r;	
}

void DockFrame::FrameLayout(Rect& r)
{
	Rect rr = r;
	parentsize = r.GetSize();
	int sz = BoundSize();
	switch(type) {
	case LEFT:
		r.left += sz;
		rr.right = r.left;
		break;
	case RIGHT:
		r.right -= sz;
		rr.left = r.right;
		break;
	case TOP:
		r.top += sz;
		rr.bottom = r.top;
		break;
	case BOTTOM:
		r.bottom -= sz;
		rr.top = r.bottom;
		break;
	}
	SetFrameRect(rr);
}

void DockFrame::Paint(Draw& draw)
{
	SplitterTree::Paint(draw);	
	if ((HasCapture() && !dragprev)) {
		Rect r = Ctrl::GetSize();
		switch(type) {
		case LEFT:
			r.left = r.right - 4;
			break;
		case RIGHT:
			r.right = r.left + 4;
			break;
		case TOP:
			r.top = r.bottom - 4;
			break;
		case BOTTOM:
			r.bottom = r.top + 4;
			break;
		}	
		draw.DrawRect(r, GUI_GlobalStyle() >= GUISTYLE_XP ? Blend(SColorHighlight, SColorFace) : SColorShadow);
	}
}

void DockFrame::Layout()
{
	SplitterTree::LayoutNode(root, SplitterRect());
}

void DockFrame::LeftDown(Point p, dword keyflags)
{
	if (SplitterRect().Contains(p))
		SplitterTree::LeftDown(p, keyflags);
	else {
		SetCapture();
		Refresh();
		ref = GetMousePos();
		size0 = BoundSize();
	}
}

void DockFrame::MouseMove(Point p, dword keyflags)
{
	if(!HasCapture())
		return;
	if (dragprev)
		return SplitterTree::MouseMove(p, keyflags);
	p = GetMousePos();
	switch(type) {
	case LEFT: size = size0 + p.x - ref.x; break;
	case RIGHT: size = size0 + ref.x - p.x; break;
	case TOP: size = size0 + p.y - ref.y; break;
	case BOTTOM: size = size0 + ref.y - p.y; break;
	}
	RefreshParentLayout();
}

void DockFrame::LeftUp(Point p, dword keyflags)
{
	if (dragprev)
		return SplitterTree::LeftUp(p, keyflags);
	Refresh();
}

Image DockFrame::CursorImage(Point p, dword keyflags)
{
	if (SplitterRect().Contains(p))
		return SplitterTree::CursorImage(p, keyflags);
	return type == LEFT || type == RIGHT ? Image::SizeHorz() : Image::SizeVert();
}

DockFrame& DockFrame::Set(int _size, int _type)
{
	type = _type;
	size = _size;
	RefreshParentLayout();
	return *this;
}

void DockFrame::Dock(bool vert, bool before, Ctrl &sibling, Ctrl &c, Size sz)
{
	animctrl = NULL;
	SplitterTree::AddSiblingUnder(vert, before, sibling, c, sz);
	if (!IsVisible())
		Show();
	//TODO: Smart resize 	
}

void DockFrame::AnimateDock(bool vert, bool before, Ctrl &sibling, Ctrl &c, Size sz)
{
	animctrl = NULL;
	SplitterTree::AddSiblingUnder(vert, before, sibling, c, Size(0, 0));
	StartNodeAnimate(c, vert ? sz.cy : sz.cx);
	//TODO: Smart resize 
	// StartFrameAnimate(c, vert ? sz.cx : sz.cy);
	if (IsAnimating())
		Ctrl::KillSetTimeCallback(-ANIM_INTERVAL, THISBACK(AnimateTick), TIMEID_ANIMATE);	
}

void DockFrame::DockRoot(Ctrl &c, bool first, Size sz)
{
	animctrl = NULL;
	SplitterTree::AddRoot(c, first, sz);
	size = max(IsRootVert() ? sz.cx : sz.cy, size);
	if (!IsVisible())
		Show();
}

void DockFrame::AnimateDockRoot(Ctrl &c, bool first, Size sz)
{
	animctrl = NULL;
	SplitterTree::AddRoot(c, first, Size(0, 0));
	StartNodeAnimate(c, IsRootVert() ? sz.cy : sz.cx);
	StartFrameAnimate(max(IsRootVert() ? sz.cx : sz.cy, IsVisible() ? size : 0));	
	if (IsAnimating())
		Ctrl::KillSetTimeCallback(-ANIM_INTERVAL, THISBACK(AnimateTick), TIMEID_ANIMATE);	
}

void DockFrame::Remove(Ctrl &c, int fsz)
{
	c.Remove();
	if (!GetFirstChild()) {
		size = 0;
		Hide();
		return;
	}
	if (fsz >= 0 && fsz != size && IsVisible()) {
		size = framesz;
		RefreshParentLayout();
	}
	else
		Layout();
}	

void DockFrame::AnimateRemove(Ctrl &c, int fsz)
{
	Ctrl::KillTimeCallback(TIMEID_ANIMATE);

	SplitterTree::Swap(c, dummy);

	if (GetFirstChild() == GetLastChild())
		fsz = 0; // Animate hide

	StartNodeAnimate(dummy, 0);
	if (fsz >= 0)
		StartFrameAnimate(fsz);
	
	if (IsAnimating()) 
		Ctrl::SetTimeCallback(-ANIM_INTERVAL, THISBACK(AnimateTick), TIMEID_ANIMATE);	
}

void DockFrame::DockSwap(Ctrl &oldctrl, Ctrl &newctrl)
{
	Swap(oldctrl, newctrl);
	if (IsAnimating() && &oldctrl == ~animctrl)
		animctrl = &newctrl;
}

void DockFrame::StartNodeAnimate(Ctrl &c, int sz)
{
	nodesz = sz;
	nodeinc = 0;
	if (sz > 0 && dummy.GetParent()) {
		dummy.Remove();
		Layout();
	}
	// Nothing to animate if only one child
	animctrl = &c;
	if (GetFirstChild() == GetLastChild())
		return;
	
	animnode = FindCtrl(root, &c);
	if (sz == 0) {
		nodeacc = animnode->parent->vert ? c.GetSize().cy : c.GetSize().cx;		
		nodeinc = -max(nodeacc / ANIM_TICKS, 1);
	}
	else {
		nodeinc = max(nodesz / ANIM_TICKS, 1);
		nodeacc = 0;	
	}
}

void DockFrame::StartFrameAnimate(int sz)
{
	frameinc = 0;
	if (!IsVisible())
		size = 0;
	if (sz != size) {
		frameinc = (sz - size) / ANIM_TICKS;
		frameinc = frameinc < 0 ? min(-1, frameinc) : max(1, frameinc);
		framesz = sz;
	}	
	if (!IsVisible())
		Show();
}

void DockFrame::AnimateTick()
{
	if (nodeinc && (~animctrl == NULL || animctrl->GetParent() != this)) {
		Ctrl::KillTimeCallback(TIMEID_ANIMATE);
		return;
	}

	bool lay = AnimateNode();
	bool play =  AnimateFrame();
	
	if (play)
		RefreshParentLayout();
	else if (lay)
		LayoutNode(animnode->parent, GetNodeRect(animnode->parent));
	
	if (!frameinc && !nodeinc) {
		animctrl = NULL;
		frameinc = 0;
		Ctrl::KillTimeCallback(TIMEID_ANIMATE);
	}
}

bool DockFrame::AnimateFrame()
{
	if (!frameinc) return false;
		
	size += frameinc;
	if (frameinc > 0 && size >= framesz || frameinc < 0 && size <= framesz) {
		size = framesz;
		frameinc = 0;
		if (size == 0)
			Hide();
	}
	return true;
}

bool DockFrame::AnimateNode()
{
	if (!nodeinc) return false;
	
	nodeacc += nodeinc;
	if (nodeinc > 0 && nodeacc >= nodesz) {
		nodeacc = nodesz;	
		nodeinc = 0;
	}
	else if (nodeacc <= 0) {
		nodeinc = 0;
		dummy.Remove();
		Layout();		
		return false;
	}
	
	SetNodeSize(animnode, Size(nodeacc, nodeacc));
	return true;
}



DockFrame::DockFrame()
{
	type = LEFT;
	size = size0 = 4;
	minsize = 0;
	sizemin = 0;
}
