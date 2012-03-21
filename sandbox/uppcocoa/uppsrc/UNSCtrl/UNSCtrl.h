#ifndef _UNSCtrl_UNSCtrl_h_
#define _UNSCtrl_UNSCtrl_h_

#ifdef flagTEST_UNS

#include <UNS/UNS.h>
#include <UNS/UNSDef.h>
#include <GL/glu.h>
//#include <RichText/RichText.h>



NAMESPACE_UPP

void uglTriangle1(Rect rect);
void uglPyramid();
void uglUppExample(Point point=Point(0,0));


#define GUIPLATFORM_CTRL_DECLS_INCLUDE "CocWnd.h"

//class Ctrl;

class Ctrl : public Pte<Ctrl> {
private:
	struct Top {
		Ptr<Ctrl>      owner;
	};

	Top         *top;
public:
	Rect glrect;
	typedef Ctrl CLASSNAME;
	Ctrl();

	static	void InitCocoa();
	static void RunCocoa();
	static void RunApp();
	static void CocoaMenu();

	static void CreateApp();
	void CreateWnd(UPP::Rect& r);
	void OpenGL();
	void GLStdView();

	virtual	void PaintGL(Rect rect);
	virtual void MouseDrag(Point p, dword);
	~Ctrl();
	
#ifdef GUIPLATFORM_CTRL_DECLS_INCLUDE
	#include GUIPLATFORM_CTRL_DECLS_INCLUDE
//#else
//	GUIPLATFORM_CTRL_DECLS
#endif

public:
	Rect GetRect() { return rect;}
	void DoPaint(UNSGraphicsContextRef context, UNSViewRef view, Rect rect);
	void DoPaint1(UNSGraphicsContextRef context, Rect rect);
	
	bool IsPopUp() { return ispopup;}
	UNSWindowRef GetUNSWindowRef() {return cocWndRef;}
	Ctrl *GetTopCtrl() {return this;}
	void AdjustRectFromCocWnd(Rect r) { r = rect;}
	void SetRect(Rect r) { r = rect;}
	
private:
	Rect rect;
	bool ispopup;
	UNSWindowRef cocWndRef;
	
//	inline void * handle() { return window; }
};

END_UPP_NAMESPACE

#endif


#endif
