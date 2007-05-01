#include "CtrlCore.h"
#include <plugin/bmp/bmp.h>

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

#define LLOG(x) // LOG(x)

int  GetClipboardFormatCode(const char *format_id)
{
	int x = (int)format_id;
	if(x >= 0 && x < 65535)
		return x;
	String fmt = format_id;
	if(fmt == "text")
		return CF_TEXT;
	if(fmt == "wtext")
		return CF_UNICODETEXT;
	if(fmt == "dib")
		return CF_DIB;
	static VectorMap<String, int> format_map;
	int f = format_map.Find(format_id);
	if(f < 0) {
		f = format_map.GetCount();
		format_map.Add(format_id,
#ifdef PLATFORM_WINCE
			::RegisterClipboardFormat(ToSystemCharset(format_id))
#else
			::RegisterClipboardFormat(format_id)
#endif
		);
	}
	return format_map[f];
}

void ClearClipboard()
{
	if(OpenClipboard(NULL)) {
		EmptyClipboard();
		CloseClipboard();
	}
}

void AppendClipboard(const char *format, const byte *data, int length)
{
	if(!OpenClipboard(NULL))
		return;
	HANDLE handle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, length + 2);
	byte *ptr;
	if(!handle || !(ptr = (byte *)GlobalLock(handle))) {
		if(handle) GlobalFree(handle);
		CloseClipboard();
		return;
	}
	memcpy(ptr, data, length);
	ptr[length] = 0;
	ptr[length + 1] = 0;
	GlobalUnlock(handle);
	if(!SetClipboardData(GetClipboardFormatCode(format), handle)) {
		LLOG("SetClipboardData error: " << GetLastErrorMessage());
		GlobalFree(handle);
		CloseClipboard();
		return;
	}
	CloseClipboard();
}

void AppendClipboard(const char *format, const String& data)
{
	AppendClipboard(format, data, data.GetLength());
}

String ReadClipboard(const char *format)
{
	if(!OpenClipboard(NULL))
		return Null;
	HGLOBAL hmem = GetClipboardData(GetClipboardFormatCode(format));
	if(hmem == 0) {
		CloseClipboard();
		return Null;
	}
	const byte *src = (const byte *)GlobalLock(hmem);
	ASSERT(src);
	int length = GlobalSize(hmem);
	if(length < 0) {
		CloseClipboard();
		return Null;
	}
	String out(src, length);
	GlobalUnlock(hmem);
	CloseClipboard();
	return out;
}

void AppendClipboardText(const String& s)
{
#ifdef PLATFORM_WINCE
	AppendClipboardUnicodeText(s.ToWString());
#else
	AppendClipboard((const char *)CF_TEXT, ToSystemCharset(s));
#endif
}

void AppendClipboardUnicodeText(const WString& s)
{
#ifndef PLATFORM_WINCE
	AppendClipboardText(s.ToString());
#endif
	AppendClipboard((const char *)CF_UNICODETEXT, (byte *)~s, 2 * s.GetLength());
}

String GetString(PasteClip& clip)
{
	if(clip.Accept("wtext")) {
		String s = ~clip;
		return WString((const wchar *)~s, wstrlen((const wchar *)~s)).ToString();
	}
	if(clip.IsAvailable("text"))
		return ~clip;
	return Null;
}

WString GetWString(PasteClip& clip)
{
	if(clip.Accept("wtext")) {
		String s = ~clip;
		return WString((const wchar *)~s, wstrlen((const wchar *)~s));
	}
	if(clip.IsAvailable("text"))
		return (~clip).ToWString();
	return Null;
}

String GetTextClip(const WString& text, const String& fmt)
{
	if(fmt == "text")
		return text.ToString();
	if(fmt == "wtext")
		return String((char *)~text, sizeof(wchar) * (text.GetLength() + 1));
	return Null;
}

String GetTextClip(const String& text, const String& fmt)
{
	if(fmt == "text")
		return text;
	if(fmt == "wtext")
		return GetTextClip(text.ToWString(), fmt);
	return Null;
}

String ReadClipboardText()
{
#ifdef PLATFORM_WINCE
	return ReadClipboardUnicodeText().ToString();
#else
	String s = ReadClipboard((const char *)CF_TEXT);
	return String(s, strlen(~s));
#endif
}

WString ReadClipboardUnicodeText()
{
	String s = ReadClipboard((const char *)CF_UNICODETEXT);
	return WString((const wchar *)~s, wstrlen((const wchar *)~s));
}

bool IsClipboardAvailable(const char *id)
{
	return ::IsClipboardFormatAvailable(GetClipboardFormatCode(id));
}

bool IsClipboardAvailableText()
{
	return IsClipboardAvailable((const char *)CF_TEXT);
}

String ClipFmtsImage()
{
	return "dib;" + ClipFmt<Image>();
}

bool AcceptImage(PasteClip& clip)
{
	return clip.Accept(ClipFmt<Image>()) || clip.Accept("dib");
}

Image GetImage(PasteClip& clip)
{
	Image m;
	if(Accept<Image>(clip)) {
		LoadFromString(m, ~clip);
		if(!m.IsEmpty())
			return m;
	}
	if(clip.Accept("dib")) {
		String data = ~clip;
		if(data.GetCount() < sizeof(BITMAPINFO)) return Null;
		BITMAPINFO *lpBI = 	(BITMAPINFO *)~data;
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
		ImageDraw   iw(hdr.biWidth, h);
		::StretchDIBits(iw.GetHandle(),
			0, 0, hdr.biWidth, h,
			0, 0, hdr.biWidth, h,
			bits, lpBI, DIB_RGB_COLORS, SRCCOPY);
		return iw;
	}
	return Null;
}

Image ReadClipboardImage()
{
	return GetImage(Ctrl::Clipboard());
}

String GetImageClip(const Image& img, const String& fmt)
{
	if(img.IsEmpty()) return Null;
	if(fmt == "dib") {
		BITMAPINFOHEADER header;
		Zero(header);
		header.biSize = sizeof(header);
		header.biWidth = img.GetWidth();
		header.biHeight = -img.GetHeight();
		header.biBitCount = 32;
		header.biPlanes = 1;
		header.biCompression = BI_RGB;
		StringBuffer b(sizeof(header) + 4 * img.GetLength());
		byte *p = (byte *)~b;
		memcpy(p, &header, sizeof(header));
		memcpy(p + sizeof(header), ~img, 4 * img.GetLength());
		return b;
	}
	if(fmt == ClipFmt<Image>())
		return StoreAsString(const_cast<Image&>(img));
	return Null;
}

void AppendClipboardImage(const Image& img)
{
	if(img.IsEmpty()) return;
	AppendClipboard(ClipFmt<Image>(), StoreAsString(const_cast<Image&>(img)));
	AppendClipboard("dib", GetImageClip(img, "dib"));
}

#endif

END_UPP_NAMESPACE
