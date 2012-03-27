#include "OSVGS.h"

#ifndef _WIN32
#define __stdcall
#endif


OSVGSOpenGL::OSVGSOpenGL(const Size &sz_, int flags_): OSVGS(sz_,flags_) {
	linewidth=-1;
	linestyle=-1;
	rgba=White();
	rgba.a=255;
	
	tobj = gluNewTess();
	
	gluTessCallback(tobj, GLU_TESS_VERTEX, (void (__stdcall *) ( )) &glVertex3dv);
	gluTessCallback(tobj, GLU_TESS_BEGIN, (void (__stdcall *) ( )) &glBegin);
	gluTessCallback(tobj, GLU_TESS_END, (void (__stdcall *) ( )) &glEnd);
//	gluTessCallback(tobj, GLU_TESS_ERROR, (void (__stdcall *) ( )) &errorCallback);
	gluTessCallback(tobj, GLU_TESS_COMBINE, (void (__stdcall *) ( )) &combineCallback);
	
	gluTessProperty(tobj, GLU_TESS_WINDING_RULE,GLU_TESS_WINDING_ODD);		
	
	glPushMatrix();
	
	glClearColor(1.f,1.f,1.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_SCISSOR_TEST);
	glOrtho(0,sz.cx,sz.cy,0,-1,1);
	
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
/*
	// The following code enables antialiasing at primitive level. 
	// However, the results may be less than satisfactory.
	// Based on some googling I have gathered that FSAA (full-scene-AA)
	// or MSAA (Multisample-AA) and similar technologies are today's 
	// solution for OpenGL AA. Enabling them from code requires certain 
	// OpenGL extensions being available, and current Win32 OpenGL 
	// implementation does not directly support them.
	//
	// Anyway, depending on the used GPU, there may be / are ways to enable 
	// FSAA/MSAA on application level or global level. (Tested on NVIDIA GPU.)
	// A basic 4x antialiasing already gives (not perfect but) pretty decent 
	// results.

	if(flags&SD_ANTIALIASED){
		glShadeModel(GL_SMOOTH);
		
		glEnable(GL_POLYGON_SMOOTH);
		glEnable(GL_POINT_SMOOTH);		
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);		
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);		
	}
*/
	glMatrixMode(GL_MODELVIEW); // GL_PROJECTION

	translation.Add(Cliprect(Rectf(sz), false));
}

OSVGSOpenGL::~OSVGSOpenGL(){
	glPopMatrix();
	glFinish();
	gluDeleteTess(tobj);
}

void	OSVGSOpenGL::Flush(){ /*glFlush(); */glFinish(); }
Sizef	OSVGSOpenGL::GetDPI(){ return Sizef(96,96); } // TODO
Sizef	OSVGSOpenGL::GetSize(){ return sz; }

void	OSVGSOpenGL::BeginClip(const Rectf &rect, bool offset){
	glPushMatrix();
	
	Rectf &r=translation.Add(Cliprect(rect,offset)).rect;
	if(translation[translation.GetCount()-2].offset) r+=Pointf(translation[translation.GetCount()-2].rect);
	r.Intersect(translation[translation.GetCount()-2].rect);
	glScissor((GLint)r.left, (GLint)(sz.cy - r.top - r.Height()), (GLsizei)r.Width(), (GLsizei)r.Height());		
	if(offset) glTranslated((float)rect.left, (float)rect.top,0);
}

void	OSVGSOpenGL::EndClip(){
	if(translation.GetCount()>1) translation.Remove(translation.GetCount()-1);
	glScissor((GLint)translation[translation.GetCount()-1].rect.left,
	          (GLint)translation[translation.GetCount()-1].rect.top,
	          (GLsizei)translation[translation.GetCount()-1].rect.right,
	          (GLsizei)translation[translation.GetCount()-1].rect.bottom);
	glPopMatrix();
}

void	OSVGSOpenGL::Clear(const RGBA &color){ 
	SetColor(color);
	glRectd(0,0,sz.cx,sz.cy);
}

void	OSVGSOpenGL::DrawLine(const Pointf &p0,const Pointf &p1, double width, const RGBA &color, int style){
	SetLineStyle(color,width,style);
	glBegin(GL_LINE_STRIP); // Polyline
	glVertex2d(p0.x,p0.y);
	glVertex2d(p1.x,p1.y);
	glEnd();
}

void	OSVGSOpenGL::DrawPolyline(const Vector<Pointf> &vertices, double width, const RGBA &color, int style){
	if(!vertices.GetCount()) return;
	SetLineStyle(color,width,style);
	glBegin(GL_LINE_STRIP); // Polyline
	for(int i=0;i<vertices.GetCount();i++) glVertex2d(vertices[i].x,vertices[i].y);
	glEnd();
}

void	OSVGSOpenGL::FillPolygon(const Vector<Pointf> &vertices, const RGBA &color){
	if(!vertices.GetCount()) return;
	SetColor(color);
	
	gluTessBeginPolygon (tobj, NULL); 
	gluTessBeginContour (tobj);  
	Vector<Vertex3D> v;
	for(int i=0;i<vertices.GetCount();i++){	
		v.Add(Vertex3D(vertices[i].x,vertices[i].y));
		gluTessVertex (tobj, (GLdouble*)&v[i], (GLdouble*)&v[i]); 
	}
	gluTessEndContour (tobj); 		
	gluEndPolygon (tobj);	
}

void	OSVGSOpenGL::DrawImage(const Pointf &p,const Image &img){
	glRasterPos2d(p.x,p.y);
	glPixelZoom(1.0f,-1.0f);
	glDrawPixels(img.GetSize().cx,img.GetSize().cy,GL_BGRA_EXT,GL_UNSIGNED_BYTE,~img);
}

/*		
	// This may be good to know too for some:
	ImageBuffer ib(sz);
	glReadPixels(0,0,sz.cx,sz.cy,GL_BGRA_EXT,GL_UNSIGNED_BYTE,(GLvoid*)(RGBA*)ib); 
*/		


void	OSVGSOpenGL::FillRectangle(const Rectf &rect, const RGBA &color){
	SetColor(color);
	glRectd(rect.left,rect.top,rect.right,rect.bottom);
}

void	OSVGSOpenGL::DrawText(const Pointf &p, const String &text, double angle, const Font &font, const RGBA &color){
	// TODO
}

void	OSVGSOpenGL::DrawArc(const Pointf &center, double rx, double ry, double startangle, double stopangle, double width, const RGBA &color, int style){
	SetLineStyle(color,width,style);
	double start=startangle*(M_PI/180.0);
	double step=4.0/max(rx,ry);
	double stop=stopangle*(M_PI/180.0);

	glBegin(GL_LINE_STRIP); // Polyline
	for(double angle=start;angle<stop;angle+=step){
		Pointf p=ellipse(center,rx,ry,angle);
		glVertex2d(p.x,p.y);
	}
	Pointf p=ellipse(center,rx,ry,stop);
	glVertex2d(p.x,p.y);
	glEnd();
}

void	OSVGSOpenGL::FillEllipse(const Pointf &center, double rx, double ry, const RGBA &color){
	SetColor(color);
	double step=4.0/max(rx,ry);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2d(center.x,center.y);
	for(double angle=0;angle<2*M_PI;angle+=step){
		Pointf p=ellipse(center,rx,ry,angle);
		glVertex2d(p.x,p.y);
	}
	Pointf p=ellipse(center,rx,ry,2*M_PI);
	glVertex2d(p.x,p.y);
	glEnd();
}

