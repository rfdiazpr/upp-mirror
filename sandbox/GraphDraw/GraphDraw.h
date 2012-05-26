#ifndef _GraphDraw_GrapDraw_h
#define _GraphDraw_GrapDraw_h

#include <iostream>

#include <CtrlLib/CtrlLib.h>

#include <Core/Core.h>

#include <ScatterDraw/ScatterDraw.h>

#define IMAGECLASS GraphDrawImg
#define IMAGEFILE <GraphDraw/GraphDraw.iml>
#include <Draw/iml_header.h>

using namespace Upp;

#ifndef TRACE_INFO
#define TRACE_INFO(TXT) //{ std::ostringstream str; str <<  "\n" << TXT; LOG(str.str().c_str()); }
#endif

#ifndef TRACE_ERROR
#define TRACE_ERROR(TXT) //{ std::ostringstream str; str <<  "\n" << TXT; LOG(str.str().c_str()); }
#endif

#include "GraphDrawTypes.h"

#include "CoordinateConverter.h"
#include "GraphFunctions.h"
#include "SeriesConfig.h"
#include "SeriesGroup.h"
#include "GridStepIterator.h"
#include "GraphElement.h"
#include "GridAxisDraw.h"




class FastMarkPlot : public MarkPlot {
private:
	template <class T>
	void DoPaint(T& w, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		w.DrawLine(cp.x, cp.y, cp.x, cp.y+1, 1, markColor);
	}

public:
	inline void Paint(Draw &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		DoPaint(p, scale, cp, size, markColor);
	}
	inline void Paint(Painter &p, const int& scale, const Point& cp, const double& size, const Color& markColor) const
	{
		DoPaint(p, scale, cp, size, markColor);
	}
};

namespace GraphDraw_ns
{


	// ------------------------------------------------------------------------------------------------------------------------------------------------------
	//
	//            G r a p h D r a  w    C l a s s e s
	//
	// ------------------------------------------------------------------------------------------------------------------------------------------------------

	typedef enum {
		MD_DRAW		   = -1,
		MD_ANTIALIASED = MODE_ANTIALIASED,
		MD_NOAA        = MODE_NOAA,
		MD_SUBPIXEL    = MODE_SUBPIXEL
	} DrawMode;


	// ============================
	// CRTP_EmptyGraphDraw   CLASS
	// ============================
	template<class TYPES, class DERIVED>
	class CRTP_EmptyGraphDraw : public SeriesGroup< TYPES, DERIVED > , public GraphElementParent
	{
		public:
		typedef CRTP_EmptyGraphDraw<TYPES, DERIVED> CLASSNAME;

		typedef typename TYPES::TypeCoordConverter                   TypeCoordConverter;
		typedef SeriesGroup<TYPES, DERIVED >                         TypeSeriesGroup;
		typedef SeriesGroup<TYPES, DERIVED >                         _B;



		protected:
		// graph elements draw aound the graph
		Vector< GraphElementFrame* >  _createdElements;
		Vector< GraphElementFrame* >  _leftElements;
		Vector< GraphElementFrame* >  _topElements;
		Vector< GraphElementFrame* >  _rightElements;
		Vector< GraphElementFrame* >  _bottomElements;
		Vector< GraphElementFrame* >  _overElements;
		Vector< TypeCoordConverter* > _xConverters;
		Vector< TypeCoordConverter* > _yConverters;


		Size     _screenPlotSize;
		Rect     _screenRect;  // whole graph screen Rect
		Point    _PlotTopLeft; // TopLeft opoint of plot area inside graph area
		Rect     _plotRect;
		DrawMode _mode;
		bool     _doFastPaint;
		Color    _plotBckgndColor;
		Color    _CtrlBckgndColor;

		inline void updateSizes( const int scale = 1 )
		{
			// --------------
			// GRAPH ELEMENTS
			// --------------
			TypeScreenCoord offsetLeft=0;
			TypeScreenCoord offsetTop=0;
			TypeScreenCoord offsetRight=0;
			TypeScreenCoord offsetBottom=0;
			
			// ======================
			//   calcul des offset
			for (int j = 0; j < _leftElements.GetCount(); j++)
			{
				offsetLeft += _leftElements[j]->GetElementWidth()*scale;
			}
			for (int j = 0; j < _topElements.GetCount(); j++)
			{
				offsetTop += _topElements[j]->GetElementWidth()*scale;
			}
			for (int j = 0; j < _bottomElements.GetCount(); j++)
			{
				offsetBottom += _bottomElements[j]->GetElementWidth()*scale;
			}
			for (int j = 0; j < _rightElements.GetCount(); j++)
			{
				offsetRight += _rightElements[j]->GetElementWidth()*scale;
			}
			
			_screenPlotSize.cx = _screenRect.GetWidth() - offsetLeft - offsetRight;
			_screenPlotSize.cy = _screenRect.GetHeight() - offsetTop - offsetBottom;

			_PlotTopLeft.x = offsetLeft;
			_PlotTopLeft.y = offsetTop;

			_plotRect = Rect(_PlotTopLeft, _screenPlotSize);

			for (int j = 0; j < _xConverters.GetCount(); j++) {
				_xConverters[j]->updateScreenSize( 0, _screenPlotSize.cx );
			}

			for (int j = 0; j < _yConverters.GetCount(); j++) {
				_yConverters[j]->updateScreenSize( _screenPlotSize.cy, 0  );
			}

			// ======================
			//  Update element positions
			Rect r;
			
			r.Set( Point( offsetLeft, offsetTop ), Size(40,40) );
			for (int j = 0; j < _leftElements.GetCount(); j++)
			{
				r.OffsetHorz( -_leftElements[j]->GetElementWidth()*scale );
				r.SetSize( _leftElements[j]->GetElementWidth()*scale ,_screenPlotSize.cy);
				_leftElements[j]->Update();
				_leftElements[j]->SetFrame(r);
			}

			r.Set( Point( offsetLeft, offsetTop ), Size(40,40) );
			for (int j = 0; j < _topElements.GetCount(); j++)
			{
				r.OffsetVert( -_topElements[j]->GetElementWidth()*scale );
				r.SetSize( _screenPlotSize.cx, _topElements[j]->GetElementWidth()*scale);
				_topElements[j]->SetFrame(r);
				_topElements[j]->Update();
			}

			r.Set( Point( offsetLeft, offsetTop+_screenPlotSize.cy ), Size(40,40) );
			for (int j = 0; j < _bottomElements.GetCount(); j++)
			{
				r.SetSize( _screenPlotSize.cx, _bottomElements[j]->GetElementWidth()*scale);
				_bottomElements[j]->SetFrame(r);
				_bottomElements[j]->Update();
				r.OffsetVert( _bottomElements[j]->GetElementWidth()*scale );
			}

			r.Set( Point( offsetLeft+_screenPlotSize.cx, offsetTop ), Size(40,40) );
			for (int j = 0; j < _rightElements.GetCount(); j++)
			{
				r.SetSize( _rightElements[j]->GetElementWidth()*scale ,_screenPlotSize.cy);
				_rightElements[j]->SetFrame(r);
				_rightElements[j]->Update();
				r.OffsetHorz( _rightElements[j]->GetElementWidth()*scale );
			}
		}

		public:
		CRTP_EmptyGraphDraw() : _mode( MD_DRAW ), _doFastPaint(false), _plotBckgndColor( LtGray() ), _CtrlBckgndColor( White() ) {};

		virtual ~CRTP_EmptyGraphDraw() {
			for (int j = 0; j < _createdElements.GetCount(); j++)
			{
				delete ( _createdElements[j] );
			}
			_createdElements.Clear();
		}

		TypeCoordConverter& GetXCoordConverter() { return *TypeSeriesGroup::_currentXConverter; }
		TypeCoordConverter& GetYCoordConverter() { return *TypeSeriesGroup::_currentYConverter; }

		DERIVED& SetPlotBackgroundColor(Color c) { _plotBckgndColor = c; return *static_cast<DERIVED*>(this); }
		DERIVED& SetCtrlBackgroundColor(Color c)     { _CtrlBckgndColor = c; return *static_cast<DERIVED*>(this); }
		DERIVED& SetMode(DrawMode m) { _mode = m; return *static_cast<DERIVED*>(this); }
		DERIVED& SetMode(int m) {
			if ((MD_DRAW<=m) && (m<=MD_SUBPIXEL)) _mode = (DrawMode)m;
			return *static_cast<DERIVED*>(this);
		}

		DERIVED& setScreenSize(Rect r, const int scale=1)	{
			_screenRect = r;
			updateSizes(scale);
			return *static_cast<DERIVED*>(this);
		}

		inline DERIVED& setScreenSize( const int scale=1 )	{
			return setScreenSize(_screenRect, scale);
		}

		TypeCoordConverter& AddXConverter(TypeCoordConverter& conv) {
			_xConverters << &conv;
			TypeSeriesGroup::_currentXConverter = &conv;
			return conv;
		}

		TypeCoordConverter& AddYConverter(TypeCoordConverter& conv) {
			_yConverters << &conv;
			TypeSeriesGroup::_currentYConverter = &conv;
			return conv;
		}


		bool IsValidForZoom(Rect r)
		{
			static const TypeGraphCoord rangeMin = std::numeric_limits<TypeGraphCoord>::epsilon() * 1000.0;
			for (int j = 0; j < _xConverters.GetCount(); j++)
			{
				if ( tabs(_xConverters[j]->toGraph( r.left ) - _xConverters[j]->toGraph( r.right )) < rangeMin ) return false;
			}

			for (int j = 0; j < _yConverters.GetCount(); j++)
			{
				if ( tabs(_yConverters[j]->toGraph( r.bottom ) - _yConverters[j]->toGraph( r.top )) < rangeMin) return false;
			}
			return true;
		}


		virtual void ZoomX(TypeScreenCoord left, TypeScreenCoord right)
		{
			for (int j = 0; j < _xConverters.GetCount(); j++)
			{
				_xConverters[j]->updateGraphSize( _xConverters[j]->toGraph( left ),
															 _xConverters[j]->toGraph( right ));
			}
			Refresh();
		}

		virtual void ZoomY(TypeScreenCoord top, TypeScreenCoord bottom)
		{
			for (int j = 0; j < _yConverters.GetCount(); j++)
			{
				_yConverters[j]->updateGraphSize( _yConverters[j]->toGraph( bottom ),
				                                  _yConverters[j]->toGraph( top ));
			}
			Refresh();
		}

		DERIVED& ZoomOnRect(Rect r)
		{
			if ( IsValidForZoom(r) )
			{
				ZoomX(r.left, r.right);
				ZoomY(r.top, r.bottom);
				Refresh();
			}
			return *static_cast<DERIVED*>(this);
		}

		DERIVED& ApplyInvZoomFactor( TypeGraphCoord zoom)
		{
			return ApplyZoomFactor( 1.0/zoom);
		}

		DERIVED& ApplyZoomFactor( TypeGraphCoord zoom)
		{
			TypeScreenCoord xDelta = (TypeScreenCoord)(_plotRect.GetWidth() * (1.0 - zoom) + .5);
			TypeScreenCoord yDelta = (TypeScreenCoord)(_plotRect.GetHeight() * (1.0 - zoom) + .5);
			Rect r = Rect( -xDelta/2,
								-yDelta/2,
								_plotRect.GetWidth() +xDelta - xDelta/2,
								_plotRect.GetHeight() +yDelta - yDelta/2 );
			return ZoomOnRect(r);
		}

		virtual void ScrollX( TypeScreenCoord xOffset)
		{
			for (int j = 0; j < _xConverters.GetCount(); j++)
			{
				_xConverters[j]->Scroll( xOffset );
			}
			Refresh();
		}

		virtual void ScrollY( TypeScreenCoord yOffset)
		{
			for (int j = 0; j < _yConverters.GetCount(); j++)
			{
				_yConverters[j]->Scroll( yOffset );
			}
			Refresh();
		}

		DERIVED& Scroll( TypeScreenCoord xOffset, TypeScreenCoord yOffset)
		{
			ScrollX(xOffset);
			ScrollY(yOffset);
			return *static_cast<DERIVED*>(this);
		}



		template<class T, int POS_OF_GRAPH>
		T& AddElement(T& v) {
				switch(POS_OF_GRAPH){
					case LEFT_OF_GRAPH:
						_leftElements << &v.SetElementPos(LEFT_OF_GRAPH);
						break;
					case BOTTOM_OF_GRAPH:
						_bottomElements << &v.SetElementPos(BOTTOM_OF_GRAPH);
						break;
					case TOP_OF_GRAPH:
						_topElements << &v.SetElementPos(TOP_OF_GRAPH);
						break;
					case RIGHT_OF_GRAPH:
						_rightElements << &v.SetElementPos(RIGHT_OF_GRAPH);
						break;
					case OVER_GRAPH:
						_overElements << &v.SetElementPos(OVER_GRAPH);
						break;
				}
				v._parent = this;
				return v;
		}

		template<class T, int POS_OF_GRAPH>
		T& AddElementAt(int pos, T& v) {
				switch(POS_OF_GRAPH){
					case LEFT_OF_GRAPH:
						_leftElements.Insert(pos, &v.SetElementPos(LEFT_OF_GRAPH));
						break;
					case BOTTOM_OF_GRAPH:
						_bottomElements.Insert(pos, &v.SetElementPos(BOTTOM_OF_GRAPH));
						break;
					case TOP_OF_GRAPH:
						_topElements.Insert(pos, &v.SetElementPos(TOP_OF_GRAPH));
						break;
					case RIGHT_OF_GRAPH:
						_rightElements.Insert(pos, &v.SetElementPos(RIGHT_OF_GRAPH));
						break;
					case OVER_GRAPH:
						_overElements.Insert(pos, &v.SetElementPos(OVER_GRAPH));
						break;
				}
				v._parent = this;
				return v;
		}

		template<class T>  T& AddLeftElement(T& v)   { return AddElement<T, LEFT_OF_GRAPH>(v); }
		template<class T>  T& AddRightElement(T& v)  { return AddElement<T, RIGHT_OF_GRAPH>(v); }
		template<class T>  T& AddTopElement(T& v)    { return AddElement<T, TOP_OF_GRAPH>(v); }
		template<class T>  T& AddBottomElement(T& v) { return AddElement<T, BOTTOM_OF_GRAPH>(v); }
		template<class T>  T& AddOverElement(T& v)   { return AddElement<T, OVER_GRAPH>(v); }

		template<class T>  T& AddLeftElementAt(int pos, T& v)   { return AddElement<T, LEFT_OF_GRAPH>(pos, v); }
		template<class T>  T& AddRightElementAt(int pos, T& v)  { return AddElement<T, RIGHT_OF_GRAPH>(pos, v); }
		template<class T>  T& AddTopElementAt(int pos, T& v)    { return AddElement<T, TOP_OF_GRAPH>(pos, v); }
		template<class T>  T& AddBottomElementAt(int pos, T& v) { return AddElement<T, BOTTOM_OF_GRAPH>(pos, v); }
		template<class T>  T& AddOverElementAt(int pos, T& v)   { return AddElement<T, OVER_GRAPH>(pos, v); }



		template<class T, int POS_OF_GRAPH>
		T& AddElement(int elementWidth, T& v) {
				v.SetElementWidth(elementWidth);
				return AddElement<T, POS_OF_GRAPH>(v);
		}

		template<class T, int POS_OF_GRAPH>
		T& AddElementAt(int pos, int elementWidth, T& v) {
				v.SetElementWidth(elementWidth);
				return AddElementAt<T, POS_OF_GRAPH>(pos, v);
		}

		template<class T>  T& AddLeftElement(int elementWidth, T& v)   { return AddElement<T, LEFT_OF_GRAPH>(elementWidth, v); }
		template<class T>  T& AddRightElement(int elementWidth, T& v)  { return AddElement<T, RIGHT_OF_GRAPH>(elementWidth, v); }
		template<class T>  T& AddTopElement(int elementWidth, T& v)    { return AddElement<T, TOP_OF_GRAPH>(elementWidth, v); }
		template<class T>  T& AddBottomElement(int elementWidth, T& v) { return AddElement<T, BOTTOM_OF_GRAPH>(elementWidth, v); }
		template<class T>  T& AddOverElement(int elementWidth, T& v)   { return AddElement<T, OVER_GRAPH>(elementWidth, v); }

		template<class T>  T& AddLeftElementAt(int pos, int elementWidth, T& v)   { return AddElementAt<T, LEFT_OF_GRAPH>(pos, elementWidth, v); }
		template<class T>  T& AddRightElementAt(int pos, int elementWidth, T& v)  { return AddElementAt<T, RIGHT_OF_GRAPH>(pos, elementWidth, v); }
		template<class T>  T& AddTopElementAt(int pos, int elementWidth, T& v)    { return AddElementAt<T, TOP_OF_GRAPH>(pos, elementWidth, v); }
		template<class T>  T& AddBottomElementAt(int pos, int elementWidth, T& v) { return AddElementAt<T, BOTTOM_OF_GRAPH>(pos, elementWidth, v); }
		template<class T>  T& AddOverElementAt(int pos, int elementWidth, T& v)   { return AddElementAt<T, OVER_GRAPH>(pos, elementWidth, v); }


		template<class T, int POS_OF_GRAPH>
		T& CloneElement(int elementWidth, T& p) {
			T* e = dynamic_cast<T*>( p.Clone() );
			e->SetElementWidth(elementWidth);
			_createdElements << e; // to manage object destruction
			AddElement<T, POS_OF_GRAPH>(*e);
			return *e;
		}
		template<class T>	T& CloneLeftElement(int elementWidth, T& p)   { return CloneElement<T, LEFT_OF_GRAPH>(elementWidth, p); }
		template<class T>	T& CloneRightElement(int elementWidth, T& p)  { return CloneElement<T, RIGHT_OF_GRAPH>(elementWidth, p); }
		template<class T>	T& CloneTopElement(int elementWidth, T& p)    { return CloneElement<T, TOP_OF_GRAPH>(elementWidth, p); }
		template<class T>	T& CloneBottomElement(int elementWidth, T& p) { return CloneElement<T, BOTTOM_OF_GRAPH>(elementWidth, p); }
		template<class T>	T& CloneOverElement(T& p)   { return CloneElement<OVER_GRAPH>(0, p); }

		template<class T, int POS_OF_GRAPH>
		T& CreateLegendElement(int elementWidth) {
			T* e = new T();
			e->SetSeries(TypeSeriesGroup::series);
			e->SetElementWidth(elementWidth);
			e->_parent = this;
			_createdElements << e; // to manage object destruction
			AddElement<T, POS_OF_GRAPH>(*e);
			return *e;
		}
		template<class T>	T& CreateLeftLegendElement(int elementWidth)   { return CreateLegendElement<T, LEFT_OF_GRAPH>(elementWidth); }
		template<class T>	T& CreateRightLegendElement(int elementWidth)  { return CreateLegendElement<T, RIGHT_OF_GRAPH>(elementWidth); }
		template<class T>	T& CreateTopLegendElement(int elementWidth)    { return CreateLegendElement<T, TOP_OF_GRAPH>(elementWidth); }
		template<class T>	T& CreateBottomLegendElement(int elementWidth) { return CreateLegendElement<T, BOTTOM_OF_GRAPH>(elementWidth); }
		template<class T>	T& CreateOverLegendElement()                   { return CreateLegendElement<OVER_GRAPH>(0); }



		template<class T, int POS_OF_GRAPH>
		T& CreateElement(int elementWidth) {
			T* e = new T();
			e->SetElementWidth(elementWidth);
			e->_parent = this;
			_createdElements << e; // to manage object destruction
			AddElement<T, POS_OF_GRAPH>(*e);
			return *e;
		}
		template<class T>	T& CreateLeftElement(int elementWidth)   { return CreateElement<T, LEFT_OF_GRAPH>(elementWidth); }
		template<class T>	T& CreateRightElement(int elementWidth)  { return CreateElement<T, RIGHT_OF_GRAPH>(elementWidth); }
		template<class T>	T& CreateTopElement(int elementWidth)    { return CreateElement<T, TOP_OF_GRAPH>(elementWidth); }
		template<class T>	T& CreateBottomElement(int elementWidth) { return CreateElement<T, BOTTOM_OF_GRAPH>(elementWidth); }
		template<class T>	T& CreateOverElement()                   { return CreateElement<OVER_GRAPH>(0); }


		template<class T, int POS_OF_GRAPH, class P1>
		T& CreateElement(int elementWidth, P1& p1 ) {
			T* e = new T(p1);
			e->SetElementWidth(elementWidth);
			e->_parent = this;
			_createdElements << e; // to manage object destruction
			AddElement<T, POS_OF_GRAPH>(*e);
			return *e;
		}
		template<class T, class P1>	T& CreateLeftElement(int elementWidth, P1& p1)   { return CreateElement<T, LEFT_OF_GRAPH>(elementWidth, p1); }
		template<class T, class P1>	T& CreateRightElement(int elementWidth, P1& p1)  { return CreateElement<T, RIGHT_OF_GRAPH>(elementWidth, p1); }
		template<class T, class P1>	T& CreateTopElement(int elementWidth, P1& p1)    { return CreateElement<T, TOP_OF_GRAPH>(elementWidth, p1); }
		template<class T, class P1>	T& CreateBottomElement(int elementWidth, P1& p1) { return CreateElement<T, BOTTOM_OF_GRAPH>(elementWidth, p1); }
		template<class T, class P1>	T& CreateOverElement(P1& p1) { return CreateElement<T, OVER_GRAPH>(0, p1); }


		template<class T, int POS_OF_GRAPH, class P1, class P2>
		T& CreateElement(int elementWidth, P1& p1, P2& p2 ) {
			T* e = new T(p1,p2);
			e->SetElementWidth(elementWidth);
			e->_parent = this;
			_createdElements << e; // to manage object destruction
			AddElement<T, POS_OF_GRAPH>(*e);
			return *e;
		}
		template<class T, class P1, class P2>	T& CreateLeftElement(int elementWidth, P1& p1, P2& p2)   { return CreateElement<T, LEFT_OF_GRAPH>(elementWidth, p1, p2); }
		template<class T, class P1, class P2>	T& CreateRightElement(int elementWidth, P1& p1, P2& p2)  { return CreateElement<T, RIGHT_OF_GRAPH>(elementWidth, p1, p2); }
		template<class T, class P1, class P2>	T& CreateTopElement(int elementWidth, P1& p1, P2& p2)    { return CreateElement<T, TOP_OF_GRAPH>(elementWidth, p1, p2); }
		template<class T, class P1, class P2>	T& CreateBottomElement(int elementWidth, P1& p1, P2& p2) { return CreateElement<T, BOTTOM_OF_GRAPH>(elementWidth, p1, p2); }
		template<class T, class P1, class P2>	T& CreateOverElement( P1& p1, P2& p2) { return CreateElement<T, OVER_GRAPH>(0, p1, p2); }


		// Refresh called from child
		virtual void RefreshFromChild( RefreshStrategy doFastPaint ) {
			if (doFastPaint == REFRESH_FAST)  _doFastPaint = true;
			Refresh();
		};

		virtual void Refresh() {};


		Image GetImage(DrawMode mode, Size size, const int scale = 1) {
			Rect _screenRectSvg = _screenRect;
			setScreenSize( size, scale );
#ifndef flagGUI
			ASSERT(mode != MD_DRAW);
#endif
			ImageBuffer ib(size);
			BufferPainter bp(ib, mode);
			Paint(bp, scale);
//			ImageDraw ib(size);
//			Paint(ib, scale);

			setScreenSize( _screenRectSvg );
			return ib;
		}

		inline Image GetImage(DrawMode mode, const int scale=1) {
			return GetImage( mode, _screenRect.Size()*scale, scale );
		}

		inline Image GetImage(const int scale=1) {
			return GetImage( _mode, scale );
		}

		template<class T>
		void Paint(T& dw, int scale)
		{
			// ------------
			// paint graph area background
			// ------------
			if ( !_CtrlBckgndColor.IsNullInstance() ) dw.DrawRect(dw.GetPaintRect(), _CtrlBckgndColor);

			// --------------------------------------
			// BEGIN paint in PLOT AREA
			dw.Clipoff(_PlotTopLeft.x, _PlotTopLeft.y, _screenPlotSize.cx, _screenPlotSize.cy);
			if (!_plotBckgndColor.IsNullInstance()) dw.DrawRect(0, 0, _screenPlotSize.cx, _screenPlotSize.cy, _plotBckgndColor);
			// --------------------------------------

			// --------------
			// GRAPH ELEMENTS on PLOT area   ( X/Y Grid, or anything else )
			// --------------
			for (int j = 0; j < _overElements.GetCount(); j++)
			{
				_overElements[j]->PaintOnPlot(dw, _screenPlotSize.cx, scale);
			}
			for (int j = 0; j < _leftElements.GetCount(); j++)
			{
				_leftElements[j]->PaintOnPlot(dw, _screenPlotSize.cx, scale);
			}

			for (int j = 0; j < _bottomElements.GetCount(); j++)
			{
				_bottomElements[j]->PaintOnPlot(dw, _screenPlotSize.cy, scale);
			}

			for (int j = 0; j < _rightElements.GetCount(); j++)
			{
				_rightElements[j]->PaintOnPlot(dw, _screenPlotSize.cx, scale);
			}

			for (int j = 0; j < _topElements.GetCount(); j++)
			{
				_topElements[j]->PaintOnPlot(dw, _screenPlotSize.cy, scale);
			}

			// ------------
			// paint DATA
			// ------------
			if (!_B::series.IsEmpty())
			{
				for ( int j = 0; j < _B::series.GetCount(); j++)
				{
					if (_B::series[j].opacity == 0 || (!_B::series[j].seriesPlot && !_B::series[j].markPlot))
						continue;

					Vector<Point> p1;

					if (_B::series[j].nbVisiblePoints==0) _B::series[j].nbVisiblePoints = _B::series[j].PointsData()->GetCount();

					// ============================================
					//     CREATE  LIST  OF  POINTS  TO  DRAW
					// ============================================
					unsigned int inc = 1;
					unsigned int nbVisiblePoints = 0;
					int imin, imax;

					typename TYPES::TypeCoordConverter& xConverter = *(_B::series[j].xConverter);
					typename TYPES::TypeCoordConverter& yConverter = *(_B::series[j].yConverter);
//					if (_B::series[j].sequential) {
//						imin = imax = Null;
//						for (int i = 1; i < _B::series[j].PointsData()->GetCount() - 1; ++i) {
//							if (IsNull(imin)) {
//								if (_B::series[j].PointsData()->x(i) >= xMin)
//									imin = i - 1;
//							} else if (IsNull(imax)) {
//								if (_B::series[j].PointsData()->x(i) >= xMin + xRange)
//									imax = i + 1;
//							}
//						}
//						if (IsNull(imin))
//						    imin = 0;
//						if (IsNull(imax))
//						    imax = _B::series[j].PointsData()->GetCount();
//					} else
					if (_B::series[j].PointsData()->IsParam()) { 				// It is a param function
						imin = 0;
						imax = _B::series[j].PointsData()->GetCount();
					} else if (IsNull(_B::series[j].PointsData()->GetCount())) {		// It is a function
						imin = ffloor(xConverter.getGraphMin() - 1);
						imax = ffloor(xConverter.getGraphMax() + 2);
					} else {
					    imin = 0;
					    imax = _B::series[j].PointsData()->GetCount();
					}


					if ( !_doFastPaint )  // DRAW ALL POINTS
					{
						nbVisiblePoints = 0;
						TypeGraphCoord x;
						TypeGraphCoord y;
						for (int c=imin; c<imax; c+=inc)
						{
							x = _B::series[j].PointsData()->x(c);
							y = _B::series[j].PointsData()->y(c);
							if (   ( xConverter.IsInGraphRange( x ) )
							    && ( yConverter.IsInGraphRange( y ) ) )
							{
								++nbVisiblePoints;
							}
							p1 << Point(xConverter.toScreen( x ),
							            yConverter.toScreen( y ));
						}
						_B::series[j].nbVisiblePoints = nbVisiblePoints;
					}
					else  // DO FAST DRAW
					{
						if ( _B::series[j].seriesPlot.IsEmpty() )
						{
							if ( _B::series[j].nbVisiblePoints>800 ) { inc = _B::series[j].nbVisiblePoints/800 + 1; }

							nbVisiblePoints = 0;
							TypeGraphCoord x;
							TypeGraphCoord y;
							for ( int c=imin; c<imax; ++c )
							{
								x = _B::series[j].PointsData()->x(c);
								y = _B::series[j].PointsData()->y(c);
								if ( ( xConverter.IsInGraphRange( x ) )
										&& ( yConverter.IsInGraphRange( y ) ) )
								{
									++nbVisiblePoints;
									if ((c%inc) == 0) p1 << Point(xConverter.toScreen( x ),
									                              yConverter.toScreen( y ));
								}
							}
							_B::series[j].nbVisiblePoints = nbVisiblePoints;
						}
						else
						{
							if ( (tabs(imax-imin)>1000) ) { inc = tabs(imax-imin)/1000 + 1; }
							for ( int c=imin; c<imax; c+=inc)
							{
								p1 << Point(xConverter.toScreen( _B::series[j].PointsData()->x(c)),
								            yConverter.toScreen( _B::series[j].PointsData()->y(c)));
							}
						}
					}

					// Draw lines
					if ( !_B::series[j].seriesPlot.IsEmpty() && (p1.GetCount()>0))
						_B::series[j].seriesPlot->Paint(dw,
						                                 p1,
						                                 scale,
						                                 _B::series[j].opacity,
						                                 fround(_B::series[j].thickness),
						                                 _B::series[j].color,
						                                 _B::series[j].dash,
						                                 LtRed(),
						                                 _B::series[j].fillColor,
						                                 xConverter.getScreenRange() / xConverter.getGraphRange(),
						                                 yConverter.getScreenRange() / yConverter.getGraphRange(),
						                                 int(yConverter.getScreenRange() * (1 + yConverter.getGraphMin() / yConverter.getGraphRange()))
						);

					// Draw marks
					if (_B::series[j].markWidth >= 1 && _B::series[j].markPlot)
					{
						if ( !_B::series[j].markPlot.IsEmpty() )
							for (int c=0; c<p1.GetCount(); ++c)
							{
								_B::series[j].markPlot->Paint(dw,
								                               scale,
								                               p1[c],
								                               _B::series[j].markWidth,
								                               _B::series[j].markColor);
							}
					}
				}
			}


			// --------------------------------------
			dw.End();
			// END of paint in PLOT AREA
			// --------------------------------------

			// --------------
			// GRAPH ELEMENTS
			// --------------
			for (int j = 0; j < _leftElements.GetCount(); j++)
			{
				dw.Offset(_leftElements[j]->GetFrame().TopLeft());
				_leftElements[j]->PaintElement(dw, scale);
				dw.End();
			}

			for (int j = 0; j < _bottomElements.GetCount(); j++)
			{
				dw.Offset(_bottomElements[j]->GetFrame().TopLeft());
				_bottomElements[j]->PaintElement(dw, scale);
				dw.End();
			}

			for (int j = 0; j < _rightElements.GetCount(); j++)
			{
				dw.Offset(_rightElements[j]->GetFrame().TopLeft());
				_rightElements[j]->PaintElement(dw, scale);
				dw.End();
			}

			for (int j = 0; j < _topElements.GetCount(); j++)
			{
				dw.Offset(_topElements[j]->GetFrame().TopLeft());
				_topElements[j]->PaintElement(dw, scale);
				dw.End();
			}

			// --------------
			// GRAPH ELEMENTS on ALL GRAPH area   ( legend, or anything else )
			// --------------
			for (int j = 0; j < _overElements.GetCount(); j++)
			{
				dw.Clipoff( _overElements[j]->GetFrame() );
				_overElements[j]->PaintOverGraph(dw, scale);
				dw.End();
			}
			TRACE_INFO("==================================" );
		}
	};


	struct GraphDrawDefaultTypes {
			typedef DataSource                                     TypeDataSource;
			typedef SeriesPlot                                     TypeSeriesPlot;
			typedef MarkPlot                                       TypeMarkPlot;
			typedef GenericCoordinateConverter                     TypeCoordConverter;
			typedef GridAxisDraw<GraphDrawDefaultTypes>            TypeGridAxisDraw;
			typedef GridStepManager<TypeCoordConverter>            TypeGridStepManager;
			typedef SeriesConfig<GraphDrawDefaultTypes>            TypeSeriesConfig;
	};


	// ============================
	//    CRTP_EmptyGraphDraw   CLASS
	// ============================
	template<class TYPES, class DERIVED>
	class CRTP_StdGraphDraw : public CRTP_EmptyGraphDraw<TYPES, DERIVED >
	{
		public:
		typedef TYPES Types;
		typedef CRTP_StdGraphDraw<TYPES, DERIVED> CLASSNAME;
		typedef CRTP_EmptyGraphDraw<TYPES, DERIVED > BASECLASS;


		typedef typename TYPES::TypeCoordConverter            TypeCoordConverter;
		typedef typename TYPES::TypeGridAxisDraw              TypeGridAxisDraw;
		typedef SeriesGroup<TYPES,CLASSNAME>                  TypeSeriesGroup;

		protected:
		TypeCoordConverter        _xConverter;
		TypeCoordConverter        _yConverter;
		TypeGridAxisDraw          _xGridDraw;
		TypeGridAxisDraw          _yGridDraw;

		public:
		CRTP_StdGraphDraw()
		: _xGridDraw(_xConverter)
		, _yGridDraw(_yConverter)
		{
			_xGridDraw.SetElementWidth(20);
			_xGridDraw.setAxisColor( Blue() ).setAxisTextFont(StdFont()).setAxisTextColor( Blue() ).setAxisTickColor( Red() );
			_xGridDraw.setGridColor( Gray() );
			_xGridDraw.setMajorTickMark( (new RoundTickMark())->SetTickLength( 4 ) );

			_yGridDraw.SetElementWidth(40);
			_yGridDraw.setAxisColor( Blue() ).setAxisTextFont(StdFont()).setAxisTextColor( Blue() ).setAxisTickColor( Red() );
			_yGridDraw.setGridColor( Gray() );
			_yGridDraw.setMajorTickMark( (new TriangleTickMark())->SetTickLength( 8 ) );

			BASECLASS::AddXConverter(_xConverter);
			BASECLASS::AddYConverter(_yConverter);
			BASECLASS::AddLeftElement(_yGridDraw);
			BASECLASS::AddBottomElement(_xGridDraw);

			setGraphSize(0, 100, 0, 100);
		};

		virtual ~CRTP_StdGraphDraw() {}


		CLASSNAME& setGraphSize(Rectf r)
		{
			_xConverter.updateGraphSize(r.TopLeft().x, r.BottomRight().x);
			_yConverter.updateGraphSize(r.TopLeft().y, r.BottomRight().y);
			BASECLASS::updateSizes();
			return *this;
		}

		CLASSNAME& setGraphSize(TypeGraphCoord x0, TypeGraphCoord x1, TypeGraphCoord y0, TypeGraphCoord y1)
		{
			_xConverter.updateGraphSize( x0, x1);
			_yConverter.updateGraphSize( y0, y1);
			BASECLASS::updateSizes();
			return *this;
		}

		TypeCoordConverter& GetXCoordConverter() { return _xConverter; }
		TypeCoordConverter& GetYCoordConverter() { return _yConverter; }

		TypeGridAxisDraw& GetXGridAxisDraw() { return _xGridDraw; }
		TypeGridAxisDraw& GetYGridAxisDraw() { return _yGridDraw; }

		CLASSNAME& setXAxisRectWidth(int v) { _xGridDraw.SetElementWidth(v); return *this; }
		CLASSNAME& setYAxisRectWidth(int v) { _yGridDraw.SetElementWidth(v); return *this; }


		CLASSNAME& setXNbGridSteps(int v) { _xGridDraw.GetGridStepManager().SetNbSteps(v); return *this; }
		CLASSNAME& setYNbGridSteps(int v) { _yGridDraw.GetGridStepManager().SetNbSteps(v); return *this; }
	};
};


#endif
