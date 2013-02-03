#ifndef _plugin_lzma_lzma_h_
#define _plugin_lzma_lzma_h_

#include "Types.h"
#include "LzmaDec.h"
#ifndef NO_LZMAENC
	#include "LzmaEnc.h"
	struct CLzmaEnc;
#endif

#include <Core/Core.h>
#include <PipeStream/PipeStream.h>

NAMESPACE_UPP

#define LZMA_BUF_SIZE uint64(0x4000)

struct LzmaInStream : ISeqInStream {
	PipeStream *buf;
	LzmaInStream() : buf(NULL){
		ISeqInStream::Read = &LzmaInStream::Read;
	}
	void Set(PipeStream* buffer){
		buf = buffer;
	}
	static SRes Read(void *p, void *buffer, size_t *size) {
		PipeStream* buf = ((LzmaInStream*)p)->buf;
		*size = min(int64(*size), buf->GetLeft());
		if (*size) {
			buf->SetLoading();
			buf->GetAll(buffer, *size);
		}
		return SZ_OK;
	}
};

struct LzmaOutStream : ISeqOutStream {
	PipeStream *buf;
	LzmaOutStream() : buf(NULL) {
		ISeqOutStream::Write = &LzmaOutStream::Write;
	}
	void Set(PipeStream* buffer){
		buf = buffer;
	}
	static size_t Write(void *p, const void *buffer, size_t size) {
		PipeStream* buf = ((LzmaOutStream*)p)->buf;
		if (size){
			buf->SetStoring();
			buf->Put(buffer, size);
		}
		return size;
	}
};

class Lzma {
	typedef Lzma CLASSNAME;
	enum { NONE, DEFLATE, INFLATE };
	
	// common
	int mode;
	PipeStream in;
	PipeStream out;
	
	// decompression
	CLzmaDec dec;
	//uint64 unpackSize;
	unsigned outPos;
	unsigned inPos;
	StringBuffer inBuf;
	int bufPos;
	ELzmaStatus status;
	
	void DecompressFirst();
	bool DecompressStep(bool last = false);
	
	// compression
	int dictSize;
	CLzmaEncHandle enc;
	CLzmaEncProps props;
	LzmaInStream inStream;
	LzmaOutStream outStream;
	
	int CompressStep(bool last = false);
	void CompressFirst();
	
public:
	Callback2<const void *, int> WhenOut;
	Callback2<int, int> WhenProgress;
	
	void Compress();
	void Decompress();
	void Put(const void *ptr, int size);
	void Put(const String& s)              { Put(~s, s.GetCount()); }
	void End();
	void Clear();
	
	void SetDictionarySize(int n) { dictSize=n; }
	
	String Get() {
		int l = out.GetLeft();
		StringBuffer b(l);
		out.GetAll(~b, l);
		return String(b);
	}
	operator String()         { return Get(); }
	String operator~()        { return Get(); }
	void  ClearOut()          { Get(); }
	
	Lzma();
	~Lzma();
};

END_UPP_NAMESPACE

#endif
