#include "DockInterface.h"

void DockInsertAnimatorCls::AnimateDock(int align, DockBase &base, DockableWindow &wnd)
{
	DragLoop &loop = Single<DragLoop>();
	loop.limit = wnd.GetScreenRect();
	DoDock(align, &wnd);
	Ctrl::ProcessEvents();
	loop.SetMaster(wnd);
	loop.Run();
	if (loop.limit.Contains(GetMousePos()))
		return;
	else
		DockSimpleDnD().DragStart(base, wnd);
}

void DockInsertAnimatorCls::AnimateDock(int align, DockBase &base, DockableWindow &target, DockableWindow &wnd)
{
	DragLoop &loop = Single<DragLoop>();
	loop.limit = wnd.GetScreenRect();
	loop.SetMaster(wnd);
	PostCallback(callback2(&wnd, &DockableWindow::DockTo, align, &target) );
	loop.Run();
	if (loop.limit.Contains(GetMousePos()))
		return;
	else
		DockSimpleDnD().DragStart(base, wnd);
}

void DockInsertAnimatorCls::AnimateOff(int align, DockBase &base, DockableWindow &wnd)
{
	
}

