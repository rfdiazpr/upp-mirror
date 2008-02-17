#include "DockCtrlExample.h"

DockCtrlExample::DockCtrlExample()
{
	CtrlLayout(*this, "DockCtrl [DEV802b.1] Example - Use 'Settings' menu options to open the control panel");
	Sizeable().Zoomable().SetRect(GetRect().left, GetRect().top, 1024, 600);

	// Usual U++ widget preperation.
	button1.SetLabel("Hello").LeftPos(10, 100).BottomPos(10, 20);
	button2.SetLabel("Brave New").HCenterPos(90).BottomPos(10, 20);
	button3.SetLabel("World").RightPos(10, 100).BottomPos(10, 20);
	
	tabctrl.Add("Tab 1");
	tabctrl.Add("Tab 2");
	
	array1.AddColumn("Numbers");
	array2.AddColumn("Roman Numbers");
	
	for(int i = 0; i < 100; i++)
	{
		array1.Add(Format("%d", i));
		array2.Add(FormatIntRoman(i, true));
	}

	// Here (but not necessarily) we "wrap" -or, if you prefer "dock" - the above widgets.
	dock1 << button1 << button2 << button3;
	dock2 << tabctrl.SizePos();
	dock3 << array1.SizePos();
	dock4 << array2.SizePos();

	// Currently all DockableCtrl derived classes (e.g. DockWindow, TabWindow) can use 3 traditional "bar" ctrls of U++. 
	// Namely, MenuBar, ToolBar and StatusBar. The aim of the support for seperate bar controls is to extend the DockableCtrl functionality.
	// So, you should see and use this "bar" support as a customization support for dockctrl framework itself;
	dock2.AddMenuBar(menubar);
	dock2.AddStatusBar(statusbar);
	dock3.AddToolBar(toolbar); 
	
	menubar.Set(THISBACK(Menubar));	
	toolbar.Set(THISBACK(Toolbar));
	
	             
	// DockCtrl::SetLayout(Ctrl& parent, cx, cy) method must be called before any initialization (except for the child ctrl adding to the dockable widgets).
	// This method prepares the dockable-widget area.
	// One of the implication of this layout mechanism is, theoretically, it allows almost infinite number of seperate DockCtrl instance 
	// in a single Application Window! Namely, you could have numbers of independent dockable window area. This is because, the SetLayout()
	// methods takes Ctrl as it's base. So, you could even have seperate dockctrl framework in a single dock Widget! ;)
	dockctrl.SetLayout(*this, GetSize().cx / 4, GetSize().cy / 3);
	
	// Magic Stuff...
	// DockableCtrl derived widgets use 3 variable to determine their place: 
	// -----------------------------------------------------------------------------------------
	// 1) Alignment: 	As usual, it can be left, top, right, bottom.
	// 2) State:	 	It can be show, hide, autohide, tab, floating, shut.
	// 3) Position:		Determines the position of the widget, for example 0 means the widget is 
	//					on its own (usually, at floating state, or shut). So, any "docked" widget
	//					is at least in position 1. In normal docking mode (no tabbing) system
	//					adds the dock automatically to the end of the list, so this variable is
	//					optional for the user. 
	// -----------------------------------------------------------------------------------------
	// Tabbing issue:	While any type of manipulation is possible with the above variables, 
	//					tabbing is a little different. There is no public method available to
	//					tab two (or more) dockable widget. Instead, we use state and position
	//					combinations.
	// 
	//					Let's assume that we have 3 widgets docked on the "top" pane. And let's
	//					say that we want to add two more docks on the top pane. We have three
	//					alternatives: a) We can add it in usual way, and let the framework
	//					handle the positioning. b) We can determine the position to dock.
	//					(e.g. if we want to add it to 2. position, position should be 2). 
	//					c) We can tab docks. Now, to tab a dock, all we have to do is to set the 
	//					two variables properly: state and position. "State" should be "tabbed".
	//					"position" should be the position where we want to tab (e.g. If we want
	//					to tab the first dock then position should be 1).
	// 
	//					This is a design issue; because, with this way it is easily possible to
	//					tab any number of dockable widgets simultaneously.
	//					

	dockctrl.Dock(dock3.SetLabel("Dock 1").DockLeft().StateShow().SetIcon(CtrlImg::new_doc()));
	dockctrl.Dock(dock4.SetLabel("Dock 2").DockRight().StateTabbed().Posit(1).SetIcon(CtrlImg::new_doc()));	// Manual tabbing example
	dockctrl.Dock(dock5.SetLabel("Dock 3").DockRight().StateTabbed().Posit(1).SetIcon(CtrlImg::new_doc()));	//
	dockctrl.Dock(dock1.SetLabel("Dock 4").DockTop().StateShow().SetIcon(CtrlImg::new_doc()));
	dockctrl.Dock(dock2.SetLabel("Dock 5").DockBottom().StateShow().SetIcon(CtrlImg::new_doc()));
	Add(dockctrl);

	//
	Add(richedit.SizePos());	
									
}

void DockCtrlExample::Toolbar(Bar &bar)
{
	toolbar.Add(CtrlImg::open(),THISBACK(Click));
	toolbar.Add(CtrlImg::new_doc(),THISBACK(Click));
	toolbar.Add(CtrlImg::save(),THISBACK(Click));
	toolbar.Separator();
	toolbar.Add(CtrlImg::undo(),THISBACK(Click));
	toolbar.Add(CtrlImg::redo(),THISBACK(Click));
	toolbar.Add(CtrlImg::paste(),THISBACK(Click));
	toolbar.Add(CtrlImg::remove(),THISBACK(Click));
	toolbar.Separator();
	toolbar.Add(CtrlImg::print(),THISBACK(Click));
}

void DockCtrlExample::Menubar(Bar &bar)
{
	bar.Add("File",THISBACK(WindowsMenu));
	bar.Add("Edit",THISBACK(EditMenu));
	bar.Add("Help",THISBACK(InfoMenu));
}

void DockCtrlExample::WindowsMenu(Bar& bar)
{
	bar.Add("New", CtrlImg::new_doc(), THISBACK(Click));
	bar.Add("Open", CtrlImg::open(),THISBACK(Click));
	bar.Add("Save", CtrlImg::save(), THISBACK(Click));
	bar.Add("Save As...", CtrlImg::save_as(),THISBACK(Click));
	bar.Separator();
	bar.Add("Exit", THISBACK(Click));
}

void DockCtrlExample::InfoMenu(Bar& bar)
{
	bar.Add("Help Topics", CtrlImg::help(), THISBACK(Click));
	bar.Add("About...", THISBACK(Click));
}

void DockCtrlExample::EditMenu(Bar& bar)
{
	bar.Add("Undo", CtrlImg::undo(), THISBACK(Click));
	bar.Add("Redo", CtrlImg::redo(), THISBACK(Click));
	bar.Separator();
	bar.Add("Paste", CtrlImg::paste(), THISBACK(Click));
	bar.Add("Remove", CtrlImg::remove(), THISBACK(Click));	
}


GUI_APP_MAIN
{
	DockCtrlExample().Run();
}

