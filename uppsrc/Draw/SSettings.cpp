#include "Draw.h"

CH_COLOR(SBlack, Black());
CH_COLOR(SGray, Gray());
CH_COLOR(SLtGray, LtGray());
CH_COLOR(SWhiteGray, WhiteGray());
CH_COLOR(SWhite, White());
CH_COLOR(SRed, Red());
CH_COLOR(SGreen, Green());
CH_COLOR(SBrown, Brown());
CH_COLOR(SBlue, Blue());
CH_COLOR(SMagenta, Magenta());
CH_COLOR(SCyan, Cyan());
CH_COLOR(SYellow, Yellow());
CH_COLOR(SLtRed, LtRed());
CH_COLOR(SLtGreen, LtGreen());
CH_COLOR(SLtYellow, LtYellow());
CH_COLOR(SLtBlue, LtBlue());
CH_COLOR(SLtMagenta, LtMagenta());
CH_COLOR(SLtCyan, LtCyan());

CH_COLOR(SColorPaper, White());
CH_COLOR(SColorFace, LtGray());
CH_COLOR(SColorText, Black());
CH_COLOR(SColorHighlight, Blue());
CH_COLOR(SColorHighlightText, White());
CH_COLOR(SColorMenu, LtGray());
CH_COLOR(SColorMenuText, Black());
CH_COLOR(SColorInfo, LtYellow());
CH_COLOR(SColorInfoText, Black());
CH_COLOR(SColorDisabled, Gray());
CH_COLOR(SColorLight, White());
CH_COLOR(SColorShadow, Gray());

#ifdef PLATFORM_WIN32
void UpdateSColors()
{
	dword c;
	c = GetSysColor(COLOR_WINDOWTEXT);
	int r0 = GetRValue(c);
	int g0 = GetGValue(c);
	int b0 = GetBValue(c);
	ChSet("SBlack", Color(r0, g0, b0));
	c = GetSysColor(COLOR_3DSHADOW);
	int r128 = GetRValue(c);
	int g128 = GetGValue(c);
	int b128 = GetBValue(c);
	ChSet("SGray", Color(r128, g128, b128));
	c = GetSysColor(COLOR_3DFACE);
	int r192 = GetRValue(c);
	int g192 = GetGValue(c);
	int b192 = GetBValue(c);
	ChSet("SLtGray", Color(r192, g192, b192));
	c = GetSysColor(COLOR_WINDOW);
	int r255 = GetRValue(c);
	int g255 = GetGValue(c);
	int b255 = GetBValue(c);
	ChSet("SWhite", Color(r255, g255, b255));
	int r224 = (r255 * 7 + r0) >> 3;
	int g224 = (r255 * 7 + r0) >> 3;
	int b224 = (r255 * 7 + r0) >> 3;

	ChSet("SWhiteGray", Color(r224, g224, b224));
	ChSet("SBlue", Color(r0, g0, b128));
	ChSet("SRed", Color(r128, g0, b0));
	ChSet("SGreen", Color(0, g128, b0));
	ChSet("SBrown", Color(r128, g128, b0));
	ChSet("SMagenta", Color(r128, g0, b255));
	ChSet("SCyan", Color(r0, g128, b128));
	ChSet("SYellow", Color(r255, g255, b0));
	ChSet("SLtRed", Color(r255, g0, b0));
	ChSet("SLtGreen", Color(r0, g255, b0));
	ChSet("SLtYellow", Color(r255, g255, b192));
	ChSet("SLtBlue", Color(0, 0, b255));
	ChSet("SLtMagenta", Color(r255, g0, b255));
	ChSet("SLtCyan", Color(r0, g255, b255));

	dword x = GetSysColor(COLOR_GRAYTEXT);

	ChSet("SColorFace", Color::FromCR(GetSysColor(COLOR_3DFACE)));
	ChSet("SColorPaper", Color::FromCR(GetSysColor(COLOR_WINDOW)));
	ChSet("SColorText", Color::FromCR(GetSysColor(COLOR_WINDOWTEXT)));
	ChSet("SColorHighlight", Color::FromCR(GetSysColor(COLOR_HIGHLIGHT)));
	ChSet("SColorHighlightText", Color::FromCR(GetSysColor(COLOR_HIGHLIGHTTEXT)));
	ChSet("SColorMenu", Color::FromCR(GetSysColor(COLOR_MENU)));
	ChSet("SColorMenuText", Color::FromCR(GetSysColor(COLOR_MENUTEXT)));
	ChSet("SColorInfo", Color::FromCR(GetSysColor(COLOR_INFOBK)));
	ChSet("SColorInfoText", Color::FromCR(GetSysColor(COLOR_INFOTEXT)));
	ChSet("SColorDisabled", x ? Color::FromCR(x) : Color(r192, g192, b192));
	ChSet("SColorLight", Color::FromCR(GetSysColor(COLOR_3DHILIGHT)));
	ChSet("SColorShadow", Color::FromCR(GetSysColor(COLOR_3DSHADOW)));
}
#endif

#ifdef PLATFORM_X11
GLOBAL_VAR(TextSettings, sKDESettings);

TextSettings &KSettings()
{
	static TextSettings s;
	return s;
}

void ReadKdeSettings(const char *path)
{
	if(!path) return;
	Vector<String> s = Split(path, ':');
	static const char *fn[] = { "kdeglobals", "share/kdeglobals", "share/config/kdeglobals", "config/kdeglobals", NULL };
	for(int i = 0; i < s.GetCount(); i++)
		for(const char **f = fn; *f; f++)
			KSettings().Load(AppendFileName(s[i], *f));
}

void ReadKDESettings()
{
	static const char *env[] = { "KDEDIR", "KDEDIRS", "KDEHOME", "XDG_CONFIG_HOME", "XDG_DATA_DIRS", NULL };
	for(const char **f = env; *f; f++)
		ReadKdeSettings(getenv(*f));
	ReadKdeSettings("/usr/share");
	ReadKdeSettings("/usr/local/share");
	ReadKdeSettings(GetHomeDirFile(".local/share"));
	ReadKdeSettings(GetHomeDirFile(".kde"));
}

String GetKDE(const char *group, const char *entry)
{
	return KSettings().Get("General", entry);
}

String GetKDE(const char *entry)
{
	return GetKDE("General", entry);
}

Color KDEColor(const char *name, Color def)
{
	String s = GetKDE(name);
	CParser p(s);
	int r, g, b;
	try {
		r = p.ReadInt();
		p.PassChar(',');
		g = p.ReadInt();
		p.PassChar(',');
		b = p.ReadInt();
	}
	catch(CParser::Error) {
		return def;
	}
	return Color(r, g, b);
}

void UpdateSColors()
{
	ReadKDESettings();
	Color ltgray = KDEColor("background", Color(220, 220, 220));
	Color black = KDEColor("foreground", Color(0, 0, 0));
	Color white = KDEColor("windowBackground", Color(255, 255, 255));
	Color blue = KDEColor("selectBackground", Color(10, 95, 137));
	int r0 = black.GetR();
	int g0 = black.GetG();
	int b0 = black.GetB();

	int r255 = white.GetR();
	int g255 = white.GetG();
	int b255 = white.GetB();

	int r220 = ltgray.GetR();
	int g220 = ltgray.GetG();
	int b220 = ltgray.GetB();

	int r128 = (r255 + r0) / 2;
	int g128 = (g255 + g0) / 2;
	int b128 = (b255 + b0) / 2;

	int br10 = blue.GetR();
	int bg95 = blue.GetG();
	int bb137 = blue.GetB();

	ChSet("SBlack", black);
	ChSet("SGray", Color(r128, g128, b128));
	ChSet("SLtGray", ltgray);
	ChSet("SWhiteGray", Color((r255 + r220) / 2, (g255 + g220) / 2, (b255 + b220) / 2));
	ChSet("SWhite", white);
	ChSet("SRed", Color(r128, g0, b0));
	ChSet("SGreen", Color(r0, g128, b0));
	ChSet("SBrown", Color(r128, g128, 0));
	ChSet("SBlue", blue);
	ChSet("SMagenta", Color(r128, g0, b255));
	ChSet("SCyan", Color(r0, g128, bb137));
	ChSet("SYellow", Color(r255, g255, b0));
	ChSet("SLtRed", Color(r255, g0, b0));
	ChSet("SLtGreen", Color(r0, g255, b0));
	ChSet("SLtYellow", Color(r255, g255, b220));
	ChSet("SLtBlue", Color(r0, g0, b255));
	ChSet("SLtMagenta", Color(r255, g0, b255));
	ChSet("SLtCyan", Color(r0, g255, b255));

	ChSet("SColorPaper", KDEColor("windowBackground", white));
	ChSet("SColorFace", KDEColor("background", ltgray));
	ChSet("SColorText", KDEColor("windowForeground", black));
	ChSet("SColorHighlight", KDEColor("selectBackground", blue));
	ChSet("SColorHighlightText", KDEColor("selectForeground", white));
	ChSet("SColorMenu", Color(
				min(sDialog.GetR() * 105 / 100, 255),
				min(sDialog.GetG() * 105 / 100, 255),
				min(sDialog.GetB() * 105 / 100, 255)
			));
	ChSet("SColorMenuText", SColorText());
	ChSet("SColorInfo", SLtYellow());
	ChSet("SColorInfoText", SColorText());
	ChSet("SColorDisabled", Blend(SColorPaper(), SColorText()));
	ChSet("SColorLight", Blend(KDEColor("buttonBackground",SLtGray()), SPaper()));
	ChSet("SColorShadow", Blend(KDEColor("buttonBackground", SLtGray()), SColorDisabled()));

	Vector<String> fs = Split(GetKDE("font"), ',');
	if(fs.GetCount() > 1) {
		int h = atoi(fs[1]);
		Draw::SetStdFont(Font().FaceName(fs[0])
		                       .Height(h ? h * 96 / 72 : 13));
	}

}
#endif

INITBLOCK {
	ChRegister("!", UpdateSColors);
}
