void Animate(Ctrl& c, const Rect& target, int type = -1);
void Animate(Ctrl& c, int x, int y, int cx, int cy, int type = -1);

bool CtrlLibDisplayError(const Value& ev);

bool EditText(String& s, const char *title, const char *label, int (*filter)(int), int maxlen = 0);
bool EditText(String& s, const char *title, const char *label, int maxlen = 0);

bool EditText(WString& s, const char *title, const char *label, int (*filter)(int), int maxlen = 0);
bool EditText(WString& s, const char *title, const char *label, int maxlen = 0);

void Show2(Ctrl& ctrl1, Ctrl& ctrl, bool show = true);
void Hide2(Ctrl& ctrl1, Ctrl& ctrl);

void UpdateFile(const char *filename);
void SelfUpdate();
bool SelfUpdateSelf();

void WindowsList();
void WindowsMenu(Bar& bar);

class DelayCallback : public Pte<DelayCallback> {
	Callback target;
	int      delay;

public:
	void     Invoke();
	void     operator<<=(Callback x)  { target = x; }
	void     SetDelay(int ms)         { delay = ms; }
	Callback Get()                    { return callback(this, &DelayCallback::Invoke); }
	Callback operator~()              { return Get(); }
	operator Callback()               { return Get(); }

	DelayCallback()                   { delay = 2000; }
	~DelayCallback()                  { KillTimeCallback(this); }
};

#ifdef PLATFORM_WIN32
struct Win32PrintDlg_;
#endif

class PrinterJob {
#ifdef PLATFORM_WIN32
	One<Win32PrintDlg_> pdlg;
#endif
#ifdef PLATFORM_X11
	Size                pgsz;
#endif
	One<Draw>           draw;
	Vector<int>         page;
	int                 from, to, current;
	bool                landscape;
	String              name;
	String              options;
	bool                Execute0(bool dodlg);

public:
	Draw&               GetDraw();
	operator            Draw&()                         { return GetDraw(); }
	const Vector<int>&  GetPages() const                { return page; }
	int                 operator[](int i) const         { return page[i]; }
	int                 GetPageCount() const            { return page.GetCount(); }

	bool                Execute();

	PrinterJob& Landscape(bool b = true)                { landscape = b; return *this; }
	PrinterJob& MinMaxPage(int minpage, int maxpage);
	PrinterJob& PageCount(int n)                        { return MinMaxPage(0, n - 1); }
	PrinterJob& CurrentPage(int currentpage);

	PrinterJob(const char *name = NULL);
	~PrinterJob();
};


#ifdef PLATFORM_WIN32

#include <ShellAPI.h>

class TrayIcon : private Ctrl {
	Image          icon;
	bool           visible;
	String         tip;
	NOTIFYICONDATA nid;
	HWND           hwnd;

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void Notify(dword msg);

public:
	Callback        WhenLeftDown;
	Callback        WhenLeftUp;
	Callback        WhenLeftDouble;
	Callback1<Bar&> WhenBar;

	void            Show(bool b = true);
	void            Hide()                                 { Show(false); }
	void            Break()                                { EndLoop(0); }
	void            Run()                                  { EventLoop(this); }

	TrayIcon&  Icon(const Image &img);
	TrayIcon&  Tip(const char *text);

	typedef TrayIcon CLASSNAME;

	TrayIcon();
	~TrayIcon();
};

class FileSelector {
protected:
	struct FileType : Moveable<FileType> {
		String name;
		String ext;
	};

	Vector<FileType> type;

	int    activetype;
	String activedir;
	String defext;
	bool   rdonly;
	bool   multi;
	bool   readonly;
	bool   asking;

	Vector<String> filename;

public:
	void Serialize(Stream& s);

	void New()                                   { filename.Clear(); }
	bool IsNew() const                           { return filename.IsEmpty(); }
	bool Execute(bool open, const char *title = NULL);
	bool ExecuteOpen(const char *title = NULL)   { return Execute(true, title); }
	bool ExecuteSaveAs(const char *title = NULL) { return Execute(false, title); }

	String Get() const;
	void  Set(const String& s)                   { filename.At(0) = s; }

	operator String() const                      { return Get(); }
	void operator=(const String& s)              { Set(s); }

	String operator~() const                     { return Get(); }
	void operator<<=(const String& s)            { Set(s); }

	int   GetCount() const                       { return filename.GetCount(); }
	const String& operator[](int i) const        { return filename[i]; }

	bool   GetReadOnly() const                   { return readonly; }
	String GetActiveDir() const                  { return activedir; }

	FileSelector& Type(const char *name, const char *ext);
	FileSelector& AllFilesType();
	FileSelector& ActiveDir(const String& dir)   { activedir = dir; return *this; }
	FileSelector& ActiveType(int i)              { activetype = i; return *this;  }
	FileSelector& DefaultExt(const char *ext)    { defext = ext; return *this; }
	FileSelector& Multi(bool b = true)           { multi = b; return *this; }
	FileSelector& ReadOnlyOption(bool b = true)  { rdonly = b; return *this; }
	FileSelector& Asking(bool b = true)          { asking = b; return *this; }
	FileSelector& NoAsking()                     { return Asking(false); }

	FileSelector();
};


#endif

#ifdef PLATFORM_X11
typedef FileSel FileSelector;
#endif

class CtrlRetriever {
public:
	struct Item {
		virtual void Retrieve() = 0;
	};

private:
	struct CtrlItem0 : Item {
		Ctrl  *ctrl;
	};

	template <class T>
	struct CtrlItem : CtrlItem0 {
		T     *value;

		virtual void Retrieve()  { *value = ~*ctrl; }
	};

	Array<Item> item;

public:
	void Put(Item *newitem)                       { item.Add(newitem); }

	template <class T>
	void Put(Ctrl& ctrl, T& val);

	template <class T>
	CtrlRetriever& operator()(Ctrl& ctrl, T& val) { Put(ctrl, val); return *this; }

	void Retrieve();

	Callback operator<<=(Callback cb);
};

template <class T>
void CtrlRetriever::Put(Ctrl& ctrl, T& val)
{
	CtrlItem<T> *m = new CtrlItem<T>();
	m->ctrl = &ctrl;
	m->value = &val;
	ctrl <<= val;
	Put(m);
}

void UpdateFile(String dst, String src);
