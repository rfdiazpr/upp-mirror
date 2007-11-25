//==============================================================================================
// DockCtrl: A dockable widget framework for U++
// Author:	 Ismail YILMAZ (A.k.a. Maldoror)
// Version:	 0.49.9.9a
//==============================================================================================

#include "DockCtrl/DockCtrl.h"

#define IMAGEFILE <DockCtrl/DockableCtrl.iml>
#define IMAGECLASS DockableCtrlImages
#include <Draw/iml.h>

//===============================================
// DockableCtrl class:
// Base class of all DockCtrl widgets.
//===============================================

DockableCtrl::DockableCtrl() 
{
	_alignment = dock_none; _state = state_shut; _position = 0;
	_dragstate = _dropstate = false; _dndtarget = dock_none;
	_hasdragbar = false; _haschildbarctrl = false;

	_childsize.Clear();
	WhenClose = THISBACK(OnFloatWindowShut);
	
	_dockctrl = NULL;
}

DockableCtrl::~DockableCtrl()
{
}

void DockableCtrl::Dock(int alignment, int state, int position, bool check)
{
	if(Alignment() == alignment && !IsHidden()) 
		return;
	if(IsDocked()) Remove();
		else
	if(IsFloating())
	{
		ShutFloatWindow();
		Style(alignment, state, position).GetBase().AddDock(this);
		return;
	}
	GetBase().RemoveDock(this);
	Style(alignment, state, position).GetBase().AddDock(this);
}

void DockableCtrl::FloatEx(Rect& windowrect)
{
	if(!IsFloating()) 
	{
		if(!IsShut()) 
		{
			Remove();
			GetBase().RemoveDock(this);
		}
		OpenFloatWindow(windowrect);
	}

}

void DockableCtrl::Float()
{
	Rect windowrect   = GetScreenView();
	windowrect.top   -= (GetChildBarSize().cy - 22);
	windowrect.right  = windowrect.left + windowrect.Width();
	windowrect.bottom = windowrect.top + windowrect.Height();
	FloatEx(windowrect);
}

void DockableCtrl::Show()
{
	if(IsHidden()) Dock(Alignment(), state_show, Position());
}

void DockableCtrl::Hide()
{ 
	if(!IsHidden() && !IsFloating()) 
	{
		ShutFloatWindow();
		GetBase().RemoveDock(this);
		Style(Alignment(), state_hide, Position());
	}
}

void DockableCtrl::AutoHide()
{
}

void DockableCtrl::Shut()
{
	ShutFloatWindow();
	GetBase().RemoveDock(this);
	Style(dock_none, state_shut, Position());
}

void DockableCtrl::OpenFloatWindow(Rect windowrect)
{
	if(!IsOpen())
	{
		if(HasDragBar()) HideDragBar();
		Title(GetLabel()).SetRect(windowrect);
		Open(GetTopWindow()); 
		Refresh();
		Style(dock_none, state_show, Position());
	}		
}

void DockableCtrl::ShutFloatWindow()
{
	if(IsOpen()) Close();
	Remove();
	if(!HasDragBar()) ShowDragBar();
}

void DockableCtrl::ContextMenu(Bar& menubar)
{
	MenuBar::Execute(THISBACK(DockableCtrlMenu)); 
}

void DockableCtrl::DockableCtrlMenu(Bar& menubar)
{
	menubar.Add(t_("Dock"),	THISBACK(DockableCtrlDockMenu));
	menubar.Add(IsDocked(), t_("Float"),THISBACK(Float));
	menubar.Separator();
	menubar.Add(IsDocked(), t_("Hide"), THISBACK(Hide));
	menubar.Add(0, t_("Auto Hide"), THISBACK(AutoHide));	
	menubar.Separator();
	menubar.Add(t_("Close"), DockableCtrlImages::I_CloseN(), THISBACK(Shut));
	
}

void DockableCtrl::DockableCtrlDockMenu(Bar& menubar)
{
	menubar.Add(Alignment() == dock_top    ? 0 : 1, t_("Top"), callback4(this, &DockableCtrl::Dock, dock_top, State(), Position(), 0));
	menubar.Add(Alignment() == dock_left   ? 0 : 1, t_("Left"), callback4(this, &DockableCtrl::Dock, dock_left, State(), Position(), 0));
	menubar.Add(Alignment() == dock_right  ? 0 : 1, t_("Right"), callback4(this, &DockableCtrl::Dock, dock_right, State(), Position(), 0));
	menubar.Add(Alignment() == dock_bottom ? 0 : 1, t_("Bottom"), callback4(this, &DockableCtrl::Dock, dock_bottom, State(), Position(), 0));
}

void DockableCtrl::DnDDragStart()
{
	Point p = GetMousePos();
	Float(); 
#ifdef PLATFORM_WIN32
	SendMessage(GetHWND(), WM_NCLBUTTONDOWN, 2, MAKELONG(p.x, p.y));
#endif
}

bool DockableCtrl::DnDCalcDragPos(Rect dock, Rect tracker)
{
	  return ((((tracker.right >= dock.left) && (tracker.right <= dock.right)) && ((tracker.bottom <= dock.bottom) && (tracker.bottom >= dock.top))) 	||
	   (((tracker.top >= dock.top) && (tracker.top <= dock.bottom)) 		&& ((tracker.left >= dock.left) && (tracker.left <= dock.right))) 		||
	   (((tracker.top >= dock.top) && (tracker.top <= dock.bottom)) 		&& ((tracker.right >= dock.left) && (tracker.right <= dock.right)))		||
	   (((tracker.left >= dock.left) && (tracker.left <= dock.right)) 		&& ((tracker.bottom >= dock.top) && (tracker.bottom <= dock.bottom))));
}

bool DockableCtrl::DnDCalcDropPos(Rect pane, Point pos)
{
	return ((pos.x > pane.left) && (pos.y > pane.top)) && ((pos.x < pane.right) && (pos.y < pane.bottom));
}
