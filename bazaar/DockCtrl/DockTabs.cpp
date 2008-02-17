#include "DockCtrl/DockCtrl.h"

#define IMAGECLASS DockCtrlImgs
#define IMAGEFILE <DockCtrl/DockableCtrl.iml>
#include <Draw/iml_source.h>

void CustomFrame::FrameLayout(Rect &r)
{
	switch(layout)
	{
		case LAYOUT_LEFT:
			LayoutFrameLeft(r, this, size);
			break;
		case LAYOUT_TOP:
			LayoutFrameTop(r, this, size);
			break;
		case LAYOUT_RIGHT:
			LayoutFrameRight(r, this, size);
			break;
		case LAYOUT_BOTTOM:
			LayoutFrameBottom(r, this, size);
			break;
	}
}

void CustomFrame::FrameAddSize(Size& sz)
{
	switch(layout)
	{
		case LAYOUT_LEFT:
			sz.cx += size;
			break;
		case LAYOUT_TOP:
			sz.cy += size;
			break;
		case LAYOUT_RIGHT:
			sz.cx += size;
			break;
		case LAYOUT_BOTTOM:
			sz.cy += size;
			break;		
	}	
}


void CustomFrame::Fix(Size& sz)
{
	if(!horizontal)
		Swap(sz.cx, sz.cy);
}

void CustomFrame::Fix(Point& p)
{
	if(!horizontal)
		Swap(p.x, p.y);
}

//----------------------------------------------------------------------------------------------

void TabInterface::Add(String name, bool activate)
{
	Tab &t = tabs.Add();
	if(IsNull(name) || name == "") name = t_("Untitled");
	else t.name 	= name;
	t.id 			= GetNextId();
	t.active 		= activate;
	t.visible 		= true;
	t.highlighted 	= false;
	t.icon			= DockCtrlImgs::DefaultImage();
	ReposTabs();
	SetActiveTab(tabs.GetCount() - 1);
}

void TabInterface::Add(DockableCtrl& dock, bool activate)
{
	Tab &t = tabs.Add();
	if(IsNull(dock.GetLabel()) || dock.GetLabel() == "") t.name = t_("Untitled");
	else t.name 	= dock.GetLabel();
	t.id 			= GetNextId();
	t.active 		= activate;
	t.visible 		= true;
	t.highlighted 	= false;
	t.icon			= dock.GetIcon();
	t.dock			= &dock;
	ReposTabs();
	SetActiveTab(tabs.GetCount() - 1);
}

void TabInterface::Remove(DockableCtrl& dock)
{
}

void TabInterface::Remove(int n)
{
}

void TabInterface::Close(int n)
{
	if(tabs.GetCount() == 0 || n < 0) return;
	if(tabs.GetCount() == 1)
	{
		tabs.Remove(0);
		ReposTabs();
		return;
	}
	
	int c  = -1;
	int nc = -1;
	int pc = -1;
	
	bool isactive = n == active ? true : false;
	
	c = Find(tabs[n].id);
	nc = GetNext(c);
	pc = GetPrev(c);
	
	if(nc == -1 && pc >= 0)	 
		c = pc;
	else if(nc >= 0 && pc == -1) 
		c = nc;
	
	if(hasscrollbar)
		scrollbar.AddTotal(-tabs[n].cx);
	tabs.Remove(n);
	ReposTabs();
	if(isactive) 
		SetActiveTab(c);
}

void TabInterface::CloseAll()
{
	for(int i = tabs.GetCount() - 1; i >= 0; i--)
		if(i != active)	
			tabs.Remove(i);

	if(hasscrollbar) scrollbar.SetTotal(tabs[0].cx);
	ReposTabs();
	SetActiveTab(0);	
}

void TabInterface::SetActiveTab(int n)
{
	if(n == active || tabs.GetCount() == 0) return;
	for(int i = 0; i < tabs.GetCount(); i++)
		if(i != n) tabs[i].active = false;
		else tabs[active = i].active = true;
	
	int count = tabs.GetCount();
	if(n >= count) n = count - 1;
	
	Tab& t = tabs[n];
	int cx = t.x - (int(hasscrollbar) * scrollbar.GetPos());
	if(hasscrollbar)
	{
		if(cx < 0)
			scrollbar.AddPos(cx - 10);
		else
		{
			cx = t.x + t.cx - GetSize().cx - scrollbar.GetPos();
			if(cx > 0)
				scrollbar.AddPos(cx + 10);
		}
	}
	if(HasMouse())MouseMove(GetMouseViewPos(), 0);
	UpdateActionRefresh();
	// Do action.
	WhenSelect();
}

void TabInterface::SetActiveTab(DockableCtrl& dock)
{
	SetActiveTab(Find(dock));	
}

TabInterface& TabInterface::HasScrollBar(bool b)
{
	if(b)
	{
		AddFrame(scrollbar); 
		scrollbar.WhenScroll = THISBACK(OnScrollBar);
	}
	else 
		RemoveFrame(scrollbar);
	Refresh();	 
	hasscrollbar = b;
	return *this;
}

int TabInterface::GetActiveTab()
{
	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].IsActive()) return i;
	return active = -1;
}

int TabInterface::Find(int id)
{
	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].id == id) return i;
	return -1;
}

int TabInterface::Find(DockableCtrl& dock)
{
	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].dock == &dock) return i;
	return -1;
}

int TabInterface::GetNext(int n)
{
	for(int i = n + 1; i < tabs.GetCount(); i++)
		if(tabs[i].visible) return i;
	return -1;
}

int TabInterface::GetPrev(int n)
{
	for(int i = n - 1; i >= 0; i--)
		if(tabs[i].visible) return i;
	return -1;
}


int TabInterface::GetWidth(int n)
{
	Tab &t = tabs[n];
	t.textsize  	= GetTextSize(t.name, style->font);
	return TAB_MARGIN * 2 + t.textsize.cx + ((hasfileicon ? TAB_SPACE : TAB_SPACE / 3) + DockCtrlImgs::DClose().GetSize().cx) * hastabbutton + (TAB_FILEICON + TAB_SPACEICON) * hasfileicon;
}

int TabInterface::GetWidth()
{
	return tabs[tabs.GetCount() - 1].Right() + style->margin * 2;
}

int TabInterface::GetHeight()
{
	return style->tabheight + style->sel.bottom + (int(hasscrollbar) * (TAB_SBHEIGHT + TAB_SBSEPARATOR));
}

void TabInterface::ReposTabs()
{
	if(!tabs.GetCount())
		return;

	int j = 0;
	bool first = true;
	for(int i = 0; i < tabs.GetCount(); i++)
	{
		tabs[i].x = first ? 0 : tabs[j].Right();
		j = i;
		first = false;
		tabs[i].visible = true;
		tabs[i].y = 0;
		tabs[i].cx = GetWidth(i);
		tabs[i].cy = style->tabheight + style->sel.top;
	}
	if(hasscrollbar) scrollbar.SetTotal(GetWidth());
}

void TabInterface::DrawLabel(Draw& w, int x, int y, Tab& t, bool isactive)
{
	if(horizontal)
		w.DrawText(x + TAB_MARGIN + (TAB_FILEICON + TAB_SPACEICON) * int(hasfileicon), 
				(y - t.textsize.cy - (isactive ? 1 : 0)) / 2, 
				t.name, style->font);
	else
		w.DrawText((y + t.textsize.cy) / 2 + (isactive ? 1 : 0), 
				x + TAB_MARGIN + (TAB_FILEICON + TAB_SPACEICON) * int(hasfileicon), 
				2700, t.name, 
				style->font); 
}

void TabInterface::DrawTab(Draw& w, Size &sz, int i)
{
	Size isz 	= DockCtrlImgs::DClose().GetSize();
	int cnt 	= tabs.GetCount();
	Tab &t 		= tabs[i];

	bool ac 	= i == active;
	bool hl 	= i == highlight;
		
	int lx 		= i > 0 ? style->extendleft : 0;
	int x 		= t.x - scrollbar.GetPos() + style->margin - lx;
	int h 		= sz.cy + (ac ? 0 : style->sel.top);

	int ndx = !IsEnabled() ? CTRL_DISABLED :
		       ac ? CTRL_PRESSED :
		       hl ? CTRL_HOT : CTRL_NORMAL;

	Image nimg, niimg;
	ImageDraw img(t.cx + lx + (ac ? style->sel.right + style->sel.left : 0),  
				(ac ? t.cy + style->sel.bottom : t.cy - style->sel.top));

	               
	const Value& sv = (cnt == 1 ? style->both : i == 0 ? style->first : i == cnt - 1 ? style->last : style->normal)[ndx];

	if(ac)
		ChPaint(img, 0,	0, 	
				t.cx + style->sel.right + style->sel.left + lx, 
				t.cy + style->sel.bottom, 
				sv);
	else
		ChPaint(img, 0, 0, 
				t.cx + lx, 
				t.cy - style->sel.top, 
				sv);


	if(hastabbutton && tabs.GetCount())
		img.DrawImage(t.cx - isz.cx - TAB_MARGIN, 
				(h - isz.cy) / 2 + (ac ? 1 : -2),
				(ac || hl) ? (tabbutton == i ? DockCtrlImgs::DCloseh : 
				ac ? DockCtrlImgs::DCloses : DockCtrlImgs::DClose) : 
				DockCtrlImgs::DClose);

	if(hasfileicon)
		img.DrawImage(TAB_MARGIN, (h - isz.cy) / 2 + (ac ? 1 : -2), t.icon);

	switch(layout)
	{
		case LAYOUT_LEFT:
			nimg = MirrorVert(RotateAntiClockwise(img));
			break;
		case LAYOUT_TOP:
			nimg = img;	
			break;
		case LAYOUT_RIGHT:
			nimg = RotateClockwise(img);
			break;
		case LAYOUT_BOTTOM:
			nimg = MirrorVert(img);
			break;
	}

	if(ac) 
		w.DrawImage((horizontal ? x - style->sel.left : 0), (horizontal ?  0 : x - style->sel.left), nimg);
	else
		w.DrawImage((horizontal ? x : style->sel.left), (horizontal ? style->sel.left : x), nimg);

	DrawLabel(w, x, h, t, ac); 	
}

void TabInterface::Paint(Draw& d)
{
	Size sz = GetSize();
	d.DrawRect(sz, SColorFace());

	if(!horizontal)
		d.DrawRect(layout == LAYOUT_LEFT ? sz.cx - 1: 1, 0, 
				1, sz.cy, LtGray());
	else
		d.DrawRect(0, layout == LAYOUT_TOP ? sz.cy  - 1 : 1, 
				sz.cx, 1, LtGray());

	Fix(sz);
	
	for(int i = 0; i < tabs.GetCount(); i++)
		if(tabs[i].IsVisible() && active != i)
			DrawTab(d, sz, i);
	
	if(active >= 0)
		DrawTab(d, sz, active);
}

void TabInterface::LeftDown(Point p, dword keyflags)
{
	if(tabbutton >= 0)
	{
		int i = Find(tabbutton);
		if(i == -1 && active >= 0)
			i = active;
		
		if(i >= 0)
		{
			DockableCtrl* dock = tabs[i].dock;
			if(dock) 
				WhenClose(tabbutton, *dock);			
		}
	}
	
	if(highlight >= 0)
	{
		SetActiveTab(highlight);
		if(active >= 0)
			WhenActive(active, *tabs[active].dock);
	}
}

void TabInterface::LeftUp(Point p, dword keyflags)
{
	if(HasCapture())
		ReleaseCapture();
}

void TabInterface::LeftDrag(Point p, dword keyflags)
{
	if(highlight >= 0)
		SetActiveTab(highlight);
	WhenDrag(active, *tabs[active].dock);
}


void TabInterface::RightDown(Point p, dword keyflags)
{
	if(highlight >= 0) 
		SetActiveTab(highlight);
	WhenContext(active, *tabs[active].dock);
}

void TabInterface::RightUp(Point p, dword keyflags)
{
}


void TabInterface::MouseMove(Point p, dword keyflags)
{
	Fix(p);
	if(HasCapture() && hasscrollbar)
	{
		scrollbar.AddPos(p.x - oldp.x, true);
		oldp = p;
		return;
	}
	
	p.x += (int(hasscrollbar) * scrollbar.GetPos()) - style->margin + style->extendleft;
	Size sz = GetSize();
	Fix(sz);	
	int h = highlight;
	bool istabbutton = false;
	bool istab = false;
	for(int i = 0; i < tabs.GetCount(); i++)
	{
		if(tabs[i].HasMouse(p))
		{
			istab = true;
			int h = sz.cy + (active == i ? 0 : style->sel.top);
			istabbutton =  hastabbutton ? tabs[i].HasMouseButton(p, h) : false;
			if(highlight != i || (istabbutton && tabbutton != i))
			{
				tabbutton = istabbutton ? i : -1;
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

	if(!istabbutton && tabbutton >= 0)
	{
		tabbutton = -1;
		Refresh();
	}
}

void TabInterface::MouseWheel(Point p, int zdelta, dword keyflags)
{
	if(!hasscrollbar) return;
	scrollbar.AddPos(-zdelta / 4, true);
	MouseMove(p, 0);
}


void TabInterface::MouseLeave()
{
	highlight = -1;
	tabbutton = -1;
	Refresh();
}



TabInterface& TabInterface::SetLayout(int l)
{
	layout = l;
	switch(l)
	{
		case LAYOUT_LEFT:
			horizontal = scrollbar.horizontal = false;
			scrollbar.SetLayout(LAYOUT_RIGHT);
			break;
		case LAYOUT_TOP:
			horizontal = scrollbar.horizontal = true;
			scrollbar.SetLayout(LAYOUT_BOTTOM);
			break;
		case LAYOUT_RIGHT:
			horizontal = scrollbar.horizontal = false;
			scrollbar.SetLayout(LAYOUT_LEFT);
			break;
		case LAYOUT_BOTTOM:
			horizontal = scrollbar.horizontal = true;
			scrollbar.SetLayout(LAYOUT_TOP);
			break;
	}
	RefreshLayout();
	return *this;	
}

TabInterface::TabInterface()
{

	id			= -1;
	active		= -1;
	highlight	= -1;
	visible  	= -1;
	fileicon	= -1;
	tabbutton	= -1;
	hasfileicon	= false;
	hastabbutton = true;
	
	SetStyle(TabCtrl::StyleDefault());
	CustomFrame::layout = LAYOUT_BOTTOM;
	HasScrollBar(true);
	SetSize(GetHeight() + 2);	
	BackPaint();
}

TabInterface::~TabInterface()
{
	for(int i = 0; i < tabs.GetCount(); i++)
		tabs.Remove(i);
}

//----------------------------------------------------------------------------------------------

TabInterface::TabScrollBar::TabScrollBar()
{
	CustomFrame::layout = LAYOUT_TOP;
	CustomFrame::SetSize(TAB_SBHEIGHT);
	CustomFrame::border = 0;

	total = 0;
	pos = 0;
	ps = 0;
	start_pos = 0;
	new_pos = 0;
	old_pos = 0;
	ready = false;
}


void TabInterface::TabScrollBar::Paint(Draw &w)
{
	if(!ready)
	{
		UpdatePos();
		ready = true;
	}
	Size sz = GetSize();
	
	w.DrawRect(sz, SColorFace());
	
	if(horizontal)
	{
		if(total > sz.cx)
			w.DrawRect(ffloor(pos), 1, fceil(size), 2, Blue);
		else
			w.DrawRect(0, sz.cy - 1, sz.cx, 1,  White);
	}
	else
	{
		if(total > sz.cy)
			w.DrawRect(1, ffloor(pos), 2, fceil(size), Blue);
		else
			w.DrawRect(sz.cx - 1, 0, 1, sz.cy, White);
	}
}

void TabInterface::TabScrollBar::LeftDown(Point p, dword keyflags)
{
	SetCapture();
	Fix(p);
	old_pos = new_pos = p.x;
	if(p.x < pos || p.x > pos + size)
		start_pos = size / 2;
	else
		start_pos = UPP::tabs(p.x - pos);
	UpdatePos();
	Refresh();
}

void TabInterface::TabScrollBar::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
	Fix(p);
	old_pos = p.x;
}

void TabInterface::TabScrollBar::MouseMove(Point p, dword keyflags)
{
	if(!HasCapture())
		return;
	Fix(p);
	new_pos = p.x;
	UpdatePos();
	Refresh();
}

void TabInterface::TabScrollBar::MouseWheel(Point p, int zdelta, dword keyflags)
{
	AddPos(-zdelta / 4, true);
}

void TabInterface::TabScrollBar::Layout()
{
	UpdatePos(false);
}

int TabInterface::TabScrollBar::GetPos() const
{
	return ffloor(ps);
}

void TabInterface::TabScrollBar::SetPos(int p, bool dontscale)
{
	Fix(sz);
	pos = total > 0 ? dontscale ? p : iscale(p, sz.cx, total) : 0;
	UpdatePos(false);
	Refresh();
}

void TabInterface::TabScrollBar::AddPos(int p, bool dontscale)
{
	Fix(sz);
	pos += total > 0 ? dontscale ? p : iscale(p, sz.cx, total) : 0;
	UpdatePos(false);
	Refresh();
}

int TabInterface::TabScrollBar::GetTotal() const
{
	return total;
}

void TabInterface::TabScrollBar::AddTotal(int t)
{
	total += t;
	UpdatePos();
	Refresh();
}

void TabInterface::TabScrollBar::SetTotal(int t)
{
	bool upd = total < t;
	total = t;
	UpdatePos(upd);
	Refresh();
}

void TabInterface::TabScrollBar::GoEnd()
{
	pos = total;
	UpdatePos(false);
	Refresh();
}

void TabInterface::TabScrollBar::GoBegin()
{
	pos = 0;
	UpdatePos(false);
	Refresh();
}

void TabInterface::TabScrollBar::Set(const TabScrollBar& t)
{
	total = t.total;
	pos = t.pos;
	ps = t.ps;
	Refresh();
}

void TabInterface::TabScrollBar::UpdatePos(bool update)
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
	WhenScroll();
}

TabInterface::TabScrollBar& TabInterface::TabScrollBar::SetLayout(int l)
{
	layout = l;
	RefreshLayout();
	return *this;	
}