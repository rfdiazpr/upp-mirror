


//#define TRACE_INFO(TXT) { std::ostringstream str; str <<  "\n" << TXT; LOG(str.str().c_str()); }
#define TRACE_INFO(TXT) std::cout <<  "\n" << TXT << std::flush
#define TRACE_ERROR(TXT) std::cout <<  "\n ### ERROR : " << TXT << std::flush


#include "GraphDraw_test.h"


using namespace GraphDraw_ns;




template<class TYPES>
class ExclusionAreaDraw : public CRTPGraphElementFrame< ExclusionAreaDraw<TYPES> >
{
	private:
	typename TYPES::TypeCoordConverter& _xConverter;
	typename TYPES::TypeCoordConverter& _yConverter;
	typedef  CRTPGraphElementFrame< ExclusionAreaDraw<TYPES> > _B;

	public:
	typedef ExclusionAreaDraw<TYPES>   CLASSNAME;
	ExclusionAreaDraw(typename TYPES::TypeCoordConverter& xConv, typename TYPES::TypeCoordConverter& yConv) :  _yConverter(yConv), _xConverter(xConv){};
	~ExclusionAreaDraw() {};

	virtual CLASSNAME* Clone() { return new CLASSNAME(_xConverter, _yConverter); }


	virtual void PaintElement(Draw& dw, int scale) { /* do noting */}
	virtual void PaintOnPlot(Draw& dw, int otherWidth, int scale)
	{
		int xMin = _xConverter.getScreenMin();
		int xMax = _xConverter.getScreenMax();
		int yMin = _yConverter.getScreenMin();
		int yMax = _yConverter.getScreenMax();
		int zXMin = _xConverter.toScreen(5);
		int zXMax = _xConverter.toScreen(7);
		int zYMin = _yConverter.toScreen(4);
		int zYMax = _yConverter.toScreen(8);


		Vector<Point> zone1;
		Vector<Point> zone2;
		zone1 << Point(xMin, zYMin) << Point(zXMin, zYMin) << Point(zXMin, yMax) << Point(xMin, yMax);
		zone2 << Point(xMax, zYMax) << Point(zXMax, zYMax) << Point(zXMax, yMin) << Point(xMax, yMin);
		dw.DrawPolygon(zone1, Color(148, 235, 242), 2, Black());
		dw.DrawPolygon(zone2, Color(250, 150, 150), 2, Black());
	}
};



double funct1(double x)	{return (sin(x/1000.0)*10.0);}
void   vfunct1(double& y, double x) {y = -x*x;}

GraphDraw_test::GraphDraw_test()
{
	
	CtrlLayout(*this, "Window title");

	points << Pointf(0,0) << Pointf(1, 3.5)<< Pointf(2,1.8)<< Pointf(4, 3)<< Pointf(5.5, 2.5);
	points << Pointf(6,7) << Pointf(7, 9)<< Pointf(8,10)<< Pointf(10, 15)<< Pointf(15, 15);

	points2 << Pointf(0,0) << Pointf(1, 2)<< Pointf(2,3)<< Pointf(4, 3)<< Pointf(5.5, 4);
	points2 << Pointf(6,5) << Pointf(7, 6)<< Pointf(8,10)<< Pointf(10, 9)<< Pointf(15, 8);


	// ================ G1 ================
	g1.AddSeries(points).PlotStyle<StaggeredSeriesPlot>();
	g1.CloneRightElement( 20, g1.GetYGridAxisDraw());
	g1.setGraphSize(0,20,0,20);
	g1.SetMode( GraphDraw_ns::MD_DRAW );



	// ================ G3 ================
	double y;
	for (int t = 0; t < 100000; ++t) {
		y = 20.0*sin(2.0*M_PI*t/100000.0);
		// create a "cloud" of points around the main line
		s1 <<Pointf(t,y);
		s2 <<Pointf(t,y + (random()*5.0)/RAND_MAX-2.5);
	}
	g3.CloneTopElement( 20, g3.GetXGridAxisDraw());
	g3.setGraphSize(0, 100000, -22, +22);
	g3.CreateLegendElement<LegendElement<MyGraphCtrl::Types>, TOP_OF_GRAPH>( 20 );//.SetBackGndColor( Color(198, 255, 0) );
	g3.CreateTopElement<GraphDraw_ns::LabelElement>( 30 )
			.SetLabel("BIG DATA SET")
			.SetFont(StdFont().Bold().Underline().Height(20))
			.SetTextColor(Green());
	g3.CreateRightElement<GraphDraw_ns::BlankAreaElement>( 15 );
	g3.SetMode( GraphDraw_ns::MD_ANTIALIASED);
	g3.AddSeries(s1).PlotStyle<LineSeriesPlot>().Legend("S1"); //.MarkStyle<TriangleMarkPlot>();
	g3.AddSeries(s2).MarkStyle<CircleMarkPlot>().Legend("S2").SetMarkWidth(1).SetSequential().SetDataThickness(1); //.MarkStyle<TriangleMarkPlot>();
	g3.AddSeries(&funct1).Legend("sin(x/1000.0)*10.0").MarkStyle<CircleMarkPlot>().SetMarkWidth(1).SetDataThickness(1).SetMarkColor(Black());
//	g3.CreateLegendElement<LegendElement<MyGraphCtrl::Types>, RIGHT_OF_GRAPH>( 90 );//.SetBackGndColor( Color(198, 255, 0) );



	// ================ G4 ================
	g4.CloneTopElement( 20, g4.GetXGridAxisDraw() );
	g4.CreateElement<ExclusionAreaDraw<MyGraphCtrl::Types>, OVER_GRAPH>(0, g4.GetXCoordConverter(), g4.GetYCoordConverter() );
	g4.CreateLegendElement<LegendElement<MyGraphCtrl::Types>, TOP_OF_GRAPH>( 20 );//.SetBackGndColor( Color(198, 255, 0) );
	g4.CreateElement<GraphDraw_ns::LabelElement, TOP_OF_GRAPH>( 50 )
			.SetLabel("This is the title")
			.SetFont(StdFont().Bold().Underline().Height(30))
			.SetTextColor(Green());
	g4.CreateElement<GraphDraw_ns::LabelElement, LEFT_OF_GRAPH>( 18 ).SetLabel("Y axis").SetFont(StdFont().Bold().Underline()).SetTextColor(Green());
	g4.CreateElement<GraphDraw_ns::LabelElement, BOTTOM_OF_GRAPH>( 18 ).SetLabel("X axis").SetFont(StdFont().Bold().Underline()).SetTextColor(Green());
	g4.AddSeries(points).PlotStyle<LineSeriesPlot>().MarkStyle<XMarkPlot>().Legend("S1");
	g4.setGraphSize(0,20,0,20);
	g4.GetYGridAxisDraw().setMajorTickMark( (new LineTickMark())->SetTickLength( 3 ) );

	// ADD Y2 AXIS
	y2CoordConverter.updateGraphSize(0, 20);
	g4.AddYConverter(y2CoordConverter);
	g4.CreateElement<MyGraphCtrl::Types::TypeGridAxisDraw, RIGHT_OF_GRAPH>( 35, y2CoordConverter ).setGridColor(Null).setMajorTickMark((new GraphDraw_ns::TriangleTickMark())->SetTickLength( 8 ));
	g4.CreateElement<GraphDraw_ns::LabelElement, RIGHT_OF_GRAPH>( 18 ).SetLabel("Y2 AXIS").SetFont(StdFont().Bold().Underline()).SetTextColor(Green());

	g4.AddSeries(points2).PlotStyle<LineSeriesPlot>().Legend("S2");; // tied to last X/Y  coordConverters ==> X / Y2
	g4.SetMode( GraphDraw_ns::MD_SUBPIXEL );

//	LegendElement<MyGraphCtrl::Types>& legend = g4.CreateElement<LegendElement<MyGraphCtrl::Types>, OVER_GRAPH>(100)
//   				 .SetLegend("[ [ [*/_R@5$(229)0 This ][*/_`R@6$(229)0 is][*/_R@5$(229)0  ][*/_,R@(255.0.255)$(229)0 the][*/_R@5$(229)0  ][*/_cR@5$(229)0 Title]]");
//	legend.SetFrame( Rect(Point(100,100), Size(350, 30)) );
//	legend.SetBackGndColor( Color(198, 255, 0) );

	g4.CreateLegendElement<LegendElement<MyGraphCtrl::Types>, RIGHT_OF_GRAPH>( 90 );//.SetBackGndColor( Color(198, 255, 0) );
	LegendElement<MyGraphCtrl::Types>& legend = g4.CreateLegendElement<LegendElement<MyGraphCtrl::Types>, OVER_GRAPH>( 120 );
	legend.SetFrame( Rect(Point(100,100), Size(350, 30)) );
	RGBA rgba;
	
	rgba.r=90;
	rgba.g=90;
	rgba.b=0;
	rgba.a=90;
	legend.SetBackGndColor( rgba );
//	legend.SetBackGndColor(  );
}


using namespace GraphDraw_ns;



GUI_APP_MAIN
{
	TRACE_INFO("STARTING application");
	One<GraphDraw_test> app = new GraphDraw_test();
	app->Sizeable().Run();
	TRACE_INFO("ENDING application");
}
