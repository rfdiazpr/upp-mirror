#include "OSVGS.h"

OSVGSDraw::OSVGSDraw(const Size &sz_,int flags_, Draw &draw_): OSVGS(sz_,flags_), draw(draw_){
}

OSVGSDraw::~OSVGSDraw(){
}

void    OSVGSDraw::Flush(){ dynamic_cast<SystemDraw *>(&draw)->Flush(); }

Sizef	OSVGSDraw::GetDPI(){ return Sizef(draw.GetPixelsPerInch()); }
Sizef	OSVGSDraw::GetSize(){ return sz; }

void	OSVGSDraw::BeginClip(const Rectf &rect, bool offset){
	if(offset) draw.ClipoffOp(Rect(rect));
	else draw.ClipOp(Rect(rect));
}

void	OSVGSDraw::EndClip(){
	draw.End();
}

void	OSVGSDraw::Clear(const RGBA &color){ draw.DrawRect(Rect(0,0,(int)floor(sz.cx),(int)floor(sz.cy)),color); }

void	OSVGSDraw::DrawLine(const Pointf &p0,const Pointf &p1, double width, const RGBA &color, int style){
	if(!style) draw.DrawLine(p0,p1,(int)floor(width),color);
	else draw.DrawLine(p0,p1,-style-2,color);
}

void	OSVGSDraw::DrawPolyline(const Vector<Pointf> &vertices, double width, const RGBA &color, int style){
	if(!vertices.GetCount()) return;
	Vector<Point> v;
	for(int i=0;i<vertices.GetCount();i++) v.Add(Point((int)(vertices[i].x),(int)(vertices[i].y)));
	if(!style) draw.DrawPolyline(v,(int)floor(width),color);
	else draw.DrawPolyline(v,-style-2,color);
}

void	OSVGSDraw::FillPolygon(const Vector<Pointf> &vertices, const RGBA &color){
	if(!vertices.GetCount()) return;
	Vector<Point> v;
	for(int i=0;i<vertices.GetCount();i++) v.Add(Point((int)(vertices[i].x),(int)(vertices[i].y)));
	draw.DrawPolygon(v,color);
}

void	OSVGSDraw::DrawImage(const Pointf &p,const Image &img){
	draw.DrawImage((int)floor(p.x),(int)floor(p.y),img); 
	// ::SetSurface seems to be about 3x faster than DrawImage, but does not support 
	// transparent areas, so let's leave the SetSurface stuff for CtrlCore to give optimized 
	// results where speed most matters.
}

void	OSVGSDraw::FillRectangle(const Rectf &rect, const RGBA &color){
	draw.DrawRect((int)floor(rect.left),(int)floor(rect.top),(int)floor(rect.Width()),(int)floor(rect.Height()),color);
}

void	OSVGSDraw::DrawText(const Pointf &p,const String &text,double angle,const Font &font, const RGBA &color){
	draw.DrawText((int)floor(p.x),(int)floor(p.y),(int)floor(angle*10),text,font,color);
}

// Optional: optimized:

void	OSVGSDraw::DrawArc(const Pointf &center, double rx, double ry, double startangle, double stopangle, double width, const RGBA &color, int style){
	Rect rect((int)(center.x-rx),(int)(center.y-ry),(int)(center.x+rx),(int)(center.y+ry));
	Point start((int)(center.x+rx*cos(startangle*(M_PI/180.0))),(int)(center.y-ry*sin(startangle*(M_PI/180.0))));
	Point end((int)(center.x+rx*cos(stopangle*(M_PI/180.0))),(int)(center.y-ry*sin(stopangle*(M_PI/180.0))));
	draw.DrawArcOp(rect,start,end,style?style-2:(int)width,color);
}

void	OSVGSDraw::FillEllipse(const Pointf &center, double rx, double ry, const RGBA &color){
	Rect rect((int)(center.x-rx),(int)(center.y-ry),(int)(center.x+rx),(int)(center.y+ry));
	draw.DrawEllipse(rect,color);
}

