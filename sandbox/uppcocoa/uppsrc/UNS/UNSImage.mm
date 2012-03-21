//#include "CtrlCoreCoc.h"
#include "UNS.h"
//#include <CtrlCore/CtrlCore.h>

#ifdef PLATFORM_COCOA

//#define IMAGECLASS SkelImg
//#define IMAGEFILE <SkeletonCoc/Skeleton.iml>
//#include <Draw/iml_source.h>
//#include <Draw/iml.h>

//using namespace Upp;

#define LLOG(x) //LOG(x)
#define LTIMING(x)   RTIMING(x)

//name is in style of apple...
NSImage *CreateNSImagepOfUppImage(UPP::Image img)
{
	UPP::Size psz = img.GetSize();
	unsigned char* dataBuffer = (unsigned char*)~img;
	          
	NSBitmapImageRep * imageBitmapImageRef = [[NSBitmapImageRep alloc]
							initWithBitmapDataPlanes:&dataBuffer
//							initWithData:data
							pixelsWide:psz.cx
							pixelsHigh:psz.cy
							bitsPerSample:8 
							samplesPerPixel:4 
							hasAlpha:YES
							isPlanar:NO 
							colorSpaceName:UNSString("NSDeviceRGBColorSpace")
#ifndef COMPILER_MINGW
							bitmapFormat:0//NSAlphaFirstBitmapFormat
#endif
							bytesPerRow:4* psz.cx
							bitsPerPixel:32];
	
	NSSize size = {psz.cx,psz.cy };
	NSImage * nsimg = [[NSImage alloc] initWithSize:size];
	[nsimg addRepresentation:imageBitmapImageRef];
	return nsimg;
	
//	NSCursor * whiteIBeamCursor = [[NSCursor alloc] initWithImage:
//	[NSImage imageNamed:@"whiteIBeamCursor"]
//	hotSpot:NSMakePoint(4.0f,4.0f)];
//	[myTextView addCursorRect:[myTextView bounds] cursor:whiteIBeamCursor];
//	[whiteIBeamCursor setOnMouseEntered:YES];
}

NSBitmapImageRep *CreateNSBitmapImageRep(UPP::Size sz)  //FIXME - more params
{
	return [[NSBitmapImageRep alloc]
							initWithBitmapDataPlanes:NULL
//							initWithData:data
							pixelsWide:sz.cx 
							pixelsHigh:sz.cy
							bitsPerSample:8 
							samplesPerPixel:4 
							hasAlpha:YES
							isPlanar:NO 
							colorSpaceName:UNSString("NSCalibratedRGBColorSpace")
//							bitmapFormat:NSAlphaFirstBitmapFormat 
							bytesPerRow:0 
							bitsPerPixel:32];  //check in which cases we need to set it? 
}


void NSImageDrawInRect(NSImage * image, UPP::Rect r)
{
	[image drawInRect: UNSRect(r)
			 fromRect: NSZeroRect
			operation: NSCompositeSourceOver
			 fraction: 1.0];
}

void NSImageDrawInNSRect(NSImage * image, NSRect rect)
{
	[image drawInRect: rect
			 fromRect: NSZeroRect
			operation: NSCompositeSourceOver
			 fraction: 1.0];
}

void NSImageDrawAtNSPoint(NSImage * image, NSPoint point)
{
	[image drawAtPoint: point
			 fromRect: NSZeroRect
			operation: NSCompositeSourceOver
			 fraction: 1.0];
}

void CocNSImageDrawAtPoint(NSImage * image, UPP::Point point)
{
	[image drawAtPoint: NSMakePoint(point.x, point.x)
			 fromRect: NSZeroRect
			operation: NSCompositeSourceOver
			 fraction: 1.0];
}


void CocBitmapDrawAtPoint(CocBitmapImageRepRef bitmap, int x, int y)
{
	[(NSBitmapImageRep *) bitmap  drawAtPoint:NSMakePoint(x,y)]; 
}



//void UFillRect(CocGraphicsContextRef _gc, UNSViewRef _dw, int x, int y, int cx, int cy, UPP::Color color)
void CocFillRect4(int x, int y, int cx, int cy, UPP::Color color)
{
	NSRect rect = NSMakeRect(x, y, cx, cy);
	[UNSColor(color) set];
	//NSRectFill(rect); //FIXME much faster but can't use on gnustep windows - undefined reference
	NSBezierPath * path = [NSBezierPath bezierPathWithRect:rect];
	[path fill];
}

void UFillRect(UPP::Rect r, UPP::Color color)
{
	[UNSColor(color) set];
	NSBezierPath * path = [NSBezierPath bezierPathWithRect:UNSRect(r)];
	[path fill];
}

void UFillNSRect(NSRect rect, UPP::Color color)
{
	[UNSColor(color) set];
	NSBezierPath * path = [NSBezierPath bezierPathWithRect:rect];
	[path fill];
}

//void UDrawRect(CocGraphicsContextRef _gc, UNSViewRef _dw, int x, int y, int cx, int cy, UPP::Color _color, int _width)
void UDrawRect4(int x, int y, int cx, int cy, int width, UPP::Color color)
{
	NSRect rect = NSMakeRect(x, y, cx, cy);
	[UNSColor(color) set];
	NSBezierPath * path = [NSBezierPath bezierPathWithRect:rect];
	[path setLineWidth:width];
	[path stroke];
}

//void UDrawRect(CocGraphicsContextRef _gc, UNSViewRef _dw, int x, int y, int cx, int cy, UPP::Color _color, int _width)
void UDrawRect(UPP::Rect r, int width, UPP::Color color)
{
	[UNSColor(color) set];
	NSBezierPath * path = [NSBezierPath bezierPathWithRect:UNSRect(r)];
	[path setLineWidth:width];
	[path stroke];	
}

void UDrawNSRect(NSRect rect, int width, UPP::Color color)
{
	[UNSColor(color) set];
	NSBezierPath * path = [NSBezierPath bezierPathWithRect:rect];
	[path setLineWidth:width];
	[path stroke];
}

void CocDrawLine(int x1, int y1, int x2, int y2, int width, UPP::Color color)
{
	NSBezierPath * path = [NSBezierPath bezierPath];
	[UNSColor(color) set];
	[path setLineWidth:width];
	[path moveToPoint:NSMakePoint(x1, y1)];
	[path lineToPoint:NSMakePoint(x2, y2)];
	[path stroke];
}

//dest and src size different
//	SetSurface(w, 20,20, sz.cx, sz.cy, imb );
	
//	bitMapDataPtr = [bitmap bitmapData];
//
//	if ((bitMapDataPtr != nil) && (pixBaseAddr != nil))
//	{
//		int i;
//		int pixmapRowBytes = GetPixRowBytes(pixMapHandle);
//		NSSize imageSize = [bitmap size];
//		for (i=0; i< imageSize.height; i++)
//		{
//			int j;
//			unsigned char *src = bitMapDataPtr + i * [bitmap bytesPerRow];
//			unsigned char *dst = pixBaseAddr + i * pixmapRowBytes;
//			for ( j=0; j<imageSize.width; ++j )
//			{
//				*dst++ = 0;
//				*dst++ = *src++;
//				*dst++ = *src++;
//				*dst++ = *src++;
//			}
//		}
//	}


void USetSurface1(const UPP::Rect& dest, const UPP::RGBA *pixels, UPP::Size psz, UPP::Point poff)
{
	unsigned char* dataBuffer = (unsigned char*)pixels;
	LLOG("DRAW IMAGE MAC SetSurface size different");
//		Point p = dest.TopLeft() + w.GetOffset();
//	XPutImage(Xdisplay, dw, gc, &ximg, srcoff.x, srcoff.y,
//	          p.x, p.y, dest.GetWidth(), dest.GetHeight());
	          
	NSBitmapImageRep * imageBitmapImageRef = [[NSBitmapImageRep alloc]
							initWithBitmapDataPlanes:&dataBuffer
//							initWithData:data
							pixelsWide:psz.cx //dest.Width() //rect.size.width
							pixelsHigh:psz.cy //dest.Height() //rect.size.height
							bitsPerSample:8 
							samplesPerPixel:4 
							hasAlpha:YES
							isPlanar:NO 
							colorSpaceName:UNSString("NSDeviceRGBColorSpace")
#ifndef COMPILER_MINGW
							bitmapFormat:0//NSAlphaFirstBitmapFormat
#endif
							bytesPerRow:4* psz.cx //dest.Width()
							bitsPerPixel:32];
	
//	[imageBitmapImageRef CGImage
//	 NSData *data = [imageBitmapImageRef representationUsingType:NSPNGFileType properties: nil];
//	[data writeToFile: UNSString("/Users/aris002/Desktop/atest1.png") atomically: NO];
	NSSize size = {psz.cx,psz.cy };
	NSImage * img = [[NSImage alloc] initWithSize:size];
	[img addRepresentation:imageBitmapImageRef];
	NSPoint point = {0,0};
	NSImageDrawAtNSPoint(img, point);
	[imageBitmapImageRef release];
	[img release];
//	[imageBitmapImageRef  drawAtPoint:NSMakePoint(poff.x, poff.y)]; //Paints faster than drawInRect?
}
	

CocBitmapImageRepRef CreateBitmapImageRep(UPP::Size sz)
{
//	return CreateNSBitmapImageRep(sz);
	LLOG("IMAGE ====bitmap======== size0="<<sz);
//	UPP::ImageBuffer img(sz);
//	unsigned char* data = (unsigned char*)(const UPP::RGBA*)img;
	NSBitmapImageRep * bitmap = [[NSBitmapImageRep alloc]
							initWithBitmapDataPlanes:NULL //&data
//							initWithData:&data
							pixelsWide:sz.cx 
							pixelsHigh:sz.cy
							bitsPerSample:8 
							samplesPerPixel:4 
							hasAlpha:YES
							isPlanar:NO 
							colorSpaceName:UNSString("NSCalibratedRGBColorSpace")
//							bitmapFormat:NSAlphaFirstBitmapFormat 
							bytesPerRow:4*sz.cx 
							bitsPerPixel:32];
	int cx = [(NSBitmapImageRep *)bitmap pixelsWide];
	int cy = [(NSBitmapImageRep *)bitmap pixelsHigh];
	int len = cx * cy;
//	UPP::ImageBuffer im(cx, cy);
	LLOG("IMAGE ====bitmap======== size"<<UPP::Size(cx,cy));
	return bitmap;
}

void CocSetContextBitmap(CocBitmapImageRepRef bitmap)
{
	[NSGraphicsContext setCurrentContext:[NSGraphicsContext graphicsContextWithBitmapImageRep:(NSBitmapImageRep *)bitmap]];
}

void CocSaveGraphicsState()
{
	[NSGraphicsContext saveGraphicsState];
}

void CocRestoreGraphicsState()
{
	[NSGraphicsContext restoreGraphicsState];
}

UPP::Image CocUppImageOfBitmap(unsigned char * pixels, int cx, int cy)
{
//	const UPP::RGBA * dataBuffer =(UPP::RGBA*) (unsigned char*)[(NSBitmapImageRep *)bitmap bitmapData];
	const UPP::RGBA * dataBuffer =(UPP::RGBA*) (unsigned char*) pixels; //[(NSBitmapImageRep *)bitmap bitmapData];
//	int cx = [(NSBitmapImageRep *)bitmap pixelsWide];
//	int cy = [(NSBitmapImageRep *)bitmap pixelsHigh];
	int len = cx * cy;
	UPP::ImageBuffer b(cx, cy);
//	UPP::Image img = UPP::CreateImage(UPP::Size(cx,cy), (UPP::RGBA*) (unsigned char*)[(NSBitmapImageRep *)bitmap bitmapData]);
//	UPP::Image img = UPP::CreateImage(UPP::Size(cx,cy), (UPP::RGBA*)&dataBuffer);
	Copy(b, dataBuffer, len);
//	memcpy(dataBuffer, b.pixels, b.GetLength() * sizeof(UPP::RGBA));

	LLOG("IMAGE ============ size"<<UPP::Size(cx,cy));
//	UPP::RGBA *s = b; //.Begin();
//	for(int i=0; i<len; i++)  //should we use memcopy or occupy the address? //old - doesnt work
//	{
//		*s++ = *dataBuffer++;
//		s->b = 0;
//		s->r = 0;
//		s->g = 0;
//		s->a = 0;
//	}
	
	return b;
}

#ifdef __APPLE__
//this should be done in ImageDraw or this should be a part of ImageDraw
unsigned char * NSImageRepToRGBA(NSImageRep * img_rep, int cx, int cy)
{
	unsigned char * rgba;
	NSBitmapImageRep * bitmap_tmp;
	NSGraphicsContext * context;
	
	rgba = (unsigned char *)malloc(4*cx*cy);
	//should we create UPP::Buffer here and send 
	
	bitmap_tmp = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:&rgba
													pixelsWide:cx
													pixelsHigh:cy
												bitsPerSample:8
												samplesPerPixel:4
														hasAlpha:YES
														isPlanar:NO
											colorSpaceName:UNSString("NSCalibratedRGBColorSpace")
													bitmapFormat:0
													bytesPerRow:(cx * 4)
													bitsPerPixel:32];
	
	context = [NSGraphicsContext graphicsContextWithBitmapImageRep: bitmap_tmp];
	[NSGraphicsContext saveGraphicsState];
	[NSGraphicsContext setCurrentContext: context];

	[img_rep drawInRect: NSMakeRect(0, 0, cx, cy)]; //could we draw just a part of a image if different size?

	[NSGraphicsContext restoreGraphicsState];
	
	[bitmap_tmp release];
	
	return rgba;
}


/*FIXME from Apple - This application is assuming that a particular image contains an NSBitmapImageRep, which is not a good assumption. 
 We are instantiating a bitmap so that whatever this is keeps working, but please do not do this.
 Break on _NSPrivateIconImageRepSubclassAssumedToBeBitmap to debug.
  This will be logged only once.  This may break in the future. */
UPP::Image CocNSImage(CocImageRef nsimg, int idx) //nsimage holds a few reps
{
	NSArray *ar = [nsimg representations];
	int count = [ar count];
	if (count>0)
	{
		if (count - 1 < idx) idx = count-1;
//		NSBitmapImageRep * bitmap = [ar objectAtIndex:idx];
		NSImageRep * imgrep = [ar objectAtIndex:idx];
		
		CGImageRef cgImage = [imgrep CGImageForProposedRect:NULL context:[NSGraphicsContext currentContext] hints:NULL];
		NSBitmapImageRep * bitmap =[[NSBitmapImageRep alloc] initWithCGImage:cgImage];
	//	return UPP:::Null;
		int width = [imgrep pixelsWide];
		int height = [imgrep pixelsHigh];
		unsigned char * rgba = NSImageRepToRGBA(imgrep, width, height);
		return CocUppImageOfBitmap(rgba, width, height);
	}
	else
		return UPP::Null;
//	CocAlert(UPP::AsString(count));
}
#endif

UPP::Image DrawOnImage(UPP::Size sz, const UPP::RGBA *pixels)
{
//	NSBitmapImageRep * bitmap = (NSBitmapImageRep *) CreateBitmapImageRep(sz);
	int len = sz.cx * sz.cy;
	UPP::ImageBuffer img(sz);
//	unsigned char* data = (unsigned char*)pixels;
	unsigned char* data = (unsigned char*)(const UPP::RGBA*)img;
	NSBitmapImageRep * bitmap = [[NSBitmapImageRep alloc]
							initWithBitmapDataPlanes:&data
//							initWithData:(unsigned char *) pixels
							pixelsWide:sz.cx
							pixelsHigh:sz.cy
							bitsPerSample:8 
							samplesPerPixel:4 
							hasAlpha:YES
							isPlanar:NO
							colorSpaceName:UNSString("NSCalibratedRGBColorSpace") //@"NSDeviceRGBColorSpace"
							bytesPerRow:4*sz.cx
							bitsPerPixel:32];

	LLOG("after bitmap create");
//	NSGraphicsContext *oldcontext = [NSGraphicsContext currentContext];					
//	[NSGraphicsContext saveGraphicsState];
	CocSaveGraphicsState();
//	NSGraphicsContext *context = [NSGraphicsContext graphicsContextWithBitmapImageRep:bitmap];
	CocSetContextBitmap(bitmap);
//	[NSGraphicsContext setCurrentContext:[NSGraphicsContext graphicsContextWithBitmapImageRep:bitmap]];
	
	LLOG("after context create");								
	CocDrawLine(0, 0, 70, 150, 4, UPP::Blue);
	CocRestoreGraphicsState();
//	[NSGraphicsContext restoreGraphicsState];

//	[NSGraphicsContext setCurrentContext:oldcontext];
	
//	unsigned char* dataBuffer = (unsigned char*)[bitmap bitmapData];
/*
	UPP::RGBA* dataBuffer =(UPP::RGBA*) (unsigned char*)[bitmap bitmapData];
	

//	UPP::Buffer<UPP::RGBA> b(len);
//	Copy(dataBuffer, b, len);
	UPP::ImageBuffer im(sz);

	UPP::Buffer<UPP::RGBA> ab(len);
//			UPP::RGBA *s = ab;
	UPP::RGBA *s = im.Begin();
//			const RGBA *s = buffer;
//			const UPP::RGBA *e = dataBuffer;
//	*im = *dataBuffer;
	for(int i=0; i<len; i++)
	{
		*s++ = *dataBuffer++;
//		//	*s++ = 0xff00ff00; //*e++;
//		s->b = 255;
//		s->a = 0;
	}
*/
//	im. = NULL; //dataBuffer;
//	UPP::Image img;
//	im = dataBuffer;
//	*im = 
//	img.data = dataBuffer;
//	im = &dataBuffer;
//	UPP::Buffer<UPP::RGBA> b(&dataBuffer); //(sz);
//	b ~ (UPP::RGBA *) [bitmap bitmapData];
//	im.Begin() =
//	im.
//	im.Begin() = (UPP::RGBA *) [bitmap bitmapData];
//	im = b; 
//	return (UPP::Image)im;
	return img;
//	[imageBitmapImageRef  drawAtPoint:NSMakePoint(0,0)];

}

void CocDrawText(int x, int y, const char *text, UPP::Font font, UPP::Color color)
{
//	UppCocAutoReleasePool mpool;
//	NSLog(@"CocDrawText =%@", UNSString(text));
//	LLOG("UNS FACE NAME="<<font.GetFaceName()<<" lineCY="<< font.GetCy()<<" text="<<text);
	NSFontManager *fontManager = [NSFontManager sharedFontManager];
//	[NSFont fontWithName:UNSString(font.GetFaceName())
//	                              size:font.GetCy()]
	NSFont *myFont = [fontManager fontWithFamily:UNSString(font.GetFaceName()) //UNSString("Arial")
                                          traits:0 // NSUnBoldFontMask //|NSItalicFontMask
                                          weight:4
                                           size:font.GetCy()-1];
	NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys:
	 myFont, NSFontAttributeName, UNSColor(color), NSForegroundColorAttributeName, nil];

//	NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys:
//	                            UNSString(font.GetFaceName()), NSFontAttributeName,
//	                            UNSColor(color), NSForegroundColorAttributeName,
//	                            nil];
	 
//
////[NSString stringWithUTF8String:UPP::AsCString(text)];
////	stringWithCString
//	NSAttributedString * currentText
//			= [[NSAttributedString alloc]
//			 initWithString:UNSString(text) attributes: attributes];
//
//	NSSize attrSize = [currentText size];
//	[currentText drawAtPoint:NSMakePoint(x, y)];
	
//	[UNSString(text) drawAtPoint:NSMakePoint(x, y) withAttributes:[NSDictionary dictionaryWithObject:UNSColor(color)
//		forKey:NSForegroundColorAttributeName]];
//LLOG("UNS FACE NAME drawatpoint");
	[UNSString(text) drawAtPoint:NSMakePoint(x, y) withAttributes:attributes];
//LLOG("//UNS FACE NAME drawatpoint");

//	 [UNSString(text) drawAtPoint:NSMakePoint(x, y) withAttributes:[NSDictionary dictionaryWithObjectsAndKeys:
//	                                [NSFont fontWithName:UNSString(font.GetFaceName()) size:font.GetCy()],
//	                 NSFontAttributeName, UNSColor(color), NSForegroundColorAttributeName , nil] ];
}

void CocDrawText1(int x, int y, const char *text, NSFont *myFont, UPP::Color color)
{
//	NSLog(@"CocDrawText1 =%@", UNSString(text));
	NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys:
	 	myFont, NSFontAttributeName, UNSColor(color), NSForegroundColorAttributeName, nil];

	[UNSString(text) drawAtPoint:NSMakePoint(x, y) withAttributes:attributes];
}

NAMESPACE_UPP	

	
END_UPP_NAMESPACE

#endif