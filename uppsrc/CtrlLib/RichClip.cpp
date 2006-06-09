#include "CtrlLib.h"

/*
struct sQtfCt_ : public RichText::ClipboardType {
	virtual int      Level() { return 0; }
	virtual RichText ReadClipboard(const RichPara::Format&)
	{
		if(IsClipboardAvailable(GetClipboardFormatCode("QTF"))) {
			return ParseQTF(::ReadClipboard(GetClipboardFormatCode("QTF")));
		}
		return RichText();
	}
	virtual void WriteClipboard(const RichText& clip) {
		::WriteClipboard(GetClipboardFormatCode("QTF"), AsQTF(clip), false);
	}
};

struct sPlainCt_ : public RichText::ClipboardType {
	virtual int      Level() { return 10; }
	virtual RichText ReadClipboard(const RichPara::Format& f)
	{
		if(IsClipboardAvailableText()) {
			return AsRichText(ReadClipboardUnicodeText(), f);
		}
		return RichText();
	}
	virtual void WriteClipboard(const RichText& text) {
		WriteClipboardUnicodeText(text.GetPlainText(), false);
	}
};

struct sRTFCt_ : public RichText::ClipboardType {
#ifdef PLATFORM_WIN32
	int              GetFormatCode() { return GetClipboardFormatCode("Rich Text Format"); }
#else
	int              GetFormatCode() { return GetClipboardFormatCode("text/richtext"); }
#endif
	virtual int      Level() { return 5; }
	virtual RichText ReadClipboard(const RichPara::Format&) {
		String data = ::ReadClipboard(GetFormatCode());
		if(!IsNull(data))
			return ParseRTF(data);
		return RichText();
	}
	virtual void     WriteClipboard(const RichText& text) {
		::WriteClipboard(GetFormatCode(), EncodeRTF(text, GetDefaultCharset()), false);
	}
};

void StdRichClipboardFormats()
{
	ONCELOCK {
		RichText::Register(Single<sPlainCt_>());
		RichText::Register(Single<sQtfCt_>());
		RichText::Register(Single<sRTFCt_>());
	}
}
*/
