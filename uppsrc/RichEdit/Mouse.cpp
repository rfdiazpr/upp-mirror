#include "RichEdit.h"

void RichEdit::CancelMode()
{
	tabmove.table = 0;
}

void RichEdit::MouseWheel(Point p, int zdelta, dword keyflags)
{
	sb.Wheel(zdelta);
}

RichHotPos RichEdit::GetHotPos(Point p)
{
	int x;
	PageY py;
	GetPagePoint(p, py, x);
	return text.GetHotPos(x, py, 4 / GetZoom(), pagesz);
}

void RichEdit::SetObjectPercent(int p)
{
	if(objectpos >= 0) {
		RichObject obj = GetObject();
		obj.SetSize(obj.GetPhysicalSize() * p / 100);
		obj.KeepRatio(true);
		ReplaceObject(obj);
	}
}

void RichEdit::SetObjectYDelta(int pt)
{
	if(objectpos >= 0) {
		RichObject obj = GetObject();
		obj.SetYDelta(pt * 25 / 3);
		ReplaceObject(obj);
	}
}

void RichEdit::LeftDown(Point p, dword flags)
{
	NextUndo();
	SetFocus();
	tabmove = GetHotPos(p);
	if(tabmove.table && tabmove.column >= -2) {
		SaveTableFormat(tabmove.table);
		SetCapture();
		Move(text.GetCellPos(tabmove.table, 0, max(tabmove.column, 0)).pos);
		return;
	}
	int c = GetHotSpot(p);
	if(c >= 0 && objectpos >= 0) {
		int pos = objectpos;
		RectTracker tracker(*this);
		RichObject obj = text.GetRichPos(pos).object;
		tracker.MinSize(Size(16, 16))
		       .MaxSize(GetZoom() * pagesz)
		       .Animation()
		       .Dashed()
		       .KeepRatio(obj.IsKeepRatio());
		int tx, ty;
		switch(c) {
		case 1:
			tracker.SetCursorImage(CtrlImg::SizeVert0());
			tx = ALIGN_CENTER; ty = ALIGN_BOTTOM;
			break;
		case 2:
			tracker.SetCursorImage(CtrlImg::SizeHorz0());
			tx = ALIGN_RIGHT; ty = ALIGN_CENTER;
			break;
		default:
			tracker.SetCursorImage(CtrlImg::SizeHoVe0());
			tx = ALIGN_RIGHT; ty = ALIGN_RIGHT;
			break;
		}
		double zoom = GetZoom().AsDouble();
		Size sz = obj.GetSize();
		sz.cx = int(zoom * sz.cx + 0.5);
		sz.cy = int(zoom * sz.cy + 0.5);
		sz = tracker.Track(Rect(objectrect.Offseted(16, -sb).TopLeft(), sz), tx, ty).Size();
		sz.cx = int(sz.cx / zoom + 0.5);
		sz.cy = int(sz.cy / zoom + 0.5);
		obj.SetSize(sz);
		ReplaceObject(obj);
	}
	else {
		c = GetMousePos(p);
		if(c >= 0) {
			Move(c, flags & K_SHIFT);
			mpos = c;
			SetCapture();
			if(cursorp.object) {
				Rect r = GetObjectRect(cursor);
				Rect rr = r.Offseted(16, -sb);
				if(rr.Contains(p)) {
					objectrect = r;
					objectpos = cursor;
					PageRect pr = text.GetCaret(cursor, pagesz);
					PlaceCaret();
					Refresh(rr);
					ReadFormat();
				}
			}
		}
	}
}

void RichEdit::MouseMove(Point p, dword flags)
{
	if(HasCapture() && (flags & K_MOUSELEFT)) {
		if(tabmove.table && tabmove.column >= 0) {
			RichTable::Format fmt = text.GetTableFormat(tabmove.table);
			if(tabmove.column >= fmt.column.GetCount() - 1)
				return;
			int sum = Sum0(fmt.column);
			int nl = 0;
			for(int i = 0; i < tabmove.column; i++)
				nl += fmt.column[i];
			int xn = fmt.column[tabmove.column] + fmt.column[tabmove.column + 1];
			int xl = tabmove.left + tabmove.cx * nl / sum + 120;
			int xh = tabmove.left + tabmove.cx * (nl + xn) / sum - 120;
			if(xl >= xh)
				return;
			int xx = minmax(GetSnapX(p.x) - tabmove.delta, xl, xh) - tabmove.left;
			fmt.column[tabmove.column] = xx * sum / tabmove.cx - nl;
			fmt.column[tabmove.column + 1] = xn - fmt.column[tabmove.column];
			text.SetTableFormat(tabmove.table, fmt);
			Finish();
		}
		else
		if(tabmove.table && tabmove.column == -1) {
			RichTable::Format fmt = text.GetTableFormat(tabmove.table);
			fmt.lm = minmax(GetSnapX(p.x) - tabmove.textleft, 0, max(tabmove.textcx - fmt.rm - 120, 0));
			text.SetTableFormat(tabmove.table, fmt);
			Finish();
		}
		else
		if(tabmove.table && tabmove.column == -2) {
			RichTable::Format fmt = text.GetTableFormat(tabmove.table);
			fmt.rm = minmax(tabmove.textcx - GetSnapX(p.x) + tabmove.textleft, 0, tabmove.textcx - fmt.lm - 120);
			text.SetTableFormat(tabmove.table, fmt);
			Finish();
		}
		else {
			PageY py = GetPageY(p.y + sb);
			int c;
			if(py > text.GetHeight(pagesz))
				c = GetLength() + 1;
			else
				c = GetNearestPos((p.x - 16) / GetZoom(), py);
			if(c != mpos) {
				mpos = -1;
				Move(c, true);
			}
		}
	}
}

void RichEdit::RightDown(Point p, dword flags)
{
	NextUndo();
	MenuBar menu;
	int l, h;
	Rect ocr = GetCaretRect();
	int c = GetMousePos(p);
	int fieldpos = -1;
	Id field;
	String fieldparam;
	String ofieldparam;
	RichObject object, o;
	if(GetSelection(l, h)) {
		if(c >= l && c < h) {
			CopyTool(menu);
			CutTool(menu);
		}
		PasteTool(menu);
	}
	else {
		LeftDown(p, flags);
		if(objectpos >= 0) {
			object = GetObject();
			if(!object) return;
			o = object;
			o.Menu(menu);
			const RichObjectType& t = object.GetType();
			if(!menu.IsEmpty())
				menu.Separator();
			menu.Add(t_("Object position.."), THISBACK(AdjustObjectSize));
			menu.Separator();
			menu.Add("20 %", THISBACK1(SetObjectPercent, 20));
			menu.Add("40 %", THISBACK1(SetObjectPercent, 40));
			menu.Add("60 %", THISBACK1(SetObjectPercent, 60));
			menu.Add("80 %", THISBACK1(SetObjectPercent, 80));
			menu.Add("90 %", THISBACK1(SetObjectPercent, 90));
			menu.Add("100 %", THISBACK1(SetObjectPercent, 100));
			menu.Break();
			menu.Add(t_("3 pt up"), THISBACK1(SetObjectYDelta, -3));
			menu.Add(t_("2 pt up"), THISBACK1(SetObjectYDelta, -2));
			menu.Add(t_("1 pt up"), THISBACK1(SetObjectYDelta, -1));
			menu.Add(t_("Baseline"), THISBACK1(SetObjectYDelta, 0));
			menu.Add(t_("1 pt down"), THISBACK1(SetObjectYDelta, 1));
			menu.Add(t_("2 pt down"), THISBACK1(SetObjectYDelta, 2));
			menu.Add(t_("3 pt down"), THISBACK1(SetObjectYDelta, 3));
			menu.Separator();
			CopyTool(menu);
			CutTool(menu);
		}
		else {
			RichPos p = cursorp;
			field = p.field;
			fieldparam = p.fieldparam;
			RichPara::FieldType *ft = RichPara::fieldtype().Get(field, NULL);
			if(ft) {
				ofieldparam = fieldparam;
				fieldpos = cursor;
				ft->Menu(menu, &fieldparam);
				if(!menu.IsEmpty())
					menu.Separator();
				CopyTool(menu);
				CutTool(menu);
			}
			else {
				int c = cursor;
				WString w = GetWordAtCursor();
				if(!w.IsEmpty() && !SpellWord(w, w.GetLength(), formatinfo.language)) {
					menu.Add(t_("Add to user dictionary"), THISBACK(AddUserDict));
					menu.Separator();
				}
				PasteTool(menu);
				ObjectTool(menu);
			}
		}
	}
	Rect r = GetCaretRect();
	Refresh(r);
	Refresh(ocr);
	paintcarect = true;
	menu.Execute();
	paintcarect = false;
	Refresh(r);
	if(object && o && o.GetSerialId() != object.GetSerialId())
		ReplaceObject(o);
	if(fieldpos >= 0 && fieldparam != ofieldparam) {
		RichText::FormatInfo f = text.GetFormatInfo(fieldpos, 1);
		Remove(fieldpos, 1);
		RichPara p;
		p.Cat(field, fieldparam, f);
		RichText clip;
		clip.Cat(p);
		Insert(fieldpos, clip, false);
		Finish();
	}
}

void RichEdit::LeftDouble(Point p, dword flags)
{
	NextUndo();
	int c = GetMousePos(p);
	if(c >= 0) {
		if(objectpos == c) {
			RichObject object = GetObject();
			if(!object) return;
			RichObject o = object;
			o.DefaultAction();
			if(object.GetSerialId() != o.GetSerialId())
				ReplaceObject(o);
		}
		else {
			RichPos rp = cursorp;
			RichPara::FieldType *ft = RichPara::fieldtype().Get(rp.field, NULL);
			if(ft) {
				int fieldpos = cursor;
				ft->DefaultAction(&rp.fieldparam);
				RichText::FormatInfo f = text.GetFormatInfo(fieldpos, 1);
				Remove(fieldpos, 1);
				RichPara p;
				p.Cat(rp.field, rp.fieldparam, f);
				RichText clip;
				clip.Cat(p);
				Insert(fieldpos, clip, false);
				Finish();
			}
			else {
				Move(c, false);
				MoveWordLeft(false);
				MoveWordRight(true);
			}
		}
	}
}

Image RichEdit::CursorImage(Point p, dword flags)
{
	static Image (*anihorz[])() = {
		CtrlImg::SizeHorz0, CtrlImg::SizeHorz1, CtrlImg::SizeHorz0, CtrlImg::SizeHorz2
	};
	static Image (*anivert[])() = {
		CtrlImg::SizeVert0, CtrlImg::SizeVert1, CtrlImg::SizeVert0, CtrlImg::SizeVert2
	};
	static Image (*anihove[])() = {
		CtrlImg::SizeHoVe0, CtrlImg::SizeHoVe1, CtrlImg::SizeHoVe0, CtrlImg::SizeHoVe2
	};
	static Image (*anipos[])() = {
		CtrlImg::horzpos1, CtrlImg::horzpos2, CtrlImg::horzpos1, CtrlImg::horzpos3
	};

	if(tablesel)
		return Image::Arrow();

	Image (**ani)();
	switch(GetHotSpot(p)) {
	case 0:
		ani = anihove;
		break;
	case 1:
		ani = anivert;
		break;
	case 2:
		ani = anihorz;
		break;
	default:
		if(text.GetRichPos(GetMousePos(p)).object) {
			return Image::Arrow();
		}
		else
		if(HasCapture() && tabmove.table && tabmove.column >= -2)
			return CtrlImg::horzpos1();
		else {
			RichHotPos hp = GetHotPos(p);
			if(hp.table > 0)
				ani = anipos;
			else
				return CtrlImg::ibeam0;
		}
	}
	return (*ani[GetTimeClick() / 200 % 4])();
}

void RichEdit::LeftRepeat(Point p, dword flags)
{
	NextUndo();
	if(HasCapture() && (flags & K_MOUSELEFT)) {
		if(p.y < 0)
			MoveUpDown(-1, true);
		if(p.y > GetSize().cy)
			MoveUpDown(1, true);
	}
}
