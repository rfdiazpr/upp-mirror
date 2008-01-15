#ifndef _Docking_DockFrame_h_
#define _Docking_DockFrame_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <Docking/SplitterTree.h>

class DockFrame : public SplitterTree, public CtrlFrame
{
public:
	typedef DockFrame CLASSNAME;

	virtual void FrameAdd(Ctrl& parent);
	virtual void FrameRemove();
	virtual void FrameAddSize(Size& sz);
	virtual void FrameLayout(Rect& r);

	virtual void Paint(Draw& draw);
	virtual void Layout();

	virtual void LeftDown(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void LeftUp(Point p, dword keyflags);
	virtual Image CursorImage(Point p, dword keyflags);
		
private:
	Point ref;
	Size  parentsize;
	int   type, minsize, sizemin;
	int   size, size0;

	Node 		*animnode;
	int	  		nodesz, nodeinc, nodeacc;
	int	  		framesz, frameinc;
	Ptr<Ctrl>	animctrl;
	ParentCtrl	dummy;

	int   BoundSize();
	Rect  SplitterRect();

	void   StartNodeAnimate(Ctrl &c, int sz);
	void   StartFrameAnimate(int sz);

	void  AnimateTick();
	bool  AnimateFrame();
	bool  AnimateNode();
	
	enum { TIMEID_ACTION_CHECK = Ctrl::TIMEID_COUNT,
		   TIMEID_ANIMATE,
           TIMEID_COUNT };	
public:
	enum { LEFT, TOP, RIGHT, BOTTOM };

	DockFrame& Set(int size, int type);
	DockFrame& Left(int size)    { return Set(size, LEFT); }
	DockFrame& Top(int size)     { return Set(size, TOP); }
	DockFrame& Right(int size)   { return Set(size, RIGHT); }
	DockFrame& Bottom(int size)  { return Set(size, BOTTOM); }

	DockFrame& MinSize(int sz)            { minsize = sz; return *this; }
	DockFrame& SizeMin(int sz)            { sizemin = sz; return *this; }

	int  GetType() const                  { return type; }
	int  GetSize() const                  { return size; }
	void SetSize(int sz)                  { size = sz; RefreshParentLayout(); }
	
	void   Dock(bool vert, bool before, Ctrl &sibling, Ctrl &c, Size sz);
	void   DockRoot(Ctrl &c, bool first, Size sz);
	void   AnimateDock(bool vert, bool before, Ctrl &sibling, Ctrl &c, Size sz);
	void   AnimateDockRoot(Ctrl &c, bool first, Size sz);

	void   AnimateRemove(Ctrl &c, int fsz = -1);
	void   Remove(Ctrl &c, int fsz = -1);

	void   DockSwap(Ctrl &oldctrl, Ctrl &newctrl);
	
	bool   IsAnimating()				  { return frameinc || nodeinc; }
	
	DockFrame();
};

#endif
