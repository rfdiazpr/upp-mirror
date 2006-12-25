class StaticText : public Ctrl, public LabelBase {
public:
	virtual void   Paint(Draw& w);
	virtual Size   GetMinSize() const;
	virtual void   LabelUpdate();

public:
	StaticText& SetFont(Font font)                      { LabelBase::SetFont(font); return *this; }
	StaticText& SetInk(Color color)                     { LabelBase::SetInk(color); return *this; }
	StaticText& SetAlign(int align)                     { LabelBase::SetAlign(align); return *this; }
	StaticText& SetImage(const Image& img, int spc = 0) { LabelBase::SetImage(img, spc); return *this; }
	StaticText& SetText(const char *text)               { LabelBase::SetText(text); return *this; }

	StaticText& operator=(const char *s)                { SetText(s); return *this; }

	StaticText();
};

class Label : public StaticText {
public:
	virtual bool   HotKey(dword key);
	virtual dword  GetAccessKeys() const;
	virtual void   AssignAccessKeys(dword used);

private:
	bool   noac;

public:
	Label& SetText(const char *text);
	Label& SetLabel(const char *lbl);

	Label& operator=(const char *s)                     { SetText(s); return *this; }

	Label();
	virtual ~Label();
};

class LabelBox : public Label {
public:
	virtual void   Paint(Draw& w);
	virtual void   AssignAccessKeys(dword used);
	virtual Rect   GetVoidRect();

public:
	LabelBox();
	virtual ~LabelBox();
	LabelBox& operator=(const char *s)   { SetText(s); return *this; }
};

struct ParentCtrl : public Ctrl {
	virtual Rect   GetVoidRect();

	ParentCtrl();
};

class StaticRect : public Ctrl {
public:
	virtual void   Paint(Draw& w);

protected:
	class Color    color;

public:
	StaticRect& Color(class Color _color);

	StaticRect();
	virtual ~StaticRect();
};

class ImageCtrl : public Ctrl {
public:
	virtual void   Paint(Draw& w);
	virtual Size   GetStdSize() const;
	virtual Size   GetMinSize() const;

protected:
	Image   img;

public:
	ImageCtrl&   SetImage(const Image& _img)          { img = _img; Refresh(); return *this; }

	ImageCtrl()                                       { Transparent(); NoWantFocus(); }
};

// BWC
typedef ImageCtrl Icon;

class Picture : public Ctrl {
public:
	virtual void   Paint(Draw& w);

protected:
	Drawing picture;
	Color   background;
	bool    ratio;

public:
	Picture& Background(Color color)             { background = color; Refresh(); return *this; }
	Picture& KeepRatio(bool keep = true)         { ratio = keep; Refresh(); return *this; }
	Picture& NoKeepRatio()                       { return KeepRatio(false); }
	Picture& Set(const Drawing& _picture)        { picture = _picture; Refresh(); return *this; }

	Picture& operator=(const Drawing& _picture)  { return Set(_picture); }

	Picture();
};

class SeparatorCtrl : public Ctrl {
	int margin;
	int size;

public:
	virtual Size GetMinSize() const;
	virtual void Paint(Draw& w);

	SeparatorCtrl& Margin(int w);
	SeparatorCtrl& SetSize(int w);

	SeparatorCtrl();
};
