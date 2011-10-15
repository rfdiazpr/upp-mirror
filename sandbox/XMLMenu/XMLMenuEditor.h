#ifndef _XMLMenuEditor_h_
#define _XMLMenuEditor_h_

#include <CtrlLib/CtrlLib.h>

#include "XMLMenuInterface.h"

NAMESPACE_UPP

#define LAYOUTFILE <XMLMenu/XMLMenuEditor.lay>
#include <CtrlCore/lay.h>

class XMLMenuEditor : public WithMenuEditorLayout<TopWindow>
{
	private :
		// interface to access menu system data
		XMLMenuInterface *iFace;
		
		// left pane
		WithCmdBoxLayout<ParentCtrl> cmdPane;

		// right pane - contains splitter for tabbed ctrl and layout editor
		WithTabItemLayout<ParentCtrl> rightPane;
		
		// tab ctrl pane
		WithTabsLayout<ParentCtrl> tabsPane;
		
		// item editor
		WithItemEditorLayout<ParentCtrl> itemPane;
		
		// vertical size of button area
		int buttonVertSize;
		
		// original sizes of cmd area and item editor area
		Size cmdSize, itemSize;

		// minimum window width and height
		int minWidth, minHeight;
	
	protected :
	
		void cancelCb(void);
		void okCb(void);
		
		// adjust layout on win changes
		void Layout(void);
		
		// gets commands from iFace and fills the command list
		void GetCommands(void);
	
	public :
	
		typedef XMLMenuEditor CLASSNAME;
		
		XMLMenuEditor(XMLMenuInterface *iFace);
		
		~XMLMenuEditor();
		
};

END_UPP_NAMESPACE

#endif
