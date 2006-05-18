#include "Draw.h"

static Color sBlack;
static Color sGray;
static Color sLtGray;
static Color sWhiteGray;
static Color sWhite;
static Color sRed;
static Color sGreen;
static Color sBrown;
static Color sBlue;
static Color sMagenta;
static Color sCyan;
static Color sYellow;
static Color sLtRed;
static Color sLtGreen;
static Color sLtYellow;
static Color sLtBlue;
static Color sLtMagenta;
static Color sLtCyan;

static Color sPaper;
static Color sDialog;
static Color sText;
static Color sHighlight;
static Color sHighlightText;
static Color sMenu;
static Color sMenuText;
static Color sInfo;
static Color sInfoText;
static Color sDisabled;
static Color sLight;
static Color sShadow;

#ifdef PLATFORM_WIN32
void UpdateSColors()
{
	dword c;
	c = GetSysColor(COLOR_WINDOWTEXT);
	int r0 = GetRValue(c);
	int g0 = GetGValue(c);
	int b0 = GetBValue(c);
	sBlack = Color(r0, g0, b0);
	c = GetSysColor(COLOR_3DSHADOW);
	int r128 = GetRValue(c);
	int g128 = GetGValue(c);
	int b128 = GetBValue(c);
	sGray = Color(r128, g128, b128);
	c = GetSysColor(COLOR_3DFACE);
	int r192 = GetRValue(c);
	int g192 = GetGValue(c);
	int b192 = GetBValue(c);
	sLtGray = Color(r192, g192, b192);
	c = GetSysColor(COLOR_WINDOW);
	int r255 = GetRValue(c);
	int g255 = GetGValue(c);
	int b255 = GetBValue(c);
	sWhite = Color(r255, g255, b255);
	int r224 = (r255 * 7 + r0) >> 3;
	int g224 = (r255 * 7 + r0) >> 3;
	int b224 = (r255 * 7 + r0) >> 3;
	sWhiteGray = Color(r224, g224, b224);
	sBlue = Color::FromCR(GetSysColor(COLOR_HIGHLIGHT));
	sRed = Color(r128, g0, b0);
	sGreen = Color(0, g128, b0);
	sBrown = Color(r128, g128, b0);
	sMagenta = Color(r128, g0, b255);
	sCyan = Color(r0, g128, b128);
	sYellow = Color(r255, g255, b0);
	sLtRed = Color(r255, g0, b0);
	sLtGreen = Color(r0, g255, b0);
	sLtYellow = Color(r255, g255, b192);
	sLtBlue = Color(0, 0, b255); // TRC 2005/08/05
	sLtMagenta = Color(r255, g0, b255);
	sLtCyan = Color(r0, g255, b255);

	sDialog = Color::FromCR(GetSysColor(COLOR_3DFACE));
	sPaper = Color::FromCR(GetSysColor(COLOR_WINDOW));
	sText = Color::FromCR(GetSysColor(COLOR_WINDOWTEXT));
	sHighlight = Color::FromCR(GetSysColor(COLOR_HIGHLIGHT));
	sHighlightText = Color::FromCR(GetSysColor(COLOR_HIGHLIGHTTEXT));
	sMenu = Color::FromCR(GetSysColor(COLOR_MENU));
	sMenuText = Color::FromCR(GetSysColor(COLOR_MENUTEXT));
	sInfo = Color::FromCR(GetSysColor(COLOR_INFOBK));
	sInfoText = Color::FromCR(GetSysColor(COLOR_INFOTEXT));
	dword x = GetSysColor(COLOR_GRAYTEXT);
	sDisabled = x ? Color::FromCR(x) : Color(r192, g192, b192);
	sLight = Color::FromCR(GetSysColor(COLOR_3DHILIGHT));
	sShadow = Color::FromCR(GetSysColor(COLOR_3DSHADOW));
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

	sBlack = black;
	sGray = Color(r128, g128, b128);
	sLtGray = ltgray;
	sWhiteGray = Color((r255 + r220) / 2, (g255 + g220) / 2, (b255 + b220) / 2);
	sWhite = white;
	sRed = Color(r128, g0, b0);
	sGreen = Color(r0, g128, b0);
	sBrown = Color(r128, g128, 0);
	sBlue = blue;
	sMagenta = Color(r128, g0, b255);
	sCyan = Color(r0, g128, bb137);
	sYellow = Color(r255, g255, b0);
	sLtRed = Color(r255, g0, b0);
	sLtGreen = Color(r0, g255, b0);
	sLtYellow = Color(r255, g255, b220);
	sLtBlue = Color(r0, g0, b255);
	sLtMagenta = Color(r255, g0, b255);
	sLtCyan = Color(r0, g255, b255);

	sPaper = KDEColor("windowBackground", white);
	sDialog = KDEColor("background", ltgray);
	sText = KDEColor("windowForeground", black);
	sHighlight = KDEColor("selectBackground", blue);
	sHighlightText = KDEColor("selectForeground", white);
	sMenu = Color(
				min(sDialog.GetR() * 105 / 100, 255),
				min(sDialog.GetG() * 105 / 100, 255),
				min(sDialog.GetB() * 105 / 100, 255)
			);
	sMenuText = sText;
	sInfo = sLtYellow;
	sInfoText = sText;
	sDisabled = Blend(sPaper, sText);
	sLight = Blend(KDEColor("buttonBackground", sLtGray), sPaper);
	sShadow = Blend(KDEColor("buttonBackground", sLtGray), sDisabled);

	Vector<String> fs = Split(GetKDE("font"), ',');
	if(fs.GetCount() > 1) {
		int h = atoi(fs[1]);
		Draw::SetStdFont(Font().FaceName(fs[0])
		                       .Height(h ? h * 96 / 72 : 13));
	}

}
#endif

static volatile bool  sSCInited;

inline void sInitSColorsImpl()
{
	ONCELOCK_(sSCInited) {
		UpdateSColors();
	}
}

inline void sInitSColors()
{
	if(!sSCInited)
		sInitSColorsImpl();
}

Color SBlack()     { sInitSColors(); return sBlack; }
Color SGray()      { sInitSColors(); return sGray; }
Color SLtGray()    { sInitSColors(); return sLtGray; }
Color SWhiteGray() { sInitSColors(); return sWhiteGray; }
Color SWhite()     { sInitSColors(); return sWhite; }
Color SRed()       { sInitSColors(); return sRed; }
Color SGreen()     { sInitSColors(); return sGreen; }
Color SBrown()     { sInitSColors(); return sBrown; }
Color SBlue()      { sInitSColors(); return sBlue; }
Color SMagenta()   { sInitSColors(); return sMagenta; }
Color SCyan()      { sInitSColors(); return sCyan; }
Color SYellow()    { sInitSColors(); return sYellow; }
Color SLtRed()     { sInitSColors(); return sLtRed; }
Color SLtGreen()   { sInitSColors(); return sLtGreen; }
Color SLtYellow()  { sInitSColors(); return sLtYellow; }
Color SLtBlue()    { sInitSColors(); return sLtBlue; }
Color SLtMagenta() { sInitSColors(); return sLtMagenta; }
Color SLtCyan()    { sInitSColors(); return sLtCyan; }

Color SColorPaper()         { sInitSColors(); return sPaper; }
Color SColorFace()          { sInitSColors(); return sDialog; }
Color SColorText()          { sInitSColors(); return sText; }
Color SColorHighlight()     { sInitSColors(); return sHighlight; }
Color SColorHighlightText() { sInitSColors(); return sHighlightText; }
Color SColorMenu()          { sInitSColors(); return sMenu; }
Color SColorMenuText()      { sInitSColors(); return sMenuText; }
Color SColorInfo()          { sInitSColors(); return sInfo; }
Color SColorInfoText()      { sInitSColors(); return sInfoText; }
Color SColorDisabled()      { sInitSColors(); return sDisabled; }
Color SColorLight()         { sInitSColors(); return sLight; }
Color SColorShadow()        { sInitSColors(); return sShadow; }
