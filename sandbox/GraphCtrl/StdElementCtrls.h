#ifndef _GraphCtrl_StdElementCtrls_h_
#define _GraphCtrl_StdElementCtrls_h_


template <class TYPES>
class StdGridAxisDrawCtrl : public CRTPGraphElementCtrl_Base<TYPES, GraphDraw_ns::GridAxisDraw<TYPES>, StdGridAxisDrawCtrl<TYPES> >
{
	typedef StdGridAxisDrawCtrl<TYPES> CLASSNAME;
	typedef CRTPGraphElementCtrl_Base<TYPES, GraphDraw_ns::GridAxisDraw<TYPES>, StdGridAxisDrawCtrl<TYPES> > _B;
	Point prevMousePoint;

	public:
	StdGridAxisDrawCtrl(typename TYPES::TypeCoordConverter& conv) : _B(conv) {}
	StdGridAxisDrawCtrl( StdGridAxisDrawCtrl& p) : _B(p) {}
	~StdGridAxisDrawCtrl() {}

	virtual CLASSNAME* Clone() { return new CLASSNAME(*this); }


	void FitToDataRefresh(GraphDraw_ns::FitToDataStrategy fitStrategy = GraphDraw_ns::ALL_SERIES) {
		_B::FitToData(fitStrategy);
		_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_TOTAL );
	}

	virtual void ContextMenu(Bar& bar) {
		bar.Add(t_("Edit properties"),     THISBACK (OpenPropertiesDlg));
		bar.Add(t_("Fit To Data"),         THISBACK1(FitToDataRefresh,  GraphDraw_ns::ALL_SERIES));
		bar.Add(t_("Fit To Visible Data"), THISBACK1(FitToDataRefresh,  GraphDraw_ns::VISIBLE_SERIES_ONLY));
	}

	virtual void OpenPropertiesDlg(void) {
		GridAxisPropertiesDlg<CLASSNAME,  typename TYPES::TypeCoordConverter> dlg;
		dlg.InitDlg(*this, _B::GetCoordConverter());
		if ( dlg.Execute() == IDOK ) {
			dlg.Retrieve();
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_TOTAL );
		}
	}

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
		if (_B::IsVertical())  zdelta = -zdelta;
		zdelta *= abs(converter.getScreenRange()) / 5;

		if ( (keyflags & (K_CTRL|K_SHIFT)) == (K_CTRL|K_SHIFT) ) // => ZOOM on wheel (this axis only)
		{
			GraphDraw_ns::GraphUndoData undo;
			undo.undoAction << converter.MakeRestoreGraphMinMaxCB();
				converter.updateGraphSize( converter.toGraph( converter.getScreenMin() - zdelta ),
				                           converter.toGraph( converter.getScreenMax() + zdelta ));
			undo.redoAction << converter.MakeRestoreGraphMinMaxCB();
			_B::_parent->AddUndoAction(undo);
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
		}
		else if ( keyflags & K_CTRL )
		{ // => ZOOM on wheel ( whole graph )
			GraphDraw_ns::GraphUndoData undo;
			undo.undoAction << _B::_parent->MakeRestoreGraphSizeCB();
				if (_B::IsVertical() ) {
					_B::_parent->ZoomY(converter.getScreenMax() + zdelta, converter.getScreenMin() - zdelta);
				} else {
					_B::_parent->ZoomX(converter.getScreenMin() - zdelta, converter.getScreenMax() + zdelta);
				}
			undo.redoAction << _B::_parent->MakeRestoreGraphSizeCB();
			_B::_parent->AddUndoAction(undo);
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
		}
		else if ( keyflags & K_SHIFT )
		{
			// => SCROLL on wheel ( on axis )
			GraphDraw_ns::GraphUndoData undo;
			undo.undoAction << converter.MakeRestoreGraphMinMaxCB();
				converter.updateGraphSize( converter.toGraph( converter.getScreenMin() - zdelta ),
				                           converter.toGraph( converter.getScreenMax() - zdelta ));
			undo.redoAction << converter.MakeRestoreGraphMinMaxCB();
			_B::_parent->AddUndoAction(undo);
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
			return this; // Capture MouseCtrl
		}
		else
		{
			// => SCROLL on wheel ( ALL vertical OR horizontal axis )
			GraphDraw_ns::GraphUndoData undo;
			undo.undoAction << _B::_parent->MakeRestoreGraphSizeCB();
				if (_B::IsVertical() ) {
					// Vertical drag
					_B::_parent->ScrollY(zdelta);
				} else {
					// Horizontal drag
					_B::_parent->ScrollX(zdelta);
				}
			undo.redoAction << _B::_parent->MakeRestoreGraphSizeCB();
			_B::_parent->AddUndoAction(undo);
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
};


template<class TYPES, class LEGENDDRAW>
class StdLegendCtrl : public GraphElementCtrl_MoveResize<TYPES, CRTPGraphElementCtrl_Base<TYPES, LEGENDDRAW, StdLegendCtrl<TYPES, LEGENDDRAW> > > {
	public:
	typedef StdLegendCtrl<TYPES, LEGENDDRAW>  CLASSNAME;
	typedef GraphElementCtrl_MoveResize<TYPES, CRTPGraphElementCtrl_Base<TYPES, LEGENDDRAW, CLASSNAME > >  _B;
	StdLegendCtrl() {}
	StdLegendCtrl(StdLegendCtrl& p) : GraphElementCtrl_MoveResize<TYPES, LEGENDDRAW>(p) {}
	virtual ~StdLegendCtrl() {}
};



template<class TYPES, class LABELDRAW>
class StdLabelCtrl : public  CRTPGraphElementCtrl_Base< TYPES, LABELDRAW, StdLabelCtrl<TYPES, LABELDRAW> > {
	public:
	typedef StdLabelCtrl<TYPES, LABELDRAW>  CLASSNAME;
	typedef CRTPGraphElementCtrl_Base< TYPES, LABELDRAW, CLASSNAME > _B;
	typedef TYPES  Types;
	
	StdLabelCtrl() {
		_B::DisablePos(GraphDraw_ns::OVER_GRAPH);
	}
	
	virtual void OpenPropertiesDlg(void) {
		_B::template TOpenPropertiesDlg<LabelPropertiesDlg>();
	}
};

#endif
