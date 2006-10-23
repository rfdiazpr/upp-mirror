int   TabCtrlMetric(int i);
Value TabCtrlLook(int i);

class TabCtrl : public Ctrl {
public:
	virtual bool  Accept();
	virtual void  Paint(Draw& draw);
	virtual void  CancelMode();
	virtual void  MouseMove(Point p, dword keyflags);
	virtual void  LeftDown(Point p, dword keyflags);
	virtual void  MouseLeave();
	virtual bool  Key(dword key, int count);
	virtual bool  HotKey(dword key);
	virtual void  Layout();
	virtual Rect  GetOpaqueRect();
	virtual Value GetData() const;
	virtual void  SetData(const Value& data);

public:
	class Item {
		TabCtrl  *owner;

		int       x;
		Point     pictpos;
		Point     textpos;
		int       cx;

		String    text;
		PaintRect pict;
		Ctrl     *ctrl;
		bool      enabled;
		Ctrl     *slave;
		dword     key;

		friend class TabCtrl;

		void Layout(int x, int y, int cy);
		void Paint(Draw& w);
		int  Right() { return x + cx; }

	public:
		Item&          Text(const String& _text);
		Item&          Picture(PaintRect d);
		Item&          SetImage(const ::Image& _im)     { return Picture(PaintRect(ImageDisplay(), _im)); }
		Item&          SetCtrl(Ctrl *_ctrl);
		Item&          SetCtrl(Ctrl& c)                 { return SetCtrl(&c); }
		Item&          Slave(Ctrl *_slave);
		Item&          Key(dword _key)                  { key = _key; return *this; }

		Item&          Enable(bool _en = true);
		Item&          Disable()                        { return Enable(false); }
		bool           IsEnabled() const                { return enabled; }

		Item();

	//deprecated:
		Item&          Control(Ctrl *c)                 { return SetCtrl(c); }
		Item&          Image(const ::Image& m)          { return SetImage(m); }
	};

private:
	struct Tabs : public Ctrl {
		virtual void Paint(Draw& w);
	};

	Array<Item> tab;
	int         x0;
	int         hot;
	int         sel;
	Tabs        tabs;
	Button      left, right;
	ParentCtrl  pane;
	bool        accept_current;

	int       (*metric)(int i);
	Value     (*look)(int i);

	static Image Fade(int i);

	int        GetTab(Point p);
	void       PaintTabs(Draw& w);
	void       ScrollInto(int i);
	void       Left();
	void       Right();
	void       SyncHot();
	void       SyncTabs();
	int        TabsRight();
	void       Go(int d);

public:
	enum ChParam {
		TABHEIGHT,
		MARGIN, SELTOP, SELLEFT, SELRIGHT, SELBOTTOM,
		LEFTEDGE, TOPEDGE, RIGHTEDGE, BOTTOMEDGE,
		METRIC_COUNT,

		FIRST = 0,
		NEXT = 4,
		BODY = 8,
		LOOK_COUNT = 9
	};

	Callback WhenSet;

	TabCtrl::Item& Add();
	TabCtrl::Item& Add(const char *text);
	TabCtrl::Item& Add(const Image& m, const char *text);
	TabCtrl::Item& Add(Ctrl& slave, const char *text);
	TabCtrl::Item& Add(Ctrl& slave, const Image& m, const char *text);

	int   GetCount() const                       { return tab.GetCount(); }
	Item& GetItem(int i)                         { return tab[i]; }
	const Item& GetItem(int i) const             { return tab[i]; }

	void Set(int i);
	int  Get() const                             { return sel; }

	void GoNext()                                { Go(1); }
	void GoPrev()                                { Go(-1); }

	Size     ComputeSize(Size pane);
	void     Add(Ctrl& c)                        { pane.Add(c.SizePos()); }
	TabCtrl& operator<<(Ctrl& c)                 { Add(c); return *this; }

	TabCtrl& AcceptCurrent(bool ac = true)         { accept_current = ac; return *this; }
	TabCtrl& AcceptAll()                           { return AcceptCurrent(false); }
	TabCtrl& Style(Value (*l)(int))                { look = l; Refresh(); return *this; }
	TabCtrl& Style(Value (*l)(int), int (*m)(int)) { metric = m; RefreshLayout(); return Style(l); }

	void Reset();

	typedef TabCtrl CLASSNAME;

	TabCtrl();
};

class TabDlg : public TopWindow {
	void    Rearrange();

	bool    binit;
	Size    sz;

	void           PlaceButton(Button& b, int& r);
	TabCtrl::Item& Add0(Ctrl& tab, const char *text);
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
