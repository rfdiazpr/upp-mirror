#ifndef _Docking_WindowFrame_h_
#define _Docking_WindowFrame_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include "TitleFrame.h"

template <class T>
class WithWindowFrame : public T {
public:
	typedef WithWindowFrame<T> CLASSNAME;
	
	WithWindowFrame<T>();

	virtual void 	MoveBegin()							{ }
	virtual void 	Moving()							{ }
	virtual void 	MoveEnd()							{ }
	virtual void 	TitleContext()						{ }
	
	virtual Image 	FrameMouseEvent(int event, Point p, int zdelta, dword keyflags);

	virtual void	ChildGotFocus()						{ titlebar.SetFocusLook(true); T::ChildGotFocus(); }
	virtual void	ChildLostFocus()					{ titlebar.SetFocusLook(false); T::ChildLostFocus(); }

	virtual void 	Paint(Draw &w) 						{ w.DrawRect(T::GetSize(), SColorFace()); }
	
	const Image &		GetIcon() const					{ return titlebar.GetImage(); } // This really should be in TopWindow
	WithWindowFrame<T> &Icon(const Image& m)			{ titlebar.SetImage(m); return *this; }
	WithWindowFrame<T> &Icon(const Image& smallicon, const Image& largeicon) { titlebar.SetImage(smallicon); return *this; }	
	WithWindowFrame<T> &Title(const char *_title)		{ titlebar.SetTitle(_title); return *this; }
	WithWindowFrame<T> &Title(const WString& _title)	{ titlebar.SetTitle(_title); return *this; }	
				
	TitleFrame &		TitleBar()						{ return titlebar; }
	WithWindowFrame<T> &AddTitleCtrl(Ctrl &c)			{ titlebar << c; return *this; }
	WithWindowFrame<T> &NoCloseButton()					{ close.Remove(); return *this; }
	WithWindowFrame<T> &SetCloseLook(const Value *_look){ close.SetLook(_look); return *this; }	
	
	void				AddWindowFrame();
	void				RemoveWindowFrame();
private:
	class MoveResizeLoop : public LocalLoop
	{
		int sizedir;
		Rect rstart;
		Point mstart;		
		WithWindowFrame<T> *wnd;
		
		void StartMoveResize(WithWindowFrame<T> *w, int dir, const Rect &r, const Point &p);
	public:	
		virtual void 	LeftUp(Point p, dword keyflags)									{ EndMoveResize(); }
		virtual void 	RightUp(Point p, dword keyflags)								{ EndMoveResize(); }
		virtual void 	MouseMove(Point p, dword keyflags);
		virtual Image 	CursorImage(Point p, dword keyflags);		
		
		void StartMove(WithWindowFrame<T> *w, const Rect &r, const Point &p)			{ StartMoveResize(w, -1, r, p); }
		void StartResize(WithWindowFrame<T> *w, int dir, const Rect &r, const Point &p) { StartMoveResize(w, dir, r, p); }
		void EndMoveResize();
	};
	ImgButton close;
	TitleFrame titlebar;
protected:
	void TitleDrag();
};

template <class T>
WithWindowFrame<T>::WithWindowFrame()
{
	titlebar.WhenContext = callback(this, &WithWindowFrame<T>::TitleContext);
	titlebar.Add(close);
	close <<= THISBACK(Close);
}

template <class T>
void WithWindowFrame<T>::AddWindowFrame()
{
	T::InsertFrame(0, OutsetFrame());  // TODO: Replace with proper window frame
	T::InsertFrame(1, InsetFrame());
	T::InsertFrame(2, titlebar); 
	titlebar.WhenDrag = callback(this, &WithWindowFrame<T>::TitleDrag);
}

template <class T>
void WithWindowFrame<T>::RemoveWindowFrame()
{
	int ix = T::FindFrame(titlebar);
	for (int i = 0; i < 3; i++)
		T::RemoveFrame(ix-2);
	titlebar.WhenDrag.Clear();
}

template <class T>
void WithWindowFrame<T>::TitleDrag()
{
	MoveResizeLoop mloop;
	mloop.StartMove(this, T::GetRect(), GetMousePos());
}

template <class T>
Image WithWindowFrame<T>::FrameMouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	if (!titlebar.IsChild()) return Image::Arrow();
	Rect rstart = T::GetRect();
	Point mstart = GetMousePos();
	Rect r = Rect(rstart.GetSize()).Deflated(8);

	int sizedir = -1;
	if (r.left > p.x)
		sizedir = 0;
	else if (r.top > p.y)
		sizedir = 1;
	else if (r.right < p.x)
		sizedir = 2;
	else if (r.bottom < p.y)
		sizedir = 3;
	
	if (sizedir >= 0 && event == Ctrl::LEFTDOWN) {
		MoveResizeLoop mloop;
		mloop.StartResize(this, sizedir, rstart, GetMousePos());
		return Image::Arrow();
	}
	return sizedir >= 0 ? ((sizedir & 1) ? Image::SizeVert() : Image::SizeHorz()) 
		: Image::Arrow();
}

template <class T>
void WithWindowFrame<T>::MoveResizeLoop::MouseMove(Point p, dword keyflags)
{
	Point pt = GetMousePos();
	if (wnd->GetParent())
		pt -= wnd->GetParent()->GetRect().TopLeft();			
	if (sizedir < 0) {		
		wnd->SetRect(rstart.Offseted(pt - mstart));
		wnd->Moving();
	}
	else {
		Rect r = rstart;
		Size szmin = wnd->GetMinSize();
		Size szmax = wnd->GetMaxSize();
		switch (sizedir) {
			case 0:
				r.left = minmax(r.left + (pt.x - mstart.x), r.right - szmax.cx, r.right - szmin.cx);
				break;
			case 1:
				r.top = minmax(r.top + (pt.y - mstart.y), r.bottom - szmax.cy, r.bottom - szmin.cy);
				break;
			case 2:
				r.right = minmax(r.right + (pt.x - mstart.x), r.left + szmin.cx, r.left + szmax.cx);
				break;					
			case 3:
				r.bottom = minmax(r.bottom + (pt.y - mstart.y), r.top + szmin.cy, r.top + szmax.cy);
				break;					
		}
		wnd->SetRect(r);
	}
}

template <class T>
Image WithWindowFrame<T>::MoveResizeLoop::CursorImage(Point p, dword keyflags)
{
#ifdef PLATFORM_X11
	return (sizedir < 0) ? Image::SizeAll() 
		: ((sizedir & 1) ? Image::SizeVert() : Image::SizeHorz());
#else
	return (sizedir < 0) ? Image::Arrow() 
		: ((sizedir & 1) ? Image::SizeVert() : Image::SizeHorz());
#endif
}

template <class T>
void WithWindowFrame<T>::MoveResizeLoop::StartMoveResize(WithWindowFrame<T> *w, int dir, const Rect &r, const Point &p)
{
	wnd = w;
	sizedir = dir;
	rstart = r;
	mstart = p;
	if (wnd->GetParent())
		mstart -= wnd->GetParent()->GetRect().TopLeft();		
	SetMaster(*wnd);
	if (sizedir < 0)
		wnd->MoveBegin();
	Run();
}

template <class T>
void WithWindowFrame<T>::MoveResizeLoop::EndMoveResize()
{
	EndLoop();
	
	if (sizedir < 0)
		wnd->MoveEnd();
}


#endif
