#include "FilterStream.h"

CONSOLE_APP_MAIN
{
	String path = GetHomeDirFile("test.gz");
	String data;
	for(int i = 0; i < 100000; i++)
		data.Cat(FormatIntBase(i, 27));
	SaveFile(path, GZCompress(data));

	for(int pass = 0; pass < 2; pass++) {	
		FileIn fin(path);
		InputFilterStream in;
		Zlib zlib;
		zlib.GZip().Decompress();
		zlib.WhenOut = callback(&in, &InputFilterStream::Out);
		in.Filter = callback<Zlib, Zlib, const void *, int>(&zlib, &Zlib::Put);
		in.End = callback(&zlib, &Zlib::End);
		in.in = &fin;
	
		if(pass)
			for(int i = 0; i < data.GetCount(); i++) {
				char c = in.Get();
				ASSERT(c == data[i]);
			}
		else
			for(int i = 0; i < data.GetCount(); i += 17) {
				int n = min(data.GetCount() - i, 17);
				String h = in.Get(n);
				ASSERT(h == data.Mid(i, n));
			}
		ASSERT(in.Get() < 0);
		ASSERT(in.IsEof());	
	}
}
