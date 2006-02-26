#ifndef __Plugin_Zim__
#define __Plugin_Zim__

class CompressedImageEncoder : public ImageEncoder
{
public:
	CompressedImageEncoder(String (*compress)(String data), String (*decompress)(String data), int magic_tag, int save_bpp);
	virtual ~CompressedImageEncoder();

	virtual void              SaveRaw(Stream& stream, const Vector<const AlphaArray *>& pages);
	virtual Array<AlphaArray> LoadRaw(Stream& stream, const Vector<int>& page_index);
	virtual Array<ImageInfo>  InfoRaw(Stream& stream);

private:
	String                  (*compress)(String data);
	String                  (*decompress)(String data);
	int                       magic_tag;
};

class ZImageEncoder : public CompressedImageEncoder
{
public:
	ZImageEncoder(int save_bpp = 0);
	virtual ~ZImageEncoder();

	static One<ImageEncoder>  New() { return new ZImageEncoder; }
};

class BZImageEncoder : public CompressedImageEncoder
{
public:
	BZImageEncoder(int save_bpp = 0);
	virtual ~BZImageEncoder();

	static One<ImageEncoder>  New() { return new BZImageEncoder; }
};

#endif//__Plugin_Zim__
