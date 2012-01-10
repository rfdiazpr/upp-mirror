#include "ScatterCtrl_Demo.h"

#include <Report/Report.h>
#include <PdfDraw/PdfDraw.h>

#define IMAGECLASS MyImages
#define IMAGEFILE <ScatterCtrl_Demo/ScatterCtrl_Demo.iml>
#include <Draw/iml_source.h>


struct Example {
	One <ScatterDemo> ctrl;
	String name;
};

Array<Example>& Examples()
{
	static Array<Example> x;
	return x;
};

void RegisterExample(const char *name, ScatterDemo *ctrl)
{
	Example& x = Examples().Add();
	x.name = name;
	x.ctrl = ctrl;
}

	
GUI_APP_MAIN
{
	RegisterExample("Basic", new Tab1);
	RegisterExample("Functions", new Tab2);
	RegisterExample("Parametric Functions", new Tab3);
	RegisterExample("Formatting", new Tab4);
	RegisterExample("Dynamic", new Tab5);
	RegisterExample("Log", new Tab6);
	RegisterExample("Operations", new Tab7);	
	RegisterExample("Secondary Y", new Tab8);
	RegisterExample("Big dataset", new Tab9);
	
	ScatterCtrl_Demo().Run();
}

ScatterCtrl_Demo::ScatterCtrl_Demo()
{
	CtrlLayout(*this, "Scatter Test");
	
	for (int i = 0; i < Examples().GetCount(); ++i)
		tab.Add(*(Examples()[i].ctrl), Examples()[i].name);

	bPreview <<= THISBACK(Preview);
	bSavePNG <<= THISBACK(SavePNG);
	bSaveJPG <<= THISBACK(SaveJPG);
#ifdef PLATFORM_WIN32
	bSaveEMF <<= THISBACK(SaveEMF);
#endif
	bCopyClipboard <<= THISBACK(CopyClipboard);
	
	paintMode.Add(ScatterDraw::MD_ANTIALIASED, "Painter Antialiased")
			 .Add(ScatterDraw::MD_NOAA, 	   "Painter No-Antialiased")
			 .Add(ScatterDraw::MD_SUBPIXEL,    "Painter Subpixel")
			 .Add(ScatterDraw::MD_DRAW, 	   "Draw");

	paintMode <<= THISBACK(SetMode);
	paintMode.SetData(ScatterDraw::MD_ANTIALIASED);
	SetMode();

	Sizeable().Zoomable().Icon(MyImages::i1());
}

void ScatterCtrl_Demo::Preview()
{
	Report r;	
	
	const Drawing &w = Examples()[tab.Get()].ctrl->Scatter().GetDrawing();
	r.DrawDrawing(300, 300, w.GetSize().cx, w.GetSize().cy, w);

	Perform(r);
}

void ScatterCtrl_Demo::SavePNG()
{
	int ntab = tab.Get();	
	
	Examples()[ntab].ctrl->Scatter().SaveToImage(Format("scatter%d.png", ntab));	
}

void ScatterCtrl_Demo::SaveJPG()
{
	int ntab = tab.Get();	
	
	Examples()[ntab].ctrl->Scatter().SaveToImage(Format("scatter%d.jpg", ntab));				
}

#ifdef PLATFORM_WIN32
void ScatterCtrl_Demo::SaveEMF()
{
	int ntab = tab.Get();	
	
	Examples()[ntab].ctrl->Scatter().SaveAsMetafile(Format("scatter%d.emf", ntab));				
}
#endif

void ScatterCtrl_Demo::CopyClipboard()
{
	Examples()[tab.Get()].ctrl->Scatter().SaveToClipboard();	
}

void ScatterCtrl_Demo::SetMode()
{
	for (int i = 0; i < Examples().GetCount(); ++i) 
		Examples()[i].ctrl->Scatter().SetMode(~paintMode);
}

