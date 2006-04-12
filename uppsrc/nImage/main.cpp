#include "Image.h"
#include "Raster.h"

#define IMAGECLASS TestImg
#define IMAGEFILE <CtrlLib/Ctrl.iml>
#include <Draw/iml.h>

struct App : public TopWindow {
	Vector<Point> list;
	Vector<Point> fblist;
	EditIntSpin   nx, ny;
	SliderCtrl    sx, sy, val;

	virtual void Paint(Draw& draw);
	virtual void Paint2(Draw& draw);
	virtual void LeftDown(Point p, dword);
	virtual void RightDown(Point p, dword);

	void Tst();
	void Refr();
	void VSlider();
	void HSlider();

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
		t += b.GetSize().cx;
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
	static Image img = CreateText("S", Arial(20).Bold(), Green, Yellow);
//	static Image img = CreateBall(10, LtBlue);
//	static Image img = CreateText("Just a little test", Roman(40).Italic(), Red, LtBlue);
	static HICON icon = IconWin32(img, true);
	::SetCursor(icon);
//	DestroyIcon(icon);
//	::SetCursor(LoadCursor(0, IDC_ARROW));
}

void App::Refr()
{
	Refresh();
}

void App::VSlider()
{
	ny <<= ~sy;
	Refr();
}

void App::HSlider()
{
	nx <<= ~sx;
	Refr();
}

App::App()
{
//	SetTimeCallback(-5, THISBACK(Tst), 33);
	Sizeable().Zoomable();
	Add(nx.LeftPos(10, 50).TopPos(10, Ctrl::STDSIZE));
	Add(ny.LeftPos(70, 50).TopPos(10, Ctrl::STDSIZE));
	nx <<= ny <<= THISBACK(Refr);
	nx <<= 100;
	ny <<= 100;
	Add(sx.BottomPos(0, 20).HSizePos());
	sx.Range(1000);
	sx <<= THISBACK(HSlider);
	Add(sy.RightPos(0, 20).VSizePos());
	sy.Range(1000);
	sy <<= THISBACK(VSlider);
	val.Range(2000);
	val <<= THISBACK(Refr);
	val <<= 0;
	Add(val.LeftPos(0, 20).VSizePos(40, 40));
	BackPaint();
}

void App::Paint2(Draw& w)
{
	FindFile ff("d:\\ImgTest\\*.*");
	Size sz = GetSize();
	w.DrawRect(GetSize(), SLtGray);
	int x = 0;
	int y = 0;
	int mcy = 0;
	BMPRaster bmp;
	while(ff) {
		if(ff.IsFile() && ff.GetLength() < 1024*1024*10) {
			Image img = bmp.LoadFile(AppendFileName("d:\\ImgTest", ff.GetName()));
	//		Image img = bmp.LoadFile(AppendFileName("d:\\ImgTest", "test32v5.bmp"));
			Size isz = img.GetSize();
			if(IsNull(img)) {
				isz.cy = Draw::GetStdFontCy();
				isz.cx = 12 * isz.cy;
			}
			if(x + isz.cx > sz.cx) {
				x = 0;
				y += mcy;
				mcy = isz.cy;
			}
			else
				mcy = max(mcy, isz.cy);
			if(IsNull(img))
				w.DrawRect(x, y, isz.cx, isz.cy, LtRed);
			img.Paint(w, x, y);
			w.DrawText(x, y, ff.GetName(), StdFont(), White);
			w.DrawText(x+2, y+2, ff.GetName(), StdFont(), White);
			w.DrawText(x+2, y+0, ff.GetName(), StdFont(), White);
			w.DrawText(x+0, y+2, ff.GetName(), StdFont(), White);
			w.DrawText(x+1, y+1, ff.GetName());
			x += isz.cx;
		}
		ff.Next();
	}
}

void App::Paint(Draw& w)
{
	Size sz = GetSize();
	w.DrawRect(GetSize(), SLtGray);
	ImageBuffer b(100, 100);
	DrawRect(b, 0, 0, 100, 100, SBlue);
	DrawRect(b, 0, 0, 20, 20, Red);
	Image img = b;
	img.Paint(w, 0, 0);
	img.Paint(w, 200, 50);
	img.Paint(w, 250, 70, 100);
	img.Paint(w, 300, 250, 100);
	ImageBuffer b1(60, 60);

	img = CreateText("Just a little test", Roman(40).Italic(), Red, LtBlue);
	img.Paint(w, 300, 300);

	img = CreateBall(80, Red);
	for(int i = 0; i < list.GetCount(); i++)
		img.Paint(w, list[i].x, list[i].y, 255, Blue);
//	img = CreateText("Hello world!", Roman(30).Italic().Underline(), SYellow, SMagenta);
	for(int i = 0; i < fblist.GetCount(); i++)
		img.Paint(w, fblist[i].x, fblist[i].y, (i * 40 + 55) % 255, Null);

//	img = Win32Icon(IDI_INFORMATION);
	img = Win32Icon(IDC_ARROW, true);
	Image img2 = Win32Icon(IDI_EXCLAMATION);
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

//	static Image h = Rescale(, 48, 48);
//	img2 = TestImg::exclamation();
	int v = int(~val) - 1000;
//	EmbossTh(Rescale(img2, ~nx, ~ny), SColorFace, SColorLight(), SColorShadow(), v)
	BMPRaster jimg;
	FileIn in("d:/imgtest/large.bmp");
	jimg.Open(in);
	ImageEncoder tgt;
	tgt.Create(~nx, ~ny);
	Rescale(tgt, jimg, jimg.GetSize());
	Sharpen(tgt, v)

//	Emboss(Rescale(jimg, ~nx, ~ny), SColorFace, v)
		.Paint(w, (sz.cx - (int)~nx) / 2, (sz.cy - (int)~ny) / 2);

	w.DrawText(200, 0, AsString(v));
/*
	PixelArray a = ImageToPixelArray(CtrlImg::exclamation());
	PixelArray dest(Size(~nx, ~ny));
	PixelCopyAntiAlias(dest, Size(~nx, ~ny), a, a.GetSize());
	w.DrawImage((sz.cx - (int)~nx) / 2, (sz.cy - (int)~ny) / 2, PixelArrayToImage(dest));
*/
	TestImg::exclamation().Paint(w, 200, 200);
/*
	LoadBMP("d:\\test.bmp").Paint(w, 500, 100);
	LoadBMP("d:\\test256.bmp").Paint(w, 500, 250);
	LoadBMP("d:\\test16.bmp").Paint(w, 500, 400);
	BmpLoader::LoadFile("d:\\test2.bmp").Paint(w, 500, 550);
	LoadICO("d:\\test.ico").Paint(w, 500, 700);
	LoadICO("d:\\theide.ico").Paint(w, 500, 850);
*/
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

#include <plugin/z/z.h>

void TestSize(const Image& img)
{
	int len = img.GetLength() * 4;
	DUMP(len);
	DUMP(ZCompress(String((const char *)~img, len)).GetLength());
}

GUI_APP_MAIN
{
//	for(int i = 0; i < 1000; i++)
//		restest.Add(CreateBall(i / 10 + 10, Color(i, 200 - i, i / 50)));
/*
	DUMP(Magenta());

	TestSize(Win32Icon(IDI_EXCLAMATION));
	TestSize(TestImg::exclamation());
	TestSize(TestImg::paste());
*/
	for(int i = 0; i < 50; i++)
	{
		RTIMING("BMPTEST");
		BMPEncoder out;
		BMPRaster  bmp;
		FileIn in("d:/imgtest/jachym.bmp");
		bmp.Open(in);
		static int bpp[] = { 1, 4, 8, 16, 24, 32 };
		for(int g = 0; g < 2; g++) {
			out.grayscale = g;
			for(int i = 0; i < __countof(bpp); i++) {
				out.bpp = bpp[i];
				String name = "d:/test";
				name << out.bpp;
				if(out.grayscale)
					name << "g";
				out.SaveFile(name + ".bmp", bmp);
			}
		}
/*
		for(int g = 0; g < 2; g++) {
			out.grayscale = g;
			for(int i = 0; i < __countof(bpp); i++) {
				out.bpp = bpp[i];
				String name = "d:/stest";
				name << out.bpp;
				if(out.grayscale)
					name << "g";
				FileOut f(name + ".bmp");
				out.SetStream(f);
				out.Create(4000, 4000, bmp);
				Rescale(out, bmp, bmp.GetSize());
			}
		}*/
	}

//	img = LoadBMP("d:/imgtest/jachym.bmp");
//	BMPEncoder e;
/*	e.SaveFile("d:/test.bmp", img);
	e.bpp = 8;
	e.SaveFile("d:/test8.bmp", img);
	e.grayscale = true;
	e.SaveFile("d:/test8g.bmp", img);
*/
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
