#ifndef _Browser_Browser_h
#define _Browser_Browser_h

#include <CtrlLib/CtrlLib.h>
#include <CppBase/CppBase.h>
#include <ide/Common/Common.h>
#include <RichEdit/RichEdit.h>
#include <PdfDraw/PdfDraw.h>

#define LAYOUTFILE <ide/Browser/Browser.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS BrowserImg
#define IMAGEFILE <ide/Browser/Browser.iml>
#include <Draw/iml_header.h>

enum { WITHBODY = 33 };

struct CppNestingInfo {
	String nesting;
	int    namespacel;
};

struct CppItemInfo : CppSimpleItem, CppNestingInfo {
	String key;
	String name;
	bool   over;
	bool   overed;
	int    inherited;
	String fn;
	int    line;
	int    typei;
	int    nesti;

	CppItemInfo() { over = overed = virt = false; inherited = line = namespacel = 0; }
};

enum {
	ITEM_TEXT,
	ITEM_NAME,
	ITEM_CPP_TYPE,
	ITEM_CPP,
	ITEM_PNAME,
	ITEM_TNAME,
	ITEM_NUMBER,
	ITEM_SIGN,
	ITEM_RET,
	ITEM_TYPE,
	ITEM_PTYPE = ITEM_TYPE + 10000,
};

struct ItemTextPart : Moveable<ItemTextPart> {
	int pos;
	int len;
	int type;
	int ii;
};

Vector<ItemTextPart> ParseItemNatural(const CppItemInfo& m, const char *natural);
Vector<ItemTextPart> ParseItemNatural(const CppItemInfo& m);

bool SplitNestKey(const String& s, String& nest, String& key);

struct BrowserFileInfo {
	Time     time;
	String   package;
	String   file;

	BrowserFileInfo() { time = Null; }
};

ArrayMap<String, BrowserFileInfo>& FileSet();

int GetItemHeight(const CppSimpleItem& m, int cx);

struct BrowserQuery {
	String name;
	String package;
	String file;
	bool   a_private, a_protected, a_public;
	bool   code, data, type, macro;
	bool   documented, notdocumented;

	void Clear();
	void Serialize(Stream& s);

	BrowserQuery() { Clear(); }
};

struct QueryDlg : public WithQueryLayout<TopWindow> {
	typedef QueryDlg CLASSNAME;

	void Serialize(Stream& s);
	void EnterPackage();
	void Clear();

	int Perform(BrowserQuery& q);

	QueryDlg();
};

struct CppItemInfoDisplay : public Display
{
	String hkey;
	int    htopic;

	int DoPaint(Draw& w, const Rect& r, const Value& q,
		        Color _ink, Color paper, dword style) const;
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color _ink, Color paper, dword style) const;
	virtual Size GetStdSize(const Value& q) const;
};

struct CppNestingInfoDisplay : public Display
{
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color _ink, Color paper, dword style) const;
};

class ItemList : public ColumnList {
	CppItemInfoDisplay display;

	friend struct ItemDisplay;

	int    GetTopic(Point p, String& key);
	String Item(int i);

public:
	bool active_topics;

	void Clear();

	ItemList();
};

class Browser : public StaticRect {
public:
	void Serialize(Stream& s);
	void SerializeWspc(Stream& s);

	ArrayCtrl              nesting;
	ItemList               item;
	Splitter               split;
	BrowserQuery           query;
	QueryDlg               querydlg;
	Callback2<String, int> WhenPos;
	int                    pos;
	Callback               WhenItem;
	Callback               WhenItemDblClk;
	Callback1<String>      WhenShowTopic;
	bool                   clickpos;
	bool                   show_inherited;

	void     LoadNest(const String& nest, ArrayMap<String, CppItemInfo>& item, int inherited);
	bool     IsCurrentItem();
	CppItem& CurrentItem();
	void     Reload();
	void     EnterNesting();
	void     EnterItem();
	void     ItemClick();
	void     ItemDblClk();
	void     GotoItem();
	void     GotoPos(int n);
	void     ItemMenu(Bar& bar);
	void     QueryNest();
	bool     FindSet(const String& knesting, const String& kitem, int nestingsc = 0, int itemsc = 0);
	bool     FindSet(const String& item);

	String      GetItem(int i);
	CppItemInfo GetItemInfo(int i);

	String      GetItem()               { return GetItem(item.GetCursor()); }
	CppItemInfo GetItemInfo()           { return GetItemInfo(item.GetCursor()); }

	bool     DoQuery();
	void     DoDoQuery()                { DoQuery(); }
	void     QueryWord(const String& w);
	void     SetId(const String& id, const Vector<String>& nest);

	void     ShowTopic(String w);
	void     ShowHelp();

	typedef Browser CLASSNAME;
	Browser();
	~Browser();
};

Vector<String> IgnoreList();
CppBase&       BrowserBase();
void           StartBrowserBase();
void           BrowserBaseScan(Stream& s, const String& fn);
void           ClearBrowserBase();
void           RescanBrowserBase();
void           SyncBrowserBase();
void           SaveBrowserBase();
void           Register(Browser *b);
void           UnRegister(Browser *b);
bool           ExistsBrowserItem(const String& item);
void           ReQualifyBrowserBase();

void           BrowserBaseScanLay(const String& fn);
void           ScanLayFile(const char *fn);

struct TopicInfo : Moveable<TopicInfo> {
	Time           time;
	String         path;
	String         title;
	Vector<int>    words;
};

void            SyncRefs();
void            SyncTopicFile(const RichText& text, const String& link, const String& path,
                              const String& title);
void            SyncTopicFile(const String& link, const String& path);
String          GetTopicTitle(const String& link);

Vector<String>  GetRefLinks(const String& ref);

int             TopicWordIndex(const String& w);

bool            MatchTopicLink(const String& link, const Vector<int>& query);

#define LAYOUTFILE <ide/Browser/Topic.lay>
#include <CtrlCore/lay.h>

struct ReferenceDlg : WithReferenceDlgLayout<TopWindow> {
	void   EnterItem();
	void   EnterItemOk();
	void   Set(const String& s);
	String Get() const            { return ~reference; }

	void   Serialize(Stream& s)   { browser.SerializeWspc(s); SerializePlacement(s); }

	typedef ReferenceDlg CLASSNAME;

	ReferenceDlg();
};

#define IMAGEFILE <ide/Browser/Topic.iml>
#define IMAGECLASS TopicImg
#include <Draw/iml_header.h>

int  CharFilterID(int c);

bool ParseTopicFileName(const String& fn, String& topic, int& lang);

Topic  ReadTopic(const char *text);
Vector<String> GatherLabels(const RichText& text);
String WriteTopic(const char *title, const RichText& text);

void LoadGroups(FileList& group, const String& dir);
void LoadTopics(FileList& topic, const String& dir, const String& filepath = Null);

struct StyleDlg;

class TopicEditor : public TopWindow {
public:
	virtual bool Key(dword key, int);
	virtual void Close();

protected:
	MenuBar           menu;
	ToolBar           tool;

	FileList          package;
	FileList          group;
	FileList          topic;

	EditString        title;
	RichEdit          editor;

	Splitter          vert;
	StaticRect        right;
	Splitter          left_right;

	String            commondir;
	Vector<String>    packagedir;

	String            filepath;

	String            laststylesheet;
	int               lastlang;

	Vector<String>    tablru;
	int               tabi;
	bool              allfonts;

	struct FileInfo {
		Time               time;
		RichEdit::UndoInfo undo;
		RichEdit::PosInfo  pos;

		FileInfo() { time = Time(1, 1, 1); }
	};

	ArrayMap<String, FileInfo> editstate;

	void   MainMenu(Bar& bar);
	void   EditMenu(Bar& bar);
	void   FormatMenu(Bar& bar);
	void   GroupMenu(Bar& bar);
	void   TableMenu(Bar& bar);
	void   TopicMenu(Bar& bar);
	void   MainTool(Bar& bar);
	void   FileBar(Bar& bar);
	void   StyleSheetMenu(Bar& bar);
	void   Exit();
	void   SetBar();

	void   NewGroup();
	void   RemoveGroup();

	String GetCurrentTopicPath();

	void   NewTopic();
	void   RenameTopic();
	void   RemoveTopic();

	String ActualPackageDir();

	TopicLink ParseTopicFilePath(const String& path);

	bool         autosave;
	ReferenceDlg ref;

	void   ShowTopic(bool b = true);
	void   HideTopic()                   { ShowTopic(false); }

	void   EnterPackage();
	void   EnterGroup();
	void   EnterTopic();

	void   SaveInc(const String& packagedir, const String& group);

	int    Execute(StyleDlg& d);
	void   EditStylesheets();
	void   StoreStylesheet();
	void   ApplyStylesheet();
	void   ApplyStylesheetGroup();

	void   Hyperlink(String&, WString&);

	void   Load(const String& path);
	void   AddLru();

	void   SyncFonts();
	void   AllFonts();

	void   Tools(Bar& bar);
	void   Label(String&);
	void   CreateQtf(const String& item, const CppItemInfo& m, String& p1, String& p2);
	void   InsertItem();

	void   GoBack()                  { WhenBack(); }
	void   FindBrokenRef();

	void   Repair();
	void   CompressGroup();

public:
	enum {
		TIMEID_AUTOSAVE = TopWindow::TIMEID_COUNT,
	    TIMEID_COUNT
	};

	static Size TopicPage()                          { return Size(3968, INT_MAX); }

	void   Serialize(Stream& s);
	void   SerializeWspc(Stream& s);

	void   ClearPackages();
	void   AddPackage(const char *name, const char *dir);
	void   FinishPackages();

	bool   Open(const String& path);
	bool   OpenLink(const String& link);
	void   GotoLabel(const String& label)                    { editor.GotoLabel(label); }
	String GetFilePath() const                               { return filepath; }
	void   Save();
	void   Flush();

	void   ExportPdf();

	void   SetEditorFocus();

	void   SetCommonDir(const String& common)                { commondir = common; }

	Callback WhenSync;
	Callback WhenBack;

	typedef TopicEditor CLASSNAME;

	TopicEditor();
	virtual ~TopicEditor();
};


#endif
