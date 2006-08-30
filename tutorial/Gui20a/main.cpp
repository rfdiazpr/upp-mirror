#include <CtrlLib/CtrlLib.h>

struct MyApp : TopWindow {
	Button exit;

	void Exit() {
		Break(999);
	}

	typedef MyApp CLASSNAME;

	MyApp() {
		SetRect(0, 0, 100, 100);
		Add(exit.SetLabel("exit").LeftPosZ(10, 64).TopPosZ(10, 24));
		exit <<= THISBACK(Exit);
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
