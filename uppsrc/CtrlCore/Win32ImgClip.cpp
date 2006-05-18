#include "CtrlCore.h"

#ifdef PLATFORM_WIN32

#ifdef NEWIMAGE

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
	int h = abs(hdr.biHeight);
	ImageDraw   w(hdr.biWidth, h);
	::StretchDIBits(w.GetHandle(),
		0, 0, hdr.biWidth, h,
		0, 0, hdr.biWidth, h,
		bits, lpBI, DIB_RGB_COLORS, SRCCOPY);
	GlobalUnlock(hmem);
	CloseClipboard();
	return w;
}

bool WriteClipboardImage(const Image& img, bool clear)
{
	if(img.IsEmpty() || !OpenClipboard(NULL))
		return false;
	if(clear)
		EmptyClipboard();
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
		return false;
	}
	byte *p = (byte *)GlobalLock(hmem);
	memcpy(p, &header, sizeof(header));
	memcpy(p + sizeof(header), ~img, 4 * img.GetLength());
	GlobalUnlock(hmem);
	SetClipboardData(CF_DIB, hmem);
	CloseClipboard();
	WriteClipboardFormat(img, false);
	return true;
}

#else

PixelArray ClipboardToPixelArray()
{
	PixelArray out;
	if(ReadClipboardFormat(out))
		return out;

	HGLOBAL hmem;
	if(!OpenClipboard(NULL))
		return Null;
	if((hmem = GetClipboardData(CF_DIB)) == NULL)
	{
		CloseClipboard();
		return Null;
	}
	const BITMAPINFO *bmi = (const BITMAPINFO *)GlobalLock(hmem);
	int hdrsize = bmi -> bmiHeader.biSize;
	if(bmi -> bmiHeader.biCompression != BI_RGB && bmi -> bmiHeader.biCompression != BI_BITFIELDS)
		return Null;
	const dword *cmasks = NULL;
	if(bmi -> bmiHeader.biBitCount <= 8)
		hdrsize += 4 << bmi -> bmiHeader.biBitCount;
	else if(bmi -> bmiHeader.biBitCount == 16 || bmi -> bmiHeader.biBitCount == 32)
	{
		hdrsize += 12;
		cmasks = reinterpret_cast<const dword *>(bmi -> bmiColors);
	}
	const void *data = (const byte *)bmi + hdrsize;
	Vector<Color> palette;
	if(bmi -> bmiHeader.biBitCount <= 8)
	{
		palette.SetCount(1 << bmi -> bmiHeader.biBitCount);
		for(int i = 0; i < palette.GetCount(); i++)
			palette[i] = Color(bmi -> bmiColors[i].rgbRed, bmi -> bmiColors[i].rgbGreen, bmi -> bmiColors[i].rgbBlue);
	}
	out.Create(bmi -> bmiHeader.biWidth, bmi -> bmiHeader.biHeight,
		bmi -> bmiHeader.biBitCount == 24 ? -3 : bmi -> bmiHeader.biBitCount, 4, cmasks, palette);
	memcpy(out.Begin(), data, out.GetBytes());
	GlobalUnlock(hmem);
	CloseClipboard();
	return out;
}

AlphaArray ClipboardToAlphaArray()
{
	AlphaArray out;
	if(ReadClipboardFormat(out))
		return out;
	out.pixel = ClipboardToPixelArray();
	return out;
}

#endif

#endif
