//==============================================================================================
// DockCtrl: A dockable widget framework for U++
// Author:	 Ismail YILMAZ (A.k.a. Maldoror)
// Version:	 0.49.9.9a
//==============================================================================================

#include "DockCtrl/DockCtrl.h"

#define IMAGEFILE <DockCtrl/DockableCtrl.iml>
#define IMAGECLASS DockWindowImages
#include <Draw/iml.h>

//===============================================
// DockWindow class:
// A dockable window widget.
//===============================================

DockWindow::DockWindow() : DockableCtrl()
{
	_hasmenubar = _hastoolbar = _hasstatusbar = false;
	_toolbar = NULL; _statusbar = NULL;
	
	ToolWindow().Sizeable().Zoomable().TopMost().BackPaint();
	
	_menubutton << THISBACK(OnMenuButton);
	_shutbutton << THISBACK(OnShutButton);
	_autohidebutton << THISBACK(OnAutoHideButton);
	
	_dragbar <<  _shutbutton.RightPos(2,  14).TopPos(2, 14).Tip(t_("Close"));
	_dragbar <<  _autohidebutton.RightPos(18, 16).TopPos(2, 14).Tip(t_("Auto Hide"));                                                           
	_dragbar <<  _menubutton.RightPos(34, 18).TopPos(2, 14).Tip(t_("Window Position")) << GetLabelCtrl().SizePos().LeftPos(6);                                                 

	_dragbar.SetDock(this);
	_dragbar.Add(GetLabelCtrl().SetAlign(ALIGN_LEFT));
	
	SetVisualStyle(ModernStyle()); //default;
	SetFrame(FieldFrame());
	ShowDragBar(); 
}

DockWindow::~DockWindow()
{
}

DockWindow& DockWindow::AddMenuBar(MenuBar& menubar)
{
	AddBarCtrl(menubar); _menubar = &menubar; _hasmenubar = true;
	AddChildBarSize(0, menubar.GetStdHeight());
	return *this;
}

DockWindow& DockWindow::AddToolBar(ToolBar& toolbar)
{
	AddBarCtrl(toolbar); _toolbar = &toolbar; _hastoolbar = true;
	AddChildBarSize(0, toolbar.GetStdHeight());
	return *this;
}

DockWindow& DockWindow::AddStatusBar(StatusBar& statusbar)
{
	AddBarCtrl(statusbar.Height(18)); _statusbar = &statusbar; _hasstatusbar = true;
	return *this;
}

void DockWindow::HideDragBar()
{
	if(!HasDragBar()) return;
	RemoveFrame(_dragbar);
	SubChildBarSize(0, 22); DockableCtrl::HideDragBar();	
}

void DockWindow::ShowDragBar()
{
	if(HasDragBar()) return;
	InsertFrame(0, _dragbar);
	AddChildBarSize(0, 22); DockableCtrl::ShowDragBar();
}

Rect DockWindow::GetDockRect()
{
	if(!HasDragBar()) return GetScreenRect();

	Rect bar = _dragbar.GetScreenRect();
	Rect wnd = GetScreenRect();
	wnd.top  = bar.top;
	
	return wnd;
}

Rect DockWindow::GetDockView()
{
	return GetDockRect();
}

void DockWindow::ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
{
	if(HasDragBar() && child == &_dragbar && event == LEFTDRAG)
	{
		Float(); 
		SendMessage(GetHWND(), WM_NCLBUTTONDOWN, 2, MAKELONG(p.x, p.y));
	}
	DockableCtrl::ChildMouseEvent(child, event, p, zdelta, keyflags);
}

void DockWindow::LeftDown(Point p, dword keyflags)
{	
	TopWindow::LeftDown(p, keyflags);
}

void DockWindow::LeftUp(Point p, dword keyflags)
{
	TopWindow::LeftUp(p, keyflags);
}

void DockWindow::RightDown(Point p, dword keyflags)
{
	DockableCtrl::RightDown(p,keyflags);
}
void DockWindow::RightUp(Point p, dword keyflags)
{
	DockableCtrl::RightUp(p, keyflags);
}

void DockWindow::Paint(Draw& d)
{
	TopWindow::Paint(d);
}

void DockWindow::MouseMove(Point p, dword keyflags)
{
}

void DockWindow::SetVisualStyle(const Style& st)
{
	_visualstyle = &st;
	_shutbutton.SetImage(_visualstyle->shutbuttonimage[0], _visualstyle->shutbuttonimage[1], _visualstyle->shutbuttonimage[2]);
	_menubutton.SetImage(_visualstyle->menubuttonimage[0],_visualstyle->menubuttonimage[1], _visualstyle->menubuttonimage[2]);
	_autohidebutton.SetImage(_visualstyle->autohidebuttonimage[0], _visualstyle->autohidebuttonimage[1], _visualstyle->autohidebuttonimage[2]);
	RefreshLayoutDeep();
}

#ifdef PLATFORM_WIN32
LRESULT  DockWindow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{

	if(IsFloating())
	{
		switch(message)
		{
			case WM_MOVE:
					Dragging(true);
					GetBase().DoDragAndDrop(*this, GetMousePos());
					break;
		
			case WM_EXITSIZEMOVE:
					if(!GetMouseLeft() && DnDHasTarget() && IsDragged()) 
					{
						Dock(DnDGetTarget(), State(), Position());
						Refresh();
						DnDTargetoutofRange();
						Dragging(false);
					}
				break;
			
			case WM_NCRBUTTONDOWN:
					ContextMenu(*_menubar);
				return 1L;
				
			default:
				break;
		}
	}
	return TopWindow::WindowProc(message, wParam, lParam);
}
#endif

//===============================================
// DockWindow::DragBarButton
// Nested helper class for DockWindow
//===============================================

Ctrl& DockWindow::DragBarButton::SetImage(const Image& normal, const Image& highlighted, const Image& pushed)
{
	_nimage = normal; _himage = highlighted; _pimage = pushed;
	return *this;
}

void DockWindow::DragBarButton::Paint(Draw& d)
{
 	d.DrawImage(0, 0, (IsPush() && HasMouse() ? _pimage : (!IsPush() && HasMouse() ? _himage : _nimage)));
}

//===============================================
// DockWindow::DragBar
// Nested helper class for DockWindow
//===============================================



void DockWindow::DragBar::FrameAdd(Ctrl& parent)
{
	parent.Add(*this);
}

void DockWindow::DragBar::FrameRemove()
{
	Ctrl::Remove();
}

void DockWindow::DragBar::FrameLayout(Rect& r)
{
	LayoutFrameTop(r, this, 22);
}

void DockWindow::DragBar::FrameAddSize(Size& s)
{
	s += Size(0, 22);
}

void DockWindow::DragBar::Paint(Draw& d)
{

	const Style* style = GetDock().GetVisualStyle();
	(GUI_GlobalStyle() >= GUISTYLE_XP) && (style != &ClassicStyle()) ? 
		d.DrawImage(0, 0, GetSize().cx, GetSize().cy, style->barbackgroundimage) : 
			d.DrawRect(GetSize(), SColorFace());
}

CH_STYLE(DockWindow, Style, ClassicStyle)
{
	barbackgroundimage = DockWindowImages::I_DragBarModernN;
	shutbuttonimage[0] = DockWindowImages::I_CloseN;
	shutbuttonimage[1] = DockWindowImages::I_CloseH;
	shutbuttonimage[2] = DockWindowImages::I_CloseP;
	autohidebuttonimage[0]  = DockWindowImages::I_HideN;
	autohidebuttonimage[1]  = DockWindowImages::I_HideH;
	autohidebuttonimage[2]  = DockWindowImages::I_HideP;
	menubuttonimage[0]  = DockWindowImages::I_DockMenuN;
	menubuttonimage[1]  = DockWindowImages::I_DockMenuH;
	menubuttonimage[2]  = DockWindowImages::I_DockMenuP;
}

CH_STYLE(DockWindow, Style, EnhancedStyle)
{
	barbackgroundimage = DockWindowImages::I_DragBarEnhancedN;
	shutbuttonimage[0] = DockWindowImages::I_CloseN;
	shutbuttonimage[1] = DockWindowImages::I_CloseH;
	shutbuttonimage[2] = DockWindowImages::I_CloseP;
	autohidebuttonimage[0]  = DockWindowImages::I_HideN;
	autohidebuttonimage[1]  = DockWindowImages::I_HideH;
	autohidebuttonimage[2]  = DockWindowImages::I_HideP;
	menubuttonimage[0]  = DockWindowImages::I_DockMenuN;
	menubuttonimage[1]  = DockWindowImages::I_DockMenuH;
	menubuttonimage[2]  = DockWindowImages::I_DockMenuP;
}

CH_STYLE(DockWindow, Style, ModernStyle)
{
	barbackgroundimage = DockWindowImages::I_DragBarModernN;
	shutbuttonimage[0] = DockWindowImages::I_CloseModernN;
	shutbuttonimage[1] = DockWindowImages::I_CloseModernH;
	shutbuttonimage[2] = DockWindowImages::I_CloseModernP;
	autohidebuttonimage[0]  = DockWindowImages::I_HideModernN;
	autohidebuttonimage[1]  = DockWindowImages::I_HideModernH;
	autohidebuttonimage[2]  = DockWindowImages::I_HideModernP;
	menubuttonimage[0]  = DockWindowImages::I_DockMenuModernN;
	menubuttonimage[1]  = DockWindowImages::I_DockMenuModernH;
	menubuttonimage[2]  = DockWindowImages::I_DockMenuModernP;
}
