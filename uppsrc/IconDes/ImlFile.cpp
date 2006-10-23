#include "IconDes.h"

class AlphaImageInfo
{
public:
	enum ENCODING { COLOR_RLE, MONO_RLE, MONO_PACKED };

	AlphaImageInfo(Size size = Size(0, 0), int encoding = COLOR_RLE, Point hotspot = Point(0, 0))
		: size(size), encoding(encoding), hotspot(hotspot) {}

	void               Serialize(Stream& stream);

public:
	int                encoding;
	Size               size;
	Point              hotspot;
};

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

void ScanIML(CParser& parser, ArrayMap<String, Image>& out_images,
             VectorMap<String, String>& out_settings)
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

				Image m = RLEToAlpha(encoded_data, image.size);
				ImageBuffer ib(m);
				ib.SetHotSpot(image.hotspot);
				m = ib;
				out_images.Add(name, m);
			}
		}
		else if(bid == "IMAGE_BEGIN16" && parser.Char('(') && !IsNull(name = parser.ReadId()) && parser.Char(')'))
		{ //TODO: FIX THESE!!!
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

				Image m = RLEToAlpha(encoded_data, idata.size);
				ImageBuffer ib(m);
				ib.SetHotSpot(idata.hotspot);
				m = ib;
				out_images.Add(name, m);
			}
		}
		else
			break;
	}
}

bool LoadIml(const String& data, VectorMap<String, Image>& img, int& format)
{
	CParser p(data);
	format = 0;
	try {
		Vector<String> name;
		while(p.Id("IMAGE_ID")) {
			p.PassChar('(');
			String n = p.ReadId();
			if(n.GetLength() > 4 && memcmp(n, "im__", 4) == 0)
				n = Null;
			name.Add(n);
			p.PassChar(')');
		}
		int ii = 0;
		while(p.Id("IMAGE_BEGIN_DATA")) {
			String data;
			while(p.Id("IMAGE_DATA")) {
				p.PassChar('(');
				for(int j = 0; j < 32; j++) {
					if(j) p.PassChar(',');
					data.Cat(p.ReadInt());
				}
				p.PassChar(')');
			}
			p.PassId("IMAGE_END_DATA");
			p.PassChar('(');
			int zlen = p.ReadInt();
			p.PassChar(',');
			int count = p.ReadInt();
			p.PassChar(')');

			data.Trim(zlen);
			Vector<Image> m = UnpackImlData(data);
			if(m.GetCount() != count || ii + count > name.GetCount())
				p.ThrowError("");
			for(int i = 0; i < count; i++)
				img.Add(name[ii++], m[i]);
		}
		if(!p.IsEof())
			p.ThrowError("");
	}
	catch(CParser::Error) {
		try {
			CParser p(data);
			ArrayMap<String, Image> m;
			VectorMap<String, String> s;
			ScanIML(p, m, s);
			if(m.GetCount())
				format = 1;
			for(int i = 0; i < m.GetCount(); i++)
				img.Add(m.GetKey(i), m[i]);
		}
		catch(...) {
			return false;
		}
	}
	return true;
}

static void PutOctalString(Stream& out, const char *b, const char *e, bool split = false)
{
	out.Put('\"');
	int64 start = out.GetPos();
	while(b < e) {
		if(split && out.GetPos() >= start + 200u) {
			out.Put("\"\r\n\t\"");
			start = out.GetPos();
		}
		if((byte)*b >= ' ' && *b != '\x7F' && *b != '\xFF') {
			if(*b == '\\' || *b == '\"' || *b == '\'')
				out.Put('\\');
			out.Put(*b++);
		}
		else if(IsDigit(b[1]))
			out.Put(Sprintf("\\%03o", (byte)*b++));
		else
			out.Put(Sprintf("\\%o", (byte)*b++));
	}
	out.Put('\"');
}

static void PutOctalString(Stream& out, const String& s, bool split = false)
{
	PutOctalString(out, s.Begin(), s.End(), split);
}

String SaveIml(const VectorMap<String, Image>& iml, int format) {
	StringStream out;
	if(format == 1) {
		for(int i = 0; i < iml.GetCount(); i++) {
			String name = iml.GetKey(i);
			if(IsNull(name))
				name = "im__" + IntStr(i);
			Image buffer = iml[i];
			out.PutLine(NFormat("IMAGE_BEGIN(%s)", name));
			int last = 0;
			for(int i = 0; i < buffer.GetHeight(); i++) {
				String scan = PackRLE(buffer[i], buffer.GetWidth());
				if(!scan.IsEmpty() || i == 0) // force at least 1 scan
				{
					for(; last < i; last++)
						out.PutLine("\tIMAGE_SCAN(\"\")");
					out.Put("\tIMAGE_SCAN(");
					PutOctalString(out, scan.Begin(), scan.End(), true);
					out.Put(")\r\n");
					last = i + 1;
				}
			}
			out.Put("IMAGE_PACKED(");
			out.Put(name);
			out.Put(", ");
			StringStream datastrm;
			Size size = buffer.GetSize();
			Point hotspot = buffer.GetHotSpot();
			int encoding = AlphaImageInfo::COLOR_RLE;
			int version = 1;
			datastrm / version;
			datastrm % size % hotspot % encoding;
			ASSERT(!datastrm.IsError());
			String s = datastrm.GetResult();
			PutOctalString(out, s.Begin(), s.End());
			out.Put(")\r\n");
		}
	}
	else {
		for(int i = 0; i < iml.GetCount(); i++)
			out << "IMAGE_ID(" << iml.GetKey(i) << ")\r\n";
		int ii = 0;
		while(ii < iml.GetCount()) {
			int bl = 0;
			int bn = 0;
			Vector<Image> bimg;
			while(bl < 4096 && ii < iml.GetCount()) {
				Image img = iml[ii++];
				bimg.Add(img);
				bl += img.GetLength();
				bn++;
			}
			String bs = PackImlData(bimg);
			out << "\r\nIMAGE_BEGIN_DATA\r\n";
			bs.Cat(0, ((bs.GetCount() + 31) & ~31) - bs.GetCount());
			const byte *s = bs;
			for(int n = bs.GetCount() / 32; n--;) {
				out << "IMAGE_DATA(";
				for(int j = 0; j < 32; j++) {
					if(j) out << ',';
					out << (int)*s++;
				}
				out << ")\r\n";
			}
			out << "IMAGE_END_DATA(" << bs.GetCount() << ", " << bn << ")\r\n";
		}
	}
	return out.GetResult();
}
