String        ZGCompress(String s, bool gzip, Gate2<int, int> progress = false);
String        ZGDecompress(String s, bool gzip, Gate2<int, int> progress = false);
void          ZGCompress(Stream& out, Stream& in, bool gzip, Gate2<int, int> progress = false);
void          ZGDecompress(Stream& out, Stream& in, bool gzip, Gate2<int, int> progress = false);

inline String ZCompress(String s, Gate2<int, int> progress = false)                   { return ZGCompress(s, false, progress); }
inline String ZDecompress(String s, Gate2<int, int> progress = false)                 { return ZGDecompress(s, false, progress); }
inline void   ZCompress(Stream& out, Stream& in, Gate2<int, int> progress = false)    { ZGCompress(out, in, false, progress); }
inline void   ZDecompress(Stream& out, Stream& in, Gate2<int, int> progress = false)  { ZGDecompress(out, in, false, progress); }

String ZCompress(const void *data, int len, Gate2<int, int> progress = false);
String ZDecompress(const void *data, int len, Gate2<int, int> progress = false);

inline String GZCompress(String s, Gate2<int, int> progress = false)                  { return ZGCompress(s, true, progress); }
inline String GZDecompress(String s, Gate2<int, int> progress = false)                { return ZGDecompress(s, true, progress); }
inline void   GZCompress(Stream& out, Stream& in, Gate2<int, int> progress = false)   { ZGCompress(out, in, true, progress); }
inline void   GZDecompress(Stream& out, Stream& in, Gate2<int, int> progress = false) { ZGDecompress(out, in, true, progress); }
