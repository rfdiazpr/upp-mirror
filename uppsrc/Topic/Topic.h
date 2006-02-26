#ifndef _Topic_Topic_h
#define _Topic_Topic_h

#include <RichEdit/RichEdit.h>
#include <PdfDraw/PdfDraw.h>

#define LAYOUTFILE <Topic/Topic.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <Topic/Topic.iml>
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
	virtual void MainTool(Bar& bar);

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

public:
	virtual void SyncFile(const String& path, const String& link);
	
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

	typedef TopicEditor CLASSNAME;
	TopicEditor();
	virtual ~TopicEditor();
};

#endif
