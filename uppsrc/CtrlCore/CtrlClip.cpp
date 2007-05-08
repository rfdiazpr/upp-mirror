#include "CtrlCore.h"

NAMESPACE_UPP

void Ctrl::DragEnter(Point p, PasteClip& d)         {}
void Ctrl::DragAndDrop(Point p, PasteClip& d)       {}
void Ctrl::DragRepeat(Point p) {}
void Ctrl::DragLeave() {}

String Ctrl::GetDropData(const String& fmt) const
{
	return GetSelectionData(fmt);
}

String Ctrl::GetSelectionData(const String& fmt) const
{
	return Null;
}

bool   Has(UDropTarget *dt, const char *fmt);
String Get(UDropTarget *dt, const char *fmt);

bool PasteClip::IsAvailable(const char *fmt) const
{
	return dt ? UPP::Has(dt, fmt) : IsClipboardAvailable(fmt);
}

String PasteClip::Get(const char *fmt) const
{
	return dt ? UPP::Get(dt, fmt) : ReadClipboard(fmt);
}

bool PasteClip::Accept()
{
	accepted = true;
	return paste;
}

bool   PasteClip::Accept(const char *_fmt)
{
	Vector<String> f = Split(_fmt, ';');
	for(int i = 0; i < f.GetCount(); i++) {
		if(IsAccepted() && fmt == _fmt)
			return paste;
		if(IsAvailable(f[i])) {
			accepted = true;
			if(paste) {
				fmt = f[i];
				data = Get(f[i]);
				return true;
			}
			break;
		}
	}
	return false;
}

PasteClip::PasteClip()
{
	prefer = 0;
	paste = true;
	accepted = false;
	dt = NULL;
}

PasteClip Ctrl::Clipboard()
{
	return PasteClip();
}

String ClipFmtsText()
{
	return "wtext;text";
}

bool AcceptText(PasteClip& clip)
{
	return clip.Accept("wtext;text");
}

void AddTextClip(VectorMap<String, String>& data, const String& text)
{
	data.GetAdd("text", GetTextClip(text, "text"));
	data.GetAdd("wtext", GetTextClip(text, "wtext"));
}

void AddTextClip(VectorMap<String, String>& data, const WString& text)
{
	data.GetAdd("text", GetTextClip(text, "text"));
	data.GetAdd("wtext", GetTextClip(text, "wtext"));
}

int Ctrl::DoDragAndDrop(const char *fmts, const Image& sample, dword actions)
{
	VectorMap<String, String> dummy;
	return DoDragAndDrop(fmts, sample, actions, dummy);
}

int Ctrl::DoDragAndDrop(const VectorMap<String, String>& data, const Image& sample, dword actions)
{
	return DoDragAndDrop("", sample, actions, data);
}


Uuid        sDndUuid;
const void *sInternalPtr;

String GetInternalDropId__(const char *type, const char *id)
{
	return "U++ Internal clip:" + AsString(sDndUuid) + '-' + type + '-' + id;
}

void NewInternalDrop__(const void *ptr)
{
	sDndUuid = Uuid::Create();
	sInternalPtr = ptr;
}

const void *GetInternalDropPtr__()
{
	return sInternalPtr;
}

END_UPP_NAMESPACE
