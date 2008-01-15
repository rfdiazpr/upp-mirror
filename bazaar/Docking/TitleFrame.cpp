#include "WindowFrame.h"

#define IMAGECLASS WindowImg
#define IMAGEFILE <Docking/Window.iml>
#include <Draw/iml_source.h>

void TitleFrame::LeftUp(Point p, dword keyflags)	
{ 
	if (close == CTRL_PRESSED) 
	{ 
		close = CTRL_HOT; 
		WhenClose(); 
		Refresh(); 
	}
}

void TitleFrame::MouseMove(Point p, dword keyflags)	
{ 
	if (close >= 0 && CloseRect(GetSize()).Contains(p)) 
	{ 
		close = GetMouseLeft() ? CTRL_PRESSED : CTRL_HOT; 
		Refresh(); 
	}
}

void TitleFrame::Paint(Draw& w)
{
	const Style *st = St();
	Rect r = GetSize();	
	Rect br = CloseRect(r);

	ChPaint(w, r, st->background[focuslook]);
	r.left += 2;
	r.right = r.right - br.Height() - 1;
	if (img) {
		w.DrawImage(r.left + 2, (r.top + 1 + r.Height() - img.GetHeight())/2, img);	
		r.left += img.GetWidth() + 2;							
	}
	StdDisplay().Paint(w, r, title, st->text[focuslook], Null, 0);
	if (close >= 0) 
		ChPaint(w, br, st->close[close]);
}

Rect TitleFrame::CloseRect(Rect r)
{
	int bsz = r.Height() - 2;
	return Rect(r.right - bsz - 1, 1, r.right - 1, r.bottom - 1);
}

TitleFrame & TitleFrame::SetFocusLook(bool _focus)
{
	if (focuslook != _focus) {
		focuslook = _focus; 
		if (_focus && GetParent() && !GetParent()->HasFocusDeep()) 
			GetParent()->SetFocus(); 
		Refresh();
	} 
	return *this;
}

TitleFrame::TitleFrame()
{
	close = CTRL_NORMAL;
	style = NULL;
	focuslook = false;
}

CH_STYLE(TitleFrame, Style, StyleDefault)
{
	background[0] = WindowImg::Background(); // No focus
	background[1] = WindowImg::Backgroundf(); // No focus
	text[0]	= SBlack(); // No focus
	text[1]	= SWhite(); // Focus
	close[0] = WindowImg::BClose();
	close[1] = WindowImg::BCloseh();
	close[2] = WindowImg::BClosep();
	close[3] = WindowImg::BClosed();
	font	= StdFont(12);
}

