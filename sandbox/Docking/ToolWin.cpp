#include "Docking.h"

NAMESPACE_UPP

Point ToolWin::ToolWinFrame::GetDir(Point p) const
{
	Size sz = GetSize();
	int b = GetBorder();
	if(p.x >= b && p.y > b && p.x < sz.cx - b && p.y < b + GetTitleCy())
		return Point(0, 0);
	if(Rect(sz).Deflated(b).Contains(p))
		return Null;
	b *= 4;
	Point r(0, 0);
	if(p.x < b)
		r.x = -1;
	if(p.x >= sz.cx - b)
		r.x = 1;
	if(p.y < b)
		r.y = -1;
	if(p.y >= sz.cy - b)
		r.y = 1;
	return r;
}

Image ToolWin::ToolWinFrame::CursorImage(Point p, dword keyflags)
{
	Point dir = HasCapture() ? dragdir : GetDir(p);
	if(IsNull(dir))
		return Image::Arrow();
	static Image (*im[9])() = {
		Image::SizeTopLeft, Image::SizeLeft, Image::SizeBottomLeft,
		Image::SizeTop, Image::Arrow, Image::SizeBottom,
		Image::SizeTopRight, Image::SizeRight, Image::SizeBottomRight,
	};
	return (*im[(dir.x + 1) * 3 + (dir.y + 1)])();
}

void ToolWin::ToolWinFrame::Layout()
{
	if(GetParent()) {
		close.Hide();
		if(client)
			client->SizePos();
	}
	else {
		close.Show();
		close.RightPos(2, 16).TopPos(2, 16);
		if(client) {
			Size sz = GetSize();
			Rect m = GetMargins();
			client->LeftPos(m.left, sz.cx - m.left - m.right).TopPos(m.top, sz.cy - m.top - m.bottom);
		}
	}
	DDUMP(GetScreenRect());
	DDUMP(client->GetRect());
	DDUMP(client->GetScreenRect());
	DDUMP(client->IsVisible());
	DDUMP(Upp::Name(client->GetParent()));
}

int ToolWin::ToolWinFrame::GetTitleCy() const
{
	return max(GetStdFontCy(), 16);
}

int ToolWin::ToolWinFrame::GetBorder() const
{
	return max(2, GetStdFontCy() / 4);
}

Rect ToolWin::ToolWinFrame::GetMargins() const
{
	Rect r;
	r.left = r.right = r.top = r.bottom = GetBorder();
	r.top += GetTitleCy();
	return r;
}

Size ToolWin::ToolWinFrame::AddMargins(Size sz) const
{
	Rect m = GetMargins();
	sz += Size(m.left + m.right, m.top + m.bottom);
	return sz;
}

Size ToolWin::ToolWinFrame::GetMinSize() const
{
	return AddMargins(client ? client->GetMinSize() : Size(0, 0));
}

Size ToolWin::ToolWinFrame::GetMaxSize() const
{
	return AddMargins(client ? client->GetMaxSize() : Size(99999, 99999));
}

void ToolWin::ToolWinFrame::SetClientRect(Rect r)
{
	Rect m = GetMargins();
	r.left += m.left;
	r.right += m.right;
	r.top += m.top;
	r.bottom += m.bottom;
	SetRect(r);
}

void ToolWin::PlaceClientRect(Rect r)
{
	Size sz = r.GetSize();
	Point pt = GetMousePos();
	int b = win.GetBorder();
	int t = win.GetTitleCy();
	if(!(pt.x >= r.left + b && pt.x < r.right - b))
		r.left = pt.x - sz.cx / 2;
	if(!(pt.y >= r.top + b && pt.y < r.top + b + t))
		r.top = pt.y - b - t / 2;
	r.SetSize(sz);
	Rect m = win.GetMargins();
	r.left += m.left;
	r.right += m.right;
	r.top += m.top;
	r.bottom += m.bottom;
	win.SetRect(r);
}

void ToolWin::ToolWinFrame::Paint(Draw& w)
{
	if(GetParent())
		return;
	Rect r = GetSize();
	int bn = GetBorder();
	for(int i = 0; i < bn; i++) {
		DrawFrame(w, r, decode(i, 0, SColorShadow(), 1, SColorLight(), SColorFace()));	
		r.Deflate(1);
	}
	int fcy = GetStdFontCy();
	int titlecy = GetTitleCy();
	w.DrawRect(r.left, r.top, r.GetWidth(), titlecy, Blend(SColorFace(), SColorShadow()));
	DrawTextEllipsis(w, r.left + fcy / 4, r.top + (titlecy - fcy) / 2,
	                 r.GetWidth() - fcy / 2 - 16, GetTitle(), "...", StdFont(), SColorText());
}

void ToolWin::ToolWinFrame::StartMouseDrag0()
{
	p0 = GetMousePos();
	rect0 = GetRect();
	SetCapture();	
}

void ToolWin::ToolWinFrame::StartMouseDrag()
{
	DLOG("StartMouseDrag1");
	if(HasCapture() || !IsNull(dragdir))
		return;
	DLOG("StartMouseDrag2");
	dragdir = Point(0, 0);
	StartMouseDrag0();
	DLOG("StartMouseDrag3");
	DDUMP(HasCapture());
}

void ToolWin::ToolWinFrame::LeftDown(Point p, dword keyflags)
{
	StartMouseDrag0();
	dragdir = GetDir(p);
	if(IsNull(dragdir))
		return;
	if(dragdir == Point(0, 0))
		client->MoveBegin();
}

void ToolWin::ToolWinFrame::MouseMove(Point p, dword keyflags)
{
	DLOG("MouseMove " << HasCapture());
	if(!HasCapture())
		return;
	DLOG("MouseMove1");
	Rect r = rect0;
	Point off = GetMousePos() - p0;
	if(dragdir == Point(0, 0)) {
		r.Offset(off);
		client->Moving();
	}
	else {
		Size minsz = GetMinSize();
		Size maxsz = GetMaxSize();
		if(dragdir.x == -1)
			r.left = minmax(r.left + off.x, r.right - maxsz.cx, r.right - minsz.cx);
		if(dragdir.x == 1)
			r.right = minmax(r.right + off.x, r.left + minsz.cx, r.left + maxsz.cx);
		if(dragdir.y == -1)
			r.top = minmax(r.top + off.y, r.bottom - maxsz.cy, r.bottom - maxsz.cy);
		if(dragdir.y == 1)
			r.bottom = minmax(r.bottom + off.y, r.top + minsz.cy, r.top + maxsz.cy);
	}
	SetRect(r);
	DDUMP(HasCapture());
}

void ToolWin::ToolWinFrame::LeftUp(Point p, dword keyflags)
{
	if(HasCapture() && dragdir == Point(0, 0)) {
		dragdir = Null;
		client->MoveEnd();
	}
}

void ToolWin::ToolWinFrame::DoClose()
{
	WhenClose();
}

ToolWin::ToolWinFrame::ToolWinFrame()
{
	dragdir = Null;
	client = NULL;
	FrameLess();
	Add(close);
	close.Image(CtrlImg::cross());
	close <<= THISBACK(DoClose);
	Sizeable();
}

void ToolWin::MoveBegin() {}

void ToolWin::Moving() {}

void ToolWin::MoveEnd() {}

ToolWin::ToolWin()
{
	win.client = this;
	win.Add(*this);
}

END_UPP_NAMESPACE
