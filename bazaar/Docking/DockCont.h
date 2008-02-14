#ifndef _Docking_DockCont_h_
#define _Docking_DockCont_h_

#include <Docking/DockableCtrl.h>
#include <Docking/TabBar.h>

class DockWindow;

struct ImgButton : public Pusher
{
	private:
		const Value *look;
		Image img;
	public:		
		ImgButton()	{ Transparent(true); }
		virtual void Paint(Draw &w);
		virtual void MouseEnter(Point p, dword kflags) 	{ Refresh(); Pusher::MouseEnter(p, kflags); }
		virtual void MouseLeave() 						{ Refresh(); Pusher::MouseLeave(); }

		ImgButton& 	SetImage(const Image &_img) { img = _img; Refresh(); return *this; }
		ImgButton& 	SetLook(const Value *_look) { look = _look; Refresh(); return *this; }
};

#define DOCKCONT_WND_OFFSET 4 // Should get from API?

class DockCont : public TopWindow
{
public:
	typedef DockCont CLASSNAME;

	virtual void Paint(Draw &w);
//	virtual void Layout();	
	
	virtual void LeftDrag(Point p, dword keyflags);
	virtual void LeftDown(Point p, dword keyflags)		{ SetFocus(); }
	virtual void RightDown(Point p, dword keyflags) 	{ TitleContext(); }
	
	virtual void ChildRemoved(Ctrl *child);
	virtual void ChildAdded(Ctrl *child);
	virtual void ChildGotFocus() 						{ RefreshFocus(true); TopWindow::ChildGotFocus(); }
	virtual void ChildLostFocus() 						{ RefreshFocus(HasFocusDeep()); TopWindow::ChildLostFocus(); }
	virtual void GotFocus() 							{ RefreshFocus(true); }
	virtual void LostFocus() 							{ RefreshFocus(HasFocusDeep()); }
#if defined(PLATFORM_WIN32)
public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
#elif defined(PLATFORM_X11)
	virtual void EventProc(XWindow& w, XEvent *event);
#endif
private:
	enum DockState {
		STATE_NONE,
		STATE_FLOATING,
		STATE_DOCKED, 
		STATE_AUTOHIDE,
	};
	
	int 		dragging;
	DockState	dockstate;	
	DockTabBar 	tabbar;
	ImgButton 	close, autohide, windowpos;	
	DockWindow *base;
	const DockableCtrl::Style *style;
	Size usersize;
	bool focus:1;

	// Callbacks
	// Tabs
	void 	TabSelected();
	void	TabDragged(int ix);
	void	TabContext(int ix);
	// Menus/Buttons
	void 	Float();
	void 	Dock(int align);
	void 	AutoHide();
	void 	AutoHideAlign(int align);
	void	DockTo(int align, DockableCtrl *target);

	int 	DoPrompt(const char *title, const char *text) const;
	Vector<DockableCtrl *> GetOtherDCs(int ix);
	void	DockMenu(Bar &bar, int ix);
	void	DockMenuItem(Bar &bar, DockableCtrl &dc, int align, int ix);
	void	AutoHideMenu(Bar &bar, int ix);
	void	AutoHideMenuItem(Bar &bar, DockableCtrl &dc, int align, int ix);	
	void 	TabsClose(int ix);
	void 	TabsDock(int align, int ix);
	void 	TabsAutoHide(int align, int ix);
	void 	TabsAutoHide0();
	void 	TabsFloat(int ix);
	void	TabClosed(Value v)					{ ValueTo<DockableCtrl *>(v)->Remove(); }
	
	int 	GetHandleSize(const DockableCtrl::Style &s) const;
	Rect	GetHandleRect(const DockableCtrl::Style &s) const;
	Rect	GetFrameRect(const DockableCtrl::Style &s) const;

	void	RefreshFocus(bool _focus)	{ if (focus != _focus) { focus = _focus; Refresh(); } }
	void 	SyncSize(DockableCtrl &dc);
	void 	AddContainerSize(Size &sz) const;
	
	DockableCtrl *	VCast(const Value &v) const 	{ return ValueTo<DockableCtrl *>(v); }
	Value 			DCCast(DockableCtrl *dc) const 	{ return RawToValue<DockableCtrl *>(dc); }
public:
	DockableCtrl &	Get(int ix) const			{ return *VCast(tabbar.Get(ix)); }
	DockableCtrl &	GetCurrent() const			{ return Get(tabbar.GetCursor()); }
	void 			AddFrom(DockCont &cont, int except = -1);
	int				GetCount() const			{ return tabbar.GetCount(); }
	void 			Clear();	
	
	virtual void 	MoveBegin();
	virtual void 	Moving();
	virtual void 	MoveEnd();		
	virtual void 	TitleContext()				{ TabContext(-1); }	
	void 			WindowMenu(Bar &bar, int ix, bool noclose);
		
	bool 			IsDocked() const			{ return dockstate == STATE_DOCKED; }
	bool			IsFloating() const			{ return dockstate == STATE_FLOATING; }
	bool 			IsAutoHide() const			{ return dockstate == STATE_AUTOHIDE; }	
	bool 			IsDockedAny() const 		{ return dockstate != STATE_FLOATING && dockstate != STATE_NONE; }
	bool			IsHidden() const			{ return dockstate == STATE_NONE; }	
	
	void			StateNotDocked(DockWindow *dock = NULL) 	{ if (dock) base = dock; dockstate = STATE_NONE; }
	void			StateDocked(DockWindow &dock);	
	void 			StateFloating(DockWindow &dock);	
	void			StateAutoHide(DockWindow &dock)	{ StateDocked(dock); Hide(); dockstate = STATE_AUTOHIDE; }

	void			StartMouseDrag(const Point &p);
	
	DockWindow *	GetDockWindow() const			{ return base; }
	void			SyncButtons(bool show = true);
		
	virtual Size	GetMinSize() const;
	virtual Size	GetMaxSize() const;
	virtual Size	GetStdSize() const;	
	Size			GetUserSize() const				{ return usersize; }
	void 			SetUserSize(Size sz)			{ usersize = sz; }
	void			SyncUserSize(bool h, bool v);		

	bool			IsDockAllowed(int align, int dc_ix = -1) const;
		
	DockCont();		
};

#endif
