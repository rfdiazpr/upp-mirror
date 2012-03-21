//using UPP::Ctrl;
//using namespace Upp;

#include "UNSWindow.h"
//#include <GL/glu.h>

#ifndef flagTEST_UNS
#include <CtrlCore/CtrlCore.h>
#endif

#define LLOG(x) LOG(x)

#ifdef SKELETON_COCOA


@implementation UNSWindow
// + (id) initialize  //FIXME redirect init
- (id)initWithCtrl:(CtrlRef *)ctrl owner:(CtrlRef *)owner style:(NSUInteger)style
{
	modal_ = NO;
	pCtrl_ = ctrl;
	pOwner_ = owner;

	UPP::Ctrl * q = (UPP::Ctrl *)ctrl;
	UPP::Rect r = q->GetRect();
	
	
	NSRect contentRect;
//	NSRect topframe;
//NSTexturedBackgroundWindowMask for larger toolbar areas	
	NSRect screenFrame = [[[NSScreen screens] objectAtIndex: 0] visibleFrame]; //FIXME form multi and btw, some say this slow func

	
	int y = screenFrame.size.height ;
	int y1 = y - r.bottom;
//	LLOG("UNSWindow initWithFrame q="<<UPP::Name(q)<<" rect="<<r<<" y1="<<y1 );
	LLOG("\nUNSwindow rect="<<r<<" y1="<<y1 );

	
	if(!q->IsPopUp())  //for popup :FIXME - need different conditions?
	{
//		UPP::Ctrl *o = (UPP::Ctrl *)owner;
//		if(o)
//			CocAlert("not popup but yes owner");	
//		NSRect topframe = NSMakeRect(r.left, y-r.bottom, r.Width(), r.Height());	
		contentRect = NSMakeRect(r.left, y1-22, r.Width(), r.Height());
		// [NSWindow contentRectForFrameRect:topframe styleMask:style];
//		CocAlert("no owner");
//	LLOG("UNSwindow initWithFrame2 contentRect"<<CocURect(contentRect) << " q"<<UPP::Name(q));
//	NSLog(@"contentRect = %@",NSStringFromRect(contentRect));
//	LLOG("UNSwindow contentRect"<<CocURect(contentRect) );
	}
	else
	{
//		LLOG("pop window initWithFrame2 frameRect"<<CocURect(contentRect) << " q"<<UPP::Name(q));
		
		UPP::Ctrl *o = (UPP::Ctrl *)owner;
		if (o)
		{
			id owin = o->GetUNSWindowRef();  //maybe we could store the owner a different way?
			const UPP::Ctrl * topctrl = o->GetTopCtrl();
			UPP::Rect tr = CocGetViewRectInScreen1(o->GetUNSWindowRef());
			
			NSRect rv =  [[ owin contentView] frame]; //.origin.y;
		//	UFillNSRect(rv, UPP::Red);
//			if(o->IsMenuBar())
//				CocAlert("menubar","");
			NSRect rw =  [ owin frame]; //.origin.y;
			UPP::Rect r0 = o->GetRect();
//			UPP::Rect rp = o->CocGetScreenRect();
			
			LLOG("NSWindow POPUP owner initWithFrame2 r0="<<r0); //<< " q"<<UPP::Name(q));

			int dy = rw.size.height - rv.size.height;  //the difference should the titlebar/cocoa toolbar height
//			CocAlert(UPP::AsString(r0));
//			CocAlert(UPP::AsString(dy));
			
		//	UPP::Rect r2 = UPP::RectC(r0.left +r.left ,  y-r0.top-r.bottom-dy , r.Width(), r.Height());
//		UPP::Rect r2 = UPP::RectC(r0.left +r.left,  y-rv.origin.y -r.bottom , r.Width(), r.Height());
			//contentRect = UNSRect(r2);
		//	contentRect = NSMakeRect(110, y-r0.top-r.bottom-dy, r.Width(), r.Height());
//			contentRect = NSMakeRect(r0.left +r.left, y-r0.top-r.bottom-dy, r.Width(), r.Height());
			contentRect = NSMakeRect(r.left +tr.left, y1-dy, r.Width(), r.Height());//
		//	CocAlert(UPP::AsString(y1));  //837 882


		}
		else
		{
		//	LLOG("NSWindow splash dy="<<dy<<" r2="<< r2 <<" r0="<< r0<<" q="<<UPP::Name(o));
			contentRect = NSMakeRect(r.left, y1, r.Width(), r.Height());//
			//CocAlert("no owner!");

		}
		
	}

		
//	LLOG("window initWithFrame2 frameRect"<<CocURect(contentRect) << " q"<<UPP::Name(q));
//	UPP::Ctrl * ctrl = (UPP::Ctrl *) pCtrl_;  //is this better or reinterpret_cast
//	UPP::Rect r = ctrl->GetRect();

	self = [super initWithContentRect:contentRect styleMask:style backing:NSBackingStoreBuffered defer:NO];

//	else
		nsview = [[UNSView alloc] initWithFrameAndCtrl:contentRect ctrl:(CtrlRef*)ctrl];

//	LLOG("window initWithFrame3 frameRect"<<CocURect(contentRect) << " q"<<UPP::Name(q));
		
	if(owner)
	{
		[nsview setOwnerCtrl:(CtrlRef*)owner];
		UPP::Ctrl * o = (UPP::Ctrl *)pOwner_; // CtrlFromWindowRef(owner);
//		LLOG("window setOwnerCtrl ="<<UPP::Name(o) );
		//CocAlert("if owner");
	}    
	
//		[newMenu addItem:[NSMenuItem separatorItem]];
//		[newMenu addItem:[NSMenuItem separatorItem]];	
//	[nsview setMenu:newMenu];
	
	if(self)
	{
		[self setContentView:nsview];
		[self setDelegate:self];
		isAcceptsMouseMoved = YES;
		th = [self frame].size.height - [nsview frame].size.height;
	}
	else
		CocAlert("can't create UNSWindow!");

//	LLOG("//window initWithFrame frameRect"<<CocURect(contentRect) << " q"<<UPP::Name(q));
	LLOG("//window initWithFrame frameRect"<<CocURect(contentRect));
	return self;	
	//addChildWindow:(NSWindow *)childWindow ordered:(NSWindowOrderingMode)orderingMode
}


-(void)windowDidMove:(NSNotification *)notification
{
	UPP::Ctrl * q = (UPP::Ctrl *)pCtrl_;
//	LLOG("windowDidMove r="<<q->GetRect());
//	CocAlert( UPP::AsString( q->GetRect() ), "1" );
	NSRect frame = [self frame];
//	UPP::Rect r= CocURect(frame);
	NSRect screenFrame = [[[NSScreen screens] objectAtIndex: 0] visibleFrame]; //FIXME form multi and btw, some say this slow func
//	NSRect screenFrameVis = [[[NSScreen screens] objectAtIndex: 0] frame]; //FIXME form multi and btw, some say this slow func

//	int th = [self titlebarHeight]; //22 
	int mh = [[NSApp mainMenu] menuBarHeight];
//	UPP::String ss = UPP::AsString(dy);
//	CocAlert(ss,"");
	NSRect viewFrame = [[self contentView] frame];

	int y = screenFrame.size.height - (frame.origin.y + frame.size.height) ;

//	UPP::Rect r1 = UPP::RectC(frame.origin.x, y+th+mh, viewFrame.size.width , viewFrame.size.height);
	UPP::Rect r1 = UPP::RectC(frame.origin.x, y, viewFrame.size.width , viewFrame.size.height);
	
//	LLOG("windowDidMove 1 r1="<<r1);
	q->AdjustRectFromCocWnd(r1);  //FIXME SetWndRect? SetPos0?
//	CocAlert( UPP::AsString( q->GetRect() ), "2" );
//	[[self contentView] refresh]; //I am not sure we need this...
//	q->SetPos0();
//	q->WndSetPos0(r1);
	LLOG("windowDidMove 2 r1="<<q->GetRect());
}

- (CGFloat)titlebarHeight
{
//		UPP::String ss = UPP::AsString([self frame].origin.y) + " " + UPP::AsString([[self contentView] frame].origin.y);
//	CocAlert(ss,"");
//	return 1050 -[self frame].origin.y - [[self contentView] frame].origin.y;
	return th;
//	return [self frame].size.height - [[self contentView] frame].size.height;
}

- (BOOL)acceptsMouseMovedEvents
{
	return isAcceptsMouseMoved;
		
}

- (void) setAcceptsMouseMovedEvents: (BOOL)flag
{
	
	isAcceptsMouseMoved = flag;
}

-(void)windowDidResize:(NSNotification *)notification
{
	UPP::Ctrl * q = (UPP::Ctrl *)pCtrl_;
//	LLOG("windowDidMove r="<<q->GetRect());
	NSRect frame = [self frame];
//	UPP::Rect r= CocURect(frame);
	NSRect screenFrame = [[[NSScreen screens] objectAtIndex: 0] visibleFrame]; //FIXME form multi and btw, some say this slow func

	int y = screenFrame.size.height - (frame.origin.y + frame.size.height) ;

	CGFloat newViewHeight = [self frame].size.height - th;

	
	NSSize newSize = {[self frame].size.width, newViewHeight};
	[[self contentView] setFrameSize:newSize];
	
	th = [self frame].size.height - [[self contentView] frame].size.height;
	
//	UPP::Rect r = CocURect([[self contentView] convertRect:[[self contentView] frame] fromView:nil]); //FIXME direct func?

//	UPP::Rect r2 = CocURect([[self contentView] frame] );
		
	UPP::Rect r = UPP::RectC(frame.origin.x, y, frame.size.width , newViewHeight);

//	LLOG("windowDidMove 1 r1="<<r1);
	q->SetRect(r);
//	q->Size([self frame].size.width, newViewHeight );
//	q->AdjustRectFromCocWnd(r1);  //FIXME SetWndRect? SetPos0?
//	[[self contentView] refresh];

	LLOG("windowDidMove 2 r1="<<q->GetRect());
//	CocAlert(UPP::AsString(r));
}

- (CtrlRef *)owner
{
	return pOwner_;	
}

- (BOOL)windowShouldClose:(id)sender
{
	if (modal_)
	{
		//[NSApp abortModal];
//		UPP::Ctrl * ctrl = (UPP::Ctrl *)pCtrl_;
//		UPP::Ctrl * top = ctrl->GetTopCtrl();
		
//		UNSWindowRef ownd = top->owner->GetUNSWindowRef();
		
//		if(ownd)
		{
			LLOG("windowWillClose 1");
//			[self resignKeyWindow];
//			[self resignMainWindow];

//			UPP::Ctrl * ctrl = (UPP::Ctrl *)pCtrl_;
//			UPP::Ctrl * top = ctrl->GetTopCtrl();
//		
//			
//			UPP::Ctrl::Windows().Remove(UPP::Ctrl::Windows().Find(top->hwnd));

//			[NSApp stopModalWithCode:UPP::IDOK];
			LLOG("windowWillClose 2");
//			[self release];

			CocAlert("modal has owner!!");
			
		}
	}
//	[super dealloc];

}


//- (id)initWithOwner(CtrlRef *)ctrl //owner:(CtrlRef *)owner//  style:(NSUInteger)style
//{
//	pCtrl_ = ctrl;
//	UPP::Rect r = (reinterpret_cast<UPP::Ctrl *>(ctrl))->GetRect();
//	NSRect windowRect = NSMakeRect(r.left, r.top, r.Width(), r.Height());	
//	[super initWithContentRect:windowRect styleMask:style backing:NSBackingStoreBuffered defer:NO];
//	
//	if (!self)
//			CocAlert("can't create UNSWindow!");
//	return self;
//}

//- (void)setOwner:(UNSWindowRef *)owner
//{
//	CocAlert("UNSWindow setOwner - Not implemented yet");
//	//pCtrl_ = owner;  //FIXME  set rect and stuff
//}


- (BOOL)acceptsFirstResponder
{
//	LLOG("window Accepts First Responder");
	return YES;
}

- (BOOL)becomeFirstResponder
{
	LLOG("window Become First Responder");
//	CocAlert("Become First Responder", "window");	
	return YES;
	
}
- (BOOL)resignFirstResponder
{
	LLOG("Resigns First Responder");
//	CocAlert("Resigns First Responder", "window");
	return YES;
}

- (BOOL)canBecomeKeyWindow
{
	return YES;  //shall we create a separate variable to switch it?
}
- (BOOL)canBecomeMainWindow
{
	return YES;
}
- (void)makeKeyWindow
{
	[super makeKeyWindow];
}

//-(void)keyDown:(NSEvent *)event //works but do we need it here
//{
////	NSLog(@"keyDown:event = %@", event);
////	[event modifierFlags]; //needs test
//	unsigned short keycode = [event keyCode]; //Carbon codes but keyboard dependant?
//	UPP::WString *characters;
//	characters = (UPP::WString *)[event characters];
//	unichar character;
//	UPP::wchar ch = (UPP::wchar)[characters characterAtIndex: 0];
////	LLOG("ch="<<ch<<" keycode="<<keycode<<" dword="<<sizeof(unichar)<<","<<sizeof(UPP::wchar));
//	LLOG("UNSWindow keyDown "<<" 0x" << UPP::Sprintf("%08x", ch) <<" desc="<<UPP::GetKeyDesc1(ch));
////	UPP::Ctrl::DoKeyboard(ch,1);	
////		[super sendEvent:event];
//}

//- (void)keyUp:(NSEvent*)event {
////	NSLog(@"keyUp = %@", event);
////	LLOG("keyUp");
////		[super sendEvent:event];
////		owner_->DispatchKeyEvent(event);	
//	
//}

- (void)setModal:(BOOL) _modal  { modal_=_modal; }
- (BOOL)isSheet                 { return modal_; }
/*
 - (void)mouseDown:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 [super sendEvent:event];
 }
 
 - (void)mouseDragged:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)mouseEntered:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)mouseExited:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 */
 
//- (void)mouseMoved:(NSEvent*)event {
// //	owner_->DispatchMouseEvent(event);
//// 	NSPoint loc = [self convertPoint:[event locationInWindow]
// //           fromView:[[self window] contentView]];
////	LLOG("window mouse moved=");  //this works but we need from view?
//}
 
 /*
 - (void)mouseUp:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)rightMouseDown:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)rightMouseDragged:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)rightMouseUp:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)otherMouseDown:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)otherMouseDragged:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)otherMouseUp:(NSEvent*)event {
 //	owner_->DispatchMouseEvent(event);
 }
 
 - (void)windowDidResignKey:(NSNotification*)notification {
 //	owner_->DispatchFocusLostEvent();
 }
 
 - (void)windowDidResignMain:(NSNotification*)notification {
 //	owner_->DispatchFocusLostEvent();
 }
 */

- (IBAction)menuItem1Action:(id)sender
{
	CocAlert("menuItem1Action");
	// dismiss the menu
//	NSMenu* menu = [[sender enclosingMenuItem] menu];
//	[menu cancelTracking];
}

@end

UNSWindowRef CreateCocWndFromCtrl(CtrlRef * ctrl, NSUInteger style)
{
	return [[UNSWindow alloc]
	       initWithCtrl:(CtrlRef*)ctrl owner:NULL style:style];
}
#endif

