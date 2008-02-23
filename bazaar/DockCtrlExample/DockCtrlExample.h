#ifndef _DockCtrlExample_DockCtrlExample_h
#define _DockCtrlExample_DockCtrlExample_h

#include <DockCtrl/DockCtrl.h>
#include <RichEdit/RichEdit.h>

using namespace Upp;

#define LAYOUTFILE <DockCtrlExample/DockCtrlExample.lay>
#include <CtrlCore/lay.h>

class DockCtrlExample : public WithDockCtrlExampleLayout<TopWindow> {
public:
	typedef DockCtrlExample CLASSNAME;
	DockCtrlExample();
	

	DockCtrl 	dockctrl;									// Main Widget 		-	acts as a "server" for it's "client" widgets.
	DockWindow	dock1, dock2, dock3, dock4, dock5, dock6;	// Actual Widgets 	-	these are the dockable "client" widgets
	
	RichEdit	richedit;									// Stuff to "wrap"	
	TabCtrl		tabctrl;									//
	Button		button1, button2, button3;					//
	ArrayCtrl	array1, array2;								//
	MenuBar 	menubar;									//
	ToolBar 	toolbar;									//
	StatusBar 	statusbar;									//

	// Other methods (not necessary; only for demo purpose...

	void Toolbar	(Bar &bar);
	void Menubar	(Bar &bar);
	void WindowsMenu(Bar& bar); 
	void InfoMenu	(Bar& bar);
	void EditMenu	(Bar& bar);
	void Click()	{}

	
};

#endif

