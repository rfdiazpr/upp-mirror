#ifndef _GraphDraw_test_GraphDraw_test_h
#define _GraphDraw_test_GraphDraw_test_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <GraphCtrl/GraphCtrl.h>


#define LAYOUTFILE <GraphCtrl_test/GraphCtrl_test.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS GraphCtrl_testImg
#define IMAGEFILE <GraphCtrl_test/GraphCtrl_test.iml>
#include <Draw/iml_header.h>

typedef XYY2LT_GraphCtrl MyGraphCtrl;

class GraphDraw_test : public WithGraphDraw_testLayout<TopWindow> {
public:
	typedef GraphDraw_test CLASSNAME;

	Vector<Pointf> points;
	Vector<Pointf> points2;
	Vector<Pointf> s1;
	Vector<Pointf> s2;
	XYY2LT_GraphCtrl::Types::TypeCoordConverter y2CoordConverter;
	GraphDraw_test();
	
	void FormatAsTime(double value, String& output, double range);

	void DoReport(void);
};

#endif
