//////////////////////////////////////////////////////////////////////
// designer/ImageBuffer: image canvas.

#ifndef __DESIGNER_IMAGEBUFFER__
#define __DESIGNER_IMAGEBUFFER__

//////////////////////////////////////////////////////////////////////

class ImageBufferBase
{
protected:
	int          data_scan;
	int          mask_scan;
	int          top;
	Size         size;
};

//////////////////////////////////////////////////////////////////////

class ImageBuffer : public ImageBufferBase
{
public:
	ImageBuffer();
	ImageBuffer(Size size);
	ImageBuffer(int cx, int cy);
	ImageBuffer(const ImageBuffer& buffer, int); // deep copy

	operator bool () const                         { return size.cx && size.cy; }

	void         Serialize(Stream& stream);

	void         WriteDib(Stream& stream, const Rect& rc, Color bgcolor = White) const;
	void         WriteDib(Stream& stream, Color bgcolor = White) const { WriteDib(stream, Rect(size), bgcolor); }
	
	static int   GetDibSize(Size size);
	int          GetDibSize() const                { return GetDibSize(size); }

	ImageBuffer& From(Draw& draw, const Rect& rc, Color bgcolor = Null);
	ImageBuffer& From(Draw& data, const Rect& rc_data, Draw& mask, Point pos_mask);
	ImageBuffer& From(Draw& draw, const Rect& rc_data, Point pos_mask) { return From(draw, rc_data, draw, pos_mask); }

	ImageBuffer& FromClipboard(Color bgcolor = Null);

	ImageBuffer  GetPart(const Rect& rc) const;
	ImageBuffer  GetPart(const Rect& rc, Size size) const;

	static void  SetBitmapHeader(BITMAPINFOHEADER& bmih, Size size);
	void         SetBitmapHeader(BITMAPINFOHEADER& bmih) const { SetBitmapHeader(bmih, size); }

	void         SetSize(Size size);
	void         SetSize(int cx, int cy)           { SetSize(Size(cx, cy)); }
	Size         GetSize() const                   { return size; }
	int          GetDataScan() const               { return data_scan; }
	int          GetMaskScan() const               { return mask_scan; }

	bool         HasMask() const;

	void         Reset()                           { Reconstruct(*this); }
	void         Clear();

	int          GetDataOffset(int x, int y) const { return 3 * x + (top - y) * data_scan; }
	int          GetDataOffset(Point pt) const     { return GetDataOffset(pt.x, pt.y); }

	int          GetMaskOffset(int x, int y) const { return x + (top - y) * mask_scan; }
	int          GetMaskOffset(Point pt) const     { return GetMaskOffset(pt.x, pt.y); }

	const byte  *Data(int x, int y) const          { return &data[GetDataOffset(x, y)]; }
	byte        *Data(int x, int y)                { return &data[GetDataOffset(x, y)]; }

	const byte  *Data(Point pt) const              { return &data[GetDataOffset(pt)]; }
	byte        *Data(Point pt)                    { return &data[GetDataOffset(pt)]; }

	const byte  *Mask(int x, int y) const          { return &mask[GetMaskOffset(x, y)]; }
	byte        *Mask(int x, int y)                { return &mask[GetMaskOffset(x, y)]; }

	const byte  *Mask(Point pt) const              { return &mask[GetMaskOffset(pt)]; }
	byte        *Mask(Point pt)                    { return &mask[GetMaskOffset(pt)]; }

	bool         Contains(int x, int y) const      { return (unsigned)x < (unsigned)size.cx && (unsigned)y < (unsigned)size.cy; }
	bool         Contains(Point pt) const          { return Contains(pt.x, pt.y); }

	void         SetPixel(int x, int y, Color color);
	void         SetPixel(Point pt, Color color)   { SetPixel(pt.x, pt.y, color); }
	Color        GetPixel(Point pt) const;

	void         PaintData(Draw& draw, const Rect& src, const Rect& dest, int rop = SRCCOPY) const;
	void         PaintMask(Draw& draw, const Rect& src, const Rect& dest, Color color0 = Black, Color color1 = White, int rop = SRCAND) const;
	void         Paint(Draw& draw, const Rect& src, const Rect& dest, Color color = Null) const;
	void         Paint(Draw& draw, const Rect& dest, Color color = Null) const;
	void         Paint(Draw& draw, Point pos, Color color = Null) const;

	void         GetScanRLE(String& dest, int y) const;
	const byte  *SetScanRLE(const byte *src, int y);

	String       GetRLE() const;
	void         SetRLE(const byte *src);

	void         SetGrayed(const Rect& rc);
	void         SetEtched(const Rect& rc);

	void         SetGrayed()                       { SetGrayed(Rect(size)); }
	void         SetEtched()                       { SetEtched(Rect(size)); }

	void         Fill(const Rect& rc, Color color);
	void         Fill(Color color)                 { Fill(Rect(size), color); }

	void         Copy(Point pos, const ImageBuffer& src, const Rect& src_rect, bool transparent = false);
	void         Copy(Point pos, const ImageBuffer& src, bool transparent = false) { Copy(pos, src, Rect(src.size), transparent); }
	void         Copy(const Rect& dest, const ImageBuffer& src, const Rect& src_rect, bool transparent = false);

	void         ChangeColor(const Rect& rc, Color src, Color dest, int tolerance = 0);
	void         ChangeColor(Color src, Color dest, int tolerance = 0) { ChangeColor(Rect(size), src, dest, tolerance); }
	
	void         FloodFill(Color color, Point pt, const Rect& rc, bool diagonal = false);
	void         FloodFill(Color color, Point pt, bool diagonal = false) { FloodFill(color, pt, Rect(size), diagonal); }

	void         Curve(const Vector<Point>& list, Color color);

	void         MirrorX(const Rect& rc);
	void         MirrorX()                         { MirrorX(Rect(size)); }

	void         MirrorY(const Rect& rc);
	void         MirrorY()                         { MirrorY(Rect(size)); }

	void         MirrorXY(const Rect& rc)          { MirrorX(rc); MirrorY(rc); }
	void         MirrorXY()                        { MirrorXY(Rect(size)); }

	ImageBuffer  GetRotated(const Rect& rc) const; // 90 deg counterclockwise
	ImageBuffer  GetRotated() const                { return GetRotated(Rect(size)); }

public:
	Vector<byte> data;
	Vector<byte> mask;

private:
	void         SetPixel(int x, int y, int count, const byte *data, const byte *mask, bool transparent = true);
};

template class DeepCopyOption<ImageBuffer>;
template class Moveable<ImageBuffer>;

#endif//__DESIGNER_IMAGEBUFFER__
