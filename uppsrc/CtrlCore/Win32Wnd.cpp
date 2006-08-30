#include "CtrlCore.h"
#pragma hdrstop

#ifdef PLATFORM_WIN32

#define LLOG(x)   //  LOG(x)
#define LOGTIMING 0

#ifdef _DEBUG
#define LOGMESSAGES 0
#endif

template<>
unsigned GetHashValue(const HWND& h)
{
	return (unsigned)h;
}

static bool sFinished;

static BOOL CALLBACK sDumpWindow(HWND hwnd, LPARAM lParam) {
	String dump;
	dump << (IsWindowEnabled(hwnd) ? "ena" : "dis") << ' '
	     << (IsWindowVisible(hwnd) ? "vis" : "hid") << ' '
	     << Sprintf("owner=0x%x ", GetWindow(hwnd, GW_OWNER));
	Ctrl *ctrl = Ctrl::CtrlFromHWND(hwnd);
	if(ctrl) {
#ifdef _DEBUG
		dump << "Ctrl: " << ::Name(ctrl);
#endif
	}
	else if(!lParam)
		return TRUE;
	else
	{
#ifdef PLATFORM_WINCE
		wchar clsname[256], title[1024];
#else
		char clsname[256], title[1024];
#endif
		::GetClassName(hwnd, clsname, __countof(clsname));
		::GetWindowText(hwnd, title, __countof(title));
		dump << "HWND: " << Sprintf("0x%x", hwnd) << ", class = "
		     << clsname << ", title = " << title;
	}
	LLOG(dump);
	return TRUE;
}

void DumpWindowOrder(bool aliens) {
#ifndef PLATFORM_WINCE
	LLOG("DumpWindowOrder" << BeginIndent);
	EnumChildWindows(NULL, &sDumpWindow, (LPARAM)(aliens ? 1 : 0));
	LLOG(EndIndent << "//DumpWindowOrder");
#endif
}

#ifndef PLATFORM_WINCE
Point GetMousePos() {
	Point p;
	return ::GetCursorPos(p) ? p : Null;
	::GetCursorPos(p);
	return p;
}
#endif

HCURSOR   Ctrl::hCursor;
HINSTANCE Ctrl::hInstance;
#ifndef flagDLL
#ifndef PLATFORM_WINCE
HANDLE    Ctrl::OverwatchThread;
HWND      Ctrl::OverwatchHWND;

Event Ctrl::OverwatchEndSession;
GLOBAL_VAR(Event, Ctrl::ExitLoopEvent)
#endif
#endif

GLOBAL_VAR(bool, Ctrl::EndSession)

#ifndef flagDLL
#ifndef PLATFORM_WINCE
LRESULT CALLBACK Ctrl::OverwatchWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_USER)
		PostQuitMessage(0);
	if(msg == WM_ENDSESSION) {
		EndSession() = true;
		ExitLoopEvent().Set();
		OverwatchEndSession.Wait();
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

DWORD WINAPI Ctrl::Win32OverwatchThread(LPVOID)
{
    WNDCLASS  wc;
    Zero(wc);
	wc.style         = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)OverwatchWndProc;
	wc.hInstance     = hInstance;
	wc.hCursor       = NULL;
	wc.hbrBackground = NULL;
	wc.lpszClassName = "UPP-OVERWATCH";
	RegisterClass(&wc);

	OverwatchHWND = CreateWindowEx(0, "UPP-OVERWATCH", "", WS_OVERLAPPEDWINDOW,
	                               -1000, -1000, 50, 50, NULL, NULL, hInstance, NULL);
	ExitLoopEvent().Set();

    MSG Msg;
    while(GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return 0;
}
#endif
#endif

static HWND  timerHWND = 0;

EXITBLOCK
{
	if(timerHWND) DestroyWindow(timerHWND);
}

LRESULT CALLBACK Ctrl::SysTimerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_TIMER) {
//		LLOG("[" << msecs() << "]: WM_TIMER");
		TimerProc(GetTickCount());
		return 0;
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

bool IsSysFlag(dword flag)
{
	BOOL b;
	return SystemParametersInfo(flag, 0, &b, 0) && b;
}

#ifdef PLATFORM_WINCE
#define L_(x)  L##x
#else
#define L_(x)  x
#endif

void Ctrl::InitWin32(HINSTANCE hInstance)
{
//	RLOGBLOCK("Ctrl::InitWin32");
#define ILOG(x) // RLOG(x)
	ILOG("SetXPStyle; hInstance = " << FormatIntHex(hInstance));
	SetXPStyle(IsWinXP() && !ScreenInfo().PaletteMode() && IsSysFlag(0x1022 /*SPI_GETFLATMENU*/));
	dword flags = 0;
	ILOG("SPI_GETDRAGFULLWINDOWS");
#ifndef PLATFORM_WINCE
	if(IsSysFlag(SPI_GETDRAGFULLWINDOWS))
		flags |= DRAGFULLWINDOW;
#endif
	ILOG("SPI_GETKEYBOARDCUES");
	if(!IsSysFlag(0x100A /*SPI_GETKEYBOARDCUES*/))
		flags |= ALTACCESSKEYS;
	ILOG("SPI_GETMENUANIMATION");
	if(IsSysFlag(0x1002 /*SPI_GETMENUANIMATION*/))
		if(IsSysFlag(0x1012 /*SPI_GETMENUFADE*/))
			flags |= EFFECT_FADE;
		else
			flags |= EFFECT_SLIDE;
	ILOG("SPI_GETDROPSHADOW");
	if(IsSysFlag(0x1024 /*SPI_GETDROPSHADOW*/))
		flags |= DROPSHADOWS;
	ILOG("SetFlags");
	SetFlags(flags);

	Ctrl::hInstance = hInstance;
	ILOG("RegisterClassW");
#ifndef PLATFORM_WINCE
	if(IsWinNT())
#endif
	{
	    WNDCLASSW  wc;
	    Zero(wc);
		wc.style         = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wc.lpfnWndProc   = (WNDPROC)Ctrl::WndProc;
		wc.hInstance     = hInstance;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL; //(HBRUSH)(COLOR_WINDOW+1);
		wc.lpszClassName = L"UPP-CLASS-W";
		RegisterClassW(&wc);
		wc.style         = 0x20000|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wc.lpszClassName = L"UPP-CLASS-DS-W";
		RegisterClassW(&wc);
		wc.style         = CS_SAVEBITS|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wc.lpszClassName = L"UPP-CLASS-SB-W";
		RegisterClassW(&wc);
		wc.style         = 0x20000|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW|CS_SAVEBITS;
		wc.lpszClassName = L"UPP-CLASS-SB-DS-W";
		RegisterClassW(&wc);
	}

	ILOG("RegisterClassA");
	WNDCLASS  wc;
	Zero(wc);
	wc.style         = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)Ctrl::WndProc;
	wc.hInstance     = hInstance;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL; //(HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = L_("UPP-CLASS-A");
	RegisterClass(&wc);
	if(IsWinXP()) {
		wc.style         = 0x20000|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wc.lpszClassName = L_("UPP-CLASS-DS-A");
		RegisterClass(&wc);
	}
	wc.style         = CS_SAVEBITS|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
	wc.lpszClassName = L_("UPP-CLASS-SB-A");
	RegisterClass(&wc);
	if(IsWinXP()) {
		wc.style         = 0x20000|CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW|CS_SAVEBITS;
		wc.lpszClassName = L_("UPP-CLASS-SB-DS-A");
		RegisterClass(&wc);
	}
	wc.style         = 0;
	wc.lpszClassName = L_("UPP-TIMER");
	wc.hCursor       = NULL;
	wc.lpfnWndProc   = &Ctrl::SysTimerProc;
	RegisterClass(&wc);

	ILOG("InitTimer");
	InitTimer();
	ILOG("SetTimer");
	timerHWND = CreateWindow(L_("UPP-TIMER"), L_(""), WS_OVERLAPPED,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, NULL, NULL);
	SetTimer(timerHWND, 1, 10, NULL);
	ILOG("Windows");
	Windows(); //?? TRC: what's the use of this?

	ChSetStyle("detect");

/* TRC 05/11/14: moved to GuiSleep to avoid thread creation in OCX DllMain
	DWORD dummy;
	OverwatchThread = CreateThread(NULL, 0x100000, Win32OverwatchThread, NULL, 0, &dummy);
	ExitLoopEvent().Wait();
*/

// TRC 05/11/18: pSetLayeredWindowAttributes moved to GLOBAL_VAR (see below) to make OCX initialization simpler
#undef ILOG
}

typedef DWORD (WINAPI *PSLWA)(HWND, DWORD, BYTE, DWORD);

static PSLWA SetLayeredWindowAttributes()
{
	static PSLWA pSet;
#ifndef PLATFORM_WINCE
	static bool inited = false;
	if(!inited) {
		inited = true;
		if(HMODULE hDLL = LoadLibrary ("user32"))
			pSet = (PSLWA) GetProcAddress(hDLL, "SetLayeredWindowAttributes");
	}
#endif
	return pSet;
}

bool Ctrl::IsAlphaSupported()
{
	return SetLayeredWindowAttributes();
}

void Ctrl::ExitWin32()
{
	sFinished = true;
	for(int i = 0; i < Windows().GetCount(); i++) {
		HWND hwnd = Windows().GetKey(i);
		if(hwnd)
			::DestroyWindow(hwnd);
	}
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
		::PostQuitMessage(0);
#ifndef flagDLL
#ifndef PLATFORM_WINCE
	OverwatchEndSession.Set();
	PostMessage(OverwatchHWND, WM_USER, 0, 0);
	LLOG("Waiting for overwatch thread to finish...");
	WaitForSingleObject(OverwatchThread, INFINITE);
	LLOG("...overwatch thread finished");
#endif
#endif
}

void Ctrl::SetTimerGranularity(int ms)
{
	if(ms > 0)
		SetTimer(timerHWND, 1, ms, NULL);
	else
		KillTimer(timerHWND, 1);
}

VectorMap< HWND, Ptr<Ctrl> >& Ctrl::Windows()
{
	static VectorMap< HWND, Ptr<Ctrl> > map;
	return map;
}

Vector<Ctrl *> Ctrl::GetTopCtrls()
{
	Vector<Ctrl *> v;
	VectorMap< HWND, Ptr<Ctrl> >& w = Windows();
	for(int i = 0; i < w.GetCount(); i++)
		if(w.GetKey(i) && w[i] && !w[i]->parent)
			v.Add(w[i]);
	return v;
}

void  Ctrl::SetMouseCursor(const Image& image)
{
#ifndef PLATFORM_WINCE
	static Image img;
	if(image.GetSerialId() != img.GetSerialId()) {
		img = image;
		HCURSOR hc = IconWin32(img, true);
		SetCursor(hc);
		if(hCursor)
			DestroyCursor(hCursor);
		hCursor = hc;
	}
#endif
}

Ctrl *Ctrl::CtrlFromHWND(HWND hwnd)
{
	return hwnd ? Windows().Get(hwnd, NULL) : NULL;
}

HWND Ctrl::GetOwnerHWND() const
{
	HWND hwnd = GetHWND();
	if(!hwnd) return NULL;
	return GetWindow(hwnd, GW_OWNER);
}

Ctrl *Ctrl::GetOwner()
{
	HWND hwnd = GetOwnerHWND();
	return hwnd ? CtrlFromHWND(hwnd) : NULL;
}

Ctrl *Ctrl::GetActiveCtrl()
{
	if(focusCtrl)
		return focusCtrl->GetTopCtrl();
	HWND actwnd = ::GetActiveWindow();
	Vector<Ctrl *> top = GetTopCtrls();
	for(int i = 0; i < top.GetCount(); i++)
		if(top[i]->IsActiveX() && top[i]->GetHWND()) {
			LLOG("-> top[" << i << "] = " << FormatIntHex(top[i]->GetHWND()));
			for(HWND hwnd = top[i]->GetHWND(); hwnd; hwnd = ::GetParent(hwnd))
				if(hwnd == actwnd) {
					LLOG("-> match for " << ::Name(top[i]));
					return top[i];
				}
		}
	LLOG("//Ctrl::GetActiveCtrl -> not found (NULL)");
	return NULL;
}

void Ctrl::Create(HWND parent, DWORD style, DWORD exstyle, bool savebits, int show, bool dropshadow)
{
	LLOG("Ctrl::Create(parent = " << (void *)parent << ") in " << ::Name(this) << BeginIndent);
	ASSERT(!IsChild() && !IsOpen());
	Rect r = GetRect();
	AdjustWindowRectEx(r, style, FALSE, exstyle);
	isopen = true;
	top = new Top;
	ASSERT(!parent || IsWindow(parent));
	if(!IsWinXP())
		dropshadow = false;
#ifdef PLATFORM_WINCE
		if(parent)
			top->hwnd = CreateWindowExW(exstyle,
			                            savebits ? dropshadow ? L"UPP-CLASS-SB-DS-W" : L"UPP-CLASS-SB-W"
			                                     : dropshadow ? L"UPP-CLASS-DS-W"    : L"UPP-CLASS-W",
			                            L"", style, r.left, r.top, r.Width(), r.Height(),
			                            parent, NULL, hInstance, this);
		else {
			top->hwnd = CreateWindowW(L"UPP-CLASS-W",
			                          L"", WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			                          parent, NULL, hInstance, this);
		}
#else
	if(IsWinNT() && (!parent || IsWindowUnicode(parent)))
		top->hwnd = CreateWindowExW(exstyle,
		                            savebits ? dropshadow ? L"UPP-CLASS-SB-DS-W" : L"UPP-CLASS-SB-W"
		                                     : dropshadow ? L"UPP-CLASS-DS-W"    : L"UPP-CLASS-W",
		                            L"", style, r.left, r.top, r.Width(), r.Height(),
		                            parent, NULL, hInstance, this);
	else
		top->hwnd = CreateWindowEx(exstyle,
		                           savebits ? dropshadow ? "UPP-CLASS-SB-DS-A" : "UPP-CLASS-SB-A"
		                                    : dropshadow ? "UPP-CLASS-DS-A"    : "UPP-CLASS-A",
		                           "", style, r.left, r.top, r.Width(), r.Height(),
		                           parent, NULL, hInstance, this);
#endif

	inloop = false;

	ASSERT(top->hwnd);

	::ShowWindow(top->hwnd, visible ? show : SW_HIDE);
//	::UpdateWindow(hwnd);
	StateH(OPEN);
	LLOG(EndIndent << "//Ctrl::Create in " << ::Name(this));
}

void Ctrl::WndDestroy()
{
	LLOG("Ctrl::WndDestroy() in " << ::Name(this) << BeginIndent);
	LLOG((DumpWindowOrder(false), ""));
	if(!top) return;
	isopen = false;
	HWND owner = GetWindow(top->hwnd, GW_OWNER);// CXL 31.10.2003 z DoRemove
	bool focus = ::GetFocus() == top->hwnd;
	LLOG("Ctrl::WndDestroy owner " << (void *)owner
	     << " focus " << focus
	     << " ::GetFocus() " << (void *)::GetFocus());
	if(owner && focus)// CXL 7.11.2003 presun - melo by to fungovat take a neblikat...
		::SetFocus(owner);
	::DestroyWindow(top->hwnd);
	LLOG(EndIndent << "//Ctrl::WndDestroy() in " << ::Name(this));
	delete top;
	top = NULL;
}

Image Ctrl::DoMouse(int e, Point p, int zd)
{
//	LLOG("Ctrl::DoMouse(" << p << ", " << e << ")");
	Image img = DispatchMouse(e, p, zd);
	SyncCaret();
	return img;
}

#ifdef _DEBUG

bool Ctrl::LogMessages;

#define x_MSG(x)       { x, #x },

struct WinMsg {
	int         ID;
	const char *name;
}
sWinMsg[] = {
#include "Win32Msg.i"
	0, NULL
};

#endif

void Ctrl::NcCreate(HWND hwnd)
{
	if(!parent)
		top->hwnd = hwnd;
}

void Ctrl::NcDestroy()
{
	if(!parent)
		top->hwnd = NULL;
}

LRESULT CALLBACK Ctrl::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(sFinished)
		return DefWindowProc(hWnd, message, wParam, lParam);
#ifdef PLATFORM_WINCE
	if(message == WM_CREATE)
#else
	if(message == WM_NCCREATE)
#endif
	{
		Ctrl *w = (Ctrl *)((LPCREATESTRUCT) lParam)->lpCreateParams;
		if(w) {
			w->NcCreate(hWnd);
			int i = Windows().Find(NULL);
			if(i >= 0) {
				Windows().SetKey(i, hWnd);
				Windows()[i] = w;
			}
			else
				Windows().Add(hWnd) = w;
		}
	}
	Ctrl *w = Windows().Get(hWnd, NULL);
#ifdef PLATFORM_WINCE
	if(message == WM_DESTROY)
#else
	if(message == WM_NCDESTROY)
#endif
	{
		if(w) w->NcDestroy();
		int i = Windows().Find(hWnd);
		if(i >= 0)
			Windows().SetKey(i, NULL);
	}
	#if LOGMESSAGES
	bool logblk = false;
	if(message != WM_SETCURSOR && message != WM_CTLCOLORBTN && message != WM_TIMER &&
#ifndef PLATFORM_WINCE
	   message != WM_NCHITTEST  && message != WM_ENTERIDLE &&
#endif
	   message != WM_CTLCOLORDLG && message != WM_CTLCOLOREDIT && message != WM_CTLCOLORLISTBOX &&
	   message != WM_CTLCOLORMSGBOX && message != WM_CTLCOLORSCROLLBAR &&
	   message != WM_CTLCOLORSTATIC && message != WM_CANCELMODE &&
	   message != 0x0118)
		for(WinMsg *m = sWinMsg; m->ID; m++)
			if(m->ID == message) {
				RLOG(m->name << ' ' << ::Name(w) <<
					Sprintf(", wParam = %d (0x%x), lParam = %d (0x%x)",
					       wParam, wParam, lParam, lParam));
				VppLog().Begin();
				logblk = true;
				break;
			}
	#endif
	LRESULT l = 0;
	if(w) {
		try
		{
#if defined(_DEBUG) && LOGTIMING
			int ticks = msecs();
			String wname = w->Name();
#endif
			l = w->WindowProc(message, wParam, lParam);
			w->SyncMoves();
#if defined(_DEBUG) && LOGTIMING
			String msgname;
			for(WinMsg *m = sWinMsg; m->ID; m++)
				if(m->ID == message) {
					msgname = m->name;
					break;
				}
			if(IsNull(msgname))
				msgname = NFormat("0x%04x", (int)message);
			LLOG(NFormat("T+%d %s 0x%08x 0x%08x -> %s", msecs(ticks), msgname, (int)wParam, (int)lParam, wname));
#endif
		}
		catch(Exc e)
		{
			LOG("Ctrl::WindowProc -> Exc: " << e);
			NEVER();
		}
	}
	else
		l = DefWindowProc(hWnd, message, wParam, lParam);
#if LOGMESSAGES
	if(logblk)
		VppLog().End();
#endif
	return l;
}

bool PassWindowsKey(int wParam)
{
	return wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9
	||     wParam == VK_INSERT || wParam == VK_DELETE
	||     wParam == VK_HOME || wParam == VK_END
	||     wParam == VK_PRIOR || wParam == VK_NEXT
	||     wParam == VK_UP || wParam == VK_DOWN
	||     wParam == VK_LEFT || wParam == VK_RIGHT
	||     wParam == VK_CLEAR || wParam == VK_SPACE
	||     wParam >= 0x90; // OEM keys
}

static void sProcessMSG(MSG& msg)
{
	if(msg.message != WM_SYSKEYDOWN && msg.message != WM_SYSKEYUP
	|| PassWindowsKey(msg.wParam) || msg.wParam == VK_MENU) //17.11 Mirek - fix to get windows menu invoked on Alt+Space
		TranslateMessage(&msg); // 04/09/07: TRC fix to make barcode reader going better
	DispatchMessage(&msg);
}

bool Ctrl::IsWaitingEvent()
{
	MSG msg;
	return ::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
}

bool Ctrl::ProcessEvent(bool *quit)
{
	if(EndSession())
		return false;
	if(!GetMouseLeft() && !GetMouseRight() && !GetMouseMiddle())
		ReleaseCtrlCapture();
	MSG msg;
	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		if(msg.message == WM_QUIT && quit)
			*quit = true;
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": sProcessMSG " << FormatIntHex(msg.message));
		sProcessMSG(msg);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": //sProcessMSG " << FormatIntHex(msg.message));
		DefferedFocusSync();
		SyncCaret();
		return true;
	}
	return false;
}

void SweepMkImageCache();

bool Ctrl::ProcessEvents(bool *quit)
{
	if(ProcessEvent(quit)) {
		while(ProcessEvent(quit));
		SweepMkImageCache();
		return true;
	}
	SweepMkImageCache();
	return false;
}

void Ctrl::EventLoop(Ctrl *ctrl)
{
	ASSERT(LoopLevel == 0 || ctrl);
	LoopLevel++;
	LLOG("Entering event loop at level " << LoopLevel << BeginIndent);
	Ctrl *ploop;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
	}

	bool quit = false;
	ProcessEvents(&quit);
	while(!EndSession() && !quit && ctrl ? ctrl->IsOpen() && ctrl->InLoop() : GetTopCtrls().GetCount())
	{
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / GuiSleep");
		SyncCaret();
		GuiSleep(1000);
		if(EndSession()) break;
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / ProcessEvents");
		ProcessEvents(&quit);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / after ProcessEvents");
	}

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
	LLOG(EndIndent << "Leaving event loop ");
}

void Ctrl::GuiSleep(int ms)
{
#if !defined(flagDLL) && !defined(PLATFORM_WINCE)

	if(!OverwatchThread) {
		DWORD dummy;
		OverwatchThread = CreateThread(NULL, 0x100000, Win32OverwatchThread, NULL, 0, &dummy);
		ExitLoopEvent().Wait();
	}
	HANDLE h[1];
	*h = ExitLoopEvent().GetHandle();
	MsgWaitForMultipleObjects(1, h, FALSE, ms, QS_ALLINPUT);
#else
	MsgWaitForMultipleObjects(0, NULL, FALSE, ms, QS_ALLINPUT);
#endif
}

void Ctrl::WndDestroyCaret()
{
	LLOG("Ctrl::WndDestroyCaret()");
	::DestroyCaret();
}

void Ctrl::WndCreateCaret(const Rect& cr)
{
	LLOG("Ctrl::WndCreateCaret(" << cr << ") in " << ::Name(this));
	HWND hwnd = GetHWND();
	if(!hwnd) return;
	Rect r;
	::GetClientRect(hwnd, r);
	Point p = r.TopLeft();
	::ClientToScreen(hwnd, p);
	::CreateCaret(hwnd, NULL, cr.Width(), cr.Height());
	::SetCaretPos(cr.left - p.x, cr.top - p.y);
	::ShowCaret(hwnd);
}

Rect Ctrl::GetWndUpdateRect() const
{
	HWND hwnd = GetHWND();
	if(!hwnd) return Null;
	Rect r;
	::GetUpdateRect(hwnd, r, FALSE);
	return r;
}

Rect Ctrl::GetWndScreenRect() const
{
	HWND hwnd = GetHWND();
	if(!hwnd) return Null;
	Rect r;
	::GetWindowRect(hwnd, r);
	return r;
}

void Ctrl::WndShow(bool b)
{
	HWND hwnd = GetHWND();
	if(hwnd)
		::ShowWindow(hwnd, b ? SW_SHOW : SW_HIDE);
}

void Ctrl::WndUpdate()
{
	HWND hwnd = GetHWND();
	if(hwnd) ::UpdateWindow(hwnd);
}

bool Ctrl::IsWndOpen() const {
	return GetHWND();
}

void Ctrl::SetAlpha(byte alpha)
{
	HWND hwnd = GetHWND();
	if(!IsAlphaSupported() || parent || !top || !hwnd)
		return;
	if(alpha == 255) {
		SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & ~0x80000);
		return;
	}
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | 0x80000);
    SetLayeredWindowAttributes() (hwnd, 0, alpha, 2);
}

Rect Ctrl::GetWorkArea()
{
	Rect r;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0);
	LLOG("Ctrl::GetWorkArea -> " << r);
	return r;
}

int Ctrl::GetKbdDelay()
{
#ifdef PLATFORM_WINCE
	return 500;
#else
	int a;
	SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &a, 0);
	return 250 + a * 750 / 4;
#endif
}

int Ctrl::GetKbdSpeed()
{
#ifdef PLATFORM_WINCE
	return 1000 / 32;
#else
	int a;
	SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &a, 0);
	return 1000 / (a + 2);
#endif
}

void Ctrl::SetWndForeground()
{
	LLOG("Ctrl::SetWndForeground() in " << ::Name(this));
	HWND hwnd = GetHWND();
	if(hwnd)
		::SetForegroundWindow(hwnd);
}

bool Ctrl::IsWndForeground() const
{
	HWND hwnd = GetHWND();
	if(!hwnd)
		return false;
	HWND fore = ::GetForegroundWindow();
	LLOG("Ctrl::IsWndForeground(): hwnd = " << (void *)hwnd
	     << ", fore = " << (void *)fore << " - " << ::Name(CtrlFromHWND(fore)));
	if(IsActiveX()) {
		while(hwnd && hwnd != fore && !!::GetParent(hwnd))
			hwnd = ::GetParent(hwnd);
	}
	return hwnd == fore;
}

bool Ctrl::WndEnable(bool b)
{
	LLOG("Ctrl::WndEnable(" << b << ") in " << ::Name(this) << ", focusCtrlWnd = " << ::Name(focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
	HWND hwnd = GetHWND();
	if(!b) {
		ReleaseCapture();
		LLOG("//Ctrl::WndEnable(" << b << ") -> true " << ::Name(this) << ", focusCtrlWnd = " << ::Name(focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
		return true;
	}
	LLOG("//Ctrl::WndEnable(" << b << ") -> false " << ::Name(this) << ", focusCtrlWnd = " << ::Name(focusCtrlWnd) << ", raw = " << (void *)::GetFocus());
	return false;
}

bool Ctrl::SetWndFocus()
{
	LLOG("Ctrl::SetWndFocus() in " << ::Name(this));
	HWND hwnd = GetHWND();
	if(hwnd) {
		LLOG("Ctrl::SetWndFocus() -> ::SetFocus(" << (void *)hwnd << ")");
//		::SetActiveWindow(hwnd);
		::SetFocus(hwnd);
		return true;
	}
	LLOG("//Ctrl::SetWndFocus() in " << ::Name(this) << ", active window = " << (void *)::GetActiveWindow());
	return false;
}

bool Ctrl::HasWndFocus() const
{
	HWND hwnd = GetHWND();
	return hwnd && ::GetFocus() == hwnd;
}

bool Ctrl::SetWndCapture()
{
	HWND hwnd = GetHWND();
	if(hwnd) {
		::SetCapture(hwnd);
		return true;
	}
	return false;
}

bool Ctrl::ReleaseWndCapture()
{
	HWND hwnd = GetHWND();
	if(hwnd && HasWndCapture())
	{
		::ReleaseCapture();
		return true;
	}
	return false;
}

bool Ctrl::HasWndCapture() const
{
	HWND hwnd = GetHWND();
	return hwnd && hwnd == ::GetCapture();
}

void Ctrl::WndInvalidateRect(const Rect& r)
{
	HWND hwnd = GetHWND();
	if(hwnd)
		::InvalidateRect(hwnd, r, false);
}

void Ctrl::WndSetPos(const Rect& rect) {
	HWND hwnd = GetHWND();
	if(hwnd) {
		Rect r = rect;
		AdjustWindowRectEx(r, ::GetWindowLong(hwnd, GWL_STYLE), FALSE,
		                   ::GetWindowLong(hwnd, GWL_EXSTYLE));
		SetWindowPos(hwnd, NULL, r.left, r.top, r.Width(), r.Height(),
		             SWP_NOACTIVATE|SWP_NOZORDER);
		if(HasFocusDeep()) {
			caretCtrl = NULL;
			SyncCaret();
		}
	}
	fullrefresh = false;
}

void Ctrl::WndUpdate(const Rect& r)
{
	Ctrl *top = GetTopCtrl();
	if(top->IsOpen()) {
		HWND hwnd = top->GetHWND();
		HDC hdc = GetDC(hwnd);
		HRGN hrgn = CreateRectRgn(0, 0, 0, 0);
		if(GetUpdateRgn(hwnd, hrgn, FALSE) != NULLREGION) {
			SelectClipRgn(hdc, hrgn);
			Draw draw(hdc);
			bool hcr = focusCtrl && focusCtrl->GetTopCtrl() == top &&
			           caretRect.Intersects(r + top->GetRect().TopLeft());
			if(hcr) ::HideCaret(hwnd);
			draw.Clip(r);
			top->UpdateArea(draw, r);
			ValidateRect(hwnd, r);
			SelectClipRgn(hdc, NULL);
			if(hcr) ::ShowCaret(hwnd);
		}
		ReleaseDC(hwnd, hdc);
		DeleteObject(hrgn);
	}
}

void  Ctrl::WndScrollView(const Rect& r, int dx, int dy)
{
	if(caretCtrl && caretCtrl->GetTopCtrl() == this) {
		WndDestroyCaret();
		caretRect.Clear();
	}
#ifdef PLATFORM_WINCE
	::ScrollWindowEx(GetHWND(), dx, dy, r, r, NULL, NULL, 0);
#else
	::ScrollWindow(GetHWND(), dx, dy, r, r);
#endif
	SyncCaret();
}

void Ctrl::PopUpHWND(HWND owner, bool savebits, bool activate, bool dropshadow, bool topmost)
{
	popup = false;
	Create(owner, WS_POPUP, topmost ? WS_EX_TOPMOST : 0, savebits,
	       owner || !activate ? SW_SHOWNOACTIVATE : SW_SHOW,
	       dropshadow);
	HWND hwnd = GetHWND();
	if(hwnd) popup = true;
	if(activate) SetFocus();
}

void Ctrl::PopUp(Ctrl *owner, bool savebits, bool activate, bool dropshadow, bool topmost)
{
	popup = false;
	Ctrl *q = owner ? owner->GetTopCtrl() : GetActiveCtrl();
	PopUpHWND(q ? q->GetHWND() : NULL, savebits, activate, dropshadow, topmost);
}

Rect Ctrl::GetScreenClient(HWND hwnd)
{
	Rect r;
	::GetClientRect(hwnd, r);
	Point tl = r.TopLeft();
	Point br = r.BottomRight();
	::ClientToScreen(hwnd, tl);
	::ClientToScreen(hwnd, br);
	LLOG("Ctrl::GetScreenClient: hwnd = " << FormatPtr(hwnd) << ", client = " << r
		<< ", screen(tl) = " << tl << ", screen(br) = " << br);
	return Rect(tl, br);
}

Rect Ctrl::GetDefaultWindowRect() {
#ifdef PLATFORM_WINCE
	return Rect(0, 0, 100, 100);
#else
	HWND hwnd = ::CreateWindow("UPP-CLASS-A", "", WS_OVERLAPPED,
		                       CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
							   NULL, NULL, NULL, NULL);
	Rect sr;
	if(hwnd) {
		::ShowWindow(hwnd, SW_HIDE);
		sr = GetScreenClient(hwnd);
		::DestroyWindow(hwnd);
	}
	else
		sr = RectC(20, 20, 500, 350);
	return sr;
#endif
}

void WaitCursor::Show() {
	if(!prev)
		prev = ::SetCursor(LoadCursor(NULL, IDC_WAIT));
}

WaitCursor::WaitCursor(bool show) {
	prev = NULL;
	if(show) Show();
}

WaitCursor::~WaitCursor() {
	if(prev) {
		::SetCursor(prev);
		prev = NULL;
	}
}

ViewDraw::ViewDraw(Ctrl *ctrl)
{
	Ctrl *top = ctrl->GetTopCtrl();
	hwnd = top->GetHWND();
	ASSERT(hwnd);
	Attach(GetDC(hwnd));
	Clipoff(ctrl->GetScreenView() - top->GetScreenRect().TopLeft());
}

ViewDraw::~ViewDraw()
{
	End();
	HDC hdc = Detach();
	if(hwnd && hdc)
		ReleaseDC(hwnd, hdc);
}

int  GetClipboardFormatCode(const char *format_id)
{
	int x = (int)format_id;
	if(x >= 0 && x < 65535)
		return x;
	static VectorMap<String, int> format_map;
	int f = format_map.Find(format_id);
	if(f < 0) {
		f = format_map.GetCount();
		format_map.Add(format_id,
#ifdef PLATFORM_WINCE
			::RegisterClipboardFormat(ToSystemCharset(format_id))
#else
			::RegisterClipboardFormat(format_id)
#endif
		);
	}
	return format_map[f];
}

void ClearClipboard()
{
	if(OpenClipboard(NULL)) {
		EmptyClipboard();
		CloseClipboard();
	}
}

void AppendClipboard(const char *format, const byte *data, int length)
{
	if(!OpenClipboard(NULL))
		return;
	HANDLE handle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, length + 2);
	byte *ptr;
	if(!handle || !(ptr = (byte *)GlobalLock(handle))) {
		CloseClipboard();
		return;
	}
	memcpy(ptr, data, length);
	ptr[length] = 0;
	ptr[length + 1] = 0;
	GlobalUnlock(handle);
	if(!SetClipboardData(GetClipboardFormatCode(format), handle)) {
		LLOG("SetClipboardData error: " << GetLastErrorMessage());
		CloseClipboard();
		return;
	}
	CloseClipboard();
}

void AppendClipboard(const char *format, const String& data)
{
	AppendClipboard(format, data, data.GetLength());
}

String ReadClipboard(const char *format)
{
	if(!OpenClipboard(NULL))
		return Null;
	HGLOBAL hmem = GetClipboardData(GetClipboardFormatCode(format));
	if(hmem == 0) {
		CloseClipboard();
		return Null;
	}
	const byte *src = (const byte *)GlobalLock(hmem);
	ASSERT(src);
	int length = GlobalSize(hmem);
	if(length < 0) {
		CloseClipboard();
		return Null;
	}
	String out(src, length);
	GlobalUnlock(hmem);
	CloseClipboard();
	return out;
}

void AppendClipboardText(const String& s)
{
#ifdef PLATFORM_WINCE
	AppendClipboardUnicodeText(s.ToWString());
#else
	AppendClipboard((const char *)CF_TEXT, ToSystemCharset(s));
#endif
}

void AppendClipboardUnicodeText(const WString& s)
{
#ifndef PLATFORM_WINCE
	AppendClipboardText(s.ToString());
#endif
	AppendClipboard((const char *)CF_UNICODETEXT, (byte *)~s, 2 * s.GetLength());
}

String ReadClipboardText()
{
#ifdef PLATFORM_WINCE
	return ReadClipboardUnicodeText().ToString();
#else
	String s = ReadClipboard((const char *)CF_TEXT);
	return String(s, strlen(~s));
#endif
}

WString ReadClipboardUnicodeText()
{
	String s = ReadClipboard((const char *)CF_UNICODETEXT);
	return WString((const wchar *)~s, wstrlen((const wchar *)~s));
}

bool IsClipboardAvailable(const char *id)
{
	return ::IsClipboardFormatAvailable(GetClipboardFormatCode(id));
}

bool IsClipboardAvailableText()
{
	return IsClipboardAvailable((const char *)CF_TEXT);
}

Vector<String> SplitCmdLine__(const char *cmd)
{
	Vector<String> out;
	while(*cmd)
		if((byte)*cmd <= ' ')
			cmd++;
		else if(*cmd == '\"')
		{
			String quoted;
			while(*++cmd && (*cmd != '\"' || *++cmd == '\"'))
				quoted.Cat(*cmd);
			out.Add(quoted);
		}
		else
		{
			const char *begin = cmd;
			while((byte)*cmd > ' ')
				cmd++;
			out.Add(String(begin, cmd));
		}
	return out;
}

#endif
