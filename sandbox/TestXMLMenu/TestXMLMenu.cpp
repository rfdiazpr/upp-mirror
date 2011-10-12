#include "TestXMLMenu.h"

using namespace Upp;

#define IMAGECLASS TestImg
#define IMAGEFILE <TestXMLMenu/TestXMLMenu.iml>
#include <Draw/iml_source.h>

TestXMLMenu::TestXMLMenu()
{
}

static void dummyCb(void)
{
}

// testing callback -- to be removed once XML is in place
static void toolBarCb(Bar &bar)
{
	bar.Add(t_("New")			, TestImg::New()			, callback(dummyCb));
	bar.Add(t_("NewCalc")		, TestImg::NewCalc()		, callback(dummyCb));
	bar.Add(t_("Open")			, TestImg::Open()			, callback(dummyCb));
	bar.Add(t_("Save")			, TestImg::Save()			, callback(dummyCb));
	bar.Add(t_("SaveAs")		, TestImg::SaveAs()			, callback(dummyCb));
	bar.Add(t_("JobInfo")		, TestImg::JobInfo()		, callback(dummyCb));
	bar.Add(t_("PrintPreview")	, TestImg::PrintPreview()	, callback(dummyCb));
	bar.Add(t_("Print")			, TestImg::Print()			, callback(dummyCb));
	bar.Add(t_("Next")			, TestImg::Next()			, callback(dummyCb));
	bar.Add(t_("Previous")		, TestImg::Previous()		, callback(dummyCb));
	bar.Add(t_("Settings")		, TestImg::Settings()		, callback(dummyCb));
	bar.Add(t_("Help")			, TestImg::Help()			, callback(dummyCb));
	bar.Add(t_("Quit")			, TestImg::Quit()			, callback(dummyCb));
	bar.Add(t_("Exit")			, TestImg::Exit()			, callback(dummyCb));
	bar.Add(t_("Flag")			, TestImg::Flag()			, callback(dummyCb));
	bar.Add(t_("Remove")		, TestImg::Remove()			, callback(dummyCb));
	bar.Add(t_("Delete")		, TestImg::Delete()			, callback(dummyCb));
	bar.Add(t_("ListAdd")		, TestImg::ListAdd()		, callback(dummyCb));
	bar.Add(t_("ListRemove")	, TestImg::ListRemove()		, callback(dummyCb));
	bar.Add(t_("RtfImport")		, TestImg::RtfImport()		, callback(dummyCb));
}

// commands generator callback
static void commandCb(XMLCommands &cmds)
{
	cmds
		.Add("New"			, callback(dummyCb))
		.Add("NewCalc"		, callback(dummyCb))
		.Add("Open"			, callback(dummyCb))
		.Add("Save"			, callback(dummyCb))
		.Add("SaveAs"		, callback(dummyCb))
		.Add("JobInfo"		, callback(dummyCb))
		.Add("PrintPreview"	, callback(dummyCb))
		.Add("Print"		, callback(dummyCb))
		.Add("Next"			, callback(dummyCb))
		.Add("Previous"		, callback(dummyCb))
		.Add("Settings"		, callback(dummyCb))
		.Add("Help"			, callback(dummyCb))
		.Add("Quit"			, callback(dummyCb))
		.Add("Exit"			, callback(dummyCb))
		.Add("Flag"			, callback(dummyCb))
		.Add("Remove"		, callback(dummyCb))
		.Add("Delete"		, callback(dummyCb))
		.Add("ListAdd"		, callback(dummyCb))
		.Add("ListRemove"	, callback(dummyCb))
		.Add("RtfImport"	, callback(dummyCb))
	;
}

// menu structure callback
void menuCb(XMLToolBar &tb)
{
	tb
		.Add("File", tb.SubMenu()
			.Add("New", t_("New"))
			.Add("NewCalc", t_("NewCalc"))
		)
		.Add("Printer", tb.SubMenu()
			.Add("Print", t_("Print"))
			.Add("PrintPreview", t_("PrintPreview"))
		)
	;
}

GUI_APP_MAIN
{

	// create window with dockable menus/toolbars
	TestXMLMenu testXMLMenu;
	
	// adds built-in commands
	testXMLMenu.SetCommands(STDBACK(commandCb));
	
	// build default menu structure
	testXMLMenu.SetMenu(STDBACK(menuCb));
	
/*
	testXMLMenu.GetMenuBar().Set(STDBACK(toolBarCb));
	
	XMLToolBarCtrl *tb;
	
	tb = new XMLToolBarCtrl(&testXMLMenu);
	tb->Set(STDBACK(toolBarCb));
	testXMLMenu.AddTop(tb, 0, 50);
	
	tb = new XMLToolBarCtrl(&testXMLMenu);
	tb->Set(STDBACK(toolBarCb));
	testXMLMenu.AddTop(tb, 0, 550);
	
	tb = new XMLToolBarCtrl(&testXMLMenu);
	tb->Set(STDBACK(toolBarCb));
	testXMLMenu.AddTop(tb, 1, 300);
	
	tb = new XMLToolBarCtrl(&testXMLMenu);
	tb->Set(STDBACK(toolBarCb));
	testXMLMenu.AddLeft(tb, 0, 50);
	
	tb = new XMLToolBarCtrl(&testXMLMenu);
	tb->Set(STDBACK(toolBarCb));
	testXMLMenu.AddRight(tb, 0, 100);
	
	tb = new XMLToolBarCtrl(&testXMLMenu);
	tb->Set(STDBACK(toolBarCb));
	testXMLMenu.AddBottom(tb, 0, 200);
*/
	
	testXMLMenu.Sizeable().Zoomable();
	testXMLMenu.Run();
}

