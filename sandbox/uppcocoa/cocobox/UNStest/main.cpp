
#include <UNSCtrl/UNSCtrl.h>

#ifdef flagTEST_UNS

using namespace Upp;

//NAMESPACE_UPP 

#define LLOG(x) LOG(x)

//#define CocWnd Ctrl
class CocWnd : public Ctrl {
	Point point;
public:
	typedef CocWnd CLASSNAME;
	virtual void PaintGL(Rect r);
	
	virtual void MouseDrag(Point p, dword) {
		LLOG("MouseDrag point="<<p);
	//	point = p;
		//Refresh();
	}
	CocWnd();	
	~CocWnd() {;}
};

void CocWnd::PaintGL(Rect rect) 
{
	LLOG("PaintGL"<<glrect);	
//	uglTriangle1(rect);
//	uglPyramid();
	GLStdView();
	uglUppExample(point);
}


CocWnd::CocWnd()
{
	point = Point(0,0);
}


//END_UPP_NAMESPACE

//using namespace Upp;

#define LLOG(x) LOG(x)


int main(int argc, const char *argv[])
{
	StdLogSetup(LOG_CERR);
	UppCocAutoReleasePool pool;
	LLOG("starting main");
//	UppCocApplication app;
	Ctrl::InitCocoa();
	CocWnd w;
	Rect r = RectC(0,0,600,400);
//	CocWnd::CreateApp();
	w.OpenGL();
//	w.CreateWnd(r);
	CocWnd::RunApp();
	LLOG("after cocoa run");
	return 0;
}

#endif