#include "ide.h"

void Ide::TabFile()
{
	int q = tabs.GetCursor();
	if(q >= 0)
		EditFile(tabs.GetFile(q));
}

void Ide::DTabFile()
{
	int q = dtabs.GetCursor();
	if(q >= 0)
		EditFile(dtabs.GetFile(q));
}

void Ide::TabFile2()
{
	int q = tabs2.GetCursor();
	if(q >= 0) {
		String fn = tabs2.GetFile(q);
		SwapEditors();
		EditFile(fn);
	}
}

void Ide::ClearTab()
{
	if(designer) {
		int c = dtabs.GetCursor();
		if(c >= 0)
			dtabs.Close(c);
		tabs.Set(dtabs);
	}
	else {
		int c = tabs.GetCursor();
		if(c >= 0)
			tabs.Close(c);
	}
	tabs2.Set(tabs);
}

void Ide::ClearTabs()
{
	dtabs.Clear();
	tabs.Clear();
	FileSelected();
}

void Ide::TabsLR(int d)
{
	QuickTabs& t = designer ? dtabs : tabs;
	int c = t.GetCursor();
	if(c < 0 || t.GetCount() <= 1)
		return;
	c = minmax(c + d, 0, t.GetCount() - 1);
	EditFile(t.GetFile(c));
}

void Ide::FileSelected()
{
	if(!IsNull(editfile))
		(designer ? dtabs : tabs).SetAddFile(editfile);
}
