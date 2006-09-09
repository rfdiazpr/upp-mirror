#include "CtrlLib.h"

#define LLOG(x) // LOG(x)

#define IMAGECLASS CtrlsImg
#define IMAGEFILE <CtrlLib/Ctrls.iml>
#include <Draw/iml_source.h>

#define IMAGECLASS ClassicCtrlsImg
#define IMAGEFILE <CtrlLib/ClassicCtrls.iml>
#include <Draw/iml.h>

void ChSetStyleStd()
{
	ChSysInit();
	Ctrl::SetXPStyle(true);
	ColoredOverride(CtrlsImg::Iml(), CtrlsImg::Iml());
}

void ChSetStyleClassic()
{
	LLOG("ChInitWinClassic");
	ChSysInit();
	Ctrl::SetXPStyle(false);
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

struct ChStyleInfo {
	const char *name;
	void (*set)();
};

static ChStyleInfo s_chs[] = {
	"host", ChDetectStyle,
	"standard", ChSetStyleStd,
	"classic", ChSetStyleClassic,
};

int ch_style;

void ChSetStyle(int q)
{
	if(q < 0 || q >= __countof(s_chs))
		q = 0;
	ch_style = q;
	(*s_chs[q].set)();
}

int ChGetStyle()
{
	return ch_style;
}

int ChGetStyleCount()
{
	return __countof(s_chs);
}

String ChGetStyleName(int q)
{
	return s_chs[q].name;
}

void ChSync()
{
	ChSetStyle(ChGetStyle());
}

#ifdef PLATFORM_X11

void ChDetectStyle()
{
	ChSetStyleStd();
}

void ChSysInit()
{
	CtrlImg::Reset();
	CtrlsImg::Reset();
	ChReset();
}

#endif
