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

String Ctrl::Xclipboard::Read(int fmt, int selection, int property)
{
	if(data.GetCount() && selection == XAtom("CLIPBOARD"))
		return data.Get(fmt, Null);
	XConvertSelection(Xdisplay, selection, fmt, property, win, CurrentTime);
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
		   event.xselectionrequest.selection == selection) {
			Request(&event.xselectionrequest);
		}
		if(XCheckTypedWindowEvent(Xdisplay, win, SelectionClear, &event) &&
		   event.xselectionclear.window == win &&
		   event.xselectionclear.selection == selection) {
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

String ReadClipboard(const char *fmt)
{
	return Ctrl::xclipboard().Read(XAtom(fmt), XAtom("CLIPBOARD"), XAtom("CLIPDATA"));
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

bool Ctrl::Xclipboard::IsAvailable(int fmt)
{
	if(data.GetCount())
		return data.Find(fmt) >= 0;
	String formats = Read(XAtom("TARGETS"), XAtom("CLIPBOARD"), XAtom("CLIPDATA"));
	int c = formats.GetCount() / sizeof(Atom);
	const Atom *m = (Atom *) ~formats;
	for(int i = 0; i < c; i++) {
		if(m[i] == fmt)
			return true;
	}
	return false;
}

bool Ctrl::ClipHas(int type, const char *fmt)
{
	if(type == 0)
		return Ctrl::xclipboard().IsAvailable(XAtom(fmt));
	return drop_formats.Find(fmt) >= 0;
}

String Ctrl::ClipGet(int type, const char *fmt)
{
	return Ctrl::xclipboard().Read(
	           XAtom(fmt),
	           type ? XAtom("XdndSelection") : XAtom("CLIPBOARD"),
	           type ? XA_SECONDARY : XAtom("CLIPDATA")
	       );
}

String Unicode__(const WString& w)
{
	return String((const char *)~w, 2 * w.GetLength());
}

WString Unicode__(const String& s)
{
	return WString((const wchar *)~s, s.GetLength() / 2);
}

const char *ClipFmtsText()
{
	return "STRING;UTF8_STRING;text/plain;text/unicode";
}

String GetString(PasteClip& clip)
{
	if(clip.Accept("STRING") || clip.Accept("text/plain"))
		return ~clip;
	if(clip.Accept("UTF8_STRING"))
		return FromUtf8(~clip).ToString();
	if(clip.Accept("text/unicode"))
		return Unicode__(~clip).ToString();
	return Null;
}

WString GetWString(PasteClip& clip)
{
	if(clip.Accept("STRING") || clip.Accept("text/plain"))
		return (~clip).ToWString();
	if(clip.Accept("UTF8_STRING"))
		return FromUtf8(~clip);
	if(clip.Accept("text/unicode"))
		return Unicode__(~clip);
	return Null;
}

String GetTextClip(const WString& text, const String& fmt)
{
	if(fmt == "STRING" || fmt == "text/plain")
		return text.ToString();
	if(fmt == "UTF8_STRING")
		return ToUtf8(text);
	if(fmt == "text/unicode")
		return Unicode__(text);
	return Null;
}

String GetTextClip(const String& text, const String& fmt)
{
	if(fmt == "STRING" || fmt == "text/plain")
		return text;
	if(fmt == "UTF8_STRING")
		return ToUtf8(text.ToWString());
	if(fmt == "text/unicode")
		return Unicode__(text.ToWString());
	return Null;
}

bool AcceptText(PasteClip& clip)
{
	return clip.Accept(ClipFmtsText());
}

void AddTextClip(VectorMap<String, String>& data, const String& text)
{
	data.GetAdd("STRING", text);
	data.GetAdd("text/plain", text);
	data.GetAdd("UTF8_STRING", ToUtf8(text.ToWString()));
	data.GetAdd("text/unicode", Unicode__(text.ToWString()));
}

void AddTextClip(VectorMap<String, String>& data, const WString& text)
{
	data.GetAdd("STRING", text.ToString());
	data.GetAdd("text/plain", text.ToString());
	data.GetAdd("UTF8_STRING", ToUtf8(text));
	data.GetAdd("text/unicode", Unicode__(text));
}

bool IsClipboardAvailable(const char *fmt)
{
	return Ctrl::xclipboard().IsAvailable(XAtom(fmt));
}

bool IsClipboardAvailableText()
{
	return IsClipboardAvailable("STRING") ||
	       IsClipboardAvailable("UTF8_STRING") ||
	       IsClipboardAvailable("text/plain") ||
	       IsClipboardAvailable("text/unicode");
}

#endif

END_UPP_NAMESPACE
