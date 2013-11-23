#ifndef _GraphCtrl_GrapCtrl_h
#define _GraphCtrl_GrapCtrl_h

#include <CtrlLib/CtrlLib.h>
#include <GraphDraw/GraphDraw.h>
#include <DropLineDash/DropLineDash.h>

using namespace Upp;


#define LAYOUTFILE <GraphCtrl/GraphCtrl.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS GraphCtrlImg
#define IMAGEFILE <GraphCtrl/GraphCtrl.iml>
#include <Draw/iml_header.h>

#include <plugin/jpg/jpg.h>
#include <plugin/tif/tif.h>


// ===============================================================================================================================
// ===============================================================================================================================

#include <GraphCtrl/GraphCtrlBase.h>
#include <GraphCtrl/StdElementPropertiesDialogs.h>
#include <GraphCtrl/ElementCtrlBase.h>
#include <GraphCtrl/StdElementCtrls.h>

// ===============================================================================================================================
// ===============================================================================================================================
struct GraphCtrlDefaultTypes {
		typedef GraphDraw_ns::GenericCoordinateConverter                                    TypeCoordConverter;
		
		typedef GraphDraw_ns::GenericCoordinateConverter                                       X_TypeCoordConverter;
		typedef GraphDraw_ns::GenericCoordinateConverter                                       X2_TypeCoordConverter;
		typedef GraphDraw_ns::GenericCoordinateConverter                                       Y_TypeCoordConverter;
		typedef GraphDraw_ns::GenericCoordinateConverter                                       Y2_TypeCoordConverter;
		typedef StdGridAxisDrawCtrl<GraphCtrlDefaultTypes, GraphDraw_ns::GridAxisDraw<GraphCtrlDefaultTypes> > X_TypeGridAxisDraw;
		typedef StdGridAxisDrawCtrl<GraphCtrlDefaultTypes, GraphDraw_ns::GridAxisDraw<GraphCtrlDefaultTypes> > X2_TypeGridAxisDraw;
		typedef StdGridAxisDrawCtrl<GraphCtrlDefaultTypes, GraphDraw_ns::GridAxisDraw<GraphCtrlDefaultTypes> > Y_TypeGridAxisDraw;
		typedef StdGridAxisDrawCtrl<GraphCtrlDefaultTypes, GraphDraw_ns::GridAxisDraw<GraphCtrlDefaultTypes> > Y2_TypeGridAxisDraw;
		typedef GraphDraw_ns::GridStepManager<>                                                TypeGridStepManager;
		typedef StdLabelCtrl<GraphCtrlDefaultTypes,GraphDraw_ns::LabelElement>                 TypeLabelElement;
		typedef StdLegendCtrl<GraphCtrlDefaultTypes, GraphDraw_ns::LegendElement<GraphCtrlDefaultTypes> > TypeLegendElement;
};

class XYY2_GraphCtrl   : public CRTP_GraphCtrl_Base< GraphCtrlDefaultTypes, XYY2_GraphCtrl, GraphDraw_ns::CRTP_XYY2GraphDraw>
{
	public:
	typedef XYY2_GraphCtrl  CLASSNAME;
	typedef CRTP_GraphCtrl_Base< GraphCtrlDefaultTypes, XYY2_GraphCtrl, GraphDraw_ns::CRTP_XYY2GraphDraw> _B;
	typedef GraphCtrlDefaultTypes  Types;
};

class XY_GraphCtrl   : public CRTP_GraphCtrl_Base< GraphCtrlDefaultTypes, XY_GraphCtrl, GraphDraw_ns::CRTP_XYGraphDraw>
{
	public:
	typedef XY_GraphCtrl  CLASSNAME;
	typedef CRTP_GraphCtrl_Base< GraphCtrlDefaultTypes, XY_GraphCtrl, GraphDraw_ns::CRTP_XYGraphDraw> _B;
	typedef GraphCtrlDefaultTypes  Types;
};


class XYLT_GraphCtrl : public CRTP_GraphCtrl_Base< GraphCtrlDefaultTypes, XYLT_GraphCtrl, GraphDraw_ns::CRTP_XYLTGraphDraw>
{
	public:
	typedef XYLT_GraphCtrl  CLASSNAME;
	typedef CRTP_GraphCtrl_Base< GraphCtrlDefaultTypes, XYLT_GraphCtrl, GraphDraw_ns::CRTP_XYLTGraphDraw> _B;
	typedef GraphCtrlDefaultTypes  Types;
};

class XYY2LT_GraphCtrl : public CRTP_GraphCtrl_Base< GraphCtrlDefaultTypes, XYY2LT_GraphCtrl, GraphDraw_ns::CRTP_XYY2LTGraphDraw>
{
	public:
	typedef XYY2LT_GraphCtrl  CLASSNAME;
	typedef CRTP_GraphCtrl_Base< GraphCtrlDefaultTypes, XYY2LT_GraphCtrl, GraphDraw_ns::CRTP_XYY2LTGraphDraw> _B;
	typedef GraphCtrlDefaultTypes  Types;
};


#endif
