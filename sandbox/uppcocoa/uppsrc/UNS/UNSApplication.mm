#include "UNS.h"
//#include <UNS/UNS.h>
//#include "UNSApplication.h"

#ifdef PLATFORM_COCOA
#define LLOG(x) //LOG(x)
#define LTIMING(x)  RTIMING(x)

@implementation UNSApplication
-(id) init
{
	self = [super init];
//	CocAlert(UPP::String(__PRETTY_FUNCTION__));
//	if (self != nil)
//	{
//		[self setDelegate:[[UNSApplicationDelegate alloc] init]];
//	}

//	[myTextView addCursorRect:[myTextView bounds] cursor:whiteIBeamCursor];
//	[whiteIBeamCursor setOnMouseEntered:YES];

#if 0
	msgTimer	= [NSTimer	scheduledTimerWithTimeInterval: 0.25  //30.0
												target:self 
												selector:@selector(msgTimerEvent)
												userInfo:self
												repeats:YES];
#endif
	return self;
}

- (void)sendEvent:(NSEvent *)event
{
	//yes you can recieve events here!
//	NSEventType eType = [event type];
//    if( eType == NSApplicationDefined )
//        ->handleAppDefinedEvent( pEvent );
//    else if( eType == NSKeyDown && ([pEvent modifierFlags] & NSCommandKeyMask) != 0 )
//    {
	/*
		NSWindow* pKeyWin = [NSApp keyWindow];
		if( pKeyWin && [pKeyWin isKindOfClass: [UNSWindow class]] )
		{
				NSLog(@"UppNSApplication sendEvent:theEvent = %@", event);

		}
	*/
//	Ctrl::ProcessEvent(event);
	
//	NSLog(@"UppNSApplication sendEvent:theEvent = %@", event);
//	switch ([event type]) 
//	{
//		case NSKeyDown:
			//		    if ([event modifierFlags] & NSControlKeyMask) {
//		{
//			NSLog(@"test");
//			NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
//			[nc postNotificationName:@"shortCutKey" object:event];
//		}
//			break;
//		default:
//			break;
//	}

//	if(([event modifierFlags] & NSCommandKeyMask) != 0)
//		NSLog(@"command event %@", event);
//	else
//	if(([event modifierFlags] & NSAlternateKeyMask) != 0)
//		NSLog(@"Alt event %@", event);
	
	NSEventType eType = [event type];	
	if(eType == NSApplicationDefined)
		NSLog(@"event=== %@", event);
//	if(eType == NSFlagsChanged)
//		NSLog(@"event=== %@", event);
	LTIMING("super sendEvent");
	[super sendEvent:event];
	
}

-(void)msgTimerEvent
{
//	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
//	NSEvent* event = [NSEvent otherEventWithType: NSApplicationDefined
//										location: NSMakePoint(0,0)
//								modifierFlags: 0
//									timestamp: 0.0
//									windowNumber: 0
//										context: nil
//										subtype: 1 //we should define our own constants for this
//										data1: 0
//										data2: 0];
//	[NSApp postEvent: event atStart: YES];
//	[pool release];
//	if ()
//	[[[self keyWindow] contentView] dbpaint:YES];
//	[[[self keyWindow] contentView] setNeedsDisplay:YES];
	
}

-(void)sendSuperEvent:(NSEvent*)event
{
	LLOG("UNSApplication sendSuperEvent");
//	NSLog(@"UNSApplication sendSuperEvent = %@", event);
	[super sendEvent: event];
}

//-(void)keyDown:(NSEvent *)event
//{
//	LLOG("NSApplication keyDown:event");	
//	[super sendEvent:event];
//}

//-(void) discardEventsMatchingMask:(unsigned int)mask
//					  beforeEvent:(NSEvent *)lastEvent
//{
//	NSLog(@"discardEventsMatchingMask:event = %@", lastEvent);	
//}

//-(void) applicationWillTerminate:(NSNotification *)aNotification
//{
//	NSLog(@"NSApplicationWillTerminate from application");
//	UPP::Ctrl::CocoaExit();
//}
@end

NAMESPACE_UPP

#ifdef SKELETON_COCOA

//bool Ctrl::m_bIsIdle;
//
//void Ctrl::CocIdle()
//{
//	m_bIsIdle = true;
//	NSEvent * event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantFuture] inMode:NSDefaultRunLoopMode dequeue:NO];
//	m_bIsIdle = false;
//}

#endif



CocApplication::CocApplication()
{
	nsapp = [UNSApplication sharedApplication];
}

CocApplication::~CocApplication()
{
//	[(NSApplication*)application terminate];
//	[nsapp terminate];
}

void CocApplication::Run()
{
	[NSApp activateIgnoringOtherApps:YES];
//	cocoaRunning = true;
//	[UNSApplication run];
//	[(NSApplication*)application run];  //need to test
}


END_UPP_NAMESPACE

/////////=======================/////////////

UppCocAutoReleasePool::UppCocAutoReleasePool()
{
//	pool = (void*)[[NSAutoreleasePool alloc] init];
#ifdef  __OBJC__
	nspool = [[NSAutoreleasePool alloc] init];
#endif
}

UppCocAutoReleasePool::~UppCocAutoReleasePool()
{
#ifdef  __OBJC__
//	[(NSAutoreleasePool*)pool release];
	[nspool release];
#endif
}
//*****************//

#endif
