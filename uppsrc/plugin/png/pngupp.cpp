#include <Draw/Draw.h>
#include "png.h"
#include <plugin/png/lib/png.h>

#define LLOG(x)  LOG(x)

static void png_read_stream(png_structp png_ptr, png_bytep buffer, png_size_t length)
{
	Stream& stream = *reinterpret_cast<Stream *>(png_get_io_ptr(png_ptr));
	if(!stream.GetAll(buffer, length))
		png_error(png_ptr, "Error reading input file!");
}

static void png_write_stream(png_structp png_ptr, png_bytep buffer, png_size_t length)
{
	Stream& stream = *reinterpret_cast<Stream *>(png_get_io_ptr(png_ptr));
	stream.Put(buffer, length);
}

static void png_flush_stream(png_structp png_ptr)
{
	Stream& stream = *reinterpret_cast<Stream *>(png_get_io_ptr(png_ptr));
	stream.Flush();
}

static void png_user_error_fn(png_structp png_ptr, png_const_charp error_msg)
{
	LLOG("PNG error: " << error_msg);
}

static void png_user_warning_fn(png_structp png_ptr, png_const_charp warning_msg)
{
	LLOG("png warning: " << warning_msg);
}

NTL_MOVEABLE(png_color)

static Size GetDotSize(Size pixel_size, png_uint_32 x_ppm, png_uint_32 y_ppm, int unit_type)
{
	if(unit_type != 1 || !x_ppm || !y_ppm)
		return Size(0, 0);
	static const double DOTS_PER_METER = 6e6 / 25.4;
	return Size(
		fround(pixel_size.cx * (DOTS_PER_METER / x_ppm)),
		fround(pixel_size.cy * (DOTS_PER_METER / y_ppm)));
}

class PNGRaster::Data {
public:
	Data();
	~Data();

public:
	png_structp  png_ptr;
	png_infop    info_ptr;

	Info         info;
	Size         size;
	RasterFormat fmt;
	int          out_bpp;
	int          row_bytes;
	Vector<RGBA> palette;
	Vector<byte> scanline;
	Vector<byte> preimage;
	Buffer<png_bytep> row_pointers;
	bool         preload;
	bool         loaded;
	bool         strip16;
	int          next_row;
	int64        soff;
};

PNGRaster::Data::Data()
{
	png_ptr = NULL;
	info_ptr = NULL;
}

PNGRaster::Data::~Data()
{
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
}

PNGRaster::PNGRaster()
{
}

PNGRaster::~PNGRaster()
{
}

bool PNGRaster::Init()
{
	if(!(data->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
	NULL, png_user_error_fn, png_user_warning_fn)))
		return false;
	if (setjmp(png_jmpbuf(data->png_ptr)))
		return false;
	if(!(data->info_ptr = png_create_info_struct(data->png_ptr)))
		return false;
	png_set_read_fn(data->png_ptr, &GetStream(), png_read_stream);
	png_read_info(data->png_ptr, data->info_ptr);
	return true;
}

bool PNGRaster::Create()
{
	data = new Data;

	data->soff = GetStream().GetPos();
	if(!Init()) {
		data.Clear();
		return false;
	}
	if (setjmp(png_jmpbuf(data->png_ptr))) {
		data.Clear();
		return false;
	}
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	png_get_IHDR(data->png_ptr, data->info_ptr, &width, &height, &bit_depth, &color_type,
		&interlace_type, NULL, NULL);

	if(height <= 0 || width <= 0)
		return false;
	data->size.cx = width;
	data->size.cy = height;

	png_uint_32 x_ppm = 0, y_ppm = 0;
	int unit_type = 0;
	png_get_pHYs(data->png_ptr, data->info_ptr, &x_ppm, &y_ppm, &unit_type);
	data->info.bpp = bit_depth;
	data->info.colors = (bit_depth < 8 ? 1 << bit_depth : 0);
	data->info.dots = GetDotSize(data->size, x_ppm, y_ppm, unit_type);
	data->info.hotspot = Point(0, 0);
	data->info.kind = IMAGE_OPAQUE;

	data->out_bpp = bit_depth;

	data->strip16 = (bit_depth > 8);
	if(data->strip16) {
		png_set_strip_16(data->png_ptr);
		data->out_bpp = 8;
	}

	if(color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
		ASSERT(bit_depth >= 8);
		data->out_bpp = 24;
		png_set_bgr(data->png_ptr);
		if(color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
			data->out_bpp = 32;
			data->info.kind = IMAGE_ALPHA;
		}
	}

	png_bytep trans_colors = 0;
	png_color_16p trans_values = 0;
	int num_trans = 0;

	png_get_tRNS(data->png_ptr, data->info_ptr, &trans_colors, &num_trans, &trans_values);
	bool has_mask = (num_trans > 0);

//	AlphaArray im(width, height, out_bpp, 4, NULL, Vector<Color>(), has_mask ? 8 : 0, 4);
//	im.SetDotSize(GetDotSize(im.GetSize(), x_ppm, y_ppm, unit_type));
//	ASSERT(im.GetRowBytes() >= png_get_rowbytes(png_ptr, info_ptr));

//	if((color_type & PNG_COLOR_MASK_PALETTE) || !(color_type & PNG_COLOR_MASK_COLOR))
	if(color_type & PNG_COLOR_MASK_PALETTE) {
		png_colorp ppal = 0;
		int pal_count = 0;
		png_get_PLTE(data->png_ptr, data->info_ptr, &ppal, &pal_count);
		pal_count = min(pal_count, 1 << min(bit_depth, 8));
		data->palette.SetCount(pal_count);
		for(int i = 0; i < pal_count; i++) {
			png_color c = ppal[i];
			RGBA rgba;
			rgba.r = c.red;
			rgba.g = c.green;
			rgba.b = c.blue;
			rgba.a = 255;
			data->palette[i] = rgba;
		}
		if(trans_colors) {
			data->info.kind = IMAGE_MASK;
			for(int i = 0; i < num_trans; i++)
				if(trans_colors[i] < data->palette.GetCount())
					data->palette[(int)trans_colors[i]] = RGBAZero();
		}

	}
	else if(!(color_type & PNG_COLOR_MASK_COLOR)) { // grayscale
		int colors = 1 << min(bit_depth, 8);
		data->palette.SetCount(colors);
		for(int i = 0; i < colors; i++) {
			int level = i * 255 / (colors - 1);
			RGBA rgba;
			rgba.r = rgba.g = rgba.b = (byte)level;
			rgba.a = 255;
			data->palette[i] = rgba;
		}
		if(trans_colors) {
			data->info.kind = IMAGE_MASK;
			for(int i = 0; i < num_trans; i++)
				if(trans_colors[i] < data->palette.GetCount())
					data->palette[(int)trans_colors[i]] = RGBAZero();
		}
	}

/*
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return out;
	}
*/

	data->row_bytes = png_get_rowbytes(data->png_ptr, data->info_ptr);
	data->next_row = 0;
	data->preload = (interlace_type != PNG_INTERLACE_NONE);
	data->loaded = false;
	switch(data->out_bpp) {
		case 1:  data->fmt.Set1mf(); break;
		case 2:  data->fmt.Set2mf(); break;
		case 4:  data->fmt.Set4mf(); break;
		case 8:  if(color_type & PNG_COLOR_MASK_ALPHA) data->fmt.Set8A(); else data->fmt.Set8(); break;
		case 24: data->fmt.Set24le(0xFF0000, 0xFF00, 0xFF); break;
		case 32: data->fmt.SetRGBA(); break;
		default: NEVER(); return false; // invalid bpp
	}
	if(!data->preload)
		data->scanline.SetCount(data->row_bytes);

	return true;
}

Size PNGRaster::GetSize()
{
	return data ? data->size : Size(0, 0);
}

Raster::Info PNGRaster::GetInfo()
{
	return data ? data->info : Raster::Info();
}

Raster::Line PNGRaster::GetLine(int line)
{
	ASSERT(data && line >= 0 && line < data->size.cy);
	if(setjmp(png_jmpbuf(data->png_ptr)))
		return Raster::Line(new RGBA[data->size.cx], true);
	if(data->preload) {
		if(!data->loaded) {
			data->loaded = true;
			data->preimage.SetCount(data->row_bytes * data->size.cy);
			data->row_pointers.Alloc(data->size.cy);
			for(int i = 0; i < data->size.cy; i++)
				data->row_pointers[i] = &data->preimage[i * data->row_bytes];
			png_read_image(data->png_ptr, data->row_pointers);
		}
		const byte *rowdata = &data->preimage[data->row_bytes * line];
		if(data->out_bpp == 32)
			return Raster::Line(reinterpret_cast<const RGBA *>(rowdata), false);
		RGBA *linebuf = new RGBA[data->size.cx];
		data->fmt.Read(linebuf, rowdata, data->size.cx, data->palette);
		return Raster::Line(linebuf, true);
	}
	else {
		if(line < data->next_row) {
			png_destroy_read_struct(&data->png_ptr, &data->info_ptr, NULL);
			GetStream().Seek(data->soff);
			if(!Init()) {
				NEVER();
			}
			if(data->strip16)
				png_set_strip_16(data->png_ptr);
			if(data->out_bpp > 8)
				png_set_bgr(data->png_ptr);
			data->next_row = 0;
		}
		while(data->next_row < line) {
			png_read_row(data->png_ptr, data->scanline, NULL);
			data->next_row++;
		}
		png_read_row(data->png_ptr, data->scanline, NULL);
		data->next_row++;
		RGBA *linebuf = new RGBA[data->size.cx];
		data->fmt.Read(linebuf, data->scanline, data->size.cx, data->palette);
		return Raster::Line(linebuf, true);
	}
}

class PNGEncoder::Data {
public:
	Data();
	~Data();

	void Start(Stream& stream, Size size, int bpp, ImageKind kind, bool interlace,
		const RGBA *palette, const PaletteCv *pal_cv);
	void WriteLine(const RGBA *rgba);

private:
	RasterFormat format;

	const PaletteCv *pal_cv;
	ImageKind kind;

	png_structp png_ptr;
	png_infop info_ptr;
	bool interlace;
	bool do_palette;
	bool do_mask;
	bool do_alpha;

	Vector<byte> imagebuf;
	Vector<byte> rowbuf;
	Size size;
	int rowbytes;
	int passes;

	int line;
};

PNGEncoder::Data::Data()
{
	png_ptr = NULL;
	info_ptr = NULL;
	pal_cv = NULL;
}

PNGEncoder::Data::~Data()
{
	png_destroy_write_struct(&png_ptr, &info_ptr);
}

void PNGEncoder::Data::Start(Stream& stream, Size size_, int bpp, ImageKind kind_, bool interlace_,
	const RGBA *imgpal, const PaletteCv *pal_cv_)
{
	size = size_;
	kind = kind_;
	pal_cv = pal_cv_;
	interlace = interlace_;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, png_user_error_fn, png_user_warning_fn);
	if(!png_ptr) {
		stream.SetError();
		return;
	}

	/* Allocate/initialize the image information data.  REQUIRED */
	info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr) {
		stream.SetError();
		return;
	}

/*
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		sptr -> SetError();
		return;
	}
*/
	png_set_write_fn(png_ptr, (void *)&stream, png_write_stream, png_flush_stream);

	int color_type, bit_depth;
	Vector<png_color> palette;

	do_alpha = (kind != IMAGE_OPAQUE && bpp != 1);
	do_mask = (do_alpha && kind == IMAGE_MASK);
	do_palette = (bpp <= 8);

	if(do_palette) {
		switch(bpp) {
			case 1: format.Set1mf(); break;
			case 2: format.Set2mf(); break;
			case 4: format.Set4mf(); break;
			default: NEVER();
			case 8: format.Set8(); break;
		}
		bit_depth = bpp;
		color_type = PNG_COLOR_TYPE_PALETTE;
		palette.SetCount(1 << bpp);
		for(int i = 0; i < palette.GetCount(); i++) {
			png_color& c = palette[i];
			c.red = imgpal[i].r;
			c.green = imgpal[i].g;
			c.blue = imgpal[i].b;
		}
		rowbytes = (size.cx * bpp + 31) >> 5 << 2;
	}
	else {
		color_type = (do_alpha ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB);
		bit_depth = 8;
		if(do_alpha) {
			format.Set32le(0xFF << 16, 0xFF << 8, 0xFF, 0xFF << 24);
			rowbytes = 4 * size.cx;
		}
		else {
			format.Set24le(0xFF << 16, 0xFF << 8, 0xFF);
			rowbytes = (3 * size.cx + 3) & -4;
		}
	}

	png_set_IHDR(png_ptr, info_ptr, size.cx, size.cy, bit_depth, color_type,
		interlace ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	if(!palette.IsEmpty())
		png_set_PLTE(png_ptr, info_ptr, palette.Begin(), palette.GetCount());

	byte transpal = (1 << bpp) - 1;
	if(do_mask && do_palette) {
		png_color_16 transrgb = { 0, 0, 0 };
		png_set_tRNS(png_ptr, info_ptr, &transpal, 1, &transrgb);
	}

//	png_set_gAMA(png_ptr, info_ptr, gamma);

	/* Optionally write comments into the image */
//	text_ptr[0].key = "Title";
//	text_ptr[0].text = "Mona Lisa";
//	text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
	#ifdef PNG_iTXt_SUPPORTED
	text_ptr[0].lang = NULL;
	#endif
//	png_set_text(png_ptr, info_ptr, text_ptr, 1);

	/* other optional chunks like cHRM, bKGD, tRNS, tIME, oFFs, pHYs, */
	/* note that if sRGB is present the gAMA and cHRM chunks must be ignored
	* on read and must be written in accordance with the sRGB profile */

	/* Write the file header information.  REQUIRED */
	png_write_info(png_ptr, info_ptr);

	png_set_bgr(png_ptr);

	passes = png_set_interlace_handling(png_ptr);
	rowbuf.SetCount(rowbytes);
	if(passes > 1)
		imagebuf.SetCount(rowbytes * size.cy);

	line = 0;
}

void PNGEncoder::Data::WriteLine(const RGBA *rgba)
{
	byte *dest = (passes > 1 ? &imagebuf[line * rowbytes] : rowbuf.Begin());
	format.Write(dest, rgba, size.cx, pal_cv);
	png_write_row(png_ptr, dest);
	if(++line >= size.cy) {
		for(int p = 1; p < passes; p++)
			for(int y = 0; y < size.cy; y++)
				png_write_row(png_ptr, &imagebuf[y * rowbytes]);

		/* You can write optional chunks like tEXt, zTXt, and tIME at the end
		* as well.  Shouldn't be necessary in 1.1.0 and up as all the public
		* chunks are supported and you can use png_set_unknown_chunks() to
		* register unknown chunks into the info structure to be written out.
		*/

		png_write_end(png_ptr, info_ptr);
	}
}

PNGEncoder::PNGEncoder(int bpp_, ImageKind kind_, bool interlace_)
: bpp(bpp_), kind(kind_), interlace(interlace_)
{
}

PNGEncoder::~PNGEncoder()
{
}

int PNGEncoder::GetPaletteCount()
{
	return (bpp > 8 ? 0 : (1 << bpp) - (kind == IMAGE_OPAQUE || bpp == 1 ? 0 : 1));
}

void PNGEncoder::Start(Size sz)
{
	data = new Data;
	data->Start(GetStream(), sz, bpp, kind, interlace,
	            bpp > 8 ? NULL : GetPalette(), GetPaletteCv());
}

void PNGEncoder::WriteLine(const RGBA *s)
{
	data->WriteLine(s);
}
