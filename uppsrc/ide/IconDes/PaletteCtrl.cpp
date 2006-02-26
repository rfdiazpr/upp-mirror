#include "IconDes.h"
#pragma hdrstop

#define IMAGESPACE PalCtrlImg
#define IMAGEFILE <ide/IconDes/palctrl.iml>
#include <Draw/iml.h>

PaletteCtrl::PaletteCtrl()
{
	NoNotNull();
	CtrlLayout(transparent);
	int cy = transparent.GetLayoutSize().cy;
	GetImplCtrl().HSizePos(0, 0).VSizePos(0, cy);
	Add(transparent.HSizePos(0, 0).BottomPos(0, cy));
	transparent.transparent_color.SetDisplay(StdColorDisplayNull());
	transparent.set_transparent_color <<= THISBACK(OnSetTransparentColor);
	transparent.set_transparent_color.SetImage(PalCtrlImg::set_transparent_color());
	SetTransparent(Color(0x00, 0xC0, 0xA0));
}

void PaletteCtrl::SetTransparent(Color color)
{
	transparent.transparent_color <<= transparent_color = color;
	WhenSetTransparent();
}

void PaletteCtrl::OnSetTransparentColor()
{
	SetTransparent(Get());
}
