class ColumnList : public Ctrl, private CtrlFrame {
public:
	virtual void  Serialize(Stream& s);
	virtual void  Paint(Draw& w);
	virtual void  Layout();
	virtual Image CursorImage(Point p, dword);
	virtual void  LeftDown(Point p, dword);
	virtual void  LeftDouble(Point p, dword);
	virtual void  RightDown(Point p, dword);
	virtual void  LeftUp(Point p, dword);
	virtual void  MouseMove(Point p, dword);
	virtual void  MouseLeave();
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);
	virtual bool  Key(dword key, int count);
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual void  CancelMode();

private:
	virtual void  FrameLayout(Rect& r);
	virtual void  FrameAddSize(Size& sz);
	virtual void  FramePaint(Draw& draw, const Rect& r);

private:
	int        ncl;
	int        cy;
	int        cursor;
	int        dx;
	int        ci;
	int        mpos;
	ScrollBar  sb;
	Scroller   scroller;
	CtrlFrame *frame;
	bool       clickkill;
	int        mi;

	struct Info : Ctrl {
		Value          value;
		Color          paper, ink;
		dword          style;
		const Display *display;
		ColumnList    *ctrl;
		Rect           slim;

		Point   Op(Point p);

		virtual void  Paint(Draw& w);
		virtual void  LeftDown(Point p, dword);
		virtual void  LeftDouble(Point p, dword);
		virtual void  RightDown(Point p, dword);
		virtual void  LeftUp(Point p, dword);
		virtual void  MouseWheel(Point p, int zdelta, dword keyflags);
		virtual void  MouseLeave();
		virtual void  MouseMove(Point p, dword);

		Info();
	} info;

	friend struct Info;

	const Display *display;

	struct Item {
		bool           sel;
		bool           canselect;
		Value          value;
		const Display *display;
	};

	Array<Item> item;
	bool        isselection;
	bool        multi;

	struct  ItemOrder;
	friend struct ItemOrder;

	void    SetSb();
	void    Scroll();
	int     GetDragColumn(int x) const;
	int     RoundedCy();
	void    Page(bool down);
	void    PointDown(Point p);
	void    ShiftSelect(int anchor, int cursor);
	void    RefreshItem(int i);
	void    RefreshCursor()                    { RefreshItem(cursor); }
	void    StdLeftClickPos(Point pos)         { WhenLeftClick(); }
	void    GetItemStyle(int i, Color& ink, Color& paper, dword& style);
	void    SyncInfoStyle();

	void    UpdateSelect();

public:
	Callback         WhenEnterItem;
	Callback         WhenKillCursor;
	Callback         WhenLeftClick;
	Callback1<Point> WhenLeftClickPos;
	Callback         WhenLeftDouble;
	Callback1<Bar&>  WhenBar;
	Callback         WhenSelection;

	int     GetColumnItems() const;
	int     GetColumnCx(int i = 0) const;
	int     GetPageItems() const;

	int     GetItem(Point p);
	Rect    GetItemRect(int i) const;

	int     GetCursor() const                          { return cursor; }
	void    SetCursor(int c);
	void    KillCursor();
	bool    IsCursor() const                           { return cursor >= 0; }

	int     GetSbPos() const                           { return sb; }
	void    SetSbPos(int y);

	void    SetFrame(CtrlFrame& frame);

	void         Clear();
	void         Add(const Value& val, bool canselect = true);
	void         Add(const Value& val, const Display& display, bool canselect = true);
	int          GetCount() const                     { return item.GetCount();; }
	const Value& Get(int i) const                     { return item[i].value; }
	const Value& operator[](int i) const              { return item[i].value; }

	void         Insert(int ii, const Value& val, bool canselect = true);
	void         Insert(int ii, const Value& val, const Display& display, bool canselect = true);
	void         Remove(int ii);

	bool         IsSelection() const                  { return isselection; }
	void         ClearSelection();
	void         SelectOne(int i, bool sel);
	bool         IsSelected(int i) const;

	void         Sort(const ValueOrder& order);

	ColumnList&  Columns(int _n)                      { ncl = _n; Refresh(); return *this; }
	ColumnList&  ItemHeight(int _cy)                  { cy = _cy; Refresh(); return *this; }
	int          GetItemHeight() const                { return cy; }
	ColumnList&  RoundSize(bool b = true);
	ColumnList&  NoRoundSize()                        { return RoundSize(false); }
	ColumnList&  ClickKill(bool b = true)             { clickkill = b; return *this; }
	ColumnList&  NoClickKill()                        { return ClickKill(false); }
	ColumnList&  SetDisplay(const Display& d)         { display = &d; return *this; }
	ColumnList&  Multi(bool b = true)                 { multi = b; return *this; }
	bool         IsMulti() const                      { return multi; }

	typedef ColumnList CLASSNAME;

	ColumnList();
	virtual ~ColumnList();
};
