#include "CtrlLib.h"

NAMESPACE_UPP

#define LTIMING(x)  // TIMING(x)


ToolBar::ToolBar()
{
	pane.Margin(2, 3);
	smargin = 0;
	ssize = 8;
	lock = 0;
	ii = 0;
	SetStyle(StyleDefault());
	buttonminsize = Null;
	maxiconsize = Null;
	kind = ToolButton::NOLABEL;
}

ToolBar::~ToolBar() {}

CH_STYLE(ToolBar, Style, StyleDefault)
{
	buttonstyle = ToolButton::StyleDefault();
	buttonminsize = Size(16, 16);
	maxiconsize = Size(INT_MAX, INT_MAX);
}

Bar::Item& ToolBar::AddItem(Callback cb)
{
	ToolButton& m = item.DoIndex(ii++);
	m.Reset();
	m.WhenAction = cb;
	m.MinSize(IsNull(buttonminsize) ? style->buttonminsize : buttonminsize);
	m.MaxIconSize(IsNull(maxiconsize) ? style->maxiconsize : maxiconsize);
	m.Kind(kind);
	m.SetStyle(style->buttonstyle);
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
	SetTimeCallback(0, PTEBACK1(Set, bar), TIMEID_POST);
}

int ToolBar::GetStdHeight()
{
	return 22 + 3;
}

END_UPP_NAMESPACE
