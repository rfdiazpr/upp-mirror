#include "Docking.h"

#define ALIGN_ASSERT(al)	ASSERT(al >= 0 && al < 4)

/*
 * Public interface
*/ 
void DockWindow::State(int reason)
{
	if (reason == Ctrl::OPEN && !dockframe[0].GetParent())
		DockLayout();
}

void DockWindow::Dock(int align, DockableCtrl &dc)
{
	ALIGN_ASSERT(align);
	DockCont *c = GetReleasedContainer(dc);
	c->StateDocked(*this);
	Dock0(align, *c);
}

void DockWindow::Dock(int align, DockableCtrl &target, DockableCtrl &dc)
{
	ALIGN_ASSERT(align);
	DockCont *c = GetReleasedContainer(dc);
	c->StateDocked(*this);
	Dock0(align, target, *c);
}

void DockWindow::DockAsTab(DockableCtrl &target, DockableCtrl &dc)
{
	DockCont *tc = GetContainer(target);
	ASSERT(tc);	
	DockCont *c = GetContainer(dc);
	if (c) {
		if (c->GetCount() == 1) {
			Undock(*c);
			Unfloat(*c);
			DestroyContainer(c);	
		}
		else
			dc.Remove();			
	}
	tc->Add(dc);
}

void DockWindow::Float(DockableCtrl &dc, Point p)
{
	if (dc.GetParent() && p.IsNullInstance()) p = GetScreenRect().TopLeft();         
	DockCont *c = GetReleasedContainer(dc);
	FloatContainer(*c, -1, p);
}

void DockWindow::AutoHide(int align, DockableCtrl &dc)
{
	ALIGN_ASSERT(align);
	DockCont *c = GetReleasedContainer(dc);
	c->StateAutoHide(*this);
	hideframe[align].AddCtrl(*c, dc.GetGroup());	
}

void DockWindow::Close(DockableCtrl &dc)
{
	DockCont *c = GetContainer(dc);
	if (c && c->GetCount() > 1) {
		dc.Remove();
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
	c->Add(dc);
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
	if (c && c->GetCount() > 1) {
		dc.Remove();
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
				int ix = hideframe[i].FindCtrl(c);
				if (ix >= 0) {
					hideframe[i].RemoveCtrl(ix);
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
			c.Close();
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
		cont->AddFrom(c, except);
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
	target.AddFrom(c);
	DestroyContainer(&c);	
}

void DockWindow::FloatContainer(DockCont &c, int except, Point p)
{
	ASSERT(IsOpen());
	if (c.IsFloating() && except < 0) return;
	if (p.IsNullInstance()) 
		p = this->GetScreenRect().CenterPoint();
	DockCont *cont = &c;
	if (except >= 0) {
		cont = CreateContainer();
		cont->AddFrom(c, except);
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
	cont->Open(this);
	cont->SetRect(Rect(p, best));
	cont->Show();
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
	except = except < 0 ? c.GetCount() : except;
	for (int i = 0; i < except; i++)
		AutoHide(align, c.Get(0));	
	while(c.GetCount() > 1)
		AutoHide(align, c.Get(1));	
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
				return cont ? &cont->GetCurrent() : NULL;
			}
		}
	}
	return NULL;
}

int DockWindow::GetDockAlign(Ctrl &c) const
{
	Ctrl *p = c.GetParent();
	while (p && p->GetParent() != this) p = p->GetParent();
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
		mx = minmax(GetSize()/2, mn, mx);
	return minmax(std, mn, mx);
}

void DockWindow::Highlight(int align, DockableCtrl &dc, DockableCtrl *target)
{
	HighlightCtrl &hl = GetHighlightCtrl();
	if (&hl == target) return;

	Ctrl *p = dc.GetParent();
	hl.SizeHint(p->GetMinSize(), p->GetMaxSize(), p->GetStdSize()).Show();
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
	HighlightCtrl &hl = GetHighlightCtrl();
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
	bool lr = hv || !simple || !target; // Whether left/right sub docking allowed
	bool tb = !hv || !simple || !target; // Whether top/bottom sub docking allowed
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
	if (target || al != DOCK_NONE) {
		GetHighlightCtrl().bounds = GetAlignBounds(al, r, IsTabbing() || !target, lr, tb);	
		// The following code handles the case of an insertion between two docked controls. In this case we must set 
		//  the highlight bounds to be a union of the bounds from each control. Very ugly.
		if (target && al != DOCK_NONE) {
			Ctrl *c = IsTL(al) ? dockframe[dock].GetPrevSibling(*target->GetParent()) 
								: dockframe[dock].GetNextSibling(*target->GetParent());
			if (c && dockframe[dock].IsCtrlVert(*c) == IsTB(al)) {
				int opal = al > 1 ? al-2 : al+2;
				GetHighlightCtrl().bounds.Union(GetAlignBounds(opal, c->GetScreenRect(), IsTabbing(), lr, tb));
			}
		}
	}
		
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
	HighlightCtrl &hl = GetHighlightCtrl();
	Point p = GetMousePos();
	if (hl.GetParent()) {
		if (!hl.bounds.Contains(p)) {
			StopHighlight(animate);
		}
		else
			return;
	}
	
	DockableCtrl *target = FindDockTarget(dc, al);
	if (al != DOCK_NONE || target) {
		dc.SyncUserSize(true, true);
		Highlight(al, dc.GetCurrent(), target);
	}
	else 
		StopHighlight(animate);
}

void DockWindow::ContainerDragEnd(DockCont &dc)
{
	if (!dc.IsFloating()) return;
	HighlightCtrl &hl = GetHighlightCtrl();

	Ctrl *p = hl.GetParent();
	int al = GetDockAlign(hl);

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

void DockWindow::DockLayout()
{
	if (dockframe[0].GetParent())
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

DockWindow::DockWindow()
{
	simple = tabbing = animate = true;
	for (int i = 0; i < 4; i++) {
		dockframe[i].Set(100, i).Hide();
		hideframe[i].SetAlign(i);
	}
	AllowDockAll().AutoHide(true).SetVertHorz();
}

// PopUpDockWindow
#define POPUP_SPACING 0

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
	
	DockableCtrl *target = GetMouseDockTarget();
	if (target == &GetHighlightCtrl()) {
		target = NULL;
	}
	if (target && !dc.IsDockAllowed(GetDockAlign(*target)))
		target = NULL;
	
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

void PopUpDockWindow::ShowInnerPopUps(DockCont &dc, DockableCtrl *target)
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
	outer[0] = WindowImg::DockLeft();
	outer[1] = WindowImg::DockTop();
	outer[2] = WindowImg::DockRight();
	outer[3] = WindowImg::DockBottom();
	
	inner[0] = WindowImg::DockLeft();
	inner[1] = WindowImg::DockTop();
	inner[2] = WindowImg::DockRight();
	inner[3] = WindowImg::DockBottom();
	inner[4] = WindowImg::DockTab();

	highlight = WindowImg::DockHL();
	
	innersize = 25;
	outersize = 25;
}

