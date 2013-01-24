extern "C" {
	#ifdef _7ZIP_ST
		#define __LZ_FIND_MT_H
	#else
		#include "lib/LzFindMt.c_"
	#endif
	#include "lib/LzmaEnc.c_"
}

#define NO_LZMAENC 1
#include "lzma.h"

NAMESPACE_UPP

static voidpf lzma_alloc_new(void* p,  size_t size) {
	voidpf t = new byte[size];
	return t;
}

static void lzma_free_new(void* p, void* address) {
	if(!address)
		return;
	delete[] (byte *)address;
}

static ISzAlloc SzAllocForLzma = { &lzma_alloc_new, &lzma_free_new };

int Lzma::CompressStep(bool last) {
	SRes res = SZ_OK;
	CLzmaEnc* p = (CLzmaEnc*) enc;
#ifndef _7ZIP_ST
	Byte allocaDummy[0x300];
	int i = 0;
	for (i = 0; i < 16; i++)
	allocaDummy[i] = (Byte)i;
#endif
	for (;;) {
		if(!last && in.GetLeft() < 1<<16)
			return SZ_OK;
		// encodes at least 2^15 bytes of data
		res = LzmaEnc_CodeOneBlock(p, false, 0, 0);
		if (WhenProgress)
			WhenProgress(p->nowPos64, RangeEnc_GetProcessed(&p->rc));
		if (res != SZ_OK || p->finished != 0){
			LzmaEnc_Finish(p);
			break;
		}
	}
	if(WhenOut) {
		StringBuffer buf(out.GetLeft());
		int64 len = out.GetLeft();
		out.Get(buf, len);
		WhenOut(~buf, len);
	}
	return res;
}

void Lzma::Compress(){
	mode = DEFLATE;
	enc = LzmaEnc_Create(&SzAllocForLzma);
	ASSERT(enc);
}

void Lzma::CompressFirst(){
	LzmaEncProps_Init(&props);
	props.dictSize = dictSize;
	props.writeEndMark = 0;
	
	SRes res = LzmaEnc_SetProps(enc, &props);
	ASSERT(res == SZ_OK);
	
	unsigned propsSize = LZMA_PROPS_SIZE;
	StringBuffer buf;
	buf.SetLength(propsSize);
	
	res = LzmaEnc_WriteProperties(enc, (unsigned char*)~buf, &propsSize);
	ASSERT(res == SZ_OK && propsSize == LZMA_PROPS_SIZE);
	
	out.Put(~buf, propsSize);
	
	inStream.Set(&in);
	outStream.Set(&out);

	res = LzmaEnc_Prepare(enc, (ISeqOutStream*)&outStream, (ISeqInStream*)&inStream, &SzAllocForLzma, &SzAllocForLzma);
	ASSERT(res == SZ_OK);
	
	inPos = 1;
}

void Lzma::Decompress(){
	mode = INFLATE;
	LzmaDec_Construct(&dec);
	in.Clear();
	out.Clear();
	bufPos = inPos = 0;
}

void Lzma::DecompressFirst(){
	StringBuffer buf(LZMA_PROPS_SIZE);
	in.Get(~buf, LZMA_PROPS_SIZE);
	
	SRes res = LzmaDec_Allocate(&dec, (const unsigned char*)~buf, LZMA_PROPS_SIZE, &SzAllocForLzma);
	ASSERT(res == SZ_OK);
	
	LzmaDec_Init(&dec);
	
	outPos = 0;
	inPos = LZMA_PROPS_SIZE;
}

bool Lzma::DecompressStep(){
	while(true){
		unsigned destLen = LZMA_BUF_SIZE;
		unsigned srcLen = min(LZMA_BUF_SIZE, uint64(bufPos + in.GetLeft()));
		
		if (srcLen < LZMA_REQUIRED_INPUT_MAX)
			break;
		unsigned origLen = srcLen;
		inBuf.SetLength(srcLen);
		in.GetAll(inBuf.Begin() + bufPos, srcLen - bufPos);
		StringBuffer outBuf(destLen);
		SRes res = LzmaDec_DecodeToBuf(
		          &dec,
		          (unsigned char*)~outBuf,
		          &destLen,
		          (const unsigned char*)~inBuf,
		          &srcLen,
		          LZMA_FINISH_ANY,
		          &status);
		ASSERT(res == SZ_OK);
		
		out.Put(~outBuf, destLen);
		
		inPos += srcLen;
		outPos += destLen;
		
		bufPos = origLen-srcLen;
		if (status == LZMA_STATUS_NEEDS_MORE_INPUT)
			break;
		
		memmove(inBuf.Begin(), inBuf.Begin() + srcLen, inBuf.GetCount()-srcLen);
		
		if (WhenProgress)
			WhenProgress(inPos, outPos);
	}
	if(WhenOut) {
		StringBuffer buf(out.GetLeft());
		int64 len = out.GetLeft();
		out.Get(buf, len);
		WhenOut(~buf, len);
	}
	return false;
}

void Lzma::Put(const void *ptr, int size){
	if(mode == INFLATE) {
		in.Put(ptr, size);
		if(inPos==0)
			DecompressFirst();
		DecompressStep();
	}
	else if(mode == DEFLATE) {
		in.Put((const char*)ptr, size);
		if(inPos==0)
			CompressFirst();
		if(in.GetLeft() > 1<<16)
			CompressStep();
	} else
		NEVER();
}

void Lzma::End(){
	if(mode == INFLATE) {
		if(inPos==0)
			DecompressFirst();
		DecompressStep();
		LzmaDec_Free(&dec, &SzAllocForLzma);
	}
	else if (mode == DEFLATE) {
		if(inPos=0)
			CompressFirst();
		CompressStep(true);
		LzmaEnc_Destroy(enc, &SzAllocForLzma, &SzAllocForLzma);
		enc = NULL;
	} else 
		NEVER();
	inPos=0;
}

void Lzma::Clear(){
	End();
	mode = NONE;
	in.Clear();
	out.Clear();
}

Lzma::Lzma() : in(1311072), out(1311072) {
	inPos = 0;
	enc = NULL;
	dec.dic = NULL;
	dictSize = 64*1024;
}

Lzma::~Lzma(){
	if(dec.dic)
		LzmaDec_Free(&dec, &SzAllocForLzma);
	if (enc)
		LzmaEnc_Destroy(enc, &SzAllocForLzma, &SzAllocForLzma);
}

END_UPP_NAMESPACE
