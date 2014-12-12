#include <CtrlLib/CtrlLib.h>
#include <PainterSvg/PainterSvg.h>

using namespace Upp;

class SvgView : public TopWindow {
public:
	virtual bool Key(dword key, int);

private:
	ImageCtrl             img;
	FileList              files;
	Splitter              splitter;
	StaticRect            view;
	String                dir;
	FrameTop<Button>      dirup;

	void Load(const char *filename);
	void Enter();
	void DoDir();
	void DirUp();

public:
	typedef SvgView CLASSNAME;

	void Serialize(Stream& s);
	void LoadDir(const char *d);
	void LoadDir()                      { LoadDir(dir); }

	SvgView();
};

void SvgView::Load(const char *filename)
{
	String svg = LoadFileBOM(filename);
	Size sz = view.GetSize();
	Rectf f = GetSvgSize(svg);
	Size isz = GetFitSize(Size(ceil(f.GetWidth()), fceil(f.GetHeight())), sz);
	ImageBuffer ib(isz);
	BufferPainter sw(ib);
	sw.Clear(White());
	sw.Scale(isz.cx / f.GetWidth());
	sw.Translate(-f.left, -f.top);
	ParseSVG(sw, svg, ""/*GetFileFolder(filename)*/);
	img.SetImage(ib);
	img.LeftPos(0, isz.cx).TopPos(0, isz.cy);
}

void SvgView::LoadDir(const char *d)
{
	files.Clear();
	dir = d;
	Title(dir);
	::Load(files, dir, "*.*");
	SortByExt(files);
}

void SvgView::DirUp()
{
	String n = DirectoryUp(dir);
	LoadDir(dir);
	files.FindSetCursor(n);
}

void SvgView::Enter()
{
	if(!files.IsCursor()) {
		Title(dir);
		return;
	}
	const FileList::File& f = files.Get(files.GetCursor());
	if(f.name == "..") {
		Title(dir);
		return;
	}
	String p = AppendFileName(dir, f.name);
	Title(p);
	if(!f.isdir)
		Load(p);
}

void SvgView::DoDir()
{
	if(!files.IsCursor())
		return;
	const FileList::File& f = files.Get(files.GetCursor());
	if(!f.isdir)
		return;
	LoadDir(AppendFileName(dir, f.name));
}

bool SvgView::Key(dword key, int)
{
	if(key == K_ENTER) {
		DoDir();
		return true;
	}
	return false;
}

void SvgView::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	SerializePlacement(s);
	files.SerializeSettings(s);
	s % dir;
	s % splitter;
}

SvgView::SvgView()
{
	splitter.SetPos(2700);
	Add(splitter.SizePos());

	files.WhenEnterItem = THISBACK(Enter);
	files.WhenLeftDouble = THISBACK(DoDir);
	dirup.Height(max(CtrlImg::DirUp().GetSize().cy, Draw::GetStdFontCy() + 6));
	dirup.SetImage(CtrlImg::DirUp());
	dirup.NormalStyle();
	dirup <<= THISBACK(DirUp);
	files.AddFrame(dirup);

	Sizeable().Zoomable();
	
	splitter.Horz(files, view);
	view.Add(img);

	dir = GetCurrentDirectory();
	dir = GetDataFile("svg");
}

GUI_APP_MAIN
{
	SvgView x;
	LoadFromFile(x);
	x.LoadDir();
	x.Run();
	StoreToFile(x);
}
