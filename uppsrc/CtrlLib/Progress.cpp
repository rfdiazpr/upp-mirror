#include "CtrlLib.h"

void ProgressIndicator::Paint(Draw& w) {
	Size sz = GetSize();
	int x0 = 0;
	int p0 = 0;
	int p = pxp;
	if(total <= 0) {
		int l = (max(sz.cx, sz.cy) - 4) & ~7;
		p0 = pxp - l / 4;
		if(p0 < 0) {
			x0 = (pxp - l / 4 + 65536) & 7;
			p0 = 0;
		}
		p = min(p - p0, max(sz.cx, sz.cy) - 4 - p0);
	}
	if(IsXPStyle() && !percent) {
		Color b = Blend(SColorShadow, SColorText);
		w.DrawRect(2, 0, sz.cx - 4, 1, b);
		w.DrawRect(2, 1, sz.cx - 4, 1, SColorPaper);
		w.DrawRect(2, sz.cy - 1, sz.cx - 4, 1, b);
		w.DrawRect(2, sz.cy - 2, sz.cx - 4, 1, SColorPaper);

		w.DrawRect(0, 2, 1, sz.cy - 4, b);
		w.DrawRect(1, 2, 1, sz.cy - 4, SColorPaper);
		w.DrawRect(sz.cx - 1, 2, 1, sz.cy - 4, b);
		w.DrawRect(sz.cx - 2, 2, 1, sz.cy - 4, SColorPaper);

		b = Blend(b, SColorShadow);

		w.DrawRect(0, 0, 1, 1, SColorFace);
		w.DrawRect(1, 0, 1, 1, SColorShadow);
		w.DrawRect(0, 1, 1, 1, SColorShadow);
		w.DrawRect(1, 1, 1, 1, b);

		w.DrawRect(sz.cx - 1, 0, 1, 1, SColorFace);
		w.DrawRect(sz.cx - 2, 0, 1, 1, SColorShadow);
		w.DrawRect(sz.cx - 1, 1, 1, 1, SColorShadow);
		w.DrawRect(sz.cx - 2, 1, 1, 1, b);


		w.DrawRect(0, sz.cy - 1, 1, 1, SColorFace);
		w.DrawRect(1, sz.cy - 1, 1, 1, SColorShadow);
		w.DrawRect(0, sz.cy - 2, 1, 1, SColorShadow);
		w.DrawRect(1, sz.cy - 2, 1, 1, b);

		w.DrawRect(sz.cx - 1, sz.cy - 1, 1, 1, SColorFace);
		w.DrawRect(sz.cx - 2, sz.cy - 1, 1, 1, SColorShadow);
		w.DrawRect(sz.cx - 1, sz.cy - 2, 1, 1, SColorShadow);
		w.DrawRect(sz.cx - 2, sz.cy - 2, 1, 1, b);

		sz -= 4;
		if(sz.cx > sz.cy) {
			int d = sz.cy / 2;
			int i = 0;
			for(;i < sz.cy; i++) {
				Color c = Blend(LtGreen, SColorPaper, abs((i - d) * 255 / (d + 1)));
				w.DrawRect(2 + p0, i + 2, p, 1, c);
			}
			int x = -1 + x0;
			while(x < p) {
				w.DrawRect(x + 2 + p0, 2, 2, sz.cy, SColorPaper);
				x += 8;
			}
			w.DrawRect(p + 2 + p0, 2, sz.cx - p - p0, sz.cy, SColorPaper);
			w.DrawRect(2, 2, p0, sz.cy, SColorPaper);
		}
		else {
			int d = sz.cx / 2;
			int i = 0;
			for(;i < sz.cx; i++) {
				Color c = Blend(LtGreen, SColorPaper, abs((i - d) * 255 / (d + 1)));
				w.DrawRect(i + 2, 2 + sz.cy - p, 1, p, c);
			}
			int y = sz.cy + 1;
			while(y > sz.cy - p) {
				w.DrawRect(2, y, sz.cx, 2, SColorPaper);
				y -= 8;
			}
			w.DrawRect(2, 2, sz.cx, sz.cy - p, SColorPaper);
		}
	}
	else {
		DrawBorder(w, sz, InsetBorder());
		sz -= 4;
		Rect r1, r2, r3;
		r1 = r2 = r3 = RectC(2, 2, sz.cx, sz.cy);
		if(sz.cx > sz.cy) {
			r1.right = r2.left = min(p, sz.cx) + 2 + p0;
			r3.right = 2 + p0;
			r3.bottom = r1.bottom;
		}
		else {
			r2.bottom = r1.top = sz.cy - min(p, sz.cy) + 2 + p0;
			r3.bottom = 2 + p0;
			r3.right = r1.right;
		}
		w.DrawRect(r1, SColorHighlight);
		w.DrawRect(r2, SColorPaper);
		w.DrawRect(r3, SColorPaper);
		if(percent) {
			String pt = Format("%d %%", 100 * actual / max(total, 1));
			Size psz = GetTextSize(pt, StdFont());
			int px = (sz.cx - psz.cx) / 2 + 2;
			int py = (sz.cy - psz.cy) / 2 + 2;
			w.Clip(r1);
			w.DrawText(px, py, pt, StdFont(), SWhite);
			w.End();
			w.Clip(r2);
			w.DrawText(px, py, pt, StdFont(), SBlack);
			w.End();
		}
	}
}

void ProgressIndicator::Layout() {
	Set(actual, total);
}

void ProgressIndicator::Set(int _actual, int _total) {
	actual = _actual;
	total = _total;
	Size sz = GetSize();
	int p;
	if(total <= 0) {
		int l = max(1, (max(sz.cx, sz.cy) - 4) & ~7);
		int p = GetTickCount() / 15 % (l + l / 4);
		if(pxp != p) {
			pxp = p;
			Refresh();
		}
		return;
	}
	else {
		sz -= 4;
		int l = max(1, max(sz.cx, sz.cy));
		p = total ? min(actual * l / total, l) : 0;
	}
	if(p != pxp) {
		pxp = p;
		Refresh();
	}
}

ProgressIndicator::ProgressIndicator() {
	NoWantFocus();
	total = actual = 0;
	percent = false;
}

ProgressIndicator::~ProgressIndicator() {}

void Progress::Reset() {
	pos = 0;
	total = 0;
	cancel = false;
	granularity = 50;
	set_time = show_time = GetTickCount(); // + 300;
}

void Progress::Init() {
	CtrlLayout(*this, t_("Working..."));
	stop <<= callback(this, &Progress::Cancel);
	info.SetAlign(ALIGN_CENTER);
	owner = NULL;
	Reset();
}

Progress::Progress() {
	Init();
}

Progress::Progress(Ctrl *owner) {
	Init();
	SetOwner(owner);
}

Progress::Progress(const char *s, int total) {
	Init();
	text = s;
	SetTotal(total);
}

Progress::Progress(Ctrl *owner, const char *s, int total) {
	Init();
	text = s;
	SetTotal(total);
	SetOwner(owner);
}

Progress::~Progress() {}

void Progress::Create() {
	if(IsOpen() || cancel) return;
	stop.Hide();
	if(owner)
		Open(owner);
	else
		Open();
	SetFocus();
	Show();
	modality.Begin(this);
	if(total) Set(pos, total);
	Setxt();
	Sync();
	Process();
}

void Progress::Process()
{
	if(!IsOpen()) {
		dword t = GetTickCount();
		if((int)(t - show_time) >= granularity) {
			Create();
			show_time = t;
		}
	}
	GuiSleep(0);
	ProcessEvents();
}

void Progress::Setxt()
{
	info = Format(text, pos);
	Process();
}

void Progress::Set(int apos, int atotal) {
	pos = apos;
	total = atotal;
	dword t = GetTickCount();
	if((int)(t - set_time) < granularity)
		return;
	set_time = t;
	while(atotal > 30000) {
		atotal >>= 8;
		apos >>= 8;
	}
	pi.Set(apos, atotal);
	Setxt();
	Sync();
	Process();
}

void Progress::SetTotal(int atotal) {
	Set(pos, atotal);
}

void Progress::SetPos(int apos) {
	Set(apos, total);
}

void Progress::Step(int n)
{
	Set(pos + n, total);
}

void Progress::Close()
{
	modality.End();
	TopWindow::Close();
}

void Progress::Cancel() {
	cancel = true;
	Close();
}

bool Progress::Canceled()
{
	stop.Show();
	Process();
	return cancel;
}

bool Progress::SetCanceled(int pos, int total)
{
	stop.Show();
	Set(pos, total);
	return cancel;
}

bool Progress::SetPosCanceled(int pos)
{
	stop.Show();
	SetPos(pos);
	return cancel;
}

bool Progress::StepCanceled(int n)
{
	stop.Show();
	Step(n);
	return cancel;
}
