#include "Skulpture.h"

#define IMAGECLASS SklSkin
#define IMAGEFILE <Skulpture/Skulpture.iml>
#include <Draw/iml_source.h>

Skulpture::Skulpture()
{
	CreateTheme();
	
	//m.LoadFromFile(m, "c:\\test.utheme");
	//m.Apply();
}

void Skulpture::CreateTheme()
{
	Theme t;
	
	t.colorFace = Color(224, 223, 222);
	t.colorShadow = Color(207, 206, 205);
	t.colorLight = Color(232, 231, 230);
	t.colorMark = Color(20, 19, 18);
	LabelBoxColor_Write(Color(203, 202, 201));
	t.dropEdge = SklSkin::DropEdge();

	CreateButton(t);
	CreateOkButton(t);
	CreateScrollBar(t);
	CreateOption(t);
	CreateSwitch(t);
	CreateSpinButtons(t);
	CreateTabCtrl(t); 
	CreateEditField(t);
	CreateDropList(t);
	CreateToolBar(t);
	CreateMenuBar(t);
	
	t.Apply();
	
	//StoreToFile(t, "c:\\test.utheme");
}

void Skulpture::CreateButton(Theme& t)
{
	Button::Style s = Button::StyleNormal();
	s.look[0] = SklSkin::Button();
	s.look[1] = SklSkin::ButtonH();
	s.look[2] = SklSkin::ButtonP();
	s.look[3] = SklSkin::ButtonD();
	t.button = s;
}

void Skulpture::CreateOkButton(Theme& t)
{
	Button::Style s = Button::StyleOk();
	s.look[0] = SklSkin::ButtonOk();
	s.look[1] = SklSkin::ButtonOkH();
	s.look[2] = SklSkin::ButtonOkP();
	s.look[3] = SklSkin::ButtonOkD();
	t.okButton = s;
}

void Skulpture::CreateScrollBar(Theme& t)
{
	ScrollBar::Style ss = ScrollBar::StyleDefault();
	ss.up.look[0] = SklSkin::ScrollUp();
	ss.up.look[1] = SklSkin::ScrollUpH();
	ss.up.look[2] = SklSkin::ScrollUpP();
	ss.up.look[3] = SklSkin::ScrollUpD();
		
	ss.vthumb[0] = SklSkin::ScrollVThumb();
	ss.vthumb[1] = SklSkin::ScrollVThumbH();
	ss.vthumb[2] = SklSkin::ScrollVThumbP();
	ss.vthumb[3] = SklSkin::ScrollVThumbD();
	
	ss.vupper[0] = SklSkin::ScrollVUpper();
	ss.vupper[1] = SklSkin::ScrollVUpperH();
	ss.vupper[2] = SklSkin::ScrollVUpperP();
	ss.vupper[3] = SklSkin::ScrollVUpperD();
	
	ss.vlower[0] = SklSkin::ScrollVLower();
	ss.vlower[1] = SklSkin::ScrollVLowerH();
	ss.vlower[2] = SklSkin::ScrollVLowerP();
	ss.vlower[3] = SklSkin::ScrollVLowerD();

	ss.down.look[0] = SklSkin::ScrollDown();
	ss.down.look[1] = SklSkin::ScrollDownH();
	ss.down.look[2] = SklSkin::ScrollDownP();
	ss.down.look[3] = SklSkin::ScrollDownD();
	
	ss.left.look[0] = SklSkin::ScrollLeft();
	ss.left.look[1] = SklSkin::ScrollLeftH();
	ss.left.look[2] = SklSkin::ScrollLeftP();
	ss.left.look[3] = SklSkin::ScrollLeftD();
	
	ss.hthumb[0] = SklSkin::ScrollHThumb();
	ss.hthumb[1] = SklSkin::ScrollHThumbH();
	ss.hthumb[2] = SklSkin::ScrollHThumbP();
	ss.hthumb[3] = SklSkin::ScrollHThumbD();
	
	ss.hupper[0] = SklSkin::ScrollHUpper();
	ss.hupper[1] = SklSkin::ScrollHUpperH();
	ss.hupper[2] = SklSkin::ScrollHUpperP();
	ss.hupper[3] = SklSkin::ScrollHUpperD();
	
	ss.hlower[0] = SklSkin::ScrollHLower();
	ss.hlower[1] = SklSkin::ScrollHLowerH();
	ss.hlower[2] = SklSkin::ScrollHLowerP();
	ss.hlower[3] = SklSkin::ScrollHLowerD();
	
	ss.right.look[0] = SklSkin::ScrollRight();
	ss.right.look[1] = SklSkin::ScrollRightH();	
	ss.right.look[2] = SklSkin::ScrollRightP();
	ss.right.look[3] = SklSkin::ScrollRightD();	
	
	t.scrollBar = ss;
}

void Skulpture::CreateOption(Theme& t)
{
	t.option.s0[0] = SklSkin::Option();
	t.option.s0[1] = SklSkin::OptionH();
	t.option.s0[2] = SklSkin::OptionP();
	t.option.s0[3] = SklSkin::OptionD();
	t.option.s1[0] = SklSkin::OptionChk();
	t.option.s1[1] = SklSkin::OptionChkH();
	t.option.s1[2] = SklSkin::OptionChkP();
	t.option.s1[3] = SklSkin::OptionChkD();
}

void Skulpture::CreateSwitch(Theme& t)
{
	t.switchCtrl.s0[0] = SklSkin::Switch();
	t.switchCtrl.s0[1] = SklSkin::SwitchH();
	t.switchCtrl.s0[2] = SklSkin::SwitchP();
	t.switchCtrl.s0[3] = SklSkin::SwitchD();
	t.switchCtrl.s1[0] = SklSkin::SwitchChk();
	t.switchCtrl.s1[1] = SklSkin::SwitchChkH();
	t.switchCtrl.s1[2] = SklSkin::SwitchChkP();
	t.switchCtrl.s1[3] = SklSkin::SwitchChkD();
}

void Skulpture::CreateSpinButtons(Theme& t)
{
	SpinButtons::Style be = SpinButtons::StyleDefault();
	be.inc.look[0] = SklSkin::SpinUp();
	be.inc.look[1] = SklSkin::SpinUpH();
	be.inc.look[2] = SklSkin::SpinUpH();
	be.inc.look[3] = SklSkin::SpinUp();
	be.dec.look[0] = SklSkin::SpinDown();
	be.dec.look[1] = SklSkin::SpinDownH();
	be.dec.look[2] = SklSkin::SpinDownH();
	be.dec.look[3] = SklSkin::SpinDown();
	t.spinButtons = be;
}

void Skulpture::CreateTabCtrl(Theme& t)
{
	TabCtrl::Style tab = TabCtrl::StyleDefault();
	tab.body = SklSkin::TabBody();
	tab.normal[0] = SklSkin::Tab(); 
	tab.normal[1] = SklSkin::TabH();	
	tab.normal[2] = SklSkin::ATab();
	tab.normal[3] = SklSkin::ATab();
	for (int i = 0; i < 4; i++)
		tab.first[i] = tab.normal[i]; 
	for (int i = 0; i < 4; i++)
		tab.last[i] = tab.normal[i]; 
	tab.sel.top = 0;
	
	t.tabCtrl = tab;
}

void Skulpture::CreateEditField(Theme& t)
{
	EditField::Style es = EditField::StyleDefault();
	es.edge[0] = SklSkin::Edit();
	es.edge[1] = SklSkin::Edit();
	es.edge[2] = SklSkin::Edit();
	es.edge[3] = SklSkin::Edit();
	
	t.editField = es;
}

void Skulpture::CreateDropList(Theme& t)
{
	DropList::Style bls = DropList::StyleDefault().Write();
	bls.trivial[0] = SklSkin::MultiRight();
	bls.trivial[1] = SklSkin::MultiRightH();
	bls.trivial[2] = SklSkin::MultiRightP();
	bls.trivial[3] = SklSkin::MultiRightH();
	
	t.dropList = bls;	
}
	
void Skulpture::CreateToolBar(Theme& t)
{
	ToolBar::Style tbs = ToolBar::StyleDefault();
	tbs.look = Color(224, 223, 222);
	tbs.arealook = Null;
	tbs.buttonstyle.look[0] = SklSkin::Tool();
	tbs.buttonstyle.look[1] = SklSkin::ToolH();
	tbs.buttonstyle.look[2] = SklSkin::ToolP();
	tbs.buttonstyle.look[3] = SklSkin::ToolD();
	tbs.buttonstyle.look[4] = SklSkin::ToolP();
	tbs.buttonstyle.look[5] = SklSkin::ToolP();
	
	t.toolBar = tbs;
}

void Skulpture::CreateMenuBar(Theme& t)
{
	MenuBar::Style mbs = MenuBar::StyleDefault();
	mbs.look = Color(224, 223, 222);
	mbs.item = SklSkin::MenuH();
	mbs.topitem[0] = SklSkin::Menu();
	mbs.topitem[1] = SklSkin::MenuH();
	mbs.topitem[2] = SklSkin::MenuP();
	mbs.topitemtext[0] = mbs.topitemtext[1] = mbs.topitemtext[2] = Black();
	mbs.itemtext = Black();
	mbs.leftgap = 22;
	mbs.popupiconbar = Color(215, 214, 213); 
	mbs.popupbody = Color(224, 223, 222);
	mbs.popupframe = SklSkin::MenuFrame();
	mbs.arealook = Null;
	mbs.separator.l1 = Color(207, 206, 205);
	mbs.separator.l2 = Color(232, 231, 230);
	
	t.menuBar = mbs;
}
