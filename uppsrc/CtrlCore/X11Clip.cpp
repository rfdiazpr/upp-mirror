#include "CtrlCore.h"

#define LLOG(x) // LOG(x)

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
	data.GetAdd(fmt) = _data;
	if(XGetSelectionOwner(Xdisplay, XAtom("CLIPBOARD") != win))
		XSetSelectionOwner(Xdisplay, XAtom("CLIPBOARD"), win, CurrentTime);
}

void Ctrl::Xclipboard::Request(XSelectionRequestEvent *se)
{
	LLOG("Request " << XAtomName(se->target));
	XEvent e;
	e.xselection.type      = SelectionNotify;
	e.xselection.display   = Xdisplay;
	e.xselection.requestor = se->requestor;
	e.xselection.selection = Atom("CLIPBOARD");
	e.xselection.target    = se->target;
	e.xselection.time      = se->time;
    e.xselection.property  = se->property;
	if(se->target == XAtom("TARGETS")) {
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
				String data = ReadPropertyData(win, event.xselection.property);
				XDeleteProperty(Xdisplay, win, event.xselection.property);
				return data;
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

int  GetClipboardFormatCode(const String& format_id)
{
	static VectorMap<String, int> format_map;
	int f = format_map.Find(format_id);
	if(f < 0) {
		f = format_map.GetCount();
		format_map.Add(format_id, XAtom(format_id));
	}
	return format_map[f];
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
/*
	String formats = Ctrl::xclipboard().Read(XAtom("TARGETS"));
	int c = formats.GetCount() / sizeof(Atom);
	const Atom *m = (Atom *) ~formats;
	for(int i = 0; i < c; i++)
		LOG("Available " << XAtomName(m[i]));
//*/
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

bool IsClipboardAvailable(const char *fmt)
{
	String formats = Ctrl::xclipboard().Read(XAtom("TARGETS"));
	int c = formats.GetCount() / sizeof(Atom);
	const Atom *m = (Atom *) ~formats;
	int xa = XAtom(fmt);
	for(int i = 0; i < c; i++)
		if(m[i] == xa)
			return true;
	return false;
}

bool IsClipboardAvailableText()
{
	return IsClipboardAvailable("UTF8_STRING");
}

#endif
