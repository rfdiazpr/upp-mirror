#ifndef __Plugin_Png__
#define __Plugin_Png__

class PngEncoder : public ImageEncoder
{
public:
	PngEncoder(bool interlace = false);
	virtual ~PngEncoder();

	virtual void              SaveRaw(Stream& stream, const Vector<const AlphaArray *>& pages);
	virtual Array<AlphaArray> LoadRaw(Stream& stream, const Vector<int>& page_index);
	virtual Array<ImageInfo>  InfoRaw(Stream& stream);

	static One<ImageEncoder>  New() { return new PngEncoder; }

	static void               Register() { AddStdMap(&New); }

public:
	bool                      interlace;
};

#endif//__Plugin_Png__
