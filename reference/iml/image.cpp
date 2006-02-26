#include <CtrlLib/CtrlLib.h>

#define  IMAGEFILE <iml/image.iml>
#include <Draw/iml.h>

struct App : TopWindow {
	void Paint(Draw& w)
	{
		Size sz = GetSize();
		w.DrawRect(sz, SGray);
		Size isz = Smiley().GetSize();
		w.DrawImage((sz.cx - isz.cx) / 2, (sz.cy - isz.cy) / 2, Smiley());
	}
	
	App()
	{
		Sizeable();
		BackPaint();
	}
};

GUI_APP_MAIN
{
	App().Run();
}
