//////////////////////////////////////////////////////////////////////
// image.h: rastrovy obrazek, 2. iterace

#include "Draw.h"
#pragma hdrstop

#define LLOG(x) // LOG(x)

#if defined(PLATFORM_WIN32) || defined(PLATFORM_X11)

#include "PixelUtil.h"

#pragma BLITZ_APPROVE

#ifdef CPU_IA32
#define I386_ASSEMBLY
#endif

#ifdef I386_ASSEMBLY
#define BLT_TIMING(routine) //RTIMING(#routine " (asm)")
#else
#define BLT_TIMING(routine) //RTIMING(#routine " (cpp)")
#endif

class Image::BitmapDraw : public Draw
{
public:
	BitmapDraw() {}
#ifdef PLATFORM_WIN32
	BitmapDraw(HDC hdc, Bitmap& image)     { Open(hdc, image); }
#endif
	BitmapDraw(Bitmap& image)              { Open(image); }
	~BitmapDraw()                          { Close(); }

	void Open(Bitmap& image);
#ifdef PLATFORM_WIN32
	void Open(HDC hdc, Bitmap& image);
#endif
	void Close();
};

RichValue<Image>::Registrator MK__s;

/*
static Size GetBitmapSize(HBITMAP bmp)
{
	BITMAP bi;
	if(!GetObject(bmp, sizeof(bi), &bi))
		return Size(0, 0);
	return Size(bmp.bmWidth, bmp.bmHeight);
}

static void PaintBitmap(HBITMAP dest, const Rect& dest_rc, HBITMAP src, const Rect& src_rc, HDC hdc = ScreenInfo())
{
	HDC ddc = CreateCompatibleDC(hdc);
	HDC sdc = CreateCompatibleDC(hdc);
	HGDIOBJ old_dest = SelectObject(ddc, dest);
	HGDIOBJ old_src = SelectObject(sdc, src);
	int old_mode = SetStretchBltMode(ddc, COLORONCOLOR);
	StretchBlt(ddc, dest_rc, sdc, src_rc);
	SelectObject(ddc, old_dest);
	SelectObject(sdc, old_src);
	DeleteDC(ddc);
	DeleteDC(sdc);
}

static void PaintBitmap(HBITMAP dest, Point dest_pos, HBITMAP src, const Rect& src_rc, HDC hdc = ScreenInfo())
{
	HDC ddc = CreateCompatibleDC(hdc);
	HDC sdc = CreateCompatibleDC(hdc);
	HGDIOBJ old_dest = SelectObject(ddc, dest);
	HGDIOBJ old_src = SelectObject(sdc, src);
	BitBlt(ddc, dest_pos, sdc, src_rc);
	SelectObject(ddc, old_dest);
	SelectObject(sdc, old_src);
	DeleteDC(ddc);
	DeleteDC(sdc);
}

static void PaintBitmap(HBITMAP dest, Point dest_pos, HBITMAP src, HDC hdc = ScreenInfo())
{
	PaintBitmap(dest, dest_pos, src, GetBitmapSize(src), hdc);
}

static void PaintBitmap(HBITMAP dest, HBITMAP src, HDC hdc = ScreenInfo())
{
	PaintBitmap(dest, Point(0, 0), src, GetBitmapSize(src), hdc);
}
*/

//////////////////////////////////////////////////////////////////////
// Image::Bitmap::Data::

Image::Bitmap::Data *Image::Bitmap::Data::Empty()
{
	static Data _empty(Size(0, 0), 0, 0, 1);
	return &_empty;
}

Image::Bitmap::Data::~Data()
{
#ifdef PLATFORM_WIN32
	if(bitmap)
		DeleteObject(bitmap);
#endif
#ifdef PLATFORM_X11
	if(pixmap) {
		if(Xdisplay) {
//			fprintf(stderr, "XFreePixmap(%08x)", (int)pixmap);
			XFreePixmap(Xdisplay, pixmap);
		}
		else {
//			fprintf(stderr, "XFreePixmap(%08x) ND", (int)pixmap);
		}
	}
#endif
}

//////////////////////////////////////////////////////////////////////
// Image::Bitmap::

#ifdef PLATFORM_WIN32
Image::Bitmap::Bitmap(HBITMAP _bitmap)
{
	if(_bitmap)
	{
		BITMAP bmp;
		GetObject(_bitmap, sizeof(bmp), &bmp);
		data = new Data(Size(bmp.bmWidth, bmp.bmHeight), bmp.bmPlanes == 1 && bmp.bmBitsPixel == 1, _bitmap, 1);
	}
	else
		(data = Data::Empty()) -> AddRef();
}
#endif

Image::Bitmap::Bitmap(bool mono)
{
	data = new Data(Size(0, 0), mono, 0, 1);
}

Image::Bitmap::Bitmap(Size size, bool mono)
{
	data = new Data(size, mono, 0, 1);
}

Image::Bitmap::Bitmap(int cx, int cy, bool mono)
{
	data = new Data(Size(cx, cy), mono, 0, 1);
}

#ifdef PLATFORM_WIN32
void Image::Bitmap::PaintDC(HDC hdc, const Rect& dest, const Rect& src, int rop) const
{
	if(!data -> bitmap)
		return;
	if(!(GetDeviceCaps(hdc, RASTERCAPS) & RC_STRETCHBLT))
	{
		PixelArray ri = HBITMAPToPixelArray(data -> bitmap, true, 0, ScreenInfo());
		ri.PaintDC(hdc, src, dest, rop);
		return;
	}
	HDC sdc = CreateCompatibleDC(0);
	HGDIOBJ old_bitmap = SelectObject(sdc, data -> bitmap);
	if(!old_bitmap)
	{ // bitmap not compatible with screen context, try destination context
		DeleteDC(sdc);
		sdc = CreateCompatibleDC(hdc);
		old_bitmap = SelectObject(sdc, data -> bitmap);
	}
	if(dest.Size() != src.Size() || dest.Width() > 1024 || dest.Height() > 1024
	|| !BitBlt(hdc, dest.TopLeft(), sdc, src, rop))
	{
		int old_mode = SetStretchBltMode(hdc, COLORONCOLOR);
		for(SmartStretch sms(dest, src, dest); sms.Get();) {
			LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": SmartStretch, " << sms.GetSrc() << " -> " << sms.GetDest());
			if(!StretchBlt(hdc, sms.GetDest(), sdc, sms.GetSrc(), rop))
			{ // todo: why does the stretchblt error occur here?
				PixelArray ri = HBITMAPToPixelArray(data -> bitmap, true, -1);
				ri.PaintDC(hdc, sms.GetSrc(), sms.GetDest(), rop);
//				break; //2004-08-13 cxl - jinak se vytiskla jenom cast
			}
			LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": //SmartStretch");;
		}
		SetStretchBltMode(hdc, old_mode);
	}
	SelectObject(sdc, old_bitmap);
	DeleteDC(sdc);
}
#endif

#ifdef PLATFORM_WIN32
void Image::Bitmap::Create(Size new_size, HDC hdc)
#endif
#ifdef PLATFORM_X11
void Image::Bitmap::Create(Size new_size)
#endif
{
	Clear();
	ASSERT(new_size.cx >= 0 && new_size.cy >= 0);
	if(new_size.cx > 0 && new_size.cy > 0)
	{
#ifdef PLATFORM_WIN32
		data -> bitmap = data -> mono
			? CreateBitmap(new_size.cx, new_size.cy, 1, 1, 0)
			: CreateCompatibleBitmap(hdc, new_size.cx, new_size.cy);
#endif
#ifdef PLATFORM_X11
		data -> pixmap = XCreatePixmap(Xdisplay, Xroot, new_size.cx, new_size.cy, data -> mono ? 1 : Xdepth);
//		fprintf(stderr, "XCreatePixmap(%08x)", (int)data->pixmap);
#endif
	}
	data -> size = new_size;
}

#ifdef PLATFORM_WIN32
void Image::Bitmap::Clone(Size new_size, HDC hdc)
#endif
#ifdef PLATFORM_X11
void Image::Bitmap::Clone(Size new_size)
#endif
{
	ASSERT(new_size.cx >= 0 && new_size.cy >= 0);
	if(new_size.cx == 0 || new_size.cy == 0)
	{
		Clear();
		return;
	}
	if(IsCreated() && (!data -> IsSingle() || new_size != data -> size))
	{
#ifdef PLATFORM_WIN32
		HBITMAP new_bitmap = data -> mono
			? CreateBitmap(new_size.cx, new_size.cy, 1, 1, 0)
			: CreateCompatibleBitmap(hdc, new_size.cx, new_size.cy);
		HDC ddc = CreateCompatibleDC(hdc ? hdc : (HDC)ScreenInfo());
		HGDIOBJ old_bitmap = SelectObject(ddc, new_bitmap);
		PaintDC(ddc, Point(0, 0));
		SelectObject(ddc, old_bitmap);
		DeleteDC(ddc);
		Clear();
		data -> bitmap = new_bitmap;
#endif
#ifdef PLATFORM_X11
		Pixmap new_pixmap = XCreatePixmap(Xdisplay, Xroot, new_size.cx, new_size.cy, data -> mono ? 1 : Xdepth);
//		fprintf(stderr, "XCreatePixmap(%08x)", (int)new_pixmap);
		GC gc = XCreateGC(Xdisplay, new_pixmap, 0, NULL);
		XCopyArea(Xdisplay, data -> pixmap, new_pixmap, gc, 0, 0, data -> size.cx, data -> size.cy, 0, 0);
		XFreeGC(Xdisplay, gc);
		XFreePixmap(Xdisplay, data -> pixmap);
//		fprintf(stderr, "XFreePixmap(%08x)", (int)data->pixmap);
		data -> pixmap = new_pixmap;
#endif
	}
	data -> size = new_size;
}

void Image::Bitmap::Clear()
{
	One<Data> new_data = new Data(Size(0, 0), data -> mono, 0, 1);
	data -> Release();
	data = -new_data;
}

void Image::Bitmap::Paint(Draw& draw, const Rect& dest, const Rect& src, PixelOp rop) const
{
#ifdef PLATFORM_WIN32
	PaintDC(draw.GetHandle(), dest, src, GetPixelROP(rop));
#else
	PaintGC(draw.GetGC(), draw.GetDrawable(), draw.GetClipList(), dest + draw.GetOffset(), src, GetPixelROP(rop));
#endif
}

#ifdef PLATFORM_X11
void Image::Bitmap::PaintGC(GC gc, Drawable dw, const Vector<Rect>& clip, const Rect& dest, const Rect& src, int rop) const
{
	XGCValues gcv;
	int gfn;
	XGetGCValues(Xdisplay, gc, GCFunction, &gcv);
	gfn = gcv.function;
	if(rop != X11_ROP2_NOP)
	{
		gcv.function = rop;
		XChangeGC(Xdisplay, gc, GCFunction, &gcv);
	}
	Size dsize = dest.Size();
	if(dsize == src.Size())
	{
		if(data -> mono)
			XCopyPlane(Xdisplay, data -> pixmap, dw, gc, src.left, src.top, dsize.cx, dsize.cy, dest.left, dest.top, 1);
		else
			XCopyArea(Xdisplay, data -> pixmap, dw, gc, src.left, src.top, dsize.cx, dsize.cy, dest.left, dest.top);
	}
	else
		DrawableToPixelArray(data -> pixmap, src, data -> mono, 0, 0)
		.PaintGC(gc, dw, clip, dest, rop == X11_ROP2_NOP ? gfn : rop);
	if(rop != X11_ROP2_NOP)
	{
		gcv.function = gfn;
		XChangeGC(Xdisplay, gc, GCFunction, &gcv);
	}
}
#endif

#ifdef PLATFORM_WIN32
static HGDIOBJ bitmap_draw_bitmap;

void Image::BitmapDraw::Open(HDC hdc, Bitmap& bitmap)
{
	ASSERT(!bitmap.IsEmpty());
	bitmap.Owner(hdc);
	HDC bdc = CreateCompatibleDC(hdc);
	Attach(bdc);
	if(bitmap.data -> bitmap)
		bitmap_draw_bitmap = ::SelectObject(bdc, bitmap.data -> bitmap);
	backdraw = true;
}
#endif

void Image::BitmapDraw::Open(Bitmap& image)
{
#ifdef PLATFORM_WIN32
	Open(ScreenInfo(), image);
#endif
#ifdef PLATFORM_X11
	gc = XCreateGC(Xdisplay, dw = image.GetPixmap(), 0, NULL);
	clip.Add().Add(Rect(image.GetSize()));
	backdraw = true;
#endif
}

void Image::BitmapDraw::Close()
{
//	while(co.GetCount())
//		End();
#ifdef PLATFORM_WIN32
	HDC hdc = Detach();
	if(hdc)
	{
		SelectObject(hdc, bitmap_draw_bitmap);
		::DeleteDC(hdc);
	}
#endif
#ifdef PLATFORM_X11
	GC gc = GetGC(); // todo: Detach?
	XFreeGC(Xdisplay, gc);
#endif
}

//////////////////////////////////////////////////////////////////////
// Image::Slot::

void Image::Slot::Add(int width)
{
	ASSERT(size.cy > 0);
	if(!bitmap || size.cx + width > bitmap.GetSize().cx)
	{
		int new_width = size.cx ? size.cx + max(size.cx, width) : max(width, 1024 / size.cy);
		bitmap.SetSize(new_width, size.cy);
	}
	size.cx += width;
}

//////////////////////////////////////////////////////////////////////
// Image::Data::

#ifdef LOG_IMAGES
static int total_images = 0;
#endif

Image::Data::Data(_VOIDDATA)
: rle(0)
, rle_count(0)
, effect(0)
, source(QUIET)
, cursor(0)
, data_pos(-1)
, mask_pos(-1)
, mask(true)
{
	AtomicWrite(refcount, 1);
	grayed = AddRef();
	etched = AddRef();
}

Image::Data::Data(const char *const *rle, int rle_count,
	Size size, int encoding, Point hotspot, int effect)
: AlphaImageInfo(size, encoding, hotspot)
, refcount(1)
, rle(rle)
, rle_count(rle_count)
, effect(effect)
, source(QUIET)
, cursor(0)
, data_pos(-1)
, mask_pos(-1)
, mask(true)
{
	grayed = GetVoidData() -> AddRef();
	etched = GetVoidData() -> AddRef();
#ifdef LOG_IMAGES
	total_images++;
	LOG("++images = " << total_images);
#endif
}

Image::Data::Data(const Data& d, Draw& draw)
: AlphaImageInfo(d)
, refcount(1)
, rle(d.rle)
, rle_count(d.rle_count)
, effect(d.effect)
, source(d.source)
, grayed(d.grayed -> AddRef())
, etched(d.etched -> AddRef())
, cursor(0)
, data_pos(d.data_pos)
, mask_pos(d.mask_pos)
, data_slot(d.data_slot)
, mask_slot(d.mask_slot)
, data(d.data)
, mask(d.mask)
{
#ifdef LOG_IMAGES
	total_images++;
	LOG("++images = " << total_images);
#endif
	if(source == OWNER && !IsEmpty())
	{
#ifdef PLATFORM_WIN32
		data.Clone(draw);
		mask.Clone(0);
#else
		data.Clone();
		mask.Clone();
#endif
	}
}

Image::Data::~Data()
{
#ifdef LOG_IMAGES
	total_images--;
	LOG("--images = " << total_images);
#endif
	grayed -> Release();
	etched -> Release();
#ifdef PLATFORM_WIN32
	FreeImages();
	FreeBitmaps();
	FreeBrushes();
#endif
	FreeCursor();
}

void Image::Data::FreeCursor()
{
#ifdef PLATFORM_WIN32
	if(cursor)
	{
		DestroyCursor(cursor);
		cursor = 0;
	}
#endif
#ifdef PLATFORM_X11
	if(cursor) {
		if(Xdisplay) {
			LOG("XFreeCursor(" << FormatIntHex(cursor) << ")");
			XFreeCursor(Xdisplay, cursor);
//			fprintf(stderr, "XFreeCursor(%08x)", cursor);
		}
		else {
//			fprintf(stderr, "XFreeCursor(%08x) ND", cursor);
		}
		cursor = 0;
	}
#endif
}

#ifdef PLATFORM_WIN32
void Image::Data::FreeImages()
{
	if(images.IsEmpty())
		return;
	static HCURSOR arrow = ::LoadCursor(0, IDC_ARROW);
	SetCursor(arrow);
//	HCURSOR hc = ::GetCursor();
//	RLOG("cursor = " << Format("%08x", hc));
	for(int i = 0; i < images.GetCount(); i++)
		if(images[i])
		{
//			RLOG("destroying image " << images[i]);
//			if(hc == images[i])
//			{
//				RLOG("unmounting cursor");
//				static HICON arrow = ::LoadCursor(0, IDC_ARROW);
//				SetCursor(hc = arrow);
//			}
//*
			if(!DestroyIcon(images[i]))
			{
//				RLOG("error destroying image " << Format("%08x", images[i]) << ": " <<
//					GetErrorMessage(GetLastError()));
			}
//*/
		}
	images.Clear();
}
#endif

#ifdef PLATFORM_WIN32
void Image::Data::FreeBrushes()
{
	for(int i = 0; i < brushes.GetCount(); i++)
		if(brushes[i])
			DeleteObject(brushes[i]);
	brushes.Clear();
}
#endif

#ifdef PLATFORM_WIN32
void Image::Data::FreeBitmaps()
{
	for(int i = 0; i < bitmaps.GetCount(); i++)
		if(bitmaps[i])
			DeleteObject(bitmaps[i]);
	bitmaps.Clear();
}
#endif

static void MakeImageEffect(AlphaArray& imb, int effect)
{
	switch(effect & Image::EFFECT_MASK)
	{
	case Image::GRAYED: AlphaGray(imb, imb.GetRect()); break;
	case Image::ETCHED: AlphaEtch(imb, imb.GetRect()); break;
	}
}

void Image::Data::Prepare(AlphaArray& imb)
{
	imb.Create(size, -3);
	AlphaSet(imb, imb.GetRect(), Color(Null));
	if(rle)
	{
		ASSERT(rle_count <= size.cy);
		for(int i = 0; i < rle_count; i++)
			RLEToAlphaScan((const byte *)rle[i], imb.GetPixelDownScan(i), imb.GetAlphaDownScan(i), size.cx);
	}
	MakeImageEffect(imb, effect);
}

bool Image::Data::Instantiate(bool force_data)
{
	if(size.cx == 0 || size.cy == 0)
		return false;
	if(source != QUIET)
		return true;
	if(size.cx * size.cy > ImageCache::MaxCacheImageSize)
	{
		MakeOwner(ScreenInfo(), force_data);
		return true;
	}

	ASSERT(size.cx > 0 && size.cy > 0);
	AlphaArray buffer;
	Prepare(buffer);

	data_slot = &ImageCache::Get().Add(size, false);
	data_pos = data_slot -> GetSize().cx - size.cx;
	{
		BitmapDraw bdraw(data_slot -> bitmap);
		buffer.pixel.Paint(bdraw, Point(data_pos, 0));
	}

	mask_slot = 0;
	mask_pos = -1;

	if(buffer.HasAlpha())
	{
		mask_slot = &ImageCache::Get().Add(size, true);
		mask_pos = mask_slot -> GetSize().cx - size.cx;
		BitmapDraw bdraw(mask_slot -> bitmap);
		buffer.alpha.Paint(bdraw, Rect(size), RectC(mask_pos, 0, size.cx, size.cy));
	}

	source = CACHE;
	return true;
}

bool Image::Data::MakeOwner(Draw& for_draw, bool force_data, const AlphaArray& buffer)
{
	if(buffer.IsEmpty())
		return false;
	Rect rc = size;
	if(force_data)
	{
#ifdef PLATFORM_WIN32
		data.Create(size, for_draw);
		BitmapDraw draw(for_draw, data);
#else
		data.Create(size);
		BitmapDraw draw(data);
#endif
		buffer.pixel.Paint(draw, rc, rc);
	}
	if(buffer.HasAlpha())
	{
		mask_pos = 0;
#ifdef PLATFORM_WIN32
		mask.Create(size, NULL);
#else
		mask.Create(size);
#endif
		BitmapDraw draw(mask);
		buffer.alpha.Paint(draw, rc, rc);
	}
	source = OWNER;
	return true;
}

bool Image::Data::MakeOwner(Draw& for_draw, bool force_data)
{
	if(IsEmpty())
		return false;
	if(source == OWNER)
	{
		if(force_data && !data)
		{ // create data when not instantiated yet
#ifdef PLATFORM_WIN32
			data.Create(size, for_draw);
			BitmapDraw mdraw(for_draw, data);
#else
			data.Create(size);
			BitmapDraw mdraw(data);
#endif
			if(data_pos >= 0 && data_slot)
			{
				data_slot -> bitmap.Paint(mdraw, Point(0, 0), RectC(data_pos, 0, size.cx, size.cy), PIXEL_COPY);
				if(mask_pos >= 0 && mask_slot)
				{
#ifdef PLATFORM_WIN32
					mask.Create(size, NULL);
#else
					mask.Create(size);
#endif
					BitmapDraw xdraw(mask);
					mask_slot -> bitmap.Paint(xdraw, Point(0, 0), RectC(mask_pos, 0, size.cx, size.cy), PIXEL_COPY);
				}
			}
			else if(rle)
			{
				AlphaArray buffer(size);
				Prepare(buffer);
				buffer.pixel.Paint(mdraw, Point(0, 0));
				if(buffer.HasAlpha())
				{
#ifdef PLATFORM_WIN32
					mask.Create(size, NULL);
					BitmapDraw xdraw(NULL, data);
#else
					mask.Create(size);
					BitmapDraw xdraw(mask);
#endif
					buffer.alpha.Paint(xdraw, Point(0, 0));
				}
			}
		}
	}
	else if(source == CACHE)
	{ // instantiated in cache - move into rasters
		Rect rc = size;
		if(force_data)
		{
#ifdef PLATFORM_WIN32
			data.Create(size, for_draw);
			BitmapDraw draw(for_draw, data);
#else
			data.Create(size);
			BitmapDraw draw(data);
#endif
			PaintData(draw, rc, rc, PIXEL_COPY);
		}
		if(mask_pos >= 0)
		{
#ifdef PLATFORM_WIN32
			mask.Create(size, NULL);
#else
			mask.Create(size);
#endif
			BitmapDraw draw(mask);
			PaintMask(draw, rc, rc, Black, White, PIXEL_COPY);
		}
		source = OWNER;
	}
	else if(source == QUIET)
	{ // not instantiated - transfer directly from AlphaArray
		Rect rc = size;
		AlphaArray buffer(size);
		Prepare(buffer);
		MakeOwner(for_draw, force_data, buffer);
	}
	return true;
}

Image::Data *Image::Data::Clone(Draw& draw, bool force_data)
{
	if(IsEmpty())
		return AddRef(); // empty bitmap
	Data *p = (refcount > 1? new Data(*this, draw) : AddRef());
	p -> MakeOwner(draw, force_data);
	return p;
}

#ifdef PLATFORM_WIN32
static void BltData(HDC ddc, const Rect& dest, HDC sdc, const Rect& src, int offset, int rop, int stretch)
{
	Size size = dest.Size();
	if(src.Size() == size)
		BitBlt(ddc, dest.left, dest.top, size.cx, size.cy, sdc, src.left + offset, src.top, rop);
	else
	{
		int mode = SetStretchBltMode(ddc, stretch);
		StretchBlt(ddc, dest.left, dest.top, size.cx, size.cy,
			sdc, src.left + offset, src.top, src.Width(), src.Height(), rop);
		SetStretchBltMode(ddc, mode);
	}
}
#endif

void Image::PaintImage(Draw& draw, const Rect& src, const Rect& dest, int fx) const
{
	if(IsEmpty())
		return;
//	LOG("Image::PaintImage, dest = " << dest << ", clip = " << draw.GetClip());
	Data *p = Get(draw, fx);
	p -> Instantiate(true);
	if(p -> mask_pos >= 0 && !(fx & XOR))
	{
		p -> PaintMask(draw, src, dest, Black, Null);
		p -> PaintData(draw, src, dest, PIXEL_XOR);
	}
	else
		p -> PaintData(draw, src, dest, fx & XOR ? PIXEL_XOR : PIXEL_COPY);
}

void Image::PaintImage(Draw& draw, const Rect& src, const Rect& dest, Color fore, Color back, Color doxor) const
{
	if(!IsNull(doxor))
	{
		fore = Color(fore.GetR() ^ doxor.GetR(), fore.GetG() ^ doxor.GetG(), fore.GetB() ^ doxor.GetB());
		back = Color(back.GetR() ^ doxor.GetR(), back.GetG() ^ doxor.GetG(), back.GetB() ^ doxor.GetB());
	}
	PaintMask(draw, src, dest, fore, back, !IsNull(doxor) ? XOR : 0);
}

void Image::Data::PaintData(Draw& draw, const Rect& src, const Rect& dest, PixelOp rop)
{
	if(src.left == src.right || src.top == src.bottom
		|| dest.left == dest.right || dest.top == dest.bottom || !Instantiate(true))
		return;
	int offset = 0;
	Bitmap& bmp = *(source == CACHE ? offset = data_pos, &data_slot -> bitmap : &data);
	bmp.Paint(draw, dest, src.OffsetedHorz(offset), rop);
//	BltData(hdc, dest, draw, src, offset, rop, COLORONCOLOR);
}

void Image::Data::PaintMask(Draw& draw, const Rect& src, const Rect& dest, Color fore, Color back, PixelOp rop0)
{
	if(src.left == src.right || src.top == src.bottom
		|| dest.left == dest.right || dest.top == dest.bottom || !Instantiate(false) || mask_pos < 0)
		return;
	int offset = 0;
	Bitmap& bmp = *(source == CACHE ? offset = mask_pos, &mask_slot -> bitmap : &mask);
	Rect so = src.OffsetedHorz(offset);
	ASSERT(so.right <= bmp.GetSize().cx && so.bottom <= bmp.GetSize().cy);
	dword text, bk;
#ifdef PLATFORM_X11
	if(dest.Size() != src.Size())
	{
		PixelArray image = DrawableToPixelArray(bmp.data -> pixmap, so, true, 0, 0);
		if(rop0 == PIXEL_NOP)
			image.Paint(draw, dest, fore, back);
		else
			image.PaintGC(draw.GetGC(), draw.GetDrawable(), draw.GetClipList(), dest + draw.GetOffset(), GetPixelROP(rop0));
		return;
	}
	int oldfn;
	XGCValues gcv;
	XGetGCValues(Xdisplay, draw.GetGC(), GCForeground | GCBackground | GCFunction, &gcv);
	text = gcv.foreground;
	bk = gcv.background;
	oldfn = gcv.function;
#endif
	if(rop0 != PIXEL_NOP || !IsNull(fore) && !IsNull(back))
	{ // full blit
		fore = Nvl(fore, Black);
		back = Nvl(back, White);
#ifdef PLATFORM_WIN32
		if(draw.IsMono())
		{
			if(rop0 == PIXEL_NOP)
			{
				static const PixelOp fb_rop[] = { PIXEL_BLACK, PIXEL_NOT_COPY, PIXEL_COPY, PIXEL_WHITE };
				rop0 = fb_rop[(CalcGray(fore) >> 15) + 2 * (CalcGray(back) >> 15)];
			}
		}
		else
		{
			text = SetTextColor(draw, fore);
			bk   = SetBkColor(draw, back);
			LLOG("Image::Data::PaintMask: old text = " << FormatIntHex(text)
				<< ", bk = " << FormatIntHex(bk)
				<< ", fore = " << FormatIntHex(fore)
				<< ", back = " << FormatIntHex(back));
		}
#endif
		if(rop0 == PIXEL_NOP)
			rop0 = PIXEL_COPY;
#ifdef PLATFORM_X11
		gcv.foreground = GetXPixel(Nvl(back, White));
		gcv.background = GetXPixel(Nvl(fore, Black));
		gcv.function = GetPixelROP(rop0);
		XChangeGC(Xdisplay, draw.GetGC(), GCForeground | GCBackground, &gcv);
		rop0 = PIXEL_NOP;
#endif
		bmp.Paint(draw, dest, so, rop0);
	}
	else if(IsNull(fore))
	{
#ifdef PLATFORM_WIN32
		if(draw.IsMono())
			rop0 = PIXEL_NOT_AND;
		else
		{
			text = SetTextColor(draw, White());
			bk   = SetBkColor(draw, Black());
			rop0 = PIXEL_AND;
			LLOG("Image::Data::PaintMask(PIXEL_AND): old text = " << FormatIntHex(text)
				<< ", bk = " << FormatIntHex(bk)
				<< ", fore = White, back = Black");
		}
#endif
#ifdef PLATFORM_X11
		gcv.foreground = 0; //GetXPixel(Black);
		gcv.background = ~0; //GetXPixel(White);
		gcv.function = X11_ROP2_AND;
		XChangeGC(Xdisplay, draw.GetGC(), GCForeground | GCBackground | GCFunction, &gcv);
		rop0 = PIXEL_NOP;
#endif
		bmp.Paint(draw, dest, so, rop0);
#ifdef PLATFORM_WIN32
		if(back != Black && (!draw.IsMono() || CalcGray(back) >= 0x8000))
#endif
#ifdef PLATFORM_X11
		if(gcv.foreground = GetXPixel(back))
#endif
		{
#ifdef PLATFORM_WIN32
			if(!draw.IsMono())
			{
				SetTextColor(draw, Black());
				SetBkColor(draw, back);
			}
			rop0 = PIXEL_OR;
#endif
#ifdef PLATFORM_X11
			gcv.background = 0;
			gcv.function = X11_ROP2_OR;
			XChangeGC(Xdisplay, draw.GetGC(), GCForeground | GCBackground | GCFunction, &gcv);
			rop0 = PIXEL_NOP;
#endif
			bmp.Paint(draw, dest, so, rop0);
		}
	}
	else
	{
#ifdef PLATFORM_WIN32
		if(!draw.IsMono())
		{
			text = SetTextColor(draw, Black());
			bk   = SetBkColor(draw, White());
			LLOG("Image::Data::PaintMask(PIXEL_AND): old text = " << FormatIntHex(text)
				<< ", bk = " << FormatIntHex(bk)
				<< ", fore = Black, back = White");
		}
		rop0 = PIXEL_AND;
#endif
#ifdef PLATFORM_X11
		gcv.foreground = ~0;
		gcv.background = 0;
		gcv.function = X11_ROP2_AND;
		XChangeGC(Xdisplay, draw.GetGC(), GCForeground | GCBackground | GCFunction, &gcv);
		rop0 = PIXEL_NOP;
#endif
		bmp.Paint(draw, dest, so, rop0);
#ifdef PLATFORM_WIN32
		if(fore != Black && (!draw.IsMono() || CalcGray(fore) >= 0x8000))
#endif
#ifdef PLATFORM_X11
		if(gcv.background = GetXPixel(fore))
#endif
		{
#ifdef PLATFORM_WIN32
			if(draw.IsMono())
				rop0 = PIXEL_NOT_OR;
			else
			{
				SetTextColor(draw, fore);
				SetBkColor(draw, Black());
				rop0 = PIXEL_OR;
			}
#endif
#ifdef PLATFORM_X11
			gcv.foreground = 0;
			gcv.function = X11_ROP2_OR;
			XChangeGC(Xdisplay, draw.GetGC(), GCForeground | GCBackground | GCFunction, &gcv);
			rop0 = PIXEL_NOP;
#endif
			bmp.Paint(draw, dest, so, rop0);
		}
	}
#ifdef PLATFORM_WIN32
	if(!draw.IsMono())
	{
		SetTextColor(draw, text);
		SetBkColor(draw, bk);
	}
#endif
#ifdef PLATFORM_X11
	gcv.foreground = text;
	gcv.background = bk;
	gcv.function = oldfn;
	XChangeGC(Xdisplay, draw.GetGC(), GCForeground | GCBackground | GCFunction, &gcv);
#endif
}

//////////////////////////////////////////////////////////////////////
// Image::

const Image& Image::Void()  { static Image image; return image; }
const Image& Image::Empty() { static Image image(new Data); return image; }

static ImageCache::RegisterName MK__s("Void", Image::Void());

#ifdef PLATFORM_WIN32
const Image& Image::AppStarting()   { static Image c = Cursor(IDC_APPSTARTING); return c; }
const Image& Image::Arrow()         { static Image c = Cursor(IDC_ARROW);       return c; }
const Image& Image::Cross()         { static Image c = Cursor(IDC_CROSS);       return c; }
const Image& Image::IBeam()         { static Image c = Cursor(IDC_IBEAM);       return c; }
const Image& Image::No()            { static Image c = Cursor(IDC_NO);          return c; }
const Image& Image::SizeAll()       { static Image c = Cursor(IDC_SIZEALL);     return c; }
const Image& Image::SizeHorz()      { static Image c = Cursor(IDC_SIZEWE);      return c; }
const Image& Image::SizeVert()      { static Image c = Cursor(IDC_SIZENS);      return c; }
const Image& Image::SizeSlash()     { static Image c = Cursor(IDC_SIZENESW);    return c; }
const Image& Image::SizeBackSlash() { static Image c = Cursor(IDC_SIZENWSE);    return c; }
const Image& Image::UpArrow()       { static Image c = Cursor(IDC_UPARROW);     return c; }
const Image& Image::Wait()          { static Image c = Cursor(IDC_WAIT);        return c; }

const Image& Image::Application()   { static Image i = Icon(IDI_APPLICATION);   return i; }
const Image& Image::Asterisk()      { static Image i = Icon(IDI_ASTERISK);      return i; }
const Image& Image::Exclamation()   { static Image i = Icon(IDI_EXCLAMATION);   return i; }
const Image& Image::Hand()          { static Image i = Icon(IDI_HAND);          return i; }
const Image& Image::Question()      { static Image i = Icon(IDI_QUESTION);      return i; }
const Image& Image::WindowsLogo()   { static Image i = Icon(IDI_WINLOGO);       return i; }
#endif

Image::Image()
: data(GetVoidData() -> AddRef())
{
}

Image::Image(const Nuller&)
: data(GetVoidData() -> AddRef())
{
}

Image::Image(const Image& i)
: data(i.AddRef())
{
}

Image::Image(const Image& (*pfn)())
: data(pfn().AddRef())
{
}

Image::Image(const Value& value)
{
	if(IsNull(value))
		data = GetVoidData() -> AddRef();
	else if(IsString(value))
		data = ImageCache::Get()[(String)value].AddRef();
	else if(IsTypeRaw<Image>(value))
		data = RawValue<Image>::Extract(value).AddRef();
	else
	{
		NEVER(); // invalid value type
		data = GetVoidData() -> AddRef();
	}
}

Image::Image(const OldInit& init)
//: data(GetVoidData() -> AddRef())
: data(new Data(init.scans, init.scan_count, Size(init.cx, init.cy), AlphaImageInfo::COLOR_RLE))
{
}

Image::Image(const Init& init)
//: data(GetVoidData() -> AddRef())
: data(new Data(init.scans, init.scan_count))
{
	MemReadStream strm(init.info, init.info_count);
	data -> Serialize(strm);
}

#ifdef PLATFORM_WIN32
Image::Image(HBITMAP pixels, HBITMAP mask)
{
	if(!pixels)
	{
		data = GetVoidData() -> AddRef();
		return;
	}

	BITMAP bmp;
	GetObject(pixels, sizeof(bmp), &bmp);
	data = new Data(0, 0, Size(bmp.bmWidth, bmp.bmHeight), AlphaImageInfo::COLOR_RLE, Point(0, 0), 0);
	data -> source = OWNER;
	data -> data = Bitmap(pixels);
	if(mask)
	{
		data -> mask = Bitmap(mask);
		data -> mask_pos = 0;
	}
}
#endif

Image::Data *Image::GetVoidData()
{
//	static int count = 0;
//	puts(String() << "GetVoidData x " << ++count);
//	fflush(stdout);
	static Data data(Data::VOIDDATA);
	return &data;
//	static One<Data> ptr = new Data;
//	return ~ptr;
}

void Image::Create(const Draw& draw, int cx, int cy)
{
	data = new Data(0, 0, Size(cx, cy), AlphaImageInfo::COLOR_RLE, Point(0, 0), 0);
	data -> source = OWNER;
#ifdef PLATFORM_WIN32
	data -> data.Create(Size(cx, cy), draw);
#endif
#ifdef PLATFORM_X11
	data -> data.Create(Size(cx, cy));
#endif
}

void Image::Create(const Draw& draw, int cx, int cy, const Drawing& drawing, Color paper)
{
	Create(draw, cx, cy);
	Set(drawing, paper);
}

dword Image::GetHashValue() const
{
	if(IsEmpty())
		return 0;
	dword i = data -> size.cx + (data -> size.cy << 16)
		+ (data -> hotspot.x << 8) + (data -> hotspot.y << 24);
	if(data -> source == OWNER)
		return i;
	switch(data -> rle_count)
	{
	default: i ^= data -> rle[data -> rle_count - 1][0] << 24;
	case 3:  i ^= data -> rle[2][0] << 16;
	case 2:  i ^= data -> rle[1][0] << 8;
	case 1:  i ^= data -> rle[0][0];
	case 0: break;
	}
	return i;
}

void Image::Serialize(Stream& stream)
{
	int version = 1;
	AlphaArray alpha;
	if(stream.IsStoring())
		alpha = ImageToAlphaArray(*this);
	stream / version % alpha;
//	LOG("Image::Serialize: " << AlphaDump(alpha));
	if(stream.IsLoading())
		*this = AlphaArrayToImage(alpha);
//	static TimingInspector ti("Image::Serialize(RleEncoder creation)");
//	ti.Start();
//	RleEncoder encoder;
//	ti.End();
/*
	if(stream.IsLoading())
	{
		Clear();
		*this = encoder.Load(stream);
	}
	else
		encoder.Save(stream, *this);
*/
}

String Image::AsString() const
{
	String imd = "Image";
	if(IsVoid())
		imd = "()";
	else if(IsEmpty())
		imd = "(Empty)";
	else
		imd << '(' << data -> size.cx << " * " << data -> size.cy << ')';
	return imd;
}

bool Image::IsSame(const Image& other) const
{
	bool ea = IsEmpty(), eb = other.IsEmpty();
	if(ea || eb)
		return ea && eb;
	if(data -> size != other.data -> size
	|| data -> hotspot != other.data -> hotspot
	|| data -> encoding != other.data -> encoding)
		return false;
	if(data -> source == OWNER || other.data -> source == OWNER)
		return false; // at least 1 image might have been modified
	return data -> rle == other.data -> rle;
}

Image::Data *Image::Get(Draw& draw, int effect) const
{
	if(IsEmpty())
		return data;

	Data **peffect;
	switch(effect & EFFECT_MASK)
	{
	default:       NEVER();
	case STANDARD: return data;
	case ETCHED:   peffect = &data -> etched; break;
	case GRAYED:   peffect = &data -> grayed; break;
	}

	if(*peffect == GetVoidData())
	{
		(**peffect).Release();
		*peffect = new Data(data -> rle, data -> rle_count,
			data -> size, data -> encoding, data -> hotspot, effect);
		if(data -> source == OWNER)
		{
			AlphaArray aa = ImageToAlphaArray(*this, draw, -3);
			MakeImageEffect(aa, effect);
			(**peffect).MakeOwner(draw, true, aa);
		}
	}

	return *peffect;
}

#ifdef PLATFORM_WIN32
HBITMAP Image::GetMaskHBITMAP() const
#else
Pixmap Image::GetMaskPixmap() const
#endif
{
	if(!MakeOwner(ScreenInfo(), false))
		return 0;
	if(data -> mask_pos < 0)
	{ // create new mask
		data -> mask_pos = 0;
#ifdef PLATFORM_WIN32
		data -> mask.Owner(data -> size, ScreenInfo());
#else
		data -> mask.Owner(data -> size);
#endif
		BitmapDraw(data -> mask).DrawRect(Rect(data -> size), Black);
	}
#ifdef PLATFORM_WIN32
	return data -> mask.GetHBITMAP();
#else
	return data -> mask.GetPixmap();
#endif
}

Size Image::RatioSize(int cx, int cy) const
{
	return GetRatioSize(GetSize(), cx, cy);
}

Size Image::FitSize(int cx, int cy) const
{
	return GetFitSize(GetSize(), cx, cy);
}

void Image::Set(Color color)
{
	if(IsNull(color))
	{
		ImageDraw(*this).DrawRect(GetRect(), Black);
		ImageMaskDraw(*this).DrawRect(GetRect(), White);
	}
	else
	{
		ImageDraw(*this).DrawRect(GetRect(), color);
		if(data -> mask_pos >= 0)
		{ // throw away mask
			data -> mask_pos = -1;
			data -> mask.Clear();
		}
	}
}

void Image::KillMask()
{
	Clone(ScreenInfo(), true);
	data -> mask_pos = -1;
	data -> mask.Clear();
}

void ImageAutoMask(Image& image, Color color, int tolerance)
{
	if(image.Clone(ScreenInfo(), true).IsEmpty())
		return;
	if(!IsNull(color))
	{ // create mask from image
#ifdef PLATFORM_WIN32
		if(tolerance == 0)
		{
			ImageDraw pdc(image);
			ImageMaskDraw mdc(image);
			SetBkColor(pdc, color);
			Size size = image.GetSize();
			BitBlt(mdc, 0, 0, size.cx, size.cy, pdc, 0, 0, SRCCOPY);
			SetBkColor(pdc, White());
			BitBlt(pdc, 0, 0, size.cx, size.cy, mdc, 0, 0, 0x220326);
		}
		else
#endif
		{ // nonzero tolerance - use special algorithm
			PixelArray h = ImageToPixelArray(image);
			PixelArray mask = PixelMask(h, color, tolerance);
			{
				ImageMaskDraw imdraw(image);
				mask.Paint(imdraw);
			}
			ImageCropMask(image);
		}
	}
	else if(image.HasMask())
	{
		image.KillMask();
	}
}

void ImageKillMask(Image& image, Color bgnd)
{
	if(!image.IsEmpty() && !IsNull(bgnd) /*&& image.HasMask()*/) {
		image.Clone(ScreenInfo(), true);
		ImageDraw idraw(image);
		Rect rc = image.GetRect();
		image.PaintImage(idraw, rc, rc, Null, bgnd);
//		idraw.DrawImage(0, 0, image, Null, bgnd);
		idraw.Close();
		image.KillMask();
	}
}

void ImageCropMask(Image& image)
{
	if(!image.IsEmpty() && image.HasMask())
	{
		image.Clone(ScreenInfo(), true);
		ImageDraw idraw(image);
		Rect rc = image.GetRect();
		image.PaintImage(idraw, rc, rc, Null, Black);
	}
}

Image ImageCrop(const Image& img, const Rect& _rc)
{
	Rect rc = _rc & img.GetRect();
	if(rc.IsEmpty())
		return Image();
	if(rc == img.GetRect())
		return img;
	Image new_image(rc.Size());
	{
		ImageDraw idraw(new_image);
		img.PaintData(idraw, rc, Rect(rc.Size()), PIXEL_COPY);
	}
	if(img.HasMask())
	{
		ImageMaskDraw imdraw(new_image);
		img.PaintMask(imdraw, rc, Rect(rc.Size()), Black, White, PIXEL_COPY);
	}
	return new_image;
}

void Image::Set(const Drawing& drawing, Color paper)
{
	if(IsEmpty()) return;
	Clone(ScreenInfo(), true);
	ImageDraw draw(*this);
	if(!IsNull(paper))
		draw.DrawRect(GetRect(), paper);
	draw.DrawDrawing(GetRect(), drawing);
}

void Image::SetHotSpot(Point pt)
{
	if(pt == data -> hotspot)
		return;
	if(data -> refcount > 1)
	{ // virtual clone
		if(data -> source == OWNER)
			Clone(ScreenInfo(), false);
		else
		{ // semiclone
			Data *p = new Data(data -> rle, data -> rle_count, data -> size,
				data -> encoding, pt, data -> effect);
			p -> data_slot = data -> data_slot;
			p -> data_pos = data -> data_pos;
			p -> mask_slot = data -> mask_slot;
			p -> mask_pos = data -> mask_pos;
			Set(p);
		}
	}
	data -> hotspot = pt;
}

Image DrawToImage(Draw& draw, const Rect& rc)
{
	if(rc.IsEmpty())
		return Image();
	Image image(draw, rc.Size());
	{
		ImageDraw idraw(image);
#ifdef PLATFORM_WIN32
		BitBlt(idraw, 0, 0, rc.Width(), rc.Height(), draw, rc.left, rc.top, SRCCOPY);
#endif
#ifdef PLATFORM_X11
		Point off = draw.GetOffset();
		XCopyArea(Xdisplay, draw.GetDrawable(), idraw.GetDrawable(), idraw.GetGC(),
			rc.left + off.x, rc.top + off.y, rc.Width(), rc.Height(), 0, 0);
#endif
	}
	return image;
}

#ifdef PLATFORM_WIN32
Image ClipboardToImage()
{
	if(!OpenClipboard(NULL))
		return Image();
	HBITMAP hbmp = (HBITMAP)GetClipboardData(CF_BITMAP);
	if(!hbmp)
	{
		CloseClipboard();
		return Image();
	}
	BITMAP bmp;
	GetObject(hbmp, sizeof(bmp), &bmp);
	Image image(bmp.bmWidth, bmp.bmHeight);
	HDC hdc = CreateCompatibleDC(0);
	HGDIOBJ old = SelectObject(hdc, hbmp);
	BitBlt(ImageDraw(image), 0, 0, bmp.bmWidth, bmp.bmHeight, hdc, 0, 0, SRCCOPY);
	SelectObject(hdc, old);
	DeleteDC(hdc);
	CloseClipboard();
	return image;
}
#endif

bool ImageToClipboard(const Image& img, Color back, bool empty)
{
#ifdef PLATFORM_WIN32
	if(img.IsEmpty())
		return false;
	Image copy = img;
	ImageKillMask(copy, back);
	return PixelArrayToClipboard(ImageToPixelArray(copy), empty);
#else
	return false; // todo
#endif
}

#ifdef PLATFORM_WIN32
/*
void Image::Paint(HDC hdc, const Rect& src, const Rect& dest, int fx) const
{
	if(IsEmpty())
		return;
	Data *p = Get(fx);
	p -> Instantiate(true);
	if(p -> mask_pos >= 0 && !(fx & doxor))
	{
		p -> PaintMask(hdc, src, dest, Black, White, SRCAND);
		p -> PaintData(hdc, src, dest, SRCINVERT);
	}
	else
		p -> PaintData(hdc, src, dest, fx & doxor ? SRCINVERT : SRCCOPY);
}
*/
#endif

#ifdef PLATFORM_WIN32
/*
void Image::Paint(HDC hdc, const Rect& src, const Rect& dest, Color fore, Color back, bool doxor) const
{
	PaintMask(hdc, src, dest, fore, back, doxor ? doxor : 0);
}
*/
#endif

void Image::Paint(Image& image, const Rect& src, const Rect& dest) const
{
	{
		ImageDraw idraw(image);
		PaintImage(idraw, src, dest, 0);
	}
	if(image.HasMask())
	{
		ImageMaskDraw imdraw(image);
		if(HasMask())
			PaintImage(imdraw, src, dest, Black, Null, Null);
		else
			imdraw.DrawRect(dest, Black);
	}
}

void Image::PaintData(Draw& draw, const Rect& src, const Rect& dest, int fx) const
{
	int rop = (fx & XOR ? PIXEL_XOR : fx & 0xFFFFFF ? fx & 0xFFFFFF : PIXEL_COPY);
	Get(draw, fx) -> PaintData(draw, src, dest, (PixelOp)rop);
}

void Image::PaintMask(Draw& draw, const Rect& src, const Rect& dest, Color fore, Color back, int fx) const
{
	if(IsNull(fore) && IsNull(back))
		return;
	if(fx & XOR)
	{ // doxor mode
		fore = Nvl(fore, Black);
		back = Nvl(back, Black);
		fx = PIXEL_XOR;
	}
	Get(draw, fx) -> PaintMask(draw, src, dest, fore, back, (PixelOp)(fx & 0xFFFFFF));
}

#ifdef PLATFORM_WIN32

template<>
unsigned GetHashValue(const HCURSOR& c)
{
	return (unsigned)c;
}

const Image& Image::Cursor(HCURSOR cursor)
{
	if(!cursor)
		return Void();
	static SegtorMap<HCURSOR, Image> cursors;
	int i = cursors.Find(cursor);
	if(i >= 0)
		return cursors[i];
	return cursors.Add(cursor) = Image(FromImage(cursor));
}
#endif

#ifdef PLATFORM_WIN32
const Image& Image::Cursor(int id)
{
	if(HIWORD(id))
		return Res(id);
	return Cursor(LoadCursor(0, MAKEINTRESOURCE(id)));
}
#endif

#ifdef PLATFORM_WIN32
const Image& Image::Cursor(const char *idc)
{
	return idc ? Cursor((int)idc) : Void();
}
#endif

#ifdef PLATFORM_WIN32
const Image& Image::Icon(int id)
{
	if(HIWORD(id))
		return Res(id);
	return Cursor(LoadIcon((HINSTANCE)0, MAKEINTRESOURCE(id)));
}
#endif

#ifdef PLATFORM_WIN32
const Image& Image::Icon(const char *idi)
{
	return idi ? Cursor(LoadIcon((HINSTANCE)0, idi)) : Void();
}
#endif

#ifdef PLATFORM_WIN32
const Image& Image::Icon(int id, bool small_icon)
{
	int size = (small_icon ? 16 : 32);
	return Cursor((HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(id),
		IMAGE_ICON, size, size, LR_DEFAULTCOLOR));
}
#endif

#ifdef PLATFORM_WIN32
const Image& Image::Res(int id)
{
	if(!id)
		return Void();
	static SegtorMap<int, Image> items;
	int i = items.Find(id);
	if(i >= 0)
		return items[i];
	Image& item = items.Add(id);
	return item;
}
#endif

#ifdef PLATFORM_WIN32
Image::Data *Image::FromImage(HCURSOR image)
{
	ICONINFO info;
	Zero(info);
	if(!image || !GetIconInfo(image, &info))
		return GetVoidData() -> AddRef(); // no image to process
	BITMAP bmp;
	Zero(bmp);
	GetObject(info.hbmMask, sizeof(bmp), &bmp);
	if(info.hbmColor)
		DeleteObject(info.hbmColor);
	DeleteObject(info.hbmMask);
	if(bmp.bmWidth <= 0 || bmp.bmHeight <= 0)
		return GetVoidData() -> AddRef();
	Size size = Size(bmp.bmWidth, bmp.bmHeight);
	if(!info.hbmColor)
		size.cy >>= 1;

	One<Data> p = new Data;
	p -> size = size;
	p -> source = CACHE;
	p -> data_slot = &ImageCache::Get().Add(size, false);
	p -> data_pos = p -> data_slot -> GetSize().cx - size.cx;
	p -> mask_slot = &ImageCache::Get().Add(size, true);
	p -> mask_pos = p -> mask_slot -> GetSize().cx - size.cx;

	BitmapDraw ddata(p -> data_slot -> bitmap), dmask(p -> mask_slot -> bitmap);
	DrawIconEx(ddata, p -> data_pos, 0, image, size.cx, size.cy, 0, 0, DI_IMAGE);
	DrawIconEx(dmask, p -> data_pos, 0, image, size.cx, size.cy, 0, 0, DI_MASK);

	if(info.fIcon)
		p -> hotspot = size >> 1;
	else
		p -> hotspot = Size(info.xHotspot, info.yHotspot);

	return -p;
}
#endif

#ifdef PLATFORM_WIN32
HBITMAP Image::GetBitmap(const Rect& rc, Color fore, Color back) const
{
	StringStream ss;
	ss % const_cast<Rect&>(rc) % fore % back;
	int i = data -> bitmaps.Find(ss);
	if(i >= 0)
		return data -> bitmaps[i];
	Size size = rc.Size();
	HDC hdc = CreateCompatibleDC(0);
	ASSERT(hdc);
	HBITMAP bitmap;
	if(!IsNull(back))
		bitmap = CreateCompatibleBitmap(ScreenInfo(), size.cx, size.cy);
	else
		bitmap = CreateBitmap(size.cx, size.cy, 1, 1, NULL);
	ASSERT(bitmap);
	HBITMAP old_bmp = (HBITMAP)SelectObject(hdc, bitmap);
	Draw mdc(hdc);
	if(IsNull(back))
	{
		SetTextColor(mdc, Black());
		SetBkColor(mdc, White());
		PaintImage(mdc, GetRect(), Rect(-rc.TopLeft(), GetSize()), Black, White);
	}
	else if(IsNull(fore))
	{
		mdc.DrawRect(Rect(size), back);
		PaintImage(mdc, GetRect(), Rect(-rc.TopLeft(), GetSize()), 0);
	}
	else
		PaintImage(mdc, GetRect(), Rect(-rc.TopLeft(), GetSize()), fore, back);
	hdc = mdc.Detach();
	SelectObject(hdc, old_bmp);
	DeleteDC(hdc);
	data -> bitmaps.Add(ss, bitmap);
	return bitmap;
}
#endif

#ifdef PLATFORM_WIN32
HCURSOR Image::GetCursor() const
{
	if(data -> cursor || IsEmpty())
		return data -> cursor;
	static Size cursor_size(GetSystemMetrics(SM_CXCURSOR), GetSystemMetrics(SM_CYCURSOR));
	return data -> cursor = (HCURSOR)GetImage(data -> hotspot, cursor_size);
}
#endif

#ifdef PLATFORM_X11
Cursor Image::GetCursor() const
{
	if(data -> cursor || IsEmpty())
		return data -> cursor;
	Size org = GetSize();
//	unsigned cx = 0, cy = 0;
//	XQueryBestCursor(Xdisplay, Xroot, org.cx, org.cy, &cx, &cy);
//	if(cx == 0 || cy == 0)
//		return 0; // ?? error
	int cx = org.cx;
	int cy = org.cy;
	AlphaArray aa = ImageToAlphaArray(*this);
//	LOG("Image before mono: " << AlphaDump(aa));
	aa.pixel = PixelMono(aa.pixel);
//	LOG("Image after mono:" << AlphaDump(aa));
	Pixmap pm_pixel = XCreatePixmap(Xdisplay, Xroot, cx, cy, 1);
//	fprintf(stderr, "XCreatePixmap(%08x)", (int)pm_pixel);
	Pixmap pm_mask = 0;
	if(aa.HasAlpha()) {
		pm_mask = XCreatePixmap(Xdisplay, Xroot, cx, cy, 1);
//		fprintf(stderr, "XCreatePixmap(%08x)", (int)pm_mask);
	}
	Vector<Rect> clipvec;
	clipvec.Add(Size(cx, cy));
	XGCValues values;

	{
		GC gc = XCreateGC(Xdisplay, pm_pixel, 0, NULL);
		XFillRectangle(Xdisplay, pm_pixel, gc, 0, 0, cx, cy);
		aa.pixel.PaintGC(gc, pm_pixel, clipvec, Black, White);
		XFreeGC(Xdisplay, gc);
	}

	if(pm_mask)
	{
		GC gc = XCreateGC(Xdisplay, pm_mask, 0, NULL);
		XFillRectangle(Xdisplay, pm_mask, gc, 0, 0, cx, cy);
		aa.alpha.PaintGC(gc, pm_mask, clipvec, White, Black);
		XFreeGC(Xdisplay, gc);
	}

	XColor cur_fg;
	cur_fg.pixel = (*Xgetpixel)(0, 0, 0);
	cur_fg.red = cur_fg.green = cur_fg.blue = 0;
//	cur_fg.pixel = (*Xgetpixel)(255, 255, 255);
//	cur_fg.red = cur_fg.green = cur_fg.blue = 0xFFFF;
	cur_fg.flags = DoRed | DoGreen | DoBlue;

	XColor cur_bg;
	cur_bg.pixel = (*Xgetpixel)(255, 255, 255);
	cur_bg.red = cur_bg.green = cur_bg.blue = 0xFFFF;
	cur_bg.flags = DoRed | DoGreen | DoBlue;
	Point hotspot = GetHotSpot();
	data -> cursor = XCreatePixmapCursor(Xdisplay, pm_pixel, pm_mask, &cur_bg, &cur_fg,
		minmax<int>(hotspot.x, 0, cx - 1), minmax<int>(hotspot.y, 0, cy - 1));
	LLOG("XCreatePixmapCursor -> " << FormatIntHex(data->cursor));
//	fprintf(stderr, "XCreatePixmapCursor(%08x)", (int)data->cursor);

//	fprintf(stderr, "XFreePixmap(%08x)", (int)pm_pixel);
	XFreePixmap(Xdisplay, pm_pixel);
	if(pm_mask) {
//		fprintf(stderr, "XFreePixmap(%08x)", (int)pm_mask);
		XFreePixmap(Xdisplay, pm_mask);
	}

	return data -> cursor;
}

Image Image::FontCursor(int id)
{
	Image img(new Data);
	img.data->cursor = XCreateFontCursor(Xdisplay, id);
	LLOG("XCreateFontCursor -> " << img.data->cursor);
	return img;
}

#include <X11/cursorfont.h>

const Image& Image::AppStarting()   { static Image c = FontCursor(XC_clock); return c; }
const Image& Image::Arrow()         { static Image c = FontCursor(XC_left_ptr); return c; }
const Image& Image::Cross()         { static Image c = FontCursor(XC_crosshair); return c; }
const Image& Image::IBeam()         { static Image c = FontCursor(XC_xterm); return c; }
const Image& Image::No()            { static Image c = FontCursor(XC_circle); return c; }
const Image& Image::SizeAll()       { static Image c = FontCursor(XC_fleur); return c; }
const Image& Image::SizeHorz()      { static Image c = FontCursor(XC_sb_v_double_arrow); return c; }
const Image& Image::SizeVert()      { static Image c = FontCursor(XC_sb_v_double_arrow); return c; }
const Image& Image::SizeSlash()     { static Image c = FontCursor(XC_sizing); return c; }
const Image& Image::SizeBackSlash() { static Image c = FontCursor(XC_arrow); return c; }
const Image& Image::UpArrow()       { static Image c = FontCursor(XC_center_ptr); return c; }
const Image& Image::Wait()          { static Image c = FontCursor(XC_watch); return c; }

#endif

#ifdef PLATFORM_WIN32
HICON Image::GetImage(Point hotspot, Size force_size, Color fore) const
{
	StringStream ss;
	ss % hotspot % force_size % fore;
	int i = data -> images.Find(ss);
	if(i >= 0)
		return data -> images[i];
	if(IsEmpty())
		return 0;
	HBITMAP pixels, mask;
	Size size = Extract(pixels, mask, force_size, fore);
	if(size.cx == 0)
		return 0;
	ICONINFO iconinfo;
	Zero(iconinfo);
	iconinfo.fIcon = (hotspot.x < 0);
	iconinfo.xHotspot = hotspot.x;
	iconinfo.yHotspot = hotspot.y;
	iconinfo.hbmMask = mask;
	iconinfo.hbmColor = pixels;
	HICON image = CreateIconIndirect(&iconinfo);
	ASSERT(image);
	DeleteObject(pixels);
	DeleteObject(mask);
//	RLOG("adding image " << Format("%08x", image));
	data -> images.Add(ss, image);
	return image;
}
#endif

#ifdef PLATFORM_WIN32
HBRUSH Image::GetBrush(Color fore, Color back) const
{
	StringStream ss;
	ss % fore % back;
	int i = data -> brushes.Find(ss);
	if(i >= 0)
		return data -> brushes[i];
	if(IsEmpty())
		return 0;
	HBITMAP bmp = GetBitmap(Rect(0, 0, 8, 8), fore, back);
	HBRUSH brush = CreatePatternBrush(bmp);
	ASSERT(brush);
//	DeleteObject(bmp);
	data -> brushes.Add(ss, brush);
	return brush;
}
#endif

#ifdef PLATFORM_WIN32
bool Image::CreateCaret(HWND hwnd, const Rect& clip_rect) const
{
	Rect r = GetRect() & clip_rect;
	if(r.IsEmpty())
		return false;
	HBITMAP bmp;
	VERIFY(bmp = CreateBitmap(r.right, r.bottom, 1, 1, NULL));
	HDC mdc = CreateCompatibleDC(0);
	HBITMAP old = (HBITMAP)SelectObject(mdc, bmp);
	{
		Draw draw(mdc);
		PaintImage(draw, r, Rect(r.Size()));
	}
	SelectObject(mdc, old);
	DeleteDC(mdc);
	bool result = ::CreateCaret(hwnd, bmp, r.right, r.bottom);
	DeleteObject(bmp);
	return result;
}
#endif

#ifdef PLATFORM_WIN32
Size Image::Extract(HBITMAP& pixels, HBITMAP& mask, Size sz, Color fore) const
{
	Size asz = sz;
	if(asz.cx == 0) asz.cx = data -> size.cx;
	if(asz.cy == 0) asz.cy = data -> size.cy;
	Rect dest = min(asz, data -> size);

	VERIFY(pixels = CreateCompatibleBitmap(ScreenInfo(), asz.cx, asz.cy));
	VERIFY(mask = CreateBitmap(asz.cx, asz.cy, 1, 1, NULL));
	HDC bdc = CreateCompatibleDC(0);
	HDC mdc = CreateCompatibleDC(0);
	HBITMAP bdc_bmp = (HBITMAP)SelectObject(bdc, pixels);
	HBITMAP mdc_bmp = (HBITMAP)SelectObject(mdc, mask);
	PatBlt(bdc, 0, 0, asz.cx, asz.cy, BLACKNESS);
	PatBlt(mdc, 0, 0, asz.cx, asz.cy, WHITENESS);
	{
		Draw bdraw(bdc), mdraw(mdc);
		if(IsNull(fore))
			PaintData(bdraw, dest, dest);
		else
			PaintMask(bdraw, dest, dest, fore, Black);
		PaintMask(mdraw, dest, dest, Black, White);
	}
	SelectObject(bdc, bdc_bmp);
	SelectObject(mdc, mdc_bmp);
	DeleteDC(bdc);
	DeleteDC(mdc);

	return dest.Size();
}
#endif

//////////////////////////////////////////////////////////////////////
// ImageDraw::

#ifdef PLATFORM_WIN32
static HGDIOBJ default_bitmap;
#endif

void ImageDraw::Open(Draw& draw, Image& image)
{
	ASSERT(!image.IsEmpty());
	image.Clone(draw, true);
#ifdef PLATFORM_WIN32
	HDC hdc = CreateCompatibleDC(draw);
	default_bitmap = ::SelectObject(hdc, image.GetHBITMAP());
	Attach(hdc);
//BUGFIX tohle jsem si dovolil pripsat :)
	if(PaletteMode() && AutoPalette()) {
		SelectPalette(hdc, GetQlibPalette(), TRUE);
		RealizePalette(hdc);
	}
//ENDOFBUGFIX
#endif
#ifdef PLATFORM_X11
	gc = XCreateGC(Xdisplay, dw = image.GetPixmap(), 0, NULL);
#ifdef PLATFORM_XFT
	xftdraw = XftDrawCreate(Xdisplay, (Drawable) dw,
	                        DefaultVisual(Xdisplay, Xscreenno), Xcolormap);
#endif
	Vector<Rect> cliplist;
	cliplist.Add(image.GetRect());
	Init(cliplist, Point(0, 0));
	LLOG("ImageDraw::Open: image = " << image.GetSize() << ", clip = " << GetClip());
#endif
	backdraw = true;
}

void ImageDraw::Close()
{
//	while(co.GetCount())
//		End();
#ifdef PLATFORM_WIN32
	HDC hdc = Detach();
	if(hdc)
	{
//BUGFIX tohle jsem si taky dovolil pripsat :)
		if(PaletteMode() && AutoPalette())
			SelectPalette(hdc, (HPALETTE)GetStockObject(DEFAULT_PALETTE), TRUE);
//ENDOFBUGFIX
		SelectObject(hdc, default_bitmap);
		::DeleteDC(hdc);
	}
#endif
#ifdef PLATFORM_X11
	if(gc)
	{
	#ifdef PLATFORM_XFT
		XftDrawDestroy(xftdraw);
	#endif
		XFreeGC(Xdisplay, gc);
		gc = NULL;
	}
#endif
}

//////////////////////////////////////////////////////////////////////
// ImageMaskDraw::

void ImageMaskDraw::Open(Image& image)
{
	ASSERT(!image.IsEmpty());
	image.Clone(ScreenInfo(), false);
#ifdef PLATFORM_WIN32
	HDC hdc = CreateCompatibleDC(0);
	default_bitmap = ::SelectObject(hdc, image.GetMaskHBITMAP());
	Attach(hdc);
#endif
#ifdef PLATFORM_X11
	gc = XCreateGC(Xdisplay, dw = image.GetMaskPixmap(), 0, NULL);
#ifdef PLATFORM_XFT
	xftdraw = XftDrawCreate(Xdisplay, (Drawable) dw,
	                        DefaultVisual(Xdisplay, Xscreenno), Xcolormap);
#endif
	Vector<Rect> cliplist;
	cliplist.Add(image.GetRect());
	Init(cliplist, Point(0, 0));
#endif
	backdraw = true;
	is_mono = true;
}

void ImageMaskDraw::Close()
{
#ifdef PLATFORM_WIN32
	HDC hdc = Detach();
	if(hdc)
	{
		SelectObject(hdc, default_bitmap);
		::DeleteDC(hdc);
	}
#endif
#ifdef PLATFORM_X11
	if(gc)
	{
	#ifdef PLATFORM_XFT
		XftDrawDestroy(xftdraw);
	#endif
		XFreeGC(Xdisplay, gc);
		gc = NULL;
	}
#endif
}

//////////////////////////////////////////////////////////////////////
// ImageCache::

int ImageCache::MaxCacheImageSize = 16384;

ImageCache& ImageCache::Get()
{
	static ImageCache cache;
	return cache;
}

Image::Slot& ImageCache::Add(Size size, bool mono)
{
	ASSERT(size.cx > 0 && size.cy > 0);
	ArrayMap<int, Image::Slot>& map = mono ? mono_cache : color_cache;
	int i = map.Find(size.cy);
	if(i < 0)
	{
		i = map.GetCount();
		map.Add(size.cy, Image::Slot(size.cy, mono));
	}
	Image::Slot& slot = map[i];
	slot.Add(size.cx);
	return slot;
}

/*
#define IMAGEFILE <tctrl/doc.iml>
#define IMAGECLASS NewImg
#include <iml.h>

class ImageTestCtrl : public TopWindow
{
public:
	virtual void  Paint(Draw& draw);
	virtual Image CursorImage(Point pt, dword keyflags);
};

void RunImageTest()
{
	ImageTestCtrl dialog;
	dialog.Sizeable();
	dialog.SetRect(100, 100, 900, 200);
	dialog.Run();
}

void ImageTestCtrl::Paint(Draw& draw)
{
	draw.DrawRect(draw.GetClip(), SLtGray);

	Rect rc(10, 10, 74, 74);
	draw.DrawImage(rc, NewImg::save_as());
	rc.OffsetRect(70, 0);
	draw.DrawImage(rc, NewImg::save_as(), Image::ETCHED);
	rc.OffsetRect(-70, 70);
	draw.DrawImage(rc, NewImg::save_as(), Image::GRAYED);
	rc.OffsetRect(140, -70);
	draw.DrawImage(rc, NewImg::save_as(), LtRed, Yellow);
	rc.OffsetRect(70, 0);
	draw.DrawImage(rc, NewImg::save_as(), LtRed, Null);
	rc.OffsetRect(70, 0);
	draw.DrawImage(rc, NewImg::save_as(), Null, Yellow);
	rc.OffsetRect(70, 0);
	draw.DrawImage(rc, Image::Arrow());
	rc.OffsetRect(70, 0);
	draw.DrawImage(rc, Image::IBeam());
	rc.OffsetRect(70, 0);
	draw.DrawImage(rc, Image::Cross());

	draw.BeginGdi();
	SelectObject(draw, NewImg::save_as().GetBrush());
	PatBlt(draw, 100, 100, 100, 100, PATCOPY);
	HBITMAP hbmp = NewImg::copy().GetBitmap(Color(), LtGreen);
	HDC dc = CreateCompatibleDC(ScreenInfo());
	HGDIOBJ old = SelectObject(dc, hbmp);
	StretchBlt(draw, 200, 100, 100, 100, dc, 0, 0, 16, 16, SRCCOPY);
	SelectObject(dc, old);
	DeleteDC(dc);
	draw.EndGdi();

	Image circle(100, 100);

	{
		ImageDraw draw(circle);
		draw.DrawRect(draw.GetClip(), LtMagenta);
		draw.SetColor(LtRed);
		draw.SetDrawPen(5, Black);
		DrawEllipse(draw, draw.GetClip());
	}

	circle.AutoMask(LtRed);
	Image cropped = circle.Crop(Rect(50, 0, 100, 50));

	draw.DrawImage(50, 50, circle);
	draw.DrawImage(200, 50, cropped);

	circle.WriteClipboard(Green);

	StringStream ss;
	ss % cropped;
	Image in_cropped;
	ss.Seek(0);
	ss.SetLoading();
	ss % in_cropped;
	draw.DrawImage(600, 50, in_cropped);
	draw.DrawText(600, 30, "Serializace...");
}

Image ImageTestCtrl::CursorImage(Point pt, dword keyflags)
{
	return NewImg::save_as;
}

//*/
#endif
