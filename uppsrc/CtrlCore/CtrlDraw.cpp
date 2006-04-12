#include "CtrlCore.h"
#pragma hdrstop

#ifdef _DEBUG
int  Ctrl::ShowRepaint;
#endif

#define LLOG(x)    // LOG(x)
#define LLOGEND()  // LOGEND()

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
	Rect view = GetView();
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

Vector<Rect> Ctrl::GetTransparentFrameRects()
{
	Vector<Rect> r;
	Subtract(GetRect().Size(), GetView(), r);
	return r;
}

Vector<Rect> Ctrl::GetOpaqueFrameRects()
{
	return Vector<Rect>();
}

Vector<Rect> Ctrl::GetTransparentViewRects()
{
	return Vector<Rect>() << GetSize();
}

Vector<Rect> Ctrl::GetOpaqueViewRects()
{
	return Vector<Rect>();
}

inline bool In(const Rect& pr, const Rect& vr) {
	return pr.left >= vr.left && pr.right <= vr.right &&
		   pr.top >= vr.top && pr.bottom <= vr.bottom;
}

void  Ctrl::DoCtrlPaint(Draw *w, Ctrl *q, Rect pr) {
	Rect r = q->GetRect();
	pr &= r;
	if(!pr.IsEmpty()) {
		if(w->Clipoff(r)) {
			pr -= r.TopLeft();
			q->CtrlPaint(*w, pr);
		}
		w->End();
	}
}

bool Ctrl::ShouldPaint(Draw& w)
{
	if(!IsTransparent())
		return true;
	Point off = GetView().TopLeft();
	Vector<Rect> op = GetOpaqueViewRects();
	for(int i = 0; i < op.GetCount(); i++)
		if(w.IsPainting(op[i] + off))
			return true;
	op = GetTransparentViewRects();
	for(int i = 0; i < op.GetCount(); i++)
		if(w.IsPainting(op[i] + off))
			return true;
	op = GetOpaqueFrameRects();
	for(int i = 0; i < op.GetCount(); i++)
		if(w.IsPainting(op[i]))
			return true;
	op = GetTransparentFrameRects();
	for(int i = 0; i < op.GetCount(); i++)
		if(w.IsPainting(op[i]))
			return true;
	return false;
}

void Ctrl::CtrlPaint(Draw& pw, Rect pr) {
	if(!IsShown() || GetRect().IsEmpty()) return;
#ifdef _DEBUG
	TimeStop();
#endif
	if(backpaint == EXCLUDEPAINT) return;
	Ctrl *q;
	Draw *w = &pw;
	BackDraw bw;
	Rect rect = GetRect();
	Rect cbox = pw.GetClip();
	LLOG("CtrlPaint " << Name() << ' ' << pw.GetClip() << ", pr: " << pr << ", cbox: " << cbox
	     << BeginIndent);
	if(!pw.IsBack()) {
		Rect r = pw.GetClip();
		if(!IsTransparent() && (backpaint == FULLBACKPAINT || r.Width() * r.Height() < AutoBackPaintAreaSize)) {
			bw.Create(pw, cbox.Size());
		#ifdef _DEBUG
			bw.DrawRect(rect.Size(), LtRed);
		#endif
			bw.Offset(-cbox.TopLeft());
			bw.SetPaintingDraw(pw, cbox.TopLeft());
			w = &bw;
		}
	}
#ifdef _DEBUG
	if(ShowRepaint && !parent) {
		pw.DrawRect(pw.GetClip(), w == &bw ? LtGreen : LtBlue);
		Draw::Flush();
		Sleep(ShowRepaint);
	}
#endif
	Rect view = GetView();
	Rect r = rect.Size();
	bool fp = !In(pr, view);
	for(q = firstchild; q; q = q->next)
		if(q->InFrame())
			DoCtrlPaint(w, q, pr);
	for(int i = 0; i < frame.GetCount(); i++) {
	#ifdef _DEBUG
		int lvl = w->GetCloffLevel();
	#endif
		if(fp) frame[i].frame->FramePaint(*w, r);
	#ifdef _DEBUG
		ASSERT(lvl == w->GetCloffLevel());
	#endif
		r = frame[i].view;
	}
	if(!view.IsEmpty()) {
		pr &= r;
		if(!pr.IsEmpty()) {
			if(w->Clipoff(r)) {
				pr -= r.TopLeft();
				if(!w->IsBack()) {
					w->Begin();
					for(q = firstchild; q; q = q->next)
						if(q->InView() && q->IsShown())
							if(q->IsTransparent()) {
								Point off = q->GetRect().TopLeft();
								Vector<Rect> op = q->GetOpaqueFrameRects();
								for(int i = 0; i < op.GetCount(); i++)
									w->ExcludeClip(op[i] + off);
								off += q->GetView().TopLeft();
								op = q->GetOpaqueViewRects();
								for(int i = 0; i < op.GetCount(); i++)
									w->ExcludeClip(op[i] + off);
							}
							else
								w->ExcludeClip(q->GetRect());
				}
			#ifdef _DEBUG
				int lvl = w->GetCloffLevel();
			#endif
				if(ShouldPaint(*w)) {
					LLOG("Paint " << Name() << ' ' << w->GetClip());
					LLOG("      pr " << pr);
					Paint(*w);
				}
			#ifdef _DEBUG
				ASSERT(lvl == w->GetCloffLevel());
			#endif
				if(!w->IsBack())
					w->End();
				for(q = firstchild; q; q = q->next)
					if(q->InView() && q->IsShown())
						DoCtrlPaint(w, q, pr);
			}
			w->End();
		}
	}
	if(w == &bw)
		bw.Put(pw, cbox.TopLeft());
#ifdef _DEBUG
	LLOG(Name() << " Paint time: " << TimeStop());
#endif
	LLOGEND();
}

void Ctrl::GatherTrect(Point offset, Vector<Rect>& tr, Vector<Rect>& er, bool all) {
	Point vo = GetView().TopLeft() + offset;
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext()) {
		Rect r = q->GetRect() + (q->inframe ? offset : vo);
		if(q->IsShown())
			if(q->backpaint == EXCLUDEPAINT)
				er.Add(r);
			else
			if(q->IsTransparent() && (q->backpaint || all)) {
				LLOG("GATHER: " << ::Name(q) << "  " << r);
				Point off = r.TopLeft();
				Vector<Rect> op = q->GetTransparentFrameRects();
				for(int i = 0; i < op.GetCount(); i++)
					tr.Add(op[i] + off);
				off += q->GetView().TopLeft();
				op = q->GetTransparentViewRects();
				for(int i = 0; i < op.GetCount(); i++)
					tr.Add(op[i] + off);
			}
			else
				q->GatherTrect(r.TopLeft(), tr, er, all);
	}
}

void Ctrl::UpdateArea(Draw& draw, const Rect& paintrect)
{
	if(IsPanicMode())
		return;
	if(backpaint != FULLBACKPAINT && !draw.IsBack()) {
		int i;
		Vector<Rect> tr;
		Vector<Rect> er;
		Vector<bool> in;
		GatherTrect(Point(0, 0), tr, er, backpaint == TRANSPARENTBACKPAINT);
		in.SetCount(tr.GetCount(), true);
		Vector<int> sqr;
		sqr.SetCount(tr.GetCount());
		for(i = 0; i < tr.GetCount(); i++) {
			sqr[i] = tr[i].Width() * tr[i].Height();
			if((tr[i] & paintrect).IsEmpty())
				in[i] = false;
		}
		for(i = 0; i < tr.GetCount(); i++)
			if(in[i])
				for(int j = i + 1; j < tr.GetCount(); j++) {
					Rect r = tr[i] | tr[j];
					int sq = r.Width() * r.Height();
					if(sq < TransparentBackPaintAreaSize &&
					   sq - sqr[i] - sqr[j] < 2 * min(sqr[i], sqr[j])) {
						sqr[i] = sq;
						tr[i] = r;
						in[j] = false;
					}
				}
		for(i = 0; i < er.GetCount(); i++)
			draw.ExcludeClip(er[i]);
		for(i = 0; i < tr.GetCount(); i++)
			if(in[i]) {
			#ifdef _DEBUG
				if(ShowRepaint) {
					draw.DrawRect(tr[i], LtRed);
					GuiFlush();
					Sleep(ShowRepaint);
				}
			#endif
				BackDraw bw;
				bw.Create(draw, tr[i].Size());
				bw.Offset(-tr[i].TopLeft());
				LLOG("Transparent backpaint " << tr[i] << BeginIndent);
				CtrlPaint(bw, paintrect & tr[i]);
				LLOGEND();
				bw.Put(draw, tr[i].TopLeft());
				draw.ExcludeClip(tr[i]);
			}
	#ifdef _DEBUG
		if(ShowRepaint) {
			draw.DrawRect(0, 0, GetRect().Width(), GetRect().Height(), Magenta);
			Draw::Flush();
			Sleep(ShowRepaint);
		}
	#endif
	}
	CtrlPaint(draw, paintrect);
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
	UpdateArea(w, Rect(Point(0, 0), GetRect().Size()));
	w.End();
}
