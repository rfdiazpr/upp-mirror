#ifndef _GraphDraw_TickMark_h_
#define _GraphDraw_TickMark_h_



namespace GraphDraw_ns
{
	class TickMark {
		private:
			const Value nullVal;
			int _tickLength;
			
		public:
			TickMark() : nullVal(Null), _tickLength(2) { UpdateTick(); }
			virtual ~TickMark() {}

			virtual void Paint(Draw &w, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const = 0;
			inline void Paint(Draw &p, ElementPosition axisPos, const int scale, const Point& cp, const Color& markColor) const { Paint(p, axisPos, scale, cp.x, cp.y, markColor); }

			virtual void Paint(Painter &p, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const = 0;
			inline void Paint( Painter &p, ElementPosition axisPos, const int scale, const Point& cp, const Color& markColor) const { Paint(p, axisPos, scale, cp.x, cp.y, markColor); }

			inline int GetTickLength() const      { return _tickLength; }
			inline TickMark* SetTickLength(int v) { _tickLength = v; UpdateTick(); return this; }

			virtual void UpdateTick() {}; // called when tick drawing needs to be recalculated
			
			virtual Rect GetTickRect(Point xyOffset, ElementPosition side, int x, int y, const int scale) const = 0;

			virtual Size GetTickSize(ElementPosition side, const int scale) const = 0;

			bool Contains(Point p, Point xyOffset, ElementPosition side, int x, int y, const int scale = 1) const {
				return GetTickRect(xyOffset, side, x, y, scale ).Contains(p);
			}
	};

	class RoundTickMark  : public TickMark {
		public:
			RoundTickMark() { SetTickLength(5);	}
			virtual ~RoundTickMark() {}

			virtual void Paint(Draw &w, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const {
				int diam = fround(scale*GetTickLength()*2);
				int radius = diam/2;
				w.DrawEllipse(x - radius, y - radius, diam, diam, markColor, 1, markColor);
			}

			virtual void Paint(Painter &p, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const {
			}
			
			virtual Rect GetTickRect(Point xyOffset, ElementPosition side, int x, int y, const int scale) const
			{
				int diam = fround(scale*GetTickLength()*2);
				x += xyOffset.x - diam/2;
				y += xyOffset.y - diam/2;
				return Rect( Point(x,y), Size(diam, diam) );
				return Null;
			}
			
			virtual Size GetTickSize(ElementPosition side, const int scale) const 
			{
				int diam = fround(scale*GetTickLength()*2);
				return Size(diam, diam);
			}

	};

	class TriangleTickMark  : public TickMark {
		public:
			TriangleTickMark() { SetTickLength(5);	}
			virtual ~TriangleTickMark() {}

			virtual void Paint(Draw &dw, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const {
				Upp::Vector<Point> p;
				p << Point(x, y);
				const int scOffset = 2*scale;
				const int scTickLength = GetTickLength()*scale;
				if (axisPos==LEFT_OF_GRAPH)	{
					p << Point(x-scTickLength, y-scOffset) << Point(x-scTickLength, y+scOffset);
				} else if (axisPos==RIGHT_OF_GRAPH)	{
					p << Point(x+scTickLength, y-scOffset) << Point(x+scTickLength, y+scOffset);
				} else if (axisPos==BOTTOM_OF_GRAPH) {
					p << Point(x-scOffset, y+scTickLength) << Point(x+scOffset, y+scTickLength);
				} else {
					p << Point(x-scOffset, y-scTickLength) << Point(x+scOffset, y-scTickLength);
				}
				p << Point(x, y);
				dw.DrawPolygon( p, markColor, scale/2, markColor);
			}

			virtual void Paint(Painter &p, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const {
			}

			virtual Rect GetTickRect(Point xyOffset, ElementPosition side, int x, int y, const int scale) const
			{
				return Null;
			}
			
			virtual Size GetTickSize(ElementPosition side, const int scale) const 
			{
				return Null;
			}

	};

	class LineTickMark  : public TickMark {
		public:
			LineTickMark() { SetTickLength(3);	}
			virtual ~LineTickMark() {}

			virtual void Paint(Draw &dw, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const {
				const int scTickLength = GetTickLength()*scale;
				switch(axisPos){
					case LEFT_OF_GRAPH:
					case RIGHT_OF_GRAPH:
						dw.DrawLine(x-scTickLength, y, x+scTickLength, y, 2*scale, markColor);
						break;
					case TOP_OF_GRAPH:
					case BOTTOM_OF_GRAPH:
					default:
						dw.DrawLine(x, y-scTickLength, x, y+scTickLength, 2*scale, markColor);
						break;
				}
			}

			virtual void Paint(Painter &p, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const {
			}

			virtual Rect GetTickRect(Point xyOffset, ElementPosition axisPos, int x, int y, const int scale) const
			{
				x += xyOffset.x;
				y += xyOffset.y;
				const int scTickLength = GetTickLength()*scale;
				Rect rect;
				switch(axisPos){
					case LEFT_OF_GRAPH:
					case RIGHT_OF_GRAPH:
						rect = Rect(Point(x-scTickLength, y-2), Size(scTickLength, 4) );
						break;
					case TOP_OF_GRAPH:
					case BOTTOM_OF_GRAPH:
					default:
						rect = Rect(Point(x-2, y-scTickLength), Size(4, scTickLength) );
						break;
				}
				return rect;
			}

			virtual Size GetTickSize(ElementPosition axisPos, const int scale) const 
			{
				const int scTickLength = GetTickLength()*scale;
				Size sz;
				switch(axisPos) {
					case LEFT_OF_GRAPH:
					case RIGHT_OF_GRAPH:
						sz = Size(scTickLength, 4);
						break;
					case TOP_OF_GRAPH:
					case BOTTOM_OF_GRAPH:
					default:
						sz = Size(4, scTickLength);
						break;
				}
				return sz;
			}
	};


	class RectTriangleTickMark  : public TickMark {
		public:
			Font tickIdFont;
			String IDtext;
			Color  textColor;
			Color  textBckgndColor;
			Color  OutlineColor;
		
		public:
		
			RectTriangleTickMark() : textColor(White), textBckgndColor(Red), OutlineColor(Black) { SetTickLength(15);	}
			virtual ~RectTriangleTickMark() {}

			virtual void UpdateTick() {
				tickIdFont.Height(GetTickLength()/2);
			}

			virtual Size GetTickSize(ElementPosition side, const int scale) const 
			{
				const int halfHeight  = GetTickLength()/3 * scale;
				const int totalLength = GetTickLength() * scale;
				switch (side) {
					case LEFT_OF_GRAPH:   return Size(totalLength, halfHeight*2); break;
					case RIGHT_OF_GRAPH:  return Size(totalLength, halfHeight*2); break;
					case BOTTOM_OF_GRAPH: return Size(halfHeight*2, totalLength); break;
					case TOP_OF_GRAPH:    return Size(halfHeight*2, totalLength); break;
					default:  break;
				}
				return Null;
			}

			virtual Rect GetTickRect(Point xyOffset, ElementPosition side, int x, int y, const int scale) const
			{
				const int halfHeight  = GetTickLength()/3 * scale;
				const int totalLength = GetTickLength() * scale;
				x += xyOffset.x;
				y += xyOffset.y;
				Rect markRect;
				switch (side) {
					case LEFT_OF_GRAPH:
						markRect = Rect( Point(x-totalLength, y-halfHeight), Size(totalLength, halfHeight*2) );
						break;
					case RIGHT_OF_GRAPH:
						markRect = Rect( Point(x, y-halfHeight), Size(totalLength, halfHeight*2) );
						break;
					case BOTTOM_OF_GRAPH:
						markRect = Rect( Point(x-halfHeight, y), Size(halfHeight*2, totalLength) );
						break;
					case TOP_OF_GRAPH:
						markRect = Rect( Point(x-halfHeight, y-totalLength), Size(halfHeight*2, totalLength) );
						break;
					default:
						break;
				}
				return markRect;
			}
			
			
			virtual void Paint(Draw &dw, ElementPosition side, const int scale, int x, int y, const Color& markColor) const {
				Upp::Vector<Point> p;
				p << Point(x, y);
				const int halfHeight  = GetTickLength()/3 * scale;
				const int totalLength = GetTickLength() * scale;
				const int coneLength  = GetTickLength()/2 * scale;
				

				switch (side) {
					case LEFT_OF_GRAPH:
						p << Point(x-coneLength, y-halfHeight) << Point(x-totalLength, y-halfHeight)<< Point(x-totalLength, y+halfHeight)<< Point(x-coneLength, y+halfHeight);
						break;
					case RIGHT_OF_GRAPH:
						p << Point(x+coneLength, y-halfHeight) << Point(x+totalLength, y-halfHeight)<< Point(x+totalLength, y+halfHeight)<< Point(x+coneLength, y+halfHeight);
						break;
					case BOTTOM_OF_GRAPH:
						p << Point(x+halfHeight, y+coneLength) << Point(x+halfHeight, y+totalLength)<< Point(x-halfHeight, y+totalLength)<< Point(x-halfHeight, y+coneLength);
						break;
					case TOP_OF_GRAPH:
						p << Point(x+halfHeight, y-coneLength) << Point(x+halfHeight, y-totalLength)<< Point(x-halfHeight, y-totalLength)<< Point(x-halfHeight, y-coneLength);
						break;
					default:
						break;
				}
				p << Point(x, y);

				dw.DrawPolygon( p, textBckgndColor, 1, OutlineColor );
				Size tickSize = GetTickSize(side, scale);
				Point xyOffset((tickSize.cx  - tickIdFont.GetWidth(IDtext[0]))/2, 0 );
				Rect textRect = GetTickRect(xyOffset, side, x, y , scale);
				dw.DrawText(textRect.TopLeft().x, textRect.TopLeft().y, IDtext, tickIdFont, textColor );
			}

			virtual void Paint(Painter &p, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const {
			}
	};

	class SmartTextTickMark  : public TickMark {
		private:
			Font tickIdFont;
			Size textSize;
			Image  backgndImg;

		public:
			String IDtext;
			Color  textColor;
			Color  textBckgndColor;
			Color  OutlineColor;
		
		public:
			SmartTextTickMark() : textColor(Green), textBckgndColor(Null), OutlineColor(Black) { SetTickLength(15); }
			virtual ~SmartTextTickMark() {}

			virtual void UpdateTick() {
				tickIdFont.Height(GetTickLength()/2);
				textSize = GraphDraw_ns::GetSmartTextSize(IDtext.Begin(), tickIdFont);
			}

			void SetText(String& str) {
				IDtext = str;
				UpdateTick();
			}
			void SetText(const char* str) { String s(str); SetText( s ); }
			
			//void SetBckgndImage(const Image& img) { backgndImg = img; }
			void SetBckgndImage(Image img) { backgndImg = img; }
			
			virtual Size GetTickSize(ElementPosition side, const int scale) const 
			{
				const int coneLength  = GetTickLength()/2 * scale;
				Size res = textSize;
				switch (side)
				{
					case LEFT_OF_GRAPH:   
					case RIGHT_OF_GRAPH:
						res.cx += coneLength;
						break;
					case BOTTOM_OF_GRAPH:
					case TOP_OF_GRAPH:
						res.cy += coneLength;
						break;
					default:
						break;
				}
				return res;
			}

			virtual Rect GetTickRect(Point xyOffset, ElementPosition side, int x, int y, const int scale) const
			{
				x += xyOffset.x;
				y += xyOffset.y;
				Rect markRect;
				Size sz = GetTickSize(side, scale);
				switch (side) {
					case LEFT_OF_GRAPH:
						markRect = Rect( Point(x-sz.cx, y-sz.cy/2), sz );
						break;
					case RIGHT_OF_GRAPH:
						markRect = Rect( Point(x, y-sz.cy/2), sz );
						break;
					case BOTTOM_OF_GRAPH:
						markRect = Rect( Point(x-sz.cx/2, y), sz );
						break;
					case TOP_OF_GRAPH:
						markRect = Rect( Point(x-sz.cx/2, y-sz.cy), sz );
						break;
					default:
						break;
				}
				return markRect;
			}
			
			
			virtual void Paint(Draw &dw, ElementPosition side, const int scale, int x, int y, const Color& markColor) const {
				Upp::Vector<Point> p;
				p << Point(x, y);
				const int halfHeight  = GetTickLength()/3 * scale;
				const int coneLength  = GetTickLength()/2 * scale;
				Point txtPt( x-textSize.cx/2, y-textSize.cy/2 );
				
				switch (side) {
					case LEFT_OF_GRAPH:
						p << Point(x-coneLength, y-halfHeight) << Point(x-coneLength, y+halfHeight);
						txtPt.x = x-textSize.cx - coneLength;
						break;
					case RIGHT_OF_GRAPH:
						p << Point(x+coneLength, y-halfHeight) << Point(x+coneLength, y+halfHeight);
						txtPt.x = x+coneLength;
						break;
					case BOTTOM_OF_GRAPH:
						p << Point(x+halfHeight, y+coneLength) << Point(x-halfHeight, y+coneLength);
						txtPt.y = y+coneLength;
						break;
					case TOP_OF_GRAPH:
						p << Point(x+halfHeight, y-coneLength) << Point(x-halfHeight, y-coneLength);
						txtPt.y = y - textSize.cy - coneLength;
						break;
					default:
						break;
				}
				p << Point(x, y);
				dw.DrawPolygon( p, markColor, 1, markColor );
				
				Rect textRect(Rect(txtPt, textSize));
				
				// Draw background image
				if ( !backgndImg.IsNullInstance())
				{
					Rect r2 = textRect;
					r2.Inflate(3);
					ChPaint(dw, r2, backgndImg );
				}
				// Draw OUTLINE
				else if ( !textBckgndColor.IsNullInstance() || !OutlineColor.IsNullInstance())
				{
					Upp::Vector<Point> outline;
					outline << Point(x, y);
					switch (side) {
						case LEFT_OF_GRAPH:
							outline << Point(x-coneLength, y+halfHeight) << textRect.BottomRight() << textRect.BottomLeft() << textRect.TopLeft() << textRect.TopRight() << Point(x-coneLength, y-halfHeight);
							break;
						case RIGHT_OF_GRAPH:
							outline << Point(x+coneLength, y-halfHeight) << textRect.TopLeft() << textRect.TopRight() << textRect.BottomRight() << textRect.BottomLeft() << Point(x+coneLength, y+halfHeight);
							break;
						case BOTTOM_OF_GRAPH:
							outline << Point(x+halfHeight, y+coneLength) << textRect.TopRight() << textRect.BottomRight() << textRect.BottomLeft() << textRect.TopLeft() << Point(x-halfHeight, y+coneLength);
							break;
						case TOP_OF_GRAPH:
							outline << Point(x-halfHeight, y-coneLength) << textRect.BottomLeft() << textRect.TopLeft() << textRect.TopRight() << textRect.BottomRight() << Point(x+halfHeight, y-coneLength);
							break;
						default:
							break;
					}
					outline << Point(x, y);
					dw.DrawPolygon( outline, textBckgndColor, 1, OutlineColor );
				}

				// Draw TEXT
				if ( !IDtext.IsEmpty() && !textColor.IsNullInstance() ) {
					GraphDraw_ns::DrawSmartText(dw, txtPt.x, txtPt.y, textSize.cx, IDtext, tickIdFont, textColor );
				}
			}

			virtual void Paint(Painter &p, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const {
			}
	};
}
#endif
