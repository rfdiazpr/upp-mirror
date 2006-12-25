#include "CtrlLib.h"


NAMESPACE_UPP

#ifdef PLATFORM_X11

#define SAVEPNG(name, m) // LOGPNG(name, m)

// very system dependent layout of certain GTK structures...
struct G_rect  { int x, y, cx, cy; };
struct G_color { dword _; word r, g, b; };

#ifdef CPU_64
#define GTK__ALLOCATION_OFFSET 64
#define GTK__COLOR_OFFSET      24

struct G_toggle {
	byte filler[144];
	int  active:1;
	int  ind:1;
	int  inconsistent:1;
};
#else
#define GTK__ALLOCATION_OFFSET 36
#define GTK__COLOR_OFFSET      12

struct G_toggle {
	byte filler[88];
	int  active:1;
	int  ind:1;
	int  inconsistent:1;
};
#endif

G_obj *gtk__parent()
{
	static G_obj *p;
	if(!p) {
		p = GTK().gtk_window_new(1);
		GTK().gtk_widget_realize(p);
	}
	return p;
}

G_obj *Setup(G_obj *widget)
{
	static G_obj *fl;
	if (!fl) {
		fl = GTK().gtk_fixed_new();
		GTK().gtk_container_add(gtk__parent(), fl);
		GTK().gtk_widget_realize(fl);
	}

	GTK().gtk_fixed_put(fl, widget, 0, 0);
	GTK().gtk_widget_realize(widget);
	GTK().gtk_widget_show(widget);
	return widget;
}

enum {
	GTK_BOX,
	GTK_CHECK,
	GTK_OPTION,
	GTK_ARROW,
	GTK_SLIDER,
	GTK_ICON,
	GTK_EXT,

	GTK_MARGIN1 = 0x0010,
	GTK_MARGIN2 = 0x0020,
	GTK_MARGIN3 = 0x0030,
	GTK_MARGIN4 = 0x0040,

	GTK_VAL1    = 0x0100,
	GTK_VAL2    = 0x0200,
	GTK_VAL3    = 0x0300,
	GTK_VAL4    = 0x0400,

	GTK_TOP     = 0x1000,
	GTK_VCENTER = 0x2000,
	GTK_BOTTOM  = 0x3000,
	GTK_LEFT    = 0x4000,
	GTK_HCENTER = 0x5000,
	GTK_RIGHT   = 0x6000,
};

static Image sLastImage;

Image GetGTK(G_obj *widget, int state, int shadow, const char *detail, int type, int cx, int cy)
{
	G_obj *icon;
	if(type == GTK_ICON) {
		GTK().gtk_widget_set_sensitive(widget, 1);
		GTK().gtk_widget_set_state(widget, 0);
		icon = GTK().gtk_widget_render_icon(widget, detail, state, NULL);
		if(!icon) return Null;
		cx = GPIXBUF().gdk_pixbuf_get_width(icon);
		cy = GPIXBUF().gdk_pixbuf_get_height(icon);
	}
	else {
		GTK().gtk_widget_set_sensitive(widget, state != 4);
		GTK().gtk_widget_set_state(widget, state);
	}
	Rect rect;
	int ht = type & 0xf000;
	rect.top = ht == GTK_VCENTER ? cy : ht == GTK_BOTTOM ? 2 * cy : 0;
	rect.left = ht == GTK_HCENTER ? cx : ht == GTK_RIGHT ? 2 * cx : 0;
	rect.SetSize(cx, cy);
	if(type >= GTK_LEFT && type <= GTK_RIGHT)
		cx *= 3;
	if(type >= GTK_TOP && type <= GTK_BOTTOM)
		cy *= 3;
	type &= ~0xf000;
	G_rect *allocation = (G_rect *)((byte*)widget + GTK__ALLOCATION_OFFSET);
	allocation->x = 0;
	allocation->y = 0;
	allocation->cx = cx;
	allocation->cy = cy;
	Image m[2];
	Color back = White;
	int margin = (type >> 4) & 15;
	for(int i = 0; i < 2; i++) {
		ImageDraw iw(cx + 2 * margin, cy + 2 * margin);
		iw.DrawRect(0, 0, cx + 2 * margin, cy + 2 * margin, back);
		static G_obj *cm = GDK().gdk_x11_colormap_foreign_new(
			GDK().gdkx_visual_get(XVisualIDFromVisual(Xvisual)), Xcolormap);
		G_obj *pixmap = GDK().gdk_pixmap_foreign_new(iw.GetDrawable());
		GDK().gdk_drawable_set_colormap(pixmap, cm);
		G_rect cr;
		cr.x = 0;
		cr.y = 0;
		cr.cx = cx;
		cr.cy = cy;
		G_obj *style = GTK().gtk_widget_get_style(widget);
		int rcx = max(rect.GetWidth() - 2 * margin, 0);
		int rcy = max(rect.GetHeight() - 2 * margin, 0);
		int t1 = (type & 0xf00) >> 8;
		switch(type & 15) {
		case GTK_BOX:
			GTK().gtk_paint_box(style, pixmap, state, shadow, &cr,
			                    widget, detail,
			                    rect.left + margin, rect.top + margin, rcx, rcy);
			break;
		case GTK_CHECK:
			GTK().gtk_paint_check(style, pixmap, state, shadow, &cr,
			                      widget, detail,
			                      rect.left + margin, rect.top + margin, rcx, rcy);
			break;
		case GTK_OPTION:
			GTK().gtk_paint_option(style, pixmap, state, shadow, &cr,
			                       widget, detail,
			                       rect.left + margin, rect.top + margin, rcx, rcy);
			break;
		case GTK_ARROW:
			GTK().gtk_paint_arrow(style, pixmap, state, shadow, &cr,
			                      widget, detail, t1, 1,
			                      rect.left + margin, rect.top + margin, rcx, rcy);
			break;
		case GTK_SLIDER:
			GTK().gtk_paint_slider(style, pixmap, state, shadow, &cr,
			                       widget, detail,
			                       rect.left + margin, rect.top + margin, rcx, rcy,
			                       t1);
			break;
		case GTK_ICON:
			GDK().gdk_draw_pixbuf(pixmap, NULL, icon, 0, 0, 0, 0, -1, -1, 0, 0, 0);
			break;
		case GTK_EXT:
			GTK().gtk_paint_extension(style, pixmap, state, shadow, &cr,
			                          widget, detail,
			                          rect.left + margin, rect.top + margin, rcx, rcy, t1);
			break;
		}
		GOBJ().g_object_unref(pixmap);
		m[i] = Crop(iw, rect.Inflated(margin));
		SAVEPNG(AsString((uintptr_t)widget) + "-" + detail + "-" + AsString(type) + "-" + AsString(i), m[i]);
		back = Black;
	}
	if(type == GTK_ICON)
		GOBJ().g_object_unref(icon);
	sLastImage = RecreateAlpha(m[0], m[1]);
	SAVEPNG(AsString((uintptr_t)widget) + "-" + detail + "-" + AsString(type), sLastImage);
	return sLastImage;
}

struct ChGtkI : Moveable<ChGtkI> {
	G_obj *widget;
	const char *detail;
	int    type;
};

Vector<ChGtkI>& ChGtkIs()
{
	static Vector<ChGtkI> x;
	return x;
}

G_obj *ChGtkLast()
{
	return ChGtkIs().Top().widget;
}

const char *ChGtkLastDetail()
{
	return ChGtkIs().Top().detail;
}

int ChGtkLastType()
{
	return ChGtkIs().Top().type;
}

void ChGtkNew(G_obj *widget, const char *detail, int type)
{
	if(GTK().gtk_widget_get_parent(widget) == NULL)
		Setup(widget);
	ChGtkI& q = ChGtkIs().Add();
	q.widget = widget;
	q.detail = detail;
	q.type = type;
}

void ChGtkNew(const char *detail, int type)
{
	ChGtkNew(ChGtkLast(), detail, type);
}

struct GtkElement {
	word gtki;
	byte state;
	byte shadow;
};

struct GtkImageMaker : ImageMaker {
	GtkElement e;
	ChGtkI     eg;
	Size       sz;

	virtual String Key() const {
		StringBuffer key;
		key.Cat((char *)&eg.widget, sizeof(eg.widget));
		key.Cat((char *)&eg.detail, sizeof(eg.detail));
		key.Cat((char *)&eg.type, sizeof(eg.type));
		key.Cat((char *)&e.gtki, sizeof(e.gtki));
		key.Cat((char *)&e.state, sizeof(e.state));
		key.Cat((char *)&e.shadow, sizeof(e.shadow));
		key.Cat((char *)&sz, sizeof(sz));
		return key;
	}
	virtual Image Make() const {
		return GetGTK(eg.widget, e.state, e.shadow & 0x7f, eg.detail, eg.type, sz.cx, sz.cy);
	}
};

Value GtkLookFn(Draw& w, const Rect& rect, const Value& v, int op)
{
	if(IsTypeRaw<GtkElement>(v)) {
		const GtkElement& e = ValueTo<GtkElement>(v);
		ChGtkI& eg = ChGtkIs()[e.gtki];
		switch(op) {
		case LOOK_MARGINS:
			return Rect(0, 0, 0, 0);
		case LOOK_ISOPAQUE:
			return false;
		}
		if(op == LOOK_PAINT || op == LOOK_PAINTEDGE) {
			Rect r = rect.Inflated((eg.type >> 4) & 15);
			GtkImageMaker gm;
			gm.e = e;
			gm.eg = eg;
			gm.sz = r.Size();
			if(e.shadow & 0x80)
				gm.sz.cy--;
			Image m = MakeImagePaintOnly(gm);
			w.DrawImage(r.left, r.top, m);
			if(e.shadow & 0x80)
				w.DrawImage(r.left + 1, r.bottom - 1, m,
				            RectC(1, gm.sz.cy - 1, gm.sz.cx - 2, 1));
			return 1;
		}
	}
	return Null;
}

Value GtkMakeCh(int shadow, int state)
{
	GtkElement e;
	e.gtki = ChGtkIs().GetCount() - 1;
	e.shadow = shadow;
	e.state = state;
	return RawToValue(e);
}

void GtkCh(const char *chvar, int i, int shadow, int state)
{
	ChSet(chvar, i, GtkMakeCh(shadow, state));
}

void GtkCh(const char *chvar, int shadow = 0)
{
	GtkCh(chvar, CTRL_NORMAL, shadow, 0);
	GtkCh(chvar, CTRL_HOT, shadow, 2);
	GtkCh(chvar, CTRL_PRESSED, shadow, 1);
	GtkCh(chvar, CTRL_DISABLED, shadow, 4);
}

void GtkChButton(const char *chvar)
{
	GtkCh(chvar, CTRL_NORMAL, 2, 0);
	GtkCh(chvar, CTRL_HOT, 2, 2);
	GtkCh(chvar, CTRL_PRESSED, 1, 1);
	GtkCh(chvar, CTRL_DISABLED, 2, 4);
}

void GtkChSlider(const char *chvar)
{
	GtkCh(chvar, CTRL_NORMAL, 2, 0);
	GtkCh(chvar, CTRL_HOT, 2, 2);
	GtkCh(chvar, CTRL_PRESSED, 2, 2);
	GtkCh(chvar, CTRL_DISABLED, 2, 4);
}

void GtkChTrough(const char *chvar)
{
	GtkCh(chvar, CTRL_NORMAL, 1, 1);
	GtkCh(chvar, CTRL_HOT, 1, 1);
	GtkCh(chvar, CTRL_PRESSED, 1, 1);
	GtkCh(chvar, CTRL_DISABLED, 1, 4);
}

void GtkChWith(const char *chvar, int i, int shadow, int state, const Image& img, Color (*color)(int i))
{
	GtkElement e;
	e.gtki = ChGtkIs().GetCount() - 1;
	e.shadow = shadow;
	e.state = state;
	ChSet(chvar, i, ChLookWith(GtkMakeCh(shadow, state), img, color, i));
}

void GtkChButtonWith(const char *chvar, const Image& img)
{
	GtkChWith(chvar, CTRL_NORMAL, 2, 0, img, ButtonMonoColor);
	GtkChWith(chvar, CTRL_HOT, 2, 2, img, ButtonMonoColor);
	GtkChWith(chvar, CTRL_PRESSED, 1, 1, img, ButtonMonoColor);
	GtkChWith(chvar, CTRL_DISABLED, 2, 4, img, ButtonMonoColor);
}

void GtkChArrow(const char *chvar, const Image& img)
{
	GtkChWith(chvar, CTRL_NORMAL, 2, 0, img, ButtonMonoColor);
	GtkChWith(chvar, CTRL_HOT, 2, 2, img, ButtonMonoColor);
	GtkChWith(chvar, CTRL_PRESSED, 1, 1, img, ButtonMonoColor);
	GtkChWith(chvar, CTRL_DISABLED, 2, 4, img, ButtonMonoColor);
}

void GtkChSbtn(const char *chvar)
{
	GtkCh(chvar, CTRL_NORMAL, 2, 0);
	GtkCh(chvar, CTRL_HOT, 2, 2);
	GtkCh(chvar, CTRL_PRESSED, 2, 1);
	GtkCh(chvar, CTRL_DISABLED, 2, 4);
}

int  GtkInt(G_obj *widget, const char *id)
{
	int x = 0;
	GTK().gtk_widget_style_get(widget, id, &x, NULL);
	return x;
}

int  GtkInt(const char *id)
{
	return GtkInt(ChGtkLast(), id);
}

void GtkIml(int uii, G_obj *w, int shadow, int state, const char *detail, int type, int cx, int cy)
{
	CtrlsImg::Set(uii, GetGTK(w, state, shadow, detail, type, cx, cy));
}

void GtkIml(int uii, G_obj *w, int shadow, const char *detail, int type, int cx, int cy)
{
	GtkIml(uii + 0, w, shadow, 0, detail, type, cx, cy);
	GtkIml(uii + 1, w, shadow, 2, detail, type, cx, cy);
	GtkIml(uii + 2, w, shadow, 1, detail, type, cx, cy);
	GtkIml(uii + 3, w, shadow, 4, detail, type, cx, cy);
}

void ChGtkColor(const char *id, int ii, int i, G_obj *widget)
{
	G_color cc = ((G_color *)((byte *)GTK().gtk_widget_get_style(widget)
	                          + GTK__COLOR_OFFSET))[ii];
	ChSet(id, i, Color(cc.r >> 8, cc.g >> 8, cc.b >> 8));
}

void ChGtkColor(const char *id, int ii, int i)
{
	ChGtkColor(id, ii, i, ChGtkLast());
}

void ChGtkColor(const char *id, int ii)
{
	ChGtkColor("ButtonTextColor", ii + 0, 0);
	ChGtkColor("ButtonTextColor", ii + 2, 1);
	ChGtkColor("ButtonTextColor", ii + 1, 2);
	ChGtkColor("ButtonTextColor", ii + 4, 3);
}

Image GtkImage(const char *id, int size, int maxh = INT_MAX)
{
	if(maxh != INT_MAX)
		maxh = Ctrl::VertLayoutZoom(maxh);
	Image m = GetGTK(gtk__parent(), size, 0, id, GTK_ICON, 0, 0);
	if(!IsNull(m)) {
		Size sz = m.GetSize();
		if(sz.cy > maxh && sz.cy)
			m = Rescale(m, sz.cx * maxh / sz.cy, maxh);
	}
	return m;
}

void ChCtrlImg(int ii, const char *id, int size, int maxh = INT_MAX)
{
	Image m = GtkImage(id, size, maxh);
	if(!IsNull(m))
		CtrlImg::Set(ii, m);
}

Image GtkChImgLook(int shadow, int state, int kind)
{
	Image m = GetGTK(ChGtkLast(), state, shadow, ChGtkLastDetail(), ChGtkLastType(), 16, 16);
	int g = ImageMargin(m, 4, 10);
	if(kind == 1)
		return Crop(m, 0, g, 16 - g, 16 - 2 * g);
	if(kind == 2)
		return Crop(m, g, g, 16 - g, 16 - 2 * g);
	return m;
}

void GtkChImgWith(const char *chvar, int i, int shadow, int state, const Image& img,
                  Color (*color)(int i), int kind)
{
	Value m = GtkChImgLook(shadow, state, kind);
	ChSet(chvar, i, IsNull(img) ? m : ChLookWith(m, img, color, i));
}

void GtkChImgWith(const char *chvar, const Image& img, int kind)
{
	GtkChImgWith(chvar, CTRL_HOT, 2, 2, img, ButtonMonoColor, kind);
	GtkChImgWith(chvar, CTRL_PRESSED, 1, 1, img, ButtonMonoColor, kind);
	GtkChImgWith(chvar, CTRL_DISABLED, 2, 4, img, ButtonMonoColor, kind);
	GtkChImgWith(chvar, CTRL_NORMAL, 2, 0, img, ButtonMonoColor, kind);
}

bool IsEmptyImage(const Image& m)
{
	const RGBA *s = ~m;
	const RGBA *e = ~m + m.GetLength();
	while(s < e) {
		if(s->a)
			return false;
		s++;
	}
	return true;
}

void ChHostSkin()
{
	if(!GTK() || !GDK() || !GOBJ())
		return;
	static struct { const char *id; int ii; int i; } col[] = {
		{ "SColorPaper", 6*5 + 0 },
		{ "SColorFace", 1*5 + 0 },
		{ "SColorText", 5*5 + 0 },
		{ "SColorHighlight", 6*5 + 3 },
		{ "SColorHighlightText", 5*5 + 3 },
		{ "SColorMenu", 6*5 + 0 },
		{ "SColorMenuText", 5*5 + 0 },
		{ "SColorDisabled", 5*5 + 4 },
		{ "SColorLight", 2*5 + 0 },
		{ "SColorShadow", 3*5 + 0 },
	};
	for(int i = 0; i < __countof(col); i++)
		ChGtkColor(col[i].id, col[i].ii, col[i].i, gtk__parent());

	ColoredOverride(CtrlsImg::Iml(), CtrlsImg::Iml());

	Color fc = Blend(SColorHighlight, SColorShadow);

	ChGtkIs().Clear();
	G_obj *w = Setup(GTK().gtk_radio_button_new(NULL));
	G_toggle *g = (G_toggle *)w;
	int is = GtkInt(w, "indicator-size") + 2;
	g->active = false;
	g->inconsistent = false;
	GtkIml(CtrlsImg::I_S0, w, 2, "radiobutton", GTK_OPTION|GTK_MARGIN1, is, is);
	g->active = true;
	GtkIml(CtrlsImg::I_S1, w, 1, "radiobutton", GTK_OPTION|GTK_MARGIN1, is, is);
	GTK().gtk_widget_destroy(w);

	w = Setup(GTK().gtk_check_button_new());
	g = (G_toggle *)w;
	g->active = false;
	g->inconsistent = false;
	GtkIml(CtrlsImg::I_O0, w, 2, "checkbutton", GTK_CHECK|GTK_MARGIN1, is, is);
	g->active = true;
	GtkIml(CtrlsImg::I_O1, w, 1, "checkbutton", GTK_CHECK|GTK_MARGIN1, is, is);
	g->active = false;
	g->inconsistent = true;
	GtkIml(CtrlsImg::I_O2, w, 3, "checkbutton", GTK_CHECK|GTK_MARGIN1, is, is);
	GTK().gtk_widget_destroy(w);

	ChSet("ButtonOverPaint", 3);
	static G_obj *button = GTK().gtk_button_new();
	ChGtkNew(button, "button", GTK_BOX|GTK_MARGIN3);
	GtkChButton("ButtonLook");
	GtkChButton("ToolButtonLook");
	ChSet("ToolButtonLook", CTRL_NORMAL, Null);
	ChSet("ToolButtonLook", CTRL_DISABLED, Null);
	GtkCh("ToolButtonLook", 4, 1, 1);
	GtkCh("ToolButtonLook", 5, 1, 1);

	ChGtkColor("ButtonTextColor", 0 * 5);
	ChSet("ButtonPressOffsetFlag", GtkInt("child-displacement-x"));
	ChSet("ButtonPressOffsetFlagY", GtkInt("child-displacement-y"));

	static G_obj *def_button;
	if(!def_button) {
		def_button = GTK().gtk_button_new();
		Setup(def_button);
		GTK().gtk_widget_set(def_button, "can-default", true, NULL);
		GTK().gtk_window_set_default(gtk__parent(), def_button);
		ChGtkNew(def_button, "button", GTK_BOX|GTK_MARGIN3);
	}
	GtkChButton("OkButtonLook");

	G_obj *adj = GTK().gtk_adjustment_new(250, 0, 1000, 1, 1, 500);

	ChSet("ScrollBarTrough", 1);
	static G_obj *vscrollbar = GTK().gtk_vscrollbar_new(adj);
	ChGtkNew(vscrollbar, "slider", GTK_SLIDER|GTK_VAL1);
	GtkChSlider("ScrollBarVertThumb");
	Image m = GetGTK(ChGtkLast(), 0, 0, "slider", GTK_SLIDER|GTK_VAL1, 16, 32);
	ChSet("ScrollBarThumbMin", GtkInt("min-slider-length"));
	int sw, as;
	ChSet("ScrollBarSize", sw = GtkInt("slider-width"));
	ChSet("ScrollBarArrowSize", as = GtkInt("stepper-size"));
	ChGtkNew("trough", GTK_BOX);
	GtkChTrough("ScrollBarVertUpper");
	GtkChTrough("ScrollBarVertLower");
	is = min(sw, as) / 2;
	bool atp = IsEmptyImage(GetGTK(ChGtkLast(), 2, 2, "vscrollbar", GTK_BOX|GTK_TOP, 16, 16));
	if(atp) {
		ChGtkNew("vscrollbar", GTK_ARROW);
		GtkCh("ScrollBarUp", 1);
		ChGtkNew("vscrollbar", GTK_ARROW|GTK_VAL1);
		GtkCh("ScrollBarDown", 1);

		static G_obj *btn = GTK().gtk_button_new();
		ChGtkNew(btn, "button", GTK_BOX);

		GtkChButton("ScrollButtonLook");

		GtkChButton("EdgeButtonLook");
		GtkChButton("LeftEdgeButtonLook");

		GtkChButtonWith("DropBoxBtn", CtrlsImg::DA());
		GtkChButtonWith("DropBoxSquaredBtn", CtrlsImg::DA());

		GtkChButtonWith("SpinUpLook", CtrlsImg::SpU());
		GtkChButtonWith("SpinDownLook", CtrlsImg::SpD());
	}
	else {
		GtkIml(CtrlsImg::I_UA, ChGtkLast(), 0, 0, "vscrollbar", GTK_ARROW, is, is);
		GtkIml(CtrlsImg::I_DA, ChGtkLast(), 0, 0, "vscrollbar", GTK_ARROW|GTK_VAL1, is, is);

		ChGtkNew("vscrollbar", GTK_BOX|GTK_TOP);
		GtkChArrow("ScrollBarUp", CtrlsImg::UA());
		ChGtkNew("vscrollbar", GTK_BOX|GTK_BOTTOM);
		GtkChArrow("ScrollBarDown", CtrlsImg::DA());
		ChGtkNew("vscrollbar", GTK_BOX|GTK_VCENTER);

		GtkChSbtn("ScrollButtonLook");

		GtkChImgWith("EdgeButtonLook", Null, 1);
		GtkChImgWith("LeftEdgeButtonLook", Null, 2);

		GtkChImgWith("DropBoxBtn", CtrlsImg::DA(), 1);
		GtkChImgWith("DropBoxSquaredBtn", CtrlsImg::DA(), 1);

		GtkChImgWith("SpinUpLook", CtrlsImg::SpU(), 1);
		GtkChImgWith("SpinDownLook", CtrlsImg::SpD(), 1);
	}


	int d = Diff(fc, SColorPaper());
	for(int x = 0; x < 4; x++)
		for(int y = 0; y < 4; y++) {
			RGBA c = sLastImage[x][y];
			if(c.a == 255) {
				int dd = Diff(c, SColorPaper());
				if(dd > d) {
					fc = c;
					d = dd;
				}
			}
		}
	ChSet("FieldFrameColor", fc);

	static G_obj *hscrollbar = GTK().gtk_hscrollbar_new(adj);
	ChGtkNew(hscrollbar, "slider", GTK_SLIDER);
	GtkChSlider("ScrollBarHorzThumb");
	ChGtkNew("trough", GTK_BOX);
	GtkChTrough("ScrollBarHorzUpper");
	GtkChTrough("ScrollBarHorzLower");
	if(atp) {
		ChGtkNew("hscrollbar", GTK_ARROW|GTK_VAL2);
		GtkCh("ScrollBarLeft", 1);
		ChGtkNew("hscrollbar", GTK_ARROW|GTK_VAL3);
		GtkCh("ScrollBarRight", 1);
	}
	else {
		GtkIml(CtrlsImg::I_LA, ChGtkLast(), 0, 0, "hscrollbar", GTK_ARROW|GTK_VAL2, is, is);
		GtkIml(CtrlsImg::I_RA, ChGtkLast(), 0, 0, "hscrollbar", GTK_ARROW|GTK_VAL3, is, is);
		ChGtkNew("hscrollbar", GTK_BOX|GTK_LEFT);
		GtkChArrow("ScrollBarLeft", CtrlsImg::LA());
		ChGtkNew("hscrollbar", GTK_BOX|GTK_RIGHT);
		GtkChArrow("ScrollBarRight", CtrlsImg::RA());
		ChGtkNew("hscrollbar", GTK_BOX|GTK_VCENTER);
	}

	GTK().gtk_object_sink(adj);

	adj = GTK().gtk_adjustment_new(0, 0, 1000, 1, 1, 500);
	w = GTK().gtk_vscrollbar_new(NULL);
	Setup(w);
	ChSet("ScrollBarOverThumb", m != GetGTK(w, 0, 0, "slider", GTK_SLIDER|GTK_VAL1, 16, 32));
	GTK().gtk_widget_destroy(w);
	GTK().gtk_object_sink(adj);

	static G_obj *menu_item = GTK().gtk_menu_item_new();
	ChGtkNew(menu_item, "menuitem", GTK_BOX);
	GtkCh("MenuItemLook", 0, 2, 2);

	static G_obj *tabctrl = GTK().gtk_notebook_new();
	ChGtkNew(tabctrl, "tab", GTK_EXT|GTK_VAL3);
	ImageBuffer ib(9, 9);
	ImageBuffer ib1(9, 9);
	m = GetGTK(tabctrl, 0, 2, "tab", GTK_EXT|GTK_VAL3, 12, 12);
	for(int i = 0; i < 5; i++) {
		RGBA *t = ~ib + i * 9 + i;
		RGBA *t1 = ~ib1 + i * 9 + i;
		for(int n = 9 - 2 * i; n--; t += 9, t1 += 9) {
			Fill(t, m[10][i], 9 - 2 * i);
			Fill(t1, m[10][11 - i], 9 - 2 * i);
		}
	}
	{
		RGBA *t = ~ib + 9 + 8;
		RGBA *t1 = ~ib1 + 9 + 8;
		for(int i = 1; i < 9; i++) {
			memcpy(t, t1, i * sizeof(RGBA));
			t += 9 - 1;
			t1 += 9 - 1;
		}
	}
	ib.SetHotSpot(Point(4, 4));
	ChSet("TabCtrlLook", 16, Image(ib));
	for(int i = 0; i < 4; i++) {
		GtkCh("TabCtrlLook", i * 4 + 0, 2, 1);
		GtkCh("TabCtrlLook", i * 4 + 1, 2, 1);
		GtkCh("TabCtrlLook", i * 4 + 2, 0x82, 0);
		GtkCh("TabCtrlLook", i * 4 + 3, 2, 1);
	}
	Vector<int> tm;
	tm << StdFont().Info().GetHeight() + 8  // TABHEIGHT
		<< 0 // MARGIN
		<< 0 << 0 << 0 << 2 // SEL*
		<< 6 << 6 << 6 << 6 // *EDGE
		<< 2
	;
	for(int i = 0; i < tm.GetCount(); i++)
		ChSet("TabCtrlMetric", i, tm[i]);

	ib.Create(3, 3);
	Fill(~ib, fc, ib.GetLength());
	ib[1][1] = Color(Null);
	ib.SetHotSpot(Point(1, 1));
	CtrlsImg::Set(CtrlsImg::I_EFE, ib);
	ib.Create(5, 5);
	Fill(~ib, fc, ib.GetLength());
	for(int x = 1; x < 4; x++)
		Fill(ib[x] + 1, SColorPaper(), 3);
	ib[2][2] = Color(Null);
	ib.SetHotSpot(Point(2, 2));
	CtrlsImg::Set(CtrlsImg::I_VE, ib);

	Color c = SColorPaper();
	for(int i = 0; i < 3; i++)
		ChSet("ButtonMonoColor", i, c.GetR() + c.GetG() + c.GetB() < 3 * 128 ? White() : Black());
	ChSet("ButtonMonoColor", 3, Gray());

	ChCtrlImg(CtrlImg::I_information, "gtk-dialog-info", 6);
	ChCtrlImg(CtrlImg::I_question, "gtk-dialog-question", 6);
	ChCtrlImg(CtrlImg::I_exclamation, "gtk-dialog-warning", 6);

	char *font_name = "";
	GOBJ().g_object_get(GTK().gtk_settings_get_default(), "gtk-font-name", &font_name, NULL);
	int xdpi = 96 * 1024;
	GOBJ().g_object_get(GTK().gtk_settings_get_default(), "gtk-xft-dpi", &xdpi, NULL);

	int fontname = Font::ARIAL;
	int fontheight = 13;
	bool bold = false;
	bool italic = false;

	const char *q = strrchr(font_name, ' ');
	if(q) {
		int h = atoi(q);
		if(h)
			fontheight = h;
		String face(font_name, q);
		fontname = Font::FindFaceNameIndex(face);

		if(fontname == 0) {
			for(;;) {
				const char *q = strrchr(face, ' ');
				if(!q) break;
				const char *s = q + 1;
				if(stricmp(s, "Bold") == 0 || stricmp(s, "Heavy") == 0)
					bold = true;
				else
				if(stricmp(s, "Italic") == 0 || stricmp(s, "Oblique") == 0)
					italic = true;
				else
				if(stricmp(s, "Regular") == 0 || stricmp(s, "Light") || stricmp(s, "Medium"))
					;
				else
					continue;
				face = String(~face, q);
			}
			fontname = Font::FindFaceNameIndex(face);
			if(fontname == 0)
				if(ToUpper(face[0]) == 'M')
					fontname = Font::COURIER;
				else
				if(ToUpper(face[0]) == 'S' && ToUpper(face[1]) == 'e')
					fontname = Font::ROMAN;
				else
					fontname = Font::ARIAL;
		}
	}

	Draw::SetStdFont(Font(fontname, (fontheight * xdpi + 512*72) / (1024*72))
	                 .Bold(bold).Italic(italic));

	static struct {
		const char *ch;
		const char *gtk;
	} bimg[] = {
		{ "OkButtonImage", "gtk-ok" },
		{ "CancelButtonImage", "gtk-cancel" },
		{ "ExitButtonImage", "gtk-quit" },
		{ "YesButtonImage", "gtk-yes" },
		{ "NoButtonImage", "gtk-no" },
		{ "AbortButtonImage", "gtk-stop" },
		{ "RetryButtonImage", "gtk-refresh" },
	};

	for(int i = 0; i < __countof(bimg); i++)
		ChSet(bimg[i].ch, GtkImage(bimg[i].gtk, 4, 16));

	ChLookFn(GtkLookFn);
}

#endif

END_UPP_NAMESPACE
