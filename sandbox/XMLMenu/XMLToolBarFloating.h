#ifndef _XMLToolBarFloating_h_
#define _XMLToolBarFloating_h_

#include "DragWindow.h"

NAMESPACE_UPP

// container for floating toolbars
class XMLToolBarCtrl;
class XMLToolBarFloating : public DragWindow
{
	friend class XMLToolBarCtrl;
	
	private:
	
		Ptr<XMLToolBarCtrl> toolBar;
#ifdef PLATFORM_POSIX
		Ptr<Ctrl> focusCtrl;
#endif

		// handler for window drag events
		virtual void WindowDragged(DragWindow::DragEvent e, Point p);
		
		XMLToolBarFloating(XMLToolBarCtrl &tb, Point p);

	public:
		~XMLToolBarFloating();
};

END_UPP_NAMESPACE

#endif
