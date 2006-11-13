#include "CtrlLib.h"

#define LLOG(x) // LOG(x)

void PopUpTable::CancelMode() {
	if(open) {
		DoClose();
		WhenCancel();
	}
}

void PopUpTable::DoClose() {
	open = false;
	Close();
}

void PopUpTable::Deactivate() {
	if(open) {
		DoClose();
		IgnoreMouseClick();
		WhenCancel();
	}
}

void PopUpTable::LeftUp(Point p, dword keyflags) {
	ArrayCtrl::LeftUp(p, keyflags);
	DoClose();
	WhenSelect();
}

bool PopUpTable::Key(dword key, int n) {
	switch(key) {
	case K_ENTER:
	case K_ALT_DOWN:
		DoClose();
		WhenSelect();
		return true;
	case K_ESCAPE:
		DoClose();
		WhenCancel();
		return true;
	case K_UP:
		if(!IsCursor()) {
			SetCursor(GetCount() - 1);
			return true;
		}
		break;
	case K_DOWN:
		if(!IsCursor()) {
			SetCursor(0);
			return true;
		}
		break;
	}
	return ArrayCtrl::Key(key, n);
}

void PopUpTable::PopUp(Ctrl *owner, int x, int top, int bottom, int width) {
	DoClose();
	int h = min(droplines, GetCount()) * GetLineCy() + 2;
	Rect rt = RectC(x, bottom, width, h);
	Rect area = Ctrl::GetWorkArea();
	bool up = false;
	if(rt.bottom > area.bottom) {
		up = true;
		rt.top = top - h;
		rt.bottom = rt.top + h;
	}
	open = false;
	Ctrl popup;
	if(GUI_PopUpEffect()) {
		if(up) {
			popup.SetRect(Rect(rt.left, rt.bottom - 1, rt.right, rt.bottom));
			popup.Add(TopPos(0, rt.Height()).LeftPos(0, rt.Width()));
		}
		else {
			popup.SetRect(Rect(rt.left, rt.top, rt.right, rt.top + 1));
			popup.Add(BottomPos(0, rt.Height()).LeftPos(0, rt.Width()));
		}
		CenterCursor();
		popup.PopUp(owner, true, true, GUI_GlobalStyle() >= GUISTYLE_XP);
		SetFocus();
		Ctrl::ProcessEvents();
		Animate(popup, rt, GUIEFFECT_SLIDE);
		Ctrl::Remove();
	}
	CenterCursor();
	SetRect(rt);
	Ctrl::PopUp(owner, true, true, GUI_GlobalStyle() >= GUISTYLE_XP);
	SetFocus();
	open = true;
}

void PopUpTable::PopUp(Ctrl *owner, int width)
{
	Rect r = owner->GetScreenRect();
	r.right = r.left + width;
	PopUp(owner, r.left, r.top, r.bottom, width);
}

void PopUpTable::PopUp(Ctrl *owner)
{
	Rect r = owner->GetScreenRect();
	PopUp(owner, r.left, r.top, r.bottom, r.Width());
}

void PopUpTable::Normal()
{
	AddColumn().Accel();
	NoHeader();
	HeaderTab(0).SetMargin(0);
	MouseMoveCursor();
	NoGrid();
	AutoHideSb();
	SetLineCy(Draw::GetStdFontCy());
}

PopUpTable::PopUpTable() {
	SetFrame(BlackFrame());
	droplines = 16;
	open = false;
}

PopUpTable::~PopUpTable() {}

Size DropList::GetMinSize() const
{
	return AddFrameSize(0, StdFont().Info().GetHeight() + 4);
}

void DropList::GotFocus() {
	Refresh();
}

void DropList::LostFocus() {
	Refresh();
}

void DropList::Paint(Draw& w) {
	LLOG("DropList::Paint");
	Value v;
	if(displayall)
		v = value;
	int i = key.Find(value);
	const Display& d = valuedisplay ? *valuedisplay : i >= 0 ? list.GetDisplay(i, 0)
	                                                         : list.GetDisplay(0);
	if(i >= 0) v = list.Get(i, 0);
	v = valueconvert->Format(v);
	Size sz = GetSize();
	bool f = HasFocus() && !push;
	Rect r = Rect(sz);
	Rect cr;
	cr.left = cr.top = 1;
	cr.right = sz.cx - 1;
	cr.bottom = sz.cy - 1;
	DrawRectMinusRect(w, r, cr, SColorPaper);
	w.DrawRect(cr, f ? SColorHighlight
	                 : push ? Blend(SColorHighlight, SColorFace, 235)
	                        : IsShowEnabled() && !IsReadOnly() ? SColorPaper
	                                                           : SColorFace);
	cr.top = (sz.cy - list.GetLineCy()) / 2;
	cr.bottom = cr.top + list.GetLineCy();
	int m = list.HeaderTab(0).GetMargin();
	cr.left += m;
	cr.right -= m;
	if(cr.left < cr.right)
		d.Paint(w, cr, v, f ? SColorHighlightText : IsShowEnabled() ? SColorText : SColorDisabled,
		        Null, f ? Display::CURSOR : Display::FOCUS|Display::CURSOR);
}

void DropList::LeftDown(Point p, dword keyflags) {
	if(WhenClick) {
		if(Ctrl::ClickFocus()) SetWantFocus();
		if(IsReadOnly()) return;
		push = true;
		Refresh();
	}
	else {
		if(IsReadOnly()) return;
		Push();
	}
}

void DropList::MouseMove(Point, dword style)
{
	if(IsReadOnly() || !WhenClick) return;
	bool b = style & K_MOUSELEFT;
	if(b != push) {
		push = b;
		Refresh();
	}
}

void DropList::MouseLeave() {
	if(!WhenClick)
		return;
	push = false;
	Refresh();
}

void DropList::LeftUp(Point p, dword) {
	push = false;
	if(Rect(GetSize()).Contains(p))
		WhenClick();
	Refresh();
}

void DropList::Change(int q) {
	if(key.GetCount() == 0) return;
	int i = key.Find(value);
	i += q;
	if(i >= key.GetCount()) i = key.GetCount() - 1;
	if(i < 0) i = 0;
	if(value != key[i]) {
		value = key[i];
		UpdateActionRefresh();
	}
}

bool DropList::Key(dword k, int) {
	if(IsReadOnly()) return false;
	switch(k) {
	case K_ALT_DOWN:
		Push();
		break;
	case K_DOWN:
	case K_RIGHT:
		Change(1);
		break;
	case K_UP:
	case K_LEFT:
		Change(-1);
		break;
	default:
		if(k >= 32 && k < 256) {
			bool b = list.Key(k, 1);
			if(list.GetCursor() >= 0 && list.GetCount() < key.GetCount() && key[list.GetCursor()] != value)
				Select();
			return b;
		}
		return false;
	}
	return true;
}

void DropList::MouseWheel(Point, int zdelta, dword)
{
	Change(zdelta < 0 ? 1 : -1);
}

void DropList::Drop() {
	if(IsReadOnly()) return;
	if(dropfocus)
		SetFocus();
	WhenDrop();
	list.SetCursor(key.Find(value));
	list.PopUp(this);
}

void DropList::Select() {
	int c = list.GetCursor();
	if(c >= 0)
		value = key[c];
	if(dropfocus)
		SetFocus();
	UpdateActionRefresh();
}

void DropList::Cancel() {
	if(dropfocus)
		SetFocus();
}

void DropList::ClearList() {
	key.Clear();
	list.Clear();
	Refresh();
	list.Refresh();
	EnableDrop(false);
}

void DropList::Clear() {
	ClearList();
	value = Value();
	Update();
}

DropList& DropList::Add(const Value& _key, const Value& text) {
	key.Add(_key);
	list.Add(text);
	Refresh();
	list.Refresh();
	EnableDrop();
	return *this;
}

void DropList::Trim(int n) {
	if(GetIndex() >= n)
		SetIndex(0);
	key.Trim(n);
	list.SetCount(n);
	Refresh();
	list.Refresh();
	EnableDrop(n);
}

void DropList::SetData(const Value& data) {
	if(value != data) {
		value = data;
		UpdateRefresh();
	}
}

Value DropList::GetData() const {
	return value;
}

Value DropList::GetValue() const {
	int i = FindKey(value);
	return i < 0 ? Null : GetValue(i);
}

void  DropList::SetValue(int i, const Value& v) {
	list.Set(i, 0, v);
	EnableDrop();
}

void  DropList::SetValue(const Value& v) {
	int i = FindKey(value);
	if(i >= 0)
		SetValue(i, v);
}

DropList& DropList::SetConvert(const Convert& cv)
{
	list.ColumnAt(0).SetConvert(cv);
	valueconvert = &cv;
	Refresh();
	return *this;
}

DropList& DropList::SetDisplay(int i, const Display& d)
{
	list.SetDisplay(i, 0, d);
	Refresh();
	return *this;
}

DropList& DropList::SetDisplay(const Display& d)
{
	list.ColumnAt(0).SetDisplay(d);
	Refresh();
	return *this;
}

DropList& DropList::SetDisplay(const Display& d, int lcy)
{
	SetDisplay(d);
	SetLineCy(lcy);
	Refresh();
	return *this;
}

DropList& DropList::ValueDisplay(const Display& d)
{
	valuedisplay = &d;
	Refresh();
	return *this;
}

void  DropList::Adjust()
{
	int i = FindKey(value);
	if(i < 0 && GetCount())
		SetIndex(0);
}

void DropList::Adjust(const Value& k)
{
	int i = FindKey(value);
	if(i < 0 && GetCount())
		SetData(k);
}

Value DropList::Format(const Value& q) const {
	int i = FindKey(q);
	return valueconvert->Format(i < 0 ? Null : GetValue(i));
}

DropList::DropList() {
	WhenPush = callback(this, &DropList::Drop);
	list.Normal();
	list.WhenSelect = callback(this, &DropList::Select);
	list.WhenCancel = callback(this, &DropList::Cancel);
	EnableDrop(false);
	displayall = false;
	valueconvert = &NoConvert();
	valuedisplay = NULL;
	dropfocus = false;
	push = false;
}

DropList::~DropList() {}

void Add(DropList& list, const VectorMap<Value, Value>& values)
{
	for(int i = 0; i < values.GetCount(); i++)
		list.Add(values.GetKey(i), values[i]);
}

void Add(MapConvert& convert, const VectorMap<Value, Value>& values)
{
	for(int i = 0; i < values.GetCount(); i++)
		convert.Add(values.GetKey(i), values[i]);
}

void Add(DropList& list, const MapConvert& convert)
{
	for(int i = 0; i < convert.GetCount(); i++)
		list.Add(convert.GetKey(i), convert.GetValue(i));
}
