#include "GraphDraw.h"


#define IMAGECLASS GraphDrawImg
#define IMAGEFILE <GraphDraw/GraphDraw.iml>
#include <Draw/iml_source.h>

namespace GraphDraw_ns
{
	void DrawHArrowEnd(Draw &w, double x0, double y0, double x1, double y1, double width, double aWidth, double aHeight, const Color &color)
	{
		DrawLine(w, x0, y0, x1, y1, width, color);
		if (fabs(x0 - x1) > 2*aHeight) {
			Vector <Point> arrow;
			if (y0 > y1)
				Swap(y0, y1);
			if (x0 > x1) {
				Swap(x0, x1);
				arrow << Point(fround(x0), fround(y0)) << Point(fround(x0+aHeight), fround(y0+aWidth)) << Point(fround(x0+aHeight), fround(y0-aWidth));
			}
			else {
				arrow << Point(fround(x1), fround(y0)) << Point(fround(x1-aHeight), fround(y0+aWidth)) << Point(fround(x1-aHeight), fround(y0-aWidth));
			}
			w.DrawPolygon(arrow, color);
		}
	}

	void DrawVArrowEnd(Draw &w, double x0, double y0, double x1, double y1, double width, double aWidth, double aHeight, const Color &color)
	{
		DrawLine(w, x0, y0, x1, y1, width, color);
		if (fabs(y0 - y1) > 2*aHeight) {
			Vector <Point> arrow;
			if (x0 > x1)
				Swap(x0, x1);
			if (y0 > y1) {
				Swap(y0, y1);
				arrow << Point(fround(x0), fround(y0)) << Point(fround(x0-aWidth), fround(y0+aHeight)) << Point(fround(x0+aWidth), fround(y0+aHeight));
			}
			else {
				arrow << Point(fround(x0), fround(y1)) << Point(fround(x0-aWidth), fround(y1-aHeight)) << Point(fround(x0+aWidth), fround(y1-aHeight));
			}
			w.DrawPolygon(arrow, color);
		}
	}

};

	Color GetNewColor(int id)
	{
		switch(id) {
			case 0:  return LtBlue();
			case 1:  return LtRed();
			case 2:  return LtGreen();
			case 3:  return Black();
			case 4:  return LtGray();
			case 5:  return Brown();
			case 6:  return Blue();
			case 7:  return Red();
			case 8:  return Green();
			case 9:  return Gray();
			case 10: return LtBlue();
			case 11: return LtRed();
			case 12: return LtGreen();
			case 13: return Black();
			case 14: return LtGray();
			case 15: return Brown();
			case 16: return Blue();
			case 17: return Red();
			case 18: return Green();
			case 19: return Gray();
		}
		return Color(Random(), Random(), Random());
	}

	String GetNewDash(int id)
	{
		switch(id) {
			case 0:		return LINE_SOLID;
			case 1:		return LINE_DOTTED;
			case 2:		return LINE_DASHED;
			case 3:		return LINE_DASH_DOT;
			case 4:		return LINE_SOLID;
			case 5:		return LINE_DOTTED;
			case 6:		return LINE_DASHED;
			case 7:		return LINE_DASH_DOT;
			case 8:		return LINE_SOLID;
			case 9:		return LINE_DOTTED;
			case 10:	return LINE_DASHED;
			case 11:	return LINE_DASH_DOT;
		}
		dword r = Random();
		if (r < 8000)
			r += 2000;
		String ret = FormatInt(r).Right(4);
		String space = " ";
		return ret.Mid(0, 1) + space + ret.Mid(1, 1) + space + ret.Mid(2, 1) + space + ret.Mid(3, 1);
	}

		MarkPlot* GetNewMarkPlot(int id)
		{
			switch(id) {
			case 0:	return new CircleMarkPlot();
			case 1:	return new SquareMarkPlot();
			case 2:	return new TriangleMarkPlot();
			case 3:	return new CrossMarkPlot();
			case 4:	return new XMarkPlot();
			case 5:	return new RhombMarkPlot();
			}
			return new CircleMarkPlot();
		}

		SeriesPlot* GetNewPlotStyle(int id) {
			return new LineSeriesPlot();
		}
