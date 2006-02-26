#include <CtrlLib/CtrlLib.h>

struct App : TopWindow {
	Image img;

	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), SWhite);
		w.DrawImage(0, 0, img);
	}
	
	App() {
		img = Image(100, 100);
		ImageDraw w(img);
		w.DrawRect(0, 0, 100, 100, SGray);
		w.DrawEllipse(0, 0, 100, 100, SGreen);
		w.DrawText(0, 0, "ImageDraw!", Arial(13).Bold(), SYellow);
	}
};

GUI_APP_MAIN
{
	App().Run();
}
