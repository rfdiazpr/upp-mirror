#include "Theme.h"

enum {
	NullLookType,
	ImageLookType,
	ColorLookType	
};

void SerializeLook(Stream &s, Value &v) {
	int type;
	if (s.IsStoring()) {
		if(IsNull(v)) {
			type = NullLookType;
			s % type;
		}
		else if(IsType<Color>(v)) {
			Color c = v;
			type = ColorLookType;
			s % type % c;
		}
		else if(IsType<Image>(v)) {
			type = ImageLookType;
			s % type;
			StringStream png;
			Image img = v;
			PNGEncoder().Save(png, img);
			String spng = png;
			s % spng;
			Point p1 = img.GetHotSpot();
			Point p2 = img.Get2ndSpot();
			s % p1 % p2;
		}
	}
	else {
		s % type; 
		if (type == NullLookType) {
			v = Null;
		}
		else if (type == ImageLookType) {
			String png;
			s % png;
			StringStream str(png);
			Image img = PNGRaster().Load(str);
			ASSERT(!IsNull(img));
			Point p1;
			Point p2;
			s % p1 % p2;
			ImageBuffer ib(img);
			ib.SetHotSpot(p1);
			ib.Set2ndSpot(p2);
			v = (Image)ib;
		}
		else if (type == ColorLookType) {
			Color c;
			s % c;
			ASSERT(!IsNull(c));
			v = c;
		}			
	}
};

void Theme::ButtonStyle::Apply()
{
	Button::StyleNormal().Write() = d;
}

void Theme::ButtonStyle::Serialize(Stream& s)
{
	if (s.IsLoading())
		d = Button::StyleNormal(); // Make sure style is initialised
	s % d.ok %d.cancel % d.exit;
	for (int i = 0; i < 4; i++)		
		SerializeLook(s, d.look[i]);
	s % d.focusmargin % d.font % d.overpaint % d.status % d.overpaint % d.transparent;
	for (int i = 0; i < 4; i++)		
		s % d.textcolor[i];
	for (int i = 0; i < 4; i++)		
		s % d.monocolor[i];
}

void Theme::OkButtonStyle::Apply()
{
	Button::StyleOk().Write() = d;
}

void Theme::OkButtonStyle::Serialize(Stream& s)
{
	if (s.IsLoading())
		d = Button::StyleOk(); // Make sure style is initialised
	s % d.ok %d.cancel % d.exit;
	for (int i = 0; i < 4; i++)		
		SerializeLook(s, d.look[i]);
	s % d.focusmargin % d.font % d.overpaint % d.status % d.overpaint % d.transparent;
	for (int i = 0; i < 4; i++)		
		s % d.textcolor[i];
	for (int i = 0; i < 4; i++)		
		s % d.monocolor[i];
}

void Theme::ScrollBarStyle::Apply()
{
	ScrollBar::StyleDefault().Write() = d;
}

void Theme::ScrollBarStyle::Serialize(Stream& s)
{
	if (s.IsLoading())
		d = ScrollBar::StyleDefault();
	for (int i = 0; i < 4; i++)		
		SerializeLook(s, d.up.look[i]);
	for (int i = 0; i < 4; i++)		
		SerializeLook(s, d.vthumb[i]);
	for (int i = 0; i < 4; i++)		
		SerializeLook(s, d.vupper[i]);
	for (int i = 0; i < 4; i++)		
		SerializeLook(s, d.vlower[i]);
	for (int i = 0; i < 4; i++)		
		SerializeLook(s, d.down.look[i]);
	for (int i = 0; i < 4; i++)		
		SerializeLook(s, d.left.look[i]);
	for (int i = 0; i < 4; i++)		
		SerializeLook(s, d.hthumb[i]);
	for (int i = 0; i < 4; i++)		
		SerializeLook(s, d.hupper[i]);
	for (int i = 0; i < 4; i++)		
		SerializeLook(s, d.hlower[i]);
	for (int i = 0; i < 4; i++)		
		SerializeLook(s, d.right.look[i]);
}

void Theme::OptionStyle::Apply()
{
	CtrlsImg::Set(CtrlsImg::I_O0, s0[0]);
	CtrlsImg::Set(CtrlsImg::I_O0h, s0[1]);
	CtrlsImg::Set(CtrlsImg::I_O0p, s0[2]);
	CtrlsImg::Set(CtrlsImg::I_O0d, s0[3]);
	CtrlsImg::Set(CtrlsImg::I_O1, s1[0]);
	CtrlsImg::Set(CtrlsImg::I_O1h, s1[1]);
	CtrlsImg::Set(CtrlsImg::I_O1p, s1[2]);
	CtrlsImg::Set(CtrlsImg::I_O1d, s1[3]);
}

void Theme::OptionStyle::Serialize(Stream& s)
{
	for (int i = 0; i < 4; i++)		
		SerializeLook(s, s0[i]);
	for (int i = 0; i < 4; i++)		
		SerializeLook(s, s1[i]);
}

void Theme::SwitchStyle::Apply()
{
	CtrlsImg::Set(CtrlsImg::I_S0, s0[0]);
	CtrlsImg::Set(CtrlsImg::I_S0h, s0[1]);
	CtrlsImg::Set(CtrlsImg::I_S0p, s0[2]);
	CtrlsImg::Set(CtrlsImg::I_S0d, s0[3]);
	CtrlsImg::Set(CtrlsImg::I_S1, s1[0]);
	CtrlsImg::Set(CtrlsImg::I_S1h, s1[1]);
	CtrlsImg::Set(CtrlsImg::I_S1p, s1[2]);
	CtrlsImg::Set(CtrlsImg::I_S1d, s1[3]);
}

void Theme::SwitchStyle::Serialize(Stream& s)
{
	for (int i = 0; i < 4; i++)		
		SerializeLook(s, s0[i]);
	for (int i = 0; i < 4; i++)		
		SerializeLook(s, s1[i]);
}

void Theme::SpinButtonsStyle::Apply()
{
	SpinButtons::StyleDefault().Write() = d;	
}

void Theme::SpinButtonsStyle::Serialize(Stream& s)
{
	ButtonStyle up;
	ButtonStyle down;
	if (s.IsLoading())
	{
		d = SpinButtons::StyleDefault();
		s % up % down;
		d.inc = up;
		d.dec = down;
	}
	else
	{
		up = d.inc;
		down = d.dec;
		s % up % down;
	}
	s % d.width;
}

void Theme::TabCtrlStyle::Apply()
{
	TabCtrl::StyleDefault().Write() = d;
}

void Theme::TabCtrlStyle::Serialize(Stream& s)
{
	if (s.IsLoading())
		d = TabCtrl::StyleDefault();
	SerializeLook(s, d.body);
	for (int i = 0; i < 4; i++)
		SerializeLook(s, d.normal[i]);
	for (int i = 0; i < 4; i++)
		SerializeLook(s, d.first[i]);
	for (int i = 0; i < 4; i++)
		SerializeLook(s, d.last[i]);
	s % d.sel;	
}

void Theme::EditFieldStyle::Apply()
{
	EditField::StyleDefault().Write() = d;
}

void Theme::EditFieldStyle::Serialize(Stream& s)
{
	if (s.IsLoading())
		d = EditField::StyleDefault();
	for (int i = 0; i < 4; i++)
		SerializeLook(s, d.edge[i]);
	s % d.disabled % d.focus % d.invalid % d.paper;
	s % d.selected % d.selectedtext % d.text % d.textdisabled;
}

void Theme::DropListStyle::Apply()
{
	DropList::StyleDefault().Write() = d;
}

void Theme::DropListStyle::Serialize(Stream& s)
{
	if (s.IsLoading())
		d = DropList::StyleDefault();
	for (int i = 0; i < 4; i++)
		SerializeLook(s, d.trivial[i]);
}

void Theme::ToolBarStyle::Apply()
{
	ToolBar::StyleDefault().Write() = d;
}

void Theme::ToolBarStyle::Serialize(Stream& s)
{
	if (s.IsLoading())
	{
		d = ToolBar::StyleDefault();
		d.buttonstyle = ToolButton::StyleDefault();
	}
	s % d.look % d.arealook;
	for (int i = 0; i < 6; i++)
		SerializeLook(s, d.buttonstyle.look[i]);
}

void Theme::MenuBarStyle::Apply()
{
	MenuBar::StyleDefault().Write() = d;
}

void Theme::MenuBarStyle::Serialize(Stream& s)
{
	if (s.IsLoading())
	{
		d = MenuBar::StyleDefault();
		d.separator = SeparatorCtrl::StyleDefault();
	}
	s % d.look % d.arealook;
	for (int i = 0; i < 3; i++)
		SerializeLook(s, d.topitem[i]);
	SerializeLook(s, d.look);
	SerializeLook(s, d.item);
	for (int i = 0; i < 3; i++)
		s % d.topitemtext[i];
	s % d.itemtext;
	s % d.leftgap;
	SerializeLook(s, d.popupbody);
	SerializeLook(s, d.popupframe);
	SerializeLook(s, d.popupiconbar);
	SerializeLook(s, d.separator.l1);
	SerializeLook(s, d.separator.l2);
}

void Theme::Apply()
{
	SColorFace_Write(colorFace);
	SColorShadow_Write(colorShadow);
	SColorLight_Write(colorLight);
	SColorMark_Write(colorMark);
	
	DropEdge_Write(dropEdge);
	button.Apply();
	okButton.Apply();
	scrollBar.Apply();
	option.Apply();
	switchCtrl.Apply();
	spinButtons.Apply();
	tabCtrl.Apply();
	editField.Apply();
	dropList.Apply();
	toolBar.Apply();
	menuBar.Apply();
}

void Theme::Serialize(Stream& s)
{
	s % colorFace % colorLight % colorMark % colorShadow;
	SerializeLook(s, dropEdge);
	s % button % okButton % scrollBar % option % switchCtrl % spinButtons % tabCtrl;
	s % editField % dropList % toolBar % menuBar;
}