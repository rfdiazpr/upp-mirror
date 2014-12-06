struct SvgParser : XmlParser {
	Painter& p;

	void ParseSVG(const char *svg, const char *folder);
};
