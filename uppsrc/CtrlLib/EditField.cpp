#include "CtrlLib.h"

void EditField::CancelMode()
{
	keep_selection = false;
}

int EditField::GetTextCx(const wchar *txt, int n, bool password)
{
	FontInfo fi = font.Info();
	if(password)
		return n * fi['*'];
	const wchar *s = txt;
	int x = 0;
	while(n--)
		x += fi[*s++];
	return x;
}

int  EditField::GetCaret(int cursor)
{
	return GetTextCx(text, cursor, password);
}

int  EditField::GetStdHeight(Font font)
{
	Size sz = Size(font.Info().GetHeight() * 4, font.Info().GetHeight() + 4);
	EditFieldFrame().FrameAddSize(sz);
	return sz.cy;
}

Size EditField::GetMinSize() const
{
	return AddFrameSize(10, font.Info().GetHeight() + 4);
}

int  EditField::GetCursor(int posx)
{
	FontInfo fi = font.Info();
	const wchar *s = text;
	posx -= 2;
	int x = 0;
	int x0 = 0;
	int i = 0;
	for(i = 0; i < text.GetLength(); i++) {
		x0 = x;
		x += fi[*s];
		if(posx < (x + x0) / 2)
			return i;
		s++;
	}
	return posx > (x + x0) / 2 ? i : 0;
}

Image EditField::CursorImage(Point, dword)
{
	return Image::IBeam();
}

bool EditField::HasBorder()
{
	Rect r = GetRect();
	Rect q = r;
	GetFrame().FrameLayout(r);
	return q.Height() != r.Height();
}

int  EditField::GetTy()
{
	return (GetSize().cy - font.Info().GetHeight()) / 2;
}

void EditField::Paints(Draw& w, int& x, int fcy, const wchar *&txt,
					   Color ink, Color paper, int n, bool password)
{
	if(n < 0) return;
	int cx = GetTextCx(txt, n, password);
	w.DrawRect(x, 0, cx, fcy, paper);
	if(password) {
		String h;
		h.Cat('*', n);
		w.DrawText(x, 0, ~h, font, ink, n);
	}
	else
		w.DrawText(x, 0, txt, font, ink, n);
	txt += n;
	x += cx;
}

void EditField::Paint(Draw& w)
{
	Size sz = GetSize();
	bool f = HasBorder();
	Color paper = IsShowEnabled() && !IsReadOnly() ? SColorPaper : SColorFace;
	Color ink = IsShowEnabled() ? SColorText : SColorDisabled;
	if(convert && convert->Scan(text).IsError())
		paper = Blend(paper, Color(255, 0, 0), 32);
	int fcy = font.Info().GetHeight();
	int yy = GetTy();
	w.DrawRect(0, 0, 2, sz.cy, paper);
	w.DrawRect(0, 0, sz.cx, yy, paper);
	w.DrawRect(0, yy + fcy, sz.cx, sz.cy - yy - fcy, paper);
	w.DrawRect(sz.cx - 2, 0, 2, sz.cy, paper);
	w.Clipoff(2, yy, sz.cx - 4, fcy);
	int x = -sc;
	if(IsNull(text) && !IsNull(nulltext)) {
		const wchar *txt = nulltext;
		Paints(w, x, fcy, txt, nullink, paper, nulltext.GetLength(), false);
	}
	else {
		const wchar *txt = text;
		int l, h;
		if(GetSelection(l, h)) {
			Paints(w, x, fcy, txt, ink, paper, l, password);
			Paints(w, x, fcy, txt, IsShowEnabled() ? SColorHighlightText() : paper,
			                       IsShowEnabled() ? SColorHighlight() : ink, h - l, password);
			Paints(w, x, fcy, txt, ink, paper, text.GetLength() - h, password);
		}
		else
			Paints(w, x, fcy, txt, ink, paper, text.GetLength(), password);
	}
	w.DrawRect(x, 0, 9999, fcy, paper);
	w.End();
}

bool EditField::GetSelection(int& l, int& h) const
{
	if(anchor < 0 || anchor == cursor) {
		l = h = cursor;
		return false;
	}
	if(anchor < cursor) {
		l = anchor;
		h = cursor;
	}
	else {
		l = cursor;
		h = anchor;
	}
	return true;
}

bool EditField::IsSelection() const
{
	return anchor >= 0 && anchor != cursor;
}

void EditField::Finish(bool refresh)
{
	if(anchor > text.GetLength()) anchor = text.GetLength();
	if(cursor > text.GetLength()) cursor = text.GetLength();
	if(cursor < 0) cursor = 0;
	Size sz = GetSize();
	if(autosize) {
		Rect r = GetRect();
		int mw = min(r.Width(), Draw::GetStdFontSize().cx);
		sz.cx = GetCaret(text.GetLength()) + 4;
		sz = AddFrameSize(sz);
		if(GetParent())
			sz.cx = min(sz.cx, GetParent()->GetSize().cx - r.left);
		sz.cx = minmax(sz.cx, mw, autosize);
		if(sz.cx != r.Width())
			LeftPos(r.left, sz.cx);
		sz = GetSize();
	}
	sz.cx -= 2;
	if(sz.cx <= 0) return;
	int x = GetCaret(cursor);
	if(x > sz.cx + sc - 1) {
		sc = x - sz.cx + 1;
		Refresh();
	}
	if(x < sc) {
		sc = x;
		Refresh();
	}
	if(refresh)
		Refresh();
	FontInfo fi = font.Info();
	SetCaret(x - sc + 2 - fi.GetRightSpace('o'), GetTy(),
	         1, min(sz.cy - 2 * GetTy(), fi.GetHeight()));
}

void EditField::Layout()
{
	Ctrl::Layout();
	sc = 0;
	Finish();
}

void EditField::GotFocus()
{
	if(!keep_selection) {
		anchor = 0;
		cursor = text.GetLength();
	}
	Finish();
}

void EditField::LostFocus()
{
	if(autoformat && IsEditable()) {
		Value v = convert->Scan(text);
		if(!v.IsError()) {
			WString s = convert->Format(v);
			if(s != text) text = s;
		}
	}
	if(!keep_selection) {
		anchor = -1;
		cursor = sc = 0;
	}
	Refresh();
}

void EditField::LeftDown(Point p, dword flags)
{
	if(!HasFocus()) {
		SetFocus();
		if(clickselect) {
			SetSelection();
			Finish();
			return;
		}
		sc = 0;
	}
	SetCapture();
	Move(GetCursor(p.x + sc), flags & K_SHIFT);
	Finish();
}

void EditField::LeftDouble(Point p, dword flags)
{
	if(IsLeNum(text[cursor])) {
		int l = cursor;
		int h = cursor;
		while(l > 0 && IsLeNum(text[l - 1]))
			l--;
		while(h < GetLength() && IsLeNum(text[h]))
			h++;
		if(h != cursor)
			while(h < GetLength() && text[h] == ' ')
				h++;
		if(l != h)
			SetSelection(l, h);
	}
}

void EditField::MouseMove(Point p, dword flags)
{
	if(!HasCapture()) return;
	Move(GetCursor(p.x + sc), true);
	Finish();
}

void EditField::SaveUndo()
{
	undotext = text;
	undoanchor = anchor;
	undocursor = cursor;
}

void EditField::Move(int newpos, bool select)
{
	bool refresh = anchor >= 0;
	if(select) {
		if(anchor < 0) anchor = cursor;
		refresh = true;
	}
	else
		anchor = -1;
	cursor = newpos;
	Finish(refresh);
}

void EditField::SetSelection(int l, int h)
{
	if(l < h) {
		anchor = max(l, 0);
		cursor = min(h, text.GetLength());
	}
	else {
		cursor = l;
		anchor = -1;
	}
	Finish();
}

bool EditField::RemoveSelection()
{
	int l, h;
	if(!GetSelection(l, h)) {
		anchor = -1;
		return false;
	}
	SaveUndo();
	Remove(l, h - l);
	cursor = l;
	anchor = -1;
	sc = 0;
	return true;
}

void EditField::Copy()
{
	int l, h;
	if(password) return;
	if(!GetSelection(l, h)) {
		l = 0;
		h = text.GetLength();
	}
	WriteClipboardUnicodeText(text.Mid(l, h - l));
}

int EditField::Insert(int pos, const WString& itext)
{
	if(IsReadOnly()) return 0;
	WString ins;
	const wchar *s = itext;
	for(;;) {
		wchar chr = *s++;
		if(chr == '\t')
			ins.Cat(WString(' ', 4));
		else
		if(chr >= ' ') {
			chr = (*filter)(chr);
			if(chr) {
				chr = convert->Filter(chr);
				if(chr) ins.Cat(chr);
			}
		}
		else
			break;
	}
	if(ins.GetCount() + text.GetCount() > maxlen) {
		BeepExclamation();
		return 0;
	}
	text.Insert(pos, ins);
	Update();
	return ins.GetLength();
}

void EditField::Remove(int pos, int n)
{
	if(IsReadOnly()) return;
	text.Remove(pos, n);
	Update();
}

void EditField::Insert(int chr)
{
	if(IsReadOnly()) return;
	if(initcaps && cursor == 0 && text.GetCount() == 0)
		chr = ToUpper(chr);
	cursor += Insert(cursor, WString(chr, 1));
	Finish();
}

void EditField::Insert(const WString& s)
{
	if(!RemoveSelection()) SaveUndo();
	cursor += Insert(cursor, s);
	Finish();
}

static inline bool IsWord(int c)
{
	return IsLetter(c) || IsDigit(c);
}

void EditField::Undo()
{
	Swap(undotext, text);
	Swap(undoanchor, anchor);
	Swap(undocursor, cursor);
	anchor = -1;
	UpdateAction();
	Finish();
}

void EditField::Cut()
{
	Copy();
	RemoveSelection();
	Action();
	Finish();
}

void EditField::Paste()
{
	Insert(ReadClipboardUnicodeText());
	Action();
	Finish();
}

void EditField::Erase()
{
	if(!IsSelection())
		SelectAll();
	RemoveSelection();
	Finish();
}

void EditField::SelectAll()
{
	SetSelection();
	Finish();
}

void EditField::MenuBar(Bar& menu) {
	menu.Add(t_("Undo"), THISBACK(Undo))
		.Key(K_ALT_BACKSPACE)
		.Key(K_CTRL_Z);
	menu.Separator();
	menu.Add(IsEditable() && IsSelection(),
			t_("Cut"), CtrlImg::cut(), THISBACK(Cut))
		.Key(K_SHIFT_DELETE)
		.Key(K_CTRL_X);
	menu.Add(IsSelection(),
			t_("Copy"), CtrlImg::copy(), THISBACK(Copy))
		.Key(K_CTRL_INSERT)
		.Key(K_CTRL_C);
	menu.Add(IsEditable()
		#ifdef PLATFORM_WIN32
			&& IsClipboardFormatAvailable(CF_TEXT)
		#endif
			,
			t_("Paste"), CtrlImg::paste(), THISBACK(Paste))
		.Key(K_SHIFT_INSERT)
		.Key(K_CTRL_V);
	menu.Add(IsEditable(),
			t_("Erase"), CtrlImg::remove(), THISBACK(Erase))
		.Key(K_DELETE);
	menu.Separator();
	menu.Add(GetLength(),
			t_("Select all"), THISBACK(SelectAll))
		.Key(K_CTRL_A);
}


void EditField::RightDown(Point p, dword keyflags)
{
	keep_selection = true;
	MenuBar::Execute(THISBACK(MenuBar));
	SetFocus();
	keep_selection = false;
}

bool EditField::Key(dword key, int rep)
{
	int q;
	bool h;
	String s;
	bool select = key & K_SHIFT;
	switch(key & ~K_SHIFT) {
	case K_LEFT:
		Move(cursor - 1, select);
		return true;
	case K_CTRL_LEFT:
		if(cursor <= 0) return true;
		q = cursor - 1;
		while(q > 0 && text[q - 1] != ' ')
			q--;
		Move(q, select);
		return true;
	case K_CTRL_RIGHT:
		if(cursor >= text.GetLength()) return true;
		q = cursor + 1;
		while(q < text.GetLength() && text[q] != ' ')
			q++;
		Move(q, select);
		return true;
	case K_RIGHT:
		Move(cursor + 1, select);
		return true;
	case K_HOME:
		Move(0, select);
		return true;
	case K_END:
		Move(text.GetLength(), select);
		return true;
	case K_CTRL_C:
	case K_CTRL_INSERT:
		Copy();
		return true;
	case K_CTRL_A:
		SetSelection();
		return true;
	}
	if(!IsEditable())
		return false;
	switch(key) {
	case K_CTRL_X:
	case K_SHIFT_DELETE:
		Cut();
		return true;
	case K_CTRL_V:
	case K_SHIFT_INSERT:
		Paste();
		return true;
	case K_CTRL_Z:
	case K_ALT_BACKSPACE:
		Undo();
		return true;
	case K_BACKSPACE:
	case K_SHIFT|K_BACKSPACE:
		if(RemoveSelection()) {
			Action();
			break;
		}
		if(cursor == 0 || IsReadOnly()) return true;
		SaveUndo();
		cursor--;
		Remove(cursor, 1);
		Action();
		break;
	case K_CTRL_BACKSPACE:
		if(RemoveSelection()) {
			Action();
			break;
		}
		if(cursor == 0 || IsReadOnly()) return true;
		SaveUndo();
		q = cursor;
		h = IsWord(text[--cursor]);
		while(cursor > 0 && IsWord(text[cursor - 1]) == h)
			cursor--;
		Remove(cursor, q - cursor);
		Action();
		break;
	case K_DELETE:
		if(RemoveSelection()) {
			Action();
			break;
		}
		if(cursor >= text.GetLength()) return true;
		SaveUndo();
		Remove(cursor, 1);
		Action();
		break;
	case K_CTRL_DELETE:
		if(RemoveSelection()) {
			Action();
			break;
		}
		if(cursor >= text.GetLength()) return true;
		q = cursor;
		h = IsWord(text[q]);
		while(IsWord(text[q]) == h && q < text.GetLength()) q++;
		SaveUndo();
		Remove(cursor, q - cursor);
		Action();
		break;
	default:
		if(key >= ' ' && key < 65536 || key == K_SHIFT_SPACE) {
			if(!RemoveSelection()) SaveUndo();
			while(rep--)
				Insert(key == K_SHIFT_SPACE ? ' ' : key);
			Action();
			return true;
		}
		else
			return false;
	}
	Finish();
	return true;
}

void EditField::SetText(const WString& txt)
{
	if(text == txt) {
		Update();
		return;
	}
	text = txt;
	sc = 0;
	if(HasFocus()) {
		cursor = txt.GetLength();
		anchor = 0;
	}
	else {
		cursor = 0;
		anchor = -1;
	}
	Update();
	Finish();
}

void EditField::SetData(const Value& data)
{
	SetText((WString)convert->Format(data));
}

Value EditField::GetData() const
{
	return convert->Scan(text);
}

void EditField::Clear()
{
	SetText(WString());
	sc = cursor = 0;
}

void EditField::Reset()
{
	Clear();
	ClearModify();
	sc = 0;
	cursor = 0;
	anchor = -1;
	password = false;
	autoformat = true;
	clickselect = false;
	filter = CharFilterUnicode;
	convert = &NoConvert();
	initcaps = false;
	maxlen = INT_MAX;
	autosize = false;
	keep_selection = false;
}

EditField& EditField::SetFont(Font _font)
{
	font = _font;
	Refresh();
	return *this;
}

EditField& EditField::NullText(const char *text, Color ink)
{
	nulltext = text;
	nullink = ink;
	Refresh();
	return *this;
}

EditField::EditField()
{
	Unicode();
	Reset();
	SetFrame(EditFieldFrame());
}

EditField::~EditField() {}

void EditIntSpin::Inc()
{
	if(IsReadOnly())
	{
		BeepExclamation();
		return;
	}
	int i = GetData();
	if(IsNull(i)) {
		if(IsNull(GetMin()) || GetMin() == INT_MIN) return;
		SetData(GetMin());
	}
	else
	if(!IsNull(GetMax()) && i < GetMax())
		SetData(i + 1);
	else
		return;
	SetFocus();
	SetSelection();
	UpdateAction();
}

void EditIntSpin::Dec()
{
	if(IsReadOnly())
	{
		BeepExclamation();
		return;
	}
	int i = GetData();
	if(IsNull(i)) {
		if(IsNull(GetMax()) || GetMax() == INT_MAX) return;
		SetData(GetMax());
	}
	else
	if(!IsNull(GetMin()) && i > GetMin())
		SetData(i - 1);
	else
		return;
	SetFocus();
	SetSelection();
	UpdateAction();
}

void EditIntSpin::Init()
{
	sb.inc.WhenAction = sb.inc.WhenRepeat = callback(this, &EditIntSpin::Inc);
	sb.dec.WhenAction = sb.dec.WhenRepeat = callback(this, &EditIntSpin::Dec);
	AddFrame(sb);
}

EditIntSpin::EditIntSpin()
{
	Init();
}

EditIntSpin::EditIntSpin(int min, int max)
{
	MinMax(min, max);
	Init();
}

EditIntSpin::~EditIntSpin() {}

bool EditIntSpin::Key(dword key, int repcnt)
{
	if(key == K_UP)   { Inc(); return true; }
	if(key == K_DOWN) { Dec(); return true; }
	return EditInt::Key(key, repcnt);
}

void EditIntSpin::MouseWheel(Point, int zdelta, dword)
{
	if(zdelta < 0)
		Dec();
	else
		Inc();
}

EditDoubleSpin::EditDoubleSpin(double inc) : inc(inc) { Init(); }
EditDoubleSpin::EditDoubleSpin(double min, double max, double inc) : EditDouble(min, max), inc(inc) { Init(); }

EditDoubleSpin::~EditDoubleSpin() {}

void EditDoubleSpin::Init()
{
	AddFrame(spin);
	spin.inc.WhenRepeat = spin.inc.WhenAction = THISBACK(Inc);
	spin.dec.WhenRepeat = spin.dec.WhenAction = THISBACK(Dec);
}

void EditDoubleSpin::Inc()
{
	if(IsReadOnly())
	{
		BeepExclamation();
		return;
	}
	double d = GetData();
	if(!IsNull(d))
	{
		d = (floor(d / inc + 1e-3) + 1) * inc;
		if(IsNull(GetMax()) || d <= GetMax())
		{
			SetData(d);
			Action();
		}
	}
}

void EditDoubleSpin::Dec()
{
	if(IsReadOnly())
	{
		BeepExclamation();
		return;
	}
	double d = GetData();
	if(!IsNull(d))
	{
		d = (ceil(d / inc - 1e-3) - 1) * inc;
		if(IsNull(GetMin()) || d >= GetMin())
		{
			SetData(d);
			Action();
		}
	}
}

bool EditDoubleSpin::Key(dword key, int repcnt)
{
	if(key == K_UP)   { Inc(); return true; }
	if(key == K_DOWN) { Dec(); return true; }
	return EditDouble::Key(key, repcnt);
}

void EditDoubleSpin::MouseWheel(Point, int zdelta, dword)
{
	if(zdelta < 0)
		Dec();
	else
		Inc();
}
