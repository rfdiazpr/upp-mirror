#ifndef _Docking_TabBar_h_
#define _Docking_TabBar_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#define IMAGECLASS TabImg
#define IMAGEFILE <Docking/TabBar.iml>
#include <Draw/iml_header.h>

class TabBar : public FrameCtrl<Ctrl>
{ 
public:
	typedef TabBar CLASSNAME;

	Callback1<int> 	WhenSelect;
	Callback1<int> 	WhenHighlight;
	Callback1<int> 	WhenDrag;
	Callback1<int> 	WhenContext;
	
	virtual void 	FrameAddSize(Size& sz);
	virtual void	FrameLayout(Rect& r);	
	virtual void 	FrameAdd(Ctrl& parent) 				{ parent.Add(*this); }
	virtual void 	FrameRemove()          				{ this->Ctrl::Remove(); }
	
	virtual void 	Paint(Draw& w);
	virtual void	MouseMove(Point p, dword keyflags);
	virtual void 	MouseLeave();
	virtual void	LeftDown(Point p, dword keyflags);
	virtual void	LeftDrag(Point p, dword keyflags);
	virtual void	RightDown(Point p, dword keyflags);
public:	
	virtual TabBar &AddTab(const WString &title);
	virtual TabBar &AddTab(Ctrl &c, const WString &title);
	void 			RemoveTab(int ix);
	void 			Clear()					{ tabs.Clear(); selix = -1; }
	
	void 			SelectTab(int ix)		{ ASSERT(ix < tabs.GetCount()); selix = max(-1, ix); Refresh(); }
	
	int  			GetSelected() const		{ return selix; }
	int 			GetCount() const		{ return tabs.GetCount(); }
	int				GetTabIx(Point p) const;
	int				GetTabIx(Ctrl &c) const	{ for (int i = 0; i < tabs.GetCount(); i++) if (tabs[i].GetCtrl() == &c) return i; return -1; }
	Ctrl *			GetTabCtrl(int ix) const{ ASSERT(ix < tabs.GetCount() && ix >= 0); return tabs[ix].GetCtrl(); }
	Ctrl *			GetSelectedCtrl() const	{ return selix >= 0 ? tabs[selix].GetCtrl() : NULL; }
	int 			GetTabSz() const		{ return tabfont.GetHeight()+4; }
	const WString &	GetTabTitle(int ix) 	{ ASSERT(ix < tabs.GetCount() && ix >= 0); return tabs[ix].GetTitle(); }

	bool			IsVert() const			{ return !(align & 1); }
	bool 			IsAutoHide() const		{ return GetCount() <= autohide; }
	int 			GetAlign() const 		{ return align; }	
	
	TabBar& 		NoSelection(bool s = true) 	{ nosel = s; Refresh(); return *this; }
	TabBar& 		NoHighlight(bool h = true) 	{ nohl = h; Refresh(); return *this; }
	
	TabBar &		Set(int _align)    		{ ASSERT(align >= 0 && align < 4); align = _align; return *this; }
	TabBar &		Left()    				{ align = LEFT; return *this; }
	TabBar &		Top()     				{ align = TOP; return *this; }
	TabBar &		Right()   				{ align = RIGHT; return *this; }
	TabBar &		Bottom()  				{ align = BOTTOM; return *this;  }	
	
	TabBar &		AutoHideMin(int hidemin = 1)	{ autohide = hidemin; return *this; }
	TabBar &		SetFont(const Font &font)		{ tabfont = font; return *this; }
	
	TabBar();
	
	struct Style : ChStyle<Style> {
		Image 	vertical[3]; // Normal, Active, Highlighted
		Image 	horizontal[3]; // Normal, Active, Highlighted
	};	
	static const Style& StyleDefault();		
		
	enum { LEFT = 0, TOP, RIGHT, BOTTOM };
	
protected:
	void DrawTab(Draw &w, Point pt, int h, int ix);
	const Style *St()									{ return style ? style : &StyleDefault(); }
	
private:
	struct Tab : public Moveable<Tab> {
		int sz;
		WString title;
		virtual const WString &GetTitle() const	{ return title; }
		virtual Ctrl *		   GetCtrl() const	{ return NULL; }		
	};
	struct CtrlTab : public Tab, public Moveable<CtrlTab> {
		int sz;
		Ctrl *ctrl;
		virtual Ctrl *		   GetCtrl() const	{ return ctrl; }		
	};	
	struct WndTab : public Tab, public Moveable<WndTab> {
		int sz;
		TopWindow *ctrl;
		virtual const WString &GetTitle() const	{ return ctrl->GetTitle(); }
		virtual Ctrl *		   GetCtrl() const	{ return ctrl; }
	};	

	Array<Tab> tabs;
	int selix;
	int hlix;
	int align;
	int autohide;
	bool nosel:1;
	bool nohl:1;
	Font tabfont;	
	
	const Style *	style;	
};

class AutoHideBar : private TabBar {
public:
	typedef	AutoHideBar CLASSNAME;

	virtual void MouseEnter(Point p, dword keyflags);	
	virtual void MouseLeave();	
	
	AutoHideBar &	AddTab(Ctrl &c);
	AutoHideBar &	AddTab(Ctrl &c, const WString &title);
	int 		 	GetTabIx(Ctrl &c)						{ return TabBar::GetTabIx(c); }
	Ctrl *			GetTabCtrl(int ix) const				{ return TabBar::GetTabCtrl(ix);  }	
	AutoHideBar	&	RemoveTab(int ix);
	AutoHideBar	&	RemoveTab(Ctrl &c)						{ return RemoveTab(GetTabIx(c)); }
	AutoHideBar &	Set(int _align)    						{ TabBar::Set(_align); return *this; }

	bool 		HasCtrl(Ctrl &c) const						{ return (TabBar::GetTabIx(c) >= 0); }
	void 		RefreshParentLayout()						{ return Ctrl::RefreshParentLayout(); }
	void 		RefreshFrame()								{ TabBar::RefreshFrame(); }
	Ctrl *		GetParent()									{ return TabBar::GetParent(); }
	
	static void SetTimeout(int delay_ms)	  				{ ASSERT(delay_ms > 0); autohide_timeout = delay_ms; }
	
	operator CtrlFrame*()             						{ return (CtrlFrame*)this; }
	operator CtrlFrame&()             						{ return *(CtrlFrame*)this; }
	operator const CtrlFrame*() const  						{ return (CtrlFrame*)this; }
	operator const CtrlFrame&() const  						{ return *(CtrlFrame*)this; }
		
	AutoHideBar();
	
private:
	static int autohide_timeout;

	struct HidePopup : public Ctrl
	{
		Callback WhenEnter;
		Callback WhenLeave;
		virtual void ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags); 
	};

	enum { TIMEID_ACTION_CHECK = Ctrl::TIMEID_COUNT,
		   TIMEID_ANIMATE,
		   TIMEID_HIDE_TIMEOUT,
           TIMEID_COUNT };
	Ctrl *	showctrl;
	HidePopup popup;
	
	void 	TabHighlight(int ix);	
	void	ShowAnimate(Ctrl *c);
	void	HideAnimate(Ctrl *c);
	void 	AnimateSizePlus(Size sz);
	void 	AnimateSizeMinus(Size sz);
	void 	AdjustSize(Rect &r, const Size &sz);
};

#endif
