enum {
	CTRL_NORMAL, CTRL_HOT, CTRL_PRESSED, CTRL_DISABLED
};

Vector<Value> CtrlsImgLook(int i);

String DeAmp(const char *s);

Size GetSmartTextSize(Draw& w, const char *text, Font font = StdFont());
int  GetSmartTextHeight(Draw& w, const char *s, int cx, Font font = StdFont());
void DrawSmartText(Draw& w, int x, int y, int cx, const char *text,
                   Font font = StdFont(), Color ink = SBlack, int accesskey = 0);

byte  ExtractAccessKey(const char *s, String& label);
bool  CompareAccessKey(byte accesskey, dword key);
byte  ChooseAccessKey(const char *s, dword used);

struct DrawLabel {
	bool      push;
	bool      focus;
	bool      disabled;

	PaintRect paintrect;
	Image     limg;
	Color     lcolor;
	int       lspc;
	String    text;
	Font      font;
	Color     ink;
	Image     rimg;
	Color     rcolor;
	int       rspc;

	int       align, valign;

	int       accesskey;

	Size      GetSize() const;
	Size      Paint(Draw& w, const Rect& r, bool visibleaccesskey = true) const;
	Size      Paint(Draw& w, int x, int y, int cx, int cy, bool visibleaccesskey = true) const;

	DrawLabel();
};

class LabelBase {
protected:
	virtual void  LabelUpdate();

	DrawLabel   lbl;

public:
	LabelBase&  SetLeftImage(const Image& bmp1, int spc = 0);
	LabelBase&  SetPaintRect(const PaintRect& pr);
	LabelBase&  SetText(const char *text);
	LabelBase&  SetFont(Font font);
	LabelBase&  SetInk(Color color);
	LabelBase&  SetRightImage(const Image& bmp2, int spc = 0);
	LabelBase&  SetAlign(int align);
	LabelBase&  SetVAlign(int align);
	LabelBase&  SetImage(const Image& bmp, int spc = 0)      { SetLeftImage(bmp, spc); return *this; }

	int         GetAlign() const                             { return lbl.align; }
	int         GetVAlign() const                            { return lbl.valign; }
	PaintRect   GetPaintRect() const                         { return lbl.paintrect; }
	String      GetText() const                              { return lbl.text; }
	Font        GetFont() const                              { return lbl.font; }
	Color       GetInk() const                               { return lbl.ink; }

	Size        PaintLabel(Draw& w, const Rect& r,
	                       bool disabled = false, bool push = false, bool focus = false, bool vak = true);
	Size        PaintLabel(Draw& w, int x, int y, int cx, int cy,
	                       bool disabled = false, bool push = false, bool focus = false, bool vak = true);
	Size        GetLabelSize() const;

	virtual ~LabelBase();
};
