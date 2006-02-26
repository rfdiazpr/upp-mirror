#include "ide.h"

IdeQuickTabs::IdeQuickTabs()
{
	font = Arial(11);
	fcy = ScreenInfo().GetFontInfo(font).GetHeight();
	hl = -1;
	cursor = -1;
	sc = 0;
	Add(left);
	left.SetImage(CtrlImg::smallleft()).ScrollStyle();
	left <<= THISBACK(Left);
	Add(right);
	right.SetImage(CtrlImg::smallright()).ScrollStyle();
	right <<= THISBACK(Right);
}

void IdeQuickTabs::PaintTab(Draw& w, int x, int y, int cx, int cy, Color paper, Color top, bool hlclose)
{
	w.DrawRect(x + 1, y + 3, cx - 2, cy - 2, paper);
	w.DrawRect(x + 1, y + 2, cx - 2, 1, top);
	w.DrawRect(x + 2, y + 1, cx - 4, 1, top);
	w.DrawRect(x + 2, y, cx - 6, 1, top);
	Color fc = Blend(SColorHighlight, SColorShadow);
	w.DrawRect(x + 0, y + 2, 1, cy - 2, fc);
	w.DrawRect(x + 1, y + 1, 1, 1, fc);
	w.DrawRect(x + 2, y, cx - 4, 1, fc);
	w.DrawRect(x + cx - 2, y + 1, 1, 1, fc);
	w.DrawRect(x + cx - 1, y + 2, 1, cy - 2, fc);
	if(tab.GetCount() > 1)
		w.DrawImage(x + cx - 13, y + (cy - 9) / 2 + 1, hlclose ? IdeImg::miniclose1() : IdeImg::miniclose());
}

void IdeQuickTabs::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, SColorFace);
	Color hc = Blend(Yellow, LtRed, 100);
	Color inactive = Blend(SColorPaper, SColorFace);
	for(int i = 0; i < tab.GetCount(); i++) {
		Tab& t = tab[i];
		if(i != cursor && t.Right() > sc && t.x < sc + sz.cx)
			PaintTab(w, t.x + 2 - sc, 2, t.cx - 1, sz.cy - 2, inactive, i == hl ? hc : inactive, i == hl && hlclose);
	}
	if(cursor >= 0) {
		Tab& t = tab[cursor];
		PaintTab(w, t.x - sc, 1, t.cx + 4, sz.cy - 2, SColorPaper, hc, cursor == hl && hlclose);
	}
	for(int i = 0; i < tab.GetCount(); i++) {
		Tab& t = tab[i];
		if(t.Right() > sc && t.x < sc + sz.cx) {
			String fn = GetFileName(t.file);
			Image m = IdeFileImage(fn);
			Size isz = m.GetSize();
			w.DrawImage(t.x + DX - sc, (fcy + HEIGHT - 4 - isz.cy) / 2 + 4 - (i == cursor), m);
			DrawFileName(w, t.x + DX - sc + isz.cx + 2, (HEIGHT - 4) / 2 + 4 - (i == cursor),
			             t.cx - DX - 4 - isz.cx - 2, fcy, fn.ToWString(),
			             false, font, SColorText, LtBlue, Null, Null, false);
		}
	}
}

void IdeQuickTabs::Layout()
{
	if(tab.GetCount()) {
		int cx = GetSize().cx - 20;
		int r = tab.Top().Right();
		if(tab.Top().Right() < sc + cx)
			sc = max(0, tab.Top().Right() - cx);
	}
	Sc();
}

void IdeQuickTabs::SyncLeftRight()
{
	if(tab.GetCount() == 0) {
		sc = 0;
		Refresh();
	}
	left.Hide();
	right.Hide();
	if(sc)
		left.LeftPos(0, 16).BottomPos(0, 16).Show();
	if(tab.GetCount() && GetSize().cx + sc - 20 < tab.Top().x + tab.Top().cx)
		right.RightPos(0, 16).BottomPos(0, 16).Show();
}

void IdeQuickTabs::Sc()
{
	if(cursor >= 0) {
		int cx = GetSize().cx - 20;
		Size sz = GetSize();
		Tab& t = tab[cursor];
		if(t.Right() > sc + cx) {
			sc = t.Right() - cx;
			Refresh();
		}
		if(t.x < sc) {
			sc = t.x;
			Refresh();
		}
	}
	SyncLeftRight();
}

void IdeQuickTabs::Left()
{
	if(tab.GetCount())
		sc = max(sc - max(GetSize().cx - 20, 0), 0);
	else
		sc = 0;
	Refresh();
	SyncLeftRight();
}

void IdeQuickTabs::Right()
{
	int cx = GetSize().cx - 20;
	if(tab.GetCount())
		sc = min(tab.Top().Right() - cx, sc + max(GetSize().cx - 20, 0));
	else
		sc = 0;
	Refresh();
	SyncLeftRight();
}

void IdeQuickTabs::Repos()
{
	int x = 0;
	for(int i = 0; i < tab.GetCount(); i++) {
		Tab& t = tab[i];
		String fn = GetFileName(t.file);
		Image m = IdeFileImage(fn);
		t.cx = ScreenInfo().GetTextSize(GetFileName(t.file), font).cx + m.GetSize().cx + 26;
		t.x = x;
		x += t.cx;
	}
}

int IdeQuickTabs::GetPos(Point p, bool& close)
{
	p.x += sc;
	close = false;
	for(int i = 0; i < tab.GetCount(); i++) {
		Tab& t = tab[i];
		if(p.x < t.Right()) {
			if(p.x > t.Right() - 13)
				close = true;
			return i;
		}
	}
	return -1;
}

void IdeQuickTabs::SetCursor(int i)
{
	ASSERT(cursor < tab.GetCount());
	cursor = i;
	Sc();
	Refresh();
}

void IdeQuickTabs::LeftDown(Point p, dword)
{
	bool close;
	int c = GetPos(p, close);
	if(c >= 0) {
		if(close)
			Close(c);
		else
		if(c != cursor) {
			cursor = c;
			Sc();
			UpdateActionRefresh();
		}
	}
}

void IdeQuickTabs::MouseMove(Point p, dword)
{
	bool close;
	int c = GetPos(p, close);
	if(c != hl || close != hlclose) {
		hl = c;
		hlclose = close;
		Refresh();
	}
}

void IdeQuickTabs::MouseLeave()
{
	CancelMode();
}

void IdeQuickTabs::CancelMode()
{
	hl = -1;
	hlclose = false;
	Refresh();
}

void IdeQuickTabs::FramePaint(Draw& w, const Rect& rr)
{
	Rect r = rr;
	r.top += fcy + HEIGHT - 1;
	FieldFrame().FramePaint(w, r);
	if(cursor >= 0) {
		Tab& t = tab[cursor];
		int cx = (rr.Width() - 6) / 10;
		w.DrawRect(rr.left + t.x + 1 - sc, rr.top + fcy + HEIGHT - 1, t.cx + 2, 1, SColorFace);
	}
}

void IdeQuickTabs::FrameLayout(Rect& r)
{
	LayoutFrameTop(r, this, fcy + HEIGHT);
	r.top++;
	r.left += 2;
	r.right -= 2;
	r.bottom -= 2;
}

void IdeQuickTabs::FrameAddSize(Size& sz)
{
	sz += 4;
	sz.cy += fcy + HEIGHT;
}

bool IdeQuickTabs::FindSetFile(const String& fn)
{
	for(int i = 0; i < tab.GetCount(); i++)
		if(tab[i].file == fn) {
			SetCursor(i);
			return true;
		}
	return false;
}

void IdeQuickTabs::SetAddFile(const String& fn)
{
	if(IsNull(fn))
		return;
	if(FindSetFile(fn))
		return;
	cursor = tab.GetCount();
	Tab& b = tab.Add();
	b.file = fn;
	Repos();
	Sc();
	Refresh();
}

void IdeQuickTabs::RenameFile(const String& fn, const String& nn)
{
	for(int i = 0; i < tab.GetCount(); i++)
		if(tab[i].file == fn) {
			tab[i].file = nn;
			Repos();
			Sc();
			Refresh();
		}
}

void IdeQuickTabs::Set(const IdeQuickTabs& tabs)
{
	cursor = tabs.cursor;
	tab.SetCount(tabs.tab.GetCount());
	for(int i = 0; i < tabs.tab.GetCount(); i++)
		tab[i].file = tabs.tab[i].file;
	Repos();
	Sc();
	Refresh();
}

void IdeQuickTabs::Clear()
{
	tab.Clear();
	cursor = -1;
	SyncLeftRight();
	Refresh();
}

void IdeQuickTabs::Close(int i)
{
	int q = -1;
	if(cursor == i) {
		cursor = -1;
		q = i;
	}
	else
	if(cursor > i)
		cursor--;
	if(tab.GetCount() == 1)
		return;
	tab.Remove(i);
	Repos();
	SyncLeftRight();
	Refresh();
	if(q >= tab.GetCount())
		q = tab.GetCount() - 1;
	if(q >= 0) {
		cursor = q;
		Sc();
		UpdateActionRefresh();
	}
}

void Ide::TabFile()
{
	int q = tabs.GetCursor();
	if(q >= 0)
		EditFile(tabs.GetFile(q));
}

void Ide::DTabFile()
{
	int q = dtabs.GetCursor();
	if(q >= 0)
		EditFile(dtabs.GetFile(q));
}

void Ide::TabFile2()
{
	int q = tabs2.GetCursor();
	if(q >= 0) {
		String fn = tabs2.GetFile(q);
		SwapEditors();
		EditFile(fn);
	}
}

void Ide::ClearTab()
{
	if(designer) {
		int c = dtabs.GetCursor();
		if(c >= 0)
			dtabs.Close(c);
		tabs.Set(dtabs);
	}
	else {
		int c = tabs.GetCursor();
		if(c >= 0)
			tabs.Close(c);
	}
	tabs2.Set(tabs);
}

void Ide::ClearTabs()
{
	dtabs.Clear();
	tabs.Clear();
	FileSelected();
}

void Ide::TabsLR(int d)
{
	IdeQuickTabs& t = designer ? dtabs : tabs;
	int c = t.GetCursor();
	if(c < 0 || t.GetCount() <= 1)
		return;
	c = minmax(c + d, 0, t.GetCount() - 1);
	EditFile(t.GetFile(c));
}

void Ide::FileSelected()
{
	if(!IsNull(editfile))
		(designer ? dtabs : tabs).SetAddFile(editfile);
}
