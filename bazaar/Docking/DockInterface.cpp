#include "DockInterface.h"
#include "DockBase.h"

#define DOCKIMG_OFFSET		0
#define POPUP_SPACING		0//12
#define DOCK_DND_BORDER		60

DockInterface & DockComplexPopUp()	{ return Single<DockComplexPopUpCls>(); }
DockInterface & DockSimplePopUp()	{ return Single<DockSimplePopUpCls>(); }
DockInterface & DockSimpleDnD()		{ return Single<DockSimpleDnDCls>(); }

// DockSimpleDnd
void DockSimpleDnDCls::DragStart(DockBase &owner, DockableWindow &wnd)
{
	owner.Float(wnd, false);
	// Reposition if not under the mouse
	Rect r = wnd.GetRect();
	Point pt = GetMousePos();
	if (r.left > pt.x || r.right < pt.x) {
		r.OffsetHorz(pt.x - r.left - r.Width()/2);
		wnd.SetRect(r);	
	}
#ifdef PLATFORM_WIN32			
	SendMessage(wnd.GetHWND(), WM_NCLBUTTONDOWN, 2, MAKELONG(pt.x, pt.y));
#endif	
}

void DockSimpleDnDCls::DragFinish(DockBase &base, DockableWindow &wnd)
{
	if (!wnd.IsFloating()) return;
	Point p = GetMousePos();
	Ctrl *owner = wnd.GetOwner();
	ASSERT(owner);
	Rect r = owner->GetScreenRect();

//	StopHighlight();
	int al = base.GetDockAlign(p);
	if (al == DockBase::DOCK_NONE) {
		al = GetBorderAlign(p, r, DOCK_DND_BORDER);
		if (al == DockBase::DOCK_NONE || base.IsDockVisible(al))
			al = DockBase::DOCK_NONE;
	}
//	if (al != DockBase::DOCK_NONE)
//		PostCallback(THISBACK2(DoDock, al, &wnd));	
}

void DockSimpleDnDCls::DragHighlight(DockBase &base, DockableWindow &wnd)
{
	Point p = GetMousePos();
	Ctrl *owner = wnd.GetOwner();
	ASSERT(owner);

	int al = base.GetDockAlign(p);
	if (al != DockBase::DOCK_NONE) {
		Rect r = base.GetDockRect(al);
		if (al == DockBase::DOCK_LEFT || al == DockBase::DOCK_RIGHT)
		    al = base.IsHorzVert() ? DockBase::DOCK_RIGHT : DockBase::DOCK_BOTTOM;
		else
			al = base.IsHorzVert() ? DockBase::DOCK_BOTTOM : DockBase::DOCK_RIGHT;
		if (!r.IsEmpty())
			return Single<DockInsertAnimatorCls>().AnimateDock(al, base, wnd);
//			HighlightTarget(r, *owner, SLtBlue());
	}
	else {
		Rect r = owner->GetScreenView();
		al = GetBorderAlign(p, r, DOCK_DND_BORDER);
		if (al != DockBase::DOCK_NONE && !base.IsDockVisible(al))
			return Single<DockInsertAnimatorCls>().AnimateDock(al, base, wnd);
			//return HighlightTarget(r, *owner, al, 7, SLtBlue());
	}	
	//StopHighlight();
}

int DockSimpleDnDCls::GetBorderAlign(const Point &p, Rect r, int border) const
{
	if (r.Contains(p)) {
		r.Deflate(DOCK_DND_BORDER);
		if (p.y < r.top)
			return DockBase::DOCK_TOP;
		else if (p.y > r.bottom)
			return DockBase::DOCK_BOTTOM;
		else if (p.x < r.left)
			return DockBase::DOCK_LEFT;
		else if (p.x > r.right)
			return DockBase::DOCK_RIGHT;
	}
	return DockBase::DOCK_NONE;
}

// DockComplexPopUp
void DockComplexPopUpCls::DragHighlight(DockBase &base, DockableWindow &wnd)
{
	Point pt = GetMousePos();
	DockPopUps &pop = Single<DockPopUps>();
	if (pop.inner[0].IsOpen()) {
		for (int i = 0; i < 4; i++) {
			if (pop.outer[i].GetScreenRect().Contains(pt)) {
				//HighlightTarget(*wnd.GetOwner(), i, 7, SLtBlue());
				return;	
			}
		}
	}
	if (pop.outer[0].IsOpen()) {
		for (int i = 0; i < 4; i++) {
			if (pop.inner[0].IsOpen() && pop.inner[i].GetScreenRect().Contains(pt)) {
				DockableWindow *target = base.GetMouseDockTarget();
				//HighlightTarget(target ? *((Ctrl *)target) : *wnd.GetOwner(), i, target ? 2 : 7, SLtBlue());
				return;	
			}
		}
	}
	if (pop.inner[4].IsOpen() && pop.inner[4].GetScreenRect().Contains(pt)) {
		DockableWindow *target = base.GetMouseDockTarget();
		//if (target) HighlightTarget(*target, -1, 1, SLtBlue());
		return;	
	}
	//StopHighlight();
}

void DockComplexPopUpCls::ShowPopUps(DockBase &docks, DockableWindow &docker, bool inner, bool outer, bool tabs)
{
//	if (!enabled) return;
	Ctrl *owner = docker.GetOwner();
	ASSERT(owner);

	DockPopUps &pop = Single<DockPopUps>();
	Rect wnd = owner->GetScreenRect();
	Point cp = wnd.CenterPoint();
	Size sz = DockImg::Get(DOCKIMG_OFFSET).GetSize();
	Rect r = Rect(sz);	
	sz /= 2;
	
	wnd.Deflate(12, 12);
	
	if (outer) {
		bool open = pop.outer[0].IsOpen();
		if (open && cp == pop.ocp) 
			for (int i = 0; i < 4; i++) pop.outer[i].Refresh();
		else {
			pop.outer[0].SetRect(r.Offseted(wnd.left + POPUP_SPACING, cp.y - sz.cy));	
			pop.outer[2].SetRect(r.Offseted(wnd.right - POPUP_SPACING - sz.cx*2, cp.y - sz.cy));
			pop.outer[1].SetRect(r.Offseted(cp.x - sz.cx, wnd.top + POPUP_SPACING));	 
			pop.outer[3].SetRect(r.Offseted(cp.x - sz.cx, wnd.bottom - POPUP_SPACING - sz.cy*2));
			if (!open) {
				for (int i = 0; i < 4; i++) {
					pop.outer[i] <<= THISBACK2(DoDock, i, &docker);
					pop.outer[i].PopUp(owner, true, false, true, true);
				}		
			}
			pop.ocp = cp;
		}
	}
	if (inner || tabs) {
		DockableWindow *target = docks.GetMouseDockTarget();		
		if (target && target->IsTabbed()) {
			DockableTabWindow *t = dynamic_cast<DockableTabWindow *>(target->GetParent());
			if (t) target = t;			
		}

		cp = (target ? target->GetScreenView() : wnd).CenterPoint();
			
		if (inner) {
			bool open = pop.inner[0].IsOpen();
			if (open && cp == pop.icp) 
				for (int i = 0; i < 4; i++) pop.inner[i].Refresh();
			else {
				if (outer) {
					int d = sz.cy * 5;
					cp.x = minmax(cp.x, wnd.left + d, wnd.right - d);
					cp.y = minmax(cp.y, wnd.top + d, wnd.bottom - d);
				}
				pop.inner[0].SetRect(r.Offseted(cp.x - sz.cx*3, cp.y - sz.cy));	
				pop.inner[2].SetRect(r.Offseted(cp.x + sz.cx, cp.y - sz.cy));
				pop.inner[1].SetRect(r.Offseted(cp.x - sz.cx, cp.y - sz.cy*3));
				pop.inner[3].SetRect(r.Offseted(cp.x - sz.cx, cp.y + sz.cy));
				pop.inner[4].SetRect(r.Offseted(cp.x-sz.cx, cp.y-sz.cy));
				for (int i = 0; i < 4; i++) {
					pop.inner[i] <<= target ? callback2(&docker, &DockableWindow::DockTo, i, target) 
						: THISBACK2(DoDock, i, &docker);
					if (!open) pop.inner[i].PopUp(owner, true, false, true, true);
				}	
			}
		}
		if (tabs && target) {
			bool open = pop.inner[4].IsOpen();
			if (open && cp == pop.icp) 
				pop.inner[4].Refresh();
			else {
				if (target) {
					pop.inner[4] <<= THISBACK2(DoDockAsTab, target, &docker);
					if (!open) pop.inner[4].PopUp(owner, true, false, true, true);
				}
				else if (open)
					pop.inner[4].Close();
			}
		}	
		else if (pop.inner[4].IsOpen())
			pop.inner[4].Close();
		pop.icp = cp;
	}
}

void DockComplexPopUpCls::HidePupups()
{
	DockPopUps &pop = Single<DockPopUps>();
	// Inner buttons
	if (pop.inner[0].IsOpen())
		for (int i = 0; i < 4; i++)
			pop.inner[i].Close();
	// Outer buttons
	if (pop.outer[0].IsOpen())
		for (int i = 0; i < 4; i++)
			pop.outer[i].Close();
	// Tab button
	if (pop.inner[4].IsOpen())
		pop.inner[4].Close();
	
	//StopHighlight();
}

DockComplexPopUpCls::DockPopUps::DockPopUps()
{
	for (int i = 0; i < 4; i++) {
		inner[i].img = outer[i].img = DockImg::Get(DOCKIMG_OFFSET + i);
	}
	inner[4].img = DockImg::DockTab();
}

