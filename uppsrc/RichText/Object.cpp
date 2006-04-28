#include "RichText.h"

#include <CtrlLib/CtrlLib.h>

RichObjectType::RichObjectType() {}
RichObjectType::~RichObjectType() {}

String RichObjectType::GetCreateName() const
{
	return Null;
}

Size RichObjectType::GetDefaultSize(const Value& data, Size maxsize) const
{
	if(IsNull(data)) return Size(0, 0);
	Size psz = GetPhysicalSize(data);
	if((psz.cx | psz.cy) == 0)
		psz = 625 * GetPixelSize(data) / 100;
	Size sz;
	for(int i = 1; i < 10000; i++) {
		sz = psz / i;
		if(sz.cx <= maxsize.cx && sz.cy <= maxsize.cy)
			break;
	}
	return sz;
}

Value RichObjectType::Read(const String& s) const
{
	return s;
}

String RichObjectType::Write(const Value& v) const
{
	return v;
}

Value RichObjectType::ReadClipboard() const
{
	return Null;
}

void RichObjectType::WriteClipboard(const Value& v) const {}
void RichObjectType::Menu(Bar& bar, RichObjectExchange& data) const {}
void RichObjectType::DefaultAction(RichObjectExchange& data) const {}
Size RichObjectType::GetPhysicalSize(const Value& data) const { return Size(0, 0); }
Size RichObjectType::GetPixelSize(const Value& data) const { return Size(1, 1); }
void RichObjectType::Paint(const Value& data, Draw& w, Size sz) const {}

String RichObjectType::GetLink(const Value& data, Point pt, Size sz) const
{
	return Null;
}

typedef VectorMap<String, RichObjectType *> helptype;

GLOBAL_VAR(helptype, RichObject::Map)

void RichObject::NewSerial()
{
	INTERLOCKED {
		static int64 s;
		serial = ++s;
	}
}

void RichObject::Register(const char *name, RichObjectType *type)
{
	Map().FindAdd(name, type);
}

void RichObject::Paint(Draw& w, Size sz) const
{
	if(type)
		type->Paint(data, w, sz);
	else {
		w.DrawRect(sz, SColorFace());
		DrawFrame(w, sz, SColorText());
		w.DrawText(2, 2, type_name);
	}
}

struct UnknownRichObject : RichObjectType {
	virtual String GetTypeName(const Value&) const;
};

String UnknownRichObject::GetTypeName(const Value&) const
{
	return Null;
}

const RichObjectType& RichObject::GetType() const
{
	return type ? *type : Single<UnknownRichObject>();
}

void   RichObject::Set(RichObjectType *_type, const Value& _data, Size maxsize)
{
	Clear();
	type = _type;
	if(type) {
		data = _data;
		physical_size = type->GetPhysicalSize(data);
		pixel_size = type->GetPixelSize(data);
		size = type->GetDefaultSize(data, maxsize);
	}
	NewSerial();
}

bool   RichObject::Set(const String& _type_name, const Value& _data, Size maxsize)
{
	NewSerial();
	type_name = _type_name;
	RichObjectType *t = Map().Get(type_name, NULL);
	if(t) {
		Set(t, _data);
		return true;
	}
	else {
		Clear();
		data = _data;
		physical_size = pixel_size = size = Size(64, 64);
	}
	return false;
}

bool   RichObject::Read(const String& _type_name, const String& _data, Size sz)
{
	NewSerial();
	type_name = _type_name;
	RichObjectType *t = Map().Get(type_name, NULL);
	if(t) {
		Clear();
		type = t;
		data = type->Read(_data);
		physical_size = type->GetPhysicalSize(data);
		pixel_size = type->GetPixelSize(data);
		size = sz;
		return true;
	}
	data = _data;
	physical_size = pixel_size = size = sz;
	return false;
}

String RichObject::GetTypeName() const
{
	return type ? type->GetTypeName(data) : type_name;
}

void   RichObject::Clear()
{
	NewSerial();
	keepratio = true;
	type = NULL;
	data = Value();
	size = physical_size = pixel_size = Size(0, 0);
	ydelta = 0;
	type_name.Clear();
}

RichObject::RichObject()
{
	Clear();
}

RichObject::RichObject(RichObjectType *type, const Value& data, Size maxsize)
{
	Set(type, data, maxsize);
}

RichObject::RichObject(const String& type, const Value& data, Size maxsize)
{
	Set(type, data, maxsize);
}

#ifndef NEWIMAGE

#ifdef PLATFORM_WIN32

struct RichDib : public RichObjectType {
	virtual String GetTypeName(const Value&) const;
	virtual Size   GetPhysicalSize(const Value& data) const;
	virtual Size   GetPixelSize(const Value& data) const;
	virtual void   Paint(const Value& data, Draw& w, Size sz) const;
	virtual Value  ReadClipboard() const;
	virtual void   WriteClipboard(const Value& v) const;

	typedef RichDib CLASSNAME;
};

String RichDib::GetTypeName(const Value&) const
{
	return "DIB";
}

Value RichDib::ReadClipboard() const
{
//	if(IsClipboardAvailable(CF_DIB))
//		return ::ReadClipboard(CF_DIB);
	return Null;
}

void  RichDib::WriteClipboard(const Value& v) const
{
	ClearClipboard();
	::WriteClipboard(CF_DIB, v);
}

Size   RichDib::GetPixelSize(const Value& data) const
{
	String d = data;
	if(d.GetLength() < sizeof(BITMAPINFOHEADER)) return Size(0, 0);
	const BITMAPINFOHEADER *header = (const BITMAPINFOHEADER *) ~d;
	return Size(header->biWidth, header->biHeight);
}

Size   RichDib::GetPhysicalSize(const Value& data) const
{
	Size sz;
	String d = data;
	if(d.GetLength() < sizeof(BITMAPINFOHEADER)) return Size(0, 0);
	const BITMAPINFOHEADER *header = (const BITMAPINFOHEADER *) ~d;
	if(header->biXPelsPerMeter && header->biYPelsPerMeter) {
		sz.cx = fround(header->biWidth * (1000.0 / 25.4 * 600.0) / header->biXPelsPerMeter);
		sz.cy = fround(header->biHeight * (1000.0 / 25.4 * 600.0) / header->biYPelsPerMeter);
	}
	else {
		int n = 10;
		do {
			sz.cx = n * header->biWidth / 10;
			sz.cy = n * header->biHeight / 10;
			n--;
		}
		while((sz.cx > 3967 || sz.cy > 3967) && n > 0);
	}
	return sz;
}

void   RichDib::Paint(const Value& data, Draw& w, Size sz) const
{
	HDC dc = w.BeginGdi();
	String d = data;
	const BITMAPINFOHEADER& header = *(const BITMAPINFOHEADER *) ~d;
	if(header.biSize >= 16) {
		byte *bits = (byte *)~d + header.biSize;
		if(header.biBitCount <= 8)
			bits += (header.biClrUsed ? header.biClrUsed : 1 << header.biBitCount)
			        * sizeof(RGBQUAD);
		if(header.biBitCount == 16 || header.biBitCount == 32 &&
		   header.biCompression == BI_BITFIELDS)
				bits += 12;
		if(header.biBitCount >= 16 && header.biClrUsed != 0)
			bits += header.biClrUsed * sizeof(RGBQUAD);
		SetStretchBltMode(dc, HALFTONE);
		StretchDIBits(dc, 0, 0, sz.cx, sz.cy,
		              0, 0, header.biWidth, header.biHeight,
		              bits, (BITMAPINFO *)~d, DIB_RGB_COLORS, SRCCOPY);
	}
	w.EndGdi();
}

INITBLOCK
{
	RichObject::Register("DIB", &Single<RichDib>());
};

#endif

struct RichPng : public RichObjectType {
	virtual String GetTypeName(const Value&) const;
	virtual Size   GetPhysicalSize(const Value& data) const;
	virtual Size   GetPixelSize(const Value& data) const;
	virtual void   Paint(const Value& data, Draw& w, Size sz) const;
	virtual Value  ReadClipboard() const;
	virtual void   WriteClipboard(const Value& v) const;

	typedef RichPng CLASSNAME;
};

String RichPng::GetTypeName(const Value&) const
{
	return "PNG";
}

Value RichPng::ReadClipboard() const
{
#ifdef PLATFORM_WIN32
#ifdef NEWIMAGE
	//FIXIMAGE
#else
	if(IsClipboardAvailable(CF_DIB))
		return PngEncoder().SaveImage(ClipboardToImage());
#endif
#endif
	return Null;
}

void  RichPng::WriteClipboard(const Value& v) const
{
	ClearClipboard();
#ifdef PLATFORM_WIN32
#ifdef NEWIMAGE
	//FIXIMAGE
#else
	ImageToClipboard(PngEncoder().LoadImage(v));
#endif
#endif
}

Size   RichPng::GetPixelSize(const Value& data) const
{
#ifdef NEWIMAGE
	//FIXIMAGE
	return Size(0, 0);
#else
	return PngEncoder().InfoImage(data).size;
#endif
}

Size   RichPng::GetPhysicalSize(const Value& data) const
{
#ifdef NEWIMAGE
	//FIXIMAGE
	return Size(0, 0);
#else
	ImageInfo ii = PngEncoder().InfoImage(data);
	Size isz = ii.dots;
	if(isz.cx == 0 || isz.cy == 0)
		isz = iscale(ii.size, Size(600, 600), ScreenInfo().GetPixelsPerInch());
	while(isz.cx > 3967 || isz.cy > 3967) isz >>= 1;
	return isz;
#endif
/*
	Size isz = PngEncoder().InfoImage(data).size;
	int n = 40;
	Size sz;
	do {
		sz.cx = n * isz.cx / 10;
		sz.cy = n * isz.cy / 10;
		n--;
	}
	while((sz.cx > 3967 || sz.cy > 3967) && n > 0);
	return sz;
*/
}

void   RichPng::Paint(const Value& data, Draw& w, Size sz) const
{
#ifdef NEWIMAGE
	//FIXIMAGE
#else
	PixelArray x = PngEncoder().LoadArray(data).pixel;
	Size outsz(min(sz.cx, 4 * x.GetWidth()), min(sz.cy, 4 * x.GetHeight()));
	if(w.IsDrawing())
		w.DrawImage(Rect(sz), PixelArrayToImage(x));
	else {
		Size scale = min(outsz, sz << 2);
		PixelArray dest(scale);
		PixelCopyAntiAlias(dest, scale, x, x.GetSize());
		if(w.IsSystem() && !w.IsDrawing())
			dest.Paint(w, Rect(sz));
		else
			w.DrawImage(Rect(sz), PixelArrayToImage(dest));
	}
#endif
}

INITBLOCK {
	RichObject::Register("PNG", &Single<RichPng>());
};

#endif

struct RichObjectTypeDrawingCls : public RichObjectType
{
	virtual String GetTypeName(const Value&) const;
	virtual Size   GetPhysicalSize(const Value& data) const;
	virtual Size   GetPixelSize(const Value& data) const;
	virtual void   Paint(const Value& data, Draw& w, Size sz) const;
	virtual Value  Read(const String& s) const;
	virtual String Write(const Value& v) const;
//	virtual Value  ReadClipboard() const;
//	virtual void   Menu(Bar& bar, RichObjectExchange& ex) const;
//	virtual void   DefaultAction(RichObjectExchange& ex) const;

	struct Data
	{
		void    Serialize(Stream& stream);

		Drawing drawing;
		Size    dot_size;
	};
};

RichObjectType *RichObjectTypeDrawing() { return &Single<RichObjectTypeDrawingCls>(); }

void RichObjectTypeDrawingCls::Data::Serialize(Stream& stream)
{
	int version = 1;
	stream % version % dot_size % drawing;
}

String RichObjectTypeDrawingCls::GetTypeName(const Value&) const
{
	return "Drawing";
}

RichObject CreateDrawingObject(const Drawing& dwg, Size dot_size, Size out_size)
{
	RichObjectTypeDrawingCls::Data data;
	data.drawing = dwg;
	data.dot_size = dot_size;
	RichObject obj(RichObjectTypeDrawing(), RawToValue(data));
	if(!IsNull(out_size))
		obj.SetSize(out_size);
	return obj;
}

Size RichObjectTypeDrawingCls::GetPixelSize(const Value& data) const
{
	if(IsTypeRaw<Data>(data))
		return ValueTo<Data>(data).dot_size;
	return Size(0, 0);
}

Size RichObjectTypeDrawingCls::GetPhysicalSize(const Value& data) const
{
	if(IsTypeRaw<Data>(data))
		return ValueTo<Data>(data).dot_size;
	return Size(0, 0);
}

Value RichObjectTypeDrawingCls::Read(const String& s) const
{
	Data data;
	StringStream ss(s);
	ss % data;
	if(ss.IsError())
		return Value();
	return RawToValue(data);
}

String RichObjectTypeDrawingCls::Write(const Value& v) const
{
	if(IsTypeRaw<Data>(v))
	{
		StringStream ss;
		ss % const_cast<Data&>(ValueTo<Data>(v));
		return ss;
	}
	return Null;
}

void RichObjectTypeDrawingCls::Paint(const Value& data, Draw& w, Size sz) const
{
	w.DrawRect(sz, White);
	if(IsTypeRaw<Data>(data))
		w.DrawDrawing(Rect(sz), ValueTo<Data>(data).drawing);
}

INITBLOCK {
	RichObject::Register("Drawing", &Single<RichObjectTypeDrawingCls>());
};
