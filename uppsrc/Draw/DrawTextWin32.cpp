#include "Draw.h"
#pragma hdrstop

#ifdef PLATFORM_WIN32

#define LLOG(x)     //  LOG(x)
#define LTIMING(x)

static VectorMap<String, dword>& sFontFace()
{
	static VectorMap<String, dword> s;
	return s;
}

int    Font::GetFaceCount()
{
	if(!Draw::sFini) Draw::InitFonts();
	return sFontFace().GetCount();
}

String Font::GetFaceName(int index) {
	if(!Draw::sFini) Draw::InitFonts();
	return index >= 0 && index < sFontFace().GetCount() ? sFontFace().GetKey(index)
	                                                    : Null;
}

dword Font::GetFaceInfo(int index) {
	if(!Draw::sFini) Draw::InitFonts();
	return index >= 0 && index < sFontFace().GetCount() ? sFontFace()[index]
	                                                    : 0;
}

int CALLBACK Draw::AddFace(const LOGFONT *logfont, const TEXTMETRIC *, dword type, LPARAM param)
{
	const char *facename = (const char *)param;
	if(facename && stricmp(logfont->lfFaceName, facename))
		return 1;

	dword typ = 0;
	if((logfont->lfPitchAndFamily & 3) == FIXED_PITCH)
		typ |= Font::FIXEDPITCH;
	if(type & TRUETYPE_FONTTYPE)
		typ |= Font::SCALEABLE;
	if(logfont->lfCharSet == SYMBOL_CHARSET)
		typ |= Font::SYMBOLTYPE;

	VectorMap<String, dword>& face = sFontFace();
	if(facename) {
		face.Add(logfont->lfFaceName) = typ;
		return 0;
	}
	face.GetAdd(logfont->lfFaceName, 0) |= typ;
	return 1;
}

int Draw::EnumFace(HDC hdc, const char *face)
{
	return EnumFontFamilies(hdc, face, AddFace, (LPARAM)face);
}

void Draw::ForceFace(HDC hdc, const char *face, const char *aface)
{
	if(!aface)
		aface = "Arial";
	if(EnumFace(hdc, face) && (aface == NULL || EnumFace(hdc, aface)))
		Panic("Missing font " + String(face));
}

const char *Draw::FontScreenSans  = "Arial";
const char *Draw::FontScreenSerif = "Times New Roman";
const char *Draw::FontScreenFixed = "Courier New";
const char *Draw::FontRoman       = "Times New Roman" ;
const char *Draw::FontArial       = "Arial";
const char *Draw::FontCourier     = "Courier New";
const char *Draw::FontSymbol      = "Symbol";
const char *Draw::FontWingdings   = "WingDings";
const char *Draw::FontTahoma      = "Tahoma";

void Draw::InitPlatformFonts() {
	HDC hdc = CreateIC("DISPLAY", NULL, NULL, NULL);
	ForceFace(hdc, FontScreenSans, NULL);
	ForceFace(hdc, FontScreenSerif, NULL);
	ForceFace(hdc, FontScreenSans, NULL);
	ForceFace(hdc, FontScreenFixed, NULL);
	ForceFace(hdc, FontRoman, NULL);
	ForceFace(hdc, FontArial, NULL);
	ForceFace(hdc, FontCourier, NULL);
	ForceFace(hdc, FontSymbol, NULL);
	ForceFace(hdc, FontWingdings, FontArial);
	ForceFace(hdc, FontTahoma, FontArial);
	EnumFace(hdc, NULL);
	DeleteDC(hdc);
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(ncm);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
	SetStdFont(Font(Font::FindFaceNameIndex(ncm.lfMenuFont.lfFaceName), abs(ncm.lfMenuFont.lfHeight)));
}

FontInfo::Data::Data()
{
	count = 1;
	hfont = NULL;
	for(int i = 0; i < 256; i++)
		width[i] = NULL;
}

FontInfo::Data::~Data()
{
	if(hfont)
		DeleteObject(hfont);
	for(int i = 0; i < 256; i++)
		if(width[i]) delete[] width[i];
}

void FontInfo::Data::GetMetrics(int page, CharMetrics *t)
{
	HDC hdc = ScreenInfo().GetHandle();
	HFONT ohfont = (HFONT) ::SelectObject(hdc, hfont);
	page <<= 8;
	bool abca = false, abcw = false;
	ABC abc[256];
	if(scaleable && !(abcw = ::GetCharABCWidthsW(hdc, page, page + 255, abc)))
		abca = ::GetCharABCWidthsA(hdc, 0, 255, abc);
	if(abcw)
	{
		for(ABC *s = abc, *lim = abc + 256; s < lim; s++, t++)
		{
			t->width = s->abcA + s->abcB + s->abcC;
			t->lspc = s->abcA;
			t->rspc = s->abcC;
		}
	}
	else
	{
		int wb[256];
		::GetCharWidthW(hdc, page, page + 255, wb);
		for(int *s = wb, *lim = wb + 256; s < lim; s++, t++)
		{
			t->width = *s - overhang;
			if(abca)
			{
				ABC aa = abc[(byte)ToAscii(page++)];
				t->lspc = aa.abcA;
				t->rspc = aa.abcC;
			}
			else
				t->lspc = t->rspc = 0;
		}
	}
	::SelectObject(hdc, ohfont);
}

FontInfo Draw::Acquire(Font font, HDC hdc, int angle, int device)
{
	if(IsNull(font))
		font = StdFont();
	if(font.GetFace() == 0)
		font.Face(AStdFont.GetFace());
	if(font.GetHeight() == 0)
		font.Height(AStdFont.GetHeight());
//	if(font.GetFace() >= sFontFace().GetCount())
//		font.SetFace(Font::ARIAL);
	FontInfo::Data *f, *fh;
	f = fh = GetFontHash((font.GetHashValue() ^ angle ^ (device << 9)) % FONTHASH);
	LLOG("Acquire " << font);
	for(;;) {
		f = f->GetNext(HASH);
		if(f == fh) break;
		if(f->font == font && f->angle == angle && f->device == device)
		{
			LLOG("Reusing " << f->font << " count:" << f->count);
			if(f->InList(LRU)) {
				f->Unlink(LRU);
				FontCached--;
				LLOG("Removing from cache " << f->font << " count:" << f->count <<
				     " cached:" << FontCached);
			}
			f->count++;
			return f;
		}
	}
	LLOG("New " << font);
	LTIMING("Acquire New");
	f = fh->InsertNext(HASH);
	f->font = font;
	f->angle = angle;
	f->device = device;

	byte chrset;
	if((sFontFace()[font.GetFace()] & Font::SCALEABLE) == 0)
		chrset = DEFAULT_CHARSET;
	else
	if(sFontFace()[font.GetFace()] & Font::SYMBOLTYPE)
		chrset = SYMBOL_CHARSET;
	else
		chrset = DEFAULT_CHARSET;
	f->hfont = CreateFont(font.GetHeight() ? -abs(font.GetHeight()) : -12,
		                  font.GetWidth(), angle, angle, font.IsBold() ? FW_BOLD : FW_NORMAL,
		                  font.IsItalic(), font.IsUnderline(), font.IsStrikeout(),
						  chrset,
						  font.IsTrueTypeOnly() ? OUT_TT_ONLY_PRECIS : OUT_DEFAULT_PRECIS,
						  CLIP_DEFAULT_PRECIS,
						  font.IsNonAntiAliased() ? NONANTIALIASED_QUALITY
						                          : DEFAULT_QUALITY,
						  DEFAULT_PITCH|FF_DONTCARE,
						  font.GetFaceName());
	ASSERT(f->hfont);
	TEXTMETRIC tm;
	HFONT hfont = (HFONT) ::SelectObject(hdc, f->hfont);
	::GetTextMetrics(hdc, &tm);
	f->ascent = tm.tmAscent;
	f->descent = tm.tmDescent;
	f->external = tm.tmExternalLeading;
	f->internal = tm.tmInternalLeading;
	f->height = f->ascent + f->descent;
	f->lineheight = f->height + f->external;
	f->overhang = tm.tmOverhang;
	f->avewidth = tm.tmAveCharWidth;
	f->maxwidth = tm.tmMaxCharWidth;
	f->firstchar = tm.tmFirstChar;
	f->charcount = tm.tmLastChar - tm.tmFirstChar + 1;
	f->default_char = tm.tmDefaultChar;
	f->fixedpitch = (tm.tmPitchAndFamily & TMPF_FIXED_PITCH) == 0;
	f->scaleable = tm.tmPitchAndFamily & TMPF_TRUETYPE;
	f->kerning.Clear();
	if(f->scaleable) {
		ABC abc;
		GetCharABCWidths(hdc, 'o', 'o', &abc);
		f->spacebefore = abc.abcA;
		f->spaceafter = abc.abcC;
	}
	else
		f->spacebefore = f->spaceafter = 0;
	int n = ::GetKerningPairs(hdc, 0, NULL);
	if(n) {
		Buffer<KERNINGPAIR> kp(n);
		::GetKerningPairs(hdc, n, kp);
		const KERNINGPAIR *p = kp;
		while(n--) {
			f->kerning.Add(MAKEWORD(p->wFirst, p->wSecond), p->iKernAmount);
			p++;
		}
	}
	::SelectObject(hdc, hfont);
	f->offset = Size(0, f->ascent);
	if(angle) {
		FontInfo font0 = Acquire(font, hdc, 0, device);
		double sina, cosa;
		SinCos(angle, sina, cosa);
		f->offset.cx = fround(font0.GetAscent() * sina);
		f->offset.cy = fround(font0.GetAscent() * cosa);
	}
	return FontInfo(f);
}

void Draw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink,
                    int n, const int *dx) {
	COLORREF cr = GetColor(ink);
	if(cr != lastTextColor) {
		LLOG("Setting text color: " << ink);
		::SetTextColor(handle, lastTextColor = cr);
	}
	if(angle) {
		SetFont(font, angle);
		::ExtTextOutW(handle, x + lastFont.ptr->offset.cx, y + lastFont.ptr->offset.cy,
		              0, NULL, (const WCHAR *)text, n, dx);
	}
	else {
		SetFont(font);
		::ExtTextOutW(handle, x, y + lastFont.GetAscent(), 0, NULL, (const WCHAR *)text,
		              n, dx);
	}
}

#endif
