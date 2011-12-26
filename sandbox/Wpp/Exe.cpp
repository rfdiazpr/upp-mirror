#include "Skylark.h"

#define LTIMING(x)

force_inline
bool Compiler::IsTrue(const Value& v)
{
	return !(IsNull(v) || IsNumber(v) && (int)v == 0 || IsValueArray(v) && v.GetCount() == 0);
}

String TypeMismatch(const char *s)
{
	return ErrorValue("<* type mismatch for '" + String(s) + "' *>");
}

Value Compiler::ExeVar::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	return stack[var_index];
}

Value Compiler::ExeConst::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	return value;
}

Value Compiler::ExeArray::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	ValueArray va;
	for(int i = 0; i < item.GetCount(); i++)
		va.Add(item[i].Eval(stack, out));
	return va;
}

Value Compiler::ExeMap::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	ValueMap m;
	for(int i = 0; i < key.GetCount(); i++)
		m.Add(key[i].Eval(stack, out), value[i].Eval(stack, out));
	return m;
}

Value Compiler::ExeNot::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	return !IsTrue(a->Eval(stack, out));
}

Value Compiler::ExeNeg::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v = a->Eval(stack, out);
	if(IsNumber(v))
		return -(double)v;
	return TypeMismatch("unary -");
}

Value Compiler::ExeMul::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 * (double)v2;
	return TypeMismatch("*");
}

Value Compiler::ExeDiv::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 / (double)v2;
	return TypeMismatch("/");
}

Value Compiler::ExeMod::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	if(IsNumber(v1) && IsNumber(v2)) {
		int m = v2;
		if(m == 0)
			return Null;
		return (int)v1 % m;
	}
	return TypeMismatch("%");
}

Value Compiler::ExeAdd::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	if(IsString(v1) && IsString(v2))
		return (String)v1 + (String)v2;
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 + (double)v2;
	if(v1.Is<RawHtmlText>() || v2.Is<RawHtmlText>()) {
		RawHtmlText h;
		h.text = AsString(v1) + AsString(v2);
		return RawToValue(h);
	}
	return AsString(v1) + AsString(v2);
}

Value Compiler::ExeSub::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 - (double)v2;
	return TypeMismatch("-");
}

Value Compiler::ExeSll::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	if(IsNumber(v1) && IsNumber(v2))
		return (int)v1 << min(32, (int)v2);
	return TypeMismatch("<<");
}

Value Compiler::ExeSrl::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	if(IsNumber(v1) && IsNumber(v2))
		return int((unsigned)(int)v1 >> min(32, (int)v2));
	return TypeMismatch(">>>");
}

Value Compiler::ExeSra::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	if(IsNumber(v1) && IsNumber(v2))
		return (int)v1 >> min(32, (int)v2);
	return TypeMismatch(">>");
}

Value Compiler::ExeLt::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	if(IsString(v1) && IsString(v2))
		return (String)v1 < (String)v2;
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 < (double)v2;
	return AsString(v1) < AsString(v2);
}

Value Compiler::ExeLte::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	if(IsString(v1) && IsString(v2))
		return (String)v1 <= (String)v2;
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 <= (double)v2;
	return AsString(v1) <= AsString(v2);
}

Value Compiler::ExeEq::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	if(IsString(v1) && IsString(v2))
		return (String)v1 == (String)v2;
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 == (double)v2;
	return AsString(v1) == AsString(v2);
}

Value Compiler::ExeNeq::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	if(IsString(v1) && IsString(v2))
		return (String)v1 != (String)v2;
	if(IsNumber(v1) && IsNumber(v2))
		return (double)v1 != (double)v2;
	return AsString(v1) != AsString(v2);
}

Value Compiler::ExeAnd::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	if(IsNumber(v1) && IsNumber(v2))
		return (int)v1 & (int)v2;
	return TypeMismatch("&");
}

Value Compiler::ExeXor::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	if(IsNumber(v1) && IsNumber(v2))
		return (int)v1 ^ (int)v2;
	return TypeMismatch("^");
}

Value Compiler::ExeOr::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	if(IsNumber(v1) && IsNumber(v2))
		return (int)v1 | (int)v2;
	return TypeMismatch("|");
}

Value Compiler::ExeAnl::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	return IsTrue(v1) && IsTrue(v2);
}

Value Compiler::ExeOrl::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value v1 = a->Eval(stack, out);
	Value v2 = b->Eval(stack, out);
	return IsTrue(v1) || IsTrue(v2);
}

Value Compiler::ExeCond::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	if(cond->Eval(stack, out))
		return ontrue->Eval(stack, out);
	else
	if(onfalse)
		return onfalse->Eval(stack, out);
	return Value();
}

Value Compiler::ExeField::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	return value->Eval(stack, out)[id];	
}

Value Compiler::ExeFn::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Vector<Value> v;
	v.SetCount(arg.GetCount());
	for(int i = 0; i < arg.GetCount(); i++)
		v[i] = arg[i].Eval(stack, out);
	return (*fn)(v);
}

Value Compiler::ExeLink::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Vector<Value> v;
	v.SetCount(arg.GetCount());
	for(int i = 0; i < arg.GetCount(); i++)
		v[i] = arg[i].Eval(stack, out);
	DDUMPC(*part);
	out << "\"";
	MakeLink(out, *part, v);
	out << "\"";
	return Value();
}

Value Compiler::ExeFirst::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	const LoopInfo& f = ValueTo<LoopInfo>(stack[var_index]);
	return f.first;
}

Value Compiler::ExeLast::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	const LoopInfo& f = ValueTo<LoopInfo>(stack[var_index]);
	return f.last;
}

Value Compiler::ExeIndex::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	const LoopInfo& f = ValueTo<LoopInfo>(stack[var_index]);
	return f.index;
}

Value Compiler::ExeKey::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	const LoopInfo& f = ValueTo<LoopInfo>(stack[var_index]);
	return f.key;
}

force_inline
static void sCatAsString(StringBuffer& out, const Value& v)
{
	if(IsNull(v))
		return;
	if(v.Is<RawHtmlText>())
		out.Cat(ValueTo<RawHtmlText>(v).text);
	else {
		const char *s;
		String h;
		if(v.Is<String>())
			s = ValueTo<String>(v);
		else {
			h = AsString(v);
			s = h;
		}
		while(*s) {
			if(*s == 31)
				out.Cat("&nbsp;");
			else
			if(*s == '<')
				out.Cat("&lt;");
			else
			if(*s == '>')
				out.Cat("&gt;");
			else
			if(*s == '&')
				out.Cat("&amp;");
			else
			if(*s == '\"')
				out.Cat("&quot;");
			else
			if((byte)*s < ' ')
				out.Cat(NFormat("&#%d;", (byte)*s));
			else
				out.Cat(*s);
			s++;
		}
	}
}

Value Compiler::ExeFor::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	Value array = value->Eval(stack, out);
	if(array.GetCount() == 0 && onempty)
		return onempty->Eval(stack, out);
	ValueMap m;
	bool map = array.Is<ValueMap>();
	if(map)
		m = array;
	int q = stack.GetCount();
	stack.Add();
	stack.Add();
	for(int i = 0; i < array.GetCount(); i++) {
		stack[q] = array[i];
		LoopInfo f;
		f.first = i == 0;
		f.last = i == array.GetCount() - 1;
		f.index = i;
		f.key = map ? m.GetKeys()[i] : (Value)i;
		stack[q + 1] = RawToValue(f);
		sCatAsString(out, body->Eval(stack, out));
	}
	stack.SetCount(q);
	return Value();
}

Value Compiler::ExeBlock::Eval(Vector<Value>& stack, StringBuffer& out) const
{
	int q = stack.GetCount();
	for(int i = 0; i < item.GetCount(); i++)
		sCatAsString(out, item[i].Eval(stack, out));
	stack.SetCount(q);
	return Value();
}

String Render(const One<Exe>& exe, Vector<Value>& var)
{
	StringBuffer b;
	Value v = exe->Eval(var, b);
	b.Cat(AsString(v));
	return b;
}
