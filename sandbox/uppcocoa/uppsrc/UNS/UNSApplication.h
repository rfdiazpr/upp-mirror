#ifndef __UNSAPPLICATION_H__
#define __UNSAPPLICATION_H__


#include "UNS.h"
#ifdef __cplusplus
//#include <CtrlCore/CtrlCore.h>
#endif

#ifdef PLATFORM_COCOA
	
@interface UNSApplication : NSApplication
{
	NSTimer *msgTimer;
}
-(void)sendEvent:(NSEvent*)event;
-(void)sendSuperEvent:(NSEvent*)event;
//-(void)keyDown:(NSEvent *)event;  //for key emulation?
//-(void) discardEventsMatchingMask:(unsigned int)mask
//					  beforeEvent:(NSEvent *)lastEvent;
	//-(NSMenu*)applicationDockMenu:(NSApplication *)sender;

	//-(NSApplicationPrintReply)application: (NSApplication *) app printFiles:(NSArray *)files withSettings: (NSDictionary *)printSettings showPrintPanels:(MacOSBOOL)bShowPrintPanels;
	//-(NSApplicationTerminateReply)applicationShouldTerminate: (NSApplication *) app;
	//-(void)systemColorsChanged: (NSNotification*) pNotification;
	//-(void)screenParametersChanged: (NSNotification*) pNotification;

	//-(void)addFallbackMenuItem: (NSMenuItem*)pNewItem;
	//-(void)removeFallbackMenuItem: (NSMenuItem*)pOldItem;
	//-(void)addDockMenuItem: (NSMenuItem*)pNewItem;
	//-(void)applicationWillBecomeActive: (NSNotification *)pNotification;
	//-(void)applicationWillResignActive: (NSNotification *)pNotification;
	//-(BOOL)applicationShouldHandleReopen: (NSApplication*)pApp hasVisibleWindows: (MacOSBOOL)bWinVisible;
	//-(void)setDockIconClickHandler: (NSObject*)pHandler;

@end



NAMESPACE_UPP


class CocApplication {
private:
	CocNSAppRef nsapp;
public:
	CocApplication();
	~CocApplication();
	void Run();
};

END_UPP_NAMESPACE

#endif

#endif
