#ifndef _Docking_DockInterface_h_
#define _Docking_DockInterface_h_

#include "DockableWindow.h"

DockInterface &	DockComplexPopUp();
DockInterface &	DockSimplePopUp();
DockInterface & DockSimpleDnD();

class DockSimpleDnDCls : public DockInterface
{
public:
	typedef DockSimpleDnDCls CLASSNAME;
	virtual void DragStart(DockBase &base, DockableWindow &wnd);
	virtual void DragFinish(DockBase &base, DockableWindow &wnd);
	virtual void DragHighlight(DockBase &base, DockableWindow &wnd);	
	
private:
	void DoDock(int align, DockableWindow *wnd)						{ if (wnd->IsFloating()) wnd->Dock(align); }
	int GetBorderAlign(const Point &p, Rect r, int border) const;
};

class DockComplexPopUpCls : public DockSimpleDnDCls
{
public:
	typedef DockComplexPopUpCls CLASSNAME;
	virtual void DragMove(DockBase &base, DockableWindow &wnd)			{ ShowPopUps(base, wnd); }
	virtual void DragFinish(DockBase &base, DockableWindow &wnd)		{ HidePupups(); }
	virtual void DragHighlight(DockBase &base, DockableWindow &wnd);
protected:
	struct PopUpButton : public Ctrl
	{
		Image img;
		virtual void 	Paint(Draw &w)	{ w.DrawImage(0, 0, img); if (IsMouseIn()) w.DrawImage(GetSize(), DockImg::Highlight()); }
		virtual void 	Close()			{ bool a = IsMouseIn(); Ctrl::Close(); if (a) WhenAction();} 
		bool 			IsMouseIn()		{ return GetScreenRect().Contains(GetMousePos()); }
	};

	struct DockPopUps
	{
		Point ocp, icp;
		PopUpButton inner[5];
		PopUpButton outer[4];	
//		bool enabled;

		DockPopUps();
	};

	void DoDock(int align, DockableWindow *wnd)						{ if (wnd->IsFloating()) wnd->Dock(align); }
	void DoDockAsTab(DockableWindow *target, DockableWindow *wnd)	{ wnd->DockAsTab(*target); }

	void ShowPopUps(DockBase &docks, DockableWindow &wnd, bool inner = true, bool outer = true, bool tabs = true);
	void HidePupups();		
};

class DockSimplePopUpCls : public DockComplexPopUpCls
{
public:
	typedef DockSimplePopUpCls CLASSNAME;
	virtual void DragMove(DockBase &base, DockableWindow &wnd)			{ ShowPopUps(base, wnd, false, true, true); }	
};

class DockAnimatorCls
{
public:
	virtual void AnimateDock(int align, DockBase &base, DockableWindow &wnd) 				{ };		
	virtual void AnimateDock(int align, DockBase &base, DockableWindow &target, DockableWindow &wnd) 	{ AnimateDock(align, base, wnd); } 		
	virtual void AnimateOff(int align, DockBase &base, DockableWindow &wnd) 				{ }
};

class DockInsertAnimatorCls : public DockAnimatorCls
{
public:
	typedef DockInsertAnimatorCls CLASSNAME;
	virtual void AnimateDock(int align, DockBase &base, DockableWindow &wnd);
	virtual void AnimateDock(int align, DockBase &base, DockableWindow &target, DockableWindow &wnd);
	virtual void AnimateOff(int align, DockBase &base, DockableWindow &wnd);
private:
	struct DragLoop : public LocalLoop
	{
		Rect limit;
		virtual void LeftUp(Point p, dword keyflags)	{ EndLoop(); }
		virtual void MouseMove(Point p, dword keyflags) { if (!limit.Contains(GetMousePos())) EndLoop(); }
		virtual void ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags) { if (!limit.Contains(GetMousePos())) EndLoop(); }
	};

	void DoDock(int align, DockableWindow *wnd)						{ if (wnd->IsFloating()) wnd->Dock(align); }
	void DoDockAsTab(DockableWindow *target, DockableWindow *wnd)	{ wnd->DockAsTab(*target); }
};


#endif
