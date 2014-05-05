#ifndef _CodeEditor_Highlight_h_
#define _CodeEditor_Highlight_h_

struct HlStyle { //TODO:SYNTAX Move to class
	Color color;
	bool  bold;
	bool  italic;
	bool  underline;
};

struct Isx : Moveable<Isx> { //TODO:SYNTAX Move to class
	int    line;
	int    pos;
	
	void Serialize(Stream& s)    { s % line % pos; }

	friend bool operator==(Isx a, Isx b) { return a.line == b.line && a.pos == b.pos; }
	friend bool operator!=(Isx a, Isx b) { return !(a == b); }
};

struct IfState : Moveable<IfState> { //TODO:SYNTAX Move to class
	enum        { IF = '0', ELIF, ELSE, ELSE_ERROR, ENDIF_ERROR };
	WString iftext;
	short   ifline;
	char    state;

	void Serialize(Stream& s)    { s % iftext % ifline % state; }

	bool operator==(const IfState& b) const {
		return iftext == b.iftext && state == b.state && ifline == b.ifline;
	}

	IfState()                         { ifline = state = 0; }
};

struct HighlightSetup { // Global highlighting settings
public:
	enum HighlightType {
		HIGHLIGHT_NONE = -1, HIGHLIGHT_CPP = 0, HIGHLIGHT_USC, HIGHLIGHT_JAVA, HIGHLIGHT_T,
		HIGHLIGHT_CALC, HIGHLIGHT_LAY, HIGHLIGHT_SCH, HIGHLIGHT_SQL, HIGHLIGHT_CS,
		HIGHLIGHT_JAVASCRIPT, HIGHLIGHT_CSS, HIGHLIGHT_JSON,
		HIGHLIGHT_COUNT
	};

#define HL_COLOR(x, a, b)      x,
	enum {
#include "hl_color.i"
		HL_COUNT
	};
#undef HL_COLOR

	static HlStyle hl_style[HL_COUNT];
	static byte    hilite_scope;
	static byte    hilite_ifdef;
	static byte    hilite_bracket;
	static bool    thousands_separator;

	static const HlStyle& GetHlStyle(int i);
	static void           SetHlStyle(int i, Color c, bool bold = false, bool italic = false, bool underline = false);
	static void           LoadHlStyles(const char *s);
	static String         StoreHlStyles();
	static void           DefaultHlStyles();

	static const char    *GetHlName(int i);
	static bool           HasHlFont(int i);
};

struct HighlightOutput : HighlightSetup {
	Vector<LineEdit::Highlight>& v;
	LineEdit::Highlight          def;
	int                          pos;

public:
	void Set(int pos, int count, const HlStyle& ink);
	void SetFont(int pos, int count, const HlStyle& f);
	void SetPaper(int pos, int count, Color paper);
	void SetInk(int pos, int count, Color ink);
	void Put(int count, const HlStyle& ink)           { Set(pos, count, ink); pos += count; }
	void Put(const HlStyle& ink)                      { Put(1, ink); }
	void Put(const HlStyle& ink, word flags)          { Put(1, ink); v[pos - 1].flags = flags; }

	const wchar *CString(const wchar *p);
	
	HighlightOutput(Vector<LineEdit::Highlight>& v);
	~HighlightOutput();
};

class SyntaxState :
public HighlightSetup // TODO:SYNTAX
{
	bool        comment;
	bool        linecomment;
	bool        string;
	bool        linecont;
	bool        was_namespace;
	char        macro;
	enum        { MACRO_OFF, MACRO_CONT, MACRO_END };

	int         cl, bl, pl;

	Vector<int>     brk;
	Vector<int>     blk;
	Vector<int>     bid;
	Vector<Isx>     par;
	Vector<IfState> ifstack;

	int         stmtline;
	int         endstmtline;
	int         seline;
	int         spar;

	static int  LoadSyntax(const char *keywords[], const char *names[]);
	static int  InitUpp(const char **q);
	static void InitKeywords();

	static Vector< Index<String> > keyword;
	static Vector< Index<String> > name;
	static Index<String> kw_upp;
	static int kw_macros, kw_logs, kw_sql_base, kw_sql_func;

	
	static Color BlockColor(int level);
	void Bracket(int pos, HighlightOutput& hls, CodeEditor& editor);

public:
	void  DropItem(int type);
	bool  Drop(int type);
	void  ClearBraces();
	void  Clear();
//		bool  MatchHilite(const SyntaxState& st) const;
	void  Grounding(const wchar *ln, const wchar *e);

	void  ScanSyntax(const wchar *ln, const wchar *e, int line, int tab_size);
	
	const Vector<Isx>& Par() const          { return par; }
	const Vector<int>& Blk() const          { return blk; }
	int   GetEndStatementLine() const       { return endstmtline; }
	int   GetStatementLine() const          { return stmtline; }
	int   GetSeLine() const                 { return seline; }
	
	void  Serialize(Stream& s);
	
	void    Set(const String& s)          { if(s.GetCount() == 0) Clear(); else LoadFromString(*this, s); }
	String  Get()                         { return StoreAsString(*this); }
	void    MacroContOff()                { if(macro != SyntaxState::MACRO_CONT) macro = SyntaxState::MACRO_OFF; }
	
	bool    IsMacro() const               { return macro; }

	bool    IsComment() const             { return comment; } // TODO:SYNTAX Replace with ASSIST logic
	bool    IsString() const              { return string; }
	bool    IsLineComment() const         { return linecomment; }
	

	void    Highlight(CodeEditor& editor, int line, Vector<LineEdit::Highlight>& hl, int pos);

	Vector<IfState> PickIfStack()         { return pick(ifstack); }

	static Color IfColor(char ifstate);

	SyntaxState()                         { Clear(); }
	
	friend class CodeEditor;
};

#endif
