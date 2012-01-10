#ifndef _ScatterDraw_ScatterDraw_h
#define _ScatterDraw_ScatterDraw_h

#include <Draw/Draw.h>
#include <Painter/Painter.h>

#include <Core/Core.h>

using namespace Upp;

#include "DataSource.h"
#include "DrawingFunctions.h"
#include "Plot.h"


Color GetOpaqueColor(const Color &color, const Color &background, const double opacity);

class ScatterDraw {
public:
	typedef ScatterDraw CLASSNAME;
	ScatterDraw();
	
	enum Formats {
		EXP,
		MON,
		DY,		
		CUSTOM
	};
	enum {
		MD_DRAW		   = -1,
		MD_ANTIALIASED = MODE_ANTIALIASED,
		MD_NOAA        = MODE_NOAA,
		MD_SUBPIXEL    = MODE_SUBPIXEL
	};
	
	#define LINE_DOTTED 	"4 10"
	#define LINE_DOTTED_SEP	"4 20"
	#define LINE_DASHED 	"12 12"
	#define LINE_DASH_DOT 	"20 10 5 10"
	#define LINE_SOLID 		""
	
protected:
	class ScatterBasicSeries {
	public:
		ScatterBasicSeries();
		void Init(int id);
		
		bool primaryY;
		bool sequential;
		
		One<SeriesPlot> seriesPlot;
		double thickness;
		Color color;
		String dash;
		
		One<MarkPlot> markPlot;
		double markWidth;
		Color markColor;
		
		Color fillColor;
		
		String legend;
		
		double opacity;

		int id;
		
		bool trendLine;
		//double trend
	};
		
	class ScatterSeries : public Moveable<ScatterSeries>, public ScatterBasicSeries {
	public:
		ScatterSeries()	{pD = 0;}
		void SetDataSource(DataSource *pointsData, bool ownsData = true) {pD = pointsData; owns = ownsData;}
		inline DataSource *PointsData()	{return pD;}
		~ScatterSeries()	{if(pD && owns) delete pD;}
	private:
		DataSource *pD;
		bool owns;
	};

	static Color GetNewColor(int id);
	static String GetNewDash(int id);
	static MarkPlot *GetNewMarkPlot(int id);
	
public:
	Callback3<String&, int, double> cbModifFormatX, cbModifFormatDeltaX;
	Callback3<String&, int, double> cbModifFormatY, cbModifFormatDeltaY;
	Callback3<String&, int, double> cbModifFormatY2, cbModifFormatDeltaY2;
			
	Callback WhenZoomScroll;
	Callback WhenSetRange;
	Callback WhenSetXYMin;
	
	ScatterDraw& SetSize(Size sz) {size = sz; return *this;};
	virtual Size GetSize() const {return size;};
	
	ScatterDraw& SetColor(const Color& _color);
	ScatterDraw& SetTitle(const String& _title);
	ScatterDraw& SetTitleFont(const Font& fontTitle);
	ScatterDraw& SetTitleColor(const Color& colorTitle);
	
	void SetLabels(const String& _xLabel, const String& _yLabel, const String& _yLabel2 = "");
	ScatterDraw& SetLabelX(const String& _xLabel);
	ScatterDraw& SetLabelY(const String& _yLabel);
	ScatterDraw& SetLabelY2(const String& _yLabel);
	ScatterDraw& SetLabelsFont(const Font& fontLabels);
	Font GetLabelsFont() {return labelsFont;};
	ScatterDraw& SetLabelsColor(const Color& colorLabels);
	
	ScatterDraw& SetPlotAreaPoz(const int& poz_x,const int& poz_y);
	ScatterDraw& H_Border(const int& poz_x);
	ScatterDraw& V_Border(const int& poz_y);
	ScatterDraw& SetPlotAreaColor(const Color& p_a_color);
	
	ScatterDraw& SetAxisColor(const Color& axis_color);
	ScatterDraw& SetAxisWidth(const int& axis_width);
	
	ScatterDraw& SetGridColor(const Color& grid_color);
	ScatterDraw& SetGridWidth(const int& grid_width);
	ScatterDraw& ShowVGrid(const bool& show);
	ScatterDraw& ShowHGrid(const bool& show);
	
	ScatterDraw& ShowLegend(const bool& show=true);
	ScatterDraw& SetLegendWeight(const int& weight);
	
	ScatterDraw& SetMode(int _mode = MD_ANTIALIASED)	{mode = _mode; Refresh(); return *this;};
	int GetMode()	{return mode;};
	
	void FitToData(bool Y = false);
	void Zoom(double scale, bool hor = true, bool ver = true); 
	void Scroll(double factorX, double factorY);
	
	enum ZoomStyle {TO_CENTER, FROM_BASE};
	ScatterDraw &SetZoomStyleX(ZoomStyle style = TO_CENTER) {zoomStyleX = style; return *this;}
	ScatterDraw &SetZoomStyleY(ZoomStyle style = TO_CENTER) {zoomStyleY = style; return *this;}

	ScatterDraw& SetRange(double rx, double ry, double ry2 = 100);
	double GetXRange()const {return xRange;}
	double GetYRange()const {return yRange;}
	double GetY2Range()const {return yRange2;}
	ScatterDraw &SetMajorUnits(double ux, double uy);
	ScatterDraw &SetMajorUnitsNum(int nx, int ny);
	ScatterDraw &SetMaxMajorUnitsX(int maxX, int maxY)	{maxMajorUnitsX = maxX; maxMajorUnitsY = maxY; return *this;}
	double GetMajorUnitsX() {return xMajorUnit;}
	double GetMajorUnitsY() {return yMajorUnit;}
	ScatterDraw& SetMinUnits(double ux, double uy);
	double GetXMinUnit () const {return xMinUnit;}
	double GetYMinUnit () const {return yMinUnit;}	
	
	ScatterDraw& SetXYMin(double xmin,double ymin,double ymin2 = 0);
	double GetXMin () const {return xMin;}
	double GetYMin () const {return yMin;}	
	double GetYMin2 () const {return yMin2;}
	
	ScatterDraw &Graduation_FormatX(Formats fi);	
	ScatterDraw &Graduation_FormatY(Formats fi);
	ScatterDraw &Graduation_FormatY2(Formats fi);
	
	ScatterDraw &AddSeries(double *yData, int numData, double x0 = 0, double deltaX = 1);
	ScatterDraw &AddSeries(double *xData, double *yData, int numData);
	ScatterDraw &AddSeries(Vector<double> &xData, Vector<double> &yData);
	ScatterDraw &AddSeries(Upp::Array<double> &xData, Upp::Array<double> &yData);
	ScatterDraw &AddSeries(Vector<Pointf> &points);
	ScatterDraw &AddSeries(Upp::Array<Pointf> &points);
	ScatterDraw &AddSeries(double (*function)(double));
	ScatterDraw &AddSeries(Pointf (*function)(double), int np, double from = 0, double to = 1);
	ScatterDraw &AddSeries(PlotFunc &function);
	ScatterDraw &AddSeries(PlotParamFunc function, int np, double from = 0, double to = 1);
	ScatterDraw &_AddSeries(DataSource *data);
	ScatterDraw &AddSeries(DataSource &data);
	
	template <class C>
	ScatterDraw &AddSeries() 	{return _AddSeries(new C());}	
	template <class C, class T1>
	ScatterDraw &AddSeries(T1 &arg1) 			
								{return _AddSeries(new C(arg1));}
	template <class C, class T1, class T2>
	ScatterDraw &AddSeries(T1 &arg1, T2 &arg2) 	
								{return _AddSeries(new C(arg1, arg2));}
	template <class C, class T1, class T2, class T3>
	ScatterDraw &AddSeries(T1 &arg1, T2 &arg2, T3 &arg3) 								
								{return _AddSeries(new C(arg1, arg2, arg3));}
	template <class C, class T1, class T2, class T3, class T4>
	ScatterDraw &AddSeries(T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4) 						
								{return _AddSeries(new C(arg1, arg2, arg3, arg4));}
	template <class C, class T1, class T2, class T3, class T4, class T5>
	ScatterDraw &AddSeries(T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5)			
								{return _AddSeries(new C(arg1, arg2, arg3, arg4, arg5));}
	template <class C, class T1, class T2, class T3, class T4, class T5, class T6>
	ScatterDraw &AddSeries(T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5, T6 &arg6)	
								{return _AddSeries(new C(arg1, arg2, arg3, arg4, arg5, arg6));}
		
	template <class Y>
	ScatterDraw &AddSeries(Vector<Y> &yData)		{return _AddSeries(new VectorY<Y>(yData));}
	template <class Y>
	ScatterDraw &AddSeries(Upp::Array<Y> &yData)	{return _AddSeries(new ArrayY<Y>(yData));}
	template <class X, class Y>
	ScatterDraw &AddSeries(VectorMap<X, Y> &data)	{return _AddSeries(new VectorMapXY<X, Y>(data));}
	template <class X, class Y>
	ScatterDraw &AddSeries(ArrayMap<X, Y> &data)	{return _AddSeries(new ArrayMapXY<X, Y>(data));}
	
	void InsertSeries(int id, double *yData, int numData, double x0 = 0, double deltaX = 1);
	void InsertSeries(int id, double *xData, double *yData, int numData);
	void InsertSeries(int id, Vector<double> &xData, Vector<double> &yData);
	void InsertSeries(int id, Upp::Array<double> &xData, Upp::Array<double> &yData);
	void InsertSeries(int id, Vector<Pointf> &points);
	void InsertSeries(int id, Upp::Array<Pointf> &points);
	void InsertSeries(int id, double (*function)(double));
	void InsertSeries(int id, Pointf (*function)(double), int np, double from = 0, double to = 1);
	void InsertSeries(int id, PlotFunc &function);
	void InsertSeries(int id, PlotParamFunc function, int np, double from = 0, double to = 1);
	void _InsertSeries(int id, DataSource *data);
	
	template <class C>
	void InsertSeries(int id) 		{_InsertSeries(id, new C());}	
	template <class C, class T1>
	void InsertSeries(int id, T1 &arg1) 				
									{_InsertSeries(id, new C(arg1));}
	template <class C, class T1, class T2>
	void InsertSeries(int id, T1 &arg1, T2 &arg2) 		
									{_InsertSeries(id, new C(arg1, arg2));}
	template <class C, class T1, class T2, class T3>
	void InsertSeries(int id, T1 &arg1, T2 &arg2, T3 &arg3) 								
									{_InsertSeries(id, new C(arg1, arg2, arg3));}
	template <class C, class T1, class T2, class T3, class T4>
	void InsertSeries(int id, T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4)						
									{_InsertSeries(id, new C(arg1, arg2, arg3, arg4));}
	template <class C, class T1, class T2, class T3, class T4, class T5>
	void InsertSeries(int id, T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5)				
									{_InsertSeries(id, new C(arg1, arg2, arg3, arg4, arg5));}
	template <class C, class T1, class T2, class T3, class T4, class T5, class T6>
	void InsertSeries(int id, T1 &arg1, T2 &arg2, T3 &arg3, T4 &arg4, T5 &arg5, T6 &arg6)	
									{_InsertSeries(id, new C(arg1, arg2, arg3, arg4, arg5, arg6));}
			
	template <class Y>
	void InsertSeries(int id, Vector<Y> &yData)		{_InsertSeries(id, new VectorY<Y>(yData));}
	template <class Y>
	void InsertSeries(int id, Upp::Array<Y> &yData)	{_InsertSeries(id, new ArrayY<Y>(yData));}
	template <class X, class Y>
	void InsertSeries(int id, VectorMap<X, Y> &data){_InsertSeries(id, new VectorMapXY<X, Y>(data));}
	template <class X, class Y>
	void InsertSeries(int id, ArrayMap<X, Y> &data)	{_InsertSeries(id, new ArrayMapXY<X, Y>(data));}
	
	ScatterDraw &PlotStyle()								{return PlotStyle(0);};
	template <class C>
	ScatterDraw &PlotStyle()								{return PlotStyle(new C());};
	template <class C, class T1>
	ScatterDraw &PlotStyle(T1 &arg1)						{return PlotStyle(new C(arg1));};
	template <class C, class T1, class T2>
	ScatterDraw &PlotStyle(T1 &arg1, T2 &arg2)				{return PlotStyle(new C(arg1, arg2));};
	template <class C, class T1, class T2, class T3>
	ScatterDraw &PlotStyle(T1 &arg1, T2 &arg2, T3 &arg3)	{return PlotStyle(new C(arg1, arg2, arg3));};			
	ScatterDraw &PlotStyle(SeriesPlot *data);
	
	ScatterDraw &NoPlot()	{return PlotStyle();};

	ScatterDraw &MarkStyle()								{return MarkStyle(0);};
	template <class C>
	ScatterDraw &MarkStyle()								{return MarkStyle(new C());};
	template <class C, class T1>
	ScatterDraw &MarkStyle(T1 &arg1)						{return MarkStyle(new C(arg1));};
	template <class C, class T1, class T2>
	ScatterDraw &MarkStyle(T1 &arg1, T2 &arg2)				{return MarkStyle(new C(arg1, arg2));};
	template <class C, class T1, class T2, class T3>
	ScatterDraw &MarkStyle(T1 &arg1, T2 &arg2, T3 &arg3)	{return MarkStyle(new C(arg1, arg2, arg3));};			
	ScatterDraw &MarkStyle(MarkPlot *data);
	
	ScatterDraw &NoMark()	{return MarkStyle();};
		
	ScatterDraw &Stroke(double thickness = 3, Color color = Null);
	ScatterDraw &Dash(const char *dash);
	ScatterDraw &Fill(Color color = Null);
	ScatterDraw &MarkColor(Color color = Null);
	ScatterDraw &MarkWidth(const double& markWidth = 8);
	ScatterDraw &Hide() {series[series.GetCount()-1].opacity = 0;	return *this;}
	
	ScatterDraw &Opacity(double opacity = 1) {series[series.GetCount()-1].opacity = opacity;	return *this;}
	ScatterDraw &Legend(const String legend);
	
	inline bool IsValid(const int& j) const {return (j >= 0 && j < series.GetCount());}
	
	ScatterDraw& SetDrawXReticle(bool set = true);
	ScatterDraw& SetDrawYReticle(bool set = true);
	ScatterDraw& SetDrawY2Reticle(bool set = true);
	
	void SetDataColor(const int& j,const Color& pcolor);
	Color GetDataColor (const int& j) const;
	void SetDataThickness(const int& j, const double& thick);
	double GetDataThickness(const int& j) const;
	void SetFillColor(const int& j, const Color& color);
	Color GetFillColor(const int& j) const;

	ScatterDraw &SetMarkWidth(const int& j, const double& width);
	double GetMarkWidth(const int& j) const;
	void SetMarkColor(const int& j, const Color& pcolor);
	::Color GetMarkColor (const int& j) const;
	void NoMark(const int& j);
	bool IsShowMark(const int& j) const throw (Exc);
	
	void SetDataPrimaryY(const int& j, const bool& primary=true);
	ScatterDraw &SetDataPrimaryY(const bool& primary); 	
	bool IsDataPrimaryY(const int& j) const throw (Exc);	
	
	void SetSequentialX(const int& j, const bool& sequential = true);
	ScatterDraw &SetSequentialX(const bool& sequential = true);
	ScatterDraw &SetSequentialXAll(const bool& sequential = true);
	
	void Show(const int& j, const bool& show = true);
	bool IsVisible(const int& j);
	ScatterDraw &ShowAll(const bool& show = true);

	void RemoveSeries(const int& j);
	void RemoveAllSeries();
	
	ScatterDraw& Id(int id);
	ScatterDraw& Id(const int& j, int id);
	int GetId(const int& j);
	
	Drawing GetDrawing();
	Image GetImage(const int& scale = 1);
	
	#ifdef PLATFORM_WIN32
	void SaveAsMetafile(const char* file) const;
	#endif
	
	ScatterDraw& LogX(const bool& logx=true) {logX=logx; return *this;}
	ScatterDraw& LogY(const bool& logy=true) {logY=logy; return *this;}	
	ScatterDraw& LogY2(const bool& logy=true) {logY2=logy; return *this;}	
	
	ScatterDraw& SetMinZoom(double x, double y = -1) {minXZoom = x; minYZoom = y; return *this;}; 
	ScatterDraw& SetMaxZoom(double x, double y = -1) {maxXZoom = x; maxYZoom = y; return *this;};

	ScatterDraw& SetFastViewX(bool set = true) {fastViewX = set;	return *this;};
	
	Font& GetTitleFont() {return titleFont;};
	int GetH_Border() {return px;};
	int GetV_Border() {return py;};
	
	double GetXByPoint(const int x);
	double GetYByPoint(const int y);
	double GetY2ByPoint(const int y);
	double GetXPointByValue(const double x);
	double GetYPointByValue(const double y);

	int GetCount() 	{return series.GetCount();}
	bool IsEmpty()	{return series.IsEmpty();}
	
protected:
	int mode;
	Size size;
	class ::Color graphColor;	
	String title;
	Font titleFont;
	Color titleColor;
	
	String xLabel, yLabel, yLabel2;
	Font labelsFont;
	Color labelsColor;
	
	int px, py;
	class ::Color plotAreaColor;
	
	bool fastViewX, sequentialXAll;
	
	Color axisColor;
	int axisWidth;
	
	double xRange, yRange, yRange2;
	double xMin, yMin, yMin2;
	double xMajorUnit, yMajorUnit, yMajorUnit2;
	double xMinUnit, yMinUnit, yMinUnit2;
	double minXZoom, maxXZoom, minYZoom, maxYZoom;
	bool drawXReticle, drawYReticle, drawY2Reticle;	
	
	int maxMajorUnitsX, maxMajorUnitsY;
	
	Color gridColor;
	int gridWidth;
	bool drawVGrid, drawHGrid;	
		
	int butDownX, butDownY;
	bool isScrolling, isLabelPopUp;
	ZoomStyle zoomStyleX, zoomStyleY;	
	
	Vector<ScatterSeries> series;
	
	bool showLegend;
	int legendWeight;
	
	void DrawLegend(Draw& w,const int& scale) const;

	virtual void Refresh() {};

	void Scrolling(bool down, Point &pt, bool isOut = false);
	
	void ExpFormat(String& s, int i, double d)	{s = FormatDoubleExp(d,1);}
	void MonFormat(String& s, int i, double d)	{s = Format("%Mon",int(d));}
	void DyFormat(String& s, int i, double d)	{s = Format("%Dy",int(d));}
	
	static String VariableFormat(double range, double d)
	{
		if (fabs(d) <= 1e-15)
			d = 0;
		if 		(0.001<=range && range<0.01) 	return FormatDouble(d,5);
		else if (0.01<=range && range<0.1) 		return FormatDouble(d,4);
		else if (0.1<=range && range<1) 		return FormatDouble(d,3);
		else if (1<=range && range<10) 			return FormatDouble(d,2);
		else if (10<=range && range<100) 		return FormatDouble(d,1);
		else if (100<=range && range<100000) 	return FormatDouble(d,0);
		else return FormatDoubleExp(d,2);
		
	}	
	String VariableFormatX(const double& d) const  {return VariableFormat(xRange, d);}
	String VariableFormatY(const double& d) const  {return VariableFormat(yRange, d);} 
	String VariableFormatY2(const double& d) const {return VariableFormat(yRange2, d);}

	template<class T>
	void SetDrawing(T& w, const int& scale);
	template<class T>
	void Plot(T& w, const int& scale,const int& l,const int& h);	
		
	void AdjustMinUnitX();
	void AdjustMinUnitY();
	void AdjustMinUnitY2();
	
	bool logX, logY, logY2;	
	
private:
	static void ParseTextMultiline(const String &text, Font fnt, 
								   Upp::Array <String> &texts, Upp::Array <Size> &sizes);
};

template <class T>
void ScatterDraw::SetDrawing(T& w, const int& scale)
{
	w.DrawRect(scale*GetSize(), graphColor);
	
	if (typeid(T) == typeid(BufferPainter)) {
		Painter &p = (Painter &)w;
		p.Translate(0, 0.5);
	}
	
	Size sz(0, 0); 
	
	if(!title.IsEmpty()) {
		Font fontTitle6;
		fontTitle6 = titleFont;
		fontTitle6.Height(scale*titleFont.GetHeight());
		fontTitle6.Width(scale*titleFont.GetWidth());
		sz = GetTextSize(title, fontTitle6);
		DrawText(w, (scale*GetSize().cx-sz.cx)/2., scale*2., 0, title, fontTitle6, titleColor);   
	}
	
	w.Offset(Point(scale*px, scale*py+scale*titleFont.GetHeight()));
	if(showLegend) 
		DrawLegend(w,scale);
	int l = scale*GetSize().cx - 2*scale*px;
	int h = scale*GetSize().cy - 2*scale*py - scale*titleFont.GetHeight();

	Font fontLabel6;
	fontLabel6 = labelsFont;
	fontLabel6.Height(scale*labelsFont.GetHeight());
	
	Size lx  = GetTextSize(xLabel, fontLabel6);
	Size ly  = GetTextSize(yLabel, fontLabel6);
	Size ly2 = GetTextSize(yLabel2, fontLabel6);
	DrawText(w, (l-lx.cx)/2., h+scale*py-lx.cy-scale*2., 0, xLabel, fontLabel6, labelsColor);
	DrawText(w, scale*2.-scale*px, (h+ly.cx)/2., 900, yLabel, fontLabel6, labelsColor);
	DrawText(w, l+scale*px-ly2.cy-2.*scale, (h+ly2.cx)/2, 900, yLabel2, fontLabel6, labelsColor);
	
	if (typeid(T) == typeid(BufferPainter)) {
		Painter &p = (Painter &)w;
		p.Translate(0.5, 0);
	}
	
	if (drawXReticle)
		for(int i = 0; xMinUnit+i*xMajorUnit <= xRange; i++){
			w.DrawLine(fround(l*xMinUnit/xRange+i*l/(xRange/xMajorUnit)), h,   
					   fround(l*xMinUnit/xRange+i*l/(xRange/xMajorUnit)), h + scale*4, 
					   fround(gridWidth*scale), axisColor);             
			Font standard6;
			standard6.Height(scale*StdFont().GetHeight());
			double gridX = xMinUnit + i*xMajorUnit + xMin;
			String gridLabelX;
			if (cbModifFormatX)
				cbModifFormatX(gridLabelX, i, gridX);
			else
				gridLabelX = VariableFormatX(gridX);
			
			Array <String> texts;
			Array <Size> sizes;
			ParseTextMultiline(gridLabelX, StdFont(), texts, sizes);
			for (int ii = 0; ii < texts.GetCount(); ++ii) {
				int cy = ii == 0 ? 0 : sizes[ii-1].cy;
				DrawText(w, l*xMinUnit/xRange+i*l/(xRange/xMajorUnit) - scale*sizes[ii].cx/2., 
							h+scale*(4+ii*cy), 0, texts[ii], standard6, axisColor);
			}
		}
	if (drawYReticle)
		for(int i = 0; yMinUnit+i*yMajorUnit <= yRange; i++){
			w.DrawLine(-(scale*4), fround(-h*yMinUnit/yRange+h-i*h/(yRange/yMajorUnit)),
					   0, fround(-h*yMinUnit/yRange+h-i*h/(yRange/yMajorUnit)),
					   fround(gridWidth*scale), axisColor);
			double gridY=yMinUnit+i*yMajorUnit+yMin;
			String gridLabelY;
			if (cbModifFormatY)
				cbModifFormatY(gridLabelY, i, gridY);
			else
				gridLabelY = VariableFormatY(gridY);
			int dx=scale*GetTextSize(gridLabelY,StdFont()).cx;
			Font Standard6;
			Standard6.Height(scale*StdFont().GetHeight());  
			DrawText(w, -dx-scale*6, fround(-h*yMinUnit/yRange+h-i*h/(yRange/yMajorUnit))-scale*8, 
						0, gridLabelY, Standard6, axisColor);
		}	
	if (drawY2Reticle)
		for(int i = 0; yMinUnit+i*yMajorUnit <= yRange; i++){
			w.DrawLine(l+(scale*4), fround(-h*yMinUnit2/yRange2+h-i*h/(yRange/yMajorUnit)),
					   l, fround(-h*yMinUnit2/yRange2+h-i*h/(yRange/yMajorUnit)),
					   fround(gridWidth*scale), axisColor);
			double gridY2=yMinUnit2+i*yMajorUnit2+yMin2;
			String gridLabelY2;
			if (cbModifFormatY2)
				cbModifFormatY2(gridLabelY2, i, gridY2);
			else
				gridLabelY2 = VariableFormatY2(gridY2);
			Font standard6;
			standard6.Height(scale*StdFont().GetHeight());  
			DrawText(w, l+scale*10, fround(-h*yMinUnit2/yRange2+h-i*h/(yRange/yMajorUnit))-scale*8, 0, 
						gridLabelY2, standard6,axisColor);
		}	
	
	Plot(w, scale, l, h);	
	ClipEnd(w);	
}

template <class T>
void ScatterDraw::Plot(T& w, const int& scale, const int& l, const int& h)
{
	double d1 = xRange/xMajorUnit;
	double d2 = yRange/yMajorUnit;

	w.DrawRect(1, 1, l-2, h-1, plotAreaColor);	

	if (drawVGrid) 
		for(int i = 0; xMinUnit+i*xMajorUnit < xRange; i++) 
			DrawLineOpa(w, fround(l*xMinUnit/xRange+i*l/d1), 0, fround(l*xMinUnit/xRange+i*l/d1), h, 
						1, 1, gridWidth, gridColor, "2 2");
	
	if (drawHGrid)
		for(int i = 0; yMinUnit+i*yMajorUnit < yRange; i++) 
			DrawLineOpa(w, 0, fround(-h*yMinUnit/yRange + h-i*h/d2), l, fround(-h*yMinUnit/yRange + h-i*h/d2), 
						1, 1, gridWidth, gridColor, "2 2");

	w.DrawLine(0, h, l, h, fround(gridWidth*scale), Black);
	w.DrawLine(0, 0, l, 0, fround(gridWidth*scale), Black);
	w.DrawLine(0, 0, 0, h, fround(gridWidth*scale), Black);
	w.DrawLine(l, 0, l, h+1, fround(gridWidth*scale), Black);
	
	Clip(w, 0, 0, l, h);
	
	if (!series.IsEmpty()) {
		for (int j = 0; j < series.GetCount(); j++) {
			if (series[j].opacity == 0 || (!series[j].seriesPlot && !series[j].markPlot))
				continue;
			Vector<Point> p1;
			int imin, imax;
			if (series[j].sequential) {
				imin = imax = Null;
				for (int i = 1; i < series[j].PointsData()->GetCount()-1; ++i) {
					if (IsNull(imin)) {
						if (series[j].PointsData()->x(i) >= xMin)
							imin = i-1;
					} else if (IsNull(imax)) {
						if (series[j].PointsData()->x(i) >= xMin+xRange)
							imax = i+1;
					}
				}
				if (IsNull(imin))
				    imin = 0;
				if (IsNull(imax))
				    imax = series[j].PointsData()->GetCount();
			} else if (series[j].PointsData()->IsParam()) { 				// It is a param function
				imin = 0;
				imax = series[j].PointsData()->GetCount();
			} else if (IsNull(series[j].PointsData()->GetCount())) {		// It is a function
				imin = fround(xMin) - 1;
				imax = fround(xMin + xRange) + 1;
			} else {
			    imin = 0;
			    imax = series[j].PointsData()->GetCount();
			}
			int numV; 
			if (fastViewX)
				numV = 1+(imax-imin)/l;
			else
				numV = 1;
			for (int i = imin; i < imax; i += numV) {
				double xx, yy;
				if (fastViewX && numV > 1) {
					yy = 0;
					int ii;
					for (ii = 0; ii < numV && i+ii < imax; ++ii) 
						yy += series[j].PointsData()->y(i+ii);
					yy /= double(ii);
					xx = (series[j].PointsData()->x(i) + series[j].PointsData()->x(i+ii-1))/2;
				} else {
					xx = series[j].PointsData()->x(i);
					yy = series[j].PointsData()->y(i);
				}
				int ix, iy;
				ix = fround(l*(xx-xMin)/xRange);
				if (series[j].primaryY)
					iy = fround(h*(yy-yMin)/yRange);
				else
					iy = fround(h*(yy-yMin2)/yRange2);
				p1 << Point(ix, h-iy);
			}
			if (!p1.IsEmpty()) 
				series[j].seriesPlot->Paint(w, p1, scale, series[j].opacity, 
											fround(series[j].thickness), series[j].color, 
											series[j].dash, plotAreaColor, series[j].fillColor, l/xRange, h/yRange, 
											int(h*(1+yMin/yRange)));
		
			if (series[j].markWidth >= 1 && series[j].markPlot) {
				for (int i = 0; i < (imax-imin)/numV; i++) 
					series[j].markPlot->Paint(w, scale, p1[i], series[j].markWidth, series[j].markColor);              
			}	
		}
	}
	w.End();
}
		
#endif

