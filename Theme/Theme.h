#ifndef _Skulpture_Theme_h_
#define _Skulpture_Theme_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class Theme
{
public:
	class ButtonStyle
	{
	public:
		void Apply();
		
		void operator =(const Button::Style& s)						{ d = s; }
		operator const Button::Style()								{ return d; } 
		
		void Serialize(Stream& s);
		
	protected:
		Button::Style d;
	};
	
	class OkButtonStyle: public ButtonStyle
	{
	public:
		void Apply();
		
		void operator =(const Button::Style& s)						{ d = s; }
		operator const Button::Style()								{ return d; } 
		
		void Serialize(Stream& s);
	};
	
	class ScrollBarStyle
	{
	public:
		void Apply();
		
		void operator =(const ScrollBar::Style& s)					{ d = s; }
		operator const ScrollBar::Style()							{ return d; } 
		
		void Serialize(Stream& s);
		
	protected:
		ScrollBar::Style d;
	};
	
	class OptionStyle
	{
	public:
		Value s0[4];
		Value s1[4];
		Value s2[4];
				
		void Apply();
		
		void Serialize(Stream& s);
	};
	
	class SwitchStyle
	{
	public:
		Value s0[4];
		Value s1[4];
		
		void Apply();
		
		void Serialize(Stream& s);
	};
	
	class SpinButtonsStyle
	{
	public:
		void Apply();
		
		void operator =(const SpinButtons::Style& s)				{ d = s; }
		operator const SpinButtons::Style()							{ return d; } 
		
		void Serialize(Stream& s);
		
	protected:
		SpinButtons::Style d;
	};
	
	class TabCtrlStyle
	{
	public:
		void Apply();
		
		void operator =(const TabCtrl::Style& s)					{ d = s; }
		operator const TabCtrl::Style()								{ return d; } 
		
		void Serialize(Stream& s);
		
	protected:
		TabCtrl::Style d;
	};
	
	class EditFieldStyle
	{
	public:
		void Apply();
		
		void operator =(const EditField::Style& s)					{ d = s; }
		operator const EditField::Style()							{ return d; } 
		
		void Serialize(Stream& s);
		
	protected:
		EditField::Style d;
	};
	
	class DropListStyle
	{
	public:
		void Apply();
		
		void operator =(const DropList::Style& s)					{ d = s; }
		operator const DropList::Style()							{ return d; } 
		
		void Serialize(Stream& s);
		
	protected:
		DropList::Style d;
	};
	
	class ToolBarStyle
	{
	public:
		void Apply();
		
		void operator =(const ToolBar::Style& s)					{ d = s; }
		operator const ToolBar::Style()							{ return d; } 
		
		void Serialize(Stream& s);
		
	protected:
		ToolBar::Style d;
	};
	
	class MenuBarStyle
	{
	public:
		void Apply();
		
		void operator =(const MenuBar::Style& s)					{ d = s; }
		operator const MenuBar::Style()							{ return d; } 
		
		void Serialize(Stream& s);
		
	protected:
		MenuBar::Style d;
	};
	
	Color colorFace;
	Color colorShadow;
	Color colorLight;
	Color colorMark;
	
	Value dropEdge;
	
	ButtonStyle button;
	OkButtonStyle okButton;
	ScrollBarStyle scrollBar;
	OptionStyle option;
	SwitchStyle switchCtrl;
	SpinButtonsStyle spinButtons;
	TabCtrlStyle tabCtrl;
	EditFieldStyle editField;
	DropListStyle dropList;
	ToolBarStyle toolBar;
	MenuBarStyle menuBar;
	
	void Apply();
	void Serialize(Stream& s);
};

#endif
