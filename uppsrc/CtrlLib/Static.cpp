#include "CtrlLib.h"

void StaticText::Paint(Draw& w)
{
	Size sz = GetSize();
	if(!IsTransparent())
		w.DrawRect(0, 0, sz.cx, sz.cy, SColorFace);
	PaintLabel(w, 0, 0, sz.cx, sz.cy, !IsShowEnabled(), false, false, VisibleAccessKeys());
}

Size StaticText::GetMinSize() const
{
	return GetLabelSize();
}

void StaticText::LabelUpdate() {
	Refresh();
}

StaticText::StaticText()
{
	NoWantFocus();
	IgnoreMouse();
	Transparent();
	SetAlign(ALIGN_LEFT);
}

Label& Label::SetText(const char *text)
{
	LabelBase::SetText(text);
	lbl.accesskey = 0;
	noac = false;
	return *this;
}

Label& Label::SetLabel(const char *_text)
{
	String text;
	byte accesskey = ExtractAccessKey(_text, text);
	LabelBase::SetText(text);
	lbl.accesskey = accesskey;
	return *this;
}

bool Label::HotKey(dword key) {
	if(CompareAccessKey(lbl.accesskey, key)) {
		IterateFocusForward(this, GetParent());
		return true;
	}
	return false;
}

dword Label::GetAccessKeys() const
{
	return AccessKeyBit(lbl.accesskey);
}

void  Label::AssignAccessKeys(dword used)
{
	if(noac)
		return;
	Ctrl *next = GetNext();
	if(!lbl.accesskey && next && next->IsInitFocus()) {
		next->AssignAccessKeys(used);
		if(!next->GetAccessKeysDeep()) {
			lbl.accesskey = ChooseAccessKey(GetText(), used);
			if(lbl.accesskey) Refresh();
			used |= AccessKeyBit(lbl.accesskey);
		}
	}
	Ctrl::AssignAccessKeys(used);
}

Label::Label() {
	noac = false;
}

Label::~Label() {}

LabelBox::LabelBox()
{
	if(IsXPStyle())
		SetInk(LtBlue);
	SetVAlign(ALIGN_TOP);
}

void  LabelBox::AssignAccessKeys(dword used)
{
	Ctrl::AssignAccessKeys(used);
}

LabelBox::~LabelBox() {}

void LabelBox::Paint(Draw& w)
{
	Size sz = GetSize();
	if(!IsTransparent())
		w.DrawRect(sz, SLtGray);
	int d = Draw::GetStdFontCy() >> 1;

	Size ts = PaintLabel(w, d, 0, sz.cx, sz.cy, !IsShowEnabled(), false, false, VisibleAccessKeys());

	if(IsXPStyle()) {
		if(sz.cy < 2 * Draw::GetStdFontCy()) {
			d = sz.cy / 2;
			if(ts.cx) {
				w.DrawRect(0, d - 1, d - d / 2, 1, SColorLight);
				w.DrawRect(0, d, d - d / 2, 1, SColorShadow);
				w.DrawRect(0, d + 1, d - d / 2, 1, SColorLight);

				w.DrawRect(d + ts.cx + d / 2, d - 1, sz.cx - ts.cx - d - d / 2, 1, SColorLight);
				w.DrawRect(d + ts.cx + d / 2, d, sz.cx - ts.cx - d - d / 2, 1, SColorShadow);
				w.DrawRect(d + ts.cx + d / 2, d + 1, sz.cx - ts.cx - d - d / 2, 1, SColorLight);
			}
			else {
				w.DrawRect(0, d - 1, sz.cx, 1, SColorLight);
				w.DrawRect(0, d, sz.cx, 1, SColorShadow);
				w.DrawRect(0, d + 1, sz.cx, 1, SColorLight);
			}
		}
		else
		if(sz.cx < 2 * Draw::GetStdFontCy()) {
			d = sz.cx / 2;
			w.DrawRect(d - 1, 0, 1, sz.cy, SColorLight);
			w.DrawRect(d, 0, 1, sz.cy, SColorShadow);
			w.DrawRect(d + 1, 0, 1, sz.cy, SColorLight);
		}
		else {
			w.DrawRect(0, d + 2, 1, sz.cy - d - 4, SColorDisabled);
			w.DrawRect(sz.cx - 1, d + 2, 1, sz.cy - d - 4, SColorDisabled);
			w.DrawRect(2, sz.cy - 1, sz.cx - 4, 1, SColorDisabled);
			w.DrawRect(2, d, d - d / 2, 1, SColorDisabled);
			w.DrawRect(d + ts.cx + d / 2, d, sz.cx - ts.cx - d - d / 2 - 2, 1, SColorDisabled);

			w.DrawRect(1, d + 1, 2, 1, SColorDisabled);
			w.DrawRect(1, d + 2, 1, 1, SColorDisabled);

			w.DrawRect(sz.cx - 3, d + 1, 2, 1, SColorDisabled);
			w.DrawRect(sz.cx - 2, d + 2, 1, 1, SColorDisabled);

			w.DrawRect(1, sz.cy - 2, 2, 1, SColorDisabled);
			w.DrawRect(1, sz.cy - 3, 1, 1, SColorDisabled);

			w.DrawRect(sz.cx - 3, sz.cy - 2, 2, 1, SColorDisabled);
			w.DrawRect(sz.cx - 2, sz.cy - 3, 1, 1, SColorDisabled);
		}
	}
	else {
		if(sz.cy < 2 * Draw::GetStdFontCy()) {
			d = sz.cy / 2;
			w.DrawRect(0, d, d - d / 2, 1, SColorShadow);
			w.DrawRect(0, d + 1, d - d / 2, 1, SColorLight);

			w.DrawRect(d + ts.cx + d / 2, d, sz.cx - ts.cx - d - d / 2, 1, SColorShadow);
			w.DrawRect(d + ts.cx + d / 2, d + 1, sz.cx - ts.cx - d - d / 2, 1, SColorLight);
		}
		else
		if(sz.cx < 2 * Draw::GetStdFontCy()) {
			d = sz.cx / 2;
			w.DrawRect(d, 0, 1, sz.cy, SColorShadow);
			w.DrawRect(d - 1, 1, 0, sz.cy, SColorLight);
		}
		else {
			w.DrawRect(0, d + 2, 1, sz.cy - d - 4, SGray);
			w.DrawRect(1, d + 2, 1, sz.cy - d - 4, SWhite);
			w.DrawRect(sz.cx - 2, d, 1, sz.cy - d - 2, SGray);
			w.DrawRect(sz.cx - 1, d, 1, sz.cy - d - 2, SWhite);
			w.DrawRect(0, sz.cy - 2, sz.cx - 1, 1, SGray);
			w.DrawRect(0, sz.cy - 1, sz.cx - 1, 1, SWhite);

			w.DrawRect(sz.cx - 1, sz.cy - 2, 1, 2, SWhite);
			w.DrawRect(1, d, d - 1, 1, SGray);
			w.DrawRect(1, d + 1, d - 1, 1, SWhite);
			w.DrawRect(0, d, 1, 2, SGray);
			w.DrawRect(d + ts.cx, d, sz.cx - ts.cx - d - 2, 1, SGray);
			w.DrawRect(d + ts.cx, d + 1, sz.cx - ts.cx - d - 2, 1, SWhite);
		}
	}
}

Vector<Rect> LabelBox::GetTransparentViewRects()
{
	Size sz = GetSize();
	Vector<Rect> r;
	int  d = Draw::GetStdFontCy();
	d >>= 1;
	Size ts = GetLabelSize();
	if(sz.cy < 2 * Draw::GetStdFontCy() || sz.cx < 2 * Draw::GetStdFontCy())
		r.Add(sz);
	else {
		r.Add(RectC(0, d + 2, 2, sz.cy - d - 4));
		r.Add(RectC(sz.cx - 2, d, 2, sz.cy - d - 2));
		r.Add(RectC(0, sz.cy - 2, sz.cx - 1, 2));
		r.Add(RectC(0, 0, sz.cx, ts.cy));
	}
	return r;
}

ParentCtrl::ParentCtrl()
{
	NoWantFocus();
	Transparent();
}

StaticRect& StaticRect::Color(class ::Color _color)
{
	if(color != _color) {
		color = _color;
		Refresh();
	}
	return *this;
}

void StaticRect::Paint(Draw& w) {
	Size sz = GetSize();
	w.DrawRect(0, 0, sz.cx, sz.cy, color);
}

StaticRect::StaticRect() {
	color = SLtGray;
	NoWantFocus();
}

StaticRect::~StaticRect() {}

void ImageCtrl::Paint(Draw& w)
{
	if(!img) return;
	Size sz = GetSize();
	Size bsz = GetStdSize();
	w.DrawImage((sz.cx - bsz.cx) / 2, (sz.cy - bsz.cy) / 2, img);
}

Size ImageCtrl::GetStdSize() const
{
	return img.GetSize();
}

Size ImageCtrl::GetMinSize() const
{
	return img.GetSize();
}

void Picture::Paint(Draw& w) {
	Size sz = GetSize();
	w.DrawRect(0, 0, sz.cx, sz.cy, background);
	if(!picture) return;
	int dx = 0;
	int dy = 0;
	Size rz = sz;
	if(ratio) {
		Size sr = picture.GetSize();
		if(sr.cx * sz.cx > sz.cy * sr.cy) {
			rz.cy = sr.cy * sz.cx / sr.cx;
			dy = (sz.cy - rz.cy) / 2;
		}
		else {
			rz.cx = sr.cx * sz.cy / sr.cy;
			dx = (sz.cx - rz.cx) / 2;
		}
	}
	w.Clipoff(dx, dy, rz.cx, rz.cy);
	w.DrawDrawing(0, 0, rz.cx, rz.cy, picture);
	w.End();
}

Picture::Picture() {
	ratio = true;
	background = White;
}

Size SeparatorCtrl::GetMinSize() const {
	return Size(size, size);
}

void SeparatorCtrl::Paint(Draw& w) {
	Size sz = GetSize();
	if(sz.cx > sz.cy) {
		int q = sz.cy / 2;
		w.DrawRect(margin, q - 1, sz.cx - 2 * margin, 1, SGray);
		if(!IsXPStyle())
			w.DrawRect(margin, q, sz.cx - 2 * margin, 1, SWhite);
	}
	else {
		int q = sz.cx / 2;
		w.DrawRect(q - 1, margin, 1, sz.cy - 2 * margin, SGray);
		if(!IsXPStyle())
			w.DrawRect(q, margin, 1, sz.cy - 2 * margin, SWhite);
	}
};

SeparatorCtrl& SeparatorCtrl::Margin(int w)
{
	if(w != margin) {
		margin = w;
		Refresh();
	}
	return *this;
}

SeparatorCtrl& SeparatorCtrl::SetSize(int w)
{
	if(w != size) {
		size = w;
		Refresh();
	}
	return *this;
}

SeparatorCtrl::SeparatorCtrl()
{
	NoWantFocus();
	Transparent();
	margin = 2;
	size = 7;
}
