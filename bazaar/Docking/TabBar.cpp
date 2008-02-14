#include "TabBar.h"

#define IMAGECLASS TabImg
#define IMAGEFILE <Docking/TabBar.iml>
#include <Draw/iml_source.h>

#define ANIM_SPEED 10
#define ANIM_FRAMES 10

#include "DockCont.h"

// AlignedFrame
void AlignedFrame::FrameLayout(Rect &r)
{
	switch(layout)
	{
		case LEFT:
			LayoutFrameLeft(r, this, framesize);
			break;
		case TOP:
			LayoutFrameTop(r, this, framesize);
			break;
		case RIGHT:
			LayoutFrameRight(r, this, framesize);
			break;
		case BOTTOM:
			LayoutFrameBottom(r, this, framesize);
			break;
	}
	r.top += border;
	r.left += border;
	r.right -= border;
	r.bottom -= border;
}

void AlignedFrame::FrameAddSize(Size& sz)
{
	sz += border * 2;
	switch(layout)
	{
		case LEFT:
			sz.cx += framesize;
			break;
		case TOP:
			sz.cy += framesize;
			break;
		case RIGHT:
			sz.cx -= framesize;
			break;
		case BOTTOM:
			sz.cy -= framesize;
			break;		
	}	
}

void AlignedFrame::FramePaint(Draw& w, const Rect& r)
{
	if(border > 0)
	{
		Rect n = r;
		switch(layout)
		{
			case LEFT:
				n.left += framesize;
				break;
			case TOP:
				n.top += framesize;
				break;
			case RIGHT:
				n.right -= framesize;
				break;
			case BOTTOM:
				n.bottom -= framesize;
				break;		
		}
		FieldFrame().FramePaint(w, n);
	}
	else
		FrameCtrl::FramePaint(w, r);		
}

AlignedFrame& AlignedFrame::SetFrameSize(int sz, bool refresh)
{
	framesize = sz; 
	if (refresh) RefreshParentLayout(); 
	return *this;	
}

void AlignedFrame::Fix(Size& sz)
{
	if(IsVert())
		Swap(sz.cx, sz.cy);
}

void AlignedFrame::Fix(Point& p)
{
	if(IsVert())
		Swap(p.x, p.y);
}

// TabScrollBar
TabScrollBar::TabScrollBar()
{	
	Clear();
}

void TabScrollBar::Clear()
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
	Fix(sz);
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
}

void TabScrollBar::Paint(Draw &w)
{
	if(!ready)
	{
		UpdatePos();
		ready = true;
	}
	Size rsz = GetSize();
	w.DrawRect(rsz, White);
	Point p;
	
	if(total > sz.cx) {
		p = Point(ffloor(pos), 1);
		rsz = Size(fceil(size), 2);
	}
	else {
		p = Point(0, 1);
		rsz = Size(sz.cx, 2);
	}
	Fix(p);
	Fix(rsz);
	w.DrawRect(p.x, p.y, rsz.cx, rsz.cy, Blue);
}

void TabScrollBar::Layout()
{
	UpdatePos(false);
}

void TabScrollBar::LeftDown(Point p, dword keyflags)
{
	SetCapture();
	Fix(p);
	old_pos = new_pos = p.x;
	if(p.x < pos || p.x > pos + size)
		start_pos = size / 2;
	else
		start_pos = tabs(p.x - pos);
	UpdatePos();
	UpdateActionRefresh();	
}

void TabScrollBar::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
	Fix(p);
	old_pos = p.x;
}

void TabScrollBar::MouseMove(Point p, dword keyflags)
{
	if(!HasCapture())
		return;

	Fix(p);
	new_pos = p.x;
	UpdatePos();
	UpdateActionRefresh();
}

void TabScrollBar::MouseWheel(Point p, int zdelta, dword keyflags)
{
	AddPos(-zdelta / 4, true);
	UpdateActionRefresh();
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

bool TabScrollBar::IsScrollable() const
{
	return IsHorz() && total > sz.cx && sz.cx > 0
		|| IsVert() && total > sz.cy && sz.cy > 0;
}

// TabBar
TabCtrl::Style TabBar::leftstyle;
TabCtrl::Style TabBar::rightstyle;
TabCtrl::Style TabBar::bottomstyle;

TabBar::TabBar()
{
	Clear();
	display = NULL;
	crosses = true;
	grouping = true;
	isctrl = false;
	isdrag = false;
	autoscrollhide = true;
	neverempty = 1;

	style[0] = style[1] = style[2] = style[3] = NULL;
	SetAlign(TOP);
	SetFrameSize(GetHeight());	
	BackPaint();
}

void TabBar::CloseAll()
{
	for(int i = tabs.GetCount() - 1; i >= 0; i--)
		if(i != active)
			tabs.Remove(i);

	SyncScrollBar(tabs[0].cx);
	MakeGroups();
	Repos();
	SetCursor(0);
}

int TabBar::GetNextId()
{
	return ++id;
}

void TabBar::ContextMenu(Bar& bar)
{
	bar.Add(tabs.GetCount() > 1, "Close", THISBACK1(Close, highlight));
	bar.Separator();
	int cnt = groups.GetCount();
	for(int i = 0; i < cnt; i++)
	{
		String name = Format("%s (%d)", groups[i].name, groups[i].count);
		Bar::Item &it = i > 0 ? bar.Add(name, THISBACK1(GroupMenu, i))
		                      : bar.Add(name, THISBACK1(DoGrouping, i));
		if(i == group)
			it.Image(TabImg::CHK);
		if(i == 0 && cnt > 1)
			bar.Separator();
	}
	bar.Separator();

	bar.Add("Close all tabs except current", THISBACK(CloseAll));
}

void TabBar::GroupMenu(Bar &bar, int n)
{
	bar.Add("Set active", THISBACK1(DoGrouping, n));
	bar.Add("Close", THISBACK1(DoCloseGroup, n));
}

bool TabBar::Tab::HasMouse(const Point& p) const
{
	return visible && p.x >= x && p.x < x + cx;
}

bool TabBar::Tab::HasMouseCross(const Point& p, int h, int type) const
{
	if(!visible)
		return false;

	Size isz = TabImg::CR0().GetSize();
	int iy = (h - isz.cy) / 2;
	int ix = x + (type ? cx - isz.cx - QT_MARGIN : QT_MARGIN);

	return p.x >= ix && p.x < ix + isz.cx &&
	       p.y >= iy && p.y < iy + isz.cy;
}

int TabBar::FindGroup(const String& g) const
{
	for(int i = 0; i < groups.GetCount(); i++)
		if(groups[i].name == g)
			return i;
	return -1;
}

void TabBar::MakeGroups()
{
	groups[0].count = tabs.GetCount();
	groups[0].first = 0;
	groups[0].last = tabs.GetCount() - 1;

	for(int i = 1; i < groups.GetCount(); i++)
	{
		groups[i].count = 0;
		groups[i].first = 10000000;
		groups[i].last = 0;
	}

	for(int i = 0; i < tabs.GetCount(); i++)
	{
		Tab &tab = tabs[i];
		int n = FindGroup(tab.group);
		ASSERT(n >= 0);
		if (n != 0) {
			if(groups[n].active < 0)
				groups[n].active = tab.id;
			groups[n].count++;
			groups[n].last = i;
	
			if(i < groups[n].first)
				groups[n].first = i;
			if(i > groups[n].last)
				groups[n].last = i;
		}
	}

	int removed = 0;
	for(int i = 1; i < groups.GetCount(); i++)
		if(groups[i].count == 0)
		{
			groups.Remove(i - removed);
			removed++;
		}
	if(group > groups.GetCount() - 1 && group > 0)
		group--;
}

void TabBar::DoGrouping(int n)
{
	group = n;
	Repos();
	SetCursor(-1);
}

void TabBar::DoCloseGroup(int n)
{
	int cnt = groups.GetCount();
	if(cnt <= 0)
		return;
	if (cnt == n)
		--group;

	String group = groups[n].name;

	for(int i = tabs.GetCount() - 1; i >= 0; i--)
	{
		if(group == tabs[i].group && tabs.GetCount() > 1) {
			Value v = tabs[i].data;
			tabs.Remove(i);
			WhenClose(v);
		}
	}
	if(cnt > 1)
		groups.Remove(n);
	MakeGroups();
	Repos();
	SetCursor(-1);
	WhenCursor();
}

void TabBar::NewGroup(const String &name, const Value &data)
{
	Group &g = groups.Add();
	g.name = name;
	g.data = data;
	g.count = 0;
	g.first = 10000000;
	g.last = 0;
	g.active = -1;
}

Value TabBar::AlignValue(int align, const Value &v, const Size &isz)
{
	if (align == TOP) return v;
	if (IsTypeRaw<Image>(v)) {
		switch(align) {
			case AlignedFrame::LEFT: 	 return RotateAntiClockwise((Image)v);
			case AlignedFrame::RIGHT:  return RotateClockwise((Image)v);
			case AlignedFrame::BOTTOM: return MirrorVert((Image)v);
		}
	}
	else if (!IsTypeRaw<Color>(v)) {
		ImageDraw w(isz.cx, isz.cy);
		ChPaint(w, isz, v);
		ImageBuffer img;
		switch(align) {
			case AlignedFrame::LEFT: 
				img = RotateAntiClockwise((Image)w);
				img.SetHotSpot(Point(1, 5));
				img.Set2ndSpot(Point(0, isz.cx/2));
				break;
			case AlignedFrame::RIGHT: 
				img = RotateClockwise((Image)w);
				img.SetHotSpot(Point(isz.cy-10, isz.cx-10));
				img.Set2ndSpot(Point(isz.cy/2, isz.cx/2));
				break;
			case AlignedFrame::BOTTOM:
				img = MirrorVert((Image)w);
				img.SetHotSpot(Point(10, 10));
				img.Set2ndSpot(Point(isz.cx/2, isz.cy/2));
				break;

		}
		return (Image)img;
	}	
	return v;
}

void TabBar::TabCenter(Point &p, const Size &sz, int h)
{
	if (IsHorz())
		p.y += (sz.cy - h) / 2;
	else
		p.x += (sz.cx - h) / 2 * ((GetAlign() == LEFT) ? 1 : -1);	
}

void TabBar::DrawTabData(Draw& w, Point p, const Size &sz, const Value& q, const Font &font, Color ink, dword style)
{
	WString txt;
	Font f = font;
	Color i = ink;
	int a = TextAngle();

	if(IsType<AttrText>(q)) {
		const AttrText& t = ValueTo<AttrText>(q);
		txt = t.text;
		if(!IsNull(t.font))
			f = t.font;
		i = IsNull(t.ink) ? ink : t.ink;
	}
	else
		txt = IsString(q) ? q : StdConvert().Format(q);
	TabCenterText(p, sz, font);
	w.DrawText(p.x, p.y, TextAngle(), txt, f, i);	
}

void TabBar::DrawTab(Draw &w, const TabCtrl::Style &s, const Size &sz, int n)
{
	TabBar::Tab &t = tabs[n];
	int cnt = tabs.GetCount();
	Size tsz, isz(0, 0);
	Point p;
	int align = GetAlign();
	bool ac = n == active;
	bool hl = n == highlight;

	int ndx = !IsEnabled() ? CTRL_DISABLED :
		       ac ? CTRL_PRESSED :
		       hl ? CTRL_HOT : CTRL_NORMAL;

	const Value& sv = (cnt == 1 ? s.both : n == 0 ? s.first : n == cnt - 1 ? s.last : s.normal)[ndx];

	if (ac) {
		p = Point(t.x - sc.GetPos() + s.margin - s.extendleft - s.sel.left, 0);
		tsz = Size(t.cx + s.sel.right + s.sel.left, t.cy + s.sel.bottom);
		if (align == LEFT)  { 
			p.x += s.sel.left;
			tsz.cx += s.sel.left;
		}
	}
	else {
		p = Point(t.x - sc.GetPos() + s.margin - s.extendleft, (IsBR() ? s.sel.bottom : s.sel.top));
		tsz = Size(t.cx + s.sel.right + s.sel.left, t.cy - s.sel.top);
	}
	
	if (IsVert())
		ChPaint(w, p.y, p.x, tsz.cy, tsz.cx, sv);
	else 
		ChPaint(w, p.x, p.y, tsz.cx, tsz.cy, sv);
	
	if(crosses && cnt > neverempty) {
		Point cp;
		const Image &cimg = TabImg::CR0(); // Use style?
		isz = cimg.GetSize();		
		Fix(isz);
		if (align == LEFT)
			cp.x = p.x + QT_MARGIN;
		else
			cp.x = p.x + tsz.cx - isz.cx - QT_MARGIN;
		cp.y = p.y + (tsz.cy - isz.cy) / 2;
		Fix(cp);
		w.DrawImage(cp.x, cp.y, (ac || hl) ? (cross == n ? TabImg::CR2 : ac ? TabImg::CR1 : TabImg::CR0) : TabImg::CR0);
		isz.cx += 2;
	}

	p.x += QT_MARGIN;
	tsz.cx -= QT_MARGIN * 2;
	switch (align) {
	case BOTTOM:
		if (ac)	p.y -= s.sel.top; break;
	case RIGHT: p.y = sz.cx - p.y; break;
	case LEFT: p.x += tsz.cx;
	}
	tsz.cx -= isz.cx;		
	Fix(p);
	Fix(tsz);
	if (display)
		display->Paint(w, Rect(p, tsz),	t.data, Black(),	
		/* For SVN version			s.text_color[ndx], ndx ); */ 
		Null, ndx);
	else {
		DrawTabData(w, p, tsz,
			t.data, s.font,
			Black(), ndx);
	}
// For SVN version			s.text_color[ndx], ndx );		
}
void TabBar::Paint(Draw &w)
{
	const TabCtrl::Style &s = *style[GetAlign()];
	Size sz = GetSize();
	
	w.DrawRect(sz, SColorFace());

	IsVert() ? 
		w.DrawRect((GetAlign() == LEFT) ? sz.cx -1 : 1, 0, 1, sz.cy, Color(128, 128, 128)) :
		w.DrawRect(0, (GetAlign() == TOP) ? sz.cy - 1 : 1, sz.cx, 1, Color(128, 128, 128));	

	if (!tabs.GetCount()) return;
	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].visible && i != active)
			DrawTab(w, s, sz, i);
	if(active >= 0)
		DrawTab(w, s, sz, active);

	if(target >= 0)
	{
		int dragtab = isctrl ? highlight : active;
		if(target != dragtab && target != dragtab + 1)
		{
			int last = GetLast();
			int first = GetFirst();
			int x = (target == last + 1 ? tabs[last].Right() : tabs[target].x)
			        - sc.GetPos() - (target <= first ? 1 : 2)
			        + s.margin - (target > 0 ? s.extendleft : 0);

			int y = s.sel.top;
			Color c(255, 0, 0);
			int cy = sz.cy - y;
			if (IsHorz()) {
				w.DrawRect(x + 1, y, 2, cy, c);
				w.DrawRect(x, y, 4, 1, c);
				w.DrawRect(x, y + cy - 1, 4, 1, c);			
			}
			else{
				w.DrawRect(y, x + 1, cy, 2, c);
				w.DrawRect(y, x, 1, 4, c);
				w.DrawRect(y + cy - 1, x, 1, 4, c);			
			}
		}
	}
}

void TabBar::Scroll()
{
	if (IsVisible()) scroller.Scroll(*this, IsVert() ? Point(0, sc.GetPos()) : Point(sc.GetPos(), 0));
}

int TabBar::GetWidth(int n)
{
	Tab &t = tabs[n];
	t.tsz = GetStdSize(t.data);
	return QT_MARGIN * 2 + t.tsz.cx + (QT_SPACE + TabImg::CR0().GetSize().cx) * crosses;
}

Size TabBar::GetStdSize(Value &q)
{
	if (display)
		return display->GetStdSize(q);
	else if (q.GetType() == STRING_V)
		return GetTextSize((String)q, StdFont());
	else if (q.GetType() == WSTRING_V)
		return GetTextSize((WString)q, StdFont());	
	return GetTextSize("A Tab", StdFont());	
}

TabBar& TabBar::Add(const Value &data, String group, bool make_active)
{
	return Insert(tabs.GetCount(), data, group, make_active);
}

TabBar& TabBar::Insert(int ix, const Value &data, String group, bool make_active)
{
	Tab &t = tabs.Insert(ix);
	
	int cnt = tabs.GetCount() - 1;
	t.data = data;
	t.id = GetNextId();
	
	int g = 0;
	if (!group.IsEmpty()) {
		g = FindGroup(group);
		if (g < 0) {
			NewGroup(group);
			g = groups.GetCount() - 1;
		}
	}		
	t.group = groups[g].name;
	MakeGroups();	
	Repos();
	active = -1;
	SetCursor(minmax(ix, 0, cnt));	
	return *this;	
}

int TabBar::GetWidth()const
{
	if (!tabs.GetCount()) return 0;
	int j = GetLast();
	return tabs[GetLast()].Right() + style[GetAlign()]->margin * 2;
}

int TabBar::GetStyleHeight(const TabCtrl::Style &s)
{
	return s.tabheight + s.sel.top /*+ style->sel.bottom */ + QT_SBSEPARATOR;
}

void TabBar::Repos()
{
	if(!tabs.GetCount())
		return;

	String g = GetGroupName();

	int j;
	bool first = true;
	j = 0;
	for(int i = 0; i < tabs.GetCount(); i++)
		j = TabPos(g, first, i, j);
	SyncScrollBar(GetWidth());
}

int TabBar::TabPos(const String &g, bool &first, int i, int j)
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
	tabs[i].cy = style[1]->tabheight + style[1]->sel.top;
	return j;	
}

void TabBar::SyncScrollBar(int total)
{
	if (total > 0)
		sc.SetTotal(total);			
	if (autoscrollhide) {
		int h = GetHeight();
		bool v = GetFrameSize() > h;
		bool nv = sc.IsScrollable();
		sc.Show(nv);
		if (v != nv) 
			SetFrameSize((nv ? sc.GetFrameSize() : 0) + h, false);		
	}
	else
		sc.Show();
}

int TabBar::FindId(int id) const
{
	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].id == id)
			return i;
	return -1;
}

int TabBar::GetNext(int n) const
{
	for(int i = n + 1; i < tabs.GetCount(); i++)
		if(tabs[i].visible)
			return i;
	return -1;
}

int TabBar::GetPrev(int n) const
{
	for(int i = n - 1; i >= 0; i--)
		if(tabs[i].visible)
			return i;
	return -1;
}

void TabBar::Clear()
{
	highlight = -1;
	active = -1;
	target = -1;
	id = -1;	
	tabs.Clear();
	groups.Clear();
	NewGroup("All");
	group = 0;
	Refresh();
}

TabBar& TabBar::Crosses(bool b)
{
	crosses = b;
	Repos();
	return *this;
}

TabBar& TabBar::Grouping(bool b)
{
	grouping = b;
	return *this;
}

TabBar& TabBar::AutoScrollHide(bool b)
{
	autoscrollhide = b;
	SyncScrollBar(GetWidth());
	return *this;
}

void TabBar::FrameSet()
{
	int al = GetAlign();
	Ctrl::ClearFrames();
	sc.Clear();
	sc.SetFrameSize(QT_SBHEIGHT).SetAlign((al >= 2) ? al - 2 : al + 2);
	sc <<= THISBACK(Scroll);
	if (!sc.IsChild())
		AddFrame(sc);

	if (!style[al]) {
		switch(al) {
			case LEFT: 
				style[al] = &StyleLeft(); 
				break;
			case TOP: 
				style[al] = &StyleDefault(); 
				break;
			case RIGHT: 
				style[al] = &StyleRight(); 
				break;
			case BOTTOM: 
				style[al] = &StyleBottom(); 
				break;
		}
	}			
	SyncScrollBar(0);
}

void TabBar::FrameLayout(Rect& r)
{
	if (autoscrollhide && tabs.GetCount()) 
		SyncScrollBar(-1); 
	AlignedFrame::FrameLayout(r);	
}

int TabBar::Find(const Value &v) const
{
	for (int i = 0; i < tabs.GetCount(); i++)
		if (tabs[i].data == v)
			return i;
	return -1;
}

void TabBar::Set(int n, const Value &data, const String &group)
{
	ASSERT(n >= 0 && n < tabs.GetCount());
	tabs[n].data = data;
	if (!IsNull(data))
		SetTabGroup(n, group);
	else
		Repos();
}

void TabBar::LeftDown(Point p, dword keyflags)
{
	if(keyflags & K_SHIFT)
	{
		highlight = -1;
		Refresh();
		SetCapture();
		Fix(p);
		oldp = p;
		return;
	}

	isctrl = keyflags & K_CTRL;
	if(isctrl)
		return;

	if(cross != -1) {
		Value v = tabs[cross].data;
		Close(cross);
		WhenClose(v);
	}
	else if(highlight >= 0) {
		SetCursor(highlight);
		WhenCursor();
	}
}

void TabBar::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
}

void TabBar::RightDown(Point p, dword keyflags)
{
	MenuBar::Execute(THISBACK(ContextMenu), GetMousePos());
}

void TabBar::MiddleDown(Point p, dword keyflags)
{
	Close(highlight);
}

void TabBar::MiddleUp(Point p, dword keyflags)
{
}

int TabBar::GetTargetTab(Point p)
{
	p.x += sc.GetPos();

	int f = GetFirst();
	int l = GetLast();

	if(tabs[f].visible && p.x < tabs[f].x + tabs[f].cx / 2)
		return f;

	for(int i = l; i >= f; i--)
		if(tabs[i].visible && p.x >= tabs[i].x + tabs[i].cx / 2)
			return i == l ? i + 1 : GetNext(i);
//		if(tabs[i].visible && p.x >= tabs[i].x && p.x <= tabs[i].x + tabs[i].cx)
//			return i;
	return -1;
}

void TabBar::MouseWheel(Point p, int zdelta, dword keyflags)
{
	sc.AddPos(-zdelta / 4, true);
	Scroll();
	MouseMove(p, 0);
}

void TabBar::MouseMove(Point p, dword keyflags)
{
	Fix(p);
	if(HasCapture())
	{
		sc.AddPos(p.x - oldp.x, true);
		oldp = p;
		return;
	}

	p.x += sc.GetPos() - style[1]->margin;
	Size sz = GetSize();
	Fix(sz);

	int h = highlight;
	bool iscross = false;
	bool istab = false;
	for(int i = 0; i < tabs.GetCount(); i++)
	{
		Point np(p.x + (i > 0 ? style[1]->extendleft : 0), p.y);
		if(tabs[i].HasMouse(np))
		{
			istab = true;
			int h = sz.cy + (active == i ? 0 : style[1]->sel.top);
			iscross = crosses ? tabs[i].HasMouseCross(np, h, GetAlign()) : false;
			if(highlight != i || (iscross && cross != i))
			{
				cross = iscross ? i : -1;
				highlight = i;
				WhenHighlight();
				Refresh();
				return;
			}
		}
	}

	if(!istab && h >= 0)
	{
		highlight = -1;
		WhenHighlight();
		Refresh();
	}

	if(!iscross && cross >= 0)
	{
		cross = -1;
		Refresh();
	}
}

void TabBar::MouseLeave()
{
	if(isdrag)
		return;
	highlight = -1;
	cross = -1;
	Refresh();
}

void TabBar::DragAndDrop(Point p, PasteClip& d)
{
	Fix(p);
	int c = GetTargetTab(p);
	int tab = isctrl ? highlight : active;

	if (&GetInternal<TabBar>(d) != this) return;

	bool sametab = c == tab || c == tab + 1;
	bool internal = AcceptInternal<TabBar>(d, "tabs");

	if(!sametab && d.IsAccepted())
	{
		if(internal)
		{
			int id = tabs[active].id;
			Tab t = tabs[tab];
			tabs.Insert(c, t);
			tabs.Remove(tab + int(c < tab));
			active = FindId(id);
			isdrag = false;
			MakeGroups();
			Repos();
			return;
		}
		else if(d.IsPaste())
		{
			CancelMode();
			return;
		}
	}
	else
	{
		//d.Reject();
		//unfortunately after Reject DragLeave stops working until d is accepted
	}

	if(c != target)
	{
		target = c;
		Refresh();
	}
}

void TabBar::CancelMode()
{
	isdrag = false;
	target = -1;
	Refresh();
}

void TabBar::LeftDrag(Point p, dword keyflags)
{
	if(keyflags & K_SHIFT)
		return;
	if(highlight < 0)
		return;

	isdrag = true;
	DoDragAndDrop(InternalClip(*this, "tabs"));
}

void TabBar::DragEnter()
{
}

void TabBar::DragLeave()
{
	target = -1;
	Refresh();
}

void TabBar::DragRepeat(Point p)
{
	if(target >= 0)
	{
		int dx = GetDragScroll(this, p, 16).x;
		if(dx != 0)
			sc.AddPos(dx);
	}
}

void TabBar::SetCursor(int n)
{
	if(tabs.GetCount() == 0)
		return;

	if(n < 0)
	{
		n = max(0, FindId(GetGroupActive()));
		active = -1;
	}

	bool is_all = IsGroupAll();
	bool same_group = tabs[n].group == GetGroupName();

	if((same_group || is_all) && active == n)
		return;

	active = n;

	if(!is_all && !same_group)
	{
		SetGroup(tabs[n].group);
		Repos();
	}

	SetGroupActive(tabs[n].id);

	int cx = tabs[n].x - sc.GetPos();
	if(cx < 0)
		sc.AddPos(cx - 10);
	else
	{
		Size sz = Ctrl::GetSize();
		Fix(sz);
		cx = tabs[n].x + tabs[n].cx - sz.cx - sc.GetPos();
		if(cx > 0)
			sc.AddPos(cx + 10);
	}
	if(HasMouse())
		MouseMove(GetMouseViewPos(), 0);

	UpdateActionRefresh();
}

void TabBar::SetTabGroup(int n, const String &group)
{
	ASSERT(n >= 0 && n < tabs.GetCount());
	int g = FindGroup(group);
	if (g <= 0) 
		NewGroup(group);
	else if (groups[g].active == tabs[n].id)
		SetGroupActive(tabs[n].id);
	tabs[n].group = group;
	MakeGroups();
	Repos();
}

void TabBar::Close(int n)
{
	if(tabs.GetCount() <= neverempty)
		return;

	if(n == active)
	{
		int c = FindId(tabs[n].id);
		int nc = GetNext(c);
		if(nc < 0)
			nc = max(0, GetPrev(c));
		SetGroupActive(tabs[nc].id);
	}
	sc.AddTotal(-tabs[n].cx);
	tabs.Remove(n);
	MakeGroups();
	Repos();
	SetCursor(-1);
}

const TabCtrl::Style& TabBar::AlignStyle(int align, TabCtrl::Style &s)
{
	Size sz(30, GetStyleHeight(s));
	for (int i = 0; i < 4; i++) s.first[i] =  	AlignValue(align, s.first[i], sz);
	for (int i = 0; i < 4; i++) s.last[i] =  	AlignValue(align, s.last[i], sz);
	for (int i = 0; i < 4; i++) s.normal[i] =  	AlignValue(align, s.normal[i], sz);
	for (int i = 0; i < 4; i++) s.both[i] =  	AlignValue(align, s.both[i], sz);
	return s;
}

const TabCtrl::Style& TabBar::StyleLeft()
{
	leftstyle = StyleDefault();
	return AlignStyle(AlignedFrame::LEFT, leftstyle);
}

const TabCtrl::Style& TabBar::StyleRight()
{
	rightstyle = StyleDefault();
	return AlignStyle(AlignedFrame::RIGHT, rightstyle);
}

const TabCtrl::Style& TabBar::StyleBottom()
{
	bottomstyle = StyleDefault();
	return AlignStyle(AlignedFrame::BOTTOM, bottomstyle);	
}

void TabBar::Serialize(Stream& s)
{
	int version = 0x00;
	int cnt;
	if (s.IsLoading()) Clear();
	
	s / version / group / highlight / active;		
	
	if (s.IsLoading()) {
		cnt = groups.GetCount();
		s / cnt;
		for (int i = 1; i < groups.GetCount(); i++) {
			Group &g = groups[i];
			s % g.data / g.active;
		}
		cnt = tabs.GetCount();
		s / cnt;
		for (int i = 0; i < tabs.GetCount(); i++) {
			Tab &t = tabs[i];
			int g = FindGroup(t.group);
			s % t.data / t.id / g;
		}
	}
	else {
		s / cnt;
		groups.SetCount(cnt);
		for (int i = 1; i < cnt; i++) {
			Group &g = groups[i];
			s % g.data / g.active;
		}
		s / cnt;
		tabs.SetCount(cnt);
		for (int i = 0; i < tabs.GetCount(); i++) {
			int g;
			Tab &t = tabs[i];
			s % t.data / t.id / g;
			t.group = groups[g].name;
		}
		MakeGroups();
		Repos();
	}
}

// DockTabBar
int AutoHideBar::autohide_timeout = 1000;

void DockTabBar::FrameLayout(Rect& r)
{
	if (IsAutoHide()) return Hide();
	Show();
	TabBar::FrameLayout(r);	
}

void DockTabBar::FrameAddSize(Size& sz)
{
	if (!IsAutoHide()) 
		TabBar::AddFrameSize(sz);
}

void DockTabBar::DrawTabData(Draw& w, Point p, const Size &sz, const Value& q, const Font &font, Color ink, dword style)
{
	DockableCtrl *d;
	if (IsTypeRaw<DockCont *>(q))
		d = &ValueTo<DockCont *>(q)->GetCurrent();
	else {
		ASSERT(IsTypeRaw<DockableCtrl *>(q));
		d = ValueTo<DockableCtrl *>(q);
	}

	TabCenterText(p, sz, font);
	if(icons)
	{
		const Image &icon = d->GetIcon();
		int al = GetAlign();
		Point ip = p;
		Size isz = icon.GetSize();
		if (al == RIGHT)
			ip.x -= font.GetHeight();
		else if (al == LEFT)
			ip.y -= isz.cy;
		if (IsVert()) {
			w.DrawImage(ip.x-1, ip.y, icon);
			p.y += (isz.cy + QT_SPACEICON) * ((al == LEFT) ? -1 : 1);
		}
		else {			
			w.DrawImage(ip.x, ip.y-2, icon);
			p.x += isz.cx + QT_SPACEICON;
		}	
	}
	w.DrawText(p.x, p.y, TextAngle(), d->GetTitle(), font, ink);
}

Size DockTabBar::GetStdSize(Value &q)
{
	DockableCtrl *d;
	if (IsTypeRaw<DockCont *>(q))
		d = &ValueTo<DockCont *>(q)->GetCurrent();
	else {
		ASSERT(IsTypeRaw<DockableCtrl *>(q));
		d = ValueTo<DockableCtrl *>(q);
	}
	Value v = d->GetTitle();	
	Size sz = TabBar::GetStdSize(v);
	sz.cx += (IsVert() ? d->GetIcon().GetHeight() : d->GetIcon().GetWidth()) + QT_SPACEICON;
	return sz;
}

void DockTabBar::LeftDrag(Point p, dword keyflags)
{
	if (keyflags & K_CTRL || keyflags & K_SHIFT)
		return TabBar::LeftDrag(p, keyflags &= ~K_SHIFT);
	if (GetHighlight() >= 0)
		WhenDrag(GetHighlight());	
}

DockTabBar::DockTabBar()
{
	autohide = -1; 
	icons = true;
}

// AutoHide bar
void AutoHideBar::MouseEnter(Point p, dword keyflags)
{
	if (showctrl && showctrl->IsOpen())
		KillTimeCallback(TIMEID_HIDE_TIMEOUT);			
	DockTabBar::MouseEnter(p, keyflags);
}

void AutoHideBar::MouseLeave()
{
	if (showctrl && showctrl->IsOpen())
		KillSetTimeCallback(autohide_timeout, THISBACK1(HideAnimate, showctrl), TIMEID_HIDE_TIMEOUT);				
	DockTabBar::MouseLeave();
}

void AutoHideBar::AddCtrl(DockCont &c, const String &group)
{ 
	TabBar::Add(RawToValue<DockCont *>(&c), group); 
	if (GetCount() == autohide+1)
		RefreshParentLayout();		
}

void AutoHideBar::RemoveCtrl(DockCont &c, int ix)
{
	if (c.IsOpen()) c.Close();

	if (showctrl == &c) {
		if (popup.IsOpen())
			popup.Close();
		showctrl->Remove();
		//KillTimeCallback(TIMEID_ANIMATE);
		KillTimeCallback(TIMEID_HIDE_TIMEOUT);			
		showctrl = NULL;				
	}
	if (ix >= 0) DockTabBar::Close(ix);
	if (GetCount() == autohide)
		RefreshParentLayout();	
}

void AutoHideBar::RemoveCtrl(int ix)
{
	if (ix >= 0) {
		DockCont *c = GetCtrl(ix);
		ASSERT(c);
		RemoveCtrl(*c, ix);
	}		
}

void AutoHideBar::TabHighlight()
{
	Ctrl *c = NULL;
	int ix = TabBar::GetHighlight();
	if (ix >= 0)
		c = GetCtrl(ix);
	if (!c || showctrl == c) 
		return;
	else if (showctrl) {
		if (c) {
			if (popup.IsOpen())
				popup.Close();
			showctrl->Remove();
			showctrl = NULL;
		}
		else 
			HideAnimate(showctrl);
	}
	if (c) {
		SetCursor(ix);
		ShowAnimate(c);
	}
}

void AutoHideBar::TabClose(Value v)
{
	RemoveCtrl(*(ValueTo<DockCont *>(v)), -1);
	if (GetCount() == autohide-1)
		RefreshParentLayout();	
}

void AutoHideBar::ShowAnimate(Ctrl *c)
{
	if (c->IsOpen())
		c->Close();
	if (popup.IsPopUp())
		popup.Close();
	Rect r = Ctrl::GetScreenRect();
	Size fr = c->AddFrameSize(Size(0, 0));
	Size sz = c->GetStdSize();// + fr;
	switch (GetAlign()) {
	 	case DockTabBar::LEFT: 
	 		sz.cy = r.Height();
	 		r.left = r.right;
	 		r.right += 5; // Popups don't work on Linux if width or hieght is 0
	 		break;
	 	case DockTabBar::TOP:
	 		sz.cx = r.Width();
	 		r.top = r.bottom;
	 		r.bottom += 5; // Popups don't work on Linux if width or hieght is 0
	 		break;
	 	case DockTabBar::RIGHT: 
	 		sz.cy = r.Height();
	 		r.right = r.left;
	 		r.left -= 5; // Popups don't work on Linux if width or hieght is 0
	 		break;
	 	case DockTabBar::BOTTOM:
	 		sz.cx = r.Width();
	 		r.bottom = r.top;
	 		r.top -= 5; // Popups don't work on Linux if width or hieght is 0
	 		break;
	};
	// use _NET_FRAME_EXTENTS to properly set ctrl size if popup has additional frame added by window manager?
	c->SetRect(sz);
	popup << *c;
	c->Show();
	popup.SetRect(r);
	popup.PopUp(GetParent(), false, true, false, false);
	AdjustSize(r, c->GetStdSize());
	showctrl = c;
	Animate(popup, r, GUIEFFECT_SLIDE);
}

void AutoHideBar::HideAnimate(Ctrl *c)
{
	ASSERT(showctrl);
	Rect r = popup.GetRect();
	AdjustSize(r, -r.GetSize());
	Animate(popup, r, GUIEFFECT_SLIDE);
	popup.Close();
	showctrl->Remove();
	showctrl = NULL;
	DockTabBar::SetCursor(-1);
}
/*
void AutoHideBar::AnimateSizeMinus(Size sz)
{
	Rect r = popup.GetScreenRect();
	AdjustSize(r, sz);
	if ((IsVert() && r.Width() <= 12) || (!IsVert() && r.Height() <= 12)) {
		popup.Close();
		showctrl->Remove();
		showctrl = NULL;
		Ctrl::KillTimeCallback(TIMEID_ANIMATE);
	}
	else
		popup.SetRect(r);
}
*/

void AutoHideBar::AdjustSize(Rect &r, const Size &sz)
{
	switch (DockTabBar::GetAlign()) {
	 	case DockTabBar::LEFT: 
	 		r.right += sz.cx;
	 		break;
	 	case DockTabBar::TOP: 
	 		r.bottom += sz.cy;
	 		break;
	 	case DockTabBar::RIGHT:
	 		r.left -= sz.cx;
	 		break;
	 	case DockTabBar::BOTTOM:
	 		r.top -= sz.cy;
	 		break;
	};		
}

int AutoHideBar::FindCtrl(DockCont &c) const
{
	if (&c == showctrl) return GetCursor();
	for (int i = 0; i < GetCount(); i++) {
		DockCont *v = ValueTo<DockCont *>(Get(i));
		if (v == &c) return i;
	}
	return -1;
}

AutoHideBar::AutoHideBar()
{
	showctrl = NULL;
	AutoHideMin(0).CanEmpty();
	popup.WhenEnter = THISBACK2(MouseEnter, Point(0, 0), 0);
	popup.WhenLeave = THISBACK(MouseLeave);
	popup.AddFrame(FieldFrame());
	DockTabBar::WhenHighlight = THISBACK(TabHighlight);
	TabBar::WhenClose = THISBACK(TabClose);
}

void AutoHideBar::HidePopup::ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags)
{
	if (event == MOUSELEAVE)
		WhenLeave();
	else if (event != MOUSEMOVE) 
		WhenEnter();
}
