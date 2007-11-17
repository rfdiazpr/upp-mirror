#include "Docking.h"

#define ALIGN_ASSERT(al)	ASSERT(al >= 0 && al < 4)
#define DOCK_DND_BORDER		50

// DockBase
void DockBase::Undock(DockableWindow &wnd, bool layout)
{
	if (wnd.IsDockedAny()) {
		if (wnd.IsAutoHide()) {
			for (int i = 0; i < 4; i++) {
				int ix = wd_autohide[i].GetTabIx(wnd);
				if (ix >= 0) {
					wd_autohide[i].RemoveTab(ix);
					if (layout) wd_autohide[i].RefreshFrame();
					break;
				}
			}		
		}
		else {
			int al = GetDockAlign(wnd);
			if (IsTB(al))
				wnd.docksz.cy = wnd.GetScreenRect().Height();			
			else
				wnd.docksz.cx = wnd.GetScreenRect().Width();			
			Ctrl *p = wnd.GetParent();
			ASSERT(p);
			if (!p) return; // Shouldn't happen in normal execution. When debugging the highlight control may remain docked
			if (wnd.IsTabbed()) {
				DockableTabWindow *q = dynamic_cast<DockableTabWindow *>(p);
				ASSERT(q);
				wnd.Remove();
				if (q->GetCount() == 1 && layout) {
					p = q->GetParent();
					DockableWindow *c = dynamic_cast<DockableWindow *>(q->GetSelectedCtrl());
					ASSERT(c);
					c->Remove();
					if (q->IsDocked()) {
						SplitterTree *sp = GetDockTree(q);
						ASSERT(sp);
						sp->Swap(*q, *c);
						c->StateDocked(*this);
					}
					else {
						c->StateNotDocked();
						Float(*c);
						c->SetRect(q->GetRect());							
					}	
					q->StateNotDocked();
					DestroyTabs(q);
				}
			}
			else
				wnd.Remove();	
			if (layout && p) {	
				p->RefreshLayout();
				for (int ix = 0; ix < 4; ix++) {
					if (!wd_dock[ix].GetFirstChild() && wd_frame[ix].IsShown())
						wd_frame[ix].Hide();
				}
			}
		}
		wnd.StateNotDocked();
	}
}

void DockBase::Unfloat(DockableWindow &wnd)
{
	if (wnd.IsFloating()) {
		if (wnd.IsOpen()) {
			Rect r = wnd.GetScreenView();
			wnd.floatpt = r.TopLeft();
			wnd.floatsz = r.GetSize();
			wnd.Close();
		}
	}
}

SplitterTree * DockBase::GetDockTree(Ctrl *c)
{
	int i = GetDockAlign(*c);
	if (i != DOCK_NONE) return &wd_dock[i];
	return NULL;
}

void DockBase::Dock(int align, DockableWindow &wnd)
{
	ALIGN_ASSERT(align);
	if (!CanOwn(wnd)) return;
	Undock(wnd);
	Unfloat(wnd);
	bool show = !wd_dock[align].GetFirstChild();
	wnd.StateDocked(*this);
	
	Size sz = wnd.GetDockSize();
	if (IsTB(align)) {
		if (IsNull(sz.cy))
			sz.cy = min(wnd.GetFloatSize().cy, wd_dock[0].GetParent()->GetParent()->GetScreenRect().Height()/2); // No GetParent for SplitterFrame	
		if (!IsNull(sz.cy) && (sz.cy > wd_frame[align].GetSize() || !wd_dock[align].GetFirstChild() && sz.cy < wd_frame[align].GetSize()))
			wd_frame[align].SetSize(sz.cy);
	}
	else {
		if (IsNull(sz.cx))
			sz.cx = min(wnd.GetFloatSize().cx, wd_dock[0].GetParent()->GetParent()->GetScreenRect().Width()/2);	// No GetParent for SplitterFrame
		
		if (!IsNull(sz.cx) && (sz.cx > wd_frame[align].GetSize() || !wd_dock[align].GetFirstChild() && sz.cx < wd_frame[align].GetSize()))
			wd_frame[align].SetSize(sz.cx);
	}
	wnd.Show();
	wd_dock[align].AddRoot(wnd, false);
	if (show) wd_frame[align].Show(); 
	wnd.SetFocus();
}

void DockBase::Dock(int align, DockableWindow &target, DockableWindow &wnd)
{
	ALIGN_ASSERT(align);
	if (!target.IsDocked() || !CanOwn(wnd)) return;
	Undock(wnd);
	Unfloat(wnd);

	Ctrl *ctrl = &target;
	SplitterTree *tree = GetDockTree(ctrl);
	if (!tree) {
		ctrl = target.GetParent();
		ASSERT(ctrl);
		tree = GetDockTree(ctrl);
	}
	ASSERT(tree);
	wnd.StateDocked(*this);	
	wnd.Show();
	tree->AddSiblingUnder(IsTB(align), IsTopLeft(align), *ctrl, wnd);	
	wnd.SetFocus();	
}

void DockBase::DockAsTab(DockableWindow &target, DockableWindow &wnd)
{
	if (!target.IsDockedAny() || !CanOwn(target) || !CanOwn(wnd)) return;	
	
	Undock(wnd);
	Unfloat(wnd);
	
	ASSERT(target.GetParent());
	
	DockableTabWindow *q = dynamic_cast<DockableTabWindow *>(&target);
	if (!q) q = dynamic_cast<DockableTabWindow *>(target.GetParent());
	if (!q) {
		SplitterTree *tree = GetDockTree(&target);
		ASSERT(tree);
		q = &tabs.Create<DockableTabWindow>();
		tree->Swap(target, *q);		
		*q << target;
		target.StateTabbed(*this);
		q->StateDocked(*this);
	}
	DockableTabWindow *w = dynamic_cast<DockableTabWindow *>(&wnd);
	if (w) {
		q->AddTabsFrom(*w);
		PostCallback(callback1(this, &DockBase::DestroyTabs, w));
	}
	else {
		*q << wnd;
		wnd.StateTabbed(*this);
	}
}

void DockBase::Float(DockableWindow &wnd, bool reposition)
{
	if (!CanOwn(wnd) || (wnd.IsFloating() && wnd.IsOpen())) return;
	Rect r = Null;
	if (wnd.IsDockedAny() && !wnd.IsAutoHide()) {
		r = wnd.GetScreenView();
		if (!wnd.floatsz.IsNullInstance()) {
			if (reposition && !wnd.floatpt.IsNullInstance())
				r = Rect(wnd.floatpt, wnd.floatsz);
			else
				r.SetSize(wnd.floatsz); 
		}
	}
	Undock(wnd);	
	wnd.Hide();
	wnd.StateFloating(*this);
	ASSERT(wd_dock[0].GetParent());
	wnd.Open(wd_dock[0].GetParent());
	if (!r.IsNullInstance()) {
		wnd.floatpt = r.TopLeft();
		wnd.floatsz = r.GetSize();
		wnd.SetRect(r);	
	}
	wnd.Show();
	wnd.SetFocus();
}

void DockBase::AutoHide(int align, DockableWindow &wnd)
{
	ALIGN_ASSERT(align);
	if (!CanOwn(wnd)) return;
	wnd.Show();
	Undock(wnd);
	Unfloat(wnd);
	wnd.StateAutoHide(*this);
	wd_autohide[align].AddTab(wnd);	
}

void DockBase::Hide(DockableWindow &wnd)
{
	if (wnd.base != this) return;
	Undock(wnd);
	Unfloat(wnd);
}

DockableWindow & DockBase::Attach(DockableWindow &wnd)
{
	if (wnd.base == this) return wnd;
	wnd.base = this;
	wnds.Add(&wnd);
	return wnd;
}

void DockBase::Detach(DockableWindow &wnd)
{
	Unfloat(wnd);
	Undock(wnd, !wnd.isdestroying);
	for (int i = 0; i < wnds.GetCount(); i++)
		if (wnds[i] == &wnd)
			wnds.Remove(i);
	wnd.base = NULL;
}

void DockBase::SetDockSize(int align, int size)
{
	ALIGN_ASSERT(align);
	wd_frame[align].SetSize(size);
}

DockableWindow *DockBase::GetMouseDockTarget()
{
	Point mp = GetMousePos();
	for (int i = 0; i < 4; i++) {
		if (wd_frame[i].IsShown()) {
			Rect r = wd_dock[i].GetScreenRect();
			if (r.Contains(mp)) {
				Ctrl *c = &wd_dock[i];
				mp -= r.TopLeft();
				c = c->ChildFromPoint(mp);
				//DockableTabWindow *q = dynamic_cast<DockableTabWindow *>(c);
				//if (q) c = q->GetSelectedCtrl();
				DockableWindow *d = dynamic_cast<DockableWindow *>(c);
				return d;
			}
		}
	}
	return NULL;
}

int DockBase::GetDockAlign(Ctrl &c) const
{
	for (Ctrl *p = c.GetParent(); p; p = p->GetParent())
		for (int i = 0; i < 4; i++)
			if (&wd_dock[i] == p) return i;
	return DOCK_NONE;
}

int DockBase::GetDockAlign(const Point &p) const
{
	for (int i = 0; i < 4; i++) 
		if (wd_dock[i].IsVisible() && wd_dock[i].GetScreenRect().Contains(p)) 
			return i; 
	return DOCK_NONE;
}

void DockBase::ShowWindowStatus()
{
	TopWindow wnd;
	ArrayCtrl list;
	
	list.AddColumn("Window");
	list.AddColumn("Position");
	list.AddColumn("");
	list.ColumnWidths("91 157 40").SetLineCy(list.GetLineCy()+2);
	list.SetCount(wnds.GetCount());
	list.NoCursor(true).MultiSelect(false);
	for (int i = 0; i < wnds.GetCount(); i++) {
		DockableWindow *wnd = wnds[i];
		StaticText *text = new StaticText();
		Button *btn = new Button();
		text->SetText(GetWindowStatus(wnd));
		btn->SetImage(CtrlsImg::DA()) <<= THISBACK2(OnWindowStatusButton, text, wnd);
		list.Set(i, 0, wnd->GetTitle());
		list.SetCtrl(i, 1, text);
		list.SetCtrl(i, 2, btn);
	}
	wnd.Add(list.SizePos());
	wnd.SetRect(Rect(0, 0, 250, 200));
	wnd.CenterScreen();
	wnd.Execute();
}

String DockBase::GetWindowStatus(DockableWindow *wnd)
{
	if (wnd->IsFloating())
		return "Floating";
	else if (wnd->IsAutoHide())
		return "AutoHide";
	else if (wnd->IsHidden())
		return "Hidden";
	String s = wnd->IsTabbed() ? "Tabbed" : "Docked";
	int al = GetDockAlign(*wnd);
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

void DockBase::OnWindowStatusButton(StaticText *text, DockableWindow *wnd)
{
	wnd->WindowMenu();
	text->SetText(GetWindowStatus(wnd));
}

DockBase::DockBase()
{
	for (int i = 0; i < 4; i++) {
		wd_frame[i].Set(wd_dock[i], 100, i).Hide();
		wd_autohide[i].Set(i);
		(i & 1) ? wd_dock[i].RootVert() : wd_dock[i].RootHorz();
	}
}

// Drag and Drop interface
void DnDDockBase::AttachedWindowDragStart(DockableWindow &wnd)
{
	Float(wnd, false);
	// Reposition if not under the mouse
	Rect r = wnd.GetRect();
	Point pt = GetMousePos();
	bool move = false;
	if (r.left > pt.x || r.right < pt.x) {
		r.OffsetHorz(pt.x - r.left - r.Width()/2);
		move = true;
	}
	if (r.top < pt.y) {
		r.OffsetVert(pt.y - r.top + 4);
		move = true;
	}
	wnd.SetRect(r);	
	StartWindowDrag(pt, wnd);
}
#if	defined(PLATFORM_WIN32)	
void DnDDockBase::StartWindowDrag(const Point &p, DockableWindow &wnd)
{
	SendMessage(wnd.GetHWND(), WM_NCLBUTTONDOWN, 2, MAKELONG(p.x, p.y));
}
#elif defined(PLATFORM_X11)
void DnDDockBase::StartWindowDrag(const Point &p, DockableWindow &wnd)
{
	Atom xwndDrag = XAtom("_NET_WM_MOVERESIZE");
	XEvent e;
	Zero(e);
	e.xclient.type = ClientMessage;
	e.xclient.message_type = xwndDrag;
	e.xclient.window = wnd.GetWindow();
	e.xclient.format = 32;
	e.xclient.display = Xdisplay;
	e.xclient.send_event = XTrue;
	e.xclient.data.l[0] = p.x;
	e.xclient.data.l[1] = p.y;
	e.xclient.data.l[2] = 8;
	e.xclient.data.l[3] = 1;
	e.xclient.data.l[4] = 0;	
	
	XUngrabPointer( Xdisplay, CurrentTime );
	XSendEvent(Xdisplay, RootWindow(Xdisplay, Xscreenno), XFalse, SubstructureNotifyMask, &e);
	XFlush(Xdisplay);	
}
#endif

void DnDDockBase::AttachedWindowDragMove(DockableWindow &wnd)
{
	int al = DOCK_NONE;
	HighlightCtrl &hl = Single<HighlightCtrl>();
	Point p = GetMousePos();
	if (hl.GetDockBase() == this) {
		if (!bounds.Contains(p))
			StopHighlightTarget();
		else
			return;
	}
	
	DockableWindow *target = FindDockTarget(wnd, al);
	if (al != DOCK_NONE || target) 
		HighlightTarget(al, wnd, target);
	else 
		StopHighlightTarget();
}

void DnDDockBase::AttachedWindowDragEnd(DockableWindow &wnd)
{
	if (!wnd.IsFloating()) return;
	int al = DOCK_NONE;
	StopHighlightTarget();

	DockableWindow *target = FindDockTarget(wnd, al);
	if (target && al != DOCK_NONE)
		Dock(al, *target, wnd);	
	else if (target && IsTabbing())
		DockAsTab(*target, wnd);	
	else if (al != DOCK_NONE)
		Dock(al, wnd);
}

void DnDDockBase::HighlightTarget(int align, DockableWindow &wnd, DockableWindow *target)
{
	HighlightCtrl &hl = Single<HighlightCtrl>();
	if (&hl == target) return;
	if (hl.GetDockBase() && hl.GetDockBase() != this)
		hl.GetDockBase()->Detach(hl);
	hl.FloatSize(wnd.GetFloatSize()).DockSize(wnd.GetDockSize());
	if (target && align != DOCK_NONE)
		Dock(align, *target, hl);	
	else if (target && IsTabbing()) {
		hl.Title(wnd.GetTitle());
		DockAsTab(*target, hl);	
	}
	else if (align != DOCK_NONE)
		Dock(align, hl);
}

void DnDDockBase::StopHighlightTarget()
{
	HighlightCtrl &hl = Single<HighlightCtrl>();
	if (hl.GetDockBase())
		hl.GetDockBase()->Detach(hl);
}

DockableWindow * DnDDockBase::FindDockTarget(DockableWindow &wnd, int &al)
{
	Ctrl *owner = wnd.GetOwner();
	ASSERT(owner);
	Rect r;

	al = DOCK_NONE;

	DockableWindow *target = GetMouseDockTarget();
	r = target ? target->GetScreenRect() : owner->GetScreenView();
	
	if (target && IsSimple()) {
		int ral = GetDockAlign(*target);
		bool hv = IsTB(ral) ? IsHorzVert() : !IsHorzVert();
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

int DnDDockBase::GetBorderAlign(const Point &p, Rect r, bool tabbing, bool allow_lr, bool allow_tb) 
{
	bounds = r;
	Point pc = r.CenterPoint();
	if (tabbing) {
		Size sz = r.GetSize();
		sz.cx = allow_lr * min(DOCK_DND_BORDER, max(sz.cx / 2 - 10, 20));
		sz.cy = allow_tb * min(DOCK_DND_BORDER, max(sz.cy / 2 - 10, 20));		
		r.Deflate(sz);
	}
	else {
		r = Rect(pc, pc);		
		if (!allow_lr) { r.left = bounds.left; r.right = bounds.right; }
		if (!allow_tb) { r.top = bounds.top; r.bottom = bounds.bottom; }
	}
	pc = p - pc;
	if (pc.x < 0) pc.x = -pc.x;
	if (pc.y < 0) pc.y = -pc.y;
	bool v = pc.y > pc.x || (p.x >= r.left && p.x <= r.right);
	if (v && p.y < r.top) {
		bounds.bottom = r.top;
		return DOCK_TOP;
	}
	else if (v && pc.y > pc.x && p.y > r.bottom) {
		bounds.top = r.bottom;
		return DOCK_BOTTOM;
	}
	else if (p.x < r.left) {
		bounds.right = r.left;
		return DOCK_LEFT;
	}
	else if (p.x > r.right) {
		bounds.left = r.right;
		return DOCK_RIGHT;
	}
	bounds = r;
	return DockBase::DOCK_NONE;
}

void DnDDockBase::HighlightCtrl::Paint(Draw &w)
{
	Rect r = GetSize();
	w.DrawRect(r, Blend(SLtBlue, SWhite, 90));
	DrawFrame(w, r, SLtBlue());
}
