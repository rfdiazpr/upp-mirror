class TabCtrl : public Ctrl, public CtrlFrame {
public:
	virtual bool   Accept();
	virtual void   Layout();
	virtual bool   HotKey(dword key);
	virtual bool   Key(dword key, int repcnt);
	virtual void   FrameAdd(Ctrl& ctrl);
	virtual void   FrameAddSize(Size& sz);
	virtual void   FrameRemove();
	virtual void   FrameLayout(Rect& rc);
	virtual void   FramePaint(Draw& draw, const Rect& rc);
	virtual Image  FrameMouseEvent(int event, Point p, int zdelta, dword keyflags);
	virtual void   Paint(Draw& w);
	virtual void   CancelMode();

	virtual Vector<Rect> GetTransparentFrameRects();
	virtual Vector<Rect> GetOpaqueFrameRects();
	virtual Vector<Rect> GetTransparentViewRects();

public:
	class Item
	{
		friend class TabCtrl;

	public:
		Item&          Text(const String& _text);
		Item&          Help(const String& _help)     { help = _help; return *this; }
		Item&          Picture(PaintRect d);
		Item&          Image(const class Image& _im)     { return Picture(PaintRect(ImageDisplay(), _im)); }
		Item&          Control(Ctrl *_ctrl);
		Item&          Slave(Ctrl *_slave);
		Item&          Key(dword _key)               { key = _key; return *this; }

		Item&          Enable(bool _en = true);
		Item&          Disable()                     { return Enable(false); }
		bool           IsEnabled() const             { return enabled; }

	protected:
		Size           GetSize(Draw& draw) const;

		Item(TabCtrl& owner);

	protected:
		TabCtrl&       owner;
		Rect           rect;
		Size           size;
		bool           enabled;
		dword          key;
		String         text; // IsVoid() -> value is printed here
		String         help;
		PaintRect      pict;
		Ctrl          *ctrl;
		Ctrl          *slave;
		Ptr<Ctrl>      activefocus;
	};

	friend class Item;

protected:
	Size           GetFrameOffset() const;
	void           RefreshItem(int item);
	virtual void   DrawItem(Draw& draw, int item);
	virtual void   DrawItemFrame(Draw& draw, Rect& rc, bool active, bool light);
	virtual void   DrawRaggedEdge(Draw& draw, Point pos, int height, bool right = true);
	void           DrawRaggedEdges(Draw& draw, const Rect& rc, int delta = 2);
	bool           DrawItemCheck(Draw& draw, const Rect& clip, int item);

private:
	Button         prev, next;
	Rect           out_frame, in_frame;
	Size           frame_size;
	Size           old_frame_size;
	Font           font;
	Array<Item>    items;
	int            hl;
	int            sidegap, vertgap;
	bool           accept_current;
	int            find_max_x; // maximum x coordinate for Find to work
	int            top;      // active item
	int            scroll;
	int            scroll_limit;
	int            ragged_left;
	int            ragged_right;

	void           ScrollInto(int _top);
	void           SetScroll(int _scroll);
	int            GetScroll() const            { return scroll; }

	int            Find(Point pt) const;

	void           RefreshTabLayout();

public:
	Callback       WhenSet;
	Callback1<Bar&> WhenBar;

	void           Clear();
	bool           IsEmpty() const                             { return items.IsEmpty(); }
	int            GetCount() const                            { return items.GetCount(); }

	Item&          Add();
	Item&          Add(const char *text)                       { return Add().Text(text); }
	Item&          Add(const ::Image& image, const char *text) { return Add(text).Image(image); }
	Item&          Add(Ctrl& slave, const char *text)          { return Add(text).Slave(&slave); }

	const Item&    GetItem(int i) const                        { return items[i]; }
	Item&          GetItem(int i)                              { return items[i]; }

	void           SetData(const Value& v)                     { Set(v); }
	Value          GetData() const                             { return Get(); }

	void           SetFont(Font f)                             { font = f; RefreshTabLayout(); }
	Font           GetFont() const                             { return font; }

	int            Get() const                                 { return top; }
	void           Set(int _top, bool focus = false);

	void           GoPrev();
	void           GoNext();

	TabCtrl&       AcceptCurrent(bool ac = true)         { accept_current = ac; return *this; }
	void           AcceptAll()                           { AcceptCurrent(false); }

	TabCtrl&       SideGap(int gap)                      { sidegap = gap + 2; RefreshTabLayout(); return *this; }
	TabCtrl&       VertGap(int gap)                      { vertgap = gap + 2; RefreshTabLayout(); return *this; }

	static Color   GetTabColor();

	typedef TabCtrl CLASSNAME;

	TabCtrl();
	~TabCtrl();
};

class TabDlg : public TopWindow {
	void    Rearrange();

	bool    binit;
	Size    sz;

	void           PlaceButton(Button& b, int& r);
	TabCtrl::Item& TabDlg::Add0(Ctrl& tab, const char *text);
	TabDlg&        AButton(Button& b);

public:
	TabCtrl  tabctrl;
	Button   ok;
	Button   cancel;
	Button   exit;
	Button   apply;

	template <class T>
	TabCtrl::Item& Add(T& tab, const char *text)                   { CtrlLayout(tab); return Add0(tab, text); }
	template <class T>
	TabCtrl::Item& Add(T& tab, const Image& img, const char *name) { return Add(tab, name).Image(img); }
	template <class T>
	TabDlg&  operator()(T& tab, const char *text)                  { Add(tab, text); return *this; }
	template <class T>
	TabDlg&  operator()(T& tab, const Image& img, const char *txt) { Add(tab, img, txt); return *this; }

	TabDlg&  OK()                                                  { return AButton(ok); }
	TabDlg&  Cancel()                                              { return AButton(cancel); }
	TabDlg&  OKCancel()                                            { return OK().Cancel(); }
	TabDlg&  Exit()                                                { return AButton(exit); }
	TabDlg&  Apply()                                               { return AButton(apply); }

	TabDlg();
};
