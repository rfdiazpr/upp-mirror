#include "CtrlLib.h"

ColorPusher::~ColorPusher() {}

void ColorPusher::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, push ? SYellow : SWhite);
	int ty = (sz.cy - w.GetFontInfo(StdFont()).GetHeight()) / 2;
	if(withtext) {
		w.DrawRect(2, 2, sz.cy - 4, sz.cy - 4, color);
		DrawFrame(w, 1, 1, sz.cy - 2, sz.cy - 2, SBlack);
		w.DrawText(sz.cy + 2, ty, FormatColor(color));
	}
	else {
		if(!IsNull(color)) {
			w.DrawRect(2, 2, sz.cx - 4, sz.cy - 4, color);
			DrawFrame(w, 1, 1, sz.cx - 2, sz.cy - 2, SBlack);
		}
		else
		if(!withtext)
			w.DrawText(max(2, (sz.cx - w.GetTextSize(nulltext).cx) / 2), ty, nulltext);
	}
	if(HasFocus())
		DrawFocus(w, GetSize());
}

void ColorPusher::LeftDown(Point p, dword)
{
	Drop();
}

bool ColorPusher::Key(dword key, int)
{
	if(key == K_SPACE) {
		Drop();
		return true;
	}
	return false;
}

void ColorPusher::Drop()
{
	if(push) return;
	push = true;
	Refresh();
	saved_color = color;
	colors.PopUp(this, color);
}

void ColorPusher::CloseColors()
{
	push = false;
	if(color != saved_color) {
		color = saved_color;
		UpdateAction();
	}
	Refresh();
}

void ColorPusher::AcceptColors()
{
	push = false;
	color = colors.Get();
	UpdateActionRefresh();
}

void ColorPusher::SetData(const Value& v)
{
	if(color != v) {
		color = v;
		Refresh();
	}
}

Value ColorPusher::GetData() const
{
	return color;
}

void ColorPusher::NewColor()
{
	if(track) {
		color = colors.Get();
		UpdateAction();
	}
}

ColorPusher::ColorPusher()
{
	nulltext = t_("(transparent)");
	color = Null;
	track = push = withtext = false;
	colors.WhenSelect = THISBACK(AcceptColors);
	colors.WhenCancel = THISBACK(CloseColors);
	colors.WhenAction = THISBACK(NewColor);
	SetFrame(FieldFrame());
}

ColorButton::~ColorButton() {}

Size ColorButton::GetMinSize() const
{
	return Size(24, 24);
}

void ColorButton::Paint(Draw& w)
{
	Size sz = GetSize();
	Size isz = image.GetSize();
	Point center = (sz - isz) / 2;
	if(IsXPStyle()) {
		if(push)
			DrawXPButton(w, sz, BUTTON_PUSH|BUTTON_TOOL);
		else
		if(HasMouse())
			DrawXPButton(w, sz, BUTTON_NORMAL|BUTTON_TOOL);
	}
	else {
		w.DrawRect(sz, SLtGray);
		if(push)
			DrawFrame(w, sz, SGray, SWhite);
		else
		if(HasMouse())
			DrawFrame(w, sz, SWhite, SGray);
	}
	if(IsNull(color))
		w.DrawImage(center.x + push, center.y + push, nullimage);
	else
		w.DrawImage(center.x + push, center.y + push, image, color);
	w.DrawImage(center.x + push, center.y + push, staticimage);
}

void  ColorButton::MouseEnter(Point p, dword keyflags)
{
	Refresh();
}

void  ColorButton::MouseLeave()
{
	Refresh();
}

ColorButton::ColorButton()
{
	SetFrame(NullFrame());
	NoTrack();
}
