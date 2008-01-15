#include "Docking.h"

#define ALIGN_ASSERT(al)	ASSERT(al >= 0 && al < 4)
/*
void DockWindow::EventProc(XWindow& w, XEvent *event)
{
	if (event->type == CirculateRequest) 
	{
		XCirculateEvent &e = event->xcirculate;
		if (e.place == PlaceOnTop)
			OpenAllFloating();
		else if (e.place == PlaceOnBottom)
			CloseAllFloating();
	}
	else if (event->type == ConfigureNotify)
	{
		return;
		int q = Xwindow().Find(fw);
		XConfigureEvent& e = event->xconfigure;		
		if (e.above == None)
			CloseAllFloating();
		else
			OpenAllFloating();
			
	}
	TopWindow::EventProc(w, event);
}
*/

/*
 * Public interface
*/ 
void DockWindow::Dock(int align, DockableCtrl &dc)
{
	ALIGN_ASSERT(align);
	DockCont *c = GetReleasedContainer(dc);
	c->StateDocked(*this);
	Dock0(align, *c);
	dc.SetFocus();
}

void DockWindow::Dock(int align, DockableCtrl &target, DockableCtrl &dc)
{
	ALIGN_ASSERT(align);
	DockCont *c = GetReleasedContainer(dc);
	c->StateDocked(*this);
	Dock0(align, target, *c);
	dc.SetFocus();		
}

void DockWindow::DockAsTab(DockableCtrl &target, DockableCtrl &dc)
{
	DockCont *tc = GetContainer(target);
	ASSERT(tc);	
	DockCont *c = GetContainer(dc);
	if (c) {
		if (c->GetDCCount() == 1) {
			Undock(*c);
			Unfloat(*c);
			DestroyContainer(c);	
		}
		else
			c->RemoveDC(dc);			
	}
	tc->AddDC(dc);
	dc.SetFocus();
}

void DockWindow::Float(DockableCtrl &dc, Point p)
{
	if (dc.GetParent() && p.IsNullInstance()) p = GetScreenRect().TopLeft();         
	DockCont *c = GetReleasedContainer(dc);
	FloatContainer(*c, -1, p);
	dc.SetFocus();
}

void DockWindow::AutoHide(int align, DockableCtrl &dc)
{
	ALIGN_ASSERT(align);
	DockCont *c = GetReleasedContainer(dc);
	c->StateAutoHide(*this);
	hideframe[align].AddTab(*c, dc.GetTitle());	
}

void DockWindow::Close(DockableCtrl &dc)
{
	DockCont *c = GetContainer(dc);
	if (c && c->GetDCCount() > 1) {
		c->RemoveDC(dc);
		c = NULL;
	}
	if (c) {
		Undock(*c);
		Unfloat(*c);
		DestroyContainer(c);
	}
}

/*
 * Docking status menu and windows
*/ 
/*
void DockWindow::ShowWindowStatus()
{
	TopWindow dc;
	ArrayCtrl list;
	
	list.AddColumn("Window");
	list.AddColumn("Position");
	list.AddColumn("");
	list.ColumnWidths("91 157 40").SetLineCy(list.GetLineCy()+2);
	list.SetCount(dcs.GetCount());
	list.NoCursor(true).MultiSelect(false);
	for (int i = 0; i < dcs.GetCount(); i++) {
		DockableCtrl *dc = dcs[i];
		StaticText *text = new StaticText();
		Button *btn = new Button();
		text->SetText(GetWindowStatus(dc));
		btn->SetImage(CtrlsImg::DA()) <<= THISBACK2(OnWindowStatusButton, text, dc);
		list.Set(i, 0, dc->GetTitle());
		list.SetCtrl(i, 1, text);
		list.SetCtrl(i, 2, btn);
	}
	dc.Add(list.SizePos());
	dc.SetRect(Rect(0, 0, 250, 200));
	dc.CenterScreen();
	dc.Execute();
}

String DockWindow::GetWindowStatus(DockableCtrl *dc)
{
	if (dc->IsFloating())
		return "Floating";
	else if (dc->IsAutoHide())
		return "AutoHide";
	else if (dc->IsHidden())
		return "Hidden";
	String s = dc->IsTabbed() ? "Tabbed" : "Docked";
	int al = GetDockAlign(*dc);
	switch (al) {
		case DOCK_LEFT:
			return s + " (LEFT)";
		case DOCK_RIGHT:
			return s + " (RIGHT)";
		case DOCK_TOP:
			return s + " (TOP)";
		case DOCK_BOTTOM:
			return s + " (BOTTOM)";
	}
	return s;
}

void DockWindow::OnWindowStatusButton(StaticText *text, DockableCtrl *dc)
{
	dc->WindowMenu();
	text->SetText(GetWindowStatus(dc));
}
*/

/*
 * Container management
*/ 
DockCont * DockWindow::CreateContainer()
{
	DockCont *dc = &conts.Create<DockCont>();
	dc->StateNotDocked(this);
	return dc;
}

DockCont * DockWindow::CreateContainer(DockableCtrl &dc)
{
	DockCont *c = CreateContainer();
	c->AddDC(dc);
	return c;
}

void DockWindow::DestroyContainer(DockCont *c)
{
	for (int i = 0; i < conts.GetCount(); i++)
		if (&conts[i] == c) 
			return conts.Remove(i);
}

DockCont *DockWindow::GetReleasedContainer(DockableCtrl &dc)
{
	DockCont *c = GetContainer(dc);
	if (c && c->GetDCCount() > 1) {
		c->RemoveDC(dc);
		c->RefreshLayout();
		c = NULL;
	}
	if (c) {
		Undock(*c);
		Unfloat(*c);
	}
	else
		c = CreateContainer(dc);
	return c;
}

/*
 * Docking/Undocking helpers
*/ 
void DockWindow::Undock0(Ctrl &c, bool do_animate, int fsz)
{
	int al = GetDockAlign(c);
	Ctrl *p = c.GetParent();
	if (p != &dockframe[al]) {
		c.Remove();
		if (p) p->RefreshLayout();
	}
	else if (do_animate)
		dockframe[al].AnimateRemove(c, fsz);
	else
		dockframe[al].Remove(c, fsz);
}

void DockWindow::Undock(DockCont &c)
{
	bool layout = true; // Possible remove?
	if (!c.IsFloating() && !c.IsHidden()) {
		int dock = GetDockAlign(c);
		if (dock >= 0 && dock < 4) {
			bool v = dockframe[dock].IsCtrlVert(c);		
			c.SyncUserSize(v, !v);
		}
		if (c.IsAutoHide()) {
			for (int i = 0; i < 4; i++) {
				int ix = hideframe[i].GetTabIx(c);
				if (ix >= 0) {
					hideframe[i].RemoveTab(ix);
					if (layout) hideframe[i].RefreshFrame();
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
			c.FloatClose();
		c.StateNotDocked();
	}
}

void DockWindow::Dock0(int align, Ctrl &c, bool do_animate)
{
	Size sz = CtrlBestSize(c);
	if (do_animate)
		dockframe[align].AnimateDockRoot(c, false, sz);
	else
		dockframe[align].DockRoot(c, false, sz);
}

void DockWindow::Dock0(int align, DockableCtrl &target, Ctrl &c, bool do_animate)
{
	DockCont *tc = GetContainer(target);
	int dock = GetDockAlign(*tc);
	bool tb = IsTB(align);
	ASSERT(tc);	
	ALIGN_ASSERT(dock);	

	Size sz = CtrlBestSize(c);
	if (do_animate)
		dockframe[dock].AnimateDock(tb, IsTL(align), *tc, c, sz);
	else
		dockframe[dock].Dock(tb, IsTL(align), *tc, c, sz);
}

/*
 * Container docking
*/ 
void DockWindow::DockContainer(int align, DockCont &c, int except)
{
	DockCont *cont = &c;
	if (except >= 0) {
		cont = CreateContainer();
		cont->AddDCs(c, except);
		c.RefreshLayout();
	}
	else {
		Unfloat(c);
		Undock(c);		
	}
	cont->StateDocked(*this);
	Dock0(align, *cont);
}

void DockWindow::DockContainer(int align, DockableCtrl &target, DockCont &c)
{
	Unfloat(c);
	Undock(c);	
	ASSERT(GetContainer(target));
	c.StateDocked(*this);
	Dock0(align, target, c);
}

void DockWindow::DockContainerAsTab(DockCont &target, DockCont &c)
{
	Unfloat(c);
	Undock(c);	
	target.AddDCs(c);
	DestroyContainer(&c);	
}

void DockWindow::FloatContainer(DockCont &c, int except, Point p)
{
	if (c.IsFloating() && except < 0) return;
	if (p.IsNullInstance()) 
		p = this->GetScreenRect().CenterPoint();
	DockCont *cont = &c;
	if (except >= 0) {
		cont = CreateContainer();
		cont->AddDCs(c, except);
		cont->SetRect(c.GetScreenRect());	
		c.RefreshLayout();	
	}	
	else {
		Unfloat(c);
		Undock(c);		
	}
		
	cont->StateFloating(*this);
	cont->Hide();
	Size best = CtrlBestSize(c, false);
	if (IsOpen())
		cont->FloatOpen(this);
	cont->SetRect(Rect(p, best));
	cont->Show();
	cont->SetFocus();	
}

void DockWindow::FloatFromTab(DockCont &c, DockableCtrl &dc)
{
	Rect r = c.GetScreenRect();
	DockCont *nc = CreateContainer(dc);
	nc->SetRect(r);
	nc->StateDocked(*this);
	nc->MoveBegin();
}

void DockWindow::AutoHideContainer(int align, DockCont &c, int except)
{
	except = except < 0 ? c.GetDCCount() : except;
	for (int i = 0; i < except; i++)
		AutoHide(align, c.GetDC(0));	
	while(c.GetDCCount() > 1)
		AutoHide(align, c.GetDC(1));	
}

/*
 * Docking Interface helpers
*/ 
DockableCtrl *DockWindow::GetMouseDockTarget()
{
	Point mp = GetMousePos();
	for (int i = 0; i < 4; i++) {
		if (dockframe[i].IsShown()) {
			Rect r = dockframe[i].GetScreenRect();
			if (r.Contains(mp)) {
				Ctrl *c = &dockframe[i];
				mp -= r.TopLeft();
				c = c->ChildFromPoint(mp);
				DockCont *cont = dynamic_cast<DockCont *>(c);
				return cont ? &cont->GetCurrentDC() : NULL;
			}
		}
	}
	return NULL;
}

int DockWindow::GetDockAlign(Ctrl &c) const
{
	for (Ctrl *p = c.GetParent(); p; p = p->GetParent())
		for (int i = 0; i < 4; i++)
			if (&dockframe[i] == p) return i;
	return DOCK_NONE;
}

int DockWindow::GetDockAlign(const Point &p) const
{
	for (int i = 0; i < 4; i++) 
		if (dockframe[i].IsVisible() && dockframe[i].GetScreenRect().Contains(p)) 
			return i; 
	return DOCK_NONE;
}

Rect DockWindow::GetDockRect(int align) const
{
	ALIGN_ASSERT(align); 
	return dockframe[align].IsVisible() ? dockframe[align].GetScreenRect() : Null;			
}

Size DockWindow::CtrlBestSize(const Ctrl &c, bool restrict) const
{
	Size mn = c.GetMinSize();
	Size mx = c.GetMaxSize();
	Size std = c.GetStdSize();
	if (restrict) 
		mx = min(GetSize()/2, mx);
	return minmax(std, mn, mx);
}

void DockWindow::Highlight(int align, DockableCtrl &dc, DockableCtrl *target)
{
	HighlightCtrl &hl = Single<HighlightCtrl>();
	if (&hl == target) return;
	
	hl.SizeHint(dc.GetMinSize(), dc.GetMaxSize(), dc.GetParent()->GetStdSize()).Show();
	if (target && align != DOCK_NONE) {
		hl.oldframesize = dockframe[GetDockAlign(*target)].GetSize();	
		Dock0(align, *target, hl, animate);	
	}
	else if (target && IsTabbing()) {
		hl.oldframesize = dockframe[GetDockAlign(*target)].GetSize();
		hl.Title(dc.GetTitle()).Icon(dc.GetIcon());
		DockAsTab(*target, hl);	
	}
	else if (align != DOCK_NONE) {
		hl.oldframesize = dockframe[align].GetSize();
		Dock0(align, hl, animate);
	}
	hl.Show();
}

void DockWindow::StopHighlight(bool do_animate)
{
	HighlightCtrl &hl = Single<HighlightCtrl>();
	if (hl.GetParent()) {
		int al = GetDockAlign(hl);
		if (al >= DOCK_LEFT && al <= DOCK_BOTTOM)
			Undock0(hl, do_animate, hl.oldframesize);
	}
}

DockableCtrl * DockWindow::FindDockTarget(DockCont &dc, int &al)
{
	DockableCtrl *target = GetMouseDockTarget();
	Rect r = target ? target->GetParent()->GetScreenRect() : GetScreenView();
	int dock = target ? GetDockAlign(*target) : DOCK_NONE;

	// Abort if the frame is already animating something (layout will be incorrect)
	al = DOCK_NONE;	
	if (target && dockframe[dock].IsAnimating())
		return NULL; 

	Point p = GetMousePos();
	bool hv = IsTB(dock) ? !IsHorzVert() : IsHorzVert();
	bool lr = simple && hv || !simple || !target; // Whether left/right sub docking allowed
	bool tb = simple && !hv || !simple || !target; // Whether top/bottom sub docking allowed
	// Get alignment in target/base view
	if (target || r.Contains(p))
        al = GetPointAlign(p, r, IsTabbing() || !target, lr, tb);
	
	// Can't dock to side of base if the dock is already visible
	if (!target && al != DOCK_NONE && IsDockVisible(al))
		al = DOCK_NONE;
	// Check if docking is allowed
	else if ((al != DOCK_NONE || target) && !dc.IsDockAllowed(target ? dock : al)) {
		al = DOCK_NONE;
		target = NULL;
	}
	
	// Prepare for highlight
	if (target || al != DOCK_NONE)
		Single<HighlightCtrl>().bounds = GetAlignBounds(al, r, IsTabbing() || !target, lr, tb);
		
	return target;	
}

Rect DockWindow::GetAlignBounds(int al, Rect r, bool center, bool allow_lr, bool allow_tb)
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
	if (!allow_lr)
		r.DeflateVert(border.cy);
	else if (!allow_tb)
		r.DeflateHorz(border.cx);
	else 
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
	Rect r = GetSize();
	w.DrawRect(r, Blend(SLtBlue, SWhite, 90));
	DrawFrame(w, r, SLtBlue());
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
	int al = DOCK_NONE;
	HighlightCtrl &hl = Single<HighlightCtrl>();
	Point p = GetMousePos();
	if (hl.GetParent()) {
		if (!hl.bounds.Contains(p))
			StopHighlight(animate);
		else
			return;
	}
	
	DockableCtrl *target = FindDockTarget(dc, al);
	if (al != DOCK_NONE || target) {
		dc.SyncUserSize(true, true);
		Highlight(al, dc.GetCurrentDC(), target);
	}
	else 
		StopHighlight(animate);
}

void DockWindow::ContainerDragEnd(DockCont &dc)
{
	if (!dc.IsFloating()) return;
	HighlightCtrl &hl = Single<HighlightCtrl>();

	int al = GetDockAlign(hl);
	Ctrl *p = hl.GetParent();
	if (p == &dockframe[al]) {
		Unfloat(dc);
		dc.StateDocked(*this);
		dockframe[al].DockSwap(hl, dc);
	}
	else if (DockCont *target = dynamic_cast<DockCont *>(p)) {
		StopHighlight(false);
		DockContainerAsTab(*target, dc);
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

DockWindow & DockWindow::AutoHide(bool v)
{
	if (v != autohide) {
		autohide = v;
		for (int i = 0; i < conts.GetCount(); i++)
			conts[i].SyncButtons(conts[i].IsDockedAny());
	}
	return *this;
}

void DockWindow::DockLayout(Ctrl &_this)
{
	// Top, Bottom, Left, Right
	_this.AddFrame(*hideframe[1]);
	_this.AddFrame(*hideframe[3]);
	_this.AddFrame(*hideframe[0]);
	_this.AddFrame(*hideframe[2]);		
	_this.AddFrame(dockframe[1]);
	_this.AddFrame(dockframe[3]);
	_this.AddFrame(dockframe[0]);
	_this.AddFrame(dockframe[2]);
}

DockWindow::DockWindow()
{
	simple = tabbing = animate = true;
	for (int i = 0; i < 4; i++) {
		dockframe[i].Set(100, i).Hide();
		hideframe[i].Set(i);
	}
	AllowDockAll().AutoHide(true).SetVertHorz();
}

