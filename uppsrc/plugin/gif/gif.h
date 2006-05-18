#ifndef _plugin_gif_gif_h_
#define _plugin_gif_gif_h_

#include <Draw/Draw.h>

class GIFRaster : public StreamRaster {
public:
	class Data;
	One<Data> data;

public:
	GIFRaster();
	~GIFRaster();

	virtual bool    Create();
	virtual Size    GetSize();
	virtual Info    GetInfo();
	virtual Line    GetLine(int line);

private:
	bool            Init();
};

class GIFEncoder : public StreamRasterEncoder {
public:
	class Data;
	One<Data> data;

public:
	GIFEncoder(bool ignore_alpha = false, String comment = Null);
	~GIFEncoder();

	GIFEncoder&  IgnoreAlpha(bool ia = true) { ignore_alpha = ia; return *this; }
	GIFEncoder&  Comment(String c)           { comment = c; return *this; }

	virtual int  GetPaletteCount();
	virtual void Start(Size sz);
	virtual void WriteLine(const RGBA *s);

private:
	bool         ignore_alpha;
	String       comment;
};

#endif
