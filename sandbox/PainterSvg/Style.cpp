#include "PainterSvg.h"

namespace Upp {

void SvgParser::Reset()
{
	state.Add();
	State& s = state.Top();
	s.fill_gradient = s.stroke_gradient = -1;
	s.fill = Black();
	s.stroke = Null;
	s.fill_opacity = s.stroke_opacity = s.stroke_width = 1;
	s.dash_offset = 0;
	s.font_size = 24;
}

void SvgParser::Style(const char *style)
{
	String key, value;
	State& s = state.Top();
	for(;;) {
		if(*style == ';' || *style == '\0') {
			value = TrimBoth(value);
			if(value != "inherit") {
				if(key == "fill") {
					if(value.StartsWith("url(")) {
						value = value.Mid(4);
						int q = value.Find(')');
						if(q >= 0)
							value.Trim(q);
						s.fill_gradient = gradient.Find(value);
						s.fill = Null;
					}
					else {
						s.fill_gradient = -1;
						s.fill = GetColor(value);
					}
				}
				else
				if(key == "fill-opacity")
					s.fill_opacity = StrDbl(value);
				else
				if(key == "fill-rule")
					sw.EvenOdd(value == "evenodd");
				else
				if(key == "stroke") {
					if(value.StartsWith("url(")) {
						value = value.Mid(4);
						int q = value.Find(')');
						if(q >= 0)
							value.Trim(q);
						s.stroke_gradient = gradient.Find(value);
						s.stroke = Null;
					}
					else {
						s.stroke = GetColor(value);
						s.stroke_gradient = -1;
					}
				}
				else
				if(key == "stroke-opacity")
					s.stroke_opacity = StrDbl(value);
				else
				if(key == "stroke-width")
					s.stroke_width = StrDbl(value);
				else
				if(key == "stroke-linecap")
					sw.LineCap(decode(value, "round", LINECAP_ROUND, "square", LINECAP_SQUARE, LINECAP_BUTT));
				else
				if(key == "stroke-linejoin")
					sw.LineJoin(decode(value, "round", LINEJOIN_ROUND, "bevel", LINEJOIN_BEVEL, LINEJOIN_MITER));
				else
				if(key == "miter-limit")
					sw.MiterLimit(max(1.0, StrDbl(value)));
				else
				if(key == "stroke-dasharray") {
					s.dash_array = value;
					sw.Dash(s.dash_array, s.dash_offset);
				}
				else
				if(key == "stroke-dashoffset") {
					s.dash_array = value;
					sw.Dash(s.dash_array, s.dash_offset);
				}
				else
				if(key == "stroke-dashoffset") {
					s.dash_offset = StrDbl(value);
					sw.Dash(s.dash_array, s.dash_offset);
				}
			}
			value.Clear();
			key.Clear();
			if(*style == '\0')
				break;
			else
				style++;
		}
		else
		if(*style == ':') {
			key << TrimBoth(value);
			value.Clear();
			style++;
		}
		else
			value.Cat(*style++);
	}
}

void SvgParser::Transform(const char *transform)
{
	CParser p(transform);
	while(!p.IsEof()) {
		String kind = ToLower(p.ReadId());
		Vector<double> r;
		p.Char('(');
		while(!p.IsEof() && !p.Char(')')) {
			r.Add(p.ReadDouble());
			p.Char(',');
		}
		if(r.GetCount() >= 1) {
			if(kind == "translate" && r.GetCount() >= 2)
				sw.Translate(r[0], r[1]);
			else
			if(kind == "rotate") {
				if(r.GetCount() >= 3)
					sw.Translate(-r[1], -r[2]);
				sw.Rotate(r[0]);
				if(r.GetCount() >= 3)
					sw.Translate(-r[1], -r[2]);
			}
			else
			if(kind == "scale" && r.GetCount() >= 2)
				sw.Scale(r[0], r[1]);
			else {
				Xform2D m;
				if(kind == "skewx")
					m.y.x = atan(r[0]);
				else
				if(kind == "skewy")
					m.x.y = atan(r[0]);
				else
				if(kind == "matrix" && r.GetCount() >= 6) {
					m.x.x = r[0];
					m.x.y = r[1];
					m.y.x = r[2];
					m.y.y = r[3];
					m.t.x = r[4];
					m.t.y = r[5];
				}
				sw.Transform(m);
			}
		}
	}
}

};