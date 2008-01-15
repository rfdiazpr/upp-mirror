#include "TabBar.h"

#define IMAGECLASS TabImg
#define IMAGEFILE <Docking/TabBar.iml>
#include <Draw/iml_source.h>

#define ANIM_SPEED 10
#define ANIM_FRAMES 10

int AutoHideBar::autohide_timeout = 1000;

void TabBar::FrameLayout(Rect& r)
{
	if (IsAutoHide()) return Hide();
	Show();
	Rect rr = r;
	int tsz = GetTabSz();
	switch(GetAlign()) {
	case TabBar::LEFT:
		r.left += tsz;
		rr.right = r.left;
		break;
	case TabBar::RIGHT:
		r.right -= tsz;
		rr.left = r.right;
		break;
	case TabBar::TOP:
		r.top += tsz;
		rr.bottom = r.top;
		break;
	case TabBar::BOTTOM:
		r.bottom -= tsz;
		rr.top = r.bottom;
		break;
	}
	SetFrameRect(rr);	
}

void TabBar::FrameAddSize(Size& sz)
{
	if (!IsAutoHide()) 
		{ IsVert() ? sz.cy += GetTabSz() : sz.cx += GetTabSz(); }
}

void TabBar::Paint(Draw& w)
{
	if (IsAutoHide()) return;
	int h = GetTabSz();
	Point pt(0, 0);
	Rect r = GetSize();
	if (align == RIGHT)
		pt.x = r.right - h;
	else if (align == BOTTOM)
		pt.y = r.bottom - h;
	w.DrawRect(r, SColorFace);
	for (int i = 0; i < tabs.GetCount(); i++) {
		DrawTab(w, pt, h, i);
		IsVert() ? pt.y += tabs[i].sz : pt.x += tabs[i].sz;
	}	
}

void TabBar::DrawTab(Draw &w, Point pt, int h, int ix)
{
	const Style *st = St();
	Tab &tab = tabs[ix];
	const WString &title = tab.GetTitle();
	Size tsz = GetTextSize(title, tabfont);

	int imgix = (ix == selix && !nosel) ? 1 : (ix == hlix && !nohl ? 2 : 0);
	if (IsVert()) {
		ChPaint(w, pt.x, pt.y, h, tsz.cx+8, st->vertical[imgix]);
		w.DrawText(pt.x + h - max((h - tsz.cy)/2, 0), pt.y + 4, -900, title, tabfont, SColorText());
	}
	else
	{
		ChPaint(w, pt.x, pt.y, tsz.cx+8, h, st->horizontal[imgix]);
		w.DrawText(pt.x+4, pt.y + max((h - tsz.cy)/2, 0), title, tabfont, SColorText());
	}	
	tab.sz = tsz.cx+8;
}

TabBar & TabBar::AddTab(const WString &title)
{
	bool h = IsAutoHide();
	Tab &t = tabs.Create<Tab>(); 
	t.sz = 0; 
	t.title = title; 
	Refresh();	
	if (h != IsAutoHide())
		RefreshParentLayout();	
	return *this;
}

TabBar & TabBar::AddTab(Ctrl &c, const WString &title)
{
	bool h = IsAutoHide();
	CtrlTab &t = tabs.Create<CtrlTab>(); 
	t.sz = 0; 
	t.ctrl = &c; 
	t.title = title; 
	Refresh();	
	if (h != IsAutoHide())
		RefreshParentLayout();	
	return *this;
}

void TabBar::MouseMove(Point p, dword keyflags)
{
	int ix = GetTabIx(p);
	WhenHighlight(ix);
	bool r = ix != hlix;
	hlix = ix;
	if (r) Refresh();
}

void TabBar::MouseLeave()
{
	if (hlix >= 0) {
		hlix = -1;
		Refresh();	
	}		
}

void TabBar::LeftDown(Point p, dword keyflags)
{
	int ix = GetTabIx(p);
	if (ix >= 0 && ix != selix) {
		selix = ix;
		WhenSelect(ix);
		Refresh();
	}
}

void TabBar::LeftDrag(Point p, dword keyflags)
{
	int ix = GetTabIx(p);
	if (ix >= 0) WhenDrag(ix);
}

void TabBar::RightDown(Point p, dword keyflags)
{
	int ix = GetTabIx(p);
	if (ix >= 0) WhenContext(ix);	
}

void TabBar::RemoveTab(int ix)
{
	bool select = selix == ix;
	ASSERT(ix < tabs.GetCount() && ix >= 0); 
	tabs.Remove(ix); 
	if (selix >= tabs.GetCount()) 
		selix = tabs.GetCount()-1;	
	if (select) 
		WhenSelect(selix); 
	Refresh();	
}

int TabBar::GetTabIx(Point p) const
{
	int x = 0;
	if (p.x < 0 || p.y < 0) return -1;
	for (int i = 0; i < tabs.GetCount(); i++) {
		x += tabs[i].sz; 
		if (IsVert() ? x > p.y : x > p.x) return i;
	}
	return -1;	
}

TabBar::TabBar()
{
	selix = autohide = hlix = -1; 
	nosel = nohl = false;
	Bottom(); 
	style = NULL; 
	tabfont = StdFont(11);	
	BackPaint();
}

void AutoHideBar::MouseEnter(Point p, dword keyflags)
{
	if (showctrl && showctrl->IsOpen())
		KillTimeCallback(TIMEID_HIDE_TIMEOUT);			
}

void AutoHideBar::MouseLeave()
{
	if (showctrl && showctrl->IsOpen())
		KillSetTimeCallback(autohide_timeout, THISBACK1(HideAnimate, showctrl), TIMEID_HIDE_TIMEOUT);				
}

AutoHideBar & AutoHideBar::AddTab(Ctrl &c)
{
	TopWindow *wnd = dynamic_cast<TopWindow *>(&c);
	wnd ? TabBar::AddTab(*wnd, wnd->GetTitle()) : TabBar::AddTab(c, WString("Tab " + AsString(TabBar::GetCount() + 1)));
	return *this;
}

AutoHideBar & AutoHideBar::AddTab(Ctrl &c, const WString &title)
{
	TabBar::AddTab(c, title);
	return *this;
}

AutoHideBar & AutoHideBar::RemoveTab(int ix)
{
	if (ix >= 0) {
		bool h = IsAutoHide();
		Ctrl *c = TabBar::GetTabCtrl(ix);
		if (c) {
			if (c->IsOpen()) c->Close();
			if (showctrl == c) {
				if (popup.IsOpen())
					popup.Close();
				showctrl->Remove();
				KillTimeCallback(TIMEID_ANIMATE);
				KillTimeCallback(TIMEID_HIDE_TIMEOUT);			
				showctrl = NULL;				
			}
		}
		TabBar::RemoveTab(ix);
		if (h != IsAutoHide())
			RefreshParentLayout();
	}		
	return *this;
}

void AutoHideBar::TabHighlight(int ix)
{
	Ctrl *c = NULL;
	if (ix >= 0) {
		SelectTab(ix);
		c = TabBar::GetTabCtrl(ix);
	}
	if (!c || showctrl == c) 
		return;
	else if (showctrl) {
		if (c) {
			if (popup.IsOpen())
				popup.Close();
			showctrl->Remove();
			showctrl = NULL;
		}
		else 
			HideAnimate(showctrl);
	}
	if (c) ShowAnimate(c);
}

void AutoHideBar::ShowAnimate(Ctrl *c)
{
	if (c->IsOpen())
		c->Close();
	Rect r = Ctrl::GetScreenRect();
	Size fr = c->AddFrameSize(Size(0, 0));
	Size sz = c->GetStdSize();// + fr;
	switch (GetAlign()) {
	 	case TabBar::LEFT: 
	 		sz.cy = r.Height();
	 		r.left = r.right;
	 		r.right += 5; // Popups don't work on Linux if width or hieght is 0
	 		break;
	 	case TabBar::TOP:
	 		sz.cx = r.Width();
	 		r.top = r.bottom;
	 		r.bottom += 5; // Popups don't work on Linux if width or hieght is 0
	 		break;
	 	case TabBar::RIGHT: 
	 		sz.cy = r.Height();
	 		r.right = r.left;
	 		r.left -= 5; // Popups don't work on Linux if width or hieght is 0
	 		break;
	 	case TabBar::BOTTOM:
	 		sz.cx = r.Width();
	 		r.bottom = r.top;
	 		r.top -= 5; // Popups don't work on Linux if width or hieght is 0
	 		break;
	};
	// use _NET_FRAME_EXTENTS to properly set ctrl size if popup has additional frame added by window manager?
	c->SetRect(sz);
	//c->SizePos();
	popup << *c;
	c->Show();
	popup.SetRect(r);
	popup.PopUp(GetParent(), false, true, false, false);
	showctrl = c;
	KillSetTimeCallback(-ANIM_SPEED, THISBACK1(AnimateSizePlus, c->GetStdSize()), TIMEID_ANIMATE);// + fr
}

void AutoHideBar::HideAnimate(Ctrl *c)
{
	ASSERT(showctrl);
	Rect r = popup.GetScreenRect();
	TabBar::SelectTab(-1);
	KillSetTimeCallback(-ANIM_SPEED, THISBACK1(AnimateSizeMinus, -r.GetSize()), TIMEID_ANIMATE);	
}

void AutoHideBar::AnimateSizePlus(Size sz)
{
	Rect r = popup.GetScreenRect();
	AdjustSize(r, sz);
	if (IsVert() && r.Width() >= sz.cx) {
		r.right = r.left + sz.cx;
		Ctrl::KillTimeCallback(TIMEID_ANIMATE);
	}
	else if (!IsVert() && r.Height() >= sz.cy) {
		r.bottom = r.top + sz.cy;
		if (GetAlign() == TabBar::BOTTOM) r.bottom++;
		Ctrl::KillTimeCallback(TIMEID_ANIMATE);	
	}	
	popup.SetRect(r);
}

void AutoHideBar::AnimateSizeMinus(Size sz)
{
	Rect r = popup.GetScreenRect();
	AdjustSize(r, sz);
	if ((IsVert() && r.Width() <= 12) || (!IsVert() && r.Height() <= 12)) {
		popup.Close();
		showctrl->Remove();
		showctrl = NULL;
		Ctrl::KillTimeCallback(TIMEID_ANIMATE);
	}
	else
		popup.SetRect(r);
}

void AutoHideBar::AdjustSize(Rect &r, const Size &sz)
{
	switch (TabBar::GetAlign()) {
	 	case TabBar::LEFT: 
	 		r.right += sz.cx / ANIM_FRAMES;
	 		break;
	 	case TabBar::TOP: 
	 		r.bottom += sz.cy / ANIM_FRAMES;
	 		break;
	 	case TabBar::RIGHT:
	 		r.left -= sz.cx / ANIM_FRAMES;
	 		break;
	 	case TabBar::BOTTOM:
	 		r.top -= sz.cy / ANIM_FRAMES;
	 		break;
	};		
}

AutoHideBar::AutoHideBar()
{
	showctrl = NULL;
	AutoHideMin(0).NoHighlight();
	popup.WhenEnter = THISBACK2(MouseEnter, Point(0, 0), 0);
	popup.WhenLeave = THISBACK(MouseLeave);
	popup.AddFrame(FieldFrame());
	TabBar::WhenHighlight = THISBACK(TabHighlight);
}

void AutoHideBar::HidePopup::ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
{
	if (event == MOUSELEAVE)
		WhenLeave();
	else if (event != MOUSEMOVE) 
		WhenEnter();
}

CH_STYLE(TabBar, Style, StyleDefault)
{
	vertical[0] = TabImg::VN(); 
	vertical[1] = TabImg::VA();
	vertical[2] = TabImg::VH();
	horizontal[0] = TabImg::HN(); 
	horizontal[1] = TabImg::HA();
	horizontal[2] = TabImg::HH();
}

