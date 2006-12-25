#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <Chameleon/Chameleon.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS MyButtonImg
#define IMAGEFILE <Chameleon/Chameleon.iml>
#include <Draw/iml.h>

struct ChEllipse {
	int   width;
	Color pen;
	Color color;

	ChEllipse() : width(width), pen(pen), color(color) {}
};

Value EllipseLook(int width, Color pen, Color color)
{
	ChEllipse e;
	e.width = width;
	e.pen = pen;
	e.color = color;
	return RawToValue(e);
}

Value MyLookFn(Draw& w, const Rect& rect, const Value& v, int op) {
	if(IsTypeRaw<ChEllipse>(v)) {
		const ChEllipse& e = ValueTo<ChEllipse>(v);
		switch(op) {
		case LOOK_MARGINS:
			return Rect(e.width, e.width, e.width, e.width);
		case LOOK_ISOPAQUE:
			return false;
		case LOOK_PAINT:
			w.DrawEllipse(rect, e.color, e.width, e.pen);
			return 1;
		}
	}
	return Null;
}

INITBLOCK {
	ChLookFn(MyLookFn);
}


struct MyApp : WithChameleonLayout<TopWindow> {
	void Set(void (*skin)());

	typedef MyApp CLASSNAME;
	MyApp();
};

void ColorSkin()
{
	ChSet("ButtonLook", 0, LtCyan());
	ChSet("ButtonLook", 1, Yellow());
	ChSet("ButtonLook", 2, Brown());
	ChSet("ButtonLook", 3, Gray());
}

void ImageSkin()
{
	for(int i = 0; i < 4; i++)
		ChSet("ButtonLook", i, MyButtonImg::Get(i));
}

void FnSkin()
{
	ChSet("ButtonLook", 0, EllipseLook(2, Black, LtGray));
	ChSet("ButtonLook", 1, EllipseLook(3, Red, WhiteGray));
	ChSet("ButtonLook", 2, EllipseLook(4, Blue, White));
	ChSet("ButtonLook", 3, EllipseLook(1, Black, Gray));
}

void MyApp::Set(void (*skin)())
{
	SetSkin(skin);
}

MyApp::MyApp()
{
	CtrlLayout(*this, "Chameleon example");
	host <<= THISBACK1(Set, ChHostSkin);
	std <<= THISBACK1(Set, ChStdSkin);
	classic <<= THISBACK1(Set, ChClassicSkin);
	color <<= THISBACK1(Set, ColorSkin);
	img <<= THISBACK1(Set, ImageSkin);
	fn <<= THISBACK1(Set, FnSkin);
}

GUI_APP_MAIN
{
	MyApp().Run();
}
