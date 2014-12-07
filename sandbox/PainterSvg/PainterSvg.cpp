#include "PainterSvg.h"

NAMESPACE_UPP

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

void SvgParser::StrokeFinishElement()
{
	State& s = state.Top();
	sw.Begin(); // because we need to set opacity
	sw.Opacity(s.stroke_opacity);
	// TODO gradients
	if(!IsNull(s.stroke) && s.stroke_width > 0) {
		sw.Stroke(s.stroke_width, s.stroke);
		closed = true;
	}
	EndElement();
}

void SvgParser::FinishElement()
{
	State& s = state.Top();
	sw.Begin(); // because we need to set opacity
	sw.Opacity(s.fill_opacity);
	// TODO gradients
	if(!IsNull(s.fill)) {
		sw.Fill(s.fill);
		closed = true;
	}
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

void SvgParser::Poly(bool line) {
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

void SvgParser::ParseG() {
	if(IsTag()) DLOG(PeekTag());
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
	} else
	if(Tag("g")) {
		StartElement();
		ParseG();
		End();
		PassEnd();
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
	catch(XmlError) {
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
