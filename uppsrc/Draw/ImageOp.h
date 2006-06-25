Image CreateImage(Size sz, Color color);
Image SetColor(const Image& img, Color c);

void  Copy(Image& dest, Point p, const Image& src, const Rect& srect);
void  Over(Image& dest, Point p, const Image& src, const Rect& srect, byte alpha);

void  Crop(RasterEncoder& tgt, Raster& img, const Rect& rc);
Image Crop(const Image& img, const Rect& rc);
Image Crop(const Image& img, int x, int y, int cx, int cy);

void  CanvasSize(RasterEncoder& tgt, Raster& img, int cx, int cy);
Image CanvasSize(const Image& img, int cx, int cy);

Image AssignAlpha(const Image& img, const Image& new_alpha);

Image Grayscale(const Image& img);
Image Grayscale(const Image& img, int amount);
Image Contrast(const Image& img, int amount = 256);

class RescaleImage {
	Raster       *src;
	Size          tsz;
	Vector<dword> horz;
	Vector<dword> vert;
	void        (*row_proc)(dword *dest, const RGBA *src, const dword *map);
	Size          size;
	int           cx4;
	int           count;
	int           segment;
	int           entry;
	int           step;
	int           segspan;
	bool          bigseg;
	Buffer<dword> row_buffers;
	int           first;
	int           full;
	const dword  *offsets;
	int           offset;
	int           y;

public:
	void Create(Size sz, Raster& src, const Rect& src_rc);
	void Get(RGBA *line);
};

void DrawRasterData(Draw& w, int x, int y, int cx, int cy, const String& data);

void  Rescale(RasterEncoder& tgt, Size sz, Raster& src, const Rect& src_rc);
Image Rescale(const Image& src, Size sz, const Rect& src_rc);
Image Rescale(const Image& src, Size sz);
Image Rescale(const Image& src, int cx, int cy);

struct ImageFilter9 {
	virtual RGBA operator()(const RGBA **mx) = 0;
};

Image Filter(const Image& img, ImageFilter9& filter);

Image Etched(const Image& img);
Image Sharpen(const Image& img, int amount = 100);

Image RotateClockwise(const Image& img);
Image RotateAntiClockwise(const Image& img);

// Experimental {
Image Colorize(const Image& img, Color color, int alpha = 100);
Image Equalight(const Image& img, int thold = 10);
Image Over(const Image& img, const Image& over, byte alpha = 255);
// }


struct ImageMaker {
	virtual String Key() const = 0;
	virtual Image  Make() const = 0;
	virtual ~ImageMaker() {}
};

Image MakeImage(const ImageMaker& m);

void RegisterImageOp(const char *id, void (*op)(Vector<Image>& m, CParser& p, void *fn), void *fn);
void RegisterImageOp(const char *id, void (*op)(Vector<Image>& m, CParser& p));
void RegisterImageOp(const char *id, Image (*op0)(CParser& p));
void RegisterImageOp(const char *id, Image (*opn)(const Image& img));
void RegisterImageOp(const char *id, Image (*op1)(const Image& img, CParser& p));
void RegisterImageOp(const char *id, Image (*op2)(const Image& img1, const Image& img2, CParser& p));

Image MakeImage(const Vector<Image>& in, const char *code);
Image MakeImage(const char *code);
Image MakeImage(const Image& in, const char *code);
