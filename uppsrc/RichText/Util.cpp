#include "RichText.h"

Draw& SimplePageDraw::Info()
{
	return w;
}

Draw& SimplePageDraw::Page(int)
{
	return w;
}

void Print(Draw& w, const RichText& text, const Rect& page, int firstpage, int lastpage,
           int copies, bool collate)
{
	firstpage = max(0, firstpage);
	int lpage = text.GetHeight(page).page;
	lastpage = min(lastpage, text.GetHeight(page).page);
	PrintPageDraw pw(w);
	Size sz = w.GetPageMMs();
	Size pgsz = page.Size();
	int x = (6000 * sz.cx / 254 - pgsz.cx) / 2;
	int y = (6000 * sz.cy / 254 - pgsz.cy) / 2;
	for(int c = 0; c < (collate ? copies : 1); c++)
		for(int i = firstpage; i <= lastpage; i++)
			for(int c = 0; c < (collate ? 1 : copies); c++) {
				w.StartPage();
				w.Offset(x, y);
				pw.SetPage(i);
				PaintInfo paintinfo;
				paintinfo.top = PageY(i, 0);
				paintinfo.bottom = PageY(i + 1, 0);
				paintinfo.indexentry = Null;
				if(text.IsPrintNoLinks())
					paintinfo.hyperlink = Null;
				text.Paint(pw, page, paintinfo);
				w.End();
				String footer = text.GetFooter();
				if(!IsNull(footer) && lpage) {
					String n = Format(footer, i + 1, lpage + 1);
					Size nsz = w.GetTextSize(n, Arial(90).Italic());
					pw.Page(i).DrawText(
						x + pgsz.cx - nsz.cx, y + pgsz.cy + 100,
						n, Arial(90).Italic());
				}
				w.EndPage();
			}
}

void RichText::ApplyZoom(Zoom z)
{
	RichStyles ostyle(style, 1);
	for(int i = 0; i < style.GetCount(); i++)
		style[i].format *= z;
	RichTxt::ApplyZoom(z, ostyle, style);
	RefreshAll();
}

#ifdef PLATFORM_WIN32
#include <commdlg.h>

void Print(HDC hdc, const RichText& text, const Rect& page, int firstpage, int lastpage, int copies, bool collate,
           const char *name)
{
	PrintDraw w(hdc, name ? (const char *)name : (const char *)GetExeTitle());
	Print(w, text, page, firstpage, lastpage, copies, collate);
}


bool Print(const RichText& text, const Rect& page, int currentpage, const char *name, HWND hwndOwner)
{
	PRINTDLG dlg;
	memset(&dlg, 0, sizeof(dlg));
	dlg.lStructSize = sizeof(dlg);
	dlg.hwndOwner = hwndOwner ? hwndOwner : ::GetActiveWindow();
	dlg.Flags = PD_ALLPAGES|PD_DISABLEPRINTTOFILE|PD_NOSELECTION|PD_RETURNDC|
		        PD_HIDEPRINTTOFILE;
	dlg.nFromPage = currentpage + 1;
	dlg.nToPage = currentpage + 1;
	dlg.nMinPage = 1;
	dlg.nMaxPage = text.GetHeight(page).page + 1;
	if(!PrintDlg(&dlg)) return false;
	if(!(dlg.Flags & PD_PAGENUMS)) {
		dlg.nFromPage = dlg.nMinPage;
		dlg.nToPage = dlg.nMaxPage;
	}
	Print(dlg.hDC, text, page, dlg.nFromPage - 1, dlg.nToPage - 1, dlg.nCopies,
	      dlg.Flags & PD_COLLATE, name);
	return true;
}

#endif

Zoom GetRichTextStdScreenZoom()
{
	return Zoom(Ctrl::VertLayoutZoom(140), 1024);
}

struct QTFDisplayCls : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const;
	virtual Size GetStdSize(const Value& q) const;
	virtual Size RatioSize(const Value& q, int cx, int cy) const;
};

Size QTFDisplayCls::GetStdSize(const Value& q) const
{
	Size sz;
	RichText txt = ParseQTF((String)q);
	txt.ApplyZoom(GetRichTextStdScreenZoom());
	sz.cx = txt.GetWidth();
	sz.cy = txt.GetHeight(Zoom(1, 1), sz.cx);
	return sz;
}

Size QTFDisplayCls::RatioSize(const Value& q, int cx, int cy) const
{
	if(cy == 0 && cx > 0) {
		RichText txt = ParseQTF((String)q);
		return Size(cx, txt.GetHeight(Zoom(1, 1), cx));
	}
	return GetStdSize(q);
}

void QTFDisplayCls::Paint(Draw& draw, const Rect& r, const Value& v, Color ink, Color paper, dword style) const
{
	String s;
	s << "[@(" << ink.GetR() << "." << ink.GetG() << "." << ink.GetB() << ") " << v;
	RichText rtext = ParseQTF(s);
	rtext.ApplyZoom(GetRichTextStdScreenZoom());
	draw.DrawRect(r, paper);
	draw.Clipoff(r);
	rtext.Paint(Zoom(1, 1), draw, 0, 0, r.Width());
	draw.End();
}

const Display& QTFDisplay()
{
	return Single<QTFDisplayCls>();
}

struct QTFDisplayCCls : QTFDisplayCls {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const;
};

void QTFDisplayCCls::Paint(Draw& draw, const Rect& r, const Value& v, Color ink, Color paper, dword style) const
{
	String s;
	s << "[@(" << ink.GetR() << "." << ink.GetG() << "." << ink.GetB() << ") " << v;
	RichText rtext = ParseQTF(s);
	rtext.ApplyZoom(GetRichTextStdScreenZoom());
	draw.DrawRect(r, paper);
	draw.Clipoff(r);
	int cy = rtext.GetHeight(Zoom(1, 1), r.Width());
	rtext.Paint(Zoom(1, 1), draw, 0, max(0, (r.Height() - cy) / 2), r.Width());
	draw.End();
}

const Display& QTFDisplayVCenter()
{
	return Single<QTFDisplayCCls>();
}

RichText AsRichText(const wchar *s, const RichPara::Format& f)
{
	RichText clip;
	RichPara p;
	p.format = f;
	p.part.Add();
	p.part[0].format = f;
	RichStyle cs;
	cs.format = f;
	cs.format.sscript = 0;
	cs.format.link.Clear();
	cs.format.indexentry.Clear();
	cs.format.language = LNG_ENGLISH;
	cs.format.label.Clear();
	clip.SetStyle(f.styleid, cs);
	WString& part = p.part.Top().text;
	while(*s) {
		if(*s == '\n') {
			clip.Cat(p);
			part.Clear();
		}
		if(*s >= 32 || *s == '\t')
			part.Cat(*s);
		s++;
	}
	clip.Cat(p);
	return clip;
}
