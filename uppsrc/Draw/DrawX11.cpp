#include "Draw.h"
#pragma hdrstop

#define LLOG(x)    // LOG(x)
#define LTIMING(x) // TIMING(x)

#ifdef PLATFORM_X11

XDisplay   *Xdisplay;
const char *Xdisplayname;
int         Xscreenno;
Visual     *Xvisual;
Window      Xroot;
Screen     *Xscreen;
Colormap    Xcolormap;
int         Xheight;
int         Xwidth;
int         XheightMM;
int         XwidthMM;
int         Xdepth;
dword       Xblack;
dword       Xwhite;
int         Xconnection;
byte       *Xmapcolor;
byte       *Xunmapcolor;

dword   (*Xgetpixel)(int r, int g, int b);

void StaticExitDraw_()
{
	Draw::FreeFonts();
}

EXITBLOCK
{
	if(Xdisplay) {
		StaticExitDraw_();
// No CloseDisplay for now...
		XCloseDisplay(Xdisplay);
		LLOG("Xdisplay closed");
		Xdisplay = NULL;
	}
	if(Xmapcolor)
		delete[] Xmapcolor;
	if(Xunmapcolor)
		delete[] Xunmapcolor;
}

void XError()
{
	Panic("X11 error !");
}

void   XError(const char *s)
{
	Panic(String("X11 error:") + s + " !");
}

static int sAcs;

bool sAllocColor(int xr, int xg, int xb)
{
	XColor ce;
	ce.red = xr;
	ce.green = xg;
	ce.blue = xb;
	ce.flags = DoRed | DoGreen | DoBlue;
	if(!XAllocColor(Xdisplay, Xcolormap, &ce)) return false;
	sAcs++;
	return sAcs < 257;
}

void sAllocColors()
{
	int r, g, b;
	XColor ce;
	for(r = 0; r < 2; r++)
		for(g = 0; g < 2; g++)
			for(b = 0; b < 2; b++)
				if(!sAllocColor(65535 * r, 65535 * g, 65535 * b)) return;
	for(r = 0; r < 3; r++)
		for(g = 0; g < 3; g++)
			for(b = 0; b < 3; b++)
				if((r == 1 || g == 1 || b == 1) &&
				   !sAllocColor((65535 * r) / 2, (65535 * g) / 2, (65535 * b) / 2)) return;
	for(r = 5; r >= 0; r--)
		for(g = 5; g >= 0; g--)
			for(b = 5; b >= 0; b--)
				if((r != 0 && r != 5 || g != 0 && g != 5 || b != 0 && b != 5) &&
				   !sAllocColor((65535 * r) / 5, (65535 * g) / 5, (65535 * b) / 5)) return;
	for(r = 1; r <= 11; r += 2)
		if(!sAllocColor((65535 * r) / 11, (65535 * r) / 11, (65535 * r) / 11)) return;
	for(int r = 255; r >= 0; r--)
		if(!sAllocColor(r << 8, r << 8, r << 8)) return;
}

dword GetPseudoColorPixel(int r, int g, int b)
{
	return Xmapcolor[r * 11 / 255 * (24 * 12) + g * 23 / 255 * 12 + b * 11 / 255];
}

static
struct Xshift {
	dword  mask;
	int    bits;
	int    shift;
	dword  Do(int c) { return (c >> bits << shift) & mask; }
}
Xred, Xgreen, Xblue;

Xshift CalcXShift(dword mask)
{
	Xshift f;
	f.mask = mask;
	f.shift = 0;
	f.bits = 0;
	while((mask & 1) == 0) {
		mask >>= 1;
		f.shift++;
	}
	while((mask & 1) == 1) {
		mask >>= 1;
		f.bits++;
	}
	f.bits = 8 - f.bits;
	if(f.bits < 0) {
		f.shift += f.bits;
		f.bits = 0;
	}
	LLOG("xshift(" << FormatIntHex(mask) << "): mask = "
		<< FormatIntHex(f.mask) << ", bits = " << f.bits << ", shift = " << f.shift);
	return f;
}

dword GetTrueColorPixel(int r, int g, int b)
{
	return Xred.Do(r) | Xgreen.Do(g) | Xblue.Do(b);
}

inline int ssq(int x) { return x * x; }

void UpdateSColors();

void InitX11Draw(const char *dispname)
{
	if(!dispname || !*dispname) {
		int f = Environment().Find("DISPLAY");
		dispname = (f >= 0 ? ~Environment()[f] : ":0.0");
	}
	Xdisplay = XOpenDisplay(dispname);
	Xdisplayname = XDisplayName(dispname);
	if(!Xdisplay) {
		puts(NFormat("Xdisplayname = %s, errno = %d, %s", Xdisplayname, errno, strerror(errno)));
		fflush(stdout);
		XError();
	}
	int Xscreenno = DefaultScreen(Xdisplay);
	Xroot = RootWindow(Xdisplay, Xscreenno);
	Xscreen = ScreenOfDisplay(Xdisplay, Xscreenno);
	Xcolormap = DefaultColormap(Xdisplay, Xscreenno);
	Xheight = DisplayHeight(Xdisplay, Xscreenno);
	Xwidth = DisplayWidth(Xdisplay, Xscreenno);
	XheightMM = DisplayHeightMM(Xdisplay, Xscreenno);
	XwidthMM = DisplayWidthMM(Xdisplay, Xscreenno);
	LLOG("Xwidth = " << Xwidth << ", XwidthMM = " << XwidthMM);
	LLOG("Xheight = " << Xheight << ", XheightMM = " << XheightMM);
	Xdepth = DefaultDepth(Xdisplay, Xscreenno);
	Xblack = BlackPixel(Xdisplay, 0);
	Xwhite = WhitePixel(Xdisplay, 0);
	Xconnection = XConnectionNumber(Xdisplay);
	Xvisual = DefaultVisual(Xdisplay, Xscreenno);
	Visual *v = Xvisual;
	if(v->c_class == TrueColor) {
//		XVisualInfo vtempl;
//		vtempl.visualid = XVisualIDFromVisual(v);
//		int nfound = 0;
//		XVisualInfo *v = XGetVisualInfo(Xdisplay, VisualIDMask, &vtempl, &nfound);
//		if(!nfound || !v)
//			XError();
		Xred = CalcXShift(v->red_mask);
		Xgreen = CalcXShift(v->green_mask);
		Xblue = CalcXShift(v->blue_mask);
		Xgetpixel = GetTrueColorPixel;
	}
	else {
		sAllocColors();
		dword color[256];
		int colorcount = max(1 << Xdepth, 256);
		Buffer<XColor> cs(colorcount);
		int i;
		for(i = 0; i < colorcount; i++)
			cs[i].pixel = i;
		XQueryColors(Xdisplay, Xcolormap, cs, colorcount);
		Xunmapcolor = new byte[3 * colorcount];
		for(i = 0; i < colorcount; i++)
		{
			Xunmapcolor[3 * i + 0] = cs[i].blue;
			Xunmapcolor[3 * i + 1] = cs[i].green;
			Xunmapcolor[3 * i + 2] = cs[i].red;
		}
		byte *cm = Xmapcolor = new byte[12 * 24 * 12];
		for(int r = 0; r < 12; r++)
			for(int g = 0; g < 24; g++)
				for(int b = 0; b < 12; b++) {
					int mind = INT_MAX;
					int mini;
					for(int i = 0; i < colorcount; i++) {
						int d = ssq(r * 255 / 11 - (cs[i].red >> 8)) +
						        ssq(g * 255 / 23 - (cs[i].green >> 8)) +
						        ssq(b * 255 / 11 - (cs[i].blue >> 8));
						if(d < mind) {
							mini = i;
							mind = d;
						}
					}
					*cm++ = mini;
				}
		Xgetpixel = GetPseudoColorPixel;
	}
//	XFree(v);

	Draw::SetStdFont(ScreenSans(12));

	UpdateSColors();
}

#ifdef PLATFORM_XFT
void SetClip(GC gc, XftDraw *xftdraw, const Vector<Rect>& cl)
#else
void SetClip(GC gc, const Vector<Rect>& cl)
#endif
{
	LTIMING("SetClip");
	Buffer<XRectangle> xr(cl.GetCount());
	LLOG("SetClip");
	for(int i = 0; i < cl.GetCount(); i++) {
		XRectangle& r = xr[i];
		const Rect& cr = cl[i];
		LLOG("[" << i << "] = " << cr);
		r.x = cr.left;
		r.y = cr.top;
		r.width = cr.Width();
		r.height = cr.Height();
	}
	XSetClipRectangles(Xdisplay, gc, 0, 0, xr, cl.GetCount(), Unsorted);
#ifdef PLATFORM_XFT
	LLOG("XftDrawSetClipRectangles, # = " << cl.GetCount() << ", xftdraw = " << FormatIntHex(xftdraw));
	XftDrawSetClipRectangles(xftdraw, 0, 0, xr, cl.GetCount());
	LLOG("//XftDrawSetClipRectangles");
#endif
}

void Draw::CloneClip()
{
	if(cloff.GetCount() > 1 && cloff.Top().clipi == cloff[cloff.GetCount() - 2].clipi) {
		const Vector<Rect>& cl = clip.Top();
		cloff.Top().clipi = clip.GetCount();
		clip.Add() <<= cl;
	}
}

void Draw::SetForeground(Color color)
{
	LTIMING("SetForeground");
	if(IsDrawing()) return;
	int p = GetXPixel(color.GetR(), color.GetG(), color.GetB());
	if(p == foreground) return;
	LTIMING("SetForeground");
	foreground = p;
	XSetForeground(Xdisplay, gc, foreground);
}

void Draw::SetClip() {
	if(IsDrawing() || dw == Xroot) return;
	LTIMING("SetClip");
#ifdef PLATFORM_XFT
	::SetClip(gc, xftdraw, clip.Top());
#else
	::SetClip(gc, clip.Top());
#endif
}

void Draw::SetLineStyle(int width)
{
	if(IsDrawing()) return;
	if(width == linewidth) return;
	linewidth = width;
	if(width < PEN_SOLID) {
		static const char dash[] = { 18, 6 };
		static const char dot[] = { 3, 3 };
		static const char dashdot[] = { 9, 6, 3, 6 };
		static const char dashdotdot[] = { 9, 3, 3, 3, 3, 3 };
		static struct {
			const char *dash;
			int   len;
		} ds[] = {
			dash, __countof(dash),
			dot, __countof(dot),
			dashdot, __countof(dashdot),
			dashdotdot, __countof(dashdotdot)
		};
		int i = -(width - PEN_DASH);
		ASSERT(i >= 0 && i < 4);
		XSetDashes(Xdisplay, gc, 0, ds[i].dash, ds[i].len);
	}
	XSetLineAttributes(Xdisplay, gc, max(width, 1),
	                   width < PEN_SOLID ? LineOnOffDash : LineSolid, CapRound, JoinRound);
}

void Draw::Init()
{
	pagePixels = Size(Xwidth, Xheight);
	pageMMs = Size(XwidthMM, XheightMM);
	inchPixels = 254 * pagePixels / pageMMs / 10;
	sheetPixels = pagePixels;
	pageOffset = Point(0, 0);
	InitFonts();
	cloff.Clear();
	clip.Clear();
	foreground = linewidth = Null;
	device = 0;
	device = 0;
	pixels = true;
	printer = aborted = backdraw = is_mono = false;
}

void Draw::Init(const Vector<Rect>& _clip, Point _offset)
{
	Init();
	clip.Add() <<= _clip;
	offset.Add(_offset);
	actual_offset = _offset;
	Cloff& f = cloff.Add();
	f.offseti = 0;
	f.clipi = 0;
	SetClip();
}

Draw::Draw()
{
	dw = None;
	gc = None;
	actual_offset = Point(0, 0);
	Init();
}

#ifdef PLATFORM_XFT
Draw::Draw(Drawable _dw, GC _gc, XftDraw *_xftdraw, const Vector<Rect>& _clip)
#else
Draw::Draw(Drawable _dw, GC _gc, const Vector<Rect>& _clip)
#endif
{
	LLOG("Draw");
//	LLOG(_clip);
	dw = _dw;
	gc = _gc;
#ifdef PLATFORM_XFT
	xftdraw = _xftdraw;
#endif
	Init(_clip);
}

Draw::~Draw()
{
}

void BackDraw::Create(Draw& w, int cx, int cy)
{
	LLOG("Creating BackDraw " << cx << "x" << cy);
	Destroy();
	size.cx = cx;
	size.cy = cy;
	dw = XCreatePixmap(Xdisplay, w.GetDrawable(), max(cx, 1), max(cy, 1), Xdepth);
	gc = XCreateGC(Xdisplay, dw, 0, 0);
#ifdef PLATFORM_XFT
	xftdraw = XftDrawCreate(Xdisplay, (Drawable) dw,
	                        DefaultVisual(Xdisplay, Xscreenno), Xcolormap);
#endif
	Vector<Rect> clip;
	clip.Add(RectC(0, 0, cx, cy));
	Init(clip, Point(0, 0));
	backdraw = true;
}

void BackDraw::Put(Draw& w, int x, int y)
{
	LLOG("Putting BackDraw");
	ASSERT(dw != None);
	XCopyArea(Xdisplay, dw, w.GetDrawable(), w.GetGC(), 0, 0, size.cx, size.cy,
	          x + w.GetOffset().x, y + w.GetOffset().y);
}

void BackDraw::Destroy()
{
	if(dw != None) {
	#ifdef PLATFORM_XFT
		XftDrawDestroy(xftdraw);
	#endif
		XFreePixmap(Xdisplay, dw);
		XFreeGC(Xdisplay, gc);
	}
}

NilDraw::NilDraw()
{
	dw = Xroot;
	gc = XCreateGC(Xdisplay, Xroot, 0, 0);
	pixels = false;
	Init(Vector<Rect>());
}

NilDraw::~NilDraw()
{
	XFreeGC(Xdisplay, gc);
}

Draw& GLOBAL_V(NilDraw, ScreenInfo)

#endif
