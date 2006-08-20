#include "CtrlCore.h"

#define LLOG(x) // RLOG(x)

#ifdef PLATFORM_WIN32

void    TopWindow::SyncSizeHints() {}

LRESULT TopWindow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = GetHWND();
	bool inloop;
	switch(message) {
	case WM_QUERYENDSESSION:
		inloop = InLoop();
		WhenClose();
		return inloop ? !InLoop() : !IsOpen();
	case WM_ENDSESSION:
		EndSession() = true;
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		IgnoreMouseUp();
		WhenClose();
		return 0;
	case WM_WINDOWPOSCHANGED:
		if(IsIconic(hwnd))
			state = MINIMIZED;
		else
		if(IsZoomed(hwnd))
			state = MAXIMIZED;
		else {
			state = OVERLAPPED;
			overlapped = GetScreenClient(hwnd);
		}
		Layout();
		break;
	}
	return Ctrl::WindowProc(message, wParam, lParam);
}

void TopWindow::SyncTitle()
{
	HWND hwnd = GetHWND();
	if(hwnd)
		if(IsWindowUnicode(hwnd)) { // TRC 04/10/17: ActiveX Unicode patch
//			ASSERT(IsWindowUnicode(hwnd));
			::SetWindowTextW(hwnd, (const WCHAR*)~title);
		}
		else
			::SetWindowText(hwnd, ToSystemCharset(title.ToString()));
}

void TopWindow::DeleteIco()
{
	if(ico)
		DestroyIcon(ico);
	if(lico)
		DestroyIcon(lico);
	ico = lico = NULL;
}

void TopWindow::SyncCaption()
{
	HWND hwnd = GetHWND();
	if(hwnd) {
		style = ::GetWindowLong(hwnd, GWL_STYLE);
		exstyle = ::GetWindowLong(hwnd, GWL_EXSTYLE);
	}
	style &= ~(WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_SYSMENU|WS_POPUP|WS_DLGFRAME);
	exstyle &= ~(WS_EX_TOOLWINDOW|WS_EX_DLGMODALFRAME);
	style |= WS_CAPTION;
	if(minimizebox)
		style |= WS_MINIMIZEBOX;
	if(maximizebox)
		style |= WS_MAXIMIZEBOX;
	if(sizeable)
		style |= WS_THICKFRAME;
	if(IsNull(icon) && !maximizebox && !minimizebox) {
		style |= WS_POPUPWINDOW|WS_DLGFRAME;
		exstyle |= WS_EX_DLGMODALFRAME;
	}
	else
		style |= WS_SYSMENU;
	if(tool)
		exstyle |= WS_EX_TOOLWINDOW;
	if(hwnd) {
		::SetWindowLong(hwnd, GWL_STYLE, style);
		::SetWindowLong(hwnd, GWL_EXSTYLE, exstyle);
		SyncTitle();
	}
	DeleteIco();
	::SendMessage(hwnd, WM_SETICON, false, (LPARAM)(ico = IconWin32(icon)));
	::SendMessage(hwnd, WM_SETICON, true, (LPARAM)(lico = IconWin32(largeicon)));
}

void TopWindow::CenterRect(HWND hwnd)
{
	SetupRect();
	if(hwnd && center == 1 || center == 2) {
		Size sz = GetRect().Size();
		Rect frmrc(sz);
		::AdjustWindowRect(frmrc, WS_OVERLAPPEDWINDOW, FALSE);
		Rect r, wr;
		wr = Ctrl::GetWorkArea().Deflated(-frmrc.left, -frmrc.top,
			frmrc.right - sz.cx, frmrc.bottom - sz.cy);
		if(center == 1) {
			::GetClientRect(hwnd, r);
			Point p = r.TopLeft();
			::ClientToScreen(hwnd, p);
			r.Offset(p);
		}
		else
			r = wr;
		Point p = r.CenterPos(sz);
		if(p.x + sz.cx > wr.right) p.x = wr.right - sz.cx;
		if(p.y + sz.cy > wr.bottom) p.y = wr.bottom - sz.cy;
		if(p.x < wr.left) p.x = wr.left;
		if(p.y < wr.top) p.y = wr.top;
		SetRect(p.x, p.y, sz.cx, sz.cy);
	}
}

void TopWindow::Open(HWND hwnd)
{
	if(dokeys && ((GetFlags() & AKD_CONSERVATIVE) == 0 || GetAccessKeysDeep() <= 1))
		DistributeAccessKeys();
	UsrLogT(3, "OPEN " + Desc(this));
	LLOG("TopWindow::Open, HWND = " << FormatIntHex((int)hwnd, 8) << ", Active = " << FormatIntHex((int)::GetActiveWindow(), 8));
	IgnoreMouseUp();
	SyncCaption();
	CenterRect(hwnd);
	Create(hwnd, style, exstyle, false, state == OVERLAPPED ? SW_SHOWNORMAL :
	                                    state == MINIMIZED  ? SW_MINIMIZE :
	                                                          SW_MAXIMIZE, false);
	PlaceFocus();
	SyncCaption();
}

void TopWindow::Open(Ctrl *owner)
{
	LLOG("TopWindow::Open(Ctrl) -> " << ::Name(owner));
	Open(owner ? owner->GetTopCtrl()->GetHWND() : NULL);
}

void TopWindow::Open()
{
	Open(::GetActiveWindow()); // :: needed because of ActiveX controls (to create modal dlgs owned by a HWND)
}

void TopWindow::OpenMain()
{
	Open((HWND) NULL);
}

void TopWindow::Minimize(bool effect)
{
	state = MINIMIZED;
	if(IsOpen())
		::ShowWindow(GetHWND(), effect ? SW_MINIMIZE : SW_SHOWMINIMIZED);
}

void TopWindow::Maximize(bool effect)
{
	state = MAXIMIZED;
	if(IsOpen())
		::ShowWindow(GetHWND(), effect ? SW_MAXIMIZE : SW_SHOWMAXIMIZED);
}

void TopWindow::Overlap(bool effect)
{
	state = OVERLAPPED;
	if(IsOpen())
		::ShowWindow(GetHWND(), effect ? SW_SHOWNORMAL : SW_RESTORE);
}

TopWindow& TopWindow::Style(dword _style)
{
	style = _style;
	if(GetHWND())
		::SetWindowLong(GetHWND(), GWL_STYLE, style);
	SyncCaption();
	return *this;
}

TopWindow& TopWindow::ExStyle(dword _exstyle)
{
	exstyle = _exstyle;
	if(GetHWND())
		::SetWindowLong(GetHWND(), GWL_EXSTYLE, exstyle);
	SyncCaption();
	return *this;
}

TopWindow& TopWindow::TopMost(bool b)
{
	return ExStyle(b ? GetExStyle() | WS_EX_TOPMOST : GetExStyle() & ~WS_EX_TOPMOST);
}

bool TopWindow::IsTopMost() const
{
	return GetExStyle() & WS_EX_TOPMOST;
}

#endif
