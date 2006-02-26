#include "CtrlCore.h"

#define LLOG(x) // LOG(x)

bool Ctrl::Logc::IsEmpty() const {
	return GetAlign() == SIZE ? GetB() <= GetA() : GetB() <= 0;
}

Size Ctrl::PosVal(int v) const {
	switch(v) {
	case MINSIZE: return GetMinSize();
	case STDSIZE: return GetStdSize();
	case MAXSIZE: return GetMaxSize();
	}
	return Size(v, v);
}

void Ctrl::Lay1(int& pos, int& r, int align, int a, int b, int sz) const
{
	pos = a;
	int size = b;
	switch(align) {
	case CENTER:
		pos = (sz - b) / 2 + a;
		break;
	case RIGHT:
		pos = sz - (a + b);
		break;
	case SIZE:
		size = sz - (a + b);
		break;
	}
	r = pos + max(size, 0);
}

Rect Ctrl::CalcRect(LogPos pos, const Rect& prect, const Rect& pview) const
{
	Rect r;
	Size sz = InFrame() ? prect.Size() : pview.Size();
	Lay1(r.left, r.right, pos.x.GetAlign(),
	     PosVal(pos.x.GetA()).cx, PosVal(pos.x.GetB()).cx, sz.cx);
	Lay1(r.top, r.bottom, pos.y.GetAlign(),
	     PosVal(pos.y.GetA()).cy, PosVal(pos.y.GetB()).cy, sz.cy);
	return r;
}

Rect Ctrl::CalcRect(const Rect& prect, const Rect& pview) const
{
	return CalcRect(pos, prect, pview);
}

Rect Ctrl::GetRect() const
{
	return rect;
}

Rect Ctrl::GetView() const
{
	return frame.GetCount() == 0 ? Rect(Size(rect.Size())) : Rect(frame[frame.GetCount() - 1].view);
}

Size Ctrl::GetSize() const
{
	return GetView().Size();
}

Rect  Ctrl::GetScreenRect() const
{
	Rect r = GetRect();
	if(parent) {
		Rect pr = inframe ? parent->GetScreenRect() : parent->GetScreenView();
		r = r + pr.TopLeft();
	}
#ifdef PLATFORM_WIN32
	else if(activex)
		r = GetWndScreenRect();
#endif
	return r;
}

Rect  Ctrl::GetScreenView() const
{
	Rect r = GetScreenRect();
	return GetView() + r.TopLeft();
}

Rect  Ctrl::GetVisibleScreenRect() const
{
	Rect r = GetRect();
	if(parent) {
		Rect pr = inframe ? parent->GetVisibleScreenRect() : parent->GetVisibleScreenView();
		Rect pr1 = inframe ? parent->GetScreenRect() : parent->GetScreenView();
		r = (r + pr1.TopLeft()) & pr;
	}
#ifdef PLATFORM_WIN32
	else if(activex)
		r = GetWndScreenRect();
#endif
	return r & GetWorkArea();
}

Rect  Ctrl::GetVisibleScreenView() const
{
	Rect r = GetVisibleScreenRect();
	return (GetView() + r.TopLeft()) & r;
}

Size  Ctrl::AddFrameSize(int cx, int cy) const
{
	Size sz = Size(cx, cy);
	for(int i = frame.GetCount() - 1; i >= 0; i--)
		frame[i].frame->FrameAddSize(sz);
	return sz;
}

Size Ctrl::GetMinSize() const
{
	int fcy = Draw::GetStdFontCy();
	return AddFrameSize(fcy / 2, fcy + 2);
}

Size Ctrl::GetStdSize() const
{
	Size sz = GetMinSize();
	sz.cx *= 10;
	return sz;
}

Size Ctrl::GetMaxSize() const {
	return GetWorkArea().Size();
}

void Ctrl::SyncLayout(bool force)
{
	LLOG("SyncLayout " << Name() << " size: " << GetSize());
	LOGBEGIN();
	bool refresh = false;
	Rect oview = GetView();
	Rect view = GetRect().Size();
	for(int i = 0; i < frame.GetCount(); i++) {
		Frame& f = frame[i];
		f.frame->FrameLayout(view);
		if(view != f.view) {
			f.view = view;
			refresh = true;
		}
	}
	if(oview.Size() != view.Size()) {
		for(Ctrl *q = GetFirstChild(); q; q = q->next) {
			q->rect = q->CalcRect(rect, view);
			LLOG("Layout set rect " << q->Name() << " " << q->rect);
			q->SyncLayout();
		}
		Refresh();
	}
	if(oview != view || force) {
		State(LAYOUTPOS);
		Layout();
	}
	if(refresh)
		RefreshFrame();
	LOGEND();
}

void Ctrl::SetPos0(LogPos p, bool _inframe)
{
	if(p == pos && inframe == _inframe) return;
	if(parent) RefreshFrame();
	pos = p;
	inframe = _inframe;
	UpdateRect();
}

void Ctrl::UpdateRect()
{
	if(parent)
		rect = CalcRect(parent->GetRect(), parent->GetView());
	else {
		Rect r = GetWorkArea();
		rect = CalcRect(r, r);
	}
	SyncLayout();
	if(parent) RefreshFrame();
}

Ctrl& Ctrl::SetPos(LogPos p, bool _inframe)
{
	if(p != pos || inframe != _inframe) {
		if(parent || !IsOpen())
			SetPos0(p, _inframe);
		else {
			Rect wa = GetWorkArea();
			WndSetPos(CalcRect(p, wa, wa));
		}
		StateH(POSITION);
	}
	return *this;
}

Ctrl& Ctrl::SetPos(LogPos p)
{
	return SetPos(p, false);
}

Ctrl& Ctrl::SetPosX(Logc x)
{
	return SetPos(LogPos(x, pos.y));
}

Ctrl& Ctrl::SetPosY(Logc y)
{
	return SetPos(LogPos(pos.x, y));
}

Ctrl& Ctrl::SetFramePos(LogPos p)
{
	return SetPos(p, true);
}

Ctrl& Ctrl::SetFramePosX(Logc x) {
	return SetPos(LogPos(x, pos.y), true);
}

Ctrl& Ctrl::SetFramePosY(Logc y) {
	return SetPos(LogPos(pos.x, y), true);
}

void  Ctrl::SetRect(int x, int y, int cx, int cy)
{
	LLOG("SetRect " << Name() << " rect: " << RectC(x, y, cx, cy));
	SetPos(PosLeft(x, cx), PosTop(y, cy));
}

void  Ctrl::SetWndRect(const Rect& r)
{
	LLOG("SetWndRect " << Name() << " rect: " << r << " (Ctrl::GetRect = " << GetRect() << ")");
	SetPos0(LogPos(PosLeft(r.left, r.Width()), PosTop(r.top, r.Height())), false);
	StateH(POSITION);
}

void Ctrl::SetRect(const Rect& r)
{
	SetRect(r.left, r.top, r.Width(), r.Height());
}

void Ctrl::SetRectX(int x, int cx) {
	SetPosX(PosLeft(x, cx));
}

void Ctrl::SetRectY(int y, int cy) {
	SetPosY(PosTop(y, cy));
}

void  Ctrl::SetFrameRect(int x, int y, int cx, int cy) {
	SetFramePos(PosLeft(x, cx), PosTop(y, cy));
}

void Ctrl::SetFrameRect(const Rect& r) {
	SetFrameRect(r.left, r.top, r.Width(), r.Height());
}

void Ctrl::SetFrameRectX(int x, int cx) {
	SetFramePosX(PosLeft(x, cx));
}

void Ctrl::SetFrameRectY(int y, int cy) {
	SetFramePosY(PosTop(y, cy));
}

Ctrl& Ctrl::SetFrame(int i, CtrlFrame& fr) {
	LLOG("SetFrame " << typeid(fr).name());
	while(frame.GetCount() <= i)
		frame.Add().frame = &NullFrame();
	frame[i].frame->FrameRemove();
	frame[i].frame = &fr;
	fr.FrameAdd(*this);
	SyncLayout();
	RefreshFrame();
	return *this;
}

Ctrl& Ctrl::AddFrame(CtrlFrame& fr) {
	LLOG("AddFrame " << typeid(fr).name());
	frame.Add().frame = &fr;
	fr.FrameAdd(*this);
	SyncLayout();
	RefreshFrame();
	return *this;
}

void Ctrl::ClearFrames() {
	for(int i = 0; i < frame.GetCount(); i++)
		frame[i].frame->FrameRemove();
	frame.Clear();
	frame.Add().frame = &NullFrame();
	RefreshFrame();
	SyncLayout();
}

void Ctrl::RemoveFrame(int i) {
	int n = frame.GetCount();
	Mitor<Frame> m;
	if(n > 1)
		for(int q = 0; q < n; q++)
			if(q != i)
				m.Add().frame = frame[q].frame;
			else
				frame[q].frame->FrameRemove();
	frame = m;
	if(frame.GetCount() == 0)
		frame.Add().frame = &NullFrame();
	RefreshFrame();
	SyncLayout();
}

int  Ctrl::FindFrame(CtrlFrame& frm)
{
	for(int i = 0; i < frame.GetCount(); i++)
		if(frame[i].frame == &frm)
			return i;
	return -1;
}


void Ctrl::RemoveFrame(CtrlFrame& frm)
{
	int i = FindFrame(frm);
	if(i >= 0)
		RemoveFrame(i);
}

void Ctrl::InsertFrame(int i, CtrlFrame& fr)
{
	int n = frame.GetCount();
	Mitor<Frame> m;
	if(n > 1)
		for(int q = 0; q < n; q++) {
			if(q == i) m.Add().frame = &fr;
			m.Add().frame = frame[q].frame;
		}
	frame = m;
	fr.FrameAdd(*this);
	SyncLayout();
	RefreshFrame();
}

Ctrl& Ctrl::LeftPos(int a, int size) {
	return SetPosX(PosLeft(a, size));
}

Ctrl& Ctrl::RightPos(int a, int size) {
	return SetPosX(PosRight(a, size));
}

Ctrl& Ctrl::TopPos(int a, int size) {
	return SetPosY(PosTop(a, size));
}

Ctrl& Ctrl::BottomPos(int a, int size) {
	return SetPosY(PosBottom(a, size));
}

Ctrl& Ctrl::HSizePos(int a, int b) {
	return SetPosX(PosSize(a, b));
}

Ctrl& Ctrl::VSizePos(int a, int b) {
	return SetPosY(PosSize(a, b));
}

Ctrl& Ctrl::SizePos() {
	return HSizePos().VSizePos();
}

Ctrl& Ctrl::HCenterPos(int size, int delta) {
	return SetPosX(PosCenter(size, delta));
}

Ctrl& Ctrl::VCenterPos(int size, int delta) {
	return SetPosY(PosCenter(size, delta));
}

Ctrl& Ctrl::LeftPosZ(int a, int size) {
	return LeftPos(HorzLayoutZoom(a), HorzLayoutZoom(size));
}

Ctrl& Ctrl::RightPosZ(int a, int size) {
	return RightPos(HorzLayoutZoom(a), HorzLayoutZoom(size));
}

Ctrl& Ctrl::TopPosZ(int a, int size) {
	return TopPos(VertLayoutZoom(a), VertLayoutZoom(size));
}

Ctrl& Ctrl::BottomPosZ(int a, int size) {
	return BottomPos(VertLayoutZoom(a), VertLayoutZoom(size));
}

Ctrl& Ctrl::HSizePosZ(int a, int b) {
	return HSizePos(HorzLayoutZoom(a), HorzLayoutZoom(b));
}

Ctrl& Ctrl::VSizePosZ(int a, int b) {
	return VSizePos(VertLayoutZoom(a), VertLayoutZoom(b));
}

Ctrl& Ctrl::HCenterPosZ(int size, int delta) {
	return HCenterPos(HorzLayoutZoom(size), HorzLayoutZoom(delta));
}

Ctrl& Ctrl::VCenterPosZ(int size, int delta) {
	return VCenterPos(VertLayoutZoom(size), VertLayoutZoom(delta));
}
