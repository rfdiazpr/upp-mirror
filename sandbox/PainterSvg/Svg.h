struct SvgParser : XmlParser {
	Painter& sw;

	void ParseSVG(const char *svg, const char *folder);

	struct Stop : Moveable<Stop> {
		RGBA   color;
		double offset;
	};
	
	struct Gradient {
		bool   resolved;
		bool   radial;
		Pointf a, b, c, f;
		double r;
		int    style;
		bool   user_space;
		String transform;

		Vector<Stop> stop;
		
		String href;
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
	Pointf       prev;
	Rectf        boundingbox;
	Xform2D      lastTransform;

	void Reset();

	static Color GetTextColor(const String& color);
	static Color GetColor(const String& text);
	
	void Style(const char *style);
	void Transform(const char *transform);
	
	String Txt(const char *id)                  { return (*this)[id]; }
	double Dbl(const char *id, double def = 0)  { return Nvl(StrDbl(Txt(id)), def); }
	
	void   Bounding(const Pointf& f);
	Pointf GP(const Gradient& g, const Pointf& p);
	double GP(const Gradient& g, double v);

	void StartElement();
	void EndElement();
	void StrokeFinishElement();
	void FinishElement();
	void AttrRect();
	void Stops(const Gradient& g);
	void Poly(bool line);
	void ParseG();
	void ParseGradient(bool radial);
	void ResolveGradient(int i);
	
	bool   ReadBool(CParser& p);
	double ReadDouble(CParser& p);
	Pointf ReadPoint0(CParser& p, bool rel);
	Pointf ReadPoint(CParser& p, bool rel);
	void   Path(const char *s);
	
	bool Parse();

	String svgFolder;
	
	SvgParser(const char *svg, const char *folder, Painter& sw);
};
