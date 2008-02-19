#include "ide.h"

using namespace astyle;

/////////////////////////////////////////////////////////////////////////////////////////
// CLASS ASStreamIterator
class ASStreamIterator : public ASSourceIterator
{
	public:
		ASStreamIterator(WString const &Src);
		virtual ~ASStreamIterator() {}
		virtual bool hasMoreLines() const;
		virtual WString nextLine();
		
	protected:
	
		WString const &src;
		int p;

}; // END Class ASStreamIterator

// Constructor Class ASStreamIterator
ASStreamIterator::ASStreamIterator(WString const &Src) : ASSourceIterator(), src(Src)
{
	// initializes buffer pointer
	p = 0;
} // END Constructor Class ASStreamIterator


bool ASStreamIterator::hasMoreLines() const
{
	return src[p] != 0;
	
} // END ASStreamIterator::hasMoreLines()

WString ASStreamIterator::nextLine()
{
	int p2 = p;
	while(src[p2] && src[p2] != '\n')
		p2++;
	WString line = src.Mid(p, p2 - p);

	if(src[p2])
	   p2++;
	p = p2;

	return line;
	
} // END ASStreamIterator::nextLine()

/////////////////////////////////////////////////////////////////////////////////////////
// Formats a string of code with a given formatter
WString Ide::FormatCodeString(WString const &Src, ASFormatter &Formatter)
{
	// Creates the output string
	WString Dest;

	// Creates the iterator for the formatter
	ASStreamIterator streamIterator(Src);

	// Initializes the formatter to work on selected stream
	Formatter.init(&streamIterator);
	
	// Processes all text in source file and put them in output file
	while (Formatter.hasMoreLines())
		Dest << Formatter.nextLine() << '\n';
	
	// returns output string
	return Dest;
	
} // END Ide::FormatCodeString()

/////////////////////////////////////////////////////////////////////////////////////////
// Formats editor's code with Ide format parameters
void Ide::FormatCode()
{
	// Gets editor contents from editor
	WString Src;
	int l, h;
	bool sel = editor.GetSelection(l, h);
	if(sel)
		Src = editor.GetSelectionW();
	else
	 	Src = editor.GetW();
	
	// Instantiate the formatter object
	ASFormatter Formatter;
	
	// Sets up the formatter for C++ formatting
	Formatter.setCStyle();
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Sets up astyle options
	Formatter.setBracketIndent(astyle_BracketIndent);
	Formatter.setNamespaceIndent(astyle_NamespaceIndent);
	Formatter.setBlockIndent(astyle_BlockIndent);
	Formatter.setCaseIndent(astyle_CaseIndent);
	Formatter.setClassIndent(astyle_ClassIndent);
	Formatter.setLabelIndent(astyle_LabelIndent);
	Formatter.setSwitchIndent(astyle_SwitchIndent);
	Formatter.setPreprocessorIndent(astyle_PreprocessorIndent);
	Formatter.setMaxInStatementIndentLength(astyle_MaxInStatementIndentLength);
	Formatter.setMinConditionalIndentLength(astyle_MinInStatementIndentLength);
	Formatter.setBreakClosingHeaderBracketsMode(astyle_BreakClosingHeaderBracketsMode);
	Formatter.setBreakElseIfsMode(astyle_BreakElseIfsMode);
	Formatter.setBreakOneLineBlocksMode(astyle_BreakOneLineBlocksMode);
	Formatter.setSingleStatementsMode(astyle_SingleStatementsMode);
	Formatter.setBreakBlocksMode(astyle_BreakBlocksMode);
	Formatter.setBreakClosingHeaderBlocksMode(astyle_BreakClosingHeaderBlocksMode);
	Formatter.setBracketFormatMode((astyle::BracketMode)astyle_BracketFormatMode);
	switch(astyle_ParensPaddingMode)
	{
		case PAD_INSIDE :
			Formatter.setParensInsidePaddingMode(true);
			Formatter.setParensOutsidePaddingMode(false);
			break;
		case PAD_OUTSIDE :
			Formatter.setParensInsidePaddingMode(false);
			Formatter.setParensOutsidePaddingMode(true);
			break;
		case PAD_BOTH :
			Formatter.setParensInsidePaddingMode(true);
			Formatter.setParensOutsidePaddingMode(true);
			break;
		default :
			Formatter.setParensOutsidePaddingMode(false);
			Formatter.setParensInsidePaddingMode(false);
			break;
	}
	Formatter.setParensUnPaddingMode(astyle_ParensUnPaddingMode);
	Formatter.setOperatorPaddingMode(astyle_OperatorPaddingMode);
	Formatter.setEmptyLineFill(astyle_EmptyLineFill);
	Formatter.setTabSpaceConversionMode(astyle_TabSpaceConversionMode);
	Formatter.setTabIndentation(editortabsize, indent_spaces ? false : true);
	Formatter.setSpaceIndentation(indent_spaces ? indent_amount : editortabsize);
	
	WString Dest = FormatCodeString(Src, Formatter);

	editor.NextUndo();
	if(sel) {
		editor.Remove(l, h - l);
		editor.SetSelection(l, l + editor.Insert(l, Dest));
	}
	else {
		editor.Remove(0, editor.GetLength());
		editor.Insert(0, Dest);
	}
}
