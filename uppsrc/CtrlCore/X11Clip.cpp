#include "CtrlCore.h"

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)

#ifdef PLATFORM_X11

Ctrl::Xclipboard::Xclipboard()
{
	XSetWindowAttributes swa;
	win = XCreateWindow(Xdisplay, RootWindow(Xdisplay, Xscreenno),
	                    0, 0, 10, 10, 0, CopyFromParent, InputOnly, CopyFromParent,
	                    0, &swa);
	XSelectInput(Xdisplay, win, PropertyChangeMask);
}

Ctrl::Xclipboard::~Xclipboard()
{
	XDestroyWindow(Xdisplay, win);
}

void Ctrl::Xclipboard::Write(int fmt, const String& _data)
{
	LLOG("SetSelectionOwner " << XAtomName(fmt));
	data.GetAdd(fmt) = _data;
//	if(XGetSelectionOwner(Xdisplay, XAtom("CLIPBOARD") != win)) // 20060926 Nicomeseas&cxl
	XSetSelectionOwner(Xdisplay, XAtom("CLIPBOARD"), win, CurrentTime);
}

void Ctrl::Xclipboard::Request(XSelectionRequestEvent *se)
{
	LLOG("Request " << XAtomName(se->target));
	XEvent e;
	e.xselection.type      = SelectionNotify;
	e.xselection.display   = Xdisplay;
	e.xselection.requestor = se->requestor;
	e.xselection.selection = XAtom("CLIPBOARD");
	e.xselection.target    = se->target;
	e.xselection.time      = se->time;
	e.xselection.property  = se->property;
	if(se->target == XAtom("TARGETS")) {
		LLOG("Request targets:");
		Buffer<Atom> x(data.GetCount());
		for(int i = 0; i < data.GetCount(); i++) {
			x[i] = data.GetKey(i);
			LLOG('\t' << XAtomName(x[i]));
		}
		XChangeProperty(Xdisplay, se->requestor, se->property, XAtom("ATOM"),
		                8 * sizeof(Atom), 0, (unsigned char*)~x,
		                data.GetCount());
	}
	else {
		int i = data.Find(se->target);
		LLOG("Request data " << i);
		if(i >= 0)
			XChangeProperty(Xdisplay, se->requestor, se->property, se->target, 8, PropModeReplace,
			                data[i], data[i].GetLength());
		else
		    e.xselection.property = None;
	}
	XSendEvent(Xdisplay, se->requestor, XFalse, 0, &e);
}

String Ctrl::Xclipboard::Read(int fmt)
{
	if(data.GetCount())
		return data.Get(fmt, Null);
	XConvertSelection(Xdisplay, XAtom("CLIPBOARD"), fmt, XAtom("CLIPDATA"), win, CurrentTime);
	XFlush(Xdisplay);
	XEvent event;
	for(int i = 0; i < 20; i++) {
		if(XCheckTypedWindowEvent(Xdisplay, win, SelectionNotify, &event)) {
			if(event.xselection.property != None) {
				XSync(Xdisplay, false);
				return ReadPropertyData(win, event.xselection.property);
			}
			return Null;
		}
		if(XCheckTypedWindowEvent(Xdisplay, win, SelectionRequest, &event) &&
		   event.xselectionrequest.owner == win &&
		   event.xselectionrequest.selection == XAtom("CLIPBOARD")) {
			Request(&event.xselectionrequest);
		}
		if(XCheckTypedWindowEvent(Xdisplay, win, SelectionClear, &event) &&
		   event.xselectionclear.window == win &&
		   event.xselectionclear.selection == XAtom("CLIPBOARD")) {
			Clear();
		}
		Sleep(10);
	}
	return Null;
}

Ctrl::Xclipboard& Ctrl::xclipboard()
{
	static Xclipboard xc;
	return xc;
}

void ClearClipboard()
{
	Ctrl::xclipboard().Clear();
}

void AppendClipboard(const char *fmt, const String& data)
{
	Ctrl::xclipboard().Write(XAtom(fmt), data);
}

String  ReadClipboard(const char *fmt)
{
	return Ctrl::xclipboard().Read(XAtom(fmt));
}

void AppendClipboardText(const String& s)
{
	AppendClipboard("STRING", s);
}

String ReadClipboardText()
{
	return ReadClipboard("STRING");
}

void AppendClipboardUnicodeText(const WString& s)
{
	AppendClipboard("UTF8_STRING", ToUtf8(s));
}

WString ReadClipboardUnicodeText()
{
	return FromUtf8(ReadClipboard("UTF8_STRING"));
}

String GetString(PasteClip& clip)
{
	if(clip.Accept("wtext"))
		return FromUtf8(~clip).ToString();
	if(clip.IsAvailable("text"))
		return ~clip;
	return Null;
}

WString GetWString(PasteClip& clip)
{
	if(clip.Accept("wtext"))
		return FromUtf8(~clip);
	if(clip.IsAvailable("text"))
		return (~clip).ToWString();
	return Null;
}

String GetTextClip(const WString& text, const String& fmt)
{
	if(fmt == "text")
		return text.ToString();
	if(fmt == "wtext")
		return ToUtf8(text);
	return Null;
}

String GetTextClip(const String& text, const String& fmt)
{
	if(fmt == "text")
		return text;
	if(fmt == "wtext")
		return GetTextClip(text.ToWString(), fmt);
	return Null;
}

bool Ctrl::Xclipboard::IsAvailable(int fmt)
{
	if(data.GetCount())
		return data.Find(fmt) >= 0;
	String formats = Read(XAtom("TARGETS"));
	int c = formats.GetCount() / sizeof(Atom);
	const Atom *m = (Atom *) ~formats;
	for(int i = 0; i < c; i++) {
		if(m[i] == fmt)
			return true;
	}
	return false;
}

bool IsClipboardAvailable(const char *fmt)
{
	String f = fmt;
	if(f == "text")
		f = "STRING";
	if(f == "wtext")
		f = "UTF8_STRING";
	return Ctrl::xclipboard().IsAvailable(XAtom(fmt));
}

bool IsClipboardAvailableText()
{
	return IsClipboardAvailable("text") || IsClipboardAvailable("wtext");
}

bool Has(UDropTarget *dt, const char *fmt)
{
	return false;
}

String Get(UDropTarget *dt, const char *fmt)
{
	return Null;
}

#endif

END_UPP_NAMESPACE
