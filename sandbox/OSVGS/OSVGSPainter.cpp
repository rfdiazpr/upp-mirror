#include "OSVGS.h"

OSVGSPainter::OSVGSPainter(const Size &sz_, int flags_, Painter &painter_): OSVGS(sz_,flags_), painter(painter_) {
	painter.EvenOdd();
}

OSVGSPainter::~OSVGSPainter(){
	
}

Sizef	OSVGSPainter::GetDPI(){ return Sizef(96,96); } // TODO
Sizef	OSVGSPainter::GetSize(){ return sz; }

void	OSVGSPainter::BeginClip(const Rectf &rect, bool offset){
	if(offset) painter.ClipoffOp(Rect(rect));
	else painter.ClipOp(Rect(rect));
}

void	OSVGSPainter::EndClip(){
	painter.End();
}

void	OSVGSPainter::Clear(const RGBA &color){ painter.DrawRect(Rect(0,0,(int)floor(sz.cx),(int)floor(sz.cy)),color); }

void	OSVGSPainter::DrawLine(const Pointf &p0,const Pointf &p1, double width, const RGBA &color, int style){
	painter.Move(p0).Line(p1);
	Stroke(width,color,style);
}

void	OSVGSPainter::DrawPolyline(const Vector<Pointf> &vertices, double width, const RGBA &color, int style){
	if(!vertices.GetCount()) return;
	painter.Move(vertices[0]);
	for(int i=1;i<vertices.GetCount();i++) painter.Line(vertices[i]);
	Stroke(width,color,style);
}

void	OSVGSPainter::FillPolygon(const Vector<Pointf> &vertices, const RGBA &color){
	if(!vertices.GetCount()) return;
	painter.Move(vertices[0]);
	for(int i=1;i<vertices.GetCount();i++) painter.Line(vertices[i]);
	painter.Fill(color);
}

void	OSVGSPainter::DrawImage(const Pointf &p,const Image &img){
	painter.DrawImage((int)floor(0.5+p.x),(int)floor(0.5+p.y),img);
}

void	OSVGSPainter::FillRectangle(const Rectf &rect, const RGBA &color){
	painter.Rectangle(rect.left,rect.top,rect.Width(),rect.Height());
	painter.Fill(color);
}

void	OSVGSPainter::DrawText(const Pointf &p,const String &text,double angle,const Font &font, const RGBA &color){
	painter.Begin();
	painter.EvenOdd(true);
	painter.Translate(p.x, p.y);
	painter.Rotate(-angle*(M_PI/180));
	painter.Text(0, 0, WString(text), font, text.GetCharCount(), 0);
	painter.Fill(color);
	if(font.IsUnderline()) {
		int cx = GetTextSize(text, font).cx;
		int a = font.GetAscent();
		int cy = max(a / 16, 1);
		painter.Rectangle(0, a + cy, cx, cy);
		painter.Fill(color);
	}
	if(font.IsStrikeout()) {
		int cx = GetTextSize(text, font).cx;
		int a = font.GetAscent();
		int cy = max(a / 16, 1);
		painter.Rectangle(0, 2 * a / 3, cx, cy);
		painter.Fill(color);
	}
	painter.End();
}

void	OSVGSPainter::DrawArc(const Pointf &center, double rx, double ry, double startangle, double stopangle, double width, const RGBA &color, int style){
	painter.Move(ellipse(center,rx,ry,startangle*(M_PI/180.0)));
	painter.Arc(center,rx,ry,startangle*(M_PI/180.0),(startangle-stopangle)*(M_PI/180.0));
	Stroke(width,color,style);
}

void	OSVGSPainter::FillEllipse(const Pointf &center, double rx, double ry, const RGBA &color){
	painter.Ellipse(center.x,center.y,rx,ry).Fill(color);
}

