#ifndef _GraphDraw_test_GraphDraw_test_h
#define _GraphDraw_test_GraphDraw_test_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <GraphCtrl/GraphCtrl.h>

// define custom graph type
typedef StdGraphCtrl< GraphCtrlDefaultTypes >  MyGraphCtrl;


#define LAYOUTFILE <GraphDraw_test/GraphDraw_test.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <GraphDraw_test/GraphDraw_test.iml>
#include <Draw/iml_header.h>



class GraphDraw_test : public WithGraphDraw_testLayout<TopWindow> {
public:
	typedef GraphDraw_test CLASSNAME;

	Vector<Pointf> points;
	Vector<Pointf> points2;
	Vector<Pointf> s1;
	Vector<Pointf> s2;
	MyGraphCtrl::Types::TypeCoordConverter y2CoordConverter;
	GraphDraw_test();
	
};

#endif
