#include "CodeEditor.h"

NAMESPACE_UPP

void Renumber(LineInfo& lf) {
	LineInfo tf;
	int l = 0;
	if(lf.GetCount()) {
		LineInfoRecord& t = tf.Add();
		t.breakpoint = lf[0].breakpoint;
		t.lineno = 0;
		t.count = lf[0].count;
		t.error = lf[0].error;
		t.edited = lf[0].edited;
		l += t.count;
	}
	for(int i = 1; i < lf.GetCount(); i++) {
		LineInfoRecord& r = lf[i];
		if(r.breakpoint.IsEmpty() && r.error == 0 && r.edited == 0 && 
			tf.Top().breakpoint.IsEmpty() && tf.Top().error == 0 && tf.Top().edited == 0)
			tf.Top().count += r.count;
		else {
			LineInfoRecord& t = tf.Add();
			t.breakpoint = r.breakpoint;
			t.error = r.error;
			t.edited = r.edited;
			t.count = r.count;
			t.lineno = l;
		}
		l += r.count;
	}
	lf = tf;
}

void ClearBreakpoints(LineInfo& lf) {
	for(int i = 0; i < lf.GetCount(); i++)
		lf[i].breakpoint.Clear();
}

void ValidateBreakpoints(LineInfo& lf) {
	for(int i = 0; i < lf.GetCount(); i++)
		if(lf[i].breakpoint[0] == 0xe)
			lf[i].breakpoint = "1";
}

void EditorBar::sPaintImage(Draw& w, int y, int fy, const Image& img)
{
	w.DrawImage(0, y + (fy - img.GetSize().cy) / 2, img);
}

void EditorBar::Paint(Draw& w) {
	static Image (*numeri[])() = {
		CodeEditorImg::N0, CodeEditorImg::N1, CodeEditorImg::N2, CodeEditorImg::N3, CodeEditorImg::N4,
		CodeEditorImg::N5, CodeEditorImg::N6, CodeEditorImg::N7, CodeEditorImg::N8, CodeEditorImg::N9,
	};
	Size sz = GetSize();
	w.DrawRect(0, 0, sz.cx, sz.cy, SColorLtFace);
	if(!editor) return;
	int fy = editor->GetFontSize().cy;
	int hy = fy >> 1;
	int y = 0;
	int i = editor->GetScrollPos().y;
	int cy = GetSize().cy;
	int lno = i;
	bool hi_if = (hilite_if_endif && (editor->highlight == CodeEditor::HIGHLIGHT_CPP
		|| editor->highlight == CodeEditor::HIGHLIGHT_JAVA));
	Vector<CodeEditor::IfState> previf;
	if(hi_if)
		previf <<= editor->ScanSyntax(i).ifstack;
	while(y < cy) {
		String b;
		int err = 0;
		int edit = 0;
		if(i < li.GetCount()) {
			const LnInfo& l = li[i];
			b = l.breakpoint;
			err = l.error;
			edit = l.edited;
			if(l.lineno >= 0)
				lno = l.lineno;
		}
		if(line_numbers && i < editor->GetLineCount()) {
			String n = AsString(i + 1);
			for(int q = 0; q < 4 && q < n.GetLength(); q++) {
				w.DrawImage(sz.cx - 8 - q * 6,
					y + (fy - CodeEditorImg::N0().GetSize().cy) / 2,
					numeri[n[n.GetLength() - 1 - q] - '0'],
					// CodeEditorImg::Vector[n[n.GetLength() - 1 - q] - '0' + CodeEditorImg::I_N0],
					Brown);
			}
		}
		if(hi_if) {
			Vector<CodeEditor::IfState> nextif;
			if(i < li.GetCount())
				nextif <<= editor->ScanSyntax(i + 1).ifstack;
			int pifl = previf.GetCount(), nifl = nextif.GetCount();
			int dif = max(pifl, nifl);
			if(--dif >= 0) {
				char p = (dif < pifl ? previf[dif].state : 0);
				char n = (dif < nifl ? nextif[dif].state : 0);
				int wd = min(2 * (dif + 1), sz.cx);
				int x = sz.cx - wd;
				Color cn = CodeEditor::SyntaxState::IfColor(n);
				if(p == n)
					w.DrawRect(x, y, 1, fy, cn);
				else {
					Color cp = CodeEditor::SyntaxState::IfColor(p);
					w.DrawRect(x, y, 1, hy, cp);
					w.DrawRect(x, y + hy, wd, 1, Nvl(cn, cp));
					w.DrawRect(x, y + hy, 1, fy - hy, cn);
					if(--dif >= 0) {
						x = sz.cx - min(2 * (dif + 1), sz.cx);
						if(!p)
							w.DrawRect(x, y, 1, hy, CodeEditor::SyntaxState::IfColor(dif < pifl ? previf[dif].state : 0));
						if(!n)
							w.DrawRect(x, y + hy, 1, fy - hy, CodeEditor::SyntaxState::IfColor(dif < nifl ? nextif[dif].state : 0));
					}
				}
			}
			previf = nextif;
		}
		if(edit > 0) {
			int width = CodeEditorImg::Breakpoint().GetWidth() >> 1;
			w.DrawRect(0, y, width, fy, LtBlue);
		}
		if(err > 0) {
			int width = CodeEditorImg::Breakpoint().GetWidth() >> 1;
			w.DrawRect(width, y, width, fy, err == 1 ? LtRed : Color(255, 175, 0));
		}

		if(!b.IsEmpty())
			sPaintImage(w, y, fy, b == "1"   ? CodeEditorImg::Breakpoint() :
			                      b == "\xe" ? CodeEditorImg::InvalidBreakpoint() :
			                                   CodeEditorImg::CondBreakpoint());
		for(int q = 0; q < 2; q++)
			if(i < li.GetCount() && ptrline[q] >= 0 && ptrline[q] == li[i].lineno)
				sPaintImage(w, y, fy, ptrimg[q]);
		y += fy;
		i++;
		lno++;
	}
}

void EditorBar::MouseMove(Point p, dword flags) {
	if(editor)
		editor->MouseMove(Point(0, p.y), flags);
}

void EditorBar::LeftDown(Point p, dword flags) {
	if(editor)
		editor->LeftDown(Point(0, p.y), flags);
}

String& EditorBar::PointBreak(int& y) {
	y = minmax(y / editor->GetFont().Info().GetHeight()
		+ editor->GetScrollPos().y, 0, editor->GetLineCount());
	return li.At(y).breakpoint;
}

void EditorBar::LeftDouble(Point p, dword flags) {
	if(!editor || !bingenabled) return;
	String& b = PointBreak(p.y);
	if(b.IsEmpty())
		b = "1";
	else
		b.Clear();
	WhenBreakpoint(p.y);
	Refresh();
}

void EditorBar::RightDown(Point p, dword flags) {
	return;
	if(!editor || !bingenabled) return;
	String& b = PointBreak(p.y);
	EditText(b, "Conditional breakpoint", "Condition");
	WhenBreakpoint(p.y);
	Refresh();
}

void EditorBar::InsertLines(int i, int count) {
	li.InsertN(minmax(i, 0, li.GetCount()), max(count, 0));
	Refresh();
}

void EditorBar::RemoveLines(int i, int count) {
	i = minmax(i, 0, li.GetCount());
	li.Remove(i, minmax(count, 0, li.GetCount() - i));
	Refresh();
}

void EditorBar::ClearLines() {
	li.Clear();
	li.Shrink();
	Refresh();
}

LineInfo EditorBar::GetLineInfo() const {
	LineInfo lf;
	int l = -2;
	for(int i = 0; i < li.GetCount(); i++) {
		const LnInfo& ln = li[i];
		if(!ln.breakpoint.IsEmpty() || ln.error || ln.edited) {
			LineInfoRecord& r = lf.Add();
			r.lineno = ln.lineno;
			r.count = 1;
			r.breakpoint = ln.breakpoint;
			r.error = ln.error;
			r.edited = ln.edited;
			l = -2;
		}
		else
		if(ln.lineno != l) {
			LineInfoRecord& r = lf.Add();
			r.lineno = l = ln.lineno;
			r.count = 1;
		}
		else
			lf.Top().count++;
		if(l >= 0) l++;
	}
	return lf;
}

void EditorBar::SetLineInfo(const LineInfo& lf, int total) {
	li.Clear();
	if(lf.GetCount() == 0) {
		for(int i = 0; i < total; i++)
			li.Add().lineno = i;
	}
	else {
		for(int i = 0; i < lf.GetCount() && li.GetCount() < total; i++) {
			const LineInfoRecord& r = lf[i];
			int l = r.lineno;
			for(int j = r.count; j-- && li.GetCount() < total;) {
				LnInfo& ln = li.Add();
				ln.lineno = l;
				ln.breakpoint = r.breakpoint;
				ln.error = r.error;
				ln.edited = r.edited;
				if(l >= 0) l++;
			}
		}
		while(li.GetCount() < total)
			li.Add().lineno = -1;
	}
}

void EditorBar::Renumber(int linecount) {
	li.SetCount(linecount);
	for(int i = 0; i < linecount; i++)
		li[i].lineno = i;
}

void EditorBar::ClearBreakpoints() {
	for(int i = 0; i < li.GetCount(); i++)
		li[i].breakpoint.Clear();
	Refresh();
}

void EditorBar::ValidateBreakpoints()
{
	for(int i = 0; i < li.GetCount(); i++)
		if(li[i].breakpoint[0] == 0xe)
			li[i].breakpoint = "1";
	Refresh();
}

String EditorBar::GetBreakpoint(int ln)
{
	return ln < li.GetCount() ? li[ln].breakpoint : Null;
}

void EditorBar::SetBreakpoint(int ln, const String& s)
{
	li.At(ln).breakpoint = s;
	WhenBreakpoint(ln);
}

void EditorBar::SetEdited(int ln, int edit)
{
	li.At(ln).edited = edit;
}

void EditorBar::SetError(int ln, int err)
{
	li.At(ln).error = err;
}

void EditorBar::ClearErrors(int line)
{
	for(int i = 0; i < li.GetCount(); i++) {
		if(li[i].error && (line < 0 || li[i].lineno == line)) {
			li[i].error = 0;
		}
	}
}

int  EditorBar::GetLineNo(int lineno) const {
	for(int i = 0; i < li.GetCount(); i++) {
		if(lineno <= li[i].lineno)
			return i;
	}
	return lineno;
}

int  EditorBar::GetNoLine(int line) const {
	int n = 0;
	for(int i = 0; i < li.GetCount(); i++) {
		if(li[i].lineno >= 0)
			n = li[i].lineno;
		if(i == line) return n;
	}
	return n;
}

void EditorBar::SetPtr(int line, const Image& img, int i)
{
	ASSERT(i >= 0 && i < 2);
	ptrline[i] = line;
	ptrimg[i] = img;
	Refresh();
}

void EditorBar::HidePtr()
{
	ptrline[0] = ptrline[1] = -1;
	Refresh();
}

void EditorBar::LineNumbers(bool b)
{
	line_numbers = b;
	Width(b ? 27 : 12);
	Refresh();
}

EditorBar::EditorBar() {
	LineNumbers(false);
	editor = NULL;
	bingenabled = true;
	hilite_if_endif = true;
	line_numbers = false;
}

EditorBar::~EditorBar()
{
}

END_UPP_NAMESPACE
