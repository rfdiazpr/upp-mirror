#ifndef _ButtonStyles_ButtonStyles_h
#define _ButtonStyles_ButtonStyles_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;


namespace ButtonStyles {

	
typedef struct {
	Image buttonIcon; // image à mettre dans le cercle
	Image tmpImg;
	Color opaqueColor; // couleur du cercle au bout du bouton
	Color alphaColor;
	int   subbuttonDeflate;
	int   subbuttonBorderWidth;
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
} ButtonLook;

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


Value MakeButtonLook( int flags, Color opaqueColor, Color alphaColor, int alphaValue, int deflate, int borderWidth, Image buttonIcon);


typedef struct {
	int stylingFlags;
	Color textColor;
	Color opaqueColor;
	Color alphaColor;
	int alphaValue;
	Image buttonIcon;
} StyledButtonStyle;


}



#endif
