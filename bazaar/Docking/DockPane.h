#ifndef _Docking_DockPane_h_
#define _Docking_DockPane_h_

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

class DockPane : public Splitter
{
public:
	typedef DockPane CLASSNAME;

private:
	struct DummyCtrl : public Ctrl
	{
		DummyCtrl() { minsize = stdsize = Null; Transparent(); NoWantFocus(); }
		Size minsize;
		Size stdsize;
		virtual Size GetMinSize() const { return minsize; }
		virtual Size GetStdSize() const { return stdsize; }
	};
	DummyCtrl dummy;
	int 	animix;
	int 	animtick;
	int 	animinc;
	int 	animinterval;
	int 	animmaxticks;

	void 	StartAnimate(int ix, Size sz);
	void 	AnimateTick();
	void 	EndAnimate();

	void 	SmartRepos(int ix, int pos);
	int 	GetMinPos(int notix);
	void 	FixChildSizes();
	
	int 	NormalPos(int ix) const			{ return (ix > 0) ? pos[ix] - pos[ix-1] : pos[ix]; }
	int		ClientToPos(const Size &sz);
	
	enum { TIMEID_ACTION_CHECK = Ctrl::TIMEID_COUNT,
		   TIMEID_ANIMATE,
           TIMEID_COUNT };	
public:
	enum { LEFT, TOP, RIGHT, BOTTOM };

	int 	FindIndex(Ctrl &child);
	Ctrl *	FindCtrl(int ix);
	int 	GetCount() const						{ return pos.GetCount(); }
	
	void	Swap(Ctrl &child, Ctrl &newctrl);
	void	Dock(Ctrl &newctrl, Size sz, int pos, bool animate);
	void 	Undock(Ctrl &newctrl, bool animate);
	void 	QuickDock(Ctrl &ctrl, int sz);
	
	int 	GetCtrlSize(int i) const				{ return NormalPos(i); }
	void	SetCtrlSize(int i, int sz)				{ pos[i] = (i == 0) ? sz : sz + pos[i-1]; }
	
	void 	Clear()									{ pos.Clear(); }
	
	bool	IsAnimating()		 					{ return animix >= 0; }
	void	SetAnimateRate(int ticks, int ms) 		{ animmaxticks = max(1, ticks);	animinterval = max(0, ms); }
	int		GetAnimMaxTicks() const					{ return animmaxticks; }
	int		GetAnimInterval() const					{ return animinterval; }
	
	DockPane();
};

#endif
