#include "CtrlCore.h"

#ifdef PLATFORM_X11
#include <locale.h>
#endif

NAMESPACE_UPP

#ifdef PLATFORM_X11

#ifdef _DEBUG
//	#define SYNCHRONIZE
#endif

#define LLOG(x)  // LOG(x)

XIM Ctrl::xim;

Atom XAtom(const char *name)
{
	Atom x;
	INTERLOCKED {
		static VectorMap<String, int> atoms;
		int q = atoms.Get(name, Null);
		if(IsNull(q)) {
			q = XInternAtom(Xdisplay, name, XFalse);
			atoms.Add(name, q);
		}
		x = q;
	}
	return x;
}

String XAtomName(Atom atom)
{
	LLOG("GetAtomName");
	return XGetAtomName(Xdisplay, atom);
}

String GetProperty(Window w, Atom property, Atom rtype)
{
	LLOG("GetProperty");
	String result;
	int format;
	unsigned long nitems, after = 1;
	long offset = 0;
	Atom type = None;
	unsigned char *data;
	long rsize = minmax((long)(XMaxRequestSize(Xdisplay) - 100), (long)256, (long)65536);
	while(after > 0) {
		if(XGetWindowProperty(Xdisplay, w, property, offset, rsize, XFalse,
	                          rtype, &type, &format, &nitems, &after, &data) != Success)
	    	break;
	    if(type == None)
	        break;
		if(data) {
			int len = format == 32 ? sizeof(unsigned long) * nitems : nitems * (format >> 3);
			result.Cat(data, len);
			XFree((char *)data);
			offset += nitems / (32 / format);
		}
		else
			break;
	}
	result.Shrink();
	XFlush(Xdisplay);
	return result;
}

bool WaitForEvent(Window w, int type, XEvent& event){
	for(int i = 0; i < 80; i++) {
		if(XCheckTypedWindowEvent(Xdisplay, w, type, &event))
			return true;
		XFlush(Xdisplay);
		Sleep(50);
	}
	LOG("WaitForEvent failed");
	return false;
}


String ReadPropertyData(Window w, Atom property, Atom rtype)
{
	static Atom XA_INCR = XAtom("INCR");
	Atom type;
	int format;
	unsigned long nitems, after;
	unsigned char *ptr;
	XEvent event;
	String r;
	if(XGetWindowProperty(Xdisplay, w, property, 0, 0, XFalse, AnyPropertyType,
	                      &type, &format, &nitems, &after, &ptr) == Success && type != None) {
		XFree(ptr);
		if(type == XA_INCR) {
			XDeleteProperty(Xdisplay, w, property);
			XEvent event;
			for(;;) {
				XFlush(Xdisplay);
				if(!WaitForEvent(w, PropertyNotify, event))
					break;
				if(event.xproperty.atom == property && event.xproperty.state == PropertyNewValue) {
					String x = GetProperty(w, property);
					if(!x.GetLength())
						break;
					r.Cat(x);
					XDeleteProperty(Xdisplay, w, property);
				}
			}
		}
		else {
			r = GetProperty(w, property);
			XDeleteProperty(Xdisplay, w, property);
		}
	}
	return r;
}

Vector<int> GetPropertyInts(Window w, Atom property, Atom rtype)
{
	Vector<int> result;
	String p = GetProperty(w, property, rtype);
	const long int *ptr = (const long int *)~p;
	const long int *lim = ptr + p.GetLength() / 4;
	result.Reserve(p.GetLength() / 4);
	while(ptr < lim)
		result.Add(*ptr++);
	return result;
}

Index<Atom>& _NET_Supported()
{
	static Index<Atom> q;
	return q;
}

bool X11ErrorTrap;

bool Ctrl::TrapX11Errors()
{
	bool b = X11ErrorTrap;
	X11ErrorTrap = true;
}

void Ctrl::UntrapX11Errors(bool b)
{
	X11ErrorTrap = b;
}

int X11ErrorHandler(XDisplay *, XErrorEvent *error)
{
#ifndef _DEBUG
	if(X11ErrorTrap) return 0;
#endif

	static const char *request[] = {
		"",
		"X_CreateWindow",
		"X_ChangeWindowAttributes",
		"X_GetWindowAttributes",
		"X_DestroyWindow",
		"X_DestroySubwindows",
		"X_ChangeSaveSet",
		"X_ReparentWindow",
		"X_MapWindow",
		"X_MapSubwindows",
		"X_UnmapWindow",
		"X_UnmapSubwindows",
		"X_ConfigureWindow",
		"X_CirculateWindow",
		"X_GetGeometry",
		"X_QueryTree",
		"X_InternAtom",
		"X_GetAtomName",
		"X_ChangeProperty",
		"X_DeleteProperty",
		"X_GetProperty",
		"X_ListProperties",
		"X_SetSelectionOwner",
		"X_GetSelectionOwner",
		"X_ConvertSelection",
		"X_SendEvent",
		"X_GrabPointer",
		"X_UngrabPointer",
		"X_GrabButton",
		"X_UngrabButton",
		"X_ChangeActivePointerGrab",
		"X_GrabKeyboard",
		"X_UngrabKeyboard",
		"X_GrabKey",
		"X_UngrabKey",
		"X_AllowEvents",
		"X_GrabServer",
		"X_UngrabServer",
		"X_QueryPointer",
		"X_GetMotionEvents",
		"X_TranslateCoords",
		"X_WarpPointer",
		"X_SetInputFocus",
		"X_GetInputFocus",
		"X_QueryKeymap",
		"X_OpenFont",
		"X_CloseFont",
		"X_QueryFont",
		"X_QueryTextExtents",
		"X_ListFonts",
		"X_ListFontsWithInfo",
		"X_SetFontPath",
		"X_GetFontPath",
		"X_CreatePixmap",
		"X_FreePixmap",
		"X_CreateGC",
		"X_ChangeGC",
		"X_CopyGC",
		"X_SetDashes",
		"X_SetClipRectangles",
		"X_FreeGC",
		"X_ClearArea",
		"X_CopyArea",
		"X_CopyPlane",
		"X_PolyPoint",
		"X_PolyLine",
		"X_PolySegment",
		"X_PolyRectangle",
		"X_PolyArc",
		"X_FillPoly",
		"X_PolyFillRectangle",
		"X_PolyFillArc",
		"X_PutImage",
		"X_GetImage",
		"X_PolyText8",
		"X_PolyText16",
		"X_ImageText8",
		"X_ImageText16",
		"X_CreateColormap",
		"X_FreeColormap",
		"X_CopyColormapAndFree",
		"X_InstallColormap",
		"X_UninstallColormap",
		"X_ListInstalledColormaps",
		"X_AllocColor",
		"X_AllocNamedColor",
		"X_AllocColorCells",
		"X_AllocColorPlanes",
		"X_FreeColors",
		"X_StoreColors",
		"X_StoreNamedColor",
		"X_QueryColors",
		"X_LookupColor",
		"X_CreateCursor",
		"X_CreateGlyphCursor",
		"X_FreeCursor",
		"X_RecolorCursor",
		"X_QueryBestSize",
		"X_QueryExtension",
		"X_ListExtensions",
		"X_ChangeKeyboardMapping",
		"X_GetKeyboardMapping",
		"X_ChangeKeyboardControl",
		"X_GetKeyboardControl",
		"X_Bell",
		"X_ChangePointerControl",
		"X_GetPointerControl",
		"X_SetScreenSaver",
		"X_GetScreenSaver",
		"X_ChangeHosts",
		"X_ListHosts",
		"X_SetAccessControl",
		"X_SetCloseDownMode",
		"X_KillClient",
		"X_RotateProperties",
		"X_ForceScreenSaver",
		"X_SetPointerMapping",
		"X_GetPointerMapping",
		"X_SetModifierMapping",
		"X_GetModifierMapping",
		"X_NoOperation",
	};

	char h[512];
	XGetErrorText(Xdisplay, error->error_code, h, 512);
	String e;
	e << "X Error: " << h;
	if(error->request_code < __countof(request))
		e << ", request: " << request[error->request_code];
	e << ", resource id: " << (int)error->resourceid << " = " << Format("%0X", (int)error->resourceid);
	RLOG(e);
	puts(e);
	BugLog() << e << "\r\n";
	UsrLogT(e);
	return 0;
}

void SetX11ErrorHandler()
{
	XSetErrorHandler(X11ErrorHandler);
}

void Ctrl::InitX11(const char *display)
{
	InitX11Draw(display);
	InitTimer();
	byte dummy[5];
	Xbuttons = XGetPointerMapping(Xdisplay, dummy, 5);

	Xeventtime = CurrentTime;
	SetX11ErrorHandler();
#ifdef SYNCHRONIZE
	XSynchronize(Xdisplay, 1);
#endif
	Vector<int> nets = GetPropertyInts(Xroot, XAtom("_NET_SUPPORTED"));
	for(int i = 0; i < nets.GetCount(); i++)
		_NET_Supported().Add(nets[i]);

	ChSync();

	GUI_GlobalStyle_Write(GUISTYLE_XP);
	GUI_DragFullWindow_Write(1);
	GUI_PopUpEffect_Write(GUIEFFECT_SLIDE);
	GUI_DropShadows_Write(1);
	GUI_AltAccessKeys_Write(1);
	GUI_AKD_Conservative_Write(0);

	setlocale(LC_ALL, "en_US.utf8");
	if(XSupportsLocale()) {
		XSetLocaleModifiers("");
		xim = XOpenIM(Xdisplay, NULL, NULL, NULL);
	}
	else {
		xim = NULL;
		LOG("IM unsupported!");
	}
}

void Ctrl::ExitX11()
{
//	if(xic)
//		XDestroyIC(xic);
	if(xim)
		XCloseIM(xim);
}

Rect Ctrl::GetDefaultWindowRect()
{
	static int pos = 0;
	pos += 10;
	int cx = Xwidth * 2 / 3;
	int cy = Xheight * 2 / 3;
	if(pos + cx + 50 > Xwidth || pos + cy + 50 > Xheight)
		pos = 0;
	return RectC(pos + 20, pos + 20, cx, cy);
}

Rect Ctrl::GetWorkArea()
{
	static Rect r;
	if(r.right == 0) {
		Vector<int> x = GetPropertyInts(Xroot, XAtom("_NET_WORKAREA"));
		if(x.GetCount())
			r = RectC(x[0], x[1], x[2], x[3]);
		else
			r = RectC(0, 0, Xwidth, Xheight);
	}
	return r;
}

Rect Ctrl::GetScreenArea()
{
	return RectC(0, 0, Xwidth, Xheight);
}

int Ctrl::GetKbdDelay()
{
	return 250;
}

int Ctrl::GetKbdSpeed()
{
	return 25;
}

#endif

END_UPP_NAMESPACE
