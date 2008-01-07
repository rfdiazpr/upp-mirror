#include "DockCont.h"

#include <Docking/DockBase.h>

#if	defined(PLATFORM_WIN32)
bool DockCont::dragging = false;

LRESULT DockCont::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCRBUTTONDOWN) {
		TitleContext();
		return 1L;
	}
	if (message == WM_MOVE && IsFloating() && base && GetMouseLeft()) {
		if (!dragging)
			MoveBegin();
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
void DockCont::StartMouseDrag(const Point &p)
{
	ContainerBase::TitleDrag();
/*	This is the code for starting a window drag using a proper X11 window
	Kept here in case I ever find a way of using it
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
	XFlush(Xdisplay);	*/
}
#endif

void DockCont::Paint(Draw &w)
{
	ContainerBase::Paint(w);
	if (!IsFloating()) {
		DockableCtrl &dc = GetCurrentDC();
		const DockableCtrl::Style &s = dc.GetStyle();
		//bool focus = HasFocusDeep() ? 1 : 0;
		Rect r = GetView();
		Point p;
		
		if (s.frame) {
			s.frame->FramePaint(w, r);
			s.frame->FrameLayout(r);
		}
		
		int hsz = GetHandleSize(s);
		if (s.handle_vert) {
			r.right = r.left + hsz;	
			p = Point(r.left-1, r.bottom - 1);
		}
		else {
			r.bottom = r.top + hsz;
			p = Point(r.left+1, r.top);
		}
		ChPaint(w, r, s.handle[focus]);
		
		Image img = dc.GetIcon();
		if (!img.IsEmpty()) {
			int isz = hsz-2;
			if (s.handle_vert) {
				p.y -= hsz;
				ChPaint(w, max(p.x+2, r.left), p.y, isz, isz, img);
				//w.DrawImage(p.x, p.y, img);
				p.y--;
			}
			else {
				ChPaint(w, p.x, max(p.y, r.top), isz, isz, img);
				//w.DrawImage(p.x, p.y, img);
				p.x += hsz + 1;
			}
		}
		if (!s.title_font.IsNull())
			w.DrawText(p.x, p.y, s.handle_vert ? 900 : 0, dc.GetTitle(), s.title_font, s.title_ink[focus]);
	}
}

void DockCont::LeftDrag(Point p, dword keyflags)
{
	Rect r = GetRect();
	GetCurrentDC().GetStyle().frame->FrameLayout(r);
	if (r.Contains(p))
		MoveBegin();
}

void DockCont::ChildRemoved(Ctrl *child)
{
	if (tabbar.GetCount()) {
		for (int i = 0; i < dcs.GetCount(); i++) {
			if (child == dcs[i]) {
				tabbar.RemoveTab(i);
				dcs.Remove(i);
				return;
			}
		}
	}
}

void 	DockCont::MoveBegin()		{ base->ContainerDragStart(*this); }
void 	DockCont::Moving()			{ base->ContainerDragMove(*this); }
void 	DockCont::MoveEnd()			{ base->ContainerDragEnd(*this); }	

void DockCont::TabSelected(int ix) 
{
	if (ix >= 0) {
		for (int i = 0; i < dcs.GetCount(); i++) {
			Ctrl *q = dcs[i];
			q->Hide();
		}
		DockableCtrl &dc = *dcs[ix];
		Icon(dc.GetIcon()).Title(dc.GetTitle());
		SyncButtons(true);
		dc.Show();
	}
}

void DockCont::TabDragged(int ix) 
{
	if (ix >= 0) {
		// Special code needed
		DockableCtrl &dc = *dcs[ix];
		RemoveDC(ix);
		TabSelected(tabbar.GetSelected());
		if (tabbar.IsAutoHide()) RefreshLayout();
		base->FloatFromTab(*this, dc);
	}
}

void DockCont::TabContext(int ix)
{
	if (ix >= 0) WindowMenu(ix);
}

int DockCont::DoPrompt(const char *title, const char *text) const
{
	return GetDCCount() > 1 ? 
		Prompt(	Format("%s Tabs", title), Image(), 
				Format("There are multiple tabs open, which would you like to %s?", text), 
				"All Tabs", "All Other Tabs", "Only This Tab") 
		: -1;	
}

Vector<DockableCtrl *> DockCont::GetOtherDCs(int ix)
{
	Vector<DockableCtrl *> dcs;
	for (int i = 0; i < tabbar.GetCount(); i++)
		if (i != ix)
			dcs.Add(&GetDC(i));
	return dcs;
}

void DockCont::WindowMenu(int ix)
{
	MenuBar bar;
	GetCurrentDC().WindowMenu(bar);
	bar.Add(true, "Docked", (IsDockedAny() && !IsAutoHide()) ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK1(DockMenu, ix));
	bar.Add(IsDockedAny(), "Floating", 	IsFloating() ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK1(TabsFloat, ix));
	bar.Separator();
	bar.Add(true, "Hide", CtrlImg::Remove(), THISBACK1(TabsClose, ix));
	bar.Add(true, "AutoHide", CtrlImg::MenuCheck0, THISBACK1(AutoHideMenu, ix));
	bar.Execute();		
}

void DockCont::DockMenu(Bar &bar, int ix)
{
	int align = GetDockBase()->GetDockAlign(*this);
	bar.Add(true, "Left", 	align == DockBase::DOCK_LEFT ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK2(TabsDock, 0, ix));
	bar.Add(true, "Top", 	align == DockBase::DOCK_TOP ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK2(TabsDock, 1, ix));
	bar.Add(true, "Right", 	align == DockBase::DOCK_RIGHT ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK2(TabsDock, 2, ix));
	bar.Add(true, "Bottom", align == DockBase::DOCK_BOTTOM ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK2(TabsDock, 3, ix));	
}

void DockCont::AutoHideMenu(Bar &bar, int ix)
{
	bar.Add(true, "Left", 	THISBACK2(TabsAutoHide, (int)DockBase::DOCK_LEFT, ix));
	bar.Add(true, "Top", 	THISBACK2(TabsAutoHide, (int)DockBase::DOCK_TOP, ix));
	bar.Add(true, "Right", 	THISBACK2(TabsAutoHide, (int)DockBase::DOCK_RIGHT, ix));
	bar.Add(true, "Bottom", THISBACK2(TabsAutoHide, (int)DockBase::DOCK_BOTTOM, ix));	
}

void DockCont::TabsClose(int ix)
{
	int resp = -1; // Selected
	if (ix < 0) {
		ix = tabbar.GetSelected();
		resp = DoPrompt("Close", "close");
	}

	if (resp > 0) // ALL
		base->CloseContainer(*this);
	else if (resp == 0) { // All Other
		DockableCtrl *dc = dcs[ix];
		tabbar.Clear();
		for (int i = 0; i < dcs.GetCount(); i++)
			if (dcs[i] != dc)
				dcs[i]->Remove();
		dcs.Clear();
		dcs.Add(dc);
		tabbar.AddTab(dc->GetTitle());
	}		
	else // Selected
		base->Close(GetDC(ix));	
}

void DockCont::TabsDock(int align, int ix)
{
	int resp = -1; // Selected
	if (ix < 0) {
		ix = tabbar.GetSelected();
		resp = DoPrompt("Dock", "dock");
	}

	if (resp > 0) // ALL
		ix = -1;
	if (resp >= 0) // All/All Other
		base->DockContainer(align, *this, ix);
	else // Selected
		base->Dock(align, GetDC(ix));	
}

void DockCont::TabsAutoHide(int align, int ix)
{
	int resp = -1; // Selected
	if (ix < 0) {
		ix = tabbar.GetSelected();
		resp = DoPrompt("Hide", "hide");
	}

	if (resp > 0) // ALL
		ix = -1;
	if (resp >= 0) // All/All Other
		base->AutoHideContainer(align, *this, ix);
	else // Selected
		base->AutoHide(align, GetDC(ix));		
}

void DockCont::TabsAutoHide0()
{
	ASSERT(GetDockBase()); 
	if (IsDocked()) {
		TabsAutoHide(GetDockBase()->GetDockAlign(GetCurrentDC()), -1); 
	}
	else if (IsAutoHide()) {
		GetDockBase()->Float(GetCurrentDC());
	}
}

void DockCont::TabsFloat(int ix)
{
	int resp = -1; // Selected
	if (ix < 0) {
		ix = tabbar.GetSelected();
		resp = DoPrompt("Float", "float");
	}

	if (resp > 0) // ALL
		ix = -1;
	if (resp >= 0) // All/All Other
		base->FloatContainer(*this, ix);
	else // Selected
		base->Float(GetDC(ix));	
}

void DockCont::AddDC(DockableCtrl &dc)
{
	SyncSize(dc);
	Add(dc);
	tabbar.AddTab(dc.GetTitle());
	dcs.Add(&dc);
	int ix = tabbar.GetCount()-1;
	tabbar.SelectTab(ix);
	TabSelected(ix);
}

void DockCont::AddDCs(DockCont &cont, int except)
{
	except = min(except, -1);
	for (int i = 0; i < except; i++) {
		DockableCtrl &dc = cont.GetDC(0);
		dc.Remove();
		AddDC(dc);		
	}
	except += 1;
	while(except < cont.GetDCCount()) {
		DockableCtrl &dc = cont.GetDC(except);
		dc.Remove();
		AddDC(dc);		
	}
}

void DockCont::Clear()
{
	tabbar.Clear();
	for (int i = 0; i < dcs.GetCount(); i++)
		dcs[i]->Remove();
	dcs.Clear();
}

int DockCont::GetHandleSize(const DockableCtrl::Style &s)
{
	return max((s.title_font.IsNull() ? 0 : s.title_font.GetHeight()), s.btnsize) + 2;
}

Rect DockCont::GetHandleRect(const DockableCtrl::Style &s)
{
	Rect r = GetFrameRect(s);
	if (s.handle_vert)
		r.left += GetHandleSize(s);
	else
		r.top += GetHandleSize(s);
	return r;
}

Rect DockCont::GetFrameRect(const DockableCtrl::Style &s)
{
	Rect r(0, 0, 0, 0);
	if (s.frame) {
		s.frame->FrameLayout(r);
		r.right = -r.right;
		r.bottom = -r.bottom;	
	}
	return r;
}

void DockCont::SyncSize(DockableCtrl &dc)
{
	if(!IsFloating()) {
		Rect r = GetHandleRect(dc.GetStyle());
		dc.HSizePos(r.left, r.right).VSizePos(r.top, r.bottom);	
	}
	else
		dc.SizePos();
}

void DockCont::StateDocked(DockBase &dock)
{
	Show(); 
	SyncButtons(true); 
	base = &dock; 
	dockstate = STATE_DOCKED;	
	if (tabbar.GetSelected() >= 0)
		SyncSize(GetCurrentDC());
}

void DockCont::StateFloating(DockBase &dock)
{
	Show(); 
	SyncButtons(false); 
	base = &dock; 
	dockstate = STATE_FLOATING;
	if (tabbar.GetSelected() >= 0)
		SyncSize(GetCurrentDC());
}

void DockCont::SyncButtons(bool show)
{
	if (show) {
		const DockableCtrl::Style &s = GetCurrentDC().GetStyle();
		Rect r = GetFrameRect(s);
		if (s.handle_vert) {
			close.SetLook(s.close).TopPos(r.top+1, s.btnsize).LeftPos(r.left+1, s.btnsize);
			autohide.SetLook(s.autohide).TopPos(r.top+2+s.btnsize, s.btnsize).LeftPos(r.left+1, s.btnsize);
			windowpos.SetLook(s.windowpos).TopPos(r.top+3+s.btnsize*2, s.btnsize).LeftPos(r.left+1, s.btnsize);
		}
		else {
			close.SetLook(s.close).RightPos(r.right+1, s.btnsize).TopPos(r.top+1, s.btnsize);
			autohide.SetLook(s.autohide).RightPos(r.right+2+s.btnsize, s.btnsize).TopPos(r.top+1, s.btnsize);
			windowpos.SetLook(s.windowpos).RightPos(r.right+3+s.btnsize*2, s.btnsize).TopPos(r.top+1, s.btnsize);				
		}
		// Set correct look for the WindowFrame on X11
		// TODO: Find a better way of doing this.
		SetCloseLook(s.close);
	}
	close.Show(show);
	autohide.Show(show);
	windowpos.Show(show);
}

Size DockCont::GetStdSize() const
{
	Size sz = usersize;
	if (IsNull(sz.cx) || IsNull(sz.cy)) {
		Size std = GetCurrentDC().GetStdSize();
		if (IsNull(sz.cx)) sz.cx = std.cx;
		if (IsNull(sz.cy)) sz.cy = std.cy;
	}
	return sz;
}

void DockCont::SyncUserSize(bool h, bool v)
{
	Size sz = GetSize();
	if (h) usersize.cx = sz.cx;
	if (v) usersize.cy = sz.cy;
}

DockCont::DockCont()
{
	dockstate = STATE_NONE;
	base = NULL;
	focus = false;	
	usersize.cx = usersize.cy = Null;
	
	ToolWindow().Sizeable(true).MaximizeBox(false).MinimizeBox(false);
	
	tabbar.Bottom().SetFont(StdFont(13));
	tabbar.AutoHideMin(1);
	tabbar.WhenSelect = THISBACK(TabSelected);
	tabbar.WhenDrag = THISBACK(TabDragged);
	tabbar.WhenContext = THISBACK(TabContext);
		
	close.Tip("Close") <<= THISBACK(Hide);
	autohide.Tip("Auto-Hide") <<= THISBACK(TabsAutoHide0);
	windowpos.Tip("Window Position") <<= THISBACK1(WindowMenu, -1);		
	close <<= THISBACK1(TabsClose, -1);
	WhenClose = THISBACK1(TabsClose, -1);
	autohide <<= THISBACK(TabsAutoHide0);
	windowpos <<= THISBACK1(WindowMenu, -1);
	*this << close << autohide << windowpos;
	AddFrame(tabbar);
}

