#include "DropGrid.h"

NAMESPACE_UPP

class MultiDisplay : public GridDisplay
{
	public:
		void Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style, Color &fg, Color &bg, Font &fnt, bool found = false, int fs = 0, int fe = 0)
		{
			w.DrawRect(x, y, cx, cy, bg);

			int ty = (cy - Draw::GetStdFontCy()) / 2 + 1;
			int tx = 3;
			
			if(IsType< Vector<String> >(val))
			{
				const Vector<String> &v = ValueTo< Vector<String> >(val);
				const char * SPACE = " ";
				int cnt = v.GetCount();
				for(int i = 0; i < cnt; i++)
				{
					fnt.Bold((i + 1) & 1);
					w.DrawText(tx, ty, v[i], fnt, fg);
					tx += GetTextSize(v[i], fnt).cx;
					w.DrawText(tx, ty, SPACE, fnt, fg);
					tx += GetTextSize(SPACE, fnt).cx;
				}
			}
			else
				w.DrawText(tx, ty, (String) val, fnt, fg);
		}
};

DropGrid::PopUpGrid::PopUpGrid()
{
	LiveCursor();
	FullColResizing(false);
	HorzGrid(false);
	DrawLastVertLine(false);
	TabAddsRow(false);
	SearchMoveCursor(true);
	SearchHideRows(false);
	ResizePanel();
	WhenLeftClick = THISBACK(CloseData);
	WhenEnter = THISBACK(CloseData);
	WhenEscape = THISBACK(CloseNoData);
	GridCtrl::WhenClose = THISBACK(CloseNoData);	
}

void DropGrid::PopUpGrid::PopUp(Ctrl *owner, const Rect &r)
{
	Close();
	SetRect(r);
	Ctrl::PopUp(owner, true, true, GUI_DropShadows());
}

void DropGrid::PopUpGrid::CloseNoData()
{
	Deactivate();
	WhenCloseNoData();
}

void DropGrid::PopUpGrid::CloseData()
{
	Deactivate();
	WhenCloseData();
}

void DropGrid::PopUpGrid::Deactivate()
{
	if(IsOpen() && IsPopUp())
	{
		WhenPopDown();
		IgnoreMouseClick();
		Ctrl::Close();
		WhenClose();
	}
}

DropGrid::DropGrid()
{
	WhenPush = THISBACK(Drop);
	list.WhenCloseData = THISBACK(CloseData);
	list.WhenCloseNoData = THISBACK(CloseNoData);
	list.WhenClose = THISBACK(Close);
	list.WhenSearchCursor = THISBACK(SearchCursor);
	EnableDrop(false);
	list_width = 0;
	list_height = 0;
	drop_lines = 16;
	display_all = false;
	header = true;
	key_col = 0;
	value_col = 0;
	rowid = -1;
	notnull = false;
	display = &Single<MultiDisplay>();
}

void DropGrid::Close()
{
	Rect r = list.GetRect();
	list_width = r.Width();
	list_height = r.Height();
	SetFocus();
}

void DropGrid::CloseData()
{
	rowid = list.GetRowId();
	value = list.Get(value_col);	
}

void DropGrid::CloseNoData()
{
	if(list.GetRowId() != rowid)
		list.SetCursorId(rowid);
}

void DropGrid::Drop()
{
	if(!IsEditable())
		return;

	if(!header)
	{
		list.HideRow(0, false);
		list.UpdateRows(true);
	}

	Rect rs = GetScreenRect();
	int width = rs.Width();	
	int height = list.GetHeight() + 4;;
	int drop_height = drop_lines * GridCtrl::GD_ROW_HEIGHT + header * GridCtrl::GD_HDR_HEIGHT + 4;
	if(!display_all && height > drop_height)
		height = drop_height;
	
	list.resize_panel.SetMinSize(Size(width, height));

	if(list_width > width)
		width = list_width;
	if(list_height > height)
		height = list_height;

	Rect rw = Ctrl::GetWorkArea();
	Rect r;
	r.left   = rs.left;
	r.right  = rs.left + width;
	r.top    = rs.bottom;
	r.bottom = rs.bottom + height;

	if(r.bottom > rw.bottom)
	{
		r.top = rs.top - height;
		r.bottom = rs.top;
	}
	if(r.right > rw.right)
	{
		int diff;
		if(rs.right <= rw.right)
			diff = rs.right - r.right;
		else
			diff = rw.right - r.right;

		r.left += diff;
		r.right += diff;
	}
	if(r.left < rw.left)
	{
		int diff = rw.left - r.left;
		r.left += diff;
		r.right += diff;

	}

	if(rowid < 0 && !list.IsEmpty())
		rowid = list.GetRowId(0);
	
	list.ClearFound();
	list.SetCursorId(rowid);
	list.PopUp(this, r);
	list.CenterCursor();
}

void DropGrid::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, SColorPaper());
	GridDisplay &disp = display ? *display : list.GetDisplay();
	bool hf = HasFocus();
	Color fg = hf ? SColorHighlightText() : SColorText();
	Color bg = !IsEnabled() ? SColorFace
	                        : notnull && IsNull(value) ? Blend(SColorPaper, Color(255, 0, 0), 32) 
	                                                   : hf ? SColorHighlight() : SColorPaper();
	                                                   
	int cnt = value_cols.GetCount();
	if(cnt > 0 && !IsNull(value))
	{
		int r = list.FindRow(rowid);
		Vector<String> v;
		for(int i = 0; i < cnt; i++)
		{
			Value val = list.Get(r, value_cols[i]);
			if(IsNull(val))
				continue;
			v.Add(list.GetColumn(value_cols[i]).GetName());
			v.Add(list.GetConvertedColumn(value_cols[i], val));
		}
		
		disp.Paint(w, 1, 1, sz.cx - 2, sz.cy - 2, RawPickToValue< Vector<String> >(v), 0, fg, bg, StdFont(), false, 0, 0);
	}
	else                           
		disp.Paint(w, 1, 1, sz.cx - 2, sz.cy - 2, list.GetConvertedColumn(value_col, value), 0, fg, bg, StdFont(), false, 0, 0);
}

void DropGrid::LeftDown(Point p, dword keyflags)
{
	if(list.IsOpen())
		list.Close();
	else
		Push();
}

void DropGrid::GotFocus()
{
	Refresh();
}

void DropGrid::LostFocus()
{
	Refresh();
}

DropGrid& DropGrid::Width(int w)
{
	list_width = w;
	return *this;
}

DropGrid& DropGrid::Height(int h)
{
	list_height = h;
	return *this;
}

DropGrid& DropGrid::SetKeyColumn(int n)
{
	key_col = n;
	return *this;
}

DropGrid& DropGrid::SetValueColumn(int n)
{
	value_col = n;
	return *this;
}

DropGrid& DropGrid::AddValueColumn(int n)
{
	value_cols.Add(n);
	return *this;
}

DropGrid& DropGrid::DisplayAll(bool b)
{
	display_all = b;
	return *this;
}

DropGrid& DropGrid::SetDropLines(int d)
{
	drop_lines = d;
	return *this;
}

DropGrid& DropGrid::NoHeader(bool b)
{
	header = b;
	return *this;
}

DropGrid& DropGrid::Resizeable(bool b /* = true*/)
{
	list.ResizePanel(b);
	return *this;
}

DropGrid& DropGrid::ColorRows(bool b)
{
	list.ColorRows(b);
	return *this;
}

DropGrid& DropGrid::NotNull(bool b)
{
	notnull = b;
	return *this;
}

DropGrid& DropGrid::SetDisplay(GridDisplay &d)
{
	display = &d;
	return *this;
}

void DropGrid::SetIndex(int n)
{
	list.SetCursor(n);
}

Value DropGrid::GetData() const
{
	return list.IsCursor() ? list.Get(key_col) : notnull ? NotNullError() : Null;
}

void DropGrid::SetData(const Value& v)
{
	int row = list.Find(v, key_col);
	if(row >= 0)
	{
		list.SetCursor(row);
		value = list.Get(value_col);
		rowid = list.GetRowId(row);
	}
}

DropGrid& DropGrid::AddColumn(const char *name, int width)
{
	list.AddColumn(name, width);
	return *this;
}

DropGrid& DropGrid::AddColumn(Id id, const char *name, int width)
{
	list.AddColumn(id, name, width);
	return *this;
}

DropGrid& DropGrid::AddIndex(const char *name)
{
	list.AddIndex(name);
	return *this;
}

DropGrid& DropGrid::AddIndex(Id id)
{
	list.AddIndex(id);
	return *this;
}

void DropGrid::ClearValue()
{
	value = Null;
	rowid = -1;
	Refresh();
}

void DropGrid::Reset()
{
	list.Reset();
	ClearValue();
	EnableDrop(false);
}

void DropGrid::Clear()
{
	list.Clear();
	ClearValue();
	EnableDrop(false);
}

Value DropGrid::Get(int c) const
{
	return list.Get(c);
}

Value DropGrid::Get(Id id) const
{
	return list.Get(id);
}

Value DropGrid::Get(int r, int c) const
{
	return list.Get(r, c);
}

Value DropGrid::Get(int r, Id id) const
{
	return list.Get(r, id);
}

void DropGrid::Set(int c, const Value& v)
{
	list.Set(c, v);
}

void DropGrid::Set(Id id, const Value& v)
{
	list.Set(id, v);
}

void DropGrid::Set(int r, int c, const Value& v)
{
	list.Set(r, c, v);
}

void DropGrid::Set(int r, Id id, const Value& v)
{
	list.Set(r, id, v);
}

void DropGrid::Set(int r, const Vector<Value> &v, int data_offset, int column_offset)
{
	list.Set(r, v, data_offset, column_offset);
}

void DropGrid::Add(const Vector<Value> &v, int data_offset, int column_offset)
{
	list.AddRow();
	list.Set(v, data_offset, column_offset);
	EnableDrop();
}

int DropGrid::Find(const Value& v, int col /* = 0*/)
{
	return list.Find(v, col);
}

int DropGrid::Find(const Value& v, Id id)
{
	return list.Find(v, id);
}

void DropGrid::Change(int dir)
{
	int c = -1;
	if(!list.IsCursor())
	{
		if(dir < 0)
			list.GoEnd();
		else
			list.GoBegin();
			
		c = list.GetCursor();
	}
	else
		c = list.GetCursor() + dir;
	
	if(list.IsValidCursor(c))
	{
		list.SetCursor(c);
		value = list.Get(value_col);
		rowid = list.GetRowId();
		
	}
	
	Refresh();
}

void DropGrid::SearchCursor()
{
	value = list.Get(value_col);
	Refresh();
}

bool DropGrid::Key(dword k, int) 
{
	if(IsReadOnly()) return false;
	
	switch(k) 
	{
		case K_ALT_DOWN:
		case K_ENTER:
			Push();
			break;
		case K_DOWN:
		case K_RIGHT:
			Change(1);
			break;
		case K_UP:
		case K_LEFT:
			Change(-1);
			break;
		case K_DELETE:
			value = Null;
			rowid = -1;
			Refresh();
			break;
		default:
			if(k >= 32 && k < 256) 
			{
				if(!list.IsOpen())
					Push();
				list.Search(k);
			}
			return false;
	}
	return true;
}


#define E__Addv(I)    list.Set(q, I - 1, p##I)
#define E__AddF(I) \
DropGrid& DropGrid::Add(__List##I(E__Value)) { \
	EnableDrop(true); \
	int q = list.GetCount(); \
	__List##I(E__Addv); \
	return *this; \
}
__Expand(E__AddF)

END_UPP_NAMESPACE






