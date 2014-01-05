#ifndef _GraphCtrl_ElementCtrlBase_h_
#define _GraphCtrl_ElementCtrlBase_h_





// ============================================================================================


template<class TYPES, class ELEMENT_CLASS>
class GraphElementCtrl_Base : public ELEMENT_CLASS {
	public:
	typedef GraphElementCtrl_Base<TYPES, ELEMENT_CLASS> CLASSNAME;
	typedef ELEMENT_CLASS _B;

	Callback  whenOpenPropertiesDlgCB;
	template <  template <class T> class DLG>
	void TOpenPropertiesDlg(void) {
		DLG<ELEMENT_CLASS> dlg;
		dlg.InitDlg(*this);
		if ( dlg.Execute() == IDOK ) {
				dlg.Retrieve();
		}
	}

	GraphElementCtrl_Base() { whenOpenPropertiesDlgCB = THISBACK( TOpenPropertiesDlg<ElementPropertiesDlg> ); }
	virtual ~GraphElementCtrl_Base() {}

	template <class PARAM>
	GraphElementCtrl_Base(PARAM& p) : ELEMENT_CLASS(p) { whenOpenPropertiesDlgCB = THISBACK( TOpenPropertiesDlg<ElementPropertiesDlg> ); }
	
	template <class PARAM1, class PARAM2>
	GraphElementCtrl_Base(PARAM1& p1, PARAM2 p2) : ELEMENT_CLASS(p1, p2) { whenOpenPropertiesDlgCB = THISBACK( TOpenPropertiesDlg<ElementPropertiesDlg> ); }
	
	template <class PARAM1, class PARAM2, class PARAM3>
	GraphElementCtrl_Base(PARAM1& p1, PARAM2 p2, PARAM3 p3) : ELEMENT_CLASS(p1, p2, p3) { whenOpenPropertiesDlgCB = THISBACK( TOpenPropertiesDlg<ElementPropertiesDlg> ); }
	
	public:

	virtual void OpenPropertiesDlg(void) {
		whenOpenPropertiesDlgCB();
	}
	
	virtual bool Contains(Point p) const                { return (_B::_frame.Contains(p)); }
	virtual void LeftDouble (Point p, dword keyflags)   { OpenPropertiesDlg(); }
	virtual void ContextMenu(Bar& bar)                  { bar.Add(t_("Edit properties"), THISBACK(OpenPropertiesDlg)); }
	virtual void RightDown(Point p, dword keyflags)     { MenuBar::Execute(THISBACK(ContextMenu)); } 
	virtual Image  CursorImage(Point p, dword keyflags) { return GraphCtrlImg::ACTIVE_CROSS(); }
};

// ============================================================================================

template <class TYPES, class BASE >
class GraphElementCtrl_FloatMoveResize : public BASE
{
	private:
	Point prevMousePoint;
	Ctrl* parentCtrl;
	int moveBorderWidth;

	public:
	typedef GraphElementCtrl_FloatMoveResize<TYPES,BASE> CLASSNAME;
	typedef BASE _B;


	static Image GetMoveResizeCursor( GraphDraw_ns::ElementBorderPosition p )
	{
		switch(p) {
			case GraphDraw_ns::ELEMENT_BORDER_LEFT : return Image::SizeHorz();
			case GraphDraw_ns::ELEMENT_BORDER_TOP : return Image::SizeVert();
			case GraphDraw_ns::ELEMENT_BORDER_RIGHT : return Image::SizeHorz();
			case GraphDraw_ns::ELEMENT_BORDER_BOTTOM : return Image::SizeVert();
			case GraphDraw_ns::ELEMENT_BORDER_CENTER : return Image::SizeAll();
			case GraphDraw_ns::ELEMENT_BORDER_TOP_LEFT : return Image::SizeTopLeft();
			case GraphDraw_ns::ELEMENT_BORDER_TOP_RIGHT : return Image::SizeTopRight();
			case GraphDraw_ns::ELEMENT_BORDER_BOTTOM_RIGHT : return Image::SizeBottomRight();
			case GraphDraw_ns::ELEMENT_BORDER_BOTTOM_LEFT : return Image::SizeBottomLeft();
			default:
				return Image::SizeAll();
				break;
		}
	}


	GraphElementCtrl_FloatMoveResize() : parentCtrl(0), moveBorderWidth(4) {}

	template <class PARAM>
	GraphElementCtrl_FloatMoveResize(PARAM& p) : _B(p), parentCtrl(0), moveBorderWidth(4) {}

	template <class PAR1, class PAR2>
	GraphElementCtrl_FloatMoveResize(PAR1& p1, PAR2 p2) : _B(p1, p2), parentCtrl(0), moveBorderWidth(4) {}

	template <class PAR1, class PAR2, class PAR3>
	GraphElementCtrl_FloatMoveResize(PAR1& p1, PAR2 p2, PAR3 p3) : _B(p1, p2, p3), parentCtrl(0), moveBorderWidth(4) {}



	GraphElementCtrl_FloatMoveResize(Ctrl& p) : parentCtrl(&p) {}
	GraphElementCtrl_FloatMoveResize( GraphElementCtrl_FloatMoveResize& p) : _B(p), parentCtrl(p.parentCtrl), moveBorderWidth(4)  {}
	virtual ~GraphElementCtrl_FloatMoveResize() {}

	virtual bool Contains(Point p) const { return (_B::_frame.Contains(p)); }

	virtual Image  CursorImage(Point p, dword keyflags) {
		if (_B::IsFloat()) {
			if ( TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_ELEMENT_FLOAT_MOVERESIZE) ) {
				return ( GetMoveResizeCursor( GraphDraw_ns::GetElementSubArea(p, _B::_floatFrame, moveBorderWidth) ) );
			}
			else if ( TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_ELEMENT_FLOAT_MOVE) ) {
				return GraphCtrlImg::ELEMENT_MOVE();
			}
		}
		return _B::CursorImage(p, keyflags);
	}
	
	
	Callback MakeRestoreElementPosCB() {
		return 	THISBACK1( _B::SetFloatFrame, _B::_floatFrame);
	}

	virtual void LeftDown(Point p, dword keyflags) {
		if (parentCtrl == 0) {
			parentCtrl = ValueTo<Ctrl*>(_B::_parent->GetParentCtrl());
		}
		if (_B::IsFloat() ) {
			
			if ( (parentCtrl != 0) && (keyflags & K_MOUSELEFT) )
			{
				RectTracker tracker(*parentCtrl);
				tracker.Dashed().Animation();
				tracker.MaxRect(parentCtrl->GetSize());
				tracker.SetCursorImage( CursorImage(p,keyflags) );
				GraphDraw_ns::GraphUndoData undo;
				undo.undoAction << MakeRestoreElementPosCB();
				if ( TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_ELEMENT_FLOAT_MOVERESIZE) ) {
					switch( GraphDraw_ns::GetElementSubArea(p, _B::_floatFrame, moveBorderWidth) ) {
						case GraphDraw_ns::ELEMENT_BORDER_LEFT : _B::_floatFrame = tracker.Track(_B::_floatFrame, ALIGN_LEFT, ALIGN_CENTER); break;
						case GraphDraw_ns::ELEMENT_BORDER_TOP : _B::_floatFrame = tracker.Track(_B::_floatFrame, ALIGN_CENTER, ALIGN_TOP); break;
						case GraphDraw_ns::ELEMENT_BORDER_RIGHT : _B::_floatFrame = tracker.Track(_B::_floatFrame, ALIGN_RIGHT, ALIGN_CENTER); break;
						case GraphDraw_ns::ELEMENT_BORDER_BOTTOM : _B::_floatFrame = tracker.Track(_B::_floatFrame, ALIGN_CENTER, ALIGN_BOTTOM); break;
						case GraphDraw_ns::ELEMENT_BORDER_CENTER : _B::_floatFrame = tracker.Track(_B::_floatFrame, ALIGN_CENTER, ALIGN_CENTER); break;
						case GraphDraw_ns::ELEMENT_BORDER_TOP_LEFT : _B::_floatFrame = tracker.Track(_B::_floatFrame, ALIGN_LEFT, ALIGN_TOP); break;
						case GraphDraw_ns::ELEMENT_BORDER_TOP_RIGHT : _B::_floatFrame = tracker.Track(_B::_floatFrame, ALIGN_RIGHT, ALIGN_TOP); break;
						case GraphDraw_ns::ELEMENT_BORDER_BOTTOM_RIGHT : _B::_floatFrame = tracker.Track(_B::_floatFrame, ALIGN_RIGHT, ALIGN_BOTTOM); break;
						case GraphDraw_ns::ELEMENT_BORDER_BOTTOM_LEFT : _B::_floatFrame = tracker.Track(_B::_floatFrame, ALIGN_LEFT, ALIGN_BOTTOM); break;
						default:
							_B::_floatFrame = tracker.Track(_B::_floatFrame, ALIGN_CENTER, ALIGN_BOTTOM);
							break;
					}
					_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_KEEP_DATA );
					
					if (p != (ValueTo<Ctrl*>(_B::_parent->GetParentCtrl()))->GetMouseViewPos()) {
						undo.redoAction << MakeRestoreElementPosCB();
						_B::_parent->AddUndoAction(undo);
					}
				}
				else if ( TEST_GC_KEYS(keyflags, GraphCtrl_Keys::K_ELEMENT_FLOAT_MOVE) ) {
					_B::_floatFrame = tracker.Track(_B::_floatFrame, ALIGN_CENTER, ALIGN_CENTER);
					_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_KEEP_DATA );

					if (p != (ValueTo<Ctrl*>(_B::_parent->GetParentCtrl()))->GetMouseViewPos()) {
						undo.redoAction << MakeRestoreElementPosCB();
						_B::_parent->AddUndoAction(undo);
					}
				}
			}
		}
		else
		{
			_B::LeftDown(p, keyflags);
		}
	}
};


#endif
