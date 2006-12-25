#include "Core.h"

#ifdef PLATFORM_WIN32
#include <plugin/z/lib/zlib.h>
#include <plugin/z/lib/zutil.h>
#else
#include <zlib.h>

#ifndef DEF_MEM_LEVEL
#define DEF_MEM_LEVEL 8
#endif

#ifndef OS_CODE
#define OS_CODE  0x03
#endif

#endif

NAMESPACE_UPP

//////////////////////////////////////////////////////////////////////

static voidpf zalloc_new(voidpf opaque, uInt items, uInt size)
{
	return new byte[items * size];
}

static void zfree_new(voidpf opaque, voidpf address)
{
	delete[] (byte *)address;
}

enum
{
	GZ_MAGIC1    = 0x1f,
	GZ_MAGIC2    = 0x8b,

	ASCII_FLAG   = 0x01, /* bit 0 set: file probably ascii text */
	HEAD_CRC     = 0x02, /* bit 1 set: header CRC present */
	EXTRA_FIELD  = 0x04, /* bit 2 set: extra field present */
	ORIG_NAME    = 0x08, /* bit 3 set: original file name present */
	COMMENT      = 0x10, /* bit 4 set: file comment present */
	RESERVED     = 0xE0, /* bits 5..7: reserved */
};

static bool check_gzip_header(Stream& stream)
{
	int pos = (int)stream.GetPos();
	byte buffer[10];
	int done = stream.Get(buffer, 10);
	if(done != 10 || buffer[0] != GZ_MAGIC1 || buffer[1] != GZ_MAGIC2)
	{
		stream.Seek(pos);
		return done > 0;
	}

	int flags = buffer[3];
	if(buffer[2] != Z_DEFLATED || (flags & RESERVED) != 0)
		return false;

	if(flags & EXTRA_FIELD)
	{ // skip the extra field
		int len = stream.Get16le();
		stream.SeekCur(len);
	}
	if(flags & ORIG_NAME)
	{ // skip the original file name
		while(stream.Get() > 0)
			;
	}
	if(flags & COMMENT)
	{ // skip the .gz file comment
		while(stream.Get() > 0)
			;
	}
	if (flags & HEAD_CRC)
		stream.Get16le();

	return !stream.IsEof();
}

//////////////////////////////////////////////////////////////////////

static String write_gzip_header()
{
	byte buffer[10] = { GZ_MAGIC1, GZ_MAGIC2, Z_DEFLATED, 0, 0, 0, 0, 0, 0, OS_CODE };
	return String(buffer, 10);
}

//////////////////////////////////////////////////////////////////////

String ZGCompress(String s, bool gzip, Gate2<int, int> progress)
{
	if(s.IsEmpty())
		return s;
	z_stream z;
	z.zalloc = zalloc_new;
	z.zfree = zfree_new;
	z.opaque = 0;
	if(deflateInit2(&z, Z_DEFAULT_COMPRESSION, Z_DEFLATED, gzip ? -MAX_WBITS : MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY) != Z_OK)
		return String::GetVoid();
//	if(deflateInit(&z, Z_DEFAULT_COMPRESSION) != Z_OK)
//		return String::GetVoid();
	int buf_size = minmax(s.GetLength() / 2, 1024, 65536);
	z.next_in = (Bytef *)s.Begin();
	z.avail_in = s.GetLength();
	Buffer<Bytef> output(buf_size);
	z.next_out = output;
	z.avail_out = buf_size;

	String out = (gzip ? write_gzip_header() : String(Null));
	while(deflate(&z, z.avail_in ? Z_NO_FLUSH : Z_FINISH) == Z_OK) {
		out.Cat(output, buf_size - z.avail_out);
		z.next_out = output;
		z.avail_out = buf_size;
		if(progress((const char *)z.next_in - ~s, s.GetLength())) {
			deflateEnd(&z);
			return String::GetVoid();
		}
	}
	if(z.avail_out < (unsigned)buf_size)
		out.Cat(output, buf_size - z.avail_out);

	deflateEnd(&z);

	if(gzip) {
		long crc = crc32(0, NULL, 0);
		crc = crc32(crc, (const Bytef *)s.Begin(), s.GetLength());
		char temp[8];
		Poke32le(temp, crc);
		Poke32le(temp + 4, s.GetLength());
		out.Cat(temp, 8);
	}

	return out;
}

//////////////////////////////////////////////////////////////////////

String ZGDecompress(String s, bool gzip, Gate2<int, int> progress)
{
	if(s.IsEmpty())
		return s;
	z_stream z;
	z.zalloc = zalloc_new;
	z.zfree = zfree_new;
	z.opaque = 0;
	if(inflateInit2(&z, gzip ? -MAX_WBITS : +MAX_WBITS) != Z_OK)
		return String::GetVoid();
//	if(inflateInit(&z) != Z_OK)
//		return String::GetVoid();
	int buf_size = minmax(s.GetLength(), 1024, 65536);
	if(gzip)
	{
		StringStream sts(s);
		if(!check_gzip_header(sts))
			return String::GetVoid();
		int off = (int)sts.GetPos();
		z.next_in = (Bytef *)s.GetIter(off);
		z.avail_in = s.GetLength() - off;
	}
	else
	{
		z.next_in = (Bytef *)s.Begin();
		z.avail_in = s.GetLength();
	}
	Buffer<Bytef> output(buf_size);
	z.next_out = output;
	z.avail_out = buf_size;

	String out;
	while(inflate(&z, Z_NO_FLUSH) == Z_OK)
	{
		out.Cat(output, buf_size - z.avail_out);
		z.next_out = output;
		z.avail_out = buf_size;
		if(progress((const char *)z.next_in - ~s, s.GetLength()))
		{
			inflateEnd(&z);
			return String::GetVoid();
		}
	}
	if(z.avail_out < (unsigned)buf_size)
		out.Cat(output, buf_size - z.avail_out);

	inflateEnd(&z);

	return out;
}

//////////////////////////////////////////////////////////////////////

void ZGCompress(Stream& out, Stream& in, bool gzip, Gate2<int, int> progress)
{
	enum { BUF_SIZE = 65536 };
	Buffer<Bytef> input(BUF_SIZE), output(BUF_SIZE);
	int avail = in.Get(input, BUF_SIZE);
	if(avail == 0)
		return;
	z_stream z;
	z.zalloc = zalloc_new;
	z.zfree = zfree_new;
	z.opaque = 0;
	if(deflateInit2(&z, Z_DEFAULT_COMPRESSION, Z_DEFLATED, gzip ? -MAX_WBITS : MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY) != Z_OK)
//	if(deflateInit(&z, Z_DEFAULT_COMPRESSION) != Z_OK)
	{
		out.SetError();
		return;
	}
	if(gzip)
		out.Put(write_gzip_header());
	z.next_in = input;
	z.avail_in = avail;
	z.next_out = output;
	z.avail_out = BUF_SIZE;
	int code;
	int flush = Z_NO_FLUSH;
	int total = (int)in.GetLeft();
	int done = avail;
	long crc = crc32(0, NULL, 0);
	if(gzip)
		crc = crc32(crc, input, avail);
	do {
		if(z.avail_in == 0 && flush == Z_NO_FLUSH) {
			if((z.avail_in = in.Get(z.next_in = input, BUF_SIZE)) == 0)
				flush = Z_FINISH;
			if(gzip)
				crc = crc32(crc, input, z.avail_in);
			done += z.avail_in;
			if(progress(done, total) || in.IsError()) {
				deflateEnd(&z);
				out.SetError();
				return;
			}
		}
		code = deflate(&z, flush);
		if(z.avail_out == 0)
		{
			out.Put(z.next_out = output, z.avail_out = BUF_SIZE);
			if(out.IsError())
			{
				deflateEnd(&z);
				return;
			}
		}
	}
	while(code == Z_OK);
	if(z.avail_out < BUF_SIZE)
		out.Put(output, BUF_SIZE - z.avail_out);
	deflateEnd(&z);
	if(gzip) {
		char temp[8];
		Poke32le(temp, crc);
		Poke32le(temp + 4, done);
		out.Put(temp, 8);
	}
}

//////////////////////////////////////////////////////////////////////

void ZGDecompress(Stream& out, Stream& in, bool gzip, Gate2<int, int> progress)
{
	enum { BUF_SIZE = 65536 };
	Buffer<Bytef> input(BUF_SIZE), output(BUF_SIZE);
	z_stream z;
	z.zalloc = zalloc_new;
	z.zfree = zfree_new;
	z.opaque = 0;
	if(inflateInit2(&z, gzip ? -MAX_WBITS : +MAX_WBITS) != Z_OK
	|| gzip && (!check_gzip_header(in) || in.IsError()))
	{
		out.SetError();
		return;
	}
	z.avail_in = 0;
	z.avail_out = BUF_SIZE;
	z.next_out = output;
	int code;
	int flush = Z_NO_FLUSH;
	int total = (int)in.GetLeft();
	int done = 0;
	do
	{
		if(z.avail_in == 0 && flush == Z_NO_FLUSH)
		{
			z.next_in = input;
			if((z.avail_in = in.Get(z.next_in = input, BUF_SIZE)) == 0)
				flush = Z_FINISH;
			done += z.avail_in;
			if(progress(done, total) || in.IsError())
			{
				inflateEnd(&z);
				out.SetError();
				return;
			}
		}
		code = inflate(&z, flush);
		if(z.avail_out == 0)
		{
			out.Put(z.next_out = output, z.avail_out = BUF_SIZE);
			if(out.IsError())
			{
				inflateEnd(&z);
				return;
			}
		}
	}
	while(code == Z_OK);
	if(z.avail_out < BUF_SIZE)
		out.Put(output, BUF_SIZE - z.avail_out);
	inflateEnd(&z);
}

//////////////////////////////////////////////////////////////////////

String ZCompress(const void *data, int len, Gate2<int, int> progress)
{
	StringStream out;
	MemReadStream in(data, len);
	ZCompress(out, in, progress);
	return out;
}

String ZDecompress(const void *data, int len, Gate2<int, int> progress)
{
	StringStream out;
	MemReadStream in(data, len);
	ZDecompress(out, in, progress);
	return out;
}

END_UPP_NAMESPACE
