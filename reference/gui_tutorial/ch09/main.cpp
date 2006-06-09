#include <CtrlLib/CtrlLib.h>

struct MyAppWindow : TopWindow {
	MenuBar menu;

	void Exit() {
		if(PromptOKCancel("Exit MyApp?"))
			Break();
	}

	void RightDown(Point, dword) {
		CallbackArgTarget<int> result;
		MenuBar menu;
		for(int i = 0; i < 10; i++)
			menu.Add(AsString(i), result[i]);
		menu.Separator();
		menu.Add("Exit", THISBACK(Exit));
		menu.Execute();
		if(!IsNull(result))
			PromptOK("You have selected " + AsString((int)result));
	}

	typedef MyAppWindow CLASSNAME;

	MyAppWindow() {
		Title("My application with context menu").Sizeable();
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
