//////////////////////////////////////////////////////////////////////
// TabCtrl: tab control.

#include "CtrlLib.h"

#define LLOG(x) // RLOG(x)

enum
{
	ICONGAP = 4,

	CELLSEP = -4,
	INITGAP = -1,
	ENDGAP  = -CELLSEP,

	BUTTON  = 16,
	BUTTGAP = 2,
	BUTTSPC = 5,

	RIGHTADD = 2 * BUTTON + BUTTGAP + BUTTSPC,
	SCROLLADD = 15,

	RAGADD = 5,
};

TabCtrl::Item::Item(TabCtrl& owner)
: owner(owner)
, text(String::GetVoid())
, enabled(true)
, slave(0)
, ctrl(NULL)
{
}

TabCtrl::Item& TabCtrl::Item::Text(const String& _text)
{
	text = _text;
	owner.RefreshTabLayout();
	return *this;
}

Size TabCtrl::Item::GetSize(Draw& draw) const
{
	if(ctrl)
		return ctrl -> GetRect().Size();
	Size ts = GetSmartTextSize(draw, text, Font(owner.font).Bold(!IsXPStyle()));
	Size is(0, 16);
	if(pict)
		is = pict.GetStdSize();
	if(is.cx > 0 && ts.cx > 0)
		is.cx += ICONGAP;
	is.cx += ts.cx + 2 * owner.sidegap;
	is.cy = max(is.cy, ts.cy) + 2 * owner.vertgap;
	return is;
}

TabCtrl::Item& TabCtrl::Item::Picture(PaintRect d)
{
	pict = d;
	owner.RefreshTabLayout();
	return *this;
}

TabCtrl::Item& TabCtrl::Item::Control(Ctrl *_ctrl)
{
	if(ctrl)
		ctrl -> Remove();
	if(_ctrl)
		owner.Ctrl::Add(*_ctrl);
	ctrl = _ctrl;
	owner.RefreshTabLayout();
	return *this;
}

TabCtrl::Item& TabCtrl::Item::Slave(Ctrl *_slave)
{
	if(slave)
		slave -> Remove();
	if(_slave) {
		_slave -> Hide();
		owner.Ctrl::Add(*_slave);
	}
	slave = _slave;
	owner.RefreshTabLayout();
	return *this;
}

TabCtrl::Item& TabCtrl::Item::Enable(bool _en)
{
	if(_en != enabled) {
		enabled = _en;
		owner.RefreshFrame();
	}
	return *this;
}

TabCtrl::TabCtrl()
: scroll(0)
, top(-1)
, frame_size(0, 0)
, old_frame_size(0, 0)
{
	font = StdFont();
	accept_current = false;
	out_frame = in_frame = Rect(0, 0, 0, 0);
	Ctrl::Transparent();
	AddFrame(*this);
	NoWantFocus();
	prev.NoWantFocus();
	prev.SetImage(CtrlImg::left_arrow());
	next.NoWantFocus();
	next.SetImage(CtrlImg::right_arrow());
	prev <<= THISBACK(GoPrev);
	next <<= THISBACK(GoNext);
	sidegap = 12;
	vertgap = 4;
	hl = -1;
}

TabCtrl::~TabCtrl()
{
}

void TabCtrl::Clear()
{
	items.Clear();
	Set(-1);
	Refresh();
}

int TabCtrl::Find(Point pt) const
{
	if(prev.IsVisible() && pt.x >= find_max_x)
		return -1;

	Point16 temp = pt;
	temp.x += scroll;
	for(int i = 0; i < items.GetCount(); i++)
		if(items[i].enabled && items[i].rect.Contains(temp))
			return i;
	return -1;
}

TabCtrl::Item& TabCtrl::Add()
{
	return items.Add(new Item(*this));
}

void TabCtrl::Set(int _top, bool focus)
{
	LLOG("TabCtrl::Set(" << _top << "), #items = " << items.GetCount());
	if(items.IsEmpty())
		_top = -1;
	else
		_top = minmax(_top, 0, items.GetCount() - 1);
	if(top >= 0 && top < items.GetCount() && items[top].slave) {
		if(items[top].slave->HasFocusDeep())
			items[top].activefocus = GetFocusCtrl();
		LLOG("-> hide slave #" << top);
		items[top].slave -> Hide();
	}
	RefreshItem(top);
	top = _top;
	if(top >= 0 && top < items.GetCount() && items[top].slave) {
		LLOG("-> show slave #" << top);
		items[top].slave->Show();
		items[top].slave->AssignAccessKeys(GetTopCtrl()->GetAccessKeysDeep()
		                                   & ~(GetAccessKeysDeep() & ~1));
		if(focus) {
			if(items[top].activefocus)
				items[top].activefocus->SetWantFocus();
			if(items[top].slave && !items[top].slave->HasFocusDeep())
				IterateFocusForward(this, GetTopCtrl());
		}
	}
	RefreshItem(top);
	ScrollInto(top);
	prev.Enable(top > 0);
	next.Enable(top + 1 < GetCount());
	WhenSet();
	LLOG("//TabCtrl::Set -> " << top);
}

void TabCtrl::ScrollInto(int i)
{
	if(i < 0 || i >= items.GetCount() || scroll_limit <= 0)
		return;
	Rect rc = items[i].rect;
	Size size = GetSize();
	if(size.cx - RIGHTADD - 2 * SCROLLADD <= rc.Width())
		SetScroll(rc.left);
	else if(rc.left - scroll < SCROLLADD)
		SetScroll(rc.left - SCROLLADD);
	else if(rc.right - scroll > size.cx - RIGHTADD - SCROLLADD)
		SetScroll(rc.right - size.cx + RIGHTADD + SCROLLADD);
}

void TabCtrl::SetScroll(int _scroll)
{
	_scroll = minmax(_scroll, 0, scroll_limit);
	LLOG("TabCtrl::SetScroll(" << _scroll << ")");
	if(_scroll != scroll) {
		scroll = _scroll;
		RefreshFrame();
	}
}

void TabCtrl::RefreshTabLayout()
{
	LLOG("TabCtrl::RefreshTabLayout, #items = " << items.GetCount() << ", top = " << top);

	frame_size = Size(INITGAP + ENDGAP, 0);

	int i;
	Draw& sdraw = ScreenInfo();
	for(i = 0; i < items.GetCount(); i++) {
		Item& item = items[i];
		if(item.slave) {
			if(item.slave -> GetParent() != this)
				Ctrl::Add(*item.slave);
		}
		item.size = item.GetSize(sdraw);
		if(item.size.cy > frame_size.cy)
			frame_size.cy = item.size.cy;
		frame_size.cx += item.size.cx + CELLSEP;
	}

	if(frame_size != old_frame_size) {
		old_frame_size = frame_size;
		RefreshParentLayout();
		RefreshLayout();
		RefreshFrame();
	}

	int xpos = INITGAP;
	for(i = 0; i < items.GetCount(); i++) {
		Item& item = items[i];
		Rect rc(xpos, 0, xpos + item.size.cx, item.size.cy);
		rc.top = frame_size.cy - item.size.cy;
		rc.bottom = frame_size.cy;
		item.rect = rc;
		xpos = rc.right + CELLSEP;
	}

	if(!items.IsEmpty())
		Set(top);

	prev.Enable(top > 0);
	next.Enable(top + 1 < GetCount());

	LLOG("//TabCtrl::RefreshTabLayout, top = " << top);
}

void TabCtrl::FrameAdd(Ctrl& ctrl)
{
	ctrl << prev << next;
}

void TabCtrl::FrameRemove()
{
	prev.Remove();
	next.Remove();
}

void TabCtrl::FrameAddSize(Size& sz)
{
	sz.cx += 2;
	sz.cy += 1 + frame_size.cy;
	if(IsXPStyle()) {
		sz.cx += 2 + 2;
		sz.cy += 2 + 2;
	}
}

void TabCtrl::FrameLayout(Rect& rc)
{
	LLOG("TabCtrl::FrameLayout: rc = " << rc << ", frame_size = " << frame_size);
	out_frame = rc;
	scroll_limit = max(frame_size.cx - rc.Width(), 0);
	scroll = minmax(scroll, 0, scroll_limit);
	bool scr = (scroll_limit > 0);
	Rect next_rect;

	rc.top += frame_size.cy;
	rc.bottom -= 1;
	next_rect = RectC(rc.right - BUTTON, rc.top - BUTTON - 2, BUTTON, BUTTON);
	if(scr) {
		next.SetFrameRect(next_rect);
		prev.SetFrameRect(next_rect.OffsetedHorz(-BUTTON - BUTTGAP));
	}
	prev.Show(scr);
	next.Show(scr);
	if(scr) {
		scroll_limit += RIGHTADD;
		find_max_x = prev.GetRect().left;
	}
	else
		find_max_x = frame_size.cx;
	rc.left++;
	rc.right -= 1;
	in_frame = rc;

	if(IsXPStyle()) {
		rc.right -= 2;
		rc.bottom -= 2;
		rc.Deflate(1);
	}

	LLOG("//TabCtrl::FrameLayout: scroll = " << scroll << ", scroll_limit = " << scroll_limit);
}

Vector<Rect> TabCtrl::GetTransparentFrameRects()
{
	return Vector<Rect>() << Size(GetRect().Width(), frame_size.cy);
}

Vector<Rect> TabCtrl::GetOpaqueFrameRects()
{
	Rect r = GetRect().Size();
	r.top = frame_size.cy;
	return Vector<Rect>() << r;
}

Vector<Rect> TabCtrl::GetTransparentViewRects()
{
	return Vector<Rect>();
}
Color TabCtrl::GetTabColor()
{
	return IsXPStyle() ? Blend(SColorPaper, SColorFace, 80) : SColorFace;
}

void TabCtrl::FramePaint(Draw& w, const Rect& rc)
{
	int right_lim = rc.right;
	if(next.IsVisible()) {
		w.ExcludeClip(next.GetRect());
		right_lim = next.GetRect().left;
	}
	if(prev.IsVisible()) {
		w.ExcludeClip(prev.GetRect());
		right_lim = prev.GetRect().left;
	}
	if(!IsTransparent())
		w.DrawRect(rc, SLtGray);
	Rect clip = w.GetClip();
	if(clip.right > right_lim)
		clip.right = right_lim;
	int sh = rc.top + frame_size.cy - 1;
	int h = rc.bottom - sh;

	if(IsXPStyle()) {
		DrawFrame(w, rc.left, sh, rc.Width() - 2, h - 2, Blend(SColorHighlight, SColorShadow));
		DrawFrame(w, rc.left + 1, sh + 1, rc.Width() - 4, h - 4, GetTabColor());
		w.DrawRect(rc.left, rc.bottom - 2, rc.Width(), 2, Blend(SColorFace, SColorShadow, 60));
		w.DrawRect(rc.left + 1, rc.bottom - 2, rc.Width() - 2, 1, Blend(SColorFace, SColorShadow, 120));
		w.DrawRect(rc.right - 2, sh, 2, h, Blend(SColorFace, SColorShadow, 60));
		w.DrawRect(rc.right - 2, sh + 1, 1, h - 2, Blend(SColorFace, SColorShadow, 120));
	}
	else
		DrawFrame(w, rc.left, sh, rc.Width(), h, SColorLight, SColorShadow);

	if(items.IsEmpty())
		return;

	ragged_left = -32767;
	ragged_right = 32767;
	if(scroll_limit > 0) {
		Size client = GetSize();
		if(top + 1 < items.GetCount() || client.cx - RIGHTADD <= items[top].rect.Width())
			ragged_right = client.cx - RIGHTADD + scroll;
		if(top > 0)
			ragged_left = scroll + 2;
	}

	w.Offset(-scroll, 0);
	clip.OffsetHorz(scroll);
	for(int i = 0; i < items.GetCount(); i++)
		if(i != top && !DrawItemCheck(w, clip, i))
			break;
	if(top >= 0 && top < items.GetCount())
		DrawItemCheck(w, clip, top);
	w.End();
}

void TabCtrl::Paint(Draw& w)
{
	w.DrawRect(GetSize(), GetTabColor());
}

void TabCtrl::Layout()
{
	Size client = GetSize();

	Set(top);
	Refresh();
}

bool TabCtrl::DrawItemCheck(Draw& draw, const Rect& clip, int i)
{
	const Item& item = items[i];
	if(item.rect.right <= ragged_left)
		return true;
	if(item.rect.left >= ragged_right)
		return false;
	if(item.rect.Intersects(clip)) {
		draw.Clip(clip);
		DrawItem(draw, i);
		draw.End();
	}
	return true;
}

void TabCtrl::DrawRaggedEdges(Draw& draw, const Rect& rc, int delta)
{
	if(ragged_left >= rc.left + delta)
		DrawRaggedEdge(draw, Point(ragged_left, rc.top), rc.Height(), false);
	if(ragged_right <= rc.right - delta)
		DrawRaggedEdge(draw, Point(ragged_right, rc.top), rc.Height(), true);
}

void TabCtrl::DrawRaggedEdge(Draw& draw, Point pos, int height, bool right_side)
{
	static const int part[] = { 5, 0, 15, 1, 40, 2, 60, 1, 80, 0, 95, 1, 100, 2 };

	int curr = pos.y;
	int right = GetSize().cx + scroll;

	for(const int *p = part; p < part + __countof(part); p += 2) {
		int next = pos.y + iscale(height, *p, 100);
		int y1, y2;
		y1 = curr;
		y2 = next;
		if(right_side) {
			int x = pos.x + p[1];
			draw.DrawRect(x, y1, 1, y2 - y1, IsXPStyle()? Blend(SColorHighlight, SColorShadow) : SColorText);
			draw.ExcludeClip(Rect(x, y1, right, y2));
		}
		else {
			int x = pos.x - p[1];
			draw.DrawRect(x, y1, 1, y2 - y1, IsXPStyle()? Blend(SColorHighlight, SColorShadow) : SColorLight);
			draw.ExcludeClip(Rect(scroll, y1, x + 1, y2));
		}
		curr = next;
	}
}

void TabCtrl::DrawItemFrame(Draw& draw, Rect& rc, bool active, bool light)
{
	Rect temp = rc;
	Color c = Blend(SColorHighlight, SColorShadow);
	if(!active) {
		if(IsXPStyle()) {
			rc.Deflate(2, 0);
			rc.top += 2;
			draw.DrawRect(rc.left + 1, rc.bottom - 1, rc.Width() - 2, 1, c);
		}
		else {
			draw.DrawRect(rc.left, rc.bottom - 1, rc.Width(), 1, SWhite);
			draw.DrawRect(rc.left, rc.bottom - 2, rc.Width(), 1, SLtGray);
			rc.Deflate(2);
		}
	}

	Size size = rc.Size();

	DrawRaggedEdges(draw, rc);

	if(IsXPStyle()) {
		Color pc = Blend(SColorPaper, SColorFace, 80);
		Color hc = active || light ? Blend(Yellow, LtRed, 100) : pc;
		Color qc = active || light ? Blend(hc, c) : c;
		draw.DrawRect(rc.left + 2, rc.top + 1, 1, 1, qc);
		draw.DrawRect(rc.left + 3, rc.top, size.cx - 5, 1, qc);
		draw.DrawRect(rc.right - 2, rc.top + 1, 1, 1, qc);
		draw.DrawRect(rc.left + 3, rc.top + 1, size.cx - 5, 1, hc);
		draw.DrawRect(rc.left + 2, rc.top + 2, size.cx - 3, 1, hc);
		draw.DrawRect(rc.left + 1, rc.top + 2, 1, size.cy - 2, c);
		draw.DrawRect(rc.right - 1, rc.top + 2, 1, size.cy - 2, c);
		if(active)
			draw.DrawRect(rc.left + 2, rc.top + 3, size.cx - 3, size.cy - 3, pc);
		else
			for(int i = 0; i < size.cy - 5; i++)
				draw.DrawRect(rc.left + 2, rc.top + 3 + i, size.cx - 3, 1,
				              Blend(pc, SColorFace, 256 * i / (size.cy - 4)));
	}
	else {
		draw.DrawRect(rc.left, rc.top, 1, size.cy - 1, SLtGray);
		draw.DrawRect(rc.left + 1, rc.top + 2, 1, size.cy - 2, SColorPaper);
		draw.DrawRect(rc.left + 2, rc.top + 1, 1, 1, SColorPaper);
		draw.DrawRect(rc.left + 2, rc.top + 2, 1, size.cy - 2, SLtGray);
		draw.DrawRect(rc.left + 3, rc.top, size.cx - 5, 1, SColorPaper);
		draw.DrawRect(rc.left + 3, rc.top + 1, size.cx - 5, size.cy - 1, SLtGray);
		draw.DrawRect(rc.right - 2, rc.top, 1, size.cy, SColorText);
		draw.DrawRect(rc.right - 1, rc.top + 2, 1, size.cy - 2, SColorText);
	}

	rc = temp;
	rc.Deflate(sidegap, vertgap + 1, sidegap, vertgap - 1);
	if(active)
		rc.OffsetVert(-1);
}

void TabCtrl::DrawItem(Draw& draw, int i)
{
	const Item& item = items[i];
	Rect rc = item.rect;
	DrawItemFrame(draw, rc, i == top, i == hl);

	if(item.ctrl) {
		Rect ctrc = item.ctrl -> GetRect();
		Size offset = ctrc.TopLeft() - rc.TopLeft();
		offset.cx -= scroll;
		ctrc.Offset(offset);
		item.ctrl -> SetRect(ctrc);
	}
	else {
		if(item.pict) {
			Size size = item.pict.GetStdSize();
			Point pos = rc.TopLeft();
			pos.y += (rc.Height() - size.cy) >> 1;
			item.pict.Paint(draw, Rect(pos, size), SBlack, SLtGray, 0);
			rc.left += size.cx + ICONGAP;
		}
		Font f = Font(font);
		if(!IsXPStyle())
			f.Bold(i == top);
		String text = item.text; //item.GetTextOrValue();
		Size size = GetSmartTextSize(draw, text, f);
		Point pos = rc.TopLeft();
		pos.y += (rc.Height() - size.cy) >> 1;
		DrawLabel drawlbl;
		drawlbl.disabled = !item.enabled;
		drawlbl.focus = HasFocus();
		drawlbl.font = f;
		drawlbl.align = ALIGN_CENTER;
		drawlbl.text = text;
		drawlbl.Paint(draw, pos.x, pos.y, rc.right - pos.x, rc.bottom - pos.y);
	}
}

void  TabCtrl::CancelMode()
{
	hl = -1;
}

Image TabCtrl::FrameMouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	if(IsXPStyle()) {
		int q = hl;
		if(event == MOUSELEAVE)
			q = -1;
		if(event == MOUSEMOVE)
			q = Find(p - GetFrameOffset());
		if(q != hl) {
			RefreshItem(hl);
			hl = q;
			RefreshItem(hl);
		}
	}
	if(event == LEFTDOWN || event == RIGHTDOWN) {
		if(!IsReadOnly()) {
			int i = Find(p - GetFrameOffset());
			if(i >= 0 && i != Get()) {
				Set(i);
				Ctrl::Action();
			}
		}
		if(event == RIGHTDOWN)
			MenuBar::Execute(WhenBar);
		return Null;
	}
	return Ctrl::FrameMouseEvent(event, p, zdelta, keyflags);
}

bool TabCtrl::Key(dword key, int repcnt)
{
	switch(key) {
	case K_CTRL_TAB:       GoNext(); Ctrl::Action(); return true;
	case K_SHIFT_CTRL_TAB: GoPrev(); Ctrl::Action(); return true;
	}
	return MenuBar::Scan(WhenBar, key);
}

bool TabCtrl::HotKey(dword key)
{
	if(Ctrl::HotKey(key))
		return true;

	switch(key) {
	case K_CTRL_TAB:       GoNext(); Ctrl::Action(); return true;
	case K_SHIFT_CTRL_TAB: GoPrev(); Ctrl::Action(); return true;
	}

	for(int i = 0; i < items.GetCount(); i++)
		if(items[i].IsEnabled() && items[i].key == key) {
			Set(i);
			return true;
		}

	return false;
}

bool TabCtrl::Accept()
{
	int old = Get();
	if(accept_current)
		return !items[old].slave || items[old].slave -> Accept();
	for(int i = 0; i < items.GetCount(); i++)
		if(items[i].slave) {
			Set(i);
			if(!items[i].slave -> Accept())
				return false;
		}
	Set(old);
	return true;
}

Size TabCtrl::GetFrameOffset() const
{
	return out_frame.TopLeft();
}

void TabCtrl::RefreshItem(int item)
{
	if(item >= 0 && item < items.GetCount())
		RefreshFrame(items[item].rect + (GetFrameOffset() - Size(scroll, 0)));
}

void TabCtrl::GoPrev()
{
	if(IsEditable()) {
		int t = top;
		for(int i = 0; i < GetCount(); i++) {
			if(--t < 0) t = GetCount() - 1;
			if(items[t].enabled) {
				Set(t, true);
				return;
			}
		}
	}
}

void TabCtrl::GoNext()
{
	if(IsEditable()) {
		int t = top;
		for(int i = 0; i < GetCount(); i++) {
			if(++t >= GetCount()) t = 0;
			if(items[t].enabled) {
				Set(t, true);
				return;
			}
		}
	}
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
	SetRect(Rect(GetRect().TopLeft(), AddFrameSize(tabctrl.AddFrameSize(sz) + Ctrl::LayoutZoom(Size(8, 40)))));
}

TabCtrl::Item& TabDlg::Add0(Ctrl& tab, const char *text)
{
	TabCtrl::Item& m = tabctrl.Add(tab, text);
	Size tsz = max(tab.GetSize(), sz);
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
