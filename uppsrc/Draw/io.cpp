#include <Draw/Draw.h>

GLOBAL_VAR(EncoderMap, ImageEncoder::StdMap)

void ImageEncoder::AddStdMap(NewEncoderProc proc)
{
	if(FindIndex(StdMap(), proc) < 0)
		StdMap().Add(proc);
}

void ImageEncoder::SaveArray(Stream& stream, const AlphaArray& array)
{
	Vector<const AlphaArray *> page_list;
	page_list.Add(&array);
	SaveRaw(stream, page_list);
}

String ImageEncoder::SaveArray(const AlphaArray& array)
{
	StringStream ss;
	SaveArray(ss, array);
	return ss;
}

bool ImageEncoder::SaveArrayFile(const char *fn, const AlphaArray& array)
{
	FileOut fout(fn);
	if(!fout.IsOpen())
		return false;
	SaveArray(fout, array);
	fout.Close();
	if(fout.IsError())
	{
		DeleteFile(fn);
		return false;
	}
	return true;
}

void ImageEncoder::SaveImage(Stream& stream, const Image& image)
{
	SaveArray(stream, ImageToAlphaArray(image, ScreenInfo(), -1));
}

String ImageEncoder::SaveImage(const Image& image)
{
	return SaveArray(ImageToAlphaArray(image, ScreenInfo(), -1));
}

bool ImageEncoder::SaveImageFile(const char *fn, const Image& image)
{
	return SaveArrayFile(fn, ImageToAlphaArray(image, ScreenInfo(), -1));
}

AlphaArray ImageEncoder::LoadArray(Stream& stream, int page_index)
{
	Vector<int> index;
	index.Add(page_index);
	Array<AlphaArray> pages = LoadRaw(stream, index);
	if(pages.IsEmpty())
		return AlphaArray();
	return pages[0];
}

AlphaArray ImageEncoder::LoadArray(const String& s, int page_index)
{
	StringStream ss(s);
	return LoadArray(ss, page_index);
}

AlphaArray ImageEncoder::LoadArrayFile(const char *fn, int page_index)
{
	FileIn fi;
	fi.SetBufferSize(1000000);
	if(!fi.Open(fn))
		return AlphaArray();
	return LoadArray(fi, page_index);
}

Image ImageEncoder::LoadImage(Stream& stream, int page_index)
{
	return AlphaArrayToImage(LoadArray(stream, page_index));
}

Image ImageEncoder::LoadImage(const String& s, int page_index)
{
	return AlphaArrayToImage(LoadArray(s, page_index));
}

Image ImageEncoder::LoadImageFile(const char *fn, int page_index)
{
	return AlphaArrayToImage(LoadArrayFile(fn, page_index));
}

Array<ImageInfo> ImageEncoder::InfoArray(Stream& stream)
{
	return InfoRaw(stream);
}

Array<ImageInfo> ImageEncoder::InfoArray(const String& s)
{
	StringStream ss(s);
	return InfoRaw(ss);
}

Array<ImageInfo> ImageEncoder::InfoArrayFile(const char *fn)
{
	FileIn fi(fn);
	if(!fi.IsOpen())
		return Array<ImageInfo>();
	return InfoArray(fi);
}

ImageInfo ImageEncoder::InfoImage(Stream& stream)
{
	Array<ImageInfo> ii = InfoArray(stream);
	if(ii.IsEmpty())
		return ImageInfo();
	return ii[0];
}

ImageInfo ImageEncoder::InfoImage(const String& s)
{
	StringStream ss(s);
	return InfoImage(ss);
}

ImageInfo ImageEncoder::InfoImageFile(const char *fn)
{
	FileIn fi(fn);
	if(!fi.IsOpen())
		return ImageInfo();
	return InfoImage(fi);
}

void ImageEncoder::SerializeAlpha(Stream& stream, AlphaArray& image)
{
	if(stream.IsLoading())
		image = LoadArray(stream);
	else
		SaveArray(stream, image);
}

void ImageEncoder::SerializeImage(Stream& stream, Image& image)
{
	if(stream.IsLoading())
		image = LoadImage(stream);
	else
		SaveImage(stream, image);
}

RleEncoder::RleEncoder() {}
RleEncoder::~RleEncoder() {}

void RleEncoder::SaveRaw(Stream& stream, const Vector<const AlphaArray *>& pages)
{
	ASSERT(pages.GetCount() == 1);
	stream % const_cast<AlphaArray&>(*pages[0]);
}

Array<AlphaArray> RleEncoder::LoadRaw(Stream& stream, const Vector<int>& page_index)
{
	ASSERT(page_index.GetCount() == 1 && page_index[0] == 0);
	Array<AlphaArray> out;
	out.SetCount(1);
	stream % out[0];
	return out;
}

Array<ImageInfo> RleEncoder::InfoRaw(Stream& stream)
{
	Array<ImageInfo> out;
	ImageInfo& info = out.Add();
	bool mono;
	Size size;
	int bpp, align;
	dword cmasks[3];
	Vector<Color> palette;
	int version = PixelArray::SerializeInfo(stream, size, mono, bpp, align, cmasks, palette);
	return out;
}

One<ImageEncoder> MapInfoArray(const EncoderMap& map, Stream& stream, Array<ImageInfo>& info)
{
	if(!stream.IsOpen())
		return One<ImageEncoder>();
	ASSERT(stream.IsLoading());
	int64 pos = stream.GetPos();
	for(int i = 0; i < map.GetCount(); i++)
	{
		One<ImageEncoder> encoder = map[i]();
		if(encoder)
		{
			info = encoder -> InfoRaw(stream);
			stream.ClearError();
			stream.Seek(pos);
			if(!info.IsEmpty())
				return encoder;
		}
	}
	return One<ImageEncoder>();
}

Array<AlphaArray> MapLoadArray(const EncoderMap& map, Stream& stream, const Vector<int>& page_index)
{
	Array<ImageInfo> info;
	One<ImageEncoder> encoder = MapInfoArray(map, stream, info);
	if(!encoder)
		return Array<AlphaArray>();
	Vector<int> pi_tmp;
	if(page_index.IsEmpty())
	{
		pi_tmp.Reserve(info.GetCount());
		while(pi_tmp.GetCount() < info.GetCount())
			pi_tmp.Add(pi_tmp.GetCount());
	}
	return encoder -> LoadRaw(stream, page_index.IsEmpty() ? pi_tmp : page_index);
}

AlphaArray MapLoadArray(const EncoderMap& map, Stream& stream, int page_index)
{
	Vector<int> index;
	index.Add(page_index);
	Array<AlphaArray> out = MapLoadArray(map, stream, index);
	if(out.IsEmpty())
		return AlphaArray();
	return out[0];
}

AlphaArray MapLoadArray(const EncoderMap& map, const String& s, int page_index)
{
	Vector<int> index;
	index.Add(page_index);
	StringStream ss(s);
	Array<AlphaArray> out = MapLoadArray(map, ss, index);
	if(out.IsEmpty())
		return AlphaArray();
	return out[0];
}

AlphaArray MapLoadArrayFile(const EncoderMap& map, const char *fn, int page_index)
{
	Vector<int> index;
	index.Add(page_index);
	FileIn ff(fn);
	if(!ff.IsOpen())
		return AlphaArray();
	Array<AlphaArray> out = MapLoadArray(map, ff, index);
	if(out.IsEmpty())
		return AlphaArray();
	return out[0];
}

One<ImageEncoder> MapInfoImage(const EncoderMap& map, Stream& stream, ImageInfo& info)
{
	Array<ImageInfo> infos;
	One<ImageEncoder> out = MapInfoArray(map, stream, infos);
	if(!out || infos.IsEmpty())
		return NULL;
	info = infos[0];
	return out;
}

Image MapLoadImage(const EncoderMap& map, Stream& stream, int page_index)
{
	return AlphaArrayToImage(MapLoadArray(map, stream, page_index));
}

Image MapLoadImage(const EncoderMap& map, const String& s, int page_index)
{
	return AlphaArrayToImage(MapLoadArray(map, s, page_index));
}

Image MapLoadImageFile(const EncoderMap& map, const char *fn, int page_index)
{
	return AlphaArrayToImage(MapLoadArrayFile(map, fn, page_index));
}

#ifdef PLATFORM_WIN32
ImageInfo ClipboardToImageInfo()
{
	ImageInfo info;
	if(!OpenClipboard(0))
		return info;
	HGLOBAL hmem = (HGLOBAL)GetClipboardData(CF_DIB);
	if(!hmem)
	{
		CloseClipboard();
		return info;
	}
	MemReadStream stream(GlobalLock(hmem), GlobalSize(hmem));
	info = BmpEncoder().InfoImage(stream);
	GlobalUnlock(hmem);
	CloseClipboard();
	return info;
}
#endif
