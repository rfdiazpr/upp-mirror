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

	public:
	typedef GraphElementCtrl_FloatMoveResize<TYPES,BASE> CLASSNAME;
	typedef BASE _B;


	GraphElementCtrl_FloatMoveResize() : parentCtrl(0) {}

	template <class PARAM>
	GraphElementCtrl_FloatMoveResize(PARAM& p) : _B(p), parentCtrl(0) {}

	template <class PAR1, class PAR2>
	GraphElementCtrl_FloatMoveResize(PAR1& p1, PAR2 p2) : _B(p1, p2), parentCtrl(0) {}

	template <class PAR1, class PAR2, class PAR3>
	GraphElementCtrl_FloatMoveResize(PAR1& p1, PAR2 p2, PAR3 p3) : _B(p1, p2, p3), parentCtrl(0) {}



	GraphElementCtrl_FloatMoveResize(Ctrl& p) : parentCtrl(&p) {}
	GraphElementCtrl_FloatMoveResize( GraphElementCtrl_FloatMoveResize& p) : _B(p), parentCtrl(p.parentCtrl)  {}
	virtual ~GraphElementCtrl_FloatMoveResize() {}

	virtual bool Contains(Point p) const { return (_B::_frame.Contains(p)); }

	virtual Image  CursorImage(Point p, dword keyflags) {
		if (_B::IsFloat()) {
			if ( keyflags & K_CTRL ) {
				return GraphCtrlImg::ELEMENT_RESIZE();
			}
			return GraphCtrlImg::ELEMENT_MOVE();
		}
		return _B::CursorImage(p, keyflags);
	}
	
	
	Callback MakeRestoreElementPosCB() {
		return 	THISBACK1( _B::SetFloatFrame, _B::_floatFrame);
	}

	
	virtual void MouseMove (Point p, dword keyflags) {
		if (parentCtrl == 0) {
			parentCtrl = ValueTo<Ctrl*>(_B::_parent->GetParentCtrl());
		}
		if (_B::IsFloat() ) {
			if ( (parentCtrl != 0) && (keyflags & K_MOUSELEFT) )
			{
				RectTracker tracker(*parentCtrl);
				tracker.Dashed().Animation();
				tracker.SetCursorImage( CursorImage(p,keyflags) );
				GraphDraw_ns::GraphUndoData undo;
				undo.undoAction << MakeRestoreElementPosCB();
					if(keyflags & K_CTRL) {
						_B::_floatFrame = tracker.Track(_B::_floatFrame, ALIGN_NULL, ALIGN_NULL);
						_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_KEEP_DATA );
					}
					else {
						_B::_floatFrame = tracker.Track(_B::_floatFrame, ALIGN_CENTER, ALIGN_CENTER);
						_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_KEEP_DATA );
					}
				undo.redoAction << MakeRestoreElementPosCB();
				_B::_parent->AddUndoAction(undo);
			}
		}
		else
		{
			_B::MouseMove(p, keyflags);
		}
	}

};


#endif
