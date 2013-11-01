#ifndef _LineDashPusher_LineDashPusher_h_
#define _LineDashPusher_LineDashPusher_h_

#include <CtrlLib/CtrlLib.h>
#include <Draw/Draw.h>
#include <Painter/Painter.h>

using namespace Upp;

class DashStyleDisplay : public Display {
	public:
		Color lineColor;
		int   lineWidth;
		int   drawMode;
		
	
	DashStyleDisplay();
	virtual void PaintBackground(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
	virtual Size GetStdSize(const Value& q) const;
	virtual Size RatioSize(const Value& q, int cx, int cy) const;

	virtual ~DashStyleDisplay() {};
	
	static DashStyleDisplay& GetDefaultDisplay();
};


class DropDashStyle : public DropList
{
	private:
	DashStyleDisplay display;
	
	public:
	DropDashStyle();
	virtual ~DropDashStyle();
	
	void SetDashColor(Color c) { display.lineColor = c; }
	void SetDashWidth(int c)   { display.lineWidth = c; }
	void SetDrawMode(int c)    { if (c==MODE_ANTIALIASED  || c==MODE_NOAA || c==MODE_SUBPIXEL ) display.drawMode = c; }
	
	template<class LIST>
	void SetValues( LIST& v)
	{
		ClearList();
		for (int c=0; c<v.GetCount(); ++c)
		{
			Add(v[c]);
		}
	}
};

// ====================================

class LineWidthDisplay : public Display {
	public:
		Color  lineColor;
		String dashStyle;
		int   drawMode;
		
	
	LineWidthDisplay();
	virtual void PaintBackground(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
	virtual Size GetStdSize(const Value& q) const;
	virtual Size RatioSize(const Value& q, int cx, int cy) const;

	virtual ~LineWidthDisplay() {};
	
	static LineWidthDisplay& GetDefaultDisplay();
};

class DropLineWidth : public DropList
{
	private:
	LineWidthDisplay display;
	
	public:
	DropLineWidth();
	virtual ~DropLineWidth();
	
	void SetLineColor(Color c) { display.lineColor = c; }
	void SetDash(String c)     { display.dashStyle = c; }
	void SetDrawMode(int c)    { if (c==MODE_ANTIALIASED  || c==MODE_NOAA || c==MODE_SUBPIXEL ) display.drawMode = c; }
	
	template<class LIST>
	void SetValues( LIST& v)
	{
		
		ClearList();
		for (int c=0; c<v.GetCount(); ++c)
		{
			Add(v[c]);
		}
	}
};


#endif
