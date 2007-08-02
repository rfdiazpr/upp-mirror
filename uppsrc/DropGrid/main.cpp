#include <CtrlLib/CtrlLib.h>
#include <GridCtrl/GridCtrl.h>
#include "DropGrid.h"

using namespace Upp;

struct App : public TopWindow
{
	DropGrid drop;
	DropList classic_drop;
	
	App()
	{
		//CtrlLayout(*this, "DropGrid");
		Sizeable().Zoomable();
		Title("DropGrid");
		
		Add(drop.LeftPos(20, 150).TopPos(20, 19));
		drop.AddColumn("ID");
		drop.AddColumn("Value");
		drop.AddColumn("Description");
		for(int i = 0; i < 20; i++)
			drop.Add(i, Format("Hello %d", i), "How are you now?");

		drop.Width(300);
		drop.DisplayAll(0);/*NoHeader(0)*///NotNull();
		drop.ColorRows();
		drop.NotNull();
		drop.SetDropLines(15);
		drop.SetValueColumn(1);
		drop.AddValueColumn(0).AddValueColumn(1);
		Add(classic_drop.LeftPos(20, 150).TopPos(60, 19));
		classic_drop.Add("ALA").Add("ELA").Add("BABA");
	}
};

#ifdef flagMAIN

GUI_APP_MAIN
{
	App().Run();
}

#endif
