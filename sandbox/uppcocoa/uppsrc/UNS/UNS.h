#ifndef _Coc1_UNS_h_
#define _Coc1_UNS_h_

//#define flagTEST_UNS 1

#ifdef flagSKELETON
#define SKELETON_COCOA 1
#define GUI_COCOA 1
#endif

//#ifdef PLATFORM_COCOA //GUI_SKELETON
//#include <Draw/Draw.h>
#include <RichText/RichText.h>

#ifdef flagTEST_UNS
#include <UNSCtrl/UNSCtrl.h>
//#else
//#include <CtrlCore/CtrlCore.h>
#endif
//NAMESPACE_UPP 


struct CtrlRef;

//extern "C"
//{
//#import <Foundation/Foundation.h>
//#import <AppKit/AppKit.h>
//}


#ifdef  __OBJC__
#include <Cocoa/Cocoa.h>
#include <Foundation/Foundation.h>

#include <UNS/UNSDef.h>

#include "UNSApplication.h"
#include "UNSApplicationDelegate.h"
#include "UNSImage.h"
#include "UNSView.h"
//#include "UNSViewGL.h"
#include <UNS/UNSWindow.h>  //if you want symbols from it to be visible outside?
#include <UNS/UNSView.h> 
//#include <UNS/CocWnd.h>

#ifdef flagWINDOW_MGR
#include <UNS/WindowManager.h>
#endif


NSString *UNSString(const char *text);
const char * CocUString(NSString * ns);
NSColor *UNSColor(UPP::Color c);
UPP::Rect CocURect(NSRect rect);
UPP::Point CocUPoint(NSPoint p);
UPP::Point CocViewPointW(NSEvent *e); //in window
UPP::Point CocViewPointV(NSEvent *e); //in View
UPP::Point CocViewPointS(NSEvent *e); //in Screen


UPP::Size CocUSizeFromNSRect(NSRect bounds);
NSRect UNSRect(UPP::Rect r);
//NSRect CenterViewRect(UNSViewRef _dw);
void UFillNSRect(NSRect rect, UPP::Color color);
void UDrawNSRect(NSRect rect, int width, UPP::Color color);

UNSWindowRef CreateCocWndFromCtrl(CtrlRef *ctrl, NSUInteger style);

void CocDrawText1(int x, int y, const char *text, NSFont *font, UPP::Color color);


#else

#include <UNS/UNSDef.h>
#include <UNS/UNSWindow.h>
#include <UNS/UNSView.h>
//#include <UNS/CocWnd.h>

#ifdef flagWIN_MGMT
#include <UNS/WindowManager.h>
#endif

#endif


CocBitmapImageRepRef CreateBitmapImageRep(UPP::Size sz);
//void USetSurface2(int x, int y, int cx, int cy, const UPP::RGBA *pixels);
//void CocSetSurface2(UPP::SystemDraw& w, int x, int y, int cx, int cy, const UPP::RGBA *pixels);

void CocFillRect4(int x, int y, int cx, int cy, UPP::Color color);
void CocDrawLine(int x1, int y1, int x2, int y2, int width, UPP::Color color);

//for tests. this function draws on NS bitmap using Cocoa draw engine
UPP::Image DrawOnImage(UPP::Size sz, const UPP::RGBA *pixels); //for testing

void CocSetContextBitmap(CocBitmapImageRepRef bitmap);
void CocSaveGraphicsState();
void CocRestoreGraphicsState();

void CocBitmapDrawAtPoint(CocBitmapImageRepRef bitmap, int x, int y);
void CocNSImageDrawAtPoint(NSImage * image, UPP::Point point);

void CocDrawText(int x, int y, const char *text, UPP::Font font, UPP::Color color);

UPP::Rect CocGetScreenRect(int screen = 0);
UPP::Rect CocGetVisibleScreenRect(int screen = 0);

UPP::Rect CocGetViewRectInScreen(UNSViewRef view);
UPP::Rect CocGetViewRectInScreen1(UNSWindowRef window);
void CocMoveResizeWindow(UNSWindowRef window, int x, int y, int cx, int cy);


UPP::String CocGetAppName();

void UFillRect(UPP::Rect r, UPP::Color color);

void UDrawRect4(int x, int y, int cx, int cy, int width, UPP::Color color);
void UDrawRect(UPP::Rect r, int width, UPP::Color color);
void USetSurface1(const UPP::Rect& dest, const UPP::RGBA *pixels, UPP::Size psz, UPP::Point poff);

//UPP::Image CocUppImageOfBitmap(CocBitmapImageRepRef bitmap);
//UPP::Image UppImageFromData(UPP::RGBA *pixels, int cx, int cy);
UPP::Image UppImageFromData(unsigned char * pixels, int cx, int cy);

#ifdef __APPLE__
NSImage *CreateNSImagepOfUppImage(UPP::Image img);
UPP::Image CocNSImage(CocImageRef nsimg, int idx = 0);
#endif

int RunCocWindowModal(UNSWindowRef winref);

struct CocAlert
{
//	CocAlert() {;}
explicit
	CocAlert(const UPP::String& msg,  const char * info = UPP::String("Info"));
	~CocAlert() {;}
};

class UppCocAutoReleasePool
{
private:
	CocAutorelesePoolRef nspool;
public:
	UppCocAutoReleasePool();
	~UppCocAutoReleasePool();
	
	inline CocAutorelesePoolRef GetPool() const { return nspool; }
};


#endif

