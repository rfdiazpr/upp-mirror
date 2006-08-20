#include <CtrlLib/CtrlLib.h>

#include <CtrlLib/CtrlLib.h>

struct MyApp : TopWindow {
	Button exit;

	typedef MyApp CLASSNAME;
	
	MyApp() {
		SetRect(0, 0, 100, 100);
		Add(exit.SetLabel("exit").LeftPosZ(10, 64).TopPosZ(10, 24));
		exit <<= Breaker(999);
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}

