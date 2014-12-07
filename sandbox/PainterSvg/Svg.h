struct SvgParser : XmlParser {
	Painter& sw;

	void ParseSVG(const char *svg, const char *folder);
	
	struct Gradient {
		bool   radial;
		Pointf a;
		Pointf b;
		double r;
		int    style;
	};
	
	ArrayMap<String, Gradient> gradient;
	
	struct State {
		int    fill_gradient;
		Color  fill;
		double fill_opacity;
		
		int    stroke_gradient;
		Color  stroke;
		double stroke_width;
		double stroke_opacity;
		
		String dash_array;
		double dash_offset;
		
		String font_family;
		double font_size;
	};
	
	Array<State> state;
	bool         closed;
	
	void Reset();

	static Color GetTextColor(const String& color);
	static Color GetColor(const String& text);
	
	void Style(const char *style);
	void Transform(const char *transform);
	
	String Txt(const char *id)  { return (*this)[id]; }
	double Dbl(const char *id)  { return Nvl(StrDbl(Txt(id))); }
	
	void StartElement();
	void EndElement();
	void StrokeFinishElement();
	void FinishElement();
	void AttrRect();
	Vector<Point> GetPoints();
	void Poly(bool line);
	void ParseG();
	
	String svgFolder;
	
	bool Parse();

	SvgParser(const char *svg, const char *folder, Painter& sw);
};
