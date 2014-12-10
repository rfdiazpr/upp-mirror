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
	g.b.x = Nvl(Nvl(g.b.x, g2.b.x), 1.0);
	g.b.y = Nvl(Nvl(g.b.y, g2.b.y));
	g.c.x = Nvl(Nvl(g.c.x, g2.c.x), 0.5);
	g.c.y = Nvl(Nvl(g.c.y, g2.c.y), 0.5);
	g.f.x = Nvl(Nvl(g.f.x, g2.f.x), g.c.x);
	g.f.y = Nvl(Nvl(g.f.y, g2.f.y), g.c.y);
	g.r = Nvl(Nvl(g.r, g2.r), 0.5);
	g.transform = Nvl(g.transform, g2.transform);
	g.style = Nvl(Nvl(g.style, g2.style), GRADIENT_PAD);
}

void SvgParser::StartElement()
{
	DLOG("ID: " << Txt("id"));
	state.Add();
	state.Top() = state[state.GetCount() - 2];
	sw.Begin();
	Transform(Txt("transform"));
	Style(Txt("style"));
	closed = false;
	boundingbox = Rectf(DBL_MAX, DBL_MAX, DBL_MIN, DBL_MIN);
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
	for(int i = 0; i < g.stop.GetCount(); i++)
		sw.ColorStop(g.stop[i].offset, g.stop[i].color);
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

Pointf SvgParser::GP(const Gradient& g, const Pointf& p)
{
	DDUMP(g.user_space);
	DDUMP(boundingbox);
	if(g.user_space)
		return p;
	DDUMP(p);
	Point h = Pointf(p.x * (boundingbox.GetWidth()) + boundingbox.left,
	                 p.y * (boundingbox.GetHeight()) + boundingbox.top);
	DDUMP(h);
	return h;
}

double SvgParser::GP(const Gradient& g, double v)
{
	if(g.user_space)
		return v;
	return v * Length(boundingbox.GetSize()) / sqrt(2.0);
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
			if(g.radial) {
				DLOG("======= Radial ========");
				DDUMP(g.f);
				DDUMP(g.c);
				DDUMP(g.r);
				DDUMP(GP(g, g.f));
				DDUMP(GP(g, g.c));
				DDUMP(GP(g, g.r));
				sw.Fill(GP(g, g.f), g.stop[0].color, GP(g, g.c), GP(g, g.r), g.stop.Top().color, g.style);
			}
			else
				sw.Fill(GP(g, g.a), g.stop[0].color, GP(g, g.b), g.stop.Top().color, g.style);
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
	Pointf p(Dbl("x"), Dbl("y"));
	double cx = Dbl("width");
	double cy = Dbl("height");
	sw.Rectangle(p.x, p.y, cx, cy);
	Bounding(p);
	Bounding(p + Pointf(cx, cy));
}

void SvgParser::ParseGradient(bool radial)
{
	DDUMP(Txt("id"));
	Gradient& g = gradient.Add(Txt("id"));
	g.radial = radial;
	g.user_space = Txt("gradientUnits") == "userSpaceOnUse"; //TODO: implement
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

void SvgParser::Poly(bool line)
{
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
			Bounding(n);
		}
	}
	catch(CParser::Error) {}
	if(r.GetCount() == 0) {
		StartElement();
		sw.Move(r[0].x, r[0].y);
		for(int i = 1; i < r.GetCount(); ++i)
			sw.Line(r[i].x, r[i].y);
		if(line)
			StrokeFinishElement();
		else
			FinishElement();
	}
}

void SvgParser::ParseG() {
	if(IsTag()) DLOG("-- TAG " << PeekTag());
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
		Pointf c(Dbl("cx"), Dbl("cy"));
		Pointf r(Dbl("rx"), Dbl("ry"));
		Bounding(c - r);
		Bounding(c + r);
		sw.Ellipse(c.x, c.y, r.x, r.y);
		FinishElement();
	}
	else
	if(TagE("line")) {
		StartElement();
		Pointf a(Dbl("x1"), Dbl("y1"));
		Pointf b(Dbl("x2"), Dbl("y2"));
		Bounding(a);
		Bounding(b);
		sw.Move(a);
		sw.Line(b);
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
		Path(Txt("d"));
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
		while(!End())
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
