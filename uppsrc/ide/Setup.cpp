#include "ide.h"
#pragma hdrstop

#define LLOG(x) // LOG(x)

int CharFilterVar(int c)
{
	return IsAlNum(c) || c == '_' ? c : 0;
}

void DlCharset(DropList& d)
{
	d.Add(CHARSET_UTF8, "UTF8");
	for(int i = 1; i < CharsetCount(); i++)
		d.Add(i, CharsetName(i));
}

class FontSelectManager {
	DropList *face;
	DropList *height;
	Option   *bold;
	Option   *italic;
	Option   *naa;

	void FaceSelect();
	void Select();

public:
	Callback  WhenAction;

	typedef FontSelectManager CLASSNAME;

	void Set(DropList& _face, DropList& _height,
	         Option& _bold, Option& _italic, Option& _naa);
	void Set(Font f);
	Font Get();
};

void FontSelectManager::FaceSelect() {
	int f = face->GetData();

	int h = -1;
	int lh = height->GetData();
	int lhh = -1;

	height->Clear();
#ifdef PLATFORM_WIN32
	int i;
	for(i = 1; i < 32; i++) {
		FontInfo fi = Font(f, i).Info();
		int hh = fi.GetHeight();
		if(hh != h) {
			height->Add(hh);
			if(hh >= lh && lhh < 0)
				lhh = hh;
			h = hh;
		}
	}
	if(lhh >= 0)
		height->SetData(lhh);
	else
	if(height->GetCount())
		height->SetIndex(0);
#else
	for(int i = 6; i < 32; i++)
		height->Add(i);
#endif
	Select();
}

void FontSelectManager::Select() {
	WhenAction();
}

void FontSelectManager::Set(DropList& _face, DropList& _height,
                            Option& _bold, Option& _italic, Option& _naa) {
	face = &_face;
	face->WhenAction = THISBACK(FaceSelect);
	height = &_height;
	height->WhenAction = THISBACK(Select);
	bold = &_bold;
	bold->WhenAction = THISBACK(Select);
	italic = &_italic;
	italic->WhenAction = THISBACK(Select);
	naa = &_naa;
	naa->WhenAction = THISBACK(Select);
	face->Clear();
	for(int i = 0; i < Font::GetFaceCount(); i++)
		if(Font::GetFaceInfo(i) & Font::FIXEDPITCH) {
			face->Add(i, Font::GetFaceName(i));
			LLOG("Face: " << Font::GetFaceName(i));
		}
	face->SetIndex(0);
	FaceSelect();
}

void FontSelectManager::Set(Font f) {
	face->SetData(f.GetFace());
	FaceSelect();
	height->SetData(f.GetHeight());
	for(int i = 0; i < height->GetCount(); i++) {
		int q = height->GetKey(i);
		if(f.GetHeight() <= q) {
			height->SetData(q);
			break;
		}
	}
	*bold = f.IsBold();
	*italic = f.IsItalic();
	*naa = f.IsNonAntiAliased();
}

Font FontSelectManager::Get() {
	Font f(face->GetData(), height->GetData());
	if(*bold) f.Bold();
	if(*italic) f.Italic();
	if(*naa) f.NonAntiAliased();
	return f;
}

void Ide::UpdateFormat(CodeEditor& editor, IdeQuickTabs& tabs)
{
	if(!IsActiveFile() || ActiveFile().tabsize <= 0)
		editor.TabSize(editortabsize);
	if(filetabs)
		editor.SetFrame(tabs);
	else
		editor.SetFrame(FieldFrame());
	editor.IndentSpaces(indent_spaces);
	editor.IndentAmount(indent_amount);
	editor.ShowTabs(show_tabs);
	editor.NoParenthesisIndent(no_parenthesis_indent);
	editor.HiliteScope(hilite_scope);
	editor.HiliteBracket(hilite_bracket);
	editor.HiliteIfDef(hilite_ifdef);
	editor.HiliteIfEndif(hilite_if_endif);
	editor.LineNumbers(line_numbers);
	editor.AutoEnclose(auto_enclose);
	editor.BorderColumn(bordercolumn, bordercolor);
	editor.Refresh();
}

void Ide::UpdateFormat() {
	SetupEditor();
	UpdateFormat(editor, tabs);
	UpdateFormat(editor2, tabs2);
	console.SetFont(consolefont);
	console.WrapText(wrap_console_text);
	statusbar.Show(show_status_bar);
	SetupBars();
}

struct FormatDlg : TabDlg {
	ColorPusher hl_color[CodeEditor::HL_COUNT];
};

void HlPusherFactory(One<Ctrl>& ctrl)
{
	ctrl.Create<ColorPusher>().NotNull().Track();
}

void Ide::ReadHlStyles(ArrayCtrl& hlstyle)
{
	hlstyle.Clear();
	for(int i = 0; i < CodeEditor::HL_COUNT; i++) {
		const CodeEditor::HlStyle& s = editor.GetHlStyle(i);
		hlstyle.Add(editor.GetHlName(i), s.color, s.bold, s.italic, s.underline);
	}
}

void Ide::SetupFormat() {
	FormatDlg dlg;
	dlg.Title("Format setup");
	WithSetupFontLayout<ParentCtrl> fnt;
	WithSetupHlLayout<ParentCtrl> hlt;
	WithSetupEditorLayout<ParentCtrl> edt;
	WithSetupIdeLayout<ParentCtrl> ide;
	dlg.Add(fnt, "Fonts");
	dlg.Add(hlt, "Syntax highlighting");
	dlg.Add(edt, "Editor");
	dlg.Add(ide, "IDE");
	dlg.WhenClose = dlg.Acceptor(IDYES);
	FontSelectManager ed, vf, con, f1, f2;
	ed.Set(fnt.face, fnt.height, fnt.bold, fnt.italic, fnt.naa);
	vf.Set(fnt.vface, fnt.vheight, fnt.vbold, fnt.vitalic, fnt.vnaa);
	con.Set(fnt.cface, fnt.cheight, fnt.cbold, fnt.citalic, fnt.cnaa);
	f1.Set(fnt.face1, fnt.height1, fnt.bold1, fnt.italic1, fnt.naa1);
	f2.Set(fnt.face2, fnt.height2, fnt.bold2, fnt.italic2, fnt.naa2);
	ed.Set(editorfont);
	vf.Set(veditorfont);
	con.Set(consolefont);
	f1.Set(font1);
	f2.Set(font2);
	DlCharset(edt.charset);
	edt.tabsize.MinMax(1, 100).NotNull();
	edt.tabsize <<= editortabsize;
	edt.indent_amount.MinMax(1, 100).NotNull();
	edt.indent_amount <<= indent_spaces ? indent_amount : editortabsize;
	edt.indent_amount.Enable(indent_spaces);
	CtrlRetriever rtvr;
	int hs = hilite_scope;
	rtvr
		(hlt.hilite_scope, hs)
		(hlt.hilite_bracket, hilite_bracket)
		(hlt.hilite_ifdef, hilite_ifdef)
		(hlt.hilite_if_endif, hilite_if_endif)
		(edt.indent_spaces, indent_spaces)
		(edt.no_parenthesis_indent, no_parenthesis_indent)
		(edt.showtabs, show_tabs)
		(edt.filetabs, filetabs)
		(edt.numbers, line_numbers)
		(edt.bookmark_pos, bookmark_pos)
		(edt.header_guards, header_guards)
		(edt.insert_include, insert_include)
		(edt.auto_enclose, auto_enclose)
		(edt.bordercolumn, bordercolumn)
		(edt.bordercolor, bordercolor)
		(ide.show_status_bar, show_status_bar)
		(ide.toolbar_in_row, toolbar_in_row)
		(ide.splash_screen, splash_screen)
		(ide.mute_sounds, mute_sounds)
		(ide.wrap_console_text, wrap_console_text)
		(ide.browser_clickpos, browser.clickpos)
		(ide.browser_closeesc, browser_closeesc)
		(ide.assist, editor.auto_assist)
		(ide.commentdp, editor.commentdp)
		(ide.hydra1_threads, hydra1_threads)
		(ide.chstyle, chstyle)
	;
	hlt.hlstyle.AddColumn("Style");
	hlt.hlstyle.AddColumn("Color").Ctrls(HlPusherFactory);
	hlt.hlstyle.AddColumn("Bold").Ctrls<Option>();
	hlt.hlstyle.AddColumn("Italic").Ctrls<Option>();
	hlt.hlstyle.AddColumn("Underline").Ctrls<Option>();
	hlt.hlstyle.ColumnWidths("130 70 40 40 70");
	hlt.hlstyle.EvenRowColor().NoHorzGrid().SetLineCy(EditField::GetStdHeight() + 2);
	ReadHlStyles(hlt.hlstyle);
	edt.charset <<= (int)default_charset;
	edt.tabsize <<= rtvr <<=
		hlt.hlstyle.WhenCtrlsAction = ed.WhenAction =
		con.WhenAction = f1.WhenAction = f2.WhenAction = dlg.Breaker(222);
	hlt.hl_restore <<= dlg.Breaker(333);
	for(int i = 0; i < ChGetStyleCount(); i++)
		ide.chstyle.Add(i, ChGetStyleName(i));
	for(;;) {
		int c = dlg.Run();
		editorfont = ed.Get();
		veditorfont = vf.Get();
		consolefont = con.Get();
		font1 = f1.Get();
		font2 = f2.Get();
		editortabsize = Nvl((int)~edt.tabsize, 4);
		rtvr.Retrieve();
		console.SetSlots(hydra1_threads);
		hilite_scope = hs;
		if(indent_spaces)
			indent_amount = ~edt.indent_amount;
		else
		{
			indent_amount = editortabsize;
			edt.indent_amount <<= editortabsize;
		}
		edt.indent_amount.Enable(indent_spaces);
		default_charset = (byte)(int)~edt.charset;
		for(int i = 0; i < CodeEditor::HL_COUNT; i++)
			editor.SetHlStyle(i, hlt.hlstyle.Get(i, 1), hlt.hlstyle.Get(i, 2),
			                     hlt.hlstyle.Get(i, 3), hlt.hlstyle.Get(i, 4));
		UpdateFormat();
		if(c == IDYES)
			break;
		if(c == 333 && PromptYesNo("Restore default highlighting colors?")) {
			editor.DefaultHlStyles();
			ReadHlStyles(hlt.hlstyle);
		}
	}
	if(filelist.IsCursor()) {
		FlushFile();
		FileCursor();
	}
	if(editor.auto_assist)
		StartBrowserBase();
	SyncCh();
}

struct MainConfigDlg : public WithConfigLayout<TopWindow> {
	EditString ce, fe;
	FrameRight<Button> cb;

	void FlagDlg();

	bool Perform(const String& startwith);

	typedef MainConfigDlg CLASSNAME;

	MainConfigDlg();
};

bool SetSw(const String& flag, Switch& sw, const char *flg) {
	if(flag == flg) {
		sw <<= 1;
		return true;
	}
	return false;
}

String GetSw(Switch& sw, const char *flag) {
	if((int)~sw)
		return String(flag) + ' ';
	return Null;
}

void MainConfigDlg::FlagDlg()
{
	WithConfLayout<TopWindow> cfg;
	CtrlLayoutOKCancel(cfg, "Configuration flags");
	Vector<String> flg = SplitFlags0(String(fe));
	cfg.other.SetFilter(FlagFilterM);
	cfg.dll <<= cfg.gui <<= cfg.mt <<= 0;
	for(int i = 0; i < flg.GetCount(); i++) {
		String f = flg[i];
		if(!SetSw(f, cfg.dll, "DLL")
		   && !SetSw(f, cfg.gui, "GUI")
		   && !SetSw(f, cfg.mt, "MT")) {
			String s = cfg.other.GetText().ToString();
			if(!s.IsEmpty())
				s << ' ';
			s << f;
			cfg.other = s.ToWString();
		}
	}
	if(cfg.Run() == IDOK) {
		String flags;
		flags
			<< GetSw(cfg.dll, "DLL")
		    << GetSw(cfg.gui, "GUI")
		    << GetSw(cfg.mt, "MT")
			<< cfg.other.GetText().ToString();
		;
		fe = Join(SplitFlags0(flags), " ").ToWString();
	}
}

MainConfigDlg::MainConfigDlg() {
	CtrlLayoutOKCancel(*this, "Main package configuration(s)");
	fe.AddFrame(cb);
	fe.SetFilter(FlagFilterM);
	cb.SetImage(CtrlImg::smallright()).NoWantFocus();
	cb <<= THISBACK(FlagDlg);
	list.AddColumn("Flags", 3).Edit(fe);
	list.AddColumn("Optional name", 2).Edit(ce);
	list.Appending().Removing();
}

bool MainConfigDlg::Perform(const String& startwith) {
	list.SetCursor(0);
	list.FindSetCursor(startwith);
	return Run() == IDOK;
}

void Ide::MainConfig() {
	package.SetCursor(0);
	if(package.GetCursor() != 0) return;
	MainConfigDlg dlg;
	for(int i = 0; i < actual.config.GetCount(); i++) {
		const Package::Config& f = actual.config[i];
		dlg.list.Add(f.param, f.name);
	}
	if(!dlg.Perform(mainconfigname)) return;
	actual.config.Clear();
	for(int i = 0; i < dlg.list.GetCount(); i++) {
		Package::Config& f = actual.config.Add();
		f.param = dlg.list.Get(i, 0);
		f.name = dlg.list.Get(i, 1);
	}
	SavePackage();
	if(dlg.list.IsCursor()) {
		mainconfigparam = dlg.list.Get(0);
		mainconfigname = Nvl((String)dlg.list.Get(1), mainconfigparam);
		MakeTitle();
	}
	SyncMainConfigList();
	SetHdependDirs();
	MakeTitle();
}
