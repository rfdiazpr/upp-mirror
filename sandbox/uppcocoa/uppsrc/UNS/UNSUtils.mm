#include "UNS.h"

#ifdef PLATFORM_COCOA
//#include <Foundation/Foundation.h>

#define LLOG(x) LOG(x)
#define LTIMING(x)   //TIMING(x)

#ifdef SKELETON_COCOA

#ifndef flagTEST_UNS
#include <CtrlCore/CtrlCore.h>
#endif

struct CtrlRef {
	UPP::Ctrl * ctrl;
};

//struct CtrlRef {
//	UPP::CocWnd * ctrl;
//};



UPP::Point CocViewPointW(NSEvent *e)
{
	LTIMING("CocViewPoint");
	NSWindow *w = [e window];
	NSView *v = [w contentView];

	NSPoint loc = [v convertPoint:[e locationInWindow] fromView:v];

//	int th = [(UNSWindow *)w titlebarHeight];
	return UPP::Point(loc.x, [v bounds].size.height -loc.y);
//	return UPP::Point(loc.x+[w frame].origin.x, [v bounds].size.height -loc.y -[w frame].origin.y);
}

UPP::Point CocViewPointV(NSEvent *e)
{
	LTIMING("CocViewPoint");
	NSWindow *w = [e window];
	NSView *v = [w contentView];
//	NSPoint loc = [self convertPoint:[event locationInWindow]
//	        fromView:self];
	NSPoint loc = [v convertPoint:[e locationInWindow]
	        fromView:v];
	        
//	NSPoint event_location = [e locationInWindow];
//	NSPoint local_point = [v convertPoint:event_location fromView:nil];
//	CocURect([view convertRect:[view frame] fromView:nil]);

//	return UPP::Point(loc.x, [v bounds].size.height -loc.y);
//	UPP::Point p = UPP::Point(loc.x, [v bounds].size.height -loc.y);
	
	NSPoint np = [NSEvent mouseLocation];

//	return p;
	int th = [(UNSWindow *)w titlebarHeight];
	return UPP::Point(loc.x, [v bounds].size.height -loc.y + th);
//	return UPP::Point(loc.x+[w frame].origin.x, [v bounds].size.height -loc.y -[w frame].origin.y);
}

#if 0
int RunCocWindowModal(UNSWindowRef winref)  //not used anymore in upp-cocoa
{
	[winref setModal:true];
	[winref setReleasedWhenClosed:YES];
//	UppCocAutoReleasePool mpool;
 //    [winref beginSheetModalForWindow:winref modalDelegate:winref];

		// wait for the sheet
	int result = NSRunContinuesResponse;
	NSModalSession session = [NSApp beginModalSessionForWindow:winref];
	while (result == NSRunContinuesResponse &&  [winref isVisible])
	{
		// alertReturnStatus will be set in alertDidEndSheet:returnCode:contextInfo:
//            if(alertReturnStatus != -1)
//               break;
// 		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

    // Run the window modally until there are no events to process:
	    result = [NSApp runModalSession:session];
		// Execute code on DefaultRunLoop
//		[[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode 
//									beforeDate:[NSDate distantFuture]];

	   [[NSRunLoop currentRunLoop] limitDateForMode:NSDefaultRunLoopMode];
			// Break the run loop if sheet was closed
//		if ([NSApp  ![winref isVisible]) 
//		{
//			LLOG("modal 1");
//
//			break;
//		}
		
//		[pool drain];
			// Execute code on DefaultRunLoop
//		[[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode 
//									beforeDate:[NSDate distantFuture]];

	}
//	LLOG("modal 1");
//	[winref setModal:false];
//	LLOG("modal 2");
//	[winref close];
	LLOG("modal 2 result="<<result);

//	[NSApp endModalSession:session];

//	LLOG("modal 3");
//		[[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode 
//									beforeDate:[NSDate distantFuture]];
									
//	LLOG("modal 21 result="<<result);


	int exitcode;// = (NSInteger)[NSApp runModalForWindow:winref];
//	CocAlert("finished modal!");
	exitcode = UPP::IDOK;  //FIXME!!!
	return exitcode;
}
#endif

#endif

NSString * UNSString(const char *text)
{
	return [NSString stringWithUTF8String:text];
}

const char * CocUString(NSString * ns)
{
	return [ns UTF8String];
}


CocAlert::CocAlert(const UPP::String& msg, const char * info)
{
	//this is prepared for UPP::Core, for panic message and debug alerts 
	//also this is better to be compiled in a separate file from other cocoa stuff 
	//otherwise sometimes it makes havoc among NSStrings in that file on Apple
	//the reason migth be threads or missing translation files or null ending(most likely?) ...

	NSString *m1 = UNSString((const char *) msg);
	NSString *b1 = UNSString("OK");
	NSString *i1 = UNSString(info);
	
	NSRunInformationalAlertPanel(i1, m1, b1, nil, nil);
}

UPP::Rect CocURect(NSRect r)
{
//	NSRect r = (NSRect)rect;
	UPP::Rect rect = UPP::RectC(r.origin.x, r.origin.y, r.size.width, r.size.height);
	return rect;
}

UPP::Point CocUPoint(NSPoint p)
{
	return UPP::Point(p.x, p.y);
}



UPP::Point CocViewPointS(NSEvent *e)
{
	LTIMING("CocViewPoint");
	NSWindow *w = [e window];
	NSView *v = [w contentView];
//	NSPoint loc = [self convertPoint:[event locationInWindow]
//	        fromView:self];
	NSPoint loc = [v convertPoint:[e locationInWindow]
	        fromView:v];
	        
	NSPoint event_location = [e locationInWindow];
	NSPoint local_point = [v convertPoint:event_location fromView:nil];
//	CocURect([view convertRect:[view frame] fromView:nil]);

//	return UPP::Point(loc.x, [v bounds].size.height -loc.y);
//	UPP::Point p = UPP::Point(loc.x, [v bounds].size.height -loc.y);
	
	NSPoint np = [NSEvent mouseLocation];
//	UPP::Point  p = UPP::Point( (int) np.x, 1050 - (int) np.y - 45);
	UPP::Point  p = UPP::Point( (int) np.x, 1050 - (int) np.y);
//	LLOG(p=)
	return p;
//	int th = [(UNSWindow *)w titlebarHeight];
//	return UPP::Point(loc.x, [v bounds].size.height -loc.y + th);
//	return UPP::Point(loc.x+[w frame].origin.x, [v bounds].size.height -loc.y -[w frame].origin.y);
}



NSColor *UNSColor(UPP::Color c)
{
	return [NSColor colorWithCalibratedRed:(c.GetR()/255.0f) green:(c.GetG()/255.0f) blue:(c.GetB()/255.0f) alpha:1.0];
//	NSColorSpace *sRGB = [NSColorSpace sRGBColorSpace];
//
//	CGFloat components[3] = { 1.0f,1.0f,1.0f };
//	components[0]=(c.GetR()/255.0f);
//	components[1]=(c.GetG()/255.0f);
//	components[2]=(c.GetB()/255.0f);
//	NSColor *sRGBRed = [NSColor colorWithColorSpace:sRGB components:components count:3];
//	return sRGBRed;
//	return [NSColor colorWithColorSpace:sRGB components:components count:3];
//	return [NSColor colorWithSRGBRed:(c.GetR()) green:(c.GetG()/255.0f) blue:(c.GetB()/255.0f) alpha:1.0];
	
//	return [NSColor colorWithDeviceRed:c.GetR() green:c.GetG() blue:c.GetB() alpha:1];
}

NSRect UNSRect(UPP::Rect r)
{
	return NSMakeRect(r.left, r.top, r.Width(), r.Height());
}

//GetCocScreen()
//		NSScreen* mainScreen = [[NSScreen screens] objectAtIndex:0];
//	self = [super initWithContentRect:[mainScreen frame]  //for full screen?

UPP::Rect CocGetScreenRect(int screen)
{
//	CocAlert("CocGetScreenRect","");
	NSScreen * mainScreen = [[NSScreen screens] objectAtIndex:screen];
	UPP::Rect rect = CocURect([[NSScreen mainScreen] frame]);
//	CocAlert(AsString(rect.TopLeft()),"");
	return rect;
}

UPP::Rect CocGetVisibleScreenRect(int screen) //excluding top menu and dock
{
//	CocAlert("CocGetScreenRect","");
//	NSString* frameAsString = NSStringFromRect([[NSScreen mainScreen] visibleFrame]);
	NSScreen * mainScreen = [[NSScreen screens] objectAtIndex:screen];
	UPP::Rect rect = CocURect([[NSScreen mainScreen] visibleFrame]);
//	CocAlert(AsString(rect.TopLeft()),"");
	return rect;
}

UPP::Rect CocGetViewRectInScreen(UNSViewRef view)
{
	CocAlert("CocGetViewRectInScreen","");
	return CocURect([view convertRect:[view frame] fromView:nil]); //FIXME direct func?
}

//used from u++ menus and maybe frames
UPP::Rect CocGetViewRectInScreen1(UNSWindowRef window)
{
//	UPP::GuiLock __;
	LTIMING("CocGetViewRectInScreen1");
//	LLOG("CocGetViewRectInScreen1 0");
	UNSWindow * wnd = (UNSWindow*)window;
//	LLOG("CocGetViewRectInScreen1 0 window="<<(UPP::int64)wnd);
//	[wnd makeKeyAndOrderFront:wnd];

//	if(wnd==nil)
//		CocAlert("f*ck CocGetViewRectInScreen1","");
//	else
//		CocAlert("else good? CocGetViewRectInScreen1","");

//	LLOG("window rect="<<CocURect([wnd frame]) );
//		CocAlert("else good? CocGetViewRectInScreen1","");
		
	UNSView * view = [wnd contentView];
//	LLOG("CocGetViewRectInScreen1 1");
	UPP::Rect r = CocURect([view convertRect:[view frame] fromView:nil]); //FIXME direct func?
//	LLOG("CocGetViewRectInScreen1 r="<<r);
	return r;
}

UPP::Size CocUSizeFromNSRect(NSRect bounds)
{
	return UPP::Size(bounds.size.width, bounds.size.height);
}

void CocMoveResizeWindow(UNSWindowRef window, int x, int y, int cx, int cy)
{
	//TODO
}


NSRect CenterViewRect(UNSViewRef _dw)  //FIXME dbg
{
	NSSize viewSize  = [_dw bounds].size;
	NSSize imageSize = { 250, 250 };
	
	NSPoint viewCenter;
	viewCenter.x = viewSize.width  * 0.50;
	viewCenter.y = viewSize.height * 0.50;
	
	NSPoint imageOrigin = viewCenter;
	imageOrigin.x -= imageSize.width  * 0.50;
	imageOrigin.y -= imageSize.height * 0.50;
	
	NSRect destRect;
	destRect.origin = imageOrigin;
	destRect.size = imageSize;
	return destRect;
}

UPP::String	CocGetAppName()
{
	return CocUString([[NSProcessInfo processInfo] processName]);
}




/*
NSLayoutManager
	height = [laymgr defaultLineHeightForFont:font];
	ascent_ = [font ascender];
	average_width_ = [font boundingRectForGlyph:[font glyphWithName:@"x"]].size.width;
*/
     


#endif
