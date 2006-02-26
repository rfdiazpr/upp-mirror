#include "CtrlLib.h"

DropChoice::DropChoice() {
	drop.SetMonoImage(Ctrl::IsXPStyle() ? CtrlImg::SmallDown() : CtrlImg::smalldown()).NoWantFocus();
	drop.WhenPush = callback(this, &DropChoice::Drop);
	drop.Disable();
	list.Normal();
	list.WhenSelect = callback(this, &DropChoice::Select);
	dropfocus = true;
}

void DropChoice::Drop() {
	if(!owner || owner->IsReadOnly() || list.GetCount() == 0 && !WhenDrop) return;
	WhenDrop();
	if(dropfocus)
		owner->SetFocus();
	if(!list.FindSetCursor(owner->GetData()) && list.GetCount() > 0)
		list.SetCursor(0);
	list.PopUp(owner);
}

void DropChoice::Select() {
	if(!owner || owner->IsReadOnly()) return;
	WhenSelect();
}

Value DropChoice::Get() const {
	if(!owner || owner->IsReadOnly()) return Value();
	int c = list.GetCursor();
	if(c < 0) return Value();
	return list.Get(c, 0);
}

int DropChoice::GetIndex() const
{
	if(!owner || owner->IsReadOnly()) return -1;
	return list.GetCursor();
}

bool DropChoice::DataSelect(Ctrl& owner, DropChoice& drop, const String& appends) {
	Value g = drop.Get();
	if(g.IsVoid()) return false;
	Value s = owner.GetData();
	if(!appends.IsVoid()) {
		String txt = s;
		if(!txt.IsEmpty()) txt.Cat(appends);
		txt.Cat((String)g);
		s = txt;
	}
	else
		s = g;
	owner.SetData(s);
	owner.WhenAction();
	return true;
}

bool DropChoice::DoKey(dword key) {
	if(owner && !owner->IsReadOnly() && list.GetCount()) {
		int q = list.GetCursor();
		switch(key) {
		case K_ALT_DOWN:
			drop.PseudoPush();
			return true;
		case K_DOWN:
			if(appending)
				drop.PseudoPush();
			else {
				list.SetCursor(q <= 0 ? list.GetCount() - 1 : q - 1);
				Select();
			}
			return true;
		case K_UP:
			if(appending)
				drop.PseudoPush();
			else {
				list.SetCursor(q < 0 || q >= list.GetCount() - 1 ? 0 : q + 1);
				Select();
			}
			return true;
		}
	}
	return false;
}

void DropChoice::Add(const Value& s) {
	list.Add(s);
	drop.Enable();
}

void DropChoice::Clear() {
	list.Clear();
	drop.Enable(always_enabled);
}

void DropChoice::Serialize(Stream& s) {
	int version = 0x00;
	int n = list.GetCount();
	s / version / n;
	Value v;
	if(s.IsLoading()) {
		Clear();
		for(int i = 0; i < n; i++) {
			s % v;
			Add(v);
		}
	}
	else
		for(int i = 0; i < n; i++) {
			v = list.Get(i, 0);
			s % v;
		}
}

void DropChoice::AddHistory(const Value& v, int max) {
	if(IsNull(v)) return;
	for(int i = 0; i < list.GetCount(); i++)
		if(list.Get(i, 0) == v) {
			list.Remove(i);
			break;
		}
	list.Insert(0, Vector<Value>() << v);
	if(list.GetCount() > max)
		list.SetCount(max);
	drop.Enable(list.GetCount() || always_enabled);
	list.KillCursor();
}

DropChoice& DropChoice::AlwaysEnabled(bool e)
{
	always_enabled = e;
	drop.Enable(list.GetCount() || always_enabled);
	return *this;
}
