#include <Esc/Esc.h>

using namespace Upp;

template <class X, class R>
void Check(const char *op, X a, R r)
{
	ArrayMap<String, EscValue> global;
	StdLib(global);
	global.Add("a", a);
	String exp = AsString(op) + "a";
	EscValue v = Evaluate(exp, global);
	LOG("a = " << a << "; " << exp << " = " << v << " (" << r << ")");
}

#define UCHK(op, a) Check(#op, a, op a)

template <class X, class Y, class R>
void Check(X a, const char *op, Y b, R r)
{
	ArrayMap<String, EscValue> global;
	StdLib(global);
	global.Add("a", a);
	global.Add("b", b);
	String exp = "a" + AsString(op) + "b";
	EscValue v = Evaluate(exp, global);
	LOG("a = " << a << ", b = " << b << "; " << exp << " = " << v << " (" << r << ")");
	if(v.IsInt64())
		ASSERT(r == v.GetInt64());
	else
		ASSERT(r == (R)v.GetNumber());
}

#define CHK(a, op, b) Check(a, #op, b, a op b)

#define CHKDBL(a, b) \
LOG("============="); \
UCHK(+, a); \
UCHK(-, a); \
UCHK(!, a); \
CHK(a, +, b); \
CHK(a, -, b); \
CHK(a, *, b); \
CHK(a, /, b); \
CHK(a, <, b); \
CHK(a, >, b); \
CHK(a, <=, b); \
CHK(a, >=, b); \
CHK(a, ==, b); \
CHK(a, &&, b); \
CHK(a, ||, b); \

#define CHKINT(a, b) \
UCHK(~, a); \
CHKDBL(a, b); \
CHK(a, %, b); \
CHK(a, <<, b); \
CHK(a, >>, b); \
CHK(a, &, b); \
CHK(a, ^, b); \
CHK(a, |, b); \

CONSOLE_APP_MAIN
{
	CHKDBL(3.1, 2.1);
	CHKDBL(3.0, 2.0);
	CHKDBL(3.0, (int64)2);
	CHKDBL((int64)3, 2.0);
	CHKINT((int64)3, (int64)2);
}
