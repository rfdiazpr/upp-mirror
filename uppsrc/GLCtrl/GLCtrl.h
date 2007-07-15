#ifndef _GLCtrl_GLCtrl_h
#define _GLCtrl_GLCtrl_h

#include <CtrlLib/CtrlLib.h>

#include <GL/gl.h>
#include <GL/glu.h>

NAMESPACE_UPP

#ifdef PLATFORM_X11

#include <GL/glx.h>

class GLCtrl : public Ctrl
{
	GLXContext SubWindowContext;
	Window     SubWindow;
	bool       IsInitialized,
	           IsMapped;
	int        DepthSize,
	           StencilSize,
	           NumberOfSamples;
	bool       DoubleBuffering,
			   MultiSampleBuffering,
			   InitializationProblem;
	String     ErrorMessage;

	int        NumInstance;
	static int NbInstance;
	static int ContextActivated;
	
	void ActivateContext();

	Point CurrentPos;
	
	Rect RectInTopWindow() const;
	void MapWindow();
	void UnMapWindow();
	bool CreateGLXWindow( Window &, GLXContext & );
	void RemoveUppXWindow();
	void EventProc(XWindow& w, XEvent *event);
	void Paint(Draw& draw);

	void MoveSubWindow();
	void OpenGLPaint();
	void OpenGLResize();

	void OpenGL();
	void CloseGL();
	
public:
	typedef GLCtrl CLASSNAME;

	GLCtrl( int  depthsize            = 24, 
	        int  stencilsize          = 0, 
	        bool doublebuffer         = true, 
			bool multisamplebuffering = false, 
			int  numberofsamples      = 0 );

	~GLCtrl();

	// User overridable methods
	virtual void GLInit();
	virtual void GLDone();	
	virtual void GLResize( int w, int h );
	virtual void GLPaint();
	virtual void State(int reason);

	void StdView();
	void Refresh();
	void PostPaintGL(); // same as Refresh()
	void PostResizeGL();
};

#else

class GLCtrl : public DHCtrl {
public:
	virtual void    State(int reason);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	HDC   hDC;
	HGLRC hRC;

	void OpenGL();
	void CloseGL();

public:
	virtual void GLPaint();

	Callback WhenGLPaint;

	void    StdView();

	HDC     GetDC() const            { return hDC; }
	HGLRC   GetHGLRC() const         { return hRC; }

	GLCtrl();
	~GLCtrl();
};
#endif

END_UPP_NAMESPACE

#endif
