#ifndef _Docking_TitleFrame_h_
#define _Docking_TitleFrame_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#define IMAGECLASS WindowImg
#define IMAGEFILE <Docking/Window.iml>
#include <Draw/iml_header.h>

struct ImgButton : public Pusher
{
	private:
		const Value *look;
		Image img;
	public:		
		virtual void Paint(Draw &w);
		virtual void MouseEnter(Point p, dword kflags) 	{ Refresh(); Pusher::MouseEnter(p, kflags); }
		virtual void MouseLeave() 						{ Refresh(); Pusher::MouseLeave(); }

		ImgButton& 	SetImage(const Image &_img) { img = _img; Refresh(); return *this; }
		ImgButton& 	SetLook(const Value *_look) { look = _look; Refresh(); return *this; }
};

class TitleFrame : public CtrlFrame, public Ctrl {
public:
	Callback WhenContext;
	Callback WhenDrag;
	
	typedef TitleFrame CLASSNAME;
	virtual void FrameAdd(Ctrl& parent)		{ parent.Add(*this); }
	virtual void FrameRemove()				{ this->Ctrl::Remove(); }
	virtual void FrameAddSize(Size& sz)		{ if (IsShown()) sz.cy += TitleSz(); }
	virtual void FrameLayout(Rect& r)		{ LayoutFrameTop(r, this, TitleSz()); }
	virtual void Paint(Draw& draw);
	
	virtual void LeftDrag(Point p, dword keyflags)  { SetFocusLook(true); WhenDrag(); }
	virtual void LeftDown(Point p, dword keyflags) 	{ SetFocusLook(true); }
	virtual void RightDown(Point p, dword keyflags) { SetFocusLook(true); WhenContext(); }
	virtual void ChildGotFocus()					{ SetFocusLook(true); }
	virtual void ChildAdded(Ctrl *child) 			{ SetChildPos(); }
	virtual void ChildRemoved(Ctrl *child) 			{ SetChildPos(); }
public:	
	struct Style : ChStyle<Style> {
		Value	background[2];
		Color	text[2];	
		Font	font;
	};	
	static const Style& StyleDefault();		
		
private:
	Image 			img;
	WString 		title;
	bool			focuslook:1;
	
	const Style *	style;	
	const Style *	St() const					{ return style ? style : &StyleDefault(); }		
	
	int 			TitleSz() const				{ return St()->font.GetHeight()+4; }
	void			SetChildPos();
public:
	enum { LEFT, TOP, RIGHT, BOTTOM };

	TitleFrame & 	SetFocusLook(bool _focus = true);
	TitleFrame & 	SetStyle(const Style &s)		{ style = &s; SetChildPos(); RefreshParentLayout(); return *this; }
	bool			IsFocusLook()					{ return focuslook; }
	TitleFrame & 	SetTitle(const WString &_title) { title = _title; Refresh(); return *this; }
	const WString &	GetTitle() const				{ return title; }
	TitleFrame &	SetImage(const Image &image)	{ img = image; Refresh(); return *this; }
	const Image &	GetImage() const				{ return img; }
	TitleFrame();
};

#endif
