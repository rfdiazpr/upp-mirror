#include "CtrlCore.h"
#pragma hdrstop

#define LLOG(x)    // LOG(x)
#define LLOGEND()  // LOGEND()
#define LTIMING(X) // TIMING(x)

void Ctrl::RefreshFrame(const Rect& r) {
	if(!IsOpen() || !IsVisible() || r.IsEmpty()) return;
	LLOG("RefreshRect " << Name() << ' ' << r);
	if(parent) {
		if(InFrame())
			parent->RefreshFrame(r + GetRect().TopLeft());
		else
			parent->Refresh(r + GetRect().TopLeft());
	}
	else {
		WndInvalidateRect(r);
#ifdef PLATFORM_WIN32
		LLOG("UpdateRect: " << GetWndUpdateRect() << ' ' << Name());
#endif
	}
}

void Ctrl::Refresh(const Rect& area) {
	if(fullrefresh || !IsVisible() || !IsOpen()) return;
	LLOG("Refresh " << Name() << ' ' <<  area);
	RefreshFrame((area + GetView().TopLeft()) & GetView());
}

void Ctrl::Refresh() {
	if(fullrefresh || !IsVisible() || !IsOpen()) return;
	LLOG("Refresh " << Name() << " full:" << fullrefresh);
	Refresh(Rect(GetSize()));
}

void Ctrl::Refresh(int x, int y, int cx, int cy) {
	Refresh(RectC(x, y, cx, cy));
}

void Ctrl::RefreshFrame(int x, int y, int cx, int cy) {
	RefreshFrame(RectC(x, y, cx, cy));
}

void Ctrl::RefreshFrame() {
	LLOG("RefreshFrame " << Name());
	RefreshFrame(Rect(GetRect().Size()));
}

void  Ctrl::ScrollRefresh(const Rect& r, int dx, int dy)
{
	if(!IsOpen() || !IsVisible() || r.IsEmpty()) return;
	int tdx = tabs(dx), tdy = tabs(dy);
	if(dx) WndInvalidateRect(RectC(dx >= 0 ? r.left : r.right - tdx, r.top - tdy, tdx, r.Height()));
	if(dy) WndInvalidateRect(RectC(r.left - tdx, dy >= 0 ? r.top : r.bottom - tdy, r.Width(), tdy));
}

bool Ctrl::AddScroll(const Rect& sr, int dx, int dy)
{
	if(!top)
		return true;
	for(int i = 0; i < top->scroll.GetCount(); i++) {
		Scroll& sc = top->scroll[i];
		if(sc.rect == sr && sgn(dx) == sgn(sc.dx) && sgn(dy) == sgn(sc.dy)) {
			sc.dx += dx;
			sc.dy += dy;
			ScrollRefresh(sc.rect, sc.dx, sc.dy);
			return false;
		}
		if(sc.rect.Intersects(sr)) {
			sc.rect |= sr;
			sc.dx = sc.dy = 0;
			WndInvalidateRect(sc.rect);
			return true;
		}
	}
	Scroll& sc = top->scroll.Add();
	sc.rect = sr;
	sc.dx = dx;
	sc.dy = dy;
	ScrollRefresh(sc.rect, sc.dx, sc.dy);
	return false;
}

Rect  Ctrl::GetClippedView()
{
	Rect sv = GetScreenView();
	Rect view = sv;
	Ctrl *q = parent;
	while(q) {
		view &= q->GetScreenView();
		q = q->parent;
	}
	return view - GetScreenRect().TopLeft();
}

void  Ctrl::ScrollView(const Rect& _r, int dx, int dy)
{
	if(IsFullRefresh())
		return;
	Size vsz = GetSize();
	dx = sgn(dx) * min(abs(dx), vsz.cx);
	dy = sgn(dy) * min(abs(dy), vsz.cy);
	Rect r = _r & vsz;
	Ctrl *w = GetTopCtrl();
	if(!w || !w->top) return;
	Rect view = InFrame() ? GetView() : GetClippedView();
	Rect sr = (r + view.TopLeft()) & view;
	sr += GetScreenRect().TopLeft() - w->GetScreenRect().TopLeft();
	if(w->AddScroll(sr, dx, dy))
		Refresh();
	else
		for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
			if(q->InView()) {
				Rect cr = q->GetRect() & r;
				if(!cr.IsEmpty()) {
					Refresh(cr);
					Refresh(cr + Point(dx, dy));
				}
			}
}

void  Ctrl::ScrollView(int x, int y, int cx, int cy, int dx, int dy) {
	ScrollView(RectC(x, y, cx, cy), dx, dy);
}

void  Ctrl::ScrollView(int dx, int dy) {
	ScrollView(Rect(GetSize()), dx, dy);
}

void  Ctrl::SyncScroll()
{
	if(parent || !top)
		return;
	Vector<Scroll> scroll = top->scroll;
	top->scroll.Clear();
	if(IsFullRefresh())
		return;
	for(int i = 0; i < scroll.GetCount(); i++) {
		Scroll& sc = scroll[i];
		if(abs(sc.dx) > 3 * sc.rect.Width() / 4 || abs(sc.dy) > 3 * sc.rect.Height() / 4) {
			LLOG("Sync scroll Invalidate rect" << sc.rect);
			WndInvalidateRect(sc.rect);
		}
		else
		if(sc.dx || sc.dy) {
			LLOG("WndScrollView " << sc.rect);
			WndScrollView(sc.rect, sc.dx, sc.dy);
		}
	}
}

Rect Ctrl::GetOpaqueRect()
{
	return IsTransparent() ? Rect(0, 0, 0, 0) : GetSize();
}

#ifdef _DEBUG
struct sDrawLevelCheck {
	int   lvl;
	Draw& w;
	sDrawLevelCheck(Draw& w) : w(w), lvl(w.GetCloffLevel()) {}
	~sDrawLevelCheck() { ASSERT(lvl == w.GetCloffLevel()); }
};

#define LEVELCHECK(w)    sDrawLevelCheck __(w)
#else
#define LEVELCHECK(w)
#endif

void Ctrl::CtrlPaint(Draw& w, const Rect& clip) {
	Rect rect = GetRect().GetSize();
	if(!IsShown() || rect.IsEmpty() || clip.IsEmpty() || !clip.Intersects(rect))
		return;
	Ctrl *q;
	LLOG("CtrlPaint " << Name() << BeginIndent);
	LLOG("clip: " << clip);
	LLOG("GetRect(): " << GetRect());
	Rect view = rect;
	for(int i = 0; i < frame.GetCount(); i++) {
		LEVELCHECK(w);
		frame[i].frame->FramePaint(w, view);
		view = frame[i].view;
	}
	if(!view.IsEmpty()) {
		if(view.Intersects(clip) && w.IsPainting(view)) {
			LOG("Painting: " << Name());
			LEVELCHECK(w);
			w.Clipoff(view);
			Paint(w);
			w.End();
		}
		bool hasviewctrls = false;
		for(q = firstchild; q; q = q->next)
			if(q->IsShown())
				if(q->InFrame()) {
					LEVELCHECK(w);
					Point off = q->GetRect().TopLeft();
					w.Offset(off);
					q->CtrlPaint(w, clip - off);
					w.End();
				}
				else
					hasviewctrls = true;
		if(hasviewctrls) {
			Rect cl = clip & view;
			w.Clip(cl);
			for(q = firstchild; q; q = q->next)
				if(q->IsShown() && q->InView()) {
					LEVELCHECK(w);
					Point off = q->GetRect().TopLeft() + view.TopLeft();
					w.Offset(off);
					q->CtrlPaint(w, cl - off);
					w.End();
				}
			w.End();
		}
	}
	LLOGEND();
}

void Reduce(Rect& r, Rect rr)
{

	rr = r & rr;
	if(rr.IsEmpty())
		return;
	int width = r.GetWidth();
	int height = r.GetHeight();
	int ts = (rr.top - r.top) * width;
	int bs = (r.bottom - rr.bottom) * width;
	int ls = (rr.left - r.left) * height;
	int rs = (r.right - rr.right) * height;
	if(ls > rs)
		if(ls > ts)
			if(ls > bs)
				r.right = rr.left;
			else
				r.top = rr.bottom;
		else
			if(ts > bs)
				r.bottom = rr.top;
			else
				r.top = rr.bottom;
	else
		if(rs > ts)
			if(rs > bs)
				r.left = rr.right;
			else
				r.top = rr.bottom;
		else
			if(ts > bs)
				r.bottom = rr.top;
			else
				r.top = rr.bottom;
}

int sShowRepaint;

void Ctrl::ShowRepaint(int q)
{
	sShowRepaint = q;
}

void ShowRepaintRect(Draw& w, const Rect& r, Color c)
{
	if(sShowRepaint) {
		w.DrawRect(r, LtBlue());
		Draw::Flush();
		Sleep(sShowRepaint);
	}
}

void Ctrl::PaintOpaqueAreas(Draw& w, Point offset, const Rect& clip)
{
	Rect r = parent ? GetRect() + offset : GetSize();
	Point off = r.TopLeft();
	Point viewpos = off + GetView().TopLeft();
	if(backpaint == EXCLUDEPAINT) {
		w.ExcludeClip(r);
		return;
	}
	if(!IsShown() || r.IsEmpty() || !w.IsPainting(r))
		return;
	Rect cview = clip & (GetView() + off);
	for(Ctrl *q = firstchild; q; q = q->next)
		q->PaintOpaqueAreas(w, q->InView() ? viewpos : off, q->InView() ? cview : clip);
	Rect opaque = (GetOpaqueRect() + viewpos) & clip;
	if(opaque.IsEmpty())
		return;
	Rect reduce = Null;
	if(parent)
		for(Ctrl *q = next; q; q = q->next)
			if(q->IsShown()) {
				Rect rr = q->GetRect() + offset;
				if(rr.Intersects(opaque))
					if(IsNull(reduce))
						reduce = rr;
					else
						reduce.Union(rr);
			}
	for(Ctrl *q = firstchild; q; q = q->GetNext())
		if(q->IsShown() && q->InView()) {
			Rect rr = q->GetRect() + viewpos;
			if(IsNull(reduce))
				reduce = rr;
			else
				reduce.Union(rr);
		}
	Reduce(opaque, reduce);
	if(opaque.IsEmpty())
		return;
	if(backpaint == FULLBACKPAINT) {
		ShowRepaintRect(w, opaque, LtBlue());
		BackDraw bw;
		bw.Create(w, opaque.GetSize());
		bw.Offset(viewpos - opaque.TopLeft());
		bw.SetPaintingDraw(w, opaque.TopLeft());
		{
			LEVELCHECK(bw);
			Paint(bw);
		}
		bw.Put(w, opaque.TopLeft());
	}
	else {
		w.Clip(opaque);
		ShowRepaintRect(w, opaque, Green());
		w.Offset(viewpos);
		{
			LEVELCHECK(w);
			Paint(w);
		}
		w.End();
		w.End();
	}
	w.ExcludeClip(opaque);
}

void Ctrl::UpdateArea(Draw& draw, const Rect& clip)
{
	if(IsPanicMode())
		return;
	LTIMING("UpdateArea");
	if(backpaint != FULLBACKPAINT)
		PaintOpaqueAreas(draw, Point(0, 0), clip);
	if(backpaint) {
		ShowRepaintRect(draw, GetSize(), Magenta());
		BackDraw bw;
		bw.Create(draw, clip.GetSize());
		bw.Offset(-clip.TopLeft());
		bw.SetPaintingDraw(draw, clip.TopLeft());
		CtrlPaint(bw, clip);
		bw.Put(draw, clip.TopLeft());
	}
	else
		CtrlPaint(draw, clip);

	RemoveFullRefresh();
}

void Ctrl::RemoveFullRefresh()
{
	fullrefresh = false;
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
		q->RemoveFullRefresh();
}

Ctrl *Ctrl::GetTopRect(Rect& r, bool inframe)
{
	if(!inframe)
		r.Offset(GetView().TopLeft());
	if(parent) {
		r.Offset(GetRect().TopLeft());
		return parent->GetTopRect(r, InFrame());
	}
	return this;
}

void  Ctrl::DoSync(Ctrl *q, Rect r, bool inframe)
{
	ASSERT(q);
	LLOG("DoSync " << ::Name(q) << " " << r);
	Ctrl *top = q->GetTopRect(r, inframe);
	top->SyncScroll();
	top->WndUpdate(r);
}

void  Ctrl::Sync()
{
	LLOG("Sync " << Name());
	if(!parent && IsOpen()) {
		LLOG("Sync UpdateWindow " << Name());
		SyncScroll();
		WndUpdate();
	}
	else
	if(parent)
		DoSync(parent, GetRect(), inframe);
	SyncCaret();
}

void Ctrl::Sync(const Rect& sr)
{
	LLOG("Sync " << Name() << "   " << sr);
	DoSync(this, sr, true);
	SyncCaret();
}

void Ctrl::DrawCtrlWithParent(Draw& w, int x, int y)
{
	if(parent) {
		Rect r = GetRect();
		Ctrl *top = parent->GetTopRect(r, inframe);
		w.Clip(x, y, r.Width(), r.Height());
		w.Offset(x - r.left, y - r.top);
		top->UpdateArea(w, r);
		w.End();
		w.End();
	}
	else
		DrawCtrl(w, x, y);
}

void Ctrl::DrawCtrl(Draw& w, int x, int y)
{
	w.Offset(x, y);
	UpdateArea(w, GetRect().GetSize());
	w.End();
}
