#include "DockBase.h"

#define ALIGN_ASSERT(al)	ASSERT(al >= 0 && al < 4)
#define DOCK_DND_BORDER		50

/*
 * Public interface
*/ 
void DockBase::Dock(int align, DockableCtrl &dc)
{
	ALIGN_ASSERT(align);
	DockCont *c = GetReleasedContainer(dc);
	c->StateDocked(*this);
	Dock0(align, *c);
	dc.SetFocus();
}

void DockBase::Dock(int align, DockableCtrl &target, DockableCtrl &dc)
{
	ALIGN_ASSERT(align);
	DockCont *c = GetReleasedContainer(dc);
	c->StateDocked(*this);
	Dock0(align, target, *c);
	dc.SetFocus();		
}

void DockBase::DockAsTab(DockableCtrl &target, DockableCtrl &dc)
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

void DockBase::Float(DockableCtrl &dc)
{
	Point p = Null;
	if (dc.GetParent()) p = dc.GetParent()->GetScreenRect().TopLeft();         
	DockCont *c = GetReleasedContainer(dc);
	FloatContainer(*c, -1, p);
	dc.SetFocus();
}

void DockBase::AutoHide(int align, DockableCtrl &dc)
{
	ALIGN_ASSERT(align);
	DockCont *c = GetReleasedContainer(dc);
	c->StateAutoHide(*this);
	db_autohide[align].AddTab(*c, dc.GetTitle());	
}

void DockBase::Close(DockableCtrl &dc)
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
void DockBase::ShowWindowStatus()
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

String DockBase::GetWindowStatus(DockableCtrl *dc)
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

void DockBase::OnWindowStatusButton(StaticText *text, DockableCtrl *dc)
{
	dc->WindowMenu();
	text->SetText(GetWindowStatus(dc));
}
*/

/*
 * Container management
*/ 
DockCont * DockBase::CreateContainer()
{
	return &conts.Create<DockCont>();
}

DockCont * DockBase::CreateContainer(DockableCtrl &dc)
{
	DockCont *c = CreateContainer();
	c->AddDC(dc);
	return c;
}

void DockBase::DestroyContainer(DockCont *c)
{
	for (int i = 0; i < conts.GetCount(); i++)
		if (&conts[i] == c) 
			return conts.Remove(i);
}

DockCont *DockBase::GetReleasedContainer(DockableCtrl &dc)
{
	DockCont *c = GetContainer(dc);
	if (c && c->GetDCCount() > 1) {
		c->RemoveDC(dc);
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
void DockBase::Undock0(Ctrl &c, bool layout)
{
	int al = GetDockAlign(c);
	Ctrl *p = c.GetParent();
//	ASSERT(p);
	c.Remove();
	if (layout && p) {	
		p->RefreshLayout();
		if (db_dock[al].IsEmpty() && db_frame[al].IsShown())
			db_frame[al].Hide();
	}
}

void DockBase::Undock(DockCont &c)
{
	bool layout = true; // Possible remove?
	if (!c.IsFloating() && !c.IsHidden()) {
		c.SyncUserSize(true, true);
		if (c.IsAutoHide()) {
			for (int i = 0; i < 4; i++) {
				int ix = db_autohide[i].GetTabIx(c);
				if (ix >= 0) {
					db_autohide[i].RemoveTab(ix);
					if (layout) db_autohide[i].RefreshFrame();
					break;
				}
			}				
		}
		else
			Undock0(c, layout);		
		c.StateNotDocked();
	}
		
}

void DockBase::Unfloat(DockCont &c)
{
	if (c.IsFloating()) {
		c.SyncUserSize(true, true);
		c.Close();
		c.StateNotDocked();
	}
}

void DockBase::Dock0(int align, Ctrl &c)
{
	db_dock[align] << c;
	// Adjust and show frame if necessary
	SyncFrameSize(align, c);
}

void DockBase::Dock0(int align, DockableCtrl &target, Ctrl &c)
{
	DockCont *tc = GetContainer(target);
	int dock = GetDockAlign(*tc);
	bool tb = IsTB(align);
	ASSERT(tc);	
	ALIGN_ASSERT(dock);	

	Size sz = Null;
	if (smart_resize) {
		// Nasty! Have to set the frame size before adding and the ctrl tell the tree the size to use for the control
		if (tb && !db_dock[dock].IsRootVert() || !tb && db_dock[dock].IsRootVert()) {
			if (db_dock[dock].IsCtrlRoot(*target.GetParent())
				|| db_dock[dock].IsCtrlNearRoot(*target.GetParent()) ) {
				int fsz = db_frame[dock].GetSize();
				sz.cx = sz.cy = CtrlBestSize(tb, c);
				fsz += sz.cx;
				db_frame[dock].SetSize(fsz);
			}	
		}
	}
	db_dock[dock].AddSiblingUnder(IsTB(align), IsTL(align), *tc, c, sz);
	// Adjust and show frame if necessary
	if (sz.IsNullInstance()) 
		SyncFrameSize(dock, c);
}

void DockBase::SyncFrameSize(int dock, Ctrl &c)
{
	if (db_dock[dock].IsCtrlRoot(c)) {
		int fsz = (db_dock[dock].GetFirstChild() == db_dock[dock].GetLastChild()) ? 0 : db_frame[dock].GetSize();
		int nsz = fsz;
		if (db_dock[dock].IsRootVert())
			nsz = IsTB(dock) ? (fsz + CtrlBestSize(true, c)) : max(fsz, CtrlBestSize(false, c)); 
		else
			nsz = IsTB(dock) ? max(fsz, CtrlBestSize(true, c)) : (fsz + CtrlBestSize(false, c)); 
		if (nsz != fsz)
			db_frame[dock].SetSize(nsz);
	}
	db_frame[dock].Show();	
}

/*
 * Container docking
*/ 
void DockBase::DockContainer(int align, DockCont &c, int except)
{
	Unfloat(c);
	Undock(c);
	DockCont *cont = &c;
	if (except >= 0) {
		cont = CreateContainer();
		cont->AddDCs(c, except);
	}
	cont->StateDocked(*this);
	Dock0(align, c);
}

void DockBase::DockContainer(int align, DockableCtrl &target, DockCont &c)
{
	Unfloat(c);
	Undock(c);	
	ASSERT(GetContainer(target));
	c.StateDocked(*this);
	Dock0(align, target, c);
}

void DockBase::DockContainerAsTab(DockableCtrl &target, DockCont &c)
{
	Unfloat(c);
	Undock(c);	
	DockCont *tc = GetContainer(target);
	ASSERT(tc);	
	tc->AddDCs(c);
	DestroyContainer(&c);	
}


void DockBase::FloatContainer(DockCont &c, int except, Point p)
{
	if (c.IsFloating() && except < 0) return;
	if (p.IsNullInstance()) 
		p = DockOwner()->GetScreenRect().CenterPoint();
	Unfloat(c);
	Undock(c);	
	DockCont *cont = &c;
	if (except >= 0) {
		cont = CreateContainer();
		cont->AddDCs(c, except);
		cont->SetRect(c.GetScreenRect());		
	}	
	cont->StateFloating(*this);
	cont->Hide();
	cont->Open(DockOwner());
	Size best = Size(CtrlBestSize(false, c, false), CtrlBestSize(true, c, false));
	cont->SetRect(Rect(p, best));
	cont->Show();
	cont->SetFocus();	
}

void DockBase::FloatFromTab(DockCont &c, DockableCtrl &dc)
{
	Rect r = c.GetScreenRect();
	DockCont *nc = CreateContainer(dc);
	nc->SetRect(r);
	nc->StateDocked(*this);
	nc->MoveBegin();
}

void DockBase::AutoHideContainer(int align, DockCont &c, int except)
{
	Unfloat(c);
	Undock(c);		
	except = min(except, -1);
	for (int i = 0; i < except; i++)
		AutoHide(align, c.GetDC(0));	
	except += 1;
	while(except < c.GetDCCount())
		AutoHide(align, c.GetDC(except));	
}

/*
 * Docking Interface helpers
*/ 
DockableCtrl *DockBase::GetMouseDockTarget()
{
	Point mp = GetMousePos();
	for (int i = 0; i < 4; i++) {
		if (db_frame[i].IsShown()) {
			Rect r = db_dock[i].GetScreenRect();
			if (r.Contains(mp)) {
				Ctrl *c = &db_dock[i];
				mp -= r.TopLeft();
				c = c->ChildFromPoint(mp);
				DockCont *cont = dynamic_cast<DockCont *>(c);
				return cont ? &cont->GetCurrentDC() : NULL;
			}
		}
	}
	return NULL;
}

int DockBase::GetDockAlign(Ctrl &c) const
{
	for (Ctrl *p = c.GetParent(); p; p = p->GetParent())
		for (int i = 0; i < 4; i++)
			if (&db_dock[i] == p) return i;
	return DOCK_NONE;
}

int DockBase::GetDockAlign(const Point &p) const
{
	for (int i = 0; i < 4; i++) 
		if (db_dock[i].IsVisible() && db_dock[i].GetScreenRect().Contains(p)) 
			return i; 
	return DOCK_NONE;
}

int DockBase::CtrlBestSize(bool vert, const Ctrl &c, bool restrict) const
{
	int mn, mx, std;
	if (vert) {
		//sz = c.GetSize().cy;
		mn = c.GetMinSize().cy;
		mx = restrict ? min(c.GetMaxSize().cy, DockOwner()->GetSize().cy/2) : c.GetMaxSize().cy;
		std = c.GetStdSize().cy;
	}
	else {
		//sz = c.GetSize().cx;
		mn = c.GetMinSize().cx;
		mx = restrict ? min(c.GetMaxSize().cy, DockOwner()->GetSize().cx/2) : c.GetMaxSize().cx;
		std = c.GetStdSize().cx;
	}
	//if (std == mn || sz >= mn && sz <= mx) std = sz;
	return minmax(std, mn, mx);
}

void DockBase::Highlight(int align, DockableCtrl &dc, DockableCtrl *target)
{
	HighlightCtrl &hl = Single<HighlightCtrl>();
	if (&hl == target) return;
	hl.SizeHint(dc.GetMinSize(), dc.GetMaxSize(), dc.GetParent()->GetStdSize()).Show();
	if (target && align != DOCK_NONE) {
		hl.oldframesize = db_frame[GetDockAlign(*target)].GetSize();	
		Dock0(align, *target, hl);	
	}
	else if (target && IsTabbing()) {
		hl.oldframesize = db_frame[GetDockAlign(*target)].GetSize();
		hl.Title(dc.GetTitle()).Icon(dc.GetIcon());
		DockAsTab(*target, hl);	
	}
	else if (align != DOCK_NONE) {
		hl.oldframesize = db_frame[align].GetSize();
		Dock0(align, hl);
	}
	hl.Show();
}

void DockBase::StopHighlight()
{
	HighlightCtrl &hl = Single<HighlightCtrl>();
	if (hl.GetParent()) {
		int al = GetDockAlign(hl);
		if (al >= DOCK_LEFT && al <= DOCK_BOTTOM) {
			db_frame[al].SetSize(hl.oldframesize);
			Undock0(hl);
		}
	}
}

DockableCtrl * DockBase::FindDockTarget(Ctrl *dock_owner, int &al)
{
	ASSERT(dock_owner);
	Rect r;

	al = DOCK_NONE;

	DockableCtrl *target = GetMouseDockTarget();
	r = target ? target->GetParent()->GetScreenRect() : dock_owner->GetScreenView();
	
	if (target && IsSimple()) {
		int ral = GetDockAlign(*target);
		bool hv = IsTB(ral) ? !IsHorzVert() : IsHorzVert();
        al = GetBorderAlign(GetMousePos(), r, IsTabbing(), hv, !hv);
	}
	else {
		Point p = GetMousePos();
		if (r.Contains(p))
			al = GetBorderAlign(p, r, IsTabbing() || !target);		
	}
	
	if (!target && al != DOCK_NONE && IsDockVisible(al))
		al = DOCK_NONE;
	return target;	
}

int DockBase::GetBorderAlign(const Point &p, Rect r, bool tabbing, bool allow_lr, bool allow_tb) 
{
	hlbounds = r;
	Point pc = r.CenterPoint();
	if (tabbing) {
		Size sz = r.GetSize();
		sz.cx = allow_lr * min(DOCK_DND_BORDER, max(sz.cx / 2 - 10, 20));
		sz.cy = allow_tb * min(DOCK_DND_BORDER, max(sz.cy / 2 - 10, 20));		
		r.Deflate(sz);
	}
	else {
		r = Rect(pc, pc);		
		if (!allow_lr) { r.left = hlbounds.left; r.right = hlbounds.right; }
		if (!allow_tb) { r.top = hlbounds.top; r.bottom = hlbounds.bottom; }
	}
	pc = p - pc;
	if (pc.x < 0) pc.x = -pc.x;
	if (pc.y < 0) pc.y = -pc.y;
	bool v = pc.y > pc.x || (p.x >= r.left && p.x <= r.right);
	if (v && p.y < r.top) {
		hlbounds.bottom = r.top;
		return DOCK_TOP;
	}
	else if (v && pc.y > pc.x && p.y > r.bottom) {
		hlbounds.top = r.bottom;
		return DOCK_BOTTOM;
	}
	else if (p.x < r.left) {
		hlbounds.right = r.left;
		return DOCK_LEFT;
	}
	else if (p.x > r.right) {
		hlbounds.left = r.right;
		return DOCK_RIGHT;
	}
	hlbounds = r;
	return DockBase::DOCK_NONE;
}

void DockBase::HighlightCtrl::Paint(Draw &w)
{
	Rect r = GetSize();
	w.DrawRect(r, Blend(SLtBlue, SWhite, 90));
	DrawFrame(w, r, SLtBlue());
}

// Drag and Drop interface
void DockBase::ContainerDragStart(DockCont &dc)
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
			tl.y += pt.y - r.top + 4;
			move = true;
		}
		FloatContainer(dc, -1, move ? tl : Null);
		dc.StartMouseDrag(pt);
	}
}

void DockBase::ContainerDragMove(DockCont &dc)
{
	int al = DOCK_NONE;
	HighlightCtrl &hl = Single<HighlightCtrl>();
	Point p = GetMousePos();
	if (hl.GetParent()) {
		if (!hlbounds.Contains(p))
			StopHighlight();
		else
			return;
	}
	
	DockableCtrl *target = FindDockTarget(dc.GetOwner(), al);
	if (al != DOCK_NONE || target) {
		dc.SyncUserSize(true, true);
		Highlight(al, dc.GetCurrentDC(), target);
	}
	else 
		StopHighlight();
}

void DockBase::ContainerDragEnd(DockCont &c)
{
	if (!c.IsFloating()) return;
	int al = DOCK_NONE;
	StopHighlight();

	DockableCtrl *target = FindDockTarget(c.GetOwner(), al);
	if (target && al != DOCK_NONE)
		DockContainer(al, *target, c);	
	else if (target && IsTabbing())
		DockContainerAsTab(*target, c);	
	else if (al != DOCK_NONE)
		DockContainer(al, c);
	if (c.GetParent())
		c.SetFocus();
}

/*
 * Misc
*/ 
void DockBase::SetFrameSize(int align, int size)
{
	ALIGN_ASSERT(align);
	db_frame[align].SetSize(size);
}

void DockBase::DockLayout(Ctrl &_this, bool autohide)
{
	// Top, Bottom, Left, Right
	if (db_autohide[0].GetParent() != &_this && autohide) {;
		_this.AddFrame(*db_autohide[1]);
		_this.AddFrame(*db_autohide[3]);
		_this.AddFrame(*db_autohide[0]);
		_this.AddFrame(*db_autohide[2]);		
	}
	if (db_dock[0].GetParent()->GetParent() == &_this) return;
	for (int i = 0; i < 4; i++) db_frame[i].Hide();
	_this.AddFrame(db_frame[1]);
	_this.AddFrame(db_frame[3]);
	_this.AddFrame(db_frame[0]);
	_this.AddFrame(db_frame[2]);
}

DockBase::DockBase()
{
	simple = tabbing = smart_resize = true;
	for (int i = 0; i < 4; i++) {
		db_frame[i].Set(db_dock[i], 100, i).Hide();
		db_autohide[i].Set(i);
	}
	SetVertHorz();
}

