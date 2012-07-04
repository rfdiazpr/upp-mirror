
//#define TRACE_INFO(TXT) { std::ostringstream str; str <<  "\n" << TXT; LOG(str.str().c_str()); }
#define TRACE_INFO(TXT) std::cout <<  "\n" << TXT << std::flush
#define TRACE_ERROR(TXT) std::cout <<  "\n ### ERROR : " << TXT << std::flush

#include <Report/Report.h>

#include <Functions4U/Functions4U.h>

#include "GraphDraw_test.h"


#define IMAGECLASS GraphDraw_testImg
#define IMAGEFILE <GraphDraw_test/GraphDraw_test.iml>
#include <Draw/iml_source.h>


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



void GraphDraw_test::FormatAsTime(double value, String& output, double range) {
	Time time;
	int64 v = value;
	time.Set(v);
	if (time.day == 1  &&  time.month == 1) output = Format("%d",  time.year);

	//output = Format("%d/%d/%d", time.day, time.month, time.year);
}

GraphDraw_test::GraphDraw_test()
{
	
	CtrlLayout(*this, "Window title");

//	static char s_date_format_main[64] = "%2:02d/%3:02d/%1:4d";

	SetDateFormat("%3:02d/%2:02d/%1:4d");

	points << Pointf(0.5,1) << Pointf(1, 3.5)<< Pointf(2,1.8)<< Pointf(4, 3)<< Pointf(5.5, 2.5);
	points << Pointf(6,7) << Pointf(7, 9)<< Pointf(8,10)<< Pointf(10, 15)<< Pointf(15, 15);

	points2 << Pointf(0,0) << Pointf(1, 2)<< Pointf(2,3)<< Pointf(4, 3)<< Pointf(5.5, 4);
	points2 << Pointf(6,5) << Pointf(7, 6)<< Pointf(8,10)<< Pointf(10, 9)<< Pointf(15, 8);


	// ================ G1 ================
	g1.AddSeries(points).PlotStyle<StaggeredSeriesPlot>();
	g1.CloneRightElement( 20, g1.GetYGridAxisDraw());
	g1.setGraphSize(10,20,10,20);
	g1.SetMode( GraphDraw_ns::MD_DRAW );
	g1.GetXCoordConverter().SetConvLog();



	// ================ G3 ================
	double y;
	for (int t = 0; t < 100000; ++t) {
		y = 20.0*sin(2.0*M_PI*t/100000.0);
		// create a "cloud" of points around the main line
		s1 <<Pointf(t,y);
		s2 <<Pointf(t,y + (Upp::Randomf()*5.0)-2.5);
	}
	g3.GetXGridAxisDraw().setAxisTimeFormat();
	{
		Time startTime(2010, 1, 1);
		Time endTime(2013, 1, 1);
		g3.setGraphSize(startTime.Get(), endTime.Get(), -22, +22);
	}

	//	g3.setGraphSize( 0, 100000, -22, +22);
	g3.CreateLegendElement<StdLegendCtrl<MyGraphCtrl::Types, LegendElement<MyGraphCtrl::Types> >, TOP_OF_GRAPH>( 20 );//.SetBackGndColor( Color(198, 255, 0) );
	g3.CreateTopElement<GraphDraw_ns::LabelElement>( 30 )
			.SetLabel("BIG DATA SET")
			.SetFont(StdFont().Bold().Underline().Height(20))
			.SetTextColor(Green());
	g3.CreateRightElement<GraphDraw_ns::BlankAreaElement>( 15 );
	g3.SetMode( GraphDraw_ns::MD_ANTIALIASED);
//	g3.AddSeries(s1).PlotStyle<LineSeriesPlot>().NoMark().Legend("S1"); //.MarkStyle<TriangleMarkPlot>();
//	g3.AddSeries(s2).NoPlot().MarkStyle<CircleMarkPlot>().Legend("S2").SetMarkWidth(1).SetSequential().SetDataThickness(1); //.MarkStyle<TriangleMarkPlot>();
//	g3.AddSeries(&funct1).NoPlot().MarkStyle<CircleMarkPlot>().Legend("sin(x/1000.0)*10.0").SetMarkWidth(1).SetDataThickness(1).SetMarkColor(Black());
//	g3.CreateLegendElement<LegendElement<MyGraphCtrl::Types>, RIGHT_OF_GRAPH>( 90 );//.SetBackGndColor( Color(198, 255, 0) );

	g3.CloneBottomElement( 20, g3.GetXGridAxisDraw()).setAxisTimeFormat(THISBACK(FormatAsTime));
//	g3.CloneBottomElement( 20, g3.GetXGridAxisDraw()).setAxisTimeFormat();




	// ================ G4 ================
	g4.CloneTopElement( 20, g4.GetXGridAxisDraw() );
	g4.CreateElement<ExclusionAreaDraw<MyGraphCtrl::Types>, OVER_GRAPH>(0, g4.GetXCoordConverter(), g4.GetYCoordConverter() );
	g4.CreateLegendElement<StdLegendCtrl<MyGraphCtrl::Types, LegendElement<MyGraphCtrl::Types> >, TOP_OF_GRAPH>( 20 );//.SetBackGndColor( Color(198, 255, 0) );
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

	g4.AddSeries(points2).PlotStyle<LineSeriesPlot>().NoMark().Legend("-----S2-----");; // tied to last X/Y  coordConverters ==> X / Y2
	g4.SetMode( GraphDraw_ns::MD_SUBPIXEL );
	
	g4.GetXCoordConverter().setGraphMaxRangeLimit(20);
	g4.GetXCoordConverter().setGraphMinRangeLimit(-10);
	g4.GetYCoordConverter().setGraphMaxRangeLimit(20);
	g4.GetYCoordConverter().setGraphMinRangeLimit(-10);
	y2CoordConverter.setGraphMaxRangeLimit(20);
	y2CoordConverter.setGraphMinRangeLimit(-10);

//	LegendElement<MyGraphCtrl::Types>& legend = g4.CreateElement<LegendElement<MyGraphCtrl::Types>, OVER_GRAPH>(100)
//   				 .SetLegend("[ [ [*/_R@5$(229)0 This ][*/_`R@6$(229)0 is][*/_R@5$(229)0  ][*/_,R@(255.0.255)$(229)0 the][*/_R@5$(229)0  ][*/_cR@5$(229)0 Title]]");
//	legend.SetFrame( Rect(Point(100,100), Size(350, 30)) );
//	legend.SetBackGndColor( Color(198, 255, 0) );

	g4.CreateLegendElement<StdLegendCtrl<MyGraphCtrl::Types, LegendElement<MyGraphCtrl::Types> >, RIGHT_OF_GRAPH>( 90 );//.SetBackGndColor( Color(198, 255, 0) );
	StdLegendCtrl<MyGraphCtrl::Types, LegendElement<MyGraphCtrl::Types> >& legend = g4.CreateLegendElement<StdLegendCtrl<MyGraphCtrl::Types, LegendElement<MyGraphCtrl::Types> >, OVER_GRAPH>( 120 );
	legend.SetFrame( Rect(Point(0,100), Size(350, 30)) );
	legend.SetParentCtrl( g4 );
	RGBA rgba;
	
	rgba.r=90;
	rgba.g=90;
	rgba.b=0;
	rgba.a=90;
	legend.SetBackGndColor( rgba );
//	legend.SetBackGndColor(  );

	bReport << THISBACK(DoReport);
}



void replaceImage(Upp::String& srcTxt, const Upp::String& label, const Upp::Image& inputImg, Upp::Size destSize )
{
	Upp::String s;
	if (!inputImg.IsEmpty() )
	{
		if (destSize.cy != 0)
		{
			if ( inputImg.GetWidth()*100/inputImg.GetHeight() > destSize.cx*100/destSize.cy )
			{
				s << AsQTF(CreatePNGObject(inputImg, destSize.cx,0));
			}
			else
			{
				s << AsQTF(CreatePNGObject(inputImg, 0, destSize.cy));
			}
		}
		else
		{
			s << AsQTF( CreatePNGObject(inputImg));
		}
	}
	else
	{
		replaceImage(srcTxt, label, GraphDraw_testImg::EMPTY_IMG(), destSize);
	}
	
//	Replace(result, label, value);
	srcTxt = Replace(srcTxt, label, s);
//	replaceVar(label,s);
};

void GraphDraw_test::DoReport(void)
{
	String qtfText = "[ [ [R@5$(229)0 This the report]&][R@5$(229)0 &][ [R@5$(229)0 #IMG]&][R@5$(229)0 &][ [R@5$(229)0 This is the end of the report]]";
	Size sz(640,480);
//	replaceImage(qtfText, "#IMG", g4.GetImage(MD_SUBPIXEL, sz, 1), Size(3600,1800));
	replaceImage(qtfText, "#IMG", g4.GetImage(MD_SUBPIXEL, sz, 1), Size(640,480));
	
	Report r;
	r << qtfText;
	Perform(r);
}

using namespace GraphDraw_ns;



GUI_APP_MAIN
{
	TRACE_INFO("STARTING application");
	One<GraphDraw_test> app = new GraphDraw_test();
	app->Sizeable().Run();
	TRACE_INFO("ENDING application");
}
