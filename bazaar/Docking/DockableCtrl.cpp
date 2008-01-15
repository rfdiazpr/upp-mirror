#include "DockableCtrl.h"

DockableCtrl & DockableCtrl::SizeHint(const Size &min, const Size &max, const Size &std)
{
	minsize = min; maxsize = max; stdsize = std; 
	return *this;	
}

DockableCtrl::DockableCtrl()
{
	style = NULL;
	maxsize = Null;
	stdsize = Null;
	minsize = Size(0, 0);
	AllowDockAll();
}

CH_STYLE(DockableCtrl, Style, StyleDefault)
{
	frame = &FieldFrame();
	handle[0] = WindowImg::Background(); //SColorFace(); // No focus
	handle[1] = WindowImg::Backgroundf(); //FieldFrameColor(); // Focus
	handle_vert = false;
	title_font = StdFont(10);
	title_ink[0] = SBlack(); 		// No focus
	title_ink[1] = SWhite(); 		// Focus
	close[0] = WindowImg::BClose();
	close[1] = WindowImg::BCloseh();
	close[2] = WindowImg::BClosep();
	close[3] = WindowImg::BClosed();
	windowpos[0] =  WindowImg::BMenu();
	windowpos[1] =  WindowImg::BMenuh();
	windowpos[2] =  WindowImg::BMenup();
	windowpos[3] =  WindowImg::BMenud();
	autohide[0] = WindowImg::BMinimize();
	autohide[1] = WindowImg::BMinimizeh();
	autohide[2] = WindowImg::BMinimizep();
	autohide[3] = WindowImg::BMinimized();
	btnsize = 10;
}

CH_STYLE(DockableCtrl, Style, StyleDefaultVert)
{
	frame = &FieldFrame();
	handle[0] = WindowImg::BackgroundV(); //SColorFace(); // No focus
	handle[1] = WindowImg::BackgroundfV(); //FieldFrameColor(); // Focus
	handle_vert = true;
	title_font = StdFont(10);
	title_ink[0] = SBlack(); // No focus
	title_ink[1] = SWhite(); // Focus
	close[0] = WindowImg::BClose();
	close[1] = WindowImg::BCloseh();
	close[2] = WindowImg::BClosep();
	close[3] = WindowImg::BClosed();
	windowpos[0] =  WindowImg::BMenu();
	windowpos[1] =  WindowImg::BMenuh();
	windowpos[2] =  WindowImg::BMenup();
	windowpos[3] =  WindowImg::BMenud();
	autohide[0] = WindowImg::BMinimize();
	autohide[1] = WindowImg::BMinimizeh();
	autohide[2] = WindowImg::BMinimizep();
	autohide[3] = WindowImg::BMinimized();
	btnsize = 10;
}

