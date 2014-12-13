#include "PainterSvg.h"

NAMESPACE_UPP

void BoundsPainter::New()
{
	boundingbox = Rectf(DBL_MAX, DBL_MAX, DBL_MIN, DBL_MIN);
	current = Pointf(0, 0);
}

Pointf BoundsPainter::PathPoint(const Pointf& p, bool rel)
{
	Pointf r;
	r.x = IsNull(p.x) ? current.x : rel ? p.x + current.x : p.x;
	r.y = IsNull(p.y) ? current.y : rel ? p.y + current.y : p.y;
	return r;
}

void BoundsPainter::SetCurrent(Pointf p, bool rel)
{
	current = PathPoint(p, rel);
	boundingbox.left = min(current.x, boundingbox.left);
	boundingbox.top = min(current.y, boundingbox.top);
	boundingbox.right = max(current.x, boundingbox.right);
	boundingbox.bottom = max(current.y, boundingbox.bottom);
}

void BoundsPainter::MoveOp(const Pointf& p, bool rel)
{
	sw.Move(p, rel);
	current = PathPoint(p, rel);
}

void BoundsPainter::LineOp(const Pointf& p, bool rel)
{
	sw.Line(p, rel);
	SetCurrent(p, rel);
}

void BoundsPainter::QuadraticOp(const Pointf& p1, const Pointf& p, bool rel)
{
	sw.Quadratic(p1, p, rel);
	SetCurrent(p, rel);
}

void BoundsPainter::QuadraticOp(const Pointf& p, bool rel)
{
	sw.Quadratic(p, rel);
	SetCurrent(p, rel);
}

void BoundsPainter::CubicOp(const Pointf& p1, const Pointf& p2, const Pointf& p, bool rel)
{
	sw.Cubic(p1, p2, p, rel);
	SetCurrent(p, rel);
}

void BoundsPainter::CubicOp(const Pointf& p2, const Pointf& p, bool rel)
{
	sw.Cubic(p2, p, rel);
	SetCurrent(p, rel);
}

void BoundsPainter::ArcOp(const Pointf& c, const Pointf& r, double angle, double sweep, bool rel)
{
	DoArc(rel ? current + c : c, r, angle, sweep, 0);
}

void BoundsPainter::SvgArcOp(const Pointf& r, double xangle, bool large, bool sweep, const Pointf& p, bool rel)
{
	DoSvgArc(r, xangle, large, sweep, PathPoint(p, rel), current);
}

void BoundsPainter::CloseOp()
{
	sw.Close();
}

void BoundsPainter::DivOp()
{
	sw.Div();
}

void BoundsPainter::TextOp(const Pointf& p, const wchar *text, Font fnt, int n, double *dx)
{
	Painter::TextOp(p, text, fnt, n, dx);
}

void BoundsPainter::CharacterOp(const Pointf& p, int ch, Font fnt)
{
//	sw.Character(p, ch, fnt);
	PaintCharacter(*this, p, ch, fnt);
}

END_UPP_NAMESPACE
