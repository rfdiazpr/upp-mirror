
#ifdef flagTEST_UNS
#include <UNSCtrl/UNSCtrl.h>
//#include <GL/glu.h>
//#include <GL/glut.h>

NAMESPACE_UPP

#define LLOG(x) LOG(x)

//for external tests - not used with upp CtrlCore or needs tweaking
void Ctrl::CreateWnd(UPP::Rect& r)
{
	LLOG("creating UNSWindow");
//	Rect r = GetDefaultWindowRect();
	LLOG("Rect = "<<r);
	rect = Rect(20,20, 400, 400);
	NSRect topframe = NSMakeRect(r.left, r.top, r.Width()+400, r.Height()+400);
	
	NSUInteger styleMask =  NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask; //  NSBorderlessWindowMask;
	
	NSRect windowRect = [NSWindow contentRectForFrameRect:topframe styleMask:styleMask];
	
	//	self = [super initWithContentRect:[mainScreen frame]
//	nswnd  =  [[NSWindow alloc]
//							 //	UppNSWindow  window =  [UppNSWindow
//							 initWithContentRect:windowRect
//							 styleMask:styleMask
//							 backing:NSBackingStoreBuffered
//							 defer:NO
//							screen:nil];
//	NSLog(@"mainScreen frame 3");
		nswnd = [[UNSWindow alloc] initWithCtrl:(CtrlRef*)this owner:(CtrlRef*)NULL style:styleMask];

	[nswnd makeKeyAndOrderFront: NSApp];
	[nswnd makeFirstResponder: nswnd];	

		
	
//	[nswnd setTitle:UNSString("Test mac Ctrl")];
	UNSViewGL * view = [[UNSViewGL alloc] initWithFrame:topframe];
//	UNSView * view = [[UNSView alloc] initWithFrame:topframe];
//	NSView * view = [[NSView alloc] initWithFrame:topframe];
	
	[nswnd setContentView:view];
	
	[nswnd setAcceptsMouseMovedEvents:YES];
	[nswnd setDelegate:view];
//	[view prepareOpenGL];

//	[view awakeFromNib];
//	[window setOwnerCtrl:(Ctrl*)this];
//	[view setImage];
//	[view setOwnerCtrl:this]; 
//	CocAlert("Window::CreateCtrl","info");
//	imageBitmapRef = CreateNSBitmapImageRep(UPP::Size(100,100));
}

void Ctrl::OpenGL()
{
	NSUInteger styleMask =  NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask; //  NSBorderlessWindowMask;
//	NSUInteger styleMask = NSBorderlessWindowMask;

	Rect	r = Rect(20,20, 700, 400);
	NSRect topframe = NSMakeRect(r.left, r.top, r.Width(), r.Height());
	
	NSRect windowRect = [NSWindow contentRectForFrameRect:topframe styleMask:styleMask];
	
	//	self = [super initWithContentRect:[mainScreen frame]
	nswnd  =  [[NSWindow alloc]
							 //	UppNSWindow  window =  [UppNSWindow
							 initWithContentRect:windowRect
							 styleMask:styleMask
							 backing:NSBackingStoreBuffered
							 defer:NO
							screen:nil];
//	NSLog(@"mainScreen frame 3");
//		nswnd = [[UNSWindow alloc] initWithCtrl:(CtrlRef*)this owner:(CtrlRef*)NULL style:styleMask];

//	[nswnd makeFirstResponder: nswnd];
	[nswnd setOpaque:NO];
	[nswnd setBackgroundColor:[NSColor colorWithDeviceRed:0 green:0 blue:0 alpha:0.7]]; 
	   

	NSRect rect;
	rect.origin.x = 0;
	rect.origin.y = 0;
	rect.size.width = 600;
	rect.size.height = 400;


	UNSViewGL* glview = [[UNSViewGL alloc] initWithFrameAndCtrlGL:rect ctrl:(CtrlRef*)this pixelFormat:NULL];

	if (glview == nil)
		LLOG("fail to create opengl view");

	[nswnd setContentView:glview];

	[nswnd makeKeyAndOrderFront: NSApp];

}


void Ctrl::DoPaint(UNSGraphicsContextRef context, UNSViewRef view, Rect rect)
{
	//test
	UDrawRect4(10,10, 50,50, 5, Red);  //tmp not work 
	
}
void Ctrl::DoPaint1(UNSGraphicsContextRef context, Rect rect)
{
	LLOG("normal paint");
	rect.left = 100;
//	UFillRect(rect, Green);
//	Font fnt = StdFont();
//	UDrawRect4(10,10, 50,50, 5, White);
//	CocDrawText(20,20, "TEST", fnt, Green);
	
}

//Size Ctrl::GetSize()
//{
//	return glrect.GetSize();
//}

END_UPP_NAMESPACE

#endif


