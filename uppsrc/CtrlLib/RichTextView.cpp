#include "CtrlLib.h"

#define LLOG(x) // LOG(x)

Rect  RichTextView::GetPage() const
{
	return Rect(0, 0, GetPageCx(), INT_MAX);
}

void  RichTextView::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(sz, background);
	sz.cx -= margin.left + margin.right;
	sz.cy -= margin.top + margin.bottom;
	w.Clipoff(margin.left, margin.top, sz.cx, sz.cy);
	PaintInfo pi;
	if(!hldec)
		pi.hyperlink = Null;
	if(sell < selh) {
		pi.sell = sell;
		pi.selh = selh;
	}
	pi.indexentry = Null;
	pi.highlightpara = highlight;
	pi.zoom = GetZoom();
	int q = sb * pi.zoom;
	scroller.Set(q);
	w.Offset(0, -q);
	SimplePageDraw pw(w);
	pi.top = PageY(0, sb);
	pi.bottom = PageY(0, sb + sz.cy / pi.zoom);
	pi.usecache = true;
	pi.sizetracking = sizetracking;
	int y = 0;
	if(vcenter && sb.GetTotal() < sb.GetPage()) {
		PageY py = text.GetHeight(GetPage());
		y = (sz.cy / zoom  - py.y) / 2;
	}
	text.Paint(pw, GetPage(), pi);
	w.End();
	w.End();
}

Zoom  RichTextView::GetZoom() const
{
	int szcx = GetSize().cx;
	if(!sb.IsShown() && sb.IsAutoHide())
		szcx -= ScrollBar::GetStdBox();
	return IsNull(zoom) ? Zoom(szcx - margin.left - margin.right, cx) : zoom;
}

int   RichTextView::GetPageCx(bool reduced) const
{
	int szcx = GetSize().cx;
	if(reduced && !sb.IsShown() && sb.IsAutoHide())
		szcx -= ScrollBar::GetStdBox();
	return IsNull(zoom) ? cx : (szcx - margin.left - margin.right) / zoom;
}

void  RichTextView::SetSb()
{
	Rect page(0, 0, GetPageCx(true), INT_MAX);
	PageY py = text.GetHeight(page);
	sb.SetTotal(py.y);
	sb.SetPage((GetSize().cy - margin.top - margin.bottom) / GetZoom());
}

bool  RichTextView::Key(dword key, int count)
{
	if(key == K_CTRL_C || key == K_SHIFT_INSERT) {
		Copy();
		return true;
	}
	return sb.VertKey(key);
}

void  RichTextView::MouseWheel(Point p, int zdelta, dword keyflags)
{
	sb.Wheel(zdelta);
}

Image RichTextView::CursorImage(Point p, dword keyflags)
{
	int pos = GetPointPos(p);
	if(WhenLink && pos >= 0 && !IsNull(GetLink(pos, p)))
		return CtrlImg::HandCursor();
	if(HasCapture())
		return CtrlImg::ibeam0();
	return Image::Arrow();
}


void RichTextView::Copy()
{
	if(anchor == cursor)
		text.WriteClipboard();
	else {
		RefreshSel();
		text.Copy(sell, selh - sell).WriteClipboard();
	}
}

void RichTextView::RightDown(Point p, dword keyflags)
{
	MenuBar b;
	b.Add(CtrlImg::copy(), t_("Copy"), THISBACK(Copy)).Key(K_CTRL_C);
	b.Execute();
}

int  RichTextView::GetPointPos(Point p) const
{
	Size sz = GetSize();
	sz.cx -= margin.left + margin.right;
	sz.cy -= margin.top + margin.bottom;
	p -= margin.TopLeft();
	Zoom zoom = GetZoom();
	p.y += sb * zoom;
	return text.GetPos(p.x / zoom, PageY(0, p.y / zoom), GetPage());
}

String RichTextView::GetLink(int pos, Point p) const
{

	String link;
	RichObject object = text.GetRichPos(pos).object;
	if(object) {
		Rect rc = text.GetCaret(pos, GetPage());
		link = object.GetLink(p - rc.TopLeft(), rc.Size());
	}

	if(IsNull(link)) {
		RichPos richpos = text.GetRichPos(pos);
		link = Nvl(richpos.fieldformat.link, richpos.format.link);
	}
	return link;
}

void RichTextView::RefreshRange(int a, int b)
{
	int l = min(a, b);
	int h = max(a, b);
	if(l == h)
		return;
	Rect r1 = text.GetCaret(l, GetPage());
	Rect r2 = text.GetCaret(h, GetPage());
	Zoom zoom = GetZoom();
	Refresh(0, zoom * (r1.top - sb), GetSize().cx, zoom * (r2.bottom - sb + zoom.d - 1));
}

void  RichTextView::RefreshSel()
{
	int l = min(cursor, anchor);
	int h = max(cursor, anchor);
	if(l != h) {
		for(;;) {
			RichPos pl = text.GetRichPos(l);
			if(!pl.table)
				break;
			l -= pl.posintab + 1;
			if(l < 0)
				break;
		}
		for(;;) {
			RichPos ph = text.GetRichPos(h);
			if(!ph.table)
				break;
			h += ph.tablen - ph.posintab + 1;
			if(h >= text.GetLength())
				break;
		}
		if(l < 0 || h >= text.GetLength())
			l = h = cursor;
	}
	if(sell == l && selh == h || sell == selh && l == h)
		return;
	RichPos pl = text.GetRichPos(l);
	RichPos ph = text.GetRichPos(h);
	RichPos psell = text.GetRichPos(sell);
	RichPos pselh = text.GetRichPos(selh);
	if(psell.parai != pl.parai || pselh.parai != ph.parai)
		Refresh();
	else {
		RefreshRange(l, sell);
		RefreshRange(h, selh);
	}
	sell = l;
	selh = h;
}

void  RichTextView::LeftDown(Point p, dword keyflags)
{
	int pos = GetPointPos(p);
	if(pos < 0) {
		cursor = 0;
		anchor = 0;
		return;
	}
	String link = GetLink(pos, p);
	if(!IsNull(link))
		WhenLink(link);
	else {
		cursor = pos;
		if(!(keyflags & K_SHIFT))
			anchor = pos;
		RefreshSel();
		SetFocus();
		SetCapture();
	}
}

void RichTextView::MouseMove(Point p, dword keyflags)
{
	if(HasCapture()) {
		int pos = GetPointPos(p);
		if(pos < 0)
			return;
		cursor = pos;
		Rect r1 = text.GetCaret(cursor, GetPage());
		sb.ScrollInto(r1.top, r1.Height());
		RefreshSel();
	}
}

void RichTextView::LeftRepeat(Point p, dword keyflags)
{
	MouseMove(p, keyflags);
}

void  RichTextView::EndSizeTracking()
{
	sizetracking = false;
	Refresh();
}

void  RichTextView::Layout()
{
	if(IsOpen()) {
		sizetracking = true;
		KillTimeCallback(TIMEID_ENDSIZETRACKING);
		SetTimeCallback(250, THISBACK(EndSizeTracking), TIMEID_ENDSIZETRACKING);
	}
	SetSb();
	Refresh();
}

Value RichTextView::GetData() const
{
	if(text.IsEmpty()) return Value();
	return GetQTF();
}

void  RichTextView::SetData(const Value& v)
{
	SetQTF(String(v));
}

void  RichTextView::Scroll()
{
	scroller.Scroll(*this, Rect(GetSize()).Deflated(margin), sb * GetZoom());
}

void RichTextView::GotoLabel(const String& lbl, bool dohighlight)
{
	Vector<RichValPos> f = text.GetValPos(GetPage(), RichText::LABELS);
	highlight = Null;
	for(int i = 0; i < f.GetCount(); i++) {
		if(f[i].data == WString(lbl)) {
			sb = f[i].py.y;
			if(dohighlight)
				highlight = f[i].pos;
			break;
		}
	}
	Refresh();
}

void  RichTextView::Clear()
{
	sb = 0;
	text.Clear();
	SetSb();
	Refresh();
	anchor = cursor = sell = selh = 0;
}

void  RichTextView::Pick(pick_ RichText& rt)
{
	sb = 0;
	anchor = cursor = sell = selh = 0;
	text = rt;
	SetSb();
	Refresh();
	highlight = -1;
}

void  RichTextView::SetQTF(const char *qtf, Zoom z, bool scolors)
{
	RichText txt = ParseQTF(qtf, scolors);
	if(z.m != z.d)
		txt.ApplyZoom(z);
	Pick(txt);
	sb.SetLine(z * 100);
}

RichTextView& RichTextView::PageWidth(int _cx)
{
	cx = _cx;
	sb = 0;
	SetSb();
	Refresh();
	return *this;
}

RichTextView& RichTextView::SetZoom(Zoom z)
{
	zoom = z;
	sb = 0;
	SetSb();
	Refresh();
	return *this;
}

RichTextView& RichTextView::Background(Color c)
{
	background = c;
	Transparent(IsNull(c));
	Refresh();
	return *this;
}

RichTextView& RichTextView::VCenter(bool b)
{
	vcenter = b;
	return *this;
}

RichTextView& RichTextView::Margins(const Rect& m)
{
	margin = m;
	Refresh();
	return *this;
}

RichTextView& RichTextView::HMargins(int a)
{
	margin.left = margin.right = a;
	Refresh();
	return *this;
}

RichTextView& RichTextView::VMargins(int a)
{
	margin.top = margin.bottom = a;
	Refresh();
	return *this;
}

RichTextView& RichTextView::Margins(int a)
{
	margin.Set(a, a, a, a);
	Refresh();
	return *this;
}

void LinkInRichTextClipboard__();

RichTextView::RichTextView()
{
	SetFrame(FieldFrame());
	AddFrame(sb);
	cx = 3968;
	sizetracking = false;
	sb.SetLine(100);
	sb.WhenScroll = THISBACK(Scroll);
//	BackPaint();
	zoom = Null;
	background = SWhite;
	vcenter = false;
	margin = Rect(0, 0, 0, 0);
	highlight = -1;
	hldec = true;
	WhenLink = callback(LaunchWebBrowser);
	NoWantFocus();
	anchor = cursor = sell = selh = 0;
	StdRichClipboardFormats();
}

RichTextView::~RichTextView() {}

RichTextCtrl::RichTextCtrl()
{
	SetZoom(Zoom(1, 1));
	Transparent();
	Background(Null);
	SetFrame(NullFrame());
	AutoHideSb();
}
