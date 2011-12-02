#include "Wpp.h"

One<Exe> Compiler::Prim()
{
	One<Exe> result;
	if(p.Char('!'))
		result = Create<ExeNot>(Prim());
	else
	if(p.Char('-'))
		result = Create<ExeNeg>(Prim());
	else
	if(p.Char('+'))
		result = Prim();
	else
	if(p.IsId()) {
		String id = p.ReadId();
		int n = var.Find(id);
		if(n < 0)
			p.ThrowError("unknown variable " + id);
		result.Create<ExeVar>().var_index = n;
	}
	else
	if(p.Char('(')) {
		result = Exp();
		p.PassChar(')');
	}
	else {
		ExeConst& c = result.Create<ExeConst>();
		if(p.Char2('0', 'x') || p.Char2('0', 'X'))
			c.value = (int)p.ReadNumber(16);
		else
		if(p.Char('0'))
			c.value = int(p.IsNumber() ? p.ReadNumber(8) : 0);
		else
			c.value = p.IsString() ? Value(p.ReadString()) : Value(p.ReadDouble());
	}
	return result;
}

One<Exe> Compiler::Mul()
{
	One<Exe> result = Prim();
	for(;;)
		if(p.Char('*'))
			result = Create<ExeMul>(result, Prim());
		else
		if(p.Char('/'))
			result = Create<ExeDiv>(result, Prim());
		else
		if(p.Char('%'))
			result = Create<ExeMod>(result, Prim());
		else
			return result;
}

One<Exe> Compiler::Add()
{
	One<Exe> result = Mul();
	for(;;)
		if(p.Char('+'))
			result = Create<ExeAdd>(result, Mul());
		else
		if(p.Char('-'))
			result = Create<ExeSub>(result, Mul());
		else
			return result;
}

One<Exe> Compiler::Shift()
{
	One<Exe> result = Add();
	for(;;)
		if(p.Char2('<', '<'))
			result = Create<ExeSll>(result, Add());
		else
		if(p.Char3('>', '>', '>'))
			result = Create<ExeSrl>(result, Add());
		else
		if(p.Char2('>', '>'))
			result = Create<ExeSra>(result, Add());
		else
			return result;
}

One<Exe> Compiler::Rel()
{
	One<Exe> result = Shift();
	for(;;)
		if(p.Char2('<', '='))
			result = Create<ExeLte>(result, Shift());
		else
		if(p.Char2('>', '='))
			result = Create<ExeLte>(Shift(), result);
		else
		if(p.Char('<'))
			result = Create<ExeLt>(result, Shift());
		else
		if(p.Char('>'))
			result = Create<ExeLt>(Shift(), result);
		else
			return result;
}

One<Exe> Compiler::Eq()
{
	One<Exe> result = Rel();
	for(;;)
		if(p.Char2('=', '='))
			result = Create<ExeEq>(result, Rel());
		else
		if(p.Char2('!', '='))
			result = Create<ExeNeq>(Rel(), result);
		else
			return result;
}

One<Exe> Compiler::And()
{
	One<Exe> result = Eq();
	while(!p.IsChar2('&', '&') && p.Char('&'))
		result = Create<ExeAnd>(result, Eq());
	return result;
}

One<Exe> Compiler::Xor()
{
	One<Exe> result = And();
	while(p.Char('^'))
		result = Create<ExeXor>(result, And());
	return result;
}

One<Exe> Compiler::Or()
{
	One<Exe> result = Xor();
	while(!p.IsChar2('|', '|') && p.Char('|'))
		result = Create<ExeOr>(result, Xor());
	return result;
}

One<Exe> Compiler::LogAnd()
{
	One<Exe> result = Or();
	while(p.Char2('&', '&'))
		result = Create<ExeAnl>(result, Or());
	return result;
}

One<Exe> Compiler::LogOr()
{
	One<Exe> result = LogAnd();
	while(p.Char2('|', '|'))
		result = Create<ExeOrl>(result, LogAnd());
	return result;
}

One<Exe> Compiler::Conditional()
{
	One<Exe> result = LogOr();
	if(p.Char('?')) {
		One<Exe> r;
		ExeCond& c = r.Create<ExeCond>();
		c.cond = result;
		c.ontrue = LogOr();
		p.PassChar(':');
		c.onfalse = LogOr();
		result = r;
	}
	return result;
}

One<Exe> Compiler::Exp()
{
	return Conditional();
}

One<Exe> Compile(const char *code, const Index<String>& vars)
{
	Compiler c(code, vars);
	try {
		return c.Exp();
	}
	catch(CParser::Error e) {
		One<Exe> result;
		result.Create<Compiler::ExeConst>().value = e;
		return result;
	}
}
