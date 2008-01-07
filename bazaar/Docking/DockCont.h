#ifndef _Docking_DockCont_h_
#define _Docking_DockCont_h_

#include <Docking/DockableCtrl.h>
#include <Docking/TabBar.h>

class DockBase;

#if defined(PLATFORM_WIN32)
	typedef TopWindow ContainerBase;
#elif defined(PLTFORM_X11)
	typedef WithWindowFrame<TopWindow> ContainerBase;
#endif

class DockCont : public ContainerBase
{
public:
	typedef DockCont CLASSNAME;

	virtual void Paint(Draw &w);
	virtual void LeftDrag(Point p, dword keyflags)		{ MoveBegin(); }
	virtual void LeftDown(Point p, dword keyflags)		{ SetFocus(); }
	virtual void RightDown(Point p, dword keyflags) 	{ WindowMenu(tabbar.GetSelected()); }
	virtual void ChildRemoved(Ctrl *child);

	virtual void ChildGotFocus() 						{ RefreshFocus(true); Ctrl::ChildGotFocus(); }
	virtual void ChildLostFocus() 						{ RefreshFocus(HasFocusDeep()); Ctrl::ChildLostFocus(); }
	virtual void GotFocus() 							{ RefreshFocus(true); }
	virtual void LostFocus() 							{ RefreshFocus(HasFocusDeep()); }
	
#if defined(PLATFORM_WIN32)
private:
	static bool dragging;
public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DockCont &SetCloseLook(const Value *_look) 	{ return *this; } // Dummy 
#elif defined(PLTFORM_X11)

#endif
private:
	enum DockState {
		STATE_NONE,
		STATE_FLOATING,
		STATE_DOCKED, 
		STATE_AUTOHIDE,
	};

	DockState	dockstate;	
	TabBar 		tabbar;
	ImgButton 	close, autohide, windowpos;	
	DockBase *	base;
	Vector<DockableCtrl *> dcs;
	Size usersize;
	bool focus:1;

	// Callbacks
	// Tabs
	void 	TabSelected(int ix);
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
	void	AutoHideMenu(Bar &bar, int ix);
	void 	TabsClose(int ix);
	void 	TabsDock(int align, int ix);
	void 	TabsAutoHide(int align, int ix);
	void 	TabsAutoHide0();
	void 	TabsFloat(int ix);

	int 	GetHandleSize(const DockableCtrl::Style &s);
	Rect	GetHandleRect(const DockableCtrl::Style &s);
	Rect	GetFrameRect(const DockableCtrl::Style &s);

	void	RefreshFocus(bool _focus)	{ if (focus != _focus) { focus = _focus; Refresh(); } }
	void 	SyncSize(DockableCtrl &dc);
public:
	virtual void 	MoveBegin();
	virtual void 	Moving();
	virtual void 	MoveEnd();		
	virtual void 	TitleContext()		{ WindowMenu(tabbar.GetSelected()); }	
	void 			WindowMenu(int ix);	
	
	DockableCtrl &	GetDC(int ix) const			{ return *dcs[ix]; }
	DockableCtrl &	GetCurrentDC() const		{ return *dcs[tabbar.GetSelected()]; }
	void			AddDC(DockableCtrl &dc);
	void 			AddDCs(DockCont &cont, int except = -1);
	void			RemoveDC(int ix)			{ dcs[ix]->Remove(); }
	void			RemoveDC(DockableCtrl &dc)	{ dc.Remove(); }
	void			SelectDC(int ix)			{ tabbar.SelectTab(ix); TabSelected(ix); }
	int				GetDCCount() const			{ return dcs.GetCount(); }
	void 			Clear();
	
	bool 			IsDocked() const			{ return dockstate == STATE_DOCKED; }
	bool			IsFloating() const			{ return dockstate == STATE_FLOATING; }
	bool 			IsAutoHide() const			{ return dockstate == STATE_AUTOHIDE; }	
	bool 			IsDockedAny() const 		{ return dockstate != STATE_FLOATING && dockstate != STATE_NONE; }
	bool			IsHidden() const			{ return dockstate == STATE_NONE; }	
	
	void			StateNotDocked()				{ dockstate = STATE_NONE; }
	void			StateDocked(DockBase &dock);	
	void 			StateFloating(DockBase &dock);	
	void			StateAutoHide(DockBase &dock)	{ StateDocked(dock); Hide(); dockstate = STATE_AUTOHIDE; }

	void			StartMouseDrag(const Point &p);
	
	DockBase *		GetDockBase() const			{ return base; }
	void			SyncButtons(bool show = true);
		
	virtual Size	GetMinSize() const	{ return GetCurrentDC().GetMinSize(); }
	virtual Size	GetMaxSize() const	{ return GetCurrentDC().GetMaxSize(); }
	virtual Size	GetStdSize() const;	
	Size			GetUserSize() 		{ return usersize; }
	void 			SetUserSize(Size sz){ usersize = sz; }
	void			SyncUserSize(bool h, bool v);		
		
	DockCont();		
};

#endif
