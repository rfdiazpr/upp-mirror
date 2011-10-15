#include "XMLMenuEditor.h"

NAMESPACE_UPP

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
	
	// gets size of item editor pane, we keep that one fixed size
	itemSize = itemPane.GetLayoutSize();
	
	// get command pane size, that one will be fixed too
	cmdSize = cmdPane.GetLayoutSize();
	
	// gets button vertical neede size
	buttonVertSize = GetLayoutSize().cy - horzSplitter.GetSize().cy;
	
	// adds items to horz splitter
	horzSplitter.Horz(cmdPane, rightPane);

	// adds layout to all ctrls
	CtrlLayout(cmdPane);
	CtrlLayout(rightPane);
	CtrlLayout(tabsPane);
	CtrlLayout(itemPane);
	
	// adjust window width to accomodate at least itemPane and cmdPane
	minWidth = itemSize.cx + cmdSize.cx;
	
	// adjust window height to accomodate cmdPane
	minHeight = cmdSize.cy;
	
	// setup tabs
	tabsPane.tabCtrl.Add(t_("Menu"));
	tabsPane.tabCtrl.Add(t_("ToolBars"));
	
	int w = max(GetSize().cx, minWidth);
	int h = max(GetSize().cy - buttonVertSize, minHeight);
	int wr = w;
	int hr = h + buttonVertSize;
	SetRect(GetRect().left, GetRect().top, wr, hr);
	
	// adds items to vertical splitter
	rightPane.vertSplitter.Vert(tabsPane, itemPane);
	
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
	int netH = GetSize().cy - buttonVertSize;
	rightPane.vertSplitter.SetPos(10000 * (netH - itemSize.cy) / netH);
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