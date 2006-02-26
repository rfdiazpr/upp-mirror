class PixelArray;
class AlphaArray;

#ifdef PLATFORM_WIN32
#ifdef COMPILER_MSC
inline unsigned GetHashValue(const HICON& h)
{
	return (unsigned)h;
}
#endif
#endif

class Image : public AssignValueTypeNo<Image, 150, Moveable<Image> >
{
public:
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
	class Data;

	friend class Data;

	class Bitmap
	{
		friend class BitmapDraw;
		friend class Data;

	public:
		Bitmap() : data(Data::Empty())                           { data -> AddRef(); }
		Bitmap(bool mono);
		Bitmap(Size size, bool mono);
		Bitmap(int cx, int cy, bool mono);
		Bitmap(const Bitmap& bmp) : data(bmp.data)               { data -> AddRef(); }
#ifdef PLATFORM_WIN32
		Bitmap(HBITMAP bitmap);
#endif
		~Bitmap()                                                { data -> Release(); }

#ifdef PLATFORM_WIN32
		HBITMAP  GetHBITMAP(HDC hdc = ScreenHDC())              { Owner(); return data -> bitmap; }
#endif

#ifdef PLATFORM_X11
		Pixmap   GetPixmap()                                     { Owner(); return data -> pixmap; }
#endif

		bool     IsEmpty() const                                 { return data -> size.cx == 0 || data -> size.cy == 0; }
		operator bool () const                                   { return !IsEmpty(); }
		Size     GetSize() const                                 { return data -> size; }
#ifdef PLATFORM_WIN32
		bool     IsCreated() const                               { return data -> bitmap; }
#endif
#ifdef PLATFORM_X11
		bool     IsCreated() const                               { return data -> pixmap; }
#endif

		void     Clear();

		void     Paint(Draw& draw, const Rect& dest, const Rect& src, PixelOp rop) const;
		void     Paint(Draw& draw, Point dest, const Rect& src, PixelOp rop) const { Paint(draw, Rect(dest, src.Size()), src, rop); }
		void     Paint(Draw& draw, Point dest, PixelOp rop) const                  { Paint(draw, dest, GetSize(), rop); }

#ifdef PLATFORM_WIN32
		void     SetSize(int cx, int cy, HDC hdc = ScreenHDC()) { Clone(Size(cx, cy), hdc); }
		void     SetSize(Size new_size, HDC hdc = ScreenHDC())  { Clone(GetSize(), hdc); }
		void     Create(Size new_size, HDC hdc = ScreenHDC());
		void     Create(HDC hdc = ScreenHDC())                  { Create(GetSize(), hdc); }
		void     Clone(Size new_size, HDC hdc = ScreenHDC());
		void     Clone(HDC hdc = ScreenHDC())                   { Clone(GetSize(), hdc); }
		void     Owner(Size new_size, HDC hdc = ScreenHDC())    { SetSize(new_size, hdc); if(!IsCreated()) Create(new_size, hdc); }
		void     Owner(HDC hdc = ScreenHDC())                   { Clone(hdc); if(!IsCreated()) Create(hdc); }

		void     PaintDC(HDC hdc, const Rect& dest, const Rect& src, int rop = SRCCOPY) const;
		void     PaintDC(HDC hdc, Point dest, const Rect& src, int rop = SRCCOPY) const { PaintDC(hdc, Rect(dest, src.Size()), src, rop); }
		void     PaintDC(HDC hdc, Point dest, int rop = SRCCOPY) const                  { PaintDC(hdc, dest, GetSize(), rop); }
#endif

#ifdef PLATFORM_X11
		void     SetSize(int cx, int cy)                         { Clone(Size(cx, cy)); }
		void     SetSize(Size new_size)                          { Clone(GetSize()); }
		void     Create(Size new_size);
		void     Create()                                        { Create(GetSize()); }
		void     Clone(Size new_size);
		void     Clone()                                         { Clone(GetSize()); }
		void     Owner(Size new_size)                            { SetSize(new_size); if(!IsCreated()) Create(new_size); }
		void     Owner()                                         { Clone(); if(!IsCreated()) Create(); }

		void     PaintGC(GC gc, Drawable dw, const Vector<Rect>& clip, const Rect& dest, const Rect& src, int rop) const;
		void     PaintGC(GC gc, Drawable dw, const Vector<Rect>& clip, Point dest, const Rect& src, int rop) const { PaintGC(gc, dw, clip, Rect(dest, src.Size()), src, rop); }
		void     PaintGC(GC gc, Drawable dw, const Vector<Rect>& clip, Point dest, int rop) const                  { PaintGC(gc, dw, clip, dest, GetSize(), rop); }
#endif

	private:
		class Data
		{
		public:
#ifdef PLATFORM_WIN32
			Data(Size size = Size(0, 0), bool mono = false, HBITMAP bitmap = NULL, int refcount = 0)
				: size(size), mono(mono), bitmap(bitmap), refcount(refcount) {}
#endif
#ifdef PLATFORM_X11
			Data(Size size = Size(0, 0), bool mono = false, Pixmap pixmap = 0, int refcount = 0)
				: size(size), mono(mono), pixmap(pixmap), refcount(refcount) {}
#endif
			~Data();

			static Data *Empty();

			void         AddRef()         { AtomicInc(refcount); }
			void         Release()        { if(AtomicDec(refcount) == 0) delete this; }
			bool         IsSingle() const { return refcount == 1; }

		public:
			Size         size;
			bool         mono;
#ifdef PLATFORM_WIN32
			HBITMAP      bitmap;
#endif
#ifdef PLATFORM_X11
			Pixmap       pixmap;
#endif
			Atomic       refcount;
		};

		Data    *data;
	};

	class Slot
	{
	public:
		Slot() : size(0, 0) {}
		Slot(int height, bool mono) : size(0, height), bitmap(mono) {}

		Size     GetSize() const { return size; }
		void     Add(int width);

	public:
		Bitmap   bitmap;

	protected:
		Size     size;
	};

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

	class Data : public AlphaImageInfo
	{
	public:
		enum _VOIDDATA { VOIDDATA };
		Data(_VOIDDATA);
		Data(const Data& d, Draw& draw = ScreenInfo());
		Data(const char *const *rle = 0, int rle_count = 0, Size size = Size(0, 0),
			int encoding = COLOR_RLE, Point hotspot = Point(0, 0), int effect = 0);
		~Data();

		Data              *AddRef()                   { AtomicInc(refcount); return this; }
		void               Release()                  { if(AtomicDec(refcount) == 0)
		                                                { ASSERT(this != GetVoidData()); delete this; } }
		bool               IsEmpty() const            { return size.cx <= 0 || size.cy <= 0; }

		bool               Instantiate(bool force_data);
		bool               MakeOwner(Draw& draw, bool force_data);
		bool               MakeOwner(Draw& draw, bool force_data, const AlphaArray& source);
		Data              *Clone(Draw& draw, bool force_data);
		void               Prepare(AlphaArray& imb);

		void               PaintData(Draw& draw, const Rect& src, const Rect& dest, PixelOp rop);
		void               PaintMask(Draw& draw, const Rect& src, const Rect& dest,
			Color fore, Color back, PixelOp rop0 = PIXEL_NOP);

		void               FreeCursor();

#ifdef PLATFORM_WIN32
		void               FreeImages();
		void               FreeBrushes();
		void               FreeBitmaps();
#endif

	public:
		Atomic             refcount;
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

#ifdef PLATFORM_WIN32
		// allocated Windows objects
		HCURSOR            cursor;
		VectorMap<String, HICON> images;
		VectorMap<String, HBRUSH> brushes;
		VectorMap<String, HBITMAP> bitmaps;
#endif

#ifdef PLATFORM_X11
		// allocated X-Window objects
		Cursor             cursor;
#endif
	};

public:
	Image();
	Image(const Nuller&);
	Image(const Image& i);
	Image(const Image& (*pfn)());
	Image(const Value& value);

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
#endif

	// resource constructors
	Image(const OldInit& init);
	Image(const Init& init);
	~Image()                                  { data -> Release(); }

	operator      Value() const               { return RichValue<Image>(*this); }

	bool          IsVoid() const              { return data == GetVoidData(); }
	bool          IsEmpty() const             { return data -> size.cx == 0; }
	operator      bool () const               { return !IsEmpty(); }

	bool          HasMask() const             { return data -> Instantiate(false) && data -> mask_pos >= 0; }
	void          KillMask();

#ifdef PLATFORM_WIN32
	HBITMAP       GetHBITMAP(Draw& draw = ScreenInfo()) const { MakeOwner(draw, true); return data -> data.GetHBITMAP(); }
	HBITMAP       GetMaskHBITMAP() const;
#endif
#ifdef PLATFORM_X11
	Pixmap        GetPixmap() const           { MakeOwner(ScreenInfo(), true); return data -> data.GetPixmap(); }
	Pixmap        GetMaskPixmap() const;
#endif

	void          Clear()                     { data -> Release(); data = GetVoidData() -> AddRef(); }
	void          Set(Color color); // Null = set transparent

	Image&        operator = (const Image& i) { Data *old = data; data = i.AddRef(); old -> Release(); return *this; }
	void          Set(const Drawing& drawing, Color paper = Null);
	Image         Effect(Draw& draw, int fx) const { return Image(Get(draw, fx) -> AddRef()); }

	void          SetHotSpot(Point pt);

	bool          IsSame(const Image& image) const;

	Size          GetSize() const             { return data -> size; }
	int           GetWidth() const            { return data -> size.cx; }
	int           GetHeight() const           { return data -> size.cy; }
	Rect          GetRect() const             { return data -> size; }
	Point         GetHotSpot() const          { return data -> hotspot; }
	int           GetEncoding() const         { return data -> encoding; }
	Size          RatioSize(int cx, int cy) const;
	Size          RatioSize(Size sz) const    { return RatioSize(sz.cx, sz.cy); }
	Size          FitSize(int cx, int cy) const;
	Size          FitSize(Size sz) const      { return FitSize(sz.cx, sz.cy); }

	dword         GetHashValue() const;
	void          Serialize(Stream& stream);
	String        AsString() const;

	void          Paint(Image& on_image, Point pos = Point(0, 0)) const { Paint(on_image, GetRect(), Rect(pos, GetSize())); }
	void          Paint(Image& on_image, const Rect& dest) const        { Paint(on_image, GetRect(), dest); }
	void          Paint(Image& on_image, const Rect& src, const Rect& dest) const;

	void          PaintImage(Draw& draw, const Rect& src, const Rect& dest, int fx = 0) const;
	void          PaintImage(Draw& draw, const Rect& src, const Rect& dest, Color fore, Color back, Color doxor = Null) const;
	void          PaintData(Draw& draw, const Rect& src, const Rect& dest, int fx = 0) const;
	void          PaintMask(Draw& draw, const Rect& src, const Rect& dest, Color fore, Color back, int fx = 0) const;

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

#ifdef PLATFORM_X11
	Cursor        GetCursor() const;
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

#ifdef PLATFORM_WIN32
	// standard Windows icons
	static const Image& Application();                 // application icon
	static const Image& Asterisk();                    // information message icon
	static const Image& Exclamation();                 // exclamation message icon
	static const Image& Hand();                        // hand icon (serious messages)
	static const Image& Question();                    // question mark
	static const Image& WindowsLogo();                 // windows logo icon
#endif
#ifdef PLATFORM_WIN32
	static const Image& Cursor(HCURSOR cursor);        // raw cursor / icon
	static const Image& Cursor(int id);                // bitmap or IDC_...
	static const Image& Cursor(const char *idc);       // just IDC_...
	static const Image& Icon(int id);                  // bitmap or IDI_...
	static const Image& Icon(const char *idi);         // just IDI_...
	static const Image& Res(int id);                   // resource bitmap

	static const Image& Icon(int id, bool small_icon); // resource icon
#endif

#ifdef PLATFORM_X11
	static Image FontCursor(int id);
#endif


private:
	Image(Data *data) : data(data)            {}
	Data         *AddRef() const              { return data -> AddRef(); }
	static Data  *GetVoidData();
	void          Set(Data *n)                { Data *old = data; data = n; old -> Release(); }

	Data         *Get(Draw& draw, int effect) const;

#ifdef PLATFORM_WIN32
	Size          Extract(HBITMAP& data, HBITMAP& mask, Size size, Color fore = Null) const;
#endif

	void          Create(const Draw& draw, int cx, int cy);
	void          Create(const Draw& draw, int cx, int cy, const Drawing& drawing, Color paper);

#ifdef PLATFORM_WIN32
	static Data  *FromImage(HCURSOR);
#endif

private:
	Data         *data;
};

template<>
inline unsigned     GetHashValue(const Image& image)             { return image.GetHashValue(); }

template<>
inline bool         IsNull(const Image& image)                   { return image.IsEmpty(); }

inline const Image& Nvl(const Image& a, const Image& b)          { return IsNull(a) ? b : a; }

template<>
inline String       AsString(const Image& image)                 { return image.AsString(); }

inline bool         operator == (const Image& a, const Image& b) { return a.IsSame(b); }
inline bool         operator != (const Image& a, const Image& b) { return !a.IsSame(b); }

/*
enum
{
	IMAGE_TX = 1, IMAGE_TY = 2, IMAGE_TD = 4,
	IMAGE_TXY            = IMAGE_TX | IMAGE_TY,
	IMAGE_TANTICLOCKWISE = IMAGE_TX | IMAGE_TD,
	IMAGE_TCLOCKWISE     = IMAGE_TY | IMAGE_TD,
};
*/

//////////////////////////////////////////////////////////////////////


class ImageCache
{
public:
	struct RegisterName
	{
		RegisterName(const char *name, const Image& image)
		{ ImageCache::Get().AddName(name, image); }
	};

public:
	static ImageCache&              Get();

	Image::Slot&                    Add(Size size, bool mono);

	// name map
	void                            AddName(const char *name, const Image& image) { name_map.FindAdd(name, image); }
	const VectorMap<String, Image>& GetNameMap() const                            { return name_map; }
	const Image&                    operator [] (const String& name) const        { return name_map.Get(name, Image::Void()); }

	static int                      MaxCacheImageSize; // maximum number of pixels (x * y) in image to cache

private:
	ArrayMap<int, Image::Slot>      mono_cache;
	ArrayMap<int, Image::Slot>      color_cache;
	VectorMap<String, Image>        name_map;
};

Image         PixelArrayToImage(const PixelArray& image, Draw& draw = ScreenInfo());
Image         AlphaArrayToImage(const AlphaArray& image, Draw& draw = ScreenInfo());

PixelArray    ImageToPixelArray(const Image& im, Draw& draw = ScreenInfo(), int bpp = 0, int alignment = 4, const dword *cmasks = NULL, const Vector<Color>& palette = Vector<Color>());
PixelArray    MaskToPixelArray(const Image& im, int bpp = 8, int alignment = 4);
AlphaArray    ImageToAlphaArray(const Image& im, Draw& draw = ScreenInfo(),
	int bpp = 0, int alignment = 4, const dword *cmasks = NULL, const Vector<Color>& palette = Vector<Color>(),
	int mask_bpp = 8, int mask_align = 4);

void          ImageAutoMask(Image& image, Color color, int tolerance = 0); // Null = destroy mask
void          ImageKillMask(Image& image, Color bgnd);                     // converts mask to background color
void          ImageCropMask(Image& image);                                 // converts non-masked parts of image to black

Image         ImageCrop(const Image& src, const Rect& rc);
Image         ImageTransform(const Image& src, int flags, Draw& draw = ScreenInfo());
Image         ImageMirrorX(const Image& src, Draw& draw = ScreenInfo());
Image         ImageMirrorY(const Image& src, Draw& draw = ScreenInfo());
Image         ImageMirrorXY(const Image& src, Draw& draw = ScreenInfo());
Image         ImageRotateAnticlockwise(const Image& src, Draw& draw = ScreenInfo());
Image         ImageRotateClockwise(const Image& src, Draw& draw = ScreenInfo());

void          ImageAlphaBlend(Image& dest, Color color, int percent, const Rect& at_least_this_part, int gamma = 2.5);
inline void   ImageAlphaBlend(Image& dest, Color color, int percent, int gamma = 2.5) { ImageAlphaBlend(dest, color, percent, dest.GetRect(), gamma); }

Image         DrawToImage(Draw& draw, const Rect& rc);
Image         ClipboardToImage();
bool          ImageToClipboard(const Image& img, Color back = White, bool empty = true);

#ifdef PLATFORM_WIN32
HMETAFILE     ImageToMetafile(const Image& img);
HENHMETAFILE  ImageToEnhMetafile(const Image& img);
String        ImageToMetafileData(const Image& img, bool enhanced = false);
#endif

class ImageInfo
{
public:
	ImageInfo() : dots(0, 0), size(0, 0), bits_per_pixel(0), src_bits_per_pixel(0), alpha(false) {}

	operator bool () const { return size.cx > 0 && size.cy > 0; }

public:
	Size                     size;               // image size in pixels
	Size                     dots;               // image size in dots
	Point                    hotspot;            // hotspot location
	int                      bits_per_pixel;     // bits per pixel in Load()ed image
	int                      src_bits_per_pixel; // bits per pixel in original image
	bool                     alpha;              // alpha channel may be present
	VectorMap<String, Value> extra;              // format-specific information
};

class ImageEncoder;

typedef One<ImageEncoder> (*NewEncoderProc)();
typedef Vector<NewEncoderProc> EncoderMap;

class ImageEncoder
{
public:
	ImageEncoder(int save_bpp = 0) : save_bpp(save_bpp), dot_size(0, 0) {}
	virtual ~ImageEncoder() {}

	static EncoderMap&        StdMap();
	static void               AddStdMap(NewEncoderProc proc);

	ImageEncoder&             DotSize(Size _dot_size)  { dot_size = _dot_size; return *this; }

	virtual void              SaveRaw(Stream& stream, const Vector<const AlphaArray *>& pages) = 0;
	virtual Array<AlphaArray> LoadRaw(Stream& stream, const Vector<int>& page_index) = 0;
	virtual Array<ImageInfo>  InfoRaw(Stream& stream) = 0;

	void                      SaveArray(Stream& stream, const AlphaArray& array);
	String                    SaveArray(const AlphaArray& array);
	bool                      SaveArrayFile(const char *fn, const AlphaArray& array);

	void                      SaveImage(Stream& stream, const Image& image);
	String                    SaveImage(const Image& image);
	bool                      SaveImageFile(const char *fn, const Image& image);

	AlphaArray                LoadArray(Stream& stream, int page_index = 0);
	AlphaArray                LoadArray(const String& s, int page_index = 0);
	AlphaArray                LoadArrayFile(const char *fn, int page_index = 0);

	Image                     LoadImage(Stream& stream, int page_index = 0);
	Image                     LoadImage(const String& s, int page_index = 0);
	Image                     LoadImageFile(const char *fn, int page_index = 0);

	Array<ImageInfo>          InfoArray(Stream& stream);
	Array<ImageInfo>          InfoArray(const String& s);
	Array<ImageInfo>          InfoArrayFile(const char *fn);

	ImageInfo                 InfoImage(Stream& stream);
	ImageInfo                 InfoImage(const String& s);
	ImageInfo                 InfoImageFile(const char *fn);

	void                      SerializeAlpha(Stream& stream, AlphaArray& image);
	void                      SerializeImage(Stream& stream, Image& image);

protected:
	Size                      dot_size;
	int                       save_bpp;
};

class RleEncoder : public ImageEncoder
{
public:
	RleEncoder();
	virtual ~RleEncoder();

	virtual void              SaveRaw(Stream& stream, const Vector<const AlphaArray *>& pages);
	virtual Array<AlphaArray> LoadRaw(Stream& stream, const Vector<int>& page_index);
	virtual Array<ImageInfo>  InfoRaw(Stream& stream);
};

extern One<ImageEncoder>  MapInfoArray(const EncoderMap& map, Stream& stream, Array<ImageInfo>& info);
inline One<ImageEncoder>  MapInfoArray(const EncoderMap& map, const String& s, Array<ImageInfo>& info)    { StringStream ss(s); return MapInfoArray(map, ss, info); }
inline One<ImageEncoder>  MapInfoArrayFile(const EncoderMap& map, const char *fn, Array<ImageInfo>& info) { FileIn fi(fn); return MapInfoArray(map, fi, info); }

extern One<ImageEncoder>  MapInfoImage(const EncoderMap& map, Stream& stream, ImageInfo& info);
inline One<ImageEncoder>  MapInfoImage(const EncoderMap& map, const String& s, ImageInfo& info)           { StringStream ss(s); return MapInfoImage(map, ss, info); }
inline One<ImageEncoder>  MapInfoImageFile(const EncoderMap& map, const char *fn, ImageInfo& info)        { FileIn fi(fn); return MapInfoImage(map, fi, info); }

extern Array<AlphaArray>  MapLoadArray(const EncoderMap& map, Stream& stream, const Vector<int>& page_index);

extern AlphaArray         MapLoadArray(const EncoderMap& map, Stream& stream, int page_index = 0);
extern AlphaArray         MapLoadArray(const EncoderMap& map, const String& s, int page_index = 0);
extern AlphaArray         MapLoadArrayFile(const EncoderMap& map, const char *fn, int page_index = 0);

extern Image              MapLoadImage(const EncoderMap& map, Stream& stream, int page_index = 0);
extern Image              MapLoadImage(const EncoderMap& map, const String& s, int page_index = 0);
extern Image              MapLoadImageFile(const EncoderMap& map, const char *fn, int page_index = 0);

inline One<ImageEncoder>  StdInfoArray(Stream& stream, Array<ImageInfo>& info)        { return MapInfoArray(ImageEncoder::StdMap(), stream, info); }
inline One<ImageEncoder>  StdInfoArray(const String& s, Array<ImageInfo>& info)       { return MapInfoArray(ImageEncoder::StdMap(), s, info); }
inline One<ImageEncoder>  StdInfoArrayFile(const char *fn, Array<ImageInfo>& info)    { return MapInfoArrayFile(ImageEncoder::StdMap(), fn, info); }

inline One<ImageEncoder>  StdInfoImage(Stream& stream, ImageInfo& info)               { return MapInfoImage(ImageEncoder::StdMap(), stream, info); }
inline One<ImageEncoder>  StdInfoImage(const String& s, ImageInfo& info)              { return MapInfoImage(ImageEncoder::StdMap(), s, info); }
inline One<ImageEncoder>  StdInfoImageFile(const char *fn, ImageInfo& info)           { return MapInfoImageFile(ImageEncoder::StdMap(), fn, info); }

inline Array<AlphaArray>  StdLoadArray(Stream& stream, const Vector<int>& page_index) { return MapLoadArray(ImageEncoder::StdMap(), stream, page_index); }

inline AlphaArray         StdLoadArray(Stream& stream, int page_index = 0)            { return MapLoadArray(ImageEncoder::StdMap(), stream, page_index); }
inline AlphaArray         StdLoadArray(const String& s, int page_index = 0)           { return MapLoadArray(ImageEncoder::StdMap(), s, page_index); }
inline AlphaArray         StdLoadArrayFile(const char *fn, int page_index = 0)        { return MapLoadArrayFile(ImageEncoder::StdMap(), fn, page_index); }

inline Image              StdLoadImage(Stream& stream, int page_index = 0)            { return MapLoadImage(ImageEncoder::StdMap(), stream, page_index); }
inline Image              StdLoadImage(const String& s, int page_index = 0)           { return MapLoadImage(ImageEncoder::StdMap(), s, page_index); }
inline Image              StdLoadImageFile(const char *fn, int page_index = 0)        { return MapLoadImageFile(ImageEncoder::StdMap(), fn, page_index); }

ImageInfo                 ClipboardToImageInfo();

#ifndef PLATFORM_WIN32
struct BITMAPFILEHEADER
{
	word    bfType;
	dword   bfSize;
	word    bfReserved1;
	word    bfReserved2;
	dword   bfOffBits;
};

struct BITMAPINFOHEADER
{
	dword      biSize;
	long       biWidth;
	long       biHeight;
	word       biPlanes;
	word       biBitCount;
	dword      biCompression;
	dword      biSizeImage;
	long       biXPelsPerMeter;
	long       biYPelsPerMeter;
	dword      biClrUsed;
	dword      biClrImportant;
};

struct RGBQUAD
{
    byte    rgbBlue;
    byte    rgbGreen;
    byte    rgbRed;
    byte    rgbReserved;
};
#endif

class BmpEncoder : public ImageEncoder
{
public:
	enum BMPFORMAT { BF_BMPFILE, BF_NAKED, BF_ICOFILE };
	BmpEncoder(int save_bpp = 0, BMPFORMAT bmpformat = BF_BMPFILE);
	virtual ~BmpEncoder();

	virtual void              SaveRaw(Stream& stream, const Vector<const AlphaArray *>& pages);
	virtual Array<AlphaArray> LoadRaw(Stream& stream, const Vector<int>& page_index);
	virtual Array<ImageInfo>  InfoRaw(Stream& stream);

	static One<ImageEncoder>  NewBmp()      { return new BmpEncoder(0, BF_BMPFILE); }
	static One<ImageEncoder>  NewIco()      { return new BmpEncoder(0, BF_ICOFILE); }

	static void               RegisterBmp() { AddStdMap(&BmpEncoder::NewBmp); }
	static void               RegisterIco() { AddStdMap(&BmpEncoder::NewIco); }
	static void               Register()    { RegisterBmp(); RegisterIco(); }

private:
	struct Header : public BITMAPINFOHEADER
	{
		RGBQUAD palette[256];
	};

	bool                      InfoRaw(Stream& stream, Header& header);

private:
	BMPFORMAT                 bmpformat;
};
