#include "CtrlLib.h"

CH_INT(DropBoxInsideEdge, 1);
CH_INT(DropBoxWidth, FrameButtonWidth());

CH_LOOK(DropBoxEdge, CtrlsImgLook(CtrlsImg::I_EFE));
CH_LOOKS(DropBoxBtn, 4, CtrlsImgLook(CtrlsImg::I_EB, CtrlsImg::DA(), ButtonMonoColor));
CH_LOOKS(DropBoxSquaredBtn, 4, CtrlsImgLook(CtrlsImg::I_EB, CtrlsImg::DA(), ButtonMonoColor));

void DropBox::CancelMode()
{
	light = -1;
}

void DropBox::AlwaysDrop(bool e)
{
	always_drop = e;
	EnableDrop(enabled);
}

void DropBox::SyncLook()
{
	int q = HasMouseInFrame(rect);
	if(light != q) {
		light = q;
		RefreshFrame();
	}
}

void DropBox::Push()
{
	if(enabled) {
		SetCapture();
		RefreshFrame();
		WhenPush();
	}
}

Image DropBox::FrameMouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	if(event == LEFTDOWN || event == LEFTDOUBLE)
		Push();
	if(event == LEFTUP)
		RefreshFrame();
	SyncLook();
	return Image::Arrow();
}

Rect DropBox::GetDropBoxRect(Rect r) const {
	r.left = r.right - DropBoxWidth();
	if(!UserEdge() && DropBoxInsideEdge()) {
		Rect mg = ChMargins(DropBoxEdge());
		r.left -= mg.right;
		r.right -= mg.right;
		r.top += mg.top;
		r.bottom -= mg.bottom;
	}
	return r;
}

void DropBox::ButtonPaint(Draw& w, const Rect& r)
{
	int q = CTRL_NORMAL;
	if(HasMouseInFrame(rect))
		q = CTRL_HOT;
	if(HasCapture())
		q = CTRL_PRESSED;
	if(!IsShowEnabled() || !enabled)
		q = CTRL_DISABLED;
	Rect box = GetDropBoxRect(r);
	w.DrawRect(box, enabled ? SColorPaper : SColorFace);
	if(UserEdge()) {
		ChPaint(w, box, DropBoxSquaredBtn(q));
	}
	else {
		ChPaintEdge(w, r, DropBoxEdge());
		ChPaint(w, box, DropBoxBtn(q));
	}
}

void DropBox::ButtonLayout(Rect& r)
{
	rect = r;
	r.right -= DropBoxWidth();
	if(!UserEdge()) {
		Rect mg = ChMargins(DropBoxEdge());
		r.left += mg.left;
		r.top += mg.top;
		r.bottom -= mg.bottom;
		if(DropBoxInsideEdge())
			r.right -= mg.right;
	}
}

void DropBox::ButtonAddSize(Size& sz)
{
	if(UserEdge()) {
		sz.cx += DropBoxWidth();
	}
	else {
		Rect mg = ChMargins(DropBoxEdge());
		sz.cy += mg.top + mg.bottom;
		sz.cx += mg.left + DropBoxWidth();
		if(DropBoxInsideEdge())
			sz.cx += mg.right;
	}
}

void DropBox::DropButton::FrameLayout(Rect& r)
{
	dropbox->ButtonLayout(r);
}

void DropBox::DropButton::FramePaint(Draw& draw, const Rect& r)
{
	dropbox->ButtonPaint(draw, r);
}

void DropBox::DropButton::FrameAddSize(Size& sz)
{
	dropbox->ButtonAddSize(sz);
}


bool DropBox::UserEdge() const
{
	return !dynamic_cast<const DropEdge *>(&GetFrame()) || GetFrameCount() != 2;
}

void DropBox::DropEdge::FrameLayout(Rect& r)
{
	if(dropbox->UserEdge())
		EditFieldFrame().FrameLayout(r);
}

void DropBox::DropEdge::FramePaint(Draw& w, const Rect& r)
{
	if(dropbox->UserEdge())
		EditFieldFrame().FramePaint(w, r);
}

void DropBox::DropEdge::FrameAddSize(Size& sz)
{
	if(dropbox->UserEdge())
		EditFieldFrame().FrameAddSize(sz);
}

DropBox::DropBox()
{
	always_drop = false;
	edge.dropbox = this;
	SetFrame(edge);
	button.dropbox = this;
	AddFrame(button);
	light = -1;
}
