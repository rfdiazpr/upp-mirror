#include "Topic.h"

#define IMAGEFILE <Topic/Topic.iml>
#define IMAGECLASS TopicImg
#include <Draw/iml_source.h>

TopicEditor::TopicEditor()
{
	SetRect(0, 0, 600, 400);
	AddFrame(menu);
	editor.InsertFrame(0, TopSeparatorFrame());
	editor.InsertFrame(1, tool);
	SetBar();
	Font tf = Arial(16).Bold();
	int dcy = EditField::GetStdHeight(tf);
	title.SetFont(tf);
	right.Add(title.HSizePos(2, 2).TopPos(0, dcy));
	right.Add(editor.VSizePos(dcy + 4, 0).HSizePos());
	vert.Add(package);
	vert.Add(group);
	vert.Add(topic);
	vert.Vert();
	Add(left_right.Horz(vert, right));
	left_right.SetPos(2000);
	Zoomable().Sizeable();

	package.NoRoundSize().Columns(2);
	group.NoRoundSize().Columns(2);
	topic.NoRoundSize().Columns(2);

	title.Tip("Topic title");

	package.WhenKillCursor = package.WhenEnterItem = THISBACK(EnterPackage);
	group.WhenKillCursor = group.WhenEnterItem = THISBACK(EnterGroup);
	topic.WhenKillCursor = topic.WhenEnterItem = THISBACK(EnterTopic);

	group.WhenBar = THISBACK(GroupMenu);
	topic.WhenBar = THISBACK(TopicMenu);

	package.NoWantFocus();
	group.NoWantFocus();
	topic.NoWantFocus();

	editor.SetPage(TopicPage());
	editor.WhenRefreshBar = THISBACK(SetBar);
	editor.WhenHyperlink = THISBACK(Hyperlink);

	editor.Disable();
	title.Disable();

	Icon(TopicImg::Topic(), TopicImg::Topic());

	lastlang = LNG_ENGLISH;

	tabi = 0;

	ActiveFocus(editor);

	Maximize();
}

TopicEditor::~TopicEditor()
{
}

void TopicEditor::Serialize(Stream& s)
{
	int version = 1;
	s / version;
	SerializePlacement(s);
	editor.SerializeSettings(s);
	s % left_right % vert;
	package.Serialize(s);
	group.Serialize(s);
	topic.Serialize(s);
	bool b;
	if(version >= 1)
		s % b;
}

void TopicEditor::SerializeWspc(Stream& s)
{
	if(s.IsStoring()) {
		for(int i = 0; i < editstate.GetCount(); i++) {
			String fn = editstate.GetKey(i);
			if(!IsNull(fn) && FileExists(fn)) {
				s % fn;
				s % editstate[i].pos;
			}
		}
		String dm;
		s % dm;
	}
	else
		for(;;) {
			String fn;
			s % fn;
			if(IsNull(fn))
				break;
			s % editstate.GetAdd(fn).pos;
		}
	String _package, _group, _topic;
	int _lang;
	int version = 3;
	s / version;
	if(version < 1)
		s % _package % _group % _topic % _lang;
	else {
		String d;
		if(version < 3)
			s % d;
		s % lastlang;
		if(version >= 2)
			s % tablru;
		if(version >= 3) {
			if(s.IsStoring()) {
				for(int i = 0; i < editstate.GetCount(); i++) {
					String fn = editstate.GetKey(i);
					if(!fn.IsEmpty() && FileExists(fn)) {
						s % fn;
						s % editstate[i].pos;
					}
				}
				String h;
				s % h;
			}
			else {
				String fn;
				editstate.Clear();
				for(;;) {
					s % fn;
					if(fn.IsEmpty()) break;
					FileInfo& fd = editstate.GetAdd(fn);
					s % fd.pos;
				}
			}
		}
	}
	s % laststylesheet;
}

void TopicEditor::ExportPdf()
{
	FileSel fs;
	fs.Type("PDF files", "*.pdf")
	  .AllFilesType()
	  .DefaultExt("pdf");
	if(!fs.ExecuteSaveAs("Output PDF file"))
		return;
	Size page = Size(3968, 6047);
	PdfDraw pdf(page + 400);
	::Print(pdf, editor.Get(), page);
	SaveFile(~fs, pdf.Finish());
}

void TopicEditor::Exit()
{
	Close();
}

void TopicEditor::GroupMenu(Bar& bar)
{
	bar.Add(package.IsCursor(), "New group..", THISBACK(NewGroup));
	bar.Add(group.IsCursor(), "Remove group..", THISBACK(RemoveGroup));
}

void TopicEditor::TopicMenu(Bar& bar)
{
	bar.Add(group.IsCursor(), "New topic..", THISBACK(NewTopic))
	   .Key(K_CTRL_N).Key(K_ALT_INSERT);
	bar.Add(topic.IsCursor(), "Rename topic..", THISBACK(RenameTopic));
	bar.Add(topic.IsCursor(), "Remove topic", THISBACK(RemoveTopic))
	   .Key(K_ALT_DELETE);
}

void TopicEditor::FileBar(Bar& bar)
{
	TopicMenu(bar);
	bar.Separator();
	GroupMenu(bar);
	bar.Separator();
	bar.Add(!IsNull(filepath), "Save", THISBACK(Save))
	   .Key(K_CTRL_S);
	bar.Separator();
	editor.PrintTool(bar);
	bar.Add("Export to PDF..", THISBACK(ExportPdf));
	bar.Separator();
	bar.Add("Exit", THISBACK(Exit));
}

void TopicEditor::EditMenu(Bar& bar)
{
	editor.CutTool(bar);
	editor.CopyTool(bar);
	editor.PasteTool(bar);
	bar.Separator();
	editor.UndoTool(bar);
	editor.RedoTool(bar);
	bar.Separator();
	editor.FindReplaceTool(bar);
}

void TopicEditor::FormatMenu(Bar& bar)
{
	editor.StyleTool(bar);
	bar.Separator();
	editor.FontTools(bar);
	bar.Separator();
	editor.InkTool(bar);
	editor.PaperTool(bar);
	bar.Separator();
	editor.LanguageTool(bar);
	editor.SpellCheckTool(bar);
	bar.Separator();
	editor.IndexEntryTool(bar);
	bar.Break();
	editor.HyperlinkTool(bar, 300);
	bar.Separator();
	editor.ParaTools(bar);
}

void TopicEditor::TableMenu(Bar& bar)
{
	editor.TableTools(bar);
}

void TopicEditor::StyleSheetMenu(Bar& bar)
{
	bar.Add("Stylesheets..", THISBACK(EditStylesheets));
	bar.Add(!IsNull(filepath), "Store stylesheet..", THISBACK(StoreStylesheet));
	bar.Add(!IsNull(filepath), "Apply stylesheet..", THISBACK(ApplyStylesheet));
	bar.Add("Apply stylesheet to group..", THISBACK(ApplyStylesheetGroup));
}

void TopicEditor::MainMenu(Bar& bar)
{
	bar.Add("Topic", THISBACK(FileBar));
	bar.Add("Edit", THISBACK(EditMenu));
//	bar.Add("Format", THISBACK(FormatMenu));
	bar.Add("Table", THISBACK(TableMenu));
	bar.Add("Stylesheet", THISBACK(StyleSheetMenu));
}

void TopicEditor::MainTool(Bar& bar)
{
	editor.StyleTool(bar);
	bar.Gap();
	editor.FontTools(bar);
	bar.Gap();
	editor.InkTool(bar);
	editor.PaperTool(bar);
	bar.Gap();
	editor.LanguageTool(bar);
	editor.SpellCheckTool(bar);
	bar.Gap();
	editor.IndexEntryTool(bar);
	bar.Break();
	editor.HyperlinkTool(bar, 300);
	bar.Gap();
	editor.ParaTools(bar);
	bar.Gap();
	editor.EditTools(bar);
	bar.Gap();
	editor.PrintTool(bar);
	editor.TableTools(bar);
}

void TopicEditor::NewGroup()
{
	if(!package.IsCursor())
		return;
	WithGroupDlgLayout<TopWindow> d;
	CtrlLayoutOKCancel(d, "New group");
	d.group.SetFilter(CharFilterID);
	if(d.Run() != IDOK)
		return;
	DirectoryCreate(AppendFileName(ActualPackageDir(), (String)~d.group + ".tpp"));
	EnterPackage();
	group.FindSetCursor((String)~d.group);
}

void TopicEditor::RemoveGroup()
{
	if(!group.IsCursor() || !PromptYesNo("Remove group?"))
		return;
	String pd = AppendFileName(ActualPackageDir(), group.GetCurrentName() + ".tpp");
	FindFile ff(AppendFileName(pd, "*.*"));
	while(ff) {
		if(ff.IsFile() || ff.IsFolder()) {
			if(PromptYesNo("Group is not empty!&Delete anyway?"))
				break;
			else
				return;
		}
		ff.Next();
	}
	ff.Close();
	Flush();
	if(!DeleteFolderDeep(pd))
		Exclamation(GetLastErrorMessage());
	EnterPackage();
}

void CreateTopic(const char *fn, int lang, const String& ss)
{
	SaveFile(fn, "TITLE(\"\")\r\nTOPIC_TEXT(" + AsCString(ss) + ")"
	             + "TOPIC_TEXT(\"[{_}%" + LNGAsText(lang) + " &]\")");
}

template <class T>
struct TopicDlg : T {
	String GetName() const {
		return (String)~T::topic + "$" + ToLower(LNGAsText(~T::lang)) + ".tpp";
	}

	TopicDlg(const char *title) {
		CtrlLayoutOKCancel(*this, title);
		T::topic.NotNull();
		T::topic.MaxLen(30);
		T::topic.SetFilter(CharFilterID);
	}
};

void TopicEditor::NewTopic()
{
	if(!package.IsCursor() || !group.IsCursor())
		return;
	TopicDlg<WithNewTopicLayout<TopWindow> > d("New topic");
	d.lang <<= lastlang;

	Vector<String> name;
	Vector<String> style;
	name.Add("<none>");
	style.Add("");
	FindFile ff(AppendFileName(commondir, "*.style"));
	while(ff) {
		name.Add(GetFileTitle(ff.GetName()));
		style.Add(LoadFile(AppendFileName(commondir, ff.GetName())));
		ff.Next();
	}
	IndexSort(name, style);
	for(int i = 0; i < name.GetCount(); i++)
		d.stylesheet.Add(name[i]);

	if(d.stylesheet.HasKey(laststylesheet))
		d.stylesheet <<= laststylesheet;
	else
		d.stylesheet.SetIndex(0);

	d.ActiveFocus(d.topic);
	String fn;
	for(;;) {
		if(d.Run() != IDOK)
			return;
		fn = AppendFileName(AppendFileName(ActualPackageDir(), group.GetCurrentName() + ".tpp"), d.GetName());
		if(!FileExists(fn))
			break;
		if(PromptYesNo("Topic already exists.&Do you want to clear it?"))
			break;
	}
	laststylesheet = ~d.stylesheet;
	lastlang = ~d.lang;
	int q = d.stylesheet.GetIndex();
	CreateTopic(fn, ~d.lang, q >= 0 ? style[q] : Null);
	SaveInc(ActualPackageDir(), group.GetCurrentName());
	Flush();
	Load(fn);
	EnterGroup();
}

void TopicEditor::RenameTopic()
{
	if(!topic.IsCursor())
		return;
	TopicDlg<WithRenameTopicLayout<TopWindow> > d("Rename topic");
	String p = GetCurrentTopicPath();
	String tn;
	int    lng;
	ParseTopicFileName(p, tn, lng);
	d.topic <<= tn;
	d.lang <<= lng;
	if(d.Run() != IDOK)
		return;
	String np = AppendFileName(GetFileFolder(p), d.GetName());
	if(FindFile(np)) {
		Exclamation("Target file aready exists!");
		return;
	}
	Flush();
	FileMove(p, np);
	Load(np);
	EnterGroup();
	SaveInc(ActualPackageDir(), group.GetCurrentName());
}

void TopicEditor::RemoveTopic()
{
	if(!topic.IsCursor() || !group.IsCursor() || !package.IsCursor()
	   || !PromptYesNo("Delete topic [* " + DeQtf(topic.GetCurrentName()) + "] ?"))
		return;
	String packagedir = ActualPackageDir();
	String p = GetCurrentTopicPath();
	Flush();
	DeleteFile(p);
	EnterGroup();
	SaveInc(packagedir, group.GetCurrentName());
}

void TopicEditor::SetBar()
{
	menu.Set(THISBACK(MainMenu));
	tool.Set(THISBACK(MainTool));
}

void TopicEditor::ClearPackages()
{
	package.Clear();
	packagedir.Clear();
}

void TopicEditor::AddPackage(const char *name, const char *dir)
{
	RealizeDirectory(dir);
	package.Add(name, TopicImg::Package());
	packagedir.Add(dir);
}

void TopicEditor::FinishPackages()
{
	String s = filepath;
	Flush();
	group.Clear();
	topic.Clear();
	Open(s);
}

void TopicEditor::Close()
{
	Save();
	TopWindow::Close();
}

void TopicEditor::SyncFile(const String& path, const String& link)
{
}

void TopicEditor::AddLru()
{
	if(filepath.IsEmpty() || tabi) return;
	LruAdd(tablru, filepath, 30);
}

bool TopicEditor::Key(dword key, int cnt)
{
	switch(key) {
	case K_CTRL_KEY|K_KEYUP:
		if(tabi) {
			tabi = 0;
			AddLru();
		}
		return true;
	case K_CTRL_TAB:
		while(tablru.GetCount()) {
			if(++tabi >= tablru.GetCount())
				tabi = 0;
			if(tabi < tablru.GetCount())
				if(Open(tablru[tabi])) {
					Sync();
					break;
				}
			tablru.Remove(tabi);
		}
		return true;
	case K_ALT_UP:
		return topic.Key(K_UP, 0);
	case K_ALT_DOWN:
		return topic.Key(K_DOWN, 0);
	case K_ALT_LEFT:
		return package.Key(K_UP, 0);
	case K_ALT_RIGHT:
		return package.Key(K_DOWN, 0);
	}
	return TopWindow::Key(key, cnt);
}

void TopicEditor::SetEditorFocus()
{
	editor.SetFocus();
	editor.ScrollToCursor();
}
