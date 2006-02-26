#include "CtrlLib.h"

#define LLOG(x) // LOG(x)

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
		PageY py = text.GetHeight(Rect(0, 0, GetPageCx(), INT_MAX));
		y = (sz.cy / zoom  - py.y) / 2;
	}
	text.Paint(pw, Rect(0, 0, GetPageCx(), INT_MAX), pi);
	w.End();
	w.End();
}

void  RichTextView::SetSb()
{
	Rect page(0, 0, GetPageCx(), INT_MAX);
	PageY py = text.GetHeight(page);
	sb.SetTotal(py.y);
	sb.SetPage((GetSize().cy - margin.top - margin.bottom) / GetZoom());
}

bool  RichTextView::Key(dword key, int count)
{
	return sb.VertKey(key);
}

void  RichTextView::MouseWheel(Point p, int zdelta, dword keyflags)
{
	sb.Wheel(zdelta);
}

Image RichTextView::CursorImage(Point p, dword keyflags)
{
	if(WhenLink && !IsNull(GetLink(p)))
		return CtrlImg::HandCursor();
	return Image::Arrow();
}

void  RichTextView::LeftDown(Point p, dword keyflags)
{
	String link = GetLink(p);
	if(!IsNull(link))
		WhenLink(link);
	else
		SetFocus();
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
	return AsQTF(text);
}

void  RichTextView::SetData(const Value& v)
{
	SetQTF(String(v));
}

void  RichTextView::Scroll()
{
	scroller.Scroll(*this, Rect(GetSize()).Deflated(margin), sb * GetZoom());
}

Zoom  RichTextView::GetZoom() const
{
	int szcx = GetSize().cx;
	if(!sb.IsShown() && sb.IsAutoHide())
		szcx -= ScrollBar::GetStdBox();
	return IsNull(zoom) ? Zoom(szcx - margin.left - margin.right, cx) : zoom;
}

int   RichTextView::GetPageCx() const
{
	int szcx = GetSize().cx;
	if(!sb.IsShown() && sb.IsAutoHide())
		szcx -= ScrollBar::GetStdBox();
	return IsNull(zoom) ? cx : (szcx - margin.left - margin.right) / zoom;
}

String RichTextView::GetLink(Point p) const
{
	Size sz = GetSize();
	sz.cx -= margin.left + margin.right;
	sz.cy -= margin.top + margin.bottom;
	p -= margin.TopLeft();
	Zoom zoom = GetZoom();
	p.y += sb * zoom;
	int px = GetPageCx();
	int pos = text.GetPos(p.x / zoom, PageY(0, p.y / zoom), Rect(0, 0, px, INT_MAX));
	if(pos < 0) return Null;
	String link;
	RichObject object = text.GetRichPos(pos).object;
	if(object) {
		Rect rc = text.GetCaret(pos, Rect(0, 0, px, INT_MAX));
		link = object.GetLink(p - rc.TopLeft(), rc.Size());
	}

	if(IsNull(link)) {
		RichPos richpos = text.GetRichPos(pos);
		link = Nvl(richpos.fieldformat.link, richpos.format.link);
	}
	if(!IsNull(link)) return link;

/* TABLE
	RichPara pa = text.Get(text.FindPara(pos));
	int ii = pa.FindPart(pos);
	if(ii < pa.GetCount() && pa[ii].field)
	{
		const Array<RichPara::Part>& fparts = pa[ii].fieldpart;
		for(int p = 0; p < fparts.GetCount(); p++)
			if(!IsNull(fparts[p].format.link))
				return fparts[p].format.link;
	}
*/
	return Null;
}

void RichTextView::GotoLabel(const String& lbl, bool dohighlight)
{
	Vector<RichValPos> f = text.GetValPos(Size(GetPageCx(), INT_MAX), RichText::LABELS);
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
}

void  RichTextView::Pick(pick_ RichText& rt)
{
	sb = 0;
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
