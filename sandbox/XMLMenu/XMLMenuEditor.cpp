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
	
	// setup tabs
	tabCtrl.Add(t_("Menu"));
	tabCtrl.Add(t_("ToolBars"));
	
	// reads the commands and put them into list
	GetCommands();
}

XMLMenuEditor::~XMLMenuEditor()
{
}

// gets commands from iFace and fills the command list
void XMLMenuEditor::GetCommands(void)
{
	Vector<String> const &ids = iFace->GetCommands().GetIds();
DLOG(ids.GetCount());
	commandList.Clear();
	for(int i = 0; i < ids.GetCount(); i++)
		commandList.Add(ids[i]);
}
	
END_UPP_NAMESPACE