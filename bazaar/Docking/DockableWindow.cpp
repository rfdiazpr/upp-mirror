#include "Docking.h"

#define IMAGECLASS DockImg
#define IMAGEFILE <Docking/Docking.iml>
#include <Draw/iml_source.h>

// ImgButton
void DockableWindow::ImgButton::Paint(Draw &w)
{
	Size sz = GetSize();
	Size isz = img.GetSize();

	if (look)
		ChPaint(w, sz, look[Pusher::GetVisualState()]);
	int dx = IsPush() * -1;
	int dy = IsPush();
	w.DrawImage((sz.cx - isz.cx) / 2 + dx, (sz.cy - isz.cy) / 2 + dy, img);	
}

// DockableWindow
bool 	DockableWindow::dragging = false;

#if	defined(PLATFORM_WIN32)
LRESULT DockableWindow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCRBUTTONDOWN) {
		WindowMenu();
		return 1L;
	}
	if (message == WM_MOVE && IsFloating() && base && GetMouseLeft()) {
		base->AttachedWindowDragMove(*this);
		dragging = true;
	}
	else if (message == WM_EXITSIZEMOVE && IsFloating() && base && !GetMouseLeft() && dragging) {
		base->AttachedWindowDragEnd(*this);
		dragging = false;
	}
		
	return TopWindow::WindowProc(message, wParam, lParam);
}
#elif defined(PLATFORM_X11)
void DockableWindow::EventProc(XWindow& w, XEvent *event)
{
/*	if (event->type == ConfigureNotify) {
		bool ml = GetMouseLeft();
		XConfigureEvent& e = event->xconfigure;
		Window w1 = e.event;
		Window w2 = e.window;
		Ctrl *c1 = Ctrl::CtrlFromWindow(e.event);
		Ctrl *c2 = Ctrl::CtrlFromWindow(e.window);
		if (!c1 && IsFloating() && base && ml) {
			base->AttachedWindowDragMove(*this);
			dragging = true;
		}
		else if (!c1 && IsFloating() && base && !ml && dragging) {
			base->AttachedWindowDragEnd(*this);
			dragging = false;				
		}			
	}*/
	TopWindow::EventProc(w, event);
	
	//_NET_WM_WINDOW_TYPE_DOCK
}
#endif

void DockableWindow::ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
{
	if (child == &titlebar && ((event & LEFT) || (event & RIGHT))) {
		SetFocus();
		if (event == LEFTDRAG && IsDockedAny()) {
			ASSERT(base);
			base->AttachedWindowDragStart(*this);
		}
	}
	else
		TopWindow::ChildMouseEvent(child, event, p, zdelta, keyflags);
}

Size DockableWindow::GetStdSize() const
{
	if (IsFloating() && !floatsz.IsNullInstance())
		return floatsz;
	else if (IsNull(docksz.cx) || IsNull(docksz.cy)) {
		Size msz = GetMinSize();
		return Size(IsNull(docksz.cx) ? msz.cx : docksz.cx, IsNull(docksz.cy) ? msz.cy : docksz.cx);
	}
	else
		return docksz;
}

void DockableWindow::CloseDocked()
{
	base->Hide(*this);
}

void DockableWindow::SetTitleBar()
{
	close.Tip("Close") <<= THISBACK(CloseDocked);
	autohide.Tip("Auto-Hide") <<= THISBACK(AutoHideToggle);
	windowpos.Tip("Window Position") <<= THISBACK(WindowMenu);
	titlebar << close << autohide << windowpos;
	titlebar.WhenContext = THISBACK(WindowMenu);
	AddFrame(titlebar);
}

void DockableWindow::UpdateFocus()
{
	titlebar.SetFocusLook(HasFocusDeep());
}

void DockableWindow::WindowMenu()
{
	if (!base) return;
	MenuBar bar;
	bar.Add(true, "Docked", (IsDockedAny() && !IsAutoHide()) ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK(DockMenu));
	bar.Add(IsDockedAny(), "Floating", 	IsFloating() ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK(Float0));
	bar.Separator();
	bar.Add(true, "Hide", CtrlImg::Remove(), THISBACK(Close));
	if (IsAutoHide())
		bar.Add(true, "AutoHide", CtrlImg::MenuCheck1, THISBACK(AutoHideToggle));
	else
		bar.Add(true, "AutoHide", CtrlImg::MenuCheck0, THISBACK(AutoHideMenu));
	bar.Execute();
}

void DockableWindow::DockMenu(Bar &bar)
{
	int align = base->GetDockAlign(*this);
	bar.Add(true, "Left", 	(align == DockBase::DOCK_LEFT) ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK1(Dock0, 0));
	bar.Add(true, "Top", 	(align == DockBase::DOCK_TOP) ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK1(Dock0, 1));
	bar.Add(true, "Right", 	(align == DockBase::DOCK_RIGHT) ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK1(Dock0, 2));
	bar.Add(true, "Bottom", (align == DockBase::DOCK_BOTTOM) ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK1(Dock0, 3));
}

void DockableWindow::AutoHideMenu(Bar &bar)
{
	bar.Add(true, "Left", 	THISBACK1(AutoHide1, (int)DockBase::DOCK_LEFT));
	bar.Add(true, "Top", 	THISBACK1(AutoHide1, (int)DockBase::DOCK_TOP));
	bar.Add(true, "Right", 	THISBACK1(AutoHide1, (int)DockBase::DOCK_RIGHT));
	bar.Add(true, "Bottom", THISBACK1(AutoHide1, (int)DockBase::DOCK_BOTTOM));
}

void DockableWindow::Float0()								
{ 
	base->Float(*this); 
}

void DockableWindow::Dock0(int align)						
{ 
	base->Dock(align, *this); 
}

void DockableWindow::AutoHide0()
{
	base->AutoHide(*this);
}

void DockableWindow::AutoHide1(int align)
{
	base->AutoHide(align, *this);
}

void DockableWindow::StateDocked(DockBase &dock)
{
	titlebar.Show();
	dock.Attach(*this);
	dock_state = STATE_DOCKED;
}

void DockableWindow::StateFloating(DockBase &dock)
{
	titlebar.Hide();
	dock.Attach(*this);
	dock_state = STATE_FLOATING;
}

void DockableWindow::DockTo0(int align, DockableWindow *target)
{
	ASSERT(base); 
	base->Dock(align, *target, *this);	
}

DockableWindow &DockableWindow::DockAsTab(DockableWindow &target)
{
	ASSERT(base); 
	base->DockAsTab(target, *this);	
	return *this;
}

DockableWindow & DockableWindow::FloatSize(Size sz)
{
	floatsz = sz; 
	if (IsFloating()) { 
		Rect r = GetRect(); 
		r.SetSize(sz); 
		SetRect(r); 
	}
	return *this;	
}

void DockableWindow::SyncButtonStyle()
{
	const Style &st = style ? *style : StyleDefault();
	close.SetLook(st.btnlook).SetImage(st.close).Refresh();
	windowpos.SetLook(st.btnlook).SetImage(st.windowpos).Refresh();
	autohide.SetLook(st.btnlook).SetImage(st.autohide).Refresh();
}

DockableWindow::DockableWindow()
{
	isdestroying = false;
	dock_state = STATE_NONE;
	base = NULL;
	style = NULL;
	SyncButtonStyle();
	floatpt = Null;
	floatsz = Size(INT_NULL, INT_NULL);
	docksz = Size(INT_NULL, INT_NULL);
	Sizeable();
	ToolWindow();
	SetTitleBar();
}

DockableWindow::~DockableWindow()
{
	isdestroying = true;
	if (base) base->Detach(*this);
}

void DockableTabWindow::ChildAdded(Ctrl *child)
{
	if (child->GetParent() != this || child == &tabbar) return;
	TopWindow *wnd = dynamic_cast<TopWindow *>(child);
	wnd ? tabbar.AddTab(*wnd, wnd->GetTitle()) : tabbar.AddTab(*child, WString("Tab " + AsString(tabbar.GetCount() + 1)));
	tabbar.SelectTab(tabbar.GetCount()-1);
	TabSelected(tabbar.GetCount()-1);
}

void DockableTabWindow::ChildRemoved(Ctrl *child)
{
	if (isdestroying) return;
	int ix = tabbar.GetTabIx(*child);
	if (ix >= 0) tabbar.RemoveTab(ix);
}

void DockableTabWindow::TabSelected(int ix) 
{
	if (ix >= 0) {
		Ctrl *c = tabbar.GetTabCtrl(ix);
		for (int i = 0; i < tabbar.GetCount(); i++) {
			Ctrl *q = tabbar.GetTabCtrl(i);
			if (q) q->Hide();
		}
		if (c) {
			c->SizePos().Show();
			const WString &t = tabbar.GetTabTitle(ix);
			DockableWindow *q = dynamic_cast<DockableWindow *>(c);
			if (q) {
				floatpt = q->GetFloatPoint();
				floatsz = q->GetFloatSize();
				docksz = q->GetDockSize();
				titlebar.SetImage(q->GetIcon());
			}
			titlebar.SetTitle(t);
			this->Title(t);
		}
	}
}

void DockableTabWindow::TabDragged(int ix) 
{
	if (ix >= 0) {
		DockableWindow *q = dynamic_cast<DockableWindow *>(tabbar.GetTabCtrl(ix));
		ASSERT(q);		
		GetDockBase()->AttachedWindowDragStart(*q);
	}
}

void DockableTabWindow::TabContext(int ix)
{
	if (ix >= 0) {
		DockableWindow *q = dynamic_cast<DockableWindow *>(tabbar.GetTabCtrl(ix));
		ASSERT(q);		
		q->WindowMenu();
	}	
}

void DockableTabWindow::AddTabsFrom(DockableTabWindow &wnd)
{
	TabBar &src = wnd.tabbar;
	int cnt = GetCount();
	int sel = cnt + src.GetSelected();
	while (src.GetCount()) {
		Ctrl *c = src.GetTabCtrl(0);
		if (c) {
			c->Remove();
			this->Add(*c);
		}
	}
	if (sel < GetCount() && sel >= 0)
		TabSelected(sel);
}

void DockableTabWindow::WindowMenu()
{
	if (!GetDockBase()) return;
	MenuBar bar;
	bar.Add(true, "Docked", (IsDockedAny() && !IsAutoHide()) ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK(DockMenu));
	bar.Add(IsDockedAny(), "Floating", 	IsFloating() ? CtrlImg::MenuCheck1 : CtrlImg::MenuCheck0, THISBACK(TabsFloat));
	bar.Separator();
	bar.Add(true, "Hide", CtrlImg::Remove(), THISBACK(TabsClose));
	bar.Add(true, "AutoHide", CtrlImg::MenuCheck0, THISBACK(AutoHideMenu));
	bar.Execute();		
}

void DockableTabWindow::DockMenu(Bar &bar)
{
	int align = GetDockBase()->GetDockAlign(*this);
	bar.Add(true, "Left", 	DockBase::DOCK_LEFT ? CtrlImg::MenuCheck0 : CtrlImg::MenuCheck1, THISBACK1(TabsDock, 0));
	bar.Add(true, "Top", 	DockBase::DOCK_TOP ? CtrlImg::MenuCheck0 : CtrlImg::MenuCheck1, THISBACK1(TabsDock, 1));
	bar.Add(true, "Right", 	DockBase::DOCK_RIGHT ? CtrlImg::MenuCheck0 : CtrlImg::MenuCheck1, THISBACK1(TabsDock, 2));
	bar.Add(true, "Bottom", DockBase::DOCK_BOTTOM ? CtrlImg::MenuCheck0 : CtrlImg::MenuCheck1, THISBACK1(TabsDock, 3));	
}

void DockableTabWindow::AutoHideMenu(Bar &bar)
{
	bar.Add(true, "Left", 	THISBACK1(TabsAutoHide, (int)DockBase::DOCK_LEFT));
	bar.Add(true, "Top", 	THISBACK1(TabsAutoHide, (int)DockBase::DOCK_TOP));
	bar.Add(true, "Right", 	THISBACK1(TabsAutoHide, (int)DockBase::DOCK_RIGHT));
	bar.Add(true, "Bottom", THISBACK1(TabsAutoHide, (int)DockBase::DOCK_BOTTOM));	
}

void DockableTabWindow::TabsClose()
{
	Ctrl *sel = tabbar.GetSelectedCtrl();
	ASSERT(sel);
	int i = DoPrompt("Close", "close");
	
	if (i >= 0) { // All/All Other
		if (i > 0) sel = NULL; //All
		for (Ctrl *c = tabbar.GetNext(); c; c)
			if (c != sel) {
				DockableWindow *d = dynamic_cast<DockableWindow *>(c);
				ASSERT(d);
				c = c->GetNext();
				d->GetDockBase()->Hide(*d);
			}
			else
				c = c->GetNext();
	}
	else {
		DockableWindow *d = dynamic_cast<DockableWindow *>(sel);
		d->GetDockBase()->Hide(*d);
	}	
}

void DockableTabWindow::TabsDock(int align)
{
	Ctrl *sel = tabbar.GetSelectedCtrl();
	ASSERT(sel);
	int i = DoPrompt("Dock", "dock");
	
	if (i >= 0) { // All/All Other
		if (i > 0) sel = NULL; //All
		for (Ctrl *c = tabbar.GetNext(); c; c)
			if (c != sel) {
				DockableWindow *d = dynamic_cast<DockableWindow *>(c);
				ASSERT(d);
				c = c->GetNext();
				d->Dock(align);
			}
			else
				c = c->GetNext();
	}
	else {
		DockableWindow *d = dynamic_cast<DockableWindow *>(sel);
		d->Dock(align);	
	}
}

void DockableTabWindow::TabsAutoHide(int align)
{
	Ctrl *sel = tabbar.GetSelectedCtrl();
	ASSERT(sel);
	int i = DoPrompt("Hide", "hide");
	
	if (i >= 0) { // All/All Other
		if (i > 0) sel = NULL; //All
		for (Ctrl *c = tabbar.GetNext(); c; c)
			if (c != sel) {
				DockableWindow *d = dynamic_cast<DockableWindow *>(c);
				ASSERT(d);
				c = c->GetNext();
				d->AutoHide(align);
			}
			else
				c = c->GetNext();
	}
	else {
		DockableWindow *d = dynamic_cast<DockableWindow *>(sel);
		d->AutoHide(align);	
	}	
}

void DockableTabWindow::TabsAutoHide0()
{
	ASSERT(GetDockBase()); 
	TabsAutoHide(GetDockBase()->GetDockAlign(*this)); 
}

void DockableTabWindow::TabsFloat()
{
	Ctrl *sel = tabbar.GetSelectedCtrl();
	ASSERT(sel);
	int i = DoPrompt("Float", "float");
	
	if (i >= 0) { // All/All Other
		if (i > 0) sel = NULL; //All
		for (Ctrl *c = tabbar.GetNext(); c; c)
			if (c != sel) {
				DockableWindow *d = dynamic_cast<DockableWindow *>(c);
				ASSERT(d);
				c = c->GetNext();
				d->Float();
			}
			else
				c = c->GetNext();
	}
	else {
		DockableWindow *d = dynamic_cast<DockableWindow *>(sel);
		d->Float();	
	}	
}

int DockableTabWindow::DoPrompt(const char *title, const char *text)
{
	return GetCount() > 1 ? 
		Prompt(	Format("%s Tabs", title), Image(), 
				Format("There are multiple tabs open, which would you like to %s?", text), 
				"All Tabs", "All Other Tabs", "Only This Tab") 
		: 1;
}

DockableTabWindow::DockableTabWindow()
{
	titlebar.Show();
	tabbar.Bottom().SetFont(StdFont(13));
	tabbar.AutoHideMin(1);
	AddFrame(tabbar);
	tabbar.WhenSelect = THISBACK(TabSelected);
	tabbar.WhenDrag = THISBACK(TabDragged);
	tabbar.WhenContext = THISBACK(TabContext);
	
	close <<= THISBACK(TabsClose);
	autohide <<= THISBACK(TabsAutoHide0);
}

CH_STYLE(DockableWindow, Style, StyleDefault)
{
	CtrlsImageLook(btnlook, CtrlsImg::I_TB, 4);
	close = 				CtrlImg::Remove();
	windowpos = 			DockImg::windowpos();
	autohide = 				DockImg::autohide();
}

