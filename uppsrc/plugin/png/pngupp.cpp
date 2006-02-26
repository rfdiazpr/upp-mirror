#include <Draw/Draw.h>
#include <plugin/png/png.h>
#include <plugin/png/lib/png.h>
#include <Draw/PixelUtil.h>

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
	RLOG("png error: " << error_msg);
}

static void png_user_warning_fn(png_structp png_ptr, png_const_charp warning_msg)
{
	RLOG("png warning: " << warning_msg);
}

NTL_MOVEABLE(png_color)

PngEncoder::PngEncoder(bool interlace)
: interlace(interlace)
{
}

PngEncoder::~PngEncoder() {}

void PngEncoder::SaveRaw(Stream& stream, const Vector<const AlphaArray *>& pages)
{
	if(pages.GetCount() != 1) {
		stream.SetError();
		return;
	}
	const AlphaArray& image = *pages[0];
	if(image.IsEmpty())
		return;

	png_structp png_ptr;
	png_infop info_ptr;
	Stream *volatile sptr = &stream;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, png_user_error_fn, png_user_warning_fn);

	if (png_ptr == NULL) {
		stream.SetError();
		return;
	}

	/* Allocate/initialize the image information data.  REQUIRED */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		stream.SetError();
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		sptr -> SetError();
		return;
	}

	png_set_write_fn(png_ptr, (void *)&stream, png_write_stream, png_flush_stream);

	Size size = image.GetSize();
	int color_type, bit_depth;
	Vector<png_color> palette;
	byte colormap[256];
	void (*reduce)(byte *, const byte *, unsigned) = 0;

	if(image.pixel.GetBPP() <= 8) {
		for(int i = 0; i < 256; i++)
			colormap[i] = i;
		bit_depth = image.pixel.GetBPP();
		if(image.HasAlpha()) {
			if(image.pixel.palette.GetCount() < 256) {
				colormap[0] = (byte)image.pixel.palette.GetCount();
				if(bit_depth < 8 && image.pixel.palette.GetCount() == 1 << bit_depth)
					bit_depth <<= 1;
			}
			else {
				int nearest = 1, dist = 1000000;
				Color replace = image.pixel.palette[0];
				for(int t = 1; t < image.pixel.palette.GetCount(); t++) {
					Color check = image.pixel.palette[t];
					int rr = replace.GetR() - check.GetR();
					int gg = replace.GetG() - check.GetG();
					int bb = replace.GetB() - check.GetB();
					int d = rr * rr + gg * gg + bb * bb;
					if(d < dist) {
						nearest = t;
						dist = d;
					}
				}
				colormap[0] = nearest;
			}
			if(bit_depth == 2)
				bit_depth = 4;
			switch(bit_depth) {
			case 1: reduce = &Blt8to1; break;
//			case 2: reduce = &Blt8to2; break;
			case 4: reduce = &Blt8to4; break;
			default: NEVER();
			case 8: break;
			}
		}
		color_type = PNG_COLOR_TYPE_PALETTE;
		palette.SetCount(1 << bit_depth);
		memset(palette.Begin(), 0, palette.GetCount() * sizeof(png_color));
		for(int i = 0; i < image.pixel.palette.GetCount(); i++)
			if(colormap[i] < palette.GetCount()) {
				Color u = image.pixel.palette[i];
				png_color p;
				p.red = u.GetR();
				p.green = u.GetG();
				p.blue = u.GetB();
				palette[(int)colormap[i]] = p;
			}
	}
	else {
		color_type = (image.HasAlpha() ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB);
		bit_depth = 8;
	}

	png_set_IHDR(png_ptr, info_ptr, size.cx, size.cy, bit_depth, color_type,
		interlace ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	if(!palette.IsEmpty())
		png_set_PLTE(png_ptr, info_ptr, palette.Begin(), palette.GetCount());

	byte transpal = 0;
	png_color_16 transrgb = { 0, 0, 0 };
	if((color_type & PNG_COLOR_MASK_PALETTE) && image.HasAlpha())
		png_set_tRNS(png_ptr, info_ptr, &transpal, 1, &transrgb);

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
	if(!(color_type & PNG_COLOR_MASK_PALETTE)) {
		PixelReader24 rd(image.pixel);
		if(color_type & PNG_COLOR_MASK_ALPHA) {
			Buffer<byte> rowbuf(4 * size.cx);
			PixelReader8 rda(image.alpha);
			int passes = png_set_interlace_handling(png_ptr);
			for(int pass = 0; pass < passes; pass++)
				for(int y = 0; y < size.cy; y++) {
					const byte *bgr = rd[y];
					const byte *mask = rda[y], *end = mask + size.cx;
					byte *wr = rowbuf;
					while(mask < end) {
						wr[0] = bgr[0];
						wr[1] = bgr[1];
						wr[2] = bgr[2];
						wr[3] = mask[0] - 1;
						wr += 4;
						bgr += 3;
						mask++;
					}
					png_write_row(png_ptr, rowbuf);
				}
		}
		else if(image.pixel.raw_bpp == -3) {
			Buffer<png_bytep> row_pointers(size.cy);
			for(int i = 0; i < size.cy; i++)
				row_pointers[i] = const_cast<byte *>(image.pixel.GetDownScan(i));
			png_write_image(png_ptr, row_pointers);
		}
		else {
			int passes = png_set_interlace_handling(png_ptr);
			for(int pass = 0; pass < passes; pass++)
				for(int y = 0; y < size.cy; y++)
					png_write_row(png_ptr, const_cast<byte *>(rd[y]));
		}
	}
	else {
		if(image.HasAlpha()) {
			Buffer<byte> row(size.cx), cv((size.cx + 1) >> 1);
			PixelReader8 rd(image.pixel), rda(image.alpha);
			int passes = png_set_interlace_handling(png_ptr);
			for(int pass = 0; pass < passes; pass++)
				for(int y = 0; y < size.cy; y++) {
					const byte *s = rd[y], *a = rda[y], *e = s + size.cx;
					byte *p = row;
					for(; s < e; s++)
						*p++ = *a++ ? (byte)0 : colormap[*s];
					if(reduce) {
						reduce(cv, row, size.cx);
						png_write_row(png_ptr, cv);
					}
					else
						png_write_row(png_ptr, row);
				}
		}
		else {
			Buffer<png_bytep> row_pointers(size.cy);
			for(int i = 0; i < size.cy; i++)
				row_pointers[i] = const_cast<byte *>(image.pixel.GetDownScan(i));
			png_write_image(png_ptr, row_pointers);
		}
	}

	/* You can write optional chunks like tEXt, zTXt, and tIME at the end
	* as well.  Shouldn't be necessary in 1.1.0 and up as all the public
	* chunks are supported and you can use png_set_unknown_chunks() to
	* register unknown chunks into the info structure to be written out.
	*/

	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);
}

static Size GetDotSize(Size pixel_size, png_uint_32 x_ppm, png_uint_32 y_ppm, int unit_type)
{
	if(unit_type != 1 || !x_ppm || !y_ppm)
		return Size(0, 0);
	static const double DOTS_PER_METER = 6e6 / 25.4;
	return Size(
		fround(pixel_size.cx * (DOTS_PER_METER / x_ppm)),
		fround(pixel_size.cy * (DOTS_PER_METER / y_ppm)));
}

Array<AlphaArray> PngEncoder::LoadRaw(Stream& stream, const Vector<int>& page_index)
{
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
		NULL, png_user_error_fn, png_user_warning_fn);

	Array<AlphaArray> out;
	if (png_ptr == NULL)
		return out;

	// Allocate/initialize the memory for image information.  REQUIRED.
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		return out;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return out;
	}

	png_set_read_fn(png_ptr, &stream, png_read_stream);
	png_read_info(png_ptr, info_ptr);

	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		&interlace_type, NULL, NULL);
	if(height <= 0 || width <= 0)
		png_error(png_ptr, "Invalid image size.");

	png_uint_32 x_ppm = 0, y_ppm = 0;
	int unit_type = 0;
	png_get_pHYs(png_ptr, info_ptr, &x_ppm, &y_ppm, &unit_type);

	int out_bpp = bit_depth;

	if(bit_depth > 8) {
		png_set_strip_16(png_ptr);
		out_bpp = 8;
	}

/*
	if(color_type & PNG_COLOR_MASK_ALPHA) {
		// todo: convert alpha channel to mask
		png_set_strip_alpha(png_ptr);
	}
*/
/* Expand paletted colors into true RGB triplets */
//if (color_type == PNG_COLOR_TYPE_PALETTE)
//png_set_expand(png_ptr);

/* Expand paletted or RGB images with transparency to full alpha channels
* so the data will be available as RGBA quartets.
*/
//if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
//png_set_expand(png_ptr);

/* Set the background color to draw transparent and alpha images over.
* It is possible to set the red, green, and blue components directly
* for paletted images instead of supplying a palette index.  Note that
* even if the PNG file supplies a background, you are not required to
* use it - you should use the (solid) application background if it has one.
*/

/*
	png_color_16 *image_background;

	if (png_get_bKGD(png_ptr, info_ptr, &image_background))
		png_set_background(png_ptr, image_background, PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);
*/
//	else
//	png_set_background(png_ptr, &my_background,
//	 PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);

/*
int intent;

if (png_get_sRGB(png_ptr, info_ptr, &intent))
png_set_gamma(png_ptr, screen_gamma, 0.45455);
else
{
double image_gamma;
if (png_get_gAMA(png_ptr, info_ptr, &image_gamma))
png_set_gamma(png_ptr, screen_gamma, image_gamma);
else
png_set_gamma(png_ptr, screen_gamma, 0.45455);
}
*/

	void (*expand)(byte *dest, const byte *src, unsigned count) = 0;
	if(color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
		ASSERT(bit_depth >= 8);
		out_bpp = 32;
		png_set_bgr(png_ptr);
		png_set_filler(png_ptr, 0, PNG_FILLER_AFTER);
	}
	else if(out_bpp < 8) {
		switch(bit_depth) {
		case 1: break; // out_bpp = 8; expand = Blt1to8; break;
		case 2: out_bpp = 4; expand = Blt2to4; break; //out_bpp = 8; expand = Blt2to8; break;
		case 4: break; // out_bpp = 8; expand = Blt4to8; break;
		default: NEVER(); return out;
		}
	}

	png_bytep trans_colors = 0;
	png_color_16p trans_values = 0;
	int num_trans = 0;

	png_get_tRNS(png_ptr, info_ptr, &trans_colors, &num_trans, &trans_values);
	bool has_mask = (num_trans > 0);

	AlphaArray im(width, height, out_bpp, 4, NULL, Vector<Color>(), has_mask ? 8 : 0, 4);
	im.SetDotSize(GetDotSize(im.GetSize(), x_ppm, y_ppm, unit_type));
//	ASSERT(im.GetRowBytes() >= png_get_rowbytes(png_ptr, info_ptr));

//	if((color_type & PNG_COLOR_MASK_PALETTE) || !(color_type & PNG_COLOR_MASK_COLOR))
	if(color_type & PNG_COLOR_MASK_PALETTE) {
		png_colorp palette = 0;
		int pal_count = 0;
		png_get_PLTE(png_ptr, info_ptr, &palette, &pal_count);
		pal_count = min(pal_count, 1 << min(bit_depth, 8));
		im.pixel.palette.SetCount(pal_count);
		for(int i = 0; i < pal_count; i++) {
			png_color c = palette[i];
			im.pixel.palette[i] = Color(c.red, c.green, c.blue);
		}
		if(trans_colors)
			for(int i = 0; i < num_trans; i++)
				if(trans_colors[i] < im.pixel.palette.GetCount())
					im.pixel.palette[(int)trans_colors[i]] = Black();

	}
	else if(!(color_type & PNG_COLOR_MASK_COLOR)) { // grayscale
		int colors = 1 << min(bit_depth, 8);
		im.pixel.palette.SetCount(colors);
		for(int i = 0; i < colors; i++) {
			int level = i * 255 / (colors - 1);
			im.pixel.palette[i] = GrayColor(level);
		}
		if(trans_colors)
			for(int i = 0; i < num_trans; i++)
				if(trans_colors[i] < im.pixel.palette.GetCount())
					im.pixel.palette[(int)trans_colors[i]] = Black();
	}

	Buffer<png_bytep> row_pointers(height);
	Vector<byte> row_buffer;
	if(expand) {
		row_buffer.SetCount(im.pixel.GetRowBytes());
		for(unsigned r = 1; r < height; r++)
			row_pointers[r - 1] = im.pixel.GetDownScan(r);
		row_pointers[height - 1] = row_buffer.Begin();
	}
	else {
		for(unsigned r = 0; r < height; r++)
			row_pointers[r] = im.pixel.GetDownScan(r);
	}

	png_read_image(png_ptr, row_pointers);
	if(expand)
		for(unsigned r = 0; r < height; r++)
			expand(im.pixel.GetDownScan(r), row_pointers[r], width);

	if(has_mask) {
		PixelWriter8 wr(im.alpha);
		if(out_bpp <= 8) {
			byte trans_map[256];
			memset(trans_map, 0, 256);
			if(trans_colors)
				for(int i = 0; i < num_trans; i++)
					trans_map[trans_colors[i]] = 1;

			PixelReader8 rd(im.pixel);
			for(unsigned y = 0; y < height; y++) {
				BltXlatB(wr[y], rd[y], width, trans_map);
				wr.Write();
			}
		}
		else {
			bool rtrans[256], gtrans[256], btrans[256];
			memset(rtrans, 0, 256);
			memset(gtrans, 0, 256);
			memset(btrans, 0, 256);
			if(trans_colors)
				for(int i = 0; i < num_trans; i++) {
					const png_color_16& c = trans_values[i];
					rtrans[c.red] = gtrans[c.green] = btrans[c.blue] = 1;
				}
			ASSERT(im.pixel.raw_bpp == 32);
			for(unsigned y = 0; y < height; y++) {
				byte *in = im.pixel.GetDownScan(y) + 3, *end = in + 4 * width;
				byte *out = wr[y];
				for(; in < end; in += 4) {
					byte o = 0;
					byte b = in[0], g = in[1], r = in[2];
					if(btrans[b] && gtrans[g] && rtrans[r])
						if(num_trans <= 2)
							o = 1;
						else {
							for(int i = 0; i < num_trans; i++)
								if(trans_values[i].red == r
								&& trans_values[i].green == g
								&& trans_values[i].blue == b) {
									o = 1;
									break;
								}
						}
					*out++ = o;
					if(o)
						in[0] = in[1] = in[3] = 0;
				}
				wr.Write();
			}
		}
	}

	// read rest of file, and get additional chunks in info_ptr - REQUIRED
	png_read_end(png_ptr, info_ptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

	out.Add() = im;
	return out;
}

Array<ImageInfo> PngEncoder::InfoRaw(Stream& stream)
{
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
		NULL /*user_error_ptr*/, NULL /*user_error_fn*/, NULL /*user_warning_fn*/);

	if (png_ptr == NULL)
		return Array<ImageInfo>();

	// Allocate/initialize the memory for image information.  REQUIRED.
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		return Array<ImageInfo>();
	}

	if (setjmp(png_jmpbuf(png_ptr))) { // Free all of the memory associated with the png_ptr and info_ptr
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return Array<ImageInfo>();
	}

	png_set_read_fn(png_ptr, (void *)&stream, png_read_stream);
	png_read_info(png_ptr, info_ptr);

	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		&interlace_type, NULL, NULL);

	png_uint_32 x_ppm = 0, y_ppm = 0;
	int unit_type = 0;
	png_get_pHYs(png_ptr, info_ptr, &x_ppm, &y_ppm, &unit_type);

	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

	Array<ImageInfo> out;
	out.SetCount(1);
	ImageInfo& ii = out[0];
	ii.size.cx = width;
	ii.size.cy = height;
	if(unit_type == 1)
		ii.dots = GetDotSize(ii.size, x_ppm, y_ppm, unit_type);

	switch(color_type) {
	case PNG_COLOR_TYPE_GRAY:
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		ii.bits_per_pixel = min(bit_depth, 8);
		break;

	case PNG_COLOR_TYPE_PALETTE:
		ii.bits_per_pixel = bit_depth;
		break;

	case PNG_COLOR_TYPE_RGB:
	case PNG_COLOR_TYPE_RGB_ALPHA:
		ii.bits_per_pixel = 24;
		break;

	default: NEVER(); return Array<ImageInfo>();
	}
	if(ii.bits_per_pixel == 2)
		ii.bits_per_pixel = 4;

	return out;
}
