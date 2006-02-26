//////////////////////////////////////////////////////////////////////
// zimage: ZLIB-based image format.

#include <Draw/Draw.h>
#pragma hdrstop

#include <plugin/zim/zim.h>

#define LLOG(x) // RLOG(x)

static void PixelZStreamInfo(Stream& stream, Size& size, Size& dot_size, Point& hotspot, int& raw_bpp, bool& mono)
{
	enum { VERSION = 1 };
	int version = VERSION;
	stream.Magic(('P' << 24) | ('X' << 16) | ('A' << 8) | 'R');
	stream / version;
	if(version > VERSION)
	{
		stream.SetError();
		return;
	}
	Pack16(stream, raw_bpp, size.cx, size.cy, hotspot.x, hotspot.y);
	stream % mono % dot_size;
}

struct ZImageDirItem
{
	Size  size;
	Size  dot_size;
	Point hotspot;
	int   raw_bpp;
	bool  mono;
	int   alpha_bpp;

	void  Serialize(Stream& stream)
	{
		PixelZStreamInfo(stream, size, dot_size, hotspot, raw_bpp, mono);
		Pack16(stream, alpha_bpp);
	}

	void  Read(const AlphaArray& aa);
	void  Create(AlphaArray& aa) const;
};

static void PixelZStreamData(Stream& stream, PixelArray& array,
	String (*compress)(String data), String (*decompress)(String data))
{
	enum { VERSION = 1 };
	int version = VERSION;
	stream / version;
	if(version > VERSION)
	{
		LLOG("PixelZStreamData -> version error: " << version);
		stream.SetError();
		return;
	}
	if(array.IsEmpty())
		return;
	if(array.GetBPP() <= 8)
		stream % array.palette;
	else if(array.GetBPP() == 16 || array.GetBPP() == 32)
	{
		dword cmask[3];
		cmask[0] = array.cmask[0];
		cmask[1] = array.cmask[1];
		cmask[2] = array.cmask[2];
		stream % cmask[0] % cmask[1] % cmask[2];
	}
	int block_size = idivceil(array.GetHeight(), 1 + (array.GetBytes() >> 24));
	Pack16(stream, block_size);
	if(block_size <= 0)
	{
		LLOG("PixelZStreamData -> block size error: " << block_size);
		stream.SetError();
		return;
	}
	int wd = array.GetWidth();
	int row = array.GetRowBytes(), row1 = row;
	bool pack8 = (array.GetBPP() == 8 && array.IsMono());
	Buffer<byte> rowbuf;
	if(pack8)
	{
		rowbuf.Alloc(row);
		row1 = array.GetRowBytes(1);
	}
	for(int i = 0; i < array.GetHeight();)
	{
		String part;
		int e = min(array.GetHeight(), i + block_size);
		if(stream.IsStoring())
		{
			for(; i < e; i++)
				if(pack8)
				{
					Blt8to1(rowbuf, array.GetDownScan(i), wd);
					part.Cat(rowbuf, row1);
				}
				else
					part.Cat(array.GetDownScan(i), row);
			part = compress(part);
		}
		stream % part;
		if(stream.IsLoading())
		{
			String dpart = decompress(part);
			if(dpart.IsVoid())
			{
				LLOG("PixelZStreamData -> decompress error @ row " << i << " (source size = " << part.GetLength() << ")");
				stream.SetError();
				return;
			}
			int x = 0;
			for(; i < e && dpart.GetLength() >= x + row1; x += row1, i++)
				if(pack8)
				{
					Blt1to8(rowbuf, (const byte *)dpart.GetIter(x), wd);
					memcpy(array.GetDownScan(i), rowbuf, wd);
				}
				else
					memcpy(array.GetDownScan(i), dpart.GetIter(x), row);
		}
	}
}

void PixelZStream(Stream& stream, PixelArray& array,
	String (*compress)(String data), String (*decompress)(String data))
{
	Size size = array.GetSize();
	Size dot_size = array.GetDotSize();
	Point hotspot = array.GetHotSpot();
	int raw_bpp = array.GetRawBPP();
	bool mono = array.IsMono();
	PixelZStreamInfo(stream, size, dot_size, hotspot, raw_bpp, mono);
	if(stream.IsError())
		return;
	if(stream.IsLoading())
	{
		if(mono)
			array.CreateMono(size, raw_bpp);
		else
			array.Create(size, raw_bpp);
		array.SetDotSize(dot_size);
		array.SetHotSpot(hotspot);
	}

	PixelZStreamData(stream, array, compress, decompress);
}

void ZImageDirItem::Read(const AlphaArray& array)
{
	size = array.GetSize();
	dot_size = array.GetDotSize();
	hotspot = array.GetHotSpot();
	raw_bpp = array.pixel.GetRawBPP();
	mono = array.pixel.IsMono();
	alpha_bpp = array.alpha.GetRawBPP();
}

void ZImageDirItem::Create(AlphaArray& array) const
{
	if(raw_bpp)
	{
		if(mono)
			array.pixel.CreateMono(size, raw_bpp);
		else
			array.pixel.Create(size, raw_bpp);
		array.pixel.SetDotSize(dot_size);
		array.pixel.SetHotSpot(hotspot);
	}
	if(alpha_bpp)
	{
		array.alpha.CreateMono(size, alpha_bpp);
		array.alpha.SetDotSize(dot_size);
		array.alpha.SetHotSpot(hotspot);
	}
}

static void AlphaZStreamInfo(Stream& stream, ZImageDirItem& item)
{
	PixelZStreamInfo(stream, item.size, item.dot_size, item.hotspot, item.raw_bpp, item.mono);
	Pack16(stream, item.alpha_bpp);
}

static void AlphaZStreamData(Stream& stream, AlphaArray& array,
	String (*compress)(String data), String (*decompress)(String data))
{
	PixelZStreamData(stream, array.pixel, compress, decompress);
	PixelZStreamData(stream, array.alpha, compress, decompress);
}

static void AlphaZStream(Stream& stream, AlphaArray& array,
	String (*compress)(String data), String (*decompress)(String data))
{
	ZImageDirItem item;
	if(stream.IsStoring())
		item.Read(array);
	AlphaZStreamInfo(stream, item);
	if(stream.IsError())
		return;
	if(stream.IsLoading())
		item.Create(array);
	AlphaZStreamData(stream, array, compress, decompress);
}

static int ImageZStreamInfo(Stream& stream, Array<ZImageDirItem>& dir, int magic_tag)
{
	enum { VERSION = 1 };
	stream.Magic(magic_tag);
	Array<ImageInfo> info;
	if(stream.IsError())
		return 0;
	int version = VERSION;
	stream / version;
	if(version > VERSION)
	{
		LLOG("ImageZStreamInfo: version error: " << version);
		stream.SetError();
		return 0;
	}
	int count = dir.GetCount();
	stream % count;
	enum { MAX_COUNT = 1000 };
	if(count < 0 || count > MAX_COUNT)
	{
		LLOG("ImageZStreamInfo: image count error: " << count);
		stream.SetError();
		return 0;
	}
	if(stream.IsLoading())
		dir.SetCount(count);
	for(int i = 0; i < count && !stream.IsError(); i++)
		stream % dir[i];
	return stream.IsError() ? 0 : version;
}

CompressedImageEncoder::CompressedImageEncoder(String (*compress)(String data), String (*decompress)(String data),
	int magic_tag, int save_bpp)
: ImageEncoder(save_bpp), compress(compress), decompress(decompress), magic_tag(magic_tag) {}

CompressedImageEncoder::~CompressedImageEncoder() {}

void CompressedImageEncoder::SaveRaw(Stream& stream, const Vector<const AlphaArray *>& pages)
{
	ASSERT(stream.IsStoring());
	Array<ZImageDirItem> dir;
	dir.Reserve(pages.GetCount());
	int i;
	for(i = 0; i < pages.GetCount(); i++)
		dir.Add().Read(*pages[i]);
	int version = ImageZStreamInfo(stream, dir, magic_tag);
	Buffer<int> offsets(pages.GetCount() + 1);
	int dirpos = (int)stream.GetPos();
	stream.Put((int)0, (int)((pages.GetCount() + 1) * sizeof(int)));
	int imgpos = (int)stream.GetPos();
	offsets[0] = 0;
	for(i = 0; i < pages.GetCount(); i++)
	{
		AlphaZStreamData(stream, const_cast<AlphaArray&>(*pages[i]), compress, decompress);
		offsets[i + 1] = stream.GetPos() - imgpos;
	}
	int endpos = stream.GetPos();
	stream.Seek(dirpos);
	for(i = 0; i <= pages.GetCount(); i++)
		stream % offsets[i];
	stream.Seek(endpos);
}

Array<AlphaArray> CompressedImageEncoder::LoadRaw(Stream& stream, const Vector<int>& page_index)
{
	ASSERT(stream.IsLoading());
	Array<AlphaArray> images;
	Array<ZImageDirItem> dirinfo;
	if(int version = ImageZStreamInfo(stream, dirinfo, magic_tag))
	{
		Buffer<int> offsets(dirinfo.GetCount() + 1);
		int i;
		for(i = 0; i <= dirinfo.GetCount(); i++)
			stream % offsets[i];
		int base = stream.GetPos();
		for(i = 0; i < page_index.GetCount(); i++)
		{
			int p = page_index[i];
			ASSERT(p >= 0 && p < dirinfo.GetCount());
			AlphaArray& array = images.Add();
			dirinfo[p].Create(array);
			stream.Seek(base + offsets[p]);
			AlphaZStreamData(stream, array, compress, decompress);
			if(stream.IsError())
				break;
		}
	}
	return images;
}

Array<ImageInfo> CompressedImageEncoder::InfoRaw(Stream& stream)
{
	ASSERT(stream.IsLoading());
	Array<ImageInfo> info;
	Array<ZImageDirItem> dirinfo;
	if(int version = ImageZStreamInfo(stream, dirinfo, magic_tag))
		for(int i = 0; i < dirinfo.GetCount(); i++)
		{
			ImageInfo& ii = info.Add();
			ii.size = dirinfo[i].size;
			ii.dots = dirinfo[i].dot_size;
			ii.hotspot = dirinfo[i].hotspot;
			ii.bits_per_pixel = dirinfo[i].raw_bpp;
		}
	return info;
}

#include <plugin/z/z.h>

static String ZCStr_(String s) { return ZCompress(s); }
static String ZDStr_(String s) { return ZDecompress(s); }

ZImageEncoder::ZImageEncoder(int save_bpp)
: CompressedImageEncoder(&ZCStr_, &ZDStr_, 'Z' * 0x1000000 + 'I' * 0x10000 + 'M' * 0x100 + 'G', save_bpp)
{
}

ZImageEncoder::~ZImageEncoder() {}

#include <plugin/bz2/bz2.h>

static String BZCStr_(String s) { return BZ2Compress(s); }
static String BZDStr_(String s) { return BZ2Decompress(s); }

BZImageEncoder::BZImageEncoder(int save_bpp)
: CompressedImageEncoder(&BZCStr_, &BZDStr_, 'B' * 0x1000000 + 'Z' * 0x10000 + 'M' * 0x100 + 'G', save_bpp)
{
}

BZImageEncoder::~BZImageEncoder() {}

#if 0
static AlphaArray ZCheckArray(const AlphaArray& array, bool bzip2)
{
	StringStream aout;
	{
		aout % const_cast<AlphaArray&>(array);
	}
	RLOG("RAW size = " << aout.GetSize());
	RLOG("GIF size = " << GifEncoder().SaveArray(array).GetLength());
	RLOG("PNG size = " << PngEncoder().SaveArray(array).GetLength());
	RLOG("JPG size = " << JpgEncoder().SaveArray(array).GetLength());
	String zimage = ZImageEncoder(0, bzip2).SaveArray(array);
	RLOG("ZIM size = " << zimage.GetLength());
	AlphaArray in_array = ZImageEncoder(0, bzip2).LoadArray(zimage);
	if(in_array.GetSize() != array.GetSize())
	{
		RDUMP(array.GetSize());
		RDUMP(in_array.GetSize());
	}
	if(in_array.GetDotSize() != array.GetDotSize())
	{
		RDUMP(array.GetDotSize());
		RDUMP(in_array.GetDotSize());
	}
	if(in_array.GetHotSpot() != array.GetHotSpot())
	{
		RDUMP(array.GetHotSpot());
		RDUMP(in_array.GetHotSpot());
	}
	for(int x = 0; x < array.GetWidth(); x++)
		for(int y = 0; y < array.GetHeight(); y++)
		{
			Color c1 = AlphaGetPixel(array, x, y);
			Color c2 = AlphaGetPixel(array, x, y);
			int distance = tabs(c1.GetR() - c2.GetR()) * 77 + tabs(c1.GetG() - c2.GetG()) * 153
				+ tabs(c1.GetB() - c2.GetB()) * 26;
			if(distance >= 256)
			{
				RLOG("[" << x << ", " << y << "]: c1 = " << c1 << ", c2 = " << c2 << ", distance = " << distance);
			}
		}
	return in_array;
}
#endif
