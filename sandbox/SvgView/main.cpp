#include <CtrlLib/CtrlLib.h>
#include <PainterSvg/PainterSvg.h>

using namespace Upp;

struct SvgView : public TopWindow {
	virtual bool Key(dword key, int);
	virtual void Paint(Draw& w);

	String                svg;
	FileList              files;
	SplitterFrame         splitter;
	String                dir;
	FrameTop<Button>      dirup;

	void Load(const char *filename);
	void Enter();
	void DoDir();
	void DirUp();

	typedef SvgView CLASSNAME;

	void Serialize(Stream& s);
	void LoadDir(const char *d);
	void LoadDir()                      { LoadDir(dir); }

	SvgView();
};

void SvgView::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, Gray());
	Rectf f = GetSvgBoundingBox(svg);
	DLOG("###### BOUNDINGBOX " << f);
	Sizef iszf = GetFitSize(f.GetSize(), Sizef(sz.cx, sz.cy) - 10.0);
	Size isz((int)ceil(iszf.cx), (int)ceil(iszf.cy));
	if(isz.cx <= 0 || isz.cy <= 0)
		return;
	ImageBuffer ib(isz);
	BufferPainter sw(ib);
	sw.Clear(White());
	sw.Scale(min(isz.cx / f.GetWidth(), isz.cy / f.GetHeight()));
	sw.Translate(-f.left, -f.top);
	ParseSVG(sw, svg, ""/*GetFileFolder(filename)*/);
	Point p = Rect(sz).CenterPos(isz);
	w.DrawImage(p.x, p.y, ib);
}

void SvgView::Load(const char *filename)
{
	LOG("================= " << filename);
	svg = LoadFileBOM(filename);
	Refresh();
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
	files.WhenEnterItem = THISBACK(Enter);
	files.WhenLeftDouble = THISBACK(DoDir);
	dirup.Height(max(CtrlImg::DirUp().GetSize().cy, Draw::GetStdFontCy() + 6));
	dirup.SetImage(CtrlImg::DirUp());
	dirup.NormalStyle();
	dirup <<= THISBACK(DirUp);
	files.AddFrame(dirup);

	Sizeable().Zoomable();
	
	AddFrame(splitter.Left(files, 200));

	dir = GetCurrentDirectory();
	dir = GetDataFile("svg");
}

GUI_APP_MAIN
{
	StreamRaster::LoadFileAny("asdfasdfasdf");

	SvgView x;
	LoadFromFile(x);
	x.LoadDir();
	
	x.files.Find("dashtest.svg");
	
	x.Run();
	StoreToFile(x);
}
