#include "IconDes.h"

struct sFloodFill {
	Rect         rc;
	Size         sz;
	Buffer<byte> flag;
	ImageBuffer  ib;
	RGBA         scolor;
	RGBA         color;
	bool         done;

	RGBA& At(int x, int y)   { return ib[y + rc.top][x + rc.left]; }
	byte& Flag(int x, int y) { return flag[y * sz.cx + x]; }
	void  Fill(Image& img, Color color, Point pt, const Rect& rc);
	void  Try(int x, int y);
};

void sFloodFill::Try(int x, int y)
{
	if(x >= 0 && x < sz.cx && y >= 0 && y < sz.cy && Flag(x, y) == 0 && At(x, y) == scolor) {
		Flag(x, y) = 1;
		At(x, y) = color;
		done = false;
	}
}

void sFloodFill::Fill(Image& img, Color _color, Point pt, const Rect& _rc)
{
	ib = img;
	rc = _rc & ib.GetSize();
	if(!rc.Contains(pt))
		return;
	scolor = ib[pt.y][pt.x];
	color = _color;
	sz = rc.GetSize();
	flag.Alloc(sz.cx * sz.cy, 0);
	pt -= rc.TopLeft();
	Flag(pt.x, pt.y) = 1;
	At(pt.x, pt.y) = color;
	do {
		done = true;
		for(int y = 0; y < sz.cy; y++)
			for(int x = 0; x < sz.cx; x++) {
				if(Flag(x, y) == 1) {
					Flag(x, y) = 2;
					Try(x + 1, y);
					Try(x - 1, y);
					Try(x, y + 1);
					Try(x, y - 1);
				}
			}
	}
	while(!done);
	img = ib;
}

void FloodFill(Image& img, Color color, Point pt, const Rect& rc)
{
	sFloodFill().Fill(img, color, pt, rc);
}

void SetImageRect(Image& dest, const Rect& drect, Color color)
{
	RGBA rgba = color;
	Rect rc = drect & Rect(dest.GetSize());
	ImageBuffer ib(dest);
	for(int y = rc.top; y < rc.bottom; y++)
		memsetex(ib[y] + rc.left, &rgba, sizeof(RGBA), rc.Width());
	dest = ib;
}

void ChangeColor(Image& img, const Rect& drc, Color src, Color dest, int tolerance)
{
	ImageBuffer ib(img);
	Rect rc = drc & ib.GetSize();
	RGBA dc = dest;
	for(int y = rc.top; y < rc.bottom; y++)
		for(int x = rc.left; x < rc.right; x++) {
			RGBA& c = ib[x][y];
			if(c.a == 0 && IsNull(src))
				c = dc;
			else
			if(abs(c.r - src.GetR()) < tolerance &&
			   abs(c.g - src.GetG()) < tolerance &&
			   abs(c.b - src.GetB()) < tolerance)
				c = dc;
		}
	img = ib;
}

void GrayImage(Image& img, const Rect& drect)
{
	ImageBuffer ib(img);
	Rect rc = drect & ib.GetSize();
	for(int y = rc.top; y < rc.bottom; y++)
		for(int x = rc.left; x < rc.right; x++) {
			RGBA& c = ib[y][x];
			c.r = c.g = c.b = Grayscale(c);
		}
	img = ib;
}

void SwapHorz(Image& img, const Rect& rect)
{
	ImageBuffer ib(img);
	Rect rc = rect & ib.GetSize();
	for(int y = rc.top; y < rc.bottom; y++) {
		RGBA *b = ib[y] + rc.left;
		RGBA *e = ib[y] + rc.right - 1;
		while(b < e) {
			Swap(*b, *e);
			b++;
			e--;
		}
	}
	img = ib;
}

void SwapVert(Image& img, const Rect& rect)
{
	ImageBuffer ib(img);
	Rect rc = rect & ib.GetSize();
	int b = rc.top;
	int e = rc.bottom - 1;
	int n = rc.right - rc.left;
	if(n > 0) {
		Buffer<RGBA> h(n);
		n *= sizeof(RGBA);
		while(b < e) {
			memcpy(h, ib[b] + rc.left, n);
			memcpy(ib[b] + rc.left, ib[e] + rc.left, n);
			memcpy(ib[e] + rc.left, h, n);
			b++;
			e--;
		}
	}
	img = ib;
}

void   SetImagePixel(Image& image, int x, int y, Color c)
{
	Size sz = image.GetSize();
	if(y >= 0 && y < sz.cy && x >= 0 && x <= sz.cx) {
		ImageBuffer ib(image);
		ib[y][x] = c;
		image = ib;
	}
}

Color GetImagePixel(const Image& src, Point p)
{
	Size sz = src.GetSize();
	if(p.x < sz.cx && p.x >= 0 && p.y <= sz.cy && p.y >= 0)
		return src[p.y][p.x];
	return Null;
}

Vector<Color> GetImagePixel(const Image& src, const Vector<Point>& p)
{
	Vector<Color> vc;
	for(int i = 0; i < p.GetCount(); i++)
		vc.Add(GetImagePixel(src, p[i]));
	return vc;
}

struct InterpolateFilter : ImageFilter9 {
	int todo;

	virtual RGBA operator()(const RGBA **mx);
};

RGBA InterpolateFilter::operator()(const RGBA **mx)
{
	RGBA t = mx[1][1];
	int na = mx[0][1].a, wa = mx[1][0].a, ea = mx[1][2].a, sa = mx[2][1].a;
	int suma = na + wa + ea + sa, half = suma >> 1;
	if(suma > 0) {
		t.r = (mx[0][1].r * na + mx[1][0].r * wa + mx[1][2].r * ea + mx[2][1].r * sa + half) / suma;
		t.g = (mx[0][1].g * na + mx[1][0].g * wa + mx[1][2].g * ea + mx[2][1].g * sa + half) / suma;
		t.b = (mx[0][1].b * na + mx[1][0].b * wa + mx[1][2].b * ea + mx[2][1].b * sa + half) / suma;
		t.a = max(max(na, sa), max(wa, ea));
		if(t != mx[1][1])
			todo++;
	}
	return t;
}

void InterpolateImage(Image& img, const Rect& _rc)
{
	Rect rc = _rc & img.GetSize();
	Image m = Crop(img, rc);
	TimeStop tm;
	Image imp = CreateImage(rc.GetSize(), Null);
	Over(imp, Point(0, 0), m, m.GetSize());
	Progress pi("Interpolating...");
	int n = rc.GetWidth() * rc.GetHeight();
	for(int qq = 0; qq < 2000; qq++) {
		InterpolateFilter f;
		f.todo = 0;
		imp = Filter(imp, f);
		Over(imp, Point(0, 0), m, m.GetSize());
		if(f.todo == 0)
			break;
		if(pi.SetCanceled(qq, 2000))
			break;
	}
	Copy(img, rc.TopLeft(), imp, imp.GetSize());
}

String AlphaToRLE(const Image& aa)
{
	String result;
	int last = 0;
	for(int y = 0; y < aa.GetHeight(); y++) {
		result.Cat(PackRLE(aa[y], aa.GetWidth()));
		result.Cat(0x80);
	}
	return result;
}

Image RLEToAlpha(const String& rle, Size sz)
{
	ImageBuffer ib(sz);
	const byte *s = rle;
	for(int y = 0; y < sz.cy; y++)
		if((const char *)s < rle.End())
			s = UnpackRLE(ib[y], s, sz.cx) + 1;
		else
			memset(ib[y], 0, sz.cx * sizeof(RGBA));
	return ib;
}

void AlphaImageInfo::Serialize(Stream& stream)
{
	int version = 1;
	stream / version;
	if(version >= 1)
		stream % size % hotspot % encoding;
}

void ScanIML(CParser& parser, Array<IMLImageInfo>& out_images, VectorMap<String, String>& out_settings)
{
	String name, bid;
	while(!parser.IsEof())
	{
		if((bid = parser.ReadId()) == "IMAGE_META" && parser.Char('(') && !IsNull(name = parser.ReadId()))
		{
			parser.PassChar(',');
			String value = parser.ReadString();
			parser.PassChar(')');
			out_settings.Add(name, value);
		}
		else if(bid == "IMAGE_BEGIN" && parser.Char('(') && !IsNull(name = parser.ReadId()) && parser.Char(')'))
		{
			String encoded_data;
			out_settings.GetAdd("wince_16bit", "0");
			String id;
			bool first = true;
			while((id = parser.ReadId()) == "IMAGE_SCAN" && parser.Char('('))
			{
				bool first_in_row = true;
				while(parser.IsChar('\"'))
				{
					String scan = parser.ReadOneString();
					if(!first && first_in_row)
						encoded_data.Cat('\x80');
					first_in_row = first = false;
					encoded_data.Cat(scan);
				}
				if(!parser.Char(')'))
					break;
			}
			AlphaImageInfo image;
			bool accepted = false;
			if(parser.Char('(') && parser.ReadId() == name && parser.Char(','))
				if(id == "IMAGE_END"
				&& (image.size.cx = parser.ReadInt()) > 0 && parser.Char(',')
				&& (image.size.cy = parser.ReadInt()) > 0 && parser.Char(')'))
				{
					accepted = true;
				}
				else if(id == "IMAGE_PACKED" && parser.IsChar('\"'))
				{
					String d = parser.ReadOneString();
					if(parser.Char(')'))
					{
						StringStream ss(d);
						ss % image;
						if(!ss.IsError())
							accepted = true;
					}
				}

			if(name.GetLength() >= 6 && !memcmp(name, "_java_", 6))
				accepted = false;

			if(accepted)
			{
				if(name.GetLength() >= 4 && !memcmp(name, "im__", 4))
					name = Null;

				IMLImageInfo& info = out_images.Add();
				info.AlphaImageInfo::operator = (image);
				info.name = name;
				info.encoded_data = encoded_data;
			}
		}
		else if(bid == "IMAGE_BEGIN16" && parser.Char('(') && !IsNull(name = parser.ReadId()) && parser.Char(')'))
		{
			out_settings.GetAdd("wince_16bit", "1");
			String encoded_data;
			String id;
			bool first = true;
			while((id = parser.ReadId()) == "IMAGE_SCAN16" && parser.Char('(') && parser.Char('L'))
			{
				bool first_in_row = true;
				while(parser.Char('\"'))
				{
					CParser::Pos pos = parser.GetPos();
					const char *end;
					String scan; // TODO = GetUnicodeScan(pos.ptr, &end);
					pos.ptr = end;
					parser.SetPos(pos);
					if(!parser.Char('\"'))
						break;
					if(!first && first_in_row)
						encoded_data.Cat('\x80');
					first_in_row = first = false;
					encoded_data.Cat(scan);
				}
				if(!parser.Char(')'))
					break;
			}
			AlphaImageInfo idata;
			bool accepted = false;
			if(id == "IMAGE_END16" && parser.Char('(') && parser.ReadId() == name && parser.Char(',')
			&& (idata.size.cx = parser.ReadInt()) > 0 && parser.Char(',')
			&& (idata.size.cy = parser.ReadInt()) > 0 && parser.Char(',')
			&& !IsNull(idata.hotspot.x = parser.ReadInt()) && parser.Char(',')
			&& !IsNull(idata.hotspot.y = parser.ReadInt()) && parser.Char(')'))
			{
				accepted = true;
			}

			if(accepted)
			{
				if(name.GetLength() >= 4 && !memcmp(name, "im__", 4))
					name = Null;

				IMLImageInfo& info = out_images.Add();
				info.AlphaImageInfo::operator = (idata);
				info.name = name;
				info.encoded_data = encoded_data;
			}
		}
		else
			break;
	}
}
