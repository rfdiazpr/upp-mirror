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
	SetType(TYPE_WINDOCK);
	
	hasmenubar 		= false;
	hastoolbar 		= false;
	hasstatusbar 	= false;
	menubar			= NULL;
	toolbar 		= NULL; 
	statusbar 		= NULL;
		
	ToolWindow().Sizeable().Zoomable().TopMost().BackPaint();
	
	_menubutton << THISBACK(OnMenuButton);
	_shutbutton << THISBACK(OnShutButton);
	_autohidebutton << THISBACK(OnAutoHideButton);

	_dragbar.SetDock(this);

	SetStyle(DockCtrlChStyle::StyleDefault());

	int cx = style->barheight - 8;
	int cy = style->barheight - 8;	
	
	_dragbar <<  _shutbutton.RightPos(2, cy).TopPos(2, cy).Tip(t_("Close"));
	_dragbar <<  _autohidebutton.RightPos(cy + 4, cy).TopPos(2, cy).Tip(t_("Auto Hide"));                                                           
	_dragbar <<  _menubutton.RightPos((2 * cy) + 6, cy).TopPos(2, cy).Tip(t_("Window Position"));                                               
	SetFrame(FieldFrame());
	ShowDragBar(); 	
}

DockWindow::~DockWindow()
{
	//GetLabelCtrl().Remove();
}

DockWindow& DockWindow::AddMenuBar(MenuBar& bar)
{
	AddBarCtrl(bar); 
	menubar = &bar; 
	hasmenubar = true;
	AddChildBarSize(0, bar.GetStdHeight());
	return *this;
}

DockWindow& DockWindow::AddToolBar(ToolBar& bar)
{
	AddBarCtrl(bar); 
	toolbar = &bar; 
	hastoolbar = true;
	AddChildBarSize(0, bar.GetStdHeight());
	return *this;
}

DockWindow& DockWindow::AddStatusBar(StatusBar& bar)
{
	AddBarCtrl(bar.Height(18)); 
	statusbar = &bar; 
	hasstatusbar = true;
	return *this;
}

void DockWindow::HideDragBar()
{
	if(!HasDragBar()) return;
	RemoveFrame(_dragbar);
	SubChildBarSize(0,_dragbar.GetHeight()); 
	DockableCtrl::HideDragBar();	
}

void DockWindow::ShowDragBar()
{
	if(HasDragBar()) return;
	InsertFrame(0, _dragbar);
	AddChildBarSize(0, _dragbar.GetHeight()); 
	DockableCtrl::ShowDragBar();
}

Rect DockWindow::GetCtrlRect()
{
	if(!HasDragBar()) return GetScreenRect();
	Rect bar = _dragbar.GetScreenRect();
	Rect wnd = GetScreenRect();
	wnd.top  = bar.top;
	
	return wnd;
}

void DockWindow::Paint(Draw& d)
{
	TopWindow::Paint(d);
}

DockWindow& DockWindow::SetStyle(const DockCtrlChStyle::Style& st)
{
	style = &st;
	_shutbutton.SetImage(style->barshut[0], style->barshut[1], style->barshut[2]);
	_menubutton.SetImage(style->barmenu[0],style->barmenu[1], style->barmenu[2]);
	_autohidebutton.SetImage(style->barhide[0], style->barhide[1], style->barhide[2]);
	RefreshLayoutDeep();
return *this;
}

void DockWindow::MouseMove(Point p, dword keyflags)
{
	TopWindow::MouseMove(p, keyflags);
}

void DockWindow::ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
{
	if(HasDragBar() && child == &_dragbar && event == LEFTDRAG) StartWindowDrag();
	TopWindow::ChildMouseEvent(child, event, p, zdelta, keyflags);
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
					SetDropTarget(DOCK_NONE, GetDropState());
					GetBase().DoDragAndDrop(*this, GetMousePos(), GetCtrlSize());
					break;
		
			case WM_EXITSIZEMOVE:
					if(!GetMouseLeft() && HasDropTarget() && IsDragged()) 
					{
						Dock(GetDropTarget(), GetDropState(), Position());
						Refresh();
						SetDropTarget();
						Dragging(false);
					}
				break;
			
			case WM_NCRBUTTONDOWN:
					ContextMenu(*menubar);
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
	Size sz = GetSize();
	d.DrawImage(0 , 0, sz.cx, sz.cy, (IsPush() && HasMouse() ? _pimage : (!IsPush() && HasMouse() ? _himage : _nimage)));
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
	LayoutFrameTop(r, this, GetHeight());
}

void DockWindow::DragBar::FrameAddSize(Size& s)
{
	s += 4;
	s.cy += GetHeight();
}

void DockWindow::DragBar::Paint(Draw& d)
{
	Size sz = GetSize();
	
	Image img = GetDock().GetIcon();
	bool hasicon = false;
	
	const DockCtrlChStyle::Style* style = GetDock().GetStyle();

	ImageDraw skin(sz.cx, sz.cy);
	ChPaint(skin, 0, 0, GetSize().cx, GetSize().cy, style->barbackground[0]);
	Image imgs = skin;


	(GUI_GlobalStyle() >= GUISTYLE_XP) && (style != &DockCtrlChStyle::StyleClassic()) ? 
	// See CtrlImgs for proper rectangle value.
	d.DrawImage(GetRect(), imgs, Rect(4, 1, 2, 20)) : d.DrawRect(GetSize(), SColorFace());

	if(!img.IsNullInstance())
	{
		hasicon = true;
		d.DrawImage(BAR_MARGIN, GetHeight() / img.GetSize().cy, style->barheight - 8, style->barheight - 8, img);
	}

	d.DrawText(BAR_MARGIN + (BAR_FILEICON + BAR_SPACEICON) * int(hasicon), GetHeight() / 12, GetDock().GetLabel(), style->font);
}

int DockWindow::DragBar::GetWidth()
{
	const DockCtrlChStyle::Style* style = GetDock().GetStyle();
	_titlesize = GetTextSize(GetDock().GetLabel(), style->font);
	return BAR_MARGIN * 2 + _titlesize.cx;
}

int DockWindow::DragBar::GetHeight()
{
	const DockCtrlChStyle::Style* style = GetDock().GetStyle();
	return style->barheight;
}


//----------------------------------------------------------------------------------------------

TabWindow::TabWindow()
{
	SetType(TYPE_TABDOCK);
	
	destroyed	= false;
	position	= -1;
	activectrl	= NULL;

	if(!tabs.IsChild()) AddFrame(tabs.SetLayout(CustomFrame::LAYOUT_BOTTOM));
	if(!pane.IsChild()) Add(pane.SizePos());
	
	tabs.WhenSelect 	= THISBACK(OnActiveTab);
	tabs.WhenClose  	= THISBACK(OnTabClose);
	tabs.WhenContext 	= THISBACK(OnTabContextMenu);
	tabs.WhenDrag		= THISBACK(OnTabDrag);
	tabs.WhenActive		= THISBACK(OnTabActive);
}

TabWindow::~TabWindow()
{
	position 	= -1;
	activectrl 	= NULL;
}

void TabWindow::Attach(DockableCtrl& ctrl, int makeactive)
{
	if(ctrl.GetType() == TYPE_TABDOCK)
		if(((bool) GetBase().DockCtrl::controlpanel.TabOptionNest.Get())) 
			AttachNested(ctrl, makeactive);				 
		else
			AttachNormal(ctrl, makeactive);
	else
		Attach0(ctrl, makeactive);
}


void TabWindow::AttachNormal(DockableCtrl& ctrl, int makeactive)
{
	TabWindow& tabwindow = reinterpret_cast<TabWindow&>(ctrl);
	PaneSplitter& p = tabwindow.GetChilds();
	int ncount =  p.GetChildCount();

	DockableCtrl* lc = p.GetChildAt(ncount);

	if(lc)
	{
		bool transfered = false;
		for(int i = 0; i < ncount - 1 ; i++)
		{
			DockableCtrl* c = p.GetChildAt(i + 1);
			if(c) 
			{
				Attach(*c);
				transfered = true;
			}
		}
		if(transfered)
		{
			Attach(*lc);
		}
		tabwindow.TabWindow::destroyed = true;
	} 
}

void TabWindow::AttachNested(DockableCtrl& ctrl, int makeactive)
{
	Attach0(ctrl, makeactive);
}

void TabWindow::Attach0(DockableCtrl& ctrl, int makeactive)
{
	ctrl.SetOwnerTab(this);
	ctrl.HideDragBar();
	tabs.Add(ctrl, makeactive);
	pane.AddChildDock(ctrl.Style(DOCK_TABBED, STATE_TABBED, 0).SetBase(&GetBase()));
	GetBase().AddCtrlRecord(ctrl);
	if(makeactive) pane.Zoom(GetActiveTab());
	pane.Layout();
	RefreshTabWindowLabel(ctrl);
	GetBase().RefreshWidgetLayout();
	GetBase().RefreshPanel();
}


void TabWindow::Detach(DockableCtrl& ctrl)
{
	if(!ctrl.IsFloating()) ctrl.ShowDragBar();
	pane.RemoveChildDock(ctrl);
	tabs.Close(position);
	if(RemoveTabWindow())
	ctrl.SetOwnerTab(NULL);
	position = -1;
	GetBase().RefreshPanel();
}

void TabWindow::DetachAll()
{
	Ctrl *childctrl = NULL;
	
	DockableCtrl* lastctrl = reinterpret_cast<DockableCtrl*>(pane.GetLastChild());
	while((childctrl = pane.GetFirstChild()))
	{
		DockableCtrl* ctrl = reinterpret_cast<DockableCtrl*>(childctrl);
		if(ctrl)
		{
			if(ctrl->GetType() == TYPE_TABDOCK)
				(reinterpret_cast<TabWindow*>(ctrl))->DetachAll();
			else  ctrl->Shut();
		}
	}
	if(lastctrl) 
	{
		int a = Alignment();
		int s = State();
		int p = Position();
		
		Style(DOCK_NONE, STATE_SHUT, 0);
		if(lastctrl->GetType() == TYPE_TABDOCK)
			(reinterpret_cast<TabWindow*>(lastctrl))->DetachAll();
		else lastctrl->Shut();
		Style(a, s, p);
		Shut();
	}
	destroyed = true;
	GetBase().RefreshPanel();
}

bool TabWindow::RemoveTabWindow()
{
	bool tabbed = false;
	TabWindow *tabwindow = NULL;
	RefreshSizeHint();
	Size sizehint = SizeHint();
	
	if(pane.GetChildCount() == 1)
	{
		DockableCtrl *lastctrl = pane.GetChildAt(1);
		if(lastctrl)
		{
			int a = Alignment();
			int s = State();
			int p = Position();
			
			pane.RemoveChildDock(*lastctrl);
			if(IsShut()) lastctrl->Shut();
			if(IsTabbed())
			{
				TabInterface& tabs = GetOwnerTab()->GetTabs();
				tabs.Close(tabs.GetActiveTab());
				GetOwnerTab()->Attach(*lastctrl);
				Shut();
			}
			else if(IsFloating())
			{
				Rect r = GetRect();
				lastctrl->FloatEx(GetRect());
				lastctrl->SetOwnerTab(NULL);
				Shut();
			}
			else
			{
				Shut();
				lastctrl->ShowDragBar();
				lastctrl->SetSizeHint(sizehint);
				lastctrl->Dock(a, s, p);
				lastctrl->SetOwnerTab(NULL);
			}
			return destroyed = true;
		}
		RemoveFrame(tabs);
	}
	pane.Layout();
	if(GetActiveCtrl()) RefreshTabWindowLabel(*GetActiveCtrl());
return false;
}

void TabWindow::OnMenuButton()
{
	DockWindow::OnMenuButton();
	if(IsShut()) DetachAll();
}

void TabWindow::OnShutButton()
{
	DetachAll();
	Shut();
}

void TabWindow::OnAutoHideButton()
{
	DockWindow::OnAutoHideButton();
}

void TabWindow::OnShutWindow()
{
	OnShutButton();
}


void TabWindow::StartTabAnimation()
{
	GetBase().RefreshWidgetLayout();
	pane.StartAnimation(pane.GetCount() + 1);
	tabs.Add(t_("Tabbed Window"), true);
	pane.Zoom(tabs.GetActiveTab());
}

void TabWindow::StopTabAnimation()
{
	pane.StopAnimation();
	tabs.Close(tabs.GetActiveTab());
	if(!RemoveTabWindow()) return;
	position = -1;
}


bool TabWindow::HasCtrlInRange()
{
	Rect r = GetCtrlRect();
	int hrange = r.Width() 	/ 3;
	int vrange = r.Height()	/ 3;
	return Rect(r.left + hrange, r.top + vrange, r.right - hrange, r.bottom - vrange).Contains(GetMousePos());
}

bool TabWindow::HasTabbedDock()
{
	return pane.HasChild();
}

int TabWindow::ChildCount()
{
	return pane.GetCount();
}

void TabWindow::SetActive(int index)
{
	if(index >= pane.GetCount()) return;
	tabs.SetActiveTab(index);
	pane.Zoom(index);
}

void TabWindow::SetActive(DockableCtrl& ctrl)
{
}

int TabWindow::GetActiveTab()
{
	return tabs.GetActiveTab();
}

DockableCtrl* TabWindow::GetActiveCtrl()
{
	return pane.GetChildAt(GetActiveTab() + 1);
}

void TabWindow::OnActiveTab()
{
	pane.Zoom(tabs.GetActiveTab());
	if(!pane.HasChild()) return;
}

void TabWindow::OnTabClose(int id, DockableCtrl& ctrl)
{
	position = id;
	ctrl.Shut();
}

void TabWindow::OnTabDrag(int id, DockableCtrl& ctrl)
{
	position = id;
	ctrl.StartWindowDrag();
}

void TabWindow::OnTabContextMenu(int id, DockableCtrl& ctrl)
{
	position = id;
	reinterpret_cast<DockWindow&>(ctrl).OnMenuButton();
}

void TabWindow::OnTabActive(int id, DockableCtrl& ctrl)
{
	position = id;
	RefreshTabWindowLabel(ctrl);
}

void TabWindow::RefreshTabWindowLabel(DockableCtrl& ctrl)
{
	int id = 0;
	AutoHideBar* hidebar 	= NULL;
	TabWindow*	 tabwindow	= NULL;

	if(IsTabbed())
	{
		TabWindow* tabwindow = GetOwnerTab();
		while(tabwindow)
		{
			int id = tabwindow->GetActiveTab();
			if(id < 0) break;
			TabInterface::Tab& sourcetab  = GetTabs().GetTabs().At(GetActiveTab());
			TabInterface::Tab& targettab  =	tabwindow->GetTabs().GetTabs().At(id);
			String& label 	= targettab.name = sourcetab.name;
			Image& icon 	= targettab.icon = sourcetab.icon;
			tabwindow->GetTabs().ReposTabs();
			if(!tabwindow->GetOwnerTab())
			{
				if(tabwindow->IsAutoHidden())
				{
					hidebar = tabwindow->GetOwnerBar();
					id = hidebar->Find((DockableCtrl&)*tabwindow);
					if(id >= 0)
					{
						TabInterface::Tab& t = hidebar->GetTabs().At(id);
						t.name = label;
						t.icon = icon;
						hidebar->ReposTabs();
						hidebar->Refresh();
					}
						
				}
				tabwindow->SetLabel(label);
				tabwindow->SetIcon (icon);
			}
			tabwindow->RefreshLayoutDeep();
			tabwindow = tabwindow->GetOwnerTab();
		}
	}
	else 
	{
		if(IsAutoHidden())
		{
			hidebar = GetOwnerBar();
			id = hidebar->Find((DockableCtrl&)*this);
			if(id >= 0)
			{
				TabInterface::Tab& t = hidebar->GetTabs().At(id);
				t.name = ctrl.GetLabel();
				t.icon = ctrl.GetIcon();
				hidebar->ReposTabs();
				hidebar->Refresh();
			}
		}
		DockableCtrl::SetLabel(ctrl.GetLabel());
		DockableCtrl::SetIcon(ctrl.GetIcon());
		GetTabs().ReposTabs();
		RefreshLayoutDeep();
	}
}
