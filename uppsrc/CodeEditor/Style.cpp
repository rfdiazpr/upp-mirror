#include "CodeEditor.h"

NAMESPACE_UPP

const HlStyle& HighlightSetup::GetHlStyle(int i)
{
	return hl_style[i];
}

const char *HighlightSetup::GetHlName(int i)
{
	return s_hl_name[i];
}

bool HighlightSetup::HasHlFont(int i)
{
	return s_hl_font[i];
}

void  HighlightSetup::SetHlStyle(int i, Color c, bool bold, bool italic, bool underline)
{
	HlStyle& st = hl_style[i];
	st.color = c;
	st.bold = bold;
	st.italic = italic;
	st.underline = underline;
	SetColor(LineEdit::INK_NORMAL, hl_style[INK_NORMAL].color);
	SetColor(LineEdit::INK_DISABLED, hl_style[INK_DISABLED].color);
	SetColor(LineEdit::INK_SELECTED, hl_style[INK_SELECTED].color);
	SetColor(LineEdit::PAPER_NORMAL, hl_style[PAPER_NORMAL].color);
	SetColor(LineEdit::PAPER_READONLY, hl_style[PAPER_READONLY].color);
	SetColor(LineEdit::PAPER_SELECTED, hl_style[PAPER_SELECTED].color);
}

void HighlightSetup::LoadHlStyles(const char *s)
{
	CParser p(s);
	try {
		while(!p.IsEof()) {
			String id = p.ReadId();
			Color c = ReadColor(p);
			bool bold = false;
			bool italic = false;
			bool underline = false;
			for(;;)
				if(p.Id("bold"))
					bold = true;
				else
				if(p.Id("italic"))
					italic = true;
				else
				if(p.Id("underline"))
					underline = true;
				else
					break;
			for(int i = 0; i < HL_COUNT; i++)
				if(id == s_hl_color[i]) {
					SetHlStyle(i, c, bold, italic, underline);
					break;
				}
			p.PassChar(';');
		}
	}
	catch(CParser::Error) {
		DefaultHlStyles();
	}
}

String HighlightSetup::StoreHlStyles()
{
	String r;
	for(int i = 0; i < HL_COUNT; i++) {
		const HlStyle& s = GetHlStyle(i);
		r << Format("%-25s", s_hl_color[i]) << ' ' << FormatColor(s.color);
		if(s.bold)
			r << " bold";
		if(s.italic)
			r << " italic";
		if(s.underline)
			r << " underline";
		r << ";\r\n";
	}
	return r;
}

void HighlightSetup::DefaultHlStyles()
{
	SetHlStyle(INK_COMMENT, Green, false, true);
	SetHlStyle(INK_CONST_STRING, Red);

	SetHlStyle(INK_CONST_STRINGOP, LtBlue);
	SetHlStyle(INK_CONST_INT, Red);
	SetHlStyle(INK_CONST_FLOAT, Magenta);
	SetHlStyle(INK_CONST_HEX, Blue);
	SetHlStyle(INK_CONST_OCT, Blue);

	SetHlStyle(INK_OPERATOR, LtBlue);
	SetHlStyle(INK_KEYWORD, LtBlue, true);
	SetHlStyle(INK_UPP, Cyan);
	SetHlStyle(PAPER_LNG, Color(255, 255, 224));
	SetHlStyle(INK_ERROR, LtRed);
	SetHlStyle(INK_PAR0, Black);
	SetHlStyle(INK_PAR1, Green);
	SetHlStyle(INK_PAR2, Magenta);
	SetHlStyle(INK_PAR3, Brown);

	SetHlStyle(INK_UPPER, Black);
	SetHlStyle(INK_SQLBASE, Black);
	SetHlStyle(INK_SQLFUNC, Black);
	SetHlStyle(INK_SQLBOOL, Black);
	SetHlStyle(INK_UPPMACROS, Cyan);
	SetHlStyle(INK_UPPLOGS, Green);

	SetHlStyle(PAPER_BLOCK1, Blend(LtBlue, White, 240));
	SetHlStyle(PAPER_BLOCK2, Blend(LtGreen, White, 240));
	SetHlStyle(PAPER_BLOCK3, Blend(LtYellow, White, 240));
	SetHlStyle(PAPER_BLOCK4, Blend(LtMagenta, White, 240));

	SetHlStyle(INK_MACRO, Magenta);
	SetHlStyle(PAPER_MACRO, Color(255, 255, 230));
	SetHlStyle(PAPER_IFDEF, Color(230, 255, 255));
	SetHlStyle(INK_IFDEF, Color(170, 170, 170));

	SetHlStyle(PAPER_BRACKET0, LtYellow);
	SetHlStyle(PAPER_BRACKET, Yellow, true);

	SetHlStyle(INK_NORMAL, SColorText);
	SetHlStyle(INK_DISABLED, SColorDisabled);
	SetHlStyle(INK_SELECTED, SColorHighlightText);
	SetHlStyle(PAPER_NORMAL, SColorPaper);
	SetHlStyle(PAPER_READONLY, SColorFace);
	SetHlStyle(PAPER_SELECTED, SColorHighlight);
}

END_UPP_NAMESPACE
