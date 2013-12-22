/*
 * GridAxisDraw.h
 *
 *  Created on: 6 avr. 2012
 *      Author: didier
 */

#ifndef GRIDAXISDRAW_H_
#define GRIDAXISDRAW_H_


#include "TickMark.h"

namespace GraphDraw_ns
{

	// ============================
	//    GridAxisDraw   CLASS
	// ============================
	typedef enum {
		AXIS_TEXT_FORMAT_STD,
		AXIS_TEXT_FORMAT_LOG,
		AXIS_TEXT_FORMAT_DATE,
		AXIS_TEXT_FORMAT_TIME,
	} AxisTextFormat;

	template<class TYPES >
	class GridAxisDraw : public CRTPGraphElement< GridAxisDraw<TYPES> >
	{
		public:
		typedef GridAxisDraw<TYPES>                   CLASSNAME;
		typedef typename TYPES::TypeGridStepManager   TypeGridStepManager;
		typedef CRTPGraphElement< GridAxisDraw<TYPES> > _B;
		typedef Callback2< const GridStepIterator&, String&> TypeFormatTextCbk; // IN: valueIterator,  OUT: formated value

		
//		protected:
		CoordinateConverter& _coordConverter;
		One< TypeGridStepManager > _gridStepManager;

		int       _axisWidth;
		Color     _axisColor;
		Font      _axisTextFont;
		Color     _axisTextColor;
		Color     _axisTickColor;
		Color     _gridColor;
		One<TickMark> _majorTickMark;
		One<TickMark> _minorTickMark;
		TypeFormatTextCbk formatTextCbk;



		public:
		GridAxisDraw(CoordinateConverter& coordConv)
		: _coordConverter( coordConv )
		, _gridStepManager( new TypeGridStepManager(coordConv) )
		, _axisWidth(2)
		, _axisColor( Blue())
		, _axisTextFont(StdFont())
		, _axisTextColor(Red())
		, _axisTickColor(Red())
		, _gridColor(LtGray())
		, _majorTickMark(new LineTickMark())
		, _minorTickMark(new LineTickMark())
		{
			_B::DisablePos(FLOAT_OVER_GRAPH);
		}

		GridAxisDraw(GridAxisDraw& p)
		: _B(p)
		, _coordConverter( p._coordConverter )
		, _gridStepManager( new TypeGridStepManager(p._coordConverter) )
		, _axisWidth(p._axisWidth)
		, _axisColor(p._axisColor)
		, _axisTextFont(p._axisTextFont)
		, _axisTextColor(p._axisTextColor)
		, _axisTickColor(p._axisTickColor)
		, _gridColor(Null)  // This is a slave Axis ==> so no need to draw grid
		, _majorTickMark(new LineTickMark())
		, _minorTickMark(new LineTickMark())
		, formatTextCbk( p.formatTextCbk )
		{
		}

		virtual ~GridAxisDraw() {}


//		virtual CLASSNAME* Clone() { return new CLASSNAME(*this); }

		virtual void Update() { _gridStepManager->Update(); }
		
		virtual CLASSNAME&  SetElementPos(ElementPosition v) {
			if (v==LEFT_OF_GRAPH || v==RIGHT_OF_GRAPH) {
				_B::DisablePos(BOTTOM_OF_GRAPH);
				_B::DisablePos(TOP_OF_GRAPH);
			} else {
				_B::DisablePos(LEFT_OF_GRAPH);
				_B::DisablePos(RIGHT_OF_GRAPH);
			}
			return _B::SetElementPos(v);
		 }

		
		virtual void FitToData(FitToDataStrategy fitStrategy) {
			typedef TypeVectorSeries*  PtrTypeVectorSeries;
			PtrTypeVectorSeries v_series = _B::_parent->GetSeries().template To<PtrTypeVectorSeries>();
			double lmin = -DOUBLE_NULL;
			double lmax =  DOUBLE_NULL;
			bool doFitToData = false;
			switch(_B::GetElementPos()) {
				case LEFT_OF_GRAPH:
				case RIGHT_OF_GRAPH:
					for (int c=0; c<(*v_series).GetCount(); ++c) {
						if ( ( (fitStrategy==ALL_SERIES)  || ((*v_series)[c].show)) && ((*v_series)[c].yConverter == &_coordConverter) ) {
							lmin = min (lmin, (*v_series)[c].PointsData()->MinY());
							lmax = max (lmax, (*v_series)[c].PointsData()->MaxY());
							doFitToData = true;
						}
					}
					break;
				case BOTTOM_OF_GRAPH:
				case TOP_OF_GRAPH:
					for (int c=0; c<(*v_series).GetCount(); ++c) {
						if ( ( (fitStrategy==ALL_SERIES)  || ((*v_series)[c].show)) && ((*v_series)[c].xConverter == &_coordConverter) ) {
							lmin = min (lmin, (*v_series)[c].PointsData()->MinX());
							lmax = max (lmax, (*v_series)[c].PointsData()->MaxX());
							doFitToData = true;
						}
					}
					break;
				case FLOAT_OVER_GRAPH:
					break;
			}
			if (doFitToData) _coordConverter.updateGraphSize( lmin, lmax );
		}


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
		CLASSNAME& setAxisTextFormat(AxisTextFormat v) {
			switch(v) {
				case AXIS_TEXT_FORMAT_STD:  formatTextCbk.Clear(); break;
				case AXIS_TEXT_FORMAT_LOG:  setAxisLogFormat(); break;
				case AXIS_TEXT_FORMAT_DATE: setAxisDateFormat(); break;
				case AXIS_TEXT_FORMAT_TIME: setAxisTimeFormat(); break;
			}
			return *this;
		}
		
		inline CLASSNAME& resetAxisTextFormat() { formatTextCbk.Clear(); return *this;  }
		inline CLASSNAME& setAxisLogFormat(TypeFormatTextCbk cbk=TypeFormatTextCbk()) {
			if ( cbk ) formatTextCbk = cbk;
			else       formatTextCbk = THISBACK(FormatAsLog10);
			_gridStepManager->setLogGridSteps();
			return *this;
		}

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

		inline CoordinateConverter& GetCoordConverter()                 { return _coordConverter;  }

		inline typename TypeGridStepManager::Iterator getMajorBeginIterator(void) { return  _gridStepManager->Begin(); }
		inline typename TypeGridStepManager::Iterator getMajorEndIterator(void)   { return  _gridStepManager->End(); }


		template<class T>
		T& CreateGridStepManager() {
				_gridStepManager =  new T(_coordConverter);
				_gridStepManager.Update();
				return (*_gridStepManager);
		}

		void FormatAsLog10( const GridStepIterator& value, String& output ) {
			if (value.getValueParam() == 1) {
				output << value.getValue() ;
			}
		}

		void FormatAsDate( const GridStepIterator& value, String& output ) {
			Date dat;
			dat.Set(int(value.getValue()));
			output = Format("%d/%d/%d",dat.day, dat.month, dat.year);
		}

		void FormatAsTime( const GridStepIterator& value, String& output ) {
			Time time;
			time.Set(int64( value.getValue() ));

			TypeGraphCoord range = value.getGraphRange();

			if (range > 7*365*24*60*60) { // 7 years
				output << "\1[1= " << time.year << "]";
			}
			else if (range > 182.5*24*60*60) { // 6 months
				if (((time.month == 1) && (time.day == 1) )
					|| value.isFirst() || value.isLast() )
				   {
					switch(_B::GetElementPos()){
						case LEFT_OF_GRAPH:
							output << "\1[1* " << time.year;
							output << " ][1 " << MonName(time.month-1) << "]";
							break;
						case RIGHT_OF_GRAPH:
							output << "\1[1 " << MonName(time.month-1) ;
							output << " ][1* " << time.year << "]";
							break;
						case BOTTOM_OF_GRAPH:
							output << "\1[1= " << MonName(time.month-1) ;
							output << "&][1*= " << time.year << "]";
							break;
						case FLOAT_OVER_GRAPH:
						case TOP_OF_GRAPH:
							output << "\1[1*= " << time.year;
							output << "&][1= " << MonName(time.month-1) << "]";
							break;
					}
				}
				else {
					output << "\1[1 " << MonName(time.month-1) << "]";
				}
			}
			else if (range > 7*24*60*60) { // 7 days
				if ( (time.day == 1) && (time.hour==0)
						|| value.isFirst() || value.isLast() )
				{
					switch(_B::GetElementPos()){
						case LEFT_OF_GRAPH:
							output << "\1[1* " << time.year;
							output << " ][1 " << int(time.day) << " " << MonName(time.month-1) << "]";
							break;
						case RIGHT_OF_GRAPH:
							output << "\1[1 " << int(time.day) << " " << MonName(time.month-1) ;
							output << " ][1* " << time.year << "]";
							break;
						case BOTTOM_OF_GRAPH:
							output << "\1[1= " << int(time.day) << " " << MonName(time.month-1) ;
							output << "&][1*= " << time.year << "]";
							break;
						case FLOAT_OVER_GRAPH:
						case TOP_OF_GRAPH:
							output << "\1[1*= " << time.year;
							output << "&][1= " << int(time.day) << " " << MonName(time.month-1) << "]";
							break;
					}
				}
				else {
					output << "\1[1 " << int(time.day) << "]";
				}
			}
			else {
				if ( value.isFirst() || value.isLast() ) {
					switch(_B::GetElementPos()){
						case LEFT_OF_GRAPH:
							output << "\1[1* " << time.year;
							output << " ][1 " << int(time.day) << " " << MonName(time.month-1) << "]";
							output << "[1 " << FormatTime( time, " h`hmm:ss" ) << "]";
							break;
						case RIGHT_OF_GRAPH:
							output << "\1[1 " << FormatTime( time, "h`hmm:ss " ) << "]";
							output << "[1 " << int(time.day) << " " << MonName(time.month-1) ;
							output << " ][1* " << time.year << "]";
							break;
						case BOTTOM_OF_GRAPH:
							output << "\1[1= " << FormatTime( time, "h`hmm:ss" ) << "&]";
							output << "[1= " << int(time.day) << " " << MonName(time.month-1) ;
							output << "&][1*= " << time.year << "]";
							break;
						case FLOAT_OVER_GRAPH:
						case TOP_OF_GRAPH:
							output << "\1[1*= " << time.year;
							output << "&][1= " << int(time.day) << " " << MonName(time.month-1) << "&]";
							output << "[1= " << FormatTime( time, "h`hmm:ss" ) << "]";
							break;
					}
				}
				else {
					output << "\1[1 " << FormatTime( time, "h`hmm:ss" ) << "]";
				}
			}
		}


		template<int GRAPH_SIDE>
		void PaintTickText(Draw& dw,  const GridStepIterator& value, TypeScreenCoord x, TypeScreenCoord y, Color& color, Font& scaledFont, int scale) {
			Upp::String text;
			if ( ! formatTextCbk )	text=FormatDouble( value.getValue() );
			else                    formatTextCbk(value, text );

			Size sz = GraphDraw_ns::GetSmartTextSize(text, scaledFont, scale);
			if (GRAPH_SIDE == LEFT_OF_GRAPH) {
				GraphDraw_ns::DrawSmartText(dw, x-sz.cx, y-(sz.cy/2), sz.cx, text, scaledFont, color, scale);
			}
			else if (GRAPH_SIDE == BOTTOM_OF_GRAPH) {
				GraphDraw_ns::DrawSmartText(dw, x-(sz.cx/2), y, sz.cx, text, scaledFont, color, scale);
			}
			else if (GRAPH_SIDE == RIGHT_OF_GRAPH) {
				GraphDraw_ns::DrawSmartText( dw, x, y-(sz.cy/2), sz.cx, text, scaledFont, color, scale);
			}
			else {
				GraphDraw_ns::DrawSmartText( dw,  x-(sz.cx/2), y-sz.cy ,sz.cx,  text, scaledFont, color, scale);
			}
		}

		virtual void PaintAxisLeft(Draw& dw, const int scale)
		{
			typename TypeGridStepManager::Iterator iter = getMajorBeginIterator();
			typename TypeGridStepManager::Iterator endIter = getMajorEndIterator();

			Font scaledAxisTextFont( _axisTextFont );
			scaledAxisTextFont.Height(scale*scaledAxisTextFont.GetHeight());
			dw.DrawLineOp(_B::GetElementWidth()*scale, _coordConverter.getScreenMin(), _B::GetElementWidth()*scale, _coordConverter.getScreenMax(), _axisWidth*scale, _axisColor );
			while ( iter != endIter)
			{
				if (_majorTickMark.IsEmpty()) {
					dw.DrawLineOp((_B::GetElementWidth()-8)*scale, *iter, _B::GetElementWidth()*scale, *iter, 2, _axisTickColor);
					PaintTickText<LEFT_OF_GRAPH>(dw, iter, (_B::GetElementWidth()-8)*scale, *iter, _axisTextColor, scaledAxisTextFont, scale);
				} else {
					_majorTickMark->Paint(dw, LEFT_OF_GRAPH, scale, _B::GetElementWidth()*scale, *iter, _axisTickColor );
					PaintTickText<LEFT_OF_GRAPH>(dw, iter, (_B::GetElementWidth()-_majorTickMark->GetTickLength()-2)*scale, *iter, _axisTextColor, scaledAxisTextFont, scale);
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
			dw.DrawLineOp(0, _coordConverter.getScreenMin(), 0, _coordConverter.getScreenMax(), _axisWidth*scale, _axisColor);
			while ( iter != endIter)
			{
				if (_majorTickMark.IsEmpty())
				{
					dw.DrawLineOp(0, *iter, 8, *iter, 2, _axisTickColor);
					PaintTickText<RIGHT_OF_GRAPH>(dw, iter, 8, *iter, _axisTextColor, scaledAxisTextFont, scale);
				} else {
					_majorTickMark->Paint(dw, RIGHT_OF_GRAPH, scale, 0, *iter, _axisTickColor );
					PaintTickText<RIGHT_OF_GRAPH>(dw, iter, (_majorTickMark->GetTickLength()+2)*scale, *iter, _axisTextColor, scaledAxisTextFont, scale);
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
			dw.DrawLineOp(_coordConverter.getScreenMin(), 0, _coordConverter.getScreenMax(),0 , _axisWidth*scale, _axisColor );
			while ( iter != endIter )
			{
				if (_majorTickMark.IsEmpty()) {
					dw.DrawLineOp(*iter, 0, *iter, 4, 2, _axisTickColor);
					PaintTickText<BOTTOM_OF_GRAPH>(dw, iter, *iter, 4, _axisTextColor, scaledAxisTextFont, scale);
				} else {
					_majorTickMark->Paint(dw, BOTTOM_OF_GRAPH, scale, *iter, 0, _axisTickColor );
					PaintTickText<BOTTOM_OF_GRAPH>(dw, iter, *iter, (_majorTickMark->GetTickLength()+2)*scale, _axisTextColor, scaledAxisTextFont, scale);
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
			dw.DrawLineOp(_coordConverter.getScreenMin(), _B::GetElementWidth()*scale, _coordConverter.getScreenMax(), _B::GetElementWidth()*scale, _axisWidth*scale, _axisColor );
			while ( iter != endIter)
			{
				if (_majorTickMark.IsEmpty()) {
					dw.DrawLineOp(*iter, _B::GetElementWidth()*scale, *iter, (_B::GetElementWidth()-4)*scale, 2, _axisTickColor);
					PaintTickText<TOP_OF_GRAPH>(dw, iter, *iter, (_B::GetElementWidth()-4)*scale, _axisTextColor, scaledAxisTextFont, scale);
				} else {
					_majorTickMark->Paint(dw, TOP_OF_GRAPH, scale, *iter, _B::GetElementWidth()*scale, _axisTickColor );
					PaintTickText<TOP_OF_GRAPH>(dw, iter, *iter, (_B::GetElementWidth()-_majorTickMark->GetTickLength()-2)*scale, _axisTextColor, scaledAxisTextFont, scale);
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

		virtual void PaintOnPlot_underData(Draw& dw, int otherWidth, int scale)
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
				case FLOAT_OVER_GRAPH:
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
				case FLOAT_OVER_GRAPH:
					break;
			}
		}
	};
}


#endif /* GRIDAXISDRAW_H_ */
