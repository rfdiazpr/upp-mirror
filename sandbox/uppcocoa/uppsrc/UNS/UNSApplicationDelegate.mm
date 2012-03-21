//#include "CtrlCoreCoc.h"
#include "UNS.h"

#ifdef PLATFORM_COCOA

//using UPP::Ctrl;
#define LLOG(x) //LOG(x)

@implementation UNSApplicationDelegate

- (void) applicationWillFinishLaunching:(NSNotification *)aNotification
{
}

- (void) applicationDidFinishLaunching:(NSNotification *)aNotification
{
}

//-(void) applicationWillTerminate:(NSNotification *)aNotification
//{
//	NSLog(@"NSApplicationWillTerminate from Delegate");
//	Ctrl::CocoaExit();
//}

-(BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return NO;
}

- (void) setWindowRef:(void*) _windowref
{
	windowref_ = _windowref;
}

@end

#endif
