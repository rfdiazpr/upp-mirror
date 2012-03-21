#include "UNSCtrl.h"

#ifdef SKELETON_COCOA

NAMESPACE_UPP
#define LLOG(x)  LOG(__PRETTY_FUNCTION__<<" "<<x)
void Ctrl::InitCocoa()
{
//	GuiLock __; 
//	StdLogSetup(LOG_CERR);
	LLOG("->");
//	ChSync();
//	CocAlert("init cocoa");
//	InitTimer();
	UppCocAutoReleasePool mpool;

	LLOG("=================Ctrl::InitCocoa 1 ");
	int i=Font::FindFaceNameIndex("Arial");
	LLOG("=================Ctrl::InitCocoa 2 ");
	Font font = Font(i,12);
	LLOG("=================Ctrl::InitCocoa 3 ");
	Font::SetStdFont(font);
//	SetStdFont(Arial(12));
	LLOG("\n\n\n===================================================================Ctrl::InitCocoa 4 ");

//	Csizeinit();
//	ReSkin();

	
//	GUI_GlobalStyle_Write(GUISTYLE_CLASSIC);
//	GUI_GlobalStyle_Write(GUISTYLE_XP);
	
	//	ColoredOverride(CtrlsImg::Iml(), CtrlsImg::Iml());
//	GUI_DragFullWindow_Write(1);
//	GUI_PopUpEffect_Write(IsCompositedGui() ? GUIEFFECT_NONE : GUIEFFECT_SLIDE);
//	GUI_PopUpEffect_Write(GUIEFFECT_NONE);

//	GUI_DropShadows_Write(1);
//	GUI_AltAccessKeys_Write(1);
//	GUI_AKD_Conservative_Write(0);
//	GUI_ToolTipDelay_Write(500);

	setlocale(LC_ALL, "en_GB.utf8"); //do we need this?
	
//	GlobalBackPaint(false); //tmp!!!
//	GlobalBackBuffer(true); //tmp!!!
//	UppCocAutoReleasePool mpool;
//	UNSApplication * application = new UNSApplication; // alll] sharedApplication];  //crashes
	[UNSApplication sharedApplication];
#if __APPLE__  //FIXME what if GNUSTEP on APPLE?
	[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular]; //otherwise no menus and mouse move etc.
#endif

//	Ctrl::SetAppName(CocGetAppName());

//	LLOG("befor cocoamenu");	
//	Ctrl::CocoaMenu(); //FIXME integrate with Upp menus
//	LLOG("after cocoamenu");

//	UNSApplicationDelegate *appDelegate  = [UNSApplicationDelegate alloc];
//	[NSApplication setDelegate:[[UNSApplicationDelegate alloc] init]];
//	CocAlert("","");

	[NSApp setDelegate: [UNSApplicationDelegate alloc]];  //should we use UNSApplication as delegate?
	[NSApp activateIgnoringOtherApps:YES]; //this should elsewhere with a param

//no need need for this if we are not using application delegate??
//but enable if you are gettin errors like "Set a breakpoint @ CGErrorBreakpoint().."
	[NSApp finishLaunching];
	

	Ctrl::CocoaMenu();
	
	//this is to process all NSApp queued launching events until we take care in our EventLoop0
	//otherwise NSWindow is not activated, mouse events not generated etc...
	//CHECK filter event to NS..system and NS..Kit?
	//What about Esc/Cancel during launch or ?
//	return;
	for(;;)
	{
//		UppCocAutoReleasePool mpool;
		NSEvent *event= [NSApp nextEventMatchingMask:NSAnyEventMask
			untilDate:nil //the same as [NSDate distantPast] 
			inMode:NSDefaultRunLoopMode
			dequeue:YES];
		if(event)
			[NSApp sendEvent: event];
		else
			break;
	}
//	CocAlert("init cocoa");
//	[NSApp run];
	LLOG("//\\\\\\");
}

void Ctrl::RunCocoa()
{
//	GuiLock __; 
	[NSApp setDelegate: [UNSApplicationDelegate alloc]];
	[NSApp activateIgnoringOtherApps:YES];
//	[NSApp run];
}

void Ctrl::CocoaMenu()
{
//	GuiLock __;
//	LLOG("inside Ctrl::CocoaMenu");
//	MemoryIgnoreLeaksBlock __;
	UppCocAutoReleasePool mpool1;

	NSString *  applicationName = [[NSProcessInfo processInfo] processName];
	NSString * quitTitle = [UNSString("Quit ") stringByAppendingString:applicationName];


	NSMenu * menubar = [[NSMenu alloc] initWithTitle:UNSString("MainMenu")];	
	// The titles of the menu items are for identification purposes only and shouldn't be localized.
	// The strings in the menu bar come from the submenu titles,
	// except for the application menu, whose title is ignored at runtime.
	NSMenuItem * appMenuItem = [menubar addItemWithTitle:UNSString("Apple") action:NULL keyEquivalent:UNSString("")];
	NSMenu *  appMenu = [[NSMenu alloc] initWithTitle:UNSString("Apple")];
		[appMenuItem setSubmenu:appMenu];
	[NSApp performSelector:@selector(setAppleMenu:) withObject:appMenu];

	[appMenu addItem:[NSMenuItem separatorItem]];

	NSMenuItem * menuItem = [appMenu addItemWithTitle:quitTitle
								action:@selector(terminate:)
						 keyEquivalent:UNSString("q")];
	[menuItem setTarget:NSApp];

	[NSApp setMainMenu:menubar];
	LLOG("//inside Ctrl::CocoaMenu");
//	[[NSApp mainMenu] insertItem:newItem atIndex:1];  //for new upp menus
//	[menubar setSubmenu:appMenu forItem:quitMenuItem];
}

//for testing without CtrlCore
void Ctrl::CreateApp()  
{
//	application = (void*)[NSApplication sharedApplication];
	[NSApplication sharedApplication];	
}


void Ctrl::RunApp()
{
	[NSApp run];
//	[(NSApplication*)application run];
}

END_UPP_NAMESPACE

#endif
