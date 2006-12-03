#include "CtrlLib.h"

CtrlsImgLook& CtrlsImgLook::operator()(int i, int n)
{
	while(n-- > 0)
		Add(CtrlsImg::Get(i++));
	return *this;
}

CtrlsImgLook& CtrlsImgLook::operator()(int i, const Image& img, Color (*fn)(int i), int n)
{
	for(int q = 0; q < n; q++)
		Add(ChLookWith(CtrlsImg::Get(i++), img, fn, q));
	return *this;
}

CtrlsImgLook& CtrlsImgLook::operator()(int i, const Image& img, int n)
{
	for(int q = 0; q < n; q++)
		Add(ChLookWith(CtrlsImg::Get(i++), img));
	return *this;
}

CtrlsImgLook::CtrlsImgLook(int i, int n)
{
	operator()(i, n);
}

CtrlsImgLook::CtrlsImgLook(int i, const Image& img, Color (*fn)(int i), int n)
{
	operator()(i, img, fn, n);
}

CtrlsImgLook::CtrlsImgLook(int i, const Image& img, int n)
{
	operator()(i, img, n);
}

String DeAmp(const char *s)
{
	String out;
	for(; *s; out.Cat(*s++))
		if(*s == '&')
			out.Cat('&');
	return out;
}

Size GetSmartTextSize(Draw& w, const char *text, Font font) {
	if(*text == '\1') {
		Size sz;
		RichText txt = ParseQTF(text + 1);
		txt.ApplyZoom(GetRichTextStdScreenZoom());
		sz.cx = txt.GetWidth();
		sz.cy = txt.GetHeight(Zoom(1, 1), sz.cx);
		return sz;
	}
	return GetTLTextSize(w, ToUnicode(text, CHARSET_DEFAULT), font);
}

int GetSmartTextHeight(Draw& w, const char *s, int cx, Font font) {
	if(*s == '\1') {
		Size sz;
		RichText txt = ParseQTF(s + 1);
		txt.ApplyZoom(GetRichTextStdScreenZoom());
		return txt.GetHeight(Zoom(1, 1), cx);
	}
	int cy = font.Info().GetHeight();
	int h = cy;
	while(*s) {
		if(*s == '\n')
			h += cy;
		s++;
	}
	return h;
}

void DrawSmartText(Draw& draw, int x, int y, int cx, const char *text, Font font, Color ink, int accesskey) {
	if(*text == '\1') {
		Size sz;
		RichText txt = ParseQTF(text + 1, false, accesskey);
		txt.ApplyZoom(GetRichTextStdScreenZoom());
		txt.Paint(Zoom(1, 1), draw, x, y, cx);
		return;
	}
	DrawTLText(draw, x, y, cx, ToUnicode(text, CHARSET_DEFAULT), font, ink, accesskey);
}

bool CompareAccessKey(byte accesskey, dword key)
{
	return accesskey && dword(ToUpper(accesskey) - 'A' + K_ALT_A) == key;
}

byte  ExtractAccessKey(const char *s, String& label)
{
	byte accesskey = 0;
	String text;
	bool qtf = *s == '\1';
	while(*s)
		if(*s == '\\' && s[1]) {
			text.Cat(s[1]);
			s += 2;
		}
		else
		if((*s == '&' && !qtf || *s == '\b') && s[1] && s[1] != '&') {
			accesskey = ToAscii(ToUpper(s[1]));
			s++;
		}
		else
			text.Cat(*s++);
	text.Shrink();
	label = text;
	return accesskey;
}

byte  ChooseAccessKey(const char *text, dword used)
{
	for(const char *s = text; *s; s++) {
		byte ac = ToAscii(*s);
		if(ac >= 'A' && ac <= 'Z' && (Ctrl::AccessKeyBit(ac) & used) == 0)
			return ac;
	}
	for(const char *s = text; *s; s++) {
		byte ac = ToUpper(ToAscii(*s));
		if(ac >= 'A' && ac <= 'Z' && ac != 'I' && ac != 'L' && (Ctrl::AccessKeyBit(ac) & used) == 0)
			return ac;
	}
	for(const char *s = text; *s; s++) {
		byte ac = ToUpper(ToAscii(*s));
		if(ac >= 'A' && ac <= 'Z' && (Ctrl::AccessKeyBit(ac) & used) == 0)
			return ac;
	}
	return 0;
}

DrawLabel::DrawLabel()
{
	push = focus = disabled = false;
	lspc = rspc = 0;
	ink = Null;
	align = valign = ALIGN_CENTER;
	accesskey = 0;
}

Size DrawLabel::GetSize() const
{
	Size isz(0, 0);
	Size sz1 = limg.GetSize();
	Size sz2 = rimg.GetSize();
	Size txtsz = *text ? GetSmartTextSize(ScreenInfo(), text, font) : paintrect.GetStdSize();

	if(!IsNull(lspc)) {
		isz.cx = lspc;
		isz.cy = sz1.cy;
		isz.cx += sz1.cx;
	}

	if(!IsNull(rspc)) {
		isz.cx += rspc;
		if(sz2.cy > isz.cy) isz.cy = sz2.cy;
		isz.cx += sz2.cx;
	}

	isz.cy = max(txtsz.cy, max(sz1.cy, sz2.cy));
	isz.cx += txtsz.cx;

	return isz;
}

Image sDis(const Image& img, bool dis)
{
	return dis ? MakeImage(img, Etched) : img;
}

Size DrawLabel::Paint(Draw& w, const Rect& r, bool visibleaccesskey) const
{
	Size sz1 = limg.GetSize();
	Size sz2 = rimg.GetSize();
	Size txtsz = *text ? GetSmartTextSize(ScreenInfo(), text, font) : paintrect.GetStdSize();
	Size isz = GetSize();
	Point p, ip;
	if(align == ALIGN_LEFT)
		p.x = r.left;
	else
	if(align == ALIGN_RIGHT)
		p.x = r.right - isz.cx;
	else
	if(align == ALIGN_CENTER)
		p.x = (r.right + r.left - isz.cx) / 2;
	if(valign == ALIGN_TOP)
		p.y = r.top;
	else
	if(valign == ALIGN_BOTTOM)
		p.y = r.bottom - isz.cy;
	else
	if(valign == ALIGN_CENTER)
		p.y = (r.bottom + r.top - txtsz.cy) / 2;
	Color color = ink;
	if(IsNull(color))
		color = disabled ? SColorDisabled : SColorText;
	int ix;
	if(IsNull(lspc))
		ix = r.left + push;
	else {
		ix = p.x + push;
		p.x += sz1.cx;
		p.x += lspc;
	}
	int iy = push + (r.top + r.bottom - sz1.cy) / 2;

	if(IsNull(lcolor))
		w.DrawImage(ix, iy, sDis(limg, disabled));
	else
		w.DrawImage(ix, iy, limg, lcolor);

	iy = push + (r.top + r.bottom - sz2.cy) / 2;
	ix = (IsNull(rspc) ? r.right - sz2.cx : p.x + txtsz.cx + rspc) + push;
	if(IsNull(rcolor))
		w.DrawImage(ix, iy, sDis(rimg, disabled));
	else
		w.DrawImage(ix, iy, rimg, rcolor);
	paintrect.Paint(w, p.x + push, p.y + push, txtsz.cx, isz.cy, color, Null);
	if(*text) {
		if(disabled)
			DrawSmartText(w, p.x + push + 1, p.y + push + (isz.cy - txtsz.cy) / 2 + 1,
			              txtsz.cx, text, font, SColorPaper);
		DrawSmartText(w, p.x + push, p.y + push, txtsz.cx,
		              text, font, color, visibleaccesskey ? accesskey : 0);
		if(focus)
			DrawFocus(w, p.x - 2, p.y, txtsz.cx + 5, isz.cy);
	}

	return isz;
}

Size DrawLabel::Paint(Draw& w, int x, int y, int cx, int cy, bool vak) const
{
	return Paint(w, RectC(x, y, cx, cy), vak);
}

void LabelBase::LabelUpdate() {}

LabelBase& LabelBase::SetLeftImage(const Image& img, int spc) {
	lbl.limg = img;
	lbl.lspc = spc;
	LabelUpdate();
	return *this;
}

LabelBase& LabelBase::SetRightImage(const Image& img, int spc) {
	lbl.rimg = img;
	lbl.rspc = spc;
	LabelUpdate();
	return *this;
}

LabelBase& LabelBase::SetPaintRect(const PaintRect& paintrect) {
	lbl.paintrect = paintrect;
	LabelUpdate();
	return *this;
}


LabelBase& LabelBase::SetText(const char *text) {
	lbl.text = text;
	LabelUpdate();
	return *this;
}

LabelBase& LabelBase::SetFont(Font font) {
	if(lbl.font != font) {
		lbl.font = font;
		LabelUpdate();
	}
	return *this;
}

LabelBase& LabelBase::SetInk(Color ink) {
	if(lbl.ink != ink) {
		lbl.ink = ink;
		LabelUpdate();
	}
	return *this;
}

LabelBase& LabelBase::SetAlign(int align) {
	if(lbl.align != align) {
		lbl.align = align;
		LabelUpdate();
	}
	return *this;
}

LabelBase& LabelBase::SetVAlign(int valign) {
	if(lbl.valign != valign) {
		lbl.valign = valign;
		LabelUpdate();
	}
	return *this;
}

Size LabelBase::PaintLabel(Draw& w, const Rect& r, bool disabled, bool push, bool focus, bool vak)
{
	DrawLabel lbl1 = lbl;
	lbl1.disabled = disabled;
	lbl1.push = push;
	lbl1.focus = focus;
	return lbl1.Paint(w, r, vak);
}


Size LabelBase::PaintLabel(Draw& w, int x, int y, int cx, int cy, bool disabled, bool push, bool focus, bool vak)
{
	return PaintLabel(w, RectC(x, y, cx, cy), disabled, push, focus, vak);
}

Size LabelBase::GetLabelSize() const
{
	return lbl.GetSize();
}

void LinkToolTipIn__();

LabelBase::~LabelBase() {
	LinkToolTipIn__();
}

void DrawFocus(Draw& w, int x, int y, int cx, int cy) {
	w.Clipoff(x, y, cx, cy);
	for(int a = 0; a < cx; a += CtrlImg::focus_h().GetWidth()) {
		w.DrawImage(a, 0, CtrlImg::focus_h());
		w.DrawImage(a, cy - 1, CtrlImg::focus_h());
	}
	for(int a = 0; a < cy; a += CtrlImg::focus_v().GetHeight()) {
		w.DrawImage(0, a, CtrlImg::focus_v());
		w.DrawImage(cx - 1, a, CtrlImg::focus_v());
	}
	w.End();
}

void DrawFocus(Draw& w, const Rect& r) {
	DrawFocus(w, r.left, r.top, r.Width(), r.Height());
}
