#ifndef _CtrlCoreCoc_UNSImage_h_
#define _CtrlCoreCoc_UNSImage_h_


#include "UNS.h"
#ifdef __cplusplus
//#include <CtrlCore/CtrlCore.h>
#endif

#ifdef PLATFORM_COCOA

//#if defined(__OBJC__)
//#undef String
//#include <Cocoa/Cocoa.h>
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//	static NSColor *UNSColor(UPP:Color c);
//void UInfo(NSString *msg);

 //FIXME encodings
NSBitmapImageRep *CreateNSBitmapImageRep(UPP::Size sz);  //FIXME more params
void NSImageDrawInRect(NSImage * image, UPP::Rect r);
void NSImageDrawInNSRect(NSImage * image, NSRect rect);
void NSImageDrawAtNSPoint(NSImage * image, NSPoint point);

//void CocDrawLine(int x1, int y1, int x2, int y2, int width, UPP::Color _color);
//void UFillRect(CocGraphicsContextRef _gc, UNSViewRef _dw, int x, int y, int cx, int cy, UPP::Color _color);
//void UDrawRect(CocGraphicsContextRef _gc, UNSViewRef _dw, int x, int y, int cx, int cy, UPP::Color _color, int _width);


//#ifdef __cplusplus
//}
//#endif
//
//
//#endif


NAMESPACE_UPP	
	

END_UPP_NAMESPACE


#endif

#endif

