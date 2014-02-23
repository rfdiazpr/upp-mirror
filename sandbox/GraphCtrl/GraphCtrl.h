#ifndef _GraphCtrl_GrapCtrl_h
#define _GraphCtrl_GrapCtrl_h


#include <DropLineDash/DropLineDash.h>

#include <CtrlLib/CtrlLib.h>


#define LAYOUTFILE <GraphCtrl/GraphCtrl.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS GraphCtrlImg
#define IMAGEFILE <GraphCtrl/GraphCtrl.iml>
#include <Draw/iml_header.h>

#include <GraphDraw/GraphDraw.h>

NAMESPACE_UPP
namespace GraphDraw_ns {

// ===============================================================================================================================
// ===============================================================================================================================

#include <GraphCtrl/GraphCtrlBase.h>
#include <GraphCtrl/StdElementPropertiesDialogs.h>
#include <GraphCtrl/ElementCtrlBase.h>
#include <GraphCtrl/StdElementCtrls.h>
};





// ===============================================================================================================================
// ===============================================================================================================================
struct GraphCtrlDefaultTypes {
		typedef GraphDraw_ns::GenericCoordinateConverter                                                                     TypeCoordConverter;
		typedef GraphDraw_ns::GenericCoordinateConverter                                                                     X_TypeCoordConverter;
		typedef GraphDraw_ns::GenericCoordinateConverter                                                                     X2_TypeCoordConverter;
		typedef GraphDraw_ns::GenericCoordinateConverter                                                                     Y_TypeCoordConverter;
		typedef GraphDraw_ns::GenericCoordinateConverter                                                                     Y2_TypeCoordConverter;
		typedef GraphDraw_ns::StdGridAxisDrawCtrl<GraphCtrlDefaultTypes, GraphDraw_ns::GridAxisDraw<GraphCtrlDefaultTypes> > X_TypeGridAxisDraw;
		typedef GraphDraw_ns::StdGridAxisDrawCtrl<GraphCtrlDefaultTypes, GraphDraw_ns::GridAxisDraw<GraphCtrlDefaultTypes> > X2_TypeGridAxisDraw;
		typedef GraphDraw_ns::StdGridAxisDrawCtrl<GraphCtrlDefaultTypes, GraphDraw_ns::GridAxisDraw<GraphCtrlDefaultTypes> > Y_TypeGridAxisDraw;
		typedef GraphDraw_ns::StdGridAxisDrawCtrl<GraphCtrlDefaultTypes, GraphDraw_ns::GridAxisDraw<GraphCtrlDefaultTypes> > Y2_TypeGridAxisDraw;
//		typedef GraphDraw_ns::GridStepManager<>                                                                              TypeGridStepManager;
		typedef GraphDraw_ns::GridStepManager                                                                                TypeGridStepManager;
		typedef GraphDraw_ns::StdLabelCtrl<GraphCtrlDefaultTypes,GraphDraw_ns::LabelElement>                                 TypeLabelElement;
		typedef GraphDraw_ns::StdLegendCtrl<GraphCtrlDefaultTypes, GraphDraw_ns::LegendElement<GraphCtrlDefaultTypes> >      TypeLegendElement;
		typedef GraphDraw_ns::StdBlankAreaCtrl<GraphCtrlDefaultTypes, GraphDraw_ns::BlankAreaElement >                       TypeBlankElement;
};

class XYY2_GraphCtrl   : public GraphDraw_ns::CRTP_GraphCtrl_Base< GraphCtrlDefaultTypes, XYY2_GraphCtrl, GraphDraw_ns::CRTP_XYY2GraphDraw<GraphCtrlDefaultTypes, XYY2_GraphCtrl> >
{
	public:
	typedef XYY2_GraphCtrl  CLASSNAME;
	typedef GraphDraw_ns::CRTP_GraphCtrl_Base< GraphCtrlDefaultTypes, XYY2_GraphCtrl, GraphDraw_ns::CRTP_XYY2GraphDraw<GraphCtrlDefaultTypes, XYY2_GraphCtrl> > _B;
	typedef GraphCtrlDefaultTypes  Types;
};

class XY_GraphCtrl   : public GraphDraw_ns::CRTP_GraphCtrl_Base< GraphCtrlDefaultTypes, XY_GraphCtrl, GraphDraw_ns::CRTP_XYGraphDraw<GraphCtrlDefaultTypes, XY_GraphCtrl> >
{
	public:
	typedef XY_GraphCtrl  CLASSNAME;
	typedef GraphDraw_ns::CRTP_GraphCtrl_Base< GraphCtrlDefaultTypes, XY_GraphCtrl, GraphDraw_ns::CRTP_XYGraphDraw<GraphCtrlDefaultTypes, XY_GraphCtrl> > _B;
	typedef GraphCtrlDefaultTypes  Types;
};


class XYLT_GraphCtrl : public GraphDraw_ns::CRTP_GraphCtrl_Base< GraphCtrlDefaultTypes, XYLT_GraphCtrl, GraphDraw_ns::CRTP_XYLTGraphDraw<GraphCtrlDefaultTypes, XYLT_GraphCtrl> >
{
	public:
	typedef XYLT_GraphCtrl  CLASSNAME;
	typedef GraphDraw_ns::CRTP_GraphCtrl_Base< GraphCtrlDefaultTypes, XYLT_GraphCtrl, GraphDraw_ns::CRTP_XYLTGraphDraw<GraphCtrlDefaultTypes, XYLT_GraphCtrl> > _B;
	typedef GraphCtrlDefaultTypes  Types;
};

class XYY2LT_GraphCtrl : public GraphDraw_ns::CRTP_GraphCtrl_Base< GraphCtrlDefaultTypes, XYY2LT_GraphCtrl, GraphDraw_ns::CRTP_XYY2LTGraphDraw<GraphCtrlDefaultTypes, XYY2LT_GraphCtrl> >
{
	public:
	typedef XYY2LT_GraphCtrl  CLASSNAME;
	typedef GraphDraw_ns::CRTP_GraphCtrl_Base< GraphCtrlDefaultTypes, XYY2LT_GraphCtrl, GraphDraw_ns::CRTP_XYY2LTGraphDraw<GraphCtrlDefaultTypes, XYY2LT_GraphCtrl> > _B;
	typedef GraphCtrlDefaultTypes  Types;
};
END_UPP_NAMESPACE
#endif
