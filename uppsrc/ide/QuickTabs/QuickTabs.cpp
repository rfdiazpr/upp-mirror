#include <ide/QuickTabs/QuickTabs.h>
#include <ide/Common/Common.h>

#define IMAGECLASS Img
#define IMAGEFILE <ide/QuickTabs/QuickTabs.iml>
#include <Draw/iml_source.h>

static String ParseGroup(const String &s)
{
	if(!s.IsEmpty())
	{
		if(s[0] == '/' || s[0] == '\\')
			return s.Mid(1);
		else if(s[1] == ':')
			return s.Mid(3);
	}
	return Null;
}

TabScrollBar::TabScrollBar()
{
	total = 0;
	pos = 0;
	ps = 0;
	start_pos = 0;
	new_pos = 0;
	old_pos = 0;
	ready = false;
}

void TabScrollBar::UpdatePos(bool update)
{
	sz = GetSize();
	if(total <= 0 || sz.cx <= 0)
		cs = ics = 0;
	else
	{
		cs = sz.cx / ((double) total + 0.5);
		ics = total / ((double) sz.cx);
	}
	size = sz.cx * cs;
	if(update)
		pos = new_pos - start_pos;
	if(pos < 0)
		pos = 0;
	else if(pos + size > sz.cx)
		pos = sz.cx - size;

	ps = total > sz.cx ? pos * ics : 0;
	WhenScroll();
}

void TabScrollBar::Paint(Draw &w)
{
	if(!ready)
	{
		UpdatePos();
		ready = true;
	}
	Size sz = GetSize();
	w.DrawRect(sz, White);
	if(total > sz.cx)
		w.DrawRect(ffloor(pos), 1, fceil(size), 2, Blue);
	else
		w.DrawRect(0, 1, sz.cx, 2, Blue);
}

void TabScrollBar::Layout()
{
	UpdatePos();
}

void TabScrollBar::LeftDown(Point p, dword keyflags)
{
	SetCapture();
	old_pos = new_pos = p.x;
	if(p.x < pos || p.x > pos + size)
		start_pos = size / 2;
	else
		start_pos = tabs(p.x - pos);
	UpdatePos();
	Refresh();
}

void TabScrollBar::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
	old_pos = p.x;
}

void TabScrollBar::MouseMove(Point p, dword keyflags)
{
	if(!HasCapture())
		return;

	new_pos = p.x;
	UpdatePos();
	Refresh();
}

void TabScrollBar::MouseWheel(Point p, int zdelta, dword keyflags)
{
	AddPos(-zdelta / 4, true);
}

int TabScrollBar::GetPos() const
{
	return ffloor(ps);
}

void TabScrollBar::SetPos(int p, bool dontscale)
{
	pos = total > 0 ? dontscale ? p : iscale(p, sz.cx, total) : 0;
	UpdatePos(false);
	Refresh();
}

void TabScrollBar::AddPos(int p, bool dontscale)
{
	pos += total > 0 ? dontscale ? p : iscale(p, sz.cx, total) : 0;
	UpdatePos(false);
	Refresh();
}

int TabScrollBar::GetTotal() const
{
	return total;
}

void TabScrollBar::SetTotal(int t)
{
	bool upd = total < t;
	total = t;
	UpdatePos(upd);
	Refresh();
}

void TabScrollBar::AddTotal(int t)
{
	total += t;
	UpdatePos();
	Refresh();
}

void TabScrollBar::GoEnd()
{
	pos = total;
	UpdatePos(false);
	Refresh();
}

void TabScrollBar::GoBegin()
{
	pos = 0;
	UpdatePos(false);
	Refresh();
}

void TabScrollBar::Set(const TabScrollBar& t)
{
	total = t.total;
	pos = t.pos;
	ps = t.ps;
	Refresh();
}

QuickTabs::QuickTabs()
{
	AddFrame(sc.Height(4));
	AddFrame(groups.Width(13));
	sc.WhenScroll = THISBACK(Scroll);
	highlight = -1;
	active = -1;
	cross = -1;
	target = -1;
	crosses = true;
	file_icons = false;
	grouping = true;
	isalt = false;
	isdrag = false;
	id = -1;
	groups.WhenGrouping = THISBACK(Group);
	BackPaint();
}

int QuickTabs::GetNextId()
{
	return ++id;
}


void QuickTabs::Group()
{
	Repos();
	SetCursor(-1);
}

bool Tab::HasMouse(Point &p)
{
	return visible && p.x > x && p.x < x + cx;
}

bool Tab::HasMouseCross(Point &p)
{
	if(!visible)
		return false;

	Size isz = Img::CR0().GetSize();
	int iy = cy / 2;
	int ix = x + cx - 20 + isz.cx / 2;

	int icx = isz.cx / 2;
	int icy = isz.cy / 2;

	return p.x > ix - icx && p.x < ix + icx &&
	       p.y > iy - icy && p.y < iy + icy;
}

GroupButton::GroupButton()
{
	Group &g = groups.Add();
	g.name = "All";
	current = 0;
}

int GroupButton::Find(const String& g)
{
	for(int i = 0; i < groups.GetCount(); i++)
		if(groups[i].path == g)
			return i;
	return -1;
}

void GroupButton::Paint(Draw &w)
{
	Size sz = GetSize();
	Size isz = Img::RH().GetSize();
	w.DrawRect(sz, SColorFace());
	w.DrawImage(0, 0, sz.cx - isz.cx, sz.cy, Img::BH);
	w.DrawImage(sz.cx - isz.cx, 0, Img::RH);
	isz = Img::AR().GetSize();
	w.DrawImage((sz.cx - isz.cx) / 2, (sz.cy - isz.cy) / 2, HasMouse() ? Img::ARH : Img::AR);
	w.DrawRect(0, 0, 1, sz.cy, Color(195, 195, 195));
	w.DrawRect(0, sz.cy - 1, sz.cx, 1, Color(128, 128, 128));
}

void GroupButton::LeftDown(Point p, dword keyflags)
{
	MenuBar::Execute(THISBACK(DoList), GetScreenRect().TopRight());
}

void GroupButton::MouseEnter(Point p, dword keyflags)
{
	Refresh();
}

void GroupButton::MouseLeave()
{
	Refresh();
}

void GroupButton::Make(const Vector<Tab> &tabs)
{
	groups[0].count = tabs.GetCount();
	groups[0].first = 0;
	groups[0].last = tabs.GetCount() - 1;

	for(int i = 1; i < groups.GetCount(); i++)
	{
		groups[i].count = 0;
		groups[i].first = 1000000;
		groups[i].last = 0;
	}

	for(int i = 0; i < tabs.GetCount(); i++)
	{
		String s = ParseGroup(tabs[i].group);
		if(IsNull(s))
			continue;
		int n = Find(tabs[i].group);
		if(n < 0)
		{
			Group &g = groups.Add();
			g.count = 1;
			g.first = 1000000;
			g.last = 0;
			g.name = s;
			g.path = tabs[i].group;
			g.active = tabs[i].id;
			n = groups.GetCount() - 1;
		}
		else
		{
			groups[n].count++;
			groups[n].last = i;
		}

		if(i < groups[n].first)
			groups[n].first = i;
		if(i > groups[n].last)
			groups[n].last = i;
	}


	int removed = 0;
	for(int i = 1; i < groups.GetCount(); i++)
		if(groups[i].count == 0)
		{
			groups.Remove(i - removed);
			removed++;
		}
	if(current > groups.GetCount() - 1 && current > 0)
		current--;

}

void GroupButton::DoList(Bar &bar)
{
	int cnt = groups.GetCount();
	for(int i = 0; i < cnt; i++)
	{
		Bar::Item &it = bar.Add(Format("%s (%d)", groups[i].name, groups[i].count), THISBACK1(DoGrouping, i));
		if(i == current)
			it.Image(Img::CHK);
		if(i == 0 && cnt > 1)
			bar.Separator();
	}
	bar.Separator();
	bar.Add("Close all tabs except current", WhenCloseAll);
	bar.Add("Close non-project tabs", WhenCloseRest);
}

void GroupButton::DoGrouping(int n)
{
	current = n;
	WhenGrouping();
}

void QuickTabs::Layout()
{
}

void QuickTabs::DrawTab(Draw &w, int i)
{
	Size sz = GetSize();
	Size isz = Img::CR0().GetSize();

	Tab &t = tabs[i];
	int x = t.x - sc.GetPos();
	int h = Img::LH().GetSize().cy;

	bool ac = i == active;
	bool hl = i == highlight;

	if(ac || hl)
	{
		int sw = Img::LH().GetSize().cx;
		int ew = Img::RH().GetSize().cx;

		w.DrawImage(x, 0, ac ? Img::LA : Img::LH);
		w.DrawImage(x + sw, 0, t.cx - sw - ew, h, ac ? Img::BA : Img::BH);
		w.DrawImage(x + t.cx - ew, 0, ac ? Img::RA : Img::RH);
		if(hl && !ac)
			w.DrawRect(x, t.cy - 1, t.cx, 1, Color(128, 128, 128));
	}
	else
	{
		int sw = Img::LN().GetSize().cx;
		int ew = Img::RN().GetSize().cx;

		w.DrawImage(x, 0, Img::LN);
		w.DrawImage(x + sw, 0, t.cx - sw - ew, h, Img::BN);
		w.DrawImage(x + t.cx - ew, 0, Img::RN);
	}

	if(crosses && tabs.GetCount() > 1)
		w.DrawImage(x + t.cx - isz.cx - MARGIN, (h - isz.cy) / 2, (ac || hl) ? (cross == i ? Img::CR2 : ac ? Img::CR1 : Img::CR0) : Img::CR0);

	if(file_icons)
	{
		Image icon = IdeFileImage(t.name);
		//Image icon = Img::ARH;
		w.DrawImage(x + MARGIN, (sz.cy - icon.GetSize().cx) / 2, icon);
	}

	String fn = GetFileName(t.name);
	int ty = (h - t.tsz.cy) / 2 + 1;
	DrawFileName(w, x + MARGIN + (FILEICON + SPACE) * int(file_icons), ty,
	             t.tsz.cx, t.tsz.cy, fn.ToWString(),
	             false, StdFont(), Black, LtBlue, Null, Null, false);

	//if(ac)
	//	DrawFocus(w, x + MARGIN - 3, ty - 3, t.tsz.cx + 6, t.tsz.cy + 6);
}


void QuickTabs::Paint(Draw &w)
{
	Size sz = GetSize();
	w.DrawRect(sz, SColorFace());
	w.DrawRect(0, sz.cy - 1, sz.cx, 1, Color(128, 128, 128));

	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].visible)
			DrawTab(w, i);

	if(target >= 0)
	{
		int last = groups.GetLast();
		int first = groups.GetFirst();

		int x = (target == last + 1 ? tabs[last].Right() : tabs[target].x) - sc.GetPos() - (target <= first ? 1 : 2);
		w.DrawImage(x, 0, Img::DND());
	}
}

void QuickTabs::Scroll()
{
	Refresh();
}

int QuickTabs::GetWidth(int n)
{
	Tab &t = tabs[n];
	t.tsz = GetTextSize(GetFileName(t.name), StdFont());
	return MARGIN * 2 + t.tsz.cx + (SPACE + Img::CR0().GetSize().cx) * crosses
	                             + (SPACE + FILEICON) * file_icons;
}

int QuickTabs::GetHeight(int n)
{
	return Img::BN().GetSize().cy;
}

void QuickTabs::Add(const char *name, bool make_active)
{
	Tab &t = tabs.Add();

	int cnt = tabs.GetCount() - 1;
	t.name = name;
	t.group = GetFileFolder(name);
	t.id = GetNextId();
	groups.Make(tabs);
	Repos();
	SetCursor(tabs.GetCount() - 1);
}

int QuickTabs::GetTotal()
{
	int j = groups.GetLast();
	return tabs[groups.GetLast()].Right() + groups.GetWidth();
}

void QuickTabs::Repos(bool update)
{
	if(!tabs.GetCount())
		return;

	String g = groups.GetName();

	int j = 0;
	bool first = true;
	for(int i = 0; i < tabs.GetCount(); i++)
	{
		bool v = IsNull(g) ? true : g == tabs[i].group;
		if(v)
		{
			tabs[i].x = first ? 0 : tabs[j].Right();
			j = i;
			first = false;
		}
		else
			tabs[i].x = 0;

		tabs[i].visible = v;
		tabs[i].y = 0;
		tabs[i].cx = GetWidth(i);
		tabs[i].cy = GetHeight(i);
	}
	//if(update)
		sc.SetTotal(GetTotal());
}

int QuickTabs::FindActive(int id)
{
	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].id == id)
			return i;
	return -1;
}

int QuickTabs::GetNext(int n)
{
	for(int i = n + 1; i < tabs.GetCount(); i++)
		if(tabs[i].visible)
			return i;
	return -1;
}

int QuickTabs::GetPrev(int n)
{
	for(int i = n - 1; i >= 0; i--)
		if(tabs[i].visible)
			return i;
	return -1;
}

void QuickTabs::Clear()
{
	tabs.Clear();
	Refresh();
}

QuickTabs& QuickTabs::FileIcons(bool b)
{
	file_icons = b;
	Repos();
	return *this;
}

QuickTabs& QuickTabs::Crosses(bool b)
{
	crosses = b;
	Repos();
	return *this;
}

QuickTabs& QuickTabs::Grouping(bool b)
{
	if(grouping == b)
		return *this;

	grouping = b;
	if(grouping)
		AddFrame(groups);
	else
	{
		groups.SetCurrent(0);
		RemoveFrame(groups);
	}
	Repos();
	return *this;
}

void QuickTabs::LeftDown(Point p, dword keyflags)
{
	mouse = p;

	if(keyflags & K_CTRL)
	{
		highlight = -1;
		Refresh();
		SetCapture();
		oldp = p;
		return;
	}

	isalt = keyflags & K_ALT;
	if(isalt)
		return;

	if(cross != -1)
		Close(cross);
	else if(highlight >= 0)
		SetCursor(highlight);
}

void QuickTabs::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
}

void QuickTabs::MiddleDown(Point p, dword keyflags)
{
	Close(highlight);
}

int QuickTabs::GetTargetTab(Point p)
{
	p.x += sc.GetPos();

	int f = groups.GetFirst();
	int l = groups.GetLast();

	if(tabs[f].visible && p.x < tabs[f].x + tabs[f].cx / 2)
		return f;

	for(int i = l; i >= f; i--)
		if(tabs[i].visible && p.x >= tabs[i].x + tabs[i].cx / 2)
			return i == l ? i + 1 : GetNext(i);
	return -1;
}

void QuickTabs::MouseWheel(Point p, int zdelta, dword keyflags)
{
	sc.AddPos(-zdelta / 4, true);
	MouseMove(p, 0);
}

void QuickTabs::MouseMove(Point p, dword keyflags)
{
	if(HasCapture())
	{
		sc.AddPos(p.x - oldp.x, true);
		oldp = p;
		return;
	}

	p.x += sc.GetPos();

	int h = highlight;
	bool iscross = false;
	bool istab = false;
	for(int i = 0; i < tabs.GetCount(); i++)
	{
		if(tabs[i].HasMouse(p))
		{
			istab = true;
			iscross = crosses ? tabs[i].HasMouseCross(p) : false;
			if(highlight != i || (iscross && cross != i))
			{
				if(iscross)
					cross = i;
				highlight = i;
				Refresh();
				return;
			}
		}
	}

	if(!istab && h >= 0)
	{
		highlight = -1;
		Refresh();
	}

	if(!iscross && cross >= 0)
	{
		cross = -1;
		Refresh();
	}
}

void QuickTabs::MouseLeave()
{
	if(isdrag)
		return;

	highlight = -1;
	cross = -1;
	mouse.Clear();
	Refresh();
}

void QuickTabs::FrameLayout(Rect &r)
{
	LayoutFrameTop(r, this, HEIGHT);
	r.top += 2;
	r.left += 2;
	r.right -= 2;
	r.bottom -= 2;
}

void QuickTabs::FrameAddSize(Size& sz)
{
	sz += 4;
	sz.cy += HEIGHT;
}

void QuickTabs::FramePaint(Draw& w, const Rect& rr)
{
	Rect r = rr;
	r.top += HEIGHT;
	FieldFrame().FramePaint(w, r);
}

void QuickTabs::DragAndDrop(Point p, PasteClip& d)
{
	int c = GetTargetTab(p);
	int tab = isalt ? highlight : active;
	bool sametab = c == tab || c == tab + 1;
	
	bool internal = AcceptInternal<QuickTabs>(d, "tabs");
	
	if(d.IsAccepted())
	{
		if(!sametab && internal)
		{
			int id = tabs[active].id;
			Tab t = tabs[tab];
			tabs.Insert(c, t);
			tabs.Remove(tab + int(c < tab));
			active = FindActive(id);
			isdrag = false;
			groups.Make(tabs);
			Repos();
			return;
		}
		else if(d.IsPaste())
		{
			CancelMode();
			return;
		}
	}
		
	if(c != target)
	{
		target = c;
		Refresh();
	}
}

void QuickTabs::CancelMode()
{
	isdrag = false;
	target = -1;
	Refresh();
}


void QuickTabs::LeftDrag(Point p, dword keyflags)
{
	if(keyflags & K_CTRL)
		return;
	if(highlight < 0)
		return;

	isdrag = true;
	DoDragAndDrop(InternalClip(*this, "tabs"));
}

void QuickTabs::DragEnter()
{
}

void QuickTabs::DragLeave()
{
	target = -1;
	Refresh();
}

void QuickTabs::DragRepeat(Point p)
{
	if(target >= 0)
	{
		int dx = GetDragScroll(this, p, 16).x;
		if(dx != 0)
			sc.AddPos(dx);
	}
}

void QuickTabs::SetCursor(int n)
{
	if(tabs.GetCount() == 0)
		return;

	if(n < 0)
		n = max(0, FindActive(groups.GetActive()));

	if(active == n)
		return;

	active = n;

	if(!groups.IsAll() && tabs[n].group != groups.GetName())
	{
		groups.SetCurrent(tabs[n].group);
		Repos();
	}

	groups.SetActive(tabs[n].id);

	bool scroll = false;
	int cx = tabs[n].x - sc.GetPos();
	if(cx < 0)
	{
		sc.AddPos(cx - 10);
		scroll = true;
	}
	else
	{
		cx = tabs[n].x + tabs[n].cx - GetSize().cx - sc.GetPos();
		if(cx > 0)
		{
			sc.AddPos(cx + 10);
			scroll = true;
		}
	}
	if(scroll)
		MouseMove(mouse, 0);
	UpdateActionRefresh();
}

void QuickTabs::Close(int n)
{
	if(tabs.GetCount() <= 1)
		return;

	int c = FindActive(tabs[n].id);
	int nc = GetNext(c);
	if(nc < 0)
		nc = max(0, GetPrev(c));

	groups.SetActive(tabs[nc].id);

	sc.AddTotal(-tabs[n].cx);
	tabs.Remove(n);
	groups.Make(tabs);
	Repos();
	SetCursor(-1);
}

bool QuickTabs::FindSetFile(const String& fn)
{
	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].name == fn) {
			SetCursor(i);
			return true;
		}
	return false;
}

void QuickTabs::SetAddFile(const String& fn)
{
	if(IsNull(fn))
		return;
	if(FindSetFile(fn))
		return;
	Add(fn, true);
	Refresh();
}

void QuickTabs::RenameFile(const String& fn, const String& nn)
{
	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].name == fn)
		{
			tabs[i].name = nn;
			tabs[i].group = GetFileFolder(nn);
		}
	Repos();
}

void QuickTabs::Set(const QuickTabs& t)
{
	active = t.active;
	cross = -1;
	highlight = -1;
	target = -1;

	tabs.SetCount(t.GetCount());
	for(int i = 0; i < t.GetCount(); i++)
	{
		tabs[i].name = t.tabs[i].name;
		tabs[i].group = t.tabs[i].group;
	}
	groups.Make(tabs);
	groups.SetCurrent(t.groups.GetCurrent());
	Repos();
	sc.Set(t.sc);
}
