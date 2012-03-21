#ifndef _UNS_UNSDef_h_
#define _UNS_UNSDef_h_

//#ifdef PLATFORM_COCOA

//#include <Foundation/Foundation.h>

//#include <UNS/UNS.h>

#ifdef  __OBJC__
//#include <Cocoa/Cocoa.h>
//#include <Foundation/Foundation.h>
#include <Cocoa/Cocoa.h>
#include <Foundation/Foundation.h>
//#include <UNS/UNSView.h> 
@class UNSWindow;
@class UNSView;

typedef NSGraphicsContext * CocGraphicsContextRef;
typedef NSBitmapImageRep * CocBitmapImageRepRef;
typedef NSImage * CocImageRef;
typedef NSEvent * CocEventRef;
typedef NSApplication * CocNSAppRef;
typedef NSAutoreleasePool * CocAutorelesePoolRef;
typedef NSWindow * NSWindowRef;
typedef UNSWindow * UNSWindowRef;
typedef UNSView * UNSViewRef;
typedef NSGraphicsContext * UNSGraphicsContextRef;

#ifdef __APPLE__
typedef AXUIElementRef  * CocAXUIElementRef;
typedef CFArrayRef CocCFArrayRef;
#endif
typedef NSArray  * CocNSArrayRef;

#else

//#include <Foundation/Foundation.h>

typedef struct NSGraphicsContext * CocGraphicsContextRef;
typedef struct NSBitmapImageRep * CocBitmapImageRepRef;
typedef struct NSImage * CocImageRef;
typedef struct NSEvent * CocEventRef;
typedef struct NSApplication * CocNSAppRef;
typedef struct NSAutoreleasePool * CocAutorelesePoolRef;
typedef struct NSWindow * NSWindowRef;
typedef struct UNSWindow * UNSWindowRef;
typedef struct UNSView * UNSViewRef;
typedef struct NSGraphicsContext * UNSGraphicsContextRef;

#ifdef __APPLE__
typedef struct AXUIElementRef * CocAXUIElementRef;
typedef struct CFArrayRef * CocCFArrayRef;
#endif
typedef struct NSArray * CocNSArrayRef;


#if __LP64__  //CHECK!
typedef long NSInteger;
#else
typedef int NSInteger;
#endif

#endif


#endif
