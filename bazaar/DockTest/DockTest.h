#ifndef _DockTest_DockTest_h
#define _DockTest_DockTest_h

#include <Docking/Docking.h>

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#define LAYOUTFILE <DockTest/DockTest.lay>
#include <CtrlCore/lay.h>

#define BUTTON_COUNT 15

class DockTest : public WithDockTestLayout< DockWindow > {
public:
	typedef DockTest CLASSNAME;
	
	// DockableCtrls
	WithToolsLayout<DockableCtrl> docker1;
	WithDockOptionsLayout<DockableCtrl> docker2;
	DockableCtrl docker3, docker4, docker5;
	// Child ctrls for DockableCtrl
	MenuBar menu;
	ArrayCtrl array1;
	ToolBar	tool1;
	TreeCtrl tree1;
	Button btns[BUTTON_COUNT];
	
	DockTest() {
		// Layout main window
		CtrlLayout(*this, "Docking Test App");
		DockLayout();
		Sizeable().MaximizeBox().MinimizeBox();
		// Prepare controls
		FillDataControls();
		FillToolBar();
		// Layout and add controls to docker windows
		CtrlLayout(docker1, "Tool Window");

		CtrlLayout(docker2, "Dock Options");
		docker2.SetStyle(DockableCtrl::StyleDefaultVert());
		docker2.dockleft 	<<= THISBACK1(OnCtrlDockAllow, 0);		
		docker2.docktop 	<<= THISBACK1(OnCtrlDockAllow, 1);		
		docker2.dockright 	<<= THISBACK1(OnCtrlDockAllow, 2);		
		docker2.dockbottom 	<<= THISBACK1(OnCtrlDockAllow, 3);		
		docker2.verthandle	<<= THISBACK(OnVertHandle);
		docker2.dockleft 	<<= true;
		docker2.docktop 	<<= true;
		docker2.dockright 	<<= true;
		docker2.dockbottom 	<<= true;
		docker2.verthandle 	<<= docker2.GetStyle().handle_vert;

		docker1.Icon(CtrlImg::print());
		docker3.Icon(CtrlImg::Home()).Add(array1.SizePos());
		docker4.Icon(CtrlImg::cut()).Add(tree1.SizePos());
		docker5.Icon(CtrlImg::save()).Add(tool1.SizePos());
		
		Dock(DOCK_TOP, docker3.SizeHint(Size(100, 100)), "ArrayCtrl");
		Dock(DOCK_LEFT, docker4.SizeHint(Size(80, 200)), "TreeCtrl");
		Dock(DOCK_LEFT, docker2);
		Dock(DOCK_BOTTOM, docker5.SizeHint(Size(90, 50)), "ToolBar");
		docker3.AddFrame(menu);
	
		menu.Set(THISBACK(OnMenu));
	
		// Set Option callbacks
		simple 		<<= THISBACK(OnSimple);
		tabbing 	<<= THISBACK(OnTabbing);
		autohide	<<= THISBACK(OnAutoHide);
		animate 	<<= THISBACK(OnAnimate);
		dockleft 	<<= THISBACK1(OnMainDockAllow, 0);		
		docktop 	<<= THISBACK1(OnMainDockAllow, 1);		
		dockright 	<<= THISBACK1(OnMainDockAllow, 2);		
		dockbottom 	<<= THISBACK1(OnMainDockAllow, 3);		
		// Set Option data
		simple 		<<= IsSimple();
		tabbing 	<<= IsTabbing();
		autohide 	<<= IsAutoHide();
		animate 	<<= IsAnimated();
		dockleft 	<<= true;
		docktop 	<<= true;
		dockright 	<<= true;
		dockbottom 	<<= true;
//		windows <<= THISBACK(ShowWindowStatus);
	}
	
	virtual void State(int reason)
	{
		DockWindow::State(reason);	
		if (reason == Ctrl::OPEN) {
			ASSERT(IsOpen());
			Float(docker1);
		}
	}
	
	void OnSimple()  { SimpleMode(~simple); }
	void OnTabbing() { Tabbing(~tabbing); }	
	void OnAutoHide() { AutoHide(~autohide); }	
	void OnAnimate() { Animate(~animate); }
	
	void OnMainDockAllow(int align)
	{
		switch (align) {
		case DOCK_LEFT:
			AllowDockLeft(~dockleft);
			return;
		case DOCK_TOP:
			AllowDockTop(~docktop);
			return;
		case DOCK_RIGHT:
			AllowDockRight(~dockright);
			return;
		case DOCK_BOTTOM:
			AllowDockBottom(~dockbottom);
			return;
		}
	}
		
	void OnCtrlDockAllow(int align)
	{
		switch (align) {
		case DOCK_LEFT:
			docker2.AllowDockLeft(~docker2.dockleft);
			return;
		case DOCK_TOP:
			docker2.AllowDockTop(~docker2.docktop);
			return;
		case DOCK_RIGHT:
			docker2.AllowDockRight(~docker2.dockright);
			return;
		case DOCK_BOTTOM:
			docker2.AllowDockBottom(~docker2.dockbottom);
			return;
		}
	}		
		
	void OnVertHandle()
	{
		bool v = ~docker2.verthandle;
		docker2.SetStyle(v ? DockableCtrl::StyleDefaultVert() : DockableCtrl::StyleDefault());
	}
		
	void OnMenu(Bar &bar)
	{
		bar.Add("Menu1", THISBACK(OnTopMenu));
		bar.Add("Menu2", THISBACK(OnTopMenu));
	}
	
	void OnTopMenu(Bar &bar) {
		bar.Add("Item1", THISBACK(DoNothing));
		bar.Add("Item2", THISBACK(DoNothing));
		bar.Add("Item3", THISBACK(DoNothing));
		bar.Add("Item4", THISBACK(DoNothing));		
	}
	
	void DoNothing() {
			
	}
	
	void FillDataControls()
	{
		array1.AddColumn("Number");
		array1.MultiSelect();
		array1.AddColumn("Roman numbers");
		array1.MultiSelect();
		for(int i = 0; i < 200; i++)
			array1.Add(i, FormatIntRoman(i, true));
		FillTree(tree1);
	}
	
	void FillTree(TreeCtrl &tree) {
		Vector<int> parent, parent2;
		parent.Add(0);
		tree.SetRoot(Image(), "The Tree");
		for(int i = 1; i < 10000; i++) {
			parent.Add(tree.Add(parent[rand() % parent.GetCount()], Image(),
			            FormatIntRoman(i, true)));
			if((rand() & 3) == 0)
				tree.Open(parent.Top());
		}
		tree.Open(0);					
	}
	
	void FillToolBar() {
		for (int i = 0; i < BUTTON_COUNT; i++) {
			btns[i].SetImage(CtrlImg::Get(i+20));
			tool1.Wrap(3).AddTool(btns[i], 20, 20);	
		}
	}
};

#endif

