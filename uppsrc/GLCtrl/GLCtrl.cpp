#include "GLCtrl.h"

NAMESPACE_UPP

#ifdef PLATFORM_X11

int GLCtrl::NbInstance = 0;
int GLCtrl::ContextActivated = 0;

bool InCreation = false; // to prevent CreateGLXWindow to be called two times

GLCtrl::GLCtrl( int depthsize, int stencilsize, bool doublebuffer, 
				bool multisamplebuffering, int numberofsamples )
{
	NumInstance = NbInstance;
	NbInstance++;
	
	IsInitialized    = false;
	IsMapped         = false;
	SubWindow        = 0;
	SubWindowContext = NULL;

	DepthSize        = depthsize;
	StencilSize      = stencilsize;
	DoubleBuffering  = doublebuffer;
	NumberOfSamples  = numberofsamples;

	CurrentPos.Clear();
	
	InitializationProblem = false;
	ErrorMessage          = "GL/GLX error.";
}

GLCtrl::~GLCtrl()
{
	if( IsInitialized )
		CloseGL();
}

bool GLCtrl::CreateGLXWindow( Window &Window, GLXContext &WindowContext )
{
	if( InCreation == true )
		return false;
	InCreation = true;
	
	Vector<int> visual;
	visual << GLX_RGBA << GLX_DEPTH_SIZE << DepthSize;
	
	if( StencilSize > 0 )
		visual << GLX_STENCIL_SIZE << StencilSize;
	
	if( DoubleBuffering )
		visual << GLX_DOUBLEBUFFER;
	
	if( MultiSampleBuffering && NumberOfSamples > 1 )
	{
		visual << GLX_SAMPLE_BUFFERS_ARB << 1 << GLX_SAMPLES_ARB << NumberOfSamples;
	}
	
	visual << None;

	// Try to find a visual
	XVisualInfo *visualInfo = NULL;
	visualInfo = glXChooseVisual( (Display*)Xdisplay, DefaultScreen(Xdisplay), visual );
	
	if( visualInfo == NULL )
	{
		ErrorMessage = "CreateGLXWindow : Impossible to find a suitable visual.";
		InCreation = false;
		return false;
	}

	// Create an OpenGL rendering context
	WindowContext = glXCreateContext((Display *)Xdisplay, 
									visualInfo,
									NULL,      // No sharing of display lists
									GL_TRUE ); // Direct rendering if possible

	if( WindowContext == NULL )
	{
		ErrorMessage = "CreateGLXWindow : GLX context creation error.";
		InCreation = false;
		return false;
	}

	if( !glXIsDirect( (Display*)Xdisplay, WindowContext ) )
		printf("GLCtrl::OpenGL : Warning : DRI off.");

	// Create an X colormap since we're not using the default visual
	Colormap colorMap;
	colorMap = XCreateColormap( Xdisplay,
								GetTopWindow()->GetWindow(),
								visualInfo->visual,
								AllocNone );

	XSetWindowAttributes winAttr;
	winAttr.colormap     = colorMap;
	winAttr.border_pixel = 0;
	winAttr.event_mask   = ExposureMask;
	winAttr.save_under   = XFalse;
	
	// Create an X window with the selected visual
	Rect r = RectInTopWindow();
	Window = XCreateWindow( Xdisplay,                              // display
							GetTopWindow()->GetWindow(),           // parent
							r.left, r.top, r.Width(), r.Height(),  // x, y, width, height
							0,                                     // border_width
							visualInfo->depth,                     // depth
							InputOutput,                           // class
							visualInfo->visual,                    // visual
							CWBorderPixel | CWColormap | 
							CWSaveUnder   | CWEventMask,           // value mask
							&winAttr );                            // attributes
	
	if( Window == 0 )
	{
		ErrorMessage = "CreateGLXWindow : XCreate window error.";
		InCreation = false;
		return false;
	}

	// Add subwindow to upp list of Xwindows
	int i = Xwindow().Find(None);
	if(i >= 0) Xwindow().SetKey(i, Window );
	XWindow& cw = i >= 0 ? Xwindow()[i] : Xwindow().Add(Window);
	cw.ctrl    = this;
	cw.exposed = true;
	cw.owner   = GetParent();
	cw.xic     = NULL;
	
	GLX = true;
	InCreation = false;
	return true;
}

void GLCtrl::EventProc(XWindow& w, XEvent *event)
{
	// Flush 'Expose' events
	while( XCheckWindowEvent( Xdisplay, SubWindow, ExposureMask, event )) {};

	if( IsMapped )
		OpenGLPaint();

	Ctrl::EventProc(w, event );
}

void GLCtrl::OpenGLResize()
{
	if( !IsInitialized )
		return;
	
	int w = GetSize().cx,
		h = GetSize().cy;
	
	ActivateContext();
	
	// Call user function
	GLResize( w, h );	
}

void GLCtrl::OpenGLPaint()
{
	if( IsMapped && IsInitialized )
	{
		ActivateContext();
		
		// Call user function
		GLPaint();
		
		if( DoubleBuffering )
			glXSwapBuffers( (Display*)Xdisplay, SubWindow ); // Buffer swap does implicit glFlush		
		else
			glFlush();
	}
}

void GLCtrl::ActivateContext()
{
	if( NbInstance > 1 && ContextActivated != NumInstance && IsInitialized )
	{
		glXMakeCurrent( (Display*)Xdisplay, SubWindow, SubWindowContext );
		ContextActivated = NumInstance;
	}
}

Rect GLCtrl::RectInTopWindow() const
{
	return GetScreenView() - GetTopCtrl()->GetScreenRect().TopLeft();
}

void GLCtrl::MoveSubWindow()
{
	Rect r = RectInTopWindow();
	int w = GetSize().cx,
		h = GetSize().cy;
	
	// TODO: use only XMove when needed	 
	XMoveResizeWindow( Xdisplay, SubWindow, r.TopLeft().x, r.TopLeft().y, w, h );
}

void GLCtrl::State(int reason)
{
	if( InitializationProblem )
		return;
	
	if( IsInitialized )
	{
		switch( reason )
		{
			case SHOW: 
			{
				if( IsShown() && !IsMapped )
					MapWindow();

				if( !IsShown() && IsMapped )
					UnMapWindow();
			}; break;
			
			case LAYOUTPOS:
			{
				MoveSubWindow();
				OpenGLResize();
			}; break;
			
			default:
				break;
		}
	}
	else
		if( GetTopWindow() && GetTopWindow()->GetWindow() )
			OpenGL();
}

void GLCtrl::Paint(Draw& draw)
{
	if( IsInitialized )
	{
		Point p = GetRect().TopLeft();
		if( CurrentPos != p )
		{
			CurrentPos = p;
			MoveSubWindow();
		}
	}

	else
		if( !GLX )
		{
			Size sz = GetSize();
			draw.DrawRect(0, 0, sz.cx, sz.cy, SWhite);
			if( InitializationProblem )
				draw.DrawText(3,3, "GLCtrl::"+ErrorMessage, Arial(12).Bold(), SRed);			
		}
}

void GLCtrl::MapWindow()
{
	if( !IsMapped )
	{
		XMapWindow( Xdisplay, SubWindow );
		IsMapped = true;
	}
}

void GLCtrl::UnMapWindow()
{
	if( IsMapped )
	{
		XUnmapWindow( Xdisplay, SubWindow );		
		IsMapped = false;
	}
}

void GLCtrl::OpenGL()
{
	if( IsInitialized || InitializationProblem || InCreation )
		return;
	
	int errorBase;
	int eventBase;
	
	// Make sure OpenGL's GLX extensions supported
	if( !glXQueryExtension( (Display*)Xdisplay, &errorBase, &eventBase ) )
	{
		ErrorMessage = "OpenGL : GLX not supported.";
		InitializationProblem = true;
		return;
	}
	
	// Create glxwindow
	if( CreateGLXWindow( SubWindow, SubWindowContext ) )
	{
		// Activate the created glxwindow
		glXMakeCurrent( (Display*)Xdisplay, SubWindow, SubWindowContext );
		
		IsInitialized = true;
		
		MapWindow();
	
		// Call user init function
		GLInit();
		GLResize( GetSize().cx, GetSize().cy );
		OpenGLPaint();		
	}
	else
		InitializationProblem = true;
}

void GLCtrl::RemoveUppXWindow()
{
	int i = Xwindow().Find(SubWindow);
	if(i >= 0) {
		Xwindow().SetKey(i, None);
		Xwindow()[i].ctrl = NULL;
	}
}

void GLCtrl::CloseGL()
{
	if( IsInitialized )
	{
		// Call user function
		GLDone();
		
		IsInitialized = false;
		InitializationProblem = false;

		UnMapWindow();
		XDestroyWindow( Xdisplay, SubWindow );

		RemoveUppXWindow();
		
		glXMakeCurrent( (Display*)Xdisplay, None, NULL );
		glXDestroyContext( (Display *)Xdisplay, SubWindowContext );		
	}
}

void GLCtrl::PostResizeGL()
{
	if( !IsInitialized )
		return;
	
	OpenGLResize();
}

void GLCtrl::PostPaintGL()
{
	if( !IsInitialized )
		return;
	
	OpenGLPaint();
}

void GLCtrl::GLInit(){}
void GLCtrl::GLDone(){}
void GLCtrl::GLResize( int w, int h ){}
void GLCtrl::GLPaint(){}

void GLCtrl::StdView()
{
	if( !IsInitialized )
		return;
		
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	Size sz = GetSize();
	glViewport(0, 0, (GLsizei)sz.cx, (GLsizei)sz.cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)(sz.cx)/(GLfloat)(sz.cy), 1.0f, 100.0f);		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLCtrl::Refresh()
{
	PostPaintGL();
}

#else

#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking
#pragma comment( lib, "glaux.lib" )								// Search For GLaux.lib While Linking

GLCtrl::GLCtrl()
{
	hDC = NULL;
	hRC = NULL;
}

GLCtrl::~GLCtrl()
{
	CloseGL();
}

void GLCtrl::OpenGL()
{
	HWND hwnd = GetHWND();
	if(!hwnd)
		return;
	hDC = ::GetDC(hwnd);
	if(!hDC)
		return;
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int pf = ChoosePixelFormat(hDC, &pfd);
	if(!pf) {
		CloseGL();
		return;
	}
	if(!SetPixelFormat(hDC, pf, &pfd)) {
		CloseGL();
		return;
	}
	DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	hRC = wglCreateContext(hDC);
}

void GLCtrl::CloseGL()
{
	if(hRC)
	    wglDeleteContext(hRC);
	if(hDC)
	    ReleaseDC(GetHWND(), hDC);
}

void GLCtrl::State(int reason)
{
	if(reason == CLOSE)
		CloseGL();
	DHCtrl::State(reason);
	if(reason == OPEN)
		OpenGL();
}

void GLCtrl::GLPaint()
{
	WhenGLPaint();
}

void GLCtrl::StdView()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	Size sz = GetSize();
	glViewport(0, 0, (GLsizei)sz.cx, (GLsizei)sz.cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)(sz.cx)/(GLfloat)(sz.cy), 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

LRESULT GLCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_PAINT && hDC && hRC) {
		wglMakeCurrent(hDC, hRC);
		GLPaint();
		SwapBuffers(hDC);
		wglMakeCurrent(NULL, NULL);
		PAINTSTRUCT ps;
		BeginPaint(GetHWND(), &ps);
		EndPaint(GetHWND(), &ps);
		return 0;
	}
	return DHCtrl::WindowProc(message, wParam, lParam);
}

#endif

END_UPP_NAMESPACE
