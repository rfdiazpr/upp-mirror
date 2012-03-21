#ifndef __UNSAPPLICATIONDELEGATE_H__
#define __UNSAPPLICATIONDELEGATE_H__

//#include "CtrlCoreCoc.h"
#include "UNS.h"

#ifdef PLATFORM_COCOA

#if defined(__OBJC__)
#include <Cocoa/Cocoa.h>

#if ((MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_5)) && !defined(COMPILER_MINGW)
@interface UNSApplicationDelegate : NSObject <NSApplicationDelegate> {
#else
@interface UNSApplicationDelegate : NSObject  {
#endif
	NSMenu*				appDockMenu;
	//CtrlRef  * pCtrl_;
	void * windowref_;	//FIXME rename or find a way for the references
//	NSLog(@"UNSApplicationDelegate : NSObject <NSApplicationDelegate>");

}

//- (void) setWindowRef:(UNSWindowRef) _windowref;
- (void) setWindowRef:(void*) _windowref;
@end

#endif

#endif

#endif
