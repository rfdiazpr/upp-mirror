#ifndef _CodeEditor_Highlight_h_
#define _CodeEditor_Highlight_h_

#define CTIMING(x) RTIMING(x)

struct HlStyle { //TODO:SYNTAX Move to class
	Color color;
	bool  bold;
	bool  italic;
	bool  underline;
};

struct Isx : Moveable<Isx> { // '(', '[' position
	int    line; //TODO:SYNTAX Move to class
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

	void Serialize(Stream& s)         { s % iftext % ifline % state; }

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
	bool        comment;       // we are in /* */ block comment
	bool        linecomment;   // we are in // line comment (because it can be continued by '\')
	bool        string;        // we are in string (becase it can be continued by '\')
	bool        linecont;      // line ended with '\'
	bool        was_namespace; // true if there was 'namespace', until '{' or ';' (not in ( [ brackets)
	char        macro;         // can be one of:
	enum        {
		MACRO_OFF = 0,  // last line was not #define
	    MACRO_CONT, // last line was #define and ended with '\'
	    MACRO_END   // last line was a macro, but ended
	};

	int         cl, bl, pl; // levels of { [ (

	Vector<int>     brk; // { ( [ stack (contains '{', ')', ']')
	Vector<int>     blk; // { line stack //TODO:SYNTAX: Join blk and bid
	Vector<int>     bid; // { indentation stack
	Vector<Isx>     par; // ( [ position stack
	Vector<IfState> ifstack;

	int         stmtline;     // line of latest "if", "else", "while", "do", "for" or -1
	int         endstmtline;  // line of latest ';' (not in ( [ brackets)
	int         seline;       // stmtline stored here on ';' (not in ( [ brackets)
	int         spar;         // ( [ level, reset on "if", "else", "while", "do", "for"

	static int  LoadSyntax(const char *keywords[], const char *names[]);
	static int  InitUpp(const char **q);
	static void InitKeywords();

	static Vector< Index<String> > keyword;
	static Vector< Index<String> > name;
	static Index<String> kw_upp;
	static int kw_macros, kw_logs, kw_sql_base, kw_sql_func;

	
	static Color BlockColor(int level);

	void Bracket(int pos, HighlightOutput& hls, CodeEditor& editor);

	void  ClearBraces();

	void  Grounding(const wchar *ln, const wchar *e);

public:
	void  Clear();

	void  ScanSyntax(const wchar *ln, const wchar *e, int line, int tab_size);

private:
	const Vector<Isx>& Par() const          { return par; }
	const Vector<int>& Blk() const          { return blk; }
	int   GetEndStatementLine() const       { return endstmtline; }
	int   GetStatementLine() const          { return stmtline; }
	int   GetSeLine() const                 { return seline; }

public:
	void  Serialize(Stream& s);
	
	void    Set(const String& s)          { CTIMING("Set"); if(s.GetCount() == 0) Clear(); else LoadFromString(*this, s); }
	String  Get()                         { CTIMING("Get"); return StoreAsString(*this); }
	
	bool    IsComment() const             { return comment; } // TODO:SYNTAX Replace with ASSIST logic
	bool    IsString() const              { return string; }
	bool    IsLineComment() const         { return linecomment; }

	void    Highlight(CodeEditor& editor, int line, Vector<LineEdit::Highlight>& hl, int pos);

	Vector<IfState> PickIfStack()         { return pick(ifstack); }

	static Color IfColor(char ifstate);

	SyntaxState()                         { Clear(); }
	
//	friend class CodeEditor;
};

#endif
