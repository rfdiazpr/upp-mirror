#ifdef PLATFORM_WIN32

NAMESPACE_UPP

float GetFps();

#define CLIP_MODE 2
/*
0 - Scissor clip
1 - Plane clip
2 - Stencil clip
3 - Manual clip
*/

struct OpenGLFont : Moveable<OpenGLFont>
{
	int scaleW;
	int scaleH;
	int lineHeight;
	int base;
	
	bool texturesUpdated;

	float vtx[1024 * 4 * 2];
	float crd[1024 * 4 * 2];

	struct CharInfo : Moveable<CharInfo>
	{
		int id;
		int x;
		int y;
		int width;
		int height;
		int xoffset;
		int yoffset;
		int xadvance;
		int page;
	};

	Vector<CharInfo> chars;
	VectorMap<int, VectorMap<int, int>> kerns;
	Vector<String> files;
	Vector<int64> pages;

	OpenGLFont() : texturesUpdated(false)
	{}

	~OpenGLFont()
	{}

	void Load(const String& fileName);
	void UpdateTextures();
	void BuildVertices();

};

struct Texture
{
	Size sz;
	Point curpos;
	
	Vector<Rect> parts;
	void Add(const Rect& r);
};

struct Resources
{
	static int64 currentSerialId;
	static Index<int64> textures;
	static VectorMap<String, OpenGLFont> fonts;
	static int64 Bind(const Image& img, bool linear = false);
	static bool Bind(int64 serialId, bool force = false);
	static OpenGLFont& GetFont(const char* fontName);
	static OpenGLFont& StdFont(bool bold = false);
};

class OpenGLDraw : public Draw {
public:
	virtual dword GetInfo() const;
	virtual Size  GetPageSize() const;

	void PlaneEquation(double eq[4], float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
	void SetClipRect(const Rect& r);
	void ScissorClip(const Rect& r);
	void PlaneClip(const Rect& r);
	void StencilClip(const Rect& r, int mode = 0);
	void SetClip(const Rect& r, int mode = 0);

	virtual void BeginOp();
	virtual void EndOp();
	virtual void OffsetOp(Point p);
	virtual bool ClipOp(const Rect& r);
	virtual bool ClipoffOp(const Rect& r);
	virtual bool ExcludeClipOp(const Rect& r);
	virtual bool IntersectClipOp(const Rect& r);
	virtual bool IsPaintingOp(const Rect& r) const;
	virtual Rect GetPaintRect() const;

	virtual	void DrawRectOp(int x, int y, int cx, int cy, Color color);
	virtual void DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color);
	virtual void DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color);

	virtual void DrawPolyPolylineOp(const Point *vertices, int vertex_count,
	                                const int *counts, int count_count,
	                                int width, Color color, Color doxor);
	virtual void DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
	                                   const int *subpolygon_counts, int scc,
	                                   const int *disjunct_polygon_counts, int dpcc,
	                                   Color color, int width, Color outline,
	                                   uint64 pattern, Color doxor);
	virtual void DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color);

	virtual void DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor);
	virtual void DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
		                    Color ink, int n, const int *dx);
	
	virtual Size GetNativeDpi() const;
	virtual void BeginNative();
	virtual void EndNative();

	virtual int  GetCloffLevel() const;

private:

	friend class  FontInfo;
	friend class  Font;

	OpenGLFont fi;
	
	struct Cloff : Moveable<Cloff> {
		bool clipping;
		Point org;
		Rect  drawing_clip;
	};
	
	float current_color[4];
	
	void SaveCurrentColor();
	void RestoreLastColor();
	
public:
	Rect         drawing_clip;
	Size         drawing_size;
	Point 		 drawing_offset;
	Rect         clip;
	float        alpha;
	float        angle;
	
private:
	Array<Cloff> cloff;
	int ci;
	int cn;

	void Reset();

protected:

	OpenGLDraw();
	void InitClip(const Rect& clip);
	void Init();

public:

	static void Flush()        { /*glFlush();*/ }	
	Point    GetOffset() const { return drawing_offset; }
	OpenGLDraw(HDC hdc, Size sz = Size(0, 0));
	virtual ~OpenGLDraw();
	void FlatView();
	void Clear();
};

Size GetTextSize(const wchar *text, const OpenGLFont& fi, int n);

END_UPP_NAMESPACE

#endif
