#include <CtrlLib/CtrlLib.h>

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
	void SetStyle(int i);
	void SetColor();
	void SetImage();
	void SetFn();

	typedef MyApp CLASSNAME;
	MyApp();
};

void MyApp::SetStyle(int i)
{
	ChSetStyle(i);
	Refresh();
}

void MyApp::SetColor()
{
	ChSetStyle(CH_STYLE_STD);
	ChSet("ButtonLook", 0, LtCyan());
	ChSet("ButtonLook", 1, Yellow());
	ChSet("ButtonLook", 2, Brown());
	ChSet("ButtonLook", 3, Gray());
	Refresh();
}

void MyApp::SetImage()
{
	ChSetStyle(CH_STYLE_STD);
	for(int i = 0; i < 4; i++)
		ChSet("ButtonLook", i, MyButtonImg::Get(i));
	Refresh();
}

void MyApp::SetFn()
{
	ChSetStyle(CH_STYLE_STD);
	ChSet("ButtonLook", 0, EllipseLook(2, Black, LtGray));
	ChSet("ButtonLook", 1, EllipseLook(3, Red, WhiteGray));
	ChSet("ButtonLook", 2, EllipseLook(4, Blue, White));
	ChSet("ButtonLook", 3, EllipseLook(1, Black, Gray));
	Refresh();
}

MyApp::MyApp()
{
	CtrlLayout(*this, "Chameleon example");
	host <<= THISBACK1(SetStyle, (int)CH_STYLE_DETECT);
	std <<= THISBACK1(SetStyle, (int)CH_STYLE_STD);
	classic <<= THISBACK1(SetStyle, (int)CH_STYLE_CLASSIC);
	color <<= THISBACK(SetColor);
	img <<= THISBACK(SetImage);
	fn <<= THISBACK(SetFn);
}

GUI_APP_MAIN
{
	MyApp().Run();
}
