#include "Docking.h"

// TitleFrame
void TitleFrame::Paint(Draw& w)
{
	const Style *st = St();
	Rect r = GetSize();	
	
	r.bottom = TitleSz();
	ChPaint(w, r, st->background[focuslook]);
	
	r.right = GetLastChild() ? GetLastChild()->GetRect().left-1 : r.right-1;
	w.Clip(r);
	Point pt = Point(r.left + 2, r.top + 1);
	if (img) {
		w.DrawImage(pt.x, (pt.y + r.Height() - img.GetHeight())/2, img);	
		pt.x += img.GetWidth() + 2;							
	}
	w.DrawText(pt.x, pt.y, title, st->font, st->text[focuslook]);
	w.End();
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
	background[0] = Colorize(DockImg::titlegrad2, SColorShadow(), 200); // No focus
	background[1] = Colorize(DockImg::titlegrad2, Blend(SColorHighlight(), SWhite(), 40), 200); // Focus
	text[0]	= SWhite(); // No focus
	text[1]	= SWhite(); // Focus
	font	  = StdFont(12);
}

