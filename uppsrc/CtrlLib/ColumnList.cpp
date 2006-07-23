#include "CtrlLib.h"

void ColumnList::MouseWheel(Point p, int zdelta, dword keyflags) {
	sb.Wheel(zdelta);
}

int  ColumnList::GetColumnCx(int i) const {
	int szcx = GetSize().cx;
	int cx = max(szcx / max(ncl, 1), Draw::GetStdFontCy());
	return i == ncl - 1 ? szcx - i * cx : cx;
}

int ColumnList::GetColumnItems() const {
	if(cy == 0) return 0;
	return GetSize().cy / cy;
}

int  ColumnList::GetDragColumn(int x) const {
	int cx = GetColumnCx();
	int i = 0;
	if(cx == 0) return -1;
	for(;;) {
		x -= GetColumnCx(i);
		if(x >= -3 && x < 4)
			return i;
		if(x < 0) return -1;
		i++;
	}
}

void ColumnList::ShiftSelect(int anchor, int cursor) {
	int i;
	int beg = -1;
	int end = -1;
	for(i = GetCount() - 1; i >= 0; i--)
		if(IsSelected(i)) {
			end = i;
			break;
		}
	for(i = 0; i < GetCount(); i++)
		if(IsSelected(i)) {
			beg = i;
			break;
		}
	int f, t;
	ClearSelection();
	if(anchor < 0)
		anchor = cursor;
	if(beg < 0) {
		f = min(anchor, cursor);
		t = max(anchor, cursor);
	}
	else
	if(cursor > anchor)
		if(anchor < end) {
			f = min(cursor, end);
			t = max(cursor, end);
		}
		else {
			f = min(cursor, beg);
			t = max(cursor, beg);
		}
	else
		if(anchor > beg) {
			f = min(beg, cursor);
			t = max(beg, cursor);
		}
		else {
			f = min(end, cursor);
			t = max(end, cursor);
		}
	for(i = f; i <= t; i++)
		SelectOne(i, true);
}

Image ColumnList::CursorImage(Point p, dword)
{
	return HasCapture() || GetDragColumn(p.x) >= 0 ? Image::SizeHorz() : Image::Arrow();
}

int ColumnList::GetItem(Point p)
{
	int i = p.y / cy + GetColumnItems() * (p.x / GetColumnCx(0));
	if(i < 0 || i >= GetPageItems())
		return -1;
	i += sb;
	return i >= 0 && i < GetCount() ? i : -1;
}

void ColumnList::PointDown(Point p) {
	int i = GetItem(p);
	if(i >= 0 && i < GetCount())
		SetCursor(i);
	else
	if(clickkill)
		KillCursor();
}

void ColumnList::LeftDown(Point p, dword flags) {
	int i = GetDragColumn(p.x);
	if(i >= 0) {
		ci = i;
		dx = p.x - GetColumnCx(0) * (i + 1);
		mpos = p.x;
		SetCapture();
		Refresh(mpos - dx, 0, 1, GetSize().cy);
	}
	else {
		int anchor = cursor;
		SetWantFocus();
		PointDown(p);
		p.y %= cy;
		p.x %= GetColumnCx(0);
		if(cursor >= 0) {
			if(flags & K_SHIFT && anchor >= 0) {
				ShiftSelect(anchor, cursor);
				WhenLeftClickPos(p);
				return;
			}
			else
			if(flags & K_CTRL) {
				if(anchor >= 0 && !IsSelection())
					SelectOne(anchor, true);
				SelectOne(cursor, !IsSelected(cursor));
				WhenLeftClickPos(p);
				return;
			}
		}
		ClearSelection();
		WhenLeftClickPos(p);
	}
}

void ColumnList::RightDown(Point p, dword flags) {
	PointDown(p);
	ClearSelection();
	MenuBar::Execute(WhenBar);
}

void ColumnList::LeftDouble(Point p, dword flags) {
	WhenLeftDouble();
}

void ColumnList::SyncInfoStyle()
{
	if(mi >= 0 && mi < item.GetCount()) {
		Color ink, paper;
		dword style;
		GetItemStyle(mi, ink, paper, style);
		if(info.ink != ink || info.paper != paper || info.style != style) {
			info.ink = ink;
			info.paper = paper;
			info.style = style;
			info.Refresh();
		}
	}
}

void ColumnList::MouseMove(Point p, dword)
{
	if(!HasCapture()) {
		int i = GetItem(p);
		if(i != mi) {
			info.Close();
			mi = -1;
			if(i >= 0) {
				Rect r = GetItemRect(i);
				if(p.x > r.left + 3 && p.x < r.right - 2) {
					mi = i;
					Item& m = item[mi];
					info.value = m.value;
					info.display = m.display ? m.display : display;
					Size sz = info.display->GetStdSize(m.value);
					if(sz.cx > r.Width()) {
						Rect wa = GetWorkArea();
						info.slim = r + GetScreenView().TopLeft();
						r.right = r.left + sz.cx;
						r.Inflate(1, 1);
						r.Offset(GetScreenView().TopLeft());
						r.Offset(min(0, wa.right - r.right), min(0, wa.bottom - r.bottom));
						info.SetRect(r);
						info.ctrl = this;
						SyncInfoStyle();
						info.PopUp(this, true, false, false);
					}
				}
			}
		}
		return;
	}
	info.Close();
	int cx = sb.GetViewSize().cx;
	Refresh(mpos - dx, 0, 1, GetSize().cy);
	mpos = p.x;
	Refresh(mpos - dx, 0, 1, GetSize().cy);
	int ocl = ncl;
	ncl = max(1, cx * (ci + 1) / max(p.x, Draw::GetStdFontCy()));
	if(ncl != ocl)
		Refresh();
	SetSb();
}

Point ColumnList::Info::Op(Point p)
{
	return p + GetScreenView().TopLeft() - ctrl->GetScreenView().TopLeft();
}

void ColumnList::Info::LeftDown(Point p, dword flags)
{
	ctrl->LeftDown(Op(p), flags);
}

void ColumnList::Info::LeftDouble(Point p, dword flags)
{
	ctrl->LeftDouble(Op(p), flags);
}

void ColumnList::Info::RightDown(Point p, dword flags)
{
	ctrl->RightDown(Op(p), flags);
}

void ColumnList::Info::LeftUp(Point p, dword flags)
{
	ctrl->LeftUp(Op(p), flags);
}

void ColumnList::Info::MouseWheel(Point p, int zdelta, dword flags)
{
	ctrl->MouseWheel(Op(p), zdelta, flags);
}

void ColumnList::Info::MouseLeave()
{
	Close();
	ctrl->mi = -1;
}

void ColumnList::Info::MouseMove(Point p, dword flags)
{
	p += GetScreenView().TopLeft();
	if(!slim.Contains(p))
		MouseLeave();
}

void ColumnList::Info::Paint(Draw& w)
{
	display->Paint(w, GetSize(), value, ink, paper, style);
}

ColumnList::Info::Info()
{
	SetFrame(BlackFrame());
}

void ColumnList::CancelMode()
{
	mi = -1;
	info.Close();
}

void ColumnList::MouseLeave()
{
	if(!info.IsOpen())
		mi = -1;
}

void ColumnList::LeftUp(Point p, dword)
{
	if(HasCapture())
		Refresh();
}

Rect ColumnList::GetItemRect(int i) const {
	i -= sb;
	int n = GetColumnItems();
	if(i < 0 || n == 0) return Rect(0, 0, 0, 0);
	int cl = i / n;
	int x = 0;
	for(int q = 0; q < cl; q++)
		x += GetColumnCx(q);
	return RectC(x, i % n * cy, GetColumnCx(cl), cy);
}

void ColumnList::Page(bool down) {
	int q = sb;
	if(down)
		sb.NextPage();
	else
		sb.PrevPage();
	if(q == sb)
		SetCursor(down ? GetCount() - 1 : 0);
	else
		SetCursor(cursor - q + sb);
}

bool ColumnList::Key(dword _key, int count) {
	int c = cursor;
	bool sel = _key & K_SHIFT;
	int key = _key & ~K_SHIFT;
	switch(key) {
	case K_UP:
		if(c < 0)
			c = GetCount() - 1;
		else
		if(c > 0)
			c--;
		break;
	case K_DOWN:
		if(c < 0)
			c = 0;
		else
		if(c < GetCount() - 1)
			c++;
		break;
	case K_LEFT:
		c = max(c - GetColumnItems(), 0);
		break;
	case K_RIGHT:
		c = min(c + GetColumnItems(), GetCount() - 1);
		break;
	case K_PAGEUP:
		Page(false);
		return true;
	case K_PAGEDOWN:
		Page(true);
		return true;
	case K_HOME:
		c = 0;
		break;
	case K_END:
		c = GetCount() - 1;
		break;
	default:
		return MenuBar::Scan(WhenBar, _key);
	}
	int anchor = cursor;
	SetCursor(c);
	if(sel)
		ShiftSelect(anchor, cursor);
	else
		ClearSelection();
	return true;
}

void ColumnList::ClearSelection() {
	bool upd = false;
	for(int i = 0; i < item.GetCount(); i++)
		if(item[i].sel) {
			item[i].sel = false;
			upd = true;
			RefreshItem(i);
		}
	UpdateSelect();
}

void ColumnList::SelectOne(int i, bool sel) {
	if(!multi) ClearSelection();
	if(item[i].canselect)
		item[i].sel = sel;
	UpdateSelect();
	RefreshItem(i);
}

bool ColumnList::IsSelected(int i) const {
	return item[i].sel;
}

void ColumnList::UpdateSelect() {
	isselection = false;
	for(int i = 0; i < item.GetCount(); i++)
		if(item[i].sel) {
			isselection = true;
			break;
		}
	SyncInfoStyle();
	Action();
	WhenSelection();
}

void ColumnList::GetItemStyle(int i, Color& ink, Color& paper, dword& style)
{
	ink = SColorText;
	paper = SColorPaper;
	const Item& m = item[i];
	style = 0;
	if(i == cursor) {
		style = isselection ? Display::CURSOR : Display::CURSOR|Display::SELECT;
		paper = isselection ? Blend(SColorHighlight, SColorFace) : SColorFace;
		if(HasFocus()) {
			style |= Display::FOCUS;
			paper = isselection ? Blend(SColorHighlight, SColorPaper) : SColorHighlight;
			ink = SColorPaper;
		}
	}
	if(m.sel) {
		style |= Display::SELECT;
		paper = SColorShadow;
		if(HasFocus()) {
			paper = SColorPaper;
			ink = SColorText;
		}
	}
}

void ColumnList::Paint(Draw& w) {
	Size sz = GetSize();
	if(GetColumnCx(0) == 0) return;
	int x = 0;
	int i = sb;
	int coli = 0;
	while(x < sz.cx) {
		int cx = GetColumnCx(coli++);
		int y = 0;
		while(y + cy <= sz.cy) {
			Rect rect = RectC(x, y, cx, cy);
			if(i < GetCount()) {
				if(w.IsPainting(rect)) {
					Color ink, paper;
					dword style;
					GetItemStyle(i, ink, paper, style);
					Rect r = rect;
					r.right--;
					const Item& m = item[i];
					(m.display ? m.display : display)->Paint(w, r, m.value, ink, paper, style);
					w.DrawRect(rect.right - 1, rect.top, 1, rect.Height(),
					           x + cx < sz.cx ? SColorDisabled : SColorPaper);
				}
				i++;
			}
			else
				w.DrawRect(rect, SColorPaper);
			y += cy;
		}
		w.DrawRect(x, y, cx, sz.cy - y, SColorPaper);
		x += cx;
	}
	if(HasCapture())
		w.DrawRect(mpos - dx, 0, 1, sz.cy, Blend(SColorHighlight, SColorFace));
	scroller.Set(sb);
}

int  ColumnList::GetPageItems() const {
	return ncl * GetColumnItems();
}

void ColumnList::SetSb() {
	sb.SetTotal(GetCount());
	sb.SetPage(GetPageItems());
}

void ColumnList::Layout() {
	SetSb();
}

void ColumnList::Scroll() {
	Size sz = GetSize();
	sz.cy = sz.cy / cy * cy;
	scroller.Scroll(*this, sz, sb, cy);
	mi = -1;
	info.Close();
}

void ColumnList::RefreshItem(int i)
{
	if(i >= 0)
		Refresh(GetItemRect(i));
}

void ColumnList::SetCursor(int c)
{
	int c0 = cursor;
	c = minmax(c, 0, GetCount() - 1);
	if(c < 0 || cursor < 0)
		Refresh();
	else
		RefreshCursor();
	cursor = c;
	int q = sb;
	sb.ScrollInto(cursor);
	if(q != sb)
		Refresh();
	else
		RefreshCursor();
	if(c0 != cursor)
		if(cursor >= 0)
			WhenEnterItem();
		else
			WhenKillCursor();
	SyncInfoStyle();
	Action();
}

void ColumnList::SetSbPos(int y)
{
	SetSb();
	sb = minmax(y, 0, GetCount() - GetPageItems());
}

void ColumnList::KillCursor()
{
	if(cursor < 0) return;
	Refresh();
	cursor = -1;
	sb.Begin();
	WhenKillCursor();
	Action();
}

void ColumnList::GotFocus()
{
	if(cursor < 0 && GetCount())
		SetCursor(sb);
	Refresh();
	SyncInfoStyle();
}

void ColumnList::LostFocus()
{
	Refresh();
	SyncInfoStyle();
}

int ColumnList::RoundedCy()
{
	Rect r = GetRect();
	Rect rr = r;
	frame->FrameLayout(r);
	return r.Height() / cy * cy + rr.Height() - r.Height();
}

void ColumnList::FrameLayout(Rect& r)
{
	r.bottom = r.top + RoundedCy();
	frame->FrameLayout(r);
}

void ColumnList::FrameAddSize(Size& sz)
{
	frame->FrameAddSize(sz);
}

void ColumnList::FramePaint(Draw& draw, const Rect& r)
{
	int ry = RoundedCy();
	Rect rr = r;
	rr.bottom = ry;
	frame->FramePaint(draw, rr);
	rr = r;
	rr.top = ry;
	draw.DrawRect(rr, SColorFace);
}

ColumnList& ColumnList::RoundSize(bool b)
{
	if(b)
		Ctrl::SetFrame(0, *this);
	else
		Ctrl::SetFrame(0, *frame);
	RefreshLayout();
	Refresh();
	return *this;
}

void ColumnList::SetFrame(CtrlFrame& _frame)
{
	frame = &_frame;
	RefreshLayout();
	Refresh();
}

struct ColumnList::ItemOrder {
	const ValueOrder *order;
	bool operator()(const Item& a, const Item& b) const {
		return (*order)(a.value, b.value);
	}
};

void ColumnList::Sort(const ValueOrder& order)
{
	ItemOrder itemorder;
	itemorder.order = &order;
	KillCursor();
	::Sort(item, itemorder);
	sb.Begin();
}

void ColumnList::Clear() {
	CancelMode();
	KillCursor();
	item.Clear();
	ClearSelection();
	Update();
	Refresh();
}

void ColumnList::Insert(int ii, const Value& val, bool canselect)
{
	Item& m = item.Insert(ii);
	int c = -1;
	if(cursor >= ii) {
		c = cursor + 1;
		KillCursor();
	}
	m.value = val;
	m.sel = false;
	m.canselect = canselect;
	m.display = NULL;
	Refresh();
	SetSb();
	if(c >= 0)
		SetCursor(c);
}

void ColumnList::Insert(int ii, const Value& val, const Display& display, bool canselect)
{
	Insert(ii, val, canselect);
	item[ii].display = &display;
}

void ColumnList::Remove(int ii)
{
	int c = -1;
	if(cursor >= ii) {
		c = max(ii, cursor - 1);
		KillCursor();
	}
	item.Remove(ii);
	Refresh();
	SetSb();
	if(c >= 0)
		SetCursor(c);
}

void ColumnList::Add(const Value& val, bool canselect)
{
	Insert(item.GetCount(), val, canselect);
}

void ColumnList::Add(const Value& val, const Display& display, bool canselect)
{
	Add(val);
	item.Top().display = &display;
}

void ColumnList::Serialize(Stream& s) {
	int version = 0;
	s / version;
	s / ncl;
	Refresh();
}

ColumnList::ColumnList() {
	clickkill = false;
	ncl = 1;
	cy = Draw::GetStdFontCy();
	cursor = -1;
	AddFrame(sb);
	sb.WhenAction = THISBACK(Scroll);
	sb.AutoHide();
	Ctrl::SetFrame(ViewFrame());
	frame = &FieldFrame();
	RoundSize();
	display = &StdDisplay();
	multi = false;
	isselection = false;
	WhenLeftClickPos = THISBACK(StdLeftClickPos);
	mi = -1;
}

ColumnList::~ColumnList() {}
