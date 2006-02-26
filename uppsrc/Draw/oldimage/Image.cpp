//////////////////////////////////////////////////////////////////////
// image.h: rastrovy obrazek, 2. iterace

#include "Draw.h"

#if defined(PLATFORM_WIN32) && defined(QLIB2IMAGE)
//#define LOG_IMAGES // uncomment this line to turn image logging on

RichValue<Image>::Registrator MK__s;

//////////////////////////////////////////////////////////////////////

/*
static Size GetBitmapSize(HBITMAP bmp)
{
	BITMAP bi;
	if(!GetObject(bmp, sizeof(bi), &bi))
		return Size(0, 0);
	return Size(bmp.bmWidth, bmp.bmHeight);
}

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

static void PaintBitmap(HBITMAP dest, Point dest_pos, HBITMAP src, HDC hdc = ScreenInfo())
{
	PaintBitmap(dest, dest_pos, src, GetBitmapSize(src), hdc);
}

//////////////////////////////////////////////////////////////////////

static void PaintBitmap(HBITMAP dest, HBITMAP src, HDC hdc = ScreenInfo())
{
	PaintBitmap(dest, Point(0, 0), src, GetBitmapSize(src), hdc);
}
*/

//////////////////////////////////////////////////////////////////////
// Image::Bitmap::

Image::Bitmap::Bitmap(HBITMAP _bitmap)
: bitmap(0)
, refcount(0)
, size(0, 0)
{
	if(_bitmap)
	{
		BITMAP bmp;
		GetObject(_bitmap, sizeof(bmp), &bmp);
		size.cx = bmp.bmWidth;
		size.cy = bmp.bmHeight;
		bitmap = new HBITMAP(_bitmap);
		refcount = new int(1);
	}
}

//////////////////////////////////////////////////////////////////////

void Image::Bitmap::Paint(HDC hdc, const Rect& dest, const Rect& src, int rop) const
{
	if(!bitmap)
		return;
	if(!(GetDeviceCaps(hdc, RASTERCAPS) & RC_STRETCHBLT))
	{
		RawImage ri = HBITMAPToRawImage(*bitmap, 0, true, ScreenInfo());
		ri.Paint(hdc, src, dest, rop);
		return;
	}
	HDC sdc = CreateCompatibleDC(0);
	HGDIOBJ old_bitmap = SelectObject(sdc, *bitmap);
	if(!old_bitmap)
	{ // bitmap not compatible with screen context, try destination context
		DeleteDC(sdc);
		sdc = CreateCompatibleDC(hdc);
		old_bitmap = SelectObject(sdc, *bitmap);
	}
	if(dest.Size() != src.Size() || dest.Width() > 1024 || dest.Height() > 1024
	|| !BitBlt(hdc, dest.TopLeft(), sdc, src, rop))
	{
		int old_mode = SetStretchBltMode(hdc, COLORONCOLOR);
		for(SmartStretch sms(dest, src, dest); sms.Get();)
			if(!StretchBlt(hdc, sms.GetDest(), sdc, sms.GetSrc(), rop))
			{ // todo: why does the stretchblt error occur here?
				RawImage ri = HBITMAPToRawImage(*bitmap, -1, true);
				ri.Paint(hdc, sms.GetSrc(), sms.GetDest(), rop);
				break;
			}
		SetStretchBltMode(hdc, old_mode);
	}
	SelectObject(sdc, old_bitmap);
	DeleteDC(sdc);
}

//////////////////////////////////////////////////////////////////////

void Image::Bitmap::Create(Size new_size, HDC hdc)
{
	Clear();
	ASSERT(new_size.cx >= 0 && new_size.cy >= 0);
	if(new_size.cx > 0 && new_size.cy > 0)
	{
		HBITMAP new_bitmap = hdc
			? CreateCompatibleBitmap(hdc, new_size.cx, new_size.cy)
			: CreateBitmap(new_size.cx, new_size.cy, 1, 1, 0);
		if(new_bitmap)
		{
			bitmap = new HBITMAP(new_bitmap);
			refcount = new int(1);
		}
	}
	size = new_size;
}

//////////////////////////////////////////////////////////////////////

void Image::Bitmap::Clone(Size new_size, HDC hdc)
{
	ASSERT(new_size.cx >= 0 && new_size.cy >= 0);
	if(new_size.cx == 0 || new_size.cy == 0)
	{
		Clear();
		return;
	}
	if(bitmap && (*refcount > 1 || new_size != size))
	{
		HBITMAP new_bitmap = hdc
			? CreateCompatibleBitmap(hdc, new_size.cx, new_size.cy)
			: CreateBitmap(new_size.cx, new_size.cy, 1, 1, 0);
		HDC ddc = CreateCompatibleDC(hdc ? hdc : (HDC)ScreenInfo());
		HGDIOBJ old_bitmap = SelectObject(ddc, new_bitmap);
		Paint(ddc, Point(0, 0));
		SelectObject(ddc, old_bitmap);
		DeleteDC(ddc);
		Clear();
		bitmap = new HBITMAP(new_bitmap);
		refcount = new int(1);
	}
	size = new_size;
}

//////////////////////////////////////////////////////////////////////

void Image::Bitmap::Clear()
{
	if(refcount && !MtDec(*refcount))
	{
		if(bitmap)
		{
			DeleteObject(*bitmap);
			delete bitmap;
		}
		delete refcount;
		refcount = 0;
		bitmap = 0;
	}
	size.cx = size.cy = 0;
}

//////////////////////////////////////////////////////////////////////

static HGDIOBJ bitmap_draw_bitmap;

void Image::BitmapDraw::Open(HDC hdc, Bitmap& bitmap)
{
	ASSERT(!bitmap.IsEmpty());
	bitmap.Owner();
	HDC bdc = CreateCompatibleDC(hdc);
	Attach(bdc);
	if(bitmap.bitmap)
		bitmap_draw_bitmap = ::SelectObject(bdc, *bitmap.bitmap);
	backdraw = true;
}

//////////////////////////////////////////////////////////////////////

void Image::BitmapDraw::Close()
{
	while(co.GetCount())
		End();
	HDC hdc = Detach();
	if(hdc)
	{
		SelectObject(hdc, bitmap_draw_bitmap);
		::DeleteDC(hdc);
	}
}

//////////////////////////////////////////////////////////////////////
// Image::Slot::

void Image::Slot::Add(int width)
{
	ASSERT(size.cy > 0);
	if(!bitmap || size.cx + width > bitmap.GetSize().cx)
	{
		int new_width = size.cx ? size.cx + max(size.cx, width) : max(width, 1024 / size.cy);
		bitmap.SetSize(new_width, size.cy, mono ? 0 : (HDC)ScreenInfo());
	}
	size.cx += width;
}

//////////////////////////////////////////////////////////////////////
// Image::Info::

void Image::Info::Serialize(Stream& stream)
{
	int version = 1;
	stream / version;
	if(version >= 1)
		stream % size % hotspot % encoding;
}

//////////////////////////////////////////////////////////////////////
// Image::Data::

#ifdef LOG_IMAGES
static int total_images = 0;
#endif

Image::Data::Data(const char *const *rle, int rle_count,
	Size size, ENCODING encoding, Point hotspot, int effect)
: Info(size, encoding, hotspot)
, refcount(1)
, rle(rle)
, rle_count(rle_count)
, effect(effect)
, source(QUIET)
, grayed(0)
, etched(0)
, cursor(0)
, data_pos(-1)
, mask_pos(-1)
{
#ifdef LOG_IMAGES
	total_images++;
	LOG("++images = " << total_images);
#endif
}

//////////////////////////////////////////////////////////////////////

Image::Data::Data(const Data& d, HDC hdc)
: Info(d)
, refcount(1)
, rle(d.rle)
, rle_count(d.rle_count)
, effect(d.effect)
, source(d.source)
, grayed(d.grayed ? d.grayed -> AddRef() : 0)
, etched(d.etched ? d.etched -> AddRef() : 0)
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
		data.Clone(hdc);
		mask.Clone(0);
	}
}

//////////////////////////////////////////////////////////////////////

Image::Data::~Data()
{
#ifdef LOG_IMAGES
	total_images--;
	LOG("--images = " << total_images);
#endif
	grayed -> Release();
	etched -> Release();
	FreeCursor();
	FreeImages();
	FreeBitmaps();
	FreeBrushes();
}

//////////////////////////////////////////////////////////////////////

void Image::Data::FreeCursor()
{
	if(cursor)
	{
		DestroyCursor(cursor);
		cursor = 0;
	}
}

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

void Image::Data::FreeBrushes()
{
	for(int i = 0; i < brushes.GetCount(); i++)
		if(brushes[i])
			DeleteObject(brushes[i]);
	brushes.Clear();
}

//////////////////////////////////////////////////////////////////////

void Image::Data::FreeBitmaps()
{
	for(int i = 0; i < bitmaps.GetCount(); i++)
		if(bitmaps[i])
			DeleteObject(bitmaps[i]);
	bitmaps.Clear();
}

//////////////////////////////////////////////////////////////////////

void Image::Data::Prepare(class ImageBuffer& imb)
{
	imb.SetSize(size);
	if(rle)
	{
		ASSERT(rle_count <= size.cy);
		for(int i = 0; i < rle_count; i++)
			imb.SetScanRLE((const byte *)rle[i], i);
	}
	switch(effect & EFFECT_MASK)
	{
	case GRAYED: imb.SetGrayed(); break;
	case ETCHED: imb.SetEtched(); break;
	}
}

//////////////////////////////////////////////////////////////////////

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
	ImageBuffer buffer;
	Prepare(buffer);

	data_slot = &ImageCache::Get().Add(size, false);
	data_pos = data_slot -> GetSize().cx - size.cx;
	buffer.Paint(BitmapDraw(data_slot -> bitmap), Point(data_pos, 0), Black);

	mask_slot = 0;
	mask_pos = -1;

	if(buffer.HasMask())
	{
		mask_slot = &ImageCache::Get().Add(size, true);
		mask_pos = mask_slot -> GetSize().cx - size.cx;
		buffer.PaintMask(BitmapDraw(mask_slot -> bitmap), Rect(size), RectC(mask_pos, 0, size.cx, size.cy),
			White, Black, SRCCOPY);
	}

	source = CACHE;
	return true;
}

//////////////////////////////////////////////////////////////////////

bool Image::Data::MakeOwner(HDC hdc, bool force_data)
{
	if(IsEmpty())
		return false;
	if(source == OWNER)
	{
		if(force_data && !data)
		{ // create data when not instantiated yet
			data.Create(size, hdc);
			BitmapDraw mdraw(hdc, data);
			if(data_pos >= 0 && data_slot)
				data_slot -> bitmap.Paint(mdraw, Point(0, 0), RectC(data_pos, 0, size.cx, size.cy));
			else if(rle)
			{
				ImageBuffer buffer(size);
				Prepare(buffer);
				buffer.Paint(mdraw, Point(0, 0), Black);
			}
		}
	}
	else if(source == CACHE)
	{ // instantiated in cache - move into rasters
		Rect rc = size;
		if(force_data)
		{
			data.Create(size, hdc);
			PaintData(BitmapDraw(hdc, data), rc, rc, SRCCOPY);
		}
		if(mask_pos >= 0)
		{
			mask.Create(size, 0);
			PaintMask(BitmapDraw(mask), rc, rc, Black, White, SRCCOPY);
		}
		source = OWNER;
	}
	else if(source == QUIET)
	{ // not instantiated - transfer directly from ImageBuffer
		Rect rc = size;
		ImageBuffer buffer(size);
		Prepare(buffer);
		if(force_data)
		{
			data.Create(size, hdc);
			buffer.Paint(BitmapDraw(hdc, data), Point(0, 0), Black);
		}
		if(buffer.HasMask())
		{
			mask_pos = 0;
			mask.Create(size, 0);
			buffer.PaintMask(BitmapDraw(mask), rc, rc, White, Black, SRCCOPY);
		}
		source = OWNER;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////

Image::Data *Image::Data::Clone(HDC hdc, bool force_data)
{
	if(IsEmpty())
		return AddRef(); // empty bitmap
	Data *p = (refcount > 1? new Data(*this, hdc) : AddRef());
	p -> MakeOwner(hdc, force_data);
	return p;
}

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

void Image::Data::PaintData(HDC hdc, const Rect& src, const Rect& dest, int rop)
{
	if(src.left == src.right || src.top == src.bottom
		|| dest.left == dest.right || dest.top == dest.bottom || !Instantiate(true))
		return;
	int offset = 0;
	Bitmap& bmp = *(source == CACHE ? offset = data_pos, &data_slot -> bitmap : &data);
	bmp.Paint(hdc, dest, src.OffsetedHorz(offset), rop);
//	BltData(hdc, dest, draw, src, offset, rop, COLORONCOLOR);
}

//////////////////////////////////////////////////////////////////////

void Image::Data::PaintMask(HDC hdc, const Rect& src, const Rect& dest, Color fore, Color back, int rop0)
{
	if(src.left == src.right || src.top == src.bottom
		|| dest.left == dest.right || dest.top == dest.bottom || !Instantiate(false) || mask_pos < 0)
		return;
	int offset = 0;
	Bitmap& bmp = *(source == CACHE ? offset = mask_pos, &mask_slot -> bitmap : &mask);
	Rect so = src.OffsetedHorz(offset);
	ASSERT(so.right <= bmp.GetSize().cx && so.bottom <= bmp.GetSize().cy);
	if(rop0 || !IsNull(fore) && !IsNull(back))
	{ // full blit
		dword text = SetTextColor(hdc, Nvl(fore, Black));
		dword bk   = SetBkColor(hdc, Nvl(back, White));
		bmp.Paint(hdc, dest, so, rop0 ? rop0 : SRCCOPY);
		SetTextColor(hdc, text);
		SetBkColor(hdc, bk);
		return;
	}
	else
	{ // masked rop0
		dword text = SetTextColor(hdc, Black);
		dword bk   = SetBkColor(hdc, White);
		bmp.Paint(hdc, dest, so, IsNull(fore) ? 0x00220326 : SRCAND);
//		BltData(hdc, dest, draw, src, offset, IsNull(fore) ? 0x00220326 : SRCAND, COLORONCOLOR);
		SetTextColor(hdc, Nvl(fore, Black));
		SetBkColor(hdc, Nvl(back, Black));
		bmp.Paint(hdc, dest, so, SRCPAINT);
//		BltData(hdc, dest, draw, src, offset, SRCPAINT, COLORONCOLOR);
		SetTextColor(hdc, text);
		SetBkColor(hdc, bk);
	}
}

//////////////////////////////////////////////////////////////////////
// Image::

const Image& Image::Void()  { static Image image; return image; }
const Image& Image::Empty() { static Image image(new Data); return image; }

static ImageCache::RegisterName MK__s("Void", Image::Void());

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

//////////////////////////////////////////////////////////////////////

Image::Image()
: data(GetVoidData() -> AddRef())
{
}

//////////////////////////////////////////////////////////////////////

Image::Image(const Nuller&)
: data(GetVoidData() -> AddRef())
{
}

//////////////////////////////////////////////////////////////////////

Image::Image(const Image& i)
: data(i.AddRef())
{
}

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

Image::Image(const OldInit& init)
: data(new Data(init.scans, init.scan_count, Size(init.cx, init.cy), COLOR_RLE))
{
}

//////////////////////////////////////////////////////////////////////

Image::Image(const Init& init)
: data(new Data(init.scans, init.scan_count))
{
	data -> Serialize(MemReadStream(init.info, init.info_count));
}

//////////////////////////////////////////////////////////////////////

Image::Image(HBITMAP pixels, HBITMAP mask)
{
	if(!pixels)
	{
		data = GetVoidData() -> AddRef();
		return;
	}

	BITMAP bmp;
	GetObject(pixels, sizeof(bmp), &bmp);
	data = new Data(0, 0, Size(bmp.bmWidth, bmp.bmHeight), COLOR_RLE, Point(0, 0), 0);
	data -> source = OWNER;
	data -> data = Bitmap(pixels);
	if(mask)
	{
		data -> mask = Bitmap(mask);
		data -> mask_pos = 0;
	}
}

//////////////////////////////////////////////////////////////////////

Image::Data *Image::GetVoidData()
{
	static Data data;
	return &data;
//	static One<Data> ptr = new Data;
//	return ~ptr;
}

//////////////////////////////////////////////////////////////////////

void Image::Create(const Draw& draw, int cx, int cy)
{
	data = new Data(0, 0, Size(cx, cy), COLOR_RLE, Point(0, 0), 0);
	data -> source = OWNER;
	data -> data.Create(Size(cx, cy), draw);
}

//////////////////////////////////////////////////////////////////////

void Image::Create(const Draw& draw, int cx, int cy, const Drawing& drawing, Color paper)
{
	Create(draw, cx, cy);
	Set(drawing, paper);
}

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

void Image::Serialize(Stream& stream)
{
//	static TimingInspector ti("Image::Serialize(RleEncoder creation)");
//	ti.Start();
	RleEncoder encoder;
//	ti.End();
	if(stream.IsLoading())
	{
		Clear();
		*this = encoder.Load(stream);
	}
	else
		encoder.Save(stream, *this);
}

//////////////////////////////////////////////////////////////////////

String Image::Dump() const
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

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

Image::Data *Image::Get(int effect) const
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

	if(*peffect == 0)
		*peffect = new Data(data -> rle, data -> rle_count,
			data -> size, data -> encoding, data -> hotspot, effect);

	return *peffect;
}

//////////////////////////////////////////////////////////////////////

HBITMAP Image::GetMaskHBITMAP() const
{
	if(!MakeOwner(ScreenInfo(), false))
		return false;
	if(data -> mask_pos < 0)
	{ // create new mask
		data -> mask_pos = 0;
		data -> mask.Owner(data -> size, 0);
		BitmapDraw(data -> mask).DrawRect(Rect(data -> size), Black);
	}
	return data -> mask.GetHBITMAP();
}

//////////////////////////////////////////////////////////////////////

Size Image::RatioSize(int cx, int cy) const
{
	return GetRatioSize(GetSize(), cx, cy);
}

//////////////////////////////////////////////////////////////////////

Size Image::FitSize(int cx, int cy) const
{
	if(cx == 0 || cy == 0 || IsEmpty())
		return Size(0, 0);
	if(cx * data -> size.cy >= cy * data -> size.cx) // too high
		return iscale(data -> size, cy, cx);
	else
		return iscale(data -> size, cx, cy);
}

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

void Image::AutoMask(Color color, int tolerance)
{
	if(Clone(ScreenInfo(), true).IsEmpty())
		return;
	if(!IsNull(color))
	{ // create mask from image
		if(tolerance == 0)
		{
			ImageDraw pdc(*this);
			ImageMaskDraw mdc(*this);
			SetBkColor(pdc, color);
			BitBlt(mdc, 0, 0, data -> size.cx, data -> size.cy, pdc, 0, 0, SRCCOPY);
			SetBkColor(pdc, White);
			BitBlt(pdc, 0, 0, data -> size.cx, data -> size.cy, mdc, 0, 0, 0x220326);
		}
		else
		{ // nonzero tolerance - use special algorithm
			RawImage mask = GetMask(ImageToRawImage(*this), color, tolerance);
			mask.Paint(ImageMaskDraw(*this));
			CropMask();
		}
	}
	else if(HasMask())
	{
		data -> mask_pos = -1;
		data -> mask.Clear();
	}
}

//////////////////////////////////////////////////////////////////////

void Image::KillMask(Color bgnd)
{
	if(!IsEmpty() && !IsNull(bgnd) && HasMask())
	{
		Clone(ScreenInfo(), true);
		ImageDraw(*this).DrawImage(0, 0, *this, Null, bgnd);
		data -> mask.Clear();
		data -> mask_pos = -1;
	}
}

//////////////////////////////////////////////////////////////////////

void Image::CropMask()
{
	if(!IsEmpty() && HasMask())
	{
		Clone(ScreenInfo(), true);
		ImageDraw(*this).DrawImage(0, 0, *this, Null, Black);
	}
}

//////////////////////////////////////////////////////////////////////

Image Crop(const Image& img, const Rect& _rc)
{
	Rect rc = _rc & img.GetRect();
	if(rc.IsEmpty())
		return Image();
	if(rc == img.GetRect())
		return img;
	Image new_image(rc.Size());
	img.PaintData(ImageDraw(new_image), rc, Rect(rc.Size()));
	if(img.HasMask())
		img.PaintMask(ImageMaskDraw(new_image), rc, Rect(rc.Size()), Black, White);
	return new_image;
}

//////////////////////////////////////////////////////////////////////

void Image::Set(const Drawing& drawing, Color paper)
{
	if(IsEmpty()) return;
	Clone(ScreenInfo(), true);
	ImageDraw draw(*this);
	if(!IsNull(paper))
		draw.DrawRect(GetRect(), paper);
	draw.DrawDrawing(GetRect(), drawing);
}

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

Image DrawToImage(Draw& draw, const Rect& rc)
{
	if(rc.IsEmpty())
		return Image();
	Image image(draw, rc.Size());
	BitBlt(ImageDraw(image), 0, 0, rc.Width(), rc.Height(), draw, rc.left, rc.top, SRCCOPY);
	return image;
}

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

bool ImageToClipboard(const Image& img, Color back, bool empty)
{
	if(img.IsEmpty() || !OpenClipboard(NULL))
		return false;
	if(empty)
		EmptyClipboard();
	Image copy = img;
	copy.KillMask(back);
	String s = BmpEncoder(-1, false).SaveString(copy);
	HGLOBAL hmem = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, s.GetLength());
	if(!hmem)
	{
		CloseClipboard();
		return false;
	}
	memcpy(GlobalLock(hmem), s, s.GetLength());
	GlobalUnlock(hmem);
	SetClipboardData(CF_DIB, hmem);
	CloseClipboard();
	return true;
}

//////////////////////////////////////////////////////////////////////

void Image::Paint(HDC hdc, const Rect& src, const Rect& dest, int fx) const
{
	if(IsEmpty())
		return;
	Data *p = Get(fx);
	p -> Instantiate(true);
	if(p -> mask_pos >= 0 && !(fx & XOR))
	{
		p -> PaintMask(hdc, src, dest, Black, White, SRCAND);
		p -> PaintData(hdc, src, dest, SRCINVERT);
	}
	else
		p -> PaintData(hdc, src, dest, fx & XOR ? SRCINVERT : SRCCOPY);
}

//////////////////////////////////////////////////////////////////////

void Image::Paint(HDC hdc, const Rect& src, const Rect& dest, Color fore, Color back, bool xor) const
{
	PaintMask(hdc, src, dest, fore, back, xor ? XOR : 0);
}

//////////////////////////////////////////////////////////////////////

#ifdef OLDIMAGE

void Image::Paint(Draw& draw, Point pos, int fx) const
{ Paint(draw, GetRect(), Rect(pos, GetSize()), fx); }

//////////////////////////////////////////////////////////////////////

void Image::Paint(Draw& draw, const Rect& dest, int fx) const
{ Paint(draw, GetRect(), dest, fx); }

//////////////////////////////////////////////////////////////////////

void Image::Paint(Draw& draw, const Rect& src, const Rect& dest, int fx) const
{ draw.DrawImage(dest, *this, src, fx); }

//////////////////////////////////////////////////////////////////////

void Image::Paint(Draw& draw, Point pos, Color fore, Color back, bool xor) const
{ Paint(draw, GetRect(), Rect(pos, GetSize()), fore, back, xor); }

//////////////////////////////////////////////////////////////////////

void Image::Paint(Draw& draw, const Rect& dest, Color fore, Color back, bool xor) const
{ Paint(draw, GetRect(), dest, fore, back, xor); }

//////////////////////////////////////////////////////////////////////

void Image::Paint(Draw& draw, const Rect& src, const Rect& dest, Color fore, Color back, bool xor) const
{ draw.DrawImage(dest, *this, src, fore, back, xor); }

#endif//OLDIMAGE

//////////////////////////////////////////////////////////////////////

void Image::Paint(Image& image, Point pos) const
{
	Paint(image, GetRect(), Rect(pos, GetSize()));
}

//////////////////////////////////////////////////////////////////////

void Image::Paint(Image& image, const Rect& dest) const
{
	Paint(image, GetRect(), dest);
}

//////////////////////////////////////////////////////////////////////

void Image::Paint(Image& image, const Rect& src, const Rect& dest) const
{
	if(HasMask())
	{
		ImageDraw(image).DrawImage(dest, *this, src);
		if(image.HasMask())
			ImageMaskDraw(image).DrawImage(dest, *this, src, Black, Null);
	}
	else
	{
		ImageDraw(image).DrawImage(dest, *this, src);
		if(image.HasMask())
			ImageMaskDraw(image).DrawRect(dest, Black);
	}
}

//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
void Image::PaintData(HDC hdc, Point pos, int fx_rop) const
{ PaintData(hdc, GetRect(), Rect(pos, GetSize()), fx_rop); }
#endif

//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
void Image::PaintData(HDC hdc, const Rect& dest, int fx_rop) const
{ PaintData(hdc, GetRect(), dest, fx_rop); }
#endif

//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
void Image::PaintData(HDC hdc, const Rect& src, const Rect& dest, int fx_rop) const
{
	int rop = (fx_rop & XOR ? SRCINVERT : fx_rop & 0xFFFFFF ? fx_rop & 0xFFFFFF : SRCCOPY);
	Get(fx_rop) -> PaintData(hdc, src, dest, rop);
}
#endif

//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
void Image::PaintMask(HDC hdc, Point pos, Color fore, Color back, int fx_rop) const
{ PaintMask(hdc, Rect(data -> size), Rect(pos, data -> size), fore, back, fx_rop); }
#endif

//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
void Image::PaintMask(HDC hdc, const Rect& dest, Color fore, Color back, int fx_rop) const
{ PaintMask(hdc, Rect(data -> size), dest, fore, back, fx_rop); }
#endif

//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
void Image::PaintMask(HDC hdc, const Rect& src, const Rect& dest, Color fore, Color back, int fx_rop) const
{
	if(IsNull(fore) && IsNull(back))
		return;
	if(fx_rop & XOR)
	{ // xor mode
		fore = Nvl(fore, Black);
		back = Nvl(back, Black);
		fx_rop = SRCINVERT;
	}
	Get(fx_rop) -> PaintMask(hdc, src, dest, fore, back, fx_rop & 0xFFFFFF);
}
#endif

//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
template BitWiseType<HCURSOR>;

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
#endif//PLATFORM_WIN32

//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
const Image& Image::Cursor(int id)
{
	if(HIWORD(id))
		return Res(id);
	return Cursor(LoadCursor(0, MAKEINTRESOURCE(id)));
}
#endif//PLATFORM_WIN32

//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
const Image& Image::Cursor(const char *idc)
{
	return idc ? Cursor((int)idc) : Void();
}
#endif//PLATFORM_WIN32

//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
const Image& Image::Icon(int id)
{
	if(HIWORD(id))
		return Res(id);
	return Cursor(LoadIcon((HINSTANCE)0, MAKEINTRESOURCE(id)));
}
#endif//PLATFORM_WIN32

//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
const Image& Image::Icon(const char *idi)
{
	return idi ? Cursor(LoadIcon((HINSTANCE)0, idi)) : Void();
}
#endif//PLATFORM_WIN32

//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
const Image& Image::Icon(int id, bool small_icon)
{
	int size = (small_icon ? 16 : 32);
	return Cursor((HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(id),
		IMAGE_ICON, size, size, LR_DEFAULTCOLOR));
}
#endif//PLATFORM_WIN32

//////////////////////////////////////////////////////////////////////

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
#endif//PLATFORM_WIN32

//////////////////////////////////////////////////////////////////////

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
#endif//PLATFORM_WIN32

//////////////////////////////////////////////////////////////////////

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
		SetTextColor(mdc, Black);
		SetBkColor(mdc, White);
		mdc.DrawImage(-rc.left, -rc.top, *this, Black, White);
	}
	else if(IsNull(fore))
	{
		mdc.DrawRect(Rect(size), back);
		mdc.DrawImage(-rc.left, -rc.top, *this);
	}
	else
		mdc.DrawImage(-rc.left, -rc.top, *this, fore, back);
	hdc = mdc.Detach();
	SelectObject(hdc, old_bmp);
	DeleteDC(hdc);
	data -> bitmaps.Add(ss, bitmap);
	return bitmap;
}
#endif//PLATFORM_WIN32

//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
HCURSOR Image::GetCursor() const
{
	if(data -> cursor || IsEmpty())
		return data -> cursor;
	static Size cursor_size(GetSystemMetrics(SM_CXCURSOR), GetSystemMetrics(SM_CYCURSOR));
	return data -> cursor = (HCURSOR)GetImage(data -> hotspot, cursor_size);
}
#endif//PLATFORM_WIN32

//////////////////////////////////////////////////////////////////////

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
#endif//PLATFORM_WIN32

//////////////////////////////////////////////////////////////////////

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
#endif//PLATFORM_WIN32

//////////////////////////////////////////////////////////////////////

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
	{ Draw draw(mdc); draw.DrawImage(Rect(r.Size()), *this, r); }
	SelectObject(mdc, old);
	DeleteDC(mdc);
	bool result = ::CreateCaret(hwnd, bmp, r.right, r.bottom);
	DeleteObject(bmp);
	return result;
}
#endif//PLATFORM_WIN32

//////////////////////////////////////////////////////////////////////

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
	if(IsNull(fore))
		PaintData(bdc, dest, dest);
	else
		PaintMask(bdc, dest, dest, fore, Black);
	PaintMask(mdc, dest, dest, Black, White);
	SelectObject(bdc, bdc_bmp);
	SelectObject(mdc, mdc_bmp);
	DeleteDC(bdc);
	DeleteDC(mdc);

	return dest.Size();
}
#endif//PLATFORM_WIN32

//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
HMETAFILE Image::GetMetafile() const
{
	if(IsEmpty())
		return 0;
	HDC hdc = CreateMetaFile(0);
	if(hdc == NULL)
		return 0;
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetWindowExtEx(hdc, GetSize().cx, GetSize().cy, 0);
	PaintMask(hdc, Point(0, 0), Black, Null);
	PaintData(hdc, Point(0, 0), HasMask() ? SRCINVERT : SRCCOPY);
	return CloseMetaFile(hdc);
}
#endif

//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
HENHMETAFILE Image::GetEnhMetafile() const
{
	if(IsEmpty())
		return 0;
	HDC hdc = CreateEnhMetaFile(0, 0, GetRect(), 0);
	if(hdc == NULL)
		return 0;
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetWindowExtEx(hdc, GetSize().cx, GetSize().cy, 0);
	PaintMask(hdc, Point(0, 0), Black, Null);
	PaintData(hdc, Point(0, 0), HasMask() ? SRCINVERT : SRCCOPY);
	return CloseEnhMetaFile(hdc);
}
#endif

//////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WIN32
String Image::GetMetafileData(bool enhanced) const
{
	String data;
	if(enhanced)
	{
		HENHMETAFILE hemf = GetEnhMetafile();
		if(hemf == 0)
			return Null; // error getting metafile
		unsigned size = GetEnhMetaFileBits(hemf, 0, 0);
		if(size > 0)
		{
			GetEnhMetaFileBits(hemf, size, (byte *)data.GetBuffer(size));
			data.ReleaseBuffer(size);
		}
		DeleteEnhMetaFile(hemf);
	}
	else
	{ // old-style meta
		HMETAFILE hmf = GetMetafile();
		if(!hmf)
			return Null; // error getting metafile
		unsigned size = GetMetaFileBitsEx(hmf, 0, 0);
		if(size > 0)
		{
			GetMetaFileBitsEx(hmf, size, data.GetBuffer(size));
			data.ReleaseBuffer(size);
		}
		DeleteMetaFile(hmf);
	}
	return data;
}
#endif

//////////////////////////////////////////////////////////////////////
// ImageDraw::

static HGDIOBJ default_bitmap;

void ImageDraw::Open(Draw& draw, Image& image)
{
	ASSERT(!image.IsEmpty());
	image.Clone(draw, true);
	HDC hdc = CreateCompatibleDC(draw);
	Attach(hdc);
	default_bitmap = ::SelectObject(hdc, image.GetHBITMAP());
	backdraw = true;
}

//////////////////////////////////////////////////////////////////////

void ImageDraw::Close()
{
	while(co.GetCount())
		End();
	HDC hdc = Detach();
	if(hdc)
	{
		SelectObject(hdc, default_bitmap);
		::DeleteDC(hdc);
	}
}

//////////////////////////////////////////////////////////////////////
// ImageMaskDraw::

void ImageMaskDraw::Open(Image& image)
{
	ASSERT(!image.IsEmpty());
	image.Clone(ScreenInfo(), false);
	HDC hdc = CreateCompatibleDC(0);
	Attach(hdc);
	default_bitmap = ::SelectObject(hdc, image.GetMaskHBITMAP());
	backdraw = true;
}

//////////////////////////////////////////////////////////////////////

void ImageMaskDraw::Close()
{
	HDC hdc = Detach();
	if(hdc)
	{
		SelectObject(hdc, default_bitmap);
		::DeleteDC(hdc);
	}
}

//////////////////////////////////////////////////////////////////////
// ImageCache::

int ImageCache::MaxCacheImageSize = 16384;

ImageCache& ImageCache::Get()
{
	static ImageCache cache;
	return cache;
}

//////////////////////////////////////////////////////////////////////

Image::Slot& ImageCache::Add(Size size, bool mono)
{
	ASSERT(size.cx > 0 && size.cy > 0);
	SegtorMap<int, Image::Slot>& map = mono ? mono_cache : color_cache;
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

//////////////////////////////////////////////////////////////////////

/*
#define IMAGEFILE <tctrl/doc.iml>
#define IMAGECLASS NewImg
#include <iml.h>

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

Image ImageTestCtrl::CursorImage(Point pt, dword keyflags)
{
	return NewImg::save_as;
}

//*/
//////////////////////////////////////////////////////////////////////

#endif
