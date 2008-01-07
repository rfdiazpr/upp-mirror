#include "TitleFrame.h"

#define IMAGECLASS WindowImg
#define IMAGEFILE <Docking/Window.iml>
#include <Draw/iml_source.h>

// ImgButton
void ImgButton::Paint(Draw &w)
{
	Size sz = GetSize();
	if (look)
		ChPaint(w, sz, look[Pusher::GetVisualState()]);
	int dx = IsPush() * -1;
	int dy = IsPush();
	if (!img.IsEmpty()) {
		Size isz = img.GetSize();
		w.DrawImage((sz.cx - isz.cx) / 2 + dx, (sz.cy - isz.cy) / 2 + dy, img);	
	}
}

// TitleFrame
void TitleFrame::Paint(Draw& w)
{
	const Style *st = St();
	Rect r = GetSize();	

	ChPaint(w, r, st->background[focuslook]);
	r.left += 2;
	r.right = GetLastChild() ? GetLastChild()->GetRect().left-1 : r.right-1;
	if (img) {
		w.DrawImage(r.left + 2, (r.top + 1 + r.Height() - img.GetHeight())/2, img);	
		r.left += img.GetWidth() + 2;							
	}
	StdDisplay().Paint(w, r, title, st->text[focuslook], Null, 0);
}

void TitleFrame::SetChildPos()
{
	const Style *st = St();
	Ctrl::LogPos lp;
	lp.x = Ctrl::Logc(ALIGN_RIGHT, 1, TitleSz()-2);
	lp.y = Ctrl::Logc(ALIGN_TOP, 1, TitleSz()-2);
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext()) 
	{
		c->SetPos(lp);
		lp.x.SetA(lp.x.GetA() + TitleSz());
	}	
}

TitleFrame::TitleFrame()
{
	style = NULL;
	focuslook = false;
}

CH_STYLE(TitleFrame, Style, StyleDefault)
{
	background[0] = WindowImg::Background(); // No focus
	background[1] = WindowImg::Backgroundf(); // No focus
	text[0]	= SBlack(); // No focus
	text[1]	= SWhite(); // Focus
	font	= StdFont(12);
}

