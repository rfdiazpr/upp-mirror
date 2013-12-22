#ifndef _GraphDraw_test_GraphDraw_test_h
#define _GraphDraw_test_GraphDraw_test_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <GraphCtrl/GraphCtrl.h>
#include <Functions4U/Functions4U.h>



#define LAYOUTFILE <GraphCtrl_test/GraphCtrl_test.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS GraphCtrl_testImg
#define IMAGEFILE <GraphCtrl_test/GraphCtrl_test.iml>
#include <Draw/iml_header.h>

typedef XYY2LT_GraphCtrl MyGraphCtrl;

typedef DynamicMarkerCtrl< MyGraphCtrl::Types, GraphDraw_ns::MarkerElement< MyGraphCtrl::Types > >  MarkerElementType;

class GraphDraw_test : public WithGraphDraw_testLayout<TopWindow> {
public:
	typedef GraphDraw_test CLASSNAME;

	Vector<Pointf> points;
	Vector<Pointf> points2;
	Vector<Pointf> s1;
	Vector<Pointf> s2;
	//XYY2LT_GraphCtrl::Types::TypeCoordConverter y2CoordConverter;
	GraphDraw_test();
	
	void FormatAsTime(double value, String& output, double range);
	
//	typedef ArrayMap<int, MarkerElementData> MarkerPosList;
//	typedef Callback2< const MarkerPosList&, int> TypeMarkerMoveCbk;
	void onMarkerMoveCBK( const GraphDraw_ns::MarkerPosList& markers, int ID);

	void DoReport(void);
};

#endif
