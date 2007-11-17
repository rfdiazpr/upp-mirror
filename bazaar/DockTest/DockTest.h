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
	
	WithTestLayout1<DockableWindow> docker1;
	DockableWindow docker2, docker3, docker4, docker5;
	MenuBar menu;
	ArrayCtrl array1;
	ToolBar	tool1;
	TreeCtrl tree1, tree2;
	Button btns[BUTTON_COUNT];
	
	DockTest() {
		// Layout main window
		CtrlLayout(*this, "Docking Test App");
		Sizeable().MaximizeBox().MinimizeBox();
		// Prepare controls
		FillDataControls();
		FillToolBar();
		// Layout and add controls to docker windows
		CtrlLayout(docker1, "Tool Window");
		docker1.Icon(CtrlImg::print());
		docker2.Icon(CtrlImg::Home()).Add(array1.SizePos());
		docker3.Icon(CtrlImg::cut()).Add(tree1.SizePos());
		docker4.Icon(CtrlImg::copy()).Add(tree2.SizePos());
		docker5.Icon(CtrlImg::save()).Add(tool1.SizePos());
		
		Dock(DOCK_TOP, docker2.SetMinSize(Size(100, 100)), "ArrayCtrl");
		Dock(DOCK_LEFT, docker3.SetMinSize(Size(80, 200)), "TreeCtrl");
		Dock(DOCK_RIGHT, docker4.SetMinSize(Size(80, 200)), "TreeCtrl");
		Dock(DOCK_BOTTOM, docker5.SetMinSize(Size(90, 22)), "ToolBar");
	
		docker3.AddFrame(menu);
	
		menu.Set(THISBACK(OnMenu));
	
		simple <<= THISBACK(OnSimple);
		tabbing <<= THISBACK(OnTabbing);
		simple <<= IsSimple();
		tabbing <<= IsTabbing();
		windows <<= THISBACK(ShowWindowStatus);
	}
	
	virtual void State(int reason)
	{
		if (reason == Ctrl::OPEN)
			Float(docker1);			
	}
	
	void OnSimple()
	{
		SimpleMode(~simple);
	}

	void OnTabbing()
	{
		Tabbing(~tabbing);
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
		FillTree(tree2);
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

