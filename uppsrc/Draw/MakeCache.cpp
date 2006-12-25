#include "Draw.h"

NAMESPACE_UPP

#define LLOG(x) // LOG(x)

static StaticCriticalSection sMakeImage;

static LRUCache<Image>& sImageCache()
{
	static LRUCache<Image> m;
	return m;
}

struct scImageMaker : LRUCache<Image>::Maker {
	const ImageMaker *m;
	bool  paintonly;

	virtual String Key() const {
		return m->Key();
	}
	virtual int    Make(Image& object) const {
		object = m->Make();
		if(paintonly) {
			SetPaintOnly___(object);
			return object.GetLength() + 20000;
		}
		return object.GetLength() + 100;
	}
};

int sMaxSize;

void SweepMkImageCache()
{
	INTERLOCKED_(sMakeImage) {
		LRUCache<Image>& cache = sImageCache();
		cache.ClearCounters();
	}
}

Image MakeImage__(const ImageMaker& m, bool paintonly)
{
	Image result;
	INTERLOCKED_(sMakeImage) {
		LRUCache<Image>& cache = sImageCache();
		scImageMaker cm;
		cm.m = &m;
		cm.paintonly = paintonly;
		result = cache.Get(cm);
		int q = min(cache.GetFoundSize() + cache.GetNewSize(), 4000000);
		if(q > sMaxSize) {
			sMaxSize = q;
			LLOG("ImageCache: Increasing maxsize to " << sMaxSize);
		}
		cache.Shrink(sMaxSize);
	}
	return result;
}

Image MakeImage(const ImageMaker& m)
{
	return MakeImage__(m, false);
}

Image MakeImagePaintOnly(const ImageMaker& m)
{
	return MakeImage__(m, true);
}

class SimpleImageMaker : public ImageMaker {
	Image (*make)(const Image& image);
	Image image;

public:
	virtual String Key() const;
	virtual Image  Make() const;

	SimpleImageMaker(const Image& image, Image (*make)(const Image& image))
	:	image(image), make(make) {}
};

String SimpleImageMaker::Key() const
{
	String key;
	int64 k = image.GetSerialId();
	key.Cat((const char *)&k, sizeof(int64));
	key.Cat((const char *)&make, sizeof(make));
	return key;
}

Image SimpleImageMaker::Make() const
{
	return (*make)(image);
}

Image MakeImage(const Image& image, Image (*make)(const Image& image))
{
	return MakeImage(SimpleImageMaker(image, make));
}

END_UPP_NAMESPACE
