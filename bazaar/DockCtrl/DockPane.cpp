//==============================================================================================
// DockCtrl: A dockable widget framework for U++
// Author:	 Ismail YILMAZ (A.k.a. Maldoror)
// Version:	 0.49.9.9a
//==============================================================================================

#include "DockCtrl/DockCtrl.h"

DockPane& DockPane::CreatePane(int alignment, int size = 0)
{ 
	created = TRUE;
	maxsize = size;
	
	switch(alignment)
	{
		case DockWindow::dock_top: return Top(container.SizePos(), size);
		case DockWindow::dock_left: return Left(container.SizePos(), size);
		case DockWindow::dock_right: return Right(container.SizePos(), size);
		case DockWindow::dock_bottom: return Bottom(container.SizePos(), size);
	}
	return *this;
}

DockPane& DockPane::DockChild(DockableCtrl& dock, int size)
{ 
	if(DnDHasSource()) 
	{
		DnDSourceoutofRange();
		DnDAnimate();
		
		if(_dndposition <= container.GetCount()) container.AddChildBefore(&(dock.SizePos()), GetChildAt(_dndposition));
		else container.AddChild(&(dock.SizePos()),container.GetLastChild());
		
	} 
	else if(dock.Position() > 0 && HasDock()) container.AddChildBefore(&(dock.SizePos()), GetChildAt(dock.Position()));
	else container << (Ctrl&) dock.SizePos(); 
	RefreshChilds();
	return *this; 
}

Ctrl* DockPane::GetChildAt(int position)
{
	Ctrl* ctrl = container.GetFirstChild();
	if(position <= 1) 
		return (ctrl != &dummy) ? ctrl : NULL;
	while((ctrl = ctrl->GetNext())) 
			if((ctrl != &dummy) && ((DockableCtrl*)ctrl)->Position() == position) return ctrl;
	return NULL;
}

void DockPane::RefreshChilds()
{
	if(!HasDock())return;
	int i = 1; 
	DockableCtrl* ctrl = (DockableCtrl*) container.GetFirstChild();
	ctrl->Posit(i);
	while((ctrl = (DockableCtrl*) ctrl->GetNext())) 
		ctrl->Posit(++i);    
}

void DockPane::DockAdd(DockableCtrl& dock)
{
	if(dock.IsDocked() && !dock.IsHidden()) PaneShow();
	dock.GetBase().Dock(dock);
}

bool DockPane::HasDock() const		
{ 
	int count = container.GetCount();
	return ((count > 0) ? (count == 1 && container.GetFirstChild() == &dummy) ? false : true : false);
}

void DockPane::DockRemove(DockableCtrl& dock)
{
	if(!dock.IsFloating())
	{
		if(GetCount() == 0) PaneHide();
		if(!dock.IsShut()) dock.Remove();
		RefreshChilds();
		RefreshPane();
	}
}

void DockPane::DockRemove()
{
}					

void DockPane::PaneShow()
{
	SetSize(maxsize).RefreshPane().Enable();
}

void DockPane::PaneHide()
{
	SetSize(0).RefreshPane().Disable();
}

void DockPane::PaneLock()
{
}

void DockPane::DnDAnimate(int position)
{
	if(!dummy.IsOpen() && DnDHasSource()) 
	{
		DnDAnimStart();
		int count = GetCount();
		if(position <= 1) container.AddChildBefore(&(dummy.SizePos()), GetChildAt(position));
			else
		if(position < count) container.AddChild(&(dummy.SizePos()),  container.GetLastChild());
			else 
		container.AddChildBefore(&(dummy.SizePos()), GetChildAt(position));
		_dndposition = position;
	}
	else if(dummy.IsOpen() && !DnDHasSource())
	{
		dummy.Remove();
		DnDAnimStop();
	}
	RefreshPane();
}

bool DockPane::DnDCalcSourcePos(Point pos)
{
	Ctrl* first = NULL;
	Ctrl* last  = NULL;
	Ctrl* next	= NULL;
	
	if(!HasDock())
		return DnDCalcEmptyPos(*container.GetFirstChild(), pos);
	if(!(first = container.GetFirstChild()) || !(last = container.GetLastChild())) 
		return false;
	if((first == last) && (first != &dummy))
		return (container.IsVert() ? DnDCalcVertPos(*first, pos) : DnDCalcHorzPos(*first, pos));
	else
	{
		next = first;
		while(next)
		{
			if(next != &dummy)
			{
				if(container.IsVert() && DnDCalcVertPos(*next, pos)) return true;
				else if(container.IsHorz() && DnDCalcHorzPos(*next, pos)) return true; 
			}
			next = next->GetNext();
		}
	}

return false;
}

bool DockPane::DnDCalcHorzPos(Ctrl& c, Point pos)
{
	Rect r = ((DockWindow&)c).GetDockView();
	int  n = ((DockableCtrl&)c).Position();
	int  i = GetCount();
	
	int hrange  = 20;

		if(!DnDHasSource() && !DnDIsAnimating() &&
		((((pos.x > r.left) && (pos.x < r.left + hrange)) || ((pos.x < r.right) && (pos.x > r.right - hrange))) && 
			(pos.y > r.top) && (pos.y < r.bottom)))
		{
			DnDSourceinRange(); DnDAnimate(pos.x > r.right - hrange ? ++n : n); 
			return true;
		}
		
		if(DnDHasSource())
		{
			r = dummy.GetScreenRect();
			if(DnDIsAnimating() && ((pos.x > r.left) && (pos.x < r.right) && (pos.y > r.top) && (pos.y < r.bottom))) return true;
			DnDSourceoutofRange(); DnDAnimate();
			return false;	
		}
		
return false;
}

bool DockPane::DnDCalcVertPos(Ctrl& c, Point pos)
{
	Rect r = ((DockWindow&)c).GetDockView();
	int  n = ((DockableCtrl&)c).Position();
	int vrange = 20;

		if(!DnDHasSource() && !DnDIsAnimating() &&
		((pos.x > r.left) && (pos.x < r.right)) && (((pos.y > r.top) && (pos.y < r.top + vrange)) || ((pos.y < r.bottom) && (pos.y > r.bottom - vrange))))
		{
			DnDSourceinRange(); DnDAnimate(pos.y > r.bottom - vrange ?  ++n : n);
			return true;
		} 

		if(DnDHasSource())
		{	
			r = dummy.GetScreenRect();
			if(DnDIsAnimating() && ((pos.x > r.left) && (pos.x < r.right) && (pos.y > r.top - 5) && (pos.y < r.bottom + 5))) return true;
			DnDSourceoutofRange(); DnDAnimate();
			return false;
		}

return false;
}

bool DockPane::DnDCalcEmptyPos(Ctrl& c, Point pos)
{
	Rect r = GetScreenView();
	int  hrange = 22;
	int  vrange = 22;
	bool state	= false;
	
	if(!DnDHasSource() && !DnDIsAnimating() && (&c != &dummy))
	{
		switch(GetType())
		{
			case LEFT:
				((pos.x < r.right + hrange) && (pos.x > r.left) && (pos.y > r.top && pos.y < r.bottom)) ?
					state = true : state = false; break;
			case RIGHT:
				((pos.x > r.left - hrange) && (pos.x < r.right) && (pos.y > r.top && pos.y < r.bottom))  ?
					state = true : state = false; break;
			case TOP:
				((pos.y < r.bottom + vrange) && (pos.y > r.top) && (pos.x > r.left && pos.x < r.right))  ? 
					state = true : state = false; break;
			case BOTTOM:
				((pos.y > r.top - vrange) && (pos.y < r.bottom) && (pos.x > r.left && pos.x < r.right)) ?
					state = true : state = false; break;
		}
		if(state) 
		{
			PaneShow();
			DnDSourceinRange(); DnDAnimate(1);
			return true;
		}
		return false;
	}
	if(DnDHasSource())
	{
		r = dummy.GetScreenRect();
		if(DnDIsAnimating() && ((pos.x > r.left) && (pos.x < r.right) && (pos.y > r.top) && (pos.y < r.bottom))) return true;

		PaneHide();
		DnDSourceoutofRange(); DnDAnimate();
		return false;
	}
return false;
}

//----------------------------------------------------------------------------------------------
void DockPane::DummyCtrl::AnimateHorz()
{
	if(!IsVisible()) Show();
}
void DockPane::DummyCtrl::AnimateVert()
{
	if(!IsVisible()) Show();
}
//----------------------------------------------------------------------------------------------

void DockPane::FrameAdd(Ctrl& parent)
{
	parent.Add(*this);
}

void DockPane::FrameRemove()
{
	this->Ctrl::Remove(); created = FALSE;
}

void DockPane::FrameAddSize(Size& sz)
{
	(type == LEFT || type == RIGHT ? sz.cx : sz.cy) += size;
}

int  DockPane::BoundSize()
{
	int maxsize = max(0, (type == LEFT || type == RIGHT ? parentsize.cx : parentsize.cy) - sizemin);
	return max(4, minmax(size, minsize - 4, maxsize));
}

void DockPane::FrameLayout(Rect& r)
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

void DockPane::Paint(Draw& draw)
{
	draw.DrawRect(
		Ctrl::GetSize(),
		HasCapture() ? GUI_GlobalStyle() >= GUISTYLE_XP ? Blend(SColorHighlight, SColorFace) : SColorShadow
		             : SColorFace()
	);
}

void DockPane::LeftDown(Point p, dword)
{
	SetCapture();
	Ctrl::Refresh();
	ref = GetMousePos();
	size0 = BoundSize();
}

void DockPane::MouseMove(Point p, dword keyflags)
{
	if(!HasCapture())
		return;
	p = GetMousePos();
	switch(type) {
	case LEFT: size = size0 + p.x - ref.x; break;
	case RIGHT: size = size0 + ref.x - p.x; break;
	case TOP: size = size0 + p.y - ref.y; break;
	case BOTTOM: size = size0 + ref.y - p.y; break;
	}
	Ctrl::RefreshParentLayout();
}

void DockPane::LeftUp(Point p, dword keyflags)
{
	Ctrl::Refresh();
}

Image DockPane::CursorImage(Point p, dword keyflags)
{
	return type == LEFT || type == RIGHT ? Image::SizeHorz() : Image::SizeVert();
}

DockPane& DockPane::Set(Ctrl& c, int _size, int _type)
{
	while(GetFirstChild())
		GetFirstChild()->Remove();
	type = _type;
	size = _size;
	Add(c.SizePos());
	switch(type) {
	case LEFT: c.HSizePos(0, 4); break;
	case RIGHT: c.HSizePos(4, 0); break;
	case TOP: c.VSizePos(0, 4); break;
	case BOTTOM: c.VSizePos(4, 0); break;
	}
	Ctrl::RefreshParentLayout();
	return *this;
}

void DockPane::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	s % size;
}

DockPane::DockPane()
{
	type = LEFT;
	size = size0 = 4;
	minsize = 0;
	sizemin = 0;
	dockcount = 0;
	horizchild = vertchild = created = FALSE;
	maxsize = 0;
	defsize = 0;
	_dndposition = dnd_none;
	_dndanim = false;
	mousestate = false;
}