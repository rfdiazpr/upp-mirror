// This OSVGS_Demo attempts to test and demonstrate the capabilities and speed of the 
// OSVGS (one-stop-vector-graphics-shop) library 

#include <OSVGS/OSVGS.h>
#include <GLCtrl/GLCtrl.h>
#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

#ifdef _WIN32

#include <MMSystem.h>

INITBLOCK{
	timeBeginPeriod(1);
}

EXITBLOCK{
	timeEndPeriod(1);
}

#define ms_timer timeGetTime()

#else

#define ms_timer GetTickCount()

#endif

#define CURRENT_FLAGS OSVGS_ANTIALIASED
#define LINEWIDTH 3.0

#define TEST(_function_,_result_) { \
	dword begin;\
	dword end;\
	int reps=2000;\
	dword zero=ms_timer;\
	while((begin=ms_timer)==zero) ;\
	for(int i=0;i<reps;i++) _function_;\
	sd->Flush();\
	end=ms_timer;\
	_result_=(double)(end-begin)*1000.0/reps;\
}

// result is in microseconds

static void	SDDemo(OSVGS *sd,String &report){
	Font font; //
	font.FaceName("Verdana").Height(12);
	//font.Italic();
	//font.Bold();

	Vector<Pointf> vertices;
	
	vertices.Add(Pointf(100.5,100.5));
	vertices.Add(Pointf(200.5,100.5));
	vertices.Add(Pointf(200.5,200.5));
	vertices.Add(Pointf(100.5,200.5));
	vertices.Add(Pointf(100.5,100.5));
	
	vertices.Add(Pointf(120.5,120.5));
	vertices.Add(Pointf(180.5,120.5));
	vertices.Add(Pointf(180.5,180.5));
	vertices.Add(Pointf(120.5,180.5));
	vertices.Add(Pointf(120.5,120.5));
	
	sd->Clear(Black());
	Sizef sz=sd->GetSize();

	double result=0;

	report.Cat("&");
	Sleep(100);
	TEST(sd->Clear(Gray()),result);
	report.Cat(Format("&Clear %.1f us",result));
	TEST(sd->DrawLine(Pointf(0,0),Pointf(sz.cx,sz.cy*0.5),1,White()),result);
	report.Cat(Format("&DrawLine(W1) %.1f us",result));
	TEST(sd->DrawLine(Pointf(0,0),Pointf(sz.cx,sz.cy*0.5),3,White()),result);
	report.Cat(Format("&DrawLine(W3) %.1f us",result));
	TEST(sd->DrawLine(Pointf(0,0),Pointf(sz.cx,sz.cy*0.5),3,White(),LS_DASH),result);
	report.Cat(Format("&DrawLine(W3,DASH) %.1f us",result));
	report.Cat("&");
	TEST(sd->FillPolygon(vertices,Red()),result);
	report.Cat(Format("&FillPolygon %.1f us",result));
	TEST(sd->DrawPolyline(vertices,LINEWIDTH,White()),result);
	report.Cat(Format("&DrawPolyline %.1f us",result));
	TEST(sd->FillRectangle(Rectf(20,20,50,50),Yellow()),result);
	report.Cat(Format("&FillRectangle %.1f us",result));
	TEST(sd->DrawImage(Pointf(35,40),Image::Hand()),result);
	report.Cat(Format("&DrawImage %.1f us",result));
	report.Cat("&");
	TEST(sd->DrawText(Pointf(20,80),"Text rotated 0 degrees",0,font,Black()),result);
	report.Cat(Format("&DrawText %.1f us",result));
	TEST(sd->DrawText(Pointf(20,80),"Text rotated 25 degrees",25,font,Black()),result);
	report.Cat(Format("&DrawTextR %.1f us",result));
	TEST(sd->DrawArc(Pointf(280,300),95,55,0,270,LINEWIDTH,Yellow()),result);
	report.Cat(Format("&DrawArc %.1f us",result));
	TEST(sd->FillEllipse(Pointf(280,300),90,50,White()),result);
	report.Cat(Format("&FillEllipse %.1f us",result));
	report.Cat("&&");

	// Clear
	sd->Clear(White()); // Full clear

	sd->BeginClip(Rectf(5,15,sz.cx-5,sz.cy-5),false);
	
	sd->Clear(Gray()); // Clipped clear
	
	// Draw lines
	sd->DrawLine(Pointf(0,0),Pointf(sz.cx,sz.cy/2),LINEWIDTH,White(),LS_SOLID);
	sd->DrawLine(Pointf(0,10),Pointf(sz.cx,10+sz.cy/2),LINEWIDTH,White(),LS_DOT);
	sd->DrawLine(Pointf(0,20),Pointf(sz.cx,20+sz.cy/2),LINEWIDTH,White(),LS_DASH);
	sd->DrawLine(Pointf(0,30),Pointf(sz.cx,30+sz.cy/2),LINEWIDTH,White(),LS_DASHDOT);
	sd->DrawLine(Pointf(0,40),Pointf(sz.cx,40+sz.cy/2),LINEWIDTH,White(),LS_DASHDOTDOT);

	// Draw Polygon with a hole
	sd->FillPolygon(vertices,Red());
	
	// Draw Polyline around polygon
	sd->DrawPolyline(vertices,LINEWIDTH,White());
	
	// Draw rectangle
	sd->FillRectangle(Rectf(20,20,50,50),Yellow());
	
	// Draw image:		
	sd->DrawImage(Pointf(35,40),Image::Hand());
	
	// Draw text
	sd->DrawText(Pointf(20,80),"Text rotated 25 degrees",25,font,Black());
	sd->DrawText(Pointf(20,80),"Text rotated 0 degrees",0,font,Black());
	
	sd->BeginClip(Rectf(200,5,250,50),true);
	sd->FillRectangle(Rectf(0,0,50,45),Green());
	sd->DrawLine(Pointf(10,10),Pointf(10,15),1,Black());
	sd->DrawLine(Pointf(10,15),Pointf(5,15),1,Black());
	sd->DrawText(Pointf(15,15),"Clipped!",-25,font,Black());
	sd->EndClip();

	// Ellipses
	sd->FillEllipse(Pointf(280,300),90,50,White());
	sd->FillEllipse(Pointf(280,300),50,50,Green());
	sd->FillEllipse(Pointf(280,300),20,50,Black());
	
	// Arcs
	sd->DrawArc(Pointf(280,300),95,55,0,270,3,Yellow());
	sd->DrawArc(Pointf(280,300),10,10,0,360,1,White());
	
	sd->EndClip();
	
	// Verification of final EndClip effect:
	sd->DrawLine(Pointf(sz.cx,0),Pointf(0,sz.cy),LINEWIDTH,Blue(),LS_SOLID);
}

//----------------------------------------------------------------------------------------------

struct TestCtrl : public Ctrl {
public:
	String report;
};

struct PainterExample : public TestCtrl {
public:
	virtual void Paint(Draw &draw) {
		report="[* Painter]";
		ImageBuffer ib(GetSize());
		
		BufferPainter pntr(ib,MODE_ANTIALIASED); // MODE_NOAA,MODE_ANTIALIASED,MODE_SUBPIXEL

		One<OSVGS> GH(new OSVGSPainter(GetSize(),CURRENT_FLAGS,pntr));
		SDDemo(~GH,report);
		draw.DrawImage(0,0,ib);
	}
};

struct DrawExample : public TestCtrl {
public:
	virtual void Paint(Draw &draw) {
		report="[* Draw]";
		One<OSVGS> GH(new OSVGSDraw(GetSize(),CURRENT_FLAGS,draw));
		SDDemo(~GH,report);
	}

};

struct OpenGLExample : GLCtrl {
public:
	double delta;
	String report;
	
	OpenGLExample(){ delta=0; }

	virtual void GLPaint() {
		report="[* OpenGL]";
		One<OSVGS> GH(new OSVGSOpenGL(GetSize(),CURRENT_FLAGS));
		SDDemo(~GH,report);
	}
};

#ifdef _WIN32
struct Direct2DExample : public TestCtrl {
public:
	virtual void Paint(Draw &draw) {
		report="[* Direct2D]";
		HDC hdc=dynamic_cast<SystemDraw *>(&draw)->BeginGdi();
		
		One<OSVGS> GH(new OSVGSDirect2D(GetSize(),CURRENT_FLAGS,hdc));
		SDDemo(~GH,report);
		
		dynamic_cast<SystemDraw *>(&draw)->EndGdi();
	}
};
#endif

class ExampleTopWindow: public TopWindow{
public:
	PainterExample pr;
	DrawExample dw;
	OpenGLExample gl;
#ifdef _WIN32
	Direct2DExample d2d;
#endif	
	
	ExampleTopWindow(){
		dw.SetRect(5,5,395,395);
		Add(dw);

		pr.SetRect(405,5,395,395);
		Add(pr);

		gl.SetRect(805,5,395,395);
		Add(gl);

#ifdef _WIN32
		d2d.SetRect(1205,5,395,395);
		Add(d2d);
#endif		

	}

	void Paint(Draw &w){
		w.DrawRect(w.GetPaintRect(),White());
		w.DrawText(5,405,"SystemDraw");
		w.DrawText(405,405,"Painter");
		w.DrawText(805,405,"OpenGL");
#ifdef _WIN32
		w.DrawText(1205,405,"Direct2D");
#endif
	}

	virtual void LeftDown(Point p,dword keyflags){
		String txt;
		txt.Cat(dw.report);
		txt.Cat(pr.report);
		txt.Cat(gl.report);
#ifdef _WIN32
		txt.Cat(d2d.report);
#endif
		PromptOK(txt);
	}
};

GUI_APP_MAIN
{
	ExampleTopWindow win;
	win.Sizeable().Zoomable();
	win.Title("OSVGS Test");
	win.Open();
	win.Maximize();
	win.Run();
}
