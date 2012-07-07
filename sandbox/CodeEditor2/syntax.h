
#define HL_NAME(N) Highlighter& HIGHLIGHTER = Highlighter::Add(N);
#define HL_PATTERN(N, B) HIGHLIGHTER.AddStyleRule(CodeEditor::N, B);
#define HL_PATTERN_ML(N, B, E) HIGHLIGHTER.AddStyleRule(CodeEditor::N, B, E);
#define HL_SUBPATTERN(N, B) HIGHLIGHTER.AddStyleRule(CodeEditor::N, B, HIGHLIGHTER.GetRulesCount()-1);
#define HL_SCOPE(P1, P2) HIGHLIGHTER.Scope(P1, P2);

{
	#include FILE
}

#undef HL_PATTERN
#undef HL_SUBPATTERN
#undef HL_PATTERN_ML
#undef HL_SCOPE
#undef FILE
