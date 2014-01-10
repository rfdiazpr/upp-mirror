#ifndef _GraphDraw_GrapDraw_h
#define _GraphDraw_GrapDraw_h

#include <iostream>

#include <RichText/RichText.h>

#include <Core/Core.h>
#include <ScatterDraw/ScatterDraw.h>
#ifdef GD_TIMINGS
#include <TimingPolicies/TimingPolicies.h>
#endif

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



struct DebugLogBlockString
{
	DebugLogBlockString(const String name, bool cond) : name(name), printLogs(cond) { if (printLogs) { VppLog() << name << EOL << LOG_BEGIN; } }
	~DebugLogBlockString()                                                          { if (printLogs) { VppLog() << LOG_END << "//" << name << EOL; } }
	const String name;
	bool printLogs;
};
#define RLOGBLOCK_STR(COND, TXT)   //StringStream str; str << TXT; DebugLogBlockString DLBS( str.GetResult(), COND );



namespace GraphDraw_ns
{
//	Size GetSmartTextSize(const char *text, Font font = StdFont(), int cx = INT_MAX);
//	int  GetSmartTextHeight(const char *s, int cx, Font font = StdFont());
//	void DrawSmartText(Draw& w, int x, int y, int cx, const char *text, Font font = StdFont(), Color ink = DefaultInk, int accesskey = 0);
	                   
	                   
	Size GetSmartTextSize(const char *text, Font& scaledFont, int scale=1, int cx=INT_MAX);
	int GetSmartTextHeight(const char *s, int cx, Font font, int scale=1);
	void DrawSmartText(Draw& draw, int x, int y, int cx, const char *text, Font& scaledFont, Color ink, int scale);
};


#define VALIDATE_IML(IML_NAME) \
	for (int c=0; c<IML_NAME::GetCount(); ++c)	{\
		Image img = IML_NAME::Get(c);\
		for(const RGBA *s = ~img; s != img.End(); ++s) {\
			if ( !(s->r <= s->a && s->g <= s->a && s->b <= s->a )) {\
				RLOG("*s =" << *s << "    " <<  IML_NAME::GetId(c));\
				break;\
			}\
		}\
	}


#include "GraphDrawTypes.h"
#include "CoordinateConverter.h"
#include "GraphFunctions.h"
#include "SeriesConfig.h"
#include "SeriesGroup.h"
#include "GridStepIterator.h"
#include "StdGraphElements.h"
#include "GridAxisDraw.h"

#ifndef __TimingPolicies_H__
class TimingStub
{
	public:
		TimingStub(const char* prefix) {}
		inline void beginTiming(void) {};
		inline void endTiming(void)  {};
		inline void reset(void) {};
		template <class STREAM> STREAM& printStats(STREAM& str)	{ return str; }
};
#endif


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

	// ============================
	// CRTP_EmptyGraphDraw   CLASS
	// ============================
	template<class TYPES, class DERIVED>
	class CRTP_EmptyGraphDraw : public SeriesGroup< DERIVED > , public GraphElementParent
	{
		public:
		typedef CRTP_EmptyGraphDraw<TYPES, DERIVED> CLASSNAME;
		typedef SeriesGroup<DERIVED >        TypeSeriesGroup;
		typedef TypeSeriesGroup                     _B;

		protected:
		// graph elements draw around the graph
		Vector< GraphElement* >  _drawElements;
		Vector< GraphElement* >  _createdElements; // the elements in this list are owned by GrapDraw
		Vector< CoordinateConverter* > _xConverters;
		Vector< CoordinateConverter* > _yConverters;

		Rect     _ctrlRect;  // whole graph screen Rect
		Rect     _plotRect;
		DrawMode _drawMode;
		bool     _doFastPaint;
		Value    _plotBckgndStyle;
		Value    _ctrlBckgndStyle;

		int       _topMarginWidth;
		int       _bottomMarginWidth;
		int       _leftMarginWidth;
		int       _rightMarginWidth;
		
		GraphUndo _undoManager;
		Image     _PlotDrawImage;
		Image     _CtrlBackgroundImage;
		
		Rect      _selectRect;
		Value     _selectStyle;

#ifndef __TimingPolicies_H__
		typedef TimingStub TimingType;
#else
		typedef NamedTimings< TimingPolicies_ns::MinMaxAverageTiming > TimingType;		
		//typedef NamedTimings< TimingPolicies_ns::NoTiming >  TimingType;
		
#endif
		
		TimingType _paintTiming;
		TimingType _paintPlotDataTiming;
		TimingType _paintPlotDataGlobalTiming;
		TimingType _paintBackGndTiming;
		TimingType _initBackGndPaintTiming;
		TimingType _paintBackGndTiming_chPaint;
		TimingType _paintBackGndTiming_paintImage;
		TimingType _paintBackGndTiming_copyImage;
		
		
		// helper method
		void AppendElementToRect(GraphElement& element, Rect& fromRect, const int scale)
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
				case FLOAT_OVER_GRAPH:
					res = element.GetFloatFrame();
					res = Rect( res.TopLeft()*scale, res.Size()*scale );
					element.SetFrame(res);
					break;
			}
		}

		void ClearPlotDrawImg() {
			_PlotDrawImage.Clear();
		}
		
		inline void updateSizes( const int scale = 1 )
		{
			Rect svg = _plotRect;
			_plotRect = _ctrlRect;
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

			if (_plotRect != svg) {
				_CtrlBackgroundImage.Clear();
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
		bool debugTrace;

		CRTP_EmptyGraphDraw()
		: _drawMode( MD_DRAW )
		, _doFastPaint(false)
		, _topMarginWidth(0)
		, _bottomMarginWidth(0)
		, _leftMarginWidth(0)
		, _rightMarginWidth(0)
		, _paintTiming("paint()")
		, _paintPlotDataTiming("paintPlotData()")
		, _paintPlotDataGlobalTiming("paintPlotDataGlobalTiming()")
		, _paintBackGndTiming("paintBckGnd()")
		, _initBackGndPaintTiming("initBackGndPaint()")
		, _paintBackGndTiming_chPaint("paintBackGndTiming_chPaint()")
		, _paintBackGndTiming_paintImage("paintBackGndTiming_paintImage()")
		, _paintBackGndTiming_copyImage("paintBackGndTiming_copyImage()")
		, debugTrace(false)
		{
			_selectRect.Clear();
			_plotBckgndStyle = LtGray();
			_ctrlBckgndStyle = White();
			setScreenSize( Size(100,100) ); // set default size
		};


		virtual ~CRTP_EmptyGraphDraw() {
			for (int j = 0; j < _createdElements.GetCount(); j++)
			{
				delete ( _createdElements[j] );
			}
			_createdElements.Clear();
		}

		virtual TypeVectorSeries& GetSeries() {
			return _B::series;
		}

		virtual Value GetParentCtrl() {
			ASSERT_(0, "CRTP_EmptyGraphDraw::GetParentCtrl()  was CALLED");
			return Null;
		}

		virtual void AddUndoAction(GraphUndoData& CB) {
			_undoManager.AddUndoAction(CB);
		}
		
		void Undo() {
			_undoManager.Undo();
			RefreshFromChild( GraphDraw_ns::REFRESH_FULL );
		}

		void Redo() {
			_undoManager.Redo();
			RefreshFromChild( GraphDraw_ns::REFRESH_FULL );
		}
		
		void FitToData(FitToDataStrategy fitStrategy=ALL_SERIES) {
			for (int j = _drawElements.GetCount()-1; j>=0; j--) {
				_drawElements[j]->FitToData(fitStrategy);
			}
			RefreshFromChild( GraphDraw_ns::REFRESH_FULL );
		}
		

		CoordinateConverter& GetXCoordConverter() { ASSERT(TypeSeriesGroup::_currentXConverter!=0); return *TypeSeriesGroup::_currentXConverter; }
		CoordinateConverter& GetYCoordConverter() { ASSERT(TypeSeriesGroup::_currentYConverter!=0); return *TypeSeriesGroup::_currentYConverter; }

		template <class T> DERIVED& SetPlotBackgroundStyle(T c) { _plotBckgndStyle = c; _CtrlBackgroundImage.Clear(); return *static_cast<DERIVED*>(this); }
		template <class T> DERIVED& SetCtrlBackgroundStyle(T c) { _ctrlBckgndStyle = c; _CtrlBackgroundImage.Clear(); return *static_cast<DERIVED*>(this); }
		
		DERIVED& SetDrawMode(DrawMode m) { _drawMode = m; return *static_cast<DERIVED*>(this); }
		DERIVED& SetDrawMode(int m) {
			if ((MD_DRAW<=m) && (m<=MD_SUBPIXEL)) _drawMode = (DrawMode)m;
			return *static_cast<DERIVED*>(this);
		}
		virtual DrawMode GetDrawMode() { return _drawMode; }

		

		DERIVED& SetTopMargin(int v)    { _topMarginWidth = v;    return *static_cast<DERIVED*>(this); }
		DERIVED& SetBottomMargin(int v) { _bottomMarginWidth = v; return *static_cast<DERIVED*>(this); }
		DERIVED& SetLeftMargin(int v)   { _leftMarginWidth = v;   return *static_cast<DERIVED*>(this); }
		DERIVED& SetRightMargin(int v)  { _rightMarginWidth = v;  return *static_cast<DERIVED*>(this); }

		DERIVED& setScreenSize(Rect r, const int scale=1)	{
			//RLOGBLOCK("setScreenSize");
			if (r!=_ctrlRect || scale != 1) {
				_ctrlRect = r;
				ClearPlotDrawImg();
				_CtrlBackgroundImage.Clear();
				updateSizes(scale);
			}
			return *static_cast<DERIVED*>(this);
		}

		inline DERIVED& setScreenSize( const int scale=1 )	{
			//RLOGBLOCK("setScreenSize");
			return setScreenSize(_ctrlRect, scale);
		}

		virtual void AddXConverter(CoordinateConverter* conv) {
			_xConverters << conv;
			TypeSeriesGroup::_currentXConverter = conv;
		}

		virtual void AddYConverter(CoordinateConverter* conv) {
			_yConverters << conv;
			TypeSeriesGroup::_currentYConverter = conv;
		}

		template <class COORDCONV>
		COORDCONV& AddXConverter(COORDCONV& conv) {
			AddXConverter(&conv);
			return conv;
		}

		template <class COORDCONV>
		COORDCONV& AddYConverter(COORDCONV& conv) {
			AddYConverter(&conv);
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

		DERIVED&  SetSelectStyle(Value p) {
			_selectStyle = p;
			return *static_cast<DERIVED*>(this);
		}


		template <class T>
		DERIVED& LinkX(T& g) {
			_xConverters.Append( g._xConverters );
			return *static_cast<DERIVED*>(this);
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

		virtual Callback MakeRestoreGraphSizeCB() {
			Callback action;
			for (int j = 0; j < _xConverters.GetCount(); j++) { action << _xConverters[j]->MakeRestoreAxisMinMaxCB(); }
			for (int j = 0; j < _yConverters.GetCount(); j++) { action << _yConverters[j]->MakeRestoreAxisMinMaxCB(); }
			return 	action;
		}

		virtual void ZoomX(TypeScreenCoord left, TypeScreenCoord right)
		{
			for (int j = 0; j < _xConverters.GetCount(); j++)
			{
				_xConverters[j]->updateGraphSize( _xConverters[j]->toGraph( left ),
				                                  _xConverters[j]->toGraph( right ));
			}
			ClearPlotDrawImg();
			Refresh();
		}

		virtual void ZoomY(TypeScreenCoord top, TypeScreenCoord bottom)
		{
			for (int j = 0; j < _yConverters.GetCount(); j++)
			{
				_yConverters[j]->updateGraphSize( _yConverters[j]->toGraph( bottom ),
				                                  _yConverters[j]->toGraph( top ));
			}
			ClearPlotDrawImg();
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

		virtual void ScrollX( TypeScreenCoord xOffset, bool doRefresh)
		{
			for (int j = 0; j < _xConverters.GetCount(); j++)
			{
				_xConverters[j]->Scroll( xOffset );
			}
			if (doRefresh) Refresh();
		}

		virtual void ScrollY( TypeScreenCoord yOffset, bool doRefresh)
		{
			for (int j = 0; j < _yConverters.GetCount(); j++)
			{
				_yConverters[j]->Scroll( yOffset );
			}
			if (doRefresh) Refresh();
		}

		DERIVED& Scroll( TypeScreenCoord xOffset, TypeScreenCoord yOffset)
		{
			ScrollX(xOffset, false);
			ScrollY(yOffset, false);
			Refresh();
			return *static_cast<DERIVED*>(this);
		}

		template<class T, int POS_OF_GRAPH>
		T& AddElement(T& v, int stackPrio) {
			v._parent = this;
			v.SetStackingPriority(stackPrio);
			v.SetElementPos((ElementPosition)POS_OF_GRAPH);
			_drawElements.Add(&v);
			Sort(_drawElements, compareGraphElementFrame);
			return v;
		}

//		    N3Upp11RawValueRepIMN12 GraphDraw_ns::SeriesGroup::GraphCtrlDefaultTypes::Test_GraphCtrl::Vector::SeriesConfig
//		 -> Vector::GraphDraw_ns::SeriesConfig::GraphCtrlDefaultTypes


		template<class T, int POS_OF_GRAPH>
		T& AddElement(int elementWidth, T& v, int stackPrio) {
				v.SetElementWidth(elementWidth);
				return AddElement<T, POS_OF_GRAPH>(v, stackPrio);
		}

		template<class T>  T& AddLeftElement(T& v, int stackPrio)   { return AddElement<T, LEFT_OF_GRAPH>(v, stackPrio); }
		template<class T>  T& AddRightElement(T& v, int stackPrio)  { return AddElement<T, RIGHT_OF_GRAPH>(v, stackPrio); }
		template<class T>  T& AddTopElement(T& v, int stackPrio)    { return AddElement<T, TOP_OF_GRAPH>(v, stackPrio); }
		template<class T>  T& AddBottomElement(T& v, int stackPrio) { return AddElement<T, BOTTOM_OF_GRAPH>(v, stackPrio); }
		template<class T>  T& AddFloatElement(T& v, int stackPrio)   { return AddElement<T, FLOAT_OVER_GRAPH>(v, stackPrio); }

		template<class T>  T& AddLeftElement(int elementWidth, T& v, int stackPrio)   { return AddElement<T, LEFT_OF_GRAPH>(elementWidth, v, stackPrio); }
		template<class T>  T& AddRightElement(int elementWidth, T& v, int stackPrio)  { return AddElement<T, RIGHT_OF_GRAPH>(elementWidth, v, stackPrio); }
		template<class T>  T& AddTopElement(int elementWidth, T& v, int stackPrio)    { return AddElement<T, TOP_OF_GRAPH>(elementWidth, v, stackPrio); }
		template<class T>  T& AddBottomElement(int elementWidth, T& v, int stackPrio) { return AddElement<T, BOTTOM_OF_GRAPH>(elementWidth, v, stackPrio); }
		template<class T>  T& AddFloatElement(int elementWidth, T& v, int stackPrio)   { return AddElement<T, FLOAT_OVER_GRAPH>(elementWidth, v, stackPrio); }

		template<class T, int POS_OF_GRAPH>
		T& CreateElement(int elementWidth, int stackPrio) {
			T* e = new T();
			e->SetElementWidth(elementWidth);
			e->_parent = this;
			_createdElements << e; // to manage object destruction
			AddElement<T, POS_OF_GRAPH>(*e, stackPrio);
			return *e;
		}

		template<class T, int POS_OF_GRAPH, class P1>
		T& CreateElement1(int elementWidth, int stackPrio, P1& p1 ) {
			T* e = new T(p1);
			e->SetElementWidth(elementWidth);
			e->_parent = this;
			_createdElements << e; // to manage object destruction
			AddElement<T, POS_OF_GRAPH>(*e, stackPrio);
			return *e;
		}

		template<class T, int POS_OF_GRAPH, class P1, class P2>
		T& CreateElement2(int elementWidth, int stackPrio, P1& p1, P2& p2 ) {
			T* e = new T(p1,p2);
			e->SetElementWidth(elementWidth);
			e->_parent = this;
			_createdElements << e; // to manage object destruction
			AddElement<T, POS_OF_GRAPH>(*e, stackPrio);
			return *e;
		}

		// Refresh called from child
		virtual void RefreshFromChild( RefreshStrategy doFastPaint ) {
			Refresh();
		};

		virtual void Refresh() {};


		Image GetImage( Size size, Color backGndColor = Upp::White(), const int scale = 1 ) {
			RLOGBLOCK_STR( debugTrace, "CRTP_EmptyGraphDraw::GetImage(" << this << ")   [ FastPaint , PlotImgEmpty ] => [ " << _doFastPaint << " , " << _PlotDrawImage.IsEmpty() << " ]");
			Rect screenRectSvg = _ctrlRect;
			Image plotDrawImageSvg = _PlotDrawImage;
			Image ctrlBackgroundImageSvg = _CtrlBackgroundImage;
			setScreenSize( size, scale );
			ImageBuffer ib(size);
			Upp::Fill( ib.Begin(),backGndColor, ib.GetLength() );
			BufferPainter bp(ib, MD_ANTIALIASED); 
			ClearPlotDrawImg();
			Paint(bp, scale);
			ClearPlotDrawImg();
			setScreenSize( screenRectSvg );
			_PlotDrawImage = plotDrawImageSvg;
			_CtrlBackgroundImage = ctrlBackgroundImageSvg;
			return ib;
		}

		Image GetImage(Size size, const int scale = 1 ) {
			return GetImage(_ctrlRect.Size()*scale, White(), scale );
		}

		inline Image GetImage(Size size, Color backGndColor ) {
			return GetImage(size, 1, backGndColor);
		}

		inline Image GetImage(const int scale=1) {
			return GetImage( _ctrlRect.Size()*scale, scale );
		}

		inline Image GetImage(Color backGndColor, const int scale=1) {
			return GetImage( backGndColor, scale );
		}

		template <class V, class XC, class YC>
		inline void addToFullPointsList(V& p1, TypeGraphCoord x, TypeGraphCoord y , XC& xConverter, YC& yConverter, int64& nbVisiblePoints, Point& prevPoint, bool& prevPointIsVisible) {
			if (   ( xConverter.IsInGraphVisibleRange( x ) )
			    && ( yConverter.IsInGraphVisibleRange( y ) ) )
			{
				++nbVisiblePoints;
				if (!prevPoint.IsNullInstance()) p1 << prevPoint;
				p1 << Point(xConverter.toScreen( x ),
				            yConverter.toScreen( y ));
				prevPoint = Null;
				prevPointIsVisible = true;
			}
			else {
				prevPoint = Point(xConverter.toScreen( x ),
				                  yConverter.toScreen( y ));
				if (prevPointIsVisible) {
					p1 << prevPoint;
					prevPoint = Null;
				}
				prevPointIsVisible = false;
			}
		}


		#define addToFullPointsListM(p1, x, y , xConverter, yConverter, nbVisiblePoints, prevPoint, prevPointIsVisible, isSeriesFilled) {\
				if ( xConverter.IsInGraphVisibleRange( x ) && ( isSeriesFilled || yConverter.IsInGraphVisibleRange( y )) )\
				{\
					++nbVisiblePoints;\
					if (!prevPoint.IsNullInstance()) p1 << prevPoint;\
					p1 << Point(xConverter.toScreen( x ),\
					            yConverter.toScreen( y ));\
					prevPoint = Null;\
					prevPointIsVisible = true;\
				}\
				else {\
					prevPoint = Point(xConverter.toScreen( x ),\
					                  yConverter.toScreen( y ));\
					if (prevPointIsVisible) {\
						p1 << prevPoint;\
						prevPoint = Null;\
					}\
					prevPointIsVisible = false;\
				}\
		}\


		template<class T>
		void PaintPlotData(T& dw, int scale)
		{
			_paintPlotDataTiming.beginTiming();
			if (!_B::series.IsEmpty())
			{
				for ( int j = 0; j < _B::series.GetCount(); j++)
				{
					if ((_B::series[j].show==false) || (!_B::series[j].seriesPlot && !_B::series[j].markPlot))
						continue;

					Vector<Point> p1;

					if ((_B::series[j].nbVisiblePoints==0)  &&  ( ! _B::series[j].PointsData()->IsExplicit())) {
							_B::series[j].nbVisiblePoints = _B::series[j].PointsData()->GetCount();
					}

					// ============================================
					//     CREATE  LIST  OF  POINTS  TO  DRAW
					// ============================================
					unsigned int inc = 1;
					int64 nbVisiblePoints = 0;
					int imin, imax;

					CoordinateConverter& xConverter = *(_B::series[j].xConverter);
					CoordinateConverter& yConverter = *(_B::series[j].yConverter);
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
					if (_B::series[j].PointsData()->IsParam()) {             // It is a param function
						imin = 0;
						imax = _B::series[j].PointsData()->GetCount();
					} else if ( _B::series[j].PointsData()->IsExplicit() ) { // It is a function
						imin = ffloor(xConverter.getGraphMin() - 1);
						imax = ffloor(xConverter.getGraphMax() + 2);
					} else {
					    imin = 0;
					    imax = _B::series[j].PointsData()->GetCount();
					}
					if ( !_doFastPaint )  // DRAW ALL POINTS
					{
						Point prevPoint = Null;
						bool prevPointIsVisible = false;
						nbVisiblePoints = 0;
						TypeGraphCoord x;
						TypeGraphCoord y;
						bool isSeriesFilled = false;
						if ( !_B::series[j].seriesPlot.IsEmpty() && !_B::series[j].fillColor.IsNullInstance() ) {
							isSeriesFilled = true;
						}
						    
						if (_B::series[j].PointsData()->IsParam()) {
							const double xmax = imax;
							for (double cx=imin; cx<xmax; ++cx)
							{
								x = _B::series[j].PointsData()->x(cx);
								y = _B::series[j].PointsData()->y(cx);
								addToFullPointsListM( p1, x, y, xConverter, yConverter, nbVisiblePoints, prevPoint, prevPointIsVisible, isSeriesFilled);
							}
						} else if (_B::series[j].PointsData()->IsExplicit() ) {
							double xmax = imax;
							double dx = double(xmax - imin)/xConverter.getScreenRange();
							for (double xx = imin; xx < xmax; xx += dx) {
								double yy = _B::series[j].PointsData()->f(xx);
								addToFullPointsListM( p1, xx, yy, xConverter, yConverter, nbVisiblePoints, prevPoint, prevPointIsVisible, isSeriesFilled);
							}

						} else {
							for (Upp::int64 c=imin; c<imax; c+=inc)
							{
								x = _B::series[j].PointsData()->x(c);
								y = _B::series[j].PointsData()->y(c);
								addToFullPointsListM( p1, x, y, xConverter, yConverter, nbVisiblePoints, prevPoint, prevPointIsVisible, isSeriesFilled);
							}
						}
						_B::series[j].nbVisiblePoints = nbVisiblePoints;
					}
					else  // DO FAST DRAW
					{
						nbVisiblePoints = 0;
						if (_B::series[j].PointsData()->IsExplicit() ) {
							double xmax = imax;
							double dx = double(xmax - imin)/800.;
							if (xConverter.getScreenRange()<800.) dx = double(xmax - imin)/xConverter.getScreenRange();

							for (double xx = imin; xx < xmax; xx += dx) {
								double yy = _B::series[j].PointsData()->f(xx);
								p1 << Point(xConverter.toScreen( xx ),
									        yConverter.toScreen( yy ));
								//addToFullPointsListM(p1, xx, yy, xConverter, yConverter, nbVisiblePoints, prevPoint, prevPointIsVisible);
							}
						} else {
							inc = tabs(imax-imin)/800 + 1;
							for ( Upp::int64 c=imin; c<imax; c+=inc)
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
						                                 Null, //_plotBckgndStyle,
						                                 _B::series[j].fillColor,
						                                 xConverter.getScreenRange() / xConverter.getGraphRange(),
						                                 yConverter.getScreenRange() / yConverter.getGraphRange(),
						                                 int(yConverter.getScreenRange() * (1 + yConverter.getGraphMin() / yConverter.getGraphRange()))
						);

					// Draw marks
					if (_B::series[j].markWidth >= 1 && _B::series[j].markPlot)
					{
						if ( !_B::series[j].markPlot.IsEmpty() ) {
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
			}
			_paintPlotDataTiming.endTiming();
		}

		template<class T>
		void Paint(T& dw, int scale)
		{
			RLOGBLOCK_STR( debugTrace, "CRTP_EmptyGraphDraw::Paint(" << this << ")   [ FastPaint , PlotImgEmpty ] => [ " << _doFastPaint << " , " << _PlotDrawImage.IsEmpty() << " ]");
			_paintTiming.beginTiming();
			_paintBackGndTiming.beginTiming();

			//RLOGBLOCK("==================================" );
			if ( _CtrlBackgroundImage.IsEmpty() )
			{
				_initBackGndPaintTiming.beginTiming();
				RGBA bckgColor;   bckgColor.r = 0; bckgColor.g = 0; bckgColor.b = 0; bckgColor.a = 0;
				ImageBuffer ib(_ctrlRect.Size());
				Upp::Fill( ib.Begin(), bckgColor, ib.GetLength() );
				BufferPainter bp(ib, _drawMode);
				_initBackGndPaintTiming.endTiming();
				// ------------
				// paint background
				// ------------
				_paintBackGndTiming_chPaint.beginTiming();
				if ( !_ctrlBckgndStyle.IsNull() ) ChPaint(bp, _ctrlRect, _ctrlBckgndStyle );
				if ( !_plotBckgndStyle.IsNull()) ChPaint(bp, _plotRect, _plotBckgndStyle );
				_paintBackGndTiming_chPaint.endTiming();
				_paintBackGndTiming_copyImage.beginTiming();
				_CtrlBackgroundImage = ib;
				_paintBackGndTiming_copyImage.endTiming();
			}
			else
			{
				_initBackGndPaintTiming.reset();
				_paintBackGndTiming_chPaint.reset();
				_paintBackGndTiming_copyImage.reset();
			}
			_paintBackGndTiming_paintImage.beginTiming();
			dw.DrawImage(0, 0, _CtrlBackgroundImage );
			_paintBackGndTiming_paintImage.endTiming();
			
			_paintBackGndTiming.endTiming();

			// --------------------------------------
			// BEGIN paint in PLOT AREA
			// --------------------------------------
			dw.Clipoff(_plotRect.left, _plotRect.top, _plotRect.GetWidth(), _plotRect.GetHeight());

			// --------------
			// GRAPH ELEMENTS on PLOT area --UNDER DATA-- ( X/Y Grid, or anything else )
			// --------------
			for (int j = 0; j < _drawElements.GetCount(); j++) {
				if ( (!_drawElements[j]->IsHidden()) && _drawElements[j]->IsFloat() ) _drawElements[j]->PaintOnPlot_underData(dw, _plotRect.GetWidth(), scale);
			}
			
			for (int j = 0; j < _drawElements.GetCount(); j++) {
				if (!_drawElements[j]->IsHidden()) {
					if      ( _drawElements[j]->IsVertical() )   _drawElements[j]->PaintOnPlot_underData(dw, _plotRect.GetWidth(), scale);
					else if ( _drawElements[j]->IsHorizontal() ) _drawElements[j]->PaintOnPlot_underData(dw, _plotRect.GetHeight(), scale);
				}
			}
			
			// ----------------
			// paint PLOT DATA
			// ----------------
			_paintPlotDataGlobalTiming.beginTiming();

			if (_doFastPaint)
			{
				RGBA bckgColor;   bckgColor.r = 0; bckgColor.g = 0; bckgColor.b = 0; bckgColor.a = 0;
				ImageBuffer ib(_plotRect.Size());
				Upp::Fill( ib.Begin(), bckgColor, ib.GetLength() );
				BufferPainter bp(ib, _drawMode);
				PaintPlotData(bp, scale);
				dw.DrawImage(0, 0, ib);
				ClearPlotDrawImg();
			}
			else
			{
				if ( _PlotDrawImage.IsEmpty() )
				{
					RGBA bckgColor;   bckgColor.r = 0; bckgColor.g = 0; bckgColor.b = 0; bckgColor.a = 0;
					ImageBuffer ib(_plotRect.Size());
					Upp::Fill( ib.Begin(), bckgColor, ib.GetLength() );
					BufferPainter bp(ib, _drawMode);
					PaintPlotData(bp, scale);
					_PlotDrawImage = ib;
				}
				dw.DrawImage(0, 0, _PlotDrawImage);
			}
			_paintPlotDataGlobalTiming.endTiming();

			// --------------
			// GRAPH ELEMENTS on PLOT area --OVER DATA-- ( X/Y Grid, or anything else )
			// --------------
			for (int j = 0; j < _drawElements.GetCount(); j++) {
				if ( (!_drawElements[j]->IsHidden()) && _drawElements[j]->IsFloat() ) _drawElements[j]->PaintOnPlot_overData(dw, _plotRect.GetWidth(), scale);
			}
			
			for (int j = 0; j < _drawElements.GetCount(); j++) {
				if (!_drawElements[j]->IsHidden()) {
					if      ( _drawElements[j]->IsVertical() )   _drawElements[j]->PaintOnPlot_overData(dw, _plotRect.GetWidth(), scale);
					else if ( _drawElements[j]->IsHorizontal() ) _drawElements[j]->PaintOnPlot_overData(dw, _plotRect.GetHeight(), scale);
				}
			}
			
			// --------------------------------------
			// Paint SELECT Rect
			// --------------------------------------
			if ( !_selectRect.IsEmpty() && !_selectStyle.IsNull()) {
				ChPaint(dw, _selectRect, _selectStyle);
			}
			// --------------------------------------
			// END of paint in PLOT AREA
			// --------------------------------------
			dw.End();

			// --------------
			// GRAPH ELEMENTS ( painted in they're own area : the ones around the PLOT area )
			// --------------
			
			for (int j = 0; j < _drawElements.GetCount(); j++)
			{
				if ( (!_drawElements[j]->IsHidden()) && (!_drawElements[j]->IsFloat()) ) {
					dw.Offset(_drawElements[j]->GetFrame().TopLeft());
					_drawElements[j]->PaintElement(dw, scale);
					dw.End();
				}
			}
			// --------------
			// GRAPH ELEMENTS on ALL GRAPH area == FLOATING ELEMENTS  ( legend, or anything else )
			// --------------
			for (int j = 0; j < _drawElements.GetCount(); j++)
			{
				if ( (!_drawElements[j]->IsHidden()) && (_drawElements[j]->IsFloat()) ) {
					dw.Clipoff( _drawElements[j]->GetFloatFrame(scale) );
					_drawElements[j]->PaintFloatElement(dw, scale);
					dw.End();
				}
			}
			_paintTiming.endTiming();
			
			//std::cout << "\n===============================";
			if (debugTrace) {
				//_paintTiming.printStats( std::cout );
				_paintTiming.printStats( VppLog() );
	
				_paintBackGndTiming.printStats( std::cout );
				_initBackGndPaintTiming.printStats( std::cout );
				_paintBackGndTiming_chPaint.printStats( std::cout );
				_paintBackGndTiming_copyImage.printStats( std::cout );
				_paintBackGndTiming_paintImage.printStats( std::cout );
	
				_paintPlotDataTiming.printStats( std::cout );
				_paintPlotDataGlobalTiming.printStats( std::cout );
			}
		}
	};


	struct GraphDrawDefaultTypes {
			typedef GraphDraw_ns::GenericCoordinateConverter          X_TypeCoordConverter;
			typedef GraphDraw_ns::GenericCoordinateConverter          X2_TypeCoordConverter;
			typedef GraphDraw_ns::GenericCoordinateConverter          Y_TypeCoordConverter;
			typedef GraphDraw_ns::GenericCoordinateConverter          Y2_TypeCoordConverter;

			typedef GraphDraw_ns::GridAxisDraw<GraphDrawDefaultTypes> X_TypeGridAxisDraw;
			typedef GraphDraw_ns::GridAxisDraw<GraphDrawDefaultTypes> X2_TypeGridAxisDraw;
			typedef GraphDraw_ns::GridAxisDraw<GraphDrawDefaultTypes> Y_TypeGridAxisDraw;
			typedef GraphDraw_ns::GridAxisDraw<GraphDrawDefaultTypes> Y2_TypeGridAxisDraw;

			typedef GridStepManager<>                                 TypeGridStepManager;
			typedef LabelElement                                      TypeLabelElement;
			typedef LegendElement<GraphDrawDefaultTypes>              TypeLegendElement;
	};


//#define COMBINE3__(a, b, c)        a##b##c
//#define COMBINE3(a, b, c)          COMBINE3__(a, b, c)
#define MAKE_GRAPHDRAW_AXIS_FN( XYZ, xyz) \
	DERIVED& COMBINE3(Hide, XYZ, Axis) (bool v)                   { COMBINE3(_, xyz, GridDraw).Hide(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Set, XYZ, AxisRectWidth)(int v)             { COMBINE3(_, xyz, GridDraw).SetElementWidth(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(set, XYZ, NbGridSteps)(int v)               { COMBINE3(_, xyz, GridDraw).GetGridStepManager().SetNbSteps(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Set, XYZ, GridColor)(Color v)               { COMBINE3(_, xyz, GridDraw).setGridColor(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Set, XYZ, AxisWidth)(int v)                 { COMBINE3(_, xyz, GridDraw).setAxisWidth(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Set, XYZ, AxisColor)(Color v)               { COMBINE3(_, xyz, GridDraw).setAxisColor(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Set, XYZ, AxisTextColor)(Color v)           { COMBINE3(_, xyz, GridDraw).setAxisTextColor(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Set, XYZ, AxisTextFont)(Font v)             { COMBINE3(_, xyz, GridDraw).setAxisTextFont(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Set, XYZ, AxisTickColor)(Color v)           { COMBINE3(_, xyz, GridDraw).setAxisTickColor(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Set, XYZ, ScaleType)(int v)                 { COMBINE3(_, xyz, Converter).SetScaleType(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Set, XYZ, AxisFormatType)(AxisTextFormat v) { COMBINE3(_, xyz, GridDraw).setAxisTextFormat(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Set, XYZ, Min)(double v)                    { COMBINE3(_, xyz, Converter).SetGraphMin(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Set, XYZ, Max)(double v)                    { COMBINE3(_, xyz, Converter).SetGraphMax(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Set, XYZ, MinRangeLimit)(double v)          { COMBINE3(_, xyz, Converter).setGraphMinRangeLimit(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Set, XYZ, MaxRangeLimit)(double v)          { COMBINE3(_, xyz, Converter).setGraphMaxRangeLimit(v); return *static_cast<DERIVED*>(this); }\
	TypeGraphCoord COMBINE3(Get, XYZ, Min)()                      { return ( COMBINE3(_, xyz, Converter).getGraphMin()); }\
	TypeGraphCoord COMBINE3(Get, XYZ, Max)()                      { return ( COMBINE3(_, xyz, Converter).getGraphMax()); }\
	Value COMBINE3(Get, XYZ, MinRangeLimit)()                     { return (COMBINE3(_, xyz, Converter).getGraphMinRangeLimit()); }\
	Value COMBINE3(Get, XYZ, MaxRangeLimit)()                     { return (COMBINE3(_, xyz, Converter).getGraphMaxRangeLimit()); }


#define MAKE_GRAPHDRAW_LABEL_FN( XYZ, xyz) \
	DERIVED& COMBINE3(Set, XYZ, Label)(const String& v)     { COMBINE3(_, xyz, Label).SetLabel(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Set, XYZ, LabelFont)(const Font& v)   { COMBINE3(_, xyz, Label).SetFont(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Set, XYZ, LabelWidth)(int v)          { COMBINE3(_, xyz, Label).SetElementWidth(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Set, XYZ, LabelColor)(const Color& v) { COMBINE3(_, xyz, Label).SetTextColor(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& COMBINE3(Hide, XYZ, Label)(bool v=true)        { COMBINE3(_, xyz, Label).Hide(v); return *static_cast<DERIVED*>(this); }

#define MAKE_GRAPHDRAW_TITLE_FN \
	DERIVED& SetTitle(const String& v)     { _title.SetLabel(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& SetTitlePosition(const ElementPosition v) { _title.SetElementPos(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& SetTitleFont(const Font& v)   { _title.SetFont(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& SetTitleWidth(int v)          { _title.SetElementWidth(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& SetTitleColor(const Color& v) { _title.SetTextColor(v); return *static_cast<DERIVED*>(this); }\
	DERIVED& HideTitle(bool v)             { _title.Hide(v); return *static_cast<DERIVED*>(this); }



	// ============================
	//    CRTP_XYGraphDraw   CLASS
	// ============================
	template<class TYPES, class DERIVED>
	class CRTP_XYGraphDraw : public CRTP_EmptyGraphDraw<TYPES, DERIVED >
	{
		public:
		typedef TYPES Types;
		typedef CRTP_XYGraphDraw<TYPES, DERIVED> CLASSNAME;
		typedef CRTP_EmptyGraphDraw<TYPES, DERIVED > _B;


		typedef typename TYPES::X_TypeCoordConverter            TypeCoordConverterX;
		typedef typename TYPES::Y_TypeCoordConverter            TypeCoordConverterY;
		typedef typename TYPES::X_TypeGridAxisDraw              TypeGridAxisDrawX;
		typedef typename TYPES::Y_TypeGridAxisDraw              TypeGridAxisDrawY;
		typedef SeriesGroup< DERIVED >                        TypeSeriesGroup;
		typedef typename TYPES::TypeLegendElement             TypeLegendElement;

		protected:
		TypeCoordConverterX   _xConverter;
		TypeCoordConverterY   _yConverter;
		TypeGridAxisDrawX     _xGridDraw;
		TypeGridAxisDrawY     _yGridDraw;
		
		public:
		CRTP_XYGraphDraw()
		: _xGridDraw(_xConverter)
		, _yGridDraw(_yConverter)
		{
			_B::AddXConverter(_xConverter);
			_B::AddYConverter(_yConverter);

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

		TypeCoordConverterX& GetXCoordConverter() { return _xConverter; }
		TypeCoordConverterY& GetYCoordConverter() { return _yConverter; }

		TypeGridAxisDrawX& GetXGridAxisDraw() { return _xGridDraw; }
		TypeGridAxisDrawY& GetYGridAxisDraw() { return _yGridDraw; }


		MAKE_GRAPHDRAW_AXIS_FN(X,x);
		MAKE_GRAPHDRAW_AXIS_FN(Y,y);
	};

	// ============================
	//    CRTP_XYLGraphDraw   CLASS
	// ============================
	template<class TYPES, class DERIVED>
	class CRTP_XYLGraphDraw : public CRTP_XYGraphDraw<TYPES, DERIVED >
	{
		public:
		typedef TYPES Types;
		typedef CRTP_XYLGraphDraw<TYPES, DERIVED> CLASSNAME;
		typedef CRTP_XYGraphDraw<TYPES, DERIVED > _B;


		typedef typename TYPES::TypeLegendElement             TypeLegendElement;

		protected:
		TypeLegendElement    _legend;
		
		public:
		CRTP_XYLGraphDraw()
		{
			_legend.SetName(t_("Legend"));
			RGBA rgba;
			rgba.r=90; rgba.g=90; rgba.b=0;	rgba.a=90;
			_legend.SetBackGroundStyle( Color(rgba) );
			_legend.SetFloatFrame(Rect(Point(40,20), Size(80, 30)));
			_B::AddFloatElement(30, _legend, 150);
		};

		virtual ~CRTP_XYLGraphDraw() {}

		DERIVED& HideLegend(bool v)             { _legend.Hide(v); return *static_cast<DERIVED*>(this); }

		//TypeLegendElement
		DERIVED& SetLegendPosition(const ElementPosition v) { _legend.SetElementPos(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetLegendFont(const Font& v)   { _legend.SetFont(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetLegendWidth(int v)          { _legend.SetElementWidth(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetLegendBackGroundStyle(const Value& v) { _legend.SetBackGroundStyle(v); return *static_cast<DERIVED*>(this); }
		DERIVED& SetLegendXSize(int v)          { Rect r = _legend.GetFloatFrame();r.right = r.left+v; _legend.SetFloatFrame(r); return *static_cast<DERIVED*>(this); }
		DERIVED& SetLegendYSize(int v)          { Rect r = _legend.GetFloatFrame();r.bottom = r.top+v; _legend.SetFloatFrame(r); return *static_cast<DERIVED*>(this); }
		DERIVED& SetLegendXPos(int v)           { Rect r = _legend.GetFloatFrame();r.right = v+r.Width(); r.left=v; _legend.SetFloatFrame(r); return *static_cast<DERIVED*>(this); }
		DERIVED& SetLegendYPos(int v)           { Rect r = _legend.GetFloatFrame();r.bottom = v+r.Height(); r.top=v; _legend.SetFloatFrame(r); return *static_cast<DERIVED*>(this); }
		TypeLegendElement& GetLegendElement()   { return _legend; }
	};

	// ============================
	//    CRTP_XYLTGraphDraw   CLASS
	// ============================
	template<class TYPES, class DERIVED>
	class CRTP_XYLTGraphDraw : public CRTP_XYLGraphDraw<TYPES, DERIVED >
	{
		public:
		typedef TYPES Types;
		typedef CRTP_XYLTGraphDraw<TYPES, DERIVED> CLASSNAME;
		typedef CRTP_XYLGraphDraw<TYPES, DERIVED>   _B;
		typedef typename TYPES::TypeLabelElement   TypeLabel;
	
		TypeLabel   _title;
		TypeLabel   _xLabel;
		TypeLabel   _yLabel;

		protected:
		
		public:
		CRTP_XYLTGraphDraw()
		{
			_title.SetName( t_("Title") );
			_xLabel.SetName( t_("X--- label") );
			_yLabel.SetName( t_("Y--- label") );

			_title.SetFont( StdFont(20).Bold().Underline()).SetTextColor(Red).SetLabel("TITLE");
			_xLabel.SetFont( StdFont(15).Bold()).SetTextColor(Green).SetLabel("X Axis label");
			_yLabel.SetFont( StdFont(15).Bold()).SetTextColor(Green).SetLabel("Y Axis label");
			_B::AddBottomElement(30, _xLabel, 25);
			_B::AddLeftElement(30, _yLabel, 25);
			_B::AddTopElement(40, _title, 200);
		}
		
		MAKE_GRAPHDRAW_TITLE_FN;
	
		MAKE_GRAPHDRAW_LABEL_FN(X,x);
		MAKE_GRAPHDRAW_LABEL_FN(Y,y);
	};


	// ============================
	//    CRTP_XYY2GraphDraw   CLASS
	// ============================
	template<class TYPES, class DERIVED>
	class CRTP_XYY2GraphDraw : public CRTP_XYLGraphDraw<TYPES, DERIVED >
	{
		public:
		typedef TYPES Types;
		typedef CRTP_XYY2GraphDraw<TYPES, DERIVED> CLASSNAME;
		typedef CRTP_XYLGraphDraw<TYPES, DERIVED>   _B;
		
		typedef typename TYPES::TypeLabelElement   TypeLabel;
		typedef typename TYPES::Y2_TypeCoordConverter TypeCoordConverterY2;
		typedef typename TYPES::Y2_TypeGridAxisDraw   TypeGridAxisDrawY2;
		typedef SeriesGroup<DERIVED>       TypeSeriesGroup;
	
		TypeCoordConverterY2   _y2Converter;
		TypeGridAxisDrawY2     _y2GridDraw;
		
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
		// forbid usage of following methods in this case
		DERIVED& setGraphSize(Rectf r) {}
		DERIVED& setGraphSize(TypeGraphCoord x0, TypeGraphCoord x1, TypeGraphCoord y0, TypeGraphCoord y1) {}
		
		public:
		DERIVED& setGraphSize(TypeGraphCoord x0, TypeGraphCoord x1, TypeGraphCoord y0, TypeGraphCoord y1, TypeGraphCoord y20, TypeGraphCoord y21 )
		{
			_B::_xConverter.updateGraphSize( x0, x1);
			_B::_yConverter.updateGraphSize( y0, y1);
			_y2Converter.updateGraphSize( y20, y21);
			_B::updateSizes();
			return *static_cast<DERIVED*>(this);
		}

		TypeCoordConverterY2& GetY2CoordConverter() { return _y2Converter; }
		TypeGridAxisDrawY2& GetY2GridAxisDraw()     { return _y2GridDraw; }
		
		MAKE_GRAPHDRAW_AXIS_FN(Y2,y2);
	};


	// ============================
	//    CRTP_XYY2LTGraphDraw   CLASS
	// ============================
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
			
			_title.SetFont( StdFont(20).Bold().Underline()).SetTextColor(Red).SetLabel("TITLE");
			_xLabel.SetFont( StdFont(15).Bold()).SetTextColor(Green).SetLabel("X Axis label");
			_yLabel.SetFont( StdFont(15).Bold()).SetTextColor(Green).SetLabel("Y Axis label");
			_y2Label.SetFont( StdFont(15).Bold()).SetTextColor(Green).SetLabel("Y2 Axis label");
			_B::AddBottomElement(30, _xLabel, 25);
			_B::AddLeftElement(30, _yLabel, 25);
			_B::AddRightElement(30, _y2Label, 55);
			_B::AddTopElement(40, _title, 200);
			_B::SetCurrentYConverter(0);
		}
		MAKE_GRAPHDRAW_TITLE_FN;
		MAKE_GRAPHDRAW_LABEL_FN(X,x);
		MAKE_GRAPHDRAW_LABEL_FN(Y,y);
		MAKE_GRAPHDRAW_LABEL_FN(Y2,y2);
	};

};



#endif
