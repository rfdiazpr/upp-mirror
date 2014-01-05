#ifndef _GraphCtrl_StdElementCtrls_h_
#define _GraphCtrl_StdElementCtrls_h_


template <class TYPES, class GRIDAXISDRAW>
class StdGridAxisDrawCtrl : public GraphElementCtrl_Base<TYPES, GRIDAXISDRAW >
{
	typedef StdGridAxisDrawCtrl<TYPES, GRIDAXISDRAW> CLASSNAME;
	typedef GraphElementCtrl_Base<TYPES, GRIDAXISDRAW > _B;
	Point prevMousePoint;
	Point selectOriginPoint, selectEndPoint;
	Rect selectRect;
	bool useLocalSelectLoop;
	Value VSelectStyle, VAxisSelectStyle;
	Value HSelectStyle, HAxisSelectStyle;
	Value currSelectStyle;
	
	template <class COORDCONV>
	void TOpenPropertiesDlg(void) {
		GridAxisPropertiesDlg<CLASSNAME,  COORDCONV> dlg;
		COORDCONV& coordConv = * dynamic_cast< COORDCONV*>( &_B::GetCoordConverter() );
		dlg.InitDlg(*this, coordConv);
		if ( dlg.Execute() == IDOK ) {
			dlg.Retrieve();
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FULL );
		}
	}

	
	public:
	template<class COORDCONV>
	StdGridAxisDrawCtrl( COORDCONV& conv)
	: _B(conv)
	, useLocalSelectLoop(true)
	{
		_B::whenOpenPropertiesDlgCB = THISBACK( TOpenPropertiesDlg<COORDCONV> );
		VAxisSelectStyle = VSelectStyle = GraphCtrlImg::VSELECT();
		HAxisSelectStyle = HSelectStyle = GraphCtrlImg::HSELECT();
		VAxisSelectStyle = GraphCtrlImg::VAXISSELECT();
		HAxisSelectStyle = GraphCtrlImg::HAXISSELECT();
		currSelectStyle = Null;
	}
	
	private:
	StdGridAxisDrawCtrl( StdGridAxisDrawCtrl& p)
	: _B(p)
	, useLocalSelectLoop(p.useLocalSelectLoop)
	{}
	
	public:
	virtual ~StdGridAxisDrawCtrl() {}


	void FitToDataRefresh(GraphDraw_ns::FitToDataStrategy fitStrategy = GraphDraw_ns::ALL_SERIES) {
		_B::FitToData(fitStrategy);
		_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FULL );
	}

	virtual void ContextMenu(Bar& bar) {
		bar.Add(t_("Edit properties"),     THISBACK (OpenPropertiesDlg));
		bar.Add(t_("Fit To Data"),         THISBACK1(FitToDataRefresh,  GraphDraw_ns::ALL_SERIES));
		bar.Add(t_("Fit To Visible Data"), THISBACK1(FitToDataRefresh,  GraphDraw_ns::VISIBLE_SERIES_ONLY));
	}

	void UseLocalSelectLoop(bool p = true) {
		useLocalSelectLoop = p;
	}

	void SetVSelectStyle(Value p) {
		VSelectStyle = p;
	}
	void SetVAxisSelectStyle(Value p) {
		VAxisSelectStyle = p;
	}

	void SetHSelectStyle(Value p) {
		HSelectStyle = p;
	}
	void SetHAxisSelectStyle(Value p) {
		HAxisSelectStyle = p;
	}

	virtual Image  CursorImage(Point p, dword keyflags) {
		if (  TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_AXIS_ZOOM) ) {
			if (!_B::GetCoordConverter().IsZoomAllowed())    return GraphCtrlImg::CROSS();
			if (_B::IsVertical() ) return GraphCtrlImg::AXIS_ZOOM_Y();
			else                   return GraphCtrlImg::AXIS_ZOOM_X();
		}
		else if ( TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_ZOOM)) {
			if (!_B::GetCoordConverter().IsZoomAllowed())    return GraphCtrlImg::CROSS();
			if (_B::IsVertical() ) return GraphCtrlImg::ZOOM_Y();
			else                   return GraphCtrlImg::ZOOM_X();
		}
		else if ( TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_AXIS_SCROLL)) {
			if (!_B::GetCoordConverter().IsScrollAllowed())  return GraphCtrlImg::CROSS();
			if (_B::IsVertical() ) return GraphCtrlImg::AXIS_SCROLL_Y();
			else                   return GraphCtrlImg::AXIS_SCROLL_X();
		}
		else if ( TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_SCROLL)) {
			if (!_B::GetCoordConverter().IsScrollAllowed())  return GraphCtrlImg::CROSS();
			if (_B::IsVertical() ) return GraphCtrlImg::SCROLL_Y();
			else                   return GraphCtrlImg::SCROLL_X();
		}
		return GraphCtrlImg::CROSS();
	}

	virtual void MouseWheel (Point p, int zdelta, dword keyflags) {
		GraphDraw_ns::CoordinateConverter& converter = _B::GetCoordConverter();
		if (zdelta < 0) zdelta = -1;
		else            zdelta =  1;
		if (_B::IsVertical())  zdelta = -zdelta;
		zdelta *= abs(converter.getScreenRange()) / 5;

		if (  TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_AXIS_ZOOM) ) // => ZOOM on wheel (this axis only)
		{
			if (!_B::GetCoordConverter().IsZoomAllowed()) return;
			GraphDraw_ns::GraphUndoData undo;
			undo.undoAction << converter.MakeRestoreAxisMinMaxCB();
				converter.updateGraphSize( converter.toGraph( converter.getScreenMin() - zdelta ),
				                           converter.toGraph( converter.getScreenMax() + zdelta ));
			undo.redoAction << converter.MakeRestoreAxisMinMaxCB();
			_B::_parent->AddUndoAction(undo);
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
		}
		else if (  TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_ZOOM) )
		{
			// => ZOOM on wheel ( whole graph )
			if (!_B::GetCoordConverter().IsZoomAllowed()) return;
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
		else if (  TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_AXIS_SCROLL) )
		{
			// => SCROLL on wheel ( on axis )
			if (!_B::GetCoordConverter().IsScrollAllowed()) return;
			GraphDraw_ns::GraphUndoData undo;
			undo.undoAction << converter.MakeRestoreAxisMinMaxCB();
				converter.updateGraphSize( converter.toGraph( converter.getScreenMin() - zdelta ),
				                           converter.toGraph( converter.getScreenMax() - zdelta ));
			undo.redoAction << converter.MakeRestoreAxisMinMaxCB();
			_B::_parent->AddUndoAction(undo);
			_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
		}
		else if (  TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_SCROLL) )
		{
			if (!_B::GetCoordConverter().IsScrollAllowed()) return;
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

	virtual void  LeftDown(Point p, dword keyflags) {
		prevMousePoint = p;
		selectOriginPoint = p-_B::GetFrame().TopLeft();
		selectEndPoint = selectOriginPoint;


			GraphDraw_ns::CoordinateConverter& converter = _B::GetCoordConverter();
			Ctrl* parentCtrl = ValueTo<Ctrl*>(_B::_parent->GetParentCtrl());
			RectTracker tracker(*parentCtrl);
			tracker.Dashed().Animation();
			tracker.SetColor(Cyan()).Width(1);

			GraphDraw_ns::GraphUndoData undo;
			undo.undoAction << _B::_parent->MakeRestoreGraphSizeCB();

			if ( TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_AXIS_ZOOM) )
			{
				if (!_B::GetCoordConverter().IsZoomAllowed()) return;
				if (_B::IsVertical() ) {
					if ( useLocalSelectLoop ) {
						currSelectStyle = VAxisSelectStyle;
						_B::_parent->DoLocalLoop( THISBACK( _selectZoomLoop ) );
					} else {
						selectRect = tracker.Track(Rect(Point(_B::GetFrame().TopLeft().x, p.y), Size(_B::GetFrame().GetSize().cx, 2)), ALIGN_CENTER, ALIGN_NULL ) - _B::GetFrame().TopLeft();
					}
					converter.updateGraphSize( converter.toGraph( selectRect.bottom ), converter.toGraph( selectRect.top ));
				} else {
					if ( useLocalSelectLoop ) {
						currSelectStyle = HAxisSelectStyle;
						_B::_parent->DoLocalLoop( THISBACK( _selectZoomLoop ) );
					} else {
						selectRect = tracker.Track(Rect(Point(p.x, _B::GetFrame().TopLeft().y), Size(2,_B::GetFrame().GetSize().cy)), ALIGN_NULL, ALIGN_CENTER) - _B::GetFrame().TopLeft();
					}
					converter.updateGraphSize( converter.toGraph( selectRect.left ), converter.toGraph( selectRect.right ));
				}
				currSelectStyle = Null;
				_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
				
				if (p != (ValueTo<Ctrl*>(_B::_parent->GetParentCtrl()))->GetMouseViewPos()) {
					undo.redoAction << _B::_parent->MakeRestoreGraphSizeCB();
					_B::_parent->AddUndoAction(undo);
				}
			}
			else if (TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_ZOOM) )
			{
				if (!_B::GetCoordConverter().IsZoomAllowed()) return;
				if (_B::IsVertical() ) {
					if ( useLocalSelectLoop ) {
						currSelectStyle = VSelectStyle;
						_B::_parent->DoLocalLoop( THISBACK( _selectZoomLoop ) );
					}
					else {
						selectRect = tracker.Track(Rect(Point(_B::GetFrame().TopLeft().x, p.y), Size(_B::GetFrame().GetSize().cx, 2)), ALIGN_CENTER, ALIGN_NULL ) - _B::GetFrame().TopLeft();
					}
					_B::_parent->ZoomY( selectRect.top, selectRect.bottom );
				} else {
					if ( useLocalSelectLoop ) {
						currSelectStyle = HSelectStyle;
						_B::_parent->DoLocalLoop( THISBACK( _selectZoomLoop ) );
					}
					else {
						selectRect = tracker.Track(Rect(Point(p.x, _B::GetFrame().TopLeft().y), Size(2,_B::GetFrame().GetSize().cy)), ALIGN_NULL, ALIGN_CENTER) - _B::GetFrame().TopLeft();
					}
					_B::_parent->ZoomX(selectRect.left, selectRect.right );
				}
				currSelectStyle = Null;
				_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );

				if (p != (ValueTo<Ctrl*>(_B::_parent->GetParentCtrl()))->GetMouseViewPos()) {
					undo.redoAction << _B::_parent->MakeRestoreGraphSizeCB();
					_B::_parent->AddUndoAction(undo);
				}
			}
			else if ( TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_AXIS_SCROLL ) ) // SCROLL --ONLY THIS AXIS--
			{
				if (!_B::GetCoordConverter().IsScrollAllowed()) return;
				_B::_parent->DoLocalLoop( THISBACK( _AxisScrollLoop ) );

				if (p != (ValueTo<Ctrl*>(_B::_parent->GetParentCtrl()))->GetMouseViewPos()) {
					undo.redoAction << _B::_parent->MakeRestoreGraphSizeCB();
					_B::_parent->AddUndoAction(undo);
				}
			}
			else if ( TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_SCROLL )) // SCROLL --ONLY THIS AXIS--
			{
				if (!_B::GetCoordConverter().IsScrollAllowed()) return;
				_B::_parent->DoLocalLoop( THISBACK( _ScrollLoop ) );

				if (p != (ValueTo<Ctrl*>(_B::_parent->GetParentCtrl()))->GetMouseViewPos()) {
					undo.redoAction << _B::_parent->MakeRestoreGraphSizeCB();
					_B::_parent->AddUndoAction(undo);
				}
			}
	}

	private:
	virtual void PaintOnPlot_overData(Draw& dw, int otherWidth, int scale) {
		_B::PaintOnPlot_overData(dw, otherWidth, scale);
		if ( useLocalSelectLoop && !currSelectStyle.IsNull() ) {
			if (_B::IsVertical() ) {
				if (selectOriginPoint.y<=selectEndPoint.y) { selectRect = Rect(Point(0, selectOriginPoint.y), Point(otherWidth, selectEndPoint.y) );    }
				else                                       { selectRect = Rect(Point(0, selectEndPoint.y),    Point(otherWidth, selectOriginPoint.y) ); }
			} else {
				if (selectOriginPoint.x<=selectEndPoint.x) { selectRect = Rect(Point(selectOriginPoint.x, 0), Point(selectEndPoint.x, otherWidth) );    }
				else                                       { selectRect = Rect(Point(selectEndPoint.x, 0),    Point(selectOriginPoint.x, otherWidth) ); }
			}
			ChPaint(dw, selectRect, currSelectStyle);
		}
	}

	void _selectZoomLoop (Point p, dword keyflags) {
		selectEndPoint = p-_B::GetFrame().TopLeft();
		_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_KEEP_DATA );
	}

	void _ScrollLoop (Point p, dword keyflags) {
		if (_B::IsVertical() ) { _B::_parent->ScrollY(p.y-prevMousePoint.y); }
		else                   { _B::_parent->ScrollX(p.x-prevMousePoint.x); }
		prevMousePoint = p;
		_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
	}

	void _AxisScrollLoop (Point p, dword keyflags) {
		int delta=0;
		if (_B::IsVertical() ) { delta = p.y-prevMousePoint.y; }
		else                   { delta = p.x-prevMousePoint.x; }
		_B::_coordConverter.Scroll( delta );
		prevMousePoint = p;
		_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_FAST );
	}

	public:
	virtual void MouseMove (Point p, dword keyflags) {
	}
};


template<class TYPES, class LEGENDDRAW>
class StdLegendCtrl : public GraphElementCtrl_FloatMoveResize<TYPES, GraphElementCtrl_Base<TYPES, LEGENDDRAW > > {
	public:
	typedef StdLegendCtrl<TYPES, LEGENDDRAW>  CLASSNAME;
	typedef GraphElementCtrl_FloatMoveResize<TYPES, GraphElementCtrl_Base<TYPES, LEGENDDRAW > >  _B;
	StdLegendCtrl() {}
	StdLegendCtrl(StdLegendCtrl& p) : GraphElementCtrl_FloatMoveResize<TYPES, LEGENDDRAW>(p) {}
	virtual ~StdLegendCtrl() {}
};



template<class TYPES, class LABELDRAW>
class StdLabelCtrl : public  GraphElementCtrl_Base< TYPES, LABELDRAW > {
	public:
	typedef StdLabelCtrl<TYPES, LABELDRAW>  CLASSNAME;
	typedef GraphElementCtrl_Base< TYPES, LABELDRAW > _B;
	typedef TYPES  Types;
	
	StdLabelCtrl() {
		_B::whenOpenPropertiesDlgCB = THISBACK( _B::template TOpenPropertiesDlg<LabelPropertiesDlg> );
		_B::DisablePos(GraphDraw_ns::FLOAT_OVER_GRAPH);
	}
};



template<class TYPES, class MARKERDRAW>
class DynamicMarkerCtrl : public  GraphElementCtrl_Base< TYPES, MARKERDRAW > {
	protected:
	typedef DynamicMarkerCtrl<TYPES, MARKERDRAW>  CLASSNAME;
	typedef GraphElementCtrl_Base< TYPES, MARKERDRAW > _B;
	typedef TYPES  Types;
	Point prevMousePoint;
	GraphDraw_ns::TypeScreenCoord selectOffset;
	
	GraphDraw_ns::MarkerPosList::Iterator currMarkerPos; // marker selected for MOVE action

	public:
	GraphDraw_ns::TypeMarkerMoveCbk whenMarkerMove;
	GraphDraw_ns::TypeMarkerMoveCbk whenMarkerMoveStart;
	GraphDraw_ns::TypeMarkerMoveCbk whenMarkerMoveEnd;
		

	DynamicMarkerCtrl(GraphDraw_ns::CoordinateConverter& coordconv)
	: _B(coordconv)
	{ }
	
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
					case GraphDraw_ns::FLOAT_OVER_GRAPH:
						break;
				}
			}
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
					case GraphDraw_ns::FLOAT_OVER_GRAPH:
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
