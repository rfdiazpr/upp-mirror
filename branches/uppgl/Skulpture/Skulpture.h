#ifndef _Skulpture_Skulpture_h
#define _Skulpture_Skulpture_h

#include <CtrlLib/CtrlLib.h>
#include <Theme/Theme.h>

using namespace Upp;

#define IMAGECLASS SklSkin
#define IMAGEFILE <Skulpture/Skulpture.iml>
#include <Draw/iml_header.h>

class Skulpture {
public:
	typedef Skulpture CLASSNAME;
	Skulpture();

	Theme m;
	
	void CreateTheme();
	void CreateButton(Theme& t);
	void CreateOkButton(Theme& t);
	void CreateScrollBar(Theme& t);
	void CreateOption(Theme& t);
	void CreateSwitch(Theme& t);
	void CreateSpinButtons(Theme& t);
	void CreateTabCtrl(Theme& t);
	void CreateEditField(Theme& t);
	void CreateDropList(Theme& t);
	void CreateToolBar(Theme& t);
	void CreateMenuBar(Theme& t);	
};

#endif

