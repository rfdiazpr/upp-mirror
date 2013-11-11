
//#define TRACE_INFO(TXT) { std::ostringstream str; str <<  "\n" << TXT; LOG(str.str().c_str()); }
#define TRACE_INFO(TXT) std::cout <<  "\n" << TXT << std::flush
#define TRACE_ERROR(TXT) std::cout <<  "\n ### ERROR : " << TXT << std::flush

#include <Report/Report.h>

#include <Functions4U/Functions4U.h>

#include "GraphCtrl_test.h"


#define IMAGECLASS GraphCtrl_testImg
#define IMAGEFILE <GraphCtrl_test/GraphCtrl_test.iml>
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
	ExclusionAreaDraw(typename TYPES::TypeCoordConverter& xConv, typename TYPES::TypeCoordConverter& yConv) :  _xConverter(xConv), _yConverter(yConv) {};
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

	output = Format("%d/%d/%d", time.day, time.month, time.year);
}

GraphDraw_test::GraphDraw_test()
{
	CtrlLayout(*this, "Window title");

	// ================ G1 ================
	points << Pointf(0.5,1) << Pointf(1, 3.5)<< Pointf(2,1.8)<< Pointf(4, 3)<< Pointf(5.5, 2.5) << Pointf(6,7) << Pointf(7, 9)<< Pointf(8,10)<< Pointf(10, 15)<< Pointf(15, 15);
	g1.AddSeries(points).PlotStyle<StaggeredSeriesPlot>();

	// ================ G3 ================
	double y;
	for (int t = 0; t < 100000; ++t) {
		y = 20.0*sin(2.0*M_PI*t/100000.0);
		// create a "cloud" of points around the main line
		s1 <<Pointf(t,y);
		s2 <<Pointf(t,y + (Upp::Randomf()*5.0)-2.5);
	}
	g3.AddSeries(s1).MarkStyle<FastMarkPlot>().Legend("S1");
	g3.AddSeries(s2).MarkStyle<FastMarkPlot>().Legend("S2");
	g3.SetPlotBackgroundImage(GraphCtrl_testImg::PLOT_BCKGND() );
	g3.GetLegendElement().SetBackgndStyle(GraphCtrl_testImg::LEGEND_BACKGND());

	// ================ G4 ================
	g4.CreateElement2<ExclusionAreaDraw<MyGraphCtrl::Types>, OVER_GRAPH>(0, 0, g4.GetXCoordConverter(), g4.GetYCoordConverter() );
	//g4.CreateElement1<GraphDraw_ns::MarkerElement<MyGraphCtrl::Types>, TOP_OF_GRAPH>(20, 0, g4.GetXCoordConverter() );
	typedef DynamicMarkerCtrl< MyGraphCtrl::Types, GraphDraw_ns::MarkerElement< MyGraphCtrl::Types > >  MarkerElementType;
	MarkerElementType& markerElem = g4.CreateElement1< MarkerElementType, TOP_OF_GRAPH>(30, 0, g4.GetXCoordConverter() );

	RectTriangleTickMark& rttm = markerElem.AddMarker<RectTriangleTickMark>(3.8);
		rttm.IDtext = "1";
		rttm.OutlineColor = Black;
		rttm.textBckgndColor = Red;
		rttm.textColor = Yellow;

	SmartTextTickMark& sttmg4_1 = markerElem.AddMarker<SmartTextTickMark>(8.5);
		sttmg4_1.SetText("\1[ [ [*@6;1  2 ]]]");
		sttmg4_1.SetBckgndImage(GraphCtrl_testImg::BACKGND());
	markerElem.AddMarker<LineTickMark>(5);
	
	g4.AddSeries(points).PlotStyle<LineSeriesPlot>().MarkStyle<XMarkPlot>().MarkWidth(3).Fill(Color(28, 255, 200)).Opacity(.5).Legend("S1");

	points2 << Pointf(0,0) << Pointf(1, 2)<< Pointf(2,3)<< Pointf(4, 3)<< Pointf(5.5, 4) << Pointf(6,5) << Pointf(7, 6)<< Pointf(8,10)<< Pointf(10, 9)<< Pointf(15, 8);
	g4.SetCurrentYConverter(1); // set Y2 as current Y axis
	g4.AddSeries(points2).PlotStyle<LineSeriesPlot>().NoMark().Legend("S2"); // tied to last X/Y  coordConverters ==> X / Y2
	g4.SetCtrlBackgroundImage(GraphCtrl_testImg::CTRL_BCKGND() );
	g4.SetPlotBackgroundImage(GraphCtrl_testImg::PLOT_BCKGND() );
	g4.GetLegendElement().SetBackgndStyle(GraphCtrl_testImg::LEGEND_BACKGND2());
	g4.GetLegendElement().SetElementWidth(28);

	MarkerElementType& markerElemG5_1 = g5.CreateElement1< MarkerElementType, TOP_OF_GRAPH >(60, 0, g5.GetXCoordConverter() );
	markerElemG5_1.AddMarker<SmartTextTickMark>(3.8).SetText("\1[ [ [@6$(255.255.192)2 @@image:200&200€„À€À€€€€€€€€€€ø§œíÛÁ’Â Œ…Ğâİ½îº‹­†ä™‡–«ÉÏŒ›ûƒ¼è¢›ëòå•ÒÒ®ÆŸï­è§ŸşúéÇ§ŸşüÅ¿à®ïŞ“‡³ĞÅ—€şÎ«ü×ùù­ÿûÙãíùÙÒ¾ÈÆºÖ„Úßæµ…ÜşÃõ÷ÎÚï‹Øä¼öÏœ¯˜ÔÁø¿ñ—åüõ£öã¾ÓüÈ¾ŒĞŞ³öÈşı©¢ù½ûÿ“úß›Úçô¤á÷Ë½Ê¶¤ÿº€ÓóÓıÒİô½½£ş°è§ÚÿºŸñĞŸßù¿ÕÆÿÖçÉ­ÒÏÿªşÔêùÁ¿ı«ú­Ø™è™úéßİ·ÿ‹ç¢¦©ŒışŞÎø­„ğ°ëöã†ÓıÒŸæÊù­à¿œ¼¤üÁß««ûüÈŒ¢û÷˜‡¬şü¨ÿ¯®›ÑŞ£²ÿ½ËÙ™•ıÖŒæ²ø­™ ‹ş£µıççÁÿĞæ†£şé¼ä¼ûÁí£ıÚŒõ¶‚÷³ı‡Ğò¯¤ã™¼ıÍµÓÎı ÿàû¢Ÿşúé§¿ËÜÿßä»–«©Ö—‰¨ò]]]");
	MarkerElementType& markerElemG5_2 = g5.CreateElement1< MarkerElementType, RIGHT_OF_GRAPH >(30, 0, g5.GetYCoordConverter() );
	markerElemG5_2.AddMarker<SmartTextTickMark>(3.8).SetText("\1[ [ [*_@6$(28.255.150)+59 M2]]]");
	MarkerElementType& markerElemG5_3 = g5.CreateElement1< MarkerElementType, BOTTOM_OF_GRAPH >(30, 0, g5.GetXCoordConverter() );
	SmartTextTickMark& sttm = markerElemG5_3.AddMarker<SmartTextTickMark>(3.8);
		sttm.SetText("\1[ [ [@6$(255.255.192)2 `-`-][*_$(255.255.192)2 M3][@6$(255.255.192)2 `-`-]]]");
		sttm.SetBckgndImage(GraphCtrl_testImg::BACKGND());
	
	MarkerElementType& markerElemG5_4 = g5.CreateElement1< MarkerElementType, LEFT_OF_GRAPH >(30, 0, g5.GetYCoordConverter() );
	SmartTextTickMark& sttm4 = markerElemG5_4.AddMarker<SmartTextTickMark>(3.8);
		sttm4.SetText("---M4---");
		sttm4.SetBckgndImage(GraphCtrl_testImg::BACKGND());

	
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
	Size sz(640*2,480*2);
	replaceImage(qtfText, "#IMG", g4.GetImage(MD_SUBPIXEL, sz, 1), sz);
	
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
