#include "DockPane.h"

int DockPane::ClientToPos(const Size &p)
{
	int w = (width>>1)*pos.GetCount()-1;
	return minmax(vert ? 10000 * p.cy / (GetSize().cy - w) : 10000 * p.cx / (GetSize().cx - w), 0, 9999);	
}

void DockPane::StartAnimate(int ix, Size sz, bool restore)
{
	if (restore) {
		ASSERT(savedpos.GetCount() == pos.GetCount());
		animpos <<= savedpos;
	}
	else {
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
		animpos <<= pos;
		SmartRepos(animpos, ix, tsz);
	}
	animtick = 0;	
	SetTimeCallback(-animinterval, THISBACK(AnimateTick), TIMEID_ANIMATE);
	AnimateTick();
}

void DockPane::AnimateTick()
{
	for (int i = 0; i < animpos.GetCount(); i++)
		pos[i] += ((animpos[i] - pos[i]) * animtick) / animmaxticks;
	animtick++;
	if (animtick == animmaxticks)
		EndAnimate();
	else
		Layout();
}

void DockPane::EndAnimate()
{
	animtick = 0;
	pos <<= animpos;
	animpos.Clear();
	KillTimeCallback(TIMEID_ANIMATE);
	if (dummy.GetParent()) {
		pos.Remove(FindIndex(dummy));	
		dummy.Remove();
	}
	FixChildSizes(); 
	Layout();
}

void DockPane::SmartRepos(Vector<int> &p, int ix, int inc)
{
	int cnt = p.GetCount();
	if (cnt == 1) {
		p[0] = 10000;
		return;
	}

	for (int i = cnt-1; i > 0; i--)
		p[i] -= p[i-1];	
	
	int n = 0;
//	int msz = 0;
	int tsz = 0;
//	Vector<int> minpos;
//	minpos.SetCount(cnt);
	
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext()) {
		if (n != ix) {
//			minpos[n] = min(ClientToPos(c->GetMinSize()), p[n]);
//			msz += minpos[n];
			tsz += p[n];
		}
		n++;
	}
	int dif = tsz - inc;
	int sum = 0;
	p[ix] += inc;
	for (int i = 0; i < cnt; i++) {
		int t = p[i];
		if (i != ix)
			p[i] = (p[i]*dif) / tsz ;
		sum += p[i];
		t = p[i];
		t++;
	}	
	dif = sum - 10000;
	if (dif)
		p[ix] += dif;
	for (int i = 1; i < cnt; i++)
		p[i] += p[i-1];	
}

void DockPane::SimpleRepos(Vector<int> &p, int ix, int inc)
{
	int cnt = p.GetCount();
	if (cnt == 1) {
		p[0] = 10000;
		return;
	}

	for (int i = cnt-1; i > 0; i--)
		p[i] -= p[i-1];	
	
	int n = 0;
	int tsz = 0;
	
	for (Ctrl *c = GetFirstChild(); c; c = c->GetNext()) {
		if (n != ix)
			tsz += p[n];
		n++;
	}
	int dif = tsz - inc;
	int sum = 0;
	p[ix] += inc;
	for (int i = 0; i < cnt; i++) {
		int t = p[i];
		if (i != ix)
			p[i] = (p[i]*dif) / tsz ;
		sum += p[i];
		t = p[i];
		t++;
	}	
	dif = sum - 10000;
	if (dif)
		p[ix] += dif;
	for (int i = 1; i < cnt; i++)
		p[i] += p[i-1];	
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
	if (sum)
		for (int i = 0; i < cnt; i++)
			pos[i] -= sum*(i+1);
	pos[cnt-1] -= rem;
	int p = pos[cnt-1];
	ASSERT(pos[cnt-1] == 10000);
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

void DockPane::Dock(Ctrl &newctrl, Size sz, int ps, bool animate, bool save)
{
	if (IsAnimating())
		EndAnimate();
		
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
	if (save) 
		SavePos();
	if (animate)
		StartAnimate(ps, sz, false);
	else {
		if (cnt)
			SmartRepos(pos, ps, tsz);
		else
			pos[ps] = 10000;
		Layout();
	}
}

void DockPane::Undock(Ctrl &child, bool animate, bool restore)
{
	if (IsAnimating())
		EndAnimate();	
	
	int ix = FindIndex(child);
	if (animate && GetFirstChild() != GetLastChild()) {
		dummy.Remove();
		Swap(child, dummy);
		StartAnimate(ix, Size(0, 0), restore);
	}
	else {
		if (restore) 
			RestorePos();
		child.Remove();
		pos.Remove(ix);	
		FixChildSizes();
		Layout();	
	}
}

DockPane::DockPane()
{
	animtick = 0;
	animinterval = 20; // milliseconds
	animmaxticks = 10;	
	pos.Clear();
}

