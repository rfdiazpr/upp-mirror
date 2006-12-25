#include "CtrlCore.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

Image ReadClipboardImage()
{
	Image m = ReadClipboardFormat<Image>();
	if(!m.IsEmpty())
		return m;
	HGLOBAL hmem;
	if(!OpenClipboard(NULL))
		return Null;
	if((hmem = GetClipboardData(CF_DIB)) == NULL) {
		CloseClipboard();
		return Null;
	}
	BITMAPINFO *lpBI = 	(BITMAPINFO *)GlobalLock(hmem);
	BITMAPINFOHEADER& hdr = lpBI->bmiHeader;
	byte *bits = (byte *)lpBI + hdr.biSize;
	if(hdr.biBitCount <= 8)
		bits += (hdr.biClrUsed ? hdr.biClrUsed : 1 << hdr.biBitCount) * sizeof(RGBQUAD);
	if(hdr.biBitCount >= 16 || hdr.biBitCount == 32) {
		if(hdr.biCompression == 3)
			bits += 12;
		if(hdr.biClrUsed != 0)
			bits += hdr.biClrUsed * sizeof(RGBQUAD);
	}
	int h = abs((int)hdr.biHeight);
	ImageDraw   w(hdr.biWidth, h);
	::StretchDIBits(w.GetHandle(),
		0, 0, hdr.biWidth, h,
		0, 0, hdr.biWidth, h,
		bits, lpBI, DIB_RGB_COLORS, SRCCOPY);
	GlobalUnlock(hmem);
	CloseClipboard();
	return w;
}

void AppendClipboardImage(const Image& img)
{
	if(img.IsEmpty() || !OpenClipboard(NULL))
		return;
	BITMAPINFOHEADER header;
	Zero(header);
	header.biSize = sizeof(header);
	header.biWidth = img.GetWidth();
	header.biHeight = -img.GetHeight();
	header.biBitCount = 32;
	header.biPlanes = 1;
	header.biCompression = BI_RGB;
	HGLOBAL hmem = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, sizeof(header) + 4 * img.GetLength());
	if(!hmem) {
		CloseClipboard();
		return;
	}
	byte *p = (byte *)GlobalLock(hmem);
	memcpy(p, &header, sizeof(header));
	memcpy(p + sizeof(header), ~img, 4 * img.GetLength());
	GlobalUnlock(hmem);
	SetClipboardData(CF_DIB, hmem);
	CloseClipboard();
	AppendClipboardFormat(img);
}

#endif

END_UPP_NAMESPACE
