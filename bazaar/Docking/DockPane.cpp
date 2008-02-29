#include "DockPane.h"

int DockPane::ClientToPos(const Size &p)
{
	int w = (width>>1)*pos.GetCount()-1;
	return minmax(vert ? 10000 * p.cy / (GetSize().cy - w) : 10000 * p.cx / (GetSize().cx - w), 0, 9999);	
}

void DockPane::StartAnimate(int ix, Size sz)
{
	int tsz = ClientToPos(sz) - NormalPos(ix);
	int msz = GetMinPos(ix);
	
	if (msz < 10000 && msz+tsz > 10000) {
		Ctrl *c = FindCtrl(ix);
		int min = ClientToPos(c->GetMinSize());
		if (min < tsz) {
			int std = ClientToPos(c->GetStdSize());
			tsz = (std < tsz) ? std : min;
		}
	}
	
	animix = ix;
	animtick = 0;
	animinc = tsz / animmaxticks;
	SmartRepos(ix, tsz % animmaxticks);

	SetTimeCallback(-animinterval, THISBACK(AnimateTick), TIMEID_ANIMATE);
}

void DockPane::AnimateTick()
{
	SmartRepos(animix, animinc);
	animtick++;
	if (animtick == animmaxticks)
		EndAnimate();
	Layout();
}

void DockPane::EndAnimate()
{
	animix = -1;
	animtick = 0;
	KillTimeCallback(TIMEID_ANIMATE);
	if (dummy.GetParent())
		Undock(dummy, false);
	else {
		FixChildSizes(); 
		Layout();
	}
}

void DockPane::SmartRepos(int ix, int inc)
{
	int cnt = pos.GetCount();
	if (cnt == 1) {
		pos[0] = 10000;
		return;
	}

	for (int i = cnt-1; i > 0; i--)
		pos[i] -= pos[i-1];	
	
	int n = 0;
	int msz = 0;
	int tsz = 0;
	Vector<int>minpos;
	minpos.SetCount(cnt);
	
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext()) {
		if (n != ix) {
			minpos[n] = min(ClientToPos(c->GetMinSize()), pos[n]);
			msz += minpos[n];
			tsz += pos[n];
		}
		n++;
	}
	int dif = tsz - inc - msz;
	tsz -= msz;
	
	pos[ix] += inc;
	if (tsz != 0 && dif != 0) {
		if (tsz <= 0) dif = -dif;
		int isz = pos[ix];
		for (int i = 0; i < cnt; i++) {
			if (i != ix) 
				pos[i] = minpos[i] + (dif * (pos[i] - minpos[i])) / tsz;
		}
	}	
	for (int i = 1; i < cnt; i++)
		pos[i] += pos[i-1];	
}

int DockPane::GetMinPos(int notix)
{
	int n = 0;
	int msz = 0;
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext()) {
		if (n != notix) msz += ClientToPos(c->GetMinSize());
		n++;
	}
	return msz;	
}

/*
void DockPane::SmartRepos(int ix, int inc)
{
	int rtot = 0;
	int cnt = pos.GetCount();
	if (cnt == 1) {
		pos[0] = 10000;
		return;
	}

	for (int i = cnt-1; i > 0; i--)
		pos[i] -= pos[i-1];	

	int dif = 10000 - inc;
	
	if (dif < 0) {
		// Not enough space
		int rem = dif % cnt;
		dif /= cnt;
		for (int i = 0; i < cnt; i++)
			pos[i] += dif;
		pos[ix] += rem;
	}
	else {
		pos[ix] += inc;
		if (dif > 0)
			for (int i = 0; i < cnt; i++) {
				if (i != ix) 
					pos[i] = (dif * pos[i]) / 10000;
			}
	}	
	for (int i = 1; i < cnt; i++)
		pos[i] += pos[i-1];	
}

void DockPane::SmartRepos(int ix, int inc)
{
	Vector<int> ratio;
	int n = 0;
	int cnt = pos.GetCount();
	int psz = ClientToPos(GetSize());;
	int msz = 0;
	int rtot = 0;
	if (cnt == 1) {
		pos[0] = 10000;
		return;
	}
              
	ratio.SetCount(cnt);
	for (int i = cnt-1; i > 0; i--)
		pos[i] -= pos[i-1];
	
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext()) {
		if (n != ix) {
			ratio[n] = ClientToPos(c->GetMinSize());
			msz += ratio[n];
			ratio[n] = max(pos[n] - ratio[n], 0);
			rtot += ratio[n];
		}
		n++;
	}
	ratio[ix] = 0;

	int dif = psz - msz;
	if (dif < 0) {
		// Not enough space
		int rem = dif % cnt;
		dif /= cnt;
		for (int i = 0; i < cnt; i++)
			pos[i] += dif;
		pos[ix] += rem;
	}
	else if (dif > 0) {;
		int d = (inc > dif) ? dif : inc;
		pos[ix] += d;
		dif -= d;
		
		int p = pos[0];
		int r = ratio[0];
		
		if (dif > 0)
			for (int i = 0; i < cnt; i++) {
				ratio[i] = rtot ? ((ratio[i] * 10000) / rtot) : (10000 / cnt);
				pos[i] = (dif * ratio[i]) / 10000;
			}
			
		p = pos[0];
		r = 0;
	}
	
	if (ix > 0) {
		int p1 = pos[0];
		int p2 = pos[1];
		int i = 0;	
	}	
	for (int i = 1; i < cnt; i++)
		pos[i] += pos[i-1];	
}
*/
void DockPane::FixChildSizes()
{
	int cnt = pos.GetCount();
	if (!cnt) return;
	if (cnt == 1) {
		pos[0] = 10000;
		return;
	}
	int sum = pos[0];
	
	for (int i = 1; i < cnt; i++)
		sum += pos[i] - pos[i-1];	
	sum -= 10000;
	int rem = sum % cnt;
	sum /= cnt;
	for (int i = 1; i < cnt; i++)
		pos[i] -= sum;
	pos[cnt-1] += rem;
}

int DockPane::FindIndex(Ctrl &child)
{
	int ix = 0;
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext()) {
		if (c == &child) return ix;
		ix++;
	}
	return -1;
}

Ctrl * DockPane::FindCtrl(int ix)
{
	Ctrl *c = GetFirstChild();
	for (int i = 0; i < ix && c; i++)
		c = c->GetNext();
	return c;
}

void DockPane::Swap(Ctrl &child, Ctrl &newctrl)
{
	newctrl.SetRect(child.GetRect());
	Ctrl::AddChildBefore(&newctrl, &child);
	Ctrl::RemoveChild(&child);
}

void DockPane::Dock(Ctrl &newctrl, Size sz, int ps, bool animate)
{
	int cnt = pos.GetCount();
	if (!cnt) animate = false;
	int tsz = cnt ? ClientToPos(sz) : 10000;
	ps = min(ps, cnt-1);
	if (ps >= 0) {
		Ctrl *c = GetFirstChild();
		for (int i = 0; i < ps; i++)
			c = c->GetNext();
		Ctrl::AddChildBefore(&newctrl, c);
		pos.Insert(ps);
		pos[ps] = (ps > 0) ? pos[ps-1] : 0;
	}
	else {
		Splitter::Add(newctrl);
		Splitter::pos.Add(0);
		ps = cnt;
		if (ps > 0) pos[ps] = pos[ps-1];
	}
	if (animate)
		StartAnimate(ps, sz);
	else {
		if (cnt)
			SmartRepos(ps, tsz);
		else
			pos[ps] = 10000;
		Layout();
	}
}

void DockPane::Undock(Ctrl &child, bool animate)
{
	if (IsAnimating())
		EndAnimate();	
	
	int ix = FindIndex(child);
	if (animate) {
		dummy.Remove();
		Swap(child, dummy);
		StartAnimate(ix, Size(0, 0));
	}
	else {
		child.Remove();
		pos.Remove(ix);	
		FixChildSizes();
		Layout();	
	}
}

void DockPane::QuickDock(Ctrl &ctrl, int sz)
{
	Add(ctrl);
	int cnt = pos.GetCount();
	pos.Add(cnt ? pos[cnt-1] + sz : sz);
}

DockPane::DockPane()
{
	animix = -1;
	animtick = 0;
	animinc = 0;
	animinterval = 20; // milliseconds
	animmaxticks = 10;	
	pos.Clear();
}

