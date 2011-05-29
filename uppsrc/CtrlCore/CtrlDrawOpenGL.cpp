#include "CtrlCore.h"

NAMESPACE_UPP

#define LLOG(x)     // DLOG(x)
#define LTIMING(x)  // TIMING(x)

#ifdef flagOPENGL

bool Ctrl::globalbackpaint;

void Ctrl::RefreshFrame(const Rect& r) {
//	HWND hwnd = GetHWND();
//	::InvalidateRect(hwnd, NULL, false);
}

void Ctrl::Refresh(const Rect& area) {
	RefreshFrame(area);
}

void Ctrl::Refresh() {
	RefreshFrame(Rect());
}

void Ctrl::Refresh(int x, int y, int cx, int cy) {
	Refresh();
}

void Ctrl::RefreshFrame(int x, int y, int cx, int cy) {
	Refresh();
}

void Ctrl::RefreshFrame() {
	Refresh();
}

void  Ctrl::ScrollRefresh(const Rect& r, int dx, int dy)
{
}

bool Ctrl::AddScroll(const Rect& sr, int dx, int dy)
{
	return false;
}

Rect  Ctrl::GetClippedView()
{
	GuiLock __;
	Rect sv = GetScreenView();
	Rect view = sv;
	Ctrl *q = parent;
	Ctrl *w = this;
	while(q) {
		view &= w->InFrame() ? q->GetScreenRect() : q->GetScreenView();
		w = q;
		q = q->parent;
	}
	return view - GetScreenRect().TopLeft();
}

void Ctrl::ScrollCtrl(Top *top, Ctrl *q, const Rect& r, Rect cr, int dx, int dy)
{
}

void  Ctrl::ScrollView(const Rect& _r, int dx, int dy)
{
}

void  Ctrl::ScrollView(int x, int y, int cx, int cy, int dx, int dy) {
}

void  Ctrl::ScrollView(int dx, int dy) {
}

void  Ctrl::SyncScroll()
{
}

Rect Ctrl::GetOpaqueRect() const
{
	return IsTransparent() ? Rect(0, 0, 0, 0) : GetSize();
}

Rect Ctrl::GetVoidRect() const
{
	return Rect(0, 0, 0, 0);
}


#ifdef _DEBUG
struct sDrawLevelCheck {
	Draw&        w;
	int          lvl;
	const Ctrl  *q;

	void Check() {
		ASSERT_(lvl == w.GetCloffLevel(), "Draw::Begin/End mismatch for " + UPP::Name(q));
	}

	sDrawLevelCheck(Draw& w, const Ctrl *q) : w(w), lvl(w.GetCloffLevel()), q(q) {}
	~sDrawLevelCheck() { Check(); }
};

#define LEVELCHECK(w, q)    sDrawLevelCheck _x_(w, q)
#define DOLEVELCHECK        _x_.Check();
#else
#define LEVELCHECK(w, q)
#define DOLEVELCHECK
#endif

void Ctrl::PaintCaret(Draw& w)
{
	GuiLock __;
	if(this == caretCtrl && WndCaretVisible)
		w.DrawRect(caretx, carety, caretcx, caretcy, Black);
}

void Ctrl::CtrlPaint(Draw& w, const Rect& clip, Ctrl* debugctrl, int depth) {
	GuiLock __;
	LEVELCHECK(w, this);
	LTIMING("CtrlPaint");
	
	Rect rect = GetRect().GetSize();
	Rect orect = rect.Inflated(overpaint);
	if(!IsShown() || orect.IsEmpty() || clip.IsEmpty() || !clip.Intersects(orect))
		return;
	
	SystemDraw& sw = (SystemDraw&) w;
		
	if(this == debugctrl)
	{
		#if CLIP_MODE == 2
		glDisable(GL_STENCIL_TEST);
		#endif
		sw.alpha = 255;
		sw.angle = 0;
		sw.FlatView();
	}
	else
	{
		glPushMatrix();
		ApplyTransform(TS_BEFORE_CTRL_PAINT);
	}
		
	Ctrl *q;
	Rect view = rect;
	for(int i = 0; i < frame.GetCount(); i++) {
		LEVELCHECK(w, NULL);
		frame[i].frame->FramePaint(w, view);
		view = frame[i].view;
	}
	Rect oview = view.Inflated(overpaint);
	bool hasviewctrls = false;
	bool viewexcluded = false;
	
	for(q = firstchild; q; q = q->next)
		if(q->IsShown())
			if(q->InFrame()) {
				if(!viewexcluded && IsTransparent() && q->GetRect().Intersects(view)) {
					w.Begin();
					w.ExcludeClip(view);
					viewexcluded = true;
				}
				LEVELCHECK(w, q);
				Point off = q->GetRect().TopLeft();
				w.Offset(off);
				q->CtrlPaint(w, clip - off, debugctrl, depth + 1);
				w.End();
			}
			else
				hasviewctrls = true;

	if(viewexcluded)
		w.End();
	DOLEVELCHECK;
	
	if(!oview.IsEmpty() && oview.Intersects(clip)) {
		glPushMatrix();
		LEVELCHECK(w, this);
		if(overpaint) {
			if(cliptobounds)
				w.Clip(oview);
			w.Offset(view.left, view.top);
			Paint(w);
			PaintCaret(w);
			w.End();
			if(cliptobounds)
				w.End();
		}
		else {
			if(cliptobounds)
				w.Clipoff(view);
			Paint(w);
			PaintCaret(w);
			if(cliptobounds)
				w.End();
		}
		glPopMatrix();
	}
	
	if(hasviewctrls && !view.IsEmpty()) {
		Rect cl = clip & view;
		for(q = firstchild; q; q = q->next)
			if(q->IsShown() && q->InView()) {
				Rect rr(q->popup ? clip : cl);
				LEVELCHECK(w, q);
				if(q->cliptobounds)
					w.Clip(rr);
				Rect qr = q->GetRect();
				Point off = qr.TopLeft() + view.TopLeft();
				Rect ocl = cl - off;
				if(ocl.Intersects(Rect(qr.GetSize()).Inflated(overpaint))) {
					w.Offset(off);
					q->CtrlPaint(w, rr - off, debugctrl, depth + 1);
					w.End();
				}
				if(q->cliptobounds)
					w.End();
			}
	}

	if(this != debugctrl)
	{
		ApplyTransform(TS_AFTER_CTRL_PAINT);
		glPopMatrix();
	}
	else
	{
		#if CLIP_MODE == 2
		glEnable(GL_STENCIL_TEST);
		#endif
	}
}

int sShowRepaint;

void Ctrl::ShowRepaint(int q)
{
	sShowRepaint = q;
}

void ShowRepaintRect(SystemDraw& w, const Rect& r, Color c)
{
	if(sShowRepaint) {
		w.DrawRect(r, c);
		SystemDraw::Flush();
		Sleep(sShowRepaint);
	}
}

bool Ctrl::PaintOpaqueAreas(SystemDraw& w, const Rect& r, const Rect& clip, bool nochild)
{
	return true;
}

inline int Area(const Rect& r)
{
	return r.GetHeight() * r.GetWidth();
}

void CombineArea(Vector<Rect>& area, const Rect& r)
{
}

void Ctrl::GatherTransparentAreas(Vector<Rect>& area, SystemDraw& w, Rect r, const Rect& clip)
{
}

Ctrl *Ctrl::FindBestOpaque(const Rect& clip)
{
	return NULL;
}

void Ctrl::UpdateArea0(SystemDraw& draw, const Rect& clip, int backpaint)
{
	CtrlPaint(draw, clip);
}

void SweepMkImageCache();

void Ctrl::UpdateArea(SystemDraw& draw, const Rect& clip)
{
	GuiLock __;
	if(IsPanicMode())
		return;
	RemoveFullRefresh();
	Point sp = GetScreenRect().TopLeft();
	Ctrl *b = FindBestOpaque(clip + sp);
	if(b) {
		Point p = b->GetScreenRect().TopLeft() - sp;
		draw.Offset(p);
		b->UpdateArea0(draw, clip.Offseted(-p), backpaint);
		draw.End();
	}
	else
		UpdateArea0(draw, clip, backpaint);
	SweepMkImageCache();
}

void Ctrl::RemoveFullRefresh()
{
	GuiLock __;
	fullrefresh = false;
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
		q->RemoveFullRefresh();
}

Ctrl *Ctrl::GetTopRect(Rect& r, bool inframe)
{
	GuiLock __;
	if(!inframe) {
		r &= Rect(GetSize());
		r.Offset(GetView().TopLeft());
	}
	if(parent) {
		r.Offset(GetRect().TopLeft());
		return parent->GetTopRect(r, InFrame());
	}
	return this;
}

void  Ctrl::DoSync(Ctrl *q, Rect r, bool inframe)
{
}

void  Ctrl::Sync()
{
}

void Ctrl::Sync(const Rect& sr)
{
}

void Ctrl::DrawCtrlWithParent(Draw& w, int x, int y)
{
	GuiLock __;
	if(parent) {
		Rect r = GetRect();
		Ctrl *top = parent->GetTopRect(r, inframe);
		w.Clip(x, y, r.Width(), r.Height());
		w.Offset(x - r.left, y - r.top);
		SystemDraw *ws = dynamic_cast<SystemDraw *>(&w);
		if(ws)
			top->UpdateArea(*ws, r);
		w.End();
		w.End();
	}
	else
		DrawCtrl(w, x, y);
}

void Ctrl::DrawCtrl(Draw& w, int x, int y)
{
	GuiLock __;
	w.Offset(x, y);
	
	SystemDraw *ws = dynamic_cast<SystemDraw *>(&w);
	if(ws)
		UpdateArea(*ws, GetRect().GetSize());

	w.End();
}

void Ctrl::SyncMoves()
{
}

Ctrl& Ctrl::BackPaintHint()
{
	return *this;
}

void  Ctrl::GlobalBackPaint(bool b)
{
}

void  Ctrl::GlobalBackPaintHint()
{
}

END_UPP_NAMESPACE

#endif