#ifndef _WithXMLMenu_h_
#define _WithXMLMenu_h_

#include "XMLToolBarFrame.h"
#include "XMLCommand.h"
#include "XMLToolBar.h"

NAMESPACE_UPP

// interface part -- to support backlink from childs
class XMLMenuInterface
{
	private:
	
	protected:
	
	public:
	
		// mouse events sent from floating toolbar
		virtual void FloatingDraggedEvent(XMLToolBarCtrl &tb, Point p) = 0;
};

// main XML menu template
template<class T> class WithXMLMenu : public T, public XMLMenuInterface
{
	private:
	
		// the available commands
		XMLCommands commands;
	
		// the configurable toolbars
		XMLToolBars toolBars;
		
		// the configurable menu bar
		XMLToolBar menuBar;
	
		// the four corner XMLToolbarFrame frames
		XMLToolBarFrame topFrame;
		XMLToolBarFrame bottomFrame;
		XMLToolBarFrame leftFrame;
		XMLToolBarFrame rightFrame;
		
		// the menu bar control
		MenuBar menuBarCtrl;
	
		// toolbar controls storage area
		Array<XMLToolBarCtrl> toolBarCtrls;
		
		// dragging flag
		bool dragging;

		// toolbar being dragged		
		Ptr<XMLToolBarCtrl> dragToolBar;
		
		// pre-docking stuffs
		Ptr<XMLToolBarFrame> preDockFrame;

		// menu and toolbars setter callbacks
		void SetMenuBar0(Bar &bar, Array<XMLToolBarItem> const *items);
		void SetMenuBar(Bar &bar) { SetMenuBar0(bar, NULL); }

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
		void SetMenu(Callback1<XMLToolBar &> tb);

		// sets toolbars entries
		void SetToolBars(Callback1<XMLToolBars &> tb);
};

template<class T> WithXMLMenu<T>::WithXMLMenu() :
	topFrame		(TOOLBAR_TOP),
	bottomFrame		(TOOLBAR_BOTTOM),
	leftFrame		(TOOLBAR_LEFT),
	rightFrame		(TOOLBAR_RIGHT)
{
	T::AddFrame(menuBarCtrl);
	
	T::AddFrame(topFrame);
	T::AddFrame(bottomFrame);
	T::AddFrame(leftFrame);
	T::AddFrame(rightFrame);
	
	dragging = false;
	dragToolBar = NULL;
	
	preDockFrame = NULL;
	
}

template<class T> WithXMLMenu<T>::~WithXMLMenu()
{
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
	if(topFrame.Contains(p))
		return &topFrame;
	else if(bottomFrame.Contains(p))
		return &bottomFrame;
	else if(leftFrame.Contains(p))
		return &leftFrame;
	else if(rightFrame.Contains(p))
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
template<class T> void WithXMLMenu<T>::SetMenuBar0(Bar &bar, Array<XMLToolBarItem> const *items)
{
	// get menu bar items
	if(!items)
		items = &menuBar.GetItems();
	
	for(int i = 0; i < items->GetCount(); i++)
	{
		// get current item
		XMLToolBarItem const &item = (*items)[i];
		
		// submenu handling
		if(item.IsSubMenu())
		{
			bar.Add(item.GetLabel(), item.GetIcon(), THISBACK1(SetMenuBar0, &item.GetSubMenu().GetItems()));
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
	cmds(commands);
	menuBarCtrl.Set(THISBACK(SetMenuBar));
}

// sets menu entries
template<class T> void WithXMLMenu<T>::SetMenu(Callback1<XMLToolBar &> tb)
{
	tb(menuBar);
	menuBarCtrl.Set(THISBACK(SetMenuBar));
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
}


END_UPP_NAMESPACE

#endif
