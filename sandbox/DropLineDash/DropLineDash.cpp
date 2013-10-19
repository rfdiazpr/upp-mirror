#include "DropLineDash.h"

using namespace Upp;

DashStyleDisplay& DashStyleDisplay::GetDefaultDisplay()
{
	return Single<DashStyleDisplay>();
}



DashStyleDisplay::DashStyleDisplay()
: lineColor(Null)
, lineWidth(2)
, drawMode(MODE_ANTIALIASED)
{
}

void DashStyleDisplay::PaintBackground(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	w.DrawRect(r, paper);
}

void DashStyleDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	ImageBuffer ib(r.Size());
	Fill( ib.Begin(), paper, ib.GetLength() );
	BufferPainter bp(ib, drawMode);
	bp.Dash( q.To<String>() );
	if ( !lineColor.IsNullInstance() ) ink = lineColor;
	bp.DrawLine(5, r.GetHeight()/2, r.GetWidth()-5, r.GetHeight()/2, lineWidth, ink);
	w.DrawImage(r.left, r.top, ib );
}

Size DashStyleDisplay::GetStdSize(const Value& q) const 
{
	return Size(48, 16);
}

Size DashStyleDisplay::RatioSize(const Value& q, int cx, int cy) const
{
	return GetStdSize(q);
}




// ================================

DropDashStyle::DropDashStyle()
{
	SetDisplay( display );

	Add("");
	Add("2 2");
	Add("3 3");
	Add("4 4");
	Add("6 6");
	Add("9 9");
	Add("12 12");
	Add("4 10");
	Add("4 20");
	Add("1 4 12 4");
	Add("1 4 1 4 10 4");
	Add("20 10 5 10");
	SetIndex(0);
}

DropDashStyle::~DropDashStyle()
{
}

// ==================================================================================
// ==================================================================================



LineWidthDisplay& LineWidthDisplay::GetDefaultDisplay()
{
	return Single<LineWidthDisplay>();
}



LineWidthDisplay::LineWidthDisplay()
: lineColor(Null)
, drawMode(MODE_ANTIALIASED)
{
}

void LineWidthDisplay::PaintBackground(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	w.DrawRect(r, paper);
}

void LineWidthDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	w.DrawRect(r, paper);
	ImageBuffer ib(r.Size());
	Fill( ib.Begin(), paper, ib.GetLength() );
	BufferPainter bp(ib, drawMode);
	bp.Dash( dashStyle );
	if ( !lineColor.IsNullInstance() ) ink = lineColor;
	bp.DrawLine(5, r.GetHeight()/2, r.GetWidth()-5, r.GetHeight()/2, q.To<int>(), ink);
	w.DrawImage(r.left, r.top, ib );
}

Size LineWidthDisplay::GetStdSize(const Value& q) const 
{
	return Size(48, 16);
}

Size LineWidthDisplay::RatioSize(const Value& q, int cx, int cy) const
{
	return GetStdSize(q);
}

// ================================

DropLineWidth::DropLineWidth()
{
	SetDisplay( display );
	Add(1);
	Add(2);
	Add(3);
	Add(4);
	Add(5);
	Add(6);
	Add(7);
	Add(8);
	Add(9);
	Add(10);
	Add(12);
	Add(14);
	SetIndex(0);
}

DropLineWidth::~DropLineWidth()
{
}

