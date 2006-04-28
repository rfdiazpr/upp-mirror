#include "CtrlCore.h"

#ifdef PLATFORM_X11

#include <locale.h>

#ifdef _DEBUG
	#define SYNCHRONIZE
#endif

#define LLOG(x) // LOG(x)

Atom      Ctrl::XA_WM_PROTOCOLS;
Atom      Ctrl::XA_WM_DELETE_WINDOW;
Atom      Ctrl::XA_WM_TAKE_FOCUS;

Atom      Ctrl::XA_UPP_SYNC;

Atom Ctrl::XA__NET_WM_CONTEXT_HELP;
Atom Ctrl::XA__MOTIF_WM_HINTS;
Atom Ctrl::XA_KWIN_RUNNING;
Atom Ctrl::XA_KWM_RUNNING;
Atom Ctrl::XA_GNOME_BACKGROUND_PROPERTIES;
Atom Ctrl::XA__QT_SETTINGS_TIMESTAMP_;
Atom Ctrl::XA__NET_SUPPORTED;
Atom Ctrl::XA__NET_VIRTUAL_ROOTS;
Atom Ctrl::XA__NET_WM_NAME;
Atom Ctrl::XA__NET_WM_ICON_NAME;
Atom Ctrl::XA__NET_WM_ICON;
Atom Ctrl::XA__NET_WM_STATE;
Atom Ctrl::XA__NET_WM_STATE_MODAL;
Atom Ctrl::XA__NET_WM_STATE_MAXIMIZED_VERT;
Atom Ctrl::XA__NET_WM_STATE_MAXIMIZED_HORZ;
Atom Ctrl::XA__NET_WM_WINDOW_TYPE;
Atom Ctrl::XA__NET_WM_WINDOW_TYPE_NORMAL;
Atom Ctrl::XA__NET_WM_WINDOW_TYPE_DIALOG;
Atom Ctrl::XA__NET_WM_WINDOW_TYPE_TOOLBAR;
Atom Ctrl::XA__KDE_NET_WM_WINDOW_TYPE_OVERRIDE;
Atom Ctrl::XA__KDE_NET_WM_FRAME_STRUT;
Atom Ctrl::XA__KDE_NET_USER_TIME;
Atom Ctrl::XA__NET_WM_STATE_STAYS_ON_TOP;
Atom Ctrl::XA__NET_WM_MOVERESIZE;
Atom Ctrl::XA_ENLIGHTENMENT_DESKTOP;
Atom Ctrl::XA_WM_STATE;
Atom Ctrl::XA_CLIPBOARD;
Atom Ctrl::XA_CLIPDATA;
Atom Ctrl::XA_TARGETS;

Atom Ctrl::XA_UTF8_STRING;

XIM Ctrl::xim;

String XAtomName(Atom atom)
{
	return XGetAtomName(Xdisplay, atom);
}

String GetProperty(Window w, Atom property, Atom rtype)
{
	String result;
	int format;
	unsigned long nitems, after = 1;
	long offset = 0;
	Atom type = None;
	while (after > 0) {
		byte *data;
		XGetWindowProperty(Xdisplay, w, property, offset, 1024, XFalse, rtype,
		                   &type, &format, &nitems, &after, &data);
		offset += 256;
		if(data) {
			result.Cat((const char *)data, format == 32 ? sizeof(unsigned long) * nitems :
			                               format == 16 ? 2 * nitems :
			                                                  nitems);
			XFree((char *)data);
		}
	}
	return result;
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

Atom XAtom(const char *name)
{
	return XInternAtom(Xdisplay, name, XFalse);
}

int X11ErrorHandler(XDisplay *, XErrorEvent *error)
{
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

void Ctrl::InitX11(const char *display)
{
	InitX11Draw(display);
	InitTimer();
	byte dummy[5];
	Xbuttons = XGetPointerMapping(Xdisplay, dummy, 5);
	XA_WM_PROTOCOLS = XAtom("WM_PROTOCOLS");
	XA_WM_DELETE_WINDOW = XAtom("WM_DELETE_WINDOW");
	XA_WM_TAKE_FOCUS = XAtom("WM_TAKE_FOCUS");
	XA_UPP_SYNC = XAtom("UPP_SYNC__");
	XA__MOTIF_WM_HINTS = XAtom("_MOTIF_WM_HINTS");
	XA_KWIN_RUNNING = XAtom("KWIN_RUNNING");
	XA_KWM_RUNNING = XAtom("KWM_RUNNING");
	XA_GNOME_BACKGROUND_PROPERTIES = XAtom("GNOME_BACKGROUND_PROPERTIES");
	XA__QT_SETTINGS_TIMESTAMP_ = XAtom(String("_QT_SETTINGS_TIMESTAMP_") + Xdisplayname);
	XA__NET_SUPPORTED = XAtom("_NET_SUPPORTED");
	XA__NET_VIRTUAL_ROOTS = XAtom("_NET_VIRTUAL_ROOTS");
	XA__NET_WM_NAME = XAtom("_NET_WM_NAME");
	XA__NET_WM_ICON_NAME = XAtom("_NET_WM_ICON_NAME");
	XA__NET_WM_ICON = XAtom("_NET_WM_ICON");
	XA__NET_WM_CONTEXT_HELP = XAtom("_NET_WM_CONTEXT_HELP");
	XA__NET_WM_STATE = XAtom("_NET_WM_STATE");
	XA__NET_WM_STATE_MODAL = XAtom("_NET_WM_STATE_MODAL");
	XA__NET_WM_STATE_MAXIMIZED_VERT = XAtom("_NET_WM_STATE_MAXIMIZED_VERT");
	XA__NET_WM_STATE_MAXIMIZED_HORZ = XAtom("_NET_WM_STATE_MAXIMIZED_HORZ");
	XA__NET_WM_WINDOW_TYPE = XAtom("_NET_WM_WINDOW_TYPE");
	XA__NET_WM_WINDOW_TYPE_NORMAL = XAtom("_NET_WM_WINDOW_TYPE_NORMAL");
	XA__NET_WM_WINDOW_TYPE_DIALOG = XAtom("_NET_WM_WINDOW_TYPE_DIALOG");
	XA__NET_WM_WINDOW_TYPE_TOOLBAR = XAtom("_NET_WM_WINDOW_TYPE_TOOLBAR");
	XA__KDE_NET_WM_WINDOW_TYPE_OVERRIDE = XAtom("_KDE_NET_WM_WINDOW_TYPE_OVERRIDE");
	XA__KDE_NET_WM_FRAME_STRUT = XAtom("_KDE_NET_WM_FRAME_STRUT");
	XA__NET_WM_STATE_STAYS_ON_TOP = XAtom("_NET_WM_STATE_STAYS_ON_TOP");
	XA__NET_WM_MOVERESIZE = XAtom("XA__NET_WM_MOVERESIZE");
	XA_ENLIGHTENMENT_DESKTOP = XAtom("ENLIGHTENMENT_DESKTOP");
	XA_UTF8_STRING = XAtom("UTF8_STRING");
	XA__KDE_NET_USER_TIME = XAtom("_KDE_NET_USER_TIME");
	XA_WM_STATE = XAtom("WM_STATE");
	XA_CLIPBOARD = XAtom("CLIPBOARD");
	XA_CLIPDATA = XAtom("CLIPDATA");
	XA_TARGETS = XAtom("TARGETS");

	Xeventtime = CurrentTime;
	XSetErrorHandler(X11ErrorHandler);
#ifdef SYNCHRONIZE
	XSynchronize(Xdisplay, 1);
#endif
	Vector<int> nets = GetPropertyInts(Xroot, Ctrl::XA__NET_SUPPORTED);
	for(int i = 0; i < nets.GetCount(); i++)
		_NET_Supported().Add(nets[i]);

	Ctrl::SetXPStyle();
	Ctrl::SetFlags(Ctrl::EFFECT_SLIDE | Ctrl::DRAGFULLWINDOW | Ctrl::DROPSHADOWS | Ctrl::ALTACCESSKEYS);

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
/*
bool Ctrl::IsOpenEffectsMode() {
	return KDESettings()("KDE", "EffectAnimateMenu") == "true";
}
*/
#endif
