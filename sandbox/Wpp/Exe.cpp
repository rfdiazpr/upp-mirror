#include "Wpp.h"

force_inline
bool Compiler::IsTrue(const Value& v)
{
	return !(IsNull(v) || IsNumber(v) && (int)v == 0);
}

String TypeMismatch(const char *s)
{
	return ErrorValue("<* type mismatch for '" + String(s) + "' *>");
}

Value Compiler::ExeVar::Eval(Vector<Value>& stack) const
{
	return stack[var_index];
}

Value Compiler::ExeConst::Eval(Vector<Value>& stack) const
{
	return value;
}

Value Compiler::ExeNot::Eval(Vector<Value>& stack) const
{
	return !IsTrue(a->Eval(stack));
}

Value Compiler::ExeNeg::Eval(Vector<Value>& stack) const
{
	Value v = a->Eval(stack);
	if(IsNumber(v))
		return -(double)v;
	return TypeMismatch("unary -");
}

Value Compiler::ExeMul::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 * (double)v2;
	return TypeMismatch("*");
}

Value Compiler::ExeDiv::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 / (double)v2;
	return TypeMismatch("/");
}

Value Compiler::ExeMod::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	if(IsNumber(v1) && IsNumber(v2)) {
		int m = v2;
		if(m == 0)
			return Null;
		return (int)v1 % m;
	}
	return TypeMismatch("%");
}

Value Compiler::ExeAdd::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	if(IsString(v1) && IsString(v2))
		return (String)v1 + (String)v2;
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 + (double)v2;
	return AsString(v1) + AsString(v2);
}

Value Compiler::ExeSub::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 - (double)v2;
	return TypeMismatch("-");
}

Value Compiler::ExeSll::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	if(IsNumber(v1) && IsNumber(v2))
		return (int)v1 << min(32, (int)v2);
	return TypeMismatch("<<");
}

Value Compiler::ExeSrl::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	if(IsNumber(v1) && IsNumber(v2))
		return int((unsigned)(int)v1 >> min(32, (int)v2));
	return TypeMismatch(">>>");
}

Value Compiler::ExeSra::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	if(IsNumber(v1) && IsNumber(v2))
		return (int)v1 >> min(32, (int)v2);
	return TypeMismatch(">>");
}

Value Compiler::ExeLt::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	if(IsString(v1) && IsString(v2))
		return (String)v1 < (String)v2;
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 < (double)v2;
	return AsString(v1) < AsString(v2);
}

Value Compiler::ExeLte::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	if(IsString(v1) && IsString(v2))
		return (String)v1 <= (String)v2;
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 <= (double)v2;
	return AsString(v1) <= AsString(v2);
}

Value Compiler::ExeEq::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	if(IsString(v1) && IsString(v2))
		return (String)v1 == (String)v2;
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 == (double)v2;
	return AsString(v1) == AsString(v2);
}

Value Compiler::ExeNeq::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	if(IsString(v1) && IsString(v2))
		return (String)v1 != (String)v2;
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 != (double)v2;
	return AsString(v1) != AsString(v2);
}

Value Compiler::ExeAnd::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	if(IsNumber(v1) && IsNumber(v2))
		return (int)v1 & (int)v2;
	return TypeMismatch("&");
}

Value Compiler::ExeXor::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	if(IsNumber(v1) && IsNumber(v2))
		return (int)v1 ^ (int)v2;
	return TypeMismatch("^");
}

Value Compiler::ExeOr::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	if(IsNumber(v1) && IsNumber(v2))
		return (int)v1 | (int)v2;
	return TypeMismatch("|");
}

Value Compiler::ExeAnl::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	return IsTrue(v1) && IsTrue(v2);
}

Value Compiler::ExeOrl::Eval(Vector<Value>& stack) const
{
	Value v1 = a->Eval(stack);
	Value v2 = b->Eval(stack);
	return IsTrue(v1) || IsTrue(v2);
}

Value Compiler::ExeCond::Eval(Vector<Value>& stack) const
{
	if(cond->Eval(stack))
		return ontrue->Eval(stack);
	else
		return onfalse->Eval(stack);
}

Value Run(const One<Exe>& exe, Vector<Value>& v)
{
	return exe->Eval(v);
}
