#ifndef _GraphCtrl_GrapCtrl_h
#define _GraphCtrl_GrapCtrl_h

#include <GraphDraw/GraphDraw.h>

using namespace Upp;



#define IMAGECLASS GraphCtrlImg
#define IMAGEFILE <GraphCtrl/GraphCtrl.iml>
#include <Draw/iml_header.h>


// ============================
//    CRTP_GraphCtrlBase   CLASS
// ============================
template<class TYPES, class DERIVED>
class CRTP_GraphCtrlBase : public GraphDraw_ns::CRTP_StdGraphDraw<TYPES, DERIVED>, public Ctrl
{
	public:
	typedef CRTP_GraphCtrlBase<TYPES, DERIVED> CLASSNAME;

	private:
	typedef GraphDraw_ns::CRTP_StdGraphDraw<TYPES, DERIVED> _GD;

	GraphDraw_ns::GraphElementFrame* elementCapture_LeftDown   ;
	GraphDraw_ns::GraphElementFrame* elementCapture_LeftDouble ;
	GraphDraw_ns::GraphElementFrame* elementCapture_LeftDrag   ;
	GraphDraw_ns::GraphElementFrame* elementCapture_RightDown  ;
	GraphDraw_ns::GraphElementFrame* elementCapture_RightDouble;
	GraphDraw_ns::GraphElementFrame* elementCapture_RightDrag  ;
	GraphDraw_ns::GraphElementFrame* elementCapture_MiddleDown ;
	GraphDraw_ns::GraphElementFrame* elementCapture_MouseMove  ;
	GraphDraw_ns::GraphElementFrame* elementCapture_MouseWheel ;


	bool plotCapture_LeftDown   ;
	bool plotCapture_LeftDouble ;
	bool plotCapture_LeftDrag   ;
	bool plotCapture_RightDown  ;
	bool plotCapture_RightDouble;
	bool plotCapture_RightDrag  ;
	bool plotCapture_MiddleDown ;
	bool plotCapture_MouseMove  ;
	bool plotCapture_MouseWheel ;

	Image CaptureMouseMove_cursorImage;

	Point prevMousePoint;

	public:



	CRTP_GraphCtrlBase()
	: elementCapture_LeftDown   (0)
	, elementCapture_LeftDouble (0)
	, elementCapture_LeftDrag   (0)
	, elementCapture_RightDown  (0)
	, elementCapture_RightDouble(0)
	, elementCapture_RightDrag  (0)
	, elementCapture_MiddleDown (0)
	, elementCapture_MouseMove  (0)
	, elementCapture_MouseWheel (0)
	, plotCapture_LeftDown   (false)
	, plotCapture_LeftDouble (false)
	, plotCapture_LeftDrag   (false)
	, plotCapture_RightDown  (false)
	, plotCapture_RightDouble(false)
	, plotCapture_RightDrag  (false)
	, plotCapture_MiddleDown (false)
	, plotCapture_MouseMove  (false)
	, plotCapture_MouseWheel (false)
	{
		setScreenSize( GetSize() );
	}

	CRTP_GraphCtrlBase(const CRTP_GraphCtrlBase& p)
	: _GD(p)
	, elementCapture_LeftDown   (0)
	, elementCapture_LeftDouble (0)
	, elementCapture_LeftDrag   (0)
	, elementCapture_RightDown  (0)
	, elementCapture_RightDouble(0)
	, elementCapture_RightDrag  (0)
	, elementCapture_MiddleDown (0)
	, elementCapture_MouseMove  (0)
	, elementCapture_MouseWheel (0)
	, plotCapture_LeftDown   (false)
	, plotCapture_LeftDouble (false)
	, plotCapture_LeftDrag   (false)
	, plotCapture_RightDown  (false)
	, plotCapture_RightDouble(false)
	, plotCapture_RightDrag  (false)
	, plotCapture_MiddleDown (false)
	, plotCapture_MouseMove  (false)
	, plotCapture_MouseWheel (false)
	{
		setScreenSize( GetSize() );
	}

	virtual void Refresh() {
		_GD::updateSizes();
		Ctrl::Refresh();
	};


	void Paint2(Draw& w) {
		setScreenSize( GetSize() );
		if (_GD::_mode == GraphDraw_ns::MD_DRAW) {
			_GD::Paint(w, 1);
		}
		else {
			ImageBuffer ib(GetSize());
			BufferPainter bp(ib, _GD::_mode);
			_GD::Paint(bp, 1);
			w.DrawImage(0, 0, ib);
		}
	}


	virtual void Paint(Draw& w) {
		if ( _GD::_doFastPaint ==false )	{
			Image m = OverrideCursor(GraphCtrlImg::SAND());
			Paint2(w);
			OverrideCursor(m);
		}
		else {
			Paint2(w);
		}
	}



	typedef GraphDraw_ns::GraphElementFrame* (GraphDraw_ns::GraphElementFrame::*mouseCallBack)(Point,dword);

	bool IsContainedByOverElement(Point p) {
		for (int j = 0; j < _GD::_overElements.GetCount(); j++) {
			if (_GD::_overElements[j]->Contains(p)) return true;
		}
		return false;
	}

	template<class RESULT, class CBCK>
	RESULT ProcessMouseCallBack(Point p, dword keyflags, CBCK cbck, RESULT defaultRes = 0)
	{
		for (int j = 0; j < _GD::_overElements.GetCount(); j++)
		{
			if (_GD::_overElements[j]->Contains(p)) {
				return ((_GD::_overElements[j]->*cbck)(p, keyflags));
			}
		}
		for (int j = 0; j < _GD::_leftElements.GetCount(); j++)
		{
			if (_GD::_leftElements[j]->Contains(p)) {
				return ((_GD::_leftElements[j]->*cbck)(p, keyflags));
			}
		}

		for (int j = 0; j < _GD::_bottomElements.GetCount(); j++)
		{
			if (_GD::_bottomElements[j]->Contains(p)) {
				return ((_GD::_bottomElements[j]->*cbck)(p, keyflags));
			}
		}

		for (int j = 0; j < _GD::_rightElements.GetCount(); j++)
		{
			if (_GD::_rightElements[j]->Contains(p)) {
				return ((_GD::_rightElements[j]->*cbck)(p, keyflags));
			}
		}

		for (int j = 0; j < _GD::_topElements.GetCount(); j++)
		{
			if (_GD::_topElements[j]->Contains(p)) {
				return ((_GD::_topElements[j]->*cbck)(p, keyflags));
			}
		}
		return defaultRes;
	}


	virtual void LeftDown(Point p, dword keyflags) {
		prevMousePoint = p;
		if ( elementCapture_LeftDown != 0) {
			elementCapture_LeftDown = elementCapture_LeftDown->LeftDown(p, keyflags);
			return;
		}
		if ( _GD::_plotRect.Contains(p)  && !IsContainedByOverElement(p) ) {
			if ( keyflags & K_CTRL )
			{
				// SELECT ZOOM
				RectTracker tracker(*this);
				Rect selectedZoomArea = tracker.Track( RectfC(p.x,p.y,0,0), ALIGN_NULL, ALIGN_NULL) - _GD::_PlotTopLeft;
				if (selectedZoomArea.Width() !=0  && selectedZoomArea.Height() != 0) {
					_GD::ZoomOnRect( selectedZoomArea );
				}
			}
			return;
		}
		elementCapture_LeftDown = ProcessMouseCallBack<GraphDraw_ns::GraphElementFrame*>(p, keyflags, &GraphDraw_ns::GraphElementFrame::LeftDown);
	}

	virtual void LeftDouble(Point p, dword keyflags) {
		if ( elementCapture_LeftDouble != 0) {
			elementCapture_LeftDouble = elementCapture_LeftDouble->LeftDouble(p, keyflags);
			return;
		}
		if ( _GD::_plotRect.Contains(p) && !IsContainedByOverElement(p) ) {
			return;
		}
		elementCapture_LeftDouble = ProcessMouseCallBack<GraphDraw_ns::GraphElementFrame*>(p, keyflags, &GraphDraw_ns::GraphElementFrame::LeftDouble);
	}

	virtual void LeftDrag(Point p, dword keyflags) {
		if ( elementCapture_LeftDrag != 0) {
			elementCapture_LeftDrag = elementCapture_LeftDrag->LeftDrag(p, keyflags);
			return;
		}
		if ( _GD::_plotRect.Contains(p) && !IsContainedByOverElement(p) ) {
			// do PLOT SCROLLING
			return;
		}
		elementCapture_LeftDrag = ProcessMouseCallBack<GraphDraw_ns::GraphElementFrame*>(p, keyflags, &GraphDraw_ns::GraphElementFrame::LeftDrag);
	}

	virtual void RightDown(Point p, dword keyflags) {
		if ( elementCapture_RightDown != 0) {
			elementCapture_RightDown = elementCapture_RightDown->RightDown(p, keyflags);
			return;
		}
		if ( _GD::_plotRect.Contains(p) && !IsContainedByOverElement(p) ) {
			return;
		}
		elementCapture_RightDown = ProcessMouseCallBack<GraphDraw_ns::GraphElementFrame*>(p, keyflags, &GraphDraw_ns::GraphElementFrame::RightDown);
	}

	virtual void RightDouble(Point p, dword keyflags) {
		if ( elementCapture_RightDouble != 0) {
			elementCapture_RightDouble = elementCapture_RightDouble->RightDouble(p, keyflags);
			return;
		}
		if ( _GD::_plotRect.Contains(p)  && !IsContainedByOverElement(p) ) {
			return;
		}
		elementCapture_RightDouble = ProcessMouseCallBack<GraphDraw_ns::GraphElementFrame*>(p, keyflags, &GraphDraw_ns::GraphElementFrame::RightDouble);
	}

	virtual void MiddleDown(Point p, dword keyflags) {
		if ( elementCapture_MiddleDown != 0) {
			elementCapture_MiddleDown = elementCapture_MiddleDown->MiddleDown(p, keyflags);
			return;
		}
		if ( _GD::_plotRect.Contains(p) && !IsContainedByOverElement(p) ) {
			// do PLOT  CENTER  at point
			return;
		}
		elementCapture_MiddleDown = ProcessMouseCallBack<GraphDraw_ns::GraphElementFrame*>(p, keyflags, &GraphDraw_ns::GraphElementFrame::MiddleDown);
	}

	virtual void MouseMove(Point p, dword keyflags) {
		if ( elementCapture_MouseMove != 0) {
			elementCapture_MouseMove = elementCapture_MouseMove->MouseMove(p, keyflags);
			return;
		}
		if (( _GD::_plotRect.Contains(p) && !IsContainedByOverElement(p) ) || plotCapture_MouseMove)  {
			if ( keyflags & K_MOUSELEFT )	{
				// LEFT SCROLL
				_GD::_doFastPaint = true;
				plotCapture_MouseMove = true;
				CaptureMouseMove_cursorImage = CursorImage(p, keyflags);
				_GD::Scroll(p.x-prevMousePoint.x, p.y-prevMousePoint.y);
				prevMousePoint = p;
				Refresh();
			} else {
				plotCapture_MouseMove = false;
				CaptureMouseMove_cursorImage = Null;
				if (_GD::_doFastPaint) { // Do complete drawing when SCROLLING FINISHED
					_GD::_doFastPaint = false;
					Refresh();
				}
			}
			return;
		}

		elementCapture_MouseMove = ProcessMouseCallBack<GraphDraw_ns::GraphElementFrame*>(p, keyflags, &GraphDraw_ns::GraphElementFrame::MouseMove);
		if (( elementCapture_MouseMove == 0) && (_GD::_doFastPaint))
		{ // Do complete drawing when nothing special to be done
			_GD::_doFastPaint = false;
			Refresh();
		}

		CaptureMouseMove_cursorImage = Null;
		if ( elementCapture_MouseMove)
		{
			CaptureMouseMove_cursorImage = CursorImage(p, keyflags);
		}
	}


	virtual Image  CursorImage(Point p, dword keyflags)
	{
		if ( !CaptureMouseMove_cursorImage.IsNullInstance() ) return CaptureMouseMove_cursorImage;
		if ( _GD::_plotRect.Contains(p) && !IsContainedByOverElement(p) ) {
			if ( keyflags & K_CTRL ) return GraphCtrlImg::ZOOM();
			return GraphCtrlImg::SCROLL();
		}
		return  ProcessMouseCallBack<Image>(p, keyflags, &GraphDraw_ns::GraphElementFrame::CursorImage, GraphDrawImg::CROSS());
	}



	virtual void MouseWheel(Point p, int zdelta, dword keyflags)
	{
		for (int j = 0; j < _GD::_overElements.GetCount(); j++)
		{
			if (_GD::_overElements[j]->Contains(p)) {
				(_GD::_overElements[j]->MouseWheel)(p, zdelta, keyflags);
				return;
			}
		}

		if ( _GD::_plotRect.Contains(p) ) {
			if ( keyflags & K_CTRL ) // => WHEEL ZOOM
			{
				if (zdelta < 0) _GD::ApplyZoomFactor(1.2);
				else            _GD::ApplyInvZoomFactor(1.2);
				_GD::_doFastPaint = true;

			}
			return;
		}

		for (int j = 0; j < _GD::_leftElements.GetCount(); j++)
		{
			if (_GD::_leftElements[j]->Contains(p)) {
				(_GD::_leftElements[j]->MouseWheel)(p, zdelta, keyflags);
				return;
			}
		}

		for (int j = 0; j < _GD::_bottomElements.GetCount(); j++)
		{
			if (_GD::_bottomElements[j]->Contains(p)) {
				(_GD::_bottomElements[j]->MouseWheel)(p, zdelta, keyflags);
				return;
			}
		}

		for (int j = 0; j < _GD::_rightElements.GetCount(); j++)
		{
			if (_GD::_rightElements[j]->Contains(p)) {
				(_GD::_rightElements[j]->MouseWheel)(p, zdelta, keyflags);
				return ;
			}
		}

		for (int j = 0; j < _GD::_topElements.GetCount(); j++)
		{
			if (_GD::_topElements[j]->Contains(p)) {
				(_GD::_topElements[j]->MouseWheel)(p, zdelta, keyflags);
				return;
			}
		}
	}
};





template <class TYPES>
class StdGridAxisDrawCtrl : public GraphDraw_ns::GridAxisDraw<TYPES>
{
	typedef StdGridAxisDrawCtrl<TYPES> CLASSNAME;
	typedef GraphDraw_ns::GridAxisDraw<TYPES> _B;

	Point prevMousePoint;


	public:
	StdGridAxisDrawCtrl(typename TYPES::TypeCoordConverter& conv) : _B(conv) {}
	StdGridAxisDrawCtrl( StdGridAxisDrawCtrl& p) : _B(p) {}
	~StdGridAxisDrawCtrl() {}


	virtual Image  CursorImage(Point p, dword keyflags) {
		if ( keyflags & K_CTRL ){
			if (keyflags & K_SHIFT) {
				if (_B::IsVertical() ) return GraphCtrlImg::AXIS_ZOOM_Y();
				else                   return GraphCtrlImg::AXIS_ZOOM_X();
			}
			if (_B::IsVertical() ) return GraphCtrlImg::ZOOM_Y();
			else                   return GraphCtrlImg::ZOOM_X();
		}

		if (keyflags & K_SHIFT) {
			if (_B::IsVertical() ) return GraphCtrlImg::AXIS_SCROLL_Y();
			else                   return GraphCtrlImg::AXIS_SCROLL_X();
		}
		if (_B::IsVertical() ) return GraphCtrlImg::SCROLL_Y();
		else                   return GraphCtrlImg::SCROLL_X();
	}

	virtual GraphDraw_ns::GraphElementFrame* MouseWheel (Point p, int zdelta, dword keyflags) {
		typename TYPES::TypeCoordConverter& converter = _B::GetCoordConverter();
		if (zdelta < 0) zdelta = -1;
		else            zdelta =  1;
		zdelta *= converter.getScreenRange() / 5;

		if ( (keyflags & (K_CTRL|K_SHIFT)) == (K_CTRL|K_SHIFT) ) // => ZOOM on wheel (this axis only)
		{
			converter.updateGraphSize( converter.toGraph( converter.getScreenMin() - zdelta ),
			                           converter.toGraph( converter.getScreenMax() + zdelta ));
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
		}
		else if ( keyflags & K_CTRL )
		{ // => ZOOM on wheel ( whole graph )
			if (_B::IsVertical() ) {
				_B::_parent->ZoomY(converter.getScreenMax() + zdelta, converter.getScreenMin() - zdelta);
			} else {
				_B::_parent->ZoomX(converter.getScreenMin() - zdelta, converter.getScreenMax() + zdelta);
			}
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
		}
		else if ( keyflags & K_SHIFT )
		{
			// => SCROLL on wheel ( on axis )
			converter.updateGraphSize( converter.toGraph( converter.getScreenMin() - zdelta ),
			                           converter.toGraph( converter.getScreenMax() - zdelta ));
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
			return this; // Capture MouseCtrl
		}
		else
		{
			// => SCROLL on wheel ( whole graph )
			if (_B::IsVertical() ) {
				// Vertical drag
				_B::_parent->ScrollY(zdelta);
			} else {
				// Horizontal drag
				_B::_parent->ScrollX(zdelta);
			}
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
			return this; // Capture MouseCtrl
		}
		return 0; // no need to capture MouseCtrl
	}

	virtual GraphDraw_ns::GraphElementFrame*  LeftDown   (Point p, dword keyflags) {
		prevMousePoint = p;
		return 0; // no need to capture MouseCtrl
	}

	virtual GraphDraw_ns::GraphElementFrame* MouseMove (Point p, dword keyflags) {
		if (keyflags & K_MOUSELEFT)
		{
			if ( keyflags & K_SHIFT )
			{
				// ==> LEFT SCROLL (this axis only)
				int delta=0;
				if (_B::IsVertical() ) {
					// Vertical drag
					delta = p.y-prevMousePoint.y;
				} else {
					// Horizontal drag
					delta = p.x-prevMousePoint.x;
				}
				typename TYPES::TypeCoordConverter& converter = _B::GetCoordConverter();
				converter.Scroll( delta );
				prevMousePoint = p;
				_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
				return this; // Capture MouseCtrl
			}
			else // if (keyflags & K_SHIFT)
			{
				// ==> LEFT SCROLL ( whole graph )
				if (_B::IsVertical() ) {
					// Vertical drag
					_B::_parent->ScrollY(p.y-prevMousePoint.y);
				} else {
					// Horizontal drag
					_B::_parent->ScrollX(p.x-prevMousePoint.x);
				}
				prevMousePoint = p;
				_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
				return this; // Capture MouseCtrl
			}
		}
		return 0; // no need to capture MouseCtrl
	}

	virtual CLASSNAME* Clone() { return new CLASSNAME(*this); }
};




template <class TYPES, class LEGEND_DRAW_CLASS >
class StdLegendCtrl : public LEGEND_DRAW_CLASS
{
	typedef StdLegendCtrl<TYPES,LEGEND_DRAW_CLASS> CLASSNAME;
	typedef LEGEND_DRAW_CLASS _B;

	Point prevMousePoint;
	Ctrl* parentCtrl;

	public:
	StdLegendCtrl() : parentCtrl(0) {}
	StdLegendCtrl(Ctrl& p) : parentCtrl(&p) {}
	StdLegendCtrl( StdLegendCtrl& p) : _B(p), parentCtrl(p.parentCtrl)  {}
	~StdLegendCtrl() {}

	virtual bool Contains(Point p) const { return (_B::_frame.Contains(p) && _B::IsOverGraph()); }

	inline StdLegendCtrl& SetParentCtrl(Ctrl& p) { parentCtrl = &p; return *this; }

	virtual Image  CursorImage(Point p, dword keyflags) {
		if ( keyflags & K_CTRL ){
			return GraphCtrlImg::ELEMENT_RESIZE();
		}
		return GraphCtrlImg::ELEMENT_MOVE();
	}
	virtual GraphDraw_ns::GraphElementFrame* MouseMove (Point p, dword keyflags) {
		if (parentCtrl != 0) {
			if (keyflags & K_MOUSELEFT)
			{
				RectTracker tracker(*parentCtrl);
				if(keyflags & K_CTRL) {
					tracker.Dashed().Animation();
					_B::_frame = tracker.Track(_B::_frame, ALIGN_NULL, ALIGN_NULL);
					_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_TOTAL );
					return 0;
				}
				tracker.Dashed().Animation();
				_B::_frame = tracker.Track(_B::_frame, ALIGN_CENTER, ALIGN_CENTER);
				_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_TOTAL );
				return 0;
			}
		}
		return 0; // no need to capture MouseCtrl
	}

	virtual CLASSNAME* Clone() { return new CLASSNAME(*this); }
};

// ===============================================================================================================================
// ===============================================================================================================================
//
//
//
// ===============================================================================================================================
// ===============================================================================================================================
struct GraphCtrlDefaultTypes {
		typedef DataSource                                                      TypeDataSource;
		typedef SeriesPlot                                                      TypeSeriesPlot;
		typedef MarkPlot                                                        TypeMarkPlot;
		typedef GraphDraw_ns::GenericCoordinateConverter<Upp::int32, double>    TypeCoordConverter;
		typedef StdGridAxisDrawCtrl<GraphCtrlDefaultTypes>                      TypeGridAxisDraw;
		typedef GraphDraw_ns::GridStepManager<TypeCoordConverter>               TypeGridStepManager;
		typedef GraphDraw_ns::SeriesConfig<GraphCtrlDefaultTypes>               TypeSeriesConfig;
		typedef Vector<TypeSeriesConfig>                                        TypeVectorSeries;
//		typedef GraphDraw_ns::CRTP_StdGraphDraw<GraphCtrlDefaultTypes>          TypeGraphDraw_base;
//		typedef TypeGraphDraw_base                                              TypeGraphDraw;
};

namespace GraphCtrl_ns
{
	void OpenGridAxisPropertiesCtrl( GraphCtrlDefaultTypes::TypeGridAxisDraw* v);
};


template<class TYPES, class DERIVED >
class CRTP_StdGraphCtrl :	public CRTP_GraphCtrlBase< TYPES, DERIVED >
{
	public:
	typedef CRTP_StdGraphCtrl<TYPES, DERIVED>  CLASSNAME;
	typedef CRTP_GraphCtrlBase<TYPES, DERIVED> _B;
	typedef TYPES                              Types;

	private:
	void OpenGridAxisProperties(GraphDraw_ns::GraphElementFrame* v, Point p, dword keyflags) {
		GraphCtrl_ns::OpenGridAxisPropertiesCtrl( dynamic_cast<typename TYPES::TypeGridAxisDraw*>(v) );
	}

	public:
	CRTP_StdGraphCtrl() {
		_B::_xGridDraw.WhenLeftDouble << THISBACK(OpenGridAxisProperties);
		_B::_yGridDraw.WhenLeftDouble << THISBACK(OpenGridAxisProperties);
	}
};

template<class TYPES = GraphCtrlDefaultTypes >
class StdGraphCtrl :	public CRTP_StdGraphCtrl< TYPES, StdGraphCtrl<TYPES> >
{
};





#endif
