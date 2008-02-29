#ifndef _Docking_DockMenu_h_
#define _Docking_DockMenu_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

class DockWindow;

class DockMenu 
{
public:
	typedef DockMenu CLASSNAME;

	DockMenu(DockWindow *dockwindow = NULL) : dock(dockwindow)	{  }

	void Set(DockWindow &dockwindow)						{ dock = &dockwindow; }

	void LayoutListMenu(Bar &bar);
	void GroupListMenu(Bar &bar);
	void WindowListMenu(Bar &bar, String group);	
	
	void GroupMenu(Bar &bar, String group);
	void GroupWindowsMenu(Bar &bar, String group);
	void GroupDockMenu(Bar &bar, String group) 				{ GroupAlignMenu(bar, group, 0); }
	void GroupHideMenu(Bar &bar, String group) 				{ GroupAlignMenu(bar, group, 1); }
	void GroupTabDockMenu(Bar &bar, String group)			{ GroupAlignMenu(bar, group, 2); }

	void WindowMenu(Bar &bar, DockableCtrl *dc);
	void WindowDockMenu(Bar &bar, DockableCtrl *dc)			{ WindowAlignMenu(bar, dc, true); }
	void WindowHideMenu(Bar &bar, DockableCtrl *dc)			{ WindowAlignMenu(bar, dc, false); }
private:
	DockWindow *dock;

	void WindowAlignMenu(Bar &bar, DockableCtrl *dc, bool dock);		
	void GroupAlignMenu(Bar &bar, String group, int mode);		
	
	void MenuDock(int align, DockableCtrl *dc);
	void MenuFloat(DockableCtrl *dc);
	void MenuAutoHide(int align, DockableCtrl *dc);	
	void MenuClose(DockableCtrl *dc);
	
	void MenuLoadLayout(int ix);
};
#endif
