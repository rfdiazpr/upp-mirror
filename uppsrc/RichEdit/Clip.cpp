#include "RichEdit.h"

NAMESPACE_UPP

#define RTFS "Rich Text Format;text/rtf;application/rtf"

bool RichEdit::Accept(PasteClip& d, RichText& clip)
{
	if(IsReadOnly())
		return false;
	for(int i = 0; i < RichObject::GetTypeCount(); i++) {
		RichObjectType& rt = RichObject::GetType(i);
		if(rt.Accept(d)) {
			Value data = rt.Read(d);
			if(!IsNull(data)) {
				RichPara p;
				p.Cat(RichObject(&rt, data, pagesz), formatinfo);
				clip.Cat(p);
			}
			return true;
		}
	}
	if(d.Accept("text/QTF"))
		clip = ParseQTF(~d);
	else
	if(d.Accept(RTFS))
		clip = ParseRTF(~d);
	else
	if(AcceptText(d))
		clip = AsRichText(GetWString(d), formatinfo);
	else
		return false;
	return true;
}

void RichEdit::ClipPaste(RichText& clip)
{
	NextUndo();
	if(clip.GetPartCount() == 1 && clip.IsTable(0)) {
		CancelSelection();
		if(cursorp.table) {
			NextUndo();
			SaveTable(cursorp.table);
			text.PasteTable(cursorp.table, cursorp.cell, clip.GetTable(0));
			Finish();
			return;
		}
	}
	clip.Normalize();
	PasteText(clip);
}

void RichEdit::DragAndDrop(Point p, PasteClip& d)
{
	dropcursor = GetMousePos(p);
	if(dropcursor >= 0) {
		RichText clip;
		if(Accept(d, clip)) {
			NextUndo();
			int c = dropcursor;
			if(InSelection(c))
				RemoveSelection();
			int sell, selh;
			bool sel = GetSelection(sell, selh);
			Move(c);
			clip.Normalize();
			droppos = cursor;
			droplen = clip.GetLength();
			ClipPaste(clip);
			if(sel) {
				if(c < sell) {
					sell += clip.GetLength();
					selh += clip.GetLength();
				}
				Select(sell, selh - sell);
			}
			return;
		}
	}
	if(!d.IsAccepted()) dropcursor = -1;
	Rect r = GetDropCaret();
	if(r != dropcaret) {
		Refresh(dropcaret);
		dropcaret = r;
		Refresh(dropcaret);
	}
}

Rect RichEdit::GetDropCaret()
{
	if(dropcursor < 0 || dropcursor > text.GetLength())
		return Rect(0, 0, 0, 0);
	RichCaret pr = text.GetCaret(dropcursor, pagesz);
	int h = pr.Height();
	Zoom zoom = GetZoom();
	Rect tr = GetTextRect();
	return RectC(pr.left * zoom + tr.left - 1, GetPosY(pr) + (pr.lineascent - pr.caretascent) * zoom - sb,
	             2, (pr.caretascent + pr.caretdescent) * zoom);
}

void RichEdit::DragRepeat(Point p)
{
	if(IsReadOnly())
		return;
	Size sz = GetSize();
	int sd = min(sz.cy / 4, 32);
	int d = 0;
	if(p.y < sd)
		d = p.y - sd;
	if(p.y > sz.cy - sd)
		d = p.y - sz.cy + sd;
	sb = (int)sb + minmax(d, -16, 16);
}

void RichEdit::Paste()
{
	if(IsReadOnly())
		return;
	RichText clip;
	if(!Accept(Clipboard(), clip))
		return;
	ClipPaste(clip);
}

void RichEdit::DragLeave()
{
	Refresh(dropcaret);
	dropcursor = -1;
	dropcaret.Clear();
}

void RichEdit::Copy()
{
	if(IsSelection()) {
		ClearClipboard();
		RichText clip = GetSelection();
		AppendClipboard("text/QTF", AsQTF(clip));
		AppendClipboard(RTFS, EncodeRTF(clip));
		AppendClipboardUnicodeText(clip.GetPlainText());
	}
	else
	if(objectpos >= 0) {
		RichObject o = GetObject();
		Vector<String> v = Split(o.GetType().GetClipFmts(), ';');
		for(int i = 0; i < v.GetCount(); i++) {
			if(i == 0)
				ClearClipboard();
			AppendClipboard(v[i], o.GetType().GetClip(o.GetData(), v[i]));
		}
	}
}

String RichEdit::GetSelection(const String& fmt) const
{
	String f = fmt;
	if(IsSelection()) {
		RichText clip = GetSelection();
		if(f == "text")
			return clip.GetPlainText().ToString();
		if(f == "wtext") {
			WString x = clip.GetPlainText();
			return String((byte *)~x, x.GetLength() + 2);
		}
		if(f == "text/QTF")
			return AsQTF(clip);
		if(InScList(f, RTFS))
			return EncodeRTF(clip);
	}
/*	else
	if(objectpos >= 0) {
		RichObject o = GetObject();
		if(InScList(fmt, o.GetType().GetClipFmts()))
			return o.GetType().GetClip(o.GetData(), fmt);
	}*/
	return Null;
}

void RichEdit::LeftDrag(Point p, dword flags)
{
	int c = GetMousePos(p);
	Size ssz = StdSampleSize();
	if(!HasCapture() && InSelection(c)) {
		RichText sample = GetSelection(5000);
		sample.ApplyZoom(Zoom(1, 8));
		ImageDraw iw(ssz);
		iw.DrawRect(0, 0, ssz.cx, ssz.cy, White);
		sample.Paint(iw, 0, 0, 128);
		ImageBuffer ib(iw);
		RGBA *s = ~ib;
		const RGBA *e = s + ib.GetLength();
		while(s < e) {
			if(s->r == 255 && s->g == 255 && s->b == 255)
				s->a = 0;
			s++;
		}
		droppos = -1;
		NextUndo();
		int sell, selh;
		if(DoDragAndDrop("text/QTF;" RTFS ";" + ClipFmtsText(), ib) == DND_MOVE
		   && GetSelection(sell, selh)) {
			if(droppos >= 0 && droppos >= selh)
				droppos -= selh - sell;
			RemoveSelection();
		}
		if(droppos >= 0)
			Select(droppos, droplen);
	}
/*	else
	if(objectpos >= 0 && c == objectpos) {
		ReleaseCapture();
		RichObject o = GetObject();
		Size sz = o.GetPhysicalSize();
		NextUndo();
		if(DoDragAndDrop(o.GetType().GetClipFmts(),
		                 o.ToImage(Size(ssz.cx, sz.cy * ssz.cx / sz.cx))) == DND_MOVE
		   && objectpos >= 0) {
			if(droppos > objectpos)
				droppos--;
			Remove(objectpos, 1);
		}
		Move(droppos);
		SetObjectPos(droppos);
	}*/
}

END_UPP_NAMESPACE
