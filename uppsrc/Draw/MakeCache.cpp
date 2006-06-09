#include "Draw.h"

#define LLOG(x) // LOG(x)

static StaticCriticalSection sMakeImage;

static LRUCache<Image>& sImageCache()
{
	static LRUCache<Image> m;
	return m;
}

struct scImageMaker : LRUCache<Image>::Maker {
	const ImageMaker *m;

	virtual String Key() const               { return m->Key(); }
	virtual int    Make(Image& object) const { object = m->Make(); return object.GetLength() + 100; }
};

int sMaxSize;

void SweepMkImageCache()
{
	INTERLOCKED_(sMakeImage) {
		LRUCache<Image>& cache = sImageCache();
		cache.ClearCounters();
	}
}

Image MakeImage(const ImageMaker& m)
{
	Image result;
	INTERLOCKED_(sMakeImage) {
		LRUCache<Image>& cache = sImageCache();
		scImageMaker cm;
		cm.m = &m;
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
