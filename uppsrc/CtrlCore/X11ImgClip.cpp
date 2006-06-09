#include "CtrlCore.h"

#ifdef PLATFORM_X11

#include <plugin/bmp/bmp.h>

Image ReadClipboardImage()
{
	Image m = ReadClipboardFormat<Image>();
	if(!m.IsEmpty())
		return m;
	Image img = StreamRaster::LoadStringAny(ReadClipboard("image/bmp"));
	if(!img.IsEmpty())
		return img;
	return StreamRaster::LoadStringAny(ReadClipboard("image/png"));
}

void AppendClipboardImage(const Image& img)
{
	WriteClipboardFormat(img);
	BMPEncoder bmp;
	AppendClipboard("image/bmp", bmp.SaveString(img));
}

#endif
