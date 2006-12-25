#include "CtrlCore.h"

#include <plugin/bmp/bmp.h>

NAMESPACE_UPP

#ifdef PLATFORM_X11

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

END_UPP_NAMESPACE
