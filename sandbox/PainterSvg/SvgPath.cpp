#include "PainterSvg.h"

NAMESPACE_UPP

void SvgParser::Bounding(const Pointf& f)
{
	boundingbox.left = min(f.x, boundingbox.left);
	boundingbox.top = min(f.y, boundingbox.top);
	boundingbox.right = max(f.x, boundingbox.right);
	boundingbox.bottom = max(f.y, boundingbox.bottom);
}

bool SvgParser::ReadBool(CParser& p)
{
	while(p.Char(','));
	if(p.Char('1')) return true;
	p.Char('0');
	return false;
}

double SvgParser::ReadDouble(CParser& p)
{
	while(p.Char(','));
	return p.IsDouble() ? p.ReadDouble() : 0;
}

Pointf SvgParser::ReadPoint0(CParser& p, bool rel)
{
	Pointf t;
	t.x = ReadDouble(p);
	t.y = ReadDouble(p);
	if(rel)
		t += prev;
	return t;
}

Pointf SvgParser::ReadPoint(CParser& p, bool rel)
{
	Pointf t = ReadPoint0(p, rel);
	prev = t;
	Bounding(t);
	return t;
}

void SvgParser::Path(const char *s)
{
	try {
		prev = Pointf(0, 0);
		CParser p(s);
		while(!p.IsEof()) {
			int c = p.GetChar();
			p.Spaces();
			bool rel = IsLower(c);
			Pointf t, t1, t2;
			switch(ToUpper(c)) {
			case 'M':
				sw.Move(ReadPoint(p, rel));
			case 'L':
				while(p.IsDouble())
					sw.Line(ReadPoint(p, rel));
				break;
			case 'Z':
				sw.Close();
				break;
			case 'H':
				while(p.IsDouble()) {
					Pointf h(p.ReadDouble(), prev.y);
					if(rel)
						h.x += prev.x;
					prev = h;
					Bounding(h);
					sw.Line(h);
				}
				break;
			case 'V':
				while(p.IsDouble()) {
					Pointf h(prev.x, p.ReadDouble());
					if(rel)
						h.y += prev.y;
					prev = h;
					Bounding(h);
					sw.Line(h);
				}
				break;
			case 'C': // TODO: Compute BB of curves correctly!
				while(p.IsDouble()) {
					t1 = ReadPoint0(p, rel);
					t2 = ReadPoint0(p, rel);
					t = ReadPoint(p, rel);
					sw.Cubic(t1, t2, t, rel);
				}
				break;
			case 'S':
				while(p.IsDouble()) {
					t2 = ReadPoint0(p, rel);
					t = ReadPoint(p, rel);
					sw.Cubic(t2, t, rel);
				}
				break;
			case 'Q':
				while(p.IsDouble()) {
					t1 = ReadPoint0(p, rel);
					t = ReadPoint(p, rel);
					sw.Quadratic(t1, t, rel);
				}
				break;
			case 'T':
				while(p.IsDouble()) {
					t = ReadPoint(p, rel);
					sw.Quadratic(t, rel);
				}
				break;
			case 'A':
				while(p.IsDouble()) {
					t1 = ReadPoint0(p, rel);
					double xangle = ReadDouble(p);
					bool large = ReadBool(p);
					bool sweep = ReadBool(p);
					t = ReadPoint0(p, rel);
					sw.SvgArc(t1, xangle * M_PI / 180.0, large, sweep, t, rel);
				}
				break;
			default:
				return;
			}
		}
	}
	catch(CParser::Error) {}
}

END_UPP_NAMESPACE
