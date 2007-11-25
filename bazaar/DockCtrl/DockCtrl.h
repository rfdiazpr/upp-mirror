//==============================================================================================
// DockCtrl: A dockable widget framework for U++
// Author:	 Ismail YILMAZ (A.k.a. Maldoror)
// Version:	 0.49.9.9a
//==============================================================================================

#ifndef DOCKCTRL_H
#define DOCKCTRL_H

#include <CtrlLib/CtrlLib.h>
#include <Draw/Draw.h>


using namespace UPP;


#define LAYOUTFILE <DockCtrl/DockCtrl.lay>
#include <CtrlCore/lay.h>

class DockCtrl;
class DockPane;
class DockWindow;
class DockableCtrl;

//----------------------------------------------------------------------------------------------

class DockableCtrl : public TopWindow
{
public:
	typedef DockableCtrl CLASSNAME;
	
	enum { dock_top = 0, dock_bottom = 1, dock_left = 2, dock_right = 3,  dock_none = 5 };
	enum { state_shut = 0, state_show = 1, state_hide = 2, state_autohide = 3 };

	DockableCtrl& Style(int alignment, int state, int position) { _alignment = alignment; _state = state; _position = position; return *this; }

	DockableCtrl& Top()						{ _alignment = dock_top; 	return *this;}				
	DockableCtrl& Left()					{ _alignment = dock_left;	return *this;}
	DockableCtrl& Right()					{ _alignment = dock_right;	return *this;}
	DockableCtrl& Bottom()					{ _alignment = dock_bottom;	return *this;}
	
	DockableCtrl& SShut()					{ _state = state_shut;		return *this;}
	DockableCtrl& SShow()					{ _state = state_show;		return *this;}
	DockableCtrl& SHide()					{ _state = state_hide;		return *this;}
	DockableCtrl& SAuto()					{ _state = state_autohide;	return *this;}
	
	DockableCtrl& Posit(int position)		{ _position = position;		return *this;}
	              
	inline int Alignment() const 			{ return _alignment;}
	inline int State()	   const			{ return _state;	}
	inline int Position()  const			{ return _position;	}		

	inline bool IsShut()					{ return _alignment == dock_none && _state == state_shut;}
	inline bool IsDocked() 					{ return _alignment != dock_none && _state != state_shut;}
	inline bool IsFloating()				{ return _alignment == dock_none && _state != state_shut;}
	inline bool IsHidden()					{ return _alignment != dock_none && _state == state_hide;}
	inline bool IsAutoHidden()				{ return _alignment != dock_none && _state == state_autohide;}

	
public:
	void Dock(int alignment, int state, int position, bool check = true);
	void FloatEx(Rect& windowrect);
	void Float();
	void Show();
	void Hide();
	void AutoHide();
	void Shut();
	
protected:
	void OpenFloatWindow(Rect windowrect);
	void ShutFloatWindow();
	
	void OnFloatWindowShut()				{ ShutFloatWindow(); Style(dock_none, state_shut, Position()); }
		
public:
	DockableCtrl& SetBase(DockCtrl* dockctrl)	{ _dockctrl = dockctrl; 	return *this;}
	DockableCtrl& SetPane(DockPane* dockpane)	{ _dockpane = dockpane;		return *this;}
	DockCtrl& GetBase()							{ return *_dockctrl; }
	DockPane& GetPane()							{ return *_dockpane; }

private:
	int  _alignment, _state, _position;

	DockCtrl* _dockctrl;
	DockPane* _dockpane;
	MenuBar   _dockmenu;
	
	friend DockPane;

//===============================================
// DockableCtrl: DnD methods.
//===============================================
	
public:
	
	bool DnDCalcDragPos(Rect dock, Rect tracker);
	bool DnDCalcDropPos(Rect pane, Point pos);
	
	void DnDTargetinRange(int dndtarget)	{ _dndtarget = dndtarget;  	}
	void DnDTargetoutofRange()				{ _dndtarget = dock_none;	}

	inline int DnDGetTarget()				{ return _dndtarget; 		}
	
	bool DnDHasTarget()						{ return _dndtarget == dock_none ? false : true; }

	void Dragging(bool dragstate = TRUE)	{ _dragstate = dragstate; }
	void Dropping(bool dropstate = TRUE)	{ _dropstate = dropstate; }
	
	inline bool IsDragged()			{ return _dragstate; }
	inline bool IsDropped()			{ return _dropstate; }

private:
	int  _dndtarget;
	bool _dragstate, _dropstate;

//===============================================
// DockableCtrl: dock-menu and dragbar methods.
//===============================================

protected:

	void ContextMenu(Bar& menubar);

	void DockableCtrlMenu(Bar& menubar);
	void DockableDockMenu(Bar& menubar);

	virtual void HideDragBar() 	{ _hasdragbar = false;}
	virtual void ShowDragBar() 	{ _hasdragbar = true; }
		
	bool HasDragBar() const		{ return _hasdragbar; }
	
private:
	bool _hasdragbar;
	
//===============================================
// DockableCtrl: label (title/ID) methods.
//===============================================	

public:
	DockableCtrl& SetLabel(const char* title, Color color = SColorText())	{ _label.SetText(title).SetInk(color);	return *this; }
	DockableCtrl& SetIcon(Image dockicon)									{ _icon = dockicon; _label.SetImage(dockicon, 4); return *this;}
   
   	String GetLabel() const	{ return _label.GetText(); }
 	Image  GetIcon () const { return _icon;			   }  	

protected:
	Label& GetLabelCtrl()	{ return _label; }
	
private:
	Label _label;
	Image _icon;

//===============================================
// DockableCtrl: BarCtrl-based child ctrl support.
//===============================================

protected:
	DockableCtrl& AddBarCtrl(CtrlFrame& barctrl)		{ AddFrame(barctrl); _haschildbarctrl = TRUE; return *this; }
	
	void AddChildBarSize(int width, int height)			{ _childsize.cx += width; _childsize.cy += height; }
	void SubChildBarSize(int width, int height)			{ _childsize.cx -= width; _childsize.cy -= height; }
	
	bool   HasChildBar() const							{ return _haschildbarctrl; 	}

public:
	Size&  GetChildBarSize()							{ return _childsize; 		}		

private:
	bool _haschildbarctrl;
	Size _childsize;

//===============================================
// DockableCtrl: Constructors/destructor.
//===============================================

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

//===============================================
// DockWindow: BarCtrl-based child ctrl support. 
//===============================================

public:
	DockWindow& AddMenuBar(MenuBar& menubar);
	DockWindow& AddToolBar(ToolBar& toolbar);
	DockWindow& AddStatusBar(StatusBar& statusbar);
	
	DockWindow& TitleColor(const Color& color)			{ _dragbar.SetColor(color); return *this; }
	
	inline bool HasMenuBar() const						{ return _hasmenubar;	}
	inline bool HasToolBar() const						{ return _hastoolbar;	}
	inline bool HasStatusBar() const					{ return _hasstatusbar; }
	
	Rect GetDockRect();
	Rect GetDockView();									
	
private:
	bool _hasmenubar, _hastoolbar, _hasstatusbar;

	MenuBar*    _menubar;   
	ToolBar*   _toolbar;
	StatusBar* _statusbar;

	
//===============================================
// Dockwindow: Event processing methods.
//===============================================

protected:
	void OnMenuButton()									{ DockableCtrl::ContextMenu(*_menubar); }
	void OnShutButton()									{ DockableCtrl::Shut(); 	}	
	void OnAutoHideButton()								{ DockableCtrl::AutoHide(); }
	
	void LeftDown(Point p, dword keyflags);
	void LeftUp(Point p, dword keyflags);
	void RightDown(Point p, dword keyflags);	
	void RightUp(Point p, dword keyflags);
	void MouseMove(Point p, dword keyflags);

	void ChildMouseEvent(Ctrl *child, int event, Point p, int zdelta, dword keyflags);

	void Paint(Draw& d);

protected:
	
#ifdef PLATFORM_WIN32
	virtual LRESULT  WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
#endif

//===============================================
// DockWindow: Class constructors/destructor.
//===============================================

public:

	DockWindow();
	~DockWindow();
	
//===============================================
// DockWindow::DragBarButton
// Nested helper class.
//===============================================

private:
	struct DragBarButton : public Button
	{
		void  Paint(Draw& d);
		Ctrl& SetImage(const Image& normal, const Image& highlighted, const Image& pushed);

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
		
		void SetColor(const Color& c)			{ _color =  c;		}
		DockWindow& GetDock()					{ return  *_parent; }
		void SetDock(DockWindow* dock)			{ _parent = dock; 	}
	
	private:
		Color		_color;
		DockWindow* _parent;
		
	protected:
	
		void FrameAdd(Ctrl& parent);
		void FrameRemove();
		void FrameLayout(Rect& r);
		void FrameAddSize(Size& s);

		void Paint(Draw& d);

	public:
		DragBar() : _parent(0) {  SetFrame(FieldFrame()); }
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
	struct Style : ChStyle<Style>
	{
		Image barbackgroundimage;
		Image shutbuttonimage[3], autohidebuttonimage[3], menubuttonimage[3];
	};
	
	static const Style& ClassicStyle();
	static const Style& EnhancedStyle();
	static const Style& ModernStyle();

	void SetVisualStyle(const Style& st);
	const Style* GetVisualStyle()	const {	return _visualstyle ? _visualstyle : &ClassicStyle(); }
	
private:

	DragBar _dragbar;
	DragBarButton _menubutton, _shutbutton, _autohidebutton;

	const Style* _visualstyle;
};

//----------------------------------------------------------------------------------------------

class DockPane : public CtrlFrame, private Ctrl 
{

//===============================================
// DockPane nested class manipulation methods.
//===============================================	

public:
    DockPane& CreatePane(int alignment, int size);							

	DockPane& DockChild(DockableCtrl& dock, int size);

	Ctrl* GetChildAt(int position);
	
 	void RefreshChilds();
 	
	void  DockAdd(DockableCtrl& dock);
	
	void  DockRemove(DockableCtrl& dock);
	void  DockRemove();				

 	void  PaneShow();
 	void  PaneHide();
 	void  PaneLock();
 	
	DockPane& Vert () 			{ container.Vert(); return *this;}								
	DockPane& Horz ()			{ container.Horz(); return *this;}
	
	void Enable ()				{ this->Ctrl::Enable();  }
	void Disable()				{ this->Ctrl::Disable(); }
	
	bool HasDock() const;	
		
	bool IsCreated() const		{ return created; 									}
	bool IsEnabled() const		{ return this->Ctrl::IsEnabled(); 					}
	
	int   GetCount ()			{ return container.GetCount();						}
	Size  GetSize  ()			{ return this->Ctrl::GetSize(); 					}

	DockPane& DefSize(int sz)	{ defsize  = sz;	return *this;}
	DockPane& SetSize(int sz)   { (sz > maxsize) ? size = maxsize : size = sz; this->Ctrl::RefreshParentLayout(); return *this;}
	DockPane& RefreshPane()		{ container.Layout();  return *this; }

public:	
	Rect  GetScreenView()		{ return this->Ctrl::GetScreenView(); 				}
	Rect  GetScreenRect()		{ return this->Ctrl::GetScreenRect();				}	

protected:
	void  Animate()			{}
	
private:
	Splitter container, basecontainer;

	int	  	dockcount, state, alignment, position, created;
	bool 	horizchild, vertchild;
	int 	maxsize, defsize;

//===============================================
// DockPane: Drag-n-Drop related methods.
//===============================================

public:
	bool	DnDCalcSourcePos(Point pos);
	
	void	DnDSourceinRange()				{ _dndsource = true;  }
	void	DnDSourceoutofRange()			{ _dndsource = false; }
	bool	DnDHasSource()	const			{ return _dndsource;  }
		
	void	DnDAnimate(int position = dnd_none);
	bool	DnDIsAnimating() const			{ return _dndanim;	  }
	
	void	DnDAnimStart()					{ _dndanim = true; 	  }
	void 	DnDAnimStop()					{ _dndanim = false;	  }
		
public:
	enum	{dnd_first = 1, dnd_last = 2, dnd_tab = 8, dnd_none = 0 };
	
private:
	bool	DnDCalcHorzPos(Ctrl& c, Point pos);
	bool	DnDCalcVertPos(Ctrl& c, Point pos);
	bool	DnDCalcEmptyPos(Ctrl& c, Point pos);
	
private:
	bool _dndsource;
	bool _dndanim;
	int  _dndposition;
	
//===============================================
// DummyCtrl: nested helper class.
// Used for DnD animation.
//===============================================
	
private:

	class DummyCtrl : public Ctrl
	{
	public:
		typedef DummyCtrl CLASSNAME;
		
		void AnimateVert();
		void AnimateHorz();
		
		void Paint(Draw& d)	{ d.DrawRect(GetSize(), GUI_GlobalStyle() >= GUISTYLE_XP ? Blend(SColorHighlight, SColorFace) : SColorShadow); }

	private:
		Rect _defrect;
		
	public:
		DummyCtrl()  { SetFrame(FieldFrame()); }
	};

private:
	DummyCtrl dummy;
	
//===============================================
// Below are the original SplitterFrame Ctrl 
// interface (access styles are modified).
//===============================================

public:	
	DockPane& SetDefSize(int sz){ defsize = sz;					}
	int   GetDefSize()			{ return defsize; 				}						

protected:
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
	int   type, minsize, sizemin;
	int   size, size0;
	bool  mousestate;

	int   BoundSize();

private:
	enum { LEFT, TOP, RIGHT, BOTTOM };

	void Serialize(Stream& s);
              
	DockPane& Set(Ctrl& c, int size, int type);
	DockPane& Left(Ctrl& c, int size)    { return Set(c, size, LEFT); }
	DockPane& Top(Ctrl& c, int size)     { return Set(c, size, TOP); }
	DockPane& Right(Ctrl& c, int size)   { return Set(c, size, RIGHT); }
	DockPane& Bottom(Ctrl& c, int size)  { return Set(c, size, BOTTOM); }

public:
	DockPane& MinSize(int sz)            { minsize = sz; return *this; }
	DockPane& SizeMin(int sz)            { sizemin = sz; return *this; }

	int  GetType() const                 { return type; }
	int  GetSize() const                 { return size; }

	DockPane();
};

//----------------------------------------------------------------------------------------------

static DockCtrl* dockctrl_ptr = NULL;

class DockCtrl : public Ctrl
{
public:
	typedef DockCtrl CLASSNAME;

//===============================================
// DockCtrl: Manipulation methods.
//===============================================

public:
	Ctrl& Dock(DockableCtrl& ctrl);	
	DockCtrl& Base(Ctrl* ctrl)				{ _basectrl = ctrl; return *this; 	}
	
	DockCtrl& SetPaneSize(int sz)			{ _size = sz;		return *this; 	}
	int GetPaneSize() const					{ return _size;			 	  		}

	DockPane& GetPane(int alignment)		{ return _pane[alignment];			}

private:

	void AddDock(DockableCtrl* dock)		{ AddDock(*dock); 								} 
	void AddDock(DockableCtrl& dock)		{ GetPane(dock.Alignment()).DockAdd(dock); 		}
	
	void RemoveDock(DockableCtrl* dock)		{ RemoveDock(*dock); 							}
	void RemoveDock(DockableCtrl& dock)		{ GetPane(dock.Alignment()).DockRemove(dock); 	}					
	void RemoveDock()						{}															

 	void ShowPane(int alignment);
 	void HidePane(int alignment);
 	void LockPane(int alignment);
 	
 	void RefreshPane(int alignment)			{ GetPane(alignment).RefreshPane(); }

public:
	void ResetLayout();	
	
private:
	int _size;
	DockPane _pane[4];	
	Ctrl* _basectrl;

private:
	friend DockableCtrl;
	friend DockWindow;
	
//===============================================
// DockCtrl: Dock and panel control methods
//===============================================

public:
	void OpenControlPanel();
	DockCtrl& Key(dword key = 0)			{ _ctrlkey = key;	return *this;	}
	dword GetKey() const					{ return _ctrlkey; 					}
		
private:
	static bool DockCtrlKeyHook(Ctrl* ctrl, dword key, int count)	{ if(key == (dockctrl_ptr->GetKey() == 0 ? K_CTRL_HOME : dockctrl_ptr->GetKey())) dockctrl_ptr->OpenControlPanel(); ctrl->Key(key, count); return true; }
	void OnPanelAction();
	void OnVisualStyleList();
		
private:
	dword _ctrlkey;
	bool  _hasctrlpanel;
	
	WithControlPanelLayout<TopWindow> controlpanel;

//===============================================
// DockCtrl: Dock list manipulation methods
//===============================================

private:
	struct CtrlRecord
	{
		DockableCtrl* ctrl;
		int alignment, state, position;
		//
		CtrlRecord() : ctrl(NULL) 		{alignment = state = position = 0;}
		CtrlRecord(DockableCtrl* _ctrl) { ctrl = _ctrl; alignment = _ctrl->Alignment(); state = _ctrl->State(); position = _ctrl->Position(); }
		~CtrlRecord() 					{ ctrl = NULL; alignment = state = position = 0;}
	};
	
Vector<CtrlRecord*> _entrylist;

private:
	void 		AddRecord(DockableCtrl* ctrl);
	void 		RemoveRecord(DockableCtrl* ctrl);
	CtrlRecord* FindRecord(DockableCtrl* ctrl);
	CtrlRecord* FindRecord(int index) {	return _entrylist.At(index); }
	inline void	ClearRecords()		  { int i = _entrylist.GetCount(); if(i !=0) for(int j = 0; j < i; j++) delete _entrylist.At(j); }

//===============================================
// DockCtrl: General DnD methods
//===============================================

private:
	void DoDragAndDrop(DockableCtrl& ctrl, Point pos);
	
//===============================================
// DockCtrl: constructors and destructor
//===============================================

public:
	DockCtrl();
	~DockCtrl();
};

#endif