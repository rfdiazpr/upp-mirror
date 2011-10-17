#include "XMLMenuEditor.h"

#include <plugin/bmp/bmp.h>

NAMESPACE_UPP

// colored Display
class ColDisp : public Display
{
		Color color;
	public :
		ColDisp(Color const &c) { color = c; }
		virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
			{ Display::Paint(w, r, q, color, paper, style); }
};
static ColDisp RedDisp(Red());

////////////////////////////////////////////////////////////////////////////////
// constructor
XMLBarEditor::XMLBarEditor()
{
	bar = NULL;
	curIcon = Null;
	itemSize = itemPane.GetLayoutSize();
	CtrlLayout(itemPane);
	Add(vertSplitter);
	vertSplitter.Vert(barTree, itemPane);
	
	// drag'n drop
	barTree.WhenDropInsert = THISBACK(dropInsertCb);
	barTree.WhenDrag = THISBACK(dragCb);
	barTree.WhenSel  = THISBACK(itemSelCb);
	
	itemPane.cmdId		<<= THISBACK(fieldsModCb);
	itemPane.label		<<= THISBACK(fieldsModCb);
	itemPane.tooltip	<<= THISBACK(fieldsModCb);
	
	itemPane.icon		<<= THISBACK(imageSelCb);
	
	itemPane.Disable();
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
		
// refresh current bar
void XMLBarEditor::RefreshBar(int treeRoot, XMLToolBar *subBar)
{
	if(!subBar)
		subBar = bar;
	if(!subBar)
		return;
	subBar->items.Clear();
	for(int iChild = 0; iChild < barTree.GetChildCount(treeRoot); iChild++)
	{
		int iNode = barTree.GetChild(treeRoot, iChild);
		subBar->items.Add(new XMLToolBarItem(ValueTo<XMLToolBarItem>(barTree.Get(iNode)), 0));
		if(barTree.GetChildCount(iNode))
		{
			subBar->items.Top().subMenu = new XMLToolBar;
			RefreshBar(iNode, ~subBar->items.Top().subMenu);
		}
		else
			subBar->items.Top().subMenu.Clear();
	}
}

// sets bar being edited
void XMLBarEditor::SetBar(XMLToolBar *_bar)
{
	// if changing bar, update current one if not null
	// before changing it
	if(bar && bar != _bar)
		RefreshBar();
	
	bar = _bar;
	barTree.Clear();
	if(!bar)
		return;
	buildTree(0, bar);
	barTree.SetRoot(Null, bar->GetName());
	barTree.OpenDeep(0);
}

// builds bar tree
void XMLBarEditor::buildTree(int root, XMLToolBar const *bar)
{
	Array<XMLToolBarItem> const &items = bar->GetItems();
	for(int iItem = 0; iItem < items.GetCount(); iItem++)
	{
		XMLToolBarItem const &item = items[iItem];
		TreeCtrl::Node node(item.GetIcon(), RawDeepToValue(item), item.GetLabel());
		int iNode = barTree.Add(root, node);
		if(items[iItem].IsSubMenu())
			buildTree(iNode, &items[iItem].GetSubMenu());
	}
}

// dragging element
void XMLBarEditor::dragCb(void)
{
	if(barTree.DoDragAndDrop(InternalClip(barTree, "mytreedrag"), barTree.GetDragSample()) == DND_MOVE)
		barTree.RemoveSelection();
}

// dropping between elements (inserts between)
void XMLBarEditor::dropInsertCb(int parent, int ii, PasteClip& d)
{
	barTree.AdjustAction(parent, d);
	if(AcceptInternal<TreeCtrl>(d, "mytreedrag"))
	{
		barTree.InsertDrop(parent, ii, d);
		barTree.SetFocus();
		return;
	}
	if(AcceptText(d))
	{
		barTree.SetCursor(barTree.Insert(parent, ii, Image(), GetString(d)));
		barTree.SetFocus();
		return;
	}
}
		
// item selection callback
void XMLBarEditor::itemSelCb()
{
	// get selected node
	int i = barTree.GetCursor();

	// if none, just disable input fields and return
	if(i < 0)
	{
		itemPane.Disable();
		return;
	}

	// enable input pane
	itemPane.Enable();
	
	// enable label and tooltip fields
	itemPane.label.Enable();

	// disable command field, it'll be filled
	// by command bar anyways
	itemPane.cmdId.Disable();
	
	// if not the root, treat as an item
	// otherwise allow just to edit bar name
	if(i > 0)
	{
		itemPane.icon.Enable();
		itemPane.tooltip.Enable();
		
		// fetch node data
		XMLToolBarItem const &item = ValueTo<XMLToolBarItem>(barTree.Get(i));
		
		bool hasChilds = barTree.GetChildCount(i);
	
		// fill fields
		if(hasChilds)
			itemPane.cmdId.Clear();
		else
			itemPane.cmdId		= item.GetId();
		itemPane.labelName = t_("Label :");
		itemPane.label		= item.GetLabel();
		itemPane.tooltip	= item.GetTooltip();
		itemPane.icon.SetImage(item.GetIcon());
		curIcon = item.GetIcon();
	}
	else
	{
		itemPane.icon.Disable();
		itemPane.tooltip.Disable();
		itemPane.cmdId.Clear();
		itemPane.labelName = t_("Bar name :");
		itemPane.label		= String(barTree.GetValue(0));
		itemPane.tooltip.Clear();
		itemPane.icon.SetImage(Null);
		curIcon = Null;
	}
}

// fields modified callback
void XMLBarEditor::fieldsModCb(void)
{
	// get selected node
	int i = barTree.GetCursor();
	if( i < 0 || bar == 0)
		return;
	if(i == 0)
	{
		bar->SetName(itemPane.label);
		barTree.SetRoot(Null, itemPane.label);
		return;
	}

	XMLToolBarItem item(ValueTo<XMLToolBarItem>(barTree.Get(i)), 0);
	item.label		= itemPane.label;
	item.commandId	= itemPane.cmdId;
	item.tooltip	= itemPane.tooltip;
	item.icon		= curIcon;
	TreeCtrl::Node node(item.GetIcon(), RawDeepToValue(item), item.GetLabel());
	barTree.SetNode(i, node);
}
		
// image selection callback
void XMLBarEditor::imageSelCb(void)
{
	static String lastPath = "";
	
	// opens a file selector, allows selection of some
	// kind of image formats
	FileSelector fs;
	fs.ActiveDir(lastPath);
	fs.Type(t_("Image files"), "*.png,*.jpg,*.ico,*.bmp");
	if(!fs.ExecuteOpen(t_("Select icon file")))
		return;
	String path = fs.Get();
	lastPath = GetFileFolder(path);
	String ext = ToUpper(GetFileExt(path));
	Image img;
	if(ext != ".ICO")
		img = StreamRaster::LoadFileAny(path);
	else
	{
		String data = LoadFile(path);
		Vector<Image> imgVec;
		try
		{
		 	imgVec = ReadIcon(data);
		}
		catch(...)
		{
		}
		if(imgVec.GetCount())
			img = imgVec[0];
		else
			img = Null;
	}
	curIcon = img;
	itemPane.icon.SetImage(img);
	fieldsModCb();
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
	
	barListPane.barList.WhenSel << THISBACK(barSelCb);
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

// sets the local copy of toolbars
void XMLBarsEditor::SetToolBars(XMLToolBars const &tb)
{
	toolBars <<= tb;
	barListPane.barList.Clear();
	for(int iBar = 0; iBar < toolBars.GetCount(); iBar++)
		barListPane.barList.Add(toolBars[iBar].GetName());
	
	// select first item
	if(barListPane.barList.GetCount())
		barListPane.barList.SetCursor(0);
}

// gets the local copy of toolbars
XMLToolBars &XMLBarsEditor::GetToolBars(void)
{
	barEditor.RefreshBar();
	for(int i = 0; i < toolBars.GetCount(); i++)
		toolBars.SetKey(i, toolBars[i].GetName());
	return toolBars;
}

// bar selection callback
void XMLBarsEditor::barSelCb(void)
{
	
	int idx = barListPane.barList.GetCursor();
	if(idx < 0)
		barEditor.SetBar(NULL);
	else
		barEditor.SetBar(&toolBars[idx]);
}
	
////////////////////////////////////////////////////////////////////////////////
void XMLMenuEditor::cancelCb(void)
{
	Break(IDCANCEL);
	Close();
}

void XMLMenuEditor::okCb(void)
{
	// updates edited toolbars
	if(iFace)
	{
		iFace->SetCommands(commands);
		iFace->SetMenuBars(menusEditor.GetToolBars());
		iFace->SetToolBars(barsEditor.GetToolBars());
	}
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
	commands <<= iFace->GetCommands();
	commands.Sort();
	Vector<String> const &ids = commands.GetIds();
	cmdPane.commandList.Clear();
	for(int i = 0; i < ids.GetCount(); i++)
	{
		// built in commands (un-modifiable) are in red color
		// custom ones in normal color
		XMLCommand const &cmd = commands[i];
		if(cmd.GetIsCustom())
			cmdPane.commandList.Add(ids[i]);
		else
			cmdPane.commandList.Add(ids[i], RedDisp);
	}
	
	// sets toolbars in bars editors
	menusEditor.SetToolBars(iFace->GetMenuBars());
	barsEditor.SetToolBars(iFace->GetToolBars());
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
		
END_UPP_NAMESPACE