//////////////////////////////////////////////////////////////////////
// Copyrights pending:
//
// TIFF library
//
// Copyright (c) 1988-1997 Sam Leffler
// Copyright (c) 1991-1997 Silicon Graphics, Inc.

#include <Draw/Draw.h>
#pragma hdrstop

#include <plugin/tif/tif.h>
#include <Draw/PixelUtil.h>

#define DBGALLOC 0

static void BltPack11(byte *dest, const byte *src, byte bit_shift, unsigned count)
{
	if(bit_shift == 0)
	{ // simple case
#if defined(CPU_IA32)
#define BLT_PACK_11_4(o) *(unsigned *)(dest + (o)) = *(const unsigned *)(src + (o));
#else
#define BLT_PACK_11_4(o) dest[(o) + 0] = src[(o) + 0]; dest[(o) + 1] = src[(o) + 1]; \
	dest[(o) + 2] = src[(o) + 2]; dest[(o) + 3] = src[(o) + 3];
#endif
		for(unsigned rep = count >> 7; rep; rep--)
		{
			BLT_PACK_11_4(0) BLT_PACK_11_4(4) BLT_PACK_11_4(8) BLT_PACK_11_4(12)
			dest += 16;
			src += 16;
		}
		if(count & 0x40)
		{
			BLT_PACK_11_4(0) BLT_PACK_11_4(4)
			dest += 8;
			src += 8;
		}
		if(count & 0x20)
		{
			BLT_PACK_11_4(0)
			dest += 4;
			src += 4;
		}
		if(count & 0x10)
		{
			dest[0] = src[0]; dest[1] = src[1];
			dest += 2;
			src += 2;
		}
		if(count & 8)
			*dest++ = *src++;
		switch(count & 7)
		{
		case 0: break;
		case 1: *dest = (*src & 0x80) | (*dest | 0x7f); break;
		case 2: *dest = (*src & 0xc0) | (*dest | 0x3f); break;
		case 3: *dest = (*src & 0xe0) | (*dest | 0x1f); break;
		case 4: *dest = (*src & 0xf0) | (*dest | 0x0f); break;
		case 5: *dest = (*src & 0xf8) | (*dest | 0x07); break;
		case 6: *dest = (*src & 0xfc) | (*dest | 0x03); break;
		case 7: *dest = (*src & 0xfe) | (*dest | 0x01); break;
		}
	}
	else
	{
		const byte shift1 = bit_shift, shift2 = 8 - bit_shift;
		byte mask;
		if(count + shift1 <= 8)
		{ // touch just 1 byte
			mask = ((1 << count) - 1) << (8 - count - shift1);
			*dest = (*dest & ~mask) | ((*src >> shift1) & mask);
			return;
		}
		mask = 0xff00 >> shift1;
		*dest = (*dest & ~mask) | ((*src >> shift1) & mask);
		dest++;
		count -= shift2;
#define BLT_SHIFT_11_1(o) dest[(o)] = (src[(o)] << shift2) | (src[(o) + 1] >> shift1);
#define BLT_SHIFT_11_4(o) BLT_SHIFT_11_1((o)) BLT_SHIFT_11_1((o) + 1) BLT_SHIFT_11_1((o) + 2) BLT_SHIFT_11_1((o) + 3)
		for(unsigned rep = count >> 6; rep; rep--)
		{
			BLT_SHIFT_11_4(0) BLT_SHIFT_11_4(4)
			dest += 8;
			src += 8;
		}
		if(count & 0x20)
		{
			BLT_SHIFT_11_4(0)
			dest += 4;
			src += 4;
		}
		if(count & 0x10)
		{
			BLT_SHIFT_11_1(0) BLT_SHIFT_11_1(1)
			dest += 2;
			src += 2;
		}
		if(count & 8)
		{
			BLT_SHIFT_11_1(0)
			dest++;
			src++;
		}
		if(count &= 7)
		{
			byte data = (count <= shift1 ? src[1] << shift2 : (src[1] << shift2) | (src[2] >> shift1));
			mask = 0xff00 >> count;
			*dest = (*dest & ~mask) | (data & mask);
		}
	}
}

static void BltPack44(byte *dest, const byte *src, bool shift, unsigned count)
{
//	RTIMING("BltPack44");
	ASSERT(count > 0);
	if(shift)
	{
		byte c = *src++, d;
		*dest = (*dest & 0xF0) | (c >> 4);
		dest++;
		count--;
		while(count >= 8)
		{
			d = src[0]; dest[0] = (c << 4) | (d >> 4);
			c = src[1]; dest[1] = (d << 4) | (c >> 4);
			d = src[2]; dest[2] = (c << 4) | (d >> 4);
			c = src[3]; dest[3] = (d << 4) | (c >> 4);
			src += 4;
			dest += 4;
			count -= 8;
		}
		if(count & 4)
		{
			d = src[0]; dest[0] = (c << 4) | (d >> 4);
			c = src[1]; dest[1] = (d << 4) | (c >> 4);
			src += 2;
			dest += 2;
		}
		if(count & 2)
		{
			d = src[0]; dest[0] = (c << 4) | (d >> 4);
			c = d;
			src++;
			dest++;
		}
		if(count & 1)
			dest[0] = (dest[0] & 15) | (c << 4);
	}
	else
	{
		unsigned c2 = count >> 1;
		if(c2)
			memcpy(dest, src, c2);
		if(count & 1)
			dest[c2] = (dest[c2] & 15) | (src[c2] & 0xF0);
	}
}

static void BltPack4(byte *dest, const byte *src, unsigned count)
{
#define BLT_PACK_4_4(o) dest[(o)] = src[4 * (o)]; dest[(o) + 1] = src[4 * (o) + 4]; \
	dest[(o) + 2] = src[4 * (o) + 8]; dest[(o) + 3] = src[4 * (o) + 12];
	for(unsigned rep = count >> 4; rep; rep--)
	{
		BLT_PACK_4_4(0) BLT_PACK_4_4(4) BLT_PACK_4_4(8) BLT_PACK_4_4(12)
		dest += 16;
		src += 4 * 16;
	}
	if(count & 8)
	{
		BLT_PACK_4_4(0) BLT_PACK_4_4(4)
		dest += 8;
		src += 4 * 8;
	}
	if(count & 4)
	{
		BLT_PACK_4_4(0)
		dest += 4;
		src += 4 * 4;
	}
	if(count & 2)
	{
		dest[0] = src[0]; dest[1] = src[4];
		dest += 2;
		src += 4 * 2;
	}
	if(count & 1)
		dest[0] = src[0];
}

#define int8 tif_int8
#define int32 tif_int32
#define uint32 tif_uint32
#include <plugin/tif/lib/tiffio.h>
#undef int32
#undef uint32

/*
void BltSwapCMYK(byte *dest, const byte *src, unsigned count)
{
#define BLT_SWAP_CMYK(o) \
	a = src[4 * (o) + 3]; b = src[4 * (o) + 2]; \
	dest[3 * (o) + 2] = ~(src[4 * (o) + 0] >= a ? src[4 * (o) + 0] : a); \
	dest[3 * (o) + 1] = ~(src[4 * (o) + 1] >= a ? src[4 * (o) + 1] : a); \
	dest[3 * (o) + 0] = ~(b >= a ? b : a);

#define BLT_SWAP_CMYK_4(o) BLT_SWAP_CMYK((o)) BLT_SWAP_CMYK((o) + 1) BLT_SWAP_CMYK((o) + 2) BLT_SWAP_CMYK((o) + 3)

	byte a, b;
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_SWAP_CMYK_4( 0) BLT_SWAP_CMYK_4( 4) BLT_SWAP_CMYK_4( 8) BLT_SWAP_CMYK_4(12)
		BLT_SWAP_CMYK_4(16) BLT_SWAP_CMYK_4(20) BLT_SWAP_CMYK_4(24) BLT_SWAP_CMYK_4(28)
		dest += 32 * 3;
		src += 32 * 4;
	}
	if(count & 16)
	{
		BLT_SWAP_CMYK_4( 0) BLT_SWAP_CMYK_4( 4) BLT_SWAP_CMYK_4( 8) BLT_SWAP_CMYK_4(12)
		dest += 16 * 3;
		src += 16 * 4;
	}
	if(count & 8)
	{
		BLT_SWAP_CMYK_4(0) BLT_SWAP_CMYK_4(4)
		dest += 8 * 3;
		src += 8 * 4;
	}
	if(count & 4)
	{
		BLT_SWAP_CMYK_4(0)
		dest += 4 * 3;
		src += 4 * 4;
	}
	if(count & 2)
	{
		BLT_SWAP_CMYK(0) BLT_SWAP_CMYK(1)
		dest += 2 * 3;
		src += 2 * 4;
	}
	if(count & 1)
	{
		BLT_SWAP_CMYK(0)
	}
}
*/

/*
void BltXlat3E(byte *p, unsigned count, const byte *table)
{
#define BLT_XLAT3E_2(o) a = p[3 * (o) + 0]; p[3 * (o) + 3] = table[p[3 * (o) + 3]]; p[3 * (o) + 0] = table[a];
#define BLT_XLAT3E_8(o) BLT_XLAT3E_2((o)) BLT_XLAT3E_2((o) + 2) BLT_XLAT3E_2((o) + 4) BLT_XLAT3E_2((o) + 6)

	byte a;
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_XLAT3E_8(0) BLT_XLAT3E_8(8) BLT_XLAT3E_8(16) BLT_XLAT3E_8(24)
		p += 32 * 3;
	}
	if(count & 16)
	{
		BLT_XLAT3E_8(0) BLT_XLAT3E_8(8)
		p += 16 * 3;
	}
	if(count & 8)
	{
		BLT_XLAT3E_8(0)
		p += 8 * 3;
	}
	if(count & 4)
	{
		BLT_XLAT3E_2(0) BLT_XLAT3E_2(2)
		p += 4 * 3;
	}
	if(count & 2)
	{
		BLT_XLAT3E_2(0)
		p += 2 * 3;
	}
	if(count & 1)
		p[0] = table[p[0]];
}
*/

/*
void BltXlat3(byte *dest, const byte *src, unsigned count, const byte *table)
{
#define BLT_XLAT3_2(o) a = src[3 * (o) + 0]; dest[3 * (o) + 3] = table[src[3 * (o) + 3]]; dest[3 * (o) + 0] = table[a];
#define BLT_XLAT3_8(o) BLT_XLAT3_2((o)) BLT_XLAT3_2((o) + 3) BLT_XLAT3_2((o) + 6) BLT_XLAT3_2((o) + 9)

	byte a;
	for(unsigned rep = count >> 5; rep; rep--)
	{
		BLT_XLAT3_8(0) BLT_XLAT3_8(24) BLT_XLAT3_8(48) BLT_XLAT3_8(72)
		dest += 32 * 3;
		src += 32 * 3;
	}
	if(count & 16)
	{
		BLT_XLAT3_8(0) BLT_XLAT3_8(24)
		dest += 16 * 3;
		src += 16 * 3;
	}
	if(count & 8)
	{
		BLT_XLAT3_8(0)
		dest += 8 * 3;
		src += 8 * 3;
	}
	if(count & 4)
	{
		BLT_XLAT3_2(0) BLT_XLAT3_2(6)
		dest += 4 * 3;
		src += 4 * 3;
	}
	if(count & 2)
	{
		BLT_XLAT3_2(0)
		dest += 2 * 3;
		src += 2 * 3;
	}
	if(count & 1)
		dest[0] = table[src[0]];
}
*/

#if DBGALLOC
double total_allocated = 0, total_freed = 0;
unsigned alloc_calls = 0, free_calls = 0, realloc_calls = 0;
Index<tsize_t> size_index;
Vector<int> size_alloc_calls, size_free_calls;

void dbgAddAlloc(tsize_t s)
{
	total_allocated += s;
	int i = size_index.Find(s);
	if(i >= 0)
		size_alloc_calls[i]++;
	else
	{
		size_index.Add(s);
		size_alloc_calls.Add(1);
		size_free_calls.Add(0);
	}
	RLOG("dbgAddAlloc(" << (int)s << "): alloc = " << total_allocated << ", free = " << total_freed << ", diff = " << (total_allocated - total_freed));
}

void dbgAddFree(tsize_t s)
{
	total_freed += s;
	int i = size_index.Find(s);
	if(i >= 0)
		size_free_calls[i]++;
	else
	{
		size_index.Add(s);
		size_alloc_calls.Add(0);
		size_free_calls.Add(1);
	}
	RLOG("dbgAddFree(" << (int)s << "): alloc = " << total_allocated << ", free = " << total_freed << ", diff = " << (total_allocated - total_freed));
}

void TiffAllocStat()
{
	DUMP(total_allocated);
	DUMP(total_freed);
	DUMP(alloc_calls);
	DUMP(free_calls);
	DUMP(realloc_calls);
	for(int i = 0; i < size_index.GetCount(); i++)
		if(size_alloc_calls[i] != size_free_calls[i])
			LOG("Alloc/free mismatch: size = " << size_index[i]
			<< ", alloc = " << size_alloc_calls[i] << ", frees = " << size_free_calls[i]);
}
#endif

extern "C" tdata_t _TIFFmalloc(tsize_t s)
{
	byte *p = new byte[s + 4];
	PokeIL(p, s);
#if DBGALLOC
	alloc_calls++;
	dbgAddAlloc(s);
#endif
	return (tdata_t)(p + 4);
}

extern "C" void    _TIFFfree(tdata_t p)
{
	if(p) {
		byte *rawp = (byte *)p - 4;
#if DBGALLOC
		free_calls++;
		dbgAddFree(PeekIL(rawp));
#endif
		delete[] (rawp);
	}
}

extern "C" tdata_t _TIFFrealloc(tdata_t p, tsize_t s)
{
	int oldsize = (p ? PeekIL((const byte *)p - 4) : 0);
	if(s <= oldsize) {
		PokeIL((byte *)p - 4, s);
		return p;
	}
	byte *newptr = new byte[s + 4];
#if DBGALLOC
	alloc_calls++;
	dbgAddAlloc(s);
#endif
	if(oldsize) {
		memcpy(newptr + 4, p, min<int>(oldsize, s));
#if DBGALLOC
		free_calls++;
		dbgAddFree(oldsize);
#endif
		delete[] ((byte *)p - 4);
	}
	PokeIL(newptr, s);
	return (tdata_t)(newptr + 4);
}

extern "C" void _TIFFmemset(void* p, int v, tsize_t c)           { memset(p, v, c); }
extern "C" void _TIFFmemcpy(void* d, const tdata_t s, tsize_t c) { memcpy(d, s, c); }
extern "C" int  _TIFFmemcmp(const tdata_t p1, const tdata_t p2, tsize_t c) { return memcmp(p1, p2, c); }

class TiffWrapper
{
public:
	TiffWrapper(Stream& stream, bool write);
	~TiffWrapper() { Close(); }

	bool             IsOpen() const      { return tiff; }
	String           GetWarnings() const { return warnings; }
	String           GetErrors() const   { return errors; }

	void             Close();

	Array<ImageInfo> GetInfo();
	PixelArray       GetArray(int page_index);
	void             PutArray(const PixelArray& im);

	static void      Warning(const char* module, const char* fmt, va_list ap);
	static void      Error(const char* module, const char* fmt, va_list ap);

private:
	void             LoadInfo();

	static tsize_t   ReadStream(thandle_t fd, tdata_t buf, tsize_t size);
	static tsize_t   WriteStream(thandle_t fd, tdata_t buf, tsize_t size);
	static toff_t    SeekStream(thandle_t fd, toff_t off, int whence);
	static int       CloseStream(thandle_t fd);
	static toff_t    SizeStream(thandle_t fd);
	static int       MapStream(thandle_t fd, tdata_t *pbase, toff_t *psize);
	static void      UnmapStream(thandle_t fd, tdata_t base, toff_t size);

private:
	Stream&          stream;
	bool             write;
	String           errors, warnings;
	TIFF            *tiff;
	struct Page
	{
		uint32         width, height;
		uint16         bits_per_sample;
		uint16         samples_per_pixel;
		uint16         photometric;
		Size           dot_size;
	};
	Array<Page>      pages;
};

extern "C" {

TIFFErrorHandler _TIFFwarningHandler = TiffWrapper::Warning;
TIFFErrorHandler _TIFFerrorHandler   = TiffWrapper::Error;

};

TiffWrapper::TiffWrapper(Stream& stream, bool write_)
: stream(stream)
, write(write_)
{
//	RTIMING("TiffWrapper::TiffWrapper");
	tiff = TIFFClientOpen("tiff@" + Format64((intptr_t)this), write ? "w" : "r", reinterpret_cast<thandle_t>(this),
		ReadStream, WriteStream, SeekStream, CloseStream, SizeStream, MapStream, UnmapStream);
}

void TiffWrapper::Close()
{
	if(tiff)
	{
		TIFFClose(tiff);
		tiff = NULL;
	}
}

void TiffWrapper::Warning(const char *fn, const char *fmt, va_list ap)
{
	if(!memcmp(fn, "tiff@", 5) && IsDigit(fn[5]))
	{
		int addr = stou(fn + 5);
		if(addr != -1 && addr != 0)
		{
			TiffWrapper& wrapper = *reinterpret_cast<TiffWrapper *>(addr);
			wrapper.warnings << VFormat(fmt, ap) << '\n';
//			RLOG("TiffWrapper::Warning: " << wrapper.errors);
		}
	}
}

void TiffWrapper::Error(const char *fn, const char *fmt, va_list ap)
{
	if(!memcmp(fn, "tiff@", 5) && IsDigit(fn[5])) {
		int addr = stou(fn + 5);
		if(addr != -1 && addr != 0) {
			TiffWrapper& wrapper = *reinterpret_cast<TiffWrapper *>(addr);
			wrapper.errors << VFormat(fmt, ap) << '\n';
//			RLOG("TiffWrapper::Error: " << wrapper.errors);
		}
	}
}

tsize_t TiffWrapper::ReadStream(thandle_t fd, tdata_t buf, tsize_t size)
{
	RTIMING("TiffWrapper::ReadStream");
	TiffWrapper& wrapper = *reinterpret_cast<TiffWrapper *>(fd);
	ASSERT(wrapper.IsOpen());
//	RLOG("TiffStream::ReadStream & " << (int)wrapper.stream.GetPos() << ", count = " << size
//		<< ", end = " << (int)(wrapper.stream.GetPos() + size));
	return wrapper.stream.Get(buf, size);
}

tsize_t TiffWrapper::WriteStream(thandle_t fd, tdata_t buf, tsize_t size)
{
	TiffWrapper& wrapper = *reinterpret_cast<TiffWrapper *>(fd);
	ASSERT(wrapper.IsOpen());
	if(!wrapper.write)
		return 0;
//	RLOG("TiffWrapper::WriteStream & " << (int)wrapper.stream.GetPos() << ", count = " << (int)size
//		<< ", end = " << (int)(wrapper.stream.GetPos() + size));
	wrapper.stream.Put(buf, size);
	return size;
}

toff_t TiffWrapper::SeekStream(thandle_t fd, toff_t off, int whence)
{
	RTIMING("TiffWrapper::SeekStream");
	TiffWrapper& wrapper = *reinterpret_cast<TiffWrapper *>(fd);
	ASSERT(wrapper.IsOpen());
	toff_t size = (toff_t)wrapper.stream.GetSize();
	toff_t destpos = (toff_t)(off + (whence == 1 ? wrapper.stream.GetPos() : whence == 2 ? size : 0));
	if(destpos > size) {
		wrapper.stream.Seek(size);
		if(wrapper.write)
			wrapper.stream.Put((int)0, (int)(destpos - size));
	}
	else
		wrapper.stream.Seek(destpos);
//	RLOG("TiffWrapper::SeekStream -> " << (int)off << ", whence = " << whence << " -> pos = " << (int)wrapper.stream.GetPos());
	return (toff_t)wrapper.stream.GetPos();
}

int TiffWrapper::CloseStream(thandle_t fd)
{
	return 0;
}

toff_t TiffWrapper::SizeStream(thandle_t fd)
{
	TiffWrapper& wrapper = *reinterpret_cast<TiffWrapper *>(fd);
	ASSERT(wrapper.IsOpen());
//	RLOG("TiffWrapper::SizeStream -> " << (int)wrapper.stream.GetSize());
	return (toff_t)wrapper.stream.GetSize();
}

int TiffWrapper::MapStream(thandle_t fd, tdata_t *pbase, toff_t *psize)
{
	return 0;
}

void TiffWrapper::UnmapStream(thandle_t fd, tdata_t base, toff_t size)
{
}

Array<ImageInfo> TiffWrapper::GetInfo()
{
	Array<ImageInfo> info;
	if(IsOpen())
	{
		LoadInfo();
		for(int i = 0; i < pages.GetCount(); i++)
		{
			const Page& page = pages[i];
			ImageInfo& out = info.Add();
			out.size.cx = page.width;
			out.size.cy = page.height;
			out.bits_per_pixel = 24;
			out.dots = page.dot_size;
			if((page.photometric == PHOTOMETRIC_PALETTE
			|| page.photometric == PHOTOMETRIC_MINISWHITE || page.photometric == PHOTOMETRIC_MINISBLACK)
			&& (page.bits_per_sample == 1 || page.bits_per_sample == 2
			|| page.bits_per_sample == 4 || page.bits_per_sample == 8))
				out.bits_per_pixel = (page.bits_per_sample == 1 ? 1 : 8);
			out.src_bits_per_pixel = page.bits_per_sample * min<int>(page.samples_per_pixel, 3);
		}
	}
	return info;
}

void TiffWrapper::LoadInfo()
{
//	RTIMING("TiffWrapper::LoadInfo");
	int count = TIFFNumberOfDirectories(tiff);
	for(int i = 0; i < count; i++)
	{
		Page& page = pages.Add();
		TIFFSetDirectory(tiff, i);
		TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &page.width);
		TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &page.height);
		float xres, yres;
		TIFFGetFieldDefaulted(tiff, TIFFTAG_XRESOLUTION, &xres);
		TIFFGetFieldDefaulted(tiff, TIFFTAG_YRESOLUTION, &yres);
		uint16 resunit;
		TIFFGetFieldDefaulted(tiff, TIFFTAG_RESOLUTIONUNIT, &resunit);
		TIFFGetFieldDefaulted(tiff, TIFFTAG_BITSPERSAMPLE, &page.bits_per_sample);
		TIFFGetFieldDefaulted(tiff, TIFFTAG_SAMPLESPERPIXEL, &page.samples_per_pixel);
		TIFFGetFieldDefaulted(tiff, TIFFTAG_PHOTOMETRIC, &page.photometric);
		double dots_per_unit = (resunit == RESUNIT_INCH ? 600.0 : resunit == RESUNIT_CENTIMETER
			? 600.0 / 2.54 : 0);
		page.dot_size.cx = (xres ? fround(page.width * dots_per_unit / xres) : 0);
		page.dot_size.cy = (yres ? fround(page.height * dots_per_unit / yres) : 0);
	}
	TIFFSetDirectory(tiff, 0);
}

struct TIFFImageHelper : public TIFFRGBAImage
{
	TIFFImageHelper();

	byte *MapDown(int x, int y, int count, bool read);
	byte *MapUp(int x, int y, int count, bool read);
	void  Flush();
	void  Flush(int y);

#if IMAGE_ORIGIN_BOTTOM
	int64 GetDownPos(int y) const { return (int64)row_bytes * (size.cy - 1 - y); }
	int64 GetUpPos(int y) const   { return (int64)row_bytes * y; }
#else
	int64 GetDownPos(int y) const { return (int64)row_bytes * y; }
	int64 GetUpPos(int y) const   { return (int64)row_bytes * (size.cy - 1 - y); }
#endif

	PixelArray dest_image;
	Size size;
	int bpp;
	int row_bytes;
	int cache_size;
	FileStream filebuffer;
	struct Row {
		Row() : x(0), size(0) {}

		Buffer<byte> mapping;
		int x, size;
	};
	enum { MAX_CACHE_SIZE = 50000000 };
	Buffer<Row> rows;
	int64 mapping_offset;
	int mapping_size;
	Vector<uint32> buffer;
	tileContigRoutine contig;
	tileSeparateRoutine separate;
	int skewfac;
	void (*packrow)(byte *dest, const byte *src, unsigned count);
//	void (*pack)(TIFFImageHelper *helper, uint32 x, uint32 y, uint32 w, uint32 h);
};

TIFFImageHelper::TIFFImageHelper()
: contig(0)
, separate(0)
, packrow(0)
, skewfac(0)
, mapping_offset(0)
, mapping_size(0)
, size(0, 0)
, bpp(0)
, row_bytes(0)
, cache_size(0)
{
}

byte *TIFFImageHelper::MapUp(int x, int y, int count, bool read)
{
	return MapDown(x, size.cy - 1 - y, count, read);
}

byte *TIFFImageHelper::MapDown(int x, int y, int count, bool read)
{
	RTIMING("MapDown");
	if(!dest_image.IsEmpty())
		return dest_image.GetDownScan(y) + x;
	else {
		ASSERT(filebuffer.IsOpen());
		Row& row = rows[y];
		if(row.size >= count && row.x <= x && row.x + row.size >= x + count)
			return &row.mapping[x - row.x];
		if(cache_size + count >= MAX_CACHE_SIZE)
			Flush();
		row.mapping.Alloc(count);
		row.x = x;
		row.size = count;
		cache_size += count;
		if(read) {
			filebuffer.Seek(GetDownPos(y) + x);
			filebuffer.GetAll(row.mapping, count);
		}
		return row.mapping;
	}
}

void TIFFImageHelper::Flush()
{
	RLOG("Flush, cache size = " << cache_size);
#if IMAGE_ORIGIN_BOTTOM
	for(int y = size.cy; --y >= 0;)
		Flush(y);
#else
	for(int y = 0; y < size.cy; y++)
		Flush(y);
#endif
	ASSERT(cache_size == 0);
}

void TIFFImageHelper::Flush(int y)
{
	Row& row = rows[y];
	if(filebuffer.IsOpen() && row.size > 0) {
		int64 fpos = GetDownPos(y) + row.x;
//		RLOG("writing row " << y << " from " << fpos << " + " << row.size << " = " << (fpos + row.size));
		filebuffer.Seek(fpos);
		filebuffer.Put(row.mapping, row.size);
		cache_size -= row.size;
		row.size = 0;
		row.mapping.Clear();
	}
}

/*
static void packTile8(TIFFImageHelper *helper, uint32 x, uint32 y, uint32 w, uint32 h)
{
	byte *dest = helper->dest.GetUpScan(y) + x;
	const byte *src = (const byte *)helper->buffer.Begin();
	unsigned srow = sizeof(uint32) * w, drow = helper->dest.GetUpRowBytes();
	for(; h; h--, src += srow, dest += drow)
		BltPack4(dest, src, w);
}
*/

static void packTile24(TIFFImageHelper *helper, uint32 x, uint32 y, uint32 w, uint32 h)
{
	int x3 = 3 * x, w3 = 3 * w;
//	byte *dest = helper->dest.GetUpScan(y) + 3 * x;
	const byte *src = (const byte *)helper->buffer.Begin();
	unsigned srow = sizeof(uint32) * w; //, drow = helper->dest.GetUpRowBytes();
	for(; h; h--, src += srow, /*dest += drow*/ y++)
		BltSwapRGB4(helper->MapUp(x3, y, w3, false), src, w);
}

static void putContig1(TIFFRGBAImage *img, tif_uint32 *cp,
	tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *pp)
{
	RTIMING("PutContig1");
	TIFFImageHelper *helper = (TIFFImageHelper *)img;
	Size size = helper->size;
	int iw = toskew + w;
#if IMAGE_ORIGIN_BOTTOM
	bool keep_y = (iw >= 0);
#else
	bool keep_y = (iw < 0);
#endif
	int x8 = x >> 3;
	int w8 = ((x + w + 7) >> 3) - x8;
	bool read = !!((x | w) & 7) && (int)w < helper->size.cx;
//	byte *dest = helper->dest.GetUpScan(y) + (x >> 3);
//	int drow = (keep_y ? helper->dest.GetUpRowBytes() : -helper->dest.GetUpRowBytes());
	int drow = keep_y ? 1 : -1;
	const byte *src = pp;
	int srow = (fromskew + w - 1) / helper->skewfac + 1;
	for(; h; h--, y += drow /*dest += drow*/, src += srow)
		BltPack11(helper->MapUp(x8, y, w8, read), src, (byte)(x & 7), w);
}

static void putContig4(TIFFRGBAImage *img, tif_uint32 *cp,
	tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *pp)
{
	RTIMING("putContig4");
	TIFFImageHelper *helper = (TIFFImageHelper *)img;
	Size size = helper->size; //dest.GetSize();
	int iw = toskew + w;
#if IMAGE_ORIGIN_BOTTOM
	bool keep_y = (iw >= 0);
#else
	bool keep_y = (iw < 0);
#endif
	int x2 = x >> 1;
	int w2 = ((x + w + 1) >> 1) - x2;
	bool read = !!((x | w) & 1) && (int)w < helper->size.cx;
//	byte *dest = helper->dest.GetUpScan(y) + (x >> 1);
	bool shift = (x & 1);
//	int drow = (keep_y ? helper->dest.GetUpRowBytes() : -helper->dest.GetUpRowBytes());
	int drow = (keep_y ? 1 : -1);
	const byte *src = pp;
	int srow = (fromskew + w - 1) / helper->skewfac + 1;
	for(; h; h--, y /*dest*/ += drow, src += srow)
		BltPack44(helper->MapUp(x2, y, w2, read), src, shift, w);
}

static void putContig8(TIFFRGBAImage *img, tif_uint32 *cp,
	tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *pp)
{
	RTIMING("PutContig8");
	TIFFImageHelper *helper = (TIFFImageHelper *)img;
	Size size = helper->size;
	int iw = toskew + w;
#if IMAGE_ORIGIN_BOTTOM
	bool keep_y = (iw >= 0);
#else
	bool keep_y = (iw < 0);
#endif
//	byte *dest = helper->dest.GetUpScan(y) + x;
//	int drow = (keep_y ? helper->dest.GetUpRowBytes() : -helper->dest.GetUpRowBytes());
	int drow = (keep_y ? 1 : -1);
	const byte *src = pp;
	int srow = (fromskew + w - 1) / helper->skewfac + 1;
	for(; h; h--, y /*dest*/ += drow, src += srow)
		helper->packrow(helper->MapUp(x, y, w, false), src, w);
}

static void putContig24(TIFFRGBAImage *img, tif_uint32 *cp, tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *pp)
{
	RTIMING("PutContig24");
	TIFFImageHelper *helper = (TIFFImageHelper *)img;
	Size size = helper->size;
	int iw = toskew + w;
	int wh = w * h;
	if(wh > helper->buffer.GetCount())
		helper->buffer.SetCount(wh);
#if IMAGE_ORIGIN_BOTTOM
	bool keep_y = (iw >= 0);
#else
	bool keep_y = (iw < 0);
#endif
	helper->contig(img, (tif_uint32 *)(keep_y ? &helper->buffer[0] : &helper->buffer[0] + w * (h - 1)),
		0, 0, w, h, fromskew, keep_y ? 0 : -2 * (int)w, pp);
	packTile24(helper, x, keep_y ? y : y - h + 1, w, h);
}

static void putSeparate(TIFFRGBAImage *img, tif_uint32 *cp,
	tif_uint32 x, tif_uint32 y, tif_uint32 w, tif_uint32 h,
	tif_int32 fromskew, tif_int32 toskew, byte *r, byte *g, byte *b, byte *a)
{
	TIFFImageHelper *helper = (TIFFImageHelper *)img;
	Size size = helper->size;
	int wh = w * h;
	if(wh > helper->buffer.GetCount())
		helper->buffer.SetCount(wh);
	int iw = toskew + w;
#if IMAGE_ORIGIN_BOTTOM
	bool keep_y = (iw >= 0);
#else
	bool keep_y = (iw < 0);
#endif
	helper->separate(img, (tif_uint32 *)(keep_y ? &helper->buffer[0] : &helper->buffer[0] + w * (h - 1)),
		0, 0, w, h, fromskew, keep_y ? 0 : -2 * (int)w, r, g, b, a);
	packTile24(helper, x, keep_y ? y : y - h + 1, w, h);
}

PixelArray TiffWrapper::GetArray(int page_index)
{
	RTIMING("TiffWrapper::GetArray");

	if(!IsOpen())
		return Null;
	LoadInfo();
	TIFFSetDirectory(tiff, page_index);
	char emsg[1024];
	TIFFImageHelper img;
	if(!TIFFRGBAImageBegin(&img, tiff, 0, emsg))
	{
		TIFFError(TIFFFileName(tiff), emsg);
		return Null;
	}

	Vector<Color> palette;
	if(img.isContig)
	{
		img.contig = img.put.contig;
		img.put.contig = putContig24;
	}
	else
	{
		img.separate = img.put.separate;
		img.put.separate = putSeparate;
	}
	int bpp = -3;
	if(img.isContig	&& (img.photometric == PHOTOMETRIC_PALETTE
	|| img.photometric == PHOTOMETRIC_MINISWHITE || img.photometric == PHOTOMETRIC_MINISBLACK)
	&& (img.bitspersample == 1 || img.bitspersample == 2 || img.bitspersample == 4 || img.bitspersample == 8))
	{
		bpp = 8;
		palette.SetCount(1 << img.bitspersample);
		tif_uint32 **ppal = (img.photometric == PHOTOMETRIC_PALETTE ? img.PALmap : img.BWmap);
		ASSERT(ppal);
//		byte rshift = 8 - img.bitspersample;
		byte mask = (1 << img.bitspersample) - 1;
		int part_last = 8 / img.bitspersample - 1;
		int i;
		for(i = 0; i <= mask; i++)
		{
			uint32 rgba = ppal[i][part_last];
			palette[i] = Color(TIFFGetR(rgba), TIFFGetG(rgba), TIFFGetB(rgba));
		}
		img.put.contig = putContig8;
		switch(img.bitspersample)
		{
		case 1: bpp = 1; img.put.contig = putContig1; break;
		case 2: bpp = 4; img.packrow = Blt2to4; img.put.contig = putContig4; break;
		case 4: bpp = 4; img.put.contig = putContig4; break;
		case 8: img.packrow = BltOp2Copy; break;
		default: NEVER();
		}
		img.skewfac = 8 / img.bitspersample;
	}

	img.size = Size(img.width, img.height);
	img.bpp = bpp;
	img.row_bytes = PixelArray::GetRowBytes(bpp, img.width, 4);
	int64 bytes = img.row_bytes * (int64)img.height;
	String tmpfile;
	if(bytes >= 100000000) {
		tmpfile = GetTempFileName();
		if(!img.filebuffer.Open(tmpfile, FileStream::CREATE))
			return Null;
		img.filebuffer.SetSize(bytes);
		if(img.filebuffer.IsError()) {
			img.filebuffer.Close();
			FileDelete(tmpfile);
			return Null;
		}
		img.rows.Alloc(img.size.cy);
	}
	else
		img.dest_image = PixelArray(img.size, img.bpp);

//	RTIMING("TiffWrapper::GetArray/RGBAImageGet");

	bool res = TIFFRGBAImageGet(&img, 0, img.width, img.height);
	TIFFRGBAImageEnd(&img);

	if(img.filebuffer.IsOpen()) {
		img.Flush();
		if(img.filebuffer.IsError() || !res) {
			img.filebuffer.Close();
			FileDelete(tmpfile);
			return Null;
		}
		img.dest_image = PixelArray(img.size, img.bpp);
		img.filebuffer.Seek(0);
		img.filebuffer.GetAll(img.dest_image.Begin(), img.dest_image.GetBytes());
		img.filebuffer.Close();
		FileDelete(tmpfile);
	}
	img.dest_image.SetDotSize(pages[page_index].dot_size);
	img.dest_image.palette = palette;
	return img.dest_image;
}

void TiffWrapper::PutArray(const PixelArray& image)
{
	TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, image.size.cx);
	TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, image.size.cy);
	TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, min<int>(image.bpp, 8));
	TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_PACKBITS);
	TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, image.bpp <= 8 ? PHOTOMETRIC_PALETTE : PHOTOMETRIC_RGB);
	TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, image.bpp <= 8 ? 1 : 3);
	TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, 1);
	TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
//	TIFFSetField(tiff, TIFFTAG_REFERENCEBLACKWHITE, refblackwhite);
//	TIFFSetField(tiff, TIFFTAG_TRANSFERFUNCTION, gray);
//	TIFFSetField(tiff, TIFFTAG_RESOLUTIONUNIT, RESUNIT_NONE);
	if(image.bpp <= 8)
	{ // palette
		uint16 rpal[256], gpal[256], bpal[256];
		int c = 1 << image.bpp;
		memset(rpal, 0, sizeof(uint16) * c);
		memset(gpal, 0, sizeof(uint16) * c);
		memset(bpal, 0, sizeof(uint16) * c);
		if(image.palette.GetCount() < c)
			c = image.palette.GetCount();
		int i;
		for(i = 0; i < c; i++)
		{
			Color co = image.palette[i];
			rpal[i] = co.GetR() * 0x101;
			gpal[i] = co.GetG() * 0x101;
			bpal[i] = co.GetB() * 0x101;
		}
		TIFFSetField(tiff, TIFFTAG_COLORMAP, rpal, gpal, bpal);
		for(i = 0; i < image.size.cy; i++)
			TIFFWriteScanline(tiff, const_cast<byte *>(image.GetDownScan(i)), i, 0);
	}
	else
	{
		PixelReader24 reader(image);
		Buffer<byte> scanbuf(3 * image.size.cx);
		for(int i = 0; i < image.size.cy; i++)
		{
			BltSwapRGB(scanbuf, reader[i], image.size.cx);
			TIFFWriteScanline(tiff, scanbuf, i, 0);
		}
	}
}

TifEncoder::TifEncoder()
{
}

TifEncoder::~TifEncoder() {}

Array<ImageInfo> TifEncoder::InfoRaw(Stream& stream)
{
//	RTIMING("TifEncoder::InfoRaw");

	TiffWrapper wrapper(stream, false);
	Array<ImageInfo> info = wrapper.GetInfo();
	errors = wrapper.GetErrors();
	warnings = wrapper.GetWarnings();
	return info;
}

Array<AlphaArray> TifEncoder::LoadRaw(Stream& stream, const Vector<int>& page_index)
{
//	RTIMING("TifEncoder::LoadRaw");

	TiffWrapper wrapper(stream, false);
	Array<AlphaArray> out;
	out.SetCount(page_index.GetCount());
	for(int i = 0; i < page_index.GetCount(); i++)
		out[i].pixel = wrapper.GetArray(page_index[i]);
	errors = wrapper.GetErrors();
	warnings = wrapper.GetWarnings();
	return out;
}

void TifEncoder::SaveRaw(Stream& stream, const Vector<const AlphaArray *>& ri)
{
	if(ri.GetCount() != 1)
	{
		// todo: save multipage images
		stream.SetError();
		return;
	}
	if(ri[0]->IsEmpty())
		return;
	TiffWrapper wrapper(stream, true);
	wrapper.PutArray(ri[0]->pixel);
	wrapper.Close();
	errors = wrapper.GetErrors();
	warnings = wrapper.GetWarnings();
	if(!errors.IsEmpty())
		stream.SetError();
}
