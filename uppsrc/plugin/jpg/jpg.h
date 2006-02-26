#ifndef __Plugin_Jpg__
#define __Plugin_Jpg__

class JpgEncoder : public ImageEncoder
{
public:
	JpgEncoder(int quality = 50);
	virtual ~JpgEncoder();

	virtual void              SaveRaw(Stream& stream, const Vector<const AlphaArray *>& pages);
	virtual Array<AlphaArray> LoadRaw(Stream& stream, const Vector<int>& page_index);
	virtual Array<ImageInfo>  InfoRaw(Stream& stream);

	static One<ImageEncoder>  New()      { return new JpgEncoder; }

	static void               Register() { AddStdMap(&New); }

private:
	int                       quality;
};

#endif//__Plugin_Jpg__
