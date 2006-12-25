#include "CtrlLib.h"

#ifdef PLATFORM_WIN32
#ifndef PLATFORM_WINCE

#include <commdlg.h>
#include <cderr.h>

#endif
#endif


NAMESPACE_UPP


#ifdef PLATFORM_WIN32

#ifndef PLATFORM_WINCE

#define LLOG(x)

enum {
	UM_TASKBAR = WM_USER + 1024,
};

TrayIcon::TrayIcon()
{
	Create(NULL, WS_POPUP, 0, false, 0, 0);
	Ctrl::Hide();
	Zero(nid);
	nid.cbSize = sizeof(nid);
	nid.uCallbackMessage = UM_TASKBAR;
	nid.hWnd = GetHWND();
	static int id;
	nid.uID = ++id;
	visible = false;
	Show();
}

TrayIcon::~TrayIcon()
{
	Hide();
	if(nid.hIcon)
		DestroyIcon(nid.hIcon);
}

void TrayIcon::Notify(dword msg)
{
	if(visible) {
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nid.hIcon = IconWin32(icon);
		int len = min(tip.GetLength(), 60);
		memcpy(nid.szTip, tip, len);
		nid.szTip[len] = 0;
		VERIFY(Shell_NotifyIcon(msg, &nid));
    }
}

/*

	enum MessageType {
		MESSAGE_NONE = 0,
		MESSAGE_INFO = 1,
		MESSAGE_WARNING = 2,
		MESSAGE_ERROR = 3
    };
	void            Message(int type, const char *title, const char *text, int timeout = 10);



void TrayIcon::Message(int type, const char *title, const char *text, int timeout)
{
	if(!IsWinXP())
	    return;
	nid.uFlags = 0x10;
	strcpy(nid.szInfo, text);
	if(title)
		strcpy(nid.szInfoTitle, title);
	else
		*nid.szInfoTitle = 0;
	nid.dwInfoFlags = type;
	nid.uTimeout = minmax(timeout, 10, 30) * 1000;
	Notify(NIM_MODIFY);
	*nid.szInfo = 0;
}
*/

void TrayIcon::Show(bool b)
{
	if(b == visible)
		return;
	if(visible)
		Notify(NIM_DELETE);
	visible = b;
	if(visible)
		Notify(NIM_ADD);
}

TrayIcon& TrayIcon::Icon(const Image &img)
{
	icon = img;
	Notify(NIM_MODIFY);
	return *this;
}

TrayIcon& TrayIcon::Tip(const char *text)
{
	tip = text;
	Notify(NIM_MODIFY);
	return *this;
}

LRESULT TrayIcon::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == UM_TASKBAR)
		switch(lParam) {
		case WM_LBUTTONDOWN:
			WhenLeftDown();
			return TRUE;
		case WM_LBUTTONUP:
			WhenLeftUp();
			return TRUE;
		case WM_LBUTTONDBLCLK:
			::SetForegroundWindow(nid.hWnd);
			WhenLeftDouble();
			return TRUE;
		case WM_RBUTTONDOWN:
			::SetForegroundWindow(nid.hWnd);
			MenuBar::Execute(NULL, WhenBar, GetMousePos());
			return TRUE;
		}
	return Ctrl::WindowProc(message, wParam, lParam);
}

#endif
#endif

END_UPP_NAMESPACE
