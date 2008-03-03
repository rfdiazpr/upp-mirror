#include "Docking.h"

#define ALIGN_ASSERT(al)	ASSERT(al >= 0 && al < 4)

/*
 * Public interface
*/ 
void DockWindow::State(int reason)
{
	if (reason == Ctrl::OPEN) {
		if (!hideframe[0].GetParent()) 
			DockLayout();
		if (!init) {
			DockInit();
			init = true;	
		}
	}
}

void DockWindow::Dock(int align, DockableCtrl &dc, int pos)
{
	ALIGN_ASSERT(align);
	Register(dc);
	DockContainer(align, *GetReleasedContainer(dc), pos);
}

void DockWindow::DockAsTab(DockableCtrl &target, DockableCtrl &dc)
{
	ASSERT(GetContainer(target));	
	Register(dc);
	DockAsTab(*GetContainer(target), dc);
}

void DockWindow::Float(DockableCtrl &dc, Point p)
{
	if (dc.GetParent() && p.IsNullInstance())
		p = GetScreenRect().TopLeft(); 
	else
		Register(dc);        
	FloatContainer(*GetReleasedContainer(dc), -1, p);
}

void DockWindow::Float(DockableCtrl &dc, const char *title, Point p)
{
	dc.Title(title); 
	Float(dc, p);	
}

void DockWindow::AutoHide(DockableCtrl &dc)
{
	int align = GetDockAlign(dc);
	AutoHide(align == DOCK_NONE ? DOCK_TOP : align,  dc);	
}

void DockWindow::AutoHide(int align, DockableCtrl &dc)
{
	ALIGN_ASSERT(align);
	Register(dc);
	DockCont *c = GetReleasedContainer(dc);
	c->StateAutoHide(*this);
	hideframe[align].AddCtrl(*c, dc.GetGroup());	
}

int DockWindow::FindDocker(DockableCtrl *dc)
{
	for (int i = 0; i < dockers.GetCount(); i++)
		if (dc == dockers[i])
			return i;
	return -1;
}

DockableCtrl &DockWindow::Register(DockableCtrl &dc)
{
	int ix = FindDocker(&dc); 
	if (ix < 0) {
		ix = dockers.GetCount();
		dockers.Add(&dc);
	}
	return *dockers[ix];
}

void DockWindow::Deregister(DockableCtrl &dc)
{
	int ix = FindDocker(&dc); 
	Close(dc);
	if (ix >= 0)
		dockers.Remove(ix);
}

void DockWindow::Close(DockableCtrl &dc)
{
	DockCont *c = GetContainer(dc);
	if (c && c->GetCount() > 1) {
		dc.Remove();
		c->Layout();
		c = NULL;
	}
	if (c) CloseContainer(*c);
}

void DockWindow::DockGroup(int align, String group, int pos)
{
	ALIGN_ASSERT(align);
	bool all = group == "All"; 
	for (int i = 0; i < dockers.GetCount(); i++)
		if (all || dockers[i]->GetGroup() == group && IsDockAllowed(align, *dockers[i]))
			Dock(align, *dockers[i], pos);
}

void DockWindow::FloatGroup(String group)
{
	bool all = group == "All"; 	
	Point p = GetScreenRect().TopLeft();
	Point inc(20, 20);	
	for (int i = 0; i < dockers.GetCount(); i++)
		if (all || dockers[i]->GetGroup() == group) {
			p += inc;
			Float(*dockers[i], p);
		}
}

void DockWindow::AutoHideGroup(int align, String group)
{
	ALIGN_ASSERT(align);
	bool all = group == "All"; 	
	for (int i = 0; i < dockers.GetCount(); i++)
		if (all || dockers[i]->GetGroup() == group)
			AutoHide(align, *dockers[i]);
}

void DockWindow::TabDockGroup(int align, String group, int pos)
{
	if (DockCont *c = TabifyGroup(group)) {
		if (c->IsDockAllowed(align))
			DockContainer(align, *c, pos);
		else
			FloatContainer(*c);
	}
}

void DockWindow::TabFloatGroup(String group)
{
	if (DockCont *c = TabifyGroup(group)) 
		FloatContainer(*c);
}

DockCont * DockWindow::TabifyGroup(String group)
{
	DockCont *c = NULL;	
	bool all = group == "All"; 
	for (int i = 0; i < dockers.GetCount(); i++)
		if (all || dockers[i]->GetGroup() == group) {
			if (c)
				DockAsTab(*c, *dockers[i]);
			else
				c = GetReleasedContainer(*dockers[i]);	
		}
	return c;
}

void DockWindow::CloseGroup(String group)
{
	bool all = group == "All"; 	
	for (int i = 0; i < dockers.GetCount(); i++)
		if (all || dockers[i]->GetGroup() == group)
			Close(*dockers[i]);
	for (int i = 0; i < conts.GetCount(); i++)
	conts[i].Layout();
}

/*
 * Container management
*/ 
DockCont * DockWindow::CreateContainer()
{
	DockCont *dc = &conts.Add();
	dc->StateNotDocked(this);
	SyncContainer(*dc);
	return dc;
}

DockCont * DockWindow::CreateContainer(DockableCtrl &dc)
{
	DockCont *c = CreateContainer();
	c->Add(dc);
	return c;
}

void DockWindow::DestroyContainer(DockCont &c)
{
	for (int i = 0; i < conts.GetCount(); i++)
		if (&conts[i] == &c) 
			return conts.Remove(i);
}

DockCont *DockWindow::GetReleasedContainer(DockableCtrl &dc)
{
	DockCont *c = GetContainer(dc);
	if (c && c->GetCount() > 1) {
		dc.Remove();
		c->RefreshLayout();
		c = NULL;
	}
	if (c) 
		Detach(*c);
	else
		c = CreateContainer(dc);
	return c;
}

void DockWindow::SyncContainer(DockCont &c)
{
	c.Grouping(grouping);
	c.WindowButtons(menubtn, hidebtn, closebtn);
	c.SyncButtons();
}

void DockWindow::SyncAll()
{
	for (int i = 0; i < conts.GetCount(); i++)
		SyncContainer(conts[i]);
}

/*
 * Docking/Undocking helpers
*/ 
void DockWindow::Undock0(Ctrl &c, bool do_animate, int fsz)
{
	int al = GetDockAlign(c);
	Ctrl *p = c.GetParent();
	if (p != &dockpane[al]) {
		c.Remove();
		if (p) {
			p->Layout();
			p->RefreshLayout();
		}
	}
	else {
		if (dockpane[al].GetFirstChild() == dockpane[al].GetLastChild())
			fsz = 0;
		dockpane[al].Undock(c, do_animate);
		if (fsz >= 0) {
			do_animate ? StartFrameAnimate(al, fsz) : dockframe[al].SetSize(fsz);
			if (!do_animate && fsz == 0)
				dockframe[al].Hide();
		}
			
	}
}

void DockWindow::StartFrameAnimate(int align, int targetsize)
{
	FrameAnim &a = frameanim[align];
//	if (a.inc != 0) dockframe[align].SetSize(a.target);
	a.target = max(targetsize, 0);
	a.inc = (targetsize - dockframe[align].GetSize()) / dockpane[0].GetAnimMaxTicks();
	if (a.inc == 0)
		a.inc = (targetsize > dockframe[align].GetSize()) ? 1 : -1;
	KillSetTimeCallback(-dockpane[0].GetAnimInterval(), THISBACK(FrameAnimateTick), TIMEID_ANIMATE);
}

void DockWindow::FrameAnimateTick()
{
	bool done = true;
	for (int i = 0; i < 4; i++) {
		FrameAnim &a = frameanim[i];
		if (a.inc) {
			int sz = dockframe[i].GetSize() + a.inc;
			if (a.inc > 0 && sz >= a.target || a.inc < 0 && sz <= a.target) {
				sz = a.target;
				a.inc = 0;
				if (sz == 0)
					dockframe[i].Hide();
			}
			else
				done = false;			
			dockframe[i].SetSize(sz);
		}
	}
	if (done)
		KillTimeCallback(TIMEID_ANIMATE);
}

void DockWindow::Undock(DockCont &c)
{
	if (!c.IsFloating() && !c.IsHidden()) {
		int dock = GetDockAlign(c);
		if (dock >= 0 && dock < 4) {
			Ctrl *p = &c;
			while (p && p->GetParent() != &dockpane[dock]) p = p->GetParent();
			ASSERT(p);
			bool v = !IsTB(dock);		
			c.SyncUserSize(v, !v);
		}
		if (c.IsAutoHide()) {
			for (int i = 0; i < 4; i++) {
				int ix = hideframe[i].FindCtrl(c);
				if (ix >= 0) {
					hideframe[i].RemoveCtrl(ix);
					hideframe[i].RefreshFrame();
					break;
				}
			}				
		}
		else
			Undock0(c);		
		c.StateNotDocked();
	}		
}

void DockWindow::Unfloat(DockCont &c)
{
	if (c.IsFloating()) {
		c.SyncUserSize(true, true);
		if (c.IsOpen() || c.IsPopUp()) 
			c.Close();
		c.StateNotDocked();
	}
}

void DockWindow::DockAsTab(DockCont &target, DockableCtrl &dc)
{
	DockCont *c = GetContainer(dc);
	if (c) {
		if (c->GetCount() == 1)
			CloseContainer(*c);
		else {
			Ctrl *c = dc.GetParent();
			dc.Remove();
			c->Layout();
		}
	}
	target.Add(dc);
}

void DockWindow::Dock0(int align, Ctrl &c, int pos, bool do_animate)
{
	Size sz = CtrlBestSize(c);
	int fsz = IsTB(align) ? sz.cy : sz.cx;
	if (!dockframe[align].IsShown())
		dockframe[align].Show();
	if (fsz > dockframe[align].GetSize())
		do_animate ? StartFrameAnimate(align, fsz) : dockframe[align].SetSize(fsz);
	dockpane[align].Dock(c, sz, pos, do_animate);
}

/*
 * Container docking
*/ 
void DockWindow::DockContainer(int align, DockCont &c, int pos, int except)
{
	DockCont *cont = &c;
	if (except >= 0) {
		cont = CreateContainer();
		cont->AddFrom(c, except);
		c.RefreshLayout();
	}
	else 
		Detach(c);
	cont->StateDocked(*this);
	Dock0(align, *cont, pos);
}

void DockWindow::DockContainerAsTab(DockCont &target, DockCont &c, bool do_nested)
{
	Detach(c);	
	if (do_nested) {
		c.StateTabbed(*this);
		target << c;
	}
	else {
		target.AddFrom(c);
		DestroyContainer(c);	
	}
}

void DockWindow::FloatContainer(DockCont &c, int except, Point p)
{
	ASSERT(IsOpen());
	if (c.IsFloating() && except < 0) return;
	if (p.IsNullInstance()) 
		p = GetScreenRect().CenterPoint();
	DockCont *cont = &c;
	if (except >= 0) {
		cont = CreateContainer();
		cont->AddFrom(c, except);
		cont->SetRect(c.GetScreenRect());	
		c.RefreshLayout();	
	}	
	else
		Detach(c);	
		
	cont->StateFloating(*this);
	Size best = CtrlBestSize(c, false);
	cont->SetRect(Rect(p, best));
	cont->Open(this);
}

void DockWindow::FloatFromTab(DockCont &c, DockCont &tab)
{
	Rect r = c.GetScreenRect();
	tab.SetRect(r);
	tab.StateDocked(*this);
	tab.MoveBegin();
}

void DockWindow::AutoHideContainer(int align, DockCont &c, int except)
{
	except = except < 0 ? c.GetCount() : except;
	for (int i = 0; i < except; i++)
		AutoHide(align, c.Get(0));	
	while(c.GetCount() > 1)
		AutoHide(align, c.Get(1));	
}

void DockWindow::CloseContainer(DockCont &c)
{
	c.Clear(); 
	Detach(c);
	DestroyContainer(c);	
}

/*
 * Docking Interface helpers
*/ 
DockCont *DockWindow::GetMouseDockTarget()
{
	Point mp = GetMousePos();
	for (int i = 0; i < 4; i++) {
		if (dockpane[i].IsShown()) {
			Rect r = dockpane[i].GetScreenRect();
			if (r.Contains(mp))
				return dynamic_cast<DockCont *>(dockpane[i].ChildFromPoint(mp -= r.TopLeft()));
		}
	}
	return NULL;
}

int DockWindow::GetDockAlign(const Ctrl &c) const
{
	Ctrl *p = c.GetParent();
	while (p && p->GetParent() != this) p = p->GetParent();
	for (int i = 0; i < 4; i++) 
		if (dockpane[i].GetParent() == p) return i;
	return DOCK_NONE;
}

int DockWindow::GetDockAlign(const Point &p) const
{
	for (int i = 0; i < 4; i++) 
		if (dockpane[i].IsVisible() && dockpane[i].GetScreenRect().Contains(p)) 
			return i; 
	return DOCK_NONE;
}

Size DockWindow::CtrlBestSize(const Ctrl &c, bool restrict) const
{
	Size mn = c.GetMinSize();
	Size mx = c.GetMaxSize();
	Size std = c.GetStdSize();
	if (restrict) 
		mx = minmax(GetSize()/2, mn, mx);
	return minmax(std, mn, mx);
}

void DockWindow::StartHighlight(DockCont *dc)
{
	int align = DOCK_NONE;
	DockCont *target = FindDockTarget(*dc, align);
	if (align != DOCK_NONE || target) {
		dc->SyncUserSize(true, true);
		Highlight(align, *dc, target);
	}
	else 
		StopHighlight(animate);
}

void DockWindow::Highlight(int align, DockCont &cont, DockCont *target)
{
	HighlightCtrl &hl = GetHighlightCtrl();
	DockableCtrl &dc = cont.GetCurrent();
	ASSERT(&hl != (Ctrl *)&cont);

	hl.SizeHint(cont.GetMinSize(), cont.GetMaxSize(), cont.GetStdSize()).Show();
	hl.oldframesize = dockframe[align].GetSize();	
	
	if (align != DOCK_NONE) {
		hl.isnested = false;
		hl.nestedkey = 0;
		hl.highlight = &dc.GetStyle().highlight[0];
		int pos = target ? dockpane[align].FindIndex(*target) : -1;
		Dock0(align, hl, pos, animate);
	}
	else if (target && IsTabbing()) {
		hl.Title(cont.GetTitle(true)).Icon(dc.GetIcon());
		hl.isnested = IsNestedTabs() && (cont.GetCount() > 1);
		hl.nestedkey =  (cont.GetCount() > 1) ? NestedToggleKey() : 0;
		hl.highlight = &dc.GetStyle().highlight[1];
		DockAsTab(*target, hl);	
	}
}

void DockWindow::StopHighlight(bool do_animate)
{
	HighlightCtrl &hl = GetHighlightCtrl();
	if (hl.GetParent()) Undock0(hl, do_animate, hl.oldframesize);
}

DockCont *DockWindow::FindDockTarget(DockCont &dc, int &dock)
{

	Point p = GetMousePos();
	Rect r = GetScreenView();
	DockCont *target = NULL;
	int align = DOCK_NONE;
	
	dock = DOCK_NONE;
	
	if (r.Contains(p)) {
		dock = GetPointAlign(p, r, true, true, true);
		if (dock != DOCK_NONE && dockpane[dock].IsVisible())
			dock = DOCK_NONE;
	}
	else {
		target = GetMouseDockTarget();		
		if (target) {
			r = target->GetScreenRect();
			dock = GetDockAlign(*target);
			align = GetPointAlign(p, r, IsTabbing(), IsTB(dock), !IsTB(dock));
		}
		else 
			return NULL;
	}
	
	if (dock != DOCK_NONE && (!dc.IsDockAllowed(dock) 
		 || dockpane[dock].IsAnimating() || IsFrameAnimating(dock))
		 || (dock == DOCK_NONE && !target)) {
		dock = DOCK_NONE;
		return NULL;
	}	
	
	// Prepare for highlight
	if (target) {
		GetHighlightCtrl().bounds = GetAlignBounds(align, r, IsTabbing());	
		if (align == DOCK_NONE) 
			dock = DOCK_NONE; // Tabbing
		// The following code handles the case of an insertion between two docked controls. In this case we must set 
		//  the highlight bounds to be a union of the bounds from each control. Very ugly.
		if (dock != DOCK_NONE) {
			Ctrl *c = IsTL(align) ? target->GetPrev() : target->GetNext();
			if (c) {
				int opal = align > 1 ? align-2 : align+2;
				GetHighlightCtrl().bounds.Union(GetAlignBounds(opal, c->GetScreenRect(), IsTabbing()));
			}
			target = IsTL(align) ? target : dynamic_cast<DockCont*>(target->GetNext());
		}
		
	}
	else if (dock != DOCK_NONE)
		GetHighlightCtrl().bounds = GetAlignBounds(dock, r, true);	
		
	return target;	
}

Rect DockWindow::GetAlignBounds(int al, Rect r, bool center)
{
	Size border = r.GetSize()/4;
	switch (al) {
	case DOCK_LEFT:
		r.right = r.left + (center ? border.cx : (GHalf_(r.left + r.right)));
		return r;
	case DOCK_TOP:
		r.bottom = r.top + (center ? border.cy : (GHalf_(r.top + r.bottom)));
		return r;
	case DOCK_RIGHT:
		r.left = r.right - (center ? border.cx : (GHalf_(r.left + r.right)));
		return r;
	case DOCK_BOTTOM:
		r.top = r.bottom - (center ? border.cy : (GHalf_(r.top + r.bottom)));
		return r;
	}
	r.Deflate(border);
	return r;
}	

int DockWindow::GetPointAlign(const Point p, Rect r, bool center, bool allow_lr, bool allow_tb)
{
	Size border = r.GetSize();
	border.cx = allow_lr ? border.cx/4 : 0;
	border.cy = allow_tb ? border.cy/4 : 0;
	if (center && r.Deflated(border).Contains(p))
		return DOCK_NONE;
	int q = GetQuad(p, r);
	int al = DOCK_NONE;
	if (q == 0 || q == 1)
		al = DOCK_TOP;
	else if (q == 2 || q == 3)
		al = DOCK_RIGHT;
	else if (q == 4 || q == 5)
		al = DOCK_BOTTOM;
	else if (q == 6 || q == 7)
		al = DOCK_LEFT;
	if (!allow_lr && (al == DOCK_LEFT || al == DOCK_RIGHT))
		al = (q == 7 || q == 2) ? DOCK_TOP : DOCK_BOTTOM;
	else if (!allow_tb && (al == DOCK_TOP || al == DOCK_BOTTOM))
		al = (q == 0 || q == 5) ? DOCK_LEFT : DOCK_RIGHT;
	return al;
}

int DockWindow::GetQuad(Point p, Rect r)
/* Finds the quadrant in the rectangle for point p:
	-------
	|\0|1/|
	|7\|/2|
	|-----| 
	|6/|\3|
	|/5|4\|
	-------
*/
{
	Point cp = r.CenterPoint();
	p -= cp;
	cp -= r.TopLeft();
	if (p.x <= 0) { // Left
		if (p.y <= 0) // Top-Left
			return ((p.y * cp.x)/cp.y > p.x) ? 7 : 0;
		else {// Bottom-Left
			p.x = -p.x;
			return ((p.y * cp.x)/cp.y > p.x) ? 5 : 6;
		}
	}
	else { // Right
		if (p.y <= 0) { // Top-Right
			p.x = -p.x;
			return ((p.y * cp.x)/cp.y > p.x) ? 2 : 1;
		}
		else // Bottom-Right
			return ((p.y * cp.x)/cp.y > p.x) ? 4 : 3;		
	}
}

void DockWindow::HighlightCtrl::Paint(Draw &w)
{	
	if (isnested || (GetMouseFlags() & nestedkey)) {
		Rect r = GetSize();
		Rect t = r;
		const TabCtrl::Style &s = TabCtrl::StyleDefault();
		t.bottom -= s.tabheight + s.sel.top + 1;	// Nasty bodge! See TabBar::GetStyleHeight
		r.top = t.bottom-1;
		r.right = max(min(150, r.GetWidth()/3), 20);
		r.left += 10;
		ChPaint(w, r, *highlight);
		ChPaint(w, t, *highlight);
	}
	else if (!isnested || (GetMouseFlags() & nestedkey))
		ChPaint(w, GetSize(), *highlight);
}

// Drag and Drop interface
void DockWindow::ContainerDragStart(DockCont &dc)
{
	if (!dc.IsFloating()) {
		// Reposition if not under the mouse
		Rect r = dc.GetScreenRect();
		Point pt = GetMousePos();
		Point tl = r.TopLeft();
		bool move = false;
		if (r.left > pt.x || r.right < pt.x) {
			tl.x += pt.x - r.left - r.Width()/2;
			move = true;
		}
		if (r.top < pt.y) {
			tl.y += pt.y - r.top + DOCKCONT_WND_OFFSET;
			move = true;
		}
		// Note: Due to different bugfix, at this point a dragged tab will have docked state but 
		//	no parent and should not be animated
		dc.SyncUserSize(true, true);
		if (animate && dc.IsDocked() && dc.GetParent()) {
			Undock0(dc, true);
			dc.StateNotDocked();
		}
		FloatContainer(dc, -1, move ? tl : Null);
		dc.StartMouseDrag(pt);
	}
}

void DockWindow::ContainerDragMove(DockCont &dc)
{
	HighlightCtrl &hl = GetHighlightCtrl();
	Point p = GetMousePos();
	if (hl.GetParent()) {
		if (!hl.bounds.Contains(p))
			StopHighlight(animate);
		return KillTimeCallback(TIMEID_ANIMATE_DELAY);
	}	
	animdelay ? 
		KillSetTimeCallback(animdelay, THISBACK1(StartHighlight, &dc), TIMEID_ANIMATE_DELAY) :
		StartHighlight(&dc);
}
	
void DockWindow::ContainerDragEnd(DockCont &dc)
{
	KillTimeCallback(TIMEID_ANIMATE_DELAY);
	if (!dc.IsFloating()) return;
	HighlightCtrl &hl = GetHighlightCtrl();

	Ctrl *p = hl.GetParent();
	int align = DOCK_NONE; 

	if (p) 
		for (int i = 0; i < 4; i++)
			if (p == &dockpane[i]) align = i;

	if (align != DOCK_NONE) {
		Unfloat(dc);
		dc.StateDocked(*this);
		dockpane[align].Swap(hl, dc);
	}
	else if (DockCont *target = dynamic_cast<DockCont *>(p)) {
		StopHighlight(false);
		bool nest = GetCtrl() ? !IsNestedTabs() : IsNestedTabs();
		DockContainerAsTab(*target, dc, nest && dc.GetCount() > 1);
	}
	else
		StopHighlight(false);
	
	if (dc.GetParent())
		dc.SetFocus();
}

/*
 * Misc
*/ 
void DockWindow::SetFrameSize(int align, int size)
{
	ALIGN_ASSERT(align);
	dockframe[align].SetSize(size);
}

DockWindow & DockWindow::Animate(bool _animate, int ticks, int interval)
{
	animate = _animate;
	ticks = max(ticks, 1);
	interval = max(interval, 1);
	for (int i = 0; i < 4; i++) 
		dockpane[i].SetAnimateRate(ticks, interval);
	return *this;
}

DockWindow & DockWindow::Grouping(bool _grouping)
{
	if (_grouping != grouping) {
		grouping = _grouping;
		SyncAll();
	}
	return *this;
}

DockWindow & DockWindow::AllowDockAll()
{
	return AllowDockLeft().AllowDockRight().AllowDockTop().AllowDockBottom();	
}

DockWindow & DockWindow::AllowDockNone()
{
	return AllowDockLeft(false).AllowDockRight(false).AllowDockTop(false).AllowDockBottom(false);
}

bool DockWindow::IsDockAllowed(int align, DockableCtrl &dc)
{
	ALIGN_ASSERT(align);
	return dockable[align] && dc.IsDockAllowed(align);
}

DockWindow & DockWindow::AutoHide(bool v)
{
	if (v != autohide) {
		autohide = v;
		SyncAll();
	}
	return *this;
}

DockWindow & DockWindow::WindowButtons(bool menu, bool hide, bool close)
{
	if (menu == menubtn && close == closebtn && hide == hidebtn) return *this;;
	menubtn = menu;
	closebtn = close;
	hidebtn = hide;
	SyncAll();
 	return *this;	
}

void DockWindow::DockLayout()
{
	if (hideframe[0].GetParent())
		for (int i = 0; i < 4; i++) {
			RemoveFrame(hideframe[i]);
			RemoveFrame(dockframe[i]);
		}
	// Top, Bottom, Left, Right
	AddFrame(hideframe[1]);
	AddFrame(hideframe[3]);
	AddFrame(hideframe[0]);
	AddFrame(hideframe[2]);		
	AddFrame(dockframe[1]);
	AddFrame(dockframe[3]);
	AddFrame(dockframe[0]);
	AddFrame(dockframe[2]);
}

DockableCtrl & DockWindow::Dockable(Ctrl &ctrl, WString title)
{
	DockableCtrl &dc = Register(ctrls.Add().SizeHint(ctrl.GetMinSize()));
	if (!IsNull(title)) dc.Title(title);
	dc << ctrl.SizePos();
	return dc;
}

void DockWindow::DockManager()
{
	DockConfigDlg(*this).Run();
}

void DockWindow::DockWindowMenu(Bar &bar)
{
	if (IsGrouping())
		menu.GroupListMenu(bar);
	else
		menu.WindowListMenu(bar, "All");
	bar.Separator();
	if (layouts.GetCount()) {
		bar.Add("Layouts", callback(&menu, &DockMenu::LayoutListMenu));
		bar.Separator();
	}
	bar.Add("Manage Windows...", THISBACK(DockManager));
}

void DockWindow::SerializeWindow(Stream &s)
{
	int version = 3;
	int v = version;
	s / v;
	ASSERT(v == version);
	
	SerializeLayout(s, true);
	
	s % tabbing % autohide % animate % nestedtabs 
	  % grouping % menubtn % closebtn % hidebtn;
	  
	 if (s.IsLoading()) SyncAll();
}

void DockWindow::SerializeLayout(Stream &s, bool withsavedlayouts)
{
	StopHighlight(false);
	int cnt = 0;
	// Groups
	ArrayMap<String, Vector<int> > groups;
	if (s.IsStoring())
		for (int i = 0; i < dockers.GetCount(); i++) {
			String g = dockers[i]->GetGroup();
			if (!g.IsEmpty()) {
				int ix = groups.Find(g);
				if (ix < 0) {
					groups.Add(dockers[i]->GetGroup(), Vector<int>());
					ix = groups.GetCount() - 1;
				}	
				groups[ix].Add(i);
			}
		}
	s % groups;
	if (s.IsLoading()) {
		// Close everything
		for (int i = 0; i < conts.GetCount(); i++)
			CloseContainer(conts[i]);
		for (int i = 0; i < 4; i++)
			dockpane[i].Clear();
		conts.Clear();
				
		for (int i = 0; i < dockers.GetCount(); i++)
			dockers[i]->SetGroup(Null);
		for (int i = 0; i < groups.GetCount(); i++) {
			Vector<int> &v = groups[i];
			const String &g = groups.GetKey(i);
			for (int j = 0; j < v.GetCount(); j++) {
				int ix = v[j];
				if (ix >= 0 && ix < dockers.GetCount())
					dockers[ix]->SetGroup(g);
			}
		}
	}
			    
	if (s.IsStoring()) {
		// Write docked
		for (int i = 0; i < 4; i++) {
			DockPane &pane = dockpane[i];
			int fsz = dockframe[i].IsShown() ? dockframe[i].GetSize() : 0;
			cnt = pane.GetCount();
			
			s / fsz / cnt;
			DockCont *dc = dynamic_cast<DockCont *>(pane.GetFirstChild());
			for (int j = 0; dc && j < pane.GetCount(); j++) {
				fsz = pane.GetCtrlSize(j);
				s / fsz % *dc;
				dc = dynamic_cast<DockCont *>(dc->GetNext());
			}
		}
		// Count Floating
		for (int i = 0; i < conts.GetCount(); i++)
			if (conts[i].IsFloating()) cnt++;
		// Write Floating
		s / cnt;
		for (int i = 0; i < conts.GetCount(); i++) {
			if (conts[i].IsFloating()) {
				conts[i].Serialize(s);
				conts[i].SerializePlacement(s, false);
			}
		}
		// Write Autohidden
		for (int i = 0; i < 4; i++) {
			cnt = hideframe[i].GetCount();
			s / cnt;
			for (int j = 0; j < hideframe[i].GetCount(); j++) {
				int ix = FindDocker(&hideframe[i].GetCtrl(j)->Get(0));
				if (ix >= 0)
					s / ix;
			}
		}		
	}
	else {
		// Read docked
		for (int i = 0; i < 4; i++) {
			DockPane &pane = dockpane[i];
			cnt = pane.GetCount();
			dockframe[i].Hide();
			int fsz, psz;
			s / fsz / cnt;

			for (int j = 0; j < cnt; j++) {
				DockCont *dc = CreateContainer();
				s / psz % *dc;
				dc->StateDocked(*this);
				pane.QuickDock(*dc, psz);
			}
			if (fsz && pane.GetCount()) {
				dockframe[i].SetSize(fsz);
				dockframe[i].Show();
			}			
		}	
		// Read floating
		s / cnt;
		for (int i = 0; i < cnt; i++) {
			DockCont *dc = CreateContainer();
			dc->Serialize(s);
			FloatContainer(*dc);
			dc->SerializePlacement(s, false);	
		}
		// Read Autohidden
		for (int i = 0; i < 4; i++) {
			s / cnt;
			for (int j = 0; j < cnt; j++) {
				int ix;
				s / ix;
				if (ix >= 0 && ix < dockers.GetCount())
					AutoHide(i, *dockers[ix]);
			}		
		}
		// Clear empty containers
		for (int i = conts.GetCount()-1; i >= 0; i--) {
			if (!conts.GetCount()) CloseContainer(conts[i]);
		}
		RefreshLayout();
	}
	bool haslay = withsavedlayouts;
	s % haslay;
	if (withsavedlayouts && (s.IsStoring() || haslay))
		s % layouts;
}

int DockWindow::SaveLayout(String name)
{
	ASSERT(!name.IsEmpty());
	StringStream s;
	SerializeLayout(s, false);
	int ix = layouts.FindAdd(name, String());
	layouts[ix] = (String)s;
	return ix;
}

void DockWindow::LoadLayout(int ix)
{
	ASSERT(ix >= 0 && ix < layouts.GetCount());
	StringStream s((String)layouts[ix]);
	SerializeLayout(s, false);
	RefreshLayout();
}

void DockWindow::EnableFloating(bool enable)
{
	for (int i = 0; i < conts.GetCount(); i++)
		if (conts[i].IsFloating())
			conts[i].Enable(enable);
	
}

DockWindow::DockWindow()
{
	menu.Set(*this);
	init = false;
	tabbing = animate = grouping = true;
	nestedtabs = false;
	menubtn = closebtn = hidebtn = true;
	animdelay = 100;
	
	for (int i = 0; i < 4; i++) {
		dockframe[i].Set(dockpane[i], 0, i);
		IsTB(i) ? dockpane[i].Horz() : dockpane[i].Vert();
		hideframe[i].SetAlign(i);
		dockframe[i].Hide();
	}
	AllowDockAll().AutoHide(true);
}

// PopUpDockWindow
#define POPUP_SPACING 0
/*
void PopUpDockWindow::ContainerDragStart(DockCont &dc)
{
	DockWindow::ContainerDragStart(dc);

	last_target = NULL;
	last_popup = NULL;
	last_cp = Null;
}

void PopUpDockWindow::ContainerDragMove(DockCont &dc)
{
	// Is the highlight the same as last time?
	if (last_popup && last_popup->IsPopUp() && last_popup->GetRect().Contains(GetMousePos()))
		return; // Don't need to do anything
	
	DockCont *target = GetMouseDockTarget();
	if (target && !dc.IsDockAllowed(GetDockAlign(*target))) target = NULL;
	
	// Hide show inner popups as necessary
	if (!target && last_target != NULL)
		HidePopUps(true, false);
	else if (target != last_target)
		ShowInnerPopUps(dc, target);
	ShowOuterPopUps(dc);
	last_target = target;
	
	// Is the mouse in an inner popup?
	int al = DOCK_NONE;
	int hal = DOCK_NONE;
	hal = PopUpHighlight(inner, 5);
	if (target && hal >= 0) 
		al = (hal == 4) ? DOCK_NONE : hal;
	else
		target = NULL;
	
	// Is the mouse in an outer popup?
	hal = PopUpHighlight(outer, 4);
	if (al == DOCK_NONE && hal != DOCK_NONE)
		al = hal;
	
	// Do highlight
	if (al != DOCK_NONE || target) {
		if (al == DOCK_NONE)
			StopHighlight(IsAnimated());		
		dc.SyncUserSize(true, true);
		Highlight(al, dc.GetCurrent(), target);
	}
	else  {
		StopHighlight(IsAnimated());
		last_popup = NULL;
	}
}

void PopUpDockWindow::ContainerDragEnd(DockCont &dc)
{
	HidePopUps(true, true);
	last_target = NULL;
	DockWindow::ContainerDragEnd(dc);
}

int PopUpDockWindow::PopUpHighlight(PopUpButton *pb, int cnt)
{
	int ix = -1;
	Point p = GetMousePos();
	for (int i = 0; i < cnt; i++) {
		if (pb[i].IsPopUp() && pb[i].GetRect().Contains(p)) {
			if (!pb[i].hlight) {
				pb[i].hlight = &(style->highlight);
				pb[i].Refresh();
			}
			ix = i;
			last_popup = &pb[i];
		}
		else if (pb[i].hlight) {
			pb[i].hlight = NULL;
			pb[i].Refresh();
		}
	}
	return ix;
}

void PopUpDockWindow::ShowOuterPopUps(DockCont &dc)
{
	Rect wrect = GetScreenRect();
	Point cp = wrect.CenterPoint();
	Size psz(style->outersize, style->outersize);
	Rect prect = Rect(psz);	
	psz /= 2;
	
	wrect.Deflate(12, 12);
	
	if (dc.IsDockAllowed(DOCK_LEFT)) 	ShowPopUp(outer[DOCK_LEFT], prect.Offseted(wrect.left + POPUP_SPACING, cp.y - psz.cy));	
	if (dc.IsDockAllowed(DOCK_TOP)) 	ShowPopUp(outer[DOCK_TOP], prect.Offseted(cp.x - psz.cx, wrect.top + POPUP_SPACING));	 
	if (dc.IsDockAllowed(DOCK_RIGHT)) 	ShowPopUp(outer[DOCK_RIGHT], prect.Offseted(wrect.right - POPUP_SPACING - psz.cx*2, cp.y - psz.cy));
	if (dc.IsDockAllowed(DOCK_BOTTOM)) 	ShowPopUp(outer[DOCK_BOTTOM], prect.Offseted(cp.x - psz.cx, wrect.bottom - POPUP_SPACING - psz.cy*2));
}

void PopUpDockWindow::ShowInnerPopUps(DockCont &dc, DockCont *target)
{
	ASSERT(target);
	Rect wrect = GetScreenRect();
	Size psz(style->innersize, style->innersize);
	Rect prect = Rect(psz);	
	psz /= 2;

	if (target != last_target) {
		last_cp = target->GetScreenRect().CenterPoint();
		int d = psz.cy * 5;
		last_cp.x = minmax(last_cp.x, wrect.left + d, wrect.right - d);
		last_cp.y = minmax(last_cp.y, wrect.top + d, wrect.bottom - d);		
	}
	Point cp = last_cp;
	
	bool lr = !IsSimple() || (IsTB(GetDockAlign(*target)) ? !IsHorzVert() : IsHorzVert());
	bool tb = !IsSimple() || !lr; // Whether top/bottom sub docking allowed

	if (lr) {
		ShowPopUp(inner[DOCK_LEFT], prect.Offseted(cp.x - psz.cx*3, cp.y - psz.cy));	
		ShowPopUp(inner[DOCK_RIGHT], prect.Offseted(cp.x + psz.cx, cp.y - psz.cy));	
	}
	else {
		inner[DOCK_LEFT].Close();	
		inner[DOCK_RIGHT].Close();
	}
	if (tb) {
		ShowPopUp(inner[DOCK_TOP], prect.Offseted(cp.x - psz.cx, cp.y - psz.cy*3));
		ShowPopUp(inner[DOCK_BOTTOM], prect.Offseted(cp.x - psz.cx, cp.y + psz.cy));
	}
	else {
		inner[DOCK_TOP].Close();	
		inner[DOCK_BOTTOM].Close();
	}
	if (IsTabbing())
		ShowPopUp(inner[4], prect.Offseted(cp.x-psz.cx, cp.y-psz.cy));
}

void PopUpDockWindow::ShowPopUp(PopUpButton &pb, const Rect &r)
{
	pb.SetRect(r);
	if (!pb.IsPopUp())
		pb.PopUp(this, false, true, true);
}

void PopUpDockWindow::HidePopUps(bool _inner, bool _outer)
{
	if (_inner)
		for (int i = 0; i < 5; i++)
			inner[i].Close();	
	if (_outer)
		for (int i = 0; i < 4; i++)
			outer[i].Close();	
}

PopUpDockWindow & PopUpDockWindow::SetStyle(const Style &s)
{
	style = &s;
	for (int i = 0; i < 5; i++)
		inner[i].icon = &s.inner[i];	
	for (int i = 0; i < 4; i++)
		outer[i].icon = &s.outer[i];
	return *this;	
}

PopUpDockWindow::PopUpDockWindow()
{
	SetStyle(StyleDefault());
}

CH_STYLE(PopUpDockWindow, Style, StyleDefault)
{
	outer[0] = DockingImg::DockLeft();
	outer[1] = DockingImg::DockTop();
	outer[2] = DockingImg::DockRight();
	outer[3] = DockingImg::DockBottom();
	
	inner[0] = DockingImg::DockLeft();
	inner[1] = DockingImg::DockTop();
	inner[2] = DockingImg::DockRight();
	inner[3] = DockingImg::DockBottom();
	inner[4] = DockingImg::DockTab();

	highlight = DockingImg::DockHL();
	
	innersize = 25;
	outersize = 25;
}
*/

