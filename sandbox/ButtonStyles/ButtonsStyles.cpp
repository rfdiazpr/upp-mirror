#include "ButtonStyles.h"

#include <Draw/Draw.h>
#include <Painter/Painter.h>

namespace ButtonStyles {

Value MakeButtonLook( int flags, Color buttoncolor, Color backcolor, int alphaValue, int deflate, int borderWidth, Image buttonIcon )
{
	ButtonLook e;
	e.buttonIcon = buttonIcon;
	e.buttoncolor = buttoncolor;
	e.backcolor = backcolor;
	e.alphaValue = alphaValue;
	e.flags = flags;
	e.deflate = deflate;
	e.borderWidth = borderWidth;
	return RawToValue(e);
}

Value MakeButtonLook( ButtonStyle_style& style )
{
	return RawToValue(style.look);
}



#define TEST(FLAG)  ((e.flags & ButtonLook::FLAG)==ButtonLook::FLAG)

Value ButtonStyleLookFunction(Draw& dw, const Rect& rect, const Value& v, int op)
{
	if( v.Is<ButtonLook>() )
	{
		const ButtonLook& e = v.To<ButtonLook>();
		switch(op)
		{
			case LOOK_MARGINS:
				return rect;
//			Rect(2,2,2,2);
			case LOOK_ISOPAQUE:
				return false;
			case LOOK_PAINT:
			{
				RGBA bckgColor;   bckgColor.r = 0; bckgColor.g = 0; bckgColor.b = 0; bckgColor.a = 0;
				ImageBuffer ib(rect.Size());
				Fill( ib.Begin(), bckgColor, ib.GetLength() );
				BufferPainter w(ib, MODE_ANTIALIASED);

				Rect LeftCircleRect( rect.TopLeft(), Size(rect.Height(), rect.Height()) );
				Rect RightCircleRect = LeftCircleRect;
				RightCircleRect.OffsetHorz(rect.Width()-rect.Height());
				Rect bandRect = rect;

				if (TEST(LEFT_END_ROUND))
				{
					bandRect.left = bandRect.left+bandRect.Height()/2;
					w.DrawEllipse( LeftCircleRect, e.backcolor);
				}
				if (TEST(RIGHT_END_ROUND))
				{
					bandRect.right = bandRect.right-bandRect.Height()/2;
					w.DrawEllipse( RightCircleRect, e.backcolor);
				}
				w.DrawRect(bandRect, e.backcolor);
				
				// apply alpha value
				RGBA* iter = ib.Begin();
				while ( iter < ib.End() ) {
					*iter = Mul8( *iter, e.alphaValue );
					++iter;
				}
				
				bandRect=rect;
				bandRect.Deflate(0, e.deflate);
				LeftCircleRect.Deflate(e.deflate);
				RightCircleRect.Deflate(e.deflate);
				if ( TEST(CONTAINS_SUBBUTON ))
				{
					if ( TEST(SUBBUTTON_RIGHT_END_ROUND ) && !TEST(SUBBUTTON_IS_SHORT_LEFT))
					{
						bandRect.right = bandRect.right-rect.Height()/2;
						if ( TEST(SUBBUTTON_IS_FULL))
						{
							w.DrawEllipse(RightCircleRect, e.buttoncolor);
						}
						else
						{
							if (TEST(SUBBUTTON_IS_SHORT_RIGHT))
							{
								w.DrawEllipse(RightCircleRect, Null, e.borderWidth, e.buttoncolor);
							}
							else
							{
								w.DrawArc(RightCircleRect, RightCircleRect.BottomCenter(), RightCircleRect.TopCenter(), e.borderWidth, e.buttoncolor);
							}
						}
					}
					if (TEST(SUBBUTTON_LEFT_END_ROUND) && !TEST(SUBBUTTON_IS_SHORT_RIGHT))
					{
						bandRect.left = bandRect.left+rect.Height()/2;
						if ( TEST(SUBBUTTON_IS_FULL))
						{
							w.DrawEllipse(LeftCircleRect, e.buttoncolor);
						}
						else
						{
							if (TEST(SUBBUTTON_IS_SHORT_LEFT))
							{
								w.DrawEllipse(LeftCircleRect, Null, e.borderWidth, e.buttoncolor);
							}
							else
							{
								w.DrawArc(LeftCircleRect, LeftCircleRect.TopCenter(), LeftCircleRect.BottomCenter(), e.borderWidth, e.buttoncolor);
							}
						}
					}
					
					if (!(TEST(SUBBUTTON_IS_SHORT_LEFT) || TEST(SUBBUTTON_IS_SHORT_RIGHT)))
					{
						if ( TEST(SUBBUTTON_IS_FULL))
						{
							w.DrawRect(bandRect, e.buttoncolor);
						}
						else
						{
							w.DrawLine(bandRect.TopLeft()   , bandRect.TopRight()   , e.borderWidth, e.buttoncolor);
							w.DrawLine(bandRect.BottomLeft(), bandRect.BottomRight(), e.borderWidth, e.buttoncolor);
						}
					}
				}
				dw.DrawImage(rect, ib);
				
				// DRAW BUTTON ICON
				if ( ! e.buttonIcon.IsEmpty() )
				{
					Point imgPos;
					if (TEST(SUB_ITEM_LEFT))
					{
						imgPos = LeftCircleRect.CenterPos(e.buttonIcon.GetSize());
					}
					else if(TEST(SUB_ITEM_RIGHT))
					{
						imgPos = RightCircleRect.CenterPos(e.buttonIcon.GetSize());
					}
					else // draw image at center
					{
						imgPos = bandRect.CenterPos(e.buttonIcon.GetSize());
					}
					dw.DrawImage( imgPos.x, imgPos.y, e.buttonIcon);
				}
				return 1;
			}
			default:
				return Null;
		}
	}
	return Null;
}
}
INITBLOCK {
	ChLookFn(ButtonStyles::ButtonStyleLookFunction);
}


