#ifndef _Docking_DockBase_h_
#define _Docking_DockBase_h_

#include "DockableWindow.h"
#include "SplitterTree.h"

class DockBase : public Pte<DockBase>
{
protected:
	SplitterFrame 	wd_frame[4];
	SplitterTree	wd_dock[4];
	AutoHideBar		wd_autohide[4];
	Array<DockableTabWindow> tabs;
	Vector<DockableWindow *> wnds;

	bool CanOwn(DockableWindow & wnd) const		{ return (wnd.base == this || wnd.base == NULL); }
	void Undock(DockableWindow &wnd, bool layout = true);
	void Unfloat(DockableWindow &wnd);
	
	SplitterTree *GetDockTree(Ctrl *c);

	void DestroyTabs(Ctrl *p)					{ for (int i = 0; i < tabs.GetCount(); i++) if (p == (Ctrl *)&tabs[i]) { tabs.Remove(i);} }
	
	bool IsTopLeft(int align) const				{ return (align == DOCK_LEFT || align == DOCK_TOP); }
	bool IsTB(int align) const					{ return (align == DOCK_TOP || align == DOCK_BOTTOM); }
	
	String 	GetWindowStatus(DockableWindow *wnd);
	void 	OnWindowStatusButton(StaticText *text, DockableWindow *wnd);
	
	DockBase();
public:
	typedef DockBase CLASSNAME;
	enum {
		DOCK_NONE	= -1,
		DOCK_LEFT 	= SplitterFrame::LEFT,
		DOCK_TOP 	= SplitterFrame::TOP,
		DOCK_RIGHT 	= SplitterFrame::RIGHT,
		DOCK_BOTTOM = SplitterFrame::BOTTOM,
	};
	
	virtual void 		AttachedWindowDragStart(DockableWindow &wnd) = 0;
	virtual void 		AttachedWindowDragMove(DockableWindow &wnd) = 0;
	virtual void 		AttachedWindowDragEnd(DockableWindow &wnd) = 0;
	
	DockableWindow &	Attach(DockableWindow &wnd);
	void				Detach(DockableWindow &wnd);
	
	void Dock(int align, DockableWindow &wnd);
	void Dock(int align, DockableWindow &wnd, const char *title)		{ wnd.Title(title); Dock(align, wnd); }
	void Dock(int align, DockableWindow &target, DockableWindow &wnd);

	void DockAsTab(DockableWindow &target, DockableWindow &wnd);

	void Float(DockableWindow &wnd, bool reposition = true);
	void Float(DockableWindow &wnd, char *title) 						{ wnd.Title(title); Float(wnd, true); }

	void AutoHide(DockableWindow &wnd)									{ AutoHide(wnd.IsDockedAny() ? GetDockAlign(wnd) : DOCK_TOP, wnd); }
	void AutoHide(int align, DockableWindow &wnd);	

	void Hide(DockableWindow &wnd);
			
	void SetHorzVert()							{ wd_dock[0].RootHorz(); wd_dock[2].RootHorz(); wd_dock[1].RootVert(); wd_dock[3].RootVert(); }
	void SetVertHorz()							{ wd_dock[1].RootHorz(); wd_dock[3].RootHorz(); wd_dock[0].RootVert(); wd_dock[2].RootVert(); }
	bool IsHorzVert() const						{ return !wd_dock[0].IsRootVert();}
	bool IsDockVisible(int align) const			{ ASSERT(align >= 0 && align <= 4); return wd_dock[align].IsVisible(); }
		
	void SetDockSize(int align, int size);
	
	DockableWindow *GetMouseDockTarget();
	int  			GetDockAlign(Ctrl &c) const;			
	int				GetDockAlign(const Point &p) const;
	Rect  			GetDockRect(int align) const	{ ASSERT(align >= 0 && align <= 4); return wd_dock[align].IsVisible() ? wd_dock[align].GetScreenRect() : Null; }
	
	void 			ShowWindowStatus();
};

class DnDDockBase : public DockBase
{
public:
	typedef DnDDockBase CLASSNAME;
	virtual void AttachedWindowDragStart(DockableWindow &wnd);
	virtual void AttachedWindowDragMove(DockableWindow &wnd);
	virtual void AttachedWindowDragEnd(DockableWindow &wnd);	

	DnDDockBase &	SimpleMode(bool _simple = true) { simple = _simple; return *this; }
	DnDDockBase &	ComplexMode()					{ return SimpleMode(false); }
	bool			IsSimple() const				{ return simple; }

	DnDDockBase &	Tabbing(bool _tabbing = true) 	{ tabbing = _tabbing; return *this; }
	DnDDockBase &	NoTabbing()						{ return Tabbing(false); }
	bool			IsTabbing() const				{ return tabbing; }
	
	DnDDockBase()	{ simple = false; tabbing = true; }
protected:
	struct HighlightCtrl : public DockableWindow
	{
		virtual void Paint(Draw &w);
		HighlightCtrl()	{ titlebar.Remove(); RemoveFrame(titlebar); }
	};
	
	void			StartWindowDrag(const Point &p, DockableWindow &wnd);
	
	virtual void 	HighlightTarget(int align, DockableWindow &wnd, DockableWindow *target);
	virtual void 	StopHighlightTarget();

	DockableWindow *FindDockTarget(DockableWindow &wnd, int &al);
	Rect			GetHlBounds(int align, DockableWindow &wnd, DockableWindow *target);
	int 			GetBorderAlign(const Point &p, Rect r, bool tabbing, bool allow_lr = true, bool allow_tb = true);

private:
	bool simple;	
	bool tabbing;
	Rect bounds;
};

template<class T>
class WithDocking : public DnDDockBase, public T
{
public:
	WithDocking<T>()
	{ 
		T::AddFrame(*wd_autohide[1]);
		T::AddFrame(*wd_autohide[3]);
		T::AddFrame(*wd_autohide[0]);
		T::AddFrame(*wd_autohide[2]);		
		T::AddFrame(wd_frame[1]);
		T::AddFrame(wd_frame[3]);
		T::AddFrame(wd_frame[0]);
		T::AddFrame(wd_frame[2]);
		//SetVertHorz();
	} // Top, Bottom, Left, Right
};

typedef WithDocking<TopWindow> 	DockWindow;
typedef WithDocking<Ctrl> 		DockCtrl;

#endif


