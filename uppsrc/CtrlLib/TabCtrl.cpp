#include "CtrlLib.h"

CH_FONT(TabCtrlFont, StdFont());

CH_INTS(TabCtrlMetric, TabCtrl::METRIC_COUNT,
       		TabCtrlFont().Info().GetHeight() + 8  // TABHEIGHT
       		<< 2 // MARGIN
       		<< 2 << 2 << 2 << 2 // SEL*
       		<< 6 << 6 << 6 << 6 // *EDGE
);

CH_LOOKS(TabCtrlLook, TabCtrl::LOOK_COUNT, CtrlsImgLook
	(CtrlsImg::I_FTAB)
	(CtrlsImg::I_TAB)
	(CtrlsImg::I_TABB, 1)
);

TabCtrl::Item& TabCtrl::Item::Text(const String& _text)
{
	text = _text;
	owner->Layout();
	return *this;
}

TabCtrl::Item& TabCtrl::Item::Picture(PaintRect d)
{
	pict = d;
	owner->Layout();
	return *this;
}

TabCtrl::Item& TabCtrl::Item::SetCtrl(Ctrl *_ctrl)
{
	if(ctrl)
		ctrl->Remove();
	ctrl = _ctrl;
	owner->Layout();
	return *this;
}

TabCtrl::Item& TabCtrl::Item::Slave(Ctrl *_slave)
{
	if(slave)
		slave->Remove();
	slave = _slave;
	if(slave)
		owner->pane.Add(*slave);
	int q = owner->sel;
	owner->sel = -1;
	owner->Set(q);
	return *this;
}

TabCtrl::Item& TabCtrl::Item::Enable(bool _en)
{
	enabled = _en;
	owner->Refresh();
	return *this;
}

TabCtrl::Item::Item()
{
	ctrl = slave = NULL;
	enabled = true;
	key = 0;
}

void TabCtrl::Item::Layout(int xp, int y, int cy)
{
	Size chsz = GetTextSize("M", TabCtrlFont());
	x = xp;
	Size sz = pict.GetStdSize();
	if(sz.cx) {
		xp += chsz.cx / 2;
		pictpos.x = xp;
		pictpos.y = y + (cy - sz.cy) / 2;
		xp += sz.cx + chsz.cx / 2;
	}
	else
		xp += chsz.cx;
	sz = GetTextSize(text, TabCtrlFont());
	if(sz.cx) {
		textpos.x = xp;
		textpos.y = y + (cy - sz.cy) / 2;
		xp += sz.cx;
	}
	if(ctrl) {
		xp += chsz.cx / 2;
		sz = ctrl->GetRect().GetSize();
		ctrl->SetRect(xp, y + (cy - sz.cy) / 2, sz.cx, sz.cy);
		xp += sz.cx + chsz.cx / 2;
	}
	else
		xp += chsz.cx;
	cx = xp - x;
}

void TabCtrl::Item::Paint(Draw& w)
{
	Size sz = pict.GetStdSize();
	pict.Paint(w, pictpos.x, pictpos.y, sz.cx, sz.cy, SColorText, Null);
	w.DrawText(textpos.x, textpos.y, text, TabCtrlFont());
}

void TabCtrl::SyncTabs()
{
	int x = TabCtrlMetric(MARGIN) - x0;
	for(int i = 0; i < tab.GetCount(); i++) {
		Item& t = tab[i];
		t.Layout(x, metric(SELTOP) * (i != sel),
		            metric(TABHEIGHT) + metric(SELTOP) * (i == sel));
		x += t.cx;
	}
	left.Show(x0 > 0);
	right.Show(tab.GetCount() && tab.Top().Right() > tabs.GetSize().cx);
}

void TabCtrl::Layout()
{
	for(int i = 0; i < tab.GetCount(); i++)
		if(tab[i].ctrl)
			tab[i].ctrl->Remove();
	for(int i = 0; i < tab.GetCount(); i++)
		if(tab[i].ctrl)
			Ctrl::Add(*tab[i].ctrl);
	Size sz = GetSize();
	int th = metric(TABHEIGHT) + metric(SELTOP);
	tabs.TopPos(0, th + metric(SELBOTTOM))
	    .HSizePos(0, metric(SELLEFT) + metric(SELRIGHT));
	SyncTabs();
	SyncHot();
	pane.VSizePos(metric(TABHEIGHT) + metric(TOPEDGE), metric(BOTTOMEDGE))
	    .HSizePos(metric(LEFTEDGE), metric(RIGHTEDGE));
	left.LeftPos(0, 16).TopPos(th - 16, 16);
	right.RightPos(0, 16).TopPos(th - 16, 16);
}

Size TabCtrl::ComputeSize(Size pane)
{
	return Size(pane.cx + metric(LEFTEDGE) + metric(RIGHTEDGE),
	            pane.cy + metric(TABHEIGHT) + metric(TOPEDGE) + metric(BOTTOMEDGE));
}

int TabCtrl::TabsRight()
{
	return tabs.GetSize().cx - metric(SELLEFT) - metric(SELRIGHT);
}

void TabCtrl::Tabs::Paint(Draw& w)
{
	static_cast<TabCtrl *>(GetParent())->PaintTabs(w);
}

Rect TabCtrl::GetOpaqueRect()
{
	return pane.GetRect();
}

void TabCtrl::PaintTabs(Draw& w)
{
	int tt = metric(SELTOP);
	int th = metric(TABHEIGHT) + tt;
	Size sz = GetSize();
	ChPaint(w, 0, th, sz.cx, sz.cy - th, look(BODY));
	Size chsz = GetTextSize("M", TabCtrlFont());
	for(int phase = 0; phase < 2; phase++) {
		for(int i = 0; i < tab.GetCount(); i++)
			if((sel == i) == phase) {
				Item& t = tab[i];
				Rect r = RectC(t.x, tt, t.cx, th - tt);
				if(phase) {
					r.left -= metric(SELLEFT);
					r.right += metric(SELRIGHT);
					r.top -= tt;
					r.bottom += metric(SELBOTTOM);
				}
				ChPaint(w, r, look(
					(i == 0 ? FIRST : NEXT) +
					(!IsEnabled() || !t.enabled ? CTRL_DISABLED :
					 phase ? CTRL_PRESSED :
					 i == hot ? CTRL_HOT : CTRL_NORMAL)
				));
				t.Paint(w);
			}
	}
}

void TabCtrl::Paint(Draw& w)
{
	int th = metric(TABHEIGHT) + metric(SELTOP);
	Size sz = GetSize();
	ChPaint(w, 0, th, sz.cx, sz.cy - th, look(BODY));
}

int  TabCtrl::GetTab(Point p)
{
	if(p.y < metric(TABHEIGHT))
		for(int i = 0; i < tab.GetCount(); i++)
			if(p.x < tab[i].Right())
				return i;
	return -1;
}

void TabCtrl::CancelMode()
{
	hot = -1;
}

void TabCtrl::SyncHot()
{
	Point p = GetMousePos() - GetScreenRect().TopLeft();
	int h = GetTab(p);
	if(h != hot) {
		hot = h;
		tabs.Refresh();
	}
}

void TabCtrl::MouseMove(Point p, dword keyflags)
{
	SyncHot();
}

void TabCtrl::LeftDown(Point p, dword keyflags)
{
	if(!IsEditable())
		return;
	int h = GetTab(p);
	if(h >= 0 && tab[h].IsEnabled()) {
		Set(h);
		Action();
	}
}

void TabCtrl::MouseLeave()
{
	SyncHot();
}

void TabCtrl::ScrollInto(int i)
{
	if(i < 0)
		return;
	Item& t = tab[i];
	int cx = tabs.GetSize().cx;
	int tr = TabsRight();
	if(t.Right() > tr) {
		x0 += t.Right() - tr;
		tabs.Refresh();
		SyncTabs();
	}
	if(t.x < metric(MARGIN)) {
		x0 += t.x - metric(MARGIN);
		tabs.Refresh();
		SyncTabs();
	}
}

void TabCtrl::Left()
{
	if(x0 <= 0)
		return;
	for(int i = tab.GetCount() - 1; i >= 0; i--)
		if(tab[i].x < metric(MARGIN)) {
			ScrollInto(i);
			break;
		}
}

void TabCtrl::Right()
{
	for(int i = 0; i < tab.GetCount(); i++)
		if(tab[i].Right() > tabs.GetRect().GetWidth() - metric(SELLEFT) - metric(SELRIGHT)) {
			ScrollInto(i);
			break;
		}
}

void TabCtrl::Set(int i)
{
	if(i != sel) {
		sel = i;
		tabs.Refresh();
		SyncTabs();
		for(int i = 0; i < tab.GetCount(); i++)
			if(tab[i].slave)
				tab[i].slave->Show(sel == i);
		if(sel >= 0)
			IterateFocusForward(tab[sel].slave, GetTopCtrl(), false, true);
		WhenSet();
	}
	ScrollInto(sel);
}

void TabCtrl::SetData(const Value& data)
{
	Set(data);
}

Value TabCtrl::GetData() const
{
	return Get();
}

TabCtrl::Item& TabCtrl::Add()
{
	Item& t = tab.Add();
	t.owner = this;
	if(sel < 0)
		Set(0);
	return t;
}

TabCtrl::Item& TabCtrl::Add(const char *text)
{
	return Add().Text(text);
}

TabCtrl::Item& TabCtrl::Add(const Image& m, const char *text)
{
	return Add().Text(text).SetImage(m);
}

TabCtrl::Item& TabCtrl::Add(Ctrl& slave, const char *text)
{
	return Add(text).Slave(&slave);
}

TabCtrl::Item& TabCtrl::Add(Ctrl& slave, const Image& m, const char *text)
{
	return Add(slave, text).SetImage(m);
}

void TabCtrl::Go(int d)
{
	if(IsEditable() && tab.GetCount()) {
		int i = sel + d;
		while(i != sel) {
			if(i < 0)
				i = tab.GetCount() - 1;
			if(i >= tab.GetCount())
				i = 0;
			if(tab[i].IsEnabled()) {
				Set(i);
				break;
			}
			i += d;
		}
	}
}

bool TabCtrl::Key(dword key, int repcnt)
{
	switch(key) {
	case K_CTRL_TAB:
		GoNext();
		Action();
		return true;
	case K_SHIFT_CTRL_TAB:
		GoPrev();
		Action();
		return true;
	}
	return Ctrl::Key(key, repcnt);
}

bool TabCtrl::HotKey(dword key)
{
	switch(key) {
	case K_CTRL_TAB:
		GoNext();
		Action();
		return true;
	case K_SHIFT_CTRL_TAB:
		GoPrev();
		Action();
		return true;
	}
	return Ctrl::HotKey(key);
}

bool TabCtrl::Accept()
{
	int ii = Get();
	if(accept_current)
		return !tab[ii].slave || tab[ii].slave -> Accept();
	for(int i = 0; i < tab.GetCount(); i++)
		if(tab[i].slave) {
			Set(i);
			if(!tab[i].slave->Accept())
				return false;
		}
	Set(ii);
	return true;
}

void TabCtrl::Reset()
{
	tab.Clear();
	x0 = 0;
	CancelMode();
	sel = -1;
	Refresh();
	accept_current = false;
}

TabCtrl::TabCtrl()
{
	hot = -1;
	sel = -1;
	x0 = 0;
	Ctrl::Add(tabs);
	Ctrl::Add(pane);
	tabs.BackPaint().IgnoreMouse();
	Ctrl::Add(left.SetImage(CtrlsImg::SLA()).ScrollStyle().NoWantFocus());
	left <<= THISBACK(Left);
	Ctrl::Add(right.SetImage(CtrlsImg::SRA()).ScrollStyle().NoWantFocus());
	right <<= THISBACK(Right);
	Transparent().NoWantFocus();
	tabs.Transparent().NoWantFocus();
	metric = TabCtrlMetric;
	look = TabCtrlLook;
}

// ----------------------------------------------------------------

void TabDlg::PlaceButton(Button& b, int& r)
{
	if(b.GetParent()) {
		b.Remove();
		AddChild(&b, &tabctrl);
		b.RightPosZ(r, 64).BottomPosZ(4, 24);
		r += 72;
	}
}

void TabDlg::Rearrange()
{
	int r = 4;
	PlaceButton(apply, r);
	PlaceButton(cancel, r);
	PlaceButton(exit, r);
	PlaceButton(ok, r);
	SetRect(Rect(GetRect().TopLeft(), tabctrl.ComputeSize(sz) + Ctrl::LayoutZoom(Size(8, 40))));
}

TabCtrl::Item& TabDlg::Add0(Ctrl& tab, const char *text)
{
	Size tsz = max(tab.GetRect().GetSize(), sz);
	TabCtrl::Item& m = tabctrl.Add(tab, text);
	if(tsz != sz) {
		sz = tsz;
		Rearrange();
	}
	return m;
}

TabDlg& TabDlg::AButton(Button& b)
{
	if(binit) {
		exit.Remove();
		binit = false;
	}
	Ctrl::Add(b);
	Rearrange();
	return *this;
}

TabDlg::TabDlg()
{
	binit = true;
	Ctrl::Add(tabctrl.HSizePosZ(4, 4).VSizePosZ(4, 36));
	sz = Size(0, 0);
	ok.Ok().SetLabel(t_("OK"));
	Acceptor(ok, IDOK);
	cancel.Cancel().SetLabel(t_("Cancel"));
	Rejector(cancel, IDCANCEL);
	apply.SetLabel(t_("Apply"));
	Acceptor(apply, IDYES);
	exit.SetLabel(t_("Close"));
	Acceptor(exit, IDEXIT);
	Ctrl::Add(exit);
}
