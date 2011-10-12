#ifndef _XMLMenuItem_h_
#define _XMLMenuItem_h_

#include "XMLToolBarCtrl.h"
#include "XMLCommand.h"

NAMESPACE_UPP

////////////////////////////////////////////////////////////////////////////////////
// a menu/bar item : combine a command with label, icon, tooltip.....
class XMLToolBar;
class XMLToolBarItem
{
	friend class XMLToolBar;
	private:
		// the command entry
		String commandId;
		
		// the command label, if any
		String label;
		
		// icon, if any
		Image icon;
		
		// tooltip, if any
		String tooltip;
		
		// a submenu, if any
		One<XMLToolBar> subMenu;
		
	public:
		String const &GetId(void) const				{ return commandId; }
		String const &GetLabel(void) const			{ return label; }
		Image const &GetIcon(void) const			{ return icon; }
		String const &GetTooltip(void) const		{ return tooltip; }
		XMLToolBar const &GetSubMenu(void) const	{ return *subMenu; }
		bool IsSubMenu(void) const					{ return !subMenu.IsEmpty(); }
		
		// xml support
		void Xmlize(XmlIO xml);
};

////////////////////////////////////////////////////////////////////////////////////
// a menu/bar : an array of menu/bar items -- builds up a toolbar or a menu
class XMLToolBar
{
	friend class XMLToolBarItem;
	private:
		// bar name
		String name;
		
		// items
		Array<XMLToolBarItem> items;
		
		// position of toolbar
		XMLToolBarCtrl::XMLToolBarState state;
		int row, col;
		
	public:
		// constructor
		XMLToolBar();
		
		// pick constructor
		XMLToolBar(XMLToolBar pick_ &tb);
		
		// copy operator
		XMLToolBar &operator=(XMLToolBar pick_ &tb);
		
		// add an entry, various ways
		XMLToolBar &SetName(String const &name);
		XMLToolBar &Add(String const &commandId);
		XMLToolBar &Add(String const &commandId, String const &label);
		XMLToolBar &Add(String const &commandId, Image const &icon);
		XMLToolBar &Add(String const &commandId, String const &label, Image const &icon);
		XMLToolBar &Add(String const &commandId, Image const &icon, String const &tooltip);
		XMLToolBar &Add(String const &commandId, String const &label, String const &tooltip);
		XMLToolBar &Add(String const &commandId, String const &label, Image const &icon, String const &tooltip);

		// add a submenu entry
		XMLToolBar &Add(String const &subLabel, XMLToolBar pick_ &subMenu);
		
		// creates a submenu entry
		XMLToolBar SubMenu(void);
		
		// add a submenu entry by callback
		XMLToolBar &Add(Callback1<XMLToolBar &> bar);

		// gets toolbar name
		String const &GetName(void) const				{ return name; }
		
		// gets toolbar items
		Array<XMLToolBarItem> const &GetItems(void) const	{ return items; }

		// sets the commands by a callback
		void Set(Callback1<XMLToolBar &> bar);

		// xml support
		void Xmlize(XmlIO xml);
};

////////////////////////////////////////////////////////////////////////////////////
class XMLToolBars : public Array<XMLToolBar>
{
	private:
	
	protected:
	
	public:
	
		XMLToolBars &Add(String const &name);
	
};

END_UPP_NAMESPACE

#endif
