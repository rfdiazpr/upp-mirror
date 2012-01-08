#include "ScatterCtrl_Demo.h"

#include <Report/Report.h>
#include <PdfDraw/PdfDraw.h>

#define IMAGECLASS MyImages
#define IMAGEFILE <ScatterCtrl_Demo/ScatterCtrl_Demo.iml>
#include <Draw/iml_source.h>


int kkmain(const String &s);

GUI_APP_MAIN
{
	ScatterCtrl_Demo().Run();
}

ScatterCtrl_Demo::ScatterCtrl_Demo()
{
	CtrlLayout(*this, "Scatter Test");
	
	tab.Add(tab1, "Basic");		
	tab.Add(tab2, "Functions");
	tab.Add(tab3, "Parametric Functions");
	tab.Add(tab4, "Formatting");
	tab.Add(tab5, "Dynamic");
	tab.Add(tab6, "Log");
	tab.Add(tab7, "Operations");	
	tab.Add(tab8, "Secondary Y");
	tab.Add(tab9, "Big dataset");
	
	bPreview <<= THISBACK(Preview);
	bSavePNG <<= THISBACK(SavePNG);
	bSaveJPG <<= THISBACK(SaveJPG);
#ifdef PLATFORM_WIN32
	bSaveEMF <<= THISBACK(SaveEMF);
#endif
	bCopyClipboard <<= THISBACK(CopyClipboard);
	
	paintMode.Add(ScatterDraw::MD_ANTIALIASED, "Painter Antialiased")
			 .Add(ScatterDraw::MD_NOAA, "Painter No-Antialiased")
			 .Add(ScatterDraw::MD_SUBPIXEL, "Painter Subpixel")
			 .Add(ScatterDraw::MD_DRAW, "Draw");
	paintMode <<= THISBACK(SetMode);
	paintMode.SetData(ScatterDraw::MD_ANTIALIASED);
	SetMode();

	Sizeable().Zoomable().Icon(MyImages::i1());
}

void ScatterCtrl_Demo::Preview()
{
	Report r;	
	int ntab = tab.Get();
	
	switch (ntab) {
		case 0 :	
			{
				const Drawing &w = tab1.scatter.GetDrawing();
				r.DrawDrawing(300, 300, w.GetSize().cx, w.GetSize().cy, w);
				break;	
			}
		case 1:
			{
				const Drawing &w = tab1.scatter.GetDrawing();
				r.DrawDrawing(300, 300, w.GetSize().cx, w.GetSize().cy, w);
				break;	
			}
		case 2:
			{
				const Drawing &w = tab2.scatter.GetDrawing();
				r.DrawDrawing(300, 300, w.GetSize().cx, w.GetSize().cy, w);
				break;	
			}
		case 3:
			{
				const Drawing &w = tab3.scatter.GetDrawing();
				r.DrawDrawing(300, 300, w.GetSize().cx, w.GetSize().cy, w);
				break;	
			}
		case 4:
			{
				const Drawing &w = tab4.scatter.GetDrawing();
				r.DrawDrawing(300, 300, w.GetSize().cx, w.GetSize().cy, w);
				break;	
			}
		case 5:
			{
				const Drawing &w = tab5.scatter.GetDrawing();
				r.DrawDrawing(300, 300, w.GetSize().cx, w.GetSize().cy, w);
				break;	
			}
		case 6:
			{
				const Drawing &w = tab6.scatter.GetDrawing();
				r.DrawDrawing(300, 300, w.GetSize().cx, w.GetSize().cy, w);
				break;	
			}						
		case 7:
			{
				const Drawing &w = tab7.scatter.GetDrawing();
				r.DrawDrawing(300, 300, w.GetSize().cx, w.GetSize().cy, w);
				break;	
			}					
		case 8:
			{
				const Drawing &w = tab8.scatter.GetDrawing();
				r.DrawDrawing(300, 300, w.GetSize().cx, w.GetSize().cy, w);
				break;	
			}				
	}
	Perform(r);
}

void ScatterCtrl_Demo::SavePNG()
{
	int ntab = tab.Get();	
	
	switch (ntab) {
		case 0:
			tab1.scatter.SaveToImage("scatter1.png");
			break;
		case 1:	
			tab2.scatter.SaveToImage("scatter2.png");
			break;
		case 2:
			tab3.scatter.SaveToImage("scatter3.png");
			break;
		case 3:
			tab4.scatter.SaveToImage("scatter4.png");
			break;
		case 4:
			tab5.scatter.SaveToImage("scatter5.png");
			break;
		case 5:
			tab6.scatter.SaveToImage("scatter6.png");
			break;
		case 6:
			tab7.scatter.SaveToImage("scatter7.png");
			break;				
		case 7:
			tab8.scatter.SaveToImage("scatter8.png");
			break;
		case 8:
			tab9.scatter.SaveToImage("scatter9.png");
			break;
	}			
}

void ScatterCtrl_Demo::SaveJPG()
{
	int ntab = tab.Get();	
	
	switch (ntab) {
		case 0:
			tab1.scatter.SaveToImage("scatter1.jpg");
			break;
		case 1:
			tab2.scatter.SaveToImage("scatter2.jpg");
			break;
		case 2:
			tab3.scatter.SaveToImage("scatter3.jpg");
			break;
		case 3:
			tab4.scatter.SaveToImage("scatter4.jpg");
			break;
		case 4:
			tab5.scatter.SaveToImage("scatter5.jpg");
			break;
		case 5:
			tab6.scatter.SaveToImage("scatter6.jpg");
			break;
		case 6:
			tab7.scatter.SaveToImage("scatter7.jpg");
			break;				
		case 7:
			tab8.scatter.SaveToImage("scatter8.jpg");
			break;		
		case 8:
			tab9.scatter.SaveToImage("scatter9.jpg");
			break;		
	}				
}

#ifdef PLATFORM_WIN32
void ScatterCtrl_Demo::SaveEMF()
{
	int ntab = tab.Get();	
	
	switch (ntab) {
		case 0:
			tab1.scatter.SaveAsMetafile("scatter1.emf");
			break;
		case 1:
			tab2.scatter.SaveAsMetafile("scatter2.emf");
			break;
		case 2:
			tab3.scatter.SaveAsMetafile("scatter3.emf");
			break;
		case 3:
			tab4.scatter.SaveAsMetafile("scatter4.emf");
			break;
		case 4:
			tab5.scatter.SaveAsMetafile("scatter5.emf");
			break;
		case 5:
			tab6.scatter.SaveAsMetafile("scatter6.emf");
			break;
		case 6:
			tab7.scatter.SaveAsMetafile("scatter7.emf");
			break;				
		case 7:
			tab8.scatter.SaveAsMetafile("scatter8.emf");
			break;			
		case 8:
			tab9.scatter.SaveAsMetafile("scatter9.emf");
			break;			
	}	
}
#endif

void ScatterCtrl_Demo::CopyClipboard()
{
	int ntab = tab.Get();	
	
	switch (ntab) {
		case 0:
			tab1.scatter.SaveToClipboard();
			break;
		case 1:
			tab2.scatter.SaveToClipboard();
			break;
		case 2:
			tab3.scatter.SaveToClipboard();
			break;
		case 3:
			tab4.scatter.SaveToClipboard();
			break;
		case 4:
			tab5.scatter.SaveToClipboard();
			break;
		case 5:
			tab6.scatter.SaveToClipboard();
			break;
		case 6:
			tab7.scatter.SaveToClipboard();
			break;				
		case 7:
			tab8.scatter.SaveToClipboard();
			break;
		case 8:
			tab9.scatter.SaveToClipboard();
			break;			
	}		
}

void ScatterCtrl_Demo::SetMode()
{
	tab1.scatter.SetMode(~paintMode);
	tab2.scatter.SetMode(~paintMode);
	tab3.scatter.SetMode(~paintMode);
	tab4.scatter.SetMode(~paintMode);
	tab5.scatter.SetMode(~paintMode);
	tab6.scatter.SetMode(~paintMode);
	tab7.scatter.SetMode(~paintMode);
	tab8.scatter.SetMode(~paintMode);
	tab9.scatter.SetMode(~paintMode);
}

