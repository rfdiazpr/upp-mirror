#include "CtrlLib.h"

#define LLOG(x) // LOG(x)

void Pusher::RefreshFocus() {
	Refresh();
}

void Pusher::RefreshPush() {
	Refresh();
}

void Pusher::PerformAction() {
	WhenAction();
}

void Pusher::GotFocus() {
	RefreshFocus();
}

void Pusher::LostFocus() {
	if(keypush) EndPush();
	RefreshFocus();
}

void Pusher::LeftDown(Point, dword) {
	if(Ctrl::ClickFocus()) SetWantFocus();
	if(IsReadOnly()) return;
	push = true;
	RefreshPush();
	WhenPush();
}

void Pusher::MouseMove(Point, dword style) {
	LLOG("Mouse move !");
	if(IsReadOnly()) return;
	bool b = style & K_MOUSELEFT;
	if(b != push) {
		push = b;
		RefreshPush();
	}
}

void Pusher::MouseLeave() {
	EndPush();
}

void Pusher::LeftRepeat(Point, dword) {
	if(IsReadOnly()) return;
	WhenRepeat();
}

void Pusher::LeftUp(Point, dword) {
	FinishPush();
}

dword Pusher::GetAccessKeys() const
{
	return AccessKeyBit(accesskey);
}

void  Pusher::AssignAccessKeys(dword used)
{
	if(!accesskey) {
		accesskey = ChooseAccessKey(label, used);
		if(accesskey) Refresh();
		used |= AccessKeyBit(accesskey);
	}
	Ctrl::AssignAccessKeys(used);
}

void  Pusher::State(int)
{
	if(keypush && GetTopCtrl() && !GetTopCtrl()->HasFocusDeep())
		EndPush();
}

void Pusher::KeyPush() {
	if(IsReadOnly()) return;
	if(keypush) {
		WhenRepeat();
		return;
	}
	push = keypush = true;
	RefreshPush();
	WhenPush();
}

bool Pusher::FinishPush()
{
	if(!IsPush())
		return false;
	EndPush();
	if(IsReadOnly())
		return false;
	PerformAction();
	return true;
}

bool Pusher::Key(dword key, int) {
	if(IsReadOnly()) return false;
	if(key == K_SPACE) {
		KeyPush();
		return true;
	}
	if(key == (K_SPACE|K_KEYUP))
		return FinishPush();
	return false;
}

bool Pusher::HotKey(dword key) {
	if(CompareAccessKey(accesskey, key)) {
		PseudoPush();
		return true;
	}
	return false;
}

void Pusher::PseudoPush() {
	if(IsReadOnly() || !IsEnabled()) return;
	KeyPush();
	Sync();
	Sleep(50);
	FinishPush();
}

void Pusher::EndPush() {
	if(push || keypush)
		RefreshPush();
	keypush = push = false;
}

Pusher&  Pusher::SetFont(Font fnt) {
	font = fnt;
	Refresh();
	return *this;
}

String Pusher::GetDesc()
{
	return label;
}

Pusher& Pusher::SetLabel(const char *atext) {
	accesskey = ExtractAccessKey(atext, label);
	Refresh();
	return *this;
}

void Pusher::CancelMode() {
	EndPush();
}

Pusher::Pusher() {
	keypush = push = false;
	accesskey = 0;
	NoInitFocus();
}

Pusher::~Pusher() {}

// ----------------

void Button::Paint(Draw& w) {
	Size sz = GetSize();
	bool ds = !IsShowEnabled();
	DrawLabel dl;
	dl.text = label;
	dl.font = font;
	dl.limg = img;
	dl.disabled = ds;
	dl.lspc = !label.IsEmpty() && !img.IsEmpty() ? 4 : 0;
	if(VisibleAccessKeys())
		dl.accesskey = accesskey;
	if(monoimg)
		dl.lcolor = SColorText;
	if(IsXPStyle()) {
		int t = ds ? BUTTON_DISABLED :
		        IsPush() ? BUTTON_PUSH :
		        HasMouse() || HasFocus() ? BUTTON_HIGHLIGHT :
		        type == OK ? BUTTON_OK :
		        BUTTON_NORMAL;
		if(scrollbutton)
			t |= BUTTON_SCROLL;
		else
		if(edgestyle || InFrame())
			t |= BUTTON_EDGE;
		DrawXPButton(w, sz, t);
		if(monoimg && (edgestyle || InFrame() || scrollbutton))
			dl.lcolor = Blend(Blend(SColorHighlight, SColorShadow), SColorText, 80);
		dl.Paint(w, 3, 3, sz.cx - 6, sz.cy - 6);
	}
	else {
		if(w.PaletteMode())
			w.DrawRect(2, 2, sz.cx - 4, sz.cy - 4, SLtGray);
		else
			for(int i = 0; i < sz.cy - 4; i++)
				w.DrawRect(2, 2 + i, sz.cx - 4, 1,
				           GradientColor(SWhiteGray, SLtGray, i + sz.cy / 2, sz.cy / 2 + sz.cy - 4));
		dl.push = IsPush();
		dl.Paint(w, 3, 3, sz.cx - 6, sz.cy - 6);
		DrawBorder(w, 0, 0, sz.cx, sz.cy,
			IsPush() ? ButtonPushBorder
			         : type == OK ? DefButtonBorder
			                      : edgestyle ? EdgeButtonBorder
			                                  : ButtonBorder
		);
		if(HasFocus() && min(sz.cx, sz.cy) > 8)
			DrawFocus(w, 4, 4, sz.cx - 8, sz.cy - 8);
	}
}

void  Button::MouseEnter(Point, dword)
{
	if(IsXPStyle())
		Refresh();
}

void  Button::MouseLeave()
{
	if(IsXPStyle())
		Refresh();
	Pusher::MouseLeave();
}

bool Button::Key(dword key, int w) {
	if(Pusher::Key(key, w)) return true;
	if(key == K_ENTER) {
		KeyPush();
		return true;
	}
	if(key == (K_ENTER|K_KEYUP))
		return FinishPush();
	return false;
}

bool Button::HotKey(dword key) {
	if(Pusher::HotKey(key)) return true;
	if(CompareAccessKey(accesskey, key)) {
		PseudoPush();
		return true;
	}
	bool up = key & K_KEYUP;
	key = key & ~K_KEYUP;
	if(key == K_ENTER && (type == OK || type == EXIT) ||
	   key == K_ESCAPE && (type == CANCEL || type == EXIT)) {
		if(up) {
			if(IsKeyPush())
				return FinishPush();
		}
		else
			KeyPush();
		return true;
	}
	return false;
}

dword Button::GetAccessKeys() const
{
	if(type == OK || type == EXIT)
		return 1;
	return Pusher::GetAccessKeys();
}

void  Button::AssignAccessKeys(dword used)
{
	if(type == OK || type == EXIT || type == CANCEL)
		return;
	return Pusher::AssignAccessKeys(used);
}

Button& Button::SetImage(const Image& _img)
{
	img = _img;
	monoimg = false;
	Refresh();
	return *this;
}

Button& Button::SetMonoImage(const Image& _img)
{
	img = _img;
	monoimg = true;
	Refresh();
	return *this;
}

Button::Button() {
	edgestyle = scrollbutton = false;
	type = NORMAL;
}

Button::~Button() {}

void SpinButtons::FrameLayout(Rect& r)
{
	if(!visible) {
		inc.Hide();
		dec.Hide();
		return;
	}
	inc.Show();
	dec.Show();
	Size sz = r.Size();
	int h = r.Height();
	int h2 = h / 2;
	int h7 = min(sz.cx / 2, 12);
	inc.SetFrameRect(r.right - h7, r.top, h7, h2);
	dec.SetFrameRect(r.right - h7, r.top + h2, h7, r.Height() - h2);
	r.right -= h7;
}

void SpinButtons::FrameAddSize(Size& sz)
{
	sz.cx += min(sz.cx / 2, 12);
}

void SpinButtons::FrameAdd(Ctrl& ctrl)
{
	ctrl.Add(inc);
	ctrl.Add(dec);
}

void SpinButtons::FrameRemove() {
	inc.Remove();
	dec.Remove();
}

SpinButtons::SpinButtons() {
	visible = true;
	inc.SetMonoImage(CtrlImg::spinup()).NoWantFocus();
	dec.SetMonoImage(CtrlImg::spindown()).NoWantFocus();
}

SpinButtons::~SpinButtons() {}

void SpinButtons::Show(bool s)
{
	visible = s;
	inc.RefreshParentLayout();
}

// -----------------

const Image& GetPushImage(int g, int q)
{
	ASSERT(g >= 0 && q < 5 && q >= 0 && q < 4);
	const Image& (*im[5][4])() = {
		{ CtrlImg::Radio0, CtrlImg::Radio0h, CtrlImg::Radio0p, CtrlImg::Radio0d },
		{ CtrlImg::Radio1, CtrlImg::Radio1h, CtrlImg::Radio1p, CtrlImg::Radio1d },
		{ CtrlImg::Check0, CtrlImg::Check0h, CtrlImg::Check0p, CtrlImg::Check0d },
		{ CtrlImg::Check1, CtrlImg::Check1h, CtrlImg::Check1p, CtrlImg::Check1d },
		{ CtrlImg::Check2, CtrlImg::Check2h, CtrlImg::Check2p, CtrlImg::Check2d },
	};
	return (*im[g][q])();
}

// -----------------

void  Option::MouseEnter(Point, dword)
{
	if(IsXPStyle())
		RefreshPush();
}

void  Option::MouseLeave()
{
	if(IsXPStyle())
		RefreshPush();
	Pusher::MouseLeave();
}

void Option::RefreshFocus() {
	Refresh();
}

void Option::RefreshPush() {
	Refresh(0, 0, (IsXPStyle() ? GetPushImage(I_CHECK0, I_NORMAL) : CtrlImg::option0()).GetSize().cx, GetSize().cy);
}

Size Option::GetMinSize() const {
	Size isz = (IsXPStyle() ? GetPushImage(I_CHECK0, I_NORMAL) : CtrlImg::option0()).GetSize();
	return AddFrameSize(isz.cx + GetSmartTextSize(ScreenInfo(), label).cx + 4,
		                max(isz.cy, StdFont().Info().GetHeight()) + 2);
}

void Option::Paint(Draw& w) {
	Size sz = GetSize();
	if(!IsTransparent())
		w.DrawRect(0, 0, sz.cx, sz.cy, SColorFace);
	Size isz = (IsXPStyle() ? GetPushImage(I_CHECK0, I_NORMAL) : CtrlImg::option0()).GetSize();
	Size tsz = GetSmartTextSize(w, label, font);
	int ty = (sz.cy - tsz.cy) / 2;
	int iy = (tsz.cy - isz.cy) / 2 + ty;
	bool ds = !IsShowEnabled();
	if(IsXPStyle()) {
		int q = ds ? I_DISABLED :
		        IsPush() ? I_PUSH :
		        HasMouse() || HasFocus()? I_HIGHLIGHT :
		        I_NORMAL;
		int g = (!notnull || threestate) && IsNull(option) ? I_CHECK2 : option == 1 ? I_CHECK1 : I_CHECK0;
		if(switchimage)
			g = option ? I_RADIO1 : I_RADIO0;
		w.DrawImage(0, iy, GetPushImage(g, q));
	}
	else {
		Image m = (!notnull || threestate) && IsNull(option) ? ds ? CtrlImg::option2d()
				                      : IsPush() ? CtrlImg::option2f()
							                     : CtrlImg::option2() :
			      option == 1 ? ds ? (switchimage ? CtrlImg::switch1d() : CtrlImg::option1d())
				                   : IsPush() ? (switchimage ? CtrlImg::switch1f() : CtrlImg::option1f())
							                  : (switchimage ? CtrlImg::switch1() : CtrlImg::option1())
				              : ds ? (switchimage ? CtrlImg::switch0d() : CtrlImg::option0d())
				                   : IsPush() ? (switchimage ? CtrlImg::switch0f() : CtrlImg::option0f())
							                  : (switchimage ? CtrlImg::switch0() : CtrlImg::option0());
		w.DrawImage(0, iy, m);
		if(!switchimage)
			w.DrawImage(0, iy, ds ? CtrlImg::optionedged
			                      : blackedge ? CtrlImg::blackoptionedge
			                                  : CtrlImg::optionedge);
	}

	DrawSmartText(w, isz.cx + 4, ty, tsz.cx, label, font, ds ? SGray : SBlack, VisibleAccessKeys() ? accesskey : 0);
}

void   Option::SetData(const Value& data) {
	Set(IsString(data) ? String(data) == "1" ? 1
	                                         : IsNull(data) ? (notnull ? 0 : int(Null)) : 0
	                   : notnull ? Nvl((int)data, 0) : (int)data);
}

Value  Option::GetData() const {
	return (int) option;
}

void  Option::PerformAction() {
	if(option == 0)
		option = threestate ? Null : 1;
	else
	if(option == 1)
		option = 0;
	else
		option = 1;
	UpdateAction();
	RefreshPush();
}

void  Option::Set(int b) {
	if(b != option) {
		option = b;
		Update();
		RefreshPush();
	}
}

Option::Option() {
	option = 0;
	notnull = true;
	switchimage = threestate = false;
	blackedge = false;
	Transparent();
}

Option::~Option() {}

// --------

void ButtonOption::Serialize(Stream& s) {
	int version = 0;
	s / version;
	bool b = Get();
	s % b;
	Set(b);
	if(s.IsLoading())
		Refresh();
}

void  ButtonOption::Paint(Draw& w) {
	Size sz = GetSize();
	Size isz = image.GetSize();
	Rect r = sz;
	if(IsXPStyle()) {
		int t = !IsEnabled() ? BUTTON_DISABLED :
		        push ? BUTTON_PUSH :
		        option ? BUTTON_CHECKED :
		        HasMouse() || HasFocus() ? BUTTON_HIGHLIGHT :
		        BUTTON_NORMAL;
		if(InFrame())
			t |= BUTTON_EDGE;
		DrawXPButton(w, sz, t);
	}
	else {
		DrawBorder(w, r, push || option ? ButtonPushBorder : ButtonBorder);
		r.Deflate(2, 2);
		w.DrawRect(r, option != push ? SWhiteGray : SLtGray);
	}
	Point p = r.CenterPos(image.GetSize());
	w.DrawImage(p.x, p.y, (option && !IsNull(image1)) ? image1 : image);
}

void  ButtonOption::LeftDown(Point, dword) {
	push = true;
	Refresh();
}

void  ButtonOption::LeftUp(Point, dword) {
	option = !option;
	push = false;
	UpdateActionRefresh();
}

void  ButtonOption::MouseMove(Point, dword flags) {
	bool p = !!(flags & K_MOUSELEFT);
	if(push != p) {
		push = p;
		Refresh();
	}
}

void  ButtonOption::MouseEnter(Point, dword) {
	Refresh();
}

void  ButtonOption::MouseLeave() {
	if(push)
		push = false;
	Refresh();
}

void  ButtonOption::SetData(const Value& v)
{
	Set(IsString(v) ? String(v) == "1" : (int)v);
}

Value ButtonOption::GetData() const
{
	return (int)Get();
}

// --------------------

DataPusher::DataPusher(const Convert& convert, const Display& display)
: convert(&convert), display(&display)
{
	SetFrame(FieldFrame());
}

DataPusher::DataPusher(const Display& display)
: convert(&NoConvert()), display(&display)
{
	SetFrame(FieldFrame());
}

DataPusher::~DataPusher() {}

void DataPusher::PerformAction()
{
	WhenPreAction();
	DoAction();
}

void DataPusher::DoAction()
{
	Action();
}

Value DataPusher::GetData() const
{
	return data;
}

void DataPusher::SetData(const Value& value)
{
	if(value != data)
	{
		data = value;
		UpdateRefresh();
	}
}

void DataPusher::Paint(Draw& draw)
{
	Rect rc = GetSize();
	Color paper = (IsPush() ? Blend(SColorHighlight, SColorFace, 235)
	                        : IsShowEnabled() && !IsReadOnly() ? SColorPaper : SColorFace);
	draw.DrawRect(rc, paper);
	rc.Deflate(2, 1);
	if(IsPush() && !IsXPStyle())
		rc += Size(1, 1);
	display -> Paint(draw, rc, convert -> Format(data),
		(IsEnabled() ? SColorText : SColorDisabled), Color(paper),
		(HasFocus() ? Display::FOCUS : 0) | (IsReadOnly() ? Display::READONLY : 0));
	if(HasFocus())
		DrawFocus(draw, GetSize());
}

void DataPusher::SetDataAction(const Value& value)
{
	SetData(value);
	UpdateActionRefresh();
}
