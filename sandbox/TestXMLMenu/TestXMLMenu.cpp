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
void menuCb(XMLToolBars &tb)
{
	tb
		.Add("Main", tb.MenuBar()
			.Add("File", tb.SubMenu()
				.Add("New"		, t_("New")		, TestImg::New())
				.Add("Open"		, t_("Open")	, TestImg::Open())
				.Add("Save"		, t_("Save")	, TestImg::Save())
				.Add("SaveAs"	, t_("SaveAs")	, TestImg::SaveAs())
				.Add("Quit"		, t_("Quit")	, TestImg::Quit())
			)
			.Add("Calc", tb.SubMenu()
				.Add("NewCalc"	, t_("NewCalc")	, TestImg::NewCalc())
			)
			.Add("Printer", tb.SubMenu()
				.Add("Print", t_("Print"), TestImg::Print())
				.Add("PrintPreview", t_("PrintPreview"), TestImg::PrintPreview())
			)
			.Add("Other", tb.SubMenu()
				.Add("Next"			, TestImg::Next())
				.Add("Previous"		, TestImg::Previous())
				.Add("Settings"		, TestImg::Settings())
				.Add("Help"			, TestImg::Help())
				.Add("More"	, tb.SubMenu()
					.Add("Flag"			, TestImg::Flag())
					.Add("Remove"		, TestImg::Remove())
					.Add("Delete"		, TestImg::Delete())
				)
				.Add("Exit"			, TestImg::Exit())
			)
		)
		.Add("Context", tb.MenuBar()
			.Add("ListAdd"		, TestImg::ListAdd())
			.Add("ListRemove"	, TestImg::ListRemove())
			.Add("RtfImport"	, TestImg::RtfImport())
		)
	;
}

// toolbars structure callback
void toolBarsCb(XMLToolBars &tb)
{
	tb
		.Add("File", tb.ToolBar(TOOLBAR_TOP, 0, 0)
			.Add("New"		, t_("New")		, TestImg::New())
			.Add("Open"		, t_("Open")	, TestImg::Open())
			.Add("Save"		, t_("Save")	, TestImg::Save())
			.Add("SaveAs"	, t_("SaveAs")	, TestImg::SaveAs())
			.Add("Quit"		, t_("Quit")	, TestImg::Quit())
		)
		.Add("Calc", tb.ToolBar(TOOLBAR_TOP, 0, 10)
			.Add("NewCalc"	, t_("NewCalc")	, TestImg::NewCalc())
		)
		.Add("Printer", tb.ToolBar(TOOLBAR_LEFT, 0, 0)
			.Add("Print", t_("Print"), TestImg::Print())
			.Add("PrintPreview", t_("PrintPreview"), TestImg::PrintPreview())
		)
		.Add("Other", tb.ToolBar(TOOLBAR_BOTTOM, 0, 0)
			.Add("Next"			, TestImg::Next())
			.Add("Previous"		, TestImg::Previous())
			.Add("Settings"		, TestImg::Settings())
			.Add("Help"			, TestImg::Help())
			.Add("Separator"	, tb.SubMenu()
				.Add("Flag"			, TestImg::Flag())
				.Add("Remove"		, TestImg::Remove())
				.Add("Delete"		, TestImg::Delete())
				.Add("ListAdd"		, TestImg::ListAdd())
				.Add("ListRemove"	, TestImg::ListRemove())
				.Add("RtfImport"	, TestImg::RtfImport())
			)
			.Add("Exit"			, TestImg::Exit())
		)
		     
	;
}

// context menu handler
void TestXMLMenu::RightDown(Point p, dword)
{
	MenuBar *menu = GetContextMenu("Context");
	if(menu)
		menu->Execute();
}

GUI_APP_MAIN
{

	// create window with dockable menus/toolbars
	TestXMLMenu testXMLMenu;
	
	// adds built-in commands
	testXMLMenu.SetCommands(STDBACK(commandCb));
	
	// build default menu structure
	testXMLMenu.SetMenuBars(STDBACK(menuCb));
	
	// build default toolbars structure
	testXMLMenu.SetToolBars(STDBACK(toolBarsCb));
	
	// don't allow dock bottom
//	testXMLMenu.NoDockBottom();
	
	testXMLMenu.Sizeable().Zoomable();
	testXMLMenu.Run();
}

