#ifndef _Docking_DockableWindow_h_
#define _Docking_DockableWindow_h_

#include "TitleFrame.h"
#include "TabBar.h"

class DockBase;
class DockableWindow;

class DockableWindow : public TopWindow
{
public:
	typedef DockableWindow CLASSNAME;

	virtual void Close()							{ if (!IsAutoHide()) dock_state = STATE_NONE; TopWindow::Close(); }
	virtual void ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags);
	virtual void LostFocus()						{ UpdateFocus(); }
	virtual void GotFocus()							{ UpdateFocus(); }
	virtual void ChildLostFocus()					{ UpdateFocus(); }
	virtual void ChildGotFocus()					{ UpdateFocus(); }	
	virtual Size GetStdSize() const;
#if	defined(PLATFORM_WIN32)
	virtual LRESULT  WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
#elif	defined(PLATFORM_X11) 
	virtual void EventProc(XWindow& w, XEvent *event);
#endif

protected:
	enum DockState {
		STATE_NONE,
		STATE_FLOATING,
		STATE_DOCKED, 
		STATE_AUTOHIDE,
		STATE_TABBED	
	};

	struct ImgButton : public Pusher
	{
		private:
			const Value *look;
			Image img;
		public:		
			virtual void Paint(Draw &w);
			virtual void MouseEnter(Point p, dword kflags) 	{ Refresh(); Pusher::MouseEnter(p, kflags); }
			virtual void MouseLeave() 						{ Refresh(); Pusher::MouseLeave(); }

			ImgButton& 	SetImage(const Image &_img) { img = _img; Refresh(); return *this; }
			ImgButton& 	SetLook(const Value *_look) { look = _look; Refresh(); return *this; }
	};

	TitleFrame			titlebar;
	ImgButton			close, autohide, windowpos;

	virtual void		DockMenu(Bar &bar);
	virtual void		AutoHideMenu(Bar &bar);

	void	SetTitleBar();
	void 	UpdateFocus();

	void 	Float0();
	void 	Dock0(int align);
	void 	AutoHide0();
	void 	AutoHide1(int align);
	void	DockTo0(int align, DockableWindow *target);
	
	void	StateNotDocked()				{ dock_state = STATE_NONE; }
	void	StateDocked(DockBase &dock);
	void 	StateFloating(DockBase &dock);
	void	StateTabbed(DockBase &dock)		{ StateDocked(dock); titlebar.Hide(); dock_state = STATE_TABBED; }
	void	StateAutoHide(DockBase &dock)	{ StateDocked(dock); dock_state = STATE_AUTOHIDE; }
	
	void 	CloseDocked();
	void 	Open() 				{ TopWindow::Open(); }
	void 	Open(Ctrl *_base) 	{ TopWindow::Open(_base); }
	void 	OpenMain()			{ TopWindow::OpenMain(); }
public:
	struct Style : ChStyle<Style> {
		Value btnlook[4];
		Image close;
		Image windowpos;
		Image autohide;
	};	
	static const Style& StyleDefault();		

protected:
	Point				floatpt;
	Size				floatsz;
	Size				docksz;
	bool				isdestroying;
	
private:
	friend class DockBase;		
	
	DockState			dock_state;	
	DockBase *			base;
	static bool 		dragging;
	const Style *		style;
	
	void 				SyncButtonStyle();
public:
	virtual void 	WindowMenu();
	
	const Image &	GetIcon()						{ return titlebar.GetImage(); } // This really should be in TopWindow
	TopWindow &		Icon(const Image& m)			{ titlebar.SetImage(m); return TopWindow::Icon(m); }
	TopWindow &		Icon(const Image& smallicon, const Image& largeicon) { titlebar.SetImage(smallicon); return TopWindow::Icon(smallicon, largeicon); }
	TopWindow &		Title(const char *_title)		{ TopWindow::Title(_title); titlebar.SetTitle(_title); return *this; }
	TopWindow &		Title(const WString& _title)	{ TopWindow::Title(_title); titlebar.SetTitle(_title); return *this; }
	DockableWindow &SetMinSize(Size sz)				{ TopWindow::SetMinSize(sz); return *this; }
	
	DockableWindow &Float()										{ Float0(); return *this; } 
	DockableWindow &Dock(int align)								{ Dock0(align); return *this; } 
	DockableWindow &DockTo(int align, DockableWindow &target)	{ DockTo0(align, &target); return *this; }
	DockableWindow &DockAsTab(DockableWindow &target);

	DockableWindow &AutoHide()						{ ASSERT(base); AutoHide0(); }
	DockableWindow &AutoHide(int align) 			{ ASSERT(base); AutoHide1(align); return *this; }
	void 			AutoHideToggle() 				{ ASSERT(base); if (IsAutoHide()) Float(); else AutoHide0(); }
	
	bool 			IsDocked() const				{ return dock_state == STATE_DOCKED; }
	bool			IsFloating() const				{ return dock_state == STATE_FLOATING; }
	bool 			IsAutoHide() const				{ return dock_state == STATE_AUTOHIDE; }
	bool			IsTabbed() const				{ return dock_state == STATE_TABBED; }	
	bool 			IsDockedAny() const 			{ return dock_state != STATE_FLOATING && dock_state != STATE_NONE; }
	bool			IsHidden() const				{ return dock_state	== STATE_NONE; }
	
	DockableWindow &FloatSize(Size sz);	
	DockableWindow &DockSize(Size sz)				{ docksz = sz; return *this; }
	Point			GetFloatPoint()					{ return floatpt; }
	Size			GetFloatSize() const			{ return floatsz; }
	Size			GetDockSize() const				{ return docksz; }
	
	DockBase *		GetDockBase() const				{ return base; }
	
	DockableWindow &SetStyle(Style &s)				{ style = &s; SyncButtonStyle(); }
	
	DockableWindow();
	~DockableWindow();
};

class DockableTabWindow : public DockableWindow
{
public:
	typedef DockableTabWindow CLASSNAME;
	
	virtual void	ChildAdded(Ctrl *child);		
	virtual void	ChildRemoved(Ctrl *child);
	
	DockableTabWindow();
	
	Ctrl *	GetSelectedCtrl() const	{ return tabbar.GetSelectedCtrl(); }
	
	void 	SelectTab(int tabix)	{ tabbar.SelectTab(tabix); }
	int		GetCount() const		{ return tabbar.GetCount(); }
	void 	AddTabsFrom(DockableTabWindow &wnd);
private:
	virtual void 		WindowMenu();	
	virtual void		DockMenu(Bar &bar);
	virtual void		AutoHideMenu(Bar &bar);
	
	void TabsClose();
	void TabsDock(int align);
	void TabsAutoHide(int align);
	void TabsAutoHide0();
	void TabsFloat();
	
	TabBar 	tabbar;
	void 	TabSelected(int ix);
	void	TabDragged(int ix);
	void	TabContext(int ix);
	
	int 	DoPrompt(const char *title, const char *text);
};

#endif

