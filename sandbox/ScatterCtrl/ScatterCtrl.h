#ifndef _ScatterCtrl_ScatterCtrl_h
#define _ScatterCtrl_ScatterCtrl_h

#include "PopUpText.h"
#include <plugin/jpg/jpg.h>
#include <ScatterDraw/ScatterDraw.h>

using namespace Upp;

class ArrayCtrlXY : public DataSource {
private:
	ArrayCtrl *data;
	bool useCols;
	int idX, idY;
	int beginData;
	int numData;

public:
	ArrayCtrlXY(ArrayCtrl &data, bool useCols = true, int idX = 0, int idY = 1, int beginData = 0, int _numData = Null) : 
		data(&data), useCols(useCols), idX(idX), idY(idY), beginData(beginData), numData(_numData) 
	{
		if (IsNull(_numData)) {
			if (!useCols)
				numData = data.GetColumnCount() - beginData;
			else
				numData = data.GetCount() - beginData;
		}
	}
	virtual inline double y(int id)	{return useCols ? data->Get(beginData + id, idY) : data->Get(idY, beginData + id);};
	virtual inline double x(int id) {
		if (IsNull(idX))
			return id;
		else
			return useCols ? data->Get(beginData + id, idX) : data->Get(idX, beginData + id);
	}
	virtual inline int GetCount()	{return numData;};
};

#include <GridCtrl/GridCtrl.h>

class GridCtrlXY : public DataSource {
private:
	GridCtrl *data;
	bool useCols;
	int idX, idY;
	int beginData;
	int numData;

public:
	GridCtrlXY(GridCtrl &data, bool useCols = true, int idX = 0, int idY = 1, int beginData = 0, int _numData = Null) : 
		data(&data), useCols(useCols), idX(idX), idY(idY), beginData(beginData), numData(_numData)
	{
		if (IsNull(_numData)) {
			if (!useCols)
				numData = data.GetColumnCount() - beginData;
			else
				numData = data.GetCount() - beginData;
		}
	}
	virtual inline double y(int id)	{return useCols ? data->Get(beginData + id, idY) : data->Get(idY, beginData + id);};
	virtual inline double x(int id) {
		if (IsNull(idX))
			return id;
		else
			return useCols ? data->Get(beginData + id, idX) : data->Get(idX, beginData + id);
	}
	virtual inline int GetCount()	{return numData;};
};

class ScatterCtrl : public StaticRect, public ScatterDraw {
public:
	typedef ScatterCtrl CLASSNAME;
	ScatterCtrl();
	
	enum MouseAction {NO_ACTION = 0, SCROLL, ZOOM_H_ENL, ZOOM_H_RED, ZOOM_V_ENL, ZOOM_V_RED, SHOW_INFO, CONTEXT_MENU};

	struct MouseBehaviour {
		bool ctrl;
		bool alt;
		bool shift;
		bool left;
		bool middle;
		int middleWheel;
		bool right;
		MouseAction action;
	};
	
	#define MAX_MOUSEBEHAVIOR 20
	
	bool SetMouseBehavior(MouseBehaviour *_mouseBehavior);
	ScatterCtrl& ShowLegend(const bool& show=true);
	ScatterCtrl& ShowContextMenu(const bool& show = true) {showContextMenu = show; return *this;}
	void SetPopText(const String x, const String y1, const String y2) {popTextX = x; popTextY = y1; popTextY2 = y2;}
	ScatterCtrl& SetMouseHandling(bool valx = true, bool valy = false);
	ScatterCtrl &ShowInfo(bool show = true);	
	
#ifdef PLATFORM_WIN32
	void SaveAsMetafile(const char* file);
#endif	
	void SaveToClipboard(bool saveAsMetafile = false);
	void SaveToImage(String fileName = Null);
	
	virtual void Refresh() {Ctrl::Refresh();};
	virtual Size GetSize() const {return Ctrl::GetSize();};
	
	ScatterCtrl& SetColor(const class::Color& _color)			{ScatterDraw::SetColor(_color);				return *this;};
	ScatterCtrl& SetGridColor(const class::Color& grid_color)	{ScatterDraw::SetGridColor(grid_color);		return *this;};
	ScatterCtrl& SetGridWidth(const int& grid_width) 			{ScatterDraw::SetGridWidth(grid_width); 	return *this;};
	ScatterCtrl& SetPlotAreaColor(const class::Color& p_a_color){ScatterDraw::SetPlotAreaColor(p_a_color); 	return *this;};
	ScatterCtrl& SetLegendWeight(const int& weight)				{ScatterDraw::SetLegendWeight(weight); 		return *this;};
	ScatterCtrl& SetAxisColor(const class::Color& axis_color)	{ScatterDraw::SetAxisColor(axis_color);		return *this;};
	ScatterCtrl& SetAxisWidth(const int& axis_width)			{ScatterDraw::SetAxisWidth(axis_width);		return *this;};
	ScatterCtrl& SetTitle(const String& title)		 			{ScatterDraw::SetTitle(title); 			return *this;};
	ScatterCtrl& SetTitleFont(const Font& fontTitle) 			{ScatterDraw::SetTitleFont(fontTitle); 		return *this;};
	ScatterCtrl& SetTitleColor(const class::Color& colorTitle)	{ScatterDraw::SetTitleColor(colorTitle);	return *this;};
	ScatterCtrl& SetLabelsFont(const Font& fontLabels) 			{ScatterDraw::SetLabelsFont(fontLabels); 	return *this;};
	ScatterCtrl& SetLabelsColor(const class::Color& colorLabels){ScatterDraw::SetLabelsColor(colorLabels);	return *this;};
	ScatterCtrl& SetLabelX(const String& _xLabel)				{ScatterDraw::SetLabelX(_xLabel); 			return *this;};
	ScatterCtrl& SetLabelY(const String& _yLabel)				{ScatterDraw::SetLabelY(_yLabel); 			return *this;};
	ScatterCtrl& SetLabelY2(const String& _yLabel)				{ScatterDraw::SetLabelY2(_yLabel); 			return *this;};
	ScatterCtrl& H_Border(const int& poz_x)						{ScatterDraw::H_Border(poz_x);		 		return *this;};
	ScatterCtrl& V_Border(const int& poz_y)						{ScatterDraw::V_Border(poz_y);		 	 	return *this;};
	
	using ScatterDraw::AddSeries; 
	ScatterCtrl &AddSeries(ArrayCtrl &data, bool useCols = true, int idX = 0, int idY = 1, int beginData = 0, int numData = Null);
	using ScatterDraw::InsertSeries; 
	void InsertSeries(int id, ArrayCtrl &data, bool useCols = true, int idX = 0, int idY = 1, int beginData = 0, int numData = Null);	
	
private:
	bool paintInfo;
	Point clickPoint;
	PopUpInfo popText;
	String popTextX, popTextY, popTextY2;
	Point popLT, popRB;
	const Point offset;

	int butDownX, butDownY;
	bool isScrolling, isLabelPopUp;
	
	bool mouseHandlingX, mouseHandlingY;
	bool showContextMenu;
	
	MouseBehaviour *mouseBehavior;
	
	void ProcessPopUp(const Point & pt);

	virtual void  Paint(Draw& w);
	virtual void  LeftDown(Point, dword);
	virtual void  LeftUp(Point, dword);
	virtual void  MiddleDown(Point, dword);
	virtual void  MouseMove(Point, dword);
	virtual void  MiddleUp(Point, dword);
	virtual void  RightDown(Point, dword);
	virtual void  RightUp(Point, dword);
	virtual void  MouseLeave();
	virtual void  MouseWheel(Point, int zdelta, dword);
	
	void DoMouseAction(bool down, Point pt, MouseAction action, int value);
	void ProcessMouse(bool down, Point &pt, bool ctrl, bool alt, bool shift, bool left, bool middle, int middleWheel, bool right); 
	void LabelPopUp(bool down, Point &pt); 
	void Scrolling(bool down, Point &pt, bool isOut = false);
	void MouseZoom(int zdelta, bool hor, bool ver);
	
	void ContextMenu(Bar& bar);
	
	virtual Image CursorImage(Point p, dword keyflags);
	
	template <class T>
	void SetDrawing(T& w, const int& scale);		
};

template <class T>
void ScatterCtrl::SetDrawing(T& w, const int& scale) {
	ScatterDraw::SetDrawing(w, scale);
	if (!IsNull(popLT) && popLT != popRB) {
		DrawVArrow(w, popLT.x, popLT.y, popLT.x, popRB.y, 1, 4, 15, SColorHighlight());
		DrawHArrow(w, popLT.x, popRB.y, popRB.x, popRB.y, 1, 4, 15, SColorHighlight());
	}
}

#endif

