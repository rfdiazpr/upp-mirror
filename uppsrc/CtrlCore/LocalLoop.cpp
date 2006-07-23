#include "CtrlCore.h"

void LocalLoop::Run()
{
	ASSERT(master);
	master->AddChild(this);
	Ptr<Ctrl> focus = GetFocusCtrl();
	SetCapture();
	EventLoop(this);
	Remove();
	if(focus)
		focus->SetFocus();
}

void DrawDragRect(Ctrl& q, const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
                  Color color, const word *pattern)
{
	ViewDraw w(&q);
	DrawDragRect(w, rect1, rect2, clip, n, color, pattern);
}

void RectTracker::LeftUp(Point, dword)
{
	EndLoop();
}

void RectTracker::RightUp(Point, dword)
{
	EndLoop();
}

Image RectTracker::CursorImage(Point, dword)
{
	if(animation)
		DrawRect(rect, rect);
	return Image::Arrow();
}

static word RectTracker_normal[] = { 0x5555, 0xaaaa, 0x5555, 0xaaaa, 0x5555, 0xaaaa, 0x5555, 0xaaaa };
static word RectTracker_dashed[] = { 0xF0F0, 0x7878, 0x3C3C, 0x1E1E, 0x0F0F, 0x8787, 0xC3C3, 0xE1E1 };
static word RectTracker_solid[]  = { 0, 0, 0, 0, 0, 0, 0, 0 };

RectTracker::RectTracker(Ctrl& master)
{
	SetMaster(master);
	Clip(Rect(0, 0, 100000, 100000));
	width = 1;
	minsize = Size(0, 0);
	maxsize = Size(100000, 100000);
	maxrect = Rect(-100000, -100000, 100000, 100000);
	keepratio = false;
	cursorimage = Image::Arrow();
	color = SColorPaper;
	pattern = RectTracker_normal;
	animation = 0;
	rounder = NULL;
}

RectTracker::~RectTracker() {}

RectTracker& RectTracker::Dashed() { pattern = RectTracker_dashed; return *this; }
RectTracker& RectTracker::Solid()  { pattern = RectTracker_solid; return *this; }

static void sGetAniPat(word *anipat, const word *src, int pos)
{
	for(int i = 0; i < 8; i++)
		anipat[i] = word(MAKELONG(src[(i + pos) % 8], src[(i + pos) % 8]) >> pos);
}

void RectTracker::DrawRect(const Rect& r1, const Rect& r2)
{
	Rect c = clip & GetMaster().GetSize();
	if(animation) {
		word anipat[8];
		int nanim = (GetTickCount() / animation) % 8;
		sGetAniPat(anipat, pattern, nanim);
		DrawDragRect(GetMaster(), Null, r2, c, width, color, anipat);
		sGetAniPat(anipat, pattern, panim);
		DrawDragRect(GetMaster(), r1, Null, c, width, color, anipat);
		panim = nanim;
	}
	else
		DrawDragRect(GetMaster(), r1, r2, c, width, color, pattern);
}

Rect RectTracker::Track(const Rect& r, int _tx, int _ty)
{
	rect = r;
	tx = _tx;
	ty = _ty;
	org = rect;
	o = rect;
	op = GetMousePos();
	GetMaster().Sync();
	DrawRect(Null, rect);
	Run();
	DrawRect(o, Null);
	return rect;
}

void RectTracker::MouseMove(Point, dword)
{
	Point p = GetMousePos();
	rect = org;
	if(tx == ALIGN_CENTER && ty == ALIGN_CENTER) {
		int x = org.left - op.x + p.x;
		int y = org.top - op.y + p.y;
		if(x + org.Width() > maxrect.right)
			x = maxrect.right - org.Width();
		if(x < maxrect.left)
			x = maxrect.left;
		if(y + org.Height() > maxrect.bottom)
			y = maxrect.bottom - org.Height();
		if(y < maxrect.top)
			y = maxrect.top;
		rect = RectC(x, y, org.Width(), org.Height());
	}
	else {
		if(tx == ALIGN_LEFT) {
			rect.left = max(org.left - op.x + p.x, maxrect.left);
			rect.left = minmax(rect.left, rect.right - maxsize.cx, rect.right - minsize.cx);
		}
		if(tx == ALIGN_RIGHT) {
			rect.right = min(org.right - op.x + p.x, maxrect.right);
			rect.right = minmax(rect.right, rect.left + minsize.cx, rect.left + maxsize.cx);
		}
		if(ty == ALIGN_TOP) {
			rect.top = max(org.top - op.y + p.y, maxrect.top);
			rect.top = minmax(rect.top, rect.bottom - maxsize.cy, rect.bottom - minsize.cy);
		}
		if(ty == ALIGN_BOTTOM) {
			rect.bottom = min(org.bottom - op.y + p.y, maxrect.bottom);
			rect.bottom = minmax(rect.bottom, rect.top + minsize.cy, rect.top + maxsize.cy);
		}
		if(keepratio) {
			if(tx == ALIGN_BOTTOM && ty == ALIGN_RIGHT) {
				Size sz = rect.Size();
				int cy = rect.Width() * org.Height() / org.Width();
				int cx = rect.Height() * org.Width() / org.Height();
				if(cx > sz.cx)
					rect.right = rect.left + cx;
				else
					rect.bottom = rect.top + cy;
			}
			else
			if(tx == ALIGN_RIGHT)
				rect.bottom = rect.top + rect.Width() * org.Height() / org.Width();
			else
			if(ty == ALIGN_BOTTOM)
				rect.right = rect.left + rect.Height() * org.Width() / org.Height();
		}
	}
	if(rect != o) {
		rect = Round(rect);
		if(rect != o) {
			DrawRect(o, rect);
			sync(rect);
			o = rect;
		}
	}
}

class PointLooper : public LocalLoop {
	const Vector<Image>& ani;
	int ani_ms;
	bool result;

public:
	virtual void  LeftUp(Point, dword);
	virtual Image CursorImage(Point p, dword keyflags);
	virtual bool  Key(dword key, int);

	operator bool() const     { return result; }

	PointLooper(Ctrl& ctrl, const Vector<Image>& ani, int ani_ms)
	: ani(ani), ani_ms(ani_ms) { SetMaster(ctrl); }
};

void  PointLooper::LeftUp(Point, dword)
{
	result = true;
	EndLoop();
}

Image PointLooper::CursorImage(Point p, dword keyflags)
{
	return ani[int(GetTimeClick() / ani_ms % ani.GetCount())];
}

bool  PointLooper::Key(dword key, int)
{
	if(key == K_ESCAPE) {
		result = false;
		EndLoop();
	}
	return true;
}

bool PointLoop(Ctrl& ctrl, const Vector<Image>& ani, int ani_ms)
{
	PointLooper p(ctrl, ani, ani_ms);
	p.Run();
	return p;
}
