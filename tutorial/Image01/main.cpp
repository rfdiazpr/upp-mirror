#include <CtrlLib/CtrlLib.h>

#define IMAGECLASS MyImages
#define IMAGEFILE  <Image01/Image01.iml>
#include <Draw/iml.h>

class MyApp : public TopWindow {
public:
	virtual void Paint(Draw& draw);
};

void MyApp::Paint(Draw& w)
{
	w.DrawRect(GetSize(), SColorFace());
	w.DrawImage(50, 50, MyImages::MyImage());
}

GUI_APP_MAIN
{
	MyApp().Sizeable().Run();
}
