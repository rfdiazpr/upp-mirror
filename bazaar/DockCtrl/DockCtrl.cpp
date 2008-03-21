#include "DockCtrl/DockCtrl.h"

#define IMAGEFILE <DockCtrl/DockableCtrl.iml>
#define IMAGECLASS DockCtrlImages
#include <Draw/iml.h>

//===============================================
// DockCtrl class:
// Actual Framework.
//===============================================

DockCtrl::DockCtrl() : DockBase()
{
	AllowAll();
}

DockCtrl::~DockCtrl()
{
}

void DockCtrl::State(int reason)
{
	if(reason == Ctrl::OPEN)
	{
		if(!ctrlinit)
		{
			InitFrameWork();
			InitDockCtrl ();
			InitDefaultLayout();
			InitCustomLayouts();	
			InitFrameWorkSettings();
			ctrlinit = true;
		}
	}
}



//===============================================
// DockBase class:
// A dockable widget framework base ctrl.
//===============================================

Ctrl& DockBase::Dock(DockableCtrl& ctrl)
{
    int alignment = ctrl.Alignment();
    // Register ctrl.
    ctrl.SetBase(this);
    if(!ctrl.IsFloating()) AddCtrlRecord(ctrl);
     switch(alignment)
   	{
       		case DockableCtrl::DOCK_LEFT:
          			GetPaneFrame(alignment).Attach(ctrl).Vert();
  		 		break;  
            case DockableCtrl::DOCK_TOP:
                    GetPaneFrame(alignment).Attach(ctrl).Horz();
                break;
            case DockableCtrl::DOCK_RIGHT:
                    GetPaneFrame(alignment).Attach(ctrl).Vert();
                break;
            case DockableCtrl::DOCK_BOTTOM:
                    GetPaneFrame(alignment).Attach(ctrl).Horz();
                break;
             default:
             	if(ctrl.IsFloating())
             	{
             		ctrl.CenterOwner();
             		ctrl.SetSize(panesize);
             		ctrl.FloatEx(Rect(0, 0, panesize.cx / 4, panesize.cy / 3));
             		ctrl.NoCenter();
             	}
             	AddCtrlRecord(ctrl);
                break;
   	}
	RefreshPanel();
	return *this;
}

DockableCtrl& DockBase::Tabify(DockableCtrl& ctrl1, DockableCtrl& ctrl2)
{
	// Register Ctrls.
	AddCtrlRecord(ctrl1);
	AddCtrlRecord(ctrl2);
	// By default (if not stated otherwise) create tabwindows from Left PaneFrame. 
	TabWindow* tabwindow = GetPaneFrame(0).AddTabWindow();
	ASSERT(tabwindow);
	tabwindow->Attach(ctrl1);
	tabwindow->Attach(ctrl2);
	return (DockableCtrl&) *tabwindow;	
}

DockableCtrl& DockBase::Tabify(DockableCtrl& ctrl1, DockableCtrl& ctrl2, DockableCtrl& ctrl3, DockableCtrl& ctrl4)
{
	return Tabify(Tabify(ctrl1, ctrl2), Tabify(ctrl3, ctrl4));
}

void DockBase::DoDragAndDrop(DockableCtrl& ctrl, Point p, Size sz)
{
 	for(int i = 0; i < 4; i++)
    {   
        for(int j = 0; j < 4; j++)
        {
         		if(i != j)
        		{	 
           	 		if(!GetPaneFrame(j).HasCtrlInRange(ctrl, p)) 
           	 		{   
           		   	 	GetPaneFrame(j).DnDSourceoutofRange(); 
               	 		GetPaneFrame(j).DnDAnimate();
            		}
        		}
        }
       	if(GetPaneFrame(i).HasCtrlInRange(ctrl, p)) 
       	{
      	 	ctrl.SetDropTarget(i, ctrl.GetDropState());
          	break;
       	}
    }
}

void DockBase::Attach(DockableCtrl& ctrl)
{
    GetPaneFrame(ctrl.Alignment()).Attach(ctrl);
}

void DockBase::Attach(DockableCtrl* ctrl)
{
    Attach(*ctrl);
}

void DockBase::Detach(DockableCtrl& ctrl)
{
    GetPaneFrame(ctrl.Alignment()).Detach(ctrl);
}

void DockBase::Detach(DockableCtrl* ctrl)
{
    Detach(*ctrl);
}

void DockBase::ShowLeftPane(bool b)
{
	if(b)	GetPaneFrame(PaneFrame::LEFT).ShowFrame();
	else	GetPaneFrame(PaneFrame::LEFT).HideFrame();
}

void DockBase::ShowTopPane(bool b)
{
	if(b)	GetPaneFrame(PaneFrame::TOP).ShowFrame();
	else	GetPaneFrame(PaneFrame::TOP).HideFrame();
}

void DockBase::ShowRightPane(bool b)
{
	if(b)	GetPaneFrame(PaneFrame::RIGHT).ShowFrame();
	else	GetPaneFrame(PaneFrame::RIGHT).HideFrame();
}

void DockBase::ShowBottomPane(bool b)
{
	if(b)	GetPaneFrame(PaneFrame::BOTTOM).ShowFrame();
	else	GetPaneFrame(PaneFrame::BOTTOM).HideFrame();
}

void DockBase::AllowAll()
{
	AllowedSides[0] = true;
	AllowedSides[1] = true;
	AllowedSides[2] = true;
	AllowedSides[3] = true;
}

void DockBase::AllowLeft()
{
	AllowedSides[0] = true;
	AllowedSides[1] = false;
	AllowedSides[2] = false;
	AllowedSides[3] = false;
}

void DockBase::AllowTop()
{
	AllowedSides[1] = true;
	AllowedSides[0] = false;
	AllowedSides[2] = false;
	AllowedSides[3] = false;
}

void DockBase::AllowRight()
{
	AllowedSides[2] = true;
	AllowedSides[0] = false;
	AllowedSides[1] = false;
	AllowedSides[3] = false;
}

void DockBase::AllowBottom()
{
	AllowedSides[3] = true;
	AllowedSides[0] = false;
	AllowedSides[1] = false;
	AllowedSides[2] = false;
}

void DockBase::AllowLeftRight()
{
	AllowedSides[0] = true;
	AllowedSides[2] = true;
	AllowedSides[1] = false;
	AllowedSides[3] = false;
}

void DockBase::AllowTopBottom()
{
	AllowedSides[1] = true;
	AllowedSides[3] = true;
	AllowedSides[0] = false;
	AllowedSides[2] = false;
}

void DockBase::AllowLeftTop()
{
	AllowedSides[0] = true;
	AllowedSides[1] = true;
	AllowedSides[2] = false;
	AllowedSides[3] = false;
}

void DockBase::AllowRightBottom()
{
	AllowedSides[2] = true;
	AllowedSides[3] = true;
	AllowedSides[0] = false;
	AllowedSides[1] = false;
}

void DockBase::AllowLeftBottom()
{
	AllowedSides[0] = true;
	AllowedSides[3] = true;
	AllowedSides[1] = false;
	AllowedSides[2] = false;
}

void DockBase::AllowTopRight()
{
	AllowedSides[1] = true;
	AllowedSides[2] = true;
	AllowedSides[0] = false;
	AllowedSides[3] = false;
}

void DockBase::AllowNestedTabbing(bool b)
{
	tabsnested = b;
	controlpanel.TabOptionNest <<= tabsnested;
}

void DockBase::ControlPanel()
{
    if(controlpanel.IsOpen()) return;
    controlpanel.Open(GetTopWindow());
    RefreshPanel(); 
}

void DockBase::OnPanelAction()
{
    CtrlRecord* ctrlrecord = GetCtrlRecordFromIndex(controlpanel.list.GetClickRow());
    if(ctrlrecord) 
    	ctrlrecord->ctrl->Menu();
}

void DockBase::RefreshPanel()
{
    if(!controlpanel.IsOpen() || !controlpanel.IsVisible()) return;
    controlpanel.list.Clear();
    for(int i = 0; i < GetCtrlRecords().GetCount(); i++) 
    {
        int 	alignment 	= 0;
        bool 	tabbed		= false;
        bool	floating	= false;
		DockableCtrl* ctrl 	= GetCtrlRecordFromIndex(i)->ctrl;
		if(ctrl->IsTabbed())
		{
			TabWindow* tabwindow = dynamic_cast<DockWindow*>(ctrl)->GetOwnerTab();
			if(tabwindow) 
			{
				alignment = tabwindow->GetBaseTab()->Alignment();
				floating = tabwindow->GetBaseTab()->IsAutoHidden();
				tabbed = true;
			}
		}
		else alignment = ctrl->Alignment();
		String a, s;
        switch(alignment)
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
                if(ctrl->IsFloating() || tabbed)
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
            	floating ?
            	s = t_("AUTO HIDDEN")
            	:
                s = t_("TABBED");
                break;
        }
        controlpanel.list.Add(0, Format("%s",ctrl->GetLabel()), a, s, ctrl->Position());
        controlpanel.list.SetCtrl(i, 0, (panelicons.At(i))->SetImage(ctrl->GetIcon().IsNullInstance() ? 
                    DockCtrlImages::DefaultImage : ctrl->GetIcon()));
      }
   controlpanel.Refresh();
}

void DockBase::Serialize(Stream& s)
{
	String 	layout	= "CTRLSLAYOUT";
	String 	settings = "DOCKCTRLSETTINGS";
	int version = 1;	
	int n = 0;
	
	s.Magic(1234765);
	
	if(s.IsStoring())
	{
		s / version;
		SerializePlacement(s);
		s / settings;
		s % tabsicons;
		s % tabsclose;
		s % tabsautoalign;
		s % tabsnested;
		s % autohideicons;
		s % autohideclose;
		s / layoutindex;
		s / skinindex;
		s / hotkey;

		s / layout;
		//  Write Layouts.
		n = layouts.GetCount();
		s / n;
		for(int i = 0; i < n; i++)
		{
			String key = layouts.GetKey(i);
			String lay = layouts.Get(key);
			s / key;
			s / lay;
		}
		// Write Layout List.
		n = controlpanel.layoutlist.GetCount();
		s / n;
		for(int i = 0; i < n; i++)
		{
			String lname = controlpanel.layoutlist.GetKey(i);
			s / lname; 
		}
	}
	else
	{
		n = 0;
		s / version;
		SerializePlacement(s);
		s / settings;
		s % tabsicons;
		s % tabsclose;
		s % tabsautoalign;
		s % tabsnested;
		s % autohideicons;
		s % autohideclose;
		s / layoutindex;
		s / skinindex;
		s / hotkey;
		
		s / layout;
		// Read Layouts.
		s / n;
		for(int i = 0; i < n; i++)
		{
			String key, lay;
			s / key;
			s / lay;
			layouts.FindAdd(key, lay);
		}
		// Read Layout List.
		s / n;
		for(int i = 0; i < n; i++)
		{
			String lname;
			s / lname;
			controlpanel.layoutlist.Add(lname);
		}
		
		// Write Ctrl values.
		controlpanel.TabOptionIcon 		<<= tabsicons;
		controlpanel.TabOptionClose 	<<= tabsclose;
		controlpanel.TabOptionNest		<<= tabsnested;
		controlpanel.TabOptionAlignment <<= tabsautoalign;
		controlpanel.AutoOptionIcon 	<<= autohideicons;
		controlpanel.AutoOptionClose 	<<= autohideclose;
		
		controlpanel.layoutlist.SetIndex(layoutindex);
		controlpanel.skinlist.SetIndex(skinindex);
	}
	s.Magic();
}

void DockBase::InitFrameWorkSettings()
{
	RefreshWidgetLayout();
	SetSkin(skinindex);
	SetWidgetLayout(layoutindex);
}

void DockBase::InitDefaultLayout()
{
	String name = t_("Default");
	if(layouts.Find(name) < 0) NewWidgetLayout(name);
}

void DockBase::SetSkin(int index)
{
	switch(index)
	{
		case 0: style = &(DockCtrlChStyle::StyleDefault()); 	break;
		case 1:	style = &(DockCtrlChStyle::StyleClassic());		break;
		case 2: style = &(DockCtrlChStyle::StyleEnhanced());	break;
		default: break;
	}

	for(int i = 0; i < GetCtrlRecords().GetCount(); i++)
		dynamic_cast<DockWindow*>(GetCtrlRecordFromIndex(i)->ctrl)->SetStyle(*style);        
 	for(int i = 0; i < GetTabWindowCount(); i++)
 		GetTabWindowFromIndex(i)->SetStyle(*style);
}

DockBase& DockBase::SetStyle(const DockCtrlChStyle::Style& s)
{
    style = &s; 
    Refresh(); 
    return *this;   
}


void DockBase::OnSelectSkin()
{
	int newindex = controlpanel.skinlist.GetIndex();
	if(skinindex == newindex) return;
	SetSkin(skinindex = newindex);
}

void DockBase::OnSelectLayout()
{
	int newindex = controlpanel.layoutlist.GetIndex();
	if(layoutindex == newindex) return;
	SetWidgetLayout(layoutindex = newindex);
}

void DockBase::InitFrameWork()
{
	panesize = GetSize();
	int cx = panesize.cx / 4;
	int cy = panesize.cy / 3;
	
	AddFrame(hide[0]);
	AddFrame(hide[1]);
	AddFrame(hide[2]);
	AddFrame(hide[3]);

	hide[0].SetLayout(DockCtrlCustomFrame::LAYOUT_LEFT);
	hide[2].SetLayout(DockCtrlCustomFrame::LAYOUT_RIGHT);
	hide[1].SetLayout(DockCtrlCustomFrame::LAYOUT_TOP);
	hide[3].SetLayout(DockCtrlCustomFrame::LAYOUT_BOTTOM);	

	AddFrame(pane[0]);
	AddFrame(pane[2]);
	AddFrame(pane[1]);
	AddFrame(pane[3]);
	
	pane[0].SetLayout(this, PaneFrame::LEFT, cx);
	pane[2].SetLayout(this, PaneFrame::RIGHT, cx);
	pane[1].SetLayout(this, PaneFrame::TOP, cy);
	pane[3].SetLayout(this, PaneFrame::BOTTOM, cy);
}

DockBase::DockBase()
{
    panesize.Clear();
    LeftPos(0, 0).TopPos(0,0);
    CtrlLayout(controlpanel);
	CtrlLayoutOKCancel(newlayoutdialog, t_("Add new layout")); 
   	controlpanel.CenterScreen().Sizeable().Zoomable().Title(t_("Settings"));
    controlpanel.list.AddColumn();
    controlpanel.list.AddColumn(t_("Window Title"));
    controlpanel.list.AddColumn(t_("Alignment"));
    controlpanel.list.AddColumn(t_("State"));
    controlpanel.list.AddColumn(t_("Position"));
    controlpanel.list.EvenRowColor();
    controlpanel.list.SetLineCy(StdFont().GetHeight() + 2);
    controlpanel.skinlist.Add(0, String(t_("Default"))).Add(1, String(t_("Classic"))).Add(2, String(t_("Enhanced"))).SetIndex(skinindex);
    controlpanel.skinlist.WhenAction 	= THISBACK(OnSelectSkin);
    controlpanel.layoutlist.WhenAction	= THISBACK(OnSelectLayout);
    controlpanel.list.WhenLeftClick 	= THISBACK(OnPanelAction); 
    controlpanel.layoutbutton		<<= THISBACK(ResetWidgetLayout);
	controlpanel.layoutadd			<<= THISBACK(OnAddNewLayout);
	controlpanel.layoutdelete		<<= THISBACK(OnDeleteLayout);
    controlpanel.TabOptionAlignment <<= THISBACK(RefreshWidgetLayout);
    controlpanel.TabOptionIcon		<<= THISBACK(RefreshWidgetLayout);
    controlpanel.TabOptionClose		<<= THISBACK(RefreshWidgetLayout);
    controlpanel.AutoOptionClose	<<= THISBACK(RefreshWidgetLayout);
    controlpanel.AutoOptionIcon		<<= THISBACK(RefreshWidgetLayout);

	controlpanel.TabOptionIcon 		<<= tabsicons;
	controlpanel.TabOptionClose 	<<= tabsclose;
	controlpanel.TabOptionNest		<<= tabsnested;
	controlpanel.TabOptionAlignment <<= tabsautoalign;

	controlpanel.AutoOptionIcon 	<<= autohideicons;
	controlpanel.AutoOptionClose 	<<= autohideclose;
	SetKey(K_CTRL_HOME);
}

DockBase::~DockBase()
{
    for(int i = 0; i < panelicons.GetCount(); i++)
        delete panelicons.At(i);
	for(int i = 0; i < layouts.GetCount(); i++)
		layouts.Remove(i);

    DeleteCtrlRecords();
    panesize.Clear();
}

void DockBase::RefreshWidgetLayout()
{
	TabWindow* tabwindow = NULL;

	tabsicons 		= (bool) controlpanel.TabOptionIcon.Get();
	tabsclose		= (bool) controlpanel.TabOptionClose.Get();
	tabsnested		= (bool) controlpanel.TabOptionNest.Get();
	tabsautoalign	= (bool) controlpanel.TabOptionAlignment.Get();
	autohideicons	= (bool) controlpanel.AutoOptionIcon.Get();
	autohideclose	= (bool) controlpanel.AutoOptionClose.Get();	

	int n = GetTabWindowCount();
	for(int i = 0; i < n; i++)
	{
		tabwindow = GetTabWindowFromIndex(i);
		if(tabsautoalign)
		{
			switch(tabwindow->Alignment())
			{
       			case DockableCtrl::DOCK_LEFT:
					tabwindow->SetLayout(DockCtrlCustomFrame::LAYOUT_RIGHT);  
       				break;
  	     		case DockableCtrl::DOCK_TOP:
					tabwindow->SetLayout(DockCtrlCustomFrame::LAYOUT_BOTTOM);   
       				break;
    	   		case DockableCtrl::DOCK_RIGHT:
					tabwindow->SetLayout(DockCtrlCustomFrame::LAYOUT_LEFT);  
      	 			break;
    	   		case DockableCtrl::DOCK_BOTTOM:
					tabwindow->SetLayout(DockCtrlCustomFrame::LAYOUT_TOP);  
       				break;   
			}
		}
		else tabwindow->SetLayout(DockCtrlCustomFrame::LAYOUT_BOTTOM);
		tabwindow->GetTabs().HasIcons(tabsicons);
   		tabwindow->GetTabs().HasButtons(tabsclose);		
	}
	for(int i = 0; i < 4; i++) GetHideBar(i).HasButtons(autohideclose).HasIcons(autohideicons);
}

void DockBase::OnAddNewLayout()
{
	newlayoutdialog.layoutstring.Clear();
	
	for(;;)
	{
		switch(newlayoutdialog.Execute())
		{
			case IDOK:
			{
				String name = newlayoutdialog.layoutstring;
				if(IsNull(name))
				{
					Exclamation(t_("You must enter a name for the new layout"));
					break;
				}
				else if(layouts.Find(name) >= 0)
				{
					if(!PromptOKCancel(Format(t_("Layout '%s' adready exists. Do you want to overwrite it?"), name)))
					break;
				}
		
				NewWidgetLayout(name);
				RefreshPanel();
				return;
			}
			case IDCANCEL:
			return;
		}
	}
}


void DockBase::OnDeleteLayout()
{
	DelWidgetLayout(layouts.GetKey(layoutindex));	
}

void DockBase::AddCtrlRecord(DockableCtrl& ctrl)
{
	CtrlRecord* record 	= NULL;
	if(GetCtrlRecord(ctrl) || ctrl.IsTabWindow()) return;
	ctrls.Add((record = new CtrlRecord()));
	if(record) 
	{
		record->ctrl = &ctrl;
		record->id	 = ctrls.GetCount();
		ImageCtrl* imagectrl = new ImageCtrl();
    	panelicons.Add(imagectrl);
 	}
}

void DockBase::RemoveCtrlRecord(DockableCtrl& ctrl)
{
	CtrlRecord* record = GetCtrlRecord(ctrl);
	if(record) delete record;
}

void DockBase::DeleteCtrlRecords()
{
	for(int i = 0; i < ctrls.GetCount(); i++)
		delete ctrls.At(i);
}

DockBase::CtrlRecord* DockBase::GetCtrlRecord(DockableCtrl& ctrl)
{
	for(int i = 0; i < ctrls.GetCount(); i++)
	{
		CtrlRecord* record = ctrls.At(i);
		if(record->ctrl == &ctrl) return record;
	}	   
	return NULL;
}

DockBase::CtrlRecord* DockBase::GetCtrlRecordFromIndex(int index)
{
	if(index >= 0 && index < ctrls.GetCount())
		return ctrls.At(index);
	return NULL;
}

DockBase::CtrlRecord* DockBase::GetCtrlRecordFromId(int id)
{
	for(int i = 0; i < ctrls.GetCount(); i++)
	{
		CtrlRecord* record = ctrls.At(i);
		if(record->id == id) return record;
	}
	return NULL;
}

DockBase::CtrlRecord* DockBase::GetCtrlRecordFromGroup(String group)
{
	return NULL;
}

DockableCtrl* DockBase::GetDockedWindowFromIndex(int index)
{
	Vector<DockableCtrl*> docks;
	docks.Clear();
	for(int i = 0; i < 8; i++)
	{
		if(i < 4) 
		{
			int n = GetPaneFrame(i).GetCount();
			if(n) for(int j = 0; j < n; j++) docks.Add(GetPaneFrame(i).GetChild(j + 1));
		}
		else
		{
			int n = GetHideBar(i - 4).GetCount();
			if(n) for(int j = 0; j < n; j++) docks.Add(GetHideBar(i - 4).GetChild(j));
		}
	}
	return docks.GetCount() ? docks.At(index) : NULL;
}

TabWindow* DockBase::GetTabWindowFromIndex(int index)
{
	Vector<TabWindow*> tabs;
	tabs.Clear();
	for(int i = 0; i < 4; i++)
	{
		int n = GetPaneFrame(i).GetTabWindows().GetCount();
		if(n) 
			for(int j = 0; j < n; j++) 
				tabs.Add(GetPaneFrame(i).GetTabWindows().At(j));
	}
	TabWindow * tabwindow = (tabs.GetCount() ? tabs.At(index) : NULL);
	return tabwindow;
}

int DockBase::GetTabWindowCount()
{
	int tabcount = 0;
	for(int i = 0; i < 4; i++)
		tabcount += GetPaneFrame(i).GetTabWindows().GetCount();
	return tabcount;
}

int DockBase::GetDockedWindowCount()
{
	int dockcount = 0;
	for(int i = 0; i < 8; i++)
		if(i < 4) dockcount += GetPaneFrame(i).GetCount();
		else dockcount += GetHideBar(i - 4).GetCount();
	return dockcount;
}

void DockBase::NewWidgetLayout(String name)
{
	StringStream s;
	SerializeLayout(s);
	int id = AddWidgetLayout(name);
	layouts[id] = (String) s;
}

int DockBase::AddWidgetLayout(String name)
{
	int id = layouts.Find(name);
	if(id < 0)
	{
		id = layouts.FindAdd(name, String());
		controlpanel.layoutlist.Add(name);
		int index = controlpanel.layoutlist.GetCount() - 1;
		controlpanel.layoutlist.SetIndex(layoutindex = index);		
	}
	return id;
}

void DockBase::AddWidgetLayout(String name, DockableCtrl& ctrl, int alignment, int state, int position)
{
	
}

void DockBase::DelWidgetLayout(String name)
{
	int id = layouts.Find(name);
	if(id < 0) return;
	if(PromptOKCancel(Format(t_("Do you really want to delete layout '%s'"), name)))
	{
		layouts.RemoveKey(name);
		controlpanel.layoutlist.Remove(id);
		id--;
		controlpanel.layoutlist.SetIndex(layoutindex = id);
	}
}

void DockBase::SetWidgetLayout(String name)
{
	int id = layouts.Find(name);
	if(id < 0) return;
	SetWidgetLayout(id);
}

void DockBase::SetWidgetLayout(int index)
{
	if(layoutindex != index) controlpanel.layoutlist.SetIndex(layoutindex = index);
	if(index == 0) 	controlpanel.layoutdelete.Disable();
	else controlpanel.layoutdelete.Enable();
	
	String key = layouts.GetKey(index);
	String& layout = layouts.Get(key);
	StringStream s(layout);
	s.SetLoading();
	SerializeLayout(s);
}

void DockBase::SerializeLayout(Stream& s, bool deflay)
{
	CtrlRecord* record	= NULL;
	DockableCtrl* ctrl	= NULL;
	
	String  name;
	int		panesize	= 0;
	int 	type 		= 0;
	int 	alignment	= 0;
	int 	state		= 0;
	int 	position	= 0;
	int 	id			= 0;
	int 	childcount	= 0;
	Size	childsize;
	
		if(s.IsStoring())
		{
			// Write Widgets.
			// Write Docked (shown or hidden) and AutoHidden widgets.
			childcount = GetDockedWindowCount();
			s / childcount;
			if(childcount)
				for(int i = 0; i < childcount; i++)
				{
					ctrl = GetDockedWindowFromIndex(i);
					ASSERT(ctrl);
					ctrl->Serialize(s);
				}
			// Write PaneFrame sizes.
			for(int i = 0; i < 4; i++)  
			{
				panesize = GetPaneFrame(i).GetSize();
				s / panesize;
			}
			childcount = 0;
			int ctrlscount = ctrls.GetCount();
			// Write Floating Dockwindows.
			for(int i = 0; i < ctrlscount * 2; i++)
			{
				if(i < ctrlscount) childcount += GetCtrlRecordFromIndex(i)->ctrl->IsFloating() ? 1 : 0;
				if(i >= ctrlscount) 
				{
					if(i == ctrlscount) s / childcount;
					ctrl = GetCtrlRecordFromIndex(i - ctrlscount)->ctrl;
					ASSERT(ctrl);
					if(ctrl->IsFloating()) ctrl->Serialize(s);
				}
			}
			childcount = 0;
			// Write Floating TabWindows.
			int tabwindowcount = GetTabWindowCount();
			if(tabwindowcount)
				for(int i = 0; i < tabwindowcount * 2; i++)
				{
					if(i <  tabwindowcount) childcount += GetTabWindowFromIndex(i)->IsFloating() ? 1 : 0;
					if(i >= tabwindowcount)
					{
						if(i == tabwindowcount) s / childcount;
						TabWindow* tabwindow = GetTabWindowFromIndex(i - tabwindowcount);
						ASSERT(tabwindow);
						if(tabwindow->IsFloating()) tabwindow->Serialize(s);
					}
				}
		}

		if(s.IsLoading())
		{
			childcount = 0;
			// Close All widgets.
			for(int i = 0; i < GetTabWindowCount(); i++)	GetTabWindowFromIndex(i)->DetachAll();
			for(int i = 0; i < ctrls.GetCount(); i++) 		GetCtrlRecordFromIndex(i)->ctrl->Shut();
			// Read Docked (shown or hidden) and AutoHidden widgets.
			s / childcount;
			if(childcount)
				for(int i = 0; i < childcount; i++)
				{
					s / type / id;
					if(type == DockableCtrl::TYPE_DOCKWINDOW)
						GetCtrlRecordFromId(id)->ctrl->Serialize(s);
					if(type == DockableCtrl::TYPE_TABWINDOW)
						GetPaneFrame(0).AddTabWindow()->Serialize(s);
				}
			// Read PaneFrame sizes.
			for(int i = 0; i < 4; i++)
			{
				s / panesize;
				GetPaneFrame(i).SetSize(panesize);
			}
			childcount = 0;
			s / childcount;
			if(childcount)
				for(int i = 0; i < childcount; i++)
				{
					s / type / id;
					GetCtrlRecordFromId(id)->ctrl->Serialize(s);
				}
			
			childcount = 0;
			s / childcount;
			if(childcount)
				for(int j = 0; j < childcount; j++)
				{
					s / type / id;
					TabWindow* tabwindow = GetPaneFrame(0).AddTabWindow();
					tabwindow->Serialize(s);
				}
		}		
		s.Close();
}

void DockBase::ResetWidgetLayout()
{
	controlpanel.layoutlist.SetIndex(layoutindex = 0);
	SetWidgetLayout(layoutindex);
	RefreshPanel();
}

bool DockBase::Key(dword key, int count)
{
	if(key == hotkey) 
	 	ControlPanel();
	 return TopWindow::Key(key, count);
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

   	const ToolBar::Style* defguistyle = &ToolBar::StyleDefault();  
	barbackground[0] = defguistyle->arealook;

    DockCtrlImgsLook(barshut, DockCtrlImages::I_CClose, 3);
    DockCtrlImgsLook(barhide, DockCtrlImages::I_CHide, 3);
    DockCtrlImgsLook(barmenu, DockCtrlImages::I_CMenu, 3);

    CtrlsImageLook(tabnormal, CtrlsImg::I_TAB, 4);
    CtrlsImageLook(tabfirst, CtrlsImg::I_FTAB, 4);
    CtrlsImageLook(tablast, CtrlsImg::I_LTAB, 4);
    CtrlsImageLook(tabboth, CtrlsImg::I_BTAB, 4);      

	stdhighlight = 	StandardHighlight(Blend(SColorHighlight, SColorPaper, 90), SColorHighlight);
	tabhighlight = AlphaHighlight(stdhighlight, 80);     

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
    
    const TabCtrl::Style* defguistyle = &TabCtrl::StyleDefault();  
	barbackground[0] =  defguistyle->normal[0];

	DockCtrlImgsLook(barshut, DockCtrlImages::I_DClose, 3);
    DockCtrlImgsLook(barhide, DockCtrlImages::I_DHide, 3);
    DockCtrlImgsLook(barmenu, DockCtrlImages::I_DMenu, 3);
  
    CtrlsImageLook(tabnormal, CtrlsImg::I_TAB, 4);
    CtrlsImageLook(tabfirst, CtrlsImg::I_FTAB, 4);
    CtrlsImageLook(tablast, CtrlsImg::I_LTAB, 4);
    CtrlsImageLook(tabboth, CtrlsImg::I_BTAB, 4);  


	stdhighlight = 	StandardHighlight(Blend(SColorHighlight, SColorPaper, 90), SColorHighlight);
	tabhighlight = AlphaHighlight(stdhighlight, 80);     
	
}
