#include "GraphCtrl.h"


#define LAYOUTFILE <GraphCtrl/GraphCtrl.lay>
#include <CtrlCore/lay.h>


#define IMAGECLASS GraphCtrlImg
#define IMAGEFILE <GraphCtrl/GraphCtrl.iml>
#include <Draw/iml_source.h>


namespace GraphCtrl_ns
{
	void OpenGridAxisPropertiesCtrl( GraphCtrlDefaultTypes::TypeGridAxisDraw* v)
	{

		GraphCtrlDefaultTypes::TypeCoordConverter& converter = v->GetCoordConverter();
		CtrlRetriever r;
		double gMin = converter.getGraphMin();
		double gMax = converter.getGraphMax();
		int majorTickLength = v->GetMajorTickLength();
		int minorTickLength = v->GetMinorTickLength();
		int scaleType = v->GetCoordConverter().GetScaleType();

		WithGridAxisPropertiesLayout<TopWindow> dlg;

		dlg.scaleType.Add(GraphDraw_ns::GenericCoordinateConverter::AXIS_SCALE_STD,   t_("Std") );
		dlg.scaleType.Add(GraphDraw_ns::GenericCoordinateConverter::AXIS_SCALE_LOG,   t_("Log") );
		dlg.scaleType.Add(GraphDraw_ns::GenericCoordinateConverter::AXIS_SCALE_POW10, t_("Pow10") );
		if ( gMin < 0) {
			dlg.scaleType.DisableValue( GraphDraw_ns::GenericCoordinateConverter::AXIS_SCALE_LOG);
		}

		r ( dlg.axisLineColor, v->_axisColor)
		  ( dlg.axisTextColor, v->_axisTextColor)
		  ( dlg.axisTickColor, v->_axisTickColor)
		  ( dlg.axisMajorTickLength, majorTickLength)
		  ( dlg.axisMinorTickLength, minorTickLength)
		  ( dlg.gridColor, v->_gridColor)
		  ( dlg.min, gMin)
		  ( dlg.max, gMax)
		  ( dlg.scaleType, scaleType)
		  ;



		CtrlLayoutOKCancel(dlg, "");

		if ( dlg.Execute() == IDOK ){
			r.Retrieve();
			v->SetMajorTickLength(majorTickLength);
			v->SetMinorTickLength(majorTickLength);
			v->GetCoordConverter().SetScaleType(scaleType);
			converter.updateGraphSize(gMin, gMax);

		}
	}




};




