
#include "UNSView.h"

#ifdef SKELETON_COCOA

#ifndef flagTEST_UNS
#include <CtrlCore/CtrlCore.h>
#endif


#define LLOG(x)  LOG(x)
#define LTIMING(x)  TIMING(x)

@implementation UNSView

//for subviews?
- (id)initWithFrameAndCtrl:(NSRect)frameRect ctrl:(CtrlRef *)ctrl
{
	pCtrl_ = ctrl;
	LLOG("nsview initWithFrame frameRect"<<CocURect(frameRect));  //FIXME for some reasons init comes 2 times
//	NSLog(@"nsview = %@",NSStringFromRect(frameRect));

	self = [super initWithFrame:frameRect];
	if (self)
	{
//		[[self window] setInitialFirstResponder:self];
//		UPP::Ctrl * q = (UPP::Ctrl *)pCtrl_;
	//	UNSWindow * window = [self window];
//	NSLog(@"nsview window= %@",NSStringFromRect( [[self window] frame] ));
		
//		LLOG("view owner ="<<UPP::Name(q));
		
//		if(q->GetOwner())
//		{
//			CocAlert("view owner");
////		//	UPP::Ctrl * owner = (UPP::Ctrl *)window.pOwner_;
//		}
	//	[[self window] makeFirstResponder:self];
	//	[[self window] setAcceptsMouseMovedEvents:YES];
#ifdef __APPLE__	
	NSTrackingAreaOptions focusTrackingAreaOptions = NSTrackingActiveInActiveApp;
    focusTrackingAreaOptions |= NSTrackingMouseEnteredAndExited;
    focusTrackingAreaOptions |= NSTrackingAssumeInside;
	 focusTrackingAreaOptions |=NSTrackingMouseMoved;
    focusTrackingAreaOptions |= NSTrackingInVisibleRect;

    NSTrackingArea *focusTrackingArea = [[NSTrackingArea alloc] initWithRect:NSZeroRect
            options:focusTrackingAreaOptions owner:self userInfo:nil];
    [self addTrackingArea:focusTrackingArea];
#endif    	
//	[self becomeFirstResponder];
	
//	viewTimer	= [NSTimer	scheduledTimerWithTimeInterval:0.05 
//												  target:self 
//												selector:@selector(refresh) 
//												userInfo:NULL 
//												 repeats:YES];
	}
	else
		CocAlert("View not created!"); //CHECK how to do init properly. (id)init should be forwarded into this
	
	return self;
}

- (id)initWithFrame:(NSRect)frameRect
{
	self = [super initWithFrame:frameRect];
	if (self)
		return self;
}

- (void)viewWillDraw
{
#ifdef __APPLE__
	[super viewWillDraw];
#endif	
//	context =  [NSGraphicsContext currentContext];
	NSRect rect = [self frame];
	
		UPP::Ctrl * ctrl = (UPP::Ctrl *) pCtrl_;
	if(!ctrl)
		CocAlert("NO UPP::ctrl!");
//		NSEvent* event = [NSEvent otherEventWithType: NSApplicationDefined
//										location: NSMakePoint(0,0)
//								modifierFlags: 0
//									timestamp: 0.0
//									windowNumber: 0
//										context: nil
//										subtype: 1 //we should define our own constants for this
//										data1: 0
//										data2: 0];
////	[NSApp postEvent: event atStart: YES];
//	LLOG("viewWillDraw");
	
		if ([[self window] isVisible] && ctrl &&  ([NSGraphicsContext	currentContextDrawingToScreen]) )
	{
	//	UPP::SystemDraw draw(context, self, UPP::Rect(0,0, (int)rect.size.width, (int)rect.size.height));
	//	ctrl->UpdateArea0(draw, UPP::Rect(0,0, (int)rect.size.width, (int)rect.size.height), false );
	//	ctrl->UpdateArea(draw, UPP::Rect(0,0, (int)rect.size.width, (int)rect.size.height));  //FIXME requireS screen rect!
//		pCtrl_->DoPaint(context, self, UPP::Rect(0,0, (int)rect.size.width, (int)rect.size.height) );
//		LLOG("UNSView drawRect "<<UPP::Name(ctrl));
		LTIMING("UNSDRAW");
//		if (!dpaint)
//		ctrl->DoPaint(context, self, UPP::RectC(0,0, (int)rect.size.width, (int)rect.size.height) );
//		else
//		{
//			UFillNSRect(rect, UPP::Red);
//			dpaint=NO;
//		}
		//LLOG("//UNSView drawRect "<<UPP::Name(ctrl));
//		NSLog(@"nsview = %@",NSStringFromRect(rect));
	}
//	[NSApp sendEvent:event];

}

-(void) viewDidMoveToWindow
{
//	CocAlert("view did move","");
//	NSTrackingArea *trackingArea = [[NSTrackingArea alloc] initWithRect:[self frame]
//	    options:NSTrackingMouseMoved+NSTrackingActiveInKeyWindow+NSTrackingMouseEnteredAndExited+NSTrackingInVisibleRect
//	    owner:self
//	    userInfo:nil];
//	[self addTrackingArea:trackingArea];
//	[self becomeFirstResponder];
}

-(BOOL) isOpaque
{
	return YES;
}

- (BOOL)wantsDefaultClipping 
{
	return YES;
}

-(void)refresh
{
	[self setNeedsDisplay:YES];
//	[self setNeedsDisplayInRect:NSMakeRect(0,0, 150,150) ];
}

-(void)dbpaint:(BOOL)test
{
//	dpaint=!dpaint;
	dpaint=test;
}

- (void)setOwnerCtrl:(CtrlRef *)owner
{
	pOwner_ = owner; //FIXME add stuff like rect change?..
	UPP::Ctrl * o = (UPP::Ctrl *)pOwner_; // CtrlFromWindowRef(owner);
//	LLOG("view setOwnerCtrl ="<<UPP::Name(o) );
	UPP::Ctrl * ctrl = (UPP::Ctrl *) pCtrl_;
//	if(!ctrl->IsPopUp())
	{
		[[self window] makeFirstResponder:self];
//		[[self window] setAcceptsMouseMovedEvents:YES];
	}
}

- (void) setFrameSize:(NSSize)newSize

{
	[super setFrameSize:newSize];

//	if ([self inLiveResize])
//	{
//		NSRect rects[4];
//		int count;
//
//		[self getRectsExposedDuringLiveResize:rects count:&count];
//
//		while (count-- > 0)
//		{
//			[self setNeedsDisplayInRect:rects[count]];
//		}
//	}
//	else
	{
//		[self setNeedsDisplay:YES];
	}
}

//maybe its possible to recieve paint events in NSWindow and avoid using NVView is not convenient
//because then you need to calculate window bar etc...
- (void)drawRect:(NSRect)rect
{
//	UppCocAutoReleasePool pool; //do wee need this here?

//	CocGraphicsContextRef context =  [NSGraphicsContext currentContext];
	context =  [NSGraphicsContext currentContext];
	
	LLOG("context="<<(UPP::uint64)context);
	
//	[NSGraphicsContext setCurrentContext:self]; ?
//	UPP::SystemDraw draw(context, self, UPP::Rect(0,0, (int)rect.size.width, (int)rect.size.height));
//	LLOG("draw rect width="<<(int)rect.size.width << " height=" <<(int)rect.size.height);
	UPP::Ctrl * ctrl = (UPP::Ctrl *) pCtrl_;
	if(!ctrl)
		CocAlert("NO UPP::ctrl!");
	
	if ([[self window] isVisible] && ctrl &&  ([NSGraphicsContext	currentContextDrawingToScreen]) )
	{
	//	UPP::SystemDraw draw(context, self, UPP::Rect(0,0, (int)rect.size.width, (int)rect.size.height));
	//	ctrl->UpdateArea0(draw, UPP::Rect(0,0, (int)rect.size.width, (int)rect.size.height), false );
	//	ctrl->UpdateArea(draw, UPP::Rect(0,0, (int)rect.size.width, (int)rect.size.height));  //FIXME requireS screen rect!
//		pCtrl_->DoPaint(context, self, UPP::Rect(0,0, (int)rect.size.width, (int)rect.size.height) );
//		LLOG("UNSView drawRect "<<UPP::Name(ctrl));
//		LTIMING("UNSDRAW");
//		if (!dpaint)
		ctrl->DoPaint(context, self, UPP::RectC((int)rect.origin.x,(int)rect.origin.y, (int)rect.size.width, (int)rect.size.height) );
//		else
//		{
//			UFillNSRect(rect, UPP::Red);
//			dpaint=NO;
//		}
		//LLOG("//UNSView drawRect "<<UPP::Name(ctrl));
//		NSLog(@"nsview = %@",NSStringFromRect(rect));
	}
//	LLOG("//context="<<(UPP::uint64)context<<"\n");
//	CocFillRect4(0,0,20,20,UPP::Red);
//	else
//		CocAlert("NO UPP::ctrl or not to screen!!!!");
//	[super drawRect:rect];
//	[self setNeedsDisplayInRect:NSRect?]; //use this to update a portion of  the view
}

- (CtrlRef *) uppCtrl
{
	return pCtrl_;
}

- (void)rightMouseDown:(NSEvent*)event {
//	UppCocAutoReleasePool mpool;

	//cocoa requires to forward right mouse button events?
//	NSResponder* next = [self nextResponder];
//	if (next != nil) {
//		[next rightMouseDown:event];
//	}
	NSPoint loc = [self convertPoint:[event locationInWindow]
	        fromView:self];
	LLOG("mouse right down="<<loc.x);
	UPP::Ctrl * q = (UPP::Ctrl *) pCtrl_;

	UPP::Point pos = UPP::Point(loc.x, [self bounds].size.height-loc.y);
//	q->DoMouse(UPP::Ctrl::RIGHTDOWN, pos);
	
//	CocFillRect4(0,0,20,20, UPP::Yellow);
	[self setNeedsDisplay:YES];  //tmp dbg
//	CocAlert("right click");
//	if(q)
//	    q->WhenRightDown();	
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
//	LLOG("nsview left down="<<pos);
//	[self setNeedsDisplayInRect:NSRect?]; //
//	[self setNeedsDisplay:YES]; //tmp!	
}

//- (void)mouseUp:(NSEvent*)theEvent {
//	UppCocAutoReleasePool mpool;
//
////	[[self window] makeFirstResponder:self];
////	[[self window] setAcceptsMouseMovedEvents:YES];
//	NSPoint loc = [self convertPoint:[theEvent locationInWindow]
//	        fromView:self];
//	
//	UPP::Ctrl * q = (UPP::Ctrl *)pCtrl_;
//	UPP::Ctrl * o = (UPP::Ctrl *)pOwner_;	
//	UPP::Point pos = UPP::Point(loc.x, [self bounds].size.height - loc.y);
//	if([theEvent clickCount] == 1) {
//		LLOG("view leftup click=1 pos="<<pos<<" owner="<<UPP::Name(o)<<" this="<<UPP::Name(q)<<" pop:"<<q->IsPopUp());
//		q->DoMouse(UPP::Ctrl::ACTION, pos);		
////        [model performSelector:@selector(toggleSelectedState) afterDelay:[NSEvent doubleClickInterval]];
//    }
//    else if([theEvent clickCount] == 2)
//    {
//		q->DoMouse(UPP::Ctrl::LEFTDOUBLE, pos);	        
//    }
//    else
//    {
//        LLOG("mouse left up="<<pos);
//        q->DoMouse(UPP::Ctrl::LEFTUP, pos);
//    }
//	[self setNeedsDisplay:YES]; //FIXME we need to refresh affected regions
//}

//mouseMoved is a not effective approach according to some and works under certain conditions
//should we use timer and get mouse pos? or tracking areas? but cocoa tracking areas are slow, too
- (void)mouseMoved:(NSEvent*)event
{
//	UPP::GuiLock __;
//	UppCocAutoReleasePool mpool;
//
////	UPP::Sleep(200);
//	NSPoint loc = [self convertPoint:[event locationInWindow]
//	        fromView:self];
//	        //[[self window] contentView]];
////	if(![[self window] isKeyWindow])        
////		[[self window] makeKeyWindow];
	NSPoint loc = [self convertPoint:[event locationInWindow]
	        fromView:self];

	UPP::Ctrl * q = (UPP::Ctrl *)pCtrl_;
	UPP::Ctrl * o = (UPP::Ctrl *)pOwner_;
//	
//	if(![[self window] isKeyWindow])        
//		[[self window] makeKeyWindow];
//	UPP::Point pos = UPP::Point(loc.x, [self bounds].size.height-loc.y);
//	q->DoMouse(UPP::Ctrl::MOUSEMOVE, pos);
	UPP::Point pos = UPP::Point(loc.x, [self bounds].size.height-loc.y);
//	q->DoMouse(UPP::Ctrl::MOUSEMOVE, pos);
////	LLOG(" size"<<UPP::Size([self bounds].size.width, [self bounds].size.height));
////	LLOG(" pize"<<q->GetRectInParentWindow());
//
//	UPP::Ctrl * q = (UPP::Ctrl *)pCtrl_;
	if(q->IsPopUp())
	{
//		CocFillRect4(0,0,10,10, UPP::Red);
//		[self setNeedsDisplay:YES]; //FIXME tmp!
//		LLOG("m moved pop="<<(bool)[[self window] isKeyWindow]);
	}
//	LLOG("view mouse moved="<<pos<<" owner="<<UPP::Name(o)<<" this="<<UPP::Name(q)<<" pop:"<<q->IsPopUp());
//	LLOG("view mousemoved owner="<<UPP::Name(o)<<" this="<<UPP::Name(q)<<" pop:"<<q->IsPopUp());
}

- (void)mouseDragged:(NSEvent *)event
{
//	UppCocAutoReleasePool mpool;

		NSPoint loc = [self convertPoint:[event locationInWindow]
	        fromView:self];
	        //[[self window] contentView]];
	if(![[self window] isKeyWindow])   //FIXME move to Proc.mm or find the other way?      
		[[self window] makeKeyWindow];
		
	UPP::Ctrl * q = (UPP::Ctrl *)pCtrl_;
	UPP::Ctrl * o = (UPP::Ctrl *)pOwner_;
	
	UPP::Point pos = UPP::Point(loc.x, [self bounds].size.height-loc.y);
//	q->DoMouse(UPP::Ctrl::LEFTDRAG, pos);
//	LLOG("drag");
//	[self setNeedsDisplay:YES]; //tmp!
}
- (void)mouseEntered:(NSEvent*)event {
	LLOG("mouseentered");
	
//	if(![[self window] isKeyWindow])        
//		[[self window] makeKeyWindow];
	
//	 wasAcceptingMouseEvents = [[self window] acceptsMouseMovedEvents];

//	[[self window] setAcceptsMouseMovedEvents:YES];

//	[[self window] makeFirstResponder:self];
    
	UPP::Ctrl * q = (UPP::Ctrl *)pCtrl_;
//	if(q->IsPopUp())
	{
		//q->SetWndFocus();
//		CocAlert("mouseEntered popup");
//		[[self window] setAcceptsMouseMovedEvents:YES];
	}
		
	   
//		[self setNeedsDisplay:YES];
}

- (void)mouseExited:(NSEvent*)event {
	UPP::Ctrl * q = (UPP::Ctrl *)pCtrl_;
//	   [[self window] setAcceptsMouseMovedEvents:wasAcceptingMouseEvents];
//	if(q->IsPopUp())
	{
//		[[self window] setAcceptsMouseMovedEvents:NO];
	}
}
//navigation keys...
- (BOOL)performKeyEquivalent:(NSEvent *)event
{
	unsigned short key = [event keyCode];
	UPP::Ctrl * q = (UPP::Ctrl *)pCtrl_;
	UPP::WString *characters;
	characters = (UPP::WString *)[event characters];
	unichar character;
//	UPP::wchar ch = (UPP::wchar)[characters characterAtIndex: 0];
	
//	LLOG("nsView performKeyEquivalent ="<<key<<" "<<q->Name());
//	LLOG("nsView performKeyEquivalent ="<<key<<" desc="<<UPP::GetKeyDesc1(key));
//	LLOG("nsView performKeyEquivalent "<<" 0x" << UPP::Sprintf("%08x", ch) <<" desc="<<UPP::GetKeyDesc1(ch));

//	UPP::Ptr<UPP::Ctrl> q = (UPP::Ctrl *)pCtrl_;
//	UPP::Ctrl::DoKeyboard(key,1);
//	q->Key(key);
//	UFillNSRect([self bounds], UPP::Red);
//	[self setNeedsDisplay:YES];
//	case 
//	q->Key(q->unicode ? keycode : k, count);
	return YES;
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

//- (void) keyDown: (NSEvent *) event
//{
//	LLOG("key down");
//}

//- (void)getImageRep
//{
//	
//}

//-(void)dealloc
//{
//	[super dealloc];
//}


@end

//SystemDraw1::SystemDraw1(CocGraphicsContextRef _gc, UNSViewRef _dw)
//{
//	gc = _gc;
//	dw = _dw;
//}

#endif
