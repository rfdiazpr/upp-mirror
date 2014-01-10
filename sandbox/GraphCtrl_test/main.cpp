
//#define TRACE_INFO(TXT) { std::ostringstream str; str <<  "\n" << TXT; LOG(str.str().c_str()); }
#define TRACE_INFO(TXT) RLOG( TXT )
#define TRACE_ERROR(TXT) RLOG(" ### ERROR : " << TXT )

#include <Report/Report.h>

#include <Functions4U/Functions4U.h>

#include "GraphCtrl_test.h"



#define IMAGECLASS GraphCtrl_testImg
#define IMAGEFILE <GraphCtrl_test/GraphCtrl_test.iml>
#include <Draw/iml_source.h>


using namespace GraphDraw_ns;




class ExclusionAreaDraw : public GraphElement
{
	private:
	CoordinateConverter& _xConverter;
	CoordinateConverter& _yConverter;
	typedef  GraphElement _B;

	public:
	typedef ExclusionAreaDraw   CLASSNAME;
	ExclusionAreaDraw(CoordinateConverter& xConv, CoordinateConverter& yConv) :  _xConverter(xConv), _yConverter(yConv) {};
	~ExclusionAreaDraw() {};


	virtual void PaintElement(Draw& dw, int scale) { /* do noting */}
	virtual void PaintOnPlot_underData(Draw& dw, int otherWidth, int scale)
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

	output = Format("%d/%d/%d", time.day, time.month, time.year);
}

void GraphDraw_test::onMarkerMoveCBK( const GraphDraw_ns::MarkerPosList& markers, int ID )
{
	m1Pos.SetData(markers.Get(1).GetPos());
	m2Pos.SetData(markers.Get(2).GetPos());
	mDelta.SetData(markers.Get(1).GetPos() - markers.Get(2).GetPos());
}


GraphDraw_test::GraphDraw_test()
{
	CtrlLayout(*this, "Window title");

	// ====================================
	//                 G1
	// ====================================
	points << Pointf(0.5,1) << Pointf(1, 3.5)<< Pointf(2,1.8)<< Pointf(4, 3)<< Pointf(5.5, 2.5) << Pointf(6,7) << Pointf(7, 9)<< Pointf(8,10)<< Pointf(10, 15)<< Pointf(15, 15);
	g1.AddSeries(points).PlotStyle<StaggeredSeriesPlot>();

	// ====================================
	//                 G3
	// ====================================
	//g3.debugTrace = true;
	
	double y;
	const int NB_POINTS_G3 = 100000;
	for (int t = 0; t < NB_POINTS_G3; ++t) {
		y = 20.0*sin(2.0*M_PI*t/100000);
		// create a "cloud" of points around the main line
		s1 <<Pointf(t, y);
		s2 <<Pointf(t, y + (Upp::Randomf()*5.0)-2.5);
	}
	g3.AddSeries(s1).MarkStyle<FastMarkPlot>().Legend("S1");
	g3.AddSeries(s2).MarkStyle<FastMarkPlot>().Legend("S2");
	g3.SetCtrlBackgroundStyle(GraphCtrl_testImg::CTRL_BCKGND() );
	g3.SetPlotBackgroundStyle(GraphCtrl_testImg::PLOT_BCKGND() );
	g3.GetLegendElement().SetBackGroundStyle(GraphCtrl_testImg::LEGEND_BACKGND());
	MarkerElementType& g3_markerElem = g3.CreateElement1< MarkerElementType, TOP_OF_GRAPH>(30, 0, g3.GetXCoordConverter() );
	SmartTextTickMark& sttmg3_1 = g3_markerElem.AddMarker<SmartTextTickMark>(1, 25000);
		sttmg3_1.SetText("\1[ [ [*@6;1  3 ]]]");
		sttmg3_1.SetBackGroundStyle(GraphCtrl_testImg::BACKGND());

	// ====================================
	//                 G4
	// ====================================
	//g4.debugTrace = true;
	g4.CreateElement2<ExclusionAreaDraw, FLOAT_OVER_GRAPH>(0, 0, g4.GetXCoordConverter(), g4.GetYCoordConverter() );
	//g4.CreateElement1<GraphDraw_ns::MarkerElement<MyGraphCtrl::Types>, TOP_OF_GRAPH>(20, 0, g4.GetXCoordConverter() );
	MarkerElementType& markerElem = g4.CreateElement1< MarkerElementType, TOP_OF_GRAPH>(30, 0, g4.GetXCoordConverter() );
	//markerElem.SetBackGroundStyle(GraphCtrl_testImg::LEGEND_BACKGND3());
	markerElem.whenMarkerMove = THISBACK(onMarkerMoveCBK);

	SmartTextTickMark& sttmg4_1 = markerElem.AddMarker<SmartTextTickMark>(1, 3.5);
		sttmg4_1.SetText("\1[ [ [*@6;1  1 ]]]");
		sttmg4_1.SetBackGroundStyle(GraphCtrl_testImg::BACKGND());
		sttmg4_1.OutlineColor = Black();
	
	SmartTextTickMark& sttmg4_2 = markerElem.AddMarker<SmartTextTickMark>(2, 8.5);
//		sttmg4_2.SetText("\1[ [ [*@6;1  2 ]]]");
		sttmg4_2.SetText(" 2 ");
		sttmg4_2.SetTextFont(Font().Bold().Width(36));
		sttmg4_2.SetBackGroundStyle(GraphCtrl_testImg::BACKGND2());
	
	g4.AddSeries(points).PlotStyle<LineSeriesPlot>().MarkStyle<XMarkPlot>().MarkWidth(3).Fill(Color(28, 255, 200)).Opacity(.5).Legend("S1");

	points2 << Pointf(0,0) << Pointf(1, 2)<< Pointf(2,3)<< Pointf(4, 3)<< Pointf(5.5, 4) << Pointf(6,5) << Pointf(7, 6)<< Pointf(8,10)<< Pointf(10, 9)<< Pointf(15, 8);
	g4.SetCurrentYConverter(1); // set Y2 as current Y axis
	g4.AddSeries(points2).PlotStyle<LineSeriesPlot>().NoMark().Legend("S2"); // tied to last X/Y  coordConverters ==> X / Y2
	g4.SetCtrlBackgroundStyle(GraphCtrl_testImg::CTRL_BCKGND() );
	g4.SetPlotBackgroundStyle(GraphCtrl_testImg::PLOT_BCKGND() );
	g4.GetLegendElement().SetBackGroundStyle(GraphCtrl_testImg::LEGEND_BACKGND3());
	g4.GetLegendElement().SetElementWidth(28);
	g4.GetYGridAxisDraw().SetVSelectStyle(GraphCtrl_testImg::VSELECT());
	

	// ====================================
	//                 G5
	// ====================================
	MarkerElementType& markerElemG5_1 = g5.CreateElement1< MarkerElementType, TOP_OF_GRAPH >(60, 0, g5.GetXCoordConverter() );
	markerElemG5_1.AddMarker<SmartTextTickMark>(1, 3.8).SetText("\1[ [ [@6$(255.255.192)2 @@image:200&200€„À€À€€€€€€€€€€ø§œíÛÁ’Â Œ…Ğâİ½îº‹­†ä™‡–«ÉÏŒ›ûƒ¼è¢›ëòå•ÒÒ®ÆŸï­è§ŸşúéÇ§ŸşüÅ¿à®ïŞ“‡³ĞÅ—€şÎ«ü×ùù­ÿûÙãíùÙÒ¾ÈÆºÖ„Úßæµ…ÜşÃõ÷ÎÚï‹Øä¼öÏœ¯˜ÔÁø¿ñ—åüõ£öã¾ÓüÈ¾ŒĞŞ³öÈşı©¢ù½ûÿ“úß›Úçô¤á÷Ë½Ê¶¤ÿº€ÓóÓıÒİô½½£ş°è§ÚÿºŸñĞŸßù¿ÕÆÿÖçÉ­ÒÏÿªşÔêùÁ¿ı«ú­Ø™è™úéßİ·ÿ‹ç¢¦©ŒışŞÎø­„ğ°ëöã†ÓıÒŸæÊù­à¿œ¼¤üÁß««ûüÈŒ¢û÷˜‡¬şü¨ÿ¯®›ÑŞ£²ÿ½ËÙ™•ıÖŒæ²ø­™ ‹ş£µıççÁÿĞæ†£şé¼ä¼ûÁí£ıÚŒõ¶‚÷³ı‡Ğò¯¤ã™¼ıÍµÓÎı ÿàû¢Ÿşúé§¿ËÜÿßä»–«©Ö—‰¨ò]]]");
	
	MarkerElementType& markerElemG5_2 = g5.CreateElement1< MarkerElementType, RIGHT_OF_GRAPH >(30, 0, g5.GetYCoordConverter() );
	markerElemG5_2.AddMarker<SmartTextTickMark>(2, 3.8).SetText("\1[ [ [*_@6$(28.255.150)+59 M2]]]");
	
	MarkerElementType& markerElemG5_3 = g5.CreateElement1< MarkerElementType, BOTTOM_OF_GRAPH >(30, 0, g5.GetXCoordConverter() );
	SmartTextTickMark& sttm = markerElemG5_3.AddMarker<SmartTextTickMark>(3, 3.8);
		sttm.SetText("\1[ [ [@6$(255.255.192)2 `-`-][*_$(255.255.192)2 M3][@6$(255.255.192)2 `-`-]]]");
		sttm.SetBackGroundStyle(GraphCtrl_testImg::BACKGND());
	
	MarkerElementType& markerElemG5_4 = g5.CreateElement1< MarkerElementType, LEFT_OF_GRAPH >(30, 0, g5.GetYCoordConverter() );
	SmartTextTickMark& sttm4 = markerElemG5_4.AddMarker<SmartTextTickMark>(4, 3.8);
		sttm4.SetText("---M4---");
		sttm4.SetBackGroundStyle(GraphCtrl_testImg::BACKGND());

	
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
		replaceImage(srcTxt, label, GraphCtrl_testImg::EMPTY_IMG(), destSize);
	}
	
//	Replace(result, label, value);
	srcTxt = Replace(srcTxt, label, s);
//	replaceVar(label,s);
};

void GraphDraw_test::DoReport(void)
{
	String qtfText = "[ [ [R@5$(229)0 This the report]&][R@5$(229)0 &][ [R@5$(229)0 #IMG]&][R@5$(229)0 &][ [R@5$(229)0 This is the end of the report]]";
	Size sz(640*3,480*3);
	replaceImage(qtfText, "#IMG", g4.GetImage(sz, 3), sz);
	
	Report r;
	r << qtfText;
	Perform(r);
}

using namespace GraphDraw_ns;


#include <ide/ide.h>

GUI_APP_MAIN
{
	VALIDATE_IML(GraphCtrl_testImg);
	VALIDATE_IML(GraphDrawImg);
	VALIDATE_IML(GraphCtrlImg);

	VALIDATE_IML(CtrlImg);
	VALIDATE_IML(CtrlsImg);
	VALIDATE_IML(ClassicCtrlsImg);
	VALIDATE_IML(BrowserImg);
	VALIDATE_IML(TopicImg);
	VALIDATE_IML(IdeCommonImg);

	


	TRACE_INFO("STARTING application");
	One<GraphDraw_test> app = new GraphDraw_test();
	app->Sizeable().Run();
	TRACE_INFO("ENDING application");
}
