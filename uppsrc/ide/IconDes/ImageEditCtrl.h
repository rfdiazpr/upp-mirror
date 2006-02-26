#ifndef __DESIGNER_IMAGEEDITCTRL__
#define __DESIGNER_IMAGEEDITCTRL__

#include "PaletteCtrl.h"

class ImageEditCtrl;
class ImageEditAdapter;

#define IMAGEFILE <ide/IconDes/misc.iml>
#define IMAGESPACE MiscImg
#include <Draw/iml_header.h>

class UndoEntry
{
public:
	UndoEntry(int snapshot_cursor = -1) : snapshot_cursor(snapshot_cursor) {}
	virtual ~UndoEntry() {}

	int            GetSnapshotCursor() const        { return snapshot_cursor; }
	bool           IsSnapshot() const               { return snapshot_cursor >= 0; }

	friend bool    operator == (const UndoEntry& e, const UndoEntry& f) { return e.Equals(f); }
	friend bool    operator != (const UndoEntry& e, const UndoEntry& f) { return !e.Equals(f); }

	// pure virtual functions cannot be put into Array
	virtual String Format() const                   { NEVER(); return String::GetVoid(); }
	virtual void   Undo()                           { NEVER(); }
	virtual void   Redo()                           { NEVER(); }
	virtual bool   Equals(const UndoEntry& e) const { NEVER(); return false; }

private:
	int            snapshot_cursor;
};

class ImagePreviewCtrl : public Ctrl
{
public:
	ImagePreviewCtrl() : background(LtGray) {}

	void         Set(const AlphaArray& buffer, Color background);
	virtual void Paint(Draw& draw);

private:
	Image        image;
	Color        background;
};

class ImageSmallCtrl;

class ImageEditCtrl : public Ctrl
{
public:
	typedef ImageEditCtrl CLASSNAME;
	ImageEditCtrl();
	virtual ~ImageEditCtrl();

	virtual void  Serialize(Stream& stream);
	void          SerializeUndo(Stream& stream);

	virtual void  Layout();
	virtual void  Paint(Draw& draw);

	virtual void  LeftDown(Point pt, dword keyflags);
	virtual void  LeftUp(Point pt, dword keyflags);
	virtual void  MouseMove(Point pt, dword keyflags);
	virtual Image CursorImage(Point pt, dword keyflags);

	virtual void  RightDown(Point pt, dword keyflags);

	virtual bool  Key(dword key, int count);

	virtual void  PreDestroy();

	void          SetImageSize(Size _size);
	Size          GetImageSize() const              { return image.GetSize(); }
	bool          IsEmpty() const                   { return image.IsEmpty(); }

	void          SetImage(pick_ AlphaArray& _image); // pick

	void          SetHotSpot(Point hotspot);
	Point         GetHotSpot() const                { return hotspot; }

	void         SetZoom(int _zoom);
	int          GetZoom() const                   { return zoom; }

	Size         GetZoomedSize() const             { return zoomed_size; }

	void         SetColor(Color _color)            { palette -> Set(_color); }
	Color        GetColor() const                  { return palette -> Get(); }
	Color        GetTransparentColor() const       { return palette -> GetTransparent(); }

	void         SetScroll(Point pt)               { scroll = pt; }
	void         SetViewCentre(Point pt);
	Point        GetScroll() const                 { return scroll; }

	void         SetCursor(int i)                  { cursor = i; }
	int          GetCursor() const                 { return cursor; }

	void         SetPalette(PaletteCtrl *_palette) { palette = _palette; }
	PaletteCtrl *GetPalette() const                { return palette; }

	void         SetSelection(const Rect& rc);
	void         ClearSelection()                  { SetSelection(Rect(Null)); }
	const Rect&  GetSelection() const              { return selection; }
	Rect         GetSelectionOrImage() const;
	bool         IsSelectionAvailable() const      { return !selection.IsEmpty(); }

	void         SetSelectionImage(pick_ AlphaArray& sel_image);
	void         ClearSelectionImage()             { SetSelectionImage(AlphaArray()); }
	const AlphaArray& GetSelectionImage()          { return selection_image; }
	void         ForceSelectionImage();

	void         SetSelectionCrop(const Rect& rc);
	const Rect&  GetSelectionCrop() const          { return crop; }

	void         ClearSelectionAndImage();
	void         PasteSelection(bool add_undo = true);
	void         PasteAndClearImage(bool add_undo = true);

	void         AddUndo(const char *cmd);

//	void         Touch();
//	void         ClearUndo();
//	void         SetUndoAnchor() { undo_anchor = undo_pos; }

//	bool         IsDirty() const { return undo_pos != undo_anchor; }

	void         DrawCurve(Draw& draw, const Vector<Point>& curve, bool undraw);

	void         RotateSelectionClockwise();
	void         RotateSelectionAnticlockwise();

	void         CropSelection();

	void         SetLeftButton(One<ImageEditAdapter> (*newproc)());

	Point        ClientToImage(Point pt) const;
	Rect         ClientToImage(const Rect& rc) const;
	Point        ImageToClient(Point pt) const;
	Rect         ImageToClient(const Rect& rc) const;

	enum
	{
		LEFT_HIT   = 0x01,
		TOP_HIT    = 0x02,
		RIGHT_HIT  = 0x04,
		BOTTOM_HIT = 0x08,
	};

	int          GetHitTest(Point pt) const;

	void         RefreshImage(Point pt);
	void         RefreshImage(const Rect& rc);
	void         RefreshImage()                   { RefreshImage(Rect(GetImageSize())); }
	void         RefreshSelectionRect(const Rect& rc);

	void         PaintFrame(Draw& draw, const Rect& rc) const;

	void         HideLeftButton();

	void         OnZoomCombo();

	void         ShowCoords(Point ip);
	void         ShowCoords(const Rect& rc);
	void         ShowCoords(Point a, Point b);

	void         SetPixel(int x, int y, Color color);
	void         SetPixel(Point pt, Color color) { SetPixel(pt.x, pt.y, color); }

	// ToolView
	void         ToolView(Bar& bar);
		void         ToolViewZoomIn(Bar& bar);
		void         OnViewZoomIn();
		void         ToolViewZoomOut(Bar& bar);
		void         OnViewZoomOut();
		void         ToolViewZoomFull(Bar& bar);
		void         OnViewZoomFull();

	void         ToolViewZoomCombo(Bar& bar);

	void         ToolMode(Bar& bar);

		void         ToolModeZoomWindow(Bar& bar);
		void         OnModeZoomWindow();

		void         ToolModeSelect(Bar& bar);
		void         OnModeSelect();

		void         ToolModeFreehand(Bar& bar);
		void         OnModeFreehand();

		void         ToolModeLine(Bar& bar);
		void         OnModeLine();

		void         ToolModeRectangle(Bar& bar);
		void         OnModeRectangle();

		void         ToolModeFilledRectangle(Bar& bar);
		void         OnModeFilledRectangle();

		void         ToolModeFloodFill(Bar& bar);
		void         OnModeFloodFill();

		void         ToolModeHotSpot(Bar& bar);
		void         OnModeHotSpot();

	void         ToolSelect(Bar& bar);

		void         ToolSelectClearSelection(Bar& bar);
		void         OnSelectClearSelection();

		void         ToolSelectSelectAll(Bar& bar);
		void         OnSelectSelectAll();

		void         ToolSelectFillSelection(Bar& bar);
		void         OnSelectFillSelection();

		void         ToolSelectTransparentSelection(Bar& bar);
		void         OnSelectTransparentSelection();

		void         ToolSelectCopy(Bar& bar);
		void         OnSelectCopy();

		void         ToolSelectCut(Bar& bar);
		void         OnSelectCut();

		void         ToolSelectChangeColor(Bar& bar);
		void         OnSelectChangeColor();

		void         ToolSelectGrayed(Bar& bar);
		void         OnSelectGrayed();

		void         ToolSelectEtched(Bar& bar);
		void         OnSelectEtched();

		void         ToolSelectMirrorX(Bar& bar);
		void         OnSelectMirrorX();

		void         ToolSelectMirrorY(Bar& bar);
		void         OnSelectMirrorY();

		void         ToolSelectMirrorXY(Bar& bar);
		void         OnSelectMirrorXY();

		void         ToolSelectMatrix(Bar& bar);
		void         OnSelectMatrix();

	void         ToolSetup(Bar& bar);

		void         ToolSetupPasteTransparent(Bar& bar);
		void         OnSetupPasteTransparent();

		void         ToolSetupDrawGrid(Bar& bar);
		void         OnSetupDrawGrid();


	Callback WhenSyncBar;

public:
	static const int zoom_list[];
	static int   zoom_count;

	// image parameters
	AlphaArray   image;
	Point        hotspot;

	// modes
	bool         paste_transparent;
	enum GRID { NONE, SINGLE, DITHERED };
	GRID         grid;
	Color        grid_color;

	Gate1<dword>               WhenKey;
	Callback1<String>          WhenCoords;
	Callback1<One<UndoEntry> > WhenAddUndo;
	Callback1<int>             WhenSetCursor;
	ImageSmallCtrl *small_image;

private:
	Point        AdjustOffset(Point offset) const;
	void         OnScroll();

private:
	int          cursor;
	int          zoom;
	ScrollBars   scroll;
	Point        offset;
	Size         zoomed_size;
	Label        zoom_label;
	DropList     zoom_combo;
	PaletteCtrl *palette;

	// edit interface
	One<ImageEditAdapter>     left_button;
	One<ImageEditAdapter> (*left_type)();
	Point        start, last;
	enum DRAG_STATE { OFF, ON, MAYBE } drag_state;

	Rect         selection, crop;
	AlphaArray   selection_image;
	Rect         restore_on_cancel;

/*
	struct UndoEntry
	{
		UndoEntry(const String& data = Null, const String& command = Null) : data(data), command(command) {}

		String data;
		String command;
	};
	template class Moveable<UndoEntry>;
*/

//	Vector<UndoEntry> undo;
//	int            undo_pos, undo_anchor;

//	Rect         undo_rc, redo_rc;
//	PopUpTable   command_list;
//	Bar         *tool_bar;
};

class UndoEntryPaint : public UndoEntry
{
public:
	UndoEntryPaint(ImageEditCtrl& editor, String command);

	virtual String Format() const { return command; }
	virtual void   Undo();
	virtual void   Redo()         {}
	virtual bool   Equals(const UndoEntry& entry) const;

private:
	ImageEditCtrl& editor;
	String         command;
	String         data;
};

class ImageSmallCtrl : public Ctrl
{
public:
	typedef ImageSmallCtrl CLASSNAME;
	ImageSmallCtrl();

	void           Attach(ImageEditCtrl *owner);

	virtual void   Paint(Draw& draw);
	virtual void   Layout();

	virtual void   LeftDown(Point pt, dword keyflags);

	void           OnScroll();

	void           RefreshImage(Point pt);
	void           RefreshImage(const Rect& rc);

private:
	Point          AdjustOffset(Point pos) const;

private:
	ImageEditCtrl *owner;
	Point          offset;
	Size           step;
	int            count;
	Color          bglist[5];
	ScrollBars     scroll;
};

class ImageEditAdapter
{
public:
	ImageEditAdapter() : image(0) {}
	virtual ~ImageEditAdapter() {}

	virtual void     Attach(ImageEditCtrl& _image) { image = &_image; }

	virtual void     PushClient(Point pt, dword keyflags);
	virtual void     DragClient(Point start, Point last, Point next, dword keyflags);
	virtual void     DropClient(Point start, Point end, dword keyflags);
	virtual Image    CursorClient(Point pt, dword keyflags);
	virtual bool     Key(dword key);

	virtual void     Push(Point pt, dword keyflags);
	virtual void     Drag(Point start, Point last, Point next, dword keyflags);
	virtual void     Drop(Point start, Point end, dword keyflags);
	virtual Image    Cursor(Point pt, dword keyflags);

	virtual void     DragRect(const Rect& last, const Rect& next, dword keyflags);
	virtual void     DropRect(const Rect& rc, dword keyflags);

protected:
	ImageEditCtrl   *image;
};

One<ImageEditAdapter> GetAdapterFreehand();
One<ImageEditAdapter> GetAdapterZoomWindow();
One<ImageEditAdapter> GetAdapterSelect();
One<ImageEditAdapter> GetAdapterLine();
One<ImageEditAdapter> GetAdapterRectangle();
One<ImageEditAdapter> GetAdapterFilledRectangle();
One<ImageEditAdapter> GetAdapterFloodFill();
One<ImageEditAdapter> GetAdapterHotSpot();

#endif//__DESIGNER_IMAGEEDITCTRL__
