struct BoundsPainter : public NilPainter {
protected:
	virtual void   MoveOp(const Pointf& p, bool rel);
	virtual void   LineOp(const Pointf& p, bool rel);
	virtual void   QuadraticOp(const Pointf& p1, const Pointf& p, bool rel);
	virtual void   QuadraticOp(const Pointf& p, bool rel);
	virtual void   CubicOp(const Pointf& p1, const Pointf& p2, const Pointf& p, bool rel);
	virtual void   CubicOp(const Pointf& p2, const Pointf& p, bool rel);
	virtual void   ArcOp(const Pointf& c, const Pointf& r, double angle, double sweep, bool rel);
	virtual void   SvgArcOp(const Pointf& r, double xangle, bool large, bool sweep,
	                        const Pointf& p, bool rel);
	virtual void   CloseOp();
	virtual void   DivOp();

//	virtual void   TransformOp(const Xform2D& m);

	Painter& sw;
	Rectf    boundingbox;
	Pointf   current;
	
	Pointf PathPoint(const Pointf& p, bool rel);
	void   SetCurrent(Pointf p, bool rel);

public:
	void  New();
	const Rectf& Get() { return boundingbox; }

	BoundsPainter(Painter& sw) : sw(sw) { New(); }
};

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
	Xform2D      lastTransform;
	BoundsPainter bp;

	void Reset();

	static Color GetTextColor(const String& color);
	static Color GetColor(const String& text);
	
	void ProcessValue(const String& key, const String& value);
	void Style(const char *style);
	void Transform(const char *transform);
	
	String Txt(const char *id)                  { return (*this)[id]; }
	double Dbl(const char *id, double def = 0)  { return Nvl(StrDbl(Txt(id)), def); }
	
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
