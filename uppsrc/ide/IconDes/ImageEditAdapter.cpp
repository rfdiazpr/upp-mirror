#include "IconDes.h"
#pragma hdrstop
#include "ImageEditCtrl.h"
#include <Draw/PixelUtil.h>

static Rect RectSortImage(Point A, Point B)
{
	return Rect(min(A.x, B.x), min(A.y, B.y), max(A.x, B.x) + 1, max(A.y, B.y) + 1);
}

Vector<Point> GetLine(Point A, Point B, const Rect& clip)
{
	Vector<Point> list;
	if(clip.Contains(A))
		list.Add(A);
	if(A == B)
		return list;
	Size dist = B - A;
	Size adist(abs(dist.cx), abs(dist.cy));
	Size sgns(sgn(dist.cx), sgn(dist.cy));
	if(adist.cx >= adist.cy)
	{ // horizontal run
		for(int count = adist.cx, step = adist.cx >> 1; --count >= 0;)
		{
			A.x += sgns.cx;
			if((step += adist.cy) >= adist.cx)
			{
				step -= adist.cx;
				A.y += sgns.cy;
			}
			if(clip.Contains(A))
				list.Add(A);
		}
	}
	else
	{
		for(int count = adist.cy, step = adist.cy >> 1; --count >= 0;)
		{
			A.y += sgns.cy;
			if((step += adist.cx) >= adist.cy)
			{
				step -= adist.cy;
				A.x += sgns.cx;
			}
			if(clip.Contains(A))
				list.Add(A);
		}
	}
	return list;
}

Vector<Point> GetRect(const Rect& rc, const Rect& clip)
{
	Vector<Point> list;
	Rect clipped = rc & clip;
	if(clipped.IsEmpty())
		return list;
	int h1 = rc.Height() - 1;
	if(rc.top == clipped.top) // draw top line
		for(int i = clipped.left; i < clipped.right; i++)
			list.Add(Point(i, rc.top));
	if(rc.bottom == clipped.bottom && h1 > 0)
		for(int i = clipped.left; i < clipped.right; i++)
			list.Add(Point(i, rc.bottom - 1));
	if(rc.left == clipped.left && h1 > 0)
		for(int i = clipped.top; i < clipped.bottom; i++)
			list.Add(Point(rc.left, i));
	if(rc.right == clipped.right && rc.Width() > 1)
		for(int i = clipped.top; i < clipped.bottom; i++)
			list.Add(Point(rc.right - 1, i));
	return list;
}

Vector<Point> GetLine(Point A, Point B)
{
	return GetLine(A, B, RectSortImage(A, B));
}

//////////////////////////////////////////////////////////////////////
// ImageEditAdapter::

void ImageEditAdapter::PushClient(Point pt, dword keyflags)
{
	Push(image -> ClientToImage(pt), keyflags);
}

void ImageEditAdapter::DragClient(Point start, Point last, Point next, dword keyflags)
{
	Drag(image -> ClientToImage(start),
		image -> ClientToImage(last), image -> ClientToImage(next), keyflags);
}

void ImageEditAdapter::DropClient(Point start, Point end, dword keyflags)
{
	Drop(image -> ClientToImage(start),
		image -> ClientToImage(end), keyflags);
}

Image ImageEditAdapter::CursorClient(Point pt, dword keyflags)
{
	return Cursor(image -> ClientToImage(pt), keyflags);
}

bool ImageEditAdapter::Key(dword key)
{
	return false;
}

void ImageEditAdapter::Push(Point pt, dword keyflags)
{
}

void ImageEditAdapter::Drag(Point start, Point last, Point next, dword keyflags)
{
	Rect rl = Null, rn = Null;
	if(!IsNull(last))
		rl = RectSortImage(start, last);
	if(!IsNull(next))
		rn = RectSortImage(start, next);
	if(rl != rn)
		DragRect(rl, rn, keyflags);
}

void ImageEditAdapter::Drop(Point start, Point end, dword keyflags)
{
	Rect rc = Null;
	if(!IsNull(start) && !IsNull(end))
		rc = RectSortImage(start, end);
	DropRect(rc, keyflags);
}

Image ImageEditAdapter::Cursor(Point pt, dword keyflags)
{
	return Image::Arrow();
}

void ImageEditAdapter::DragRect(const Rect& last, const Rect& next, dword keyflags)
{
	ViewDraw draw(image);
	image -> PaintFrame(draw, last);
	image -> PaintFrame(draw, next);
	if(!next.IsEmpty())
		image -> ShowCoords(next);
}

void ImageEditAdapter::DropRect(const Rect& rc, dword keyflags)
{
}

#define ADAPTER(name) \
One<ImageEditAdapter> GetAdapter##name() { return new ImageEditAdapter##name; }

class ImageEditAdapterFreehand : public ImageEditAdapter
{
public:
	virtual void     Push(Point pt, dword keyflags);
	virtual void     Drag(Point start, Point last, Point next, dword keyflags);
	virtual Image    CursorClient(Point pt, dword keyflags) { return MiscImg::freehand_cursor(); }
};

ADAPTER(Freehand)

void ImageEditAdapterFreehand::Push(Point pt, dword keyflags)
{
	image -> AddUndo("od ruky");
	image -> ShowCoords(pt);
	image -> SetPixel(pt, image -> GetColor());
}

void ImageEditAdapterFreehand::Drag(Point start, Point last, Point next, dword keyflags)
{
	if(!IsNull(last) && !IsNull(next))
	{
		Vector<Point> line = GetLine(last, next, Rect(image -> GetImageSize()));
		Color co = image -> GetColor();
		for(int i = 0; i < line.GetCount(); i++)
			image -> SetPixel(line[i], co);
		image -> ShowCoords(next);
	}
}

//////////////////////////////////////////////////////////////////////
// ImageEditAdapterZoomWindow

class ImageEditAdapterZoomWindow : public ImageEditAdapter
{
public:
	virtual void DropRect(const Rect& rc, dword keyflags);

private:
};

ADAPTER(ZoomWindow)

void ImageEditAdapterZoomWindow::DropRect(const Rect& rc, dword keyflags)
{
	Size zooms = image -> GetSize() / rc.Size();
	int lzoom = min(zooms.cx, zooms.cy);
	int i = BinFindIndex<const int *>(image -> zoom_list, image -> zoom_list + image -> zoom_count,
		min(zooms.cx, zooms.cy), StdLess<int>());
	if(i >= image -> zoom_count)
		i--;
	image -> SetZoom(image -> zoom_list[i]);
	image -> SetScroll((image -> GetSize() >> 1) - image -> ImageToClient(rc.CenterPoint()));
}

//////////////////////////////////////////////////////////////////////
// ImageEditAdapterSelect::

class ImageEditAdapterSelect : public ImageEditAdapter
{
public:
	ImageEditAdapterSelect() : hit_test(-1) {}

	virtual void     PushClient(Point pt, dword keyflags);
	virtual void     Drag(Point pt, Point last, Point next, dword keyflags);
	virtual void     Drop(Point pt, Point end, dword keyflags);
	virtual Image    CursorClient(Point pt, dword keyflags);
	virtual bool     Key(dword key);

private:
	int hit_test;
	bool scale;
	Rect start_selection;
	Rect start_crop;
};

ADAPTER(Select)

Image ImageEditAdapterSelect::CursorClient(Point pt, dword keyflags)
{
	int hit = (keyflags & K_MOUSELEFT ? hit_test : image -> GetHitTest(pt));
	if(hit < 0)
		return MiscImg::select_cursor();
	switch(hit)
	{
	case 0: return Image::SizeAll();
	case ImageEditCtrl::LEFT_HIT:
	case ImageEditCtrl::RIGHT_HIT: return Image::SizeHorz();
	case ImageEditCtrl::TOP_HIT:
	case ImageEditCtrl::BOTTOM_HIT: return Image::SizeVert();
	case ImageEditCtrl::LEFT_HIT | ImageEditCtrl::TOP_HIT:
	case ImageEditCtrl::RIGHT_HIT | ImageEditCtrl::BOTTOM_HIT: return Image::SizeBackSlash();
	case ImageEditCtrl::RIGHT_HIT | ImageEditCtrl::TOP_HIT:
	case ImageEditCtrl::LEFT_HIT | ImageEditCtrl::BOTTOM_HIT: return Image::SizeSlash();
	}
	return Image::Arrow();
}

bool ImageEditAdapterSelect::Key(dword key)
{
	if(key == K_ESCAPE)
	{
		image -> ClearSelectionAndImage();
		return true;
	}
	return false;
}

void ImageEditAdapterSelect::PushClient(Point pt, dword keyflags)
{
	hit_test = image -> GetHitTest(pt);
	if(hit_test < 0)
	{
		image -> PasteSelection(false);
		image -> ClearSelectionAndImage();
	}
	else if(hit_test == 0 || keyflags & K_CTRL)
	{ // move selection
		Rect sel = image -> GetSelection();
		if(image -> GetSelectionImage().IsEmpty())
		{ // get selection from image
			if(hit_test || !(keyflags & K_CTRL))
				image -> ForceSelectionImage();
			else
				image -> SetSelectionImage(AlphaCrop(image -> image, sel));
		}
	}
	scale = (hit_test > 0 && keyflags & K_CTRL);
	if(!scale && (keyflags & K_CTRL)) // paste selection to current position in copy mode
		image -> PasteSelection();
	start_selection = image -> GetSelection();
	start_crop = image -> GetSelectionCrop();
}

void ImageEditAdapterSelect::Drop(Point start, Point end, dword keyflags)
{
	if(hit_test < 0)
		image -> SetSelection(RectSortImage(start, end) & Rect(image -> GetImageSize()));
	else
		Drag(start, Null, end, keyflags);
}

void ImageEditAdapterSelect::Drag(Point pt, Point last, Point next, dword keyflags)
{
	if(hit_test < 0)
	{
		ImageEditAdapter::Drag(pt, last, next, keyflags);
		return;
	}

	Rect sel = start_selection, crop = start_crop;
	if(!IsNull(next))
	{
		Size delta = next - pt;
		if(hit_test == 0)
			sel += delta;
		else
		{
			sel.right--; sel.bottom--; crop.right--; crop.bottom--;

			if(hit_test & image -> LEFT_HIT)
			{ sel.left   += delta.cx; if(!scale) crop.left   += delta.cx; }
			if(hit_test & image -> TOP_HIT)
			{ sel.top    += delta.cy; if(!scale) crop.top    += delta.cy; }
			if(hit_test & image -> RIGHT_HIT)
			{ sel.right  += delta.cx; if(!scale) crop.right  += delta.cx; }
			if(hit_test & image -> BOTTOM_HIT)
			{ sel.bottom += delta.cy; if(!scale) crop.bottom += delta.cy; }

			if(sel.right < sel.left) Swap(sel.left, sel.right);
			if(sel.bottom < sel.top) Swap(sel.top, sel.bottom);
			if(crop.right < crop.left) Swap(crop.left, crop.right);
			if(crop.bottom < crop.top) Swap(crop.top, crop.bottom);
			sel.right++; sel.bottom++; crop.right++; crop.bottom++;
		}
		image -> ShowCoords(sel);
	}
	image -> SetSelection(sel);
	image -> SetSelectionCrop(crop);
}

//////////////////////////////////////////////////////////////////////
// ImageEditAdapterCurve

class ImageEditAdapterCurve : public ImageEditAdapter
{
public:
	ImageEditAdapterCurve(const Image& cursor, const char *command) : cursor(cursor), command(command) {}

	virtual void          Drag(Point start, Point last, Point next, dword keyflags);
	virtual void          Drop(Point start, Point end, dword keyflags);

	virtual Vector<Point> GetCurve(Point a, Point b, const Rect& clip) const = 0;

	virtual Image         CursorClient(Point pt, dword keyflags) { return cursor; }

private:
	Image         cursor;
	const char   *command;
};

void ImageEditAdapterCurve::Drag(Point start, Point last, Point next, dword keyflags)
{
	Rect rc(image -> GetImageSize());
	ViewDraw draw(image);
	Vector<Point> ll, nl;
	if(!IsNull(last))
		ll = GetCurve(start, last, rc);
	if(!IsNull(next))
		nl = GetCurve(start, next, rc);
	image -> DrawCurve(draw, ll, true);
	image -> DrawCurve(draw, nl, false);
	if(!IsNull(next))
		image -> ShowCoords(start, next);
}

void ImageEditAdapterCurve::Drop(Point start, Point end, dword keyflags)
{
	image -> AddUndo(command);
	Vector<Point> curve = GetCurve(start, end, Rect(image -> GetImageSize()));
	AlphaSetPixel(image -> image, curve, image -> GetColor());
	{
		ViewDraw vdraw(image);
		image -> DrawCurve(vdraw, curve, true);
	}
	if(image -> small_image)
		image -> small_image -> Refresh();
}

//////////////////////////////////////////////////////////////////////
// ImageEditAdapterLine::

class ImageEditAdapterLine : public ImageEditAdapterCurve
{
public:
	ImageEditAdapterLine() : ImageEditAdapterCurve(MiscImg::line_cursor(), "úsečka") {}

	virtual Vector<Point> GetCurve(Point a, Point b, const Rect& clip) const;
};

ADAPTER(Line)

Vector<Point> ImageEditAdapterLine::GetCurve(Point a, Point b, const Rect& clip) const
{
	return GetLine(a, b, clip);
}

///////////////////////////////////////////////////////////////////////
// ImageEditAdapterRectangle::

class ImageEditAdapterRectangle : public ImageEditAdapterCurve
{
public:
	ImageEditAdapterRectangle() : ImageEditAdapterCurve(MiscImg::rectangle_cursor(), "rámeček") {}

	virtual Vector<Point> GetCurve(Point a, Point b, const Rect& clip) const;
};

ADAPTER(Rectangle)

Vector<Point> ImageEditAdapterRectangle::GetCurve(Point a, Point b, const Rect& clip) const
{
	return GetRect(RectSortImage(a, b), clip);
}

//////////////////////////////////////////////////////////////////////
// ImageEditAdapterFloodFill::

class ImageEditAdapterFloodFill : public ImageEditAdapter
{
public:
	virtual Image CursorClient(Point pt, dword keyflags) { return MiscImg::flood_fill_cursor(); }
	virtual void  Push(Point pt, dword keyflags);
};

ADAPTER(FloodFill)

void ImageEditAdapterFloodFill::Push(Point pt, dword keyflags)
{
	if(!image -> image.GetRect().Contains(pt))
		return;
	image -> AddUndo("výplň");
	AlphaFloodFill(image -> image, image -> GetColor(), pt, image -> GetRect(), keyflags & K_CTRL);
	image -> RefreshImage();
}

//////////////////////////////////////////////////////////////////////
// ImageEditAdapterHotSpot::

class ImageEditAdapterHotSpot : public ImageEditAdapter
{
public:
	virtual void  Push(Point pt, dword keyflags);
	virtual void  DragClient(Point pt, Point last, Point next, dword keyflags) {}
	virtual Image CursorClient(Point pt, dword keyflags) { return MiscImg::hotspot_cursor(); }
};

ADAPTER(HotSpot)

void ImageEditAdapterHotSpot::Push(Point pt, dword keyflags)
{
	image -> AddUndo("vztažný bod");
	image -> SetHotSpot(pt);
}

//////////////////////////////////////////////////////////////////////
// ImageEditAdapterFilledRectangle::

class ImageEditAdapterFilledRectangle : public ImageEditAdapter
{
public:
	virtual Image CursorClient(Point pt, dword keyflags) { return MiscImg::filled_rectangle_cursor(); }
	virtual void  DropRect(const Rect& rc, dword keyflags);
};

ADAPTER(FilledRectangle)

void ImageEditAdapterFilledRectangle::DropRect(const Rect& rc, dword keyflags)
{
	image -> AddUndo("obdélník");
	AlphaSet(image -> image, rc, image -> GetColor());
	image -> RefreshImage(rc);
}
