#ifndef _GraphDraw_TickMark_h_
#define _GraphDraw_TickMark_h_



namespace GraphDraw_ns
{
	class TickMark {
		private:
			const Value nullVal;
			
		public:
			int _tickLength;
			TickMark() : nullVal(Null), _tickLength(2) { UpdateTick(); }
			virtual ~TickMark() {}

			virtual void Paint(Draw &w, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor, const Value& v) const = 0;
			inline void Paint(Draw &p, ElementPosition axisPos, const int scale, const Point& cp, const Color& markColor, const Value& v) const { Paint(p, axisPos, scale, cp.x, cp.y, markColor, v ); }
			inline void Paint( Draw &p, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const { Paint(p, axisPos, scale, x, y, markColor, nullVal ); }
			inline void Paint(Draw &p, ElementPosition axisPos, const int scale, const Point& cp, const Color& markColor) const { Paint(p, axisPos, scale, cp.x, cp.y, markColor, nullVal ); }

			virtual void Paint(Painter &p, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor, const Value& v) const = 0;
			inline void Paint( Painter &p, ElementPosition axisPos, const int scale, const Point& cp, const Color& markColor, const Value& v) const { Paint(p, axisPos, scale, cp.x, cp.y, markColor, v ); }
			inline void Paint(Painter &p, ElementPosition axisPos, const int scale,  int x, int y, const Color& markColor) const { Paint(p, axisPos, scale, x, y, markColor, nullVal ); }
			inline void Paint(Painter &p, ElementPosition axisPos, const int scale, const Point& cp, const Color& markColor) const { Paint(p, axisPos, scale, cp.x, cp.y, markColor, nullVal ); }

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

			virtual void Paint(Draw &w, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor, const Value& v) const {
				int diam = fround(scale*_tickLength*2);
				int radius = diam/2;
				w.DrawEllipse(x - radius, y - radius, diam, diam, markColor, 1, markColor);
			}

			virtual void Paint(Painter &p, ElementPosition axisPos, const int& scale, int x, int y, const Color& markColor, const Value& v) const {
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

	class TriangleTickMark  : public TickMark {
		public:
			TriangleTickMark() { SetTickLength(5);	}
			virtual ~TriangleTickMark() {}

			virtual void Paint(Draw &dw, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor, const Value& v) const {
				Upp::Vector<Point> p;
				p << Point(x, y);
				const int scOffset = 2*scale;
				const int scTickLength = _tickLength*scale;
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

			virtual void Paint(Painter &p, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor, const Value& v) const {
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

			virtual void Paint(Draw &dw, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor, const Value& v) const {
				const int scTickLength = _tickLength*scale;
				if ((axisPos==LEFT_OF_GRAPH) || ( axisPos==RIGHT_OF_GRAPH ))	{
					dw.DrawLine(x-scTickLength, y, x+scTickLength, y, 2*scale, markColor);
				} else {
					dw.DrawLine(x, y-scTickLength, x, y+scTickLength, 2*scale, markColor);
				}
			}

			virtual void Paint(Painter &p, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor, const Value& v) const {
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



	class RectTriangleTickMarkConfig {
		public:
		String IDtext;
		Color  textColor;
		Color  textBckgndColor;
		Color  OutlineColor;
	};
	
	class RectTriangleTickMark  : public TickMark {
		public:
			Font tickIdFont;
		
		public:
		
			RectTriangleTickMark() { SetTickLength(15);	}
			virtual ~RectTriangleTickMark() {}

			virtual void UpdateTick() {
				tickIdFont.Height(_tickLength/2);
			}

			virtual Size GetTickSize(ElementPosition side, const int scale) const 
			{
				const int halfHeight  = _tickLength/3 * scale;
				const int totalLength = _tickLength * scale;
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
				const int halfHeight  = _tickLength/3 * scale;
				const int totalLength = _tickLength * scale;
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
			
			
			virtual void Paint(Draw &dw, ElementPosition side, const int scale, int x, int y, const Color& markColor, const Value& v) const {
				Upp::Vector<Point> p;
				p << Point(x, y);
				const int halfHeight  = _tickLength/3 * scale;
				const int totalLength = _tickLength * scale;
				const int coneLength  = _tickLength/2 * scale;
				

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

				
				if ( v.IsNull() ) {
					dw.DrawPolygon( p, markColor, 1, Black()  );
				}
				else {
					const RectTriangleTickMarkConfig& par = v.To<RectTriangleTickMarkConfig>();

					dw.DrawPolygon( p, par.textBckgndColor, 1, par.OutlineColor );
					Size tickSize = GetTickSize(side, scale);
					Point xyOffset((tickSize.cx  - tickIdFont.GetWidth(par.IDtext[0]))/2, 0 );
					Rect textRect = GetTickRect(xyOffset, side, x, y , scale);
					dw.DrawText(textRect.TopLeft().x, textRect.TopLeft().y, par.IDtext, tickIdFont, par.textColor );
				}
			}

			virtual void Paint(Painter &p, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor, const Value& v) const {
			}
			
			

	};

}
#endif
