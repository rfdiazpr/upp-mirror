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
	dockalignment 	= DOCK_NONE;
	dockstate		= STATE_SHUT;
	dockposition	= 0;
	isdropped 		= false;
	isdropped		= false;
	dndtarget		= DOCK_NONE;
	dndstate		= 0;
	hasdragbar		= false;
	haschildbar		= false;
	base			= NULL;
	WhenClose		= THISBACK(OnShutWindow);
	size.cx			= 0;
	size.cy			= 0;
	childsize.SetNull();	
}

DockableCtrl::~DockableCtrl()
{
}

DockableCtrl& DockableCtrl::Style(int alignment, int state, int position)
{
	dockalignment 	= alignment;
	dockstate		= state;
	dockposition	= position;
	return *this;
}

void DockableCtrl::Dock(int alignment, int state, int position, bool check)
{
	if(Alignment() == alignment && !IsHidden()) return;
	else if(IsTabbed())
	{
		GetOwnerTab().Detach(*this);
		if(state == STATE_TABBED) state = STATE_SHOW;
	}
	else if(IsFloating()) 
	{
		ShutWindow();
	}
	else if(IsAutoHidden())
	{
		GetBase().GetHideBar(Alignment()).Detach(*this);
		if(state == STATE_AUTO) state = STATE_SHOW;
	}
	else 
	{
		RefreshSizeHint();
		GetBase().Detach(*this);
	}
	Style(alignment, state, position);
	GetBase().Dock(*this);	

}

void DockableCtrl::FloatEx(Rect& r)
{
	if(IsFloating()) return;
	if(IsTabbed()) 
	{
		Size sz = GetOwnerTab().SizeHint();
		GetOwnerTab().Detach(*this);
		Alignment() == DOCK_LEFT || 
		Alignment() == DOCK_RIGHT ?
		r.right  = r.left + sz.cx :
		r.bottom = r.top + 	sz.cy; 	
	}
	else if(IsAutoHidden())
	{
	//	r.right = r.left + size.cx;
	//	r.bottom = r.top + size.cy;
		GetBase().GetHideBar(Alignment()).Detach(*this);
	}
	else if(!IsShut()) 
		GetBase().Detach(*this);

	OpenWindow(r);
	GetBase().RefreshPanel();
}

void DockableCtrl::Float()
{
	if(IsFloating()) return;
	Rect  r  =  GetScreenView();
	Point p  = GetMousePos();
	if(r.top > p.y) r.top -= r.top - p.y - 4;
	r.right	 = r.left + r.Width();
	r.bottom = r.top + r.Height();
	FloatEx(r);
}

void DockableCtrl::Show()
{
	if(!IsHidden() || IsTabbed()) return;
	Dock(Alignment(), STATE_SHOW, Position());
	GetBase().RefreshPanel();
}

void DockableCtrl::Hide()
{
	if(IsTabbed()) return;
	if(IsHidden())
	{
		Show();
		return;
	}
	ShutWindow();
	GetBase().Detach(*this);
	Style(Alignment(), STATE_HIDE, Position());
	GetBase().RefreshPanel();
}

void DockableCtrl::AutoHide()
{
	if(IsAutoHidden())
	{
		GetBase().GetHideBar(Alignment()).Detach(*this);
		GetBase().Dock(this->Style(Alignment(), STATE_SHOW, Position()));
		return;
	}
	RefreshSizeHint();
	if(IsTabbed())
		GetOwnerTab().Detach(*this);
	else
	{
		RefreshSizeHint();
		GetBase().Detach(*this);
	}
	Style(Alignment(), STATE_AUTO, Position());
	GetBase().Dock(*this);
	GetBase().RefreshPanel();
}

void DockableCtrl::Shut()
{
	if(IsShut()) return;
	ShutWindow();
	if(IsTabbed()) 
		GetOwnerTab().Detach(*this);
	else if(IsAutoHidden())
		GetBase().GetHideBar(Alignment()).Detach(*this);
	else 
		GetBase().Detach(*this);

	if(GetType() == TYPE_TABDOCK)
		(reinterpret_cast<TabWindow&>(*this)).DetachAll();
	
	Style(DOCK_NONE, STATE_SHUT, 0);
	GetBase().RefreshPanel();
}

void DockableCtrl::OpenWindow(Rect& r)
{
	if(IsPopUp()) Close();
	if(IsOpen()) return;
	RefreshSizeHint();
	r.right  = r.left + size.cx;
	r.bottom = r.top + size.cy;
	if(HasDragBar()) HideDragBar();
	Title(GetLabel());
	SetRect(r);
	Open(GetTopWindow());
	Refresh();
	Style(DOCK_NONE, STATE_SHOW, 0);
}

void DockableCtrl::ShutWindow()
{
	if(IsPopUp()) Close();
	RefreshSizeHint();
	if(!IsOpen() || !IsFloating()) return;
	Rect r 	 = GetRect();
	r.right  = r.left + size.cx;
	r.bottom = r.top + size.cy;
	Close();
	Remove();
	SetRect(r);  
	if(!HasDragBar()) ShowDragBar();
}

void DockableCtrl::OnShutWindow()
{
	ShutWindow(); if(!IsShut()) Shut();
}

void DockableCtrl::Settings()
{
	GetBase().ControlPanel();
}

void DockableCtrl::Menu()
{
	ContextMenu(menu);
}

void DockableCtrl::ContextMenu(Bar& menubar)
{
	MenuBar::Execute(THISBACK(DockableCtrlMenu)); 
}

void DockableCtrl::DockableCtrlMenu(Bar& menubar)
{
	menubar.Add(t_("Dock"),	THISBACK(DockableCtrlDockMenu));
	menubar.Add(!IsFloating(), t_("Float"),THISBACK(Float));
	menubar.Separator();
	menubar.Add(IsDocked(), IsHidden() ? t_("Show") : t_("Hide"), THISBACK(Hide));
	menubar.Add(!IsAutoHidden(), t_("Auto Hide"), THISBACK(AutoHide));	
	menubar.Separator();
	menubar.Add(t_("Settings"), THISBACK(Settings));
	menubar.Separator();
	menubar.Add(t_("Close"), DockableCtrlImages::CClose(), THISBACK(Shut));
	
}

void DockableCtrl::DockableCtrlDockMenu(Bar& menubar)
{
	menubar.Add(Alignment() == DOCK_TOP  ? 0 : 1, t_("Top"), callback4(this, &DockableCtrl::Dock, DOCK_TOP, State(), Position(), 0));
	menubar.Add(Alignment() == DOCK_LEFT  ? 0 : 1, t_("Left"), callback4(this, &DockableCtrl::Dock, DOCK_LEFT, State(), Position(), 0));
	menubar.Add(Alignment() == DOCK_RIGHT ? 0 : 1, t_("Right"), callback4(this, &DockableCtrl::Dock, DOCK_RIGHT, State(), Position(), 0));
	menubar.Add(Alignment() == DOCK_BOTTOM ? 0 : 1, t_("Bottom"), callback4(this, &DockableCtrl::Dock, DOCK_BOTTOM, State(), Position(), 0));
}

void DockableCtrl::StartWindowDrag()
{
	Point pt = GetMousePos();
	if(IsTabbed())
		FloatEx(Rect(pt.x - 4, pt.y + 4, pt.x + size.cx, pt.y + size.cy));
	else 
		Float();
	
#ifdef PLATFORM_WIN32
	SendMessage(GetHWND(), WM_NCLBUTTONDOWN, 2, MAKELONG(pt.x, pt.y));
#endif
}

DockableCtrl& DockableCtrl::SetLabel(String docktitle)
{
	IsNull(docktitle) || docktitle == "" ? title = t_("Untitled") : title = docktitle;
	if(IsVisible()) Refresh();
	return *this;
}

DockableCtrl& DockableCtrl::SetIcon(Image dockicon)
{
	icon = dockicon;
	return *this;
}

void DockableCtrl::RefreshSizeHint()
{
	size = GetSize();
	for(int i = 0; i < GetFrameCount(); i++)
		GetFrame(i).FrameAddSize(size);
}

