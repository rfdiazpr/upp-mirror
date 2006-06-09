#include "CtrlLib.h"

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
	return font.Info().GetHeight() + 6;
}

int  EditField::GetCursor(int posx)
{
	FontInfo fi = font.Info();
	const wchar *s = text;
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

void EditField::Paints(Draw& w, int& x, int cy, const wchar *&txt,
					   Color ink, Color paper, int n, bool password)
{
	if(n < 0) return;
	int cx = GetTextCx(txt, n, password);
	bool f = HasBorder();
	w.DrawRect(x + 1, f, cx, cy, paper);
	if(password) {
		String h;
		h.Cat('*', n);
		w.DrawText(x + 1, f, ~h, font, ink, n);
	}
	else
		w.DrawText(x + 1, f, txt, font, ink, n);
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
	w.DrawRect(0, 0, 1, sz.cy, paper);
	w.DrawRect(sz.cx - 1, 0, 1, sz.cy, paper);
	w.Clip(1, 0, sz.cx - 2, sz.cy);
	if(f) {
		w.DrawRect(0, 0, sz.cx, 1, paper);
		w.DrawRect(0, sz.cy - 1, sz.cx, 1, paper);
		w.End();
		w.Clip(1, 1, sz.cx - 2, sz.cy - 2);
	}
	int x = -sc;
	if(IsNull(text) && !IsNull(nulltext)) {
		const wchar *txt = nulltext;
		Paints(w, x, sz.cy, txt, nullink, paper, nulltext.GetLength(), false);
	}
	else {
		const wchar *txt = text;
		int l, h;
		if(GetSelection(l, h)) {
			Paints(w, x, sz.cy, txt, ink, paper, l, password);
			Paints(w, x, sz.cy, txt, IsShowEnabled() ? SColorHighlightText() : paper,
			                         IsShowEnabled() ? SColorHighlight() : ink, h - l, password);
			Paints(w, x, sz.cy, txt, ink, paper, text.GetLength() - h, password);
		}
		else
			Paints(w, x, sz.cy, txt, ink, paper, text.GetLength(), password);
	}
	w.DrawRect(x + 1, 0, sz.cx - x - f, sz.cy, paper);
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
	bool f = HasBorder();
	FontInfo fi = font.Info();
	SetCaret(x - sc + 1 - fi.GetRightSpace('o'), f, 1, min(sz.cy - 2 * f, fi.GetHeight()));
}

void EditField::Layout()
{
	Ctrl::Layout();
	sc = 0;
	Finish();
}

void EditField::GotFocus()
{
	anchor = 0;
	cursor = text.GetLength();
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
	anchor = -1;
	cursor = sc = 0;
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
		Copy();
		RemoveSelection();
		Action();
		break;
	case K_CTRL_V:
	case K_SHIFT_INSERT:
		Insert(ReadClipboardUnicodeText());
		Action();
		return true;
	case K_CTRL_Z:
	case K_ALT_BACKSPACE:
		Swap(undotext, text);
		Swap(undoanchor, anchor);
		Swap(undocursor, cursor);
		anchor = -1;
		UpdateAction();
		break;
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
	SetFrame(FieldFrame());
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
