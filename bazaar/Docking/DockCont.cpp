#include "DockCont.h"
#include "Docking.h"

// ImgButton
void ImgButton::Paint(Draw &w)
{
	Size sz = GetSize();
	if (look)
		ChPaint(w, sz, look[Pusher::GetVisualState()]);
	int dx = IsPush() * -1;
	int dy = IsPush();
	if (!img.IsEmpty()) {
		Size isz = img.GetSize();
		w.DrawImage((sz.cx - isz.cx) / 2 + dx, (sz.cy - isz.cy) / 2 + dy, img);	
	}
}

// DockCont (Dockable Container)
#if	defined(PLATFORM_WIN32)
LRESULT DockCont::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCRBUTTONDOWN) {
		TitleContext();
		return 1L;
	}
	if (message == WM_NCLBUTTONDOWN && IsFloating() && base)
		dragging = 1;
	if (message == WM_NCLBUTTONUP)
		dragging = 0;
	if (message == WM_MOVE && IsFloating() && base && GetMouseLeft() && dragging) {
		if (dragging == 1)	{
			MoveBegin();
			dragging++;
		}
		Moving();
		dragging = true;
	}
	else if (message == WM_EXITSIZEMOVE && IsFloating() && base && !GetMouseLeft() && dragging) {
		MoveEnd();
		dragging = false;
	}
		
	return TopWindow::WindowProc(message, wParam, lParam);
}

void DockCont::StartMouseDrag(const Point &p)
{
	SendMessage(GetHWND(), WM_NCLBUTTONDOWN, 2, MAKELONG(p.x, p.y));	
}
#elif defined(PLATFORM_X11)
void DockCont::EventProc(XWindow& w, XEvent *event)
{
	if (IsOpen()) {
		switch(event->type) {
		case ConfigureNotify:{
			XConfigureEvent& e = event->xconfigure;
			if (Point(e.x, e.y) != GetScreenRect().TopLeft()) {
				if (!dragging)
					MoveBegin();
				Moving();				
				SetFocus();
				dragging = true;
			}
			}
			break;
		case FocusIn:
			XFocusChangeEvent &e = event->xfocus;
			if (e.mode == NotifyUngrab) {
				dragging = false;
				MoveEnd();
//				SetFocus();
				return;
			}
			break;
		}
	}
	TopWindow::EventProc(w, event);	
}

void DockCont::StartMouseDrag(const Point &p)
{
	Atom xwndDrag = XAtom("_NET_WM_MOVERESIZE");
	XEvent e;
	Zero(e);
	e.xclient.type = ClientMessage;
	e.xclient.message_type = xwndDrag;
	e.xclient.window = GetWindow();
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

void DockCont::Paint(Draw &w)
{
	TopWindow::Paint(w);
	if (!IsFloating() && !IsTabbed()) {
		DockableCtrl &dc = GetCurrent();
		const DockableCtrl::Style &s = *style;
		//bool focus = HasFocusDeep() ? 1 : 0;
		Rect r = GetView();
		const Rect &m = s.handle_margins;
		Point p;
		
		if (s.frame) {
			s.frame->FramePaint(w, r);
			s.frame->FrameLayout(r);
		}
		
		int hsz = GetHandleSize(s);
		if (s.handle_vert) {
			r.right = r.left + hsz;	
			p = Point(r.left-1 + m.left, r.bottom - m.bottom);
		}
		else {
			r.bottom = r.top + hsz;
			p = Point(r.left + m.left, r.top + m.top);
		}
		ChPaint(w, r, s.handle[focus]);
		
		Image img = dc.GetIcon();
		if (!img.IsEmpty()) {
			int isz = hsz;
			if (s.handle_vert) {
				isz -= (m.left + m.right);
				p.y -= hsz;
				ChPaint(w, max(p.x+m.left, r.left), p.y, isz, isz, img);
				p.y -= 2;
			}
			else {
				isz -= (m.top + m.bottom);
				ChPaint(w, p.x, max(p.y, r.top), isz, isz, img);
				p.x += isz + 2;
			}
		}
		if (!s.title_font.IsNull()) {
			if (s.handle_vert)
				r.top = GetClipWidth(r).y + m.top;
			else
			 	r.right = GetClipWidth(r).x - m.right;
			w.Clip(r);
			WString text = IsNull(dc.GetGroup()) ? dc.GetTitle() : (WString)Format("%s (%s)", dc.GetTitle(), dc.GetGroup());
			w.DrawText(p.x, p.y, s.handle_vert ? 900 : 0, text, s.title_font, s.title_ink[focus]);
			w.End();
		}
	}
}

Point DockCont::GetClipWidth(const Rect &r) const
{
	return (windowpos.GetParent() && windowpos.IsShown()) ? windowpos.GetRect().BottomLeft() :
		   (autohide.GetParent() && autohide.IsShown())   ? autohide.GetRect().BottomLeft() : 
		   (close.GetParent() && close.IsShown())   	  ? close.GetRect().BottomLeft() : 
		   r.TopRight();
}

void DockCont::LeftDrag(Point p, dword keyflags)
{
	Rect r = GetSize();
	style->frame->FrameLayout(r);
	if (r.Contains(p))
		MoveBegin();
}

void DockCont::ChildRemoved(Ctrl *child)
{
	if (!tabbar.GetCount()) return;
	Ctrl *c = dynamic_cast<DockableCtrl *>(child);
	if (!c) c = dynamic_cast<DockCont *>(child);
	if (c)
		for (int i = 0; i < GetCount(); i++)
			if (c == GetCtrl(i)) {
				tabbar.Close(i);
				if (tabbar.GetCount())
					TabSelected();
				break;				
			}
	if (!tabbar.GetCount())
		base->CloseContainer(*this);
}

void DockCont::ChildAdded(Ctrl *child)
{
	if (child->GetParent() != this) 
		return;
	else if (DockableCtrl *dc = dynamic_cast<DockableCtrl *>(child)) {
		Value v = ValueCast(dc);
		tabbar.Insert(0, v, dc->GetGroup(), true);
		SyncSize(*dc, dc->GetStyle());
	}	
	else if (DockCont *dc = dynamic_cast<DockCont *>(child)) {
		Value v = ValueCast(dc);
		tabbar.Insert(0, v, Null, true);
		SyncSize(*dc, dc->GetCurrent().GetStyle());
	}	
	else 
		return;	
	TabSelected();	
	if (GetCount() >= 2) RefreshLayout();
}

void 	DockCont::MoveBegin()		{ base->ContainerDragStart(*this); }
void 	DockCont::Moving()			{ base->ContainerDragMove(*this); }
void 	DockCont::MoveEnd()			{ base->ContainerDragEnd(*this); }	

void DockCont::TabSelected() 
{
	int ix = tabbar.GetCursor();
	if (ix >= 0) {
		DockableCtrl &dc = Get(ix);
		Ctrl *ctrl = GetCtrl(ix);
		if (ctrl != &(base->GetHighlightCtrl())) {
			Ctrl *first = &tabbar;
			for (Ctrl *c = first->GetNext(); c; c = c->GetNext())
				if (c != ctrl) c->Hide();
		}
		style = &dc.GetStyle();
		Icon(dc.GetIcon()).Title(dc.GetTitle());
		SyncSize(*ctrl, *style);
		SyncButtons(!IsTabbed() && !IsFloating());
		ctrl->Show();
		if (IsTabbed()) {
			DockCont *c = static_cast<DockCont *>(GetParent());
			c->tabbar.SyncRepos();
			c->TabSelected();
			c->RefreshFrame();
		}
		else
			Refresh();
	}
}

void DockCont::TabDragged(int ix) 
{
	if (ix >= 0) {
		// Special code needed
		Value v = tabbar.Get(ix);
		if (IsDockCont(v)) {
			DockCont *c = ContCast(v);
			c->Remove();
			base->FloatFromTab(*this, *c);
		}
		else {
			DockableCtrl *c = DockCast(v);
			c->Remove();
			base->FloatFromTab(*this, *c);
		}
		if (tabbar.IsAutoHide()) 
			RefreshLayout();		
	}
}

void DockCont::TabContext(int ix)
{
	MenuBar bar;
	WindowMenu(bar, ix, true);
	tabbar.ContextMenu(bar);
	bar.Execute();
}

int DockCont::DoPrompt(const char *title, const char *text) const
{
	return GetCount() > 1 ? 
		Prompt(	Format("%s Tabs", title), Image(), 
				Format("There are multiple tabs open, which would you like to %s?", text), 
				"All Tabs", "All Other Tabs", "Only This Tab") 
		: 1;	
}

void DockCont::WindowMenu(Bar &bar, int ix, bool noclose)
{
	GetCurrent().WindowMenu(bar);
	bar.Add(true, "Dock", (IsDockedAny() && !IsAutoHide()) ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK1(DockMenu, ix));
	bar.Add(IsDockedAny(), "Float", 	IsFloating() ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK1(TabsFloat, ix));
	if (base->IsAutoHide())
		bar.Add(true, "Auto-Hide", CtrlImg::MenuCheck0, THISBACK1(AutoHideMenu, ix));
	bar.Separator();
	if (!noclose) bar.Add(true, "Close", CtrlImg::Remove(), THISBACK1(TabsClose, ix));	
}

void DockCont::DockMenu(Bar &bar, int ix)
{
	int align = GetDockWindow()->GetDockAlign(*this);
	DockableCtrl &dc = GetCurrent();
	int t = DockWindow::DOCK_LEFT;
	DockMenuItem(bar, ix, t++, align, "Left");
	DockMenuItem(bar, ix, t++, align, "Top");
	DockMenuItem(bar, ix, t++, align, "Right");
	DockMenuItem(bar, ix, t++, align, "Bottom");
}

void DockCont::DockMenuItem(Bar &bar, int ix, int al, int align, const char *str)
{
	bar.Add(IsDockAllowed(al, ix), str, align == al ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK2(TabsDock, al, ix));		
}

void DockCont::AutoHideMenu(Bar &bar, int ix)
{
	bar.Add(true, "Left", 	THISBACK2(TabsAutoHide, (int)DockWindow::DOCK_LEFT, ix));
	bar.Add(true, "Top", 	THISBACK2(TabsAutoHide, (int)DockWindow::DOCK_TOP, ix));
	bar.Add(true, "Right", 	THISBACK2(TabsAutoHide, (int)DockWindow::DOCK_RIGHT, ix));
	bar.Add(true, "Bottom", THISBACK2(TabsAutoHide, (int)DockWindow::DOCK_BOTTOM, ix));	
}

void DockCont::TabsClose(int ix)
{
	int resp = -1; // Selected
	if (ix < 0) {
		ix = tabbar.GetCursor();
		resp = DoPrompt("Close", "close");
	}

	if (resp > 0) // ALL
		base->CloseContainer(*this);
	else if (resp == 0) { // All Other
		Ctrl *tab = GetCtrl(ix);
		tabbar.Clear();
		Ctrl *first = &tabbar;
		for (Ctrl *c = first->GetNext(); c; c = c->GetNext())
			if (c != tab) c->Remove();
	}		
	else // Selected
		GetCtrl(ix)->Remove();	
}

void DockCont::TabsDock(int align, int ix)
{
	int resp = -1; // Selected
	if (ix < 0) {
		ix = tabbar.GetCursor();
		resp = DoPrompt("Dock", "dock");
	}
	if (resp > 0) ix = -1; // ALL
	if (resp >= 0) // All/All Other
		base->DockContainer(align, *this, ix);
	else // Selected
		IsDockCont(tabbar.Get(ix)) ? 
			base->DockContainer(align, *ContCast(tabbar.Get(ix)), -1) : 
			base->Dock(align, Get(ix));	
}

void DockCont::TabsAutoHide(int align, int ix)
{
	int resp = -1; // Selected
	if (ix < 0) {
		ix = tabbar.GetCursor();
		resp = DoPrompt("Hide", "hide");
	}
	if (resp > 0) ix = -1; // ALL
	if (resp >= 0) // All/All Other
		base->AutoHideContainer(align, *this, ix);
	else // Selected
		IsDockCont(tabbar.Get(ix)) ? 
			base->AutoHideContainer(align, *ContCast(tabbar.Get(ix)), -1) :
			base->AutoHide(align, Get(ix));		
}

void DockCont::TabsAutoHide0()
{
	ASSERT(GetDockWindow()); 
	IsAutoHide() ? TabsFloat(0) : TabsAutoHide(GetDockWindow()->GetDockAlign(GetCurrent()), IsAutoHide() ? 1 : -1);		 
}

void DockCont::TabsFloat(int ix)
{
	int resp = -1; // Selected
	if (ix < 0) {
		ix = tabbar.GetCursor();
		resp = DoPrompt("Float", "float");
	}
	if (resp > 0) ix = -1; // ALL
	if (resp >= 0) // All/All Other
		base->FloatContainer(*this, ix);
	else // Selected
		IsDockCont(tabbar.Get(ix)) ? 
			base->FloatContainer(*ContCast(tabbar.Get(ix)), -1) :
			base->Float(Get(ix));			
}

void DockCont::AddFrom(DockCont &cont, int except)
{
	except = except < 0 ? cont.GetCount() : except;
	for (int i = cont.GetCount() - 1; i >= 0; i--)
		if (i != except) {
			Ctrl *c = cont.GetCtrl(i);
			c->Remove();
			Add(*c);				
		}
}

void DockCont::Clear()
{
	for (int i = 0; i < tabbar.GetCount(); i++)
		CtrlCast(tabbar.Get(i))->Close();
	tabbar.Clear();
}

int DockCont::GetHandleSize(const DockableCtrl::Style &s) const
{
	return max((s.title_font.IsNull() ? 0 : s.title_font.GetHeight()), s.btnsize)
			 + s.handle_margins.top + s.handle_margins.bottom; 
}

Rect DockCont::GetHandleRect(const DockableCtrl::Style &s) const
{
	Rect r = GetFrameRect(s);
	if (s.handle_vert)
		r.left += GetHandleSize(s);
	else
		r.top += GetHandleSize(s);
	return r;
}

Rect DockCont::GetFrameRect(const DockableCtrl::Style &s) const
{
	Rect r(0, 0, 0, 0);
	if (s.frame) {
		s.frame->FrameLayout(r);
		r.right = -r.right;
		r.bottom = -r.bottom;	
	}
	return r;
}

void DockCont::SyncSize(Ctrl &c, const DockableCtrl::Style &s)
{
	if(!IsFloating() && !IsTabbed()) {
		Rect r = GetHandleRect(s);
		c.HSizePos(r.left, r.right).VSizePos(r.top, r.bottom);	
	}
	else
		c.SizePos();
}

void DockCont::SyncCurrent()
{
	Value v = ~tabbar;
	Ctrl *c = NULL;
	DockableCtrl *dc = NULL;
	if (IsDockCont(v)) {
		DockCont *cont = ContCast(v);
		c = cont;
		dc = &cont->GetCurrent();
	}
	else {
		dc = DockCast(v);
		c = dc;
	}
	SyncSize(*c, dc->GetStyle());
}

void DockCont::StateDocked(DockWindow &dock)
{
	base = &dock;
	SyncButtons(true);
	dockstate = STATE_DOCKED;	
	if (tabbar.HasCursor()) SyncCurrent();
	Show(); 
}

void DockCont::StateFloating(DockWindow &dock)
{
	SyncButtons(false); 
	base = &dock; 
	dockstate = STATE_FLOATING;
	if (tabbar.HasCursor()) SyncCurrent();
	Show(); 
}

void DockCont::SyncButtons(bool show)
{
	if (show && base && style) {
		Ctrl::LogPos lp;
		const DockableCtrl::Style &s = *style;
		Rect r = GetFrameRect(s);
		int btnsize = GetHandleSize(s) - 3;
		
		Logc &inc = s.handle_vert ? lp.y : lp.x;
		lp.x = s.handle_vert ? Ctrl::Logc(ALIGN_LEFT, r.left+1, btnsize) : Ctrl::Logc(ALIGN_RIGHT, r.right+1, btnsize);
		lp.y = Ctrl::Logc(ALIGN_TOP, r.top+1, btnsize);		
		
		if (close.GetParent()) {
			close.SetLook(s.close).SetPos(lp).Show();
			inc.SetA(inc.GetA() + btnsize + 1);
		}
		bool ah = base->IsAutoHide();
		autohide.Show(ah);
		if (ah && autohide.GetParent()) {
			autohide.SetLook(s.autohide).SetPos(lp);
			inc.SetA(inc.GetA() + btnsize + 1);		
		}
		if (windowpos.GetParent())
			windowpos.SetLook(s.windowpos).SetPos(lp).Show();
	}
	else {
		close.Hide();
		autohide.Hide();
		windowpos.Hide();
	}
}

void DockCont::GroupRefresh()
{
	for (int i = 0; i < tabbar.GetCount(); i++)
		if (!IsDockCont(tabbar.Get(i)))
			tabbar.SetTabGroup(i, DockCast(tabbar.Get(i))->GetGroup());
	Refresh();
}

void DockCont::WindowButtons(bool menu, bool hide, bool _close)
{
	AddRemoveButton(windowpos, menu);
	AddRemoveButton(autohide, hide);
	AddRemoveButton(close, _close);
	SyncButtons(!IsTabbed() && !IsFloating());
}

void DockCont::AddRemoveButton(Ctrl &c, bool state)
{
	if (state && !c.GetParent()) 
		Add(c); 
	else if (!state) 
		c.Remove();	
}

void DockCont::Highlight()
{
	if (!style || (!IsOpen() && !IsPopUp() && !GetParent())) return;
	ViewDraw v(this); 
	Rect h = GetHandleRect(*style);
	Rect r = GetSize();
	r.left += h.left;
	r.top += h.top;
	r.right -= h.right;
	r.bottom -= h.bottom; 
	ChPaint(v, r, style->highlight);
}

void DockCont::AddContainerSize(Size &sz) const
{
	if (style) {
		int d = (tabbar.IsAutoHide() ? 0 : tabbar.GetFrameSize()) + ((!IsFloating() || dragging) ? GetHandleSize(*style) : 0);
		style->handle_vert ? sz.cx += d : sz.cy += d;
	}	
}

Size DockCont::GetMinSize() const
{ 
	Size sz = tabbar.GetCount() ? GetCurrent().GetMinSize() : Size(0, 0); 
	AddContainerSize(sz);
	return sz;
}

Size DockCont::GetMaxSize() const	
{ 
	Size sz = tabbar.GetCount() ? GetCurrent().GetMaxSize() : Size(0, 0);
	AddContainerSize(sz);
	return sz;
}

Size DockCont::GetStdSize() const
{
	Size sz = usersize;
	if (IsNull(sz.cx) || IsNull(sz.cy)) {
		Size std = GetCurrent().GetStdSize();
		if (IsNull(sz.cx)) sz.cx = std.cx;
		if (IsNull(sz.cy)) sz.cy = std.cy;
	}
	AddContainerSize(sz);
	return sz;
}

void DockCont::SyncUserSize(bool h, bool v)
{
	Size sz = GetSize();
	if (h) usersize.cx = sz.cx;
	if (v) usersize.cy = sz.cy;
}

int DockCont::GetDockAlign() const
{
	return base->GetDockAlign(*this); 	
}

bool DockCont::IsDockAllowed(int align, int dc_ix) const
{
	if (dc_ix >= 0) return IsDockAllowed0(align, tabbar.Get(dc_ix));
	else if (!base->IsDockAllowed(align)) return false;
	
	for (int i = 0; i < tabbar.GetCount(); i++)
		if (!IsDockAllowed0(align, tabbar.Get(i))) return false;
	return true;
}

bool DockCont::IsDockAllowed0(int align, const Value &v) const
{
	return IsDockCont(v) ? ContCast(v)->IsDockAllowed(align, -1) : base->IsDockAllowed(align, *DockCast(v));
}

DockableCtrl & DockCont::Get(int ix) const
{ 
	Value v = tabbar.Get(ix); 
	return IsDockCont(v) ? ContCast(v)->GetCurrent() : *DockCast(v); 
}

WString DockCont::GetTitle(bool force_count) const
{
	if ((IsTabbed() || force_count) && tabbar.GetCount() > 1) 
		return (WString)Format("%s (%d/%d)", GetCurrent().GetTitle(), tabbar.GetCursor()+1, tabbar.GetCount()); 
	return GetCurrent().GetTitle();	
}

DockCont::DockCont()
{
	dragging = false;
	dockstate = STATE_NONE;
	base = NULL;
	focus = false;	
	style = NULL;
	usersize.cx = usersize.cy = Null;
	BackPaint();
	
	ToolWindow().NoCenter().Sizeable(true).MaximizeBox(false).MinimizeBox(false);

	*this << close << autohide << windowpos;
	
	tabbar.AutoHideMin(1);
	tabbar.WhenCursor 		= THISBACK(TabSelected);
	tabbar.WhenDrag 		= THISBACK(TabDragged);
	tabbar.WhenContext 		= THISBACK(TabContext);
	tabbar.WhenClose 		= THISBACK(TabClosed);
	AddFrame(tabbar);
	tabbar.SetBottom();	

	close.Tip("Close") 					<<= THISBACK1(TabsClose, -1);
	autohide.Tip("Auto-Hide") 			<<= THISBACK (TabsAutoHide0);
	windowpos.Tip("Window Position") 	<<= THISBACK1(TabContext, -1);		
	WhenClose 							  = THISBACK1(TabsClose, -1);
}

void DockCont::Serialize(Stream& s)
{
	int ctrl = 'D';
	int cont = 'C';
	const Vector<DockableCtrl *> &dcs = base->GetDockableCtrls();
	
	if (s.IsStoring()) {		
		if (GetCount() == 1 && IsDockCont(tabbar.Get(0)))
			return ContCast(tabbar.Get(0))->Serialize(s);

		int cnt = GetCount();
		s / cont / cnt;
		for (int i = GetCount() - 1; i >= 0; i--) {
			if (IsDockCont(tabbar.Get(i)))
				ContCast(tabbar.Get(i))->Serialize(s);
			else {
				DockableCtrl *dc = DockCast(tabbar.Get(i));
				int ix = base->FindDocker(dc);
				s / ctrl / ix;					
			}									
		}
		int cursor = tabbar.GetCursor();
		int groupix = tabbar.GetGroup();
		s / cursor / groupix;
	}
	else {
		int cnt;
		int type;
		
		s / type / cnt;
		ASSERT(type == cont);
		for (int i = 0; i < cnt; i++) {
			int64 pos = s.GetPos();
			s / type;
			if (type == cont) {
				s.Seek(pos);
				DockCont *dc = base->CreateContainer();
				dc->Serialize(s);
				base->DockContainerAsTab(*this, *dc, true);
			}
			else if (type == ctrl) {
				int ix;
				s / ix;
				if (ix >= 0 && ix <= dcs.GetCount())
					Add(*dcs[ix]);
			}
			else
				ASSERT(false);								
		}
		int cursor, groupix;
		s / cursor / groupix;
		tabbar.SetGroup(groupix);
		tabbar.SetCursor(min(tabbar.GetCount()-1, cursor));
		TabSelected();
	}	
}

