#include "Draw.h"
#pragma hdrstop

#if defined(PLATFORM_WIN32) || defined(PLATFORM_X11)

#define LLOG(x)    // LOG(x)
#define LTIMING(x) TIMING(x)

bool Draw::sFini;

void Draw::SinCos(int angle, double& sina, double& cosa)
{
	angle = angle % 3600;
	switch(angle) {
	case    0: sina =  0; cosa =  1; break;
	case  900: sina =  1; cosa =  0; break;
	case 1800: sina =  0; cosa = -1; break;
	case 2700: sina = -1; cosa =  0; break;
	default:
		double a = angle * M_PI / 1800.0;
		sina = sin(a);
		cosa = cos(a);
		break;
	}
}

RichValue<Font>::Registrator MK__s;

bool Draw::StdFontSizeSet;
Size Draw::StdFontSize;
Font Draw::AStdFont;

void Draw::InitFonts()
{
	if(sFini) return;
	INTERLOCKED {
		if(sFini) return;
		sFini = true;
		GetFontHash(0);
		GetFontLru();
		InitPlatformFonts();
	}
}

int FontFilter(int c)
{
	return c >= 'a' && c <= 'z' || c >= '0' && c <= '9' ? c : c >= 'A' && c <= 'Z' ? ToLower(c) : 0;
}

int  Font::FindFaceNameIndex(const char *name) {
	if(!Draw::sFini) Draw::InitFonts();
	for(int i = 0; i < GetFaceCount(); i++)
		if(GetFaceName(i) == name)
			return i;
	String n = Filter(name, FontFilter);
	for(int i = 0; i < GetFaceCount(); i++)
		if(Filter(GetFaceName(i), FontFilter) == n)
			return i;
	return 0;
}

void Draw::SetStdFont(Font font)
{
	AStdFont = font;
	StdFontSizeSet = false;
}

Size Draw::GetStdFontSize()
{
	if(!StdFontSizeSet) {
		StdFontSizeSet = true;
		FontInfo fi = ScreenInfo().GetFontInfo(AStdFont);
		StdFontSize = Size(fi.GetAveWidth(), fi.GetHeight());
	}
	return StdFontSize;
}

String Font::GetFaceName() const {
	if(IsNull()) return String();
	if(GetFace() == 0)
		return "STDFONT";
	return GetFaceName(GetFace());
}

dword Font::GetFaceInfo() const {
	if(IsNull()) return 0;
	return GetFaceInfo(GetFace());
}

Font& Font::FaceName(const String& name) {
	int n = FindFaceNameIndex(name);
	Face(n < 0 ? 0xffff : n);
	return *this;
}

void Font::Serialize(Stream& s) {
	int version = 1;
	s / version;
	if(version >= 1) {
		int f = GetFace();
		if(f > COURIER)
			f = -1;
		s / f;
		String name;
		if(f < 0) {
			name = GetFaceName();
			s % name;
		}
		if(s.IsLoading())
			if(f >= 0)
				Face(f);
			else
				FaceName(name);
	}
	else {
		String name = GetFaceName();
		s % name;
		if(s.IsLoading()) {
			FaceName(name);
			if(IsNull())
				Face(COURIER);
		}
	}
	s % flags % height % width;
}

template<>
String AsString(const Font& f) {
	if(IsNull(f)) return "<null>";
	String s = "<" + f.GetFaceName() + Format(":%d", f.GetHeight());
	if(f.IsBold())
		s += " Bold";
	if(f.IsItalic())
		s += " Italic";
	if(f.IsUnderline())
		s += " Underline";
	if(f.IsStrikeout())
		s += " Strikeout";
	return s + '>';
}

int Draw::FontCacheMax = 32;
int Draw::FontCached;

void Draw::FreeFonts() {
	FontCacheMax = FontCached = 0;
	sFini = false;
	for(int i = 0; i < FONTHASH; i++)
		GetFontHash(i)->DeleteList(HASH);
}

FontInfo::FontInfo(const FontInfo& f)
{
	Retain(f);
}

FontInfo& FontInfo::operator=(const FontInfo& f)
{
	Release();
	Retain(f);
	return *this;
}

//# Pretty ugly code....
FontInfo::Data *Draw::GetFontHash(int i) {
	static byte buff[FONTHASH * sizeof(FontLink)];
	static FontLink *fonthash;
	if(!fonthash) {
		fonthash = (FontLink *)buff;
		for(int i = 0; i < FONTHASH; i++)
			fonthash[i].LinkSelfAll();
	}
	ASSERT(i >= 0 && i < FONTHASH);
	return (FontInfo::Data *)&fonthash[i];
}

//# Pretty ugly code....
FontInfo::Data *Draw::GetFontLru() {
	static byte buff[sizeof(FontLink)];
	static FontLink *fontlru;
	if(!fontlru)
		fontlru = new(buff) FontLink;
	return (FontInfo::Data *)fontlru;
}

bool FontInfo::IsEqual(byte _charset, Font f, int angle, int device) const
{
	return ptr && ptr->font == f && ptr->angle == angle && ptr->device == device &&
	       charset == _charset;
}

FontInfo::CharMetrics *FontInfo::GetPage(int page) const
{
	CharMetrics *& cm = ptr->width[page];
	if(!cm) {
		cm = new CharMetrics[256];
		ptr->GetMetrics(page, cm);
	}
	return cm;
}

int FontInfo::GetWidth(int c) const {
	if(c < 0) c = (byte)c;
	if(charset != CHARSET_UNICODE)
		c = ToUnicode(c, charset);
	return c < 65536 ? GetPage(c >> 8)[c & 0xff].width : 0;
}

int FontInfo::GetLeftSpace(int c) const {
	if(c < 0) c = (byte)c;
	if(charset != CHARSET_UNICODE)
		c = ToUnicode(c, charset);
	return c < 65536 ? GetPage(c >> 8)[c & 0xff].lspc : 0;
}

int FontInfo::GetRightSpace(int c) const {
	if(c < 0) c = (byte)c;
	if(charset != CHARSET_UNICODE)
		c = ToUnicode(c, charset);
	return c < 65536 ? GetPage(c >> 8)[c & 0xff].rspc : 0;
}

void Draw::Release(FontInfo::Data *font) {
	ASSERT(font->count > 0);
	LLOG("Release " << font->font << " count:" << font->count);
	if(--font->count == 0) {
		if(FontCacheMax == 0) {
			delete font;
			return;
		}
		FontInfo::Data *lru = GetFontLru();
		font->LinkAfter(lru, LRU);
		FontCached++;
		LLOG("Placed to cache " << font->font << " cached:" << FontCached);
		while(FontCached > FontCacheMax) {
			ASSERT(lru->GetPrev(LRU) != lru);
			FontCached--;
			LLOG("Deleting from cache " << lru->GetPrev(LRU)->font <<
				        " cached: " << FontCached);
			delete lru->GetPrev(LRU);
		}
	}
}

void FontInfo::Release()
{
	if(ptr) Draw::Release(ptr);
}

FontInfo Draw::GetFontInfo(byte charset, Font font) {
	if(charset == CHARSET_DEFAULT)
		charset = GetDefaultCharset();
	if(lastFont.IsEqual(charset, font, 0, device))
		return lastFont;
#ifdef PLATFORM_WIN32
	HDC hdc = GetHandle();
	FontInfo fi = Acquire(font, hdc ? hdc : ScreenInfo().GetHandle(), 0, device);
#endif
#ifdef PLATFORM_X11
	FontInfo fi = Acquire(font, 0, device);
#endif
	fi.charset = charset;
	return fi;
}

FontInfo Draw::GetFontInfo(Font font)
{
	return GetFontInfo(CHARSET_DEFAULT, font);
}

FontInfo Draw::GetFontInfoW(Font font)
{
	return GetFontInfo(CHARSET_UNICODE, font);
}

void Draw::SetFont(Font font, int angle) {
	LLOG("Set font: " << font << " face: " << font.GetFaceName());
	if(lastFont && lastFont.IsEqual(CHARSET_UNICODE, font, angle, device))
		return;
#ifdef PLATFORM_WIN32
	lastFont = Acquire(font, GetHandle(), angle, device);
	HFONT h = (HFONT) SelectObject(handle, lastFont.ptr->hfont);
	if(!orgFont) orgFont = h;
#endif
#ifdef PLATFORM_X11
	lastFont = Acquire(font, angle, device);
#endif
}

Size Draw::GetTextSize(const wchar *text, Font font, int n) {
	FontInfo fi = GetFontInfoW(font);
	if(n < 0)
		n = wstrlen(text);
	Size sz;
	sz.cx = 0;
	const wchar *wtext = (const wchar *)text;
	while(n > 0) {
		sz.cx += fi[*wtext++];
		n--;
	}
	sz.cy = fi.GetHeight();
	return sz;
}

WString TextUnicode(const char *s, int n, byte cs, Font font)
{
	if(n < 0)
		n = strlen(s);
	if(font.GetFace() == Font::SYMBOL) {
		WStringBuffer b(n);
		wchar *t = b;
		while(n > 0) {
			*t++ = *s++;
			n--;
		}
		return b;
	}
	else
		return ToUnicode(s, n, cs);
}

void Draw::DrawText(int x, int y, int angle, const wchar *text, Font font,
		            Color ink, int n, const int *dx)
{
	if(IsNull(ink)) return;
	if(n < 0)
		n = wstrlen(text);
	DrawTextOp(x, y, angle, text, font, ink, n, dx);
}

// ----------------------------

void Draw::DrawText(int x, int y, const wchar *text, Font font,
                    Color ink, int n, const int *dx)
{
	DrawText(x, y, 0, text, font, ink, n, dx);
}

// ---------------------------

Size Draw::GetTextSize(const WString& text, Font font)
{
	return GetTextSize(text, font, text.GetLength());
}

void Draw::DrawText(int x, int y, int angle, const WString& text, Font font,
                    Color ink, const int *dx)
{
	DrawText(x, y, angle, ~text, font, ink, text.GetLength(), dx);
}

void Draw::DrawText(int x, int y, const WString& text, Font font, Color ink, const int *dx)
{
	DrawText(x, y, 0, text, font, ink, dx);
}

// ---------------------------

Size Draw::GetTextSize(const char *text, byte charset, Font font, int n)
{
	return GetTextSize(TextUnicode(text, n, charset, font), font);
}

void Draw::DrawText(int x, int y, int angle, const char *text, byte charset, Font font,
                    Color ink, int n, const int *dx)
{
	DrawText(x, y, angle, TextUnicode(text, n, charset, font), font, ink, dx);
}

void Draw::DrawText(int x, int y, const char *text, byte charset, Font font,
                    Color ink, int n, const int *dx)
{
	DrawText(x, y, 0, text, charset, font, ink, n, dx);
}

// ---------------------------

Size Draw::GetTextSize(const char *text, Font font, int n)
{
	return GetTextSize(text, CHARSET_DEFAULT, font, n);
}

void Draw::DrawText(int x, int y, int angle, const char *text,
                    Font font, Color ink, int n, const int *dx)
{
	DrawText(x, y, angle, text, CHARSET_DEFAULT, font, ink, n, dx);
}

void Draw::DrawText(int x, int y, const char *text, Font font,
                    Color ink, int n, const int *dx)
{
	DrawText(x, y, text, CHARSET_DEFAULT, font, ink, n, dx);
}

// ---------------------------

Size Draw::GetTextSize(const String& text, Font font)
{
	return GetTextSize(text, font, text.GetLength());
}

void Draw::DrawText(int x, int y, int angle, const String& text, Font font,
                    Color ink, const int *dx)
{
	DrawText(x, y, angle, text, font, ink, text.GetLength(), dx);
}

void Draw::DrawText(int x, int y, const String& text, Font font, Color ink, const int *dx)
{
	WString h = TextUnicode(text, text.GetLength(), CHARSET_DEFAULT, font);
	DrawText(x, y, h, font, ink, h.GetLength(), dx);
}

#endif
