#ifndef _XMLMenuItem_h_
#define _XMLMenuItem_h_

#include "XMLCommand.h"

NAMESPACE_UPP

typedef enum
{
	TOOLBAR_CLOSED,
	TOOLBAR_FLOATING,
	TOOLBAR_HORZ_POPUP,
	TOOLBAR_VERT_POPUP,
	TOOLBAR_SQUARE_POPUP,
	TOOLBAR_TOP,
	TOOLBAR_BOTTOM,
	TOOLBAR_LEFT,
	TOOLBAR_RIGHT
	
} XMLToolBarState;

////////////////////////////////////////////////////////////////////////////////////
// a menu/bar item : combine a command with label, icon, tooltip.....
class XMLToolBar;
class XMLToolBarItem : DeepCopyOption<XMLToolBarItem>
{
	friend class XMLToolBar;
	friend class XMLBarEditor;
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
		
		// constructor
		XMLToolBarItem();
		
		// copy constructor
		XMLToolBarItem(const XMLToolBarItem &item, int dummy);

#ifdef flagDEBUG
		// debugging stuff -- dumps bar content
		void Dump(int level = 0);
#endif
};

////////////////////////////////////////////////////////////////////////////////////
// a menu/bar : an array of menu/bar items -- builds up a toolbar or a menu
class XMLToolBar : DeepCopyOption<XMLToolBar>
{
	friend class XMLToolBarItem;
	friend class XMLBarEditor;
	private:
		// bar name
		String name;
		
		// items
		Array<XMLToolBarItem> items;
		
		// position of toolbar
		XMLToolBarState state;
		int x, y;
		
	public:
		// constructor
		XMLToolBar();
		
		// pick constructor
		XMLToolBar(XMLToolBar pick_ &tb);
		
		// copy constructor
		XMLToolBar(XMLToolBar const &tb, int dummy);
		
		// copy operator
		XMLToolBar &operator=(XMLToolBar pick_ &tb);
		
		// add an entry, various ways
		XMLToolBar &SetName(String const &_name)		{ name = _name; return *this; }
		XMLToolBar &SetState(XMLToolBarState _state)	{ state = _state; return *this; }
		XMLToolBar &SetPos(int _x, int _y)				{ x = _x; y = _y; return *this; }

		XMLToolBar &Add(String const &commandId);
		XMLToolBar &Add(String const &commandId, String const &label);
		XMLToolBar &Add(String const &commandId, Image const &icon);
		XMLToolBar &Add(String const &commandId, String const &label, Image const &icon);
		XMLToolBar &Add(String const &commandId, Image const &icon, String const &tooltip);
		XMLToolBar &Add(String const &commandId, String const &label, String const &tooltip);
		XMLToolBar &Add(String const &commandId, String const &label, Image const &icon, String const &tooltip);

		// add a submenu entry
		XMLToolBar &Add(String const &subLabel, XMLToolBar pick_ &subMenu);
		XMLToolBar &Add(String const &subLabel, Image const &icon, XMLToolBar pick_ &subMenu);
		
		// creates a submenu entry
		XMLToolBar SubMenu(void);
		
		// add a submenu entry by callback
		XMLToolBar &Add(Callback1<XMLToolBar &> bar);

		// gets toolbar name
		String const &GetName(void) const				{ return name; }
		
		// get toolbar state and position
		XMLToolBarState GetState(void)	{ return state; }
		int Getx(void)									{ return x; }
		int Gety(void)									{ return y; }
		
		// gets toolbar items
		Array<XMLToolBarItem> const &GetItems(void) const	{ return items; }

		// sets the commands by a callback
		void Set(Callback1<XMLToolBar &> bar);

		// xml support
		void Xmlize(XmlIO xml);

#ifdef flagDEBUG
		// debugging stuff -- dumps bar content
		void Dump(int level = 0);
#endif
};

////////////////////////////////////////////////////////////////////////////////////
class XMLToolBars : public ArrayMap<String, XMLToolBar>
{
	private:
	
	protected:
	
	public:
	
		// adds a new toolbar
		XMLToolBars &Add(String const &name, XMLToolBar pick_ &tb);
		
		// returns an empty toolbar at a given pos and state
		XMLToolBar ToolBar(XMLToolBarState state, int row, int col);
		XMLToolBar MenuBar(void) { return ToolBar(TOOLBAR_TOP, 0, 0); }

		// creates a submenu entry
		XMLToolBar SubMenu(void);

#ifdef flagDEBUG
		// debugging stuff -- dumps bar content
		void Dump(int level = 0);
#endif
};

END_UPP_NAMESPACE

#endif
