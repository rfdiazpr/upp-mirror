class Slider : public Ctrl {
	virtual void  CancelMode();
	virtual void  Paint(Draw& draw);
	virtual void  LeftDown(Point p, dword keyflags);
	virtual void  LeftRepeat(Point p, dword keyflags);
	virtual void  MouseMove(Point p, dword keyflags);
	virtual void  MouseEnter(Point p, dword);
	virtual void  MouseLeave();
	virtual void  LeftUp(Point, dword);
	virtual void  Layout();

protected:
	int     thumbsize;
	int     thumbpos;
	int     delta;
	bool    horz:1;
	bool    track:1;
	bool    jump:1;
	bool    edgestyle:1;
	byte    push:2;
	bool    light;

	Rect    GetThumbRect() const;
	void    Drag(Point p);
	int     GetHV(int h, int v) const;
	int     GetHV(Point p) const                           { return GetHV(p.x, p.y); }
	int     GetHV(Size s) const                            { return GetHV(s.cx, s.cy); }
	void    Bounds();

public:
	bool    IsHorz() const                                 { return horz; }
	bool    IsVert() const                                 { return !horz; }
	bool    Set(int thumbpos, int thumbsize);
	bool    Set(int _thumbpos)                             { return Set(_thumbpos, thumbsize); }
	int     GetPos() const                                 { return thumbpos; }
	int     GetThumbSize() const                           { return thumbsize; }
	int     GetRange() const;
//	int     GetSlideSize() const;

	Slider& Horz(bool b = true)                            { horz = b; return *this; }
	Slider& Vert()                                         { return Horz(false); }
	Slider& Track(bool b = true)                           { track = b; return *this; }
	Slider& NoTrack()                                      { track = false; return *this; }
	Slider& EdgeStyle(bool b = true)                       { edgestyle = b; return *this; }
	Slider& NoEdgeStyle()                                  { return EdgeStyle(false); }

	Slider& Jump(bool b = true)                            { jump = b; return *this; }

	Callback WhenPrev;
	Callback WhenNext;
	Callback WhenLeftClick;

	operator int()                                         { return thumbpos; }

	Slider();
};

class ScrollBar : public FrameCtrl<Ctrl> {
public:
	virtual void Layout();
	virtual Size GetStdSize() const;

	virtual void FrameLayout(Rect& r);
	virtual void FrameAddSize(Size& sz);

protected:
	Button  prev, next;
	Slider  slider;
	int     pagepos;
	int     pagesize;
	int     totalsize;
	int     linesize;
	int     minthumb;
	bool    horz:1;
	bool    autohide:1;
	bool    autodisable:1;

	void    Position();
	void    Uset(int a);

public:
	Callback WhenScroll;
	Callback WhenVisibility;
	Callback WhenLeftClick;

	bool    IsHorz() const                  { return slider.IsHorz(); }
	bool    IsVert() const                  { return slider.IsVert(); }

	void    Set(int pagepos, int pagesize, int totalsize);

	bool    Set(int pagepos);
	void    SetPage(int pagesize);
	void    SetTotal(int totalsize);

	bool    ScrollInto(int pos, int linesize);
	bool    ScrollInto(int pos)             { return ScrollInto(pos, linesize); }

	bool    VertKey(dword key, bool homeend = true);
	bool    HorzKey(dword key);

	void    PrevLine();
	void    NextLine();
	void    PrevPage();
	void    NextPage();
	void    Begin();
	void    End();

	void    Wheel(int zdelta, int lines = 3);

	Size    GetViewSize() const;
	Size    GetReducedViewSize() const;

	int     Get() const                     { return pagepos; }
	int     GetPage() const                 { return pagesize; }
	int     GetTotal() const                { return totalsize; }
	int     GetLine() const                 { return linesize; }

	ScrollBar& Horz(bool b = true)          { slider.Horz(b); return *this; }
	ScrollBar& Vert()                       { return Horz(false); }

	ScrollBar& SetLine(int _line)           { linesize = _line; return *this; }

	ScrollBar& Track(bool b = true)         { slider.Track(b); return *this; }
	ScrollBar& NoTrack()                    { slider.NoTrack(); return *this; }
	ScrollBar& Jump(bool b = true)          { slider.Jump(b); return *this; }
	ScrollBar& NoJump(bool b = true)        { return Jump(false); }
	ScrollBar& AutoHide(bool b = true);
	ScrollBar& NoAutoHide()                 { return AutoHide(false); }
	bool       IsAutoHide() const           { return autohide; }
	ScrollBar& AutoDisable(bool b = true);
	ScrollBar& NoAutoDisable()              { return AutoDisable(false); }
	ScrollBar& MinThumb(int sz)             { minthumb = sz; }

	operator int() const                    { return pagepos; }
	int operator=(int pagepos)              { Set(pagepos); return pagepos; }

	static int GetStdBox()                  { return 17; }

	ScrollBar();
	virtual ~ScrollBar();
};

class VScrollBar : public ScrollBar {
public:
	int operator=(int pagepos)              { Set(pagepos); return pagepos; }
};

class HScrollBar : public ScrollBar {
public:
	int operator=(int pagepos)              { Set(pagepos); return pagepos; }

	HScrollBar() { Horz(); }
};

class SizeGrip : public FrameRight<Ctrl> {
public:
	virtual void  Paint(Draw& w);
	virtual void  LeftDown(Point p, dword);
	virtual Image CursorImage(Point p, dword);

public:
	SizeGrip();
	virtual ~SizeGrip();
};

class ScrollBars : public CtrlFrame {
public:
	virtual void FrameLayout(Rect& r);
	virtual void FrameAddSize(Size& sz);
	virtual void FramePaint(Draw& w, const Rect& r);
	virtual void FrameAdd(Ctrl& ctrl);
	virtual void FrameRemove();

protected:
	Ctrl      *box;
	StaticRect the_box;
	SizeGrip   grip;

	void    Scroll();

public:
	HScrollBar x;
	VScrollBar y;

	Callback   WhenScroll;
	Callback   WhenLeftClick;

	void    Set(Point pos, Size page, Size total);
	bool    Set(Point pos);
	bool    Set(int x, int y);
	void    SetPage(Size page);
	void    SetPage(int cx, int cy);
	void    SetTotal(Size total);
	void    SetTotal(int cx, int cy);

	void    SetX(int pos, int page, int total)       { x.Set(pos, page, total); }
	bool    SetX(int _x)                             { return x.Set(_x); }
	void    SetPageX(int cx)                         { x.SetPage(cx); }
	void    SetTotalX(int cx)                        { x.SetTotal(cx); }

	void    SetY(int pos, int page, int total)       { y.Set(pos, page, total); }
	bool    SetY(int _y)                             { return y.Set(_y); }
	void    SetPageY(int cy)                         { y.SetPage(cy); }
	void    SetTotalY(int cy)                        { y.SetTotal(cy); }

	bool    ScrollInto(Point pos, Size linesize);
	bool    ScrollInto(const Rect& r)                { return ScrollInto(r.TopLeft(), r.Size()); }
	bool    ScrollInto(Point pos);

	bool    ScrollIntoX(int pos, int linesize)       { return x.ScrollInto(pos, linesize); }
	bool    ScrollIntoX(int pos)                     { return x.ScrollInto(pos); }
	bool    ScrollIntoY(int pos, int linesize)       { return y.ScrollInto(pos, linesize); }
	bool    ScrollIntoY(int pos)                     { return y.ScrollInto(pos); }

	bool    Key(dword key);

	void    LineUp()                                 { y.PrevLine(); }
	void    LineDown()                               { y.NextLine(); }
	void    PageUp()                                 { y.PrevPage(); }
	void    PageDown()                               { y.NextPage(); }
	void    VertBegin()                              { y.Begin(); }
	void    VertEnd()                                { y.End(); }

	void    LineLeft()                               { x.PrevLine(); }
	void    LineRight()                              { x.NextLine(); }
	void    PageLeft()                               { x.PrevPage(); }
	void    PageRight()                              { x.NextPage(); }
	void    HorzBegin()                              { x.Begin(); }
	void    HorzEnd()                                { x.End(); }

	void    WheelX(int zdelta, int lines = 3)        { x.Wheel(zdelta, lines); }
	void    WheelY(int zdelta, int lines = 3)        { y.Wheel(zdelta, lines); }

	Size    GetViewSize() const;
	Size    GetReducedViewSize() const;

	Point   Get() const                              { return Point(x, y); }
	int     GetX() const                             { return x; }
	int     GetY() const                             { return y; }
	Size    GetPage() const                          { return Size(x.GetPage(), y.GetPage()); }
	Size    GetTotal() const                         { return Size(x.GetTotal(), y.GetTotal()); }
	Size    GetLine() const                          { return Size(x.GetLine(), y.GetLine()); }

	void    ShowX(bool show)                         { x.Show(show); }
	void    HideX()                                  { ShowX(false); }
	void    ShowY(bool show)                         { y.Show(show); }
	void    HideY()                                  { ShowY(false); }
	void    Show(bool show = true)                   { x.Show(show); y.Show(show); }
	void    Hide()                                   { Show(false); }

	ScrollBars& SetLine(int linex, int liney);
	ScrollBars& SetLine(Size line)                   { return SetLine(line.cx, line.cy); }
	ScrollBars& SetLine(int line)                    { return SetLine(line, line); }

	ScrollBars& Track(bool b = true);
	ScrollBars& NoTrack()                            { return Track(false); }
	ScrollBars& Jump(bool b = true);
	ScrollBars& NoJump(bool b = true)                { return Jump(false); }
	ScrollBars& AutoHide(bool b = true);
	ScrollBars& NoAutoHide()                         { return AutoHide(false); }
	ScrollBars& AutoDisable(bool b = true);
	ScrollBars& NoAutoDisable()                      { return AutoDisable(false); }

	ScrollBars& Box(Ctrl& box);
	ScrollBars& WithSizeGrip()                       { the_box.Add(grip); return *this; }

	operator Point() const                           { return Get(); }
	Point operator=(Point p)                         { Set(p); return p; }

	ScrollBars();
	virtual ~ScrollBars();
};

class Scroller {
	Point psb;

public:
	void Scroll(Ctrl& p, const Rect& rc, Point newpos, Size cellsize = Size(1, 1));
	void Scroll(Ctrl& p, const Rect& rc, int newpos, int linesize = 1);
	void Scroll(Ctrl& p, Point newpos);
	void Scroll(Ctrl& p, int newposy);

	void Set(Point pos)          { psb = pos; }
	void Set(int pos)            { psb = Point(0, pos); }
	void Clear()                 { psb = Null; }

	Scroller()                   { psb = Null; }
};
