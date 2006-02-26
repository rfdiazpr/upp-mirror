#include <Ole/Ctrl/OleCtrl.h>
#include <TCtrlLib/Calc/TCtrlLibCalc.h>
#pragma hdrstop

#include "calc_idl.h"

#define LAYOUTFILE "calc.lay"
#include <CtrlCore/lay.h>

class Calculator : public OcxRunnableControl, public DispatchInterface<ICalculator>
{
public:
	typedef Calculator CLASSNAME;
	Calculator();

	// ICalculator

	STDMETHOD(get_Input)(BSTR *result);
	STDMETHOD(put_Input)(BSTR input);
	STDMETHOD(Calculate)();
	STDMETHOD(get_Output)(BSTR *result);

private:
	void OnCalc();
	void OnHistoryAction();

private:
	WithCalculatorLayout<TopWindow> dialog;
	GradientDisplay bgnd;
};

OCX_OBJECT(Calculator)

Calculator::Calculator()
: bgnd(Point(0, 0), Color(128, 192, 192), Point(-1, 0), White)
{
	dialog.Background(bgnd);
	CtrlLayout(dialog);
	dialog.ok.Ok() <<= THISBACK(OnCalc);
	dialog.ok.Tip("Evaluate given expression");
	NoWantFocus();
	dialog.NoWantFocus();
	SetRect(dialog.GetLayoutSize());
	Add(dialog.SizePos());
	dialog.history.AutoHideSb();
	dialog.history.AddColumn("Výraz", 10);
	dialog.history.AddColumn("Výsledek", 2);
	dialog.history <<= THISBACK(OnHistoryAction);
}

void Calculator::OnCalc()
{
	PromptOK("Fuck me!");
	Vector<Value> values;
	values << ~dialog.expr << Value();
	if(dialog.history.GetCount() > 10)
		dialog.history.Remove(dialog.history.GetCount() - 1);
	dialog.history.Insert(0, values);
	dialog.history.SetCursor(0);
}

void Calculator::OnHistoryAction()
{
	if(!dialog.history.IsCursor())
		return;

	String expr = dialog.history.Get(0);
	String result;
	try {
		CalcNodePtr node = CalcParser().Scan(expr);
		CalcContext context;
		result = StdFormat(node->Calc(context));
	}
	catch(Exc e) {
		result = e;
	}
	dialog.expr <<= expr;
	dialog.history.Set(1, result);
	dialog.value <<= result;
	dialog.expr.SetWantFocus();
	dialog.expr.SetSelection();
}

HRESULT Calculator::get_Input(BSTR *result)
{
	ReturnString(result, ~dialog.expr);
	return S_OK;
}

HRESULT Calculator::put_Input(BSTR input)
{
	dialog.expr <<= BSTRToString(input);
	return S_OK;
}

HRESULT Calculator::Calculate()
{
	OnCalc();
	return S_OK;
}

HRESULT Calculator::get_Output(BSTR *result)
{
	ReturnString(result, ~dialog.value);
	return S_OK;
}

OCX_APP_MAIN
{
	SetLanguage(LNG_CZECH);
	SetDefaultCharset(CHARSET_WIN1250);
}
