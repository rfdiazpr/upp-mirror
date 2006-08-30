#include "Draw.h"

#ifdef PLATFORM_XFT

#define LLOG(x)    // LOG(x)
#define LTIMING(x) // RTIMING(x)

struct XFTFontFaceInfo {
	String      name;
	bool        fixed:1;
	bool        scaleable:1;
	bool        compose:1;

	XFTFontFaceInfo()
	{
		fixed = scaleable = false;
	}
};

ArrayMap<String, XFTFontFaceInfo>& XFTFontFace()
{
	static ArrayMap<String, XFTFontFaceInfo> x;
	return x;
}

FontInfo::Data::Data()
{
	count = 1;
	for(int i = 0; i < 256; i++)
		width[i] = NULL;
	xftfont = NULL;
	default_width = NULL;
}

FontInfo::Data::~Data()
{
	if(xftfont)
		XftFontClose(Xdisplay, xftfont);
	for(int i = 0; i < 256; i++)
		if(width[i] && width[i] != default_width) delete[] width[i];
	if(default_width)
		delete[] default_width;
}

void FontInfo::Data::GetMetrics(int page, CharMetrics *t)
{
	LTIMING("GetMetrics");
	LLOG("GetMetrics " << font << " char:" << page * 256 << ", 256");
	if(xftfont) {
		wchar h[1];//TODO Optimize!!!
		for(int i = 0; i < 256; i++) {
			LTIMING("XftTextExtents16");
			*h = (page << 8) + i;
			XGlyphInfo info;
			XftTextExtents16(Xdisplay, xftfont, h, 1, &info);
			t[i].width = info.xOff;
			t[i].lspc = -info.x;
			t[i].rspc = info.xOff - info.width + info.x;
		}
		return;
	}
}

char *basic_fonts[] = {
	"sans-serif",
	"serif",
	"sans-serif",
	"monospace",
	"serif",
	"sans-serif",
	"monospace",
};

static bool sCheckComposed(const char *face)
{
	XftFont *xftfont = XftFontOpen(Xdisplay, Xscreenno,
	                               XFT_FAMILY, XftTypeString, (char *)face,
	                               XFT_PIXEL_SIZE, XftTypeInteger, 20,
	                               (void *)0);
	int n = 0;
	for(int c = 0; c < 128; c++)
		if(!XftCharExists(Xdisplay, xftfont, c + 256))
			n++;
	XftFontClose(Xdisplay, xftfont);
	return n > 10;
}

void Draw::InitPlatformFonts()
{
	for(int i = 0; i < __countof(basic_fonts); i++) {
		XFTFontFaceInfo& f = XFTFontFace().Add(basic_fonts[i]);
		f.name = basic_fonts[i];
		f.scaleable = true;
		f.fixed = i == 3 || i == 6;
		f.compose = sCheckComposed(basic_fonts[i]);
	}
	FcFontSet *fs = XftListFonts(Xdisplay, Xscreenno, (void *)0, XFT_FAMILY, XFT_SPACING,
	                             XFT_SCALABLE, (void *)0);
	for(int i = 0; i < fs->nfont; i++) {
		FcChar8 *family = NULL;
		if(FcPatternGetString(fs->fonts[i], FC_FAMILY, 0, &family) == 0 && family) {
			XFTFontFaceInfo& f = XFTFontFace().GetAdd((char *)family);
			int spacing;
			if(FcPatternGetInteger(fs->fonts[i], FC_SPACING, 0, &spacing) == 0 && spacing == XFT_MONO)
				f.fixed = true;
			FcBool scaleable;
			if(FcPatternGetBool(fs->fonts[i], FC_SCALABLE, 0, &scaleable) == 0 && scaleable)
				f.scaleable = true;
			f.compose = sCheckComposed((char *)family);
		}
	}
	static const char *replacements[] = {
		"Bitstream Vera Sans",
		"Times New Roman;Bitstream Vera Serif",
		"Bitstream Vera Sans;Arial",
		"Courier New;Bitstream Vera Sans Mono",
		"Times New Roman;Bitstream Vera Serif",
		"Arial;Bitstream Vera Sans",
		"Courier New;Bitstream Vera Sans Mono",
	};
	for(int i = 0; i < 7; i++) {
		Vector<String> f = Split(replacements[i], ';');
		for(int q = 0; q < f.GetCount(); q++)
			if(XFTFontFace().Find(f[q]) >= 0) {
				basic_fonts[i] = PermanentCopy(f[q]);
				XFTFontFace()[i].compose = sCheckComposed(basic_fonts[i]);
				break;
			}
	}
	FcFontSetDestroy(fs);
}

int    Font::GetFaceCount()
{
	if(!Draw::sFini) Draw::InitFonts();
	return XFTFontFace().GetCount();
}

String Font::GetFaceName(int index)
{
	if(!Draw::sFini) Draw::InitFonts();
	return index >= 0 && index < XFTFontFace().GetCount() ? XFTFontFace().GetKey(index)
	                                                      : Null;
}

dword Font::GetFaceInfo(int index) {
	if(!Draw::sFini) Draw::InitFonts();
	dword w = 0;
	if(index >= 0 && index < XFTFontFace().GetCount()) {
		XFTFontFaceInfo& fi = XFTFontFace()[index];
		if(fi.fixed)
			w |= FIXEDPITCH;
		if(fi.scaleable)
			w |= SCALEABLE;
		if(fi.compose)
			w |= COMPOSED;
	}
	return w;
}

XftFont *Draw::CreateXftFont(Font font, int angle)
{
	LTIMING("CreateXftFont");
	XftFont *xftfont;
	double sina, cosa;
	int hg = abs(font.GetHeight());
	if(hg == 0) hg = 10;
	int i = font.GetFace();
	if(i < 0 || i >= XFTFontFace().GetCount())
		i = 0;
	const char *face = i < 7 ? basic_fonts[i] : ~XFTFontFace().GetKey(i);
	DUMP(i);
	DUMP(face);
	if(angle) {
		XftMatrix mx;
		SinCos(angle, sina, cosa);
		mx.xx = cosa;
		mx.xy = -sina;
		mx.yx = sina;
		mx.yy = cosa;
		xftfont = XftFontOpen(Xdisplay, Xscreenno,
		                      XFT_FAMILY, XftTypeString, face,
		                      XFT_SLANT, XftTypeInteger, int(font.IsItalic() ? 110 : 0),
		                      XFT_PIXEL_SIZE, XftTypeInteger, hg,
		                      XFT_MATRIX, XftTypeMatrix, &mx,
		                      XFT_WEIGHT, XftTypeInteger, int(font.IsBold() ? 200 : 0),
		                      XFT_ANTIALIAS, XftTypeBool, FcBool(!font.IsNonAntiAliased()),
		                      XFT_MINSPACE, XftTypeBool, (FcBool)1,
		                      (void *)0);
	}
	else
		xftfont = XftFontOpen(Xdisplay, Xscreenno,
		                      XFT_FAMILY, XftTypeString, face,
		                      XFT_SLANT, XftTypeInteger, int(font.IsItalic() ? 110 : 0),
		                      XFT_PIXEL_SIZE, XftTypeInteger, hg,
		                      XFT_WEIGHT, XftTypeInteger, int(font.IsBold() ? 200 : 0),
		                      XFT_ANTIALIAS, XftTypeBool, FcBool(!font.IsNonAntiAliased()),
		                      XFT_MINSPACE, XftTypeBool, (FcBool)1,
	                          (void *)0);
	return xftfont;
}

FontInfo Draw::Acquire(Font font, int angle, int device)
{
	LTIMING("Acquire");
	if(IsNull(font))
		font = StdFont();
	if(font.GetFace() == 0)
		font.Face(AStdFont.GetFace());
	if(font.GetHeight() == 0)
		font.Height(AStdFont.GetHeight());
	FontInfo::Data *f, *fh;
	f = fh = GetFontHash((font.GetHashValue() ^ angle ^ (device << 9)) % (int)FONTHASH);
	LLOG("Acquire " << font << " device: " << device);
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
	int hg = abs(font.GetHeight());
	if(hg == 0) hg = 10;
	f->xftfont = CreateXftFont(font, angle);
	f->filename = NULL;
	f->ascent = (int16)f->xftfont->ascent;
	f->descent = (int16)f->xftfont->descent;
	f->height = f->ascent + f->descent;
	f->lineheight = (int16)f->xftfont->height;
	f->external = 0;
	f->internal = 0;
	f->overhang = 0;
	f->maxwidth = (int16)f->xftfont->max_advance_width;
	f->avewidth = f->maxwidth;
	f->default_char = '?';
	f->fixedpitch = font.GetFaceInfo() & Font::FIXEDPITCH;
	unsigned long a;
	f->underline_thickness = max(hg / 20, 1);
	f->underline_position = max(hg / 15, int(f->descent > 0));
	if(angle) {
		SinCos(angle, f->sina, f->cosa);
		f->offset.cx = fround(f->ascent * f->sina);
		f->offset.cy = fround(f->ascent * f->cosa);
	}
	FontInfo fi = FontInfo(f);
	fi.GetPage(0);
	return fi;
}

String FontInfo::GetFileName() const
{
	if(IsNull(ptr->filename)) {
		char *fn = NULL;
		XftPatternGetString(ptr->xftfont->pattern, XFT_FILE, 0, &fn);
		if(fn)
			ptr->filename = fn;
	}
	return ptr->filename;
}

void Draw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
                      Color ink, int n, const int *dx) {
	LTIMING("DrawText");
	LLOG("DrawText " << ToUtf8(text) << " color:" << ink << " font:" << font);
	//TODO - X11 seems to crash when displaying too long strings (?)
	int ox = x + actual_offset.x;
	int oy = y + actual_offset.y;
	SetForeground(ink);
	SetFont(font, angle);
	const FontInfo::Data *fd = lastFont.ptr;
	XftColor c;
	c.color.red = ink.GetR() << 8;
	c.color.green = ink.GetG() << 8;
	c.color.blue = ink.GetB() << 8;
	c.color.alpha = 0xffff;
	c.pixel = GetXPixel(ink.GetR(), ink.GetG(), ink.GetB());
	if(angle) {
		int xpos = 0;
		int xpp = 0;
		int pfi = -1;
		for(int i = 0; i < n; i++) {
			wchar h = text[i];
			XftDrawString16(xftdraw, &c, fd->xftfont,
			                int(ox + xpos * fd->cosa + fd->offset.cx),
			                int(oy - xpos * fd->sina + fd->offset.cy),
			                (FcChar16 *)&h, 1);
			xpos += dx ? dx[i] : lastFont[text[i]];
		}
		if(font.IsUnderline() || font.IsStrikeout()) {
			xpos = xpp + lastFont[(byte)text[n - 1]];
			if(font.IsUnderline())
				DrawLine(
					int(x + fd->offset.cx + fd->underline_position * fd->sina),
					int(y + fd->offset.cy + fd->underline_position * fd->cosa),
					int(x + xpos * fd->cosa + fd->offset.cx + fd->underline_position * fd->sina),
					int(y - xpos * fd->sina + fd->offset.cy + fd->underline_position * fd->cosa),
					fd->underline_thickness,
					ink
				);
			if(font.IsStrikeout()) {
				int p = 2 * fd->ascent / 3;
				DrawLine(
					int(x + p * fd->sina),
					int(y + p * fd->cosa),
					int(x + xpos * fd->cosa + p * fd->sina),
					int(y - xpos * fd->sina + p * fd->cosa),
					fd->underline_thickness,
					ink
				);
			}
		}
	}
	else {
		if(dx) {
			int xpos = ox;
			Buffer<XftCharSpec> ch(n);
			for(int i = 0; i < n; i++) {
				ch[i].ucs4 = text[i];
				ch[i].x = xpos;
				ch[i].y = oy + fd->ascent;
				xpos += dx[i];
			}
			XftDrawCharSpec(xftdraw, &c, fd->xftfont, ch, n);
		}
		else
			XftDrawString16(xftdraw, &c, fd->xftfont, ox, oy + fd->ascent,
			                (FcChar16 *)text, n);
		LLOG("XftColor: r=" << c.color.red << ", g=" << c.color.green << ", b=" << c.color.blue
			<< ", alpha=" << c.color.alpha << ", pixel=" << FormatIntHex(c.pixel));
		if(font.IsUnderline() || font.IsStrikeout()) {
			int cx;
			if(dx && n > 0) {
				cx = 0;
				Sum(cx, dx, dx + n - 1);
				cx += lastFont[text[n - 1]];
			}
			else
				cx = GetTextSize(text, font, n).cx;
			if(font.IsUnderline())
				DrawRect(x, y + lastFont.GetAscent() + lastFont.ptr->underline_position,
				         cx, lastFont.ptr->underline_thickness, ink);
			if(font.IsStrikeout())
				DrawRect(x, y + 2 * lastFont.GetAscent() / 3,
				         cx, lastFont.ptr->underline_thickness, ink);
		}
	}
}

#endif
