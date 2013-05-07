#ifndef _ButtonStyles_ButtonStyles_h
#define _ButtonStyles_ButtonStyles_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;


namespace ButtonStyles {

typedef enum  {
	NORMAL = 0,
	HOT,
	PRESSED,
	DISABLED,
} ButtonPushState;

	
class ButtonLook {
	public:
	Image buttonIcon; // image à mettre dans le cercle
	Image tmpImg;
	Color buttoncolor; // couleur du cercle au bout du bouton
	Color backcolor;
	int   deflate;
	int   borderWidth;
	int   alphaValue;
	int   flags;
	enum {
		LEFT_END_ROUND             = 0x0001,
		RIGHT_END_ROUND            = 0x0002,
		CONTAINS_SUBBUTON          = 0x0004,
		SUBBUTTON_IS_FULL          = 0x0008|CONTAINS_SUBBUTON,
		SUBBUTTON_LEFT_END_ROUND   = 0x0010|CONTAINS_SUBBUTON,
		SUBBUTTON_RIGHT_END_ROUND  = 0x0020|CONTAINS_SUBBUTON,
		SUB_ITEM_LEFT              = 0x0040,
		SUB_ITEM_RIGHT             = 0x0080,
		
		SUBBUTTON_IS_SHORT_LEFT    = SUB_ITEM_LEFT|CONTAINS_SUBBUTON,
		SUBBUTTON_IS_SHORT_RIGHT   = SUB_ITEM_RIGHT|CONTAINS_SUBBUTON,

		TEXT_USED_AS_MASK          = 0x8000,

		BOTH_ENDS_ROUND            = LEFT_END_ROUND | RIGHT_END_ROUND,
		BOTH_SUBBUTTON_ENDS_ROUND  = SUBBUTTON_LEFT_END_ROUND | SUBBUTTON_RIGHT_END_ROUND,
		ALL_ENDS_ROUND             = BOTH_ENDS_ROUND | BOTH_SUBBUTTON_ENDS_ROUND
	};
	
	ButtonLook() {
		buttonIcon = Null; // image à mettre dans le cercle
		buttoncolor = White(); // couleur du cercle au bout du bouton
		backcolor = WhiteGray();
		deflate = 6;
		borderWidth = 2;
		alphaValue = 50;
		flags = ButtonLook::ALL_ENDS_ROUND | ButtonLook::SUBBUTTON_IS_FULL;
	}
} ;


class ButtonStyle_style {
	public:
	ButtonLook look;
	Color textColor;
	
	ButtonStyle_style() {
		textColor = Black();
	}
};

enum {
	BUTTON_STYLE_0a = 0,
	BUTTON_STYLE_0b = ButtonLook::SUB_ITEM_LEFT,
	BUTTON_STYLE_0c = ButtonLook::SUB_ITEM_RIGHT,

	// NO SUBBUTTON ( ICON CENTER )
	BUTTON_STYLE_1a = ButtonLook::LEFT_END_ROUND,
	BUTTON_STYLE_1b = ButtonLook::RIGHT_END_ROUND,
	BUTTON_STYLE_1c = ButtonLook::BOTH_ENDS_ROUND,
	// NO SUBBUTTON ( ICON LEFT )
	BUTTON_STYLE_1aL = ButtonLook::LEFT_END_ROUND|ButtonLook::SUB_ITEM_LEFT,
	BUTTON_STYLE_1bL = ButtonLook::RIGHT_END_ROUND|ButtonLook::SUB_ITEM_LEFT,
	BUTTON_STYLE_1cL = ButtonLook::BOTH_ENDS_ROUND|ButtonLook::SUB_ITEM_LEFT,
	// NO SUBBUTTON ( ICON RIGHT )
	BUTTON_STYLE_1aR = ButtonLook::LEFT_END_ROUND|ButtonLook::SUB_ITEM_RIGHT,
	BUTTON_STYLE_1bR = ButtonLook::RIGHT_END_ROUND|ButtonLook::SUB_ITEM_RIGHT,
	BUTTON_STYLE_1cR = ButtonLook::BOTH_ENDS_ROUND|ButtonLook::SUB_ITEM_RIGHT,
	
	// FULL  SUBBUTTON
	BUTTON_STYLE_2a = ButtonLook::LEFT_END_ROUND |ButtonLook::SUBBUTTON_IS_FULL|ButtonLook::SUBBUTTON_LEFT_END_ROUND,
	BUTTON_STYLE_2b = ButtonLook::RIGHT_END_ROUND|ButtonLook::SUBBUTTON_IS_FULL|ButtonLook::SUBBUTTON_RIGHT_END_ROUND,
	BUTTON_STYLE_2c = ButtonLook::LEFT_END_ROUND |ButtonLook::SUBBUTTON_IS_FULL|ButtonLook::BOTH_SUBBUTTON_ENDS_ROUND,
	BUTTON_STYLE_2d = ButtonLook::RIGHT_END_ROUND|ButtonLook::SUBBUTTON_IS_FULL|ButtonLook::BOTH_SUBBUTTON_ENDS_ROUND,
	BUTTON_STYLE_2e = ButtonLook::LEFT_END_ROUND |ButtonLook::SUBBUTTON_IS_FULL|ButtonLook::SUBBUTTON_LEFT_END_ROUND |ButtonLook::SUBBUTTON_IS_SHORT_LEFT,
	BUTTON_STYLE_2f = ButtonLook::RIGHT_END_ROUND|ButtonLook::SUBBUTTON_IS_FULL|ButtonLook::SUBBUTTON_RIGHT_END_ROUND|ButtonLook::SUBBUTTON_IS_SHORT_RIGHT,
	BUTTON_STYLE_2g = ButtonLook::ALL_ENDS_ROUND |ButtonLook::SUBBUTTON_IS_FULL,
	BUTTON_STYLE_2h = ButtonLook::ALL_ENDS_ROUND |ButtonLook::SUBBUTTON_IS_FULL|ButtonLook::SUBBUTTON_IS_SHORT_LEFT,
	BUTTON_STYLE_2i = ButtonLook::ALL_ENDS_ROUND |ButtonLook::SUBBUTTON_IS_FULL|ButtonLook::SUBBUTTON_IS_SHORT_RIGHT,

	// EMPTY SUBBUTTON
	BUTTON_STYLE_3a  = ButtonLook::LEFT_END_ROUND |ButtonLook::SUBBUTTON_LEFT_END_ROUND,
	BUTTON_STYLE_3b  = ButtonLook::RIGHT_END_ROUND|ButtonLook::SUBBUTTON_RIGHT_END_ROUND,
	BUTTON_STYLE_3c  = ButtonLook::LEFT_END_ROUND |ButtonLook::BOTH_SUBBUTTON_ENDS_ROUND,
	BUTTON_STYLE_3d  = ButtonLook::RIGHT_END_ROUND|ButtonLook::BOTH_SUBBUTTON_ENDS_ROUND,
	BUTTON_STYLE_3e  = ButtonLook::LEFT_END_ROUND |ButtonLook::SUBBUTTON_LEFT_END_ROUND |ButtonLook::SUBBUTTON_IS_SHORT_LEFT,
	BUTTON_STYLE_3f  = ButtonLook::RIGHT_END_ROUND|ButtonLook::SUBBUTTON_RIGHT_END_ROUND|ButtonLook::SUBBUTTON_IS_SHORT_RIGHT,
	BUTTON_STYLE_3g  = ButtonLook::ALL_ENDS_ROUND,
	BUTTON_STYLE_3h  = ButtonLook::ALL_ENDS_ROUND |ButtonLook::SUBBUTTON_IS_SHORT_LEFT,
	BUTTON_STYLE_3i  = ButtonLook::ALL_ENDS_ROUND |ButtonLook::SUBBUTTON_IS_SHORT_RIGHT,


};



Value MakeButtonLook( int flags, Color buttoncolor, Color backcolor, int alphaValue, int deflate, int borderWidth, Image buttonIcon);
Value MakeButtonLook( ButtonStyle_style& style );





// Accesseur permettant de recuperer le style par defaut en fonction du type passé
template<class STYLE>
inline STYLE  GetInitStyle() { return STYLE(); };

template<>  inline Button::Style  GetInitStyle<Button::Style>() { return Button::StyleNormal(); };
template<>  inline ButtonOption::Style  GetInitStyle<ButtonOption::Style>() { return ButtonOption::StyleDefault(); };



template <class STYLE>
inline STYLE toChameleon(ButtonStyle_style* styles)
{
	STYLE s = GetInitStyle<STYLE>();
	s.look[0] = MakeButtonLook( styles[0] );
	s.look[1] = MakeButtonLook( styles[1] );
	s.look[2] = MakeButtonLook( styles[2] );
	s.look[3] = MakeButtonLook( styles[3] );
	s.textcolor[0] = styles[0].textColor;
	s.textcolor[1] = styles[1].textColor;
	s.textcolor[2] = styles[2].textColor;
	s.textcolor[3] = styles[3].textColor;
	return s;
}


// -------------------------
// Skinned buttons template
// -------------------------
template <class BUTTON>
class TStyledButton : public BUTTON {
	private:
		typename BUTTON::Style bChameleon;
		ButtonStyle_style  styles[4];

	public:
		typedef TStyledButton<BUTTON>  CLASSNAME;
		
		TStyledButton() {
			bChameleon = toChameleon<typename BUTTON::Style>(styles);
			BUTTON::SetStyle(bChameleon);
		}

		CLASSNAME& pushState(int c) { return *this; } // dummy function
		
		#define  MAKE_STYLE_FN(N, PREFIX) \
			CLASSNAME& PREFIX##_color(Color  c)    { styles[N-1].look.buttoncolor = c; bChameleon = toChameleon<typename BUTTON::Style>( styles ); return *this; }\
			CLASSNAME& PREFIX##_backColor(Color c) { styles[N-1].look.backcolor = c;   bChameleon = toChameleon<typename BUTTON::Style>( styles ); return *this; }\
			CLASSNAME& PREFIX##_textColor(Color c) { styles[N-1].textColor = c;        bChameleon = toChameleon<typename BUTTON::Style>( styles ); return *this; }\
			CLASSNAME& PREFIX##_flags(int c)       { styles[N-1].look.flags = c;       bChameleon = toChameleon<typename BUTTON::Style>( styles ); return *this; }\
			CLASSNAME& PREFIX##_alpha(int c)       { styles[N-1].look.alphaValue = c;  bChameleon = toChameleon<typename BUTTON::Style>( styles ); return *this; }\
			CLASSNAME& PREFIX##_deflate(int c)     { styles[N-1].look.deflate = c;     bChameleon = toChameleon<typename BUTTON::Style>( styles ); return *this; }\
			CLASSNAME& PREFIX##_borderWidth(int c) { styles[N-1].look.borderWidth = c; bChameleon = toChameleon<typename BUTTON::Style>( styles ); return *this; }\
			CLASSNAME& PREFIX##_setIcon(Image c)   { styles[N-1].look.buttonIcon = c;  bChameleon = toChameleon<typename BUTTON::Style>( styles ); return *this; }

		MAKE_STYLE_FN( 1, NORM )
		MAKE_STYLE_FN( 2, HOT )
		MAKE_STYLE_FN( 3, PRESS )
		MAKE_STYLE_FN( 4, DISAB )
		#undef MAKE_STYLE_FN

		void SetIcon(Image icon) {
			styles[0].look.buttonIcon = icon;
			styles[1].look.buttonIcon = icon;
			styles[2].look.buttonIcon = icon;
			styles[3].look.buttonIcon = icon;
			bChameleon = toChameleon<typename BUTTON::Style>( styles );
			return *this;
		}

		CLASSNAME&  SetFont(Font fnt)          { BUTTON::SetFont(fnt); return *this; }
		CLASSNAME&  SetLabel(const char *text) { BUTTON::SetLabel(text); return *this; }
		CLASSNAME&  Tip(const char *text)      { BUTTON::Tip(text); return *this; }
		CLASSNAME&  ClickFocus(bool cf = true) { BUTTON::ClickFocus(cf); return *this; }
		CLASSNAME&  NoClickFocus()             { BUTTON::NoClickFocus(); return *this; }

};


}



#endif
