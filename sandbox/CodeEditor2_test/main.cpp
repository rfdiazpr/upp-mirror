#include <CodeEditor2/CodeEditor.h>
using namespace Upp;

#include <CodeEditor/CodeEditor.h>

#define TEST 0

struct App : public TopWindow {
	typedef App CLASSNAME;
	ParentCtrl top, bottom;
	CodeEditor syntax, test;
	Splitter s;
	Button open_syntax, open_test, apply;

	void Apply(){
		Highlighter& h = Highlighter::Add("tmp");
		h.LoadSyntax(~syntax, true);
		test.Highlight("tmp");
		String t = ~test;
		test.Clear();
		test.Set(t);
	}

	void LoadSyntax(){
		static FileSelector fs;
		if(!fs.ExecuteOpen("Load syntax file ..."))
			return;
		syntax.Set(LoadFile(fs.Get()));
		Apply();
	}

	void LoadTest(){
		static FileSelector fs;
		if(!fs.ExecuteOpen("Load test file ..."))
			return;
		test.Set(LoadFile(fs.Get()));
	}

	App(){
		Title("CodeEditor2 test").Icon(CtrlImg::new_doc());
		Sizeable().Zoomable().Maximize();
		#if TEST
		Add(test.SizePos());
		test.SetFocus();
		test.ShowTabs();
		test.CheckEdited();
		test.LineNumbers(true);
		test.BarLine(true);
		test.HiliteScope(2);
		test.HiliteIfEndif(true);
		test.Highlight("cpp");
		//test.Set(LoadFile(__FILE__));
		test.Set(String("line1\n\"line2\"\nline3\n"));
		return;
		#endif

		Add(s.SizePos());
		s.Vert(top.SizePos(), bottom.SizePos());
		
		top.Add(syntax.VSizePosZ(5,0).HSizePosZ(5,160));
		top.Add(open_syntax.RightPosZ(5,150).TopPosZ(5,25));
		top.Add(apply.RightPosZ(5,150).BottomPosZ(0,25));
		bottom.Add(test.VSizePosZ(0,5).HSizePosZ(0,160));
		bottom.Add(open_test.RightPosZ(5,150).TopPosZ(0,25));
		
		apply.SetImage(CtrlImg::go_forward()).SetLabel("Highlight");
		open_syntax.SetImage(CtrlImg::open()).SetLabel("Load syntax");
		open_test.SetImage(CtrlImg::open()).SetLabel("Load test");
		
		open_syntax <<= THISBACK(LoadSyntax);
		open_test <<= THISBACK(LoadTest);
		apply <<= THISBACK(Apply);
		
		test.SetFocus();
		test.ShowTabs();
		test.LineNumbers(true);
		test.BarLine(true);
		test.HiliteScope(2);
		test.HiliteIfEndif(true);
		//test.CheckEdited();
		
		syntax.Highlight("syntax");
		syntax.ShowTabs();
		syntax.LineNumbers(true);
		//syntax.BarLine(true);
		syntax.HiliteScope(2);
		syntax.CheckEdited();
		
		String fn(__FILE__);
		fn.Replace("_test"DIR_SEPS"main.cpp", DIR_SEPS"c.syntax");
		syntax.Set(LoadFile(fn));
		test.Set(LoadFile(__FILE__));
		
		Apply();
	}
};

GUI_APP_MAIN{
	App().Run();
}
