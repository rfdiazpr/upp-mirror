#include <Draw/Draw.h>
#include "jpg.h"
#define HAVE_BOOLEAN
#define boolean int
#undef FAR
extern "C" {
#include "lib/jinclude.h"
#include "lib/jpeglib.h"
#include "lib/jerror.h"
}
#undef XINT32

#define LLOG(x) // LOG(x)

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
	LOG("Error exit!");
	throw Exc();
}

class JPGRaster::Data {
	friend class JPGRaster;

public:
	Data();
	~Data();

	bool Create(Stream& stream);
	void Construct();
	void Free();
	Raster::Info GetInfo();
	Raster::Line GetLine(int line);

private:
	bool Init();

private:
	Stream *stream;
	int64 stream_fpos;

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	Size size;
	Size dot_size;
	int bypp;
	bool finish;

	int next_line;
};

void JPGRaster::Data::Construct()
{
	stream = NULL;
	size = dot_size = Null;
	next_line = 0;
	finish = false;

	jpeg_create_decompress(&cinfo);
}

JPGRaster::Data::Data()
{
	Construct();
}

void JPGRaster::Data::Free()
{
	if(finish)
		jpeg_abort_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
}

JPGRaster::Data::~Data()
{
	try {
		Free();
	}
	catch(Exc) {}
}

bool JPGRaster::Data::Create(Stream& stream_)
{
	stream = &stream_;
	stream_fpos = stream->GetPos();
	cinfo.err = jpeg_std_error(&jerr);
	cinfo.err->output_message = &NoOutput;
	cinfo.dct_method = JDCT_IFAST;
	jerr.error_exit = error_exit;

	return Init();
}

bool JPGRaster::Data::Init()
{
	jpeg_stream_src(&cinfo, *stream);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	if(cinfo.output_components != 1 && cinfo.output_components != 3)
		throw Exc(NFormat("invalid number of components: %d", cinfo.output_components)); // invalid number of components

	size.cx = cinfo.output_width;
	size.cy = cinfo.output_height;

	bypp = cinfo.output_components;

	double dot_scaling = (cinfo.density_unit == 1 ? 600 : cinfo.density_unit == 2 ? 600.0 / 2.54 : 0);
	if(dot_scaling && cinfo.X_density > 0) {
		dot_size.cx = fround(cinfo.image_width  * dot_scaling / cinfo.X_density);
		dot_size.cy = fround(cinfo.image_height * dot_scaling / cinfo.Y_density);
	}

	finish = true;
	return true;
}

Raster::Info JPGRaster::Data::GetInfo()
{
	Raster::Info info;
	try {
		info.bpp = 24;
		info.colors = 0;
		info.dots = dot_size;
		info.hotspot = Null;
		info.kind = IMAGE_OPAQUE;
	}
	catch(Exc e) {
		LLOG(e);
	}
	return info;
}

Raster::Line JPGRaster::Data::GetLine(int line)
{
	DUMP(line);
	try {
		ASSERT(line >= 0 && line < size.cy);
		if(line < next_line) {
			stream->Seek(stream_fpos);
			Stream *s = stream;
			Free();
			Construct();
			Create(*s);
			next_line = 0;
			LOG("Seek back");
		}
		Buffer<byte> rowbuf(bypp * size.cx);
		JSAMPROW rowptr[] = { (JSAMPROW)~rowbuf };
		while(next_line++ < line)
			jpeg_read_scanlines(&cinfo, rowptr, 1);
		jpeg_read_scanlines(&cinfo, rowptr, 1);
		RGBA *out = new RGBA[size.cx];
		const byte *src = ~rowbuf, *end = ~rowbuf + 3 * size.cx;
		RGBA *dest = out;
		if(bypp == 3) {
			for(; src < end; src += 3, dest++) {
				dest->r = src[0];
				dest->g = src[1];
				dest->b = src[2];
				dest->a = 255;
			}
		}
		else if(bypp == 1) {
			for(; src < end; src++, dest++) {
				dest->r = dest->g = dest->b = *src;
				dest->a = 255;
			}
		}
		else {
			NEVER();
		}
		return Raster::Line(out, true);
	}
	catch(Exc e) {
		LLOG(e);
		return Raster::Line(new RGBA[size.cx], true);
	}
}

JPGRaster::JPGRaster()
{
}

JPGRaster::~JPGRaster()
{
}

bool JPGRaster::Create()
{
	try {
		ASSERT(sizeof(JSAMPLE) == sizeof(byte));
		data = new Data;
		return data->Create(GetStream());
	}
	catch(Exc e) {
		LLOG(e);
		SetError();
		data.Clear();
		return false;
	}
}

Size JPGRaster::GetSize()
{
	return data->size;
}

Raster::Info JPGRaster::GetInfo()
{
	Raster::Info info;
	try {
		info.kind = IMAGE_OPAQUE;
		info.bpp = 24;
		info.colors = 0;
		info.dots = data->dot_size;
		info.hotspot = Null;
	}
	catch(Exc e) {
		RLOG(e);
	}
	return info;
}

Raster::Line JPGRaster::GetLine(int line)
{
	ASSERT(data);
	return data->GetLine(line);
}

class JPGEncoder::Data {
public:
	Data();
	~Data();

	void Start(Stream& stream, Size size, int quality);
	void WriteLine(const RGBA *rgba);

private:
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	Size size;
	Buffer<byte> rowbuf;
	int line;
};

JPGEncoder::Data::Data()
{
	cinfo.err = jpeg_std_error(&jerr);
	cinfo.err->output_message = &NoOutput;
	jpeg_create_compress(&cinfo);
	cinfo.dct_method = JDCT_IFAST;
}

JPGEncoder::Data::~Data()
{
	jpeg_destroy_compress(&cinfo);
}

void JPGEncoder::Data::Start(Stream& stream, Size size_, int quality)
{
	size = size_;

	jpeg_stream_dest(&cinfo, stream);

	cinfo.image_width = size.cx;
	cinfo.image_height = size.cy;
	cinfo.input_components = 3; // # of color components per pixel
	cinfo.in_color_space = JCS_RGB; // colorspace of input image

	jpeg_set_defaults(&cinfo);

/*
	if(dot_size.cx || dot_size.cy)
	{ // set up image density
		cinfo.density_unit = 1; // dots per inch
		cinfo.X_density = dot_size.cx ? fround(size.cx * 600.0 / dot_size.cx) : 0;
		cinfo.Y_density = dot_size.cy ? fround(size.cy * 600.0 / dot_size.cy) : 0;
	}
*/

	jpeg_set_quality(&cinfo, quality, true); // limit to baseline-JPEG values
	jpeg_start_compress(&cinfo, true);

	rowbuf.Alloc(3 * size.cx);

	line = 0;

	ASSERT(sizeof(JSAMPLE) == sizeof(byte));
}

void JPGEncoder::Data::WriteLine(const RGBA *rgba)
{
	for(byte *dest = ~rowbuf, *end = dest + 3 * size.cx; dest < end; dest += 3, rgba++) {
		dest[0] = rgba->r;
		dest[1] = rgba->g;
		dest[2] = rgba->b;
	}
	JSAMPROW rowptr[] = { ~rowbuf };
	jpeg_write_scanlines(&cinfo, rowptr, 1);
	if(++line >= size.cy)
		jpeg_finish_compress(&cinfo);
}

JPGEncoder::JPGEncoder(int quality_)
: quality(quality_)
{
}

JPGEncoder::~JPGEncoder()
{
}

int JPGEncoder::GetPaletteCount()
{
	return 0;
}

void JPGEncoder::Start(Size sz)
{
	data = new Data;
	data->Start(GetStream(), sz, quality);
}

void JPGEncoder::WriteLine(const RGBA *s)
{
	data->WriteLine(s);
}
