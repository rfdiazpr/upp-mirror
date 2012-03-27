#ifndef _OSVGS_OSVGS_h_
#define _OSVGS_OSVGS_h_

#ifdef _WIN32

#ifndef WINVER                  // Minimum platform is Windows 7
#define WINVER 0x0601
#endif

#ifndef _WIN32_WINNT            // Minimum platform is Windows 7
#define _WIN32_WINNT 0x0601
#endif

#ifndef _WIN32_WINDOWS          // Minimum platform is Windows 7
#define _WIN32_WINDOWS 0x0601
#endif

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#pragma comment( lib, "d2d1.lib" )		// Search For GLu32.lib While Linking
#pragma comment( lib, "dwrite.lib" )		// Search For GLu32.lib While Linking

#endif

#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.

#ifdef _WIN32
#pragma comment( lib, "opengl32.lib" )	// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )		// Search For GLu32.lib While Linking
#endif

#include <GL/gl.h>
#include <GL/glu.h>


// ---------------------------------------------------------------------------------------------
// Defines

#define	LS_SOLID			0
#define	LS_DASH				1
#define	LS_DOT				2
#define	LS_DASHDOT			3
#define	LS_DASHDOTDOT		4

#define OSVGS_ANTIALIASED	1

// ---------------------------------------------------------------------------------------------
// Virtual base class

class OSVGS{
protected:
	dword	flags;
	Sizef	sz;
	
inline	Pointf	ellipse(const Pointf &center, double rx, double ry, double angle){
		return center+Pointf(rx*cos(angle),-ry*sin(angle));	// Mathematical rotation
	}
	
public:
	OSVGS(Sizef sz_,dword flags_=0): sz(sz_), flags(flags_) { }
virtual ~OSVGS(){}

	// required functionality:

virtual void	Flush(){}	// This is only implemented if target mode is not immediate
virtual Sizef	GetDPI()=0;	// Returns the actual DPI of the underlying hardware for proper scaling
virtual Sizef	GetSize()=0; // Returns the size of drawing area
virtual	void	BeginClip(const Rectf &rect, bool offset=false)=0; // BeginClip calls can be be nested, if offset is true, P0,0 will be aligned with rect(top,left)
virtual	void	EndClip()=0; // End clip must be called for each BeginClip exactly once

// The following functions work in the clipped/offsetted coordinate space:

virtual	void	Clear(const RGBA &color)=0; // Clears the clipped area using color
virtual	void	DrawLine(const Pointf &p0,const Pointf &p1, double width, const RGBA &color, int style=LS_SOLID)=0;
virtual	void	DrawPolyline(const Vector<Pointf> &vertices, double width, const RGBA &color, int style=LS_SOLID)=0;
virtual	void	FillPolygon(const Vector<Pointf> &vertices, const RGBA &color)=0; // Fills the polygon with color
virtual	void	DrawImage(const Pointf &p,const Image &img)=0; // Puts the image at p at 1:1 scale, use BeginClip/EndClip to limit area
virtual	void	FillRectangle(const Rectf &rect, const RGBA &color)=0; // Fills rectangle with color
virtual	void	DrawText(const Pointf &p,const String &text,double angle,const Font &font, const RGBA &color)=0; // Draws the angle degrees rotated text at p

// optional functionality: Emulated here if not implemented by the backend:

virtual void	DrawArc(const Pointf &center, double rx, double ry, double startangle, double stopangle, double width, const RGBA &color, int style=LS_SOLID){
	double start=startangle*(M_PI/180.0);
	double step=4.0/max(rx,ry);
	double stop=stopangle*(M_PI/180.0);
	Vector<Pointf> vertices;
	for(double angle=start;angle<stop;angle+=step) vertices.Add(ellipse(center,rx,ry,angle));
	vertices.Add(ellipse(center,rx,ry,stop));
	DrawPolyline(vertices,width,color,style);
}

virtual void	FillEllipse(const Pointf &center, double rx, double ry, const RGBA &color){
	double step=4.0/max(rx,ry);
	Vector<Pointf> vertices;
	for(double angle=0;angle<2*M_PI;angle+=step) vertices.Add(ellipse(center,rx,ry,angle));
	vertices.Add(ellipse(center,rx,ry,2*M_PI));
	FillPolygon(vertices,color);
}

};

// ---------------------------------------------------------------------------------------------
// SystemDraw implementation

class OSVGSDraw: public OSVGS{
protected:
	Draw &draw;
	
public:
	OSVGSDraw(const Size &sz_,int flags_, Draw &draw_);	
	~OSVGSDraw();

virtual void    Flush();
virtual Sizef	GetDPI();
virtual Sizef	GetSize();
virtual	void	BeginClip(const Rectf &rect, bool offset=false);
virtual	void	EndClip();

virtual	void	Clear(const RGBA &color);
virtual	void	DrawLine(const Pointf &p0,const Pointf &p1, double width, const RGBA &color, int style=LS_SOLID);
virtual	void	DrawPolyline(const Vector<Pointf> &vertices, double width, const RGBA &color, int style=LS_SOLID);
virtual	void	FillPolygon(const Vector<Pointf> &vertices, const RGBA &color);
virtual	void	DrawImage(const Pointf &p,const Image &img);
virtual	void	FillRectangle(const Rectf &rect, const RGBA &color);
virtual	void	DrawText(const Pointf &p,const String &text,double angle,const Font &font, const RGBA &color);
virtual void	DrawArc(const Pointf &center, double rx, double ry, double startangle, double stopangle, double width, const RGBA &color, int style=LS_SOLID);
virtual void	FillEllipse(const Pointf &center, double rx, double ry, const RGBA &color);

};

// ---------------------------------------------------------------------------------------------
// Painter implementation

class OSVGSPainter: public OSVGS{
protected:
	Painter &painter;
	
	virtual inline Vector<double> dash_pattern(int linestyle,double linewidth){
		Vector<double> dashes;
		switch(linestyle){
			default:
			case LS_DOT:
				dashes.Add(1*linewidth);
				dashes.Add(2*linewidth);
				return dashes;
			case LS_DASH:
				dashes.Add(5*linewidth);
				dashes.Add(3*linewidth);
				return dashes;
			case LS_DASHDOT:
				dashes.Add(8*linewidth);
				dashes.Add(2*linewidth);
				dashes.Add(1*linewidth);
				dashes.Add(2*linewidth);
				return dashes;
			case LS_DASHDOTDOT:
				dashes.Add(8*linewidth);
				dashes.Add(2*linewidth);
				dashes.Add(1*linewidth);
				dashes.Add(2*linewidth);
				dashes.Add(1*linewidth);
				dashes.Add(2*linewidth);
				return dashes;
		}
	}
	
	virtual inline void	Stroke(double width,const RGBA &color,int style){
		if(style!=LS_SOLID) painter.Dash(dash_pattern(style,width),0);
		painter.Stroke(width,color);		
	}
	
	
public:
	OSVGSPainter(const Size &sz_, int flags_, Painter &painter_);
	~OSVGSPainter();

virtual Sizef	GetDPI();
virtual Sizef	GetSize();
virtual	void	BeginClip(const Rectf &rect, bool offset=false);
virtual	void	EndClip();

virtual	void	Clear(const RGBA &color);
virtual	void	DrawLine(const Pointf &p0,const Pointf &p1, double width, const RGBA &color, int style=LS_SOLID);
virtual	void	DrawPolyline(const Vector<Pointf> &vertices, double width, const RGBA &color, int style=LS_SOLID);
virtual	void	FillPolygon(const Vector<Pointf> &vertices, const RGBA &color);
virtual	void	DrawImage(const Pointf &p,const Image &img);
virtual	void	FillRectangle(const Rectf &rect, const RGBA &color);	
virtual	void	DrawText(const Pointf &p,const String &text,double angle,const Font &font, const RGBA &color);
virtual void	DrawArc(const Pointf &center, double rx, double ry, double startangle, double stopangle, double width, const RGBA &color, int style=LS_SOLID);
virtual void	FillEllipse(const Pointf &center, double rx, double ry, const RGBA &color);

};

// ---------------------------------------------------------------------------------------------
// OpenGL 1.1 implementation

#ifndef CALLBACK
#define CALLBACK
#endif

class OSVGSOpenGL: public OSVGS{
	
	class Vertex3D: public Moveable<Vertex3D>{
	public:
		double x,y,z;
		Vertex3D(){
			x=0;
			y=0;
			z=0;
		}
		Vertex3D(double x_,double y_,double z_=0){
			x=x_;
			y=y_;
			z=z_;
		}
	};


	class Cliprect{
	public:
		Rectf 	rect;
		bool	offset;
		Cliprect(const Rectf &r,bool off){
			rect=r;
			offset=off;
		}
	};
	
	Array<Cliprect> translation;

	GLUtesselator *tobj;
	
	static void CALLBACK combineCallback(GLdouble c[3], void *d[4], GLfloat w[4], void **out) { 
		GLdouble *nv = (GLdouble *) malloc(sizeof(GLdouble)*3); // can't use new, this probably get's free()d inside opengl
		nv[0] = c[0]; 
		nv[1] = c[1]; 
		nv[2] = c[2]; 
		*out = nv;  
	}
	
	double linewidth;
	int linestyle;
	RGBA	rgba;
	
inline	void	SetColor(const RGBA &color){
		if(color!=rgba){
			rgba=color;
			glColor4ub(color.r,color.g,color.b,color.a);
		}
	}

inline	void	SetLineStyle(const RGBA &color,double width,int style){
		SetColor(color);
		if(linewidth!=width) glLineWidth((GLfloat)(linewidth=width));
		if(linestyle!=style){
			linestyle=style;
			switch(linestyle){
				case LS_SOLID:
					glDisable(GL_LINE_STIPPLE);
					break;
				case LS_DASH:
					glEnable(GL_LINE_STIPPLE);
					glLineStipple((GLint)ceil(width),0xf8f8);
					break;
				case LS_DOT:
					glEnable(GL_LINE_STIPPLE);
					glLineStipple((GLint)ceil(width),0x8888);
					break;
				case LS_DASHDOT:
					glEnable(GL_LINE_STIPPLE);
					glLineStipple((GLint)ceil(width),0xe4e4);
					break;
				case LS_DASHDOTDOT:
					glEnable(GL_LINE_STIPPLE);
					glLineStipple((GLint)ceil(width),0xff24);
					break;
			}
		}
	}
	
	
public:
	
	OSVGSOpenGL(const Size &sz_, int flags_);
	~OSVGSOpenGL();
	
virtual void	Flush();
virtual Sizef	GetDPI();
virtual Sizef	GetSize();
virtual	void	BeginClip(const Rectf &rect, bool offset=false);
virtual	void	EndClip();

virtual	void	Clear(const RGBA &color);
virtual	void	DrawLine(const Pointf &p0,const Pointf &p1, double width, const RGBA &color, int style=LS_SOLID);
virtual	void	DrawPolyline(const Vector<Pointf> &vertices, double width, const RGBA &color, int style=LS_SOLID);
virtual	void	FillPolygon(const Vector<Pointf> &vertices, const RGBA &color);
virtual	void	DrawImage(const Pointf &p,const Image &img);
virtual	void	FillRectangle(const Rectf &rect, const RGBA &color);
virtual	void	DrawText(const Pointf &p, const String &text, double angle, const Font &font, const RGBA &color);
virtual void	DrawArc(const Pointf &center, double rx, double ry, double startangle, double stopangle, double width, const RGBA &color, int style=LS_SOLID);
virtual void	FillEllipse(const Pointf &center, double rx, double ry, const RGBA &color);

};

// ---------------------------------------------------------------------------------------------
// Direct2D implementation: Win32 only

#ifdef _WIN32

class OSVGSDirect2D: public OSVGS{
private:

	template<class Interface> inline void SafeRelease(Interface **ppInterfaceToRelease){
	    if (*ppInterfaceToRelease != NULL){
	        (*ppInterfaceToRelease)->Release();
	        (*ppInterfaceToRelease) = NULL;
	    }
	}
	
protected:
	Array<D2D1_MATRIX_3X2_F> translation;
		
	ID2D1Factory *m_pD2DFactory;
    ID2D1DCRenderTarget *m_pDCRT;
    ID2D1SolidColorBrush *m_pBrush;
	ID2D1StrokeStyle	*m_pStrokeStyle[5];
     
	IDWriteFactory *m_pDWriteFactory;
	IDWriteTextFormat *m_pTextFormat;
	
	Font current_font;
	RGBA current_color;
	
	void	SetFont(const Font &font);
	
inline void	SetColor(const RGBA &color){
		if(current_color!=color){
			current_color=color;
			if(!m_pBrush) m_pDCRT->CreateSolidColorBrush(D2D1::ColorF(color.r/255.0f,color.g/255.0f,color.b/255.0f,color.a/255.0f),&m_pBrush);
			else m_pBrush->SetColor(D2D1::ColorF(color.r/255.0f,color.g/255.0f,color.b/255.0f,color.a/255.0f));
		}
	}

public:
	OSVGSDirect2D(const Size &sz_,int flags_,HDC hdc);
	~OSVGSDirect2D();
	
virtual void	Flush();
virtual Sizef	GetDPI();
virtual Sizef	GetSize();
virtual	void	BeginClip(const Rectf &rect, bool offset=false);
virtual	void	EndClip();

virtual	void	Clear(const RGBA &color);
virtual	void	DrawLine(const Pointf &p0,const Pointf &p1, double width, const RGBA &color, int style=LS_SOLID);
virtual	void	DrawPolyline(const Vector<Pointf> &vertices, double width, const RGBA &color, int style=LS_SOLID);
virtual	void	FillPolygon(const Vector<Pointf> &vertices, const RGBA &color);
virtual	void	DrawImage(const Pointf &p,const Image &img);
virtual	void	FillRectangle(const Rectf &rect, const RGBA &color);
virtual	void	DrawText(const Pointf &p,const String &text,double angle,const Font &font, const RGBA &color);
virtual void	DrawArc(const Pointf &center, double rx, double ry, double startangle, double stopangle, double width, const RGBA &color, int style=LS_SOLID);
virtual void	FillEllipse(const Pointf &center, double rx, double ry, const RGBA &color);

};
#endif

#endif
