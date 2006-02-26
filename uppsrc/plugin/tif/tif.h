#ifndef __Plugin_Tif__
#define __Plugin_Tif__

class TifEncoder : public ImageEncoder
{
public:
	TifEncoder();
	virtual ~TifEncoder();

	virtual void              SaveRaw(Stream& stream, const Vector<const AlphaArray *>& pages);
	virtual Array<AlphaArray> LoadRaw(Stream& stream, const Vector<int>& page_index);
	virtual Array<ImageInfo>  InfoRaw(Stream& stream);

	static One<ImageEncoder>  New()      { return new TifEncoder; }

	static void               Register() { AddStdMap(&New); }

public:
	String                    errors;
	String                    warnings;

private:
	static int                GetBPP(int bits_per_sample, int samples_per_pixel);
};

#endif//__Plugin_Tif__
