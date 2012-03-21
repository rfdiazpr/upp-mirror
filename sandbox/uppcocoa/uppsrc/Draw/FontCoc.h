#ifndef _Draw_FontCoc_h_
#define _Draw_FontCoc_h_

struct CocFont : Moveable<CocFont>
{
	float scaleW;
	float scaleH;
	float lineHeight;
	float base;
	float scale;
	
	bool texturesUpdated;

	float vtx[1024 * 4 * 2];
	float crd[1024 * 4 * 2];

	struct CharInfo : Moveable<CharInfo>
	{
		int id;
		float x;
		float y;
		float width;
		float height;
		float xoffset;
		float yoffset;
		float xadvance;
		int page;
	};
	

	Vector<CharInfo> chars;
	VectorMap<int, VectorMap<int, float> > kerns;
	Vector<String> images;
	Vector<int64> pages;

	CocFont() : texturesUpdated(false), scale(1.0f)
	{}

	~CocFont()
	{}

	void Load(const String& fileName);
	void Parse(const char* xml, bool parsePages);

};

struct Resources
{
	static int64 currentSerialId;
	static ArrayMap<int64, Texture> textures;
	static VectorMap<String, CocFont> fonts;
	static int64 Bind(const Image& img, bool linear = false);
	static bool Bind(int64 serialId, bool force = false);
	static CocFont& GetFont(const char* fontName, int fontHeight);
	static CocFont& GetFontBrc(const char* fontName, const byte* fontDef, const byte** imagesData, const int* imagesSize, int imagesCount, int fontHeight);
	static CocFont& GetFont(const Font& font);
};

Size GetTextSize(const wchar *text, const CocFont& fi, int n);



#endif
