#include "DockCtrl/DockCtrl.h"


PaneSplitter::PaneSplitter()
{
	animating = false;
	x = 0;
	y = 0;
}

PaneSplitter::~PaneSplitter()
{
	animating = false;
}

void PaneSplitter::AddChildDock(DockableCtrl& dock)
{
	Splitter::Add((Ctrl &)dock);
	ReposChilds();
}

void PaneSplitter::AddChildDock(DockableCtrl& dock, int position)
{
	int count = GetCount();
	if(!HasChild() || position > count) AddChildDock(dock);
	else AddChildBefore((Ctrl*) &dock, (Ctrl *) GetChildAt(position)); 	
	ReposChilds();	
}

void PaneSplitter::RemoveChildDock(DockableCtrl& dock)
{
	RemoveChildDock(dock.Position());
}

void PaneSplitter::RemoveChildDock(int position)
{
	DockableCtrl* ctrl = GetChildAt(position);
	if(ctrl) ctrl->Remove();
	ReposChilds();
}

void PaneSplitter::RemoveChilds()
{
	if(!HasChild()) return;
	for(int i = 1; i < GetCount(); i++)
		GetChildAt(i)->Shut();
}

void PaneSplitter::ReposChilds()
{
	if(!HasChild()) return;
	int i = 1;
	DockableCtrl* ctrl = (DockableCtrl*) GetFirstChild();
	ctrl->Posit(i);
	while(ctrl = (DockableCtrl*) ctrl->GetNext())
		ctrl->Posit(++i);
}

DockableCtrl* PaneSplitter::GetChildAt(int position)
{
	if(HasChild())
	{
		Ctrl* ctrl = GetFirstChild();
		while(ctrl)
		{
			if(ctrl != &animationctrl)
				if(((DockableCtrl*)ctrl)->Position() == position) 
					return (DockableCtrl*) ctrl;
			ctrl = ctrl->GetNext();
		}
	}
	return NULL;
}

int PaneSplitter::GetChildCount()
{
	int childcount = GetCount();
	return IsAnimating() ?  childcount - 1 : childcount;
}

bool PaneSplitter::HasChild() const
{
	int childcount = GetCount();
	return ((GetCount() > 0) ? (childcount == 1 && GetFirstChild() == &animationctrl) ? false : true : false); 
}

void PaneSplitter::StartAnimation(int position)
{
	if(animationctrl.IsOpen()) return;
	if(!HasChild() || position > GetCount()) 
		Add(animationctrl.SizePos());
	else AddChildBefore(&animationctrl.SizePos(), (Ctrl *) GetChildAt(position));
	animating = true;
	Animate(position);
}

void PaneSplitter::StartAnimation(DockableCtrl& dock, int position)
{
	StartAnimation(position);
}

void PaneSplitter::StopAnimation()
{
   if(!animationctrl.IsOpen()) return;
   animationctrl.Remove();
   animating = false; 
}

void PaneSplitter::Animate(int position)
{
}

void PaneSplitter::AnimateCallback()
{
}

void PaneSplitter::Layout()
{
	Splitter::Layout();
}

//----------------------------------------------------------------------------------------------

void PaneSplitter::AnimationCtrl::AnimateHorz()
{
	if(!IsVisible()) Show();
	Ctrl::LeftPos(0, 0).Layout();
}
void PaneSplitter::AnimationCtrl::AnimateVert()
{
	if(!IsVisible()) Show();
	Ctrl::TopPos(0, 0).Layout();
}
