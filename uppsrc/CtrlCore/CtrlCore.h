#ifndef CTRLCORE_H
#define CTRLCORE_H

#include <Draw/Draw.h>

enum {
	K_DELTA        = 0x010000,

	K_ALT          = 0x080000,
	K_SHIFT        = 0x040000,
	K_CTRL         = 0x020000,

	K_KEYUP        = 0x100000,

	K_MOUSEMIDDLE  = 0x200000,
	K_MOUSERIGHT   = 0x400000,
	K_MOUSELEFT    = 0x800000,
	K_MOUSEDOUBLE  = 0x1000000,

	K_SHIFT_CTRL = K_SHIFT|K_CTRL,
};

#ifdef PLATFORM_WIN32
#include "Win32Keys.h"
#endif

#ifdef PLATFORM_X11
#include "X11Keys.h"
#endif

#include "MKeys.h"

enum {
	DELAY_MINIMAL = 0
};

void *SetTimeCallback(int delay_ms, Callback cb, void *id = NULL); // delay_ms < 0 -> periodic
void  KillTimeCallback(void *id);
bool  ExistsTimeCallback(void *id);
dword GetTimeClick();

inline
void  PostCallback(Callback cb, void *id = NULL)                { SetTimeCallback(1, cb, NULL); }

class TimeCallback
{
public:
	~TimeCallback()                      { Kill(); }

	void Set(int delay, Callback cb)     { ::SetTimeCallback(delay, cb, this); }
	void Kill()                          { ::KillTimeCallback(this); }
	void KillSet(int delay, Callback cb) { Kill(); Set(delay, cb); }

private:
	byte dummy;
};


class Ctrl;

class CtrlFrame {
public:
	virtual void FrameLayout(Rect& r) = 0;
	virtual void FrameAddSize(Size& sz) = 0;
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual void FrameAdd(Ctrl& parent);
	virtual void FrameRemove();

#ifdef flagSO
	CtrlFrame();
	virtual ~CtrlFrame();
#else
	CtrlFrame() {}
	virtual ~CtrlFrame() {}
#endif

private:
	CtrlFrame(const CtrlFrame&);
	void operator=(const CtrlFrame&);
};

CtrlFrame& NullFrame();

class BorderFrame : public CtrlFrame {
public:
	virtual void FrameLayout(Rect& r);
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual void FrameAddSize(Size& sz);

protected:
	const ColorF *border;

public:
#ifdef flagSO
	BorderFrame(const ColorF *border);
	virtual ~BorderFrame();
#else
	BorderFrame(const ColorF *border) : border(border) {}
#endif
};

CtrlFrame& InsetFrame();
CtrlFrame& OutsetFrame();
CtrlFrame& ButtonFrame();
CtrlFrame& ThinInsetFrame();
CtrlFrame& ThinOutsetFrame();
CtrlFrame& BlackFrame();

CtrlFrame& XPFieldFrame();

CtrlFrame& FieldFrame();

CtrlFrame& TopSeparatorFrame();
CtrlFrame& BottomSeparatorFrame();
CtrlFrame& LeftSeparatorFrame();
CtrlFrame& RightSeparatorFrame();

void LayoutFrameLeft(Rect& r, Ctrl *ctrl, int cx);
void LayoutFrameRight(Rect& r, Ctrl *ctrl, int cx);
void LayoutFrameTop(Rect& r, Ctrl *ctrl, int cy);
void LayoutFrameBottom(Rect& r, Ctrl *ctrl, int cy);

dword GetMouseFlags();

Point GetMousePos();
dword GetMouseFlags();

#ifdef PLATFORM_WIN32
inline bool GetShift()       { return !!(GetKeyState(VK_SHIFT) & 0x8000); }
inline bool GetCtrl()        { return !!(GetKeyState(VK_CONTROL) & 0x8000); }
inline bool GetAlt()         { return !!(GetKeyState(VK_MENU) & 0x8000); }
inline bool GetCapsLock()    { return !!(GetKeyState(VK_CAPITAL) & 1); }
inline bool GetMouseLeft()   { return !!(GetKeyState(VK_LBUTTON) & 0x8000); }
inline bool GetMouseRight()  { return !!(GetKeyState(VK_RBUTTON) & 0x8000); }
inline bool GetMouseMiddle() { return !!(GetKeyState(VK_MBUTTON) & 0x8000); }
#endif

#ifdef PLATFORM_X11
bool GetShift();
bool GetCtrl();
bool GetAlt();
bool GetCapsLock();
bool GetMouseLeft();
bool GetMouseRight();
bool GetMouseMiddle();

String XAtomName(Atom atom);
Atom   XAtom(const char *name);

String      GetProperty(Window w, Atom property, Atom rtype = AnyPropertyType);
Vector<int> GetPropertyInts(Window w, Atom property, Atom rtype = AnyPropertyType);
String      ReadPropertyData(Window w, Atom property, Atom rtype = AnyPropertyType);

Index<Atom>& _NET_Supported();
#endif

class Data {
public:
	virtual bool   Accept();
	virtual void   Reject();
	virtual void   SetData(const Value& data);
	virtual Value  GetData() const;
	virtual void   Serialize(Stream& s);
	virtual void   SetModify();
	virtual void   ClearModify();
	virtual bool   IsModified() const;

	Value        operator~() const             { return GetData(); }
	const Value& operator<<=(const Value& v)   { SetData(v); return v; }
	bool         IsNullInstance() const        { return GetData().IsNull(); }

	virtual ~Data() {}
};

class TopWindow;

class Ctrl : public Data, public Pte<Ctrl> {
public:
	virtual bool   Accept();
	virtual void   Reject();
	virtual void   Serialize(Stream& s);
	virtual void   SetModify();
	virtual void   ClearModify();
	virtual bool   IsModified() const;

public:
	enum PlacementConstants {
		CENTER   = 0,
		LEFT     = 1,
		RIGHT    = 2,
		TOP      = 1,
		BOTTOM   = 2,
		SIZE     = 3,

		MINSIZE  = -16380,
		MAXSIZE  = -16381,
		STDSIZE  = -16382,
	};

	class Logc {
		dword data;

		static int LSGN(dword d)       { return int16(d & 0x7fff | ((d & 0x4000) << 1)); }

	public:
		bool  operator==(Logc q) const { return data == q.data; }
		bool  operator!=(Logc q) const { return data != q.data; }
		int   GetAlign() const         { return (data >> 30) & 3; }
		int   GetA() const             { return LSGN(data >> 15); }
		int   GetB() const             { return LSGN(data); }
		void  SetAlign(int align)      { data = (data & ~(3 << 30)) | (align << 30); }
		void  SetA(int a)              { data = (data & ~(0x7fff << 15)) | ((a & 0x7fff) << 15); }
		void  SetB(int b)              { data = (data & ~0x7fff) | (b & 0x7fff); }
		bool  IsEmpty() const;

		Logc(int al, int a, int b)     { data = (al << 30) | ((a & 0x7fff) << 15) | (b & 0x7fff); }
		Logc()                         { data = 0xffffffff; }
	};

	struct LogPos {
		Logc x, y;

		bool operator==(LogPos b) const   { return x == b.x && y == b.y; }
		bool operator!=(LogPos b) const   { return !(*this == b); }

		LogPos(Logc x, Logc y)            : x(x), y(y) {}
		LogPos()                          {}
	};

	static Logc PosLeft(int pos, int size)       { return Logc(LEFT, pos, size); }
	static Logc PosRight(int pos, int size)      { return Logc(RIGHT, pos, size); }
	static Logc PosTop(int pos, int size)        { return Logc(TOP, pos, size); }
	static Logc PosBottom(int pos, int size)     { return Logc(BOTTOM, pos, size); }
	static Logc PosSize(int lpos, int rpos)      { return Logc(SIZE, lpos, rpos); }
	static Logc PosCenter(int size, int offset)  { return Logc(CENTER, offset, size); }
	static Logc PosCenter(int size)              { return Logc(CENTER, 0, size); }

	typedef bool (*MouseHook)(Ctrl *ctrl, bool inframe, int event, Point p,
	                          int zdelta, dword keyflags);
	typedef bool (*KeyHook)(Ctrl *ctrl, dword key, int count);
	typedef bool (*StateHook)(Ctrl *ctrl, int reason);

	static dword KEYtoK(dword);

private:
	Ctrl(Ctrl&);
	void operator=(Ctrl&);

private:
	enum { MIDDLEBUTTON = 0x80000000 };

	struct Frame : Moveable<Frame> {
		CtrlFrame *frame;
		Rect16     view;

		Frame()    { view.Clear(); }
	};
	Ctrl        *parent;

	struct Scroll : Moveable<Scroll> {
		Rect rect;
		int  dx;
		int  dy;
	};

	struct Top {
#ifdef PLATFORM_WIN32
		HWND           hwnd;
#endif
#ifdef PLATFORM_X11
		Window         window;
#endif
		Vector<Scroll> scroll;
	};

	union {
		Ctrl        *prev;
		Top         *top;
	};
	union {
		Ctrl        *next;
		int          exitcode;
	};
	Ctrl        *firstchild, *lastchild;//16
	LogPos       pos;//8
	Rect16       rect;
	Mitor<Frame> frame;//16
	String       info;//4
	int16        caretx, carety, caretcx, caretcy;//8

	bool         unicode:1;

#ifdef PLATFORM_WIN32
	bool         activex:1;
#endif
	bool         fullrefresh:1;

	bool         transparent:1;
	bool         visible:1;
	bool         enabled:1;
	bool         wantfocus:1;
	bool         initfocus:1;
	bool         editable:1;
	bool         modify:1;
	bool         ignoremouse:1;
	bool         inframe:1;
	bool         inloop:1;
	bool         isopen:1;
	bool         popup:1;
	byte         backpaint:2;//2

	bool         akv:1;
	bool         destroying:1;
#ifdef PLATFORM_X11
	bool         ignoretakefocus:1;
#endif

	static  Ptr<Ctrl> mouseCtrl;
	static  Point     mousepos;
	static  Ptr<Ctrl> focusCtrl;
	static  Ptr<Ctrl> focusCtrlWnd;
	static  Ptr<Ctrl> lastActiveWnd;
	static  Ptr<Ctrl> caretCtrl;
	static  Rect      caretRect;
	static  Ptr<Ctrl> captureCtrl;
	static  bool      ignoreclick;
	static  bool      ignorekeyup;
	static  bool      mouseinview;
	static  bool      mouseinframe;
	static  int       LoopLevel;
	static  Ctrl     *LoopCtrl;

	static  Ptr<Ctrl>           defferedSetFocus;
	static  Vector< Ptr<Ctrl> > defferedChildLostFocus;

	static  Ptr<Ctrl> repeatTopCtrl;
	static  Point     repeatMousePos;

	static  Vector<MouseHook>& mousehook();
	static  Vector<KeyHook>&   keyhook();
	static  Vector<StateHook>& statehook();

#ifdef flagSO
	static Ptr<Ctrl> FocusCtrl();
	static void      FocusCtrl(Ptr<Ctrl> fc);
#else
	static Ptr<Ctrl> FocusCtrl() { return focusCtrl; }
	static void      FocusCtrl(Ptr<Ctrl> fc) { focusCtrl = fc; }
#endif

	void    StateDeep(int state);

	void    RemoveChild0(Ctrl *q);

	Size    PosVal(int v) const;
	void    Lay1(int& pos, int& r, int align, int a, int b, int sz) const;
	Rect    CalcRect(LogPos pos, const Rect& prect, const Rect& pview) const;
	Rect    CalcRect(const Rect& prect, const Rect& pview) const;
	void    UpdateRect();
	void    SetPos0(LogPos p, bool inframe);
	void    SetWndRect(const Rect& r);

	static  void  EndIgnore();
	static  void  LRep();
	static  void  LRepeat();
	static  void  RRep();
	static  void  RRepeat();
	static  void  KillRepeat();
	static  void  CheckMouseCtrl();
	static  void  DoCursorShape();
	bool    IsMouseActive() const;
	Image   MouseEventH(int event, Point p, int zdelta, dword keyflags);
	Image   FrameMouseEventH(int event, Point p, int zdelta, dword keyflags);
	Image   MEvent0(int e, Point p, int zd);
	Image   DispatchMouse(int e, Point p, int zd = 0);
	Image   DispatchMouseEvent(int e, Point p, int zd = 0);
	void    LogMouseEvent(const char *f, const Ctrl *ctrl, int event, Point p, int zdelta, dword keyflags);

	void    StateH(int reason);

	void    RefreshAccessKeys();
	void    RefreshAccessKeysDo(bool vis);
	static  void  DefferedFocusSync();
	static  void  SyncCaret();
	static  bool  DispatchKey(dword keycode, int count);
	void    SetFocusWnd();
	void    KillFocusWnd();

	void    SyncLayout(bool force = false);
	bool    AddScroll(const Rect& sr, int dx, int dy);
	void    ScrollRefresh(const Rect& r, int dx, int dy);
	void    SyncScroll();
	void    DoCtrlPaint(Draw *w, Ctrl *q, Rect pr);
	void    CtrlPaint(Draw& pw, Rect pr);
	void    RemoveFullRefresh();
	bool    ShouldPaint(Draw& w);
	void    GatherTrect(Point offset, Vector<Rect>& tr, Vector<Rect>& er, bool all);
	void    UpdateArea(Draw& draw, const Rect& paintrect);
	Ctrl   *GetTopRect(Rect& r, bool inframe);
	void    DoSync(Ctrl *q, Rect r, bool inframe);

	void    SetInfoPart(int i, const char *txt);
	String  GetInfoPart(int i) const;

// System window interface...
	void WndShow(bool b);

	void WndSetPos(const Rect& rect);

	bool IsWndOpen() const;

	bool SetWndCapture();
	bool HasWndCapture() const;
	bool ReleaseWndCapture();
	void SetMouseCursor(const Image& m);

	static void DoDeactivate(Ptr<Ctrl> pfocusCtrl, Ptr<Ctrl> nfocusCtrl);
	static void DoKillFocus(Ptr<Ctrl> pfocusCtrl, Ptr<Ctrl> nfocusCtrl);
	static void DoSetFocus(Ptr<Ctrl> pfocusCtrl, Ptr<Ctrl> nfocusCtrl, bool activate);

	void ActivateWnd();
	void ClickActivateWnd();
	bool SetFocus0(bool activate);
	bool SetWndFocus();
	bool HasWndFocus() const;

	static void WndDestroyCaret();
	void WndCreateCaret(const Rect& cr);

	void WndInvalidateRect(const Rect& r);

	void SetWndForeground();
	bool IsWndForeground() const;

	bool WndEnable(bool b);

	Rect GetWndUpdateRect() const;
	Rect GetWndScreenRect() const;
	void WndScrollView(const Rect& r, int dx, int dy);
	void WndUpdate();
	void WndUpdate(const Rect& r);

	void WndDestroy();

	static void InitTimer();

	static String appname;

	friend class ViewDraw;
	friend class TopWindow;
	friend class WaitCursor;

#ifdef PLATFORM_WIN32
	static LRESULT CALLBACK SysTimerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	static Event&    ExitLoopEvent();
	static bool&     EndSession();
	static bool      IsEndSession()            { return EndSession(); }
	static HINSTANCE hInstance;

protected:
	static HCURSOR   hCursor;

	static VectorMap< HWND, Ptr<Ctrl> >& Windows();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static Event  OverwatchEndSession;
	static HWND   OverwatchHWND;
	static HANDLE OverwatchThread;

	static LRESULT CALLBACK OverwatchWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static DWORD WINAPI Win32OverwatchThread(LPVOID);

	static Rect GetScreenClient(HWND hwnd);
	void  Create(HWND parent, DWORD style, DWORD exstyle, bool savebits, int show, bool dropshadow);
	Image DoMouse(int e, Point p, int zd = 0);

	friend void sSetCursor(Ctrl *ctrl, const Image& m);

public:
	virtual void    NcCreate(HWND hwnd);
	virtual void    NcDestroy();
	virtual void    PreDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	HWND  GetHWND() const              { return parent ? NULL : top ? top->hwnd : NULL; }
	HWND  GetOwnerHWND() const;

	static Ctrl  *CtrlFromHWND(HWND hwnd);
#endif

#ifdef PLATFORM_X11
protected:
	struct XWindow {
		Ptr<Ctrl>    ctrl;
		bool         exposed;
		Vector<Rect> invalid;
		Ptr<Ctrl>    owner;
		Ptr<Ctrl>    last_active;
		XIC          xic;
	};
private:
	static ArrayMap<Window, XWindow>& Xwindow();
	static Ptr<Ctrl> WndCaretCtrl;
	static int       WndCaretTime;
	static Rect      WndCaretRect;
	static bool      WndCaretVisible;
	static int       Xbuttons;
	static int       Xbuttontime;
	static Window    grabWindow, focusWindow;
	static Point     mousePos;

	static void     ProcessEvent(XEvent *event);
	static void     TimerAndPaint();
	static void     ProcessEvent(XEvent& event);
	       void     Invalidate(XWindow& xw, const Rect& r);
	static void     XorCaret();
	static void     AnimateCaret();
	       void     DoPaint(const Vector<Rect>& invalid);
	       void     SetLastActive(XWindow *w, Ctrl *la);
	       XWindow *GetXWindow();
	static void     SyncMousePos();

	friend bool  GetMouseRight();
	friend bool  GetMouseLeft();
	friend bool  GetMouseMiddle();
	friend Point GetMousePos();

protected:
	       void   Create(Ctrl *owner, bool redirect, bool savebits);
	       void   SyncExpose();
	       void   TakeFocus();
	static Window GetXServerFocusWindow();
	       void   AddGlobalRepaint();
	static void   KillFocus(Window w);
	static void   FocusSync();

public:
	struct Xclipboard {
		Window win;

		VectorMap<int, String> data;

		String Read(int fmt);
		void   Write(int fmt, const String& data);

		void   Clear()                     { data.Clear(); }
		void   Request(XSelectionRequestEvent *se);

		Xclipboard();
		~Xclipboard();
	};

	static Xclipboard& xclipboard();

	static int  Xeventtime;

	static XIM  xim;

	virtual void    EventProc(XWindow& w, XEvent *event);
	        Window  GetWindow() const         { return parent ? None : top ? top->window : None; }
	static  Ctrl   *CtrlFromWindow(Window w);

#endif

private:
			void    DoRemove();

protected:
	static void     TimerProc(dword time);
	virtual Point   GetSysWindowViewOffset();

			Ctrl&   Unicode()                         { unicode = true; return *this; }

public:
	enum StateReason {
		FOCUS      = 10,
		ACTIVATE   = 11,
		DEACTIVATE = 12,
		SHOW       = 13,
		ENABLE     = 14,
		EDITABLE   = 15,
		OPEN       = 16,
		CLOSE      = 17,
		POSITION   = 100,
		LAYOUTPOS  = 101,
	};

	enum MouseEvents {
		BUTTON        = 0x0F,
		ACTION        = 0xF0,

		MOUSEENTER    = 0x10,
		MOUSEMOVE     = 0x20,
		MOUSELEAVE    = 0x30,
		CURSORIMAGE   = 0x40,
		MOUSEWHEEL    = 0x50,

		DOWN          = 0x80,
		UP            = 0x90,
		DOUBLE        = 0xa0,
		REPEAT        = 0xb0,

		LEFTDOWN      = LEFT|DOWN,
		LEFTDOUBLE    = LEFT|DOUBLE,
		LEFTREPEAT    = LEFT|REPEAT,
		LEFTUP        = LEFT|UP,

		RIGHTDOWN     = RIGHT|DOWN,
		RIGHTDOUBLE   = RIGHT|DOUBLE,
		RIGHTREPEAT   = RIGHT|REPEAT,
		RIGHTUP       = RIGHT|UP,
	};

	enum {
		NOBACKPAINT,
		FULLBACKPAINT,
		TRANSPARENTBACKPAINT,
		EXCLUDEPAINT,
	};

	static  Vector<Ctrl *> GetTopCtrls();
	static  Vector<Ctrl *> GetTopWindows();
	static  void   CloseTopCtrls();

	static  void   InstallMouseHook(MouseHook hook);
	static  void   DeinstallMouseHook(MouseHook hook);

	static  void   InstallKeyHook(KeyHook hook);
	static  void   DeinstallKeyHook(KeyHook hook);

	static  void   InstallStateHook(StateHook hook);
	static  void   DeinstallStateHook(StateHook hook);

	virtual void   Paint(Draw& draw);

	virtual void   CancelMode();

	virtual void   Activate();
	virtual void   Deactivate();

	virtual Image  FrameMouseEvent(int event, Point p, int zdelta, dword keyflags);
	virtual Image  MouseEvent(int event, Point p, int zdelta, dword keyflags);
	virtual void   MouseEnter(Point p, dword keyflags);
	virtual void   MouseMove(Point p, dword keyflags);
	virtual void   LeftDown(Point p, dword keyflags);
	virtual void   LeftDouble(Point p, dword keyflags);
	virtual void   LeftRepeat(Point p, dword keyflags);
	virtual void   LeftUp(Point p, dword keyflags);
	virtual void   RightDown(Point p, dword keyflags);
	virtual void   RightDouble(Point p, dword keyflags);
	virtual void   RightRepeat(Point p, dword keyflags);
	virtual void   RightUp(Point p, dword keyflags);
	virtual void   MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void   MouseLeave();

	virtual Image  CursorImage(Point p, dword keyflags);

	virtual bool   Key(dword key, int count);
	virtual void   GotFocus();
	virtual void   LostFocus();
	virtual bool   HotKey(dword key);

	virtual dword  GetAccessKeys() const;
	virtual void   AssignAccessKeys(dword used);

	virtual void   PostInput();

	virtual void   ChildGotFocus();
	virtual void   ChildLostFocus();
	virtual void   ChildAdded(Ctrl *child);
	virtual void   ChildRemoved(Ctrl *child);
	virtual void   ParentChange();

	virtual void   State(int reason);

	virtual void   Layout();

	virtual Size   GetMinSize() const;
	virtual Size   GetStdSize() const;
	virtual Size   GetMaxSize() const;

	virtual bool   IsShowEnabled() const;

	virtual Vector<Rect> GetTransparentFrameRects();
	virtual Vector<Rect> GetOpaqueFrameRects();
	virtual Vector<Rect> GetTransparentViewRects();
	virtual Vector<Rect> GetOpaqueViewRects();

	virtual void   Updated();

	virtual void   Close();

	virtual bool   IsOcxChild();

	virtual String GetDesc() const;

	Callback       WhenAction;

	void             AddChild(Ctrl *child);
	void             AddChild(Ctrl *child, Ctrl *insafter);
	void             AddChildBefore(Ctrl *child, Ctrl *insbefore);
	void             RemoveChild(Ctrl *child);
	Ctrl            *GetParent() const           { return parent; }
	Ctrl            *GetLastChild() const        { return lastchild; }
	Ctrl            *GetFirstChild() const       { return firstchild; }
	Ctrl            *GetPrev() const             { return parent ? prev : NULL; }
	Ctrl            *GetNext() const             { return parent ? next : NULL; }

	bool             IsChild() const             { return parent; }

	Ctrl            *ChildFromPoint(Point& pt) const;

	bool             IsForeground() const;
	void             SetForeground();

	const Ctrl      *GetTopCtrl() const;
	Ctrl            *GetTopCtrl();
	const Ctrl      *GetOwner() const;
	Ctrl            *GetOwner();
	const Ctrl      *GetTopCtrlOwner() const;
	Ctrl            *GetTopCtrlOwner();

	const TopWindow *GetTopWindow() const;
	TopWindow       *GetTopWindow();

	const TopWindow *GetMainWindow() const;
	TopWindow       *GetMainWindow();

	Ctrl&            SetFrame(int i, CtrlFrame& frm);
	Ctrl&            SetFrame(CtrlFrame& frm)            { return SetFrame(0, frm); }
	Ctrl&            AddFrame(CtrlFrame& frm);
	const CtrlFrame& GetFrame(int i = 0) const           { return *frame[i].frame; }
	CtrlFrame&       GetFrame(int i = 0)                 { return *frame[i].frame; }
	void             RemoveFrame(int i);
	void             RemoveFrame(CtrlFrame& frm);
	void             InsertFrame(int i, CtrlFrame& frm);
	int              FindFrame(CtrlFrame& frm);
	int              GetFrameCount() const               { return frame.GetCount(); }
	void             ClearFrames();

	bool        IsOpen() const;

	void        Shutdown()                               { destroying = true; }
	bool        IsShutdown() const                       { return destroying; }

	Ctrl&       SetPos(LogPos p, bool inframe);

	Ctrl&       SetPos(LogPos p);
	Ctrl&       SetPos(Logc x, Logc y)                   { return SetPos(LogPos(x, y)); }
	Ctrl&       SetPosX(Logc x);
	Ctrl&       SetPosY(Logc y);

	void        SetRect(const Rect& r);
	void        SetRect(int x, int y, int cx, int cy);
	void        SetRectX(int x, int cx);
	void        SetRectY(int y, int cy);

	Ctrl&       SetFramePos(LogPos p);
	Ctrl&       SetFramePos(Logc x, Logc y)              { return SetFramePos(LogPos(x, y)); }
	Ctrl&       SetFramePosX(Logc x);
	Ctrl&       SetFramePosY(Logc y);

	void        SetFrameRect(const Rect& r);
	void        SetFrameRect(int x, int y, int cx, int cy);
	void        SetFrameRectX(int x, int cx);
	void        SetFrameRectY(int y, int cy);

	bool        InFrame() const                          { return inframe; }
	bool        InView() const                           { return !inframe; }
	LogPos      GetPos() const                           { return pos; }

	void        RefreshLayout()                          { SyncLayout(true); }
	void        RefreshParentLayout()                    { if(parent) parent->RefreshLayout(); }

	Ctrl&       LeftPos(int a, int size = STDSIZE);
	Ctrl&       RightPos(int a, int size = STDSIZE);
	Ctrl&       TopPos(int a, int size = STDSIZE);
	Ctrl&       BottomPos(int a, int size = STDSIZE);
	Ctrl&       HSizePos(int a = 0, int b = 0);
	Ctrl&       VSizePos(int a = 0, int b = 0);
	Ctrl&       SizePos();
	Ctrl&       HCenterPos(int size = STDSIZE, int delta = 0);
	Ctrl&       VCenterPos(int size = STDSIZE, int delta = 0);

	Ctrl&       LeftPosZ(int a, int size = STDSIZE);
	Ctrl&       RightPosZ(int a, int size = STDSIZE);
	Ctrl&       TopPosZ(int a, int size = STDSIZE);
	Ctrl&       BottomPosZ(int a, int size = STDSIZE);
	Ctrl&       HSizePosZ(int a = 0, int b = 0);
	Ctrl&       VSizePosZ(int a = 0, int b = 0);
	Ctrl&       HCenterPosZ(int size = STDSIZE, int delta = 0);
	Ctrl&       VCenterPosZ(int size = STDSIZE, int delta = 0);

	Rect        GetRect() const;
	Rect        GetScreenRect() const;

	Rect        GetView() const;
	Rect        GetScreenView() const;
	Size        GetSize() const;

	Rect        GetVisibleScreenRect() const;
	Rect        GetVisibleScreenView() const;

	Size        AddFrameSize(int cx, int cy) const;
	Size        AddFrameSize(Size sz) const              { return AddFrameSize(sz.cx, sz.cy); }

	void        Refresh(const Rect& r);
	void        Refresh(int x, int y, int cx, int cy);
	void        Refresh();
	bool        IsFullRefresh() const                    { return fullrefresh; }

	void        RefreshFrame(const Rect& r);
	void        RefreshFrame(int x, int y, int cx, int cy);
	void        RefreshFrame();

	void        ScrollView(const Rect& r, int dx, int dy);
	void        ScrollView(int x, int y, int cx, int cy, int dx, int dy);
	void        ScrollView(int dx, int dy);
	void        ScrollView(const Rect& r, Size delta)    { ScrollView(r, delta.cx, delta.cy); }
	void        ScrollView(Size delta)                   { ScrollView(delta.cx, delta.cy); }

	void        Sync();
	void        Sync(const Rect& r);

	void        DrawCtrl(Draw& w, int x = 0, int y = 0);
	void        DrawCtrlWithParent(Draw& w, int x = 0, int y = 0);

	bool    HasChild(Ctrl *ctrl) const;
	bool    HasChildDeep(Ctrl *ctrl) const;

	Ctrl&   IgnoreMouse(bool b = true)                   { ignoremouse = b; return *this; }
	Ctrl&   NoIgnoreMouse()                              { return IgnoreMouse(false); }
	bool    HasMouse() const;
	bool    HasMouseDeep() const;
	bool    HasMouseInFrame(const Rect& r);
	bool    HasMouseIn(const Rect& r);
	static Ctrl *GetMouseCtrl();

	static void IgnoreMouseClick();
	static void IgnoreMouseUp();

	bool    SetCapture();
	bool    ReleaseCapture();
	bool    HasCapture() const;
	static bool ReleaseCtrlCapture();

	bool    SetFocus();
	bool    HasFocus() const                   { return FocusCtrl() == this; }
	bool    HasFocusDeep() const               { return HasFocus() || HasChildDeep(FocusCtrl()); }
	Ctrl&   WantFocus(bool ft = true)          { wantfocus = ft; return *this; }
	Ctrl&   NoWantFocus()                      { return WantFocus(false); }
	bool	IsWantFocus() const                { return wantfocus; }
	bool    SetWantFocus();
	Ctrl&   InitFocus(bool ft = true)          { initfocus = ft; return *this; }
	Ctrl&   NoInitFocus()                      { return InitFocus(false); }
	bool    IsInitFocus()                      { return initfocus; }
	Ctrl   *GetFocusChild() const              { return HasChild(FocusCtrl()) ? ~FocusCtrl() : NULL; }
	Ctrl   *GetFocusChildDeep() const          { return HasChildDeep(FocusCtrl()) ? ~FocusCtrl() : NULL; }

	void    CancelModeDeep();

	void    SetCaret(int x, int y, int cx, int cy);
	void    SetCaret(const Rect& r);
	void    KillCaret();

	static Ctrl *GetFocusCtrl()                { return FocusCtrl(); }

	static bool  IterateFocusForward(Ctrl *ctrl, Ctrl *top, bool noframe = false, bool init = false);
	static bool  IterateFocusBackward(Ctrl *ctrl, Ctrl *top, bool noframe = false);

	static dword AccessKeyBit(byte accesskey);
	dword   GetAccessKeysDeep() const;
	void    DistributeAccessKeys();
	bool    VisibleAccessKeys();

	void    Show(bool show = true);
	void    Hide()                             { Show(false); }
	bool    IsShown() const                    { return visible; }
	bool    IsVisible() const;

	void    Enable(bool enable = true);
	void    Disable()                          { Enable(false); }
	bool    IsEnabled() const                  { return enabled; }

	Ctrl&   SetEditable(bool editable = true);
	Ctrl&   SetReadOnly()                      { return SetEditable(false); }
	bool    IsEditable() const                 { return editable; }
	bool    IsReadOnly() const                 { return !editable; }

	void    ResetModify()                      { modify = false; }
	bool    IsModifySet() const                { return modify; }

	void    UpdateRefresh();
	void    Update();
	void    Action();
	void    UpdateAction();
	void    UpdateActionRefresh();

	Ctrl&   BackPaint(int bp = FULLBACKPAINT)  { backpaint = bp; return *this; }
	Ctrl&   TransparentBackPaint()             { backpaint = TRANSPARENTBACKPAINT; return *this; }
	Ctrl&   NoBackPaint()                      { return BackPaint(NOBACKPAINT); }
	int     GetBackPaint()                     { return backpaint; }
	Ctrl&   Transparent(bool bp = true)        { transparent = bp; return *this; }
	Ctrl&   NoTransparent()                    { return Transparent(false); }
	bool    IsTransparent() const              { return transparent; }
#ifdef PLATFORM_WIN32
	Ctrl&   ActiveX(bool ax = true)            { activex = ax; return *this; }
	Ctrl&   NoActiveX()                        { return ActiveX(false); }
	bool    IsActiveX() const                  { return activex; }
#endif

	Ctrl&   Info(const char *txt)              { info = txt; return *this; }
	String  GetInfo() const                    { return info; }

	Ctrl&   Tip(const char *txt);
	Ctrl&   HelpLine(const char *txt);
	Ctrl&   Description(const char *txt);
	Ctrl&   HelpTopic(const char *txt);

	String  GetTip() const;
	String  GetHelpLine() const;
	String  GetDescription() const;
	String  GetHelpTopic() const;

	void    Add(Ctrl& ctrl)                    { AddChild(&ctrl); }
	Ctrl&   operator<<(Ctrl& ctrl)             { Add(ctrl); return *this; }

	void    Remove();

	const Value& operator<<=(const Value& v)   { SetData(v); return v; }

	Callback     operator<<=(Callback action)  { WhenAction = action; return action; }
	Callback&    operator<<(Callback action)   { return WhenAction << action; }

	void    SetTimeCallback(int delay_ms, Callback cb, int id = 0);
	void    KillTimeCallback(int id = 0);
	void    KillSetTimeCallback(int delay_ms, Callback cb, int id);
	bool    ExistsTimeCallback(int id = 0) const;
	void    PostCallback(Callback cb, int id = 0);
	void    KillPostCallback(Callback cb, int id);

	enum { TIMEID_COUNT = 1 };

	static void  SetTimerGranularity(int ms);

	static Ctrl *GetActiveCtrl();
	static Ctrl *GetActiveWindow();

	static Ctrl *GetVisibleChild(Ctrl *ctrl, Point p, bool pointinframe);

#ifdef PLATFORM_WIN32
	void   PopUpHWND(HWND hwnd, bool savebits = true, bool activate = true, bool dropshadow = false,
	                 bool topmost = false);
#endif
	void   PopUp(Ctrl *owner = NULL, bool savebits = true, bool activate = true, bool dropshadow = false,
	             bool topmost = false);

	void   SetAlpha(byte alpha);

	static bool IsWaitingEvent();
	static bool ProcessEvent(bool *quit = NULL);
	static bool ProcessEvents(bool *quit = NULL);

	bool   IsPopUp() const          { return popup; }

	static void  EventLoop(Ctrl *loopctrl = NULL);
	static int   GetLoopLevel()     { return LoopLevel; }
	static Ctrl *GetLoopCtrl()      { return LoopCtrl; }

	void   EndLoop()                { inloop = false; }
	void   EndLoop(int code)        { ASSERT(!parent); exitcode = code; EndLoop(); }
	bool   InLoop() const           { return inloop; }
	int    GetExitCode() const      { return exitcode; }

	void SetMinSize(Size sz) {} // see CtrlLayout template and WindowCtrl...

protected:
	static Size Dsize;
	static Size Csize;
	static void Csizeinit();

public:
	static const char *GetZoomText();
	static void SetZoomSize(Size sz, Size bsz = Size(0, 0));
	static int  HorzLayoutZoom(int cx);
	static int  VertLayoutZoom(int cy);
	static Size LayoutZoom(int cx, int cy);
	static Size LayoutZoom(Size sz);
	static void NoLayoutZoom();

	static int  AutoBackPaintAreaSize;
	static int  TransparentBackPaintAreaSize;
	static bool ClickFocus();
	static void ClickFocus(bool cf);

	static Rect   GetWorkArea();
	static int    GetKbdDelay();
	static int    GetKbdSpeed();
	static bool   IsAlphaSupported();
	static Rect   GetDefaultWindowRect();
	static String GetAppName();
	static void   SetAppName(const String& appname);

	static bool   IsXPStyle();
	static void   SetXPStyle(bool b = true);

	enum {
		DRAGFULLWINDOW   = 0x00000001,
		EFFECT_SLIDE     = 0x00000002,
		EFFECT_FADE      = 0x00000004,
		DROPSHADOWS      = 0x00000008,
		ALTACCESSKEYS    = 0x00000010,
		AKD_CONSERVATIVE = 0x00000020,
	};

	static dword  GetFlags();
	static bool   IsFlag(dword w);
	static void   SetFlags(dword w);
	static void   AddFlags(dword w);

	String      Name() const;

#ifdef _DEBUG
	virtual void   Dump() const;
	virtual void   Dump(Stream& s) const;

	static bool LogMessages;
	static int  ShowRepaint;
#endif

	static bool MemoryCheck;

#ifdef PLATFORM_WIN32
	static void InitWin32(HINSTANCE hinst);
	static void ExitWin32();
	static void GuiFlush()                              { ::GdiFlush(); }
#endif

#ifdef PLATFORM_X11
	static void InitX11(const char *display);
	static void ExitX11();
	static void GuiFlush()                              { XFlush(Xdisplay); }
#endif

	static void GuiSleep(int ms);

	Ctrl();
	virtual ~Ctrl();
};

String Name(const Ctrl *ctrl);
String Desc(const Ctrl *ctrl);
void   Dump(const Ctrl *ctrl);

inline Ctrl *operator<<(Ctrl *parent, Ctrl& child) {
	parent->Add(child);
	return parent;
}

String GetKeyDesc(int key);

Vector< Ptr<Ctrl> > DisableCtrls(const Vector<Ctrl *>& ctrl, Ctrl *exclude = NULL);
void EnableCtrls(const Vector< Ptr<Ctrl> >& ctrl);

template <class T>
class FrameCtrl : public T, public CtrlFrame {
public:
	virtual void FrameAdd(Ctrl& parent) { parent.Add(*this); }
	virtual void FrameRemove()          { this->Ctrl::Remove(); }

	FrameCtrl()                         { this->NoWantFocus(); }
};

template <class T>
class FrameLR : public FrameCtrl<T> {
public:
	virtual void FrameAddSize(Size& sz) { sz.cx += (cx ? cx : sz.cy) * this->IsShown(); }

protected:
	int cx;

public:
	FrameLR& Width(int _cx)             { cx = _cx; this->RefreshParentLayout(); return *this; }
	int      GetWidth() const           { return cx; }
	FrameLR()                           { cx = 0; }
};

template <class T>
class FrameLeft : public FrameLR<T> {
public:
	virtual void FrameLayout(Rect& r) {
		LayoutFrameLeft(r, this, this->cx ? this->cx : r.Height());
	}
};

template <class T>
class FrameRight : public FrameLR<T> {
public:
	virtual void FrameLayout(Rect& r) {
		LayoutFrameRight(r, this, this->cx ? this->cx : r.Height());
	}
};

template <class T>
class FrameTB : public FrameCtrl<T> {
public:
	virtual void FrameAddSize(Size& sz) { sz.cy += (cy ? cy : sz.cx) * this->IsShown(); }

protected:
	int cy;

public:
	FrameTB& Height(int _cy)            { cy = _cy; this->RefreshParentLayout(); return *this; }
	int      GetHeight() const          { return cy; }
	FrameTB()                           { cy = 0; }
};

template <class T>
class FrameTop : public FrameTB<T> {
public:
	virtual void FrameLayout(Rect& r) {
		LayoutFrameTop(r, this, this->cy ? this->cy : r.Width());
	}
};

template <class T>
class FrameBottom : public FrameTB<T> {
public:
	virtual void FrameLayout(Rect& r) {
		LayoutFrameBottom(r, this, this->cy ? this->cy : r.Width());
	}
};

class Modality {
	Ptr<Ctrl>           active;
	bool                fore_only;
	Vector< Ptr<Ctrl> > enable;

public:
	void Begin(Ctrl *modal, bool fore_only = false);
	void End();

	~Modality()      { End(); }
};

class ViewDraw : public Draw {
public:
	ViewDraw(Ctrl *ctrl);
	~ViewDraw();

protected:
#ifdef PLATFORM_WIN32
	HWND   hwnd;
#endif
#ifdef PLATFORM_X11
	bool   caret;
#endif
};

#ifdef PLATFORM_WIN32
#ifdef COMPILER_MSC
inline unsigned GetHashValue(const HWND& hwnd)
{
	return (unsigned)hwnd;
}
#endif
#endif

class LocalLoop : public Ctrl {
	Ctrl *master;

public:
	void  Run();
	void  SetMaster(Ctrl& m)      { master = &m; }
	Ctrl& GetMaster() const       { return *master; }

	LocalLoop()                   { master = NULL; }
};

void DrawDragRect(Ctrl& q, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                  Color color, const word *pattern);

bool PointLoop(Ctrl& ctrl, const Vector<Image>& ani, int ani_ms);

class RectTracker : public LocalLoop {
public:
	virtual void  LeftUp(Point, dword);
	virtual void  RightUp(Point, dword);
	virtual void  MouseMove(Point p, dword);
	virtual Image CursorImage(Point, dword);

public:
	struct Rounder {
		virtual Rect Round(const Rect& r) = 0;
	};

protected:
	Rect            rect;
	int             tx, ty;
	Rect            maxrect;
	Size            minsize, maxsize;
	bool            keepratio;
	Rect            clip;
	Color           color;
	Image           cursorimage;
	int             width;
	const word     *pattern;
	int             animation;
	int             panim;
	Rounder        *rounder;

	Rect            org;
	Rect            o;
	Point           op;

	Rect            Round(const Rect& r)           { return rounder ? rounder->Round(r) : r; }

	virtual void    DrawRect(const Rect& r1, const Rect& r2);

public:
	Callback1<Rect> sync;

	RectTracker&    SetCursorImage(const Image& m) { cursorimage = m; return *this; }
	RectTracker&    MinSize(Size sz)               { minsize = sz; return *this; }
	RectTracker&    MaxSize(Size sz)               { maxsize = sz; return *this; }
	RectTracker&    MaxRect(const Rect& mr)        { maxrect = mr; return *this; }
	RectTracker&    Clip(const Rect& c)            { clip = c; return *this; }
	RectTracker&    Width(int n)                   { width = n; return *this; }
	RectTracker&    SetColor(Color c)              { color = c; return *this; }
	RectTracker&    Pattern(const word *p)         { pattern = p; return *this; }
	RectTracker&    Dashed();
	RectTracker&    Solid();
	RectTracker&    Animation(int step_ms = 40)    { animation = step_ms; return *this; }
	RectTracker&    KeepRatio(bool b)              { keepratio = b; return *this; }
	RectTracker&    Round(Rounder& r)              { rounder = &r; return *this; }

	Rect            Get()                          { return rect; }

	Rect Track(const Rect& r, int tx = ALIGN_RIGHT, int ty = ALIGN_BOTTOM);

	RectTracker(Ctrl& master);
	virtual ~RectTracker();
};

class WaitCursor {
#ifdef PLATFORM_WIN32
protected:
	HCURSOR prev;
#endif
public:
	void    Show();

	WaitCursor(bool show = true);
	~WaitCursor();
};

class AutoWaitCursor : public WaitCursor {
protected:
	int&   avg;
	int    time0;

public:
	void Cancel()                   { time0 = 0; }

	AutoWaitCursor(int& avg);
	~AutoWaitCursor();
};

void    ClearClipboard();
void    AppendClipboard(const char *format, const String& data);
String  ReadClipboard(const char *format);
bool    IsClipboardAvailable(const char *format);

inline  void WriteClipboard(const char *format, const String& data)
	{ ClearClipboard(); AppendClipboard(format, data); }

void    AppendClipboardText(const String& s);
String  ReadClipboardText();
void    AppendClipboardUnicodeText(const WString& s);
WString ReadClipboardUnicodeText();
bool    IsClipboardAvailableText();

inline  void WriteClipboardText(const String& s)
	{ ClearClipboard(); AppendClipboardText(s); }
inline  void WriteClipboardUnicodeText(const WString& s)
	{ ClearClipboard(); AppendClipboardUnicodeText(s); }

template <class T>
inline void AppendClipboardFormat(const T& object) {
	AppendClipboard(typeid(T).name(), StoreAsString(const_cast<T&>(object)));
}

template <class T>
inline void WriteClipboardFormat(const T& object) {
	ClearClipboard();
	AppendClipboardFormat(object);
}

template <class T>
inline bool ReadClipboardFormat(T& object)
{
	String s = ReadClipboard(typeid(T).name());
	return !IsNull(s) && LoadFromString(object, s);
}

template <class T>
bool IsClipboardFormatAvailable()
{
	return IsClipboardAvailable(typeid(T).name());
}

template <class T>
inline T ReadClipboardFormat() {
	T object;
	ReadClipboardFormat(object);
	return object;
}

Image  ReadClipboardImage();
void   AppendClipboardImage(const Image& img);

inline void WriteClipboardImage(const Image& img)
	{ ClearClipboard(); AppendClipboardImage(img); }


#include <CtrlCore/TopWindow.h>

bool (*&DisplayErrorFn())(const Value& v);
inline bool DisplayError(const Value& v) { return DisplayErrorFn()(v); }

#ifdef PLATFORM_WIN32

Vector<String>& coreCmdLine__();

#define GUI_APP_MAIN \
Vector<String> SplitCmdLine__(const char *cmd);\
void GuiMainFn_();\
\
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) \
{ \
	Ctrl::InitWin32(hInstance); \
	coreCmdLine__() = SplitCmdLine__(lpCmdLine); \
	AppInitEnvironment__(); \
	GuiMainFn_(); \
	Ctrl::CloseTopCtrls(); \
	UsrLog("---------- About to delete this log..."); \
	DeleteUsrLog(); \
	Ctrl::ExitWin32(); \
	AppExit__(); \
	return GetExitCode(); \
} \
\
void GuiMainFn_()

#endif

#ifdef PLATFORM_POSIX

#define GUI_APP_MAIN \
void GuiMainFn_(); \
\
int main(int argc, const char **argv, const char **envptr) { \
	AppInit__(argc, argv, envptr); \
	Ctrl::InitX11(NULL); \
	GuiMainFn_(); \
	Ctrl::CloseTopCtrls(); \
	UsrLog("---------- About to delete this log..."); \
	DeleteUsrLog(); \
	Ctrl::ExitX11(); \
	AppExit__(); \
	return GetExitCode(); \
} \
 \
void GuiMainFn_()

#endif

#ifdef PLATFORM_WIN32

class DHCtrl : public Ctrl {
public:
	virtual void    State(int reason);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void    NcCreate(HWND hwnd);
	virtual void    NcDestroy();

private:
	HWND   hwnd;

	void CloseHWND();
	void OpenHWND();
	void SyncHWND();

public:
	HWND    GetHWND()                              { return hwnd; }

	DHCtrl();
	~DHCtrl();
};

#endif

#endif
