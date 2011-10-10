#ifndef _WithXMLMenu_h_
#define _WithXMLMenu_h_

#include "XMLToolBarFrame.h"

NAMESPACE_UPP

// interface part -- to support backlink from childs
class XMLMenuInterface
{
	private:
	
	protected:
	
	public:
	
		// mouse events sent from floating toolbar
		virtual void FloatingDraggedEvent(XMLToolBar &tb, Point p) = 0;
};

// main XML menu template
template<class T> class WithXMLMenu : public T, public XMLMenuInterface
{
	private:
	
		// the menu
		MenuBar menuBar;
	
		// the four corner XMLToolbarFrame frames
		XMLToolBarFrame topFrame;
		XMLToolBarFrame bottomFrame;
		XMLToolBarFrame leftFrame;
		XMLToolBarFrame rightFrame;
		
		// toolbars storage area
		Array<XMLToolBar> toolBars;
		
		// dragging flag
		bool dragging;

		// toolbar being dragged		
		Ptr<XMLToolBar> dragToolBar;
		
		// pre-docking stuffs
		Ptr<XMLToolBarFrame> preDockFrame;

	protected:
	
		// finds index of a given toolbar, -1 if none
		int FindIndex(XMLToolBar &tb);
		int FindFloating(XMLToolBar &tb);
		
	public:
	
		WithXMLMenu();
		~WithXMLMenu();
		
		// adds a toolbar, hidden, undocked, centered on screen and floating when opened
		WithXMLMenu<T> &Add(XMLToolBar *tb);
		
		// adds a toolbar, at a requested location and docking state
		WithXMLMenu<T> &Add(XMLToolBar *tb, XMLToolBar::XMLToolBarState state, int aPos = -1, int bPos = -1);
		WithXMLMenu<T> &AddTop(XMLToolBar *tb, int aPos = -1, int bPos = -1);
		WithXMLMenu<T> &AddBottom(XMLToolBar *tb, int aPos = -1, int bPos = -1);
		WithXMLMenu<T> &AddLeft(XMLToolBar *tb, int aPos = -1, int bPos = -1);
		WithXMLMenu<T> &AddRight(XMLToolBar *tb, int aPos = -1, int bPos = -1);
		WithXMLMenu<T> &AddFloating(XMLToolBar *tb, int aPos = -1, int bPos = -1);
		
		// removes a toolbar
		WithXMLMenu<T> &Remove(XMLToolBar *tb);
		
		// docks/undocks/hide a toolbar
		WithXMLMenu<T> &Reposition(XMLToolBar *tb, XMLToolBar::XMLToolBarState state, int aPos = -1, int bPos = -1);
		
		// query a dock frame under screen point
		XMLToolBarFrame *QueryDockFrame(Point p);

		// child frame mouse event handler captures the click on toolbar's frames
		virtual void ChildFrameMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags);
		
		// mouse events sent from floating toolbar
		virtual void FloatingDraggedEvent(XMLToolBar &tb, Point p);

		// the drag loop
		void DragLoop(Point startP);
		
		MenuBar &GetMenuBar(void) { return menuBar; }
};

template<class T> WithXMLMenu<T>::WithXMLMenu() :
	topFrame		(XMLToolBar::TOOLBAR_TOP),
	bottomFrame		(XMLToolBar::TOOLBAR_BOTTOM),
	leftFrame		(XMLToolBar::TOOLBAR_LEFT),
	rightFrame		(XMLToolBar::TOOLBAR_RIGHT)
{
	T::AddFrame(menuBar);
	
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

// finds index of a given toolbar, -1 if none
template<class T> int WithXMLMenu<T>::FindIndex(XMLToolBar &tb)
{
	for(int i = 0; i < toolBars.GetCount(); i++)
		if(&toolBars[i] == &tb)
			return i;
	return -1;
}
	
template<class T> int WithXMLMenu<T>::FindFloating(XMLToolBar &tb)
{
	for(int i = 0; i < toolBars.GetCount(); i++)
		if(&toolBars[i] == &tb && tb.GetIsFloating())
			return i;
	return -1;
}

// adds a toolbar, hidden, undocked, centered on screen and floating when opened first time,
// on previous undocked position for next times
template<class T> WithXMLMenu<T> &WithXMLMenu<T>::Add(XMLToolBar *tb)
{
	if(FindIndex(tb) >= 0)
		return *this;
	toolBars.Add(tb);
}

// adds a toolbar, at a requested location and docking state
template<class T> WithXMLMenu<T> &WithXMLMenu<T>::Add(XMLToolBar *tb, XMLToolBar::XMLToolBarState state, int aPos, int bPos)
{
	// check if already there, we don't want it added again
	if(FindIndex(*tb) < 0)
		toolBars.Add(tb);
	
	// reposition the toolbar
	Reposition(tb, state, aPos, bPos);
	
	return *this;
}

template<class T> WithXMLMenu<T> &WithXMLMenu<T>::AddTop(XMLToolBar *tb, int aPos, int bPos)
{
	return Add(tb, XMLToolBar::TOOLBAR_TOP, aPos, bPos);
}

template<class T> WithXMLMenu<T> &WithXMLMenu<T>::AddBottom(XMLToolBar *tb, int aPos, int bPos)
{
	return Add(tb, XMLToolBar::TOOLBAR_BOTTOM, aPos, bPos);
}

template<class T> WithXMLMenu<T> &WithXMLMenu<T>::AddLeft(XMLToolBar *tb, int aPos, int bPos)
{
	return Add(tb, XMLToolBar::TOOLBAR_LEFT, aPos, bPos);
}

template<class T> WithXMLMenu<T> &WithXMLMenu<T>::AddRight(XMLToolBar *tb, int aPos, int bPos)
{
	return Add(tb, XMLToolBar::TOOLBAR_RIGHT, aPos, bPos);
}

template<class T> WithXMLMenu<T> &WithXMLMenu<T>::AddFloating(XMLToolBar *tb, int aPos, int bPos)
{
	return Add(tb, XMLToolBar::TOOLBAR_FLOATING, aPos, bPos);
}

// removes a toolbar
template<class T> WithXMLMenu<T> &WithXMLMenu<T>::Remove(XMLToolBar *tb)
{
	int i = FindIndex(*tb);
	if(i < 0)
		return *this;
	tb->Close();
	toolBars.Remove(i);
}

// docks/undocks/hide a toolbar
template<class T> WithXMLMenu<T> &WithXMLMenu<T>::Reposition(XMLToolBar *tb, XMLToolBar::XMLToolBarState state, int aPos, int bPos)
{
	switch(state)
	{
		case XMLToolBar::TOOLBAR_CLOSED :
			tb->Close();
			break;
			
		case XMLToolBar::TOOLBAR_FLOATING :
			tb->Float(Point(aPos, bPos));
			break;
			
		case XMLToolBar::TOOLBAR_TOP :
			tb->Dock(topFrame, aPos, bPos);
			break;
			
		case XMLToolBar::TOOLBAR_BOTTOM :
			tb->Dock(bottomFrame, aPos, bPos);
			break;
			
		case XMLToolBar::TOOLBAR_LEFT :
			tb->Dock(leftFrame, aPos, bPos);
			break;
			
		case XMLToolBar::TOOLBAR_RIGHT :
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
	dragToolBar = dynamic_cast<XMLToolBar *>(child);
	if(!dragToolBar)
		return;

	// setup drag point for dragging the toolbar
	Point dragPoint = p + dragToolBar->GetScreenRect().TopLeft();
	
	// execute the drag loop
	DragLoop(dragPoint);
}

// mouse events sent from floating toolbar
template<class T> void WithXMLMenu<T>::FloatingDraggedEvent(XMLToolBar &tb, Point p)
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
			XMLToolBar::XMLToolBarState state = preDockFrame->GetToolBarState();
			if(state == XMLToolBar::TOOLBAR_TOP || state == XMLToolBar::TOOLBAR_BOTTOM)
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
	
	// if dropped on a frame, dock there
	if(preDockFrame)
	{
		DLOG("DOCKING WINDOW....");
		dragToolBar->UnPreDock(*preDockFrame);
		dragToolBar->Dock(*preDockFrame, pp);
		DLOG("DONE DOCKING WINDOW....");
	}
	else
	{
		DLOG("FLOATING WINDOW....");
		// otherwise float it
		dragToolBar->Float(ps);
		DLOG("DONE FLOATING WINDOW....");
	}

	// re-accepts mouse events
	Ctrl::NoIgnoreMouse();

	// end dragging mode
	dragging = false;
}

END_UPP_NAMESPACE

#endif
