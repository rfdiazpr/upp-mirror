#ifndef __UNSVIEW_H__
#define __UNSVIEW_H__

#include "UNS.h"

//#ifdef SKELETON_COCOA

#ifdef __cplusplus
//#include <CtrlCore/CtrlCore.h>
#endif

struct CtrlRef;
//struct CocGraphicsContextRef;

#ifdef  __OBJC__

#ifdef __APPLE__
@interface UNSView : NSView <NSWindowDelegate>
#else
@interface UNSView : NSView //CHECK GNUSTEP versions
#endif
{
//@private
//	UPP::Window 					* pWindow_; //test
#if defined(__cplusplus) && defined(__APLE__)
	UPP::Ctrl * pCtrl_; //cant get it compiled on windows MINGW
#else
	CtrlRef *  pCtrl_;
	CtrlRef * pOwner_;
#endif
//	CocBitmapImageRepRef  imageBitmapImageRef;
//	NSImage							* image_;//tmp? or can we use it for alpha images?
//r	NSTimer                         * viewTimer;
	NSGraphicsContext * context;
	BOOL wasAcceptingMouseEvents;
@public	
	BOOL dpaint;
}

//+ (id) initialize;
- (id)initWithFrameAndCtrl:(NSRect)frameRect ctrl:(CtrlRef *)ctrl;
- (void) setOwnerCtrl:(CtrlRef *)owner;
- (id) initWithFrame: (NSRect)frameRect;
- (CtrlRef *) uppCtrl;
//- (id) initWithBitmapImageRep;
//- (BOOL)acceptsFirstResponder;
//- (BOOL)becomeFirstResponder;
//- (void)mouseDown:(NSEvent*)event;
//- (void)mouseMoved:(NSEvent*)event;
//- (void) setOwnerWindow:(UPP::Window *)owner; 
- (void)refresh;
- (void)dbpaint:(BOOL)test;
//- (NSImage*)makeImage;
//- (void)setImage;
@end

#ifdef __APPLE__
@interface UNSViewGL : NSView <NSWindowDelegate> 
#else
@interface UNSViewGL : NSView //CHECK GNUSTEP versions
#endif
{
	@private
		NSOpenGLContext*     glContext;
		NSOpenGLPixelFormat* pixelFormat;
		bool ready;
#if defined(__cplusplus) && defined(__APLE__)
	UPP::Ctrl * pCtrl_; //cant get it compiled on windows MINGW
#else
	CtrlRef *  pCtrl_;
	CtrlRef * pOwner_;
#endif
}

- (void) _surfaceNeedsUpdate:(NSNotification*)notification;
+ (NSOpenGLPixelFormat*)defaultPixelFormat;
- (id)initWithFrameAndCtrlGL:(NSRect)frameRect ctrl:(CtrlRef *)ctrl pixelFormat:(NSOpenGLPixelFormat*)format;
- (id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)format;
- (CtrlRef *) uppCtrl;
//- (void)setOpenGLContext:(NSOpenGLContext*)context;
- (NSOpenGLContext*)openGLContext;
//- (void)clearGLContext;
//- (void)prepareOpenGL;
//- (void)update;
//- (void)setPixelFormat:(NSOpenGLPixelFormat*)pixelFormat;
//- (NSOpenGLPixelFormat*)pixelFormat;
@end

#endif


//class SystemDraw1 {
//	CocGraphicsContextRef gc;
//	UNSViewRef dw;
//public:
//	SystemDraw1(CocGraphicsContextRef gc, UNSViewRef dw);
//	SystemDraw1() {;}
//	~SystemDraw1() {;}
//};

//#endif

#endif
