#include "CtrlLib.h"

#define LTIMING(x)  // TIMING(x)

void ToolButton::UpdateTip()
{
	LTIMING("UpdateTip");
	String s = tiptext;
	if(accel)
		s << " (" << GetKeyDesc(accel) << ')';
	Ctrl::Tip(s);
}

Bar::Item& ToolButton::Text(const char *text)
{
	ExtractAccessKey(text, tiptext);
	UpdateTip();
	return *this;
}

Bar::Item& ToolButton::Check(bool check)
{
	checked = check;
	return *this;
}

Bar::Item& ToolButton::Radio(bool check)
{
	return Check(check);
}

Bar::Item& ToolButton::Key(dword key)
{
	if(key) {
		accel = key;
		UpdateTip();
	}
	return *this;
}

Bar::Item& ToolButton::Repeat(bool r)
{
	repeat = r;
	return *this;
}

Bar::Item& ToolButton::Image(const ::Image& img)
{
	if(!img.IsSame(image)) {
		image = img;
		Refresh();
	}
	return *this;
}

Bar::Item& ToolButton::Enable(bool enable)
{
	Ctrl::Enable(enable);
	return *this;
}

Bar::Item& ToolButton::Tip(const char *tip)
{
	tiptext = tip;
	UpdateTip();
	return *this;
}

Bar::Item& ToolButton::Help(const char *help)
{
	HelpLine(help);
	return *this;
}

Bar::Item& ToolButton::Topic(const char *help)
{
	HelpTopic(help);
	return *this;
}

Bar::Item& ToolButton::Description(const char *desc)
{
	Ctrl::Description(desc);
	return *this;
}

void  ToolButton::Paint(Draw& w)
{
	paint_checked = checked;
	Size sz = GetSize();
	Size isz = image.GetSize();
	w.DrawRect(sz, checked && !HasMouse() ? SWhiteGray : SColorFace);
	Point center = (sz - isz) / 2;
	if(IsEnabled()) {
		bool push = HasMouse() && GetMouseLeft();
		if(IsXPStyle()) {
			if(push)
				DrawXPButton(w, sz, BUTTON_PUSH|BUTTON_TOOL);
			else
			if(HasMouse())
				DrawXPButton(w, sz, BUTTON_NORMAL|BUTTON_TOOL);
			else
			if(checked)
				DrawXPButton(w, sz, BUTTON_TOOL|BUTTON_CHECKED);
			else
				w.DrawRect(sz, SColorFace);
			DrawHighlightImage(w, center.x, center.y, image, HasMouse());
		}
		else {
			DrawHighlightImage(w, center.x + push, center.y + push, image, HasMouse());
			if(checked || HasMouse() && GetMouseLeft())
				DrawFrame(w, sz, SGray, SWhite);
			else
			if(HasMouse())
				DrawFrame(w, sz, SWhite, SGray);
		}
	}
	else
		w.DrawImage(center.x, center.y, MakeImage(image, "etched"));
}

void  ToolButton::MouseEnter(Point, dword)
{
	BarCtrl::SendHelpLine(this);
	Refresh();
}

void  ToolButton::MouseLeave()
{
	BarCtrl::ClearHelpLine(this);
	Refresh();
}

Size  ToolButton::GetMinSize() const
{
	Size sz = image.GetSize();
	if(sz.IsEmpty())
		sz = Size(16, 16);
	return sz + 6;
}

void  ToolButton::LeftDown(Point, dword)
{
	Refresh();
	if(repeat)
		WhenAction();
}

void  ToolButton::LeftRepeat(Point, dword)
{
	Refresh();
	if(repeat)
		WhenAction();
}

void  ToolButton::LeftUp(Point, dword)
{
	Refresh();
	if(!repeat)
		WhenAction();
}

bool  ToolButton::HotKey(dword key)
{
	if(key == accel) {
		WhenAction();
		return true;
	}
	return false;
}

void ToolButton::FinalSync()
{
	if(checked != paint_checked)
		Refresh();
}

void ToolButton::Reset()
{
	Tip("");
	Help("");
	Topic("");
	Description("");
	repeat = false;
	accel = 0;
	checked = false;
	NoWantFocus();
}

ToolButton::ToolButton()
{
	Reset();
	checked = false;
	paint_checked = false;
}

String ToolButton::GetDesc() const
{
	return tiptext;
}

ToolButton::~ToolButton() {}

// ------------------------------

Bar::Item& ToolBar::AddItem(Callback cb)
{
	ToolButton& m = item.DoIndex(ii++);
	m.Reset();
	m.WhenAction = cb;
	pane.Add(&m, Null);
	return m;
}

Bar::Item& ToolBar::AddSubMenu(Callback1<Bar&> proc)
{
	if(item.GetCount())
		Separator();
	proc(*this);
	return NilItem();
}

void ToolBar::Clear()
{
	item.Clear();
	BarCtrl::Clear();
}

bool ToolBar::HotKey(dword key)
{
	return Bar::Scan(proc, key);
}

void ToolBar::Set(Callback1<Bar&> bar)
{
	LTIMING("SetToolBar");
	if(lock) return;
	lock++;
	BarCtrl::IClear();
	ii = 0;
	bar(*this);
	proc = bar;
	item.SetCount(ii);
	for(int i = 0; i < item.GetCount(); i++)
		item[i].FinalSync();
	BarCtrl::IFinish();
	SyncBar();
	lock--;
}

void ToolBar::Post(Callback1<Bar&> bar)
{
	KillTimeCallback(TIMEID_POST);
	SetTimeCallback(0, THISBACK1(Set, bar), TIMEID_POST);
}

int ToolBar::GetStdHeight()
{
	return 22 + 3;
}

ToolBar::ToolBar()
{
	pane.Margin(2, 3);
	smargin = 0;
	ssize = 8;
	lock = 0;
	ii = 0;
}

ToolBar::~ToolBar() {}
