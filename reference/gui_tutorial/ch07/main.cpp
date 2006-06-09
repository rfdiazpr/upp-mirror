#include <CtrlLib/CtrlLib.h>

struct MyAppWindow : TopWindow {
	MenuBar menu;

	void Exit() {
		if(PromptOKCancel("Exit MyApp?"))
			Break();
	}

	void SubMenu(Bar& bar) {
		bar.Add("Exit", THISBACK(Exit));
	}

	void MainMenu(Bar& bar) {
		bar.Add("Menu", THISBACK(SubMenu));
	}

	typedef MyAppWindow CLASSNAME;

	MyAppWindow() {
		Title("My application with menu").Sizeable();
		AddFrame(menu);
		menu.Set(THISBACK(MainMenu));
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
