#ifndef _GraphCtrl_GraphFunctions_h_
#define _GraphCtrl_GraphFunctions_h_


Color GetNewColor(int id);
String GetNewDash(int id);
MarkPlot *GetNewMarkPlot(int id);
SeriesPlot* GetNewPlotStyle(int id);

namespace GraphDraw_ns
{
	void DrawVArrowEnd(Draw &w, double x0, double y0, double x1, double y1, double width, double aWidth, double aHeight, const Color &color);
	void DrawHArrowEnd(Draw &w, double x0, double y0, double x1, double y1, double width, double aWidth, double aHeight, const Color &color);
};

#endif
