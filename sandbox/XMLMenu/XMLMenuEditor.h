#ifndef _XMLMenuEditor_h_
#define _XMLMenuEditor_h_

#include <CtrlLib/CtrlLib.h>

#include "XMLMenuInterface.h"

NAMESPACE_UPP

#define LAYOUTFILE <XMLMenu/XMLMenuEditor.lay>
#include <CtrlCore/lay.h>

////////////////////////////////////////////////////////////////////////////////
// bar editor class -- allow editing of a single menu/toolbar
class XMLBarEditor : public ParentCtrl
{
	private:
		// toolbar being edited
		XMLToolBar *bar;
		
		// bar item being edited
		XMLToolBarItem *item;
		Size itemSize;
	
		// item editor pane
		WithItemEditorLayout<ParentCtrl> itemPane;
		
		// tree ctrl containing bar structure
		TreeCtrl barTree;

		// vertical splitter dividing tree from item editor
		Splitter vertSplitter;
		
		// layouts control
		void Layout(void);
		
	protected:
	
	public:
		typedef XMLBarEditor CLASSNAME;
		
		// constructor
		XMLBarEditor();
		
		// gets minimum size of bar editor
		Size GetMinSize(void);
		
		// sets bar being edited
		void SetBar(XMLToolBar &bar);
};

////////////////////////////////////////////////////////////////////////////////
class XMLBarsEditor : public ParentCtrl
{
	private:
	
		// MenuBars and ToolBars lists
		WithBarListLayout<ParentCtrl> barListPane;
		
		// horizontal splitter
		Splitter horzSplitter;
		
		// the bar editor
		XMLBarEditor barEditor;
		
		// size of selector
		Size selectorSize;
	
	protected:
	
	public:
	
		// constructor
		XMLBarsEditor();

		// gets minimum size of bar editor
		Size GetMinSize(void);

		// adjust layout on win changes
		void Layout(void);

		// set title
		void SetTitle(const char *s);
};

////////////////////////////////////////////////////////////////////////////////
class XMLMenuEditor : public WithMenuEditorLayout<TopWindow>
{
	private :
		// interface to access menu system data
		XMLMenuInterface *iFace;
		
		// left pane
		WithCmdBoxLayout<ParentCtrl> cmdPane;
		
		// the menus and bars editors
		XMLBarsEditor menusEditor, barsEditor;

		// right pane - contains tab ctrl
		TabCtrl tabCtrl;
		
		// vertical size of button area
		int buttonVertSize;
		
		// original sizes of cmd area and item editor area
		Size cmdSize, editorSize, tabCtrlSize;

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
