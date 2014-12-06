#include "PainterSvg.h"

NAMESPACE_UPP

class Svg2DTransform {
public:
	Svg2DTransform() 						{Init();}
	Svg2DTransform(Svg2DTransform &transf) 	{*this = transf;}
	void Init() {
		transX = transY = 0;
		angle = 0;
		scaleX = scaleY = 1;
	}
	void Apply(Painter &sw) {
		sw.End();
		sw.Begin();
		sw.Translate(transX, transY);
		sw.Scale(scaleX, scaleY);
		sw.Rotate(angle);
	}
	void Translate(double x, double y) {
		transX += x;
		transY += y;
	}
	void Scale(double x, double y) {
		scaleX *= x;
		scaleY *= y;
	}
	void Rotate(double ang) {
		angle += ang;
	}
	Svg2DTransform &operator=(const Svg2DTransform &transf) {
		if (this == &transf)      // Same object?
      		return *this; 
		transX = transf.transX;
		transY = transf.transY;
		angle = transf.angle;
		scaleX = transf.scaleX;
		scaleY = transf.scaleY;
		return *this;
	}
	
private:
	double transX;
	double transY;
	double angle;
	double scaleX;
	double scaleY;
};

class SvgStyle {
public:
	SvgStyle()					{Init();}
	SvgStyle(SvgStyle &transf)  {*this = transf;}
	void Init() {
		strokeWidth = 1;
		strokeFill = Null;
		strokeColor = Black();
		strokeOpacity = 1;
	}
	void Get(String str) {
		String value;
		Color color;
		
		color = GetColorXml(GetValueStringXml(str, "stroke:"));
		if (!color.IsNullInstance())
			strokeColor = color;
		
		value = GetValueStringXml(str, "stroke-width:");
		if (value != "")
			strokeWidth = atoi(value);
		
		color = GetColorXml(GetValueStringXml(str, "fill:"));
		if (!color.IsNullInstance())
			strokeFill = color;
		
		value = GetValueStringXml(str, "opacity:");
		if (value != "")
			strokeOpacity = atof(value);
	}
	void Apply(Painter &sw) {
		sw.Opacity(strokeOpacity);	
		if (!strokeFill.IsNullInstance())	
			sw.Fill(strokeFill);				
		sw.Stroke(strokeWidth, strokeColor);
	}
	SvgStyle &operator=(const SvgStyle &style) {
		if (this == &style)      // Same object?
      		return *this; 
		strokeColor = style.strokeColor;
		strokeFill = style.strokeFill;
		strokeOpacity = style.strokeOpacity;
		strokeWidth = style.strokeWidth;
		return *this;
	}
	
public:
	int strokeWidth;
	Color strokeFill;
	Color strokeColor;
	double strokeOpacity;
};

void SvgPaint_Rect(Painter& sw, XmlParser &xp, Svg2DTransform transf, SvgStyle style) {
	int x, y, width, height;
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "x") 
			x = atoi(xp[i]);
		else if (attr == "y") 
			y = atoi(xp[i]);
		else if (attr == "width") 
			width = atoi(xp[i]);
		else if (attr == "height") 
			height = atoi(xp[i]);
		else if (attr == "style") 
			style.Get(xp[i]);
		else if (attr == "transform") 
			ApplyTransf(transf, xp, i);
	}				
	transf.Apply(sw);
	sw.Rectangle(x, y, width, height);	
	style.Apply(sw);
}

void SvgPaint_Image(Painter& sw, XmlParser &xp, Svg2DTransform transf, SvgStyle style, 
					const char *svgFolder) {
	int x, y, width, height;
	String fileName;
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "x") 
			x = atoi(xp[i]);
		else if (attr == "y") 
			y = atoi(xp[i]);
		else if (attr == "width") 
			width = atoi(xp[i]);
		else if (attr == "height") 
			height = atoi(xp[i]);
		else if (attr == "xlink:href") 
			fileName = xp[i];
		else if (attr == "style") 
			style.Get(xp[i]);
		else if (attr == "transform") 
			ApplyTransf(transf, xp, i);		
	}				
	if (!FileExists(fileName))
		fileName = AppendFileName(svgFolder, fileName);
	transf.Apply(sw);
	Image img = StreamRaster::LoadFileAny(fileName);
	if (!IsNull(img))
		sw.Rectangle(x, y, width, height).Fill(StreamRaster::LoadFileAny(fileName), x, y, width, 0).Stroke(0, Black());
	else {
		sw.Rectangle(x, y, width, height).Fill(White()).Stroke(1, Black());
		int wh = 20;
		sw.Move(x+5, y+5).Line(x+5+wh, y+5).Line(x+5+wh, y+5+wh).Line(x+5, y+5+wh).Line(x+5, y+5).Stroke(1, Black());
		sw.Move(x+5+5, y+5+5).Line(x+5+wh-5, y+5+wh-5).Stroke(3, Red());
		sw.Move(x+5+wh-5, y+5+5).Line(x+5+5, y+5+wh-5).Stroke(3, Red());
		sw.Move(x, y);
	}
	style.Apply(sw);
}

void SvgPaint_Ellipse(Painter& sw, XmlParser &xp, Svg2DTransform transf, SvgStyle style) {
	int x, y, width, height;
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "cx") 
			x = atoi(xp[i]);
		else if (attr == "cy") 
			y = atoi(xp[i]);
		else if (attr == "rx") 
			width = atoi(xp[i]);
		else if (attr == "ry") 
			height = atoi(xp[i]);
		else if (attr == "style") 
			style.Get(xp[i]);
		else if (attr == "transform") 
			ApplyTransf(transf, xp, i);		
	}							
	transf.Apply(sw);
	sw.Ellipse(x, y, width, height);	
	style.Apply(sw);
}

void SvgPaint_Line(Painter& sw, XmlParser &xp, Svg2DTransform transf, SvgStyle style) {
	int x1, y1, x2, y2;
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "x1") 
			x1 = atoi(xp[i]);
		else if (attr == "y1") 
			y1 = atoi(xp[i]);
		else if (attr == "x2") 
			x2 = atoi(xp[i]);
		else if (attr == "y2") 
			y2 = atoi(xp[i]);
		else if (attr == "style") 
			style.Get(xp[i]);
		else if (attr == "transform") 
			ApplyTransf(transf, xp, i);
	}				
	transf.Apply(sw);	
	sw.Move(x1, y1);
	sw.Line(x2, y2);	
	style.Apply(sw);
}

Array<Point> GetPolygonPointsXml(String &strpoints) {
	Array<Point> points;
	int newpos, pos;
	char separator;
	
	if (strpoints.Find(',') >= 0)
		separator = ',';
	else
		separator = ' ';
	
	pos = newpos = 0;
	while (newpos != strpoints.GetCount()) {
		int x, y;
		if ((newpos = strpoints.Find(separator, pos)) < 0)
			break;
		x = atoi(strpoints.Mid(pos, newpos+1));
		pos = newpos+1;
		if ((newpos = strpoints.Find(' ', pos+1 < strpoints.GetCount()? pos+1: pos)) < 0)	// Jump possible spaces 
			newpos = strpoints.GetCount();
		y = atoi(strpoints.Mid(pos, newpos+1));
		pos = newpos+1;
		Point &point = points.Add();
		point.x = x;
		point.y = y;
	}
	return points;
}

void SvgPaint_Polygon(Painter& sw, XmlParser &xp, Svg2DTransform transf, SvgStyle style) {
	Array<Point> points;
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "points") {
			String strpoints;
			strpoints = NormalizeSpaces(xp[i]);
			points = GetPolygonPointsXml(strpoints);
		} else if (attr == "style") 
			style.Get(xp[i]);
		else if (attr == "transform") 
			ApplyTransf(transf, xp, i);		
	}			
	transf.Apply(sw);
	sw.Move(points[0].x, points[0].y);
	for (int i = 1; i < points.GetCount(); ++i)				
		sw.Line(points[i].x, points[i].y);
	style.Apply(sw);
}

void SvgPaint_Text(Painter& sw, XmlParser &xp, Svg2DTransform transf, SvgStyle style) {
	int x, y;
	String font;
	int factor = 20;			// Factor to get better resolution text
	
	Font f(Font::ARIAL, 12);
	String str = xp.ReadText();
	StringBuffer stext;
	stext = str;
	
	int i, j;
	for (i = j = 0; i < stext.GetCount(); ++i, ++j) {
		if (stext[i] == '\n')
			stext[j] = ' ';	
		else if (stext[i] == '\r')
			--j;
		else
			stext[j] = stext[i];
	}
	stext.SetCount(j);
	String text = stext;
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "x") 
			x = atoi(xp[i]);
		else if (attr == "y") 
			y = atoi(xp[i]);
		else if (attr == "style") { 
			style.Get(xp[i]);
			
			if (style.strokeFill.IsNullInstance())
				style.strokeFill = style.strokeColor;
			String fontText;
			fontText = GetValueStringXml(xp[i], "font-family:");
			if (fontText.Find("Roman") >= 0)
				f.Face(Font::ROMAN);
			else if (fontText.Find("Arial") >= 0)
				f.Face(Font::ARIAL);			
			else if (fontText.Find("Courier") >= 0)
				f.Face(Font::COURIER);		
			f.Height(atoi(GetValueStringXml(xp[i], "font-size:"))*factor);
			if (f.GetHeight() == 0)
				f.Height(12*factor);
		} else if (attr == "transform") {
			Array<double> args;
			args = GetTransformArgs(xp[i], "translate");
			if (!args.IsEmpty()) 
				transf.Translate(args[0], args[1]);
			args = GetTransformArgs(xp[i], "scale");
			if (!args.IsEmpty()) 
				transf.Scale(args[0], args[1]);			
		}
	}
	transf.Translate(x, y-f.GetHeight()/factor);
	transf.Scale(1./factor, 1./factor);						
	transf.Apply(sw);	
	sw.Text(0, 0, text, f);
	style.Apply(sw);
}

void SvgPaint_Path(Painter& sw, XmlParser &xp, Svg2DTransform transf, SvgStyle &style) {
	String path;
	
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "d") 
			path = xp[i];
		else if (attr == "style") 
			style.Get(xp[i]);
		else if (attr == "transform") 
			ApplyTransf(transf, xp, i);		
	}		
	transf.Apply(sw);	
	if (!path.IsEmpty())
		sw.Path(path);
	style.Apply(sw);
}

void SvgPaint_G(Painter& sw, XmlParser &xp, Svg2DTransform &transf, SvgStyle &style) {
	for (int i = 0; i < xp.GetAttrCount(); ++i) {
		String attr = xp.GetAttr(i);
		if (attr == "style") 
			style.Get(xp[i]);
		else if (attr == "transform") 
			ApplyTransf(transf, xp, i);
	}		
}

void ParseG(Painter& p, XmlParser &xp, Svg2DTransform transf, SvgStyle style, const char *svgFolder) {
	if(xp.TagE("rect")) 
		SvgPaint_Rect(p, xp, transf, style);
	else if(xp.TagE("ellipse"))
		SvgPaint_Ellipse(p, xp, transf, style);				
	else if(xp.TagE("line"))
		SvgPaint_Line(p, xp, transf, style);				
	else if(xp.TagE("polygon"))
		SvgPaint_Polygon(p, xp, transf, style);				
	else if(xp.TagE("polyline"))
		SvgPaint_Polygon(p, xp, transf, style);	
	else if(xp.TagE("path"))
		SvgPaint_Path(p, xp, transf, style);	
	else if(xp.TagE("image"))
		SvgPaint_Image(p, xp, transf, style, svgFolder);		
	else if(xp.Tag("text")) {
		SvgPaint_Text(p, xp, transf, style);
		xp.PassEnd();	
	} else if(xp.Tag("g")) {
		Svg2DTransform transfG(transf);
		SvgStyle styleG(style);
		
		SvgPaint_G(p, xp, transfG, styleG);
					
		while(!xp.End()) 
			ParseG(p, xp, transfG, styleG, svgFolder);
	} else
		xp.Skip();
}

void ParseSVG(Painter& p, const char *svgFile, const char *svgFolder) {
	XmlParser xp(svgFile);
	while(!xp.IsTag())
		xp.Skip();
	xp.PassTag("svg");
	Svg2DTransform transf;
	SvgStyle style;
	p.Begin();
	while(!xp.End()) {
		transf.Init();
		style.Init();
		ParseG(p, xp, transf, style, svgFolder);
	}
	p.End();
}

END_UPP_NAMESPACE
