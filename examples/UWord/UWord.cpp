#include <RichEdit/RichEdit.h>
#include <PdfDraw/PdfDraw.h>

#define IMAGECLASS UWordImg
#define IMAGEFILE  <UWord/UWord.iml>
#include <Draw/iml.h>

FileSel& UWordFs()
{
	static FileSel fs;
	return fs;
}

FileSel& PdfFs()
{
	static FileSel fs;
	return fs;
}

class UWord : public TopWindow {
protected:
	RichEdit   editor;
	MenuBar    menubar;
	ToolBar    toolbar;
	StatusBar  statusbar;
	String     filename;
	
	void Load(const String& filename);
	void New();
	void Open();
	void Save();
	void SaveAs();
	void Print();
	void Pdf();
	void About();
	void Destroy();
	void SetBar();
	void FileBar(Bar& bar);
	void AboutMenu(Bar& bar);
	void MainMenu(Bar& bar);
	void MainBar(Bar& bar);

public:
	typedef UWord CLASSNAME;

	UWord();
};

void UWord::FileBar(Bar& bar)
{
	bar.Add("New", CtrlImg::new_doc(), THISBACK(New))
	   .Key(K_CTRL_N)
	   .Help("Open new window");
	bar.Add("Open..", CtrlImg::open(), THISBACK(Open))
	   .Key(K_CTRL_O)
	   .Help("Open existing document");
	bar.Add(editor.IsModified(), "Save", CtrlImg::save(), THISBACK(Save))
	   .Key(K_CTRL_S)
	   .Help("Save current document");
	bar.Add("SaveAs", CtrlImg::save_as(), THISBACK(SaveAs))
	   .Help("Save current document with a new name");
	bar.ToolGap();
	bar.MenuSeparator();
	bar.Add("Print..", CtrlImg::print(), THISBACK(Print))
	   .Key(K_CTRL_P)
	   .Help("Print document");
	#ifdef PLATFORM_WIN32
	bar.Add("Export to PDF..", UWordImg::pdf(), THISBACK(Pdf))
	   .Help("Export document to PDF file");
	#endif
	if(bar.IsMenuBar()) {
		bar.Separator();
		bar.Add("Exit", THISBACK(Destroy));
	}
}

void UWord::AboutMenu(Bar& bar)
{
	bar.Add("About..", THISBACK(About));
	bar.Separator();
	bar.Add("What is this ?", callback(PerformDescription))
	   .Key(K_SHIFT_F1);
}

void UWord::MainMenu(Bar& bar)
{
	bar.Add("File", THISBACK(FileBar));
	bar.Add("Window", callback(WindowsMenu));
	bar.Add("Help", THISBACK(AboutMenu));
}

void UWord::New()
{
	new UWord;
}

void UWord::Load(const String& name)
{
	editor.SetQTF(LoadFile(name));
	filename = name;
	editor.ClearModify();
	Title(filename);
}

void UWord::Open()
{
	FileSel& fs = UWordFs();
	if(fs.ExecuteOpen())
		if(filename.IsEmpty() && !editor.IsModified())
			Load(fs);
		else
			(new UWord)->Load(fs);
	else
		statusbar.Temporary("Loading aborted.");
}

void UWord::Save()
{
	if(!editor.IsModified()) return;
	if(filename.IsEmpty())
		SaveAs();
	else
		if(SaveFile(filename, editor.GetQTF())) {
			statusbar.Temporary("File " + filename + " was saved.");
			ClearModify();
		}
		else
			Exclamation("Error saving the file [* " + DeQtf(filename) + "]!");
}

void UWord::SaveAs()
{
	FileSel& fs = UWordFs();
	if(fs.ExecuteSaveAs()) {
		filename = fs;
		Title(filename);
		Save();
	}
}

void UWord::Print()
{
	editor.Print();
}

void UWord::Pdf()
{
	#ifdef PLATFORM_WIN32
	FileSel& fs = PdfFs();
	if(!fs.ExecuteSaveAs("Output PDF file"))
		return;
	Size page = Size(3968, 6047);
	PdfDraw pdf;
	::Print(pdf, editor.Get(), page);
	SaveFile(~fs, pdf.Finish());
	#endif
}

void UWord::About()
{
	PromptOK("[A5 uWord]&Using [*^www://upp.sf.net^ Ultimate`+`+] technology.");
}

void UWord::Destroy()
{
	if(editor.IsModified()) {
		switch(PromptYesNoCancel("Do you want to save the changes to the document?")) {
		case 1:
			Save();
			break;
		case -1:
			return;
		}
	}
		
	delete this;
}

void UWord::MainBar(Bar& bar)
{
	FileBar(bar);
	bar.Separator();
	editor.DefaultBar(bar);
}

void UWord::SetBar()
{
	toolbar.Set(THISBACK(MainBar));
}

UWord::UWord()
{
	AddFrame(menubar);
	AddFrame(TopSeparatorFrame());
	AddFrame(toolbar);
	AddFrame(statusbar);
	Add(editor.SizePos());
	menubar.Set(THISBACK(MainMenu));
	Sizeable().Zoomable();
	WhenClose = THISBACK(Destroy);
	menubar.WhenHelp = toolbar.WhenHelp = statusbar;
	static int doc;
	Title(Format("Document%d", ++doc));
	Icon(CtrlImg::File());
	editor.ClearModify();
	editor <<= THISBACK(SetBar);
	SetBar();
	editor.WhenRefreshBar = THISBACK(SetBar);
	OpenMain();
	ActiveFocus(editor);
}

void SerializeApp(Stream& s)
{
	int version = 0;
	s / version;
	s % UWordFs()
	  % PdfFs();
}

GUI_APP_MAIN
{
	SetLanguage(LNG_ENGLISH);

	UWordFs().Type("QTF files", "*.qtf")
	         .AllFilesType()
	         .DefaultExt("qtf");
	PdfFs().Type("PDF files", "*.pdf")
	       .AllFilesType()
	       .DefaultExt("pdf");

	LoadFromFile(callback(SerializeApp));
	new UWord;
	Ctrl::EventLoop();
	StoreToFile(callback(SerializeApp));
}
