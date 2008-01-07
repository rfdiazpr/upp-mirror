#ifndef _Docking_DockBase_h_
#define _Docking_DockBase_h_

#include "DockableCtrl.h"

#include "DockCont.h"
#include "SplitterTree.h"

class DockBase : public Pte<DockBase>
{
public:
	typedef DockBase CLASSNAME;
	enum {
		DOCK_NONE	= -1,
		DOCK_LEFT 	= SplitterFrame::LEFT,
		DOCK_TOP 	= SplitterFrame::TOP,
		DOCK_RIGHT 	= SplitterFrame::RIGHT,
		DOCK_BOTTOM = SplitterFrame::BOTTOM,
	};

private:
	Array<DockCont> conts;
	bool simple:1;	
	bool tabbing:1;
	bool smart_resize:1;
	Rect hlbounds;
	SplitterFrame 	db_frame[4];
	SplitterTree	db_dock[4];
	AutoHideBar		db_autohide[4];	
	
public:	
	void Dock(int align, DockableCtrl &dc);
	void Dock(int align, DockableCtrl &dc, const char *title)		{ dc.Title(title); Dock(align, dc); }
	void Dock(int align, DockableCtrl &target, DockableCtrl &dc);

	void DockAsTab(DockableCtrl &target, DockableCtrl &dc);

	void Float(DockableCtrl &dc);
	void Float(DockableCtrl &dc, char *title) 						{ dc.Title(title); Float(dc); }

	void AutoHide(DockableCtrl &dc)									{ int al = GetDockAlign(dc); AutoHide(al == DOCK_NONE ? DOCK_TOP : al,  dc); }
	void AutoHide(int align, DockableCtrl &dc);	

	void Close(DockableCtrl &dc);
			
	void SetHorzVert()							{ db_dock[0].RootHorz(); db_dock[2].RootHorz(); db_dock[1].RootVert(); db_dock[3].RootVert(); }
	void SetVertHorz()							{ db_dock[1].RootHorz(); db_dock[3].RootHorz(); db_dock[0].RootVert(); db_dock[2].RootVert(); }
	bool IsHorzVert() const						{ return !db_dock[0].IsRootVert();}
	bool IsDockVisible(int align) const			{ ASSERT(align >= 0 && align <= 4); return db_dock[align].IsVisible(); }
		
	void SetFrameSize(int align, int size);

	DockBase &	SimpleMode(bool _simple = true) { simple = _simple; return *this; }
	DockBase &	ComplexMode()					{ return SimpleMode(false); }
	bool		IsSimple() const				{ return simple; }

	DockBase &	SmartResize(bool resize = true)	{ smart_resize = resize; }
	DockBase &	NoSmartResize()					{ SmartResize(false); }
	bool		IsSmartResize()					{ return smart_resize; }

	DockBase &	Tabbing(bool _tabbing = true) 	{ tabbing = _tabbing; return *this; }
	DockBase &	NoTabbing()						{ return Tabbing(false); }
	bool		IsTabbing() const				{ return tabbing; }
	
	DockBase();		
protected:
	// Interface overloads
	virtual void ContainerDragStart(DockCont &c);
	virtual void ContainerDragMove(DockCont &c);
	virtual void ContainerDragEnd(DockCont &c);

	// Container docking/undocking
	void DockContainer(int align, DockCont &c, int except = -1);
	void DockContainer(int align, DockableCtrl &target, DockCont &c);
	void DockContainerAsTab(DockableCtrl &target, DockCont &c);	
	void FloatContainer(DockCont &c, int except = -1, Point p = Null);
	void FloatFromTab(DockCont &c, DockableCtrl &dc);
	void AutoHideContainer(int align, DockCont &c, int except);
	void CloseContainer(DockCont &c)			{ c.Clear(); Undock(c); DestroyContainer(&c); }
	
	void Undock(DockCont &c);
	void Unfloat(DockCont &c);

	// Helpers
	bool IsTL(int align) const					{ return (align == DOCK_LEFT || align == DOCK_TOP); }
	bool IsTB(int align) const					{ return (align == DOCK_TOP || align == DOCK_BOTTOM); }
	DockableCtrl *GetMouseDockTarget();
	DockableCtrl *FindDockTarget(Ctrl *dock_owner, int &al);

	// DnD interface
	void Highlight(int align, DockableCtrl &dc, DockableCtrl *target);
	void StopHighlight();

	// Initialisation
	void DockLayout(Ctrl &_this, bool autohide = true);

	friend DockCont;
private:
	// Container management
	DockCont *	GetContainer(DockableCtrl &dc)			{ return dynamic_cast<DockCont *>(dc.GetParent()); }
	DockCont *  CreateContainer();
	DockCont *	CreateContainer(DockableCtrl &dc);
	void		DestroyContainer(DockCont *c);
	DockCont * 	GetReleasedContainer(DockableCtrl &dc);

	// Helpers		
	Ctrl *	DockOwner()	const							{ return db_dock[0].GetParent()->GetParent(); }					
	bool 	CanOwn(DockCont &c) const					{ return (c.GetDockBase() == this || c.GetDockBase() == NULL); }
	void 	Dock0(int align, Ctrl &c);
	void 	Dock0(int align, DockableCtrl &target, Ctrl &c);
	void 	Undock0(Ctrl &c, bool layout = true);		
	int  	GetDockAlign(Ctrl &c) const;			
	int		GetDockAlign(const Point &p) const;
	Rect  	GetDockRect(int align) const	{ ASSERT(align >= 0 && align <= 4); return db_dock[align].IsVisible() ? db_dock[align].GetScreenRect() : Null; }		
	SplitterTree *	GetDockTree(Ctrl *c);
	int		CtrlBestSize(bool vert, const Ctrl &c, bool restrict = true) const;

	void 	SyncFrameSize(int dock, Ctrl &c);

	// DnD interface
	struct HighlightCtrl : public DockableCtrl
	{
		HighlightCtrl() { Transparent(false); }		
		virtual void Paint(Draw &w);
		int oldframesize;
	};	
	Rect	GetHlBounds(int align, DockableCtrl &dc, DockableCtrl *target);
	int 	GetBorderAlign(const Point &p, Rect r, bool tabbing, bool allow_lr = true, bool allow_tb = true);	
};

template<class T>
class WithDocking : public DockBase, public T
{
public:
	WithDocking<T>() { DockLayout(*this); }
};

typedef WithDocking<TopWindow> 	DockWindow;
typedef WithDocking<Ctrl> 		DockCtrl;

#endif


