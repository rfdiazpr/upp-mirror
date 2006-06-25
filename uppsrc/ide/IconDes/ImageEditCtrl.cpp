#include "IconDes.h"

#define IMAGECLASS EditImg
//#define IMAGECLASS EditImg
#define IMAGEFILE  <ide/IconDes/editor.iml>
#include <Draw/iml.h>

#define IMAGECLASS  MiscImg
#define IMAGEFILE   <ide/IconDes/misc.iml>
#include <Draw/iml_source.h>

#define LAYOUTFILE <ide/IconDes/editor.lay>
#include <CtrlCore/lay.h>

void ImagePreviewCtrl::Set(const Image& img, Color bg)
{
	background = bg;
	ImageDraw iw(img.GetSize());
	iw.DrawRect(img.GetSize(), background);
	iw.DrawImage(0, 0, img);
	image = iw;
	Refresh();
}

void ImagePreviewCtrl::Paint(Draw& draw)
{
	draw.DrawRect(GetSize(), background);
	Size bs = image.GetSize(), ws = GetSize(), ratio = ws / bs;
	int r = max(1, min(ratio.cx, ratio.cy));
	Size spart = (r * min(bs, ws)) / r, dpart = spart * r;
	if(spart.cx > 0 && spart.cy > 0)
	{
		Point pos = (GetSize() - dpart) >> 1;
		Rect drc(pos, dpart);
		DrawRectMinusRect(draw, ws, drc, background);
		draw.DrawImage(drc.left, drc.top, image);
	}
}

enum
{
	SCROLL_SIZE = 16,
	BUTTONSIZE = 8,
	SEL_RC = 2,
	VIEW_GAP = 16,
	MIN_GRID_ZOOM = 4,
};

//$-
const int ImageEditCtrl::zoom_list[] =
{
	1, 2, 3, 4, 6, 8, 10, 12, 16, 20, 24, 28, 32,
};

int ImageEditCtrl::zoom_count = __countof(zoom_list);
//$+
ImageSmallCtrl::ImageSmallCtrl()
: owner(0)
, offset(0, 0)
{
	SetFrame(InsetFrame());
	AddFrame(scroll);
	scroll.WhenScroll = THISBACK(OnScroll);
}

void ImageSmallCtrl::Attach(ImageEditCtrl *_owner)
{
	owner = _owner;
	owner->small_image = this;
	Layout();
}

void ImageSmallCtrl::Paint(Draw& draw)
{
	Size size(0, 0);
	if(owner)
		size = owner->GetImageSize();
	Rect clip = draw.GetClip();
	int dd = (count - 1) >> 1;
	if(size.cx && size.cy)
		for(int inst = -dd; inst <= +dd; inst++)
		{
			Rect whole = clip;

			if(inst > -dd)
				if(step.cx)
					whole.left = step.cx * (inst + dd);
				else
					whole.top = step.cy * (inst + dd);
			if(inst < +dd)
				if(step.cx)
					whole.right = step.cx * (inst + dd + 1);
				else
					whole.bottom = step.cy * (inst + dd + 1);

			if(whole.IsEmpty())
				continue;

			Size s = step * inst;
			Rect src = (clip + offset - s) & Rect(size);
			Rect drc = src - offset + s;

			Color trans = Nvl(bglist[inst + dd], owner->GetTransparentColor());

			draw.DrawRect(whole, trans);

			draw.DrawImage(drc.left, drc.top, owner->image, src);

			const Image& sim = owner->GetSelectionImage();
			if(owner->IsSelectionAvailable() && !sim.IsEmpty())
			{
				Rect org = owner->GetSelection();
				Rect sel = org & src;
				if(!sel.IsEmpty())
				{
					Rect crop = owner->GetSelectionCrop();
					Rect src(crop.TopLeft() + (sel.TopLeft() - org.TopLeft()), sel.Size());
					src &= Rect(sim.GetSize());
					src &= crop;
					if(!src.IsEmpty())
					{
						Rect rc = src + org.TopLeft() - offset - crop.TopLeft();
						if(!owner->paste_transparent)
							draw.DrawRect(rc, trans);
						draw.DrawImage(rc.left, rc.top, sim, src);
					}
				}
			}
		}
	else
		draw.DrawRect(clip, SLtGray);
}

void ImageSmallCtrl::LeftDown(Point pt, dword keyflags)
{
	if(owner)
	{
		Size size = owner->GetImageSize();
		pt = minmax<Point>(Point(0, 0), pt + offset, size);
//		pt = Point(
//			minmax(pt.x + offset.x, 0, size.cx - 1),
//			minmax(pt.y + offset.y, 0, size.cy - 1));
		owner->SetViewCentre(pt);
	}
}

void ImageSmallCtrl::Layout()
{
	if(owner)
	{
		Size im = owner->GetImageSize() + VIEW_GAP;
		Size cl = GetSize();
		if(cl.cx > cl.cy)
		{ // horizontal direction
			count = (3 * im.cx >= cl.cx ? 1 : 5 * im.cx >= cl.cx ? 3 : 5);
			step = Size(cl.cx / count, 0);
		}
		else
		{
			count = (3 * im.cy >= cl.cy ? 1 : 5 * im.cy >= cl.cy ? 3 : 5);
			step = Size(0, cl.cy / count);
		}
		switch(count)
		{
		case 5:
			bglist[0] = Black;
			bglist[1] = Gray;
			bglist[2] = Null;
			bglist[3] = LtGray;
			bglist[4] = White;
			break;

		case 3:
			bglist[0] = Black;
			bglist[1] = Null;
			bglist[2] = White;
			break;

		default:
			bglist[0] = Null;
			break;
		}
		offset = AdjustOffset(scroll);
		scroll.Set(offset, GetSize(), owner->GetImageSize() + 2 * VIEW_GAP);
		Refresh();
	}
}

void ImageSmallCtrl::OnScroll()
{
	Point newpos = AdjustOffset(scroll);
	if(!IsFullRefresh())
	{
		Sync();
		Size delta = offset - newpos;
		offset = newpos;
		ScrollView(delta);
	}
	else
		offset = newpos;
}

Point ImageSmallCtrl::AdjustOffset(Point pos) const
{
	Size size = GetSize() - 2 * VIEW_GAP;
	Size im = owner->GetImageSize();
	if(im.cx < size.cx)
		pos.x = (im.cx - size.cx) >> 1;
	if(im.cy < size.cy)
		pos.y = (im.cy - size.cy) >> 1;
	return pos - VIEW_GAP;
}

void ImageSmallCtrl::RefreshImage(Point pt)
{
	RefreshImage(Rect(pt, Size(1, 1)));
}

void ImageSmallCtrl::RefreshImage(const Rect& rc)
{
	if(owner)
	{
		Rect r = rc & Rect(owner->GetImageSize());
		if(!r.IsEmpty())
		{
			int dd = (count - 1) >> 1;
			for(int i = -dd; i <= +dd; i++)
				Refresh(r - offset + i * step);
		}
	}
}

//$-
static const ColorF CommandListBorder[] =
{
	(ColorF)5,
	&SBlack,  &SBlack,  &SBlack,  &SBlack,
	&SLtGray, &SLtGray, &SLtGray, &SLtGray,
	&SLtGray, &SLtGray, &SLtGray, &SLtGray,
	&SGray,   &SGray,   &SWhite,  &SWhite,
	&SBlack,  &SBlack,  &SLtGray, &SLtGray,
};

static BorderFrame CommandListFrame(CommandListBorder);
//$+

UndoEntryPaint::UndoEntryPaint(ImageEditCtrl& editor, String command)
: UndoEntry(editor.GetCursor())
, editor(editor)
, command(command)
{
	StringStream ss;
	editor.SerializeUndo(ss);
	data = ss;
}

void UndoEntryPaint::Undo()
{
	editor.WhenSetCursor(GetSnapshotCursor());
	StringStream ss(data);
	editor.SerializeUndo(ss);
}

bool UndoEntryPaint::Equals(const UndoEntry& entry) const
{
	const UndoEntryPaint *p = dynamic_cast<const UndoEntryPaint *>(&entry);
	return p && p->GetSnapshotCursor() == GetSnapshotCursor() && p->data == data;
}

ImageEditCtrl::ImageEditCtrl()
: zoom(16)
, offset(0, 0)
, palette(0)
, selection(Null)
, crop(Null)
, grid(DITHERED)
, grid_color(Black)
, small_image(0)
, hotspot(0, 0)
, restore_on_cancel(Null)
, cursor(0)
//, undo_pos(0)
//, undo_anchor(0)
//, tool_bar(0)
{
//	command_list.AutoHideSb().NoHeader().NoGrid().MouseMoveCursor();
//	command_list.AddItem();
//	command_list.AddColumn();
//	command_list.SetFrame(CommandListFrame);

	BackPaint();
	SetFrame(InsetFrame());
	AddFrame(scroll);
	scroll.AutoHide();
	scroll.WhenScroll = THISBACK(OnScroll);
	drag_state = OFF;
//	__asm int 3
	SetLeftButton(&GetAdapterSelect);
	zoom_combo.SetRect(0, 0, 50, 20);
	for(int i = 0; i < __countof(zoom_list); i++)
		zoom_combo.Add(zoom_list[i], NFormat("%d x", zoom_list[i]));
	zoom_combo.WhenAction = THISBACK(OnZoomCombo);
	zoom_combo <<= zoom;
}

ImageEditCtrl::~ImageEditCtrl()
{
}

void ImageEditCtrl::Serialize(Stream& stream)
{
	int version = StreamHeading(stream, 3, 3, 3, "ImageEditCtrl");
	if(version >= 1)
	{
		int z = zoom;
		stream % zoom;
		bool shit = 0;
		stream.Pack(shit, paste_transparent);
		if(stream.IsLoading())
			SetZoom(z);
		int v = (int)grid;
		stream / v;
		grid = (GRID)v;
		stream % grid_color;
	}
	if(stream.IsLoading())
		Refresh();
}

void ImageEditCtrl::SerializeUndo(Stream& stream)
{
	int version = 1;
	Size size = GetImageSize();
	Point hotspot = GetHotSpot();
	String rle;
	if(stream.IsStoring())
		rle = StoreImageAsString(image);
	stream / version % size % hotspot % rle;
	if(stream.IsLoading())
	{
		SetImageSize(size);
		SetHotSpot(hotspot);
		image = LoadImageFromString(rle);
	}
}

void ImageEditCtrl::SetImageSize(Size _size)
{
	ImageBuffer b(_size);
	image = b;
	Layout();
}

void ImageEditCtrl::SetImage(const Image& _image)
{
	image = _image;
	SetHotSpot(Point(0, 0));
	Layout();
//	ClearUndo();
}

void ImageEditCtrl::SetHotSpot(Point pt)
{
	if(hotspot != pt)
	{
		Size spot = EditImg::hotspot_image().GetSize(), half = (spot - zoom) >> 1;
		Refresh(Rect(ImageToClient(hotspot) - half, spot));
		Refresh(Rect(ImageToClient(pt) - half, spot));
		hotspot = pt;
	}
}

void ImageEditCtrl::SetZoom(int _zoom)
{
	Size size = GetSize();
	Point ic = ClientToImage(Point(size >> 1));
	if(zoomed_size.cx < size.cx) ic.x = image.GetSize().cx >> 1;
	if(zoomed_size.cy < size.cy) ic.y = image.GetSize().cy >> 1;
	zoom = _zoom;
	zoom_combo <<= zoom;
	Layout();
	scroll = ic * zoom - (size >> 1);
	WhenSyncBar();
}

void ImageEditCtrl::SetLeftButton(One<ImageEditAdapter> (*newproc)())
{
	if(left_type == &GetAdapterHotSpot || newproc == &GetAdapterHotSpot)
		Refresh();
	HideLeftButton();
	left_button = newproc();
	left_type = newproc;
	if(left_button)
		left_button->Attach(*this);
	PasteAndClearImage();
	ClearSelection();
	WhenSyncBar();
}

void ImageEditCtrl::HideLeftButton()
{
	if(drag_state == ON && left_button)
		left_button->DragClient(start, last, Null, 0);
	drag_state = OFF;
	WhenSyncBar();
}

Point ImageEditCtrl::AdjustOffset(Point pt) const
{
	Size size = GetSize() - 2 * VIEW_GAP;
	if(zoomed_size.cx < size.cx)
		pt.x = (zoomed_size.cx - size.cx) >> 1;
	if(zoomed_size.cy < size.cy)
		pt.y = (zoomed_size.cy - size.cy) >> 1;
	return pt - VIEW_GAP;
}

void ImageEditCtrl::Layout()
{
	zoomed_size = image.GetSize() * zoom;
	offset = AdjustOffset(scroll);
	Refresh();
	scroll.Set(offset + VIEW_GAP, GetSize(), zoomed_size + 1 + 2 * VIEW_GAP);
	scroll.SetLine(zoom, zoom);
	if(small_image)
		small_image->Layout();
}

static void DrawButton(Draw& draw, int x, int y)
{
	if(draw.GetClip().Intersects(Rect(x, y, x + BUTTONSIZE, y + BUTTONSIZE)))
	{
		DrawFrame(draw, x, y, BUTTONSIZE, BUTTONSIZE, White, Black);
		draw.DrawRect(x + 1, y + 1, BUTTONSIZE - 2, BUTTONSIZE - 2, LtGray);
	}
}

static void SmartIntersect(Rect& dest, const Rect& src)
{
	if(src.left   > dest.left)   dest.left   = src.left;
	if(src.right  < dest.right)  dest.right  = src.right;
	if(src.top    > dest.top)    dest.top    = src.top;
	if(src.bottom < dest.bottom) dest.bottom = src.bottom;
}

void ImageEditCtrl::Paint(Draw& draw)
{
	LOG("offset = " << offset);

	Size im = image.GetSize();
	if(im.cx == 0 || im.cy == 0)
	{
		draw.DrawRect(draw.GetClip(), SLtGray);
		return;
	}

	Rect out_rc = ImageToClient(Rect(im));

	Rect bar_rc = out_rc;
	if(zoom >= MIN_GRID_ZOOM && grid != NONE)
	{
		bar_rc.right++;
		bar_rc.bottom++;
	}

	DrawRectMinusRect(draw, GetSize(), bar_rc, White);

	Image m = image;
	if(!selection.IsEmpty())
		if(paste_transparent)
			Over(m, selection.TopLeft(), selection_image, selection.GetSize());
		else
			Copy(m, selection.TopLeft(), selection_image, selection.GetSize());
	const RGBA *s = m;
	for(int y = 0; y < im.cy; y++)
		for(int x = 0; x < im.cx; x++) {
			Color c = *s;
			draw.DrawRect(x * zoom + out_rc.left, y * zoom + out_rc.top, zoom, zoom,
			              s->a == 255 ? c : palette->GetTransparent());
			s++;
		}


	if(!selection.IsEmpty())
	{
		Rect rc = ImageToClient(selection);
		Size ho = EditImg::sel_horz().GetSize(), ve = EditImg::sel_vert().GetSize();
		Rect clip = draw.GetClip();
		if(rc.left < clip.left)
			rc.left += (clip.left - rc.left) & -ho.cx;
		if(rc.right > clip.right)
			rc.right -= (rc.right - clip.right) & -ho.cx;
		if(rc.top < clip.top)
			rc.top += (clip.top - rc.top) & -ve.cy;
		if(rc.bottom > clip.bottom)
			rc.bottom -= (rc.bottom - clip.bottom) & -ve.cy;
		Rect ro = rc;
		ro.Inflate(ve.cx, ho.cy);
		DrawRect(draw, ro.left, ro.top, ro.Width(), ho.cy, EditImg::sel_horz(), true);
		DrawRect(draw, ro.left, rc.bottom, ro.Width(), ho.cy, EditImg::sel_horz(), true);
		DrawRect(draw, ro.left, rc.top, ve.cx, rc.Height(), EditImg::sel_vert(), true);
		DrawRect(draw, rc.right, rc.top, ve.cx, rc.Height(), EditImg::sel_vert(), true);

		ro = rc = ImageToClient(selection);
		ro.Inflate(BUTTONSIZE);
		Point cc((rc.left + rc.right - BUTTONSIZE) >> 1, (rc.top + rc.bottom - BUTTONSIZE) >> 1);
		DrawButton(draw, ro.left, ro.top);
		DrawButton(draw, cc.x, ro.top);
		DrawButton(draw, rc.right, ro.top);
		DrawButton(draw, ro.left, cc.y);
		DrawButton(draw, rc.right, cc.y);
		DrawButton(draw, ro.left, rc.bottom);
		DrawButton(draw, cc.x, rc.bottom);
		DrawButton(draw, rc.right, rc.bottom);
	}

	if(zoom >= MIN_GRID_ZOOM && grid != NONE) {
		int i;
		Size size = out_rc.Size() + 1;
		for(i = out_rc.left; i <= out_rc.right; i += zoom)
			draw.DrawRect(i, out_rc.top, 1, size.cy, grid_color);
		for(i = out_rc.top; i <= out_rc.bottom; i += zoom)
			draw.DrawRect(out_rc.left, i, size.cx, 1, grid_color);
	}

	if(left_type == &GetAdapterHotSpot)
	{
		Point pt = ImageToClient(hotspot) - ((EditImg::hotspot_image().GetSize() - zoom) >> 1);
		draw.DrawImage(pt.x, pt.y, EditImg::hotspot_image());
//		.Paint(draw, ImageToClient(hotspot)
//		- ((EditImg::hotspot_image().GetSize() - zoom) >> 1));
	}
}

Point ImageEditCtrl::ClientToImage(Point pt) const
{
	if(IsNull(pt))
		return pt;
	return idivfloor(pt + offset, zoom);
}

Rect ImageEditCtrl::ClientToImage(const Rect& rc) const
{
	return Rect(ClientToImage(rc.TopLeft()), ClientToImage(rc.BottomRight() - 1) + 1);
}

Point ImageEditCtrl::ImageToClient(Point pt) const
{
	if(IsNull(pt))
		return pt;
	return pt * zoom - offset;
}

Rect ImageEditCtrl::ImageToClient(const Rect& rc) const
{
	return Rect(ImageToClient(rc.TopLeft()), ImageToClient(rc.BottomRight()));
}

void ImageEditCtrl::OnScroll()
{
	Point newpos = AdjustOffset(scroll);
	LOG("scroll newpos = " << newpos);
	if(!IsFullRefresh())
	{
//		Sync(); // not needed because of Fidler's bug
		Size delta = offset - newpos;
		offset = newpos; // beware of Fidler's sync bug
		ScrollView(delta);
	}
	else
		offset = newpos;
}

void ImageEditCtrl::RefreshImage(Point pt)
{
	RefreshImage(Rect(pt.x, pt.y, pt.x + 1, pt.y + 1));
}

void ImageEditCtrl::RefreshImage(const Rect& rc)
{
//	DUMP(rc);
	Rect r = rc & Rect(GetImageSize());
	if(!r.IsEmpty())
	{
		Refresh(ImageToClient(r));
		if(small_image)
			small_image->RefreshImage(r);
	}
	WhenSyncBar();
}

void ImageEditCtrl::RefreshSelectionRect(const Rect& rc)
{
	if(rc.IsEmpty())
		return;
	Rect client = ImageToClient(rc) & Rect(GetSize());
	Rect outer = client;
	outer.Inflate(BUTTONSIZE);
	Refresh(outer.left, outer.top, outer.Width(), BUTTONSIZE);
	Refresh(outer.left, client.bottom, outer.Width(), BUTTONSIZE);
	Refresh(outer.left, client.top, BUTTONSIZE, client.Height());
	Refresh(client.right, client.top, BUTTONSIZE, client.Height());
}

void ImageEditCtrl::LeftDown(Point pt, dword keyflags)
{
	SetFocus();
	if(keyflags & K_SHIFT)
	{ // flood fill
		Rect rc = Rect(GetImageSize());
		pt = ClientToImage(pt);
		if(rc.Contains(pt))
		{
			AddUndo(t_("fill"));
			FloodFill(image, GetColor(), pt, rc);
			RefreshImage(rc);
		}
		return;
	}
	if(left_button)
	{
		drag_state = MAYBE;
		start = last = pt;
		SetCapture();
		left_button->PushClient(pt, keyflags);
	}
}

void ImageEditCtrl::LeftUp(Point pt, dword keyflags)
{
	ReleaseCapture();
	if(drag_state == ON)
	{
		HideLeftButton();
		if(left_button)
			left_button->DropClient(start, pt, keyflags);
	}
	drag_state = OFF;
}

void ImageEditCtrl::RightDown(Point pt, dword keyflags)
{
	SetFocus();
	SetColor(GetImagePixel(image, ClientToImage(pt)));
}

void ImageEditCtrl::MouseMove(Point pt, dword keyflags)
{
	if(drag_state != ON)
		ShowCoords(ClientToImage(pt));
	switch(drag_state)
	{
	case ON:
		left_button->DragClient(start, last, pt, keyflags);
		last = pt;
		break;

	case MAYBE:
		if(tabs(pt.x - start.x) >= 5 || tabs(pt.y - start.y) >= 5)
		{
			drag_state = ON;
			left_button->DragClient(start, Null, pt, keyflags);
			last = pt;
		}
		break;

	case OFF:
		if(keyflags & K_MOUSERIGHT) {
			pt = ClientToImage(pt);
			SetColor(GetImagePixel(image, pt));
		}
		break;
	}
}

Image ImageEditCtrl::CursorImage(Point pt, dword keyflags)
{
	if(keyflags & K_SHIFT)
		return MiscImg::flood_fill_cursor();
	return (left_button ? left_button->CursorClient(pt, keyflags) : Image());
}

void ImageEditCtrl::OnZoomCombo()
{
	SetZoom(~zoom_combo);
}

void ImageEditCtrl::ToolView(Bar& bar)
{
	ToolViewZoomIn(bar);
	ToolViewZoomOut(bar);
	ToolViewZoomFull(bar);
	ToolModeZoomWindow(bar);
	bar.ToolSeparator();
	ToolViewZoomCombo(bar);
}

void ImageEditCtrl::ToolViewZoomIn(Bar& bar)
{
	bar.Add(t_("Zoom in"), MiscImg::zoom_in(), THISBACK(OnViewZoomIn))
		.Key(K_ADD)
		.Help(t_("Zoom in view"));
}

void ImageEditCtrl::ToolViewZoomOut(Bar& bar)
{
	bar.Add(t_("Zoom out"), MiscImg::zoom_out(), THISBACK(OnViewZoomOut))
		.Key(K_SUBTRACT)
		.Help(t_("Zoom out view"));
}

void ImageEditCtrl::ToolViewZoomFull(Bar& bar)
{
	bar.Add(t_("Zoom full"), MiscImg::zoom_full(), THISBACK(OnViewZoomFull))
		.Help(t_("Show image in actual size"));
}

void ImageEditCtrl::ToolViewZoomCombo(Bar& bar)
{
	zoom_label = t_("Zoom:");
	bar.ToolBreak();
	bar.AddTool(zoom_label, 60);
	bar.AddTool(zoom_combo, 50);
}

void ImageEditCtrl::OnViewZoomIn()
{
	int x = ::Find(zoom_list, zoom_list + __countof(zoom_list), zoom, StdEqual<int>()) - zoom_list;
	if(x >= 0 && x < __countof(zoom_list) - 1)
		SetZoom(zoom_list[x + 1]);
	SetFocus();
}

void ImageEditCtrl::OnViewZoomOut()
{
	int x = ::Find(zoom_list, zoom_list + __countof(zoom_list), zoom, StdEqual<int>()) - zoom_list;
	if(x > 0)
		SetZoom(zoom_list[x - 1]);
	SetFocus();
}

void ImageEditCtrl::OnViewZoomFull()
{
	SetZoom(1);
	SetFocus();
}

void ImageEditCtrl::ToolMode(Bar& bar)
{
	ToolModeSelect(bar);
	bar.Separator();
	ToolModeFreehand(bar);
	ToolModeLine(bar);
	ToolModeRectangle(bar);
	ToolModeFilledRectangle(bar);
	ToolModeFloodFill(bar);
	bar.Separator();
	ToolModeHotSpot(bar);
}

void ImageEditCtrl::ToolModeZoomWindow(Bar& bar)
{
	bar.Add(t_("Zoom area..."), MiscImg::zoom_window(), THISBACK(OnModeZoomWindow))
		.Check(left_type == &GetAdapterZoomWindow)
		.Key(K_CTRL_W)
		.Help(t_("Zoom in selected area"));
}

void ImageEditCtrl::OnModeZoomWindow()
{
	SetLeftButton(GetAdapterZoomWindow);
}

void ImageEditCtrl::ToolModeSelect(Bar& bar)
{
	bar.Add(t_("Select area"), MiscImg::select(), THISBACK(OnModeSelect))
		.Check(left_type == &GetAdapterSelect)
		.Key(K_CTRL_D)
		.Help(t_("Select part of image for further operations"));
}

void ImageEditCtrl::OnModeSelect()
{
	SetLeftButton(GetAdapterSelect);
}

void ImageEditCtrl::ToolModeFreehand(Bar& bar)
{
	bar.Add(t_("Freehand"), MiscImg::freehand(), THISBACK(OnModeFreehand))
		.Check(left_type == &GetAdapterFreehand)
		.Key(K_CTRL_P)
		.Help(t_("Draw dots using mouse cursor"));
}

void ImageEditCtrl::OnModeFreehand()
{
	SetLeftButton(GetAdapterFreehand);
}

void ImageEditCtrl::ToolModeLine(Bar& bar)
{
	bar.Add(t_("Lines"), MiscImg::line(), THISBACK(OnModeLine))
		.Check(left_type == &GetAdapterLine)
		.Key(K_CTRL_L)
		.Help(t_("Draw lines"));
}

void ImageEditCtrl::OnModeLine()
{
	SetLeftButton(&GetAdapterLine);
}

void ImageEditCtrl::ToolModeRectangle(Bar& bar)
{
	bar.Add(t_("Rectangles"), MiscImg::rectangle(), THISBACK(OnModeRectangle))
		.Check(left_type == &GetAdapterRectangle)
		.Key(K_CTRL_R)
		.Help(t_("Draw rectangular frames"));
}

void ImageEditCtrl::OnModeRectangle()
{
	SetLeftButton(&GetAdapterRectangle);
}

void ImageEditCtrl::ToolModeFilledRectangle(Bar& bar)
{
	bar.Add(t_("Filled rectangles"), MiscImg::filled_rectangle(), THISBACK(OnModeFilledRectangle))
		.Check(left_type == &GetAdapterFilledRectangle)
		.Key(K_CTRL_R)
		.Help(t_("Paint filled rectangles with active color"));
}

void ImageEditCtrl::OnModeFilledRectangle()
{
	SetLeftButton(&GetAdapterFilledRectangle);
}

void ImageEditCtrl::ToolModeFloodFill(Bar& bar)
{
	bar.Add(t_("Fill"), MiscImg::flood_fill(), THISBACK(OnModeFloodFill))
		.Check(left_type == &GetAdapterFloodFill)
		.Key(K_CTRL_F)
		.Help(t_("Flood fill closed areas ([Ctrl] - enable diagonal steps)"));
}

void ImageEditCtrl::OnModeFloodFill()
{
	SetLeftButton(&GetAdapterFloodFill);
}

void ImageEditCtrl::ToolModeHotSpot(Bar& bar)
{
	bar.Add(t_("Hotspot"), MiscImg::hotspot(), THISBACK(OnModeHotSpot))
		.Check(left_type == &GetAdapterHotSpot)
		.Key(K_CTRL_B)
		.Help(t_("Set up image hotspot"));
}

void ImageEditCtrl::OnModeHotSpot()
{
	SetLeftButton(&GetAdapterHotSpot);
}

void ImageEditCtrl::ToolSelect(Bar& bar)
{
	ToolSelectSelectAll(bar);
	ToolSelectClearSelection(bar);
	ToolSelectFillSelection(bar);
	ToolSelectTransparentSelection(bar);
	ToolSelectChangeColor(bar);
	ToolSelectGrayed(bar);
	ToolSelectEtched(bar);
	bar.Separator();
	ToolSelectMatrix(bar);
	bar.Separator();
	ToolSelectMirrorX(bar);
	ToolSelectMirrorY(bar);
}

void ImageEditCtrl::ToolSelectCut(Bar& bar)
{
	bar.Add(!IsEmpty(), t_("Cut"), CtrlImg::cut(), THISBACK(OnSelectCut))
		.Key(K_CTRL_X)
		.Help(t_("Cut image data and place it on the system clipboard"));
}

void ImageEditCtrl::ToolSelectCopy(Bar& bar)
{
	bar.Add(!IsEmpty(), t_("Copy"), CtrlImg::copy(), THISBACK(OnSelectCopy))
		.Key(K_CTRL_C)
		.Help(t_("Copy image data on the system clipboard"));
}

void ImageEditCtrl::ToolSelectSelectAll(Bar& bar)
{
	bar.Add(t_("Select all"), MiscImg::select_all(), THISBACK(OnSelectSelectAll))
		.Key(K_CTRL_A)
		.Help(t_("Select the whole image"));
}

void ImageEditCtrl::OnSelectSelectAll()
{
	PasteAndClearImage();
	SetLeftButton(&GetAdapterSelect);
	SetSelection(Rect(GetImageSize()));
}

void ImageEditCtrl::ToolSelectClearSelection(Bar& bar)
{
	bar.Add(IsSelectionAvailable(), t_("Hide selection"), MiscImg::clear_selection(), THISBACK(OnSelectClearSelection))
		.Help(t_("Forget selected area"));
}

void ImageEditCtrl::OnSelectClearSelection()
{
	ClearSelectionAndImage();
}

void ImageEditCtrl::ToolSelectFillSelection(Bar& bar)
{
		bar.Add(t_("Fill selection"), MiscImg::fill_selection(), THISBACK(OnSelectFillSelection))
		.Key(K_SPACE)
		.Help(t_("Fill selection area with current color"));
}

void ImageEditCtrl::OnSelectFillSelection()
{
	AddUndo(t_("fill selection"));
	Rect rc = GetSelectionOrImage();
	SetImageRect(image, rc, GetColor());
	RefreshImage(rc);
}

void ImageEditCtrl::ToolSelectTransparentSelection(Bar& bar)
{
	bar.Add(t_("Make transparent"), MiscImg::transparent_selection(), THISBACK(OnSelectTransparentSelection))
		.Key(K_DELETE)
		.Help(t_("Make selection transparent"));
}

void ImageEditCtrl::OnSelectTransparentSelection()
{
	AddUndo(t_("make transparent"));
	Rect rc = GetSelectionOrImage();

	if(!selection.IsEmpty() && !selection_image.IsEmpty()) // gray selection image
		SetImageRect(selection_image, selection_image.GetSize(), Null);
	else
		SetImageRect(image, rc, Null);

	RefreshImage(rc);
}

void ImageEditCtrl::ToolSelectChangeColor(Bar& bar)
{
	bar.Add(t_("Change color"), MiscImg::change_color(), THISBACK(OnSelectChangeColor))
		.Key(K_CTRL_H)
		.Help(t_("Replace active color with another selected color"));
}

void ImageEditCtrl::OnSelectChangeColor()
{
	Rect rc = GetSelectionOrImage() & Rect(GetImageSize());
	WithChangeColorLayout<TopWindow> dlg;
	dlg.preview.SetFrame(InsetFrame());
	PaletteCtrl pal;
	dlg.palette.NoWantFocus() << pal.SizePos();
	pal <<= GetColor();
	pal.SetTransparent(GetTransparentColor());
	CtrlLayoutOKCancel(dlg, t_("Replace color"));
	dlg.tolerance.MinMax(0, 255);
	dlg.tolerance <<= 0;
	dlg.tolerance <<= pal.WhenSetTransparent = pal.WhenSetColor = dlg.Breaker(IDRETRY);
	for(;;)
	{
		Image converted;
		bool cv_sel = (!selection.IsEmpty() && !selection_image.IsEmpty());
		if(cv_sel)
			converted = selection_image;
		else
			converted = Crop(image, rc);
		ChangeColor(converted, converted.GetSize(), GetColor(), pal.Get(), ~dlg.tolerance);
		dlg.preview.Set(converted, pal.GetTransparent());
		switch(dlg.Run())
		{
		case IDRETRY:
			break;

		case IDOK:
			if(cv_sel)
				selection_image = converted;
			else
			{
				AddUndo(t_("color change"));
				Copy(image, rc.TopLeft(), converted, converted.GetSize());
			}
			RefreshImage(rc);
			return;

		case IDCANCEL:
			return;
		}
	}
}

void ImageEditCtrl::ToolSelectGrayed(Bar& bar)
{
	bar.Add(t_("Grayscale"), MiscImg::grayed(), THISBACK(OnSelectGrayed))
		.Key(K_CTRL_I)
		.Help(t_("Discard color information for selected area and convert to grayscale"));
}

void ImageEditCtrl::OnSelectGrayed()
{
	Rect rc = GetSelectionOrImage();
	if(!selection.IsEmpty() && !selection_image.IsEmpty()) // gray selection image
		GrayImage(selection_image, selection_image.GetSize());
	else
	{
		AddUndo(t_("grayscale"));
		GrayImage(image, rc);
	}
	RefreshImage(rc);
}

void ImageEditCtrl::ToolSelectEtched(Bar& bar)
{
	bar.Add(t_("Interpolate"), MiscImg::etched(), THISBACK(OnSelectEtched))
		.Key(K_CTRL_E)
		.Help(t_("Perform color interpolation of opaque points"));
}

void ImageEditCtrl::OnSelectEtched()
{
	Rect rc = GetSelectionOrImage();
	if(!selection.IsEmpty() && !selection_image.IsEmpty()) // etch selection image
		InterpolateImage(selection_image, selection_image.GetSize());
	else
	{
		AddUndo(t_("etched"));
		InterpolateImage(image, rc);
	}
	RefreshImage(rc);
}

void ImageEditCtrl::ToolSelectMirrorX(Bar& bar)
{
	bar.Add(t_("Reverse sides"), MiscImg::mirror_x(), THISBACK(OnSelectMirrorX))
		.Key(K_SHIFT_CTRL_X)
		.Help(t_("Reverse left / right side (vertical mirror)"));
}

void ImageEditCtrl::OnSelectMirrorX()
{
	Rect rc = GetSelectionOrImage();
	if(!selection.IsEmpty() && !selection_image.IsEmpty())
	{ // mirror selection image
		SwapHorz(selection_image, selection_image.GetSize());
		crop.OffsetHorz(selection_image.GetSize().cx - crop.left - crop.right);
	}
	else
	{ // mirror image in place
		AddUndo(t_("reverse sides"));
		SwapHorz(image, rc);
	}
	RefreshImage(rc);
}

void ImageEditCtrl::ToolSelectMirrorY(Bar& bar)
{
	bar.Add(t_("Top to bottom"), MiscImg::mirror_y(), THISBACK(OnSelectMirrorY))
		.Key(K_SHIFT_CTRL_Y)
		.Help(t_("Exchange image top & bottom (horizontal mirror)"));
}

void ImageEditCtrl::OnSelectMirrorY()
{
	Rect rc = GetSelectionOrImage();
	if(!selection.IsEmpty() && !selection_image.IsEmpty())
	{ // mirror selection image
		SwapVert(selection_image, selection_image.GetSize());
		crop.OffsetHorz(selection_image.GetSize().cy - crop.top - crop.bottom);
	}
	else
	{ // mirror image in place
		AddUndo(t_("top to bottom"));
		SwapVert(image, rc);
	}
	RefreshImage(rc);
}

void ImageEditCtrl::ToolSelectMirrorXY(Bar& bar)
{
	bar.Add(t_("Rotate 180 deg"), MiscImg::mirror_xy(), THISBACK(OnSelectMirrorXY))
		.Key(K_CTRL_U)
		.Help(t_("Rotate 180 deg (sides & top / bottom)"));
}

void ImageEditCtrl::OnSelectMirrorXY()
{
	Rect rc = GetSelectionOrImage();
	if(!selection.IsEmpty() && !selection_image.IsEmpty())
	{ // mirror selection image
		SwapVert(selection_image, selection_image.GetSize());
		SwapHorz(selection_image, selection_image.GetSize());
		crop.Offset(selection_image.GetSize() - crop.TopLeft() - crop.BottomRight());
	}
	else {
		SwapVert(image, rc);
		SwapHorz(image, rc);
	}
	RefreshImage(rc);
}

void ImageEditCtrl::ToolSelectMatrix(Bar& bar)
{
	bar.Add(t_("Matrix"), MiscImg::matrix(), THISBACK(OnSelectMatrix))
		.Key(K_CTRL_M)
		.Help(t_("Block copy selected area"));
}

//static ConfigItem matrix_config("MatrixLayout", 1, 1, 1);

INITBLOCK
{
	RegisterGlobalConfig("MatrixLayout");
}

void ImageEditCtrl::OnSelectMatrix()
{
	if(selection.IsEmpty())
		return;
	WithImageMatrixLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, t_("Block copy"));
	Size size = image.GetSize();
	dlg.count_x.MinMax(1, size.cx).NotNull();
	dlg.count_x <<= 2;
	dlg.count_y.MinMax(1, size.cy).NotNull();
	dlg.count_y <<= 2;
	dlg.gap_x.MinMax(-size.cx, size.cx).NotNull();
	dlg.gap_x <<= 0;
	dlg.gap_y.MinMax(-size.cy, size.cy).NotNull();
	dlg.gap_y <<= 0;

	LoadFromGlobal(dlg, "MatrixLayout");
	if(dlg.Run() != IDOK)
		return;
	StoreToGlobal(dlg, "MatrixLayout");
	Size count(~dlg.count_x, ~dlg.count_y);
	Size gap(~dlg.gap_x, ~dlg.gap_y);
	gap += selection.Size();

	if(selection_image.IsEmpty()) // read selection image
		SetSelectionImage(Crop(image, selection));

	for(Size p(0, 0); p.cy < count.cy; p.cy++)
		for(p.cx = 0; p.cx < count.cx; p.cx++)
			if(paste_transparent)
				Over(image, selection.TopLeft() + p * gap, selection_image, crop);
			else
				Copy(image, selection.TopLeft() + p * gap, selection_image, crop);

	RefreshImage();
}

void ImageEditCtrl::ToolSetup(Bar& bar)
{
	ToolSetupPasteTransparent(bar);
	ToolSetupDrawGrid(bar);
}

void ImageEditCtrl::ToolSetupPasteTransparent(Bar& bar)
{
	bar.Add(t_("Paste transparent"), MiscImg::paste_transparent(), THISBACK(OnSetupPasteTransparent))
		.Check(paste_transparent)
		.Key(K_CTRL_T)
		.Help(t_("Insert only non-transparent selection parts."));
}

void ImageEditCtrl::OnSetupPasteTransparent()
{
	paste_transparent = !paste_transparent;
	if(!selection_image.IsEmpty())
		RefreshImage(selection);
	WhenSyncBar();
}

void ImageEditCtrl::ToolSetupDrawGrid(Bar& bar)
{
	bar.Add(t_("Grid"),
		(grid == NONE ? MiscImg::grid_none()
		: grid == DITHERED ? MiscImg::grid_dithered() : MiscImg::grid_color()),
		THISBACK(OnSetupDrawGrid))
		.Key(K_CTRL_G)
		.Help(t_("Display grid dividing individual image pixels"));
}

void ImageEditCtrl::OnSetupDrawGrid()
{
	grid = (GRID)!grid;
	RefreshImage();
}

void ImageEditCtrl::ShowCoords(Point ip)
{
	if(!IsNull(ip))
		WhenCoords(NFormat("x = %d, y = %d", ip.x, ip.y));
}

void ImageEditCtrl::ShowCoords(const Rect& rc)
{
	if(!rc.IsEmpty())
		WhenCoords(::AsString(rc));
}

void ImageEditCtrl::ShowCoords(Point a, Point b)
{
	if(IsNull(a))
		ShowCoords(b);
	else if(IsNull(b))
		ShowCoords(a);
	else
		WhenCoords(NFormat("[%d, %d]->[%d, %d] ... (%d, %d]",
			a.x, a.y, b.x, b.y, tabs(b.x - a.x) + 1, tabs(b.y - a.y) + 1));
}

bool ImageEditCtrl::Key(dword key, int count)
{
	if(WhenKey(key))
		return true;
	if(left_button && left_button->Key(key))
		return true;
	if(scroll.Key(key))
		return true;

	Size shift(0, 0);
	switch(key & ~K_SHIFT)
	{
	case K_ESCAPE:
		HideLeftButton();
		return true;

	case K_LEFT:  shift.cx = -1; break;
	case K_UP:    shift.cy = -1; break;
	case K_RIGHT: shift.cx = +1; break;
	case K_DOWN:  shift.cy = +1; break;
	default:
		return false;
	}

	if(shift.cx | shift.cy)
	{
		AddUndo(t_("move"));
		Rect rc = GetSelectionOrImage();
		if(IsSelectionAvailable())
			ForceSelectionImage();
		if(!GetSelectionImage().IsEmpty())
			SetSelection(rc + shift);
		else
		{ // scroll whole image
			Image new_image = CreateImage(GetImageSize(), Color(Null));
			Size sz = GetImageSize();
			Copy(new_image, Point(shift), image, sz);
			image = new_image;
			RefreshImage();
		}
		if(rc.Contains(hotspot))
			SetHotSpot(hotspot + shift);
		return true;
	}

	return false;
}

void ImageEditCtrl::SetPixel(int x, int y, Color color)
{
	SetImagePixel(image, x, y, color);
	RefreshImage(Point(x, y));
}

void ImageEditCtrl::PaintFrame(Draw& draw, const Rect& rc) const
{
	if(!rc.IsEmpty())
	{
		Rect out = ImageToClient(rc);
		out.right++;
		out.bottom++;
		static word pattern[] = { 0xF0F0, 0x7878, 0x3C3C, 0x1E1E, 0x0F0F, 0x8787, 0xC3C3, 0xE1E1 };
		DrawDragRect(draw, out, Null, out, 1, SColorPaper, pattern);
	}
}

void ImageEditCtrl::SetSelection(const Rect& rc)
{
	if(rc != selection && !(rc.IsEmpty() && selection.IsEmpty()))
	{
		RefreshSelectionRect(selection);
		if(!selection_image.IsEmpty())
			RefreshImage(selection);
		RefreshSelectionRect(selection = rc);
		if(!selection_image.IsEmpty())
			RefreshImage(selection);
	}
}

Rect ImageEditCtrl::GetSelectionOrImage() const
{
	if(!selection.IsEmpty())
		return selection;
	return Rect(GetImageSize());
}

void ImageEditCtrl::SetSelectionImage(const Image& _sel_image)
{
	selection_image = _sel_image;
	crop = Rect(selection_image.GetSize());
	RefreshImage(selection);
	restore_on_cancel = Null;
}

void ImageEditCtrl::ForceSelectionImage()
{
	if(selection.IsEmpty() || selection_image.IsEmpty())
	{
		AddUndo(t_("selection"));
		Rect rc = GetSelectionOrImage();
		SetSelectionImage(Crop(image, rc));
		SetImageRect(image, rc, Color(Null));
		restore_on_cancel = rc;
	}
}

void ImageEditCtrl::SetSelectionCrop(const Rect& rc)
{
	crop = rc;
	RefreshImage(selection);
}

void ImageEditCtrl::OnSelectCopy()
{
	Rect imr = Rect(GetImageSize());
	Rect rc = GetSelectionOrImage() & imr;
	Image part = Crop(image, rc);
	WriteClipboardFormat(part);
	AppendClipboardImage(part);
}

void ImageEditCtrl::OnSelectCut()
{
	AddUndo(t_("cut"));
	OnSelectCopy();
	OnSelectTransparentSelection();
	ClearSelection();
}

int ImageEditCtrl::GetHitTest(Point pt) const
{
	if(selection.IsEmpty())
		return -1;
	Rect cl = ImageToClient(selection);
	Rect lg = cl;
	lg.Inflate(BUTTONSIZE);
	Point cc((cl.left + cl.right - BUTTONSIZE) >> 1, (cl.top + cl.bottom - BUTTONSIZE) >> 1);
	if(!lg.Contains(pt))
		return -1;
	if(pt.y >= lg.top && pt.y < cl.top)
	{
		if(pt.x < cl.left)
			return TOP_HIT | LEFT_HIT;
		if(pt.x >= cc.x && pt.x < cc.x + BUTTONSIZE)
			return TOP_HIT;
		if(pt.x >= cl.right)
			return TOP_HIT | RIGHT_HIT;
	}
	else if(pt.y >= cc.y && pt.y < cc.y + BUTTONSIZE)
	{
		if(pt.x < cl.left)
			return LEFT_HIT;
		if(pt.x >= cl.right)
			return RIGHT_HIT;
	}
	else if(pt.y >= cl.bottom && pt.y < lg.bottom)
	{
		if(pt.x < cl.left)
			return BOTTOM_HIT | LEFT_HIT;
		if(pt.x >= cc.x && pt.x < cc.x + BUTTONSIZE)
			return BOTTOM_HIT;
		if(pt.x >= cl.right)
			return BOTTOM_HIT | RIGHT_HIT;
	}
	lg = cl;
	lg.Inflate(EditImg::sel_vert().GetSize());
	if(lg.Contains(pt))
		return 0;
	return -1;
}

void ImageEditCtrl::PreDestroy()
{
	left_button.Clear();
}

void ImageEditCtrl::PasteSelection(bool add_undo)
{
	if(!selection.IsEmpty() && !selection_image.IsEmpty())
	{
		if(add_undo)
			AddUndo(t_("insert"));
		if(paste_transparent)
			Over(image, selection.TopLeft(), selection_image, crop);
		else
			Copy(image, selection.TopLeft(), selection_image, crop);
		RefreshImage(selection);
		restore_on_cancel = Null;
	}
}

void ImageEditCtrl::PasteAndClearImage(bool add_undo)
{
	PasteSelection(add_undo);
	ClearSelectionImage();
}

void ImageEditCtrl::DrawCurve(Draw& draw, const Vector<Point>& curve, bool undraw)
{
	Color color = GetColor();
	Color trans = palette->GetTransparent();
	Rect rc = Rect(GetImageSize()) & ClientToImage(draw.GetClip());
	int delta = (zoom >= MIN_GRID_ZOOM && grid != NONE);
	int size = zoom - delta;
	Vector<Color> img_color;
	if(undraw)
		img_color = GetImagePixel(image, curve);

	for(int i = 0; i < curve.GetCount(); i++)
		if(rc.Contains(curve[i]))
		{
			Color c = Nvl(undraw ? GetImagePixel(image, curve[i]) : color, trans);
			Point pt = ImageToClient(curve[i]);
			if(size <= 1)
				draw.DrawRect(pt.x, pt.y, 1, 1, c);
			else
				draw.DrawRect(pt.x + delta, pt.y + delta, size, size, c);
		}
}

void ImageEditCtrl::RotateSelectionAnticlockwise()
{
	Rect rc = GetSelectionOrImage();
	if(!IsSelectionAvailable())
		SetSelection(rc);
	if(GetSelectionImage().IsEmpty())
	{ // cut image
		SetSelectionImage(image);
	}
	Rect new_sel(rc.left, rc.top, rc.left + rc.Height(), rc.top + rc.Width());
	int ht = selection_image.GetSize().cx;
	Rect new_crop(crop.top, ht - crop.right, crop.bottom, ht - crop.left);
	SetSelection(new_sel);
	SetSelectionImage(RotateAntiClockwise(selection_image));
	SetSelectionCrop(new_crop);
}

void ImageEditCtrl::RotateSelectionClockwise()
{
	RotateSelectionAnticlockwise();
	OnSelectMirrorXY();
}

void ImageEditCtrl::SetViewCentre(Point pt)
{
	SetScroll(ImageToClient(pt) - ((GetSize() - zoom) >> 1));
}

void ImageEditCtrl::CropSelection()
{
	if(!selection.IsEmpty() && !selection_image.IsEmpty())
	{ // crop image according to its own crop rectangle
		SetSelectionImage(Crop(selection_image, crop));
		RefreshImage(selection);
	}
	else
	{
		AddUndo(t_("crop"));
		Size size = GetImageSize();
		Rect rc = GetSelectionOrImage() & Rect(size);
		if(rc.IsEmpty())
			SetImageRect(image, image.GetSize(), Color(Null));
		else
		{
			if(rc.left > 0)
				SetImageRect(image, Rect(0, 0, rc.left, size.cy), Color(Null));
			if(rc.right < size.cx)
				SetImageRect(image, Rect(rc.right, 0, size.cx, size.cy), Color(Null));
			if(rc.top > 0)
				SetImageRect(image, Rect(rc.left, 0, rc.right, rc.top), Color(Null));
			if(rc.bottom < size.cy)
				SetImageRect(image, Rect(rc.left, rc.bottom, rc.right, size.cy), Color(Null));
		}
		RefreshImage();
	}
}

void ImageEditCtrl::ClearSelectionAndImage()
{
	if(!restore_on_cancel.IsEmpty() && !selection_image.IsEmpty())
	{
		Copy(image, restore_on_cancel.TopLeft(), selection_image, restore_on_cancel.GetSize());
		RefreshImage(restore_on_cancel);
	}
	restore_on_cancel = Null;
	ClearSelection();
	ClearSelectionImage();
}

void ImageEditCtrl::AddUndo(const char *cmd)
{
	WhenAddUndo(new UndoEntryPaint(*this, cmd));
}
