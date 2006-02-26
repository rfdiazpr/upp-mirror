#include "CtrlLib.h"

#define LLOG(x) // LOG(x)

ToolTip::ToolTip()
{
	SetFrame(BlackFrame());
}

Size ToolTip::GetMinSize() const
{
	return AddFrameSize(GetSmartTextSize(ScreenInfo(), text) + 4);
}

void ToolTip::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, SLtYellow);
	DrawSmartText(w, 2, 2, sz.cx - 4, text);
}

void ToolTip::PopUp(Ctrl *owner, Point p, bool effect)
{
	LLOG("ToolTip::PopUp" << Desc(owner) << " @ " << p);
	Rect r = GetWorkArea();
	Size sz = GetMinSize();
	p.x = max(p.x + sz.cx > r.right ? r.right - sz.cx : p.x, 0);
	p.y = max(p.y + sz.cy > r.bottom ? r.bottom - sz.cy : p.y, 0);
	if(IsFlag(EFFECT_SLIDE) && effect)
		SetRect(p.x, p.y, sz.cx, 1);
	else
		SetRect(p.x, p.y, sz.cx, sz.cy);
	Ctrl::PopUp(owner, true, false, true);
	if(effect)
		Animate(*this, p.x, p.y, sz.cx, sz.cy);
}

Ptr<Ctrl> tipctrl;
bool      showmode;

ToolTip& AppToolTip()
{
	static ToolTip tip;
	return tip;
}

void CloseToolTip()
{
	ToolTip& q = AppToolTip();
	if(q.IsOpen())
		q.Close();
}

void EndShowMode()
{
	showmode = false;
}

void ShowToolTip()
{
	LLOG("ShowToolTip");
	CloseToolTip();
	if(tipctrl) {
		String text = tipctrl->GetTip();
		LLOG("-> showing tip: " << text);
		Ctrl *top = tipctrl->GetTopCtrl(); //GetTopWindow();
		if(!text.IsEmpty() && top && top->IsForeground()) {
			LLOG("-> foreground");
			ToolTip& q = AppToolTip();
			q.Set(text);
			q.PopUp(top, GetMousePos() + Size(0, 22), !showmode);
			showmode = true;
			KillTimeCallback((void *)EndShowMode);
			return;
		}
		LLOG("-> background / empty text, top = " << ::Name(top));
	}
	SetTimeCallback(200, callback(EndShowMode), (void *)EndShowMode);
}

bool ToolTipHook(Ctrl *ctrl, bool inframe, int event, Point p, int zdelta, dword keyflags)
{
	if(event == Ctrl::MOUSEMOVE && ctrl != &AppToolTip())
	{
		ctrl = Ctrl::GetVisibleChild(ctrl, p, inframe);
		if(ctrl != tipctrl) {
			LLOG("ToolTipHook / ctrl change -> " << ::Name(ctrl));
			tipctrl = ctrl;
			KillTimeCallback((void *)ToolTipHook);
			if(showmode)
				ShowToolTip();
			else {
				LLOG("ToolTipHook -> SetTimeCallback");
				SetTimeCallback(1000, callback(ShowToolTip), (void *)ToolTipHook);
			}
		}
	}
	if(event == Ctrl::MOUSELEAVE) {
		CloseToolTip();
		SetTimeCallback(1000, callback(EndShowMode), (void *)EndShowMode);
	}
	if((event & Ctrl::ACTION) == Ctrl::DOWN) {
		CloseToolTip();
		EndShowMode();
	}
	return false;
}

bool ToolTipStateHook(Ctrl *ctrl, int reason)
{
	if((reason == Ctrl::CLOSE || reason == Ctrl::ENABLE) && ctrl != &AppToolTip()) {
		LLOG("ToolTipStateHook -> close");
		CloseToolTip();
		EndShowMode();
	}
	return false;
}

INITBLOCK {
	LLOG("Ctrl::InstallMouseHook(ToolTipHook)");
	Ctrl::InstallMouseHook(ToolTipHook);
	Ctrl::InstallStateHook(ToolTipStateHook);
}

void LinkToolTipIn__() {}  // "Call" from elsewhere to force linking this module in...

class QTFPopUp : public RichTextCtrl {
public:
	virtual void Deactivate();
	virtual void CancelMode();

protected:
	int  width;
	int  maxheight;
	bool open;

	void DoClose();

public:
	void      PopUp(Ctrl *parent);

	QTFPopUp& Width(int _width)                   { width = _width; return *this; }
	QTFPopUp& MaxHeight(int _maxheight)           { maxheight = _maxheight; return *this; }

	QTFPopUp();
};

void QTFPopUp::Deactivate() {
	if(open)
		DoClose();
}

void QTFPopUp::CancelMode() {
	DoClose();
}

void QTFPopUp::DoClose() {
	if(open) {
		open = false;
		Close();
	}
}

void QTFPopUp::PopUp(Ctrl *parent) {
	Close();
	Rect r = Rect(0, 0, width, maxheight);
	GetFrame().FrameLayout(r);
	int cy = min(maxheight, GetHeight(r.Width()) + maxheight - r.Height());
	Rect area = GetWorkArea();
	Point p = GetMousePos();
	r.top = max(area.top, p.y + 16);
	r.bottom = r.top + cy;
	if(r.bottom > area.bottom) {
		r.bottom = area.bottom;
		r.top = r.bottom - cy;
	}
	r.left = max(area.left, p.x - width / 2);
	r.right = r.left + width;
	if(r.right > area.right) {
		r.right = area.right;
		r.left = r.right - width;
	}
	open = false;
	SetRect(r);
	Ctrl::PopUp(parent);
	SetFocus();
	open = true;
}

QTFPopUp::QTFPopUp() {
	static ColorF helpborder[] = {
		(ColorF)2,
		&SBlack, &SBlack, &SBlack, &SBlack,
		&SLtGray, &SLtGray, &SLtGray, &SLtGray,
	};
	static BorderFrame helpframe(helpborder);
	width = 250;
	maxheight = 150;
	Margins(5).Background(LtYellow).SetFrame(helpframe);
	open = false;
}

void PerformDescription()
{
	Ctrl *top = Ctrl::GetActiveWindow();
	if(!top) return;
	Vector<Image> vi;
	vi.Add(CtrlImg::HelpCursor0());
	vi.Add(CtrlImg::HelpCursor1());
	if(PointLoop(*top, vi, 300)) {
		Point p = GetMousePos();
		Ctrl *ctrl = Ctrl::GetVisibleChild(top, p - top->GetRect().TopLeft(), true);
		while(ctrl) {
			String desc = ctrl->GetDescription();
			if(!IsEmpty(desc)) {
				static QTFPopUp qtf;
				qtf.SetQTF(desc);
				qtf.PopUp(top);
				return;
			}
			ctrl = ctrl->GetParent();
		}
	}
}
