#include <CtrlLib/CtrlLib.h>

void DrawCells(Draw& w, int x, int y, const char *text, Font f)
{
	FontInfo fi = w.GetFontInfo(f);
	const char *s = text;
	int a = 0;
	while(*s) {
		int wd = fi[*s];
/*		int l = fi.GetLeftSpace(*s);
		int r = fi.GetRightSpace(*s);
		LOG(String(s, 1) << " wd:" << wd << " l:" << l << " r:" << r);
		w.DrawRect(a, y, l, fi.GetHeight(), SLtCyan);
		w.DrawRect(a + wd - r, y, r, fi.GetHeight(), SLtCyan);
		DrawFrame(w, a, y, wd, fi.GetHeight(), SBlack);
		a += wd;
*/		s++;
	}
	w.DrawText(x, y, text, f);
}

struct Test : TopWindow {
	virtual void Paint(Draw& w)
	{
		Size sz = GetSize();
		w.DrawRect(0, 0, sz.cx, sz.cy, White);
		DrawCells(w, 0, 0, "Hello world of X11 programming !", Arial(50));
		DrawCells(w, 0, 80, "Hello world of X11 programming !", Roman(50).Underline());
		DrawCells(w, 0, 160, "Hello world of X11 programming !", Courier(50));
		DrawCells(w, 0, 240, "Tohle by mìlo být hezky pìknì èesky", Arial(50));
		DrawCells(w, 0, 320, "Tohle by mìlo být hezky pìknì èesky", Roman(50));
		DrawCells(w, 0, 400, "Tohle by mìlo být hezky pìknì èesky", Courier(50));
		DrawCells(w, 0, 460, "Hello world of X11 programming !", ScreenSans(50));
		DrawCells(w, 0, 520, "Hello world of X11 programming !", ScreenSerif(50));
		for(int i = 0; i <= 3600; i += 200)
			w.DrawText(300, 300, i, "Angle !", Arial(50));
		w.DrawText(400, 300, 300, "Angle !", ScreenSans(50).Underline());
	}
};

/*	LOG("--- Scaleable fonts ------------------------");
	for(int i = 0; i < Font::GetFaceCount(); i++)
		if(Font::GetFaceInfo(i) & Font::SCALEABLE)
			LOG(Font::GetFaceName(i));
	LOG("--- Non-scaleable fonts ------------------------");
	for(int i = 0; i < Font::GetFaceCount(); i++)
		if(!(Font::GetFaceInfo(i) & Font::SCALEABLE))
			LOG(Font::GetFaceName(i));*/
//	SetDefaultCharset(CHARSET_WIN1250);
//	Test().Run();
