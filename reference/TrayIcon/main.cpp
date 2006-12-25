#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS Tray
#define IMAGEFILE  <TrayIcon/tray.iml>
#include <Draw/iml.h>

struct App : TrayIcon {
	void Info() {
		Icon(Tray::Icon1());
		PromptOK("TrayIcon launched this prompt!");
		Icon(Tray::Icon());
	}

	void Menu(Bar& bar) {
		bar.Add("Info..", THISBACK(Info));
		bar.Separator();
		bar.Add("Exit", THISBACK(Break));
	}

	typedef App CLASSNAME;

	App() {
		WhenBar = THISBACK(Menu);
		WhenLeftDouble = THISBACK(Info);
		Icon(Tray::Icon());
		Tip("This is U++ tray icon");
	}
};

GUI_APP_MAIN
{
	App().Run();
}
