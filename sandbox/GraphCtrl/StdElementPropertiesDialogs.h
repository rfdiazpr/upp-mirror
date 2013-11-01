#ifndef _GraphCtrl_StdElementPropertiesDialogs_h_
#define _GraphCtrl_StdElementPropertiesDialogs_h_


template <class ELEMENT>
class ElementPropertiesDlg : public WithElementBaseLayout<TopWindow> {
	public:
	CtrlRetriever r1;
	ELEMENT*      elem;
	int           pos;
	
	public:
	typedef ElementPropertiesDlg<ELEMENT>  CLASSNAME;
	typedef WithElementBaseLayout<TopWindow> _B;
	
	ElementPropertiesDlg() : elem(0), pos(100) {
		CtrlLayoutOKCancel(*this, "");
	}
	virtual ~ElementPropertiesDlg() {}

	
	void InitDlg(ELEMENT& element) {
		_B::Title(element._name);
		elem = &element;
		position.Add(GraphDraw_ns::OVER_GRAPH, t_("FLOAT") );
		position.Add(GraphDraw_ns::TOP_OF_GRAPH, t_("TOP") );
		position.Add(GraphDraw_ns::BOTTOM_OF_GRAPH, t_("BOTTOM") );
		position.Add(GraphDraw_ns::LEFT_OF_GRAPH, t_("LEFT") );
		position.Add(GraphDraw_ns::RIGHT_OF_GRAPH, t_("RIGHT") );

		for (int c=0; c<position.GetCases().GetCount(); ++c) {
			 const Switch::Case& caseData = position.GetCases()[c];
			if ((elem->_allowedPosMask & ValueTo<int>(caseData.value)) == 0 ) position.DisableValue(caseData.value);
		}

		pos = elem->_pos;
		r1( width, elem->_width)
		  ( hide, elem->_hide)
		  ( stackingPrio, elem->_stackingPriority)
		  ( position, pos)
		  ;
	}
	
	virtual void Retrieve() {
		r1.Retrieve();
		elem->SetElementPos(static_cast<GraphDraw_ns::ElementPosition>(pos));
		elem->_parent->RefreshFromChild( GraphDraw_ns::REFRESH_TOTAL );
	}
};
// ============================================================================================

// Default GridAxisPropertiesDlg  class
// it opens the minimal dialog for the CoordConverter beeing used
template <class LABELDRAW>
class LabelPropertiesDlg : public WithLabelPropertiesDlgLayout<ElementPropertiesDlg<LABELDRAW> > {
	public:
	CtrlRetriever r2;
	
	public:
	typedef LabelPropertiesDlg<LABELDRAW>  CLASSNAME;
	typedef WithLabelPropertiesDlgLayout<ElementPropertiesDlg<LABELDRAW> > _B;
	
	LabelPropertiesDlg() {
		SetLayout_LabelPropertiesDlgLayout(*this, true);
		 Size sz = _B::GetLayoutSize();
		 _B::SetMinSize(sz);
		 _B::SetRect(sz);
	}
	
	virtual ~LabelPropertiesDlg() {}

	void InitDlg(LABELDRAW& gda) {
		_B::InitDlg(gda);
		r2( _B::textColor, _B::elem->_color)
		  ( _B::labelText, _B::elem->_label)
		  ;
	}
		
	virtual void Retrieve() {
		r2.Retrieve();
		_B::Retrieve();
	}
};
// ============================================================================================

// Default GridAxisPropertiesDlg  class
// it opens the minimal dialog for the CoordConverter beeing used
template <class GRIDAXISDRAW, class COORDCONVERTER>
class GridAxisPropertiesDlg : public WithGridAxisPropertiesBaseLayout<ElementPropertiesDlg<GRIDAXISDRAW> > {
	public:
	COORDCONVERTER* converter;
	CtrlRetriever r2;
	double gMin;
	double gMax;
	
	public:
	typedef GridAxisPropertiesDlg<GRIDAXISDRAW, COORDCONVERTER>  CLASSNAME;
	typedef WithGridAxisPropertiesBaseLayout<ElementPropertiesDlg<GRIDAXISDRAW> > _B;
	
	GridAxisPropertiesDlg() : converter(0) {
		SetLayout_GridAxisPropertiesBaseLayout(*this, true);
		 Size sz = _B::GetLayoutSize();
		 _B::SetMinSize(sz);
		 _B::SetRect(sz);
	}
	
	virtual ~GridAxisPropertiesDlg() {}

	void InitDlg(GRIDAXISDRAW& gda, COORDCONVERTER& conv) {
		_B::InitDlg(gda);
		converter = &conv;
		gMin = converter->getGraphMin();
		gMax = converter->getGraphMax();

		//scaleType.Hide();

		r2( _B::axisLineColor, _B::elem->_axisColor)
		  ( _B::axisTextColor, _B::elem->_axisTextColor)
		  ( _B::axisTickColor, _B::elem->_axisTickColor)
		  ( _B::axisWidth,     _B::elem->_axisWidth)
		  ( _B::gridColor,     _B::elem->_gridColor)
		  ( _B::min, gMin)
		  ( _B::max, gMax)
		  ;
	}
		
	virtual void Retrieve() {
		r2.Retrieve();
		converter->updateGraphSize(gMin, gMax);
		_B::Retrieve();
	}
};

// specialized class  for   GenericCoordinateConverter
template <class GRIDAXISDRAW>
class GridAxisPropertiesDlg<GRIDAXISDRAW, GraphDraw_ns::GenericCoordinateConverter> : public WithGenericGridAxisPropertiesLayout<ElementPropertiesDlg<GRIDAXISDRAW> > {
	public:
	int iscaleType;
	typedef GraphDraw_ns::GenericCoordinateConverter COORDCONVERTER;
	COORDCONVERTER* converter;
	CtrlRetriever r2;
	double gMin;
	double gMax;
	
	public:
	typedef GridAxisPropertiesDlg<GRIDAXISDRAW, GraphDraw_ns::GenericCoordinateConverter>  CLASSNAME;
	typedef WithGenericGridAxisPropertiesLayout<ElementPropertiesDlg<GRIDAXISDRAW> > _B;
	
	GridAxisPropertiesDlg() : converter(0) {
		SetLayout_GenericGridAxisPropertiesLayout(*this, true);
		 Size sz = _B::GetLayoutSize();
		 _B::SetMinSize(sz);
		 _B::SetRect(sz);
	}
	
	virtual ~GridAxisPropertiesDlg() {}

	void InitDlg(GRIDAXISDRAW& gda, COORDCONVERTER& conv) {
		_B::InitDlg(gda);
		converter = &conv;
		gMin = converter->getGraphMin();
		gMax = converter->getGraphMax();
		iscaleType = converter->GetScaleType();
		

		_B::scaleType.Add(GraphDraw_ns::GenericCoordinateConverter::AXIS_SCALE_STD,   t_("Std") );
		_B::scaleType.Add(GraphDraw_ns::GenericCoordinateConverter::AXIS_SCALE_LOG,   t_("Log") );
		_B::scaleType.Add(GraphDraw_ns::GenericCoordinateConverter::AXIS_SCALE_POW10, t_("Pow10") );
		if ( gMin <= 0) {
			_B::scaleType.DisableValue( GraphDraw_ns::GenericCoordinateConverter::AXIS_SCALE_LOG);
		}

		r2( _B::axisLineColor, _B::elem->_axisColor)
		  ( _B::axisTextColor, _B::elem->_axisTextColor)
		  ( _B::axisTickColor, _B::elem->_axisTickColor)
		  ( _B::axisWidth,     _B::elem->_axisWidth)
		  ( _B::gridColor,     _B::elem->_gridColor)
		  ( _B::min, gMin)
		  ( _B::max, gMax)
		  ( _B::scaleType, iscaleType)
		  ;
	}
		
	virtual void Retrieve() {
		r2.Retrieve();
		converter->updateGraphSize(gMin, gMax);
		converter->SetScaleType(iscaleType);
		_B::Retrieve();
	}
};

#endif
