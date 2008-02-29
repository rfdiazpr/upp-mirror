#ifndef _Docking_DockCont_h_
#define _Docking_DockCont_h_

#include <Docking/DockableCtrl.h>
#include <Docking/DockTabBar.h>

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
		STATE_NONE = -1,
		STATE_FLOATING,
		STATE_DOCKED, 
		STATE_AUTOHIDE,
		STATE_TABBED
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

	void	DockMenu(Bar &bar, int ix);
	void	DockMenuItem(Bar &bar, int ix, int al, int align, const char *str);
	void	AutoHideMenu(Bar &bar, int ix);
	void	AutoHideMenuItem(Bar &bar, DockableCtrl &dc, int align, int ix);	
	int 	DoPrompt(const char *title, const char *text) const;
	
	void 	TabsClose(int ix);
	void 	TabsDock(int align, int ix);
	void 	TabsAutoHide(int align, int ix);
	void 	TabsAutoHide0();
	void 	TabsFloat(int ix);
	void	TabClosed(Value v)								{ ValueTo<DockableCtrl *>(v)->Remove(); if (tabbar.GetCount() == 1) RefreshLayout(); }
	
	int 	GetHandleSize(const DockableCtrl::Style &s) const;
	Rect	GetHandleRect(const DockableCtrl::Style &s) const;
	Rect	GetFrameRect(const DockableCtrl::Style &s) const;

	Point	GetClipWidth(const Rect &r) const;
	void 	AddRemoveButton(Ctrl &c, bool state);
	void	RefreshFocus(bool _focus)	{ if (focus != _focus) 	{ focus = _focus; Refresh(); } }
	void 	SyncSize(Ctrl &c, const DockableCtrl::Style &s);
	void	SyncCurrent();
	void 	AddContainerSize(Size &sz) const;
	bool 	IsDockAllowed0(int align, const Value &v) const;
	void	SyncButtons(bool show);

	Ctrl *			CtrlCast(const Value &v) const		{ return IsDockCont(v) ? (Ctrl *)ContCast(v) : (Ctrl *)DockCast(v); }
	DockCont *		ContCast(const Value &v) const 		{ return ValueTo<DockCont *>(v); } 
	DockableCtrl *	DockCast(const Value &v) const 		{ return ValueTo<DockableCtrl *>(v); }
	bool			IsDockCont(const Value &v) const	{ return IsType<DockCont *>(v); }
	Ctrl * 			GetCtrl(int ix)						{ return CtrlCast(tabbar.Get(ix)); }
	Value 			ValueCast(DockableCtrl *dc) const 	{ return RawToValue<DockableCtrl *>(dc); }
	Value 			ValueCast(DockCont *dc) const 		{ return RawToValue<DockCont *>(dc); }
public:
	void 			SetCursor(int ix)					{ tabbar.SetCursor(ix); TabSelected(); }	
	int 			GetCursor()	const					{ return tabbar.GetCursor(); }
	DockableCtrl &	Get(int ix) const;
	DockableCtrl &	GetCurrent() const					{ return Get(tabbar.GetCursor()); }
	void 			AddFrom(DockCont &cont, int except = -1);
	int				GetCount() const					{ return tabbar.GetCount(); }
	void 			Clear();	
	
	virtual void 	MoveBegin();
	virtual void 	Moving();
	virtual void 	MoveEnd();		
	virtual void 	TitleContext()				{ TabContext(-1); }	
	void 			WindowMenu(Bar &bar, int ix, bool noclose);
		
	bool 			IsDocked() const			{ return dockstate == STATE_DOCKED; }
	int				GetDockAlign() const;		
	bool			IsFloating() const			{ return dockstate == STATE_FLOATING; }
	bool 			IsAutoHide() const			{ return dockstate == STATE_AUTOHIDE; }	
	bool			IsTabbed() const			{ return dockstate == STATE_TABBED; }
	bool 			IsDockedAny() const 		{ return dockstate != STATE_FLOATING && dockstate != STATE_NONE; }
	bool			IsHidden() const			{ return dockstate == STATE_NONE; }	
	
	void			StateNotDocked(DockWindow *dock = NULL) 	{ if (dock) base = dock; dockstate = STATE_NONE; }
	void			StateDocked(DockWindow &dock);	
	void 			StateFloating(DockWindow &dock);	
	void			StateAutoHide(DockWindow &dock)				{ StateDocked(dock); Hide(); dockstate = STATE_AUTOHIDE; }
	void			StateTabbed(DockWindow &dock)				{ StateFloating(dock); Hide(); dockstate = STATE_TABBED; }
	
	void			StartMouseDrag(const Point &p);
	
	DockWindow *	GetDockWindow() const			{ return base; }
	void			SyncButtons()					{ SyncButtons(IsDockedAny() && !IsTabbed()); }
	
	void 			Grouping(bool grouping)			{ tabbar.Grouping(grouping); GroupRefresh(); }
	void			GroupRefresh();	
	
	void			WindowButtons(bool menu, bool hide, bool close);
	
	
	void			Highlight();
	
	virtual Size	GetMinSize() const;
	virtual Size	GetMaxSize() const;
	virtual Size	GetStdSize() const;	
	Size			GetUserSize() const				{ return usersize; }
	void 			SetUserSize(Size sz)			{ usersize = sz; }
	void			SyncUserSize(bool h, bool v);		

	WString 		GetTitle(bool force_count = false) const; 
	
	bool			IsDockAllowed(int align, int dc_ix = -1) const;
		
	virtual void 	Serialize(Stream& s);
		
	DockCont();		
};


#endif
