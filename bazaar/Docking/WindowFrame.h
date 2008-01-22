#ifndef _Docking_WindowFrame_h_
#define _Docking_WindowFrame_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#define IMAGECLASS WindowImg
#define IMAGEFILE <Docking/Window.iml>
#include <Draw/iml_header.h>
/*
class TitleFrame : public CtrlFrame, public Ctrl {
public:
	Callback WhenContext;
	Callback WhenDrag;
	Callback WhenClose;
	
	typedef TitleFrame CLASSNAME;
	virtual void FrameAdd(Ctrl& parent)		{ parent.Add(*this); }
	virtual void FrameRemove()				{ this->Ctrl::Remove(); }
	virtual void FrameAddSize(Size& sz)		{ if (IsShown()) sz.cy += TitleSz(); }
	virtual void FrameLayout(Rect& r)		{ LayoutFrameTop(r, this, TitleSz()); }
	virtual void Paint(Draw& draw);
	
	virtual void LeftDrag(Point p, dword keyflags)  { SetFocusLook(true, true); WhenDrag(); }
	virtual void LeftDown(Point p, dword keyflags) 	{ MouseMove(p, keyflags); SetFocusLook(true, true); }
	virtual void LeftUp(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void RightDown(Point p, dword keyflags) { SetFocusLook(true, true); WhenContext(); }
public:	
	struct Style : ChStyle<Style> {
		Value	background[2];
		Color	text[2];	
		Value 	close[4];
		Font	font;
	};	
	static const Style& StyleDefault();		
		
private:
	Image 			img;
	WString 		title;
	bool			focuslook:1;
	int				close;
	
	const Style *	style;	
	const Style *	St() const					{ return style ? style : &StyleDefault(); }		
	
	int 			TitleSz() const				{ return St()->font.GetHeight()+4; }
	Rect			CloseRect(Rect r);
public:
	enum { LEFT, TOP, RIGHT, BOTTOM };

	TitleFrame & 	SetFocusLook(bool _focus = true, bool _pfocus = false);
	TitleFrame & 	SetStyle(const Style &s)		{ style = &s; RefreshParentLayout(); return *this; }
	bool			IsFocusLook()					{ return focuslook; }
	TitleFrame & 	SetTitle(const WString &_title) { title = _title; Refresh(); return *this; }
	const WString &	GetTitle() const				{ return title; }
	TitleFrame &	SetImage(const Image &image)	{ img = image; Refresh(); return *this; }
	const Image &	GetImage() const				{ return img; }
	TitleFrame &	CloseButton(bool b = true)		{ close = b ? ((close >= 0) ? b : CTRL_NORMAL) : -1; Refresh(); return *this; }
	TitleFrame();
};

template <class T>
class WithWindowFrame : public WithMoveResize<T> {
public:
	typedef WithWindowFrame<T> CLASSNAME;
	
	WithWindowFrame<T>();

	virtual void 	TitleContext()						{ }
	
	virtual Image 	FrameMouseEvent(int event, Point p, int zdelta, dword keyflags);

	virtual void	ChildGotFocus()						{ titlebar.SetFocusLook(true); T::ChildGotFocus(); }
	virtual void	ChildLostFocus()					{ titlebar.SetFocusLook(false); T::ChildLostFocus(); }

	virtual void 	Paint(Draw &w) 						{ w.DrawRect(T::GetSize(), SColorFace()); }
	
	virtual Size   		GetMinSize() const;
	const Image &		GetIcon() const					{ return titlebar.GetImage(); } // This really should be in TopWindow
	WithWindowFrame<T> &Icon(const Image& m)			{ titlebar.SetImage(m); return *this; }
	WithWindowFrame<T> &Icon(const Image& smallicon, const Image& largeicon) { titlebar.SetImage(smallicon); return *this; }	
	WithWindowFrame<T> &Title(const char *_title)		{ titlebar.SetTitle(_title); return *this; }
	WithWindowFrame<T> &Title(const WString& _title)	{ titlebar.SetTitle(_title); return *this; }	
				
	TitleFrame &		TitleBar()						{ return titlebar; }
	
	void				AddWindowFrame();
	void				RemoveWindowFrame();
private:
	typedef WithMoveResize<T> BaseT;

	TitleFrame titlebar;
protected:
	void TitleDrag()									{ WithMoveResize<T>::StartMove(); }
};

template <class T>
WithWindowFrame<T>::WithWindowFrame()
{
	titlebar.WhenContext = callback(this, &WithWindowFrame<T>::TitleContext);
	titlebar.WhenClose = THISBACK(Close);
}

template <class T>
Size WithWindowFrame<T>::GetMinSize() const
{
	return T::AddFrameSize(T::GetMinSize());
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
Image WithWindowFrame<T>::FrameMouseEvent(int event, Point p, int zdelta, dword keyflags)
{
	if (!titlebar.IsChild()) return Image::Arrow();
	Point mstart = GetMousePos();
	Rect r = Rect(T::GetRect().GetSize()).Deflated(8);

	int sizedir = -1;
	if (r.left > p.x)
		sizedir = BaseT::RESIZE_LEFT;
	else if (r.top > p.y)
		sizedir = BaseT::RESIZE_TOP;
	else if (r.right < p.x)
		sizedir = BaseT::RESIZE_RIGHT;
	else if (r.bottom < p.y)
		sizedir = BaseT::RESIZE_BOTTOM;
	
	if (sizedir >= 0 && event == Ctrl::LEFTDOWN) {
		BaseT::StartResize(sizedir);
		return Image::Arrow();
	}
	return sizedir >= 0 ? ((sizedir & 1) ? Image::SizeVert() : Image::SizeHorz()) 
		: Image::Arrow();
}
*/

#endif
