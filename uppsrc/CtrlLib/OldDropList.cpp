#include "CtrlLib.h"

#if OLDDROPLIST
NAMESPACE_UPP

#define LLOG(x) // LOG(x)

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
	Color bg = notnull && i < 0 ? Blend(SColorPaper, Color(255, 0, 0), 32) :
	           f ? SColorHighlight
	             : push ? Blend(SColorHighlight, SColorFace, 235)
	                    : IsShowEnabled() && !IsReadOnly() ? SColorPaper
	                                                       : SColorFace;
	w.DrawRect(cr, bg);
	cr.top = (sz.cy - list.GetLineCy()) / 2;
	cr.bottom = cr.top + list.GetLineCy();
	int m = list.HeaderTab(0).GetMargin();
	cr.left += m;
	cr.right -= m;
	if(cr.left < cr.right) {
		w.Clip(cr);
		d.Paint(w, cr, v, f ? SColorHighlightText : IsShowEnabled() ? SColorText : SColorDisabled,
		        bg, f ? Display::CURSOR : Display::FOCUS|Display::CURSOR);
		w.End();
	}
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
			if(list.GetCursor() >= 0 && list.GetCursor() < key.GetCount() && key[list.GetCursor()] != value)
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
	RefreshFrame();
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
	return notnull && IsNull(value) ? NotNullError() : value;
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
	dropfocus = true;
	push = false;
	notnull = false;
}

DropList::~DropList() {}

END_UPP_NAMESPACE
#endif
