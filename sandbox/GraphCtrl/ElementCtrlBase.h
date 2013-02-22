#ifndef _GraphCtrl_ElementCtrlBase_h_
#define _GraphCtrl_ElementCtrlBase_h_


template<class TYPES, class ELEMENT_CLASS, class DERIVED>
class CRTPGraphElementCtrl_Base : public ELEMENT_CLASS {
	public:
	typedef CRTPGraphElementCtrl_Base<TYPES, ELEMENT_CLASS, DERIVED> CLASSNAME;
	typedef ELEMENT_CLASS _B;

	CRTPGraphElementCtrl_Base() {}
	virtual ~CRTPGraphElementCtrl_Base() {}

	template <class PARAM>
	CRTPGraphElementCtrl_Base(PARAM& p) : ELEMENT_CLASS(p) {}
	
	template <class PARAM1, class PARAM2>
	CRTPGraphElementCtrl_Base(PARAM1& p1, PARAM2 p2) : ELEMENT_CLASS(p1, p2) {}
	
	template <class PARAM1, class PARAM2, class PARAM3>
	CRTPGraphElementCtrl_Base(PARAM1& p1, PARAM2 p2, PARAM3 p3) : ELEMENT_CLASS(p1, p2, p3) {}
	
	public:
	
	// helper class to make 
	template <  template <class T> class DLG>
	void TOpenPropertiesDlg(void) {
		DLG<ELEMENT_CLASS> dlg;
		dlg.InitDlg(*this);
		if ( dlg.Execute() == IDOK ) {
				dlg.Retrieve();
		}
	}

	virtual void OpenPropertiesDlg(void) {
		TOpenPropertiesDlg<ElementPropertiesDlg>();
	}
	
	virtual bool Contains(Point p) const                                          { return (_B::_frame.Contains(p)); }
	virtual GraphDraw_ns::GraphElementFrame* LeftDouble (Point p, dword keyflags) { OpenPropertiesDlg(); return 0; }
	void ContextMenu(Bar& bar)                                                    { bar.Add(t_("Edit properties"), THISBACK(OpenPropertiesDlg)); }
	virtual GraphDraw_ns::GraphElementFrame* RightDown(Point p, dword keyflags)   { MenuBar::Execute(THISBACK(ContextMenu)); return 0; } 
	virtual Image  CursorImage(Point p, dword keyflags)                           { return GraphCtrlImg::ACTIVE_CROSS(); }
};

// ============================================================================================

template <class TYPES, class LEGEND_DRAW_CLASS >
class GraphElementCtrl_MoveResize : public LEGEND_DRAW_CLASS
{
	private:
	Point prevMousePoint;
	Ctrl* parentCtrl;

	public:
	typedef GraphElementCtrl_MoveResize<TYPES,LEGEND_DRAW_CLASS> CLASSNAME;
	typedef LEGEND_DRAW_CLASS _B;


	GraphElementCtrl_MoveResize() : parentCtrl(0) {}
	GraphElementCtrl_MoveResize(Ctrl& p) : parentCtrl(&p) {}
	GraphElementCtrl_MoveResize( GraphElementCtrl_MoveResize& p) : _B(p), parentCtrl(p.parentCtrl)  {}
	virtual ~GraphElementCtrl_MoveResize() {}

	virtual bool Contains(Point p) const { return (_B::_frame.Contains(p)); }

	virtual Image  CursorImage(Point p, dword keyflags) {
		if (_B::IsOverGraph()) {
			if ( keyflags & K_CTRL ){
				return GraphCtrlImg::ELEMENT_RESIZE();
			}
			return GraphCtrlImg::ELEMENT_MOVE();
		}
		return GraphCtrlImg::ACTIVE_CROSS();
	}
	
	virtual GraphDraw_ns::GraphElementFrame* MouseMove (Point p, dword keyflags) {
		if (parentCtrl == 0) {
			parentCtrl = ValueTo<Ctrl*>(_B::_parent->GetParentCtrl());
		}
		if ((parentCtrl != 0) && _B::IsOverGraph()) {
			if (keyflags & K_MOUSELEFT)
			{
				RectTracker tracker(*parentCtrl);
				if(keyflags & K_CTRL) {
					tracker.Dashed().Animation();
					_B::_overFrame = tracker.Track(_B::_overFrame, ALIGN_NULL, ALIGN_NULL);
					_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_TOTAL );
					return 0;
				}
				tracker.Dashed().Animation();
				_B::_overFrame = tracker.Track(_B::_overFrame, ALIGN_CENTER, ALIGN_CENTER);
				_B::_parent->RefreshFromChild( GraphDraw_ns::REFRESH_TOTAL );
				return 0;
			}
		}
		return 0; // no need to capture MouseCtrl
	}

	virtual CLASSNAME* Clone() { return new CLASSNAME(*this); }
};


#endif
