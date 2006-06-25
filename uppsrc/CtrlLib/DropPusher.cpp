#include "CtrlLib.h"

DropPusher::DropPusher()
{
	NoWantFocus();
	SetConvert(*this);
	drop.SetMonoImage(CtrlsImg::DA()).NoWantFocus();
	AddFrame(drop);
	drop.WhenPush = THISBACK(OnDrop);
	popup.Normal();
	popup.WhenSelect = THISBACK(OnSelect);
}

DropPusher::~DropPusher() {}

Value DropPusher::Format(const Value& v) const
{
	int i = FindIndex(keys, v);
	if(i >= 0)
		return popup.Get(i, 0);
	return v;
}

void DropPusher::Clear()
{
	keys.Clear();
	popup.Clear();
}

void DropPusher::Add(Value value, Value display)
{
	keys.Add(value);
	popup.Add(display);
}

Value DropPusher::GetValue() const
{
	int i = FindIndex(keys, GetData());
	return i < 0 ? Null : popup.Get(i, 0);
}

void DropPusher::DoAction()
{
	WhenClick();
	DataPusher::DoAction();
}

void DropPusher::OnDrop()
{
	WhenDrop();
	popup.PopUp(this);
	int i = FindIndex(keys, GetData());
	if(i >= 0)
		popup.SetCursor(i);
}

void DropPusher::OnSelect()
{
	int c = popup.GetCursor();
	if(c >= 0)
		SetDataAction(keys[c]);
}
