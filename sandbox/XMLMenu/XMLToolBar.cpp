#include "XMLToolBar.h"

NAMESPACE_UPP

////////////////////////////////////////////////////////////////////////////////////
// XMLToolBarItem

////////////////////////////////////////////////////////////////////////////////////
// XMLToolBar

// constructor
XMLToolBar::XMLToolBar()
{
	name = "";
	items.Clear();
	state = XMLToolBarCtrl::TOOLBAR_TOP;
	x = 0;
	y = 0;
}

// pick constructor
XMLToolBar::XMLToolBar(XMLToolBar pick_ &tb)
{
	name = tb.name;
	items = tb.items;
	state = tb.state;
	x = tb.x;
	y = tb.y;
}

// copy operator
XMLToolBar &XMLToolBar::operator=(XMLToolBar pick_ &tb)
{
	name = tb.name;
	items = tb.items;
	state = tb.state;
	x = tb.x;
	y = tb.y;
	return *this;
}

// add an entry, various ways
XMLToolBar &XMLToolBar::Add(String const &commandId)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->commandId = commandId;
	item->label = commandId;
	item->icon = Null;
	item->subMenu.Clear();
	item->tooltip = "";
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(String const &commandId, String const &label)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->commandId = commandId;
	item->label = label;
	item->icon = Null;
	item->subMenu.Clear();
	item->tooltip = "";
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(String const &commandId, Image const &icon)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->commandId = commandId;
	item->label = commandId;
	item->icon = icon;
	item->subMenu.Clear();
	item->tooltip = "";
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(String const &commandId, String const &label, Image const &icon)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->commandId = commandId;
	item->label = label;
	item->icon = icon;
	item->subMenu.Clear();
	item->tooltip = "";
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(String const &commandId, Image const &icon, String const &tooltip)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->commandId = commandId;
	item->label = commandId;
	item->icon = icon;
	item->subMenu.Clear();
	item->tooltip = tooltip;
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(String const &commandId, String const &label, String const &tooltip)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->commandId = commandId;
	item->label = label;
	item->icon = Null;
	item->subMenu.Clear();
	item->tooltip = tooltip;
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(String const &commandId, String const &label, Image const &icon, String const &tooltip)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->commandId = commandId;
	item->label = label;
	item->icon = icon;
	item->subMenu.Clear();
	item->tooltip = tooltip;
	items.Add(item);
	return *this;
}

// add a submenu entry
XMLToolBar &XMLToolBar::Add(String const &subLabel, XMLToolBar pick_ &subMenu)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->label = subLabel;
	item->subMenu = new XMLToolBar(subMenu);
	item->subMenu->name = subLabel;
	items.Add(item);
	return *this;
}

XMLToolBar &XMLToolBar::Add(String const &subLabel, Image const &icon, XMLToolBar pick_ &subMenu)
{
	XMLToolBarItem *item = new XMLToolBarItem;
	item->label = subLabel;
	item->icon = icon;
	item->subMenu = new XMLToolBar(subMenu);
	item->subMenu->name = subLabel;
	items.Add(item);
	return *this;
}

// creates a submenu entry
XMLToolBar XMLToolBar::SubMenu(void)
{
	return XMLToolBar();
}

// add a submenu entry by callback
XMLToolBar &XMLToolBar::Add(Callback1<XMLToolBar &> bar)
{
	return *this;
}

// sets the commands by a callback
void XMLToolBar::Set(Callback1<XMLToolBar &> bar)
{
}

////////////////////////////////////////////////////////////////////////////////////

// adds a new toolbar
XMLToolBars &XMLToolBars::Add(String const &name, XMLToolBar pick_ &tb)
{
	ArrayMap<String, XMLToolBar>::Add(name, tb);
	Top().SetName(name);
	return *this;
}

// returns an empty toolbar at a given pos and state
XMLToolBar XMLToolBars::ToolBar(XMLToolBarCtrl::XMLToolBarState state, int row, int col)
{
	XMLToolBar res;
	res.SetState(state);
	res.SetPos(row, col);
	return res;
}

// creates a submenu entry
XMLToolBar XMLToolBars::SubMenu(void)
{
	return XMLToolBar();
}


END_UPP_NAMESPACE