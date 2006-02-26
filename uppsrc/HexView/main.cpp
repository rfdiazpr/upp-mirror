#ifdef flagMAIN

#include "HexView.h"

GUI_APP_MAIN
{
	LOG(0xffffffffffffffff);
	LOG(0xffffffff);
	TopWindow win;
	HexView hv;
	win.Add(hv.SizePos());
	hv.SetTotal(0x80000000);
	hv.SetSc((dword)&WinMain);
	win.Sizeable().Zoomable();
	win.Run();
}

#endif
