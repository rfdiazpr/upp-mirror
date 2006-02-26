#ifndef __DESIGNER_PALETTECTRL__
#define __DESIGNER_PALETTECTRL__

#define LAYOUTFILE <ide/IconDes/palette.lay>
#include <CtrlCore/lay.h>

class PaletteCtrl : public ColorSelector
{
public:
	typedef PaletteCtrl CLASSNAME;
	PaletteCtrl();

	void     SetTransparent(Color color);
	Color    GetTransparent() const     { return transparent_color; }

public:
	Callback WhenSetTransparent;

private:
	void     OnSetTransparentColor();

private:
	Color    transparent_color;
	WithPaletteTransparentLayout<Ctrl> transparent;
};

#endif//__DESIGNER_PALETTECTRL__
