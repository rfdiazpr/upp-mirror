#include "CtrlCore.h"

NAMESPACE_UPP

void Ctrl::DragEnter(Point p, PasteClip& d)         {}
void Ctrl::DragAndDrop(Point p, PasteClip& d)       {}
void Ctrl::ChildDragAndDrop(Point p, PasteClip& d)  {}
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

#ifdef PLATFORM_WIN32
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
#endif

#ifdef PLATFORM_X11
bool   Is(const char *fmt);
String XDnDGet(const char *fmt);

bool PasteClip::IsAvailable(const char *fmt) const
{
	return Ctrl::ClipHas(type, fmt);
}

String PasteClip::Get(const char *fmt) const
{
	return Ctrl::ClipGet(type, fmt);
}
#endif

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
	paste = true;
	accepted = false;
#ifdef PLATFORM_WIN32
	dt = NULL;
#else
	type = 0;
#endif
}

PasteClip& Ctrl::Clipboard()
{
	static PasteClip d;
	d.fmt.Clear();
	return d;
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

Ctrl *Ctrl::FindCtrl(Ctrl *ctrl, Point& p)
{
	for(;;) {
		Ctrl *c = ctrl->ChildFromPoint(p);
		if(!c) break;
		ctrl = c;
	}
	return ctrl;
}

static String sUnicode(const WString& w)
{
	return String((const char *)~w, w.GetLength() * 2);
}

Image MakeDragImage(const Image& arrow, Image sample)
{
	ImageBuffer b;
	if(IsNull(sample)) {
		sample = CtrlCoreImg::DndData();
		b = sample;
		Over(b, Point(0, 0), arrow, arrow.GetSize());
	}
	else {
		b.Create(128, 128);
		memset(~b, 0, sizeof(RGBA) * b.GetLength());
		Size ssz = sample.GetSize();
		Over(b, Point(2, 22), sample, sample.GetSize());
		for(int y = 20; y < 96; y++) {
			RGBA *s = b[y];
			RGBA *e = s + 96;
			while(s < e)
				(s++)->a >>= 1;
			e += 32;
			int q = 128;
			while(s < e) {
				s->a = (s->a * q) >> 8;
				q -= 4;
				s++;
			}
		}
		int qq = 128;
		for(int y = 96; y < 128; y++) {
			RGBA *s = b[y];
			RGBA *e = s + 96;
			while(s < e) {
				s->a = (s->a * qq) >> 8;
				s++;
			}
			e += 32;
			int q = 255;
			while(s < e) {
				s->a = (s->a * q * qq) >> 16;
				q -= 8;
				s++;
			}
			qq -= 4;
		}
		RGBA *s = b[21] + 1;
		RGBA c1 = Blue();
		RGBA c2 = White();
		for(int a = 255; a > 0; a -= 3) {
			c1.a = c2.a = a;
			*s++ = c1;
			Swap(c1, c2);
		}
		s = b[21] + 1;
		c1 = Black();
		c2 = White();
		for(int a = 255; a > 0; a -= 8) {
			c1.a = c2.a = a;
			*s = c1;
			s += b.GetWidth();
			Swap(c1, c2);
		}
	}
	Over(b, Point(0, 0), arrow, arrow.GetSize());
	return b;
}

END_UPP_NAMESPACE
