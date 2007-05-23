#include <ide/QuickTabs/QuickTabs.h>

#define IMAGECLASS Img
#define IMAGEFILE <ide/QuickTabs/QuickTabs.iml>
#include <Draw/iml_source.h>

TabScrollBar::TabScrollBar()
{
	total = 0;
	pos = 0;
	ps = 0;
	start_pos = 0;
	new_pos = 0;
	old_pos = 0;
	ready = false;
	
	//AddFrame(ls.Width(15));
	//AddFrame(rs.Width(15));
	ls.Color(Green);
	rs.Color(Green);
}

void TabScrollBar::UpdatePos(bool update)
{
	sz = GetSize();
	size = total > 0 ? (sz.cx * sz.cx) / total : 0;
	if(update)
		pos = new_pos - start_pos;
	if(pos < 0)
		pos = 0;
	if(pos + size > sz.cx)
		pos = sz.cx - size;
	
	ps = total > sz.cx ? (sz.cx > 0 ? pos * total / sz.cx : 0) : 0;
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
		w.DrawRect(pos, 1, size, 2, Blue);
	else
		w.DrawRect(0, 1, sz.cx, 2, /*Color(170,170,204)*/Blue);
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
		start_pos = abs(p.x - pos);
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

int TabScrollBar::GetPos() const
{
	return ps;
}

void TabScrollBar::SetPos(int p, bool dontscale)
{
	pos = total > 0 ? dontscale ? p : iscale(p, sz.cx, total) : 0;
	UpdatePos(false);
	Refresh();
}

void TabScrollBar::AddPos(int p, bool dontscale)
{
	pos += dontscale ? p : iscale(p, sz.cx, total);
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
	//AddFrame(list);
	AddFrame(sc.Height(4));
	sc.WhenScroll = THISBACK(Scroll);
	highlight = -1;
	active = -1;
	cross = -1;
	target = -1;
	middle_button = false;
	BackPaint();
}

bool QuickTabs::Tab::HasMouse(Point &p)
{
	return p.x > x && p.x < x + cx;
}

bool QuickTabs::Tab::HasMouseCross(Point &p)
{
	Size isz = Img::CR0().GetSize();
	int iy = cy / 2;
	int ix = x + cx - 20 + isz.cx / 2;
	
	int icx = isz.cx / 2;
	int icy = isz.cy / 2;
	
	return p.x > ix - icx && p.x < ix + icx &&
	       p.y > iy - icy && p.y < iy + icy;
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
	
	bool isactive = i == active || i == highlight;
	
	if(isactive)
	{
		int sw = Img::LH().GetSize().cx;
		int ew = Img::RH().GetSize().cx;

		w.DrawImage(x, 0, Img::LH());
		w.DrawImage(x + sw, 0, t.cx - sw - ew, h, Img::BH());
		w.DrawImage(x + t.cx - ew, 0, Img::RH());
		if(i == highlight && i != active)
			w.DrawRect(x, t.cy - 1, t.cx, 1, Color(128, 128, 128));
	}
	else
	{
		int sw = Img::LN().GetSize().cx;
		int ew = Img::RN().GetSize().cx;
		
		w.DrawImage(x, 0, Img::LN());
		w.DrawImage(x + sw, 0, t.cx - sw - ew, h, Img::BN());
		w.DrawImage(x + t.cx - ew, 0, Img::RN());
	}
	if(tabs.GetCount() > 1)	
		w.DrawImage(x + t.cx - isz.cx - MARGIN, (h - isz.cy) / 2, isactive ? (cross == i ? Img::CR2() : Img::CR1()) : Img::CR0());
	
	String fn = GetFileName(t.name);
	DrawFileName(w, x + MARGIN, (h - t.tsz.cy) / 2 + 1,
	             t.tsz.cx, t.tsz.cy, fn.ToWString(),
	             false, StdFont(), SColorText, LtBlue, Null, Null, false);
}


void QuickTabs::Paint(Draw &w)
{
	Size sz = GetSize();
	w.DrawRect(sz, SColorFace());
	w.DrawRect(0, sz.cy - 1, sz.cx, 1, Color(128, 128, 128));

	for(int i = 0; i < tabs.GetCount(); i++)
		DrawTab(w, i);
	
	if(target >= 0)
	{
		bool last = target == tabs.GetCount();
		bool first = target == 0;
		
		int x = (last ? tabs[tabs.GetCount() - 1].Right() : tabs[target].x) - sc.GetPos() - (first ? 1 : 2);
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
	return MARGIN * 2 + t.tsz.cx + SPACE + Img::CR0().GetSize().cx;
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
	t.x = cnt == 0 ? 0 : tabs[cnt - 1].Right();
	t.y = 0;
	t.cx = GetWidth(cnt);
	t.cy = GetHeight(cnt);
	if(make_active)
		active = cnt;
	sc.AddTotal(t.cx);
	sc.GoEnd();
}

int QuickTabs::GetTotal()
{
	return tabs[tabs.GetCount() - 1].Right();
}

void QuickTabs::Repos(bool update)
{
	if(!tabs.GetCount())
		return;
	
	for(int i = 0; i < tabs.GetCount(); i++)
	{
		tabs[i].x = i == 0 ? 0 : tabs[i - 1].Right();
		tabs[i].y = 0;
		if(update)
		{
			tabs[i].cx = GetWidth(i);
			tabs[i].cy = GetHeight(i);
		}
	}
	sc.SetTotal(GetTotal());
}

void QuickTabs::Clear()
{
	tabs.Clear();
	Refresh();
}

void QuickTabs::LeftDown(Point p, dword keyflags)
{
	if(!middle_button)
	{
		if(keyflags & K_CTRL)
		{
			highlight = -1;
			Refresh();
			SetCapture();
			oldp = p;
			return;
		}
	}
	else
	{
		middle_button = false;
		cross = highlight;
	}
	
	mouse = p;
	
	if(cross != -1 && tabs.GetCount() > 1)
	{
			
		Close(cross);
		if(cross == active)
		{
			active = -1;
			SetCursor(cross);
		}
		else
		{
			if(cross < active)
				active = max(active - 1, 0);
			Refresh();
		}
	}
	else
		SetCursor(highlight);
}

void QuickTabs::LeftUp(Point p, dword keys)
{
	ReleaseCapture();
}

int QuickTabs::GetTargetTab(Point p)
{
	p.x += sc.GetPos();
	
	if(p.x < tabs[0].x + tabs[0].cx / 2)
		return 0;
	
	for(int i = tabs.GetCount() - 1; i >= 0; i--)
		if(p.x > tabs[i].x + tabs[i].cx / 2)
			return i + 1;
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
	
	bool iscross = false;
	for(int i = 0; i < tabs.GetCount(); i++)
	{
		if(tabs[i].HasMouse(p))
		{
			iscross = tabs[i].HasMouseCross(p);
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
	if(!iscross && cross >= 0)
	{
		cross = -1;
		Refresh();
	}
}

void QuickTabs::MouseLeave()
{
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

#ifdef PLATFORM_WIN32
LRESULT QuickTabs::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_MBUTTONDOWN)
		middle_button = true;
	return Ctrl::WindowProc(message, wParam, lParam);
}
#endif

void QuickTabs::DragAndDrop(Point p, PasteClip& d)
{
	int c = GetTargetTab(p);
	bool sametab = c == active || c == active + 1;
	
	if(!sametab && AcceptInternal<QuickTabs>(d, "tabs") && d.IsAccepted())
	{
		Tab t = tabs[active];
		tabs.Insert(c, t);
		tabs.Remove(active + int(c < active));
		active = c - int(c > active);
		Repos(true);
		return;
	}
	
	if(c != target)
	{
		target = c;	
		Refresh();	
	}
}

void QuickTabs::LeftDrag(Point p, dword keyflags)
{
	if(keyflags & K_CTRL)
		return;
	
	DoDragAndDrop(InternalClip(*this, "tabs"));
}

void QuickTabs::DragEnter(Point p, PasteClip& d)
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
		sc.AddPos(GetDragScroll(this, p, 16).x);
}

void QuickTabs::SetCursor(int n)
{
	int last = tabs.GetCount() - 1;
	
	if(last < 0)
		return;
	if(n > last)
		n = last;
	
	if(active == n)
		return;
		
	active = n;
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
	sc.AddTotal(-tabs[n].cx);
	tabs.Remove(n);
	Repos(true);
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
			tabs[i].name = nn;	
	Repos(true);
}

void QuickTabs::Set(const QuickTabs& t)
{
	active = t.active;
	cross = -1;
	highlight = -1;
	target = -1;

	tabs.SetCount(t.GetCount());
	for(int i = 0; i < t.GetCount(); i++)
		tabs[i].name = t.tabs[i].name;
	Repos(true);
	sc.Set(t.sc);
}
