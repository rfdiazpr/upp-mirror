//==============================================================================================
// DockCtrl: A dockable widget framework for U++
// Author:	 Ismail YILMAZ
// Version:	 0.50 (DEV-0802b.1)
//==============================================================================================

#ifndef DOCKCTRL_H
#define DOCKCTRL_H

#include <CtrlLib/CtrlLib.h>
#include <Draw/Draw.h>

using namespace UPP;

#define LAYOUTFILE <DockCtrl/DockCtrl.lay>
#include <CtrlCore/lay.h>

#define IMAGECLASS DockCtrlImgs
#define IMAGEFILE <DockCtrl/DockableCtrl.iml>
#include <Draw/iml_header.h>

class PaneSplitter;
class TabInterface;
class AutoHideBar;
class PaneFrame;
class DockCtrl;
class DockWindow;
class TabWindow;
class DockableCtrl;

//----------------------------------------------------------------------------------------------
// DockCtrl: Chameleon Support.
//
struct DockCtrlChStyle : Moveable<DockCtrlChStyle>
{
	struct Style : ChStyle<Style>
	{
		Font 	font;
		int 	bartitlesize;
		int		barheight;
		Rect	barmargins;
		Value	barbackground[1];
		Value	barshut[3];
		Value	barhide[3];
		Value	barmenu[3];
		int		tabheight;
		int		tabmargin;
		int		tabextendleft;
		Rect	tabsel;
		Rect	tabedge;
		Value	tabnormal[4];
		Value	tabfirst[4];
		Value	tablast[4];
		Value	tabboth[4];
		Value	hidebutton[4];	
		
	};
	const Style* style;
	static const Style& StyleDefault();
	static const Style& StyleClassic();
	static const Style& StyleEnhanced();
};

static void DockCtrlImgsLook(Value* look, int i, int n)
{
	while(n--)
		*look++ = DockCtrlImgs::Get(i++);
}

//----------------------------------------------------------------------------------------------

struct CustomFrame : FrameCtrl<Ctrl>
{
public:
	enum { LAYOUT_LEFT, LAYOUT_TOP, LAYOUT_RIGHT, LAYOUT_BOTTOM };

	int layout;
	int size;
	int border;
	
	bool horizontal;
		
	virtual void FrameLayout(Rect &r);
	virtual void FrameAddSize(Size& sz);

	void Fix(Size& sz);
	void Fix(Point& p);	

	bool IsHorz()			{ return layout == LAYOUT_TOP || layout == LAYOUT_BOTTOM; }
	bool IsVert()			{ return !IsHorz(); }
	
	void SetSize(int sz)	{ size = sz; }
		
	CustomFrame() : border(0), size(0), layout(LAYOUT_TOP), horizontal(true) {}	
};

class TabInterface : public CustomFrame
{
private:


	class TabScrollBar : public CustomFrame
	{
	private:
		int total;
		double pos, ps;
		int new_pos;
		int old_pos;
		double start_pos;
		double size;
		double cs, ics;
		void UpdatePos(bool update = true);
		bool ready;
		Size sz;

	public:
		TabScrollBar();

		virtual void Paint(Draw &w);
		virtual void LeftDown(Point p, dword keyflags);
		virtual void LeftUp(Point p, dword keyflags);
		virtual void MouseMove(Point p, dword keyflags);
		virtual void MouseWheel(Point p, int zdelta, dword keyflags);
		virtual void Layout();

		int  GetPos() const;
		void SetPos(int p, bool dontscale = false);
		void AddPos(int p, bool dontscale = false);
		int  GetTotal() const;
		void AddTotal(int t);
		void SetTotal(int t);
		void GoEnd();
		void GoBegin();
		void Set(const TabScrollBar& t);
		TabScrollBar& SetLayout(int l);
		Callback WhenScroll;
	};


	
protected:
	void OnScrollBar() { Refresh(); };
	int  GetPos() { return scrollbar.GetPos(); }
	Point mouse, oldp;
	TabScrollBar scrollbar; 

	enum 
	{	TAB_MARGIN = 6, 
		TAB_SPACE = 20, 
		TAB_SBHEIGHT = 4, 
		TAB_SBSEPARATOR = 1, 
		TAB_FILEICON = 16, 
		TAB_SPACEICON = 5 
	};

	struct Tab : Moveable<Tab>
	{
		DockableCtrl* dock;
		bool 	active, visible, highlighted;
		int		id;
		int 	x, y, cx, cy;
		Size 	textsize;
		String 	name;
		Image	icon;
		int		Right()	 { return x + cx; };
		int		Bottom() { return y + cy; };
		bool	IsActive() { return active; }
		bool	IsVisible()	{ return visible; }
		bool	IsHighligted()	{ return highlighted; }
		bool	HasMouse(const Point& p)	{ return visible && Rect(x, y, x + cx, y + cy).Contains(p);}
		bool	HasMouseButton(const Point& p, int h) 
		{ 
			if(!visible) return false;
			Size isz = DockCtrlImgs::DClose().GetSize();
			int iy = (h - isz.cy) / 2;
			int ix = x + cx - isz.cx - TAB_MARGIN;
			return p.x >= ix && p.x < ix + isz.cx && p.y >= iy && p.y < iy + isz.cy;
		}
		Tab() : id(-1), active(false), visible(false), highlighted(false) { dock = NULL; }
		~Tab() { if(dock) dock = NULL; }
	};

	Vector<Tab> tabs;
	int id;
	int active, highlight, visible;
	int fileicon, tabbutton;
	bool hastabbutton, hasfileicon, hasscrollbar;

public:
	const TabCtrl::Style *style;
	TabInterface& SetStyle(const TabCtrl::Style& s)  { style = &s; Refresh(); return *this; }
	static const TabCtrl::Style& StyleDefault();

public:
	void Add(DockableCtrl& dock, bool activate = false);
	void Add(String name, bool activate = false);
	void Remove(DockableCtrl& dock);
	void Remove(int n);
	void Close(int n);
	void CloseAll();
	
	void SetActiveTab(int n);
	void SetActiveTab(DockableCtrl& dock);
	int  GetActiveTab();
	int  Find(int id);
	int	 Find(DockableCtrl& dock);
	int  GetNext(int n);
	int  GetPrev(int n);
	virtual void ReposTabs();
	virtual void DrawTab(Draw& w, Size &sz, int i);
	void DrawLabel(Draw& w, int x, int y, Tab& t, bool isactive);	

	Vector<Tab>& GetTabs()	{return tabs; } 
	
	int GetWidth(int n);
	int GetWidth();
	int GetHeight();
	
	int GetNextId() { return ++id; }
	
	TabInterface& HasButtons(bool b) 	{ hastabbutton = b; ReposTabs(); if(IsOpen() && IsChild()) Refresh(); return *this; }
	TabInterface& HasIcons(bool b)	 	{ hasfileicon  = b;	ReposTabs(); if(IsOpen() && IsChild()) Refresh(); return *this; }
	TabInterface& HasScrollBar(bool b);	
	
	TabInterface& SetLayout(int l);
		
protected:
	virtual void Paint(Draw& d);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual void LeftDrag(Point p, dword keyflags);
	virtual void RightDown(Point p, dword keyflags);
	virtual void RightUp(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);
	virtual void MouseLeave();

public:
	Callback2<int, DockableCtrl&> WhenContext;
	Callback2<int, DockableCtrl&> WhenClose;
	Callback2<int, DockableCtrl&> WhenDrag;
	Callback2<int, DockableCtrl&> WhenActive;
	Callback WhenHighlight;
	Callback WhenSelect;

public:
	typedef TabInterface CLASSNAME;
	
	TabInterface();
	~TabInterface();
};

//----------------------------------------------------------------------------------------------

class PaneSplitter : public Splitter
{
public:
	typedef PaneSplitter CLASSNAME;
	
	void AddChildDock(DockableCtrl& dock, int position);
	void AddChildDock(DockableCtrl& dock);
	void RemoveChildDock(DockableCtrl& dock);
	void RemoveChildDock(int position);
	void RemoveChilds();
	void ReposChilds();
	DockableCtrl* GetChildAt(int position); 
	int  GetChildCount();
	bool HasChild() const;					

	void StartAnimation(DockableCtrl& dock, int position);
	void StartAnimation(int position);
	void StopAnimation();
	inline bool IsAnimating() const	{ return animating; }
	void Animate(int position = 0);
	
	Rect AnimationArea() { return animationctrl.GetScreenRect();}
	Ctrl* GetAnimationCtrl() { return (Ctrl*) &animationctrl; }		

	virtual void Layout();
	
private:

	Array<int> positions;
	void AnimateCallback();
	
	class AnimationCtrl : public Ctrl
	{
	public:
		typedef AnimationCtrl CLASSNAME;
		void AnimateVert();
		void AnimateHorz();
		void Paint(Draw& d)	{ d.DrawRect(GetSize(), GUI_GlobalStyle() >= GUISTYLE_XP ? Blend(SColorHighlight, SColorFace) : SColorShadow); }
	public:
		AnimationCtrl()  { SetFrame(FieldFrame()); }
	};
	AnimationCtrl animationctrl;
	bool animating;
	int pos, x, y;

public:
	PaneSplitter();
	~PaneSplitter();
};

//----------------------------------------------------------------------------------------------

class DockableCtrl : Pte<DockableCtrl>, public TopWindow
{
public:
	typedef DockableCtrl CLASSNAME;
	
	enum { DOCK_LEFT, DOCK_TOP, DOCK_RIGHT, DOCK_BOTTOM, DOCK_TABBED, DOCK_NONE };
	enum { STATE_SHUT, STATE_SHOW, STATE_HIDE, STATE_AUTO, STATE_TABBED };
	enum { TYPE_WINDOCK, TYPE_TABDOCK, TYPE_BARDOCK };	

	DockableCtrl& DockTop()					{ dockalignment = DOCK_TOP;		return *this;}
	DockableCtrl& DockLeft()				{ dockalignment = DOCK_LEFT;	return *this;}			
	DockableCtrl& DockRight()				{ dockalignment = DOCK_RIGHT;	return *this;}
	DockableCtrl& DockBottom()				{ dockalignment = DOCK_BOTTOM;	return *this;}
	DockableCtrl& DockTabbed()				{ dockalignment = DOCK_TABBED;	return *this;}

	DockableCtrl& StateShut()				{ dockstate =  STATE_SHUT;		return *this;}
	DockableCtrl& StateShow()				{ dockstate =  STATE_SHOW;		return *this;}
	DockableCtrl& StateHide()				{ dockstate =  STATE_HIDE;		return *this;}
	DockableCtrl& StateAuto()				{ dockstate =  STATE_AUTO;		return *this;}
	DockableCtrl& StateTabbed()				{ dockstate =  STATE_TABBED;	return *this;}
	
	DockableCtrl& Posit(int position)		{ dockposition = position;		return *this;}
	
	DockableCtrl& Style(int alignment, int state, int position);

	inline int Alignment() 	const			{ return dockalignment; }
	inline int State() 		const			{ return dockstate;		}
	inline int Position()	const			{ return dockposition;	}
	
	inline bool IsDocked() 					{ return !IsShut() && !IsTabbed() && !IsFloating();	}
	inline bool IsHidden()					{ return IsDocked() && dockstate == STATE_HIDE;		}
	inline bool IsAutoHidden()				{ return IsDocked() && dockstate == STATE_AUTO;		}
	inline bool IsShut()					{ return dockalignment == DOCK_NONE && dockstate == STATE_SHUT;}
	inline bool IsFloating()				{ return dockalignment == DOCK_NONE && dockstate != STATE_SHUT;}
	inline bool IsTabbed()					{ return /*dockalignment == DOCK_TABBED &&*/ dockstate == STATE_TABBED;}

	int GetType()							{ return type;	}
	virtual Size& SizeHint()				{ return size;	}

protected:
	virtual void  SetSizeHint(Size& sz)		{ size = sz; 	}
	virtual void  RefreshSizeHint();	
	void SetType(int _type)					{ type = _type; }
		
private:
	int dockalignment, dockstate, dockposition, type;
	Size size;
	
public:
	virtual void Dock(int alignment, int state, int position, bool check = true);
	virtual void FloatEx(Rect& r);
	virtual void Float();
	virtual void Show();
	virtual void Hide();
	virtual void AutoHide();
	virtual void Shut();
	virtual void Menu();
	virtual void Settings();
	
protected:
	void OpenWindow(Rect& r);
	void ShutWindow();
	void OnShutWindow()									{ ShutWindow(); Style(DOCK_NONE, STATE_SHUT, Position()); }

	virtual void Layout()								{ TopWindow::Layout(); if(IsOpen() && IsFloating()) RefreshSizeHint(); }
		
protected:
	DockableCtrl& 	SetBase(DockCtrl* ctrlbase)			{ base = ctrlbase;		return *this;	}
	DockCtrl& 		GetBase()							{ ASSERT(base); 		return *base;	}
	DockableCtrl&	SetOwnerTab(TabWindow *tabwin)		{ tabwindow = tabwin;	return *this;	}
	TabWindow&		GetOwnerTab()						{ ASSERT(tabwindow); return *tabwindow;	}	
	    			            
	friend DockCtrl;
	friend DockWindow;
	friend TabWindow;
	friend PaneFrame;
	
private:
	DockCtrl *base;
	TabWindow *tabwindow;
	MenuBar menu;

//===============================================
// DockableCtrl: DnD methods.
//===============================================
	
public:
	
	void Dragging(bool state = false)					{ isdragged = state; 	}
	void Dropping(bool state = false)					{ isdropped = state;	}

	void SetDropTarget(int target = DOCK_NONE, int state = STATE_SHOW)	{ dndtarget = target; dndstate = state;	}
	int	 GetDropTarget()												{ return dndtarget;		}
	int  GetDropState()													{ return dndstate;		}
	bool HasDropTarget()												{ return dndtarget == DOCK_NONE ? false : true;}			
	bool IsDropTargetAvailable(Rect paneframe, Point p)					{ return paneframe.Contains(p); }
	
	inline bool IsDragged()								{ return isdragged; }
	inline bool IsDropped()								{ return isdropped; }

protected:
	void StartWindowDrag();
	
private:
	int  dndtarget, dndstate;
	bool isdragged, isdropped;
	
//===============================================
// DockableCtrl: dock-menu and dragbar methods.
//===============================================

protected:

	void ContextMenu(Bar& menubar);
	void DockableCtrlMenu(Bar& menubar);
	void DockableCtrlDockMenu(Bar& menubar);

//===============================================
// DockableCtrl: BarCtrl-based child ctrl support.
//===============================================
	
protected:
	DockableCtrl& AddBarCtrl(CtrlFrame& barctrl)		{ AddFrame(barctrl);  haschildbar = TRUE; return *this; }
	void AddChildBarSize(int width, int height)			{ childsize.cx += width; childsize.cy += height; }
	void SubChildBarSize(int width, int height)			{ childsize.cx -= width; childsize.cy -= height; }
	virtual void HideDragBar() 							{ hasdragbar = false;		}
	virtual void ShowDragBar() 							{ hasdragbar = true; 		}
	bool HasChildBar() const							{ return haschildbar; 		}		
	bool HasDragBar() const								{ return hasdragbar; 		}

public:
	Size&  GetChildBarSize()							{ return childsize; 		}		

private:
	Size childsize;
	bool haschildbar;
	bool hasdragbar;


//===============================================
// DockableCtrl: label (title/ID) methods.
//===============================================	
	
public:
	DockableCtrl& SetLabel(String  docktitle);									
	DockableCtrl& SetIcon(Image dockicon);

 	String GetLabel() const	{ return  title; }
	Image  GetIcon () const { return  icon;  }  	
	
private:
	String title;
	Image  icon;
					
public:
	DockableCtrl& Add(Ctrl& ctrl)						{ this << ctrl; return *this; }
	DockableCtrl& operator<<(Ctrl& ctrl)				{ Add(ctrl);	return *this; }

	DockableCtrl();
	~DockableCtrl();
};

//----------------------------------------------------------------------------------------------

class DockWindow : public DockableCtrl
{
public:
	typedef DockWindow CLASSNAME;

	DockWindow&	AddMenuBar(MenuBar& bar);
	DockWindow& AddToolBar(ToolBar& bar);
	DockWindow& AddStatusBar(StatusBar& bar);
	
	inline bool HasMenuBar()	{ return hasmenubar; 	}
	inline bool HasToolBar()	{ return hastoolbar; 	}
	inline bool HasStatusBar()	{ return hasstatusbar;	}

	virtual Rect GetCtrlRect();
	virtual Rect GetCtrlView()	{ return GetCtrlRect(); 		}	
	virtual Size GetCtrlSize()	{ return GetCtrlRect().Size();	}

protected:
	void OnMenuButton()			{ DockableCtrl::ContextMenu(*menubar); 	}
	void OnShutButton()			{ DockableCtrl::Shut();					}
	void OnAutoHideButton()		{ DockableCtrl::AutoHide();				}
	void ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags);
	void MouseMove(Point p, dword keyflags);
	void Paint(Draw& d);	

#ifdef PLATFORM_WIN32
	virtual LRESULT  WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
#endif
	
private: 
	bool hasmenubar, hastoolbar, hasstatusbar;
	MenuBar*   menubar;
	ToolBar*   toolbar;
	StatusBar* statusbar;
	friend TabWindow;

//===============================================
// DockWindow::DragBarButton
// Nested helper class.
//===============================================

private:
	struct DragBarButton : public Button
	{
	public:
		void  Paint(Draw& d);
		Ctrl& SetImage(const Image& normal, const Image& highlighted, const Image& pushed);

		const	DockCtrlChStyle::Style* style;
		static const DockCtrlChStyle::Style& StyleDefault();
		static const DockCtrlChStyle::Style& StyleClassic();
		static const DockCtrlChStyle::Style& StyleEnhanced();

		DockWindow& SetStyle(const DockCtrlChStyle::Style& s);
		const DockCtrlChStyle::Style*  GetStyle()				{ return style ? style : &DockCtrlChStyle::StyleDefault(); 	}

	private:
		Image _nimage, _himage, _pimage;
	};

//===============================================
// DockWindow::DragBar
// Nested helper class.
//===============================================

private:
	class DragBar : public Ctrl, public CtrlFrame
	{
	public:
		typedef DragBar CLASSNAME;
		enum {	BAR_MARGIN = 6, BAR_SPACE = 20, BAR_SBHEIGHT = 4, BAR_SBSEPARATOR = 1, BAR_FILEICON = 16, BAR_SPACEICON = 5 };

		void SetColor(const Color& c)			{ _color =  c;		}
		void SetDock(DockWindow* dock)			{ _parent = dock; 	}
		DockWindow& GetDock()					{ ASSERT(_parent); return  *_parent; }	
		int GetWidth();
		int GetHeight();
		
	private:
		Color		_color;
		DockWindow* _parent;
		Size		_titlesize;
		
	public:
	
		void FrameAdd(Ctrl& parent);
		void FrameRemove();
		void FrameLayout(Rect& r);
		void FrameAddSize(Size& s);

		void Paint(Draw& d);

	public:
		DragBar() : _parent(0), _titlesize(0) {  SetFrame(FieldFrame()); }
	};
	
//===============================================
// DockWindow: Dragbar related methods.
//===============================================

public:
	virtual void HideDragBar(); 	
	virtual void ShowDragBar();

//===============================================
// DockWindow: Chameleon support
//===============================================

public:
	const	DockCtrlChStyle::Style* style;
	static const DockCtrlChStyle::Style& StyleDefault();
	static const DockCtrlChStyle::Style& StyleClassic();
	static const DockCtrlChStyle::Style& StyleEnhanced();

	DockWindow& SetStyle(const DockCtrlChStyle::Style& s);
	const DockCtrlChStyle::Style*  GetStyle()				{ return style ? style : &DockCtrlChStyle::StyleDefault(); 	}

	
private:
	DragBar _dragbar;
	DragBarButton _menubutton, _shutbutton, _autohidebutton;
	
public:
	DockWindow();
	~DockWindow();
};

//----------------------------------------------------------------------------------------------

class TabWindow : public DockWindow
{
public:
	typedef TabWindow CLASSNAME;
	
	void Attach(DockableCtrl& ctrl, int makeactive = false);
	void Detach(DockableCtrl& ctrl);
	void DetachAll();
	bool HasCtrlInRange();
	bool HasTabbedDock();
	int	 ChildCount();
	void SetActive(int index);
	void SetActive(DockableCtrl& ctrl);
	DockableCtrl& GetActiveCtrl();
	int  GetActiveTab();
	
	void StartTabAnimation(); 
	void StopTabAnimation();	
	bool IsTabAnimating()				{ return pane.IsAnimating(); 	}
	bool IsDestroyed()					{ return destroyed;				}	

	TabInterface& 	GetTabs()			{ return tabs;	}
	PaneSplitter&	GetChilds()			{ return pane; 	}
	
	TabWindow&		SetLayout(int n) 	{ if(tabs.IsChild()) RemoveFrame(tabs); AddFrame(tabs.SetLayout(n)); return *this; }
	
protected:
	virtual void OnActiveTab();
	virtual void OnTabClose(int id, DockableCtrl& ctrl);
	virtual void OnTabDrag(int id, DockableCtrl& ctrl);
	virtual void OnTabActive(int id, DockableCtrl& ctrl);
	virtual void OnTabContextMenu(int id, DockableCtrl& ctrl);	
	
private:
	void AttachNormal(DockableCtrl& ctrl, int makeactive = false);
	void AttachNested(DockableCtrl& ctrl, int makeactive = false);
	void Attach0(DockableCtrl& ctrl, int makeactive = false);
		
	bool RemoveTabWindow();
	void UpdateTabLayout();
	void SetTabWindowLabel(DockableCtrl& ctrl);
		
private:
	TabInterface tabs;
	PaneSplitter pane;
	DockableCtrl* activectrl;
	int position;
	bool destroyed;

public:
	TabWindow();
	~TabWindow();
};

//----------------------------------------------------------------------------------------------

class AutoHideBar : public TabInterface
{
public:
	typedef AutoHideBar CLASSNAME;

	AutoHideBar& 	Attach(DockableCtrl& ctrl);
	void 			Detach(DockableCtrl& ctrl);

	void ShowBar();
	void HideBar();

	int	 GetSize()			{ return CustomFrame::size; }
	void SetSize(int sz) 	{ CustomFrame::size = sz; RefreshParentLayout(); }

protected:

	virtual void	FrameRemove();
	virtual void	FrameAdd(Ctrl& parent);
	virtual void 	MouseMove(Point p, dword keysflags);
	virtual void 	MouseLeave();
	virtual void	MouseEnter(Point p, dword keyflags);


	void ShowWindow();
	void HideWindow();
	
	void OnHighlight();
	void OnClose(int id, DockableCtrl& ctrl);
	
	void AdjustSize(Rect &r, const Size &sz);

private:	
	struct PopupWindow : public Ctrl
	{
		Callback WhenEnter;
		Callback WhenLeave;
		virtual void ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags); 
		virtual void Paint(Draw& d) { Ctrl::Paint(d); }
	};

	PopupWindow 	popup;
	DockableCtrl* 	ctrl;
		
public:
	const	DockCtrlChStyle::Style* style;
	static const DockCtrlChStyle::Style& StyleDefault();
	static const DockCtrlChStyle::Style& StyleClassic();
	static const DockCtrlChStyle::Style& StyleEnhanced();

	AutoHideBar& SetStyle(const DockCtrlChStyle::Style& s)	{ style = &s; Refresh(); return *this; }
	const DockCtrlChStyle::Style*  GetStyle()				{ return style ? style : &DockCtrlChStyle::StyleDefault(); 	}

public:
	AutoHideBar();
    ~AutoHideBar(); 
};

//----------------------------------------------------------------------------------------------
class PaneFrame : public CtrlFrame, private Ctrl
{

public:
	typedef PaneFrame CLASSNAME;

	PaneFrame& Attach(DockableCtrl& ctrl);
	PaneFrame& Detach(DockableCtrl& ctrl);


	void  ShowFrame();
 	void  ShowFrame(Size& sz);
 	void  HideFrame();
 	void  LockFrame();
 	void  UnlockFrame();

	bool  HasChild(DockableCtrl& ctrl);
	DockableCtrl* GetChild(int position);
	int	  GetCount();
	Vector<TabWindow*>& GetTabWindows();
	
private:
	PaneFrame& AttachAsDock(DockableCtrl& ctrl);
	PaneFrame& AttachAsTab(DockableCtrl& ctrl);
	PaneFrame& AttachAsAuto(DockableCtrl& ctrl);

	TabWindow*	AddTabWindow(DockableCtrl& ctrl);
	void		RemoveTabWindow(TabWindow* tabwindow);
	void		RemoveTabWindow(int position);
	void 		RemoveTabWindows();
	void		AddtoTabWindow(DockableCtrl& ctrl);
	void		RemovefromTabWindow(DockableCtrl& ctrl);
	void		RemovefromTabWindow(int position);
	void		RefreshTabWindowList();
	TabWindow*	FindTabWindow(int position);
	inline TabWindow* GetActiveTabWindow();
	void		SetActiveTabWindow(TabWindow* tabwindow);
	bool		HasTabWindow(TabWindow* tabwindow);
			

private:
	PaneSplitter dockcontainer;
	AutoHideBar	 hidebar;
	TabWindow*	 activetabwindow;
	Vector<TabWindow*> tabwindows;
		
public:
	virtual void FrameAdd(Ctrl& parent);
	virtual void FrameRemove();
	virtual void FrameAddSize(Size& sz);
	virtual void FrameLayout(Rect& r);

	virtual void Paint(Draw& draw);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual Image CursorImage(Point p, dword keyflags);

private:
	Point ref;
	Size  parentsize;
	int   type, minsize, sizemin, maxsize;
	int   size, size0;
	int	  dndpos;
	bool  created;
	bool  locked;
	int   BoundSize();
	DockCtrl* base;
	int	  ncount;
	
public:
	enum { LEFT, TOP, RIGHT, BOTTOM };

	PaneFrame& Create(int alignment, int size); 
	PaneFrame& Set(Ctrl& c, int size, int type);
	PaneFrame& Left(Ctrl& c, int size)    		{ return Set(c, size, LEFT); }
	PaneFrame& Top(Ctrl& c, int size)    	 	{ return Set(c, size, TOP); }
	PaneFrame& Right(Ctrl& c, int size)  	 	{ return Set(c, size, RIGHT); }
	PaneFrame& Bottom(Ctrl& c, int size)  		{ return Set(c, size, BOTTOM); }

	PaneFrame& MinSize(int sz)        		    { minsize = sz; return *this; }
	PaneFrame& MaxSize(int sz)					{ maxsize = sz; return *this; }
	PaneFrame& SizeMin(int sz)         			{ sizemin = sz; return *this; }

	PaneFrame& Vert()							{ dockcontainer.Vert(); return *this; 	}
	PaneFrame& Horz()							{ dockcontainer.Horz(); return *this; 	}
	PaneFrame& RefreshPaneFrame()				{ dockcontainer.Layout(); return *this; }
	
	PaneFrame& SetBase(DockCtrl* ctrlbase)		{ base = ctrlbase;	return *this;	}
	DockCtrl&  GetBase()						{ ASSERT(base); return *base; 		}
	
	int  GetType() const                      	{ return type; }
	int  GetSize() const                      	{ return size; }
	void SetSize(int sz)                      	{ size = sz; RefreshParentLayout(); }

	Rect GetScreenView()						{ return Ctrl::GetScreenView(); 	}
	Rect GetScreenRect()						{ return Ctrl::GetScreenRect();		}	
	
	void Enable()								{ Ctrl::Enable(); 	}
	void Disable()								{ Ctrl::Disable();	}
	void Show(bool show = true)				  	{ Ctrl::Show(show); }
	void Hide()								  	{ Ctrl::Hide(); 	}
	
	bool IsShown()							  	{ return Ctrl::IsShown(); 	}
	bool IsEnabled()							{ return Ctrl::IsEnabled();	}
	bool IsCreated()							{ return created;			}
	bool IsLocked()								{ return locked;			}
	         
public:
	PaneFrame();
	~PaneFrame();
	
//===============================================
// DockPane: Drag-n-Drop related methods.
//===============================================

public:
	bool	HasCtrlInRange (DockableCtrl&  ctrl, Point p);
	
	void	StartPaneFrameAnimation(DockableCtrl& ctrl, int position);
	void	StartTabWindowAnimation(DockableCtrl& ctrl, int position);
	void	StopPaneFrameAnimation();
	void	StopTabWindowAnimation();
		    	                    
private:
	bool	CalculateCtrlRange (DockableCtrl& t, Ctrl& c, Point p);
	bool	CalculateEmptyRange (Ctrl& c, Point p);

public:		    	               
	void	DnDSourceinRange()				{ _dndsource = true;  }
	void	DnDSourceoutofRange()			{ _dndsource = false; }
	bool	DnDHasSource()	const			{ return _dndsource;  }
		
	void	DnDAnimate(int position = dnd_none);

	void	DoDropAnimation(int command = ANIMATE_ALL, int position = dnd_none, DockableCtrl* dock = NULL);	
	bool	DnDIsAnimating() const			{ return _dndanim;	  }
	
	void	DnDAnimStart()					{ _dndanim = true; 	  }
	void 	DnDAnimStop()					{ _dndanim = false;	  }
		
public:
	enum	{ dnd_first = 1, dnd_last = 2, dnd_tab = 128, dnd_none = -1 };
	enum	{ TAB_ANIMATION_START, TAB_ANIMATION_STOP, PANE_ANIMATION_START, PANE_ANIMATION_STOP, ANIMATE_ALL};
		
private:
	bool _dndsource;
	bool _dndanim;
	int  _dndposition;
	int  _dndtype;
};

//----------------------------------------------------------------------------------------------

class DockCtrl : public Ctrl
{
public:
	typedef DockCtrl CLASSNAME;

	enum { ALLOW_LEFT, ALLOW_TOP, ALLOW_RIGHT, ALLOW_BOTTOM, ALLOW_ALL, ALLOW_NONE };
	
	Ctrl& Dock(DockableCtrl& ctrl);

	void ShowPaneFrame(int alignment);
	void HidePaneFrame(int alignment);
	void LockPaneFrame(int alignment);
	
	DockCtrl&		SetLayout(Ctrl& parent, int cx, int cy);
	
	Ctrl& 			GetBase()					{ ASSERT(base); return *base;	}
	PaneFrame& 		GetPaneFrame(int alignment)	{ return pane[alignment]; 		}
	AutoHideBar& 	GetHideBar(int alignment)	{ return hide[alignment];		}
	
	DockCtrl&	AllowSide(int sides)			{ allowedsides = sides; return *this; 	}
	DockCtrl& 	SetKey(dword key = 0)			{ ctrlkey = key;	return *this; 		}
	dword 		GetKey() const					{ return ctrlkey; }
	void		ControlPanel();				

	void RefreshWidgetLayout();
		              
private:
	void Attach(DockableCtrl& ctrl);
	void Attach(DockableCtrl* ctrl);
	void Detach(DockableCtrl& ctrl);
	void Detach(DockableCtrl* ctrl);
	void DoDragAndDrop(DockableCtrl& ctrl, Point p, Size& sz);

private:
	struct CtrlRecord : Moveable<CtrlRecord>
	{
		DockableCtrl* ctrl;
		int alignment, state, position;
		CtrlRecord() { ctrl = NULL; }
		CtrlRecord(DockableCtrl& _ctrl)
		{
			ctrl = &_ctrl;
			alignment = ctrl->Alignment();
			state = ctrl->State();
			position = ctrl->Position();
		}
	};
	Vector<CtrlRecord*> ctrlrecords;
	void AddCtrlRecord(DockableCtrl& ctrl);
	CtrlRecord* GetCtrlRecord(DockableCtrl* ctrl);
	CtrlRecord* GetCtrlRecord(int index);
	void DeleteCtrlRecord(DockableCtrl& ctrl);
	void DeleteCtrlRecords();

public:
	Vector<TabWindow*>& GetTabWindows() { return tabwindows; }

private:
	void RemoveTabWindows();
	
private:
	void PanelAction();
	void SelectSkin ();
	void RefreshPanel();
			
private:
	PaneFrame 	pane[4];
	AutoHideBar	hide[4];
	
	Ctrl *base;
	Size panesize;
	int allowedsides;
	dword ctrlkey;
	Vector<TabWindow*> tabwindows;
	Vector<ImageCtrl*> panelicons;
	WithControlPanelLayout<TopWindow> controlpanel;
	
	friend DockableCtrl;
	friend DockWindow;
	friend TabWindow;
	friend PaneFrame;

public:
	const	DockCtrlChStyle::Style* style;
	static const DockCtrlChStyle::Style& StyleDefault();
	static const DockCtrlChStyle::Style& StyleClassic();
	static const DockCtrlChStyle::Style& StyleEnhanced();

	DockCtrl& SetStyle(const DockCtrlChStyle::Style& s);
	const DockCtrlChStyle::Style*  GetStyle()				{ return style ? style : &DockCtrlChStyle::StyleDefault(); 	}

public:
	DockCtrl();
	~DockCtrl();
};



#endif