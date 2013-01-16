#include "CodeEditor.h"

NAMESPACE_UPP

#define LLOG(x)    // LOG(x)
#define LDUMP(x)   // DUMP(x)


HlStyles::HlStyle HlStyles::list[HlStyles::HL_COUNT];
VectorMap<String,Highlighter> Highlighter::list;

//curent highlighter - TODO: do this better
#define chl Highlighter::Get(highlight)

void CodeEditor::MatchSubrules(const CodeEditor::Rule& rule, const String& s, int offset, CodeEditor::Cache& v){
	int begin, end;
	for(int i = 0; i < rule.subrules.GetCount(); i++){
		LLOG("testing subrule "<<i<<":"<<rule.subrules[i]<<" on "<<s);
		Rule& r = chl.rules[rule.subrules[i]];
		while(r.begin.GlobalMatch(s)) {
			Hl& hl = v.Add();
			Hl& hl2 = v.Add();
			r.begin.GetMatchPos(-1, begin, end);
			hl.pos = begin+offset;
			hl2.pos = end+offset;
			hl.rule = hl2.rule = rule.subrules[i];
			hl.level = hl2.level = -1;
			hl.begin = true;
			LLOG("Added sub begining "<<hl<< ", end "<<hl2);
		}
	}
}

void CodeEditor::Recalculate(int ln, bool force){
	Cache& v = cache[ln];
	if(force)
		v.dirty = true;
	else
		ASSERT(v.dirty);
	ASSERT(ln<line.GetCount());
	RTIMING(".Recalculate");
	LLOG("Recalculate "<<ln);
	v.Clear();
	int begin, end;
	// apply all the rules on this line
	LDUMP(chl.rules.GetCount());
	LDUMP(highlight);
	for(int i = 0; i < chl.rules.GetCount(); i++){
		if(chl.rules[i].sub) continue;
		LLOG("Testing rule "<<i);
		while(chl.rules[i].begin.GlobalMatch(line[ln].text)){
			Hl& hl = v.Add();
			chl.rules[i].begin.GetMatchPos(-1, begin, end);
			hl.pos = begin;
			hl.rule = i;
			hl.begin = true;
			LLOG("Added begining "<<hl);
			if (!chl.rules[i].multi) {
				Hl& hl = v.Add();
				hl.pos = end;
				hl.rule = i;
				LLOG("Added end "<<hl);
				MatchSubrules(chl.rules[i], line[ln].text.Mid(begin,end-begin), begin, v);
				continue;
			}
		}
		if (!chl.rules[i].multi)
			continue;
		while(chl.rules[i].end.GlobalMatch(line[ln].text)){
			Hl& hl = v.Add();
			chl.rules[i].end.GetMatchPos(-1, begin, end);
			hl.pos = end;
			hl.rule = i;
			LLOG("Added multi end "<<hl);
		}
	}
	Sort(v);
	// remove overlaps
	Vector<int> rem;
	int rule = -1;
	while(ln>0 && rule<0) {
		if(!cache[--ln].IsEmpty()) {
			if(cache[ln].Top().begin) {
				rule = cache[ln].Top().rule;
			} else {
				break;
			}
		}
	}
	LLOG("last rule: "<<rule);
	for(int i = 0; i < v.GetCount(); i++){
		LDUMP(i);
		if(rule>=0){
			if(v[i].rule==rule && !v[i].begin){
				rule = -1;
				LLOG("clean rule");
			} else {
				LDUMP(v[i].level);
				if(v[i].level==0) //TODO
					rem.Add(i);
			}
			continue;
		} else {
			if (v[i].begin) {
				rule = v[i].rule;
				LDUMP(rule);
			} else
				LOG("End tag without open (line"<<ln<<"+x, rule"<<v[i].rule<<")");
		}
	}
	v.Remove(rem);
	v.dirty = false;
}

void CodeEditor::RefreshLine(int i) {
	RTIMING(".RefreshLine");
	LLOG("RefreshLine "<<i);
	//something changed on line i
	cache[i].dirty = true;
	Recalculate(i);
	LineEdit::RefreshLine(i);
}

const CodeEditor::Hl& CodeEditor::FindPartner(int ln, int i, int& ln2) {
	const Hl& h = cache[ln][i];
	ln2 = ln;
	if(h.begin) {
		while(ln2<cache.GetCount()&&ln2-ln<limit){
			if(cache[ln2].dirty) Recalculate(ln2);
			while(i<cache[ln2].GetCount()){
				const Hl& r = cache[ln2][i];
				if(r.rule == h.rule && r.begin != h.begin)
					return r;
				i++;
			}
			ln2++;
			i = 0;
		}
	} else {
		while(ln2>=0&&ln-ln2<limit){
			if(cache[ln2].dirty) Recalculate(ln2);
			while(i>=0){
				const Hl& r = cache[ln2][i];
				if(r.rule == h.rule && r.begin != h.begin)
					return r;
				i--;
			}
			ln2--;
			if(ln2<0) break;
			i = cache[ln2].GetCount() - 1;
		}
	}
	LOG("Ignoring unclosed multi (line "<<ln<<",rule "<<h.rule<<")");
	ln2 = -1;
	return h;
}

const CodeEditor::Hl& CodeEditor::FindNext(int ln, int rule, int& ln2) {
	static Hl h;
	if((ln2 = ln+1) >= cache.GetCount()){
		ln2 = -1;
		return h;
	}
	int i = 0;
	while(ln2<cache.GetCount()&&ln2-ln<limit){
		if(cache[ln2].dirty) Recalculate(ln2);
		while(i<cache[ln2].GetCount()){
			const Hl& r = cache[ln2][i];
			if(r.rule == rule)
				return r;
			i++;
		}
		ln2++;
		i = 0;
	}
	ln2 = -1;
	return h;
}

const CodeEditor::Hl& CodeEditor::FindPrev(int ln, int rule, int& ln2) {
	static Hl h;
	if((ln2 = ln-1) < 0)
		return h;
	int i = cache[ln2].GetCount() - 1;
	while(ln2>=0&&ln-ln2<limit){
		if(cache[ln2].dirty) Recalculate(ln2);
		while(i>=0){
			const Hl& r = cache[ln2][i];
			if(r.rule == rule)
				return r;
			i--;
		}
		ln2--;
		if(ln2<0) break;
		i = cache[ln2].GetCount() - 1;
	}
	return h;
}

void CodeEditor::Fill(Vector<LineEdit::Highlight>& h, int ln, const CodeEditor::Hl& a, int ln2, const CodeEditor::Hl& b) const {
	int from, to;
	if(a.begin) {
		from = a.pos;
		to = (ln < ln2) ? h.GetCount()-1 : b.pos; 
	} else {
		from = (ln > ln2) ? 0 : b.pos;
		to = a.pos;
	}
	LLOG("Fill line " << ln << " from "<<from<<" to "<<to<<" with rule "<<a.rule);
	for(int i = from; i < to; i++){
		if(!chl.rules[a.rule].ink.IsNullInstance())
			h[i].ink = chl.rules[a.rule].ink;
		if(!chl.rules[a.rule].paper.IsNullInstance())
			h[i].paper = chl.rules[a.rule].paper;
		h[i].font.Bold(chl.rules[a.rule].font&Highlighter::BOLD);
		h[i].font.Italic(chl.rules[a.rule].font&Highlighter::ITALIC);
		h[i].font.Underline(chl.rules[a.rule].font&Highlighter::UNDERLINE);
	}
}

void CodeEditor::FillLine(Vector<LineEdit::Highlight>& h, const CodeEditor::Rule& rule) const {
	for(int i = 0; i < h.GetCount(); i++){
		if(!rule.ink.IsNullInstance())
			h[i].ink = rule.ink;
		if(!rule.paper.IsNullInstance())
			h[i].paper = rule.paper;
		h[i].font.Bold(rule.font&Highlighter::BOLD);
		h[i].font.Italic(rule.font&Highlighter::ITALIC);
		h[i].font.Underline(rule.font&Highlighter::UNDERLINE);
	}
}

void CodeEditor::HighlightLine(int ln, Vector<LineEdit::Highlight>& h, int pos){
	RTIMING(".HighlightLine");
	LLOG("Highlight "<<ln);
	Cache& v = cache.At(ln);
	if(v.dirty) {
		Recalculate(ln);
	}
	for(int i = 0; i < v.GetCount(); i++){
		if(!v[i].begin && !chl.rules[v[i].rule].multi) continue;
		int ln2;
		const Hl& p = FindPartner(ln, i, ln2);
		if(ln2 >= 0)
			Fill(h, ln, v[i], ln2, p);
	}
	for(int i = 0; i < chl.rules.GetCount(); i++){
		if(!chl.rules[i].multi) continue;
		int pln,nln;
		const Hl& p = FindPrev(ln, i, pln);
		const Hl& n = FindNext(ln, i, nln);
		if(nln>=0 && pln>=0 && p.begin && !n.begin) {
			LLOG("Fill entire line " << ln << " with rule "<<i);
			FillLine(h, chl.rules[i]);
		}
	}
}

#define HL_COLOR(x, a, b)    #x,
static const char *s_hl_colors[] = {
#include "hl_color.i"
};
#undef  HL_COLOR

inline int Style(String s){
	for(int i = 0; i < CodeEditor::HL_COUNT; i++){
		if(s_hl_colors[i]==s)
			return i;
	}
	LOG("Unkown style '"<<s<<"'!");
	return -1;
}

void Highlighter::LoadSyntax(const String& syntax, bool ignore_name){
	LLOG("Load syntax");
	rules.Clear();
	CParser p(syntax);
	p.SkipSpaces();
	String name,start,end;
	while(!p.IsEof()){
		if(p.Id("HL_NAME")){
			p.Char('(');
			String name = p.ReadString();
			p.Char(')');
			p.Char(';');
			if(!ignore_name)
				list.SetKey(Pos(),name);
			continue;
		}
		else if(p.Id("HL_SCOPE")){
			p.Char('(');
			String start = p.ReadString();
			p.Char(',');
			String end = p.ReadString();
			p.Char(')');
			p.Char(';');
			Scope(start, end);
			continue;
		}
		else if(p.Id("HL_PATTERN")){
			p.Char('(');
			String name = p.ReadId();
			p.Char(',');
			String start = p.ReadString();
			p.Char(')');
			p.Char(';');
			AddStyleRule(Style(name),start);
			continue;
		}
		else if(p.Id("HL_PATTERN_ML")){
			p.Char('(');
			String name = p.ReadId();
			p.Char(',');
			String start = p.ReadString();
			p.Char(',');
			String end = p.ReadOneString();
			p.Char(')');
			p.Char(';');
			AddStyleRule(Style(name),start,end);
			continue;
		}
		else if(p.Id("HL_SUBPATTERN")){
			p.Char('(');
			String name = p.ReadId();
			p.Char(',');
			String start = p.ReadString();
			p.Char(')');
			p.Char(';');
			AddStyleRule(Style(name),start,rules.GetCount()-1);
			continue;
		}
	}
}

void Highlighter::LoadSyntaxFile(const char* fn){
	LoadSyntax(LoadFile(fn));
}

Highlighter::Rule& Highlighter::AddRule(const char* begin, const char* end, Color ink, Color paper, int font){
	Rule& r = AddRule(begin, -1, ink, paper, font);
	r.end.SetPattern(end);
	r.end.Compile();
	r.end.Study();
	r.multi = true;
	if(r.end.IsError())
		NEVER_(String("Error ")+r.end.GetErrorCode()+" while compiling '"+end+"'");
	return r;
}

Highlighter::Rule& Highlighter::AddRule(const char* pattern, int subrule, Color ink, Color paper, int font){
	if(subrule >= 0)
		ASSERT(subrule < rules.GetCount());
	Rule& r = rules.Add();
	r.begin.SetPattern(pattern);
	r.begin.Compile();
	r.begin.Study();
	r.ink = ink;
	r.paper = paper;
	r.font = font;
	if(subrule>=0){
		LLOG("setting subrule "<<subrule);
		r.sub = true;
		rules[subrule].subrules.Add(rules.GetCount()-1);
	}
	if(r.begin.IsError())
		NEVER_(String("Error ")+r.begin.GetErrorCode()+" while compiling '"+pattern+"'");
	return r;
}

void Highlighter::AddStyleRule(int n, const char* begin, const char* end){
	const CodeEditor::HlStyle& s = CodeEditor::GetHlStyle(n);
	Color paper, ink;
	if(String(s_hl_colors[n]).StartsWith("PAPER")){
		paper = s.color;
		ink = Null;
	} else {
		ink = s.color;
		paper = Null;
	}
	int font = (s.bold?BOLD:0) | (s.italic?ITALIC:0) | (s.underline?UNDERLINE:0);
	AddRule(begin, end, ink, paper, font);
}

void Highlighter::AddStyleRule(int n, const char* pattern, int subrule){
	const CodeEditor::HlStyle& s = CodeEditor::GetHlStyle(n);
	Color paper, ink;
	if(String(s_hl_colors[n]).StartsWith("PAPER")){
		paper = s.color;
		ink = Null;
	} else {
		ink = s.color;
		paper = Null;
	}
	int font = (s.bold?BOLD:0) | (s.italic?ITALIC:0) | (s.underline?UNDERLINE:0);
	AddRule(pattern, subrule, ink, paper, font);
}

void Highlighter::Scope(const char* start, const char* end){
	scope_start.SetPattern(start);
	scope_start.Compile();
	scope_end.SetPattern(end);
	scope_end.Compile();
}

void HlStyles::Defaults(){
	Set(INK_COMMENT, Green, false, true);
	Set(INK_CONST_STRING, Red);

	Set(INK_CONST_STRINGOP, LtBlue);
	Set(INK_CONST_INT, Red);
	Set(INK_CONST_FLOAT, Magenta);
	Set(INK_CONST_HEX, Blue);
	Set(INK_CONST_OCT, Blue);

	Set(INK_OPERATOR, LtBlue);
	Set(INK_KEYWORD, LtBlue, true);
	Set(INK_UPP, Cyan);
	Set(PAPER_LNG, Color(255, 255, 224));
	Set(INK_ERROR, LtRed);
	Set(INK_PAR0, Black);
	Set(INK_PAR1, Green);
	Set(INK_PAR2, Magenta);
	Set(INK_PAR3, Brown);

	Set(INK_UPPER, Black);
	Set(INK_SQLBASE, Black);
	Set(INK_SQLFUNC, Black);
	Set(INK_SQLBOOL, Black);
	Set(INK_UPPMACROS, Cyan);
	Set(INK_UPPLOGS, Green);

	Set(PAPER_BLOCK1, Blend(LtBlue, White, 240));
	Set(PAPER_BLOCK2, Blend(LtGreen, White, 240));
	Set(PAPER_BLOCK3, Blend(LtYellow, White, 240));
	Set(PAPER_BLOCK4, Blend(LtMagenta, White, 240));

	Set(INK_MACRO, Magenta);
	Set(PAPER_MACRO, Color(255, 255, 230));
	Set(PAPER_IFDEF, Color(230, 255, 255));
	Set(INK_IFDEF, Color(170, 170, 170));

	Set(PAPER_BRACKET0, LtYellow);
	Set(PAPER_BRACKET, Yellow, true);

	Set(INK_NORMAL, SColorText);
	Set(INK_DISABLED, SColorDisabled);
	Set(INK_SELECTED, SColorHighlightText);
	Set(PAPER_NORMAL, SColorPaper);
	Set(PAPER_READONLY, SColorFace);
	Set(PAPER_SELECTED, SColorHighlight);
}

void  HlStyles::Set(int i, Color c, bool bold, bool italic, bool underline){
	HlStyle& st = list[i];
	st.color = c;
	st.bold = bold;
	st.italic = italic;
	st.underline = underline;
//	SetColor(LineEdit::INK_NORMAL, HlStyles::Get(INK_NORMAL).color);
//	SetColor(LineEdit::INK_DISABLED, HlStyles::Get(INK_DISABLED).color);
//	SetColor(LineEdit::INK_SELECTED, HlStyles::Get(INK_SELECTED).color);
//	SetColor(LineEdit::PAPER_NORMAL, HlStyles::Get(PAPER_NORMAL).color);
//	SetColor(LineEdit::PAPER_READONLY, HlStyles::Get(PAPER_READONLY).color);
//	SetColor(LineEdit::PAPER_SELECTED, HlStyles::Get(PAPER_SELECTED).color);
}

void CodeEditor::ZoomFont(int d){
	if(d!=0){
		int h = font.GetCy();
		int q = font.GetHeight();
		while(font.GetCy() == h && (d < 0 ? font.GetCy() > 5 : font.GetCy() < 40))
			font.Height(q += d);
	} else
		font.Height(16);
	SetFont(font);
	bar.Refresh();
}


INITBLOCK{
	HlStyles::Defaults();
	
	Highlighter::Add("none");
	
	#define FILE "c.syntax"
	#include "syntax.h"

	#define FILE "syntax.syntax"
	#include "syntax.h"
}

END_UPP_NAMESPACE