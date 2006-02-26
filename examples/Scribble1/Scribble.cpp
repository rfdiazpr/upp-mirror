#include <CtrlLib/CtrlLib.h>

class Scribble : public TopWindow {
public:
	virtual void   Paint(Draw& w);
	virtual void   MouseMove(Point p, dword keyflags);
	virtual void   LeftDown(Point p, dword keyflags);

protected:
	Vector< Vector<Point> > image;

public:
	Scribble();
};

void Scribble::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(0, 0, sz.cx, sz.cy, SWhite);
	for(int i = 0; i < image.GetCount(); i++) {
		const Vector<Point>& stroke = image[i];
		Point p1 = stroke[0];
		for(int j = 1; j < stroke.GetCount(); j++) {
			Point p2 = stroke[j];
			w.DrawLine(p1, p2);
			p1 = p2;
		}
		w.DrawRect(p1.x, p1.y, 1, 1, SBlack);
	}
}

void Scribble::MouseMove(Point p, dword keyflags)
{
	if(HasCapture()) {
		image.Top().Add() = p;
		Refresh();
	}
}

void Scribble::LeftDown(Point p, dword keyflags)
{
	image.Add().Add() = p;
	SetCapture();
	Refresh();
}

Scribble::Scribble()
{
	Title("Scribble").Zoomable().Sizeable();
	AddFrame(InsetFrame());
	BackPaint();
}

GUI_APP_MAIN
{
	Scribble().Run();
}
