#include "CtrlLib.h"

NAMESPACE_UPP

#define LTIMING(x)  // TIMING(x)

CH_STYLE(ToolBar, Style, StyleDefault)
{
	CtrlsImageLook(look, CtrlsImg::I_TB, 6);
}

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

Bar::Item& ToolButton::Image(const UPP::Image& img)
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
	Ctrl *q = GetParent()->GetParent();
	if(!q || !q->IsTransparent())
		w.DrawRect(sz, checked && !HasMouse() ? Blend(SColorFace, SColorLight) : SColorFace);
	Point center = (sz - isz) / 2;
	ChPaint(w, sz, look[IsEnabled() ? HasMouse() ? GetMouseLeft() ? CTRL_PRESSED
						                                          : checked ? 5 : CTRL_HOT
				                                 : checked ? 4 : CTRL_NORMAL
					                : CTRL_DISABLED]);
	if(IsEnabled())
		DrawHighlightImage(w, center.x, center.y, image, HasMouse());
	else
		w.DrawImage(center.x, center.y, DisabledImage(image));
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
	sz += 6;
	return Size(max(sz.cx, minsize.cx), max(sz.cy, minsize.cy));
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
	look = ToolBar::StyleDefault().look;
	minsize = Size(0, 0);
}

ToolButton::ToolButton()
{
	Reset();
	checked = false;
	paint_checked = false;
	Transparent();
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
	m.SetMinSize(buttonminsize);
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
	SetStyle(StyleDefault());
	buttonminsize = Size(0, 0);
}

ToolBar::~ToolBar() {}

END_UPP_NAMESPACE
