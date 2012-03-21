#include "UNSView.h"
//#include "GLView1.h"

#include <GL/gl.h>
#include <GL/glu.h>
//#include <ApplicationServices/ApplicationServices.h>

#ifndef flagTEST_UNS
#include <CtrlCore/CtrlCore.h>
#endif


#define LLOG(x) LOG(x)

@implementation UNSViewGL

- (CtrlRef *) uppCtrl
{
	return pCtrl_;
}

//apple requires? glview to have this
+ (NSOpenGLPixelFormat*)defaultPixelFormat  //FIXME what value/s should be here?
{
	static NSOpenGLPixelFormatAttribute attrs[] =
	{
		 NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)32,
		NSOpenGLPFANoRecovery,
		NSOpenGLPFAWindow,
			NSOpenGLPFAColorSize,        (NSOpenGLPixelFormatAttribute) 32, //24,
//			NSOpenGLPFADepthSize,        (NSOpenGLPixelFormatAttribute)16,
			NSOpenGLPFADoubleBuffer,
			NSOpenGLPFAAccelerated,
		NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)1,
		NSOpenGLPFASamples,  (NSOpenGLPixelFormatAttribute)4,
			(NSOpenGLPixelFormatAttribute)0
	};
	static NSOpenGLPixelFormat * pfmt = [[NSOpenGLPixelFormat alloc] initWithAttributes: attrs];  //CHECK memory leaks
	//[pfmt retain];  //CHECK
	return pfmt;
}

- (id)initWithFrameAndCtrlGL:(NSRect)frameRect ctrl:(CtrlRef *)ctrl pixelFormat:(NSOpenGLPixelFormat*)format
{
	LLOG("initWithFrameAndCtrlGL !!!!!!!!!!!!!?  unsgl");
	if(format)
	{
		pixelFormat = format;  //CHECK release if new?		
	}
	else
	{
		pixelFormat = [UNSViewGL defaultPixelFormat];
	}		
	[pixelFormat retain];
	glContext  = [[NSOpenGLContext alloc] initWithFormat: pixelFormat shareContext: nil];
	//	glContext  = [self openGLContext];
	pCtrl_ = ctrl;
	return [self initWithFrame: frameRect];
}

- (id)initWithFrame: (NSRect)frameRect //in case you want to use without Ctrl
{
	LLOG("initWithFrame ======  unsgl");

	self = [super initWithFrame: frameRect];
//	return self;
	
	if(self)
	{
		NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
		
//		[nc addObserver: self
//				selector: @selector( reshape )
//					name: NSViewFrameDidChangeNotification
//					object: self];
					
		[[NSNotificationCenter defaultCenter] addObserver:self
					selector:@selector(_surfaceNeedsUpdate:)
					name:NSViewGlobalFrameDidChangeNotification
					object:self];
	}
	
	return self;
}

- (id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)format
{
	LLOG("initWithFrame & pixel format unsgl");
	self = [super initWithFrame:frameRect];

	if (self != nil)
	{
		pixelFormat   = [format retain];

		[[NSNotificationCenter defaultCenter] addObserver:self
					selector:@selector(_surfaceNeedsUpdate:)
					name:NSViewGlobalFrameDidChangeNotification
					object:self];
	}

	return self;
}

- (NSOpenGLContext*)openGLContext
{
	return 	glContext;
}

//FIXME
-(void) viewDidMoveToWindow
{
//	CocAlert("view did move","");
	LLOG("GL  view did move");
	
//	NSTrackingArea *trackingArea = [[NSTrackingArea alloc] initWithRect:[self frame]
//	    options:NSTrackingMouseMoved+NSTrackingActiveInKeyWindow+NSTrackingMouseEnteredAndExited+NSTrackingInVisibleRect
//	    owner:self
//	    userInfo:nil];
//	[self addTrackingArea:trackingArea];
//	[self becomeFirstResponder];
 //   [super viewDidMoveToWindow];
//	if (context)
//		[ context clearDrawable];
    if ([self window] == nil)
    {
		LLOG("GL  view did move NO WINDOW????");
//	        [context clearDrawable];
        return;
    }
	static NSOpenGLPixelFormatAttribute attrs[] =
	{
		NSOpenGLPFANoRecovery,
		NSOpenGLPFAWindow,
			NSOpenGLPFAColorSize,        (NSOpenGLPixelFormatAttribute)24,
			NSOpenGLPFADepthSize,        (NSOpenGLPixelFormatAttribute)16,
			NSOpenGLPFADoubleBuffer,
			NSOpenGLPFAAccelerated,
		NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)1,
		NSOpenGLPFASamples,  (NSOpenGLPixelFormatAttribute)4,
			(NSOpenGLPixelFormatAttribute)0
	};
	
	if(!ready)
	{
		pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes: attrs];
		glContext  = [[NSOpenGLContext alloc] initWithFormat: pixelFormat shareContext: nil];
	}
	else
		glContext  = [self openGLContext];
		

	if(glContext == nil)
		LLOG("ERROR: failed to retrieve gl context");
	else
		LLOG("retrieve gl context?.. SUCCESS!!!"); 
		   
	[glContext setView:self];
	
	//move below to Ctrl GLInit();
//	[NSOpenGLContext makeCurrentContext];
//	glContext  = [NSOpenGLContext currentContext];
	
//	[[NSOpenGLContext currentContext] setView: self];
	 LLOG("set view gl context?.. SUCCESS 1");     
	NSSize  newSize =[self frame].size;
//	glContext  = [self openGLContext];
	[glContext makeCurrentContext];
	glViewport( 0, 0, (GLsizei)newSize.width, (GLsizei)newSize.height );
	glMatrixMode( GL_PROJECTION ); glLoadIdentity(); 
	gluPerspective( 45.0f, (GLfloat)newSize.width / (GLfloat)newSize.height, 0.1f, 100.0f ); 
	glMatrixMode( GL_MODELVIEW ); glLoadIdentity(); 
	ready = YES;
//[[self openGLContext] update];
}

- (void)dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver: self];
	
	[glContext release];
	[pixelFormat release];
	[super dealloc];
}

- (BOOL)isOpaque
{
	return NO;
}


//CHECK apple recommends this?
- (void) _surfaceNeedsUpdate:(NSNotification*)notification
{
//   [self update];
	[self setNeedsDisplay:YES];
}

- (void)reshape
{
	[self setNeedsDisplay:YES];
	return;
	//move outside  - reshape in Ctrl?
	
	NSSize size = [self frame].size;
	bool notgl = [NSOpenGLContext currentContext] != glContext;
	
	[glContext update];
	
	if(notgl)
		[glContext makeCurrentContext];
	
	glViewport( 0, 0, (GLint) size.width, (GLint) size.height );
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	
//	gluPerspective( .., size.width / size.height, 1.0f, 1000.0f );
	
	if( notgl )
		[NSOpenGLContext clearCurrentContext];
}


- (void)drawRect: (NSRect) rect
{
	if( !ready )
	{
		//FIXME check context???
		return;
	}
	UPP::Ctrl * ctrl = (UPP::Ctrl *) pCtrl_; // [self  uppCtrl];
	
	if(!ctrl)  //assert or just ignore?
		CocAlert("NO UPP::ctrl from unsviewgl drawrect????");

	LLOG("draw rect 1");  //FIXME layers!!!
//	if ([[self window] isVisible] && ctrl &&  ([NSGraphicsContext	currentContextDrawingToScreen]) )
//		ctrl->DoPaint1((UNSGraphicsContextRef)[NSGraphicsContext currentContext], UPP::RectC((int)rect.origin.x,(int)rect.origin.y, (int)rect.size.width, (int)rect.size.height) );

//	return;	
	//FIXME move outside
            
	[glContext makeCurrentContext];

#ifdef __APPLE__
	GLint zeroOpacity = 0;
	[glContext setValues:(GLint*)&zeroOpacity forParameter:NSOpenGLCPSurfaceOpacity];
#endif

	ctrl->PaintGL(CocURect(rect));

	[glContext flushBuffer];

	[NSOpenGLContext clearCurrentContext];
		LLOG("//draw rect");
	
#ifdef flagTEST_UNS
	//	context =  [NSGraphicsContext currentContext];
	if ([[self window] isVisible] && ctrl &&  ([NSGraphicsContext	currentContextDrawingToScreen]) )
//		ctrl->DoPaint((UNSGraphicsContextRef)[NSGraphicsContext currentContext], self, UPP::RectC((int)rect.origin.x,(int)rect.origin.y, (int)rect.size.width, (int)rect.size.height) );
		ctrl->DoPaint1((UNSGraphicsContextRef)[NSGraphicsContext currentContext], UPP::RectC((int)rect.origin.x,(int)rect.origin.y, (int)rect.size.width, (int)rect.size.height) );
#endif
}

- (void)lockFocus  //FIXME when normal context
{
//	NSOpenGLContext* context = glContext;// [self glContext];

	[super lockFocus];

	if ([glContext view] != self)
	{
		[glContext setView:self];
	}

	[glContext makeCurrentContext];
}

- (void)mouseDown:(NSEvent*)event {
//	UPP::GuiLock __;
///	UppCocAutoReleasePool mpool;

//	[[self window] makeFirstResponder:self];
//	[[self window] setAcceptsMouseMovedEvents:YES];
//	NSPoint loc = [self convertPoint:[event locationInWindow]
//	        fromView:[[self window] contentView]];
	NSPoint loc = [self convertPoint:[event locationInWindow]
	        fromView:self];
	
	UPP::Ctrl * q = (UPP::Ctrl *)pCtrl_;
	UPP::Point pos = UPP::Point(loc.x, [self bounds].size.height -loc.y);
//	q->DoMouse(UPP::Ctrl::LEFTDOWN, pos);
	LLOG(" gl nsview left down="<<pos);
//	[self setNeedsDisplayInRect:NSRect?]; //
//	[self setNeedsDisplay:YES]; //tmp!	
}

- (void)mouseMoved:(NSEvent*)event
{
	NSPoint loc = [self convertPoint:[event locationInWindow]
	        fromView:self];

	UPP::Ctrl * q = (UPP::Ctrl *)pCtrl_;
//	UPP::Ctrl * o = (UPP::Ctrl *)pOwner_;
//	q->DoMouse(UPP::Ctrl::MOUSEMOVE, pos);
	UPP::Point pos = UPP::Point(loc.x, [self bounds].size.height-loc.y);
//	q->DoMouse(UPP::Ctrl::MOUSEMOVE, pos);
////	LLOG(" size"<<UPP::Size([self bounds].size.width, [self bounds].size.height));
////	LLOG(" pize"<<q->GetRectInParentWindow());
//
//	if(q)
//		q->MouseMove(pos);
		
//	LLOG("view mousemoved owner="<<UPP::Name(o)<<" this="<<UPP::Name(q)<<" pop:"<<q->IsPopUp());
	LLOG("viewGL mousemoved pos ="<<pos);
}

- (void)mouseDragged:(NSEvent*)event
{
	NSPoint loc = [self convertPoint:[event locationInWindow]
	        fromView:self];

	UPP::Ctrl * q = (UPP::Ctrl *)pCtrl_;
//	UPP::Ctrl * o = (UPP::Ctrl *)pOwner_;
//	q->DoMouse(UPP::Ctrl::MOUSEMOVE, pos);
	UPP::Point pos = UPP::Point(loc.x, [self bounds].size.height-loc.y);
//	q->DoMouse(UPP::Ctrl::MOUSEMOVE, pos);
////	LLOG(" size"<<UPP::Size([self bounds].size.width, [self bounds].size.height));
////	LLOG(" pize"<<q->GetRectInParentWindow());
//
	if(q)
		q->MouseDrag(pos,0);
	LLOG("viewGL mouseDragged pos ="<<pos);
}

-(BOOL)isFlipped
{
	return YES;  //should we add a method to swicth?
}

- (BOOL)acceptsFirstResponder
{
	return YES;
}

- (BOOL)becomeFirstResponder
{
	LLOG("view Become First Responder "<<UPP::uint64(self));
	return YES;
	
}

- (BOOL)resignFirstResponder
{
//	CocAlert("resign","");
	LLOG("view Resigns First Responder");
	return YES;
}

@end