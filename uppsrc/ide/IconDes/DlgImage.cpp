#include "IconDes.h"
#pragma hdrstop

#include <Image/Image.h>
#include <Draw/PixelUtil.h>
#include <plugin/gif/gif.h>
#include <plugin/jpg/jpg.h>
#include <plugin/png/png.h>
#include <plugin/tif/tif.h>

#define LAYOUTFILE <ide/IconDes/panel.lay>
#include <CtrlCore/lay.h>

#define LID(nm) static Id nm(#nm);

LID(CNAME) LID(CWIDTH) LID(CHEIGHT) LID(CFORMAT) LID(CHOTX) LID(CHOTY)
LID(CDATA) LID(CDESC)

void SwapRows(ArrayCtrl& ctrl, int a, int b)
{
	if(a == b)
		return;
	if(a > b)
		Swap(a, b);
	Vector<Value> brow = ctrl.ReadRow(b);
	Vector<Value> arow = ctrl.ReadRow(a);
	ctrl.Set(a, brow);
	ctrl.Set(b, arow);
}

bool SaveChangedFile(String fn, String data)
{
	return LoadFile(fn) == data || SaveFile(fn, data);
}

INITBLOCK
{
	JpgEncoder::Register();
	PngEncoder::Register();
	TifEncoder::Register();
	GifEncoder::Register();
	BmpEncoder::Register();
}

template <class T>
inline void SerializeImageEnum(Stream& stream, T& x)
{
	int i = (int)x;
	stream / i;
	x = (T)i;
}

bool IsPasteAvailable()
{
#ifdef PLATFORM_WIN32
	return ClipboardFormat<AlphaArray>::IsAvailable()
		|| IsClipboardFormatAvailable(CF_DIB);
#else
	return false; // todo
#endif
}

class Setup
{
public:
	Setup();

	void                NewFile();
	void                Serialize(Stream& stream);

	bool                Edit();

public:
	bool                split_horz;
	bool                paste_transparent;
	bool                high_list;
	bool                wince_16bit;

	// grid setup
	int                 grid_zoom;
	int                 grid;
	Color               grid_color;

	// file setup
	String              filename;
	String              header_file;
	String              wrapper_name;
	enum WRAPPER { NO_WRAPPER, CLASS_WRAPPER, SPACE_WRAPPER };
	int                 wrapper_kind;

	enum JAVA_EXPORT { NO_EXPORT, FILE_EXPORT, CPP_EXPORT };
	int                 java_export;
	String              java_source;
	String              java_package;

	String              gif_folder;
	Color               gif_transparent;

	String              icon_file;
	String              icon_large, icon_small;
	int                 icon_bpp;
};

Setup::Setup()
: split_horz(false)
, paste_transparent(true)
, high_list(true)
, wince_16bit(false)
, grid(ImageEditCtrl::SINGLE)
, grid_color(Black)
, grid_zoom(16)
{
	NewFile();
}

void Setup::NewFile()
{
	wince_16bit = false;
	filename = Null;
	header_file = Null;
	wrapper_kind = NO_WRAPPER;
	wrapper_name = Null;
	java_export = NO_EXPORT;
	java_source = Null;
	java_package = Null;
	gif_folder = Null;
	gif_transparent = Null;
	icon_file = Null;
	icon_large = icon_small = Null;
	icon_bpp = 8;
}

void Setup::Serialize(Stream& stream)
{
	int version = StreamHeading(stream, 10, 8, 10, "Setup");
	if(version >= 8)
	{
		stream.Pack(split_horz, paste_transparent);
		SerializeImageEnum(stream, grid);
		stream % grid_color;
		stream % header_file;
		SerializeImageEnum(stream, wrapper_kind);
		stream % wrapper_name;
		stream % high_list;
	}
	if(version >= 9)
		stream % wince_16bit;
	if(version >= 10)
		stream % grid_zoom;
}

class DlgSetup : public WithSetupLayout<TopWindow>
{
public:
	typedef DlgSetup CLASSNAME;
	DlgSetup();

	bool Run(Setup& setup);

private:
	void OnWrapperKind();
	void OnGridColor();
	void OnBrowse();
	void OnAbsPath();
	void OnRelPath();
	void OnJavaExport();
	void OnBrowseJavaSource();

private:
	FrameRight<Button> browse;
	String filename;
	FrameRight<Button> browse_java;
};

DlgSetup::DlgSetup()
{
	CtrlLayoutOKCancel(*this, t_("Properties"));
	wrapper_kind.Add(Setup::NO_WRAPPER, t_("global"));
	wrapper_kind.Add(Setup::CLASS_WRAPPER, t_("class"));
	wrapper_kind.Add(Setup::SPACE_WRAPPER, t_("namespace"));
	wrapper_kind <<= THISBACK(OnWrapperKind);
	grid_color <<= THISBACK(OnGridColor);
	header_file.AddFrame(browse);
	browse.SetImage(CtrlImg::right_arrow());
	browse <<= THISBACK(OnBrowse);
	abs_path <<= THISBACK(OnAbsPath);
	rel_path <<= THISBACK(OnRelPath);
	java_export <<= THISBACK(OnJavaExport);
	java_source.AddFrame(browse_java);
	browse_java.SetImage(CtrlImg::right_arrow());
	browse_java <<= THISBACK(OnBrowseJavaSource);
	gif_transparent.NoNotNull();
	icon_bpp.Add(1, t_("monochrome"));
	icon_bpp.Add(4, t_("16 colors"));
	icon_bpp.Add(8, t_("256 colors"));
	icon_bpp.Add(16, t_("High-color (16 bits/pixel)"));
	icon_bpp.Add(24, t_("True-color (24 bits/pixel)"));
}

bool DlgSetup::Run(Setup& setup)
{
	CtrlRetriever rtvr;
	rtvr
		(split_horz,        setup.split_horz)
		(paste_transparent, setup.paste_transparent)
		(high_list,         setup.high_list)
		(wince_16bit,       setup.wince_16bit)
		(grid,              setup.grid)
		(grid_color,        setup.grid_color)
		(header_file,       setup.header_file)
		(wrapper_kind,      setup.wrapper_kind)
		(wrapper_name,      setup.wrapper_name)
		(java_export,       setup.java_export)
		(java_source,       setup.java_source)
		(java_package,      setup.java_package)
		(gif_folder,        setup.gif_folder)
		(gif_transparent,   setup.gif_transparent)
		(icon_file,         setup.icon_file)
		(icon_large,        setup.icon_large)
		(icon_small,        setup.icon_small)
		(icon_bpp,          setup.icon_bpp)
	;
	filename = NormalizePath(setup.filename);
	rel_path.Enable(!IsNull(filename));
	OnWrapperKind();
	OnJavaExport();
	if(TopWindow::Run() != IDOK)
		return false;
	rtvr.Retrieve();
	return true;
}

void DlgSetup::OnBrowse()
{
	String s = ~header_file;
	if(IsNull(s) && !IsNull(filename))
		s = GetFileDirectory(filename) + GetFileTitle(filename) + "Img.h";
	FileSelector fsel;
	fsel.Type(t_("Prototypes (*.h)"), "*.h");
	fsel.DefaultExt("h");
	fsel <<= s;
	if(fsel.ExecuteSaveAs(t_("Save prototype as...")))
	{
		header_file <<= ~fsel;
		OnRelPath();
	}
}

void DlgSetup::OnJavaExport()
{
	bool en = ((int)~java_export != Setup::NO_EXPORT);
	java_source.Enable(en);
	java_package.Enable(en);
}

void DlgSetup::OnBrowseJavaSource()
{
	FileSelector fs;
	fs.Type(t_("Java source files (*.java)"), "*.java")
		.DefaultExt("java");
	fs <<= ~java_source;
	if(fs.ExecuteSaveAs(t_("Save Java source file as...")))
		java_source <<= ~fs;
}

void DlgSetup::OnAbsPath()
{
	if(!IsNull(filename))
		header_file <<= NormalizePath(String(~header_file), GetFileDirectory(filename));
}

void DlgSetup::OnRelPath()
{
	if(!IsNull(filename))
	{
		OnAbsPath();
		String fn = ~header_file;
		if(fn.GetLength() <= 3 || filename.GetLength() <= 3
			|| fn[2] != '\\' || filename[2] != '\\'
			|| fn[1] != ':' || filename[1] != ':'
			|| fn[0] != filename[0]) // different drive
			;
		else
		{ // same drive
			const char *d = fn.Begin() + 3, *s = filename.Begin() + 3;
			for(;;)
			{
				const char *de = d, *se = s;
				while(*de && *de != '\\')
					de++;
				while(*se && *se != '\\')
					se++;
				if(!*se)
				{ // dest in subfolder of source or both in same folder
					fn = String(d);
					break;
				}
				if(*de && de - d == se - s && !ComparePath(d, s, de - d))
				{ // same folder
					d = de + 1;
					s = se + 1;
					continue;
				}
				// different folders or d is the terminator - generate '..' sequence
				String tail = d;
				fn = "..\\";
				int back = 1;
				while(*++se)
					if(*se == '\\')
						fn << "..\\";
				fn << tail;
				break;
			}
		}
		header_file <<= fn;
	}
}

void DlgSetup::OnGridColor()
{
	grid <<= ImageEditCtrl::SINGLE;
}

void DlgSetup::OnWrapperKind()
{
	wrapper_name.Enable((int)~wrapper_kind != Setup::NO_WRAPPER);
}

bool Setup::Edit()
{
	return DlgSetup().Run(*this);
}

enum
{
	MAX_UNDO = 20,
	LISTWIDTH = 100,
};

class IconDesigner;

class DlgImage : public TopWindow, public Convert
{
public:
	typedef DlgImage CLASSNAME;
	DlgImage(IconDesigner *owner);

	String              ConfigName()        { return "icondes-ctrl"; }
	void                LoadConfig();
	String              GetFileName() const { return setup.filename; }
	void                Save();
//	virtual void        SyncUsc()           {}
//	virtual void        ActivateDesigner()  { SetForeground(); }
	void                SaveEditPos()       { FileMap().GetAdd(setup.filename) = GetEditPos(); }
	void                EditMenu(Bar& menu) { ToolMain(menu); }

	virtual Value       Format(const Value& value) const;

	struct EditPos
	{
		EditPos() : index(-1) {}

		int    index;
		Color  color;
		String color_cfg;
	};

	typedef ArrayMap<String, EditPos> FileEditPosMap;
	static FileEditPosMap& FileMap();

	EditPos             GetEditPos();
	void                RestoreEditPos(EditPos ep);

	void                OpenWindow();
	void                Run();

	virtual bool        HotKey(dword key);

	void                Serialize(Stream& stream);

	bool                FlushFile();
	bool                DoLoad(String filename, bool warn_ne);
	bool                DoSave(String filename);
	bool                DoSaveFile() { return DoSave(setup.filename); }

private:
	void                ToolMain(Bar& bar);

//	void                ToolFile(Bar& bar);

//	void                ToolFileNew(Bar& bar);
//	void                OnFileNew();
//	void                ToolFileOpen(Bar& bar);
//	void                OnFileOpen();
//	void                ToolFileSave(Bar& bar);
//	void                OnFileSave();
//	void                ToolFileSaveAs(Bar& bar);
//	void                OnFileSaveAs();
//	void                ToolFileExport(Bar& bar);
//	void                OnFileExport();
//	void                ToolFileQuit(Bar& bar);
//	void                OnFileQuit();

	void                OnFileRecent(const String& s) { if(FlushFile()) DoLoad(s, true); }

	void                ToolEdit(Bar& bar);

	void                ToolEditMoveUp(Bar& bar);
	void                OnEditMoveUp();
	void                ToolEditMoveDown(Bar& bar);
	void                OnEditMoveDown();
	void                ToolEditInsert(Bar& bar);
	void                OnEditInsert();
	void                ToolEditDuplicate(Bar& bar);
	void                OnEditDuplicate();
	void                ToolEditInsertPaste(Bar& bar);
	void                OnEditInsertPaste();
	void                ToolEditInsertFile(Bar& bar);
	void                OnEditInsertFile();
	void                ToolEditSplit(Bar& bar);
	void                OnEditSplit();
	void                ToolEditDelete(Bar& bar);
	void                OnEditDelete();
	void                ToolEditCrop(Bar& bar);
	void                OnEditCrop();

	void                ToolEditPaste(Bar& bar);
	void                OnEditPaste();

	void                ToolEditUndo(Bar& bar);
	void                OnEditUndo();
	void                ToolEditRedo(Bar& bar);
	void                OnEditRedo();

	void                OnEditUndoList();
	void                OnEditUndoListSelect();

	void                OnEditRedoList();
	void                OnEditRedoListSelect();

	void                ToolSelect(Bar& bar);

	void                ToolSelectRotateClockwise(Bar& bar);
	void                OnSelectRotateClockwise();
	void                ToolSelectRotateAnticlockwise(Bar& bar);
	void                OnSelectRotateAnticlockwise();

	void                ToolMode(Bar& bar);

	void                ToolSetup(Bar& bar);

	void                ToolSetupSplit(Bar& bar);
	void                OnSetupSplit();
	void                ToolSetupProperties(Bar& bar);
	void                OnSetupProperties();

	void                UpdateSetup();
	void                UpdateImageSplitter();

	void                SetBar() { menu_bar.Set(THISBACK(ToolMain)); tool_bar.Set(THISBACK(ToolMain)); }

	void                Rescan() { SetBar(); }
//		PostCallback(THISBACK(SetBar), this);
// 	}

	void                OnCoords(String s);
	void                OnImageSize();
	void                OnImageName();
	void                OnImage();
	void                OnImageCursor(int i);
	void                OnListMenu(Bar& bar);

	void                OnSetTransparentColor();

	bool                OnImageKey(dword key);
	bool                OnNameKey(dword key);

	void                AddUndo(One<UndoEntry> entry);
	void                SetUndoAnchor();
	void                ClearUndo();
	bool                IsDirty() const { return undo_pos != undo_anchor; }

	void                WriteJava(String _iml_file, Stream& iml_stream);
	void                WriteFile(Stream& stream, String _filename);
//	void                ReadFile(String file);
	void                WriteGif(String folder, Color transparent);
	void                WriteIcon(String _file, String _small, String _large, int _bpp);

	void                SetFileName(String fn);
	void                UpdateCaption();

	void                SetImage(const AlphaArray& im, const char *command);

	AlphaImageInfo      Get(int row);
	void                Set(int row, const IMLImageInfo& data);
	int                 InsertRaw(int row, const IMLImageInfo& data);

private:
	class UndoEntrySwap : public UndoEntry
	{
	public:
		UndoEntrySwap(DlgImage& owner, const String& command, int old_pos, int new_pos)
			: owner(owner), command(command), old_pos(old_pos), new_pos(new_pos) {}

		virtual String Format() const                   { return command; }
		virtual bool   Equals(const UndoEntry& e) const { return false; }
		virtual void   Undo();
		virtual void   Redo();

	private:
		DlgImage&      owner;
		String         command;
		int            old_pos;
		int            new_pos;
	};

	friend class UndoEntrySwap;

	class UndoEntryName : public UndoEntry
	{
	public:
		UndoEntryName(DlgImage& owner, int cursor, const String& old_name, const String& new_name)
			: owner(owner), cursor(cursor), old_name(old_name), new_name(new_name) {}

		virtual String Format() const                   { return t_("Image name"); }
		virtual bool   Equals(const UndoEntry& e) const { return false; }
		virtual void   Undo();
		virtual void   Redo();

	private:
		DlgImage&      owner;
		int            cursor;
		String         old_name;
		String         new_name;
	};

	friend class UndoEntryName;

	class UndoEntryList : public UndoEntry
	{
	public:
		UndoEntryList(DlgImage& owner, const String& command)
			: owner(owner), command(command) {}

		void           Insert(int pos, const IMLImageInfo& data);
		void           Remove(int pos, const IMLImageInfo& data);

		virtual String Format() const                   { return command; }
		virtual bool   Equals(const UndoEntry& e) const { return false; }
		virtual void   Undo();
		virtual void   Redo();

	private:
		struct Item
		{
			int         cursor;
			bool        remove; // false = insert, true = remove
			IMLImageInfo image;
//			AlphaImageInfo data;
//			String      name;
//			String      rle;
		};

	private:
		Array<Item>   items;
		DlgImage&     owner;
		String        command;
	};

	friend class UndoEntryList;

	class UndoEntrySetup : public UndoEntry
	{
	public:
		UndoEntrySetup(DlgImage& owner, const Setup& old_setup, const Setup& new_setup)
			: owner(owner), old_setup(old_setup), new_setup(new_setup) {}

		virtual String Format() const  { return t_("properties"); }
		virtual bool   Equals(const UndoEntry& e) const;
		virtual void   Undo();
		virtual void   Redo();

	private:
		DlgImage&      owner;
		Setup          old_setup;
		Setup          new_setup;
	};

	friend class UndoEntrySetup;

	struct ImageListCtrl : public WithListLayout<StaticRect>
	{
		virtual bool Key(dword key, int repcnt) { return name.HasFocusDeep() && WhenKey(key); }

		Gate1<dword> WhenKey;
	};

private:
	MenuBar             menu_bar;
	ToolBar             tool_bar;
	StatusBar           status_bar;
	ToolButton          undo_btn, redo_btn;
	SizeGrip            size_grip;
	Splitter            hsplitter, vsplitter, isplitter;
	ImageListCtrl       image_list;
	PaletteCtrl         palette;
	ImageEditCtrl       image_designer;
	ImageSmallCtrl      image_small;
	EditField           coords;
	int                 old_cursor;
	String              old_name;
	LRUList             recent_files;
	String              recent_export;
	AlphaArray          image_tail;

	Array<UndoEntry>    undo;
	Rect                undo_rc, redo_rc;
	int                 undo_pos, undo_anchor;
	PopUpTable          command_list;
//	Bar                *tool_bar;
	IconDesigner       *owner;

	Setup               setup;
};

/*
static Array<DlgImage>& GetDesigners() {
	static Array<DlgImage> list;
	return list;
}
*/

GLOBAL_VAR(DlgImage::FileEditPosMap, DlgImage::FileMap);

class IconDesigner : public IdeDesigner
{
public:
	One<DlgImage>  designer;

public:
	virtual void   Close()                    { delete this; }
	virtual void   ChildGotFocus()            { /*designer->FrameFocus();*/ }
	virtual bool   Key(dword key, int repcnt) { return designer->Key(key, repcnt) || IdeDesigner::Key(key, repcnt); }
	virtual String GetFileName() const        { return designer->GetFileName(); }
	virtual void   Save()                     { designer->Save(); }
	virtual void   SyncUsc()                  {} // designer->SyncUsc(); }
	virtual void   ActivateDesigner()         { SetForeground(); }
	virtual void   SaveEditPos()              { designer->SaveEditPos(); }
	virtual void   EditMenu(Bar& menu)        { designer->EditMenu(menu); }

	void Serialize(Stream& s)                 { designer->Serialize(s); }
	bool Load(const char *filename)           { return designer->DoLoad(filename, false); }
//	void LoadConfig()                         { designer->LoadConfig(); }
	void SetConfigName(const char *name)      { /*designer->cfgname = name;*/ }

	IconDesigner()                            { designer = new DlgImage(this); Add(designer->SizePos()); }
};

//ConfigItem DlgImage::config("DlgImage", 6, 6, 6);
INITBLOCK
{
	RegisterGlobalConfig("DlgImage");
};

//void RunDlgImage() { DlgImage().Run(); }

static int CharFilterIdent(int c) { return IsAlNum(c) || c == '_' ? c : 0; }

void DlgImage::UndoEntrySwap::Undo()
{
	int sc = owner.image_list.list.GetCursorSc();
	owner.image_list.list.KillCursor();
	SwapRows(owner.image_list.list, old_pos, new_pos);
	owner.image_list.list.SetCursor(old_pos);
	owner.image_list.list.ScCursor(sc);
	owner.image_list.name.SetWantFocus();
}

void DlgImage::UndoEntrySwap::Redo()
{
	int sc = owner.image_list.list.GetCursorSc();
	owner.image_list.list.KillCursor();
	SwapRows(owner.image_list.list, old_pos, new_pos);
	owner.image_list.list.SetCursor(new_pos);
	owner.image_list.list.ScCursor(sc);
	owner.image_list.name.SetWantFocus();
}

//////////////////////////////////////////////////////////////////////
// UndoEntryName::

void DlgImage::UndoEntryName::Undo()
{
	owner.image_list.list.SetCursor(cursor);
	owner.image_list.list.Set(CNAME, old_name);
//	owner.image_list.name <<= old_name;
//	owner.image_list.list.Refresh();
}

void DlgImage::UndoEntryName::Redo()
{
	owner.image_list.list.SetCursor(cursor);
	owner.image_list.list.Set(CNAME, new_name);
//	owner.image_list.name <<= new_name;
//	owner.image_list.list.Refresh();
}

//////////////////////////////////////////////////////////////////////
// UndoEntryList::

void DlgImage::UndoEntryList::Insert(int pos, const IMLImageInfo& data)
{
	Item& item = items.Add();
	item.cursor = pos;
	item.remove = false;
	item.image  = data;
}

void DlgImage::UndoEntryList::Remove(int pos, const IMLImageInfo& data)
{
	Item& item = items.Add();
	item.cursor = pos;
	item.remove = true;
	item.image  = data;
}

void DlgImage::UndoEntryList::Undo()
{
	if(!items.IsEmpty() && owner.image_list.list.KillCursor())
	{
		for(int i = items.GetCount(); --i >= 0;)
		{
			const Item& item = items[i];
			if(item.remove && item.cursor <= owner.image_list.list.GetCount()) // reinsert item
				owner.InsertRaw(item.cursor, item.image);
			if(!item.remove && item.cursor < owner.image_list.list.GetCount()) // remove previously inserted item
				owner.image_list.list.Remove(item.cursor);
		}
		owner.image_list.list.SetCursor(minmax(items[0].cursor, 0, owner.image_list.list.GetCount() - 1));
	}
}

void DlgImage::UndoEntryList::Redo()
{
	if(!items.IsEmpty() && owner.image_list.list.KillCursor())
	{
		for(int i = 0; i < items.GetCount(); i++)
		{
			const Item& item = items[i];
			if(item.remove && item.cursor < owner.image_list.list.GetCount()) // remove item
				owner.image_list.list.Remove(item.cursor);
			if(!item.remove && item.cursor <= owner.image_list.list.GetCount()) // insert item
				owner.InsertRaw(item.cursor, item.image);
		}
		owner.image_list.list.SetCursor(minmax(items.Top().cursor, 0, owner.image_list.list.GetCount() - 1));
	}
}

//////////////////////////////////////////////////////////////////////
// UndoEntrySetup::

bool DlgImage::UndoEntrySetup::Equals(const UndoEntry& e) const
{
	return false;
//	const UndoEntrySetup *p = dynamic_cast<const UndoEntrySetup *>(&e);
//	if(e
}

void DlgImage::UndoEntrySetup::Undo()
{
	owner.setup = old_setup;
	owner.UpdateSetup();
}

void DlgImage::UndoEntrySetup::Redo()
{
	owner.setup = new_setup;
	owner.UpdateSetup();
}

class DlgImageDisplayCls : public Display
{
public:
	virtual void Paint(Draw& draw, const Rect& rc, const Value& value, Color ink, Color paper, dword style) const;
};

void DlgImageDisplayCls::Paint(Draw& draw, const Rect& rc, const Value& value, Color ink, Color paper, dword style) const
{
	draw.DrawRect(rc, paper);
	if(IsValueArray(value))
	{
		ValueArray va(value);
		Size size(va[0], va[1]);
		Point hotspot(va[2], va[3]);
		int format = va[4];
		String data = va[5];

		AlphaArray aa(size);
		RLEToAlpha(aa, data);
		AlphaKillMask(aa, paper);
		Size out_size = min(GetFitSize(size, rc.Size()), size);
		if(out_size != size)
		{
			PixelArray scaled(out_size, -3);
			PixelSet(scaled, scaled.GetRect(), paper);
			PixelCopyAntiAlias(scaled, scaled.GetRect(), aa.pixel, aa.GetRect());
			aa.pixel = scaled;
			size = out_size;
		}
		draw.Clip(rc);
		Rect dest(rc.CenterPoint() - (size >> 1), size);
/*
		if(aa.HasAlpha())
		{
			aa.alpha.Paint(draw, dest.OffsetedHorz(-1), White, Null);
			aa.alpha.Paint(draw, dest.OffsetedHorz(+1), White, Null);
			aa.alpha.Paint(draw, dest.OffsetedVert(-1), White, Null);
			aa.alpha.Paint(draw, dest.OffsetedVert(+1), White, Null);
		}
*/
		aa.Paint(draw, dest);
		draw.End();
	}
}

const Display& DlgImageDisplay()
{
	static DlgImageDisplayCls dc;
	return dc;
}

DlgImage::DlgImage(IconDesigner *owner_)
{
	owner = owner_;

	old_cursor = -1;

/*
	if(topwindow)
	{
		Sizeable().Zoomable();
		Icon(MiscImg::small_app(), MiscImg::large_app());
		Add(size_grip);
	}
	else
		SizePos();
*/

//	CalcWindowRect(*this, Ctrl::GetWorkArea().Size() * 3 >> 2);
//	WhenClose = THISBACK(OnFileQuit);

	command_list.AutoHideSb().NoHeader().NoGrid().MouseMoveCursor();
	command_list.AddIndex();
	command_list.AddColumn();
	undo_pos = 0;
	undo_anchor = 0;

	AddFrame(TopSeparatorFrame());
	AddFrame(tool_bar);
	Rescan();
//	tool_bar.Multiline();
//	tool_bar.SetFrame(BottomJoinFrame());
//	info_bar.SetFrame(EmptyFrame());
//	info_bar.Height(Ctrl::VertFontZoom(20));
//	tool_bar.AddFrame(coords);
//	coords.Width(150);
	coords.SetReadOnly();
	*this << hsplitter.SizePos();
	hsplitter.Horz(vsplitter, isplitter);
	hsplitter.SetPos(2500);
	hsplitter.NoTransparent();

	UpdateImageSplitter();
	isplitter.SetPos(8000);
	isplitter.NoTransparent();
	image_small.Attach(&image_designer);

//	palette.SetFrame(BottomJoinFrame());
	palette.WhenSetTransparent = THISBACK(OnSetTransparentColor);

	image_list.Transparent().NoWantFocus();
	CtrlLayout(image_list);
	vsplitter.Vert(image_list, palette);
	vsplitter.SetPos(5000);
	vsplitter.NoTransparent();

	image_designer.WhenCoords = THISBACK(OnCoords);
	image_designer.SetPalette(&palette);
	image_designer.WhenKey = THISBACK(OnImageKey);
	image_designer.WhenAddUndo = THISBACK(AddUndo);
	image_designer.WhenSetCursor = THISBACK(OnImageCursor);
//	image_designer.WhenUndoRedo = THISBACK(OnImageUndoRedo);

	image_list.list.AutoHideSb().NoHeader().NoVertGrid();
//	image_list.SetFrame(TopJoinFrame());
	image_list.list.AddCtrl(CNAME, image_list.name);
	image_list.list.AddCtrl(CWIDTH, image_list.width);
	image_list.list.AddCtrl(CHEIGHT, image_list.height);
	image_list.list.AddIndex(CFORMAT);
	image_list.list.AddIndex(CHOTX);
	image_list.list.AddIndex(CHOTY);
	image_list.list.AddIndex(CDATA);
	image_list.list.AddColumn("", 10)
		.Add(CNAME).Add(CWIDTH).Add(CHEIGHT).Add(CFORMAT).Add(CHOTX).Add(CHOTY)
		.SetConvert(*this);
	image_list.list.AddColumnAt(CWIDTH, "", 3)
		.Add(CHEIGHT).Add(CFORMAT).Add(CHOTX).Add(CHOTY).Add(CDATA)
		.SetDisplay(DlgImageDisplay());
	image_list.list.WhenBar = THISBACK(OnListMenu);

	image_list.width.MinMax(1, 16384).NotNull();
	image_list.height.MinMax(1, 16384).NotNull();

	image_list.height <<= image_list.width.WhenAction = THISBACK(OnImageSize);
	image_list.name <<= THISBACK(OnImageName);
	image_list.name.SetFilter(CharFilterIdent);
	image_list.WhenKey = THISBACK(OnNameKey);

	image_list.list.WhenCursor = THISBACK(OnImage);

	recent_files.WhenSelect = THISBACK(OnFileRecent);

	image_designer.WhenSyncBar = THISBACK(SetBar);
}

void DlgImage::LoadConfig()
{
	LoadFromGlobal(*this, ConfigName());
}

void DlgImage::Save()
{
	StoreToGlobal(*this, ConfigName());
	if(IsDirty())
		DoSaveFile();
}

void DlgImage::Serialize(Stream& stream)
{
	int version = StreamHeading(stream, 3, 2, 3, "DlgImage");
	if(version < 1)
		return;
	stream % recent_files;
	stream % image_designer;
	stream % hsplitter % vsplitter % isplitter;
	if(version >= 3)
		palette.SerializeConfig(stream);
	setup.grid_zoom = image_designer.GetZoom();
	stream % setup;
	if(stream.IsLoading()) {
		setup.NewFile();
		image_designer.SetZoom(setup.grid_zoom);
	}
//	SerializePlacement(stream);
}

void DlgImage::Run()
{
//	Size screen = Ctrl::GetWorkArea().Size();
//	SetRect(screen.cx >> 3, screen.cy >> 3, (screen.cx * 6) >> 3, (screen.cy * 6) >> 3);
//	OnFileNew();
	OpenWindow();
	TopWindow::Run();
}

DlgImage::EditPos DlgImage::GetEditPos()
{
	EditPos out;
	out.index = image_list.list.GetCursor();
	out.color = ~palette;
	StringStream palcfg;
	palette.SerializeConfig(palcfg);
	out.color_cfg = palcfg;
	return out;
}

void DlgImage::RestoreEditPos(EditPos ep)
{
	if(ep.index >= 0 && ep.index < image_list.list.GetCount())
		image_list.list.SetCursor(ep.index);
	palette <<= ep.color;
	StringStream palcfg(ep.color_cfg);
	palette.SerializeConfig(palcfg);
}

void DlgImage::OpenWindow()
{
	LoadFromGlobal(*this, "DlgImage");
	OpenMain();
	OnImage();
	UpdateSetup();
}

void DlgImage::ToolMain(Bar& bar)
{
//	bar.Add("Soubor", THISBACK(ToolFile));
	bar.Add(t_("Edit"), THISBACK(ToolEdit));
	if(image_list.list.IsCursor())
	{
		bar.Add(t_("Select"), THISBACK(ToolSelect));
		bar.Add(t_("Mode"), THISBACK(ToolMode));
		bar.Add(t_("View"), callback((ImageEditCtrl *)&image_designer, &ImageEditCtrl::ToolView));
	}
	bar.Add(t_("Setup"), THISBACK(ToolSetup));
	if(bar.IsToolBar())
		bar.Add(coords.SizePos(), 150);
}

void DlgImage::OnCoords(String s)
{
	if(s != String(~coords))
		coords <<= s;
}

/*
void DlgImage::ToolFile(Bar& bar)
{
	ToolFileNew(bar);
	ToolFileOpen(bar);
	ToolFileSave(bar);
	ToolFileSaveAs(bar);
	ToolFileExport(bar);
	if(bar.IsMenuBar())
		recent_files(bar);
	bar.MenuSeparator();
	ToolFileQuit(bar);
}
*/

/*
void DlgImage::ToolFileNew(Bar& bar)
{
	bar.Add("Nov??", CtrlImg::new_doc(), THISBACK(OnFileNew))
		.Key(K_CTRL_N)
		.Help("Zav?¸?­t aktu??ln?­ soubor a zalo??it nov??");
}
*/

/*
void DlgImage::OnFileNew()
{
	if(FlushFile())
	{
		image_list.list.Clear();
		setup.NewFile();
		ClearUndo();
		SetFileName(Null);
	}
}
*/

/*
void DlgImage::ToolFileOpen(Bar& bar)
{
	bar.Add("Otev?¸?­t", CtrlImg::open(), THISBACK(OnFileOpen))
		.Key(K_CTRL_O)
		.Help("Na?¨?­st obr??zky z diskov?©ho souboru");
}
*/

/*
void DlgImage::OnFileOpen()
{
	if(FlushFile())
	{
		String s = setup.filename;
		FileSelector fsel;
		fsel.Type("Soubory obr??zk?? (*.iml)", "*.iml")
		.DefaultExt("iml");
		fsel <<= s;
		if(!fsel.ExecuteOpen("Otev?¸?­t"))
			return;
		DoLoad(~fsel);
	}
}
*/

/*
void DlgImage::ToolFileSave(Bar& bar)
{
	bar.Add("Ulo??it", CtrlImg::save(), THISBACK(OnFileSave))
		.Key(K_CTRL_S)
		.Help("Aktualizovat diskov?? soubor s obr??zky");
}
*/

/*
void DlgImage::OnFileSave()
{
	DoSaveFile();
}
*/

/*
void DlgImage::ToolFileSaveAs(Bar& bar)
{
	bar.Add("Ulo??it j&ako", CtrlImg::save_as(), THISBACK(OnFileSaveAs))
		.Help("Ulo??it soubor s obr??zky pod jin??m jm?©nem");
}
*/

/*
void DlgImage::OnFileSaveAs()
{
	DoSave(Null);
}
*/

/*
void DlgImage::ToolFileExport(Bar& bar)
{
	bar.Add("Ulo??it", THISBACK(OnFileExport))
		.Key(K_SHIFT_CTRL_S)
		.Help("Exportovat obr??zek do souboru");
}
*/

/*
void DlgImage::OnFileExport()
{
	int i = image_list.list.GetCursor();
	if(i < 0)
		return;
	Image::Info data = Get(i);
	String ident = image_list.list.Get(i, CNAME);
	String rle = image_list.list.Get(i, CDATA);
	AlphaArray buffer(data.size, -3);
	RLEToAlpha(buffer, rle);
	if(buffer.IsEmpty())
	{
		BeepExclamation();
		return;
	}
	FileSel fsel;
	fsel.Type("Form??t GIF (*.gif)",  "*.gif")
		.Type("Form??t JPEG (*.jpg)", "*.jpg;*.jpeg")
		.Type("Form??t PNG (*.png)",  "*.png")
		.Type("Form??t TIFF (*.tif)", "*.tif;*.tiff")
		.Type("Form??t BMP (*.bmp)",  "*.bmp")
		.Type("Form??t ICO (*.ico)",  "*.ico")
		.DefaultExt("jpg");
	fsel <<= recent_export;
	if(!fsel.ExecuteSaveAs(NFormat("Exportovat %s jako", Nvl(ident, "obr??zek"))))
		return;
	recent_export = ~fsel;
	One<ImageEncoder> encoder;
	bool ok = false;
	String ext = ToLower(GetFileExt(recent_export));
	if(ext == ".gif") encoder = new GifEncoder;
	if(ext == ".jpg" || ext == ".jpeg") encoder = new JpgEncoder;
	if(ext == ".png") encoder = new PngEncoder;
	if(ext == ".tif" || ext == ".tiff") encoder = new TifEncoder;
	if(ext == ".bmp") encoder = new BmpEncoder;
	if(ext == ".ico") encoder = new BmpEncoder(0, BmpEncoder::BF_ICOFILE);
	if(!encoder)
	{
		Exclamation(NFormat("Nepodporovan?? form??t obr??zku: [* \1%s\1].", ext));
		return;
	}
	if(!encoder -> SaveArrayFile(recent_export, buffer))
	{
		Exclamation(NFormat("Chyba p?¸i z??pisu do souboru [* \1%s\1].", recent_export));
		return;
	}
}
*/

/*
void DlgImage::ToolFileQuit(Bar& bar)
{
	bar.Add("Konec\tAlt+F4", THISBACK(OnFileQuit))
		.Help("Ukon?¨it pr??ci s editorem obr??zk??");
}
*/

/*
void DlgImage::OnFileQuit()
{
	if(FlushFile())
	{
		StoreToGlobal(*this, "DlgImage");
#ifdef flagIDERW
		SetTimeCallback(0, THISBACK(CloseDesigner));
#else
		EndLoop();
#endif
	}
}
*/

void DlgImage::ToolEdit(Bar& bar)
{
	ToolEditUndo(bar);
	ToolEditRedo(bar);
	bar.Separator();
	ToolEditInsert(bar);
	ToolEditDuplicate(bar);
	ToolEditInsertPaste(bar);
	ToolEditInsertFile(bar);
	bar.Separator();
	ToolEditSplit(bar);
	ToolEditDelete(bar);
	if(image_list.list.IsCursor())
	{
		bar.Separator();
		image_designer.ToolSelectCut(bar);
		image_designer.ToolSelectCopy(bar);
		ToolEditPaste(bar);
		bar.Separator();
		ToolEditCrop(bar);
	}
}

void DlgImage::ToolEditUndo(Bar& bar)
{
	String op = t_("Undo");
	if(undo_pos > 0)
		op << " (" << undo[undo_pos - 1].Format() << ')';

	undo_btn.Enable(undo_pos > 0).Image(CtrlImg::undo()).Tip(op)
	        .Help(t_("Undo last action"));
	undo_btn <<= THISBACK(OnEditUndo);
	bar.AddTool(undo_btn);
	bar.AddMenu(undo_pos > 0, op, CtrlImg::undo(), THISBACK(OnEditUndo))
	   .Key(K_CTRL_Z)
	   .Help(t_("Undo last action"));

	bar.Add(undo_pos > 0, MiscImg::command_list(), THISBACK(OnEditUndoList))
	   .Tip(t_("Action list"))
	   .Help(t_("List of recent actions"));
}

void DlgImage::OnEditUndo()
{
	TIMING("Undo");
	if(undo_pos > 0)
	{
		image_designer.PasteAndClearImage();
		int new_pos = undo_pos - 1;
		if(undo_pos == undo.GetCount())
			image_designer.AddUndo(t_("undo"));
		undo[undo_pos = new_pos].Undo();
		if(undo_pos > 0 && !undo[undo_pos].IsSnapshot() && undo[undo_pos - 1].IsSnapshot())
			undo_pos--;
		UpdateCaption();
		Rescan();
	}
}

void DlgImage::OnEditUndoList()
{
	if(undo_pos > 0)
	{
		command_list.Clear();
		for(int i = undo_pos - 1; i >= 0; i--)
		{
			String s = undo[i].Format();
			if(i == undo_anchor)
				s << '*';
			command_list.Add(i, s);
			if(i > 0 && !undo[i].IsSnapshot() && undo[i - 1].IsSnapshot())
				i--; // skip fixup snapshot before
		}
		command_list.WhenSelect = THISBACK(OnEditUndoListSelect);
		command_list.PopUp(&undo_btn, 150);
	}
}

void DlgImage::OnEditUndoListSelect()
{
	if(command_list.IsCursor())
	{
		int new_pos = command_list.Get(0);
		if(new_pos < undo.GetCount() && new_pos != undo_pos)
		{
			image_designer.PasteAndClearImage();
			if(undo_pos == undo.GetCount())
				image_designer.AddUndo(t_("undo"));
			while(undo_pos > new_pos)
				undo[--undo_pos].Undo();
			if(undo_pos > 0 && !undo[undo_pos].IsSnapshot() && undo[undo_pos - 1].IsSnapshot())
				undo_pos--;
			UpdateCaption();
			Rescan();
		}
	}
}

void DlgImage::ToolEditRedo(Bar& bar)
{
	bool avail = false;
	String op = t_("Redo");
	if(undo_pos < undo.GetCount() && !undo[undo_pos].IsSnapshot())
	{
		avail = true;
		op << " (" << undo[undo_pos].Format() << ')';
	}
	else if(undo_pos + 1 < undo.GetCount())
	{
		avail = true;
		op << " (" << undo[undo_pos].Format() << ')';
	}

	redo_btn.Enable(avail).Image(CtrlImg::redo()).Tip(op)
	        .Help(t_("Redo last undone action"));
	redo_btn <<= THISBACK(OnEditRedo);
	bar.AddTool(redo_btn);
	bar.AddMenu(avail, op, CtrlImg::redo(), THISBACK(OnEditRedo))
	   .Key(K_CTRL_Y)
	   .Help(t_("Redo last undone action"));

	bar.Add(avail, MiscImg::command_list(), THISBACK(OnEditRedoList))
	   .Tip(t_("Action list"))
	   .Help(t_("L"));
}

void DlgImage::OnEditRedo()
{
	TIMING("Redo");
	if(undo_pos < undo.GetCount() && !undo[undo_pos].IsSnapshot())
		undo[undo_pos++].Redo();
	else if(undo_pos + 1 < undo.GetCount())
	{
		if(undo[++undo_pos].IsSnapshot())
			undo[undo_pos].Undo();
		else
			undo[undo_pos++].Redo();
	}
	UpdateCaption();
	Rescan();
}

void DlgImage::OnEditRedoList()
{
	if(undo_pos < undo.GetCount())
	{
		command_list.Clear();
		for(int i = undo_pos; i < undo.GetCount(); i++)
		{
			String s;
			if(!undo[i].IsSnapshot())
				s = undo[i].Format();
			else if(i > undo_pos && undo[i - 1].IsSnapshot())
				s = undo[i - 1].Format();
			else
				continue;
			if(i == undo_anchor)
				s << '*';
			command_list.Add(i, s);
		}
		command_list.WhenSelect = THISBACK(OnEditRedoListSelect);
		command_list.PopUp(&redo_btn, 150);
	}
}

void DlgImage::OnEditRedoListSelect()
{
	if(command_list.IsCursor())
	{
		int new_pos = command_list.Get(0);
		if(new_pos < undo.GetCount())
		{
			image_designer.ClearSelectionAndImage();
			while(undo_pos <= new_pos)
				if(!undo[undo_pos].IsSnapshot())
					undo[undo_pos++].Redo();
				else if(undo_pos + 1 <= new_pos)
				{
					if(undo[++undo_pos].IsSnapshot())
						undo[undo_pos].Undo();
					else
						undo[undo_pos++].Redo();
				}
				else
					break;
			UpdateCaption();
			Rescan();
		}
	}
}

//void DlgImage::ToolEditMoveUp(Bar& bar)

void DlgImage::OnEditMoveUp()
{
	int c;
	if(image_list.list.Accept() && (c = image_list.list.GetCursor()) > 0)
		AddUndo(new UndoEntrySwap(*this, t_("move up"), c, c - 1));
}

//void DlgImage::ToolEditMoveDown(Bar& bar)

void DlgImage::OnEditMoveDown()
{
	int c;
	if(image_list.list.Accept()
	&& (c = image_list.list.GetCursor()) + 1 < image_list.list.GetCount())
		AddUndo(new UndoEntrySwap(*this, t_("move down"), c, c + 1));
}

void DlgImage::ToolEditInsert(Bar& bar)
{
	bar.Add(t_("New bitmap"), MiscImg::insert(), THISBACK(OnEditInsert))
		.Key(K_INSERT)
		.Help(t_("Add a new image into the list"));
}

void DlgImage::OnEditInsert()
{
	int i = image_list.list.GetCursor() + 1;
	int old = i - 1;
	IMLImageInfo data;
	data.size = Size(16, 16);
	if(old >= 0)
		data.AlphaImageInfo::operator = (Get(old));
	One<UndoEntryList> cmd = new UndoEntryList(*this, t_("new image"));
	cmd -> Insert(i, data);
	AddUndo(-cmd);
}

void DlgImage::ToolEditDuplicate(Bar& bar)
{
	bar.Add(image_list.list.IsCursor(), t_("Duplicate bitmap"), MiscImg::duplicate(), THISBACK(OnEditDuplicate))
		.Key(K_CTRL_INSERT)
			.Help(t_("Add a copy of the current image into the list"));
}

void DlgImage::OnEditDuplicate()
{
	int c = image_list.list.GetCursor();
	if(c < 0)
		return;

	One<UndoEntryList> cmd = new UndoEntryList(*this, t_("duplicate"));
	cmd -> Insert(c + 1, IMLImageInfo(Get(c), Null, image_list.list.Get(CDATA)));
	AddUndo(-cmd);
}

void DlgImage::ToolEditInsertPaste(Bar& bar)
{
	bar.Add(t_("New from clipboard"), MiscImg::insert_paste(), THISBACK(OnEditInsertPaste))
		.Key(K_SHIFT_INSERT)
		.Help(t_("Insert image from clipboard into the list"));
}

void DlgImage::OnEditInsertPaste()
{
#ifdef PLATFORM_WIN32
	AlphaArray image = ClipboardToAlphaArray();
	if(image.IsEmpty())
	{
		Exclamation(t_("Error reading clipboard image"));
		return;
	}
	PixelSetConvert(image.pixel, -3);
	if(image.HasAlpha())
		PixelSetConvert(image.alpha, 8);
	else
		image.CreateAlpha();

	IMLImageInfo data;
	data.size = image.GetSize();
	data.encoded_data = AlphaToRLE(image);
	One<UndoEntryList> cmd = new UndoEntryList(*this, t_("new from clipboard"));
	cmd -> Insert(image_list.list.GetCursor() + 1, data);
	AddUndo(-cmd);
#endif
}

void DlgImage::ToolEditInsertFile(Bar& bar)
{
	bar.Add(t_("New from file..."), MiscImg::insert_file(), THISBACK(OnEditInsertFile))
		.Key(K_CTRL_I)
		.Help(t_("Insert image from file into the list"));
}

static String MakeIdent(const char *i)
{
	String out;
	for(; *i; i++)
		if(IsAlNum(*i))
		{
			if(out.IsEmpty() && IsDigit(*i))
				out.Cat('_');
			out.Cat(*i);
		}
		else if(!out.IsEmpty() && *out.Last() != '_')
			out.Cat('_');
	return out;
}

void DlgImage::OnEditInsertFile()
{
	FileSelector fs;
	fs.Type(t_("Images (bmp, png, gif, jpeg, tiff, cur, ico)"), t_("*bmp;*.png;*.gif;*.jpg;*.jpeg;*.tif;*.tiff;*.cur;*.ico"))
		.DefaultExt("bmp")
		.Multi();
	if(fs.ExecuteOpen())
	{
		int cursor = image_list.list.GetCursor();
		for(int i = 0; i < fs.GetCount(); i++)
		{
			String fn = fs[i];
			FileIn fi(fn);
			if(!fi.IsOpen())
			{
				if(!PromptOKCancel(NFormat(t_("Error opening file [* \1%s\1]. Continue?"), fn)))
					break;
				continue;
			}
			Array<AlphaArray> image = StdLoadArray(fi, Vector<int>());
			if(image.IsEmpty())
			{
				if(!PromptOKCancel(NFormat(t_("Image [* \1%s\1]: file read or format error.\nContinue?"), fn)))
					break;
				continue;
			}
			for(int a = 0; a < image.GetCount(); a++)
			{
				AlphaArray& im = image[a];
				PixelSetConvert(im.pixel, -3);
				if(im.HasAlpha())
					PixelSetConvert(im.alpha, 8);
				else
					im.CreateAlpha();
				IMLImageInfo info;
				info.size = im.GetSize();
				One<UndoEntryList> cmd = new UndoEntryList(*this, t_("new from file"));
				String title = MakeIdent(ToLower(GetFileTitle(fn)));
				if(image.GetCount() > 1)
					title << '_' << (a + 1);
				info.encoded_data = AlphaToRLE(im);
				info.name = title;
				cmd -> Insert(++cursor, info);
				AddUndo(-cmd);
			}
		}
	}
}

void DlgImage::ToolEditSplit(Bar& bar)
{
	bar.Add(image_list.list.IsCursor(), t_("Split image"), MiscImg::split(), THISBACK(OnEditSplit))
		.Help(t_("Split image into smaller parts"));
}

void DlgImage::OnEditSplit()
{
	int i = image_list.list.GetCursor();
	if(i < 0)
		return;
	image_designer.PasteAndClearImage();
	image_designer.ClearSelection();
	WithSplitLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, t_("Split image"));
	dlg.cx.MinMax(1, 1024).NotNull();
	dlg.cy.MinMax(1, 1024).NotNull();
	dlg.cx <<= dlg.cy <<= (int)image_designer.GetImageSize().cy;
	if(dlg.Run() != IDOK)
		return;
	One<UndoEntryList> cmd = new UndoEntryList(*this, t_("split"));
	Size item(~dlg.cx, ~dlg.cy);
	IMLImageInfo data = Get(i);
	const AlphaArray& org = image_designer.image;
	int ni = i;
	for(int y = 0; y < org.GetSize().cy; y += item.cy)
		for(int x = 0; x < org.GetSize().cx; x += item.cx)
		{
			AlphaArray part = AlphaCrop(org, Rect(x, y, x + item.cx, y + item.cy));
			data.size = part.GetSize();
			data.encoded_data = AlphaToRLE(part);
			cmd -> Insert(++ni, data);
		}
	AddUndo(-cmd);
}

void DlgImage::ToolEditDelete(Bar& bar)
{
	bar.Add(image_list.list.IsCursor(), t_("Remove bitmap"), CtrlImg::remove(), THISBACK(OnEditDelete))
		.Key(K_CTRL_DELETE)
		.Help(t_("Remove bitmap from the list"));
}

void DlgImage::OnEditDelete()
{
	int c = image_list.list.GetCursor();
	if(c >= 0)
	{
		One<UndoEntryList> cmd = new UndoEntryList(*this, t_("remove image"));
		cmd -> Remove(c, IMLImageInfo(Get(c), ~image_list.name, AlphaToRLE(image_designer.image)));
		AddUndo(-cmd);
	}
}

void DlgImage::ToolEditPaste(Bar& bar)
{
	bar.Add(IsPasteAvailable() && image_list.list.IsCursor(), t_("Insert"), CtrlImg::paste(), THISBACK(OnEditPaste))
		.Key(K_CTRL_V)
		.Help(t_("Insert clipboard into image"));
}

void DlgImage::OnEditPaste()
{
#ifdef PLATFORM_WIN32
	if(!image_list.list.IsCursor())
		return;

	AlphaArray image = ClipboardToAlphaArray();
	if(image.IsEmpty())
	{
		Exclamation(t_("Error reading clipboard image."));
		return;
	}
	PixelSetConvert(image.pixel, -3);
	if(image.HasAlpha())
		PixelSetConvert(image.alpha, 8);

	Size old_size = image_designer.GetImageSize();
	Size new_size = image.GetSize();
	if(new_size.cx > old_size.cx || new_size.cy > old_size.cy)
	{ // large image
		enum { IGNORE_SIZE, ENLARGE_CANVAS, };
		static int last = IGNORE_SIZE;
		WithPasteLargeLayout<TopWindow> large;
		CtrlLayoutOKCancel(large, t_("Insert image"));
		large.select <<= last;
		if(large.Run() != IDOK)
			return;
		switch(last = ~large.select)
		{
		case IGNORE_SIZE:
			break;

		case ENLARGE_CANVAS:
			if(new_size.cx > old_size.cx)
				image_list.width <<= (int)new_size.cx;
			if(new_size.cy > old_size.cy)
				image_list.height <<= (int)new_size.cy;
			OnImageSize();
			break;
		}
	}
	image_designer.ClearSelectionAndImage();
	image_designer.SetLeftButton(&GetAdapterSelect);
	image_designer.SetSelection(Rect(new_size));
	image_designer.SetSelectionImage(image);
#endif
}

void DlgImage::ToolEditCrop(Bar& bar)
{
	bar.Add(t_("Crop"), MiscImg::crop(), THISBACK(OnEditCrop))
		.Help(t_("Crop image at selection (erase everything except selection)"));
}

void DlgImage::OnEditCrop()
{
	int c =	image_list.list.GetCursor();
	if(c >= 0
		&& image_designer.IsSelectionAvailable() && image_designer.GetSelectionImage().IsEmpty()
		&& PromptYesNo(t_("Crop image and change its size?")))
	{
		AlphaImageInfo data = Get(c);
		One<UndoEntryList> cmd = new UndoEntryList(*this, t_("crop"));
		cmd -> Remove(c, IMLImageInfo(data, ~image_list.name, image_list.list.Get(CDATA)));
		AlphaArray new_image = AlphaCrop(image_designer.image, image_designer.GetSelection());
		data.size = new_image.GetSize();
		cmd -> Insert(c, IMLImageInfo(data, ~image_list.name, AlphaToRLE(new_image)));
		AddUndo(-cmd);
	}
	else
		image_designer.CropSelection();
}

void DlgImage::ToolSelect(Bar& bar)
{
	image_designer.ToolSelect(bar);
	bar.Separator();
	ToolSelectRotateAnticlockwise(bar);
	image_designer.ToolSelectMirrorXY(bar);
	ToolSelectRotateClockwise(bar);
}

void DlgImage::ToolSelectRotateClockwise(Bar& bar)
{
	bar.Add(t_("90 degrees right"), MiscImg::rotate_clockwise(), THISBACK(OnSelectRotateClockwise))
		.Help(t_("Rotate selection by 90 degrees clockwise"));
}

void DlgImage::OnSelectRotateClockwise()
{
	if(!image_list.list.IsCursor())
		return;
	Size size = image_designer.GetImageSize();
	if(!image_designer.IsSelectionAvailable() && size.cx != size.cy)
	{
		if(PromptOKCancel(t_("Rotate image and change its dimensions?")))
		{
			AlphaArray rot = AlphaTransform(image_designer.image, IMAGE_TCLOCKWISE);
			SetImage(rot, t_("rotate 90 degrees right"));
			return;
		}
	}
	image_designer.RotateSelectionClockwise();
}

void DlgImage::ToolSelectRotateAnticlockwise(Bar& bar)
{
	bar.Add(t_("90 degrees left"), MiscImg::rotate_anticlockwise(), THISBACK(OnSelectRotateAnticlockwise))
		.Help(t_("Rotate selection by 90 degrees anticlockwise"));
}

void DlgImage::OnSelectRotateAnticlockwise()
{
	if(!image_list.list.IsCursor())
		return;
	Size size = image_designer.GetImageSize();
	if(!image_designer.IsSelectionAvailable() && size.cx != size.cy)
	{
		if(PromptOKCancel(t_("Rotate image and change its dimensions?")))
		{
			SetImage(AlphaTransform(image_designer.image, IMAGE_TANTICLOCKWISE), t_("rotate 90 degrees left"));
			return;
		}
	}
	image_designer.RotateSelectionAnticlockwise();
}

void DlgImage::ToolMode(Bar& bar)
{
	image_designer.ToolMode(bar);
}

void DlgImage::ToolSetup(Bar& bar)
{
	image_designer.ToolSetup(bar);
	bar.Separator();
	ToolSetupSplit(bar);
	bar.Separator();
	ToolSetupProperties(bar);
}

void DlgImage::ToolSetupSplit(Bar& bar)
{
	bar.Add(setup.split_horz ? t_("Horizontal split") : t_("Vertical split"),
		setup.split_horz ? MiscImg::split_horz() : MiscImg::split_vert(),
		THISBACK(OnSetupSplit))
		.Help(t_("Swap positions of zoomed & true size image view"));
}

void DlgImage::OnSetupSplit()
{
	setup.split_horz = !setup.split_horz;
	UpdateImageSplitter();
}

void DlgImage::ToolSetupProperties(Bar& bar)
{
	bar.Add(t_("Properties"), MiscImg::setup_properties(), THISBACK(OnSetupProperties))
		.Key(K_ALT_ENTER)
		.Help(t_("Display / edit editor properties"));
}

void DlgImage::OnSetupProperties()
{
	setup.grid = image_designer.grid;
	setup.grid_color = image_designer.grid_color;
	setup.paste_transparent = image_designer.paste_transparent;
	Setup old_setup = setup, new_setup = setup;
	if(new_setup.Edit())
		AddUndo(new UndoEntrySetup(*this, old_setup, new_setup));
}

void DlgImage::UpdateSetup()
{
	UpdateImageSplitter();
	image_list.list.SetLineCy(setup.high_list ? 30 : 16);
	image_designer.grid = (ImageEditCtrl::GRID)setup.grid;
	image_designer.grid_color = setup.grid_color;
	image_designer.paste_transparent = setup.paste_transparent;
	image_designer.RefreshImage();
	Rescan();
}

void DlgImage::UpdateImageSplitter()
{
	if(setup.split_horz)
		isplitter.Horz(image_designer, image_small);
	else
		isplitter.Vert(image_designer, image_small);
	//isplitter.RefreshLayout();
}

Value DlgImage::Format(const Value& row) const
{
	ValueArray va = row;
	String name = va[1];
	Size org = image_designer.GetImageSize();
	int wd = Nvl((int)va[2], org.cx);
	int ht = Nvl((int)va[3], org.cy);
	if(IsNull(name))
		name << wd << " x " << ht;
	else if(setup.high_list)
		name << "\n(" << wd << " x " << ht << ')';
	return name;
}

void DlgImage::OnImageName()
{
	if(!image_list.list.IsCursor())
		return;
	int b, e;
	image_list.name.GetSelection(b, e);
	One<UndoEntryName> cmd = new UndoEntryName(*this,
		image_list.list.GetCursor(), old_name, ~image_list.name);
	old_name = ~image_list.name;
	AddUndo(-cmd);
	image_list.name.SetSelection(b, e);
}

void DlgImage::OnImageSize()
{
	int c = image_list.list.GetCursor();
	if(c < 0)
		return;

	Size size = image_designer.GetImageSize();
	if(!IsNull(~image_list.width))
		size.cx = (int)~image_list.width;
	if(!IsNull(~image_list.height))
		size.cy = (int)~image_list.height;
	Size old_size = image_designer.GetImageSize();
	if(size != old_size)
	{
		Size tail = image_tail.GetSize();
		Size new_tail = max(tail, old_size);
		if(new_tail != tail)
		{
			AlphaArray larger_buffer(new_tail);
			AlphaSet(larger_buffer, new_tail, Color(Null));
			if(tail.cx > 0 && tail.cy > 0)
				AlphaCopy(larger_buffer, image_tail.GetSize(), image_tail, image_tail.GetSize(), false);
			image_tail = larger_buffer;
		}
		AlphaCopy(image_tail, old_size, image_designer.image, old_size, false);
		One<UndoEntryList> cmd = new UndoEntryList(*this, t_("image size"));
		AlphaImageInfo data = Get(c);
		data.size = old_size;
		cmd -> Remove(c, IMLImageInfo(data, ~image_list.name, AlphaToRLE(image_designer.image)));
		AlphaArray new_image(size, -3);
		AlphaSet(new_image, size, Color(Null));
		data.size = size;
		AlphaCopy(new_image, size, image_tail, size, false);
		cmd -> Insert(c, IMLImageInfo(data, ~image_list.name, AlphaToRLE(new_image)));
		AddUndo(-cmd);
	}
}

void DlgImage::SetImage(const AlphaArray& im, const char *command)
{
	int c = image_list.list.GetCursor();
	if(c >= 0 && image_list.list.KillCursor())
	{
		AlphaImageInfo data = Get(c);
		One<UndoEntryList> cmd = new UndoEntryList(*this, command);
		String name = image_list.list.Get(c, CNAME);
		cmd -> Remove(c, IMLImageInfo(data, name, image_list.list.Get(c, CDATA)));
		data.size = im.GetSize();
		cmd -> Insert(c, IMLImageInfo(data, name, AlphaToRLE(im)));
		AddUndo(-cmd);
	}
}

/*
void DlgImage::SetImageRaw(ImageBuffer& im)
{
	int i = image_list.list.GetCursor();
	if(i >= 0)
	{
		Image::Info data = Get(i);
		data.SetSize(im.GetSize());
		Set(i, data, im.GetRLE());
		image_designer.SetImage(im);
		image_list.list.SetCursor(i);
	}
}
*/

void DlgImage::OnImage()
{
	static bool updating = false;
	if(updating)
		return;
	updating = true;
	int new_cursor = image_list.list.GetCursor();
	if(old_cursor >= 0 && old_cursor < image_list.list.GetCount())
	{ // compress old image
		image_designer.PasteSelection();
		Point hotspot = image_designer.GetHotSpot();
		image_list.list.Set(old_cursor, CHOTX, (int)hotspot.x);
		image_list.list.Set(old_cursor, CHOTY, (int)hotspot.y);
		image_list.list.Set(old_cursor, CDATA, AlphaToRLE(image_designer.image));
		if(undo_pos > 0 && undo[undo_pos - 1].GetSnapshotCursor() == old_cursor)
		{ // add fixup snapshot for old image
			image_designer.AddUndo(t_("cursor"));
		}
	}
//	image_list.list.SetCursor(new_cursor);
	if(new_cursor >= 0 && new_cursor < image_list.list.GetCount())
	{ // decompress image
		AlphaImageInfo data = Get(new_cursor);
		image_designer.SetImageSize(data.size);
		image_designer.SetHotSpot(data.hotspot);
		RLEToAlpha(image_designer.image, (String)image_list.list.Get(new_cursor, CDATA));
	}
	else
		image_designer.SetImageSize(Size(0, 0));
	image_designer.ClearSelectionAndImage();
	image_designer.SetCursor(new_cursor);
	image_designer.Refresh();
	old_cursor = new_cursor;
	old_name = (new_cursor >= 0 ? String(image_list.list.Get(CNAME)) : String(Null));
	Rescan();
	updating = false;
}

bool DlgImage::FlushFile()
{
	if(image_list.list.GetCount() == 0 || !IsDirty()) // assume empty list is always flushed
		return true;

	switch(PromptYesNoCancel(t_("Save changes in current file?")))
	{
	case 1:  return DoSave(setup.filename);
	case 0:  return true;
	default: return false;
	}
}

bool DlgImage::DoLoad(String fn, bool warn_ne)
{
	String s = LoadFile(fn);
	if(s.IsVoid())
	{
		if(warn_ne)
		{
			Exclamation(NFormat(t_("Error reading file [* \1%s\1]."), fn));
			recent_files.RemoveEntry(fn);
			return false;
		}
		else
		{
			SetFileName(fn);
			ClearUndo();
			RestoreEditPos(FileMap().Get(setup.filename, EditPos()));
			return true;
		}
	}
	image_list.list.Clear();
	OnImage();
	setup.NewFile();
	try
	{
		CParser parser(s, fn, 1);
		Array<IMLImageInfo> info;
		VectorMap<String, String> param;
		AlphaScanIML(parser, info, param);
		image_list.list.SetCount(info.GetCount());
		for(int i = 0; i < info.GetCount(); i++)
		{
			const IMLImageInfo& ii = info[i];
			image_list.list.Set(i, CWIDTH,  ii.size.cx);
			image_list.list.Set(i, CHEIGHT, ii.size.cy);
			image_list.list.Set(i, CFORMAT, ii.encoding);
			image_list.list.Set(i, CHOTX,   ii.hotspot.x);
			image_list.list.Set(i, CHOTY,   ii.hotspot.y);
			image_list.list.Set(i, CNAME,   ii.name);
			image_list.list.Set(i, CDATA,   ii.encoded_data);
		}
		String scan;
		if(!IsNull(scan = param.Get("header", Null)))
			setup.header_file = scan;
		if(!IsNull(scan = param.Get("class", Null)))
		{
			setup.wrapper_name = scan;
			setup.wrapper_kind = setup.CLASS_WRAPPER;
		}
		if(!IsNull(scan = param.Get("space", Null)))
		{
			setup.wrapper_name = scan;
			setup.wrapper_kind = setup.SPACE_WRAPPER;
		}
		if(!IsNull(scan = param.Get("java_file", Null)))
		{
			setup.java_export = Setup::FILE_EXPORT;
			setup.java_source = scan;
		}
		if(!IsNull(scan = param.Get("java_prog", Null)))
		{
			setup.java_export = Setup::CPP_EXPORT;
			setup.java_source = scan;
		}
		if(!IsNull(scan = param.Get("java_package", Null)))
			setup.java_package = scan;
		if(!IsNull(scan = param.Get("gif_folder", Null)))
			setup.gif_folder = scan;
		if(!IsNull(scan = param.Get("gif_transparent", Null)))
		{
			int c = stou(~scan);
			setup.gif_transparent = Color((c >> 16) & 255, (c >> 8) & 255, c & 255);
		}
		if(!IsNull(scan = param.Get("icon_file", Null)))
			setup.icon_file = scan;
		if(!IsNull(scan = param.Get("icon_small", Null)))
			setup.icon_small = scan;
		if(!IsNull(scan = param.Get("icon_large", Null)))
			setup.icon_large = scan;
		if(!IsNull(scan = param.Get("icon_bpp", Null)))
		{
			setup.icon_bpp = atoi(scan);
			if(setup.icon_bpp != 1 && setup.icon_bpp != 4 && setup.icon_bpp != 8
			&& setup.icon_bpp != 16 && setup.icon_bpp != 24)
				setup.icon_bpp = 8;
		}
		setup.wince_16bit = *param.Get("wince_16bit", "0") == '1';
	}
	catch(Exc e)
	{
		if(warn_ne)
			Exclamation(NFormat(t_("Error in file [* \1%s\1]: \1%s"), fn, e));
		else
			PutConsole(NFormat(t_("Error in file '%s': %s"), fn, e));
		return false;
	}
	image_list.list.SetCursor(0);
	SetFileName(fn);
	ClearUndo();
	RestoreEditPos(FileMap().Get(setup.filename, EditPos()));
	return true;
}

bool DlgImage::DoSave(String f)
{
	if(IsNull(f))
	{
		FileSelector fsel;
		fsel.Type(t_("Image lists (*.iml)"), "*.iml")
		.DefaultExt("iml");
		if(!fsel.ExecuteSaveAs(t_("Save as")))
			return false;
		f = ~fsel;
	}
	String tmp = f + ".$tmp";
	FileOut fp(tmp);
	if(!fp.IsOpen())
	{
		Exclamation(NFormat(t_("Error creating temporary file [* \1%s\1]."), tmp));
		return false;
	}
	WriteFile(fp, f);
	if(setup.java_export != setup.NO_EXPORT)
		WriteJava(f, fp);
	fp.Close();
	if(fp.IsError())
	{
		Exclamation(NFormat(t_("Error writing file [* \1%s\1]."), tmp));
		return false;
	}
	FileDelete(f);
	if(!FileMove(tmp, f))
	{
		Exclamation(NFormat(t_("Error updating file [* \1%s\1]."), f));
		return false;
	}
	if(!IsNull(setup.gif_folder))
		WriteGif(setup.gif_folder, setup.gif_transparent);
	if(!IsNull(setup.icon_file))
		WriteIcon(setup.icon_file, setup.icon_small, setup.icon_large, setup.icon_bpp);
	SetUndoAnchor();
	SetFileName(f);
	return true;
}

void DlgImage::SetFileName(String f)
{
	if(!IsNull(f))
		recent_files.NewEntry(f);
	setup.filename = f;
	UpdateCaption();
}

void DlgImage::UpdateCaption()
{
	String cap;
	cap << '[' << Nvl(setup.filename, t_("unnamed"));
	if(IsDirty())
		cap << " *";
	cap << t_("] - image editor");
	Title(cap.ToWString());
}

static void PutOctalString(Stream& out, const char *b, const char *e, bool split = false)
{
	out.Put('\"');
	int64 start = out.GetPos();
	while(b < e) {
		if(split && out.GetPos() >= start + 200u) {
			out.Put("\"\r\n\t\"");
			start = out.GetPos();
		}
		if((byte)*b >= ' ' && *b != '\x7F'&& *b != '\xFF') {
			if(*b == '\\' || *b == '\"' || *b == '\'')
				out.Put('\\');
			out.Put(*b++);
		}
		else if(IsDigit(b[1]))
			out.Put(Sprintf("\\%03o", (byte)*b++));
		else
			out.Put(Sprintf("\\%o", (byte)*b++));
	}
	out.Put('\"');
}

static void PutOctalString(Stream& out, const String& s, bool split = false)
{
	PutOctalString(out, s.Begin(), s.End(), split);
}

static void PutKeyString(Stream& out, const char *tag, const String& s)
{
	if(!IsNull(s))
	{
		out.Put("IMAGE_META(");
		out.Put(tag);
		out.Put(", ");
		PutOctalString(out, s);
		out.Put(")");
		out.PutCrLf();
	}
}

static WString AlphaScanToUnicode(const byte *scan, const byte *alpha, int width)
{
	WString out;
	const byte *end = alpha + width;
	while(alpha < end)
	{
		const byte *b = alpha;
		while(alpha < end && *alpha == 1)
			alpha++;
		int empty = alpha - b;
		scan += 3 * empty;
		if(alpha >= end)
			return out;
		b = alpha;
		while(alpha < end && *alpha == 0)
			alpha++;
		int full = alpha - b;
		b = scan;
		scan += 3 * full;
		while(empty || full)
			if(empty >= 0x80)
			{
				out.Cat(0x007F);
				empty -= 0x7F;
			}
			else
			{
				int scanned = 0;
				const byte *enumscan = b;
				for(; scanned + 3 < full; b += 3, scanned++)
					if(b[0] == b[3] && b[0] == b[6] && b[1] == b[4] && b[1] == b[7] && b[2] == b[5] && b[2] == b[8])
						break;
				if(scanned >= full - 3)
					scanned = full;
				full -= scanned;
				while(scanned)
				{
					int part = min(scanned, 0xFF);
					scanned -= part;
					out.Cat(part * 0x100 + empty);
					for(; --part >= 0; enumscan += 3)
						out.Cat((enumscan[0] >> 3) | ((enumscan[1] >> 3) << 5) | ((enumscan[2] >> 3) << 10));
					empty = 0;
				}
				if(full > 0)
				{ // repeat segment
					word reppixel = (b[0] >> 3) | ((b[1] >> 3) << 5) | ((b[2] >> 3) << 10);
					int rep = 3;
					for(b += 9; rep < full; b += 3, rep++)
						if(b[-3] != b[0] || b[-2] != b[1] || b[-1] != b[2])
							break;
					full -= rep;
					while(rep > 0)
					{
						int part = min(rep, 0xFF);
						out.Cat(part * 0x100 + empty + 0x80);
						out.Cat(reppixel);
						empty = 0;
						rep -= part;
					}
				}
			}
	}
	return out;
}

static const char anon_name[] = "im__";

void DlgImage::WriteFile(Stream& stream, String _filename)
{
	String header;
	int sc = image_list.list.GetCursorSc();
	int c = image_list.list.GetCursor();
	image_list.list.KillCursor();
	header.Cat('/', 70);
	String ifname = "__" + GetFileTitle(setup.header_file) + "__";

	{ // correct non-identifier characters
		for(int i = 0; i < ifname.GetLength(); i++)
			if(!IsAlNum(ifname[i]) && ifname[i] != '_')
				ifname.Set(i, '_');
	}

	header << "\r\n"
		"// " << GetFileTitle(setup.header_file)
		<< ": image header file generated from " << setup.filename << "\r\n\r\n"
		"#ifndef " << ifname << "\r\n"
		"#define " << ifname << "\r\n\r\n";
	if(!IsNull(setup.header_file))
		PutKeyString(stream, "header", setup.header_file);
	if(setup.java_export != setup.NO_EXPORT)
	{
		PutKeyString(stream, setup.java_export == setup.FILE_EXPORT ? "java_file" : "java_prog", setup.java_source);
		PutKeyString(stream, "java_package", setup.java_package);
	}
	if(!IsNull(setup.gif_folder))
	{
		PutKeyString(stream, "gif_folder", setup.gif_folder);
		if(!IsNull(setup.gif_transparent))
			PutKeyString(stream, "gif_transparent", FormatInt(
				(setup.gif_transparent.GetR() << 16)
				+ (setup.gif_transparent.GetG() << 8)
				+ setup.gif_transparent.GetB()));
	}

	if(!IsNull(setup.icon_file))
	{
		PutKeyString(stream, "icon_file", setup.icon_file);
		PutKeyString(stream, "icon_small", setup.icon_small);
		PutKeyString(stream, "icon_large", setup.icon_large);
		PutKeyString(stream, "icon_bpp", FormatInt(setup.icon_bpp));
	}

	if(setup.wrapper_kind == setup.CLASS_WRAPPER)
	{
		PutKeyString(stream, "class", setup.wrapper_name);
		header << "class " << setup.wrapper_name
			<< "\r\n"
			"{\r\n"
			"public:\r\n";
	}
	else if(setup.wrapper_kind == setup.SPACE_WRAPPER)
	{
		PutKeyString(stream, "space", setup.wrapper_name);
		stream.PutCrLf();
		header << "namespace " << setup.wrapper_name
			<< "\r\n"
			"{\r\n";
	}

	for(int i = 0; i < image_list.list.GetCount(); i++)
	{
		AlphaImageInfo data = Get(i);
		String name = image_list.list.Get(i, CNAME);
		String rle = image_list.list.Get(i, CDATA);
		if(IsNull(name))
			name = anon_name + IntStr(i);
		AlphaArray buffer(data.size, -3);
		AlphaSet(buffer, buffer.GetRect(), Color(Null));
		RLEToAlpha(buffer, rle);
		if(setup.wince_16bit)
		{
			stream.PutLine(NFormat("IMAGE_BEGIN16(%s)", name));
			int last = 0;
			for(int i = 0, h = data.size.cy; i < h; i++)
			{
				WString uscan = AlphaScanToUnicode(buffer.GetPixelDownScan(i), buffer.GetAlphaDownScan(i), buffer.GetWidth());
				if(!uscan.IsEmpty() || i == 0) // force at least 1 scan
				{
					for(; last < i; last++)
						stream.PutLine("\tIMAGE_SCAN16(L\"\")");
					stream.Put("\tIMAGE_SCAN16(L\"");
					for(const wchar *p = uscan, *e = uscan.End(); p < e; p++)
						stream.Put(NFormat("\\x%04x", *p));
					stream.Put("\")\r\n");
					last = i + 1;
				}
			}
			stream.PutLine(NFormat("IMAGE_END16(%s, %d, %d, %d, %d)",
				name, data.size.cx, data.size.cy, data.hotspot.x, data.hotspot.y));
		}
		else
		{
			stream.PutLine(NFormat("IMAGE_BEGIN(%s)", name));
			int last = 0;
			for(int i = 0, h = data.size.cy; i < h; i++)
			{
				String scan;
				AlphaScanToRLE(scan, buffer.GetPixelDownScan(i), buffer.GetAlphaDownScan(i), buffer.GetWidth());
				if(!scan.IsEmpty() || i == 0) // force at least 1 scan
				{
					for(; last < i; last++)
						stream.PutLine("\tIMAGE_SCAN(\"\")");
					stream.Put("\tIMAGE_SCAN(");
					PutOctalString(stream, scan.Begin(), scan.End(), true);
					stream.Put(")\r\n");
					last = i + 1;
				}
			}
			stream.Put("IMAGE_PACKED(");
			stream.Put(name);
			stream.Put(", ");
			StringStream datastrm;
			datastrm % data;
			ASSERT(!datastrm.IsError());
			String s = datastrm.GetResult();
			PutOctalString(stream, s.Begin(), s.End());
			stream.Put(")\r\n");
		}
		if(setup.wrapper_kind == setup.CLASS_WRAPPER)
			header << "\tstatic const Image& " << name << "();\r\n";
		else
		{
			if(setup.wrapper_kind == setup.SPACE_WRAPPER)
				header << '\t';
			header << "const Image& " << name << "();\r\n";
		}
	}

	if(setup.wrapper_kind == setup.CLASS_WRAPPER)
		header << "};\r\n";
	else if(setup.wrapper_kind == setup.SPACE_WRAPPER)
		header << "} // namespace " << setup.wrapper_name << "\r\n";

	header << "\r\n"
		"#endif//" << ifname << "\r\n";

	String fullhdr = NormalizePath(setup.header_file, GetFileDirectory(_filename));
//	String curdir = GetCurrentDirectory();
//	SetCurrentDirectory(GetFileDirectory(_filename));
	if(!IsNull(setup.header_file) && !SaveChangedFile(fullhdr, header))
		Exclamation(NFormat(t_("Error saving header file [* \1%s\1]."), fullhdr));
//	SetCurrentDirectory(curdir);

	image_list.list.SetCursor(c);
	image_list.list.ScCursor(sc);
}

void DlgImage::WriteJava(String _iml_file, Stream& iml_stream)
{
	Size max_size(1, 1);
	int i, items = image_list.list.GetCount();
	for(i = 0; i < items; i++)
		max_size = max(max_size, Get(i).size);
	AlphaArray image(max_size.cx * items, 2 * max_size.cy);
	AlphaSet(image, image.GetRect(), Color(Null));

	String jfn = setup.java_source;
	if(IsNull(jfn))
		jfn = ForceExt(_iml_file, ".java");

	String clss = GetFileTitle(jfn);
	String java;
	String cmt('/', 70);

	java << cmt << "\r\n"
		"// class "
		<< clss << " - image generated from " << _iml_file << "\r\n"
		"\r\n"
		"package " << setup.java_package << ";\r\n"
		"\r\n"
		"import java.awt.*;\r\n"
		"import utility.*;\r\n"
		"\r\n"
		"public final class " << clss << " extends ImageMap\r\n"
		"{\r\n"
		"\t" << cmt << "\r\n"
		"\r\n"
		"\tpublic " << clss << "()\r\n"
		"\t{\r\n"
		"\t\tsuper(" << (max_size.cx * items) << ", " << max_size.cy << ", " << max_size.cx
		<< ", \"" << clss << "\");\r\n"
		"\t}\r\n"
		"\r\n"
		"\t" << cmt << "\r\n"
		"\r\n";

	{ // generate combined image
		for(i = 0; i < items; i++)
		{
			AlphaImageInfo data = Get(i);
			String name = image_list.list.Get(i, CNAME);
			if(!IsNull(name))
				java << "\tpublic static final int " << name << " = " << i << ";\r\n";

			String rle = image_list.list.Get(i, CDATA);
			AlphaArray buffer(data.size, -3);
			RLEToAlpha(buffer, rle);
			Rect src(data.size);
			Rect dest = src;
			dest.OffsetHorz(i * max_size.cx);
			AlphaCopy(image, dest, buffer, src, true);
//			PixelCopy(image.pixel, dest.OffsetedVert(2 * max_size.cy), buffer.alpha, src);
			AlphaEtch(buffer, src);
			dest.OffsetVert(max_size.cy);
			AlphaCopy(image, dest, buffer, src, true);
//			PixelCopy(image.pixel, dest.OffsetedVert(2 * max_size.cy), buffer.alpha, src);
		}
	}

	java << "\r\n"
		"\t" << cmt << "\r\n"
		"}\r\n";

	if(!SaveChangedFile(jfn, java))
	{
		Exclamation(NFormat("Nelze ulo??it soubor [* \1%s\1].", jfn));
		return;
	}

	if(setup.java_export == setup.FILE_EXPORT)
	{ // save gif
		String imp = GifEncoder(true).SaveArray(image);
		jfn = ForceExt(_iml_file, ".gif");
		if(!SaveChangedFile(jfn, imp))
		{
			Exclamation(NFormat(t_("Error saving file [* \1%s\1]."), jfn));
			return;
		}
	}
	else
	{ // export gif to iml source
		iml_stream.PutLine(NFormat("IMAGE_BEGIN(_java_%s)", clss));
		AlphaImageInfo data(image.GetSize());
		int last = 0;
		for(int i = 0, h = data.size.cy; i < h; i++)
		{
			String scan;
			AlphaScanToRLE(scan, image.GetPixelDownScan(i), image.GetAlphaDownScan(i), image.GetWidth());
			if(!scan.IsEmpty() || i == 0) // force at least 1 scan
			{
				for(; last < i; last++)
					iml_stream.PutLine("\tIMAGE_SCAN(\"\")");
				iml_stream.Put("\tIMAGE_SCAN(");
				PutOctalString(iml_stream, scan.Begin(), scan.End(), true);
				iml_stream.Put(")\r\n");
				last = i + 1;
			}
		}
		iml_stream.Put("IMAGE_PACKED(_java_");
		iml_stream.Put(clss);
		iml_stream.Put(", ");
		StringStream datastrm;
		datastrm % data;
		ASSERT(!datastrm.IsError());
		String s = datastrm.GetResult();
		PutOctalString(iml_stream, s.Begin(), s.End());
		iml_stream.Put(")\r\n");
/*
		jfn = ForceExt(_iml_file, ".cpp");

		StringStream fp;
		fp << cmt << "\n"
			<< "// " << clss << "ToString/Stream - generated from file " << _iml_file << "\n"
			"\n"
			"#include <Core/Core.h>\n"
			"\n"
			"static const char iml_data[] =\n\t";
		PutOctalString(fp, imp, true);
		fp << ";\n"
			"\n"
			<< cmt << "\n"
			"\n"
			"void " << clss << "ToString(String& s)\n"
			"{\n"
			"\ts.Cat(iml_data, " << imp.GetLength() << ");\n"
			"}\n"
			"\n"
			<< cmt << "\n"
			"\n"
			"void " << clss << "ToStream(Stream& s)\n"
			"{\n"
			"\ts.Put(iml_data, " << imp.GetLength() << ");\n"
			"}\n";
		if(!SaveChangedFile(jfn, fp))
		{
			Exclamation(NFormat("Chyba p?¸i z??pisu do souboru [* \1%s\1].", jfn));
			return;
		}
*/
	}
}

void DlgImage::WriteGif(String _folder, Color _transparent)
{
	for(int i = 0; i < image_list.list.GetCount(); i++)
	{
		AlphaImageInfo info = Get(i);
		String name = image_list.list.Get(i, CNAME);
		if(IsNull(name))
			continue;
		String fn = AppendFileName(_folder,ForceExt(name, ".gif"));
		String rle = image_list.list.Get(i, CDATA);
		AlphaArray buffer(info.size, -3);
		RLEToAlpha(buffer, rle);
		if(!IsNull(_transparent))
			AlphaKillMask(buffer, _transparent);
		String s = GifEncoder().SaveArray(buffer);
		if(!SaveChangedFile(fn, s)
			&& !PromptOKCancel(NFormat(t_("Error saving image [* \1%s\1].\nContinue?"), fn)))
			break;
	}
}

void DlgImage::WriteIcon(String file, String smallicon, String largeicon, int bpp)
{
	int si = -1, li = -1;
	for(int i = 0; i < image_list.list.GetCount(); i++)
	{
		String n = image_list.list.Get(i, CNAME);
		if(!IsNull(smallicon) && smallicon == n) si = i;
		if(!IsNull(largeicon) && largeicon == n) li = i;
	}
	if(!IsNull(smallicon) && si < 0)
	{
		Exclamation(t_("Small application icon image not found in list."));
		return;
	}
	if(!IsNull(largeicon) && li < 0)
	{
		Exclamation(t_("Large application icon image was not found in the list."));
		return;
	}
	AlphaArray small_buf, large_buf;
	if(si >= 0)
	{
		String rle = image_list.list.Get(si, CDATA);
		AlphaImageInfo info = Get(si);
		small_buf.Create(info.size, -3);
		RLEToAlpha(small_buf, rle);
		small_buf.pixel.SetHotSpot(info.hotspot);
	}
	if(li >= 0)
	{
		String rle = image_list.list.Get(li, CDATA);
		AlphaImageInfo info = Get(li);
		large_buf.Create(info.size, -3);
		RLEToAlpha(large_buf, rle);
		large_buf.SetHotSpot(info.hotspot);
	}
	Vector<const AlphaArray *> va;
	if(!small_buf.IsEmpty())
		va.Add(&small_buf);
	if(!large_buf.IsEmpty())
		va.Add(&large_buf);
	StringStream icondata;
	BmpEncoder(bpp, BmpEncoder::BF_ICOFILE).SaveRaw(icondata, va);
	file = NormalizePath(file, GetFileDirectory(setup.filename));
	if(!SaveChangedFile(file, icondata))
		Exclamation(NFormat(t_("Error saving file [* \1%s\1]."), file));
}

static const char *scanner;

static int SkipWhite()
{
	while(*scanner && (byte)*scanner <= ' ')
		scanner++;
	return (byte)*scanner;
}

static String GetIdent()
{
	SkipWhite();
	if(!IsAlpha(*scanner) && *scanner != '_')
		return Null;
	const char *start = scanner;
	while(*scanner && (IsAlNum(*scanner) || *scanner == '_'))
		scanner++;
	return String(start, scanner);
}

static String GetUnicodeScan()
{
	WString rawdata;
	while(*scanner == '\\' && scanner[1] == 'x'
	&& IsXDigit(scanner[2]) && IsXDigit(scanner[3]) && IsXDigit(scanner[4]) && IsXDigit(scanner[5]))
		rawdata.Cat(strtol(scanner + 2, (char **)&scanner, 16));
	String scandata;
	const wchar *p = rawdata.Begin(), *e = rawdata.End();
	while(p < e)
	{
		wchar ctl = *p++;
		byte skip = ctl & 0x7F, full = ctl >> 8;
		while(skip)
		{
			byte part = min<byte>(skip, 0x3F);
			scandata.Cat(0xc0 + part);
			skip -= part;
		}
		if(ctl & 0x80)
		{ // repeat segment
			if(p >= e) return scandata; // overflow
			ctl = *p++;
			byte b = (ctl << 3) & 0xF8, g = (ctl >> 2) & 0xF8, r = (ctl >> 7) & 0xF8;
			while(full)
			{
				byte part = min<byte>(full, 0x3F);
				full -= part;
				scandata.Cat(0x80 + part);
				scandata.Cat(b);
				scandata.Cat(g);
				scandata.Cat(r);
			}
		}
		else
		{ // enum segment
			if(p + full > e) // overflow
				return scandata;
			while(full)
			{
				byte part = min<byte>(full, 0x7F);
				scandata.Cat(part);
				full -= part;
				do
				{
					ctl = *p++;
					byte b = (ctl << 3) & 0xF8, g = (ctl >> 2) & 0xF8, r = (ctl >> 7) & 0xF8;
					scandata.Cat(b);
					scandata.Cat(g);
					scandata.Cat(r);
				}
				while(--part);
			}
		}
	}
	return scandata;
}

static bool Check(char c)
{
	if(SkipWhite() != (byte)c)
		return false;
	scanner++;
	return true;
}

static int GetInt()
{
	SkipWhite();
	return strtol(scanner, (char **)&scanner, 10);
}

/*
void DlgImage::ReadFile(String s)
{
	scanner = s;
	String bid, name;
	while(*scanner)
	{
		if(scanner[0] == '/' && scanner[1] == '/' && scanner[2] == '!')
		{
			CParser parser(scanner + 3);
			name = parser.ReadId();
			parser.PassChar('=');
			String scan = parser.ReadOneString();
			scanner = parser.GetPtr();
			if(name == "header")
				setup.header_file = scan;
			else if(name == "class")
			{
				setup.wrapper_name = scan;
				setup.wrapper_kind = setup.CLASS_WRAPPER;
			}
			else if(name == "space")
			{
				setup.wrapper_name = scan;
				setup.wrapper_kind = setup.SPACE_WRAPPER;
			}
			else if(name == "java_file")
			{
				setup.java_export = Setup::FILE_EXPORT;
				setup.java_source = scan;
			}
			else if(name == "java_prog")
			{
				setup.java_export = Setup::CPP_EXPORT;
				setup.java_source = scan;
			}
			else if(name == "java_package")
				setup.java_package = scan;
			else if(name == "gif_folder")
				setup.gif_folder = scan;
			else if(name == "gif_transparent")
			{
				int c = stou(scan);
				setup.gif_transparent = Color((c >> 16) & 255, (c >> 8) & 255, c & 255);
			}
			else if(name == "icon_file")
				setup.icon_file = scan;
			else if(name == "icon_small")
				setup.icon_small = scan;
			else if(name == "icon_large")
				setup.icon_large = scan;
			else if(name == "icon_bpp")
			{
				setup.icon_bpp = atoi(scan);
				if(setup.icon_bpp != 1 && setup.icon_bpp != 4 && setup.icon_bpp != 8
				&& setup.icon_bpp != 16 && setup.icon_bpp != 24)
					setup.icon_bpp = 8;
			}
		}
		else if((bid = GetIdent()) == "IMAGE_BEGIN"
		&& Check('(') && !IsNull(name = GetIdent()) && Check(')'))
		{
			setup.wince_16bit = false;
			String data;
			String id;
			bool first = true;
			while((id = GetIdent()) == "IMAGE_SCAN" && Check('('))
			{
				bool first_in_row = true;
				while(SkipWhite() == '\"')
				{
					CParser parser(scanner);
					String scan = parser.ReadOneString();
					scanner = parser.GetPtr();
					if(!first && first_in_row)
						data.Cat('\x80');
					first_in_row = first = false;
					data.Cat(scan);
				}
				if(!Check(')'))
					break;
			}
			Image::Info idata;
			bool accepted = false;
			if(Check('(') && GetIdent() == name && Check(','))
				if(id == "IMAGE_END"
				&& (idata.size.cx = GetInt()) > 0 && Check(',')
				&& (idata.size.cy = GetInt()) > 0 && Check(')'))
				{
					accepted = true;
				}
				else if(id == "IMAGE_PACKED" && SkipWhite() == '\"')
				{
					CParser parser(scanner);
					String d = parser.ReadOneString();
					scanner = parser.GetPtr();
					if(Check(')'))
					{
						StringStream ss(d);
						ss % idata;
						if(!ss.IsError())
							accepted = true;
					}
				}

			if(name.GetLength() >= 6 && !memcmp(name, "_java_", 6))
				accepted = false;

			if(accepted)
			{
				if(name.GetLength() >= 4 && !memcmp(name, anon_name, sizeof(anon_name) - 1))
					name = Null;
				int r = image_list.list.GetCount();
				image_list.list.Add();
				image_list.list.Set(r, CWIDTH,  (int)idata.size.cx);
				image_list.list.Set(r, CHEIGHT, (int)idata.size.cy);
				image_list.list.Set(r, CFORMAT, (int)idata.encoding);
				image_list.list.Set(r, CHOTX,   (int)idata.hotspot.x);
				image_list.list.Set(r, CHOTY,   (int)idata.hotspot.y);
				image_list.list.Set(r, CNAME,   name);
				image_list.list.Set(r, CDATA,   data);
			}
		}
		else if(bid == "IMAGE_BEGIN16" && Check('(') && !IsNull(name = GetIdent()) && Check(')'))
		{
			setup.wince_16bit = true;
			String data;
			String id;
			bool first = true;
			while((id = GetIdent()) == "IMAGE_SCAN16" && Check('(') && Check('L'))
			{
				bool first_in_row = true;
				while(Check('\"'))
				{
					String scan = GetUnicodeScan();
					if(!Check('\"'))
						break;
					if(!first && first_in_row)
						data.Cat('\x80');
					first_in_row = first = false;
					data.Cat(scan);
				}
				if(!Check(')'))
					break;
			}
			Image::Info idata;
			bool accepted = false;
			if(id == "IMAGE_END16" && Check('(') && GetIdent() == name && Check(',')
			&& (idata.size.cx = GetInt()) > 0 && Check(',')
			&& (idata.size.cy = GetInt()) > 0 && Check(',')
			&& !IsNull(idata.hotspot.x = GetInt()) && Check(',')
			&& !IsNull(idata.hotspot.y = GetInt()) && Check(')'))
			{
				accepted = true;
			}

			if(accepted)
			{
				if(name.GetLength() >= 4 && !memcmp(name, anon_name, sizeof(anon_name) - 1))
					name = Null;
				int r = image_list.list.GetCount();
				image_list.list.Add();
				image_list.list.Set(r, CWIDTH,  (int)idata.size.cx);
				image_list.list.Set(r, CHEIGHT, (int)idata.size.cy);
				image_list.list.Set(r, CFORMAT, (int)idata.encoding);
				image_list.list.Set(r, CHOTX,   (int)idata.hotspot.x);
				image_list.list.Set(r, CHOTY,   (int)idata.hotspot.y);
				image_list.list.Set(r, CNAME,   name);
				image_list.list.Set(r, CDATA,   data);
			}
		}
		// skip to end of line
		while(*scanner && *scanner++ != '\n')
			;
	}
}
*/

bool DlgImage::HotKey(dword key)
{
	if(image_designer.Key(key, 1))
		return true;
	switch(key)
	{
	case K_CTRL_UP:
		OnEditMoveUp();
		break;

	case K_CTRL_DOWN:
		OnEditMoveDown();
		break;

	case K_UP:
	case K_ALT_UP:
	case K_PAGEUP:
		if(image_list.list.GetCursor() > 0)
			image_list.list.SetCursor(image_list.list.GetCursor() - 1);
		return true;

	case K_DOWN:
	case K_ALT_DOWN:
	case K_PAGEDOWN:
		if(image_list.list.GetCursor() + 1 < image_list.list.GetCount())
			image_list.list.SetCursor(image_list.list.GetCursor() + 1);
		return true;
	}
	return Ctrl::HotKey(key);
}

bool DlgImage::OnImageKey(dword key)
{
	if(key > ' ' && key < 0x7F && (IsAlNum(key) || key == '_'))
	{
		image_list.name.SetFocus();
		image_list.name.SetSelection(image_list.name.GetLength());
		return image_list.name.Key(key, 1);
	}
	return false;
}

void DlgImage::OnImageCursor(int i)
{
	image_list.list.SetCursor(i);
}

bool DlgImage::OnNameKey(dword key)
{
	switch(key)
	{
	case K_UP:
	case K_PAGEUP:
	case K_DOWN:
	case K_PAGEDOWN:
		return image_list.list.Key(key, 1);

	case K_CTRL_V:
#ifdef PLATFORM_WIN32
		if(!IsClipboardFormatAvailable(CF_TEXT) && IsPasteAvailable())
		{
			OnEditPaste();
			return true;
		}
#endif
		break;
	}
	return false;
}

void DlgImage::OnSetTransparentColor()
{
	image_designer.Refresh();
	image_small.Refresh();
}

void DlgImage::OnListMenu(Bar& bar)
{
	ToolEditInsert(bar);
	ToolEditDuplicate(bar);
	ToolEditInsertPaste(bar);
	ToolEditInsertFile(bar);
	ToolEditDelete(bar);
}

AlphaImageInfo DlgImage::Get(int row)
{
	AlphaImageInfo data;
	if(row >= 0 && row < image_list.list.GetCount())
	{
		data.size.cx = max((int)image_list.list.Get(row, CWIDTH), 1);
		data.size.cy = max((int)image_list.list.Get(row, CHEIGHT), 1);
		data.hotspot.x = (int)image_list.list.Get(row, CHOTX);
		data.hotspot.y = (int)image_list.list.Get(row, CHOTY);
		data.encoding = static_cast<AlphaImageInfo::ENCODING>((int)image_list.list.Get(row, CFORMAT));
	}
	return data;
}

void DlgImage::Set(int row, const IMLImageInfo& data)
{
	int i = image_list.list.GetCursor();
	image_list.list.Set(row, CWIDTH,  (int)data.size.cx);
	image_list.list.Set(row, CHEIGHT, (int)data.size.cy);
	image_list.list.Set(row, CFORMAT, (int)data.encoding);
	image_list.list.Set(row, CHOTX,   (int)data.hotspot.x);
	image_list.list.Set(row, CHOTY,   (int)data.hotspot.y);
	if(!data.encoded_data.IsVoid())
		image_list.list.Set(row, CDATA, data.encoded_data);
	if(!data.name.IsVoid())
		image_list.list.Set(row, CNAME, data.name);
	image_list.list.SetCursor(i);
}

int DlgImage::InsertRaw(int row, const IMLImageInfo& data)
{
	row = minmax(row, 0, image_list.list.GetCount());
	int i = image_list.list.GetCursor();
	Vector<Value> values;
	values.SetCount(image_list.list.GetIndexCount());
	image_list.list.Insert(row, values);
	Set(row, data);
	image_list.list.SetCursor(i);
	return row;
}

/*
void DlgImage::OnImageUndoRedo()
{
	UpdateCaption();
}
*/

void DlgImage::AddUndo(One<UndoEntry> entry)
{
	if(entry -> IsSnapshot() && undo_pos > 0 && *entry == undo[undo_pos - 1])
		return;
	undo.SetCount(undo_pos);
	if(!entry -> IsSnapshot())
	{
		if(image_list.list.IsCursor() && undo_pos > 0 && undo[undo_pos - 1].IsSnapshot())
		{ // add fixup snapshot
			undo.Add(new UndoEntryPaint(image_designer, t_("painting")));
			undo_pos = undo.GetCount();
		}
		entry -> Redo();
	}
	undo.Add(-entry);
	undo_pos++;
	if(undo.GetCount() >= MAX_UNDO)
	{
		int remove = undo.GetCount() - MAX_UNDO;
		undo.Remove(0, remove);
		undo_pos -= remove;
		if(undo_anchor >= 0)
			undo_anchor = max(undo_anchor - remove, -1);
		ASSERT(undo_pos >= 0);
	}
	UpdateCaption();
	Rescan();
}

void DlgImage::SetUndoAnchor()
{
	undo_anchor = undo_pos;
}

void DlgImage::ClearUndo()
{
	undo.Clear();
	undo_anchor = undo_pos = 0;
	UpdateCaption();
	Rescan();
}

/*
void DlgImage::CloseDesigner() {
	Array<DlgImage>& list = GetDesigners();
	for(int i = 0; i < list.GetCount(); i++)
		if(&list[i] == this)
		{
			list.Remove(i);
			return;
		}
}
*/

/*
bool CloseImageLayout() {
	Array<DlgImage>& list = GetDesigners();
	while(!list.IsEmpty())
	{
		if(!list[0].FlushFile())
			return false;
		list.Remove(0);
	}
	return true;
}
*/

/*
bool SaveImageLayout() {
	Array<DlgImage>& list = GetDesigners();
	bool is_ok = true;
	for(int i = 0; i < list.GetCount(); i++)
		if(!list[i].DoSaveFile())
			is_ok = false;
	return is_ok;
}
*/

/*
void ImageLayout(const String& open_file) {
	Array<DlgImage>& list = GetDesigners();
	for(int i = 0; i < list.GetCount(); i++)
		if(!ComparePath(list[i].GetFileName(), open_file))
		{
			list[i].SetWantFocus();
			return;
		}
	DlgImage& ld = list.Add();
	ld.DoLoad(open_file);
	ld.OpenWindow();
}
*/

//Image GetIwIcon() {
//	return MiscImg::small_app();
//}

bool IsIconFile(const char *path)
{
	return !stricmp(GetFileExtPos(path), ".iml");
}

struct IconDesModule : public IdeModule {
	virtual Image FileIcon(const char *path) {
		return IsIconFile(path) ? MiscImg::small_app() : Null;
	}
	virtual IdeDesigner *CreateDesigner(const char *path, byte cs) {
		if(IsIconFile(path)) {
			One<IconDesigner> d = new IconDesigner();
			d->designer->LoadConfig();
			if(!d->designer->DoLoad(path, false))
				return false;
			d->SizePos();
			return d.Detach();
		}
		return false;
	}
	virtual void Serialize(Stream& s) {
		int version = 0;
		s / version;
		//SerializeLayEditPos(s);
	}
};

void RegisterIconDes()
{
	RegisterIdeModule(Single<IconDesModule>());
}

INITBLOCK {
	RegisterGlobalConfig("icondes-ctrl");
}
