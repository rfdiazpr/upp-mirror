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
		Vector< GraphElementFrame* >  _drawElements;
		Vector< GraphElementFrame* >  _createdElements;
		Vector< TypeCoordConverter* > _xConverters;
		Vector< TypeCoordConverter* > _yConverters;

		Rect     _screenRect;  // whole graph screen Rect
		Rect     _plotRect;
		DrawMode _mode;
		bool     _doFastPaint;
		Color    _plotBckgndColor;
		Color    _CtrlBckgndColor;

		int       _topMarginWidth;
		int       _bottomMarginWidth;
		int       _leftMarginWidth;
		int       _rightMarginWidth;
		
		
		// helper method
		void AppendElementToRect(GraphElementFrame& element, Rect& fromRect, const int scale)
		{
			Rect res = fromRect;
			switch(element.GetElementPos()) {
				case TOP_OF_GRAPH:
					res.bottom = res.top+element.GetElementWidth()*scale;
					fromRect.top = res.bottom;
					element.SetFrame(res);
					break;
				case BOTTOM_OF_GRAPH:
					res.top = res.bottom-element.GetElementWidth()*scale;
					fromRect.bottom = res.top;
					element.SetFrame(res);
					break;
				case LEFT_OF_GRAPH:
					res.right = res.left+element.GetElementWidth()*scale;
					fromRect.left = res.right;
					element.SetFrame(res);
					break;
				case RIGHT_OF_GRAPH:
					res.left = res.right-element.GetElementWidth()*scale;
					fromRect.right = res.left;
					element.SetFrame(res);
					break;
				case OVER_GRAPH:
					res = element.GetOverFrame();
					res = Rect( res.TopLeft()*scale, res.Size()*scale );
					element.SetFrame(res);
					break;
			}
		}

		inline void updateSizes( const int scale = 1 )
		{
			_plotRect = _screenRect;
			_plotRect.left   += _leftMarginWidth*scale;
			_plotRect.right  -= _rightMarginWidth*scale;
			_plotRect.top    += _topMarginWidth*scale;
			_plotRect.bottom -= _bottomMarginWidth*scale;

			Sort(_drawElements, compareGraphElementFrame);

			for (int j = _drawElements.GetCount()-1; j>=0; j--) {
				if (!_drawElements[j]->IsHidden()) {
					AppendElementToRect(*(_drawElements[j]), _plotRect, scale);
				}
			}

			for (int j = 0; j < _drawElements.GetCount(); j++) {
				if (!_drawElements[j]->IsHidden()) {
					_drawElements[j]->AdjustToPlotRect( _plotRect );
				}
			}

			for (int j = 0; j < _xConverters.GetCount(); j++) {
				_xConverters[j]->updateScreenSize( 0, _plotRect.GetWidth() );
			}

			for (int j = 0; j < _yConverters.GetCount(); j++) {
				_yConverters[j]->updateScreenSize( _plotRect.GetHeight(), 0  );
			}
		}
		

		public:
		CRTP_EmptyGraphDraw()
		: _mode( MD_DRAW )
		, _doFastPaint(false)
		, _plotBckgndColor( LtGray() )
		, _CtrlBckgndColor( White() )
		, _topMarginWidth(0)
		, _bottomMarginWidth(0)
		, _leftMarginWidth(0)
		, _rightMarginWidth(0)
		{};


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

		DERIVED& SetTopMargin(int v)    { _topMarginWidth = v;    return *static_cast<DERIVED*>(this); }
		DERIVED& SetBottomMargin(int v) { _bottomMarginWidth = v; return *static_cast<DERIVED*>(this); }
		DERIVED& SetLeftMargin(int v)   { _leftMarginWidth = v;   return *static_cast<DERIVED*>(this); }
		DERIVED& SetRightMargin(int v)  { _rightMarginWidth = v;  return *static_cast<DERIVED*>(this); }

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

		void SetCurrentXConverter(int n) {
			ASSERT( n < _xConverters.GetCount() );
			TypeSeriesGroup::_currentXConverter =  _xConverters[n];
		}

		void SetCurrentYConverter(int n) {
			ASSERT( n < _yConverters.GetCount() );
			TypeSeriesGroup::_currentYConverter =  _yConverters[n];
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
		T& AddElement(T& v, int stackPrio) {
			v._parent = this;
			v.SetStackingPriority(stackPrio);
			_drawElements << &v.SetElementPos((ElementPosition)POS_OF_GRAPH);
			Sort(_drawElements, compareGraphElementFrame);
			return v;
		}

		template<class T, int POS_OF_GRAPH>
		T& AddElement(int elementWidth, T& v, int stackPrio) {
				v.SetElementWidth(elementWidth);
				return AddElement<T, POS_OF_GRAPH>(v, stackPrio);
				
		}

		template<class T>  T& AddLeftElement(T& v, int stackPrio)   { return AddElement<T, LEFT_OF_GRAPH>(v, stackPrio); }
		template<class T>  T& AddRightElement(T& v, int stackPrio)  { return AddElement<T, RIGHT_OF_GRAPH>(v, stackPrio); }
		template<class T>  T& AddTopElement(T& v, int stackPrio)    { return AddElement<T, TOP_OF_GRAPH>(v, stackPrio); }
		template<class T>  T& AddBottomElement(T& v, int stackPrio) { return AddElement<T, BOTTOM_OF_GRAPH>(v, stackPrio); }
		template<class T>  T& AddOverElement(T& v, int stackPrio)   { return AddElement<T, OVER_GRAPH>(v, stackPrio); }

		template<class T>  T& AddLeftElement(int elementWidth, T& v, int stackPrio)   { return AddElement<T, LEFT_OF_GRAPH>(elementWidth, v, stackPrio); }
		template<class T>  T& AddRightElement(int elementWidth, T& v, int stackPrio)  { return AddElement<T, RIGHT_OF_GRAPH>(elementWidth, v, stackPrio); }
		template<class T>  T& AddTopElement(int elementWidth, T& v, int stackPrio)    { return AddElement<T, TOP_OF_GRAPH>(elementWidth, v, stackPrio); }
		template<class T>  T& AddBottomElement(int elementWidth, T& v, int stackPrio) { return AddElement<T, BOTTOM_OF_GRAPH>(elementWidth, v, stackPrio); }
		template<class T>  T& AddOverElement(int elementWidth, T& v, int stackPrio)   { return AddElement<T, OVER_GRAPH>(elementWidth, v, stackPrio); }



/*
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


		template<class T>  T& AddLeftElementAt(int pos, T& v)   { return AddElementAt<T, LEFT_OF_GRAPH>(pos, v); }
		template<class T>  T& AddRightElementAt(int pos, T& v)  { return AddElementAt<T, RIGHT_OF_GRAPH>(pos, v); }
		template<class T>  T& AddTopElementAt(int pos, T& v)    { return AddElementAt<T, TOP_OF_GRAPH>(pos, v); }
		template<class T>  T& AddBottomElementAt(int pos, T& v) { return AddElementAt<T, BOTTOM_OF_GRAPH>(pos, v); }
		template<class T>  T& AddOverElementAt(int pos, T& v)   { return AddElementAt<T, OVER_GRAPH>(pos, v); }


		template<class T, int POS_OF_GRAPH>
		T& AddElementAt(int pos, int elementWidth, T& v) {
				v.SetElementWidth(elementWidth);
				return AddElementAt<T, POS_OF_GRAPH>(pos, v);
		}


		template<class T>  T& AddLeftElementAt(int pos, int elementWidth, T& v)   { return AddElementAt<T, LEFT_OF_GRAPH>(pos, elementWidth, v); }
		template<class T>  T& AddRightElementAt(int pos, int elementWidth, T& v)  { return AddElementAt<T, RIGHT_OF_GRAPH>(pos, elementWidth, v); }
		template<class T>  T& AddTopElementAt(int pos, int elementWidth, T& v)    { return AddElementAt<T, TOP_OF_GRAPH>(pos, elementWidth, v); }
		template<class T>  T& AddBottomElementAt(int pos, int elementWidth, T& v) { return AddElementAt<T, BOTTOM_OF_GRAPH>(pos, elementWidth, v); }
		template<class T>  T& AddOverElementAt(int pos, int elementWidth, T& v)   { return AddElementAt<T, OVER_GRAPH>(pos, elementWidth, v); }
*/

		template<class T, int POS_OF_GRAPH>
		T& CloneElement(int elementWidth, T& p, int stackPrio=-1) {
			T* e = dynamic_cast<T*>( p.Clone() );
			e->SetElementWidth(elementWidth);
			if (stackPrio==-1) {
				stackPrio = e->GetStackingPriority();
			}
			_createdElements << e; // to manage object destruction
			AddElement<T, POS_OF_GRAPH>(*e, stackPrio);
			return *e;
		}
		template<class T>	T& CloneLeftElement(int elementWidth, T& p, int stackPrio=-1)   { return CloneElement<T, LEFT_OF_GRAPH>(elementWidth, p, stackPrio); }
		template<class T>	T& CloneRightElement(int elementWidth, T& p, int stackPrio=-1)  { return CloneElement<T, RIGHT_OF_GRAPH>(elementWidth, p, stackPrio); }
		template<class T>	T& CloneTopElement(int elementWidth, T& p, int stackPrio=-1)    { return CloneElement<T, TOP_OF_GRAPH>(elementWidth, p, stackPrio); }
		template<class T>	T& CloneBottomElement(int elementWidth, T& p, int stackPrio=-1) { return CloneElement<T, BOTTOM_OF_GRAPH>(elementWidth, p, stackPrio); }
		template<class T>	T& CloneOverElement(T& p, int stackPrio=-1)                     { return CloneElement<OVER_GRAPH>(0, p, stackPrio); }


		template<class T, int POS_OF_GRAPH>
		T& AddLegendElement(int elementWidth, T& legend, int stackPrio) {
			legend.SetParentCtrl(* static_cast<DERIVED*>(this));
			legend.SetSeries(TypeSeriesGroup::series);
			legend.SetElementWidth(elementWidth);
			legend._parent = this;
			AddElement<T, POS_OF_GRAPH>(legend,stackPrio);
			return legend;
		}
		template<class T>	T& AddLeftLegendElement(int elementWidth, T& legend, int stackPrio)   { return AddLegendElement<T, LEFT_OF_GRAPH>(elementWidth, legend, stackPrio); }
		template<class T>	T& AddRightLegendElement(int elementWidth, T& legend, int stackPrio)  { return AddLegendElement<T, RIGHT_OF_GRAPH>(elementWidth, legend, stackPrio); }
		template<class T>	T& AddTopLegendElement(int elementWidth, T& legend, int stackPrio)    { return AddLegendElement<T, TOP_OF_GRAPH>(elementWidth, legend, stackPrio); }
		template<class T>	T& AddBottomLegendElement(int elementWidth, T& legend, int stackPrio) { return AddLegendElement<T, BOTTOM_OF_GRAPH>(elementWidth, legend, stackPrio); }
		template<class T>	T& AddOverLegendElement(int elementWidth, T& legend, int stackPrio)   { return AddLegendElement<T, OVER_GRAPH>(elementWidth, legend, stackPrio); }

		template<class T, int POS_OF_GRAPH>
		T& CreateLegendElement(int elementWidth, int stackPrio) {
			T* e = new T();
			_createdElements << e; // to manage object destruction
			return AddLegendElement(elementWidth, *e, stackPrio);
		}
		template<class T>	T& CreateLeftLegendElement(int elementWidth, int stackPrio)   { return CreateLegendElement<T, LEFT_OF_GRAPH>(elementWidth, stackPrio); }
		template<class T>	T& CreateRightLegendElement(int elementWidth, int stackPrio)  { return CreateLegendElement<T, RIGHT_OF_GRAPH>(elementWidth, stackPrio); }
		template<class T>	T& CreateTopLegendElement(int elementWidth, int stackPrio)    { return CreateLegendElement<T, TOP_OF_GRAPH>(elementWidth, stackPrio); }
		template<class T>	T& CreateBottomLegendElement(int elementWidth, int stackPrio) { return CreateLegendElement<T, BOTTOM_OF_GRAPH>(elementWidth, stackPrio); }
		template<class T>	T& CreateOverLegendElement(int stackPrio)                     { return CreateLegendElement<OVER_GRAPH>(0, stackPrio); }



		template<class T, int POS_OF_GRAPH>
		T& CreateElement(int elementWidth, int stackPrio) {
			T* e = new T();
			e->SetElementWidth(elementWidth);
			e->_parent = this;
			_createdElements << e; // to manage object destruction
			AddElement<T, POS_OF_GRAPH>(*e, stackPrio);
			return *e;
		}
		template<class T>	T& CreateLeftElement(int elementWidth, int stackPrio)   { return CreateElement<T, LEFT_OF_GRAPH>(elementWidth, stackPrio); }
		template<class T>	T& CreateRightElement(int elementWidth, int stackPrio)  { return CreateElement<T, RIGHT_OF_GRAPH>(elementWidth, stackPrio); }
		template<class T>	T& CreateTopElement(int elementWidth, int stackPrio)    { return CreateElement<T, TOP_OF_GRAPH>(elementWidth, stackPrio); }
		template<class T>	T& CreateBottomElement(int elementWidth, int stackPrio) { return CreateElement<T, BOTTOM_OF_GRAPH>(elementWidth, stackPrio); }
		template<class T>	T& CreateOverElement(int stackPrio)                     { return CreateElement<OVER_GRAPH>(0, stackPrio); }


		template<class T, int POS_OF_GRAPH, class P1>
		T& CreateElement1(int elementWidth, int stackPrio, P1& p1 ) {
			T* e = new T(p1);
			e->SetElementWidth(elementWidth);
			e->_parent = this;
			_createdElements << e; // to manage object destruction
			AddElement<T, POS_OF_GRAPH>(*e, stackPrio);
			return *e;
		}
		template<class T, class P1>	T& CreateLeftElement1(int elementWidth, int stackPrio, P1& p1)   { return CreateElement1<T, LEFT_OF_GRAPH>(elementWidth, stackPrio, p1); }
		template<class T, class P1>	T& CreateRightElement1(int elementWidth, int stackPrio, P1& p1)  { return CreateElement1<T, RIGHT_OF_GRAPH>(elementWidth, stackPrio, p1); }
		template<class T, class P1>	T& CreateTopElement1(int elementWidth, int stackPrio, P1& p1)    { return CreateElement1<T, TOP_OF_GRAPH>(elementWidth, stackPrio, p1); }
		template<class T, class P1>	T& CreateBottomElement1(int elementWidth, int stackPrio, P1& p1) { return CreateElement1<T, BOTTOM_OF_GRAPH>(elementWidth, stackPrio, p1); }
		template<class T, class P1>	T& CreateOverElement1(int stackPrio, P1& p1)                       { return CreateElement1<T, OVER_GRAPH>(0, stackPrio, p1); }


		template<class T, int POS_OF_GRAPH, class P1, class P2>
		T& CreateElement2(int elementWidth, int stackPrio, P1& p1, P2& p2 ) {
			T* e = new T(p1,p2);
			e->SetElementWidth(elementWidth);
			e->_parent = this;
			_createdElements << e; // to manage object destruction
			AddElement<T, POS_OF_GRAPH>(*e, stackPrio);
			return *e;
		}
		template<class T, class P1, class P2>	T& CreateLeftElement2(int elementWidth, int stackPrio, P1& p1, P2& p2)   { return CreateElement2<T, LEFT_OF_GRAPH>(elementWidth, stackPrio, p1, p2); }
		template<class T, class P1, class P2>	T& CreateRightElement2(int elementWidth, int stackPrio, P1& p1, P2& p2)  { return CreateElement2<T, RIGHT_OF_GRAPH>(elementWidth, stackPrio, p1, p2); }
		template<class T, class P1, class P2>	T& CreateTopElement2(int elementWidth, int stackPrio, P1& p1, P2& p2)    { return CreateElement2<T, TOP_OF_GRAPH>(elementWidth, stackPrio, p1, p2); }
		template<class T, class P1, class P2>	T& CreateBottomElement2(int elementWidth, int stackPrio, P1& p1, P2& p2) { return CreateElement2<T, BOTTOM_OF_GRAPH>(elementWidth, stackPrio, p1, p2); }
		template<class T, class P1, class P2>	T& CreateOverElement2(int stackPrio, P1& p1, P2& p2)                      { return CreateElement2<T, OVER_GRAPH>(0, stackPrio, p1, p2); }


		// Refresh called from child
		virtual void RefreshFromChild( RefreshStrategy doFastPaint ) {
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
			dw.Clipoff(_plotRect.left, _plotRect.top, _plotRect.GetWidth(), _plotRect.GetHeight());
			if (!_plotBckgndColor.IsNullInstance()) dw.DrawRect(0, 0, _plotRect.GetWidth(), _plotRect.GetHeight(), _plotBckgndColor);
			// --------------------------------------

			// --------------
			// GRAPH ELEMENTS on PLOT area   ( X/Y Grid, or anything else )
			// --------------

			for (int j = 0; j < _drawElements.GetCount(); j++) {
				if ( (!_drawElements[j]->IsHidden()) && _drawElements[j]->IsOverGraph() ) _drawElements[j]->PaintOnPlot(dw, _plotRect.GetWidth(), scale);
			}
			
			for (int j = 0; j < _drawElements.GetCount(); j++) {
				if (!_drawElements[j]->IsHidden()) {
					if      ( _drawElements[j]->IsVertical() )   _drawElements[j]->PaintOnPlot(dw, _plotRect.GetWidth(), scale);
					else if ( _drawElements[j]->IsHorizontal() ) _drawElements[j]->PaintOnPlot(dw, _plotRect.GetHeight(), scale);
				}
			}
			// ------------
			// paint DATA
			// ------------
			if (!_B::series.IsEmpty())
			{
				for ( int j = 0; j < _B::series.GetCount(); j++)
				{
					if ((_B::series[j].show==false) || (!_B::series[j].seriesPlot && !_B::series[j].markPlot))
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
						                                 _plotBckgndColor,
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
			// GRAPH ELEMENTS (painted in they're own area)
			// --------------
			
			for (int j = 0; j < _drawElements.GetCount(); j++)
			{
				if ( (!_drawElements[j]->IsHidden()) && (!_drawElements[j]->IsOverGraph()) ) {
					dw.Offset(_drawElements[j]->GetFrame().TopLeft());
					_drawElements[j]->PaintElement(dw, scale);
					dw.End();
				}
			}
			// --------------
			// GRAPH ELEMENTS on ALL GRAPH area   ( legend, or anything else )
			// --------------
			for (int j = 0; j < _drawElements.GetCount(); j++)
			{
				if ( (!_drawElements[j]->IsHidden()) && (_drawElements[j]->IsOverGraph()) ) {
					dw.Clipoff( _drawElements[j]->GetFrame() );
					_drawElements[j]->PaintOverGraph(dw, scale);
					dw.End();
				}
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
			typedef GridStepManager<>                              TypeGridStepManager;
			typedef SeriesConfig<GraphDrawDefaultTypes>            TypeSeriesConfig;
	};


	// ============================
	//    CRTP_EmptyGraphDraw   CLASS
	// ============================
	template<class TYPES, class DERIVED>
	class CRTP_XYGraphDraw : public CRTP_EmptyGraphDraw<TYPES, DERIVED >
	{
		public:
		typedef TYPES Types;
		typedef CRTP_XYGraphDraw<TYPES, DERIVED> CLASSNAME;
		typedef CRTP_EmptyGraphDraw<TYPES, DERIVED > _B;


		typedef typename TYPES::TypeCoordConverter            TypeCoordConverter;
		typedef typename TYPES::TypeGridAxisDraw              TypeGridAxisDraw;
		typedef SeriesGroup<TYPES,CLASSNAME>                  TypeSeriesGroup;
		typedef typename TYPES::TypeLegendElement             TypeLegendElement;

		protected:
		TypeCoordConverter   _xConverter;
		TypeCoordConverter   _yConverter;
		TypeGridAxisDraw     _xGridDraw;
		TypeGridAxisDraw     _yGridDraw;
		TypeLegendElement    _legend;
		
		public:
		CRTP_XYGraphDraw()
		: _xGridDraw(_xConverter)
		, _yGridDraw(_yConverter)
		{
			_legend.SetName(t_("Legend"));
			RGBA rgba;
			rgba.r=90; rgba.g=90; rgba.b=0;	rgba.a=90;
			_legend.SetBackGndColor( rgba );
			_legend.SetOverFrame(Rect(Point(40,20), Size(80, 30)));
			_B::AddOverLegendElement(30, _legend, 150);
			
			_B::SetTopMargin(   10);
			_B::SetBottomMargin( 0);
			_B::SetLeftMargin(   0);
			_B::SetRightMargin( 15);

			_xGridDraw.SetName( t_("X axis") );
			_xGridDraw.SetElementWidth(25);
			_xGridDraw.setAxisColor( Blue() ).setAxisTextFont(StdFont()).setAxisTextColor( Blue() ).setAxisTickColor( Red() );
			_xGridDraw.setGridColor( Gray() );
			_xGridDraw.setMajorTickMark( (new LineTickMark())->SetTickLength( 3 ) );

			_yGridDraw.SetName( t_("Y axis") );
			_yGridDraw.SetElementWidth(40);
			_yGridDraw.setAxisColor( Blue() ).setAxisTextFont(StdFont()).setAxisTextColor( Blue() ).setAxisTickColor( Red() );
			_yGridDraw.setGridColor( Gray() );
			_yGridDraw.setMajorTickMark( (new LineTickMark())->SetTickLength( 3 ) );

			
			_B::AddXConverter(_xConverter);
			_B::AddYConverter(_yConverter);
			_B::AddLeftElement(_yGridDraw, 20);
			_B::AddBottomElement(_xGridDraw, 20);
			setGraphSize(0, 100, 0, 100);
		};

		virtual ~CRTP_XYGraphDraw() {}


		DERIVED& setGraphSize(Rectf r)
		{
			_xConverter.updateGraphSize(r.TopLeft().x, r.BottomRight().x);
			_yConverter.updateGraphSize(r.TopLeft().y, r.BottomRight().y);
			_B::updateSizes();
			return *static_cast<DERIVED*>(this);
		}

		DERIVED& setGraphSize(TypeGraphCoord x0, TypeGraphCoord x1, TypeGraphCoord y0, TypeGraphCoord y1)
		{
			_xConverter.updateGraphSize( x0, x1);
			_yConverter.updateGraphSize( y0, y1);
			_B::updateSizes();
			return *static_cast<DERIVED*>(this);
		}

		TypeCoordConverter& GetXCoordConverter() { return _xConverter; }
		TypeCoordConverter& GetYCoordConverter() { return _yConverter; }

		TypeGridAxisDraw& GetXGridAxisDraw() { return _xGridDraw; }
		TypeGridAxisDraw& GetYGridAxisDraw() { return _yGridDraw; }



		//TypeLegendElement
		DERIVED& SetLegendPosition(const ElementPosition v) { _legend.SetElementPos(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetLegendFont(const Font& v)   { _legend.SetFont(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetLegendWidth(int v)          { _legend.SetElementWidth(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetLegendBckgndColor(const Color& v) { _legend.SetBackGndColor(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetLegendBckgndColor(const RGBA& v)  { _legend.SetBackGndColor(v); return *static_cast<DERIVED*>(this); }
		DERIVED& HideLegend(bool v)             { _legend.Hide(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetLegendXSize(int v)          { Rect r = _legend.GetOverFrame();r.right = r.left+v; _legend.SetOverFrame(r); return *static_cast<DERIVED*>(this); }
		DERIVED& SetLegendYSize(int v)          { Rect r = _legend.GetOverFrame();r.bottom = r.top+v; _legend.SetOverFrame(r); return *static_cast<DERIVED*>(this); }
		DERIVED& SetLegendXPos(int v)           { Rect r = _legend.GetOverFrame();r.right = v+r.Width(); r.left=v; _legend.SetOverFrame(r); return *static_cast<DERIVED*>(this); }
		DERIVED& SetLegendYPos(int v)           { Rect r = _legend.GetOverFrame();r.bottom = v+r.Height(); r.top=v; _legend.SetOverFrame(r); return *static_cast<DERIVED*>(this); }


		DERIVED& SetXAxisRectWidth(int v) { _xGridDraw.SetElementWidth(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYAxisRectWidth(int v) { _yGridDraw.SetElementWidth(v); return *static_cast<DERIVED*>(this); }


		DERIVED& setXNbGridSteps(int v) { _xGridDraw.GetGridStepManager().SetNbSteps(v); return *static_cast<DERIVED*>(this); }
		DERIVED& setYNbGridSteps(int v) { _yGridDraw.GetGridStepManager().SetNbSteps(v); return *static_cast<DERIVED*>(this); }
		
		DERIVED& SetXGridColor(Color v) { _xGridDraw.setGridColor(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYGridColor(Color v) { _yGridDraw.setGridColor(v); return *static_cast<DERIVED*>(this); }
		
		DERIVED& SetXAxisWidth(int v) { _xGridDraw.setAxisWidth(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYAxisWidth(int v) { _yGridDraw.setAxisWidth(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetXAxisColor(Color v) { _xGridDraw.setAxisColor(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYAxisColor(Color v) { _yGridDraw.setAxisColor(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetXAxisTextColor(Color v) { _xGridDraw.setAxisTextColor(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYAxisTextColor(Color v) { _yGridDraw.setAxisTextColor(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetXAxisTextFont(Font v) { _xGridDraw.setAxisTextFont(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYAxisTextFont(Font v) { _yGridDraw.setAxisTextFont(v); return *static_cast<DERIVED*>(this); }
		
		DERIVED& SetXAxisTickColor(Color v) { _xGridDraw.setAxisTickColor(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYAxisTickColor(Color v) { _yGridDraw.setAxisTickColor(v); return *static_cast<DERIVED*>(this); }
		
		
		DERIVED& SetXScaleType(int v) { _xConverter.SetScaleType(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYScaleType(int v) { _yConverter.SetScaleType(v); return *static_cast<DERIVED*>(this); }

		DERIVED& SetXAxisFormatType(AxisTextFormat v) { _xGridDraw.setAxisTextFormat(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYAxisFormatType(AxisTextFormat v) { _yGridDraw.setAxisTextFormat(v); return *static_cast<DERIVED*>(this); }


		DERIVED& SetXMin(double v) { _xConverter.SetGraphMin(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetXMax(double v) { _xConverter.SetGraphMax(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYMin(double v) { _yConverter.SetGraphMin(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYMax(double v) { _yConverter.SetGraphMax(v); return *static_cast<DERIVED*>(this); }
	};


	template<class TYPES, class DERIVED>
	class CRTP_XYLTGraphDraw : public CRTP_XYGraphDraw<TYPES, DERIVED >
	{
		public:
		typedef TYPES Types;
		typedef CRTP_XYLTGraphDraw<TYPES, DERIVED> CLASSNAME;
		typedef CRTP_XYGraphDraw<TYPES, DERIVED>   _B;
		typedef typename TYPES::TypeLabelElement   TypeLabel;
	
		TypeLabel   _title;
		TypeLabel   _xLabel;
		TypeLabel   _yLabel;

		protected:
		
		public:
		CRTP_XYLTGraphDraw()
		{
			_title.SetName( t_("Title") );
			_xLabel.SetName( t_("X label") );
			_yLabel.SetName( t_("Y label") );

			_title.SetFont( StdFontZ(20).Bold().Underline()).SetTextColor(Red).SetLabel("TITLE");
			_xLabel.SetFont( StdFontZ(15).Bold()).SetTextColor(Green).SetLabel("X Axis label");
			_yLabel.SetFont( StdFontZ(15).Bold()).SetTextColor(Green).SetLabel("Y Axis label");
			_B::AddLeftElement(30, _xLabel, 25);
			_B::AddBottomElement(30, _yLabel, 25);
			_B::AddTopElement(40, _title, 200);
		}
		
		
		DERIVED& SetTitle(const String& v)     { _title.SetLabel(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetTitlePosition(const ElementPosition v) { _title.SetElementPos(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetTitleFont(const Font& v)   { _title.SetFont(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetTitleWidth(int v)          { _title.SetElementWidth(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetTitleColor(const Color& v) { _title.SetTextColor(v); return *static_cast<DERIVED*>(this); }
		DERIVED& HideTitle(bool v)             { _title.Hide(v); return *static_cast<DERIVED*>(this); }
		
	
		DERIVED& SetXLabel(const String& v)     { _xLabel.SetLabel(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetXLabelFont(const Font& v)   { _xLabel.SetFont(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetXLabelWidth(int v)          { _xLabel.SetElementWidth(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetXLabelColor(const Color& v) { _xLabel.SetTextColor(v); return *static_cast<DERIVED*>(this); }

		DERIVED& SetYLabel(const String& v)     { _yLabel.SetLabel(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYLabelFont(const Font& v)   { _yLabel.SetFont(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYLabelWidth(int v)          { _yLabel.SetElementWidth(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYLabelColor(const Color& v) { _yLabel.SetTextColor(v); return *static_cast<DERIVED*>(this); }
	};


	template<class TYPES, class DERIVED>
	class CRTP_XYY2GraphDraw : public CRTP_XYGraphDraw<TYPES, DERIVED >
	{
		public:
		typedef TYPES Types;
		typedef CRTP_XYY2GraphDraw<TYPES, DERIVED> CLASSNAME;
		typedef CRTP_XYGraphDraw<TYPES, DERIVED>   _B;
		
		typedef typename TYPES::TypeLabelElement   TypeLabel;
		typedef typename TYPES::TypeCoordConverter TypeCoordConverter;
		typedef typename TYPES::TypeGridAxisDraw   TypeGridAxisDraw;
		typedef SeriesGroup<TYPES,CLASSNAME>       TypeSeriesGroup;
	
		TypeCoordConverter   _y2Converter;
		TypeGridAxisDraw     _y2GridDraw;
		
		public:
		CRTP_XYY2GraphDraw()
		: _y2GridDraw(_y2Converter)
		{
			_y2GridDraw.SetName( t_("Y2 axis") );
			_y2GridDraw.SetElementWidth(40);
			_y2GridDraw.setAxisColor( Blue() ).setAxisTextFont(StdFont()).setAxisTextColor( Blue() ).setAxisTickColor( Red() );
			_y2GridDraw.setGridColor( Null );
			_y2GridDraw.setMajorTickMark( (new LineTickMark())->SetTickLength( 3 ) );

			_B::AddYConverter(_y2Converter);
			_B::AddRightElement(_y2GridDraw, 50);
			setGraphSize(0, 100, 0, 100, 0, 100);
			_B::SetCurrentYConverter(0);
		};

		virtual ~CRTP_XYY2GraphDraw() {}

		private:
		DERIVED& setGraphSize(Rectf r)
		{
			return *static_cast<DERIVED*>(this);
		}
		
		public:
		DERIVED& setGraphSize(TypeGraphCoord x0, TypeGraphCoord x1, TypeGraphCoord y0, TypeGraphCoord y1, TypeGraphCoord y20, TypeGraphCoord y21 )
		{
			_B::_xConverter.updateGraphSize( x0, x1);
			_B::_yConverter.updateGraphSize( y0, y1);
			_y2Converter.updateGraphSize( y20, y21);
			_B::updateSizes();
			return *static_cast<DERIVED*>(this);
		}

		TypeCoordConverter& GetY2CoordConverter() { return _y2Converter; }
		TypeGridAxisDraw& GetY2GridAxisDraw()     { return _y2GridDraw; }


		DERIVED& SetY2AxisRectWidth(int v)   { _y2GridDraw.SetElementWidth(v); return *static_cast<DERIVED*>(this); }
		DERIVED& setY2NbGridSteps(int v)     { _y2GridDraw.GetGridStepManager().SetNbSteps(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetY2GridColor(Color v)     { _y2GridDraw.setGridColor(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetY2AxisWidth(int v)       { _y2GridDraw.setAxisWidth(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetY2AxisColor(Color v)     { _y2GridDraw.setAxisColor(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetY2AxisTextColor(Color v) { _y2GridDraw.setAxisTextColor(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetY2AxisTextFont(Font v)   { _y2GridDraw.setAxisTextFont(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetY2AxisTickColor(Color v) { _y2GridDraw.setAxisTickColor(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetY2ScaleType(int v)       { _y2Converter.SetScaleType(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetY2AxisFormatType(AxisTextFormat v) { _y2GridDraw.setAxisTextFormat(v); return *static_cast<DERIVED*>(this); }

		DERIVED& SetY2Min(double v) { _y2Converter.SetGraphMin(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetY2Max(double v) { _y2Converter.SetGraphMax(v); return *static_cast<DERIVED*>(this); }
	};


	template<class TYPES, class DERIVED>
	class CRTP_XYY2LTGraphDraw : public CRTP_XYY2GraphDraw<TYPES, DERIVED >
	{
		public:
		typedef TYPES Types;
		typedef CRTP_XYY2LTGraphDraw<TYPES, DERIVED> CLASSNAME;
		typedef CRTP_XYY2GraphDraw<TYPES, DERIVED>   _B;
		typedef typename TYPES::TypeLabelElement     TypeLabel;
	
		TypeLabel   _title;
		TypeLabel   _xLabel;
		TypeLabel   _yLabel;
		TypeLabel   _y2Label;

		protected:
		
		public:
		CRTP_XYY2LTGraphDraw()
		{
			_title.SetName( t_("Title") );
			_xLabel.SetName( t_("X label") );
			_yLabel.SetName( t_("Y label") );
			_y2Label.SetName( t_("Y2 label") );
			
			_title.SetFont( StdFontZ(20).Bold().Underline()).SetTextColor(Red).SetLabel("TITLE");
			_xLabel.SetFont( StdFontZ(15).Bold()).SetTextColor(Green).SetLabel("X Axis label");
			_yLabel.SetFont( StdFontZ(15).Bold()).SetTextColor(Green).SetLabel("Y Axis label");
			_y2Label.SetFont( StdFontZ(15).Bold()).SetTextColor(Green).SetLabel("Y2 Axis label");
			_B::AddBottomElement(30, _xLabel, 25);
			_B::AddLeftElement(30, _yLabel, 25);
			_B::AddRightElement(30, _y2Label, 55);
			_B::AddTopElement(40, _title, 200);
			_B::SetCurrentYConverter(0);
		}
		
		
		DERIVED& SetTitle(const String& v)       { _title.SetLabel(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetTitlePosition(const ElementPosition v) { _title.SetElementPos(v); _B::updateSizes(); return *static_cast<DERIVED*>(this); }
		DERIVED& SetTitleFont(const Font& v)     { _title.SetFont(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetTitleWidth(int v)            { _title.SetElementWidth(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetTitleColor(const Color& v)   { _title.SetTextColor(v); return *static_cast<DERIVED*>(this); }
		DERIVED& HideTitle(bool v)               { _title.Hide(v); return *static_cast<DERIVED*>(this); }
		
		DERIVED& SetXLabel(const String& v)      { _xLabel.SetLabel(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetXLabelFont(const Font& v)    { _xLabel.SetFont(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetXLabelWidth(int v)           { _xLabel.SetElementWidth(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetXLabelColor(const Color& v)  { _xLabel.SetTextColor(v); return *static_cast<DERIVED*>(this); }

		DERIVED& SetYLabel(const String& v)      { _yLabel.SetLabel(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYLabelFont(const Font& v)    { _yLabel.SetFont(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYLabelWidth(int v)           { _yLabel.SetElementWidth(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetYLabelColor(const Color& v)  { _yLabel.SetTextColor(v); return *static_cast<DERIVED*>(this); }

		DERIVED& SetY2Label(const String& v)     { _y2Label.SetLabel(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetY2LabelFont(const Font& v)   { _y2Label.SetFont(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetY2LabelWidth(int v)          { _y2Label.SetElementWidth(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetY2LabelColor(const Color& v) { _y2Label.SetTextColor(v); return *static_cast<DERIVED*>(this); }
	};
};



#endif
