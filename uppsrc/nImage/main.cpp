#include <CtrlLib/CtrlLib.h>

struct App : TopWindow {
	virtual void Paint(Draw& draw);
};

GUI_APP_MAIN
{
	App().Run();
}
