#ifndef __Plugin_Gif__
#define __Plugin_Gif__

class GifEncoder : public ImageEncoder
{
public:
	GifEncoder(bool optimize_palette = true, String comment = Null);
	virtual ~GifEncoder();

	virtual void              SaveRaw(Stream& stream, const Vector<const AlphaArray *>& pages);
	virtual Array<AlphaArray> LoadRaw(Stream& stream, const Vector<int>& page_index);
	virtual Array<ImageInfo>  InfoRaw(Stream& stream);

	static One<ImageEncoder>  New()      { return new GifEncoder; }

	static void               Register() { AddStdMap(&New); }

private:
	bool                      optimize_palette;
	String                    comment;
};

#endif//__Plugin_Gif__
