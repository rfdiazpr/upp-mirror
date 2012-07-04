/*
 * GridAxisDraw.h
 *
 *  Created on: 6 avr. 2012
 *      Author: didier
 */

#ifndef GRIDAXISDRAW_H_
#define GRIDAXISDRAW_H_


namespace GraphDraw_ns
{

	class TickMark {

		public:
			int _tickLength;
			TickMark() : _tickLength(2) { UpdateTick(); }
			virtual ~TickMark() {}

			virtual void Paint(Draw &w, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const {};
			inline void Paint(Draw &p, ElementPosition axisPos, const int scale, const Point& cp, const Color& markColor) const { Paint(p, axisPos, scale, cp.x, cp.y, markColor); }

			virtual void Paint(Painter &p, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const {};
			inline void Paint(Painter &p, ElementPosition axisPos, const int scale, const Point& cp, const Color& markColor) const { Paint(p, axisPos, scale, cp.x, cp.y, markColor); }

			inline int GetTickLength()       { return _tickLength; }
			inline TickMark* SetTickLength(int v) { _tickLength = v; UpdateTick(); return this; }

			virtual void UpdateTick() {}; // called when tick drawing needs to be recalculated
	};

	class RoundTickMark  : public TickMark {
		public:
			RoundTickMark() { SetTickLength(5);	}
			virtual ~RoundTickMark() {}

			virtual void Paint(Draw &w, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const {
				int diam = fround(scale*_tickLength*2);
				int radius = diam/2;
				w.DrawEllipse(x - radius, y - radius, diam, diam, markColor, 1, markColor);
			}

			virtual void Paint(Painter &p, ElementPosition axisPos, const int& scale, int x, int y, const Color& markColor) const {
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
				const int scTickLength = _tickLength*scale;
				if (axisPos==LEFT_OF_GRAPH)	{
					p << Point(x-scTickLength, y-scOffset) << Point(x-scTickLength, y+scOffset);
				} else if (axisPos==RIGHT_OF_GRAPH)	{
					p << Point(x+scTickLength, y-scOffset) << Point(x+scTickLength, y+scOffset);
				} else if (axisPos==BOTTOM_OF_GRAPH)	{
					p << Point(x-scOffset, y+scTickLength) << Point(x+scOffset, y+scTickLength);
				} else {
					p << Point(x-scOffset, y-scTickLength) << Point(x+scOffset, y-scTickLength);
				}
				p << Point(x, y);
				dw.DrawPolygon( p, markColor, scale/2, markColor);
			}

			virtual void Paint(Painter &p, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const {
			}
	};

	class LineTickMark  : public TickMark {
		public:
			LineTickMark() { SetTickLength(3);	}
			virtual ~LineTickMark() {}

			virtual void Paint(Draw &dw, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const {
				const int scTickLength = _tickLength*scale;
				if ((axisPos==LEFT_OF_GRAPH) || ( axisPos==RIGHT_OF_GRAPH ))	{
					dw.DrawLine(x-scTickLength, y, x+scTickLength, y, 2*scale, markColor);
				} else {
					dw.DrawLine(x, y-scTickLength, x, y+scTickLength, 2*scale, markColor);
				}
			}

			virtual void Paint(Painter &p, ElementPosition axisPos, const int scale, int x, int y, const Color& markColor) const {
			}
	};



	// ============================
	//    GridAxisDraw   CLASS
	// ============================
	template<class TYPES >
	class GridAxisDraw : public CRTPGraphElementFrame< GridAxisDraw<TYPES> >
	{
		public:
		typedef GridAxisDraw<TYPES>                   CLASSNAME;
		typedef typename TYPES::TypeCoordConverter    TypeCoordConverter;
		typedef typename TYPES::TypeGridStepManager   TypeGridStepManager;
		typedef CRTPGraphElementFrame< GridAxisDraw<TYPES> > _B;
		typedef Callback3<TypeGraphCoord, String&, TypeGraphCoord> TypeFormatTextCbk; // value,  formated value,  range

//		protected:
		int       _axisWidth;
		Color     _axisColor;
		Font      _axisTextFont;
		Color     _axisTextColor;
		Color     _axisTickColor;
		Color     _gridColor;
		One<TickMark> _majorTickMark;
		One<TickMark> _minorTickMark;
		TypeFormatTextCbk formatTextCbk;


		One< TypeGridStepManager > _gridStepManager;
		TypeCoordConverter& _coordConverter;

		public:
		GridAxisDraw(TypeCoordConverter& coordConv)
		: _gridStepManager( new TypeGridStepManager(coordConv) )
		, _axisWidth(2)
		, _axisColor( Blue())
		, _axisTextFont(StdFont())
		, _axisTextColor(Red())
		, _axisTickColor(Red())
		, _gridColor(LtGray())
		, _coordConverter( coordConv )
		, _majorTickMark(new TickMark())
		, _minorTickMark(new TickMark())
		{
		}

		GridAxisDraw(GridAxisDraw& p)
		: _B(p)
		, _gridStepManager( new TypeGridStepManager(p._coordConverter) )
		, _axisWidth(p._axisWidth)
		, _axisColor(p._axisColor)
		, _axisTextFont(p._axisTextFont)
		, _axisTextColor(p._axisTextColor)
		, _axisTickColor(p._axisTickColor)
		, _gridColor(Null)  // This is a slave Axis ==> so no need to draw grid
		, _coordConverter( p._coordConverter )
		, _majorTickMark(new TickMark())
		, _minorTickMark(new TickMark())
		, formatTextCbk( p.formatTextCbk )
		{
		}

		virtual ~GridAxisDraw() {}


		virtual CLASSNAME* Clone() { return new CLASSNAME(*this); }

		virtual void Update() { _gridStepManager->Update(); }

		TypeGridStepManager& GetGridStepManager() { return *_gridStepManager; }
		inline CLASSNAME& setAxisColor(Color v)                         { _axisColor = v; return *this;  }
		inline CLASSNAME& setAxisWidth(int v)                           { _axisWidth = v; return *this;  }
		inline CLASSNAME& setAxisTextFont(Font v)                       { _axisTextFont = v; return *this; }
		inline CLASSNAME& setAxisTextColor(Color v)                     { _axisTextColor = v; return *this; }
		inline CLASSNAME& setAxisTickColor(Color v)                     { _axisTickColor = v; return *this; }
		inline CLASSNAME& setGridColor(Color v)                         { _gridColor = v; return *this; }
		inline CLASSNAME& setMajorTickMark(TickMark* v)                 { _majorTickMark = v; return *this;  }
		inline CLASSNAME& setMinorTickMark(TickMark* v)                 { _minorTickMark = v; return *this;  }
		inline CLASSNAME& setAxisTextFormat(TypeFormatTextCbk v)        { formatTextCbk = v; return *this;  }
		inline CLASSNAME& resetAxisTextFormat()                         { formatTextCbk = Null; return *this;  }
		inline CLASSNAME& setAxisDateFormat( TypeFormatTextCbk cbk=TypeFormatTextCbk() ) {
			if ( cbk ) formatTextCbk = cbk;
			else       formatTextCbk = THISBACK(FormatAsDate);
			_gridStepManager->setDateGridSteps();
			return *this;
		}
		
		inline CLASSNAME& setAxisTimeFormat( TypeFormatTextCbk cbk=TypeFormatTextCbk() ) {
			if ( cbk ) formatTextCbk = cbk;
			else       formatTextCbk = THISBACK(FormatAsTime);
			_gridStepManager->setTimeGridSteps();
			return *this;
		}


		inline int GetMajorTickLength()       { return _majorTickMark->GetTickLength(); }
		inline void SetMajorTickLength(int v) { _majorTickMark->SetTickLength(v); }

		inline int GetMinorTickLength()       { return _minorTickMark->GetTickLength(); }
		inline void SetMinorTickLength(int v) { _minorTickMark->SetTickLength(v); }

		inline TypeCoordConverter& GetCoordConverter()                 { return _coordConverter;  }

/*		inline Color GetAxisColor(Color v)                        { _axisColor = v; return *this;  }
		inline int GetAxisWidth(int v)                            { _axisWidth = v; return *this;  }
		inline Font GetAxisTextFont(Font v)                       { _axisTextFont = v; return *this; }
		inline Color GetAxisTextColor(Color v)                    { _axisTextColor = v; return *this; }
		inline Color GetAxisTickColor(Color v)                    { _axisTickColor = v; return *this; }
		inline Color GetGridColor(Color v)                        { _gridColor = v; return *this; }
*/
		inline typename TypeGridStepManager::Iterator getMajorBeginIterator(void) { return  _gridStepManager->Begin(); }
		inline typename TypeGridStepManager::Iterator getMajorEndIterator(void)   { return  _gridStepManager->End(); }


		template<class T>
		T& CreateGridStepManager() {
				_gridStepManager =  new T(_coordConverter);
				_gridStepManager.Update();
				return (*_gridStepManager);
		}

		void FormatAsDate(TypeGraphCoord v, String& output, TypeGraphCoord range) {
			Date dat;
			dat.Set(range);
			dat.Set(int(v));
			output = Format("%d/%d/%d",dat.day, dat.month, dat.year);
		}

		void FormatAsTime(TypeGraphCoord value, String& output, TypeGraphCoord range) {
			Time time;
			time.Set(int64(value));
			if (range > 7*365*24*60*60) { // 7 years
				output = Format("%d", time.year);
			}
			else if (range > 182.5*24*60*60) { // 6 months
				if ((time.month == 1) && (time.day == 1) ) {
					output << time.year;
				}
				else {
					output << MonName(time.month-1);
				}
			}
			else if (range > 7*24*60*60) { // 7 days
				if ((time.month == 1) && (time.day == 1) && (time.hour==0) ) {
					output << int(time.day) << " " << MonName(time.month-1) << " " << time.year;
				}
				else if ( (time.day == 1) && (time.hour==0) ) {
					output << int(time.day) << " " << MonName(time.month-1);
				}
				else {
					output << int(time.day);
				}
			}
			else {
//				if ((time.day == 1) && (time.hour == 0) && (time.minute == 0) && (time.second == 0)) {
//					output = Format("%d/%d", time.day, time.month);
//				}
//				else if ( (time.hour == 0) && (time.minute == 0) && (time.second == 0)) {
//					output = Format("%dh%d:%d", time.hour, time.minute, time.second);
//				}
//				else {
//					output = Format("%d/%d", time.month, time.year);
//				}
				output = Format("%dh%d:%d", time.hour, time.minute, time.second);
			}
		}


		template<int GRAPH_SIDE>
		void PaintTickText(Draw& dw,  TypeGraphCoord v, TypeScreenCoord x, TypeScreenCoord y, Color& color, Font& font) {
			Upp::String text;
			if ( ! formatTextCbk )	text=FormatDouble(v);
			else                    formatTextCbk(v, text, _coordConverter.getSignedGraphRange() );
			Size sz = GetTextSize(text, font);
			if (GRAPH_SIDE == LEFT_OF_GRAPH) {
				dw.DrawText( x-sz.cx, y-(sz.cy/2) , text, font, color);
			}
			else if (GRAPH_SIDE == BOTTOM_OF_GRAPH) {
				dw.DrawText( x-(sz.cx/2), y , text, font, color);
			}
			else if (GRAPH_SIDE == RIGHT_OF_GRAPH) {
				dw.DrawText( x, y-(sz.cy/2) , text, font, color);
			}
			else {
				dw.DrawText( x-(sz.cx/2), y-sz.cy , text, font, color);
			}
		}

		virtual void PaintAxisLeft(Draw& dw, const int scale)
		{
			typename TypeGridStepManager::Iterator iter = getMajorBeginIterator();
			typename TypeGridStepManager::Iterator endIter = getMajorEndIterator();

			Font scaledAxisTextFont( _axisTextFont );
			scaledAxisTextFont.Height(scale*scaledAxisTextFont.GetHeight());
			dw.DrawLineOp(_B::GetElementWidth()*scale, _coordConverter.getScreenMin(), _B::GetElementWidth()*scale, _coordConverter.getScreenMax(), 2, _axisColor );
			while ( iter != endIter)
			{
				if (_majorTickMark.IsEmpty()) {
					dw.DrawLineOp((_B::GetElementWidth()-8)*scale, *iter, _B::GetElementWidth()*scale, *iter, 2, _axisTickColor);
					PaintTickText<LEFT_OF_GRAPH>(dw, iter.getValue(), (_B::GetElementWidth()-8)*scale, *iter, _axisTextColor, scaledAxisTextFont);
				} else {
					_majorTickMark->Paint(dw, LEFT_OF_GRAPH, scale, _B::GetElementWidth()*scale, *iter, _axisTickColor );
					PaintTickText<LEFT_OF_GRAPH>(dw, iter.getValue(), (_B::GetElementWidth()-_majorTickMark->GetTickLength()-2)*scale, *iter, _axisTextColor, scaledAxisTextFont);
				}
				++iter;
			}
		}

		virtual void PaintAxisRight(Draw& dw, const int scale)
		{
			typename TypeGridStepManager::Iterator iter = getMajorBeginIterator();
			typename TypeGridStepManager::Iterator endIter = getMajorEndIterator();

			Font scaledAxisTextFont( _axisTextFont );
			scaledAxisTextFont.Height(scale*scaledAxisTextFont.GetHeight());
			dw.DrawLineOp(0, _coordConverter.getScreenMin(), 0, _coordConverter.getScreenMax(), 2, _axisColor);
			while ( iter != endIter)
			{
				if (_majorTickMark.IsEmpty())
				{
					dw.DrawLineOp(0, *iter, 8, *iter, 2, _axisTickColor);
					PaintTickText<RIGHT_OF_GRAPH>(dw, iter.getValue(), 8, *iter, _axisTextColor, scaledAxisTextFont);
				} else {
					_majorTickMark->Paint(dw, RIGHT_OF_GRAPH, scale, 0, *iter, _axisTickColor );
					PaintTickText<RIGHT_OF_GRAPH>(dw, iter.getValue(), (_majorTickMark->GetTickLength()+2)*scale, *iter, _axisTextColor, scaledAxisTextFont);
				}
				++iter;
			}
		}

		virtual void PaintAxisBottom(Draw& dw, const int scale)
		{
			typename TypeGridStepManager::Iterator iter = getMajorBeginIterator();
			typename TypeGridStepManager::Iterator endIter = getMajorEndIterator();

			Font scaledAxisTextFont( _axisTextFont );
			scaledAxisTextFont.Height(scale*scaledAxisTextFont.GetHeight());
			dw.DrawLineOp(_coordConverter.getScreenMin(), 0, _coordConverter.getScreenMax(),0 , 2, _axisColor );
			while ( iter != endIter )
			{
				if (_majorTickMark.IsEmpty()) {
					dw.DrawLineOp(*iter, 0, *iter, 4, 2, _axisTickColor);
					PaintTickText<BOTTOM_OF_GRAPH>(dw, iter.getValue(), *iter, 4, _axisTextColor, scaledAxisTextFont);
				} else {
					_majorTickMark->Paint(dw, BOTTOM_OF_GRAPH, scale, *iter, 0, _axisTickColor );
					PaintTickText<BOTTOM_OF_GRAPH>(dw, iter.getValue(), *iter, (_majorTickMark->GetTickLength()+2)*scale, _axisTextColor, scaledAxisTextFont);
				}
				++iter;
			}
		}

		virtual void PaintAxisTop(Draw& dw, const int scale)
		{
			typename TypeGridStepManager::Iterator iter = getMajorBeginIterator();
			typename TypeGridStepManager::Iterator endIter = getMajorEndIterator();

			Font scaledAxisTextFont( _axisTextFont );
			scaledAxisTextFont.Height(scale*scaledAxisTextFont.GetHeight());
			dw.DrawLineOp(_coordConverter.getScreenMin(), _B::GetElementWidth()*scale, _coordConverter.getScreenMax(), _B::GetElementWidth()*scale, 2, _axisColor );
			while ( iter != endIter)
			{
				if (_majorTickMark.IsEmpty()) {
					dw.DrawLineOp(*iter, _B::GetElementWidth()*scale, *iter, (_B::GetElementWidth()-4)*scale, 2, _axisTickColor);
					PaintTickText<TOP_OF_GRAPH>(dw, iter.getValue(), *iter, (_B::GetElementWidth()-4)*scale, _axisTextColor, scaledAxisTextFont);
				} else {
					_majorTickMark->Paint(dw, TOP_OF_GRAPH, scale, *iter, _B::GetElementWidth()*scale, _axisTickColor );
					PaintTickText<TOP_OF_GRAPH>(dw, iter.getValue(), *iter, (_B::GetElementWidth()-_majorTickMark->GetTickLength()-2)*scale, _axisTextColor, scaledAxisTextFont);
				}
				++iter;
			}
		}

		virtual void PaintVGrid(Draw& dw, int xRange)
		{
			if ( !_gridColor.IsNullInstance())
			{
				typename TypeGridStepManager::Iterator iter = getMajorBeginIterator();
				typename TypeGridStepManager::Iterator endIter = getMajorEndIterator();
				while( iter != endIter)
				{
					dw.DrawLineOp( 0, *iter, xRange, *iter, 1, _gridColor );
					++iter;
				}
			}
		}

		virtual void PaintHGrid(Draw& dw, int yRange)
		{
			if ( !_gridColor.IsNullInstance())
			{
				typename TypeGridStepManager::Iterator iter = getMajorBeginIterator();
				typename TypeGridStepManager::Iterator endIter = getMajorEndIterator();
				while( iter != endIter)
				{
					dw.DrawLineOp( *iter, 0, *iter, yRange, 1, _gridColor );
					++iter;
				}
			}
		}

		virtual void PaintOnPlot(Draw& dw, int otherWidth, int scale)
		{
			switch(_B::GetElementPos()){
				case LEFT_OF_GRAPH:
				case RIGHT_OF_GRAPH:
					PaintVGrid(dw, otherWidth);
					break;
				case BOTTOM_OF_GRAPH:
				case TOP_OF_GRAPH:
					PaintHGrid(dw, otherWidth);
					break;
			}
		}

		virtual void PaintElement(Draw& dw, int scale)
		{
			switch(_B::GetElementPos()){
				case LEFT_OF_GRAPH:
					PaintAxisLeft(dw, scale);
					break;
				case BOTTOM_OF_GRAPH:
					PaintAxisBottom(dw, scale);
					break;
				case TOP_OF_GRAPH:
					PaintAxisTop(dw, scale);
					break;
				case RIGHT_OF_GRAPH:
					PaintAxisRight(dw, scale);
					break;
			}
		}
	};
}

#endif /* GRIDAXISDRAW_H_ */
