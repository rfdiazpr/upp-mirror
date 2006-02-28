#include "Draw.h"

#ifdef PLATFORM_XLFD

#define LLOG(x)    // LOG(x)
#define LTIMING(x) // RTIMING(x)


enum {
	XLFD_ZERO, XLFD_FOUNDRY, XLFD_FAMILY, XLFD_WEIGHT, XLFD_SLANT, XLFD_SET_WIDTH,
	XLFD_NOTHING, XLFD_PIXELS, XLFD_POINTS, XLFD_HRES, XLFD_VRES, XLFD_SPACING,
	XLFD_AVG_WIDTH, XLFD_CHARSET1, XLFD_CHARSET2
};

struct X11FontFaceInfo {
	String      name;
	String      style[4];
	bool        fixed:1;
	bool        setwidth:1;
	word        encoding;
	int         scaleable;

	void Dump();

	X11FontFaceInfo()
	{
		fixed = false; encoding = 0;
		scaleable = -1;
	}
};

ArrayMap<String, X11FontFaceInfo>& X11FontFace()
{
	static ArrayMap<String, X11FontFaceInfo> x;
	return x;
}

FontInfo::Data::Data()
{
	count = 1;
	for(int i = 0; i < 256; i++)
		width[i] = NULL;
	for(int i = 0; i < 16; i++)
		xfont[i] = 0;
}

FontInfo::Data::~Data()
{
	for(int i = 0; i < 16; i++)
		if(xfont[i])
			XUnloadFont(Xdisplay, xfont[i]);
	for(int i = 0; i < 256; i++)
		if(width[i]) delete[] width[i];
}

static bool sTestXLFD(const char *xlfd)
{
	int fontCount;
	char **fontList = XListFonts(Xdisplay, xlfd, 1, &fontCount);
	XFreeFontNames(fontList);
	return fontCount;
}

byte sChrSet(int i)
{
	if(i == 15)
		return CHARSET_KOI8_R;
	if(i >= 1 && i <= 10)
		return i;
	return i - 2;
}

bool X11IsScaleableFace(int facei)
{
	X11FontFaceInfo& fi = X11FontFace()[facei];
	if(fi.scaleable < 0) {
		fi.scaleable = 1;
		if(fi.encoding)
			for(int q = 1; q < 16; q++)
				if((1 << q) & fi.encoding) {
					int cs = sChrSet(q);
					if(!sTestXLFD(Draw::MakeXLFD(cs, Font(facei, 300), 300, 300))) {
						fi.scaleable = 0;
						break;
					}
				}
		else
			if(!sTestXLFD(Draw::MakeXLFD(0, Font(facei, 300), 300, 300)))
				fi.scaleable = 0;
	}
	return fi.scaleable;
}

void FontInfo::Data::CreateFont(int i, int cs)
{
	LLOG("Creating font " << i << " charset " << CharsetName(cs));
	const X11FontFaceInfo& fi = X11FontFace()[font.GetFace()];
	int hg = abs(font.GetHeight());
	if(hg == 0) hg = 10;
	String xlfd;
	scaleable = X11IsScaleableFace(font.GetFace());
	xlfd = Draw::MakeXLFD(cs, font, hg, angle);
	LLOG("Trying XLFD " << xlfd);
	if(!sTestXLFD(xlfd)) {
		LLOG("Non-scaleable, trying to get right height:");
		int fontCount;
		char **fontList = XListFonts(Xdisplay, Draw::MakeXLFD(cs, font, Null),
		                                                      32767, &fontCount);
		xlfd = "*";
		if(fontCount) {
			int fh = -1;
			int fhh = INT_MAX;
			for(int i = 0; i < fontCount; i++) {
				LLOG(fontList[i]);
				int h2 = atoi(Split(fontList[i], '-', false)[XLFD_PIXELS]);
				if(h2 > fh && h2 <= hg)
					fh = h2;
				if(h2 < fhh && h2 >= hg)
					fhh = h2;
			}
			if(fh < 0 && fhh != INT_MAX)
				fh = fhh;
			if(fh > 0)
				xlfd = Draw::MakeXLFD(cs, font, fh);
		}
		XFreeFontNames(fontList);
		scaleable = false;
	}
	LLOG("XLFD: " << xlfd);
	xfont[i] = XLoadFont(Xdisplay, xlfd);
	XFontStruct *xf = XQueryFont(Xdisplay, xfont[i]);
	ascent = (int16)xf->ascent;
	descent = (int16)xf->descent;
	height = ascent + descent;
	lineheight = height;
	external = 0;
	internal = 0;
	overhang = 0;
	maxwidth = xf->max_bounds.width;
	avewidth = maxwidth;
	default_char = xf->default_char;
	fixedpitch = font.GetFaceInfo() & Font::FIXEDPITCH;
	unsigned long a;
	underline_position = XGetFontProperty(xf, XA_UNDERLINE_POSITION, &a)
	                     ? a : hg / 8;
	underline_thickness = XGetFontProperty(xf, XA_UNDERLINE_THICKNESS, &a)
	                      ? a : max(hg / 20, 1);
	XFreeFontInfo(NULL, xf, 1);
	if(angle) {
		Draw::SinCos(angle, sina, cosa);
		offset.cx = fround(ascent * sina);
		offset.cy = fround(ascent * cosa);
	}
}

void FontInfo::Data::GetMetrics(int page, CharMetrics *t)
{
	const X11FontFaceInfo& fi = X11FontFace()[font.GetFace()];
	int mi = 0;
	int qi = -1;
	for(int i = 0; i < 256; i++) {
		int wc = (page << 8) + i;
		t[i].fonti = 0;
		t[i].chr = ToAscii(wc);
		if(!t[i].chr)
			t[i].chr = '?';
		for(int q = 1; q < 16; q++)
			if((1 << q) & fi.encoding) {
				int cs = sChrSet(q);
				int c = FromUnicode(wc, cs);
				if(ToUnicode(c, cs) == wc) {
					t[i].fonti = q;
					t[i].chr = c;
					if(!xfont[q])
						CreateFont(q, cs);
					ASSERT(xfont[q]);
					mi++;
					if(qi < 0)
						qi = q;
					break;
				}
			}
	}
	if(mi == 0)
		CreateFont(0, CHARSET_DEFAULT);
	else
	if(mi != 256)
		for(int i = 0; i < 256; i++)
			if(t[i].fonti == 0)
				t[i].fonti = qi;
	Index<int> fonti;
	for(int i = 0; i < 256; i++) {
		fonti.FindAdd(t[i].fonti);
		t[i].width = maxwidth;
		t[i].lspc = t->rspc = 0;
	}
	for(int fi = 0; fi < fonti.GetCount(); fi++) {
		XFontStruct *xf = XQueryFont(Xdisplay, xfont[fonti[fi]]);
		for(int i = 0; i < 256; i++) {
			if(t[i].fonti == fonti[fi]) {
				if(xf->per_char) {
					int c = t[i].chr;
					if(c >= xf->min_char_or_byte2 && c <= xf->max_char_or_byte2) {
						XCharStruct& cs = xf->per_char[c - xf->min_char_or_byte2];
					  	t[i].width = cs.width;
						t[i].lspc = cs.lbearing;
						t[i].rspc = cs.width - cs.rbearing;
					}
				}
			}
		}
		XFreeFontInfo(NULL, xf, 1);
	}
}

const FontInfo::CharMetrics& FontInfo::Get(wchar c){
	return GetPage(c >> 8)[c & 0xff];
}

void X11FontFaceInfo::Dump()
{
	LLOG(name << " normal:" << style[0] << " italic:" << style[1] <<
	     " bold:" << style[2] << " bold italic:" << style[3] <<
	     " fixed:" << fixed <<
	     " scaleable:" << scaleable << " encodings:" << encoding);
}

const char *basic_fonts[] = {
	"adobe-helvetica",
	"adobe-times",
	"misc-fixed",
	"microsoft-times new roman;urw-nimbus roman no9 l;adobe-times",
	"microsoft-arial;urw-nimbus sans l;adobe-helvetica",
	"microsoft-courier;urw-nimbus mono l;adobe-courier",
	"microsoft-symbol;urw-standard symbols l;urw-symbol;adobe-symbol"
};

int FindFont(const ArrayMap<String, X11FontFaceInfo>& fnf, const char *s)
{
	Vector<String> p = Split(s, ';');
	for(int j = 0, f; j < p.GetCount(); j++)
		if((f = fnf.Find(p[j])) >= 0)
			return f;
	Panic(NFormat("None of font %s found", s));
}

void Draw::InitPlatformFonts()
{
	int fontCount;
	char **fontList = XListFonts(Xdisplay,  "-*-*-*-*-*-*-*-*-*-*-*-*-*-*",
	                             0xFFFF, &fontCount);
	ArrayMap<String, X11FontFaceInfo> fnf;
	for(int i = 0; i < fontCount; i++) {
		Vector<String> fi = Split(fontList[i], '-', false);
		fi.SetCount(XLFD_CHARSET2 + 1);
		String name = ToLower(fi[XLFD_FOUNDRY] + "-" + fi[XLFD_FAMILY]);
		bool sw = false;
		if(fi[XLFD_SET_WIDTH] != "normal") {
			name = name + "-" + fi[XLFD_SET_WIDTH];
			sw = true;
		}
		X11FontFaceInfo& ff = fnf.GetAdd(name);
		ff.name = name;
		ff.setwidth = sw;
		int c = *fi[XLFD_SPACING];
		if(c == 'm' || c == 'M' || c == 'c' || c == 'C')
			ff.fixed = true;
		int sti = 0;
		if(fi[XLFD_WEIGHT] == "black" ||
		   fi[XLFD_WEIGHT] == "bold" ||
		   fi[XLFD_WEIGHT] == "demi bold")
			sti = 2;
		c = *fi[XLFD_SLANT];
		if(c == 'i' || c == 'o')
			sti++;
		ff.style[sti] = fi[XLFD_WEIGHT] + "-" + fi[XLFD_SLANT];
		if(fi[XLFD_PIXELS] == "0" && fi[XLFD_POINTS] == "0" &&
		   fi[XLFD_AVG_WIDTH] == "0")
			ff.scaleable |= Font::SCALEABLE;
		if(fi[XLFD_CHARSET1] == "iso8859") {
			int i = atoi(fi[XLFD_CHARSET2]);
			if(i >= 1 && i <= 10)
				ff.encoding |= 1 << i;
			if(i >= 13 && i <= 16)
				ff.encoding |= 1 << (i - 2);
		}
		if(fi[XLFD_CHARSET1] == "koi8" && fi[XLFD_CHARSET2] == "r")
			ff.encoding |= 0x8000;
	}
    XFreeFontNames(fontList);
    for(int i = 0; i < fnf.GetCount(); i++) {
    	X11FontFaceInfo& fi = fnf[i];
    	String rs = "*-*";
    	for(int j = 0; j < 4; j++)
    		if(!IsNull(fi.style[j])) {
    			rs = fi.style[j];
    			break;
    		}
    	for(int j = 0; j < 4; j++)
    		if(IsNull(fi.style[j]))
    			fi.style[j] = rs;
    }
	Index<int> used;
	for(int i = 0; i < 7; i++) {
		int q = FindFont(fnf, basic_fonts[i]);
		X11FontFace().Add(fnf[q].name, fnf[q]);
		if(i == 0)
			X11FontFace().Add(fnf[q].name, fnf[q]);
		used.Add(q);
	}
	for(int i = 0; i < fnf.GetCount(); i++)
		if(used.Find(i) < 0)
			X11FontFace().Add(fnf[i].name, fnf[i]);
	for(int i = 0; i < X11FontFace().GetCount(); i++)
		X11FontFace()[i].Dump();
}

int    Font::GetFaceCount()
{
	if(!Draw::sFini) Draw::InitFonts();
	return X11FontFace().GetCount();
}

String Font::GetFaceName(int index) {
	if(!Draw::sFini) Draw::InitFonts();
	return index >= 0 && index < X11FontFace().GetCount() ? X11FontFace().GetKey(index)
	                                                      : Null;
}

dword Font::GetFaceInfo(int index) {
	if(!Draw::sFini) Draw::InitFonts();
	dword w = 0;
	if(index >= 0 && index < X11FontFace().GetCount()) {
		X11FontFaceInfo& fi = X11FontFace()[index];
		if(fi.fixed)
			w |= FIXEDPITCH;
		if(X11IsScaleableFace(index))
			w |= SCALEABLE;
		if(fi.encoding == 0)
			w |= SYMBOL;
	}
	return w;
}

/*
int    Font::FindFaceNameIndex(const char *name) {
	if(!Draw::sFini) Draw::InitFonts();
	return X11FontFace().Find(name);
}
*/

String Draw::MakeXLFD(byte cs, Font font, int height, int angle)
{
	const X11FontFaceInfo& fi = X11FontFace()[font.GetFace()];
	String facename = font.GetFaceName();

	String setwidth = "normal";
	if(fi.setwidth) {
		const char *q = strrchr(facename, '-');
		setwidth = q + 1;
		facename = String(~facename, q);
	}
	String xlfd = '-' + facename + '-';
	xlfd << fi.style[2 * font.IsBold() + font.IsItalic()];
	xlfd << '-' << setwidth << "--";
	if(IsNull(height))
		xlfd << '*';
	else {
		height = abs(height);
		if(angle) {
			double sina, cosa;
			SinCos(angle, sina, cosa);
			sina *= height;
			cosa *= height;
			String q = Format("[%.6f %.6f %.6f %.6f]", cosa, sina, -sina, cosa);
			for(const char *s = q; *s; s++)
				if(*s == '-')
					xlfd << '~';
				else
					xlfd << *s;
		}
		else
			xlfd << abs(height);
	}
	xlfd << "-*-*-*-*-*-";
	if(cs >= CHARSET_ISO8859_1 && cs <= CHARSET_ISO8859_10 &&
	   (fi.encoding & (1 << cs)))
	   	xlfd << Format("iso8859-%d", cs);
	else
	if(cs >= CHARSET_ISO8859_13 && cs <= CHARSET_ISO8859_15 &&
	   (fi.encoding & (1 << (cs - 2))))
	   	xlfd << Format("iso8859-%d", cs);
	else
	if(cs == CHARSET_KOI8_R && (fi.encoding & 0x8000))
		xlfd << "koi8-r";
	else
		xlfd << "*-*";
	return xlfd;
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
	f = fh = GetFontHash((font.GetHashValue() ^ angle ^ (device << 9)) % FONTHASH);
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
	FontInfo fi = FontInfo(f);
	fi.Get('x');
	return fi;
}

void Draw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
                      Color ink, int n, const int *dx) {
	LTIMING("DrawText");
	LLOG("DrawText " << FromUnicode(text) << " color:" << ink << " font:" << font);
	int ox = x + actual_offset.x;
	int oy = y + actual_offset.y;
	SetForeground(ink);
	SetFont(font, angle);
	const FontInfo::Data *fd = lastFont.ptr;
	if(angle) {
		int xpos = 0;
		int xpp = 0;
		int pfi = -1;
		for(int i = 0; i < n; i++) {
			const FontInfo::CharMetrics& cm = lastFont.Get(text[i]);
			char chr = cm.chr;
			if(cm.fonti != pfi) {
				pfi = cm.fonti;
				XSetFont(Xdisplay, gc, lastFont.ptr->xfont[cm.fonti]);
			}
			XDrawString(Xdisplay, dw, gc,
			            int(ox + xpos * fd->cosa + fd->offset.cx),
			            int(oy - xpos * fd->sina + fd->offset.cy),
			            &chr, 1);
			xpp = xpos;
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
		{
			Buffer<XTextItem> ti(n);
			Buffer<char> chr(n);
			for(int i = 0; i < n; i++) {
				const FontInfo::CharMetrics& cm = lastFont.Get(text[i]);
				chr[i] = cm.chr;
				XTextItem& q = ti[i];
				q.chars = (char *)chr + i;
				q.nchars = 1;
				q.delta = dx && i > 0 ? dx[i - 1] - lastFont[(byte)text[i - 1]]
				                      : 0;
				q.font = fd->xfont[cm.fonti];
			}
			XDrawText(Xdisplay, dw, gc, ox, oy + lastFont.GetAscent(), ti, n);
		}
		if(font.IsUnderline() || font.IsStrikeout()) {
			int cx;
			if(dx) {
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
