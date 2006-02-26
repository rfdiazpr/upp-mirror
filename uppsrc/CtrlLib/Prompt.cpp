#include "CtrlLib.h"

struct PromptDlgWnd__ : TopWindow {
	virtual bool HotKey(dword key) {
		if(TopWindow::HotKey(key))
			return true;
		if(IsAlpha(key))
			return TopWindow::HotKey(K_ALT_A + ToUpper((int)key) - 'A');
		return false;
	}
};

int Prompt(const char *title, const Image& iconbmp, const char *qtf, bool okcancel,
           const char *button1, const char *button2, const char *button3,
		   int cx)
{
	int fcy = Draw::GetStdFontCy();
	PromptDlgWnd__ dlg;
	RichTextCtrl qtfctrl;
	Icon         icon;
	icon.SetImage(iconbmp);
	icon.TopPos(fcy, Ctrl::STDSIZE).LeftPos(fcy, Ctrl::STDSIZE);
	Button b1, b2, b3;
	qtfctrl.SetQTF(qtf, GetRichTextStdScreenZoom());
	int bcy = 2 * fcy;
	int bcx = 6 * fcy;
	Size bsz = icon.GetStdSize();
	if(cx == 0) {
		cx = qtfctrl.GetWidth();
		if(!cx)
			cx = 350;
		cx += 2 * fcy;
		if(bsz.cx)
			cx += bsz.cx + fcy;
	}
	int nbtn = !!button1 + !!button2 + !!button3;
	cx = min(550, max(nbtn * bcx + (1 + nbtn) * fcy, cx));
	int qcx = cx - 2 * fcy;
	if(bsz.cx)
		qcx -= bsz.cx + fcy;
	int ccy = qtfctrl.GetHeight(qcx);
	int qcy = min(400, ccy);
	if(qcy <= ccy) {
		qcx += ScrollBar::GetStdBox() + fcy;
		cx += ScrollBar::GetStdBox();
	}
	int mcy = max(qcy, bsz.cy);
	int cy = mcy + 4 * fcy;
	dlg.SetRect(Size(cx, cy));
	fcy /= 2;
	dlg << icon.TopPos(fcy + (mcy - bsz.cy) / 2, bsz.cy).LeftPos(fcy, bsz.cx);
	dlg << qtfctrl.TopPos(fcy + (mcy - qcy) / 2, qcy).LeftPos(2 * fcy + bsz.cx, qcx);
	if(okcancel)
		b1.Ok();
	b1.SetLabel(button1);
	b1.WhenAction = dlg.Breaker(1);
	b2.WhenAction = dlg.Breaker(0);
	b3.WhenAction = dlg.Breaker(-1);
	b1.SetFont(Arial(-12));
	b2.SetFont(Arial(-12));
	b3.SetFont(Arial(-12));
	if(button2) {
		b2.SetLabel(button2);
		if(button3) {
			dlg << b2.BottomPos(fcy, bcy).HCenterPos(bcx);
			dlg << b3.SetLabel(button3).BottomPos(fcy, bcy).RightPos(fcy, bcx);
			if(okcancel)
				b3.Cancel();
		}
		else {
			dlg << b2.BottomPos(fcy, bcy).RightPos(fcy, bcx);
			if(okcancel)
				b2.Cancel();
		}
		dlg << b1.BottomPos(fcy, bcy).LeftPos(fcy, bcx);
	}
	else
		dlg << b1.BottomPos(fcy, bcy).HCenterPos(bcx);
	dlg.WhenClose = dlg.Breaker(button3 ? -1 : 0);
	dlg.Open();
	dlg.Title(title);
	return dlg.RunAppModal();
}

int Prompt(const char *title, const Image& iconbmp, const char *qtf,
           const char *button1, const char *button2, const char *button3,
		   int cx)
{
	return Prompt(title, iconbmp, qtf, true, button1, button2, button3, cx);
}

void PromptOK(const char *qtf) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONINFORMATION);
#endif
	Prompt(Ctrl::GetAppName(), CtrlImg::information(), qtf, t_("OK"));
}

void Exclamation(const char *qtf) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONEXCLAMATION);
#endif
	Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(), qtf, t_("OK"));
}

void ShowExc(const Exc& exc) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONEXCLAMATION);
#endif
	Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(), DeQtf(exc), t_("OK"));
}

int PromptOKCancel(const char *qtf) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONQUESTION);
#endif
	return Prompt(Ctrl::GetAppName(), CtrlImg::question(), qtf, t_("OK"), t_("Cancel"));
}

int PromptYesNo(const char *qtf) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONQUESTION);
#endif
	return Prompt(Ctrl::GetAppName(), CtrlImg::question(), qtf, false, t_("&Yes"), t_("&No"));
}

int PromptYesNoCancel(const char *qtf) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONQUESTION);
#endif
	return Prompt(Ctrl::GetAppName(), CtrlImg::question(), qtf, true, t_("&Yes"), t_("&No"), t_("Cancel"));
}

int PromptAbortRetry(const char *qtf) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONEXCLAMATION);
#endif
	return Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(), qtf, false, t_("&Abort"), t_("&Retry"));
}

int PromptRetryCancel(const char *qtf) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONEXCLAMATION);
#endif
	return Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(), qtf, true, t_("&Retry"), t_("Cancel"));
}

int PromptAbortRetryIgnore(const char *qtf) {
#ifdef PLATFORM_WIN32
	MessageBeep(MB_ICONEXCLAMATION);
#endif
	return Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(), qtf,
		          false, t_("&Abort"), t_("&Retry"), t_("&Cancel"));
}
