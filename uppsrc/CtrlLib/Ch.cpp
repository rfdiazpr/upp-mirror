#include "CtrlLib.h"

#define LLOG(x)  // RLOG(x)

#define IMAGECLASS CtrlsImg
#define IMAGEFILE <CtrlLib/Ctrls.iml>
#include <Draw/iml_source.h>

#define IMAGECLASS ClassicCtrlsImg
#define IMAGEFILE <CtrlLib/ClassicCtrls.iml>
#include <Draw/iml_source.h>

void ChSysInit();

void ChStdSkin()
{
	ChSysInit();
	ChSet("GUI_GlobalStyle", GUISTYLE_XP);
	ColoredOverride(CtrlsImg::Iml(), CtrlsImg::Iml());
}

void ChClassicSkin()
{
	LLOG("ChInitWinClassic");
	ChSysInit();
	ChSet("GUI_GlobalStyle", GUISTYLE_CLASSIC);
	ColoredOverride(CtrlsImg::Iml(), ClassicCtrlsImg::Iml());
	for(int q = 0; q < 4; q++)
		CtrlsImg::Set(CtrlsImg::I_HTB + q, AdjustColors(CtrlsImg::Get(ClassicCtrlsImg::I_B + q)));
	Color wc = Blend(SColorFace(), SColorPaper(), 170);
	ChSet("ScrollBarHorzUpper", wc);
	ChSet("ScrollBarHorzUpper", CTRL_PRESSED, SColorText());
	ChSetf("ScrollBarHorzThumb", ScrollButtonLook);
	ChSet("ScrollBarHorzLower", wc);
	ChSet("ScrollBarHorzLower", CTRL_PRESSED, SColorText());
	ChSet("ScrollBarVertUpper", wc);
	ChSet("ScrollBarVertUpper", CTRL_PRESSED, SColorText());
	ChSetf("ScrollBarVertThumb", ScrollButtonLook);
	ChSet("ScrollBarVertLower", wc);
	ChSet("ScrollBarVertLower", CTRL_PRESSED, SColorText());

	ChSet("ButtonPressOffsetFlag", 1);
	ChSet("ButtonMonoColor", SColorText());
	ChSet("LabelBoxTextColor", SColorText());
}

#ifdef PLATFORM_X11

void ChSysInit()
{
	CtrlImg::Reset();
	CtrlsImg::Reset();
	ChReset();
}

#endif
