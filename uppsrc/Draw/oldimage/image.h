//////////////////////////////////////////////////////////////////////
// image.h: rastrovy obrazek, 2. iterace

#ifndef __Graphics_Image__
#define __Graphics_Image__

//////////////////////////////////////////////////////////////////////

//#define OLDIMAGE

class Image
{
public:
	enum ENCODING { COLOR_RLE, MONO_RLE, MONO_PACKED };
	template BitWiseType<ENCODING>;

	enum SOURCE { QUIET, CACHE, OWNER };

	enum // Paint effects
	{
		EFFECT_MASK = 0x0F000000,

		STANDARD    = 0x00000000,
		GRAYED      = 0x01000000,
		ETCHED      = 0x02000000,

		XOR         = 0x10000000,
	};

	class BitmapDraw;

	class Bitmap
	{
		friend class BitmapDraw;

	public:
		Bitmap() : bitmap(0), size(0, 0), refcount(0) {}
		Bitmap(Size size) : bitmap(0), refcount(0), size(size) {}
		Bitmap(int cx, int cy) : bitmap(0), refcount(0), size(cx, cy) {}
		Bitmap(const Bitmap& bmp) : bitmap(bmp.bitmap), refcount(bmp.refcount), size(bmp.size)
		{ if(refcount) MtInc(*refcount); }
		Bitmap(HBITMAP bitmap);
		~Bitmap() { Clear(); }

		HBITMAP  GetHBITMAP(HDC hdc = ScreenInfo())              { Owner(); return bitmap ? *bitmap : 0; }

		bool     IsEmpty() const                                 { return size.cx == 0 || size.cy == 0; }
		operator bool () const                                   { return !IsEmpty(); }
		Size     GetSize() const                                 { return size; }
		bool     IsCreated() const                               { return bitmap; }

		void     Clear();

		void     SetSize(int cx, int cy, HDC hdc = ScreenInfo()) { Clone(Size(cx, cy), hdc); }
		void     SetSize(Size new_size, HDC hdc = ScreenInfo())  { Clone(size, hdc); }
		// HDC = 0 -> mono

		void     Create(Size new_size, HDC hdc = ScreenInfo());
		void     Create(HDC hdc = ScreenInfo())                  { Create(size, hdc); }
		void     Clone(Size new_size, HDC hdc = ScreenInfo());
		void     Clone(HDC hdc = ScreenInfo())                   { Clone(size, hdc); }
		void     Owner(Size new_size, HDC hdc = ScreenInfo())    { SetSize(new_size, hdc); if(!IsCreated()) Create(new_size, hdc); }
		void     Owner(HDC hdc = ScreenInfo())                   { Clone(hdc); if(!IsCreated()) Create(hdc); }

		void     Paint(HDC hdc, const Rect& dest, const Rect& src, int rop = SRCCOPY) const;
		void     Paint(HDC hdc, Point dest, const Rect& src, int rop = SRCCOPY) const { Paint(hdc, Rect(dest, src.Size()), src, rop); }
		void     Paint(HDC hdc, Point dest, int rop = SRCCOPY) const                  { Paint(hdc, dest, size, rop); }

	private:
		HBITMAP *bitmap;
		int     *refcount;
		Size     size;
	};

	class BitmapDraw : public Draw
	{
	public:
		BitmapDraw() {}
		BitmapDraw(HDC hdc, Bitmap& image)     { Open(hdc, image); }
		BitmapDraw(Bitmap& image)              { Open(ScreenInfo(), image); }
		~BitmapDraw()                          { Close(); }

		void Open(Bitmap& image)               { Open(ScreenInfo(), image); }
		void Open(HDC hdc, Bitmap& image);
		void Close();
	};

	class Slot
	{
	public:
		Slot() : size(0, 0), mono(false) {}
		Slot(int height, bool mono) : size(0, height), mono(mono) {}

		Size     GetSize() const { return size; }
		void     Add(int width);

	public:
		Bitmap   bitmap;

	protected:
		Size     size;
		bool     mono;
	};
	template class Moveable<Slot>;

	struct OldInit
	{
		short cx, cy, scan_count;
		const char *const *scans;
	};

	struct Init
	{
		const char *const *scans;
		const char *info;
		short scan_count, info_count;

	};

	class Info
	{
	public:
		Info(Size size = Size(0, 0), ENCODING encoding = COLOR_RLE, Point hotspot = Point(0, 0))
			: size(size), encoding(encoding), hotspot(hotspot) {}

		void               Serialize(Stream& stream);

	public:
		ENCODING           encoding;
		Size               size;
		Point              hotspot;
	};

	class Data : public Info
	{
	public:
		Data(const Data& d, HDC hdc = ScreenInfo());
		Data(const char *const *rle = 0, int rle_count = 0, Size size = Size(0, 0),
			ENCODING encoding = COLOR_RLE, Point hotspot = Point(0, 0), int effect = 0);
		~Data();

		Data              *AddRef()                   { InterlockedIncrement(&refcount); return this; }
		void               Release()                  { if(this && InterlockedDecrement(&refcount) == 0) delete this; }

		bool               IsEmpty() const            { return size.cx <= 0 || size.cy <= 0; }

		bool               Instantiate(bool force_data);
		bool               MakeOwner(HDC hdc, bool force_data);
		Data              *Clone(HDC hdc, bool force_data);
		void               Prepare(class ImageBuffer& imb);

#ifdef PLATFORM_WIN32
		void               PaintData(HDC hdc, const Rect& src, const Rect& dest, int rop);
		void               PaintMask(HDC hdc, const Rect& src, const Rect& dest, Color fore, Color back, int rop0);

		void               FreeCursor();
		void               FreeImages();
		void               FreeBrushes();
		void               FreeBitmaps();
#endif

	public:
		long               refcount;
		SOURCE             source;
		const char *const *rle;
		int                rle_count;
		int                effect;
		Slot              *data_slot;
		int                data_pos;
		Slot              *mask_slot;
		int                mask_pos; // >= 0 - has mask (even when mask_slot == 0)
		Data              *grayed, *etched;

		// explicit content
		Bitmap             data, mask;

		// allocated Windows objects
		HCURSOR            cursor;
		VectorMap<String, HICON> images;
		VectorMap<String, HBRUSH> brushes;
		VectorMap<String, HBITMAP> bitmaps;
	};

public:
	Image();
	Image(const Nuller&);
	Image(const Image& i);
	Image(const Value& value);
	Image(const Image& (*pfn)()) : data(pfn().AddRef()) {}

	Image(int cx, int cy)                     { Create(ScreenInfo(), cx, cy); }
	Image(Size size)                          { Create(ScreenInfo(), size.cx, size.cy); }
	Image(const Draw& draw, int cx, int cy)   { Create(draw, cx, cy); }
	Image(const Draw& draw, Size size)        { Create(draw, size.cx, size.cy); }

	Image(int cx, int cy, const Drawing& drawing, Color paper)             { Create(ScreenInfo(), cx, cy, drawing, paper); }
	Image(Size size, const Drawing& drawing, Color paper)                  { Create(ScreenInfo(), size.cx, size.cy, drawing, paper); }
	Image(Draw& draw, int cx, int cy, const Drawing& drawing, Color paper) { Create(draw, cx, cy, drawing, paper); }
	Image(Draw& draw, Size size, const Drawing& drawing, Color paper)      { Create(draw, size.cx, size.cy, drawing, paper); }

#ifdef PLATFORM_WIN32
	Image(HBITMAP data, HBITMAP mask = 0);
#endif//PLATFORM_WIN32

	// resource constructors
	Image(const OldInit& init);
	Image(const Init& init);
	~Image()                                  { data -> Release(); }

	operator      Value() const               { return RichValue<Image>(*this); }

	bool          IsVoid() const              { return data == GetVoidData(); }
	bool          IsEmpty() const             { return data -> size.cx == 0; }
	operator      bool () const               { return !IsEmpty(); }

	bool          HasMask() const             { return data -> Instantiate(false) && data -> mask_pos >= 0; }

#ifdef PLATFORM_WIN32
	HBITMAP       GetHBITMAP(Draw& draw = ScreenInfo()) const { MakeOwner(draw, true); return data -> data.GetHBITMAP(); }
	HBITMAP       GetMaskHBITMAP() const;

	HMETAFILE     GetMetafile() const;
	HENHMETAFILE  GetEnhMetafile() const;
	String        GetMetafileData(bool enhanced = false) const;
#endif//PLATFORM_WIN32

	void          Clear()                     { data -> Release(); data = GetVoidData() -> AddRef(); }
	void          Set(Color color); // Null = set transparent
	void          AutoMask(Color color, int tolerance = 0); // Null = destroy mask
	void          KillMask(Color bgnd); // converts mask to background color
	void          CropMask();           // converts non-masked parts of image to black

	Image&        operator = (const Image& i) { Data *old = data; data = i.AddRef(); old -> Release(); return *this; }
	void          Set(const Drawing& drawing, Color paper = Null);
	Image         Effect(int fx) const        { return Image(Get(fx) -> AddRef()); }

	void          SetHotSpot(Point pt);

	bool          IsSame(const Image& image) const;

	Size          GetSize() const             { return data -> size; }
	Rect          GetRect() const             { return data -> size; }
	Point         GetHotSpot() const          { return data -> hotspot; }
	ENCODING      GetEncoding() const         { return data -> encoding; }
	Size          RatioSize(int cx, int cy) const;
	Size          RatioSize(Size sz) const    { return RatioSize(sz.cx, sz.cy); }
	Size          FitSize(int cx, int cy) const;
	Size          FitSize(Size sz) const      { return FitSize(sz.cx, sz.cy); }

	dword         GetHashValue() const;
	void          Serialize(Stream& stream);
	String        ToString() const;

#ifdef OLDIMAGE
	// color: paints color bitmap or mono bitmap black on white
	void          Paint(Draw& draw, Point pos = Point(0, 0), int fx = 0) const;
	void          Paint(Draw& draw, const Rect& dest, int fx = 0) const;
	void          Paint(Draw& draw, const Rect& src, const Rect& dest, int fx = 0) const;//->Draw

	// mono: paints bitmap mask for coloured bitmaps, bitmap data for mono bitmaps
	// When rop == 0, fore or back may be Null in which case a suitable ROP
	// is applied
	void          Paint(Draw& draw, Point pos, Color fore, Color back, bool xor = false) const;
	void          Paint(Draw& draw, const Rect& dest, Color fore, Color back, bool xor = false) const;
	void          Paint(Draw& draw, const Rect& src, const Rect& dest, Color fore, Color back, bool xor = false) const;//->Draw
#endif

	void          Paint(Image& image, Point pos = Point(0, 0)) const;
	void          Paint(Image& image, const Rect& dest) const;
	void          Paint(Image& image, const Rect& src, const Rect& dest) const;

#ifdef PLATFORM_WIN32
	void          Paint(HDC hdc, const Rect& src, const Rect& dest, int fx) const;
	void          Paint(HDC hdc, const Rect& src, const Rect& dest, Color fore, Color back,
		                bool xor = false) const;

	void          PaintData(HDC hdc, Point pos, int fx_rop = 0) const;
	void          PaintData(HDC hdc, const Rect& dest, int fx_rop = 0) const;
	void          PaintData(HDC hdc, const Rect& src, const Rect& dest, int fx_rop = 0) const;

	void          PaintMask(HDC hdc, Point pos, Color fore, Color back, int fx_rop = 0) const;
	void          PaintMask(HDC hdc, const Rect& dest, Color fore, Color back, int fx_rop = 0) const;
	void          PaintMask(HDC hdc, const Rect& src, const Rect& dest, Color fore, Color back, int fx_rop = 0) const;
#endif

#ifdef PLATFORM_WIN32
	HCURSOR       GetCursor() const;
	HICON         GetImage(Point hotspot = Point(-1, -1), Size size = Size(0, 0), Color fore = Null) const;
	HICON         GetIcon() const { return GetImage(); }

	// fore == Null - use color bitmap or mono bitmap black on white
	// fore != Null - use mono bitmap or color bitmap mask
	// back == Null - create mono bitmap from mask
	HBITMAP       GetBitmap(Color fore = Null, Color back = White) const { return GetBitmap(GetRect(), fore, back); }
	HBITMAP       GetBitmap(const Rect& rc, Color fore = Null, Color back = White) const; // cropped
	HBRUSH        GetBrush(Color fore = Null, Color back = White) const;

	bool          CreateCaret(HWND hwnd, const Rect& clip_rect) const;
	bool          CreateCaret(HWND hwnd) const { return CreateCaret(hwnd, GetRect()); }
#endif

	Image&        Clone(Draw& draw, bool force_data)           { Set(data -> Clone(draw, force_data)); return *this; }
	bool          MakeOwner(Draw& draw, bool force_data) const { return data -> MakeOwner(draw, force_data); }
	bool          Instantiate(bool force_data) const           { return data -> Instantiate(force_data); }

public:
	static const Image& Void();
	static const Image& Empty();

	// standard cursors
	static const Image& AppStarting();                 // arrow & small hourglass
	static const Image& Arrow();                       // standard cursor
	static const Image& Cross();                       // crosshairs
	static const Image& IBeam();                       // edit field cursor
	static const Image& No();                          // slashed circle
	static const Image& SizeAll();                     // four arrows
	static const Image& SizeHorz();                    // left-right arrow
	static const Image& SizeVert();                    // up-down arrow
	static const Image& SizeSlash();                   // '/' - direction arrows
	static const Image& SizeBackSlash();               // '\' - direction arrows
	static const Image& UpArrow();                     // vertical arrow
	static const Image& Wait();                        // hourglass

	// standard Windows icons
	static const Image& Application();                 // application icon
	static const Image& Asterisk();                    // information message icon
	static const Image& Exclamation();                 // exclamation message icon
	static const Image& Hand();                        // hand icon (serious messages)
	static const Image& Question();                    // question mark
	static const Image& WindowsLogo();                 // windows logo icon

#ifdef PLATFORM_WIN32
	static const Image& Cursor(HCURSOR cursor);        // raw cursor / icon
	static const Image& Cursor(int id);                // bitmap or IDC_...
	static const Image& Cursor(const char *idc);       // just IDC_...
	static const Image& Icon(int id);                  // bitmap or IDI_...
	static const Image& Icon(const char *idi);         // just IDI_...
	static const Image& Res(int id);                   // resource bitmap

	static const Image& Icon(int id, bool small_icon); // resource icon
#endif//PLATFORM_WIN32

private:
	Image(Data *data) : data(data)            {}
	Data         *AddRef() const              { return data -> AddRef(); }
	static Data  *GetVoidData();
	void          Set(Data *n)                { Data *old = data; data = n; old -> Release(); }

	Data         *Get(int effect) const;
	Size          Extract(HBITMAP& data, HBITMAP& mask, Size size, Color fore = Null) const;

	void          Create(const Draw& draw, int cx, int cy);
	void          Create(const Draw& draw, int cx, int cy, const Drawing& drawing, Color paper);

#ifdef PLATFORM_WIN32
	static Data  *FromImage(HCURSOR);
#endif//PLATFORM_WIN32

private:
	Data         *data;
};

template AssignValueTypeNo<Image, 150>;
template class Moveable<Image>;

inline dword        GetHashValue(const Image& image)             { return image.GetHashValue(); }
inline bool         IsNull(const Image& image)                   { return image.IsEmpty(); }
inline const Image& Nvl(const Image& a, const Image& b)          { return IsNull(a) ? b : a; }

inline bool         operator == (const Image& a, const Image& b) { return a.IsSame(b); }
inline bool         operator != (const Image& a, const Image& b) { return !a.IsSame(b); }

enum
{
	IMAGE_TX = 1, IMAGE_TY = 2, IMAGE_TD = 4,
	IMAGE_TXY            = IMAGE_TX | IMAGE_TY,
	IMAGE_TANTICLOCKWISE = IMAGE_TX | IMAGE_TD,
	IMAGE_TCLOCKWISE     = IMAGE_TY | IMAGE_TD,
};

Image         Crop(const Image& src, const Rect& rc);
Image         Transform(const Image& src, int flags, Draw& draw = ScreenInfo());
inline Image  MirrorX(const Image& src, Draw& draw = ScreenInfo())              { return Transform(src, IMAGE_TX, draw); }
inline Image  MirrorY(const Image& src, Draw& draw = ScreenInfo())              { return Transform(src, IMAGE_TY, draw); }
inline Image  MirrorXY(const Image& src, Draw& draw = ScreenInfo())             { return Transform(src, IMAGE_TXY, draw); }
inline Image  RotateAnticlockwise(const Image& src, Draw& draw = ScreenInfo())  { return Transform(src, IMAGE_TANTICLOCKWISE, draw); }
inline Image  RotateClockwise(const Image& src, Draw& draw = ScreenInfo())      { return Transform(src, IMAGE_TCLOCKWISE, draw); }

void          AlphaBlend(Image& dest, Color color, int percent, const Rect& at_least_this_part, int gamma = 2.5);
inline void   AlphaBlend(Image& dest, Color color, int percent, int gamma = 2.5) { AlphaBlend(dest, color, percent, dest.GetRect(), gamma); }

Image         DrawToImage(Draw& draw, const Rect& rc);
Image         ClipboardToImage();
bool          ImageToClipboard(const Image& img, Color back = White, bool empty = true);

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

class ImageMaskDraw : public Draw
{
public:
	ImageMaskDraw() {}
	ImageMaskDraw(Image& image) { Open(image); }
	~ImageMaskDraw()            { Close(); }

	void Open(Image& image);
	void Close();
};

//////////////////////////////////////////////////////////////////////

class ImageCache
{
public:
	struct RegisterName
	{
		RegisterName(const char *name, const Image& image)
		{ ImageCache::Get().AddName(name, image); }
	};

	typedef VectorMap<String, Image> ImageMap;

public:
	static ImageCache&       Get();

	Image::Slot&             Add(Size size, bool mono);

	// name map
	void                     AddName(const char *name, const Image& image) { name_map.FindAdd(name, image); }
	const ImageMap&          GetNameMap() const                            { return name_map; }
	const Image&             operator [] (const String& name) const        { return name_map(name, Image::Void()); }

	static int               MaxCacheImageSize; // maximum number of pixels (x * y) in image to cache

private:
	SegtorMap<int, Image::Slot> mono_cache;
	SegtorMap<int, Image::Slot> color_cache;
	VectorMap<String, Image> name_map;
};

//////////////////////////////////////////////////////////////////////

#endif//__Graphics_Image__
