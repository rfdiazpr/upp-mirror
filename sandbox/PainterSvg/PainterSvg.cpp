#include "PainterSvg.h"

NAMESPACE_UPP

void SvgParser::ResolveGradient(int i)
{
	Gradient& g = gradient[i];
	if(g.resolved || g.href.GetCount() < 2)
		return;
	int q = gradient.Find(g.href.Mid(1));
	g.resolved = true;
	if(q < 0)
		return;
	ResolveGradient(q);
	Gradient& g2 = gradient[q];
	if(g.stop.GetCount() == 0)
		g.stop <<= g2.stop;
	g.a.x = Nvl(Nvl(g.a.x, g2.a.x));
	g.a.y = Nvl(Nvl(g.a.y, g2.a.y));
	g.b.x = Nvl(Nvl(g.b.x, g2.b.x));
	g.b.y = Nvl(Nvl(g.b.y, g2.b.y));
	g.c.x = Nvl(Nvl(g.c.x, g2.c.x));
	g.c.y = Nvl(Nvl(g.c.y, g2.c.y));
	g.f.x = Nvl(Nvl(g.f.x, g2.f.x), g.c.x);
	g.f.y = Nvl(Nvl(g.f.y, g2.f.y), g.c.y);
	g.r = Nvl(Nvl(g.r, g2.r));
	g.transform = Nvl(g.transform, g2.transform);
	g.style = Nvl(Nvl(g.style, g2.style), GRADIENT_PAD);
}

void SvgParser::StartElement()
{
	state.Add();
	state.Top() = state[state.GetCount() - 2];
	sw.Begin();
	Transform(Txt("transform"));
	Style(Txt("style"));
	closed = false;
}

void SvgParser::EndElement()
{
	if(!closed)
		sw.Stroke(0, Black()); // Finish path to allow new transformations, if not yet done
	state.Drop();
	sw.End();
}

void SvgParser::Stops(const Gradient& g)
{
	DDUMP(g.stop.GetCount());
	for(int i = 0; i < g.stop.GetCount(); i++) {
		DDUMP(g.stop[i].offset);
		DDUMP(g.stop[i].color);
		sw.ColorStop(g.stop[i].offset, g.stop[i].color);
	}
}

void SvgParser::StrokeFinishElement()
{
	State& s = state.Top();
	if(s.stroke_opacity != 1) {
		sw.Begin();
		sw.Opacity(s.stroke_opacity);
	}
	if(s.stroke_gradient >= 0 && s.stroke_width > 0) {
		ResolveGradient(s.stroke_gradient);
		Gradient& g = gradient[s.stroke_gradient];
		Stops(g);
		if(g.radial)
			sw.Stroke(s.stroke_width, g.f, White(), g.c, g.r, White(), g.style);
		else
			sw.Stroke(s.stroke_width, g.a, RGBAZero(), g.b, RGBAZero(), g.style);
		sw.ClearStops();
		closed = true;
	}
	else
	if(!IsNull(s.stroke) && s.stroke_width > 0) {
		sw.Stroke(s.stroke_width, s.stroke);
		closed = true;
	}
	if(s.stroke_opacity != 1)
		sw.End();
	EndElement();
}

void SvgParser::FinishElement()
{
	State& s = state.Top();
	if(s.fill_opacity != 1) {
		sw.Begin();
		sw.Opacity(s.fill_opacity);
	}
	if(s.fill_gradient >= 0) {
		ResolveGradient(s.fill_gradient);
		Gradient& g = gradient[s.fill_gradient];
		if(g.stop.GetCount()) {
			Stops(g);
		DDUMP(g.a);
		DDUMP(g.b);
			if(g.radial)
				sw.Fill(g.f, g.stop[0].color, g.c, g.r, g.stop.Top().color, g.style);
			else
				sw.Fill(g.a, g.stop[0].color, g.b, g.stop.Top().color, g.style);
			sw.ClearStops();
		}
		closed = true;
	}
	else
	if(!IsNull(s.fill)) {
		sw.Fill(s.fill);
		closed = true;
	}
	if(s.fill_opacity != 1)
		sw.End();
	StrokeFinishElement();
}

void SvgParser::AttrRect()
{
	sw.Rectangle(Dbl("x"), Dbl("y"), Dbl("width"), Dbl("height"));
}

Vector<Point> SvgParser::GetPoints() {
	Vector<Point> r;
	String value = Txt("points");
	try {
		CParser p(value);
		while(!p.IsEof()) {
			Pointf n;
			n.x = p.ReadDouble();
			p.Char(',');
			n.y = p.ReadDouble();
			r.Add(n);
		}
	}
	catch(CParser::Error) {}
	return r;
}

void SvgParser::Poly(bool line)
{
	Vector<Point> p = GetPoints();
	StartElement();
	sw.Move(p[0].x, p[0].y);
	for(int i = 1; i < p.GetCount(); ++i)
		sw.Line(p[i].x, p[i].y);
	if(line)
		StrokeFinishElement();
	else
		FinishElement();
}

void SvgParser::ParseGradient(bool radial)
{
	DDUMP(Txt("id"));
	Gradient& g = gradient.Add(Txt("id"));
	g.radial = radial;
	g.object_space = Txt("gradientUnits") == "objectBoundingBox"; //TODO: implement
	g.transform = Txt("gradientTransform");
	g.c.x = Dbl("cx", Null);
	g.c.y = Dbl("cy", Null);
	g.r = Dbl("r", Null);
	g.f.x = Dbl("fx", g.c.x);
	g.f.y = Dbl("fy", g.c.y);
	g.a.x = Dbl("x1", Null);
	g.a.y = Dbl("y1", Null);
	g.b.x = Dbl("x2", Null);
	g.b.y = Dbl("y2", Null);
	g.style = decode(Txt("spreadMethod"), "pad", GRADIENT_PAD, "reflect", GRADIENT_REFLECT,
	                 "repeat", GRADIENT_REPEAT, (int)Null);
	g.href = Txt("xlink:href");
	g.resolved = IsNull(g.href);
	while(!End())
		if(TagE("stop")) {
			Stop &s = g.stop.Add();
			s.offset = 0;
			String st = Txt("style");
			const char *style = st;
			double opacity = 0;
			Color  color;
			String key, value;
			for(;;) {
				if(*style == ';' || *style == '\0') {
					value = TrimBoth(value);
					if(key == "stop-color")
						color = GetColor(value);
					else
					if(key == "stop-opacity")
						opacity = StrDbl(value);
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
			s.color = clamp(int(opacity * 255 + 0.5), 0, 255) * color;
			s.offset = Dbl("offset");
		}
		else
			Skip();
}

void SvgParser::ParseG() {
	if(IsTag()) DLOG(PeekTag());
	if(Tag("defs"))
		while(!End())
			if(Tag("linearGradient"))
				ParseGradient(false);
			else
			if(Tag("radialGradient"))
				ParseGradient(true);
			else // TODO: Implement pattern
				Skip();
	if(TagE("rect")) {
		StartElement();
		AttrRect();
		FinishElement();
	}
	else
	if(TagE("ellipse")) {
		StartElement();
		sw.Ellipse(Dbl("cx"), Dbl("cy"), Dbl("rx"), Dbl("ry"));
		FinishElement();
	}
	else
	if(TagE("line")) {
		StartElement();
		sw.Move(Dbl("x1"), Dbl("y1"));
		sw.Line(Dbl("x2"), Dbl("y2"));
		FinishElement();
	}
	else
	if(TagE("polygon"))
		Poly(false);
	else
	if(TagE("polyline"))
		Poly(true);
	else
	if(TagE("path")) {
		StartElement();
		sw.Path(Txt("d"));
		FinishElement();
	}
	else
	if(TagE("image")) {
		StartElement();
		String fileName = Txt("xlink:href");
		if (!FileExists(fileName))
			fileName = AppendFileName(svgFolder, fileName);
		Image img = StreamRaster::LoadFileAny(fileName);
		if(!IsNull(img)) {
			AttrRect();
			sw.Fill(StreamRaster::LoadFileAny(fileName), Dbl("x"), Dbl("y"), Dbl("width"), 0);
		}
		EndElement();
	}
	else
	if(Tag("text")) {
		StartElement();
		String text = ReadText();
		text.Replace("\n", " ");
		text.Replace("\r", "");
		text.Replace("\t", " ");
		State& s = state.Top();
		int face = Font::SANSSERIF;
		if(findarg(s.font_family, "courier", "monospace") >= 0)
			face = Font::MONOSPACE;
		if(findarg(s.font_family, "roman;serif"))
			face = Font::SERIF;
		sw.Text(Dbl("x"), Dbl("y"), text, Font(face, (int)s.font_size)); // TODO:Fix font size
		FinishElement();
		PassEnd();	
	}
	else
	if(Tag("g")) {
		StartElement();
		ParseG();
		EndElement();
	}
	else
		Skip();
}

bool SvgParser::Parse() {
	try {
		while(!IsTag())
			Skip();
		PassTag("svg");
		sw.Begin();
		while(!End())
			ParseG();
		sw.End();
	}
	catch(XmlError e) {
		DDUMP(e);
		return false;
	}
	return true;
}

SvgParser::SvgParser(const char *svg, const char *folder, Painter& sw)
:	sw(sw),
	XmlParser(svg)
{
	svgFolder = folder;
	Reset();
}

bool ParseSVG(Painter& p, const char *svg, const char *folder)
{
	return SvgParser(svg, folder, p).Parse();
}

END_UPP_NAMESPACE
