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

		WithGridAxisPropertiesLayout<TopWindow> dlg;
		r ( dlg.axisLineColor, v->_axisColor)
		  ( dlg.axisTextcolor, v->_axisTextColor)
		  ( dlg.gridColor, v->_gridColor)
		  ( dlg.min, gMin)
		  ( dlg.max, gMax)
		  ;
		CtrlLayoutOKCancel(dlg, "");

		if ( dlg.Execute() == IDOK ){
			r.Retrieve();
			converter.updateGraphSize(gMin, gMax);
		}
	}




};




