#include "Image.h"

struct App : public TopWindow {
	Vector<Point> list;
	Vector<Point> fblist;

	virtual void Paint(Draw& draw);
	virtual void LeftDown(Point p, dword);
	virtual void RightDown(Point p, dword);

	void Tst();

	typedef App CLASSNAME;

	App();
};

void DrawRect(ImageBuffer& b, int x, int y, int cx, int cy, Color color, int alpha = 255)
{
	RGBA ba;
	ba.a = alpha;
	ba.b = color.GetB();
	ba.r = color.GetR();
	ba.g = color.GetG();
	RGBA *t = b[y] + x;
	for(int i = 0; i < cy; i++) {
		Fill(t, t + cx, ba);
		t += b.GetLineDelta();
	}
}

void App::LeftDown(Point p, dword)
{
	list.Add(p);
	Refresh();
}

void App::RightDown(Point p, dword)
{
	fblist.Add(p);
	Refresh();
}


Image CreateText(const char *text, Font font, Color color1, Color color2)
{
	Size sz = ScreenInfo().GetTextSize(text, font);
	ImageDraw w(sz);
/*
	w.alpha.DrawRect(0, 0, sz.cx, sz.cy, GrayColor(255));
	w.DrawRect(0, 0, sz.cx, sz.cy, GrayColor(255));
	w.DrawText(0, 0, text, font, GrayColor(0));
	return w;
//*/
//*
	LOG("DrawText");
	w.alpha.DrawRect(0, 0, sz.cx, sz.cy, GrayColor(0));
	w.alpha.DrawText(0, 0, text, font, GrayColor(255));
	ImageBuffer b(w);
	for(int y = 0; y < sz.cy; y++)
		for(int x = 0; x < sz.cx; x++) {
			RGBA& a = b[y][x];
			Color c = Blend(color1, color2, 255 * (x + y) / (sz.cx + sz.cy));
			a.r = c.GetR();
			a.g = c.GetG();
			a.b = c.GetB();
	//		a.r = a.g = a.b = 0;
	//		a.a = 255 - a.a;
		}
	return b;
//*/
}

Image CreateBall(int r, Color color)
{
	int rr = 2 * r;
	int r2 = r * r;
	ImageBuffer b(rr, rr);
	b.SetHotSpot(Point(4, 4));
	for(int y = 0; y < rr; y++)
		for(int x = 0; x < rr; x++) {
			RGBA& a = b[y][x];
			a.r = color.GetR();
			a.g = color.GetG();
			a.b = color.GetB();
			int q = ((x - r) * (x - r) + (y - r) * (y - r)) * 256 / r2;
			a.a = q <= 255 ? q : 0;
	/*		if(a.a > 200)
				a.a = 255;
			else
				a.a = 0;*/
		}
	return b;
}

Vector<Image> restest;

void App::Tst()
{
	static Size cursor_size(GetSystemMetrics(SM_CXCURSOR), GetSystemMetrics(SM_CYCURSOR));
	static Image img = CreateBall(cursor_size.cx / 2, Magenta);
	static HICON icon = IconWin32(img, true);
	::SetCursor(icon);
//	DestroyIcon(icon);
//	::SetCursor(LoadCursor(0, IDC_ARROW));
}

App::App()
{
	SetTimeCallback(-5, THISBACK(Tst), 33);
	Sizeable().Zoomable();
}

void App::Paint(Draw& w)
{
	w.DrawRect(GetSize(), SWhite);
	ImageBuffer b(100, 100);
	DrawRect(b, 0, 0, 100, 100, SBlue);
	DrawRect(b, 0, 0, 20, 20, Red);
	Image img = b;
	img.Paint(w, 0, 0);
	img.Paint(w, 200, 50);
	img.Paint(w, 250, 70, 100);
	img.Paint(w, 300, 250, 100);
	ImageBuffer b1(60, 60);

	img = CreateText("Just a little test", Roman(40).Italic(), Black, Black);
	img.Paint(w, 300, 300);
	w.DrawText(300, 340, "Just a little test", Roman(40).Italic(), Black);
	w.DrawText(301, 380, "Just a little test", Roman(40).Italic(), Black);
	w.DrawText(302, 420, "Just a little test", Roman(40).Italic(), Black);

	img = CreateBall(80, Red);
	for(int i = 0; i < list.GetCount(); i++)
		img.Paint(w, list[i].x, list[i].y, /*(i * 40 + 55) % */255, Blue);
//	img = CreateText("Hello world!", Roman(30).Italic().Underline(), SYellow, SMagenta);
	for(int i = 0; i < fblist.GetCount(); i++)
		img.Paint(w, fblist[i].x, fblist[i].y, (i * 40 + 55) % 255, Null);

//	img = Win32Icon(IDI_INFORMATION);
	img = Win32Icon(IDC_ARROW, true);
	Image img2 = Win32Icon(IDI_INFORMATION);
	Image img3 = Win32Icon(IDC_IBEAM, true);
	w.DrawRect(0, 400, 160, 40, SCyan);
	DrawIcon(w.GetHandle(), 0, 400, LoadIcon(0, IDI_INFORMATION));
	img.Paint(w, 80, 400);
	img2.Paint(w, 40, 400);
	img3.Paint(w, 100, 400);
	DrawIcon(w.GetHandle(), 0, 450, LoadIcon(0, IDI_INFORMATION));
	img.Paint(w, 80, 450);
	img2.Paint(w, 40, 450);
	img3.Paint(w, 100, 450);

	for(int i = 0; i < 20; i++) {
		Image bimg = CreateBall(10 + i, Cyan);
		HICON icon = IconWin32(bimg, false);
		DrawIcon(w.GetHandle(), 500 + 32 * i, 0, icon);
		DestroyIcon(icon);
		bimg.Paint(w, 500 + 32 * i, 50);
	}

	HICON icon = IconWin32(img2);
	DrawIcon(w.GetHandle(), 500, 100, icon);
	DestroyIcon(icon);

//	DrawIcon(w.GetHandle(), 40, 400, LoadCursor(0, IDC_WAIT));

//	DrawIconEx(w.GetHandle(), 0, 400, LoadCursor(0, IDC_ARROW), 0, 0, 0, NULL, 0x0010 | DI_NORMAL | DI_DEFAULTSIZE);;



//	for(int i = 0; i < 100; i++)
//		restest[i].Paint(w, i, 300);

/*
	for(int i = 0; i < 1000; i++) {
		RTIMING("AlphaBlend");
		img.Paint(w, 0, 0, false);
	}
	w.DrawRect(600, 10, 20, 20, SGray);
	for(int i = 0; i < 1000; i++) {
		RTIMING("Fallback");
		img.Paint(w, 0, 0, true);
	}
	w.DrawRect(600, 10, 20, 20, SCyan);
*/
}

#include <commdlg.h>

GUI_APP_MAIN
{
//	for(int i = 0; i < 1000; i++)
//		restest.Add(CreateBall(i / 10 + 10, Color(i, 200 - i, i / 50)));

	DUMP(Magenta());

	App app;
	app.Run();
/*
	PRINTDLG dlg;
	memset(&dlg, 0, sizeof(dlg));
	dlg.lStructSize = sizeof(dlg);
	dlg.hwndOwner = NULL;
	dlg.Flags = PD_ALLPAGES|PD_DISABLEPRINTTOFILE|PD_NOSELECTION|PD_RETURNDC|
		        PD_HIDEPRINTTOFILE;
	dlg.nFromPage = 1;
	dlg.nToPage = 1;
	dlg.nMinPage = 1;
	dlg.nMaxPage = 1;
	if(!PrintDlg(&dlg)) return;
	if(!(dlg.Flags & PD_PAGENUMS)) {
		dlg.nFromPage = dlg.nMinPage;
		dlg.nToPage = dlg.nMaxPage;
	}
	PrintDraw w(dlg.hDC, "");
	w.StartPage();
	w.DrawRect(0, 0, 3000, 3000, GrayColor(80));
	CreateBall(1000, Black).Paint(w, 0, 0, true);
	w.EndPage();*/
}
