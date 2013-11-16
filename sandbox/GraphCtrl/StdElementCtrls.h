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

	virtual void MouseWheel (Point p, int zdelta, dword keyflags) {
		typename TYPES::TypeCoordConverter& converter = _B::GetCoordConverter();
		if (zdelta < 0) zdelta = -1;
		else            zdelta =  1;
		if (_B::IsVertical())  zdelta = -zdelta;
		zdelta *= abs(converter.getScreenRange()) / 5;

		if ( (keyflags & (K_CTRL|K_SHIFT)) == (K_CTRL|K_SHIFT) ) // => ZOOM on wheel (this axis only)
		{
			GraphDraw_ns::GraphUndoData undo;
			undo.undoAction << converter.MakeRestoreAxisMinMaxCB();
				converter.updateGraphSize( converter.toGraph( converter.getScreenMin() - zdelta ),
				                           converter.toGraph( converter.getScreenMax() + zdelta ));
			undo.redoAction << converter.MakeRestoreAxisMinMaxCB();
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
			undo.undoAction << converter.MakeRestoreAxisMinMaxCB();
				converter.updateGraphSize( converter.toGraph( converter.getScreenMin() - zdelta ),
				                           converter.toGraph( converter.getScreenMax() - zdelta ));
			undo.redoAction << converter.MakeRestoreAxisMinMaxCB();
			_B::_parent->AddUndoAction(undo);
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
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
		}
	}

	virtual void  LeftDown   (Point p, dword keyflags) {
		prevMousePoint = p;
	}

	private:
	void _MouseMove(Point p, dword keyflags) {
		if (keyflags & K_MOUSELEFT)
		{
			if ( keyflags & K_SHIFT ) // SCROLL --ONLY THIS AXIS--
			{
				// ==> LEFT SCROLL 
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
			}
			else // SCROLL  --WHOLE GRAPH--
			{
				// ==> LEFT SCROLL 
				if (_B::IsVertical() ) {
					// Vertical drag
					_B::_parent->ScrollY(p.y-prevMousePoint.y);
				} else {
					// Horizontal drag
					_B::_parent->ScrollX(p.x-prevMousePoint.x);
				}
				prevMousePoint = p;
			}
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
		}
	}
	public:
	virtual void MouseMove (Point p, dword keyflags) {
		if (keyflags & K_MOUSELEFT)
		{
			GraphDraw_ns::GraphUndoData undo;
			undo.undoAction << _B::_parent->MakeRestoreGraphSizeCB();
				_B::_parent->DoLocalLoop( THISBACK( _MouseMove ) );
			undo.redoAction << _B::_parent->MakeRestoreGraphSizeCB();
			_B::_parent->AddUndoAction(undo);
		}
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



template<class TYPES, class MARKERDRAW>
class DynamicMarkerCtrl : public  CRTPGraphElementCtrl_Base< TYPES, MARKERDRAW, DynamicMarkerCtrl<TYPES, MARKERDRAW> > {
	protected:
	typedef DynamicMarkerCtrl<TYPES, MARKERDRAW>  CLASSNAME;
	typedef CRTPGraphElementCtrl_Base< TYPES, MARKERDRAW, CLASSNAME > _B;
	typedef TYPES  Types;
	Point prevMousePoint;
	GraphDraw_ns::TypeScreenCoord selectOffset;
	
	GraphDraw_ns::MarkerPosList::Iterator currMarkerPos; // marker selected for MOVE action

	public:
	GraphDraw_ns::TypeMarkerMoveCbk whenMarkerMove;
	GraphDraw_ns::TypeMarkerMoveCbk whenMarkerMoveStart;
	GraphDraw_ns::TypeMarkerMoveCbk whenMarkerMoveEnd;
		

	DynamicMarkerCtrl(typename TYPES::TypeCoordConverter& coordconv)
	: _B(coordconv)
	{ }
	
//	virtual void OpenPropertiesDlg(void) {
//		_B::template TOpenPropertiesDlg<LabelPropertiesDlg>();
//	}

	private:

	void _MoveMarker (Point p, dword keyflags) {
		if (keyflags & K_MOUSELEFT)
		{
			if (_B::IsVertical() ) {
				// Vertical drag
				(*currMarkerPos) =  _B::_coordConverter.toGraph(p.y+selectOffset);
			} else {
				// Horizontal drag
				(*currMarkerPos) =  _B::_coordConverter.toGraph(p.x+selectOffset);
			}
			prevMousePoint = p;
			whenMarkerMove(_B::markers, currMarkerPos->GetID());
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_KEEP_DATA );
		}
	}
	
	public:
	virtual void  LeftDown(Point p, dword keyflags) {
		prevMousePoint = p;
	}

	virtual void MouseMove (Point p, dword keyflags) {
		if (keyflags & K_MOUSELEFT)
		{
			GraphDraw_ns::MarkerPosList::Iterator iter = _B::markers.End();
			GraphDraw_ns::MarkerPosList::ConstIterator endIter = _B::markers.Begin();
			while ( iter != endIter ) {
				--iter;
				GraphDraw_ns::MarkerElementData& markerData = *iter;
				if (_B::_coordConverter.IsInGraphVisibleRange(markerData)) {
					switch( _B::GetElementPos() ) {
						case GraphDraw_ns::LEFT_OF_GRAPH:
							if ( markerData.GetTickMark().Contains(p, _B::GetFrame().TopLeft(),  _B::GetElementPos(), _B::GetElementWidth(), _B::_coordConverter.toScreen(markerData) )) {
								currMarkerPos = iter;
								selectOffset = _B::_coordConverter.toScreen(markerData) - p.y;
								whenMarkerMoveStart(_B::markers, currMarkerPos->GetID());
								_B::_parent->DoLocalLoop( THISBACK( _MoveMarker ) );
								whenMarkerMoveEnd(_B::markers, currMarkerPos->GetID());
							}
							break;
						case GraphDraw_ns::BOTTOM_OF_GRAPH:
							if ( markerData.GetTickMark().Contains(p, _B::GetFrame().TopLeft(), _B::GetElementPos(), _B::_coordConverter.toScreen(markerData), 0)) {
								currMarkerPos = iter;
								selectOffset = _B::_coordConverter.toScreen(markerData) - p.x;
								whenMarkerMoveStart(_B::markers, currMarkerPos->GetID());
								_B::_parent->DoLocalLoop( THISBACK( _MoveMarker ) );
								whenMarkerMoveEnd(_B::markers, currMarkerPos->GetID());
							}
							break;
						case GraphDraw_ns::TOP_OF_GRAPH:
							if ( markerData.GetTickMark().Contains(p, _B::GetFrame().TopLeft(), _B::GetElementPos(),  _B::_coordConverter.toScreen(markerData), _B::GetElementWidth() )) {
								currMarkerPos = iter;
								selectOffset = _B::_coordConverter.toScreen(markerData) - p.x;
								whenMarkerMoveStart(_B::markers, currMarkerPos->GetID());
								_B::_parent->DoLocalLoop( THISBACK( _MoveMarker ) );
								whenMarkerMoveEnd(_B::markers, currMarkerPos->GetID());
							}
							break;
						case GraphDraw_ns::RIGHT_OF_GRAPH:
							if ( markerData.GetTickMark().Contains(p, _B::GetFrame().TopLeft(), _B::GetElementPos(), 0, _B::_coordConverter.toScreen(markerData) )) {
								currMarkerPos = iter;
								selectOffset = _B::_coordConverter.toScreen(markerData) - p.y;
								whenMarkerMoveStart(_B::markers, currMarkerPos->GetID());
								_B::_parent->DoLocalLoop( THISBACK( _MoveMarker ) );
								whenMarkerMoveEnd(_B::markers, currMarkerPos->GetID());
							}
							break;
						case GraphDraw_ns::OVER_GRAPH:
							break;
					}
				}
			}


			//GraphDraw_ns::GraphUndoData undo;
			//undo.undoAction << _B::_parent->MakeRestoreGraphSizeCB();
			//	_B::_parent->DoLocalLoop( THISBACK( _MouseMove ) );
			//undo.redoAction << _B::_parent->MakeRestoreGraphSizeCB();
			//_B::_parent->AddUndoAction(undo);
		}
	}

	virtual Image  CursorImage(Point p, dword keyflags) {
		GraphDraw_ns::MarkerPosList::Iterator iter = _B::markers.Begin();
		GraphDraw_ns::MarkerPosList::ConstIterator endIter = _B::markers.End();

		while ( iter != endIter ) {
			GraphDraw_ns::MarkerElementData& markerData = *iter;
			if (_B::_coordConverter.IsInGraphVisibleRange(markerData)) {
				switch( _B::GetElementPos() ) {
					case GraphDraw_ns::LEFT_OF_GRAPH:
						if ( markerData.GetTickMark().Contains(p, _B::GetFrame().TopLeft(), _B::GetElementPos(), _B::GetElementWidth(), _B::_coordConverter.toScreen(markerData) ) ) return GraphCtrlImg::SCROLL_Y();
						break;
					case GraphDraw_ns::BOTTOM_OF_GRAPH:
						if ( markerData.GetTickMark().Contains(p, _B::GetFrame().TopLeft(), _B::GetElementPos(), _B::_coordConverter.toScreen(markerData), 0 )) return GraphCtrlImg::SCROLL_X();
						break;
					case GraphDraw_ns::TOP_OF_GRAPH:
						if ( markerData.GetTickMark().Contains(p, _B::GetFrame().TopLeft(), _B::GetElementPos(), _B::_coordConverter.toScreen(markerData), _B::GetElementWidth() )) return GraphCtrlImg::SCROLL_X();
						break;
					case GraphDraw_ns::RIGHT_OF_GRAPH:
						if ( markerData.GetTickMark().Contains(p, _B::GetFrame().TopLeft(), _B::GetElementPos(), 0, _B::_coordConverter.toScreen(markerData) ) ) return GraphCtrlImg::SCROLL_Y();
						break;
					case GraphDraw_ns::OVER_GRAPH:
						return _B::CursorImage(p,keyflags);
						break;
				}
			}
			++iter;
		}
		return _B::CursorImage(p,keyflags);
	}

};


#endif
