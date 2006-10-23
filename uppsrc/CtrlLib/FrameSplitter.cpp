#include "CtrlLib.h"

void SplitterFrame::FrameAdd(Ctrl& parent)
{
	parent.Add(*this);
}

void SplitterFrame::FrameRemove()
{
	this->Ctrl::Remove();
}

void SplitterFrame::FrameAddSize(Size& sz)
{
	(type == LEFT || type == RIGHT ? sz.cx : sz.cy) += size;
}

void SplitterFrame::FrameLayout(Rect& r)
{
	Rect rr = r;
	maxsize = max(0, r.GetWidth() - 4);
	int msz = max(0, r.GetHeight() - 4);
	switch(type) {
	case LEFT:
		r.left += size;
		rr.right = r.left;
		break;
	case RIGHT:
		r.right -= size;
		rr.left = r.right;
		break;
	case TOP:
		maxsize = msz;
		r.top += size;
		rr.bottom = r.top;
		break;
	case BOTTOM:
		maxsize = msz;
		r.bottom -= size;
		rr.top = r.bottom;
		break;
	}
	SetFrameRect(rr);
}

void SplitterFrame::Paint(Draw& draw)
{
	draw.DrawRect(
		Ctrl::GetSize(),
		HasCapture() ? GUI_GlobalStyle() >= GUISTYLE_XP ? Blend(SColorHighlight, SColorFace) : SColorShadow
		             : SColorFace()
	);
}

void SplitterFrame::LeftDown(Point p, dword)
{
	SetCapture();
	Refresh();
	ref = GetMousePos();
	size0 = size;
}

void SplitterFrame::MouseMove(Point p, dword keyflags)
{
	if(!HasCapture())
		return;
	p = GetMousePos();
	switch(type) {
	case LEFT: size = size0 + p.x - ref.x; break;
	case RIGHT: size = size0 + ref.x - p.x; break;
	case TOP: size = size0 + p.y - ref.y; break;
	case BOTTOM: size = size0 + ref.y - p.y; break;
	}
	size = minmax(size, 4, maxsize);
	RefreshParentLayout();
}

void SplitterFrame::LeftUp(Point p, dword keyflags)
{
	Refresh();
}

Image SplitterFrame::CursorImage(Point p, dword keyflags)
{
	return type == LEFT || type == RIGHT ? Image::SizeHorz() : Image::SizeVert();
}

SplitterFrame& SplitterFrame::Set(Ctrl& c, int _size, int _type)
{
	while(GetFirstChild())
		GetFirstChild()->Remove();
	type = _type;
	size = _size;
	Add(c.SizePos());
	switch(type) {
	case LEFT: c.HSizePos(0, 4); break;
	case RIGHT: c.HSizePos(4, 0); break;
	case TOP: c.VSizePos(0, 4); break;
	case BOTTOM: c.VSizePos(4, 0); break;
	}
	RefreshParentLayout();
	return *this;
}

void SplitterFrame::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	s % size;
}

SplitterFrame::SplitterFrame()
{
	type = LEFT;
	size = maxsize = 0;
}
