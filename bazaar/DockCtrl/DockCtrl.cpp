//==============================================================================================
// DockCtrl: A dockable widget framework for U++
// Author:	 Ismail YILMAZ (A.k.a. Maldoror)
// Version:	 0.49.9.9a
//==============================================================================================

#include "DockCtrl/DockCtrl.h"

#define IMAGEFILE <DockCtrl/DockableCtrl.iml>
#define IMAGECLASS DockCtrlImages
#include <Draw/iml.h>

//===============================================
// DockCtrl class:
// A dockable widget framework base ctrl.
//===============================================

DockCtrl::DockCtrl()
{
	_size = 0;
	_ctrlkey = 0;
	::dockctrl_ptr = this;
	
	
	LeftPos(0, 0).TopPos(0,0);
	
	InstallKeyHook(&DockCtrl::DockCtrlKeyHook);
	
	CtrlLayout(controlpanel);
	controlpanel.CenterScreen().Sizeable().Zoomable().Title(t_("Control Panel."));
	controlpanel.list.WhenLeftDouble = THISBACK(OnPanelAction);
	controlpanel.list.AddColumn(t_("Below are the installed dockable windows:"));
	controlpanel.skinlist.Add(0, t_("Classic")).Add(1, t_("Enhanced")).Add(2, t_("Modern")).SetIndex(2);
	controlpanel.skinlist.WhenAction = THISBACK(OnVisualStyleList);
	controlpanel.layoutbutton <<= THISBACK(ResetLayout);
}

DockCtrl::~DockCtrl()
{
	ClearRecords();
	DeinstallKeyHook(&DockCtrl::DockCtrlKeyHook);
	::dockctrl_ptr = NULL;
}

Ctrl& DockCtrl::Dock(DockableCtrl& ctrl)
{
	int alignment = ctrl.Alignment();

	ctrl.SetBase(this);	
	
	if(ctrl.IsDocked())
	{
		DockPane& pane = GetPane(alignment);
		
		if(!pane.IsCreated())  _basectrl->AddFrame(pane.CreatePane(alignment, GetPaneSize()));

		switch(alignment)
		{
			case DockableCtrl::dock_top:
					pane.DockChild(ctrl, GetPaneSize()).Horz();
				break;
				
			case DockableCtrl::dock_bottom:
					pane.DockChild(ctrl, GetPaneSize()).Horz();
				break;
				
			case DockableCtrl::dock_left:
					pane.DockChild(ctrl, GetPaneSize()).Vert();
				break;

			case DockableCtrl::dock_right:
					pane.DockChild(ctrl, GetPaneSize()).Vert();
				break;
			
		}
	}
	AddRecord(&ctrl);
	if(controlpanel.list.Find(ctrl.GetLabel()) == -1) controlpanel.list.Add(Format("%s", ctrl.GetLabel()));
	
	return *this;
}

void DockCtrl::ResetLayout()
{
   int i = 0; 
   if((i = _entrylist.GetCount()) != 0) 
   	for(int j = 0; j < i; j++)
   	{
   		CtrlRecord* rec = FindRecord(j);
   		if(rec->ctrl->IsShut()) 
   			AddDock(rec->ctrl->Style(rec->alignment, rec->state, rec->position));
   		else 
   			rec->ctrl->Dock(rec->alignment, rec->state, rec->position);
   	}
}

void DockCtrl::OpenControlPanel()
{
	if(controlpanel.IsOpen())
	{
		controlpanel.Close(); controlpanel.Remove();
	} controlpanel.Open();
}

void DockCtrl::OnPanelAction()
{
	DockableCtrl* dock = FindRecord(controlpanel.list.GetClickRow())->ctrl;
	if(dock->IsHidden()) dock->Show();
		else
	if(dock->IsShut()) dock->Float();	
		else
	PromptOK(Format(t_("Dock '%s' is already open"), dock->GetLabel()));
	controlpanel.Remove();
}

void DockCtrl::OnVisualStyleList()
{
	const DockWindow::Style* st;
	
	switch(controlpanel.skinlist.GetIndex())
	{ 
		case 0: st = &DockWindow::ClassicStyle(); break;
	    case 1: st = &DockWindow::EnhancedStyle(); break;
	    case 2: st = &DockWindow::ModernStyle(); break;
	}
	       
	for(int i = 0; i < _entrylist.GetCount(); i++)
	{
		DockWindow* dock = dynamic_cast<DockWindow*> (FindRecord(i)->ctrl);
		if(dock) dock->SetVisualStyle(*st);
	}
	    
}

void DockCtrl::AddRecord(DockableCtrl* ctrl)
{
	if(FindRecord(ctrl) == NULL) _entrylist.Add(new CtrlRecord(ctrl)); 
}

void DockCtrl::RemoveRecord(DockableCtrl* ctrl)
{
}

DockCtrl::CtrlRecord* DockCtrl::FindRecord(DockableCtrl* ctrl)
{
	CtrlRecord* rec = NULL;
	for(int i = 0; i < _entrylist.GetCount(); i++)
		if((rec = _entrylist.At(i))->ctrl == ctrl) return rec;	
	return NULL;	
}

void DockCtrl::DoDragAndDrop(DockableCtrl& ctrl, Point pos)
{
                          
	for(int i = 0; i < 4; i++)
	{	
		
		for(int j = 0; j < 4; j++)
		if( i != j)
		{ 
			if(!GetPane(j).DnDCalcSourcePos(pos)) 
			{	
				GetPane(j).DnDSourceoutofRange(); 
				GetPane(j).DnDAnimate();
			}
		}

		Rect droparea = GetPane(i).GetScreenView();
    	if(ctrl.DnDHasTarget()) ctrl.DnDTargetoutofRange();
		if(GetPane(i).DnDCalcSourcePos(pos)) 
		{
			ctrl.DnDTargetinRange(i);
			break;
		}
	}
}
