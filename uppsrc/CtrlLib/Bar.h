class BarPane : public Ctrl {
public:
	virtual void Paint(Draw& w);
	virtual void LeftDown(Point pt, dword keyflags);
	virtual void MouseMove(Point p, dword);

private:
	struct Item {
		Ctrl *ctrl;
		int   gapsize;
	};
	Array<Item>    item;
	Vector<int>    breakpos;
	bool           horz;
	Ctrl          *phelpctrl;
	int            vmargin, hmargin;
	bool           menu;

	int&     HoVe(Size& sz)                  { return horz ? sz.cx : sz.cy; }
	int&     VeHo(Size& sz)                  { return horz ? sz.cy : sz.cx; }
	int&     HoVe(bool horz, Size& sz) const { return horz ? sz.cx : sz.cy; }
	int&     VeHo(bool horz, Size& sz) const { return horz ? sz.cy : sz.cx; }

public:
	Callback WhenLeftClick;

	void  IClear();
	void  Clear();
	bool  IsEmpty() const                    { return item.IsEmpty(); }

	void  Add(Ctrl *ctrl, int gapsize);
	void  AddBreak()                         { Add(NULL, Null); }
	void  AddGap(int gapsize)                { Add(NULL, gapsize); }

	void  Margin(int v, int h)               { vmargin = v; hmargin = h; }

	Size  Repos(bool horz, int maxsize);
	Size  GetPaneSize(bool _horz, int maxsize) const;

	void  SubMenu()                          { menu = true; }

	BarPane();
	virtual ~BarPane();
};

class Bar : public Ctrl {
public:
	struct Item {
		virtual Item& Text(const char *text);
		virtual Item& Key(dword key);
		virtual Item& Repeat(bool repeat = true);
		virtual Item& Image(const class Image& img);
		virtual Item& Check(bool check);
		virtual Item& Radio(bool check);
		virtual Item& Enable(bool _enable = true);
		virtual Item& Tip(const char *tip);
		virtual Item& Help(const char *help);
		virtual Item& Topic(const char *topic);
		virtual Item& Description(const char *desc);
		virtual void  FinalSync();

		Item& Key(KeyInfo& (*key)());

		Item();
		virtual ~Item();
	};

protected:
	virtual Item&  AddItem(Callback cb) = 0;
	virtual Item&  AddSubMenu(Callback1<Bar&> proc) = 0;
	virtual void   AddCtrl(Ctrl *ctrl, int gapsize) = 0;
	virtual void   AddCtrl(Ctrl *ctrl, Size sz) = 0;

	class ScanKeys;

public:
	virtual bool   IsEmpty() const = 0;
	virtual void   Separator() = 0;

	static  Item&  NilItem();
	static bool    Scan(Callback1<Bar&> proc, dword key);

	void   Break();
	void   Gap(int size = 8);
	void   GapRight()                               { Gap(INT_MAX); }

	void   AddNC(Ctrl& ctrl);
	void   Add(Ctrl& ctrl)                          { AddCtrl(&ctrl, ctrl.GetMinSize()); }
	void   Add(Ctrl& ctrl, Size sz)                 { AddCtrl(&ctrl, sz); }
	void   Add(Ctrl& ctrl, int cx, int cy = 0)      { AddCtrl(&ctrl, Size(cx, cy)); }

	void   Add(bool en, Ctrl& ctrl)                     { Add(ctrl); ctrl.Enable(en); }
	void   Add(bool en, Ctrl& ctrl, Size sz)            { Add(ctrl, sz); ctrl.Enable(en); }
	void   Add(bool en, Ctrl& ctrl, int cx, int cy = 0) { Add(ctrl, cx, cy); ctrl.Enable(en); }


	Item&  Add(bool enable, const char *text, const UPP::Image& image, Callback callback);
	Item&  Add(bool enable, KeyInfo& (*key)(), const UPP::Image& image, Callback callback);
	Item&  Add(const char *text, const UPP::Image& image, Callback callback);
	Item&  Add(const String& text, const UPP::Image& image, Callback callback);
	Item&  Add(KeyInfo& (*key)(), const UPP::Image& image, Callback callback);

	Item&  Add(bool enable, const char *text, Callback callback);
	Item&  Add(bool enable, KeyInfo& (*key)(), Callback callback);
	Item&  Add(const char *text, Callback callback);
	Item&  Add(KeyInfo& (*key)(), Callback callback);

	void   MenuSeparator();
	void   MenuBreak();
	void   MenuGap(int size = 8);
	void   MenuGapRight()                           { MenuGap(INT_MAX); }

	void   AddMenu(Ctrl& ctrl);
	void   AddMenu(Ctrl& ctrl, Size sz);
	void   AddMenu(Ctrl& ctrl, int cx, int cy = 0)  { AddMenu(ctrl, Size(cx, cy)); }

	Item&  AddMenu(bool enable, const char *text, const UPP::Image& image, Callback callback);
	Item&  AddMenu(bool enable, KeyInfo& (*key)(), const UPP::Image& image, Callback callback);
	Item&  AddMenu(const char *text, const UPP::Image& image, Callback callback);
	Item&  AddMenu(const String& t, const UPP::Image& m, Callback c) { AddMenu(t, m, c); }
	Item&  AddMenu(KeyInfo& (*key)(), const UPP::Image& m, Callback c);

	Item&  Add(bool enable, const char *text, Callback1<Bar&> proc);
	Item&  Add(const char *text, Callback1<Bar&> proc);
	Item&  Add(bool enable, const char *text, const UPP::Image& image, Callback1<Bar&> proc);
	Item&  Add(const char *text, const UPP::Image& image, Callback1<Bar&> proc);

	void   ToolSeparator();
	void   ToolBreak();
	void   ToolGap(int size = 8);
	void   ToolGapRight()                           { ToolGap(INT_MAX); }

	void   AddTool(Ctrl& ctrl);
	void   AddTool(Ctrl& ctrl, Size sz);
	void   AddTool(Ctrl& ctrl, int cx, int cy = 0)  { AddTool(ctrl, Size(cx, cy)); }

	Item&  Add(const UPP::Image& image, Callback callback);
	Item&  Add(bool enable, const UPP::Image& image, Callback callback);

	virtual void AddKey(dword key, Callback cb);
	        void AddKey(KeyInfo& (*key)(), Callback cb);

	virtual bool IsMenuBar() const                  { return false; }
	virtual bool IsToolBar() const                  { return false; }
	virtual bool IsScanKeys() const                 { return false; }
	virtual bool IsScanHelp() const                 { return false; }

	typedef Bar CLASSNAME;
	Bar();
	virtual ~Bar();
};

class BarCtrl : public Bar, public CtrlFrame {
public:
	virtual void   Layout();

	virtual void   FrameLayout(Rect& r);
	virtual void   FrameAddSize(Size& sz);
	virtual void   FrameAdd(Ctrl& ctrl);
	virtual void   FrameRemove();

	virtual bool   IsEmpty() const;
	virtual void   Separator();

protected:
	virtual void   AddCtrl(Ctrl *ctrl, int gapsize);
	virtual void   AddCtrl(Ctrl *ctrl, Size sz);

private:
	class SizeCtrl : public StaticRect {
	public:
		virtual Size GetMinSize() const;

	private:
		Size size;

	public:
		void  SetSize(Size sz)             { size = sz; }

		SizeCtrl()                         { NoWantFocus(); }
	};

	int                  sii;
	Array<SeparatorCtrl> separator;
	int                  zii;
	Array<SizeCtrl>      sizer;
	int                  align;

protected:
	BarPane pane;
	int     smargin;
	int     ssize;

	void     SyncBar();

	void     IClear();
	void     IFinish();

	void     Clear();


public:
	Callback1<const String&> WhenHelp;
	Callback WhenLeftClick;

	static BarCtrl *GetBarCtrlParent(Ctrl *child);
	static void     SendHelpLine(Ctrl *q);
	static void     ClearHelpLine(Ctrl *q);

	enum {
		BAR_LEFT, BAR_RIGHT, BAR_TOP, BAR_BOTTOM
	};

	BarCtrl& Align(int align);
	BarCtrl& Top()                       { return Align(BAR_TOP); }
	BarCtrl& Bottom()                    { return Align(BAR_BOTTOM); }
	BarCtrl& Left()                      { return Align(BAR_LEFT); }
	BarCtrl& Right()                     { return Align(BAR_RIGHT); }
	int      GetAlign() const            { return align; }

	typedef BarCtrl CLASSNAME;

	BarCtrl();
	virtual ~BarCtrl();
};

class MenuItemBase;

CtrlFrame& MenuFrame();

class MenuBar : public BarCtrl {
public:
	virtual void  LeftDown(Point, dword);
	virtual bool  Key(dword key, int count);
	virtual bool  HotKey(dword key);
	virtual void  ChildGotFocus();
	virtual void  ChildLostFocus();
	virtual void  Deactivate();
	virtual void  CancelMode();

protected:
	virtual Item& AddItem(Callback cb);
	virtual Item& AddSubMenu(Callback1<Bar&> proc);

private:
	Array<MenuItemBase> item;

	MenuBar  *parentmenu;
	MenuBar  *submenu;
	Ctrl     *submenuitem;
	Ptr<Ctrl> restorefocus;
	bool      doeffect;
	Font      font;
	int       leftgap;
	int       lock;

	friend class MenuItemBase;
	friend class SubMenuBase;
	friend class TopSubMenuItem;
	friend class SubMenuItem;

	void     SetParentMenu(MenuBar *parent)    { parentmenu = parent; }
	MenuBar *GetParentMenu()                   { return parentmenu; }
	void     SetActiveSubmenu(MenuBar *sm, Ctrl *menuitem);
	MenuBar *GetActiveSubmenu()                { return submenu; }
	MenuBar *GetMasterMenu();
	MenuBar *GetLastSubmenu();
	void     DelayedClose();
	void     KillDelayedClose();
	void     SubmenuClose();
	void     PostDeactivate();
	void     SyncState();
	void     SetupRestoreFocus();

protected:
	enum {
		TIMEID_STOP = BarCtrl::TIMEID_COUNT,
		TIMEID_SUBMENUCLOSE,
		TIMEID_POST,
		TIMEID_COUNT
	};

public:
	virtual bool IsMenuBar() const                  { return true; }

	static int GetStdHeight(Font font = StdFont());

	void     CloseMenu();


	void     Set(Callback1<Bar&> menu);
	void     Post(Callback1<Bar&> bar);

	void     PopUp(Ctrl *owner, Point p, Size sz = Size(0, 0));
	void     PopUp(Point p)                         { PopUp(GetActiveCtrl(), p); }
	void     PopUp()                                { PopUp(GetMousePos()); }

	void     Execute(Ctrl *owner, Point p);
	void     Execute(Point p)                       { Execute(GetActiveCtrl(), p); }
	void     Execute()                              { Execute(GetMousePos()); }

	static void Execute(Ctrl *owner, Callback1<Bar&> proc, Point p);
	static void Execute(Callback1<Bar&> proc, Point p) { Execute(GetActiveCtrl(), proc, p); }
	static void Execute(Callback1<Bar&> proc)          { Execute(proc, GetMousePos()); }

	bool     IsEmpty()                              { return item.IsEmpty(); }
	void     Clear();

	MenuBar& LeftGap(int cx)                        { leftgap = cx; return *this; }
	MenuBar& SetFont(Font f)                        { font = f; return *this; }
	Font     GetFont() const                        { return font; }

	typedef MenuBar CLASSNAME;

	MenuBar();
	virtual ~MenuBar();
};

class ToolButton : public Ctrl, public Bar::Item {
public:
	virtual void   Paint(Draw& w);
	virtual void   MouseEnter(Point, dword);
	virtual void   MouseLeave();
	virtual Size   GetMinSize() const;
	virtual void   LeftDown(Point, dword);
	virtual void   LeftRepeat(Point, dword);
	virtual void   LeftUp(Point, dword);
	virtual bool   HotKey(dword key);
	virtual String GetDesc() const;

	virtual Bar::Item& Text(const char *text);
	virtual Bar::Item& Key(dword key);
	virtual Bar::Item& Repeat(bool repeat = true);
	virtual Bar::Item& Image(const UPP::Image& img);
	virtual Bar::Item& Enable(bool _enable);
	virtual Bar::Item& Tip(const char *tip);
	virtual Bar::Item& Help(const char *help);
	virtual Bar::Item& Topic(const char *help);
	virtual Bar::Item& Description(const char *desc);
	virtual Bar::Item& Radio(bool check);
	virtual Bar::Item& Check(bool check);
	virtual void       FinalSync();

protected:
	String  tiptext;
	dword   accel;
	bool    checked;
	bool    paint_checked;
	bool    repeat;

private:
	UPP::Image image;

	void  SendHelpLine();
	void  ClearHelpLine();
	void  UpdateTip();

public:
	void  Reset();

	ToolButton();
	virtual ~ToolButton();
};

class ToolBar : public BarCtrl {
public:
	virtual bool HotKey(dword key);

protected:
	virtual Item& AddItem(Callback cb);
	virtual Item& AddSubMenu(Callback1<Bar&> proc);

private:
	int               ii;
	Array<ToolButton> item;
	int               lock;
	Callback1<Bar&>   proc;

protected:
	enum {
		TIMEID_POST = BarCtrl::TIMEID_COUNT,
		TIMEID_COUNT
	};

public:
	virtual bool IsToolBar() const                  { return true; }

	static int GetStdHeight();

	void Clear();
	void Set(Callback1<Bar&> bar);
	void Post(Callback1<Bar&> bar);

	typedef ToolBar  CLASSNAME;

	ToolBar();
	virtual ~ToolBar();
};

class LRUList {
	Vector<String> lru;
	int            limit;
	void           Select(String s, Callback1<const String&> WhenSelect);

public:
	static int GetStdHeight();

	void        Serialize(Stream& stream);

	void        operator()(Bar& bar, Callback1<const String&> WhenSelect);

	void        NewEntry(const String& path);
	void        RemoveEntry(const String& path);

	LRUList&    Limit(int _limit)                       { limit = _limit; return *this; }
	int         GetLimit() const                        { return limit; }
	int         GetCount() const                        { return lru.GetCount(); }

	typedef LRUList CLASSNAME;

	LRUList()   { limit = 6; }
};

class ToolTip : public Ctrl {
public:
	virtual void Paint(Draw& w);
	virtual Size GetMinSize() const;

private:
	String  text;

public:
	void Set(const char *_text)        { text = _text; }

	void PopUp(Ctrl *owner, Point p, bool effect);

	ToolTip();
};

void PerformDescription();
