//==============================================================================================
// DockCtrl: A dockable widget framework for U++
// Author:	 Ismail YILMAZ
// Version:	 0.50 (DEV-0802b.1)
//==============================================================================================

#include "DockCtrl/DockCtrl.h"

#define IMAGEFILE <DockCtrl/DockableCtrl.iml>
#define IMAGECLASS DockCtrlImages
#include <Draw/iml.h>

//===============================================
// DockCtrl class:
// A dockable widget framework base ctrl.
//===============================================

Ctrl& DockCtrl::Dock(DockableCtrl& ctrl)
{
	ASSERT(base);
    int alignment = ctrl.Alignment();
    ctrl.SetBase(this);
    AddCtrlRecord(ctrl);
 
    if(!ctrl.IsFloating())
    {
        PaneFrame& paneframe = GetPaneFrame(alignment);
        switch(alignment)
        {
            case DockableCtrl::DOCK_LEFT:
                    paneframe.Attach(ctrl).Vert();
                break;  
            case DockableCtrl::DOCK_TOP:
                    paneframe.Attach(ctrl).Horz();
                break;
            case DockableCtrl::DOCK_RIGHT:
                    paneframe.Attach(ctrl).Vert();
                break;
            case DockableCtrl::DOCK_BOTTOM:
                    paneframe.Attach(ctrl).Horz();
                break;
            case DockableCtrl::DOCK_TABBED:
                break;
            default:
                break;
        }
        
    }
    RefreshPanel();
    return *this;
}

void DockCtrl::DoDragAndDrop(DockableCtrl& ctrl, Point p, Size& sz)
{
    for(int i = 0; i < 4; i++)
    {   
        
        for(int j = 0; j < 4; j++)
        if( i != j)
        { 
            if(!GetPaneFrame(j).HasCtrlInRange(ctrl, p)) 
            {   
                GetPaneFrame(j).DnDSourceoutofRange(); 
                GetPaneFrame(j).DnDAnimate();
            }
        }
        if(GetPaneFrame(i).HasCtrlInRange(ctrl, p)) 
        {
            ctrl.SetDropTarget(i, ctrl.GetDropState());
            break;
        }
    }
}

void DockCtrl::Attach(DockableCtrl& ctrl)
{
    GetPaneFrame(ctrl.Alignment()).Attach(ctrl);
}

void DockCtrl::Attach(DockableCtrl* ctrl)
{
    Attach(*ctrl);
}

void DockCtrl::Detach(DockableCtrl& ctrl)
{
    GetPaneFrame(ctrl.Alignment()).Detach(ctrl);
}

void DockCtrl::Detach(DockableCtrl* ctrl)
{
    Detach(*ctrl);
}

void DockCtrl::ShowPaneFrame(int alignment)
{
}

void DockCtrl::HidePaneFrame(int alignment)
{
}

void DockCtrl::LockPaneFrame(int alignment)
{
}

void DockCtrl::ControlPanel()
{
    if(controlpanel.IsOpen()) return;
    controlpanel.Open();
    RefreshPanel(); 
}

void DockCtrl::AddCtrlRecord(DockableCtrl& ctrl)
{
    if(ctrl.GetType() == DockableCtrl::TYPE_TABDOCK)
        return;
    for(int i = 0; i < ctrlrecords.GetCount(); i++)
        if(ctrlrecords.At(i)->ctrl == &ctrl) return;
    ctrlrecords.Add(new CtrlRecord(ctrl));

    ImageCtrl* imagectrl = new ImageCtrl();
    panelicons.Add(imagectrl);
}

void DockCtrl::DeleteCtrlRecord(DockableCtrl& ctrl)
{
    DockableCtrl* ctrltodelete = NULL;
    for(int i = 0; i < ctrlrecords.GetCount(); i++)
    {
        ctrltodelete = ctrlrecords.At(i)->ctrl;
        if(ctrltodelete == &ctrl)
        {
            ctrlrecords.Remove(i);
            delete ctrltodelete;
        }
    }
}

void DockCtrl::DeleteCtrlRecords()
{
    int i = ctrlrecords.GetCount(); 
    if(i !=0) 
        for(int j = 0; j < i; j++) 
            delete ctrlrecords.At(j);
}

DockCtrl::CtrlRecord* DockCtrl::GetCtrlRecord(DockableCtrl* ctrl)
{
    CtrlRecord* ctrlrecord = NULL;
    for(int i = 0; i < ctrlrecords.GetCount(); i++)
        if((ctrlrecord = ctrlrecords.At(i)) == ctrlrecord) return ctrlrecord;   
    return NULL;    
}

DockCtrl::CtrlRecord* DockCtrl::GetCtrlRecord(int index)
{
	return (index < ctrlrecords.GetCount() &&
		   index >= 0) ? ctrlrecords.At(index) : NULL;
}

void DockCtrl::PanelAction()
{
    CtrlRecord* ctrlrecord = GetCtrlRecord(controlpanel.list.GetClickRow());
    if(ctrlrecord) 
    	ctrlrecord->ctrl->Menu();
}

void DockCtrl::RefreshPanel()
{
    if(!controlpanel.IsOpen()) return;
    controlpanel.list.Clear();
    for(int i = 0; i < ctrlrecords.GetCount(); i++)
    {
        CtrlRecord* ctrlrecord = ctrlrecords.At(i);
        if(ctrlrecord->ctrl) 
        {
            DockableCtrl* ctrl = ctrlrecord->ctrl;
            String a, s;
            switch(ctrl->Alignment())
            {
                case DockableCtrl::DOCK_LEFT:
                    a = t_("LEFT");
                    break;
                case DockableCtrl::DOCK_TOP:
                    a = t_("TOP");
                    break;
                case DockableCtrl::DOCK_RIGHT:
                    a = t_("RIGHT");
                    break;
                case DockableCtrl::DOCK_BOTTOM:
                    a = t_("BOTTOM");
                    break;
                case DockableCtrl::DOCK_NONE:
                    if(ctrl->IsFloating())
                        a = t_("FLOATING");
                    else
                        a = t_("NONE");
                    break;     
            }
            switch(ctrl->State())
            {
                case DockableCtrl::STATE_SHOW:
                    s = t_("SHOWN");
                    break;
                case DockableCtrl::STATE_HIDE:
                    s = t_("HIDDEN");
                    break;
                case DockableCtrl::STATE_AUTO:
                    s = t_("AUTO HIDDEN");
                    break;
                case DockableCtrl::STATE_SHUT:
                    s = t_("SHUT");
                    break;
                case DockableCtrl::STATE_TABBED:
                    s = t_("TABBED");
                    break;
            }
            controlpanel.list.Add(NULL, Format("%s",ctrl->GetLabel()), a, s, ctrl->Position());
            controlpanel.list.SetCtrl(i, 0, 
                    (panelicons.At(i))->SetImage(ctrl->GetIcon().IsNullInstance() ? 
                    DockCtrlImages::DefaultImage : ctrl->GetIcon()));
        }
    }
   controlpanel.Refresh();
}

DockCtrl& DockCtrl::SetStyle(const DockCtrlChStyle::Style& s)
{
    style = &s; 
    Refresh(); 
    return *this;   
}


void DockCtrl::SelectSkin()
{
    switch(controlpanel.skinlist.GetIndex())
    { 
        case 0: 
            style = &DockCtrlChStyle::StyleClassic();
            break;
        case 1: 
            style = &DockCtrlChStyle::StyleEnhanced(); 
            break;
        case 2: 
            style = &DockCtrlChStyle::StyleDefault(); 
            break;
    }
        
    for(int i = 0; i < ctrlrecords.GetCount(); i++)
    {
        DockWindow* ctrl = dynamic_cast<DockWindow*> (GetCtrlRecord(i)->ctrl);
        if(ctrl)
            ctrl->SetStyle(*style);
    }
   for(int i = 0; i < 4; i++)
   {
        PaneFrame& frame = GetPaneFrame(i);
        for(int j = 0; j < frame.GetTabWindows().GetCount(); j++)
           frame.GetTabWindows().At(j)->SetStyle(*style);
   }    
}

DockCtrl& DockCtrl::SetLayout(Ctrl& parent, int cx, int cy)
{
	base = &parent;
	ASSERT(base);
	
	panesize = Size(cx, cy);
	
	parent.AddFrame(hide[0]);
	parent.AddFrame(hide[1]);
	parent.AddFrame(hide[2]);
	parent.AddFrame(hide[3]);

	hide[0].SetLayout(CustomFrame::LAYOUT_LEFT);
	hide[2].SetLayout(CustomFrame::LAYOUT_RIGHT);
	hide[1].SetLayout(CustomFrame::LAYOUT_TOP);
	hide[3].SetLayout(CustomFrame::LAYOUT_BOTTOM);	

	parent.AddFrame(pane[0]);
	parent.AddFrame(pane[2]);
	parent.AddFrame(pane[1]);
	parent.AddFrame(pane[3]);
	
	pane[0].SetLayout(this, PaneFrame::LEFT, cx);
	pane[2].SetLayout(this, PaneFrame::RIGHT, cx);
	pane[1].SetLayout(this, PaneFrame::TOP, cy);
	pane[3].SetLayout(this, PaneFrame::BOTTOM, cy);
			
return *this;
}

DockCtrl::DockCtrl()
{
    panesize.Clear();
    ctrlkey 	= 0;
    base    	= NULL;
    LeftPos(0, 0).TopPos(0,0);
    CtrlLayout(controlpanel);
    controlpanel.CenterScreen().Sizeable().Zoomable().Title(t_("Settings"));
    controlpanel.list.AddColumn();
    controlpanel.list.AddColumn(t_("Window Title"));
    controlpanel.list.AddColumn(t_("Alignment"));
    controlpanel.list.AddColumn(t_("State"));
    controlpanel.list.AddColumn(t_("Position"));
    controlpanel.list.EvenRowColor();
    controlpanel.list.SetLineCy(StdFont().GetHeight() + 2);
    controlpanel.skinlist.Add(0, t_("Classic")).Add(1, t_("Enhanced")).Add(2, t_("Default")).SetIndex(2);
    controlpanel.skinlist.WhenAction = THISBACK(SelectSkin);
    controlpanel.list.WhenLeftClick = THISBACK(PanelAction); 
    controlpanel.layoutbutton		<<= THISBACK(ResetWidgetLayout);
    controlpanel.TabOptionAlignment <<= THISBACK(RefreshWidgetLayout);
    controlpanel.TabOptionIcon		<<= THISBACK(RefreshWidgetLayout);
    controlpanel.TabOptionClose		<<= THISBACK(RefreshWidgetLayout);
    controlpanel.AutoOptionClose	<<= THISBACK(RefreshWidgetLayout);
    controlpanel.AutoOptionIcon		<<= THISBACK(RefreshWidgetLayout);

	controlpanel.TabOptionIcon 		<<= true;
	controlpanel.TabOptionClose 	<<= true;
	controlpanel.TabOptionAlignment <<= false;
	controlpanel.TabOptionNest		<<= false;
	
	controlpanel.AutoOptionIcon 	<<= true;
	controlpanel.AutoOptionClose 	<<= true;
}

DockCtrl::~DockCtrl()
{
    for(int i = 0; i < panelicons.GetCount(); i++)
        delete panelicons.At(i);

    DeleteCtrlRecords();
    panesize.Clear();
    ctrlkey 	= 0;
    base    	= NULL;
}

void DockCtrl::RefreshWidgetLayout()
{
	TabWindow* tabwindow = NULL;
	bool ti = (bool) controlpanel.TabOptionIcon.Get();
	bool tb = (bool) controlpanel.TabOptionClose.Get();
	bool tl = (bool) controlpanel.TabOptionAlignment.Get();
	bool ai = (bool) controlpanel.AutoOptionIcon.Get();
	bool ab = (bool) controlpanel.AutoOptionClose.Get();
	
	for(int i = 0; i < 4; i++)
	{
       	PaneFrame& frame = GetPaneFrame(i);
       	AutoHideBar& bar = GetHideBar(i);

  		bar.HasButtons(ab);
   		bar.HasIcons(ai);
       	
     	for(int j = 0; j < frame.GetTabWindows().GetCount(); j++)
        {
        	tabwindow = frame.GetTabWindows().At(j);
			if(tl)
			{
				tabwindow->RemoveFrame(tabwindow->GetTabs());
  		      	switch(tabwindow->Alignment())
        		{
        			case DockableCtrl::DOCK_LEFT:
						tabwindow->SetLayout(CustomFrame::LAYOUT_RIGHT);  
   	       				break;
   	  	     		case DockableCtrl::DOCK_TOP:
						tabwindow->SetLayout(CustomFrame::LAYOUT_BOTTOM);   
  	       				break;
   	    	   		case DockableCtrl::DOCK_RIGHT:
						tabwindow->SetLayout(CustomFrame::LAYOUT_LEFT);  
   	      	 			break;
   	    	   		case DockableCtrl::DOCK_BOTTOM:
						tabwindow->SetLayout(CustomFrame::LAYOUT_TOP);  
   	       				break;   
   	       		}
			}
			else 
				tabwindow->SetLayout(CustomFrame::LAYOUT_BOTTOM);
			
	     	   	tabwindow->GetTabs().HasIcons(ti);
     	   		tabwindow->GetTabs().HasButtons(tb);   						
   		}
	}	
}


void DockCtrl::ResetWidgetLayout()
{
	CtrlRecord *ctrlrecord 	= NULL;
	DockableCtrl* ctrl		= NULL;
	int ctrlcount 			= ctrlrecords.GetCount();
	
	for(int i = 0; i < ctrlcount * 2; i++)
		if(i < ctrlcount) 
		{	
			ctrlrecord = GetCtrlRecord(i);
			if(ctrlrecord && !ctrlrecord->ctrl->IsShut())
				ctrlrecord->ctrl->Shut();
				
		}
		else
		{
			ctrlrecord = GetCtrlRecord(i - ctrlcount);
			if(ctrlrecord)
				Dock(ctrlrecord->ctrl->Style(
						ctrlrecord->alignment,	
						ctrlrecord->state, 
						ctrlrecord->position));
		}
	RefreshPanel();
}

CH_STYLE(DockCtrlChStyle, Style, StyleClassic)
{
    font        = StdFont();
    barmargins  = Rect(2,2,2,2);
    barheight   = font.Info().GetHeight() + 8;  
    tabheight   = font.Info().GetHeight() + 8;
    tabmargin   = 2;
    tabsel      = Rect(2, 2, 2, 2);
    tabedge     = Rect(6, 6, 6, 6);
    tabextendleft = 0;
    
    DockCtrlImgsLook(barbackground, DockCtrlImages::I_CBarBackground, 1);
    DockCtrlImgsLook(barshut, DockCtrlImages::I_CClose, 3);
    DockCtrlImgsLook(barhide, DockCtrlImages::I_CHide, 3);
    DockCtrlImgsLook(barmenu, DockCtrlImages::I_CMenu, 3);

 
    CtrlsImageLook(tabnormal, ClassicCtrlsImg::I_TAB, 4);
    CtrlsImageLook(tabfirst, ClassicCtrlsImg::I_FTAB, 4);
    CtrlsImageLook(tablast, ClassicCtrlsImg::I_LTAB, 4);
    CtrlsImageLook(tabboth, ClassicCtrlsImg::I_BTAB, 4);        
}

CH_STYLE(DockCtrlChStyle, Style, StyleEnhanced)
{
    font        = StdFont();
    barmargins  = Rect(2,2,2,2);
    barheight   = font.Info().GetHeight() + 8;  
    tabheight   = font.Info().GetHeight() + 8;
    tabmargin   = 2;
    tabsel      = Rect(2, 2, 2, 2);
    tabedge     = Rect(6, 6, 6, 6);
    tabextendleft = 0;

    DockCtrlImgsLook(barbackground, DockCtrlImages::I_EBarBackground, 1);
    DockCtrlImgsLook(barshut, DockCtrlImages::I_CClose, 3);
    DockCtrlImgsLook(barhide, DockCtrlImages::I_CHide, 3);
    DockCtrlImgsLook(barmenu, DockCtrlImages::I_CMenu, 3);

    CtrlsImageLook(tabnormal, CtrlsImg::I_TAB, 4);
    CtrlsImageLook(tabfirst, CtrlsImg::I_FTAB, 4);
    CtrlsImageLook(tablast, CtrlsImg::I_LTAB, 4);
    CtrlsImageLook(tabboth, CtrlsImg::I_BTAB, 4);       
}

CH_STYLE(DockCtrlChStyle, Style, StyleDefault)
{
    font        = StdFont();
    barmargins  = Rect(2,2,2,2);
    barheight   = font.Info().GetHeight() + 8;  
    tabheight   = font.Info().GetHeight() + 8;
    tabmargin   = 2;
    tabsel      = Rect(2, 2, 2, 2);
    tabedge     = Rect(6, 6, 6, 6);
    tabextendleft = 0;
    
    DockCtrlImgsLook(barbackground, DockCtrlImages::I_DBarBackground, 1);
    DockCtrlImgsLook(barshut, DockCtrlImages::I_DClose, 3);
    DockCtrlImgsLook(barhide, DockCtrlImages::I_DHide, 3);
    DockCtrlImgsLook(barmenu, DockCtrlImages::I_DMenu, 3);
  
    CtrlsImageLook(tabnormal, CtrlsImg::I_TAB, 4);
    CtrlsImageLook(tabfirst, CtrlsImg::I_FTAB, 4);
    CtrlsImageLook(tablast, CtrlsImg::I_LTAB, 4);
    CtrlsImageLook(tabboth, CtrlsImg::I_BTAB, 4);       
}

