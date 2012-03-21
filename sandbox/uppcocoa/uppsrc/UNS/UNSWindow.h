#ifndef __UNSWINDOW_H__
#define __UNSWINDOW_H__

#include "UNS.h"

#ifdef SKELETON_COCOA

#ifdef __cplusplus
//#include <CtrlCore/CtrlCore.h>
#endif

NAMESPACE_UPP

//class Ctrl;

END_UPP_NAMESPACE

struct CtrlRef;

#ifdef  __OBJC__


#if defined(__APPLE__)
@interface UNSWindow : NSWindow  <NSWindowDelegate>
#else
@interface UNSWindow : NSWindow
#endif
{
@public
#if defined(__cplusplus) && defined(__APLE__)  //need to harmonize typedefs!!!! spelling on on purpose dbg
	UPP::Ctrl * pCtrl_; //can't get it compiled on windows MINGW
#else
	CtrlRef *  pCtrl_;
	CtrlRef * pOwner_;
#endif
	UNSView * nsview;
	//tmp
	 UNSView* myButtonView;
	 UNSView* myRadioView;
	// GradientView* myGradientView;
	// PictView* myPictView;
	 NSView* myProgressView;
	// TrackView* myTrackView;
	
	// NSPopUpButton* myPopupButton;		// nib-based control
	NSPopUpButton* myCustomPopupButton;			// popup button from scratch
	NSPopUpButton* myCustomPopupButtonPullDown;	// pull down button from scratch
	
//	 NSImageView* myImageView;
	
	NSMenu* appMenuBar;
	BOOL isAcceptsMouseMoved;
	BOOL modal_;
	CGFloat th; //titlebar height
}
//+ (id) initialize;
- (id)initWithCtrl:(CtrlRef *)ctrl owner:(CtrlRef *)owner style:(NSUInteger)style;

//- (id)initWithOwner(CtrlRef *)ctrl;// owner:(CtrlRef *)owner; // style:(NSUInteger)style;

//-(void)setOwner:(UNSWindowRef *)owner; 
//- (void)createPopupButtons; //tmp 
//- (void)addMenuItemViews:(NSMenu*)menu;
- (CtrlRef *)owner;
- (void)setModal:(BOOL) _modal;
- (BOOL)isSheet;
- (void)makeKeyWindow;
- (void) setAcceptsMouseMovedEvents: (BOOL)flag;
- (CGFloat) titlebarHeight;

@end
#endif
//#else
//#include <Core/Core.h>
#endif





#endif
