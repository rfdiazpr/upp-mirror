#ifndef _WithXMLMenu_h_
#define _WithXMLMenu_h_

#include "XMLMenuInterface.h"
#include "XMLMenuEditor.h"

NAMESPACE_UPP

// main XML menu template
template<class T> class WithXMLMenu : public T, public XMLMenuInterface
{
	private:
	
		// the available commands
		XMLCommands commands;
	
		// the configurable toolbars
		XMLToolBars toolBars;
		
		// the configurable menu bars
		// the bar named 'Main' is the main menu
		// others are context menus
		XMLToolBars menuBars;
	
		// the four corner XMLToolbarFrame frames
		XMLToolBarFrame topFrame;
		XMLToolBarFrame bottomFrame;
		XMLToolBarFrame leftFrame;
		XMLToolBarFrame rightFrame;
		
		// the menu bars
		Array<MenuBar> menuBarCtrls;
	
		// toolbar controls storage area
		Array<XMLToolBarCtrl> toolBarCtrls;
		
		// flags stating allowed dock point and main menu bar
		bool dockTop, dockBottom, dockLeft, dockRight;
		bool mainMenu;
		
		// refrash frames when menu/toolbars changed
		// or docking points changed
		void RefreshFrames(void);
		
		// dragging flag
		bool dragging;

		// toolbar being dragged		
		Ptr<XMLToolBarCtrl> dragToolBar;
		
		// pre-docking stuffs
		Ptr<XMLToolBarFrame> preDockFrame;
		
		// menu and toolbars setter callbacks
		void SetMenuBar0(Bar &bar, int mnIdx, Array<XMLToolBarItem> const *items);
		void SetMenuBar(Bar &bar, int mnIdx) { SetMenuBar0(bar, mnIdx, NULL); }

		void SetToolBar0(Bar &bar, int tbIdx, Array<XMLToolBarItem> const *items);
		void SetToolBar(Bar &bar, int tbIdx) { SetToolBar0(bar, tbIdx, NULL); }
		
		// calls user command handler, passing command ID to it
		void callUserHandler(String const &s) { UserCmdHandler(s); }
		
		// docks/undocks/hide a toolbar
		WithXMLMenu<T> &Reposition(XMLToolBarCtrl *tb, XMLToolBarState state, int aPos = -1, int bPos = -1);
		
		// query a dock frame under screen point
		XMLToolBarFrame *QueryDockFrame(Point p);

		// child frame mouse event handler captures the click on toolbar's frames
		virtual void ChildFrameMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags);
		
		// mouse events sent from floating toolbar
		virtual void FloatingDraggedEvent(XMLToolBarCtrl &tb, Point p);

		// run the menu editor
		void runEditorCb(void) { XMLMenuEditor(this).RunAppModal(); }
		
		// right click context menu
		void ContextMenu(Bar& bar) { bar.Add(t_("Customize"), THISBACK(runEditorCb)); }

		// right click event sent from various controls
		virtual void RightClickEvent(Point p) { MenuBar::Execute(THISBACK(ContextMenu)); }

		// the drag loop
		void DragLoop(Point startP);
		
	public:
	
		typedef WithXMLMenu<T> CLASSNAME;
	
		WithXMLMenu();
		~WithXMLMenu();
		
		// user commands handler
		Callback1<String> UserCmdHandler;
		
		// sets builtin commands
		void SetCommands(Callback1<XMLCommands &> cmds);
		
		// sets menu entries
		void SetMenuBars(Callback1<XMLToolBars &> tb);

		// sets toolbars entries
		void SetToolBars(Callback1<XMLToolBars &> tb);
		
		// gets/sets commands, menu and toolbars
		virtual XMLCommands const &GetCommands(void) { return commands; }
		
		// controls docking and main menu behaviour
		WithXMLMenu<T> &DockTop(bool b = true)		{ dockTop = b;		RefreshFrames(); return *this; } 
		WithXMLMenu<T> &NoDockTop(void)				{ return DockTop(false); }
		WithXMLMenu<T> &DockBottom(bool b = true)	{ dockBottom = b;	RefreshFrames(); return *this; } 
		WithXMLMenu<T> &NoDockBottom(void)			{ return DockBottom(false); }
		WithXMLMenu<T> &DockLeft(bool b = true)		{ dockLeft = b;		RefreshFrames(); return *this; } 
		WithXMLMenu<T> &NoDockLeft(void)			{ return DockLeft(false); }
		WithXMLMenu<T> &DockRight(bool b = true)	{ dockRight = b;	RefreshFrames(); return *this; } 
		WithXMLMenu<T> &NoDockRight(void)			{ return DockRight(false); }
		
		WithXMLMenu<T> &MainMenu(bool b = true)		{ mainMenu = b; RefreshFrames(); return *this; }
		WithXMLMenu<T> &NoMainMenu(void)			{ return MainMenu(false); }
		
		// gets a context menu by name -- NULL if none
		MenuBar *GetContextMenu(String const &name);
};

template<class T> WithXMLMenu<T>::WithXMLMenu() :
	topFrame		(TOOLBAR_TOP),
	bottomFrame		(TOOLBAR_BOTTOM),
	leftFrame		(TOOLBAR_LEFT),
	rightFrame		(TOOLBAR_RIGHT)
{
	// allows, by default, docking on 4 corners and embeds
	// main menu too
	dockTop = dockBottom = dockLeft = dockRight = true;
	mainMenu = true;
	
	dragging = false;
	dragToolBar = NULL;
	
	preDockFrame = NULL;

	RefreshFrames();
	
}

template<class T> WithXMLMenu<T>::~WithXMLMenu()
{
}

// refrash frames when menu/toolbars changed
// or docking points changed
template<class T> void WithXMLMenu<T>::RefreshFrames(void)
{
	// removes all frames
	T::ClearFrames();
	
	// adds main menu
	if(mainMenu)
	{
		int mainIdx = menuBars.Find("Main");
		if(mainIdx >= 0)
		{
			MenuBar &bar = menuBarCtrls[mainIdx];
			T::AddFrame(bar);
		}
	}

	// add docking frames
	if(dockTop)
		T::AddFrame(topFrame);
	if(dockBottom)
		T::AddFrame(bottomFrame);
	if(dockLeft)
		T::AddFrame(leftFrame);
	if(dockRight)
		T::AddFrame(rightFrame);
}
		

// docks/undocks/hide a toolbar
template<class T> WithXMLMenu<T> &WithXMLMenu<T>::Reposition(XMLToolBarCtrl *tb, XMLToolBarState state, int aPos, int bPos)
{
	switch(state)
	{
		case TOOLBAR_CLOSED :
			tb->Close();
			break;
			
		case TOOLBAR_FLOATING :
			tb->Float(Point(aPos, bPos));
			break;
			
		case TOOLBAR_TOP :
			tb->Dock(topFrame, aPos, bPos);
			break;
			
		case TOOLBAR_BOTTOM :
			tb->Dock(bottomFrame, aPos, bPos);
			break;
			
		case TOOLBAR_LEFT :
			tb->Dock(leftFrame, aPos, bPos);
			break;
			
		case TOOLBAR_RIGHT :
			tb->Dock(rightFrame, aPos, bPos);
			break;
			
		default:
			NEVER();
	}
	return *this;
}

// query a dock frame under point
template<class T> XMLToolBarFrame *WithXMLMenu<T>::QueryDockFrame(Point p)
{
	// reset point to be on control rect
	if(dockTop && topFrame.Contains(p))
		return &topFrame;
	else if(dockBottom && bottomFrame.Contains(p))
		return &bottomFrame;
	else if(dockLeft && leftFrame.Contains(p))
		return &leftFrame;
	else if(dockRight && rightFrame.Contains(p))
		return &rightFrame;
	else
		return NULL;
}

// child frame mouse event handler captures the click on toolbar's frames
template<class T> void WithXMLMenu<T>::ChildFrameMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
{
	// handles just left down event
	if(event != Ctrl::LEFTDOWN)
		return;
	
	// ignore re-entrant events
	if(dragging)
		return;

	// reacts just to events related to XMLToolBar
	dragToolBar = dynamic_cast<XMLToolBarCtrl *>(child);
	if(!dragToolBar)
		return;

	// setup drag point for dragging the toolbar
	Point dragPoint = p + dragToolBar->GetScreenRect().TopLeft();
	
	// execute the drag loop
	DragLoop(dragPoint);
}

// mouse events sent from floating toolbar
template<class T> void WithXMLMenu<T>::FloatingDraggedEvent(XMLToolBarCtrl &tb, Point p)
{
	// ignore event if toolbar is not floating
	// should not happen, but.....
	if(!tb.GetIsFloating())
		return;
	
	// ignode re-entrant events
	if(dragging)
		return;

	// setup drag toolbar
	dragToolBar = &tb;
	
	// run drag loop
	DragLoop(p);
}

template<class T> void WithXMLMenu<T>::DragLoop(Point dragPoint)
{
	// marks dragging active to avoid re-entrant events
	dragging = true;
	
	// check if dragtoolbar is valid -- interna error if not
	ASSERT(dragToolBar);
	
	// ignore mouse events on main control, when dragging
	Ctrl::IgnoreMouse();
	
	// popup-ize the toolbar
	dragToolBar->Popup(dragPoint);

#ifdef PLATFORM_POSIX
	// this is needed for X11, otherwise the GetMouseLeft() function
	// may return false even if mouse is down after un-floating
	Sleep(30);
	dragToolBar->SetCapture();
#endif

	// loop up to mouse button is released
	Point ps, pp;
	do
	{
		Ctrl::ProcessEvents();
		ps = GetMousePos();
		pp = ps - Ctrl::GetRect().TopLeft();
		Rect r = dragToolBar->GetRect();
		r = r + ps - dragPoint;
		dragPoint = ps;
		dragToolBar->SetRect(r);
		
		// query an eventual dock frame under mouse
		if(preDockFrame)
			preDockFrame->UnPreDock(*dragToolBar);
		preDockFrame = QueryDockFrame(pp);
		if(preDockFrame)
		{
			XMLToolBarState state = preDockFrame->GetToolBarState();
			if(state == TOOLBAR_TOP || state == TOOLBAR_BOTTOM)
				dragToolBar->PopHorz(ps);
			else
				dragToolBar->PopVert(ps);
			preDockFrame->PreDock(*dragToolBar, pp);
		}
		else
			dragToolBar->PopSquare(ps);
		Ctrl::RefreshLayout();
	}
	while(GetMouseLeft());
	
#ifdef PLATFORM_POSIX
	dragToolBar->ReleaseCapture();
#endif

	// if dropped on a frame, dock there
	if(preDockFrame)
	{
		dragToolBar->UnPreDock(*preDockFrame);
		dragToolBar->Dock(*preDockFrame, pp);
	}
	else
	{
		// otherwise float it
		dragToolBar->Float(ps);
	}

	// re-accepts mouse events
	Ctrl::NoIgnoreMouse();

	// end dragging mode
	dragging = false;
}

// menu and toolbars setter callbacks
template<class T> void WithXMLMenu<T>::SetMenuBar0(Bar &bar, int mnIdx, Array<XMLToolBarItem> const *items)
{
	// get menu bar items
	if(!items)
		items = &menuBars[mnIdx].GetItems();
	
	for(int i = 0; i < items->GetCount(); i++)
	{
		// get current item
		XMLToolBarItem const &item = (*items)[i];
		
		// submenu handling
		if(item.IsSubMenu())
		{
			bar.Add(item.GetLabel(), item.GetIcon(), THISBACK2(SetMenuBar0, mnIdx, &item.GetSubMenu().GetItems()));
			continue;
		}

		// get command id and find command
		// if not found, just skip it
		String id = item.GetId();
		if(!commands.Has(id))
			continue;
		
		// get command from id
		XMLCommand const &cmd = commands.Get(id);
		
		// get callback from command
		bool isCustom = cmd.GetIsCustom();
		Callback handler;
		if(isCustom)
			handler = THISBACK1(callUserHandler, id);
		else
			handler = cmd.GetCallback();
		
		// get remaining item data
		String label = item.GetLabel();
		if(label == "")
			label = id;
		Image const &img = item.GetIcon();
		
		Ctrl *ctrl = cmd.GetCtrl();
		bool enabled = cmd.GetIsEnabled();
		
		// adds to menu
		if(ctrl)
			bar.Add(*ctrl);
		else
			bar.Add(enabled, label, img, handler);
	}
}

template<class T> void WithXMLMenu<T>::SetToolBar0(Bar &bar, int tbIdx, Array<XMLToolBarItem> const *items)
{
	// get menu bar items
	if(!items)
		items = &toolBars[tbIdx].GetItems();
	
	for(int i = 0; i < items->GetCount(); i++)
	{
		// get current item
		XMLToolBarItem const &item = (*items)[i];
		
		// submenu handling
		if(item.IsSubMenu())
		{
			bar.Add(item.GetLabel(), item.GetIcon(), THISBACK2(SetToolBar0, tbIdx, &item.GetSubMenu().GetItems()));
			continue;
		}

		// get command id and find command
		// if not found, just skip it
		String id = item.GetId();
		if(!commands.Has(id))
			continue;
		
		// get command from id
		XMLCommand const &cmd = commands.Get(id);
		
		// get callback from command
		bool isCustom = cmd.GetIsCustom();
		Callback handler;
		if(isCustom)
			handler = THISBACK1(callUserHandler, id);
		else
			handler = cmd.GetCallback();
		
		// get remaining item data
		String label = item.GetLabel();
		if(label == "")
			label = id;
		Image const &img = item.GetIcon();
		
		Ctrl *ctrl = cmd.GetCtrl();
		bool enabled = cmd.GetIsEnabled();
		
		// adds to menu
		if(ctrl)
			bar.Add(*ctrl);
		else
			bar.Add(enabled, label, img, handler);
	}
}

// sets builtin commands
template<class T> void WithXMLMenu<T>::SetCommands(Callback1<XMLCommands &> cmds)
{
	// setup commands
	cmds(commands);
	
	// refresh menus
	for(int iBar = 0; iBar < menuBars.GetCount(); iBar++)
		menuBarCtrls[iBar].Set(THISBACK1(SetMenuBar, iBar));
	
	// refresh toolbars
	for(int iBar = 0; iBar < toolBars.GetCount(); iBar++)
		toolBarCtrls[iBar].Set(THISBACK1(SetToolBar, iBar));
	
	// refresh frames
	RefreshFrames();
}

// sets menu entries
template<class T> void WithXMLMenu<T>::SetMenuBars(Callback1<XMLToolBars &> tb)
{
	tb(menuBars);
	menuBarCtrls.Clear();
	for(int iBar = 0; iBar < menuBars.GetCount(); iBar++)
	{
		menuBarCtrls.Add(new MenuBar);

		// workaround for main menu... without this one
		// it appears as a popup menu
		if(menuBars[iBar].GetName() == "Main")
			T::AddFrame(menuBarCtrls[iBar]);

		menuBarCtrls[iBar].Set(THISBACK1(SetMenuBar, iBar));
	}

	// refresh toolbars
	RefreshFrames();
}

// sets toolbars entries
template<class T> void WithXMLMenu<T>::SetToolBars(Callback1<XMLToolBars &> tb)
{
	tb(toolBars);
	toolBarCtrls.Clear();
	for(int iBar = 0; iBar < toolBars.GetCount(); iBar++)
	{
		XMLToolBar &toolBar = toolBars[iBar];
		toolBarCtrls.Add(new XMLToolBarCtrl(this));
		XMLToolBarCtrl &toolBarCtrl = toolBarCtrls.Top();
		Reposition(&toolBarCtrl, toolBar.GetState(), toolBar.Getx(), toolBar.Gety());
		toolBarCtrls[iBar].Set(THISBACK1(SetToolBar, iBar));
	}

	// refresh toolbars
	RefreshFrames();
}

// gets a context menu by name -- NULL if none
template<class T> MenuBar *WithXMLMenu<T>::GetContextMenu(String const &name)
{
	int idx = menuBars.Find(name);
	if(idx < 0)
		return NULL;
	return &menuBarCtrls[idx];
}

END_UPP_NAMESPACE

#endif
