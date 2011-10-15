#include "XMLMenuEditor.h"

NAMESPACE_UPP

////////////////////////////////////////////////////////////////////////////////
// constructor
XMLBarEditor::XMLBarEditor()
{
	bar = NULL;
	item = NULL;
	itemSize = itemPane.GetLayoutSize();
	CtrlLayout(itemPane);
	Add(vertSplitter);
	vertSplitter.Vert(barTree, itemPane);
}

// gets minimum size of bar editor
Size XMLBarEditor::GetMinSize(void)
{
	return Size(itemSize.cx, itemSize.cy * 2);
}

// layouts control
void XMLBarEditor::Layout(void)
{
	int h = GetSize().cy;
	if(h)
		vertSplitter.SetPos(10000 * (h - itemSize.cy) / h);
}
		
// sets bar being edited
void XMLBarEditor::SetBar(XMLToolBar &bar)
{
}

////////////////////////////////////////////////////////////////////////////////
// constructor
XMLBarsEditor::XMLBarsEditor()
{
	// get toolbar/menu selector pane sizes, it's width will be fixed too
	selectorSize = barListPane.GetLayoutSize();
	
	CtrlLayout(barListPane);

	Add(horzSplitter.SizePos());
	horzSplitter.Horz(barListPane, barEditor);
}

// gets minimum size of bar editor
Size XMLBarsEditor::GetMinSize(void)
{
	return Size(
		selectorSize.cx + barEditor.GetMinSize().cx,
		max(selectorSize.cy, barEditor.GetMinSize().cy)
	);
}

// adjust layout on win changes
void XMLBarsEditor::Layout(void)
{
	horzSplitter.SetPos(10000 * selectorSize.cx / GetSize().cx);
}

// set title
void XMLBarsEditor::SetTitle(const char *s)
{
	barListPane.title = s;
}

////////////////////////////////////////////////////////////////////////////////
void XMLMenuEditor::cancelCb(void)
{
	Break(IDCANCEL);
	Close();
}

void XMLMenuEditor::okCb(void)
{
	Break(IDOK);
	Close();
}

XMLMenuEditor::XMLMenuEditor(XMLMenuInterface *_iFace) : iFace(_iFace)
{
	CtrlLayout(*this);
	
	// setup ok and cancel handlers
	okButton.Ok() <<= THISBACK(okCb);
	cancelButton.Cancel() <<= THISBACK(cancelCb);
	
	// get command pane size, that one will be fixed in width
	cmdSize = cmdPane.GetLayoutSize();
	
	// gets menu/bar editor minimum sizes
	editorSize = menusEditor.GetMinSize();
	
	// now calculate tabctrl minimum sizes
	int tx = editorSize.cx;
	int ty = cmdSize.cy;
	tabCtrlSize = Size(tx, ty);
	
	// gets button vertical neede size
	buttonVertSize = GetLayoutSize().cy - horzSplitter.GetSize().cy;
	
	// adds items to horz splitter
	horzSplitter.Horz(cmdPane, tabCtrl);
	
	// adds items to tabctrl
	tabCtrl.Add(menusEditor.SizePos(), t_("Menus"));
	tabCtrl.Add(barsEditor.SizePos(), t_("ToolBars"));

	// adds layout to all ctrls
	CtrlLayout(cmdPane);
	
	// set title for selector panes
	menusEditor.SetTitle(t_("Available menus"));
	barsEditor.SetTitle(t_("Available toolbars"));
	
	// adjust window width to accomodate at least itemPane and cmdPane
	minWidth = cmdSize.cx + tabCtrlSize.cx;
	
	// adjust window height to accomodate cmdPane
	minHeight = cmdSize.cy;
	
	SetMinSize(Size(minWidth, minHeight));
	
	int w = max(GetSize().cx, minWidth);
	int h = max(GetSize().cy - buttonVertSize, minHeight);
	int wr = w;
	int hr = h + buttonVertSize;
	SetRect(GetRect().left, GetRect().top, wr, hr);
	
	Sizeable();
	
	// reads the commands and put them into list
	GetCommands();
}

XMLMenuEditor::~XMLMenuEditor()
{
}

// adjust layout on win changes
void XMLMenuEditor::Layout(void)
{
	// adjust splitter sizes
	horzSplitter.SetPos(10000 * cmdSize.cx / GetSize().cx);
}
		
// gets commands from iFace and fills the command list
void XMLMenuEditor::GetCommands(void)
{
	Vector<String> const &ids = iFace->GetCommands().GetIds();
	cmdPane.commandList.Clear();
	for(int i = 0; i < ids.GetCount(); i++)
		cmdPane.commandList.Add(ids[i]);
}
	
END_UPP_NAMESPACE