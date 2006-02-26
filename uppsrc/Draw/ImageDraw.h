class ImageDraw : public Draw
{
public:
	ImageDraw() {}
	ImageDraw(Draw& draw, Image& image)  { Open(draw, image); }
	ImageDraw(Image& image)              { Open(ScreenInfo(), image); }
	~ImageDraw()                         { Close(); }

	void Open(Image& image)              { Open(ScreenInfo(), image); }
	void Open(Draw& draw, Image& image);
	void Close();
};

class ImageMaskDraw : public Draw
{
public:
	ImageMaskDraw() {}
	ImageMaskDraw(Image& image) { Open(image); }
	~ImageMaskDraw()            { Close(); }

	void Open(Image& image);
	void Close();
};
