//////////////////////////////////////////////////////////////////////
// jpgupp.cpp: JPEG image file format.

#pragma hdrstop
#include <Draw/Draw.h>
#include <plugin/jpg/jpg.h>
#include <Draw/PixelUtil.h>
//#include <tgraph/picture.h>
//#define __RPCNDR_H__
//#define __wtypes_h__
#define HAVE_BOOLEAN
#define boolean int
#undef FAR
extern "C" {
#include "lib/jinclude.h"
#include "lib/jpeglib.h"
#include "lib/jerror.h"
}
#undef XINT32

static void NoOutput(j_common_ptr cinfo)
{
}

enum
{
	STREAM_BUF_SIZE = 16384,
	ROW_BUF_SIZE    = 16384,
};

struct jpg_stream_destination_mgr
{
	jpeg_destination_mgr  pub;
	Stream               *stream;
	JOCTET               *buffer;
};

typedef jpg_stream_destination_mgr *stream_dest_ptr;

static void init_destination(j_compress_ptr cinfo)
{
	stream_dest_ptr dest = (stream_dest_ptr)cinfo -> dest;
	dest -> buffer = (JOCTET *)
		(*cinfo -> mem -> alloc_small)((j_common_ptr)cinfo, JPOOL_IMAGE,
			STREAM_BUF_SIZE * sizeof(JOCTET));
	dest -> pub.next_output_byte = dest -> buffer;
	dest -> pub.free_in_buffer = STREAM_BUF_SIZE;
}

static boolean empty_output_buffer(j_compress_ptr cinfo)
{
	stream_dest_ptr dest = (stream_dest_ptr)cinfo -> dest;
	dest -> stream -> Put(dest -> buffer, STREAM_BUF_SIZE * sizeof(JOCTET));
	dest -> pub.next_output_byte = dest -> buffer;
	dest -> pub.free_in_buffer = STREAM_BUF_SIZE;
	return true;
}

static void term_destination(j_compress_ptr cinfo)
{
	stream_dest_ptr dest = (stream_dest_ptr)cinfo -> dest;
	size_t done = STREAM_BUF_SIZE - dest -> pub.free_in_buffer;
	if(done > 0)
		dest -> stream -> Put(dest -> buffer, done * sizeof(JOCTET));
}

static void jpeg_stream_dest(j_compress_ptr cinfo, Stream& stream)
{
	if(cinfo -> dest == NULL)
		cinfo -> dest = (jpeg_destination_mgr *)
			(*cinfo -> mem -> alloc_small)((j_common_ptr)cinfo, JPOOL_PERMANENT,
				sizeof(jpg_stream_destination_mgr));

	stream_dest_ptr dest = (stream_dest_ptr)cinfo -> dest;
	dest -> pub.init_destination    = init_destination;
	dest -> pub.empty_output_buffer = empty_output_buffer;
	dest -> pub.term_destination    = term_destination;
	dest -> pub.next_output_byte    = NULL;
	dest -> pub.free_in_buffer      = 0;
	dest -> stream                  = &stream;
	dest -> buffer                  = NULL;
}

struct jpg_stream_source_mgr
{
	jpeg_source_mgr pub;
	Stream         *stream;
	JOCTET         *buffer;
	bool            start_of_file;
};

typedef jpg_stream_source_mgr *stream_src_ptr;

static void init_source(j_decompress_ptr cinfo)
{
	stream_src_ptr src = (stream_src_ptr)cinfo -> src;
	src -> start_of_file = true;
}

static boolean fill_input_buffer(j_decompress_ptr cinfo)
{
	stream_src_ptr src = (stream_src_ptr)cinfo -> src;
	size_t nbytes = src -> stream -> Get(src -> buffer, STREAM_BUF_SIZE * sizeof(JOCTET));
	if(nbytes == 0)
	{
		if(src -> start_of_file)
			ERREXIT(cinfo, JERR_INPUT_EMPTY);
		WARNMS(cinfo, JWRN_JPEG_EOF);
		/* Insert a fake EOI marker */
		src -> buffer[0] = (JOCTET) 0xFF;
		src -> buffer[1] = (JOCTET) JPEG_EOI;
		nbytes = 2;
	}

	src -> pub.next_input_byte = src -> buffer;
	src -> pub.bytes_in_buffer = nbytes;
	src -> start_of_file       = false;

	return true;
}

static void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
	stream_src_ptr src = (stream_src_ptr)cinfo -> src;
	int above = num_bytes - src -> pub.bytes_in_buffer;
	if(above < 0)
	{ // we're still within the input buffer
		src -> pub.next_input_byte += num_bytes;
		src -> pub.bytes_in_buffer = -above;
	}
	else
	{
		src -> stream -> Seek(src -> stream -> GetPos() + above);
		src -> start_of_file = false;
		src -> pub.next_input_byte = NULL;
		src -> pub.bytes_in_buffer = 0;
	}
}

static void term_source(j_decompress_ptr cinfo)
{
	stream_src_ptr src = (stream_src_ptr)cinfo -> src;
	src -> pub.next_input_byte = NULL;
	src -> pub.bytes_in_buffer = 0;
}

static void jpeg_stream_src(j_decompress_ptr cinfo, Stream& stream)
{
	stream_src_ptr src = (stream_src_ptr)cinfo -> src;
	if(src == NULL)
	{
		cinfo -> src = (jpeg_source_mgr *)
			(*cinfo -> mem -> alloc_small)((j_common_ptr)cinfo, JPOOL_PERMANENT,
				sizeof(jpg_stream_source_mgr));
		src = (stream_src_ptr)cinfo -> src;
		src -> buffer = (JOCTET *)(*cinfo -> mem -> alloc_small)((j_common_ptr)cinfo,
			JPOOL_PERMANENT, STREAM_BUF_SIZE * sizeof(JOCTET));
	}

	src -> pub.init_source       = init_source;
	src -> pub.fill_input_buffer = fill_input_buffer;
	src -> pub.skip_input_data   = skip_input_data;
	src -> pub.resync_to_restart = jpeg_resync_to_restart;
	src -> pub.term_source       = term_source;
	src -> pub.bytes_in_buffer   = 0;
	src -> pub.next_input_byte   = NULL;
	src -> stream                = &stream;
}

static void error_exit(j_common_ptr cinfo)
{
	(*cinfo -> err -> output_message)(cinfo);
	throw 0;
}

JpgEncoder::JpgEncoder(int quality)
: ImageEncoder(24)
, quality(quality)
{
}

JpgEncoder::~JpgEncoder() {}

Array<ImageInfo> JpgEncoder::InfoRaw(Stream& stream)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	try
	{
		Array<ImageInfo> out;
		out.SetCount(1);
		ImageInfo& info = out[0];
		if(!stream.IsOpen())
			return Array<ImageInfo>();
		ASSERT(stream.IsLoading());

		cinfo.err = jpeg_std_error(&jerr);
		cinfo.err->output_message = &NoOutput;
		jerr.error_exit = error_exit;

		jpeg_create_decompress(&cinfo);
		jpeg_stream_src(&cinfo, stream);
		jpeg_read_header(&cinfo, TRUE);

		info.size.cx = cinfo.image_width;
		info.size.cy = cinfo.image_height;
		info.bits_per_pixel = 8 * cinfo.num_components;

		double dot_scaling = (cinfo.density_unit == 1 ? 600 : cinfo.density_unit == 2 ? 600.0 / 2.54 : 0);
		if(dot_scaling && cinfo.X_density > 0)
		{
			info.dots.cx = fround(cinfo.image_width  * dot_scaling / cinfo.X_density);
			info.dots.cy = fround(cinfo.image_height * dot_scaling / cinfo.Y_density);
		}

		jpeg_destroy_decompress(&cinfo);
		return out;
	}
	catch(int)
	{
		jpeg_destroy_decompress(&cinfo);
		return Array<ImageInfo>();
	}
}

Array<AlphaArray> JpgEncoder::LoadRaw(Stream& stream, const Vector<int>& page_index)
{
	Array<AlphaArray> out;
	if(!stream.IsOpen())
		return out;
	ASSERT(stream.IsLoading());

//	TIMING("JpegFormat::Load");
	ASSERT(sizeof(JSAMPLE) == sizeof(byte));
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	try
	{
		cinfo.err = jpeg_std_error(&jerr);
		cinfo.err->output_message = &NoOutput;
		jerr.error_exit = error_exit;

		jpeg_create_decompress(&cinfo);
		jpeg_stream_src(&cinfo, stream);
		jpeg_read_header(&cinfo, TRUE);
		cinfo.dct_method = JDCT_IFAST;
		jpeg_start_decompress(&cinfo);

		if(cinfo.output_components != 1 && cinfo.output_components != 3)
			throw 0; // invalid number of components

		Size size(cinfo.output_width, cinfo.output_height);

		int bypp = cinfo.output_components;
		PixelArray image(size, bypp == 3 ? - 3 : 8 * bypp);
		double dot_scaling = (cinfo.density_unit == 1 ? 600 : cinfo.density_unit == 2 ? 600.0 / 2.54 : 0);
		if(dot_scaling && cinfo.X_density > 0)
		{
			Size dots;
			dots.cx = fround(cinfo.image_width  * dot_scaling / cinfo.X_density);
			dots.cy = fround(cinfo.image_height * dot_scaling / cinfo.Y_density);
			image.SetDotSize(dots);
		}

		Buffer<JSAMPROW> rows(size.cy);

		int i;
		for(i = 0; i < size.cy; i++)
			rows[i] = image.GetUpScan(size.cy - 1 - i);

		for(int y = 0, count; y < size.cy && (count = jpeg_read_scanlines(&cinfo, rows + y, size.cy - y)) > 0; y += count)
			;

		if(bypp == 3)
		{ // reorder RGB elements
/*
			FileOut rawdata(GetExeDirFile("jpeg.raw"));
			if(rawdata.IsOpen())
			{
				for(int i = 0; i < size.cy; i++)
					rawdata.Put(rows[i], bypp * size.cx);
				rawdata.Close();
			}

			for(i = 1; i < size.cy; i += 2)
				if(memcmp(rows[i - 1], rows[i], bypp * size.cx))
				{
					LOG("Row pair mismatch: " << i);
				}
*/

			for(i = 0; i < size.cy; i++)
			{
				byte *p = rows[i];
				int n = size.cx;
				byte t;
				while(n >= 4)
				{
					t = p[0]; p[0] = p[2]; p[2] = t;
					t = p[3]; p[3] = p[5]; p[5] = t;
					t = p[6]; p[6] = p[8]; p[8] = t;
					t = p[9]; p[9] = p[11]; p[11] = t;
					p += 12;
					n -= 4;
				}
				if(n & 2)
				{
					t = p[0]; p[0] = p[2]; p[2] = t;
					t = p[3]; p[3] = p[5]; p[5] = t;
					p += 6;
				}
				if(n & 1)
				{
					t = p[0]; p[0] = p[2]; p[2] = t;
				}
			}
		}
		else
		{
			image.palette.SetCount(256);
			for(int i = 0; i < 256; i++)
				image.palette[i] = GrayColor(i);
		}

		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);

		out.Add().pixel = image;
	}
	catch(int)
	{
		jpeg_destroy_decompress(&cinfo);
	}
	return out;
}

void JpgEncoder::SaveRaw(Stream& stream, const Vector<const AlphaArray *>& pages)
{
	ASSERT(stream.IsOpen() && stream.IsStoring());
	if(pages.GetCount() != 1)
	{
		stream.SetError();
		return;
	}
	const AlphaArray& image = *pages[0];
	if(image.IsEmpty())
		return;

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	try
	{
		cinfo.err = jpeg_std_error(&jerr);
		cinfo.err->output_message = &NoOutput;
		jpeg_create_compress(&cinfo);
		jpeg_stream_dest(&cinfo, stream);
		cinfo.dct_method = JDCT_IFAST;

		Size size = image.GetSize();

		cinfo.image_width = size.cx;
		cinfo.image_height = size.cy;
		cinfo.input_components = 3; // # of color components per pixel
		cinfo.in_color_space = JCS_RGB; // colorspace of input image

		jpeg_set_defaults(&cinfo);

		if(dot_size.cx || dot_size.cy)
		{ // set up image density
			cinfo.density_unit = 1; // dots per inch
			cinfo.X_density = dot_size.cx ? fround(size.cx * 600.0 / dot_size.cx) : 0;
			cinfo.Y_density = dot_size.cy ? fround(size.cy * 600.0 / dot_size.cy) : 0;
		}

		jpeg_set_quality(&cinfo, quality, true); // limit to baseline-JPEG values
		jpeg_start_compress(&cinfo, true);

		ASSERT(sizeof(JSAMPLE) == sizeof(byte));

		int part_rows = max(1, 300000 / size.cx);
		int part_bytes = 3 * size.cx;
		Buffer<byte> data(part_bytes * part_rows);
		Buffer<JSAMPROW> rows(part_rows);
		PixelReader24 reader(image.pixel);

		for(int h = 0; h < part_rows; h++)
			rows[h] = data + h * part_bytes;

		for(int i = 0, n; (n = min(size.cy, i + part_rows) - i) > 0; i += n)
		{
			for(int t = 0; t < n; t++)
				BltSwapRGB(rows[t], reader[i + t], image.GetWidth());
			jpeg_write_scanlines(&cinfo, rows, n);
		}

		jpeg_finish_compress(&cinfo);
		jpeg_destroy_compress(&cinfo);
	}
	catch(int)
	{
		jpeg_destroy_compress(&cinfo);
		stream.SetError();
	}
}

//////////////////////////////////////////////////////////////////////
