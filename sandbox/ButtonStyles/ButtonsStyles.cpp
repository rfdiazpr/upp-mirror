#include "ButtonStyles.h"

#include <Draw/Draw.h>
#include <Painter/Painter.h>

namespace ButtonStyles {

Value MakeButtonLook( int flags, Color opaqueColor, Color alphaColor, int alphaValue, int deflate, int borderWidth, Image buttonIcon )
{
	ButtonLook e;
	e.buttonIcon = buttonIcon;
	e.opaqueColor = opaqueColor;
	e.alphaColor = alphaColor;
	e.alphaValue = alphaValue;
	e.flags = flags;
	e.subbuttonDeflate = deflate;
	e.subbuttonBorderWidth = borderWidth;
	return RawToValue(e);
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
				Rect LeftCircleRect( rect.TopLeft(), Size(rect.Height(), rect.Height()) );
				Rect RightCircleRect = LeftCircleRect;
				RightCircleRect.OffsetHorz(rect.Width()-rect.Height());
				Rect bandRect = rect;
				ImageDraw iw( rect.Size() );

				if (TEST(LEFT_END_ROUND))
				{
					bandRect.left = bandRect.left+bandRect.Height()/2;
					iw.Alpha().DrawEllipse( LeftCircleRect, GrayColor(e.alphaValue));
				}
				if (TEST(RIGHT_END_ROUND))
				{
					bandRect.right = bandRect.right-bandRect.Height()/2;
					iw.Alpha().DrawEllipse( RightCircleRect, GrayColor(e.alphaValue));
				}
				bandRect.Deflate(0,1); // to have same size as circles
				iw.Alpha().DrawRect(bandRect, GrayColor(e.alphaValue));
				dw.DrawImage( rect, iw , e.alphaColor);

//#define DO_ANTIALIASING 1
				#if DO_ANTIALIASING
				ImageBuffer ib(rect.Size());
				BufferPainter w(ib, MODE_ANTIALIASED);
				#else
				#define w dw
				#endif


				bandRect=rect;
				bandRect.Deflate(0, e.subbuttonDeflate);
				LeftCircleRect.Deflate(e.subbuttonDeflate);
				RightCircleRect.Deflate(e.subbuttonDeflate);
				if ( TEST(CONTAINS_SUBBUTON ))
				{
					
					if ( TEST(SUBBUTTON_RIGHT_END_ROUND ) && !TEST(SUBBUTTON_IS_SHORT_LEFT))
					{
						bandRect.right = bandRect.right-rect.Height()/2;
						if ( TEST(SUBBUTTON_IS_FULL))
						{
							w.DrawEllipse(RightCircleRect, e.opaqueColor);
						}
						else
						{
							if (TEST(SUBBUTTON_IS_SHORT_RIGHT))
							{
								w.DrawEllipse(RightCircleRect, Null, e.subbuttonBorderWidth, e.opaqueColor);
							}
							else
							{
								w.DrawArc(RightCircleRect, RightCircleRect.BottomCenter(), RightCircleRect.TopCenter(), e.subbuttonBorderWidth, e.opaqueColor);
							}
						}
					}
					if (TEST(SUBBUTTON_LEFT_END_ROUND) && !TEST(SUBBUTTON_IS_SHORT_RIGHT))
					{
						bandRect.left = bandRect.left+rect.Height()/2;
						if ( TEST(SUBBUTTON_IS_FULL))
						{
							w.DrawEllipse(LeftCircleRect, e.opaqueColor);
						}
						else
						{
							if (TEST(SUBBUTTON_IS_SHORT_LEFT))
							{
								w.DrawEllipse(LeftCircleRect, Null, e.subbuttonBorderWidth, e.opaqueColor);
							}
							else
							{
								w.DrawArc(LeftCircleRect, LeftCircleRect.TopCenter(), LeftCircleRect.BottomCenter(), e.subbuttonBorderWidth, e.opaqueColor);
							}
						}
					}
					
					if (!(TEST(SUBBUTTON_IS_SHORT_LEFT) || TEST(SUBBUTTON_IS_SHORT_RIGHT)))
					{
						if ( TEST(SUBBUTTON_IS_FULL))
						{
							w.DrawRect(bandRect, e.opaqueColor);
						}
						else
						{
							w.DrawLine(bandRect.TopLeft()   , bandRect.TopRight()   , e.subbuttonBorderWidth, e.opaqueColor);
							w.DrawLine(bandRect.BottomLeft(), bandRect.BottomRight(), e.subbuttonBorderWidth, e.opaqueColor);
						}
					}
					#if DO_ANTIALIASING
					dw.DrawImage(rect, ib);
					#endif
				}
				
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


