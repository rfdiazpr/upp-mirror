#include "CtrlLib.h"

NAMESPACE_UPP

void DropList::Sync() {
	Value v;
	if(displayall)
		v = value;
	int i = key.Find(value);
	const Display& d = valuedisplay ? *valuedisplay : i >= 0 ? list.GetDisplay(i, 0)
	                                                         : list.GetDisplay(0);
	if(i >= 0) v = list.Get(i, 0);
	MultiButton::SetDisplay(d);
	MultiButton::SetValueCy(list.GetLineCy());
	v = valueconvert->Format(v);
	Set(v);
}

void DropList::Change(int q) {
	if(key.GetCount() == 0) return;
	int i = key.Find(value);
	i += q;
	if(i >= key.GetCount()) i = key.GetCount() - 1;
	if(i < 0) i = 0;
	if(value != key[i]) {
		value = key[i];
		Sync();
		UpdateAction();
	}
}

bool DropList::Key(dword k, int) {
	if(IsReadOnly()) return false;
	switch(k) {
	case K_ALT_DOWN:
		Drop();
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
	Sync();
	UpdateAction();
}

void DropList::Cancel() {
	if(dropfocus)
		SetFocus();
	Sync();
}

void DropList::Remove(int i)
{
	key.Remove(i);
	list.Remove(i);
	Adjust();
}

void DropList::ClearList() {
	key.Clear();
	list.Clear();
	Sync();
	list.Refresh();
	EnableDrop(false);
}

void DropList::Clear() {
	ClearList();
	value = Value();
	Sync();
	Update();
}

DropList& DropList::Add(const Value& _key, const Value& text)
{
	key.Add(_key);
	list.Add(text);
	list.Refresh();
	EnableDrop();
	Sync();
	return *this;
}

DropList& DropList::AddSeparator()
{
	key.Add(Null);
	list.AddSeparator();
	list.Refresh();
	EnableDrop();
	Sync();
	return *this;
}

void DropList::Trim(int n) {
	if(GetIndex() >= n)
		SetIndex(0);
	key.Trim(n);
	list.SetCount(n);
	Sync();
	list.Refresh();
	EnableDrop(n);
}

void DropList::SetData(const Value& data) {
	if(value != data) {
		value = data;
		Update();
		Sync();
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
	Sync();
}

void  DropList::SetValue(const Value& v) {
	int i = FindKey(value);
	if(i >= 0)
		SetValue(i, v);
	Sync();
}

DropList& DropList::SetConvert(const Convert& cv)
{
	list.ColumnAt(0).SetConvert(cv);
	valueconvert = &cv;
	Sync();
	return *this;
}

DropList& DropList::SetDisplay(int i, const Display& d)
{
	list.SetDisplay(i, 0, d);
	Sync();
	return *this;
}

DropList& DropList::SetDisplay(const Display& d)
{
	list.ColumnAt(0).SetDisplay(d);
	Sync();
	return *this;
}

DropList& DropList::SetDisplay(const Display& d, int lcy)
{
	SetDisplay(d);
	SetLineCy(lcy);
	Sync();
	return *this;
}

DropList& DropList::ValueDisplay(const Display& d)
{
	valuedisplay = &d;
	Sync();
	return *this;
}

void  DropList::Adjust()
{
	int i = FindKey(value);
	if(i < 0)
		if(GetCount())
			SetIndex(0);
		else
			SetData(Null);
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

DropList& DropList::AlwaysDrop(bool e)
{
	alwaysdrop = e;
	if(e)
		EnableDrop();
	return *this;
}

DropList::DropList()
{
	list.Normal();
	list.WhenSelect = THISBACK(Select);
	list.WhenCancel = THISBACK(Cancel);
	displayall = false;
	valueconvert = &NoConvert();
	valuedisplay = NULL;
	dropfocus = false;
	notnull = false;
	alwaysdrop = false;
	NoInitFocus();
	EnableDrop(false);
	AddButton().Main().WhenPush = THISBACK(Drop);
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

END_UPP_NAMESPACE
