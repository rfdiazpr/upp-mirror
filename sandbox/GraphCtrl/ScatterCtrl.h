/*
 * ScatterCtrl.h
 *
 *  Created on: 30 avr. 2012
 *      Author: didier
 */

#ifndef SCATTERCTRL_H_
#define SCATTERCTRL_H_

#include <GraphCtrl/GraphCtrl.h>

using namespace GraphDraw_ns;

class ScatterCtrl : public CRTP_StdGraphCtrl< GraphCtrlDefaultTypes, ScatterCtrl >
{
	public:
	typedef GraphCtrlDefaultTypes                        TYPES;
	typedef ScatterCtrl                                  CLASSNAME;
	typedef CRTP_StdGraphCtrl< TYPES, CLASSNAME >        _B;
	typedef StdLegendCtrl<TYPES, LegendElement<TYPES> >  TypeLegend;
	typedef LabelElement                                 TypeLabel;

	TypeLegend&  _legend;
	TypeLabel&   _title;
	TypeLabel*   _xLabel;
	TypeLabel*   _yLabel;
	TypeLabel*   _y2Label;

	GraphCtrlDefaultTypes::TypeCoordConverter y2CoordConverter;
	GraphCtrlDefaultTypes::TypeGridAxisDraw   y2GridAxisDraw;

	double xRange;
	double yRange;
	double y2Range;

	ScatterCtrl()
	: _legend( CreateLegendElement<TypeLegend, TOP_OF_GRAPH>( 20 ) )
	, _title( CreateElement<TypeLabel, TOP_OF_GRAPH>( 50 )
	          .SetLabel("This is the title")
	          .SetFont(StdFont().Bold().Underline().Height(30))
	          .SetTextColor( Green()) )
	, _xLabel(0)
	, _yLabel(0)
	, _y2Label(0)
	, y2GridAxisDraw(y2CoordConverter)
	, xRange(10)
	, yRange(10)
	, y2Range(10)
	{
		SetMode( MD_ANTIALIASED);
		SetPlotBackgroundColor(Null);
		y2GridAxisDraw._gridColor = Null;
		SetDefaultStylesSeriesAdd();
	}
	
	virtual ~ScatterCtrl() {};



	ScatterCtrl& SetRange(double rx, double ry, double ry2) {
		xRange = rx;
		yRange = ry;
		y2Range = ry2;
		return *this;
	};

	ScatterCtrl& SetRange(double rx, double ry) {
		xRange = rx;
		yRange = ry;
		return *this;
	};
	double GetXRange()const {return _B::_xConverter.getGraphRange();}
	double GetYRange()const {return _B::_yConverter.getGraphRange();}
	double GetY2Range()const {return 0;}
	ScatterCtrl &SetMajorUnits(double ux, double uy) {return *this;};
	ScatterCtrl &SetMajorUnitsNum(int nx, int ny) {return *this;};
	ScatterCtrl &SetMaxMajorUnitsX(int maxX, int maxY)  {return *this;};
	double GetMajorUnitsX() {return 0;}
	double GetMajorUnitsY() {return 0;}
	ScatterCtrl& SetMinUnits(double ux, double uy) {return *this;};
	double GetXMinUnit () const {return 0;}
	double GetYMinUnit () const {return 0;}

	ScatterCtrl& SetXYMin(double xmin,double ymin) {
		_B::_xConverter.updateGraphSize(xmin, xmin+xRange);
		_B::_yConverter.updateGraphSize(ymin, ymin+yRange);
		return *this;
	}

	ScatterCtrl& SetXYMin(double xmin,double ymin,double ymin2) {
		_B::_xConverter.updateGraphSize(xmin, xmin+xRange);
		_B::_yConverter.updateGraphSize(ymin, ymin+yRange);
		_B::AddRightElementAt(0, 50, y2GridAxisDraw );
		y2CoordConverter.updateGraphSize(ymin2, ymin2+y2Range);
		AddYConverter(y2CoordConverter);
		_B::_currentYConverter = &_B::GetYCoordConverter();
		return *this;
	}

	double GetXMin () const {return 0;}
	double GetYMin () const {return 0;}
	double GetYMin2 () const {return 0;}

	enum Formats {
		EXP,
		MON,
		DY,
		CUSTOM
	};

	ScatterCtrl &Graduation_FormatX(Formats fi) {return *this;}
	ScatterCtrl &Graduation_FormatY(Formats fi) {return *this;}
	ScatterCtrl &Graduation_FormatY2(Formats fi) {return *this;}

	Callback3<String&, int, double> cbModifFormatX, cbModifFormatDeltaX;
	Callback3<String&, int, double> cbModifFormatY, cbModifFormatDeltaY;
	Callback3<String&, int, double> cbModifFormatY2, cbModifFormatDeltaY2;

	ScatterCtrl &SetDataPrimaryY(const int& j, const bool& primary=true) {
		if (j < (series.GetCount()-1)) {
			if (primary) series[j].yConverter = &_B::GetYCoordConverter();
			else         series[j].yConverter = &y2CoordConverter;
		}
		return *this;
	};
	ScatterCtrl &SetDataPrimaryY(const bool& primary) {
		if (series.GetCount() > 0) {
			if (primary) series[series.GetCount()-1].yConverter = &_B::GetYCoordConverter();
			else         series[series.GetCount()-1].yConverter = &y2CoordConverter;
		}
		return *this;
	}

	ScatterCtrl& SetDrawXReticle(bool set = true) {return *this;}
	ScatterCtrl& SetDrawYReticle(bool set = true) {return *this;}
	ScatterCtrl& SetDrawY2Reticle(bool set = true) {return *this;}


	enum MouseAction {NO_ACTION = 0, SCROLL, ZOOM_H_ENL, ZOOM_H_RED, ZOOM_V_ENL, ZOOM_V_RED, SHOW_INFO, CONTEXT_MENU};

	struct MouseBehaviour {
		bool ctrl;
		bool alt;
		bool shift;
		bool left;
		bool middle;
		int  middleWheel;
		bool right;
		MouseAction action;
	};

	#define MAX_MOUSEBEHAVIOR 20

	bool SetMouseBehavior(MouseBehaviour *_mouseBehavior) {return true; };
	ScatterCtrl& ShowLegend(const bool& show=true) {return *this;}
	ScatterCtrl& ShowContextMenu(const bool& show = true)  {return *this;}
	void SetPopText(const String x, const String y1, const String y2)  {}
	ScatterCtrl& SetMouseHandling(bool valx = true, bool valy = false) {return *this;}
	ScatterCtrl &ShowInfo(bool show = true) {return *this;}
//
#ifdef PLATFORM_WIN32
	void SaveAsMetafile(const char* file) {};
#endif
	void SaveToClipboard(bool saveAsMetafile = false){};
	void SaveToFile(String fileName = Null) {};

	//

	ScatterCtrl& LogX(const bool& logx=true)  { GetXCoordConverter().SetConvLog(); GetXGridAxisDraw().setAxisLogFormat(); return *this;}
	ScatterCtrl& LogY(const bool& logy=true)  { GetYCoordConverter().SetConvLog(); GetYGridAxisDraw().setAxisLogFormat(); return *this;}
	ScatterCtrl& LogY2(const bool& logy=true) { y2CoordConverter.SetConvLog() ;    y2GridAxisDraw.setAxisLogFormat();     return *this;}
//	ScatterCtrl& LogX(const bool& logx=true)  { GetXCoordConverter().SetConvLog(); return *this;}
//	ScatterCtrl& LogY(const bool& logy=true)  { GetYCoordConverter().SetConvLog(); return *this;}
//	ScatterCtrl& LogY2(const bool& logy=true) { y2CoordConverter.SetConvLog() ;    return *this;}

	ScatterCtrl& SetMinZoom(double x, double y = -1) {return *this;}
	ScatterCtrl& SetMaxZoom(double x, double y = -1) {return *this;}

	ScatterCtrl& SetFastViewX(bool set = true) {return *this;}
	ScatterCtrl& SetSequentialXAll(const bool& sequential)  {return *this;}
//	virtual void Refresh() {Ctrl::Refresh();};
//	virtual Size GetSize() const {return Ctrl::GetSize();};
//
	ScatterCtrl& SetColor(const class::Color& _color)			   {return *this;}
	ScatterCtrl& SetGridColor(const class::Color& grid_color)	{
		GetXGridAxisDraw()._gridColor = grid_color;
		GetYGridAxisDraw()._gridColor = grid_color;
		return *this;
	}
	ScatterCtrl& SetGridWidth(const int& grid_width) 			   {
		return *this;
	}

	ScatterCtrl& SetPlotAreaColor(const class::Color& p_a_color){
		SetPlotBackgroundColor(p_a_color);
		return *this;
	}

	ScatterCtrl& SetLegendWeight(const int& weight)				   {return *this;}
	ScatterCtrl& SetAxisColor(const class::Color& axis_color)	{
		GetXGridAxisDraw()._axisColor = axis_color;
		GetYGridAxisDraw()._axisColor = axis_color;
		y2GridAxisDraw._axisColor = axis_color;
		return *this;
	}
	ScatterCtrl& SetAxisWidth(const int& axis_width)			   {
		GetXGridAxisDraw()._axisWidth = axis_width;
		GetYGridAxisDraw()._axisWidth = axis_width;
		y2GridAxisDraw._axisWidth = axis_width;
		return *this;
	}
	ScatterCtrl& SetTitle(const String& title)		 			   {
		_title.SetLabel(title); return *this;
	}
	ScatterCtrl& SetTitleFont(const Font& fontTitle) 			   { _title.SetFont(fontTitle); return *this;}
	ScatterCtrl& SetTitleColor(const class::Color& colorTitle)	{ _title.SetTextColor(colorTitle); return *this;}
	ScatterCtrl& SetLabelsFont(const Font& fontLabels) 			{
		if (_xLabel)  _xLabel->SetFont(fontLabels);
		if (_yLabel)  _yLabel->SetFont(fontLabels);
		if (_y2Label) _y2Label->SetFont(fontLabels);
		return *this;
	}

	ScatterCtrl& SetLabelsColor(const class::Color& colorLabels){
		if (_xLabel) _xLabel->SetTextColor(colorLabels);
		if (_yLabel) _yLabel->SetTextColor(colorLabels);
		if (_y2Label) _y2Label->SetTextColor(colorLabels);
		return *this;
	}

	ScatterCtrl& SetLabelX(const String& xLabel) {
		if (_xLabel==0) _xLabel = &CreateElement<TypeLabel, BOTTOM_OF_GRAPH>( 18 ).SetLabel(xLabel).SetFont(StdFont().Bold()).SetTextColor(Green());
		else            _xLabel->SetLabel(xLabel);
		return *this;
	}

	ScatterCtrl& SetLabelY(const String& yLabel) {
		if (_yLabel==0) _yLabel = &CreateElement<TypeLabel, LEFT_OF_GRAPH>( 18 ).SetLabel(yLabel).SetFont(StdFont().Bold()).SetTextColor(Green());
		else            _yLabel->SetLabel(yLabel);
		return *this;
	}

	ScatterCtrl& SetLabelY2(const String& yLabel)				   {
		if (_y2Label==0) _y2Label = &CreateElement<TypeLabel, RIGHT_OF_GRAPH>( 18 ).SetLabel(yLabel).SetFont(StdFont().Bold()).SetTextColor(Green());
		else             _y2Label->SetLabel(yLabel);
		return *this;
	}
	ScatterCtrl& SetPlotAreaMargin(const int hLeft, const int hRight, const int vTop, const int vBottom)
																               {return *this;}
	ScatterCtrl& SetPlotAreaLeftMargin(const int margin)		   { _yGridDraw.SetElementWidth(margin); updateSizes(); return *this;}
	ScatterCtrl& SetPlotAreaTopMargin(const int margin)			{return *this;}
	ScatterCtrl& SetPlotAreaRightMargin(const int margin)		   {return *this;}
	ScatterCtrl& SetPlotAreaBottomMargin(const int margin)		{ _xGridDraw.SetElementWidth(margin); updateSizes(); return *this;}
};

#endif /* SCATTERCTRL_H_ */
