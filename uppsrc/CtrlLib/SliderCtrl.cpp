#include "CtrlLib.h"

SliderCtrl::SliderCtrl()
: min(0)
, max(100)
, value(Null)
, step(1)
, round_step(true)
{
	Transparent();
	NoWantFocus();
}

void SliderCtrl::Paint(Draw& w)
{
	Size size = GetSize();
	int half = size.cy >> 1;
	DrawBorder(w, 2, half - 2, size.cx - 4, 4, InsetBorder);
	if(!IsNull(value)) {
		int v = SliderToClient(value);
		Size thumb = CtrlImg::hthumb().GetSize();
		int thumb_y = (size.cy - thumb.cy) >> 1;
		w.DrawImage(v, thumb_y, HasCapture() || HasFocus() ? CtrlImg::hthumb_white()
		                                                   : CtrlImg::hthumb());
	}
}

bool SliderCtrl::Key(dword key, int repcnt)
{
	if(IsEditable())
		switch(key) {
		case K_LEFT:
		case K_UP:
			Dec();
			return true;
		case K_RIGHT:
		case K_DOWN:
			Inc();
			return true;
		}
	return Ctrl::Key(key, repcnt);
}

void SliderCtrl::LeftDown(Point pos, dword keyflags)
{
	if(!IsEditable())
		return;
	int thumb = SliderToClient(value);
	if(IsNull(thumb)) {
		value = ClientToSlider(pos.x);
		UpdateActionRefresh();
	}
	else
	if(pos.x >= thumb && pos.x < thumb + CtrlImg::hthumb().GetWidth())
		SetCapture();
	else
	if(pos.x < thumb)
		Dec();
	else
		Inc();
	Refresh();
}

void SliderCtrl::LeftRepeat(Point p, dword f)
{
	if(!HasCapture())
		LeftDown(p, f);
}

void SliderCtrl::LeftUp(Point pos, dword keyflags)
{
	Refresh();
}

void SliderCtrl::MouseMove(Point pos, dword keyflags)
{
	if(HasCapture()) {
		int n = ClientToSlider(pos.x);
		if(n != value) {
			value = n;
			UpdateActionRefresh();
		}
	}
}

void SliderCtrl::SetData(const Value& v)
{
	int i = v;
	if(!IsNull(i))
		i = minmax(i, min, max);
	if(i != value) {
		value = i;
		UpdateRefresh();
	}
}

Value SliderCtrl::GetData() const
{
	return value;
}

SliderCtrl& SliderCtrl::MinMax(int _min, int _max)
{
	if(min != _min || max != _max) {
		min = _min;
		max = _max;
		if(!IsNull(value)) {
			int v = minmax(value, min, max);
			if(value != v) {
				value = v;
				Update();
			}
		}
		Refresh();
	}
	return *this;
}

int SliderCtrl::SliderToClient(int v) const
{
	if(IsNull(v))
		return Null;
	v = minmax(v, min, max);
	if(max > min)
		v = iscale(v - min, GetSize().cx - CtrlImg::hthumb().GetWidth(), max - min);
	else
		v = 0;
	return v;
}

int SliderCtrl::ClientToSlider(int x) const
{
	if(max <= min)
		return min;
	return minmax(min + iscale(x, max - min, GetSize().cx - CtrlImg::hthumb().GetWidth()), min, max);
}

void SliderCtrl::Dec()
{
	int n = value;
	if(IsNull(value))
		n = max;
	else
	if(n > min) {
		if(round_step && step > 1)
			n = idivfloor(n - 1, step) * step;
		else
			n -= step;
		if(n < min)
			n = min;
	}
	if(n != value) {
		value = n;
		UpdateActionRefresh();
	}
}

void SliderCtrl::Inc()
{
	int n = value;
	if(IsNull(value))
		n = min;
	else
	if(n < max) {
		if(round_step && step > 1)
			n = idivceil(n + 1, step) * step;
		else
			n += step;
		if(n > max)
			n = max;
	}
	if(n != value) {
		value = n;
		UpdateActionRefresh();
	}
}

SliderCtrl::~SliderCtrl() {}
