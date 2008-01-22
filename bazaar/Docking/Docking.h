#ifndef _Docking_Docking_h_
#define _Docking_Docking_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <Docking/DockCont.h>
#include <Docking/DockFrame.h>
#include <Docking/DockableCtrl.h>

class DockWindow : public TopWindow
{
public:
	typedef DockWindow CLASSNAME;
	
	enum {
		DOCK_NONE	= -1,
		DOCK_LEFT 	= SplitterFrame::LEFT,
		DOCK_TOP 	= SplitterFrame::TOP,
		DOCK_RIGHT 	= SplitterFrame::RIGHT,
		DOCK_BOTTOM = SplitterFrame::BOTTOM,
	};

protected:

	// DnD interface
	struct HighlightCtrl : public DockableCtrl
	{
		HighlightCtrl() { Transparent(false); }		
		virtual void Paint(Draw &w);
		int oldframesize;
		Rect bounds;
	};
	virtual void ContainerDragStart(DockCont &dc);
	virtual void ContainerDragMove(DockCont &dc);
	virtual void ContainerDragEnd(DockCont &dc);
	virtual HighlightCtrl &GetHighlightCtrl()			{ return hlc; }
		
	void Highlight(int align, DockableCtrl &dc, DockableCtrl *target);
	void StopHighlight(bool do_animate);

	int  GetDockAlign(Ctrl &c) const;			
	int	 GetDockAlign(const Point &p) const;
	Rect GetDockRect(int align) const;
	
	// Container docking/undocking
	void DockContainer(int align, DockCont &c, int except = -1);
	void DockContainer(int align, DockableCtrl &target, DockCont &c);
	void DockContainerAsTab(DockCont &target, DockCont &c);	
	void FloatContainer(DockCont &c, int except = -1, Point p = Null);
	void FloatFromTab(DockCont &c, DockableCtrl &dc);
	void AutoHideContainer(int align, DockCont &c, int except);
	void CloseContainer(DockCont &c)			{ c.Clear(); Undock(c); DestroyContainer(&c); }
	
	void Undock(DockCont &c);
	void Unfloat(DockCont &c);

	// Helpers
	bool IsTL(int align) const					{ return align < 2; } // (align == DOCK_LEFT || align == DOCK_TOP)
	bool IsTB(int align) const					{ return align & 1; } // (align == DOCK_TOP || align == DOCK_BOTTOM)
	DockableCtrl *GetMouseDockTarget();
	DockableCtrl *FindDockTarget(DockCont &dc, int &al);

	friend class DockCont;

private:
	Array<DockCont> conts;
	bool simple:1;	
	bool tabbing:1;
	bool autohide:1;
	bool animate:1;
	bool dockable[4];
	HighlightCtrl 	hlc;
	DockFrame		dockframe[4];
	AutoHideBar		hideframe[4];	

public:	
	void DockLayout();

	void Dock(int align, DockableCtrl &dc);
	void Dock(int align, DockableCtrl &dc, const char *title)		{ dc.Title(title); Dock(align, dc); }
	void Dock(int align, DockableCtrl &target, DockableCtrl &dc);
	void DockAsTab(DockableCtrl &target, DockableCtrl &dc);
	void Float(DockableCtrl &dc, Point p = Null);
	void Float(DockableCtrl &dc, char *title, Point p = Null) 		{ dc.Title(title); Float(dc, p); }
	void AutoHide(DockableCtrl &dc)									{ int al = GetDockAlign(dc); AutoHide(al == DOCK_NONE ? DOCK_TOP : al,  dc); }
	void AutoHide(int align, DockableCtrl &dc);	
	void Close(DockableCtrl &dc);
			
	DockWindow &SetHorzVert()					{ dockframe[0].RootHorz(); dockframe[2].RootHorz(); dockframe[1].RootVert(); dockframe[3].RootVert(); return *this; }
	DockWindow &SetVertHorz()					{ dockframe[1].RootHorz(); dockframe[3].RootHorz(); dockframe[0].RootVert(); dockframe[2].RootVert(); return *this; }
	bool 		IsHorzVert() const				{ return !dockframe[0].IsRootVert();}
	bool 		IsDockVisible(int align) const	{ ASSERT(align >= 0 && align <= 4); return dockframe[align].IsVisible(); }
		
	void 		SetFrameSize(int align, int size);

	DockWindow &SimpleMode(bool _simple = true) { simple = _simple; return *this; }
	DockWindow &ComplexMode()					{ return SimpleMode(false); }
	bool		IsSimple() const				{ return simple; }

/*	DockWindow &	SmartResize(bool resize = true)	{ smart_resize = resize; }
	DockWindow &	NoSmartResize()					{ SmartResize(false); }
	bool		IsSmartResize()					{ return smart_resize; }
*/
	DockWindow &Animate(bool _animate = true) 	{ animate = _animate; return *this; }
	DockWindow &NoAnimate()						{ return Animate(false); }
	bool		IsAnimated() const				{ return animate; }

	DockWindow &Tabbing(bool _tabbing = true) 	{ tabbing = _tabbing; return *this; }
	DockWindow &NoTabbing()						{ return Tabbing(false); }
	bool		IsTabbing() const				{ return tabbing; }
	
	DockWindow &AllowDockAll()					{ return AllowDockLeft().AllowDockRight().AllowDockTop().AllowDockBottom(); }
	DockWindow &AllowDockNone()					{ return AllowDockLeft(false).AllowDockRight(false).AllowDockTop(false).AllowDockBottom(false); }
	DockWindow &AllowDockLeft(bool v = true)	{ dockable[DOCK_LEFT] = v; return *this; }
	DockWindow &AllowDockTop(bool v = true)		{ dockable[DOCK_TOP] = v; return *this; }
	DockWindow &AllowDockRight(bool v = true)	{ dockable[DOCK_RIGHT] = v; return *this; }
	DockWindow &AllowDockBottom(bool v = true)	{ dockable[DOCK_BOTTOM] = v; return *this; }
	bool		IsDockAllowed(int align)		{ ASSERT(align >= 0 && align < 4); return dockable[align]; }
	bool		IsDockAllowed(int align, DockableCtrl &dc)	{ return dockable[align] && dc.IsDockAllowed(align); }
	
	DockWindow &AutoHide(bool v = true);
	bool		IsAutoHide()					{ return autohide; }
	
	DockWindow();		

private:
	// Container management
	DockCont *	GetContainer(DockableCtrl &dc)			{ return dynamic_cast<DockCont *>(dc.GetParent()); }
	DockCont *  CreateContainer();
	DockCont *	CreateContainer(DockableCtrl &dc);
	void		DestroyContainer(DockCont *c);
	DockCont * 	GetReleasedContainer(DockableCtrl &dc);

	// Helpers		
	bool 		CanOwn(DockCont &c) const				{ return (c.GetDockWindow() == this || c.GetDockWindow() == NULL); }
	void 		Dock0(int align, Ctrl &c, bool do_animate = false);
	void 		Dock0(int align, DockableCtrl &target, Ctrl &c, bool do_animate = false);
	void 		Undock0(Ctrl &c, bool do_animate = false, int fsz = -1);		
	SplitterTree *GetDockTree(Ctrl *c);
	
	Rect 		GetAlignBounds(int al, Rect r, bool center, bool allow_lr = true, bool allow_tb = true);
	int 		GetPointAlign(const Point p, Rect r, bool center, bool allow_lr = true, bool allow_tb = true);
	int 		GetQuad(Point p, Rect r);
	
	Size		CtrlBestSize(const Ctrl &c, bool restrict = true) const;
	void 		SyncFrameSize(int dock, Ctrl &c);
};

class PopUpDockWindow : public DockWindow
{
public:
	struct Style : ChStyle<Style> {
		Value inner[5];
		Value outer[4];
		Value highlight;
		int innersize;
		int outersize;
	};	
	static const Style& StyleDefault();		
	
protected:
	virtual void ContainerDragStart(DockCont &dc);
	virtual void ContainerDragMove(DockCont &dc);
	virtual void ContainerDragEnd(DockCont &dc);		
	
private:
	struct PopUpButton : public Ctrl
	{
		const Value *icon;
		const Value *hlight;
		virtual void Paint(Draw &w)	{ Rect r = GetSize(); ChPaint(w, r, *icon); if (hlight) ChPaint(w, r, *hlight); }
		PopUpButton() { hlight = NULL; }
	};

	const Style * 	style;	
	DockableCtrl *	last_target;
	PopUpButton *	last_popup;
	Point 			last_cp;
	PopUpButton 	inner[5];
	PopUpButton 	outer[4];	
	
	int 	PopUpHighlight(PopUpButton *pb, int cnt);
	void 	ShowOuterPopUps(DockCont &dc);
	void 	ShowInnerPopUps(DockCont &dc, DockableCtrl *target);
	void 	ShowPopUp(PopUpButton &pb, const Rect &r);
	void 	HidePopUps(bool _inner, bool _outer);	
public:
	PopUpDockWindow &	SetStyle(const Style &s);
	PopUpDockWindow();
};

#endif
