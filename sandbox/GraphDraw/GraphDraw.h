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

#include "GraphFunctions.h"
#include "SeriesConfig.h"
#include "SeriesGroup.h"
#include "CoordinateConverter.h"
#include "GridStepIterator.h"
#include "GraphElement.h"
#include "GridAxisDraw.h"





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
	//    EmptyGraphDraw   CLASS
	// ============================
	template<class TYPES>
	class EmptyGraphDraw : public SeriesGroup< TYPES, EmptyGraphDraw<TYPES> > , GraphElementParent
	{
		public:
		typedef EmptyGraphDraw<TYPES> CLASSNAME;

		typedef typename TYPES::TypeCoordConverter                   TypeCoordConverter;
		typedef SeriesGroup<TYPES,CLASSNAME >                        TypeSeriesGroup;
		typedef typename TYPES::TypeCoordConverter::TypeScreenCoord  TypeScreenCoord;
		typedef typename TYPES::TypeCoordConverter::TypeGraphCoord   TypeGraphCoord;



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
#define _seriesList  TypeSeriesGroup::series

		Size     _screenPlotSize;
		Rect     _screenRect;  // whole graph screen Rect
		Point    _PlotTopLeft; // TopLeft opoint of plot area inside graph area
		Rect     _plotRect;
		DrawMode _mode;
		bool     _doFastPaint;

		inline void updateSizes()
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
				offsetLeft += _leftElements[j]->GetElementWidth();
			}
			for (int j = 0; j < _topElements.GetCount(); j++)
			{
				offsetTop += _topElements[j]->GetElementWidth();
			}
			for (int j = 0; j < _bottomElements.GetCount(); j++)
			{
				offsetBottom += _bottomElements[j]->GetElementWidth();
			}
			for (int j = 0; j < _rightElements.GetCount(); j++)
			{
				offsetRight += _rightElements[j]->GetElementWidth();
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
				r.OffsetHorz( -_leftElements[j]->GetElementWidth() );
				r.SetSize( _leftElements[j]->GetElementWidth() ,_screenPlotSize.cy);
				_leftElements[j]->Update();
				_leftElements[j]->SetFrame(r);
			}

			r.Set( Point( offsetLeft, offsetTop ), Size(40,40) );
			for (int j = 0; j < _topElements.GetCount(); j++)
			{
				r.OffsetVert( -_topElements[j]->GetElementWidth() );
				r.SetSize( _screenPlotSize.cx, _topElements[j]->GetElementWidth());
				_topElements[j]->SetFrame(r);
				_topElements[j]->Update();
			}

			r.Set( Point( offsetLeft, offsetTop+_screenPlotSize.cy ), Size(40,40) );
			for (int j = 0; j < _bottomElements.GetCount(); j++)
			{
				r.SetSize( _screenPlotSize.cx, _bottomElements[j]->GetElementWidth());
				_bottomElements[j]->SetFrame(r);
				_bottomElements[j]->Update();
				r.OffsetVert( _bottomElements[j]->GetElementWidth() );
			}

			r.Set( Point( offsetLeft+_screenPlotSize.cx, offsetTop ), Size(40,40) );
			for (int j = 0; j < _rightElements.GetCount(); j++)
			{
				r.SetSize( _rightElements[j]->GetElementWidth() ,_screenPlotSize.cy);
				_rightElements[j]->SetFrame(r);
				_rightElements[j]->Update();
				r.OffsetHorz( _rightElements[j]->GetElementWidth() );
			}
		}

		public:
		EmptyGraphDraw() : _mode( MD_DRAW ), _doFastPaint(false) {};

		virtual ~EmptyGraphDraw() {
			for (int j = 0; j < _createdElements.GetCount(); j++)
			{
				delete ( _createdElements[j] );
			}
			_createdElements.Clear();
		}

		TypeCoordConverter& GetXCoordConverter() { return *TypeSeriesGroup::_currentXConverter; }
		TypeCoordConverter& GetYCoordConverter() { return *TypeSeriesGroup::_currentYConverter; }


		CLASSNAME& SetMode(DrawMode m) { _mode = m; return *this; }

		CLASSNAME& setScreenSize(Rect r)	{
			_screenRect = r;
			updateSizes();
			return *this;
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
				if ( fabs(_xConverters[j]->toGraph( r.left ) - _xConverters[j]->toGraph( r.right )) < rangeMin ) return false;
			}

			for (int j = 0; j < _yConverters.GetCount(); j++)
			{
				if ( fabs(_yConverters[j]->toGraph( r.bottom ) - _yConverters[j]->toGraph( r.top )) < rangeMin) return false;
			}
			return true;
		}


		virtual CLASSNAME& ZoomX(TypeScreenCoord left, TypeScreenCoord right)
		{
			for (int j = 0; j < _xConverters.GetCount(); j++)
			{
				_xConverters[j]->updateGraphSize( _xConverters[j]->toGraph( left ),
															 _xConverters[j]->toGraph( right ));
			}
			Refresh();
			return *this;
		}

		virtual CLASSNAME& ZoomY(TypeScreenCoord top, TypeScreenCoord bottom)
		{
			for (int j = 0; j < _yConverters.GetCount(); j++)
			{
				_yConverters[j]->updateGraphSize( _yConverters[j]->toGraph( bottom ),
				                                  _yConverters[j]->toGraph( top ));
			}
			Refresh();
			return *this;
		}

		CLASSNAME& ZoomOnRect(Rect r)
		{
			if ( IsValidForZoom(r) )
			{
				ZoomX(r.left, r.right);
				ZoomY(r.top, r.bottom);
				Refresh();
			}
			return *this;
		}

		CLASSNAME& ApplyInvZoomFactor( TypeGraphCoord zoom)
		{
			return ApplyZoomFactor( 1.0/zoom);
		}

		CLASSNAME& ApplyZoomFactor( TypeGraphCoord zoom)
		{
			typename TYPES::TypeCoordConverter::TypeScreenCoord xDelta = _plotRect.GetWidth() * (1.0 - zoom) + .5;
			typename TYPES::TypeCoordConverter::TypeScreenCoord yDelta = _plotRect.GetHeight() * (1.0 - zoom) + .5;
			Rect r = Rect( -xDelta/2,
								-yDelta/2,
								_plotRect.GetWidth() +xDelta - xDelta/2,
								_plotRect.GetHeight() +yDelta - yDelta/2 );
			return ZoomOnRect(r);
		}

		virtual CLASSNAME& ScrollX( TypeScreenCoord xOffset)
		{
			for (int j = 0; j < _xConverters.GetCount(); j++)
			{
				_xConverters[j]->updateGraphSize( _xConverters[j]->toGraph( _xConverters[j]->getScreenMin() - xOffset ),
				                                  _xConverters[j]->toGraph( _xConverters[j]->getScreenMax() - xOffset ));
			}
			Refresh();
			return *this;
		}

		virtual CLASSNAME& ScrollY( TypeScreenCoord yOffset)
		{
			for (int j = 0; j < _yConverters.GetCount(); j++)
			{
				_yConverters[j]->updateGraphSize( _yConverters[j]->toGraph( _yConverters[j]->getScreenMin() - yOffset ),
				                                  _yConverters[j]->toGraph( _yConverters[j]->getScreenMax() - yOffset ));
			}
			Refresh();
			return *this;
		}

		CLASSNAME& Scroll( TypeScreenCoord xOffset, TypeScreenCoord yOffset)
		{
			ScrollX(xOffset);
			ScrollY(yOffset);
			return *this;
		}



		template<class T, int POS_OF_GRAPH>
		CLASSNAME& AddElement(T& v) {
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
				return *this;
		}

		template<class T>  CLASSNAME& AddLeftElement(T& v)   { return AddElement<T, LEFT_OF_GRAPH>(v); }
		template<class T>  CLASSNAME& AddRightElement(T& v)  { return AddElement<T, RIGHT_OF_GRAPH>(v); }
		template<class T>  CLASSNAME& AddTopElement(T& v)    { return AddElement<T, TOP_OF_GRAPH>(v); }
		template<class T>  CLASSNAME& AddBottomElement(T& v) { return AddElement<T, BOTTOM_OF_GRAPH>(v); }
		template<class T>  CLASSNAME& AddOverElement(T& v)   { return AddElement<T, OVER_GRAPH>(v); }



		template<class T, int POS_OF_GRAPH>
		CLASSNAME& AddElement(int elementWidth, T& v) {
				v.SetElementWidth(elementWidth);
				return AddElement<T, POS_OF_GRAPH>(v);
		}

		template<class T>  CLASSNAME& AddLeftElement(int elementWidth, T& v)   { return AddElement<T, LEFT_OF_GRAPH>(elementWidth, v); }
		template<class T>  CLASSNAME& AddRightElement(int elementWidth, T& v)  { return AddElement<T, RIGHT_OF_GRAPH>(elementWidth, v); }
		template<class T>  CLASSNAME& AddTopElement(int elementWidth, T& v)    { return AddElement<T, TOP_OF_GRAPH>(elementWidth, v); }
		template<class T>  CLASSNAME& AddBottomElement(int elementWidth, T& v) { return AddElement<T, BOTTOM_OF_GRAPH>(elementWidth, v); }
		template<class T>  CLASSNAME& AddOverElement(int elementWidth, T& v)   { return AddElement<T, OVER_GRAPH>(elementWidth, v); }


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
		virtual void RefreshFromChild( RefreshStrategy doFastPaint )
		{
			if (doFastPaint == REFRESH_FAST)  _doFastPaint = true;
			Refresh();
		};

		virtual void Refresh() {};

		//			template<class T>
		//			void Paint(T& dw)
		void Paint(Draw& dw, int scale=1)
		{

			// ------------
			// paint graph area background
			// ------------
			dw.DrawRect(dw.GetPaintRect(), White());

			// --------------------------------------
			// BEGIN paint in PLOT AREA
			dw.Clipoff(_PlotTopLeft.x, _PlotTopLeft.y, _screenPlotSize.cx, _screenPlotSize.cy);
			dw.DrawRect(0, 0, _screenPlotSize.cx, _screenPlotSize.cy, LtGray());
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
			if (!_seriesList.IsEmpty())
			{
				for (unsigned int j = 0; j < _seriesList.GetCount(); j++)
				{
					if (_seriesList[j].opacity == 0 || (!_seriesList[j].seriesPlot && !_seriesList[j].markPlot))
						continue;

					Vector<Point> p1;

					if (_seriesList[j].nbVisiblePoints==0) _seriesList[j].nbVisiblePoints = _seriesList[j].PointsData()->GetCount();

					// ============================================
					//     CREATE  LIST  OF  POINTS  TO  DRAW
					// ============================================
					unsigned int inc = 1;
					unsigned int nbVisiblePoints = 0;
					int imin, imax;

//					if (_seriesList[j].sequential) {
//						imin = imax = Null;
//						for (int i = 1; i < _seriesList[j].PointsData()->GetCount() - 1; ++i) {
//							if (IsNull(imin)) {
//								if (_seriesList[j].PointsData()->x(i) >= xMin)
//									imin = i - 1;
//							} else if (IsNull(imax)) {
//								if (_seriesList[j].PointsData()->x(i) >= xMin + xRange)
//									imax = i + 1;
//							}
//						}
//						if (IsNull(imin))
//						    imin = 0;
//						if (IsNull(imax))
//						    imax = _seriesList[j].PointsData()->GetCount();
//					} else
					if (_seriesList[j].PointsData()->IsParam()) { 				// It is a param function
						imin = 0;
						imax = _seriesList[j].PointsData()->GetCount();
					} else if (IsNull(_seriesList[j].PointsData()->GetCount())) {		// It is a function
						imin = _seriesList[j].xConverter->getGraphMin() - 1;
						imax = _seriesList[j].xConverter->getGraphMax() + 1;
					} else {
					    imin = 0;
					    imax = _seriesList[j].PointsData()->GetCount();
					}


					if ( !_doFastPaint )  // DRAW ALL POINTS
					{
						nbVisiblePoints = 0;
						TypeGraphCoord x;
						TypeGraphCoord y;
						for (int c=imin; c<imax; c+=inc)
						{
							x = _seriesList[j].PointsData()->x(c);
							y = _seriesList[j].PointsData()->y(c);
							if (   ( _seriesList[j].xConverter->IsInGraphRange( x ) )
							    && ( _seriesList[j].yConverter->IsInGraphRange( y ) ) )
							{
								++nbVisiblePoints;
							}
							p1 << Point(_seriesList[j].xConverter->toScreen( x ),
							            _seriesList[j].yConverter->toScreen( y ));
						}
						_seriesList[j].nbVisiblePoints = nbVisiblePoints;
					}
					else  // DO FAST DRAW
					{
						if ( _seriesList[j].seriesPlot.IsEmpty() )
						{
							if ( _seriesList[j].nbVisiblePoints>800 ) { inc = _seriesList[j].nbVisiblePoints/800 + 1; }

							nbVisiblePoints = 0;
							TypeGraphCoord x;
							TypeGraphCoord y;
							for ( int c=imin; c<imax; ++c )
							{
								x = _seriesList[j].PointsData()->x(c);
								y = _seriesList[j].PointsData()->y(c);
								if ( ( _seriesList[j].xConverter->IsInGraphRange( x ) )
										&& ( _seriesList[j].yConverter->IsInGraphRange( y ) ) )
								{
									++nbVisiblePoints;
									if ((c%inc) == 0) p1 << Point(_seriesList[j].xConverter->toScreen( x ),
									                              _seriesList[j].yConverter->toScreen( y ));
								}
							}
							_seriesList[j].nbVisiblePoints = nbVisiblePoints;
						}
						else
						{
							if ( (fabs(imax-imin)>1000) ) { inc = fabs(imax-imin)/1000 + 1; }
							for ( int c=imin; c<imax; c+=inc)
							{
								p1 << Point(_seriesList[j].xConverter->toScreen( _seriesList[j].PointsData()->x(c)),
								            _seriesList[j].yConverter->toScreen( _seriesList[j].PointsData()->y(c)));
							}
						}
					}

					// Draw lines
					if ( !_seriesList[j].seriesPlot.IsEmpty() )
						_seriesList[j].seriesPlot->Paint(dw,
						                                 p1,
						                                 scale,
						                                 _seriesList[j].opacity,
						                                 fround(_seriesList[j].thickness),
						                                 _seriesList[j].color,
						                                 _seriesList[j].dash,
						                                 LtRed(),
						                                 _seriesList[j].fillColor,
						                                 1,//_xConverter.getGraphToScreenScale(),//l / xRange,
						                                 1,//_yConverter.getGraphToScreenScale(),//h / yRange,
						                                 0//int(h * (1 + yMin / yRange)));
						);

					// Draw marks
					if (_seriesList[j].markWidth >= 1 && _seriesList[j].markPlot)
					{
						if ( !_seriesList[j].markPlot.IsEmpty() )
							for (unsigned int c=0; c<p1.GetCount(); ++c)
							{
								_seriesList[j].markPlot->Paint(dw,
								                               scale,
								                               p1[c],
								                               _seriesList[j].markWidth,
								                               _seriesList[j].markColor);
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


	// ============================
	//    EmptyGraphDraw   CLASS
	// ============================
	struct GraphDrawDefaultTypes {
			typedef DataSource                                     TypeDataSource;
			typedef SeriesPlot                                     TypeSeriesPlot;
			typedef MarkPlot                                       TypeMarkPlot;
			typedef GenericCoordinateConverter<Upp::int32, double> TypeCoordConverter;
			typedef GridAxisDraw<GraphDrawDefaultTypes>            TypeGridAxisDraw;
			typedef GridStepManager<TypeCoordConverter>            TypeGridStepManager;
			typedef SeriesConfig<GraphDrawDefaultTypes>            TypeSeriesConfig;
	};

	template<class TYPES = GraphDrawDefaultTypes>
	class StdGraphDraw : public EmptyGraphDraw<TYPES>
	{
		public:
		typedef TYPES Types;
		typedef StdGraphDraw<TYPES> CLASSNAME;
		typedef EmptyGraphDraw<TYPES> BASECLASS;


		typedef typename TYPES::TypeCoordConverter            TypeCoordConverter;
		typedef typename TYPES::TypeGridAxisDraw              TypeGridAxisDraw;
		typedef SeriesGroup<TYPES,CLASSNAME>                  TypeSeriesGroup;
		typedef typename TypeCoordConverter::TypeScreenCoord  ScreenCoord;
		typedef typename TypeCoordConverter::TypeGraphCoord   GraphCoord;

		protected:
		TypeCoordConverter        _xConverter;
		TypeCoordConverter        _yConverter;
		TypeGridAxisDraw          _xGridDraw;
		TypeGridAxisDraw          _yGridDraw;

#define _seriesList  TypeSeriesGroup::series

		public:
		StdGraphDraw()
		: _xGridDraw(_xConverter)
		, _yGridDraw(_yConverter)
		{
			_xGridDraw.SetElementWidth(20);
			_xGridDraw.setAxisColor( Blue() ).setAxisTextFont(StdFont()).setAxisTextColor( Blue() ).setAxisTickColor( Red() );
			_xGridDraw.setGridColor( Gray() );
			_xGridDraw.setMajorTickMark( (new RoundTickMark())->SetTickLength( 4 ) );

			_yGridDraw.SetElementWidth(30);
			_yGridDraw.setAxisColor( Blue() ).setAxisTextFont(StdFont()).setAxisTextColor( Blue() ).setAxisTickColor( Red() );
			_yGridDraw.setGridColor( Gray() );
			_yGridDraw.setMajorTickMark( (new TriangleTickMark())->SetTickLength( 8 ) );

			BASECLASS::AddXConverter(_xConverter);
			BASECLASS::AddYConverter(_yConverter);
			BASECLASS::AddLeftElement(_yGridDraw);
			BASECLASS::AddBottomElement(_xGridDraw);

			setGraphSize(0, 100, 0, 100);
		};

		virtual ~StdGraphDraw() {}


		CLASSNAME& setGraphSize(Rectf r)
		{
			_xConverter.updateGraphSize(r.TopLeft().x, r.BottomRight().x);
			_yConverter.updateGraphSize(r.TopLeft().y, r.BottomRight().y);
			BASECLASS::updateSizes();
			return *this;
		}

		CLASSNAME& setGraphSize(GraphCoord x0, GraphCoord x1, GraphCoord y0, GraphCoord y1)
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
