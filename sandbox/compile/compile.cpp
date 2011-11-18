#include "compile.h"

One<Oper> Compiler::Prim(CParser& p)
{
	One<Oper> result;
	if(p.Char('!'))
		result.Create<ExeNot>().val = Prim(p);
	else
	if(p.Char('-'))
		result.Create<ExeNeg>().val = Prim(p);
	else
	if(p.IsId()) {
		String name = p.ReadId();
		double *v = var.Get(name, NULL);
		if(!v)
			p.ThrowError("unknown variable " + name);
		result.Create<ExeVar>().var = v;
	}
	else
	if(p.Char('(')) {
		result = Exp(p);
		p.PassChar(')');
	}
	else
		result.Create<ExeConst>().value = p.ReadDouble();
	return result;
}

One<Oper> Compiler::Mul(CParser& p)
{
	One<Oper> result = Prim(p);
	for(;;)
		if(p.Char('*'))
			result = Binary<ExeMul>(result, Prim(p));
		else
		if(p.Char('/'))
			result = Binary<ExeDiv>(result, Prim(p));
		else
			return result;
}

One<Oper> Compiler::Add(CParser& p)
{
	One<Oper> result = Mul(p);
	for(;;)
		if(p.Char('+'))
			result = Binary<ExeAdd>(result, Mul(p));
		else
		if(p.Char('-'))
			result = Binary<ExeSub>(result, Mul(p));
		else
			return result;
}

One<Oper> Compiler::Cond(CParser& p)
{
	One<Oper> result = Add(p);
	for(;;)
		if(p.Char2('<', '='))
			result = Binary<ExeLte>(result, Add(p));
		else
		if(p.Char2('>', '='))
			result = Binary<ExeLte>(Add(p), result);
		else
		if(p.Char2('=', '='))
			result = Binary<ExeEq>(result, Add(p));
		else
		if(p.Char2('!', '='))
			result = Binary<ExeNeq>(result, Add(p));
		else
		if(p.Char('<'))
			result = Binary<ExeLt>(result, Add(p));
		else
		if(p.Char('>'))
			result = Binary<ExeLt>(Add(p), result);
		else
			return result;
}

One<Oper> Compiler::And(CParser& p)
{
	One<Oper> result = Cond(p);
	for(;;)
		if(p.Char2('&', '&'))
			result = Binary<ExeAnd>(result, Cond(p));
		else
			return result;
}

One<Oper> Compiler::Or(CParser& p)
{
	One<Oper> result = And(p);
	for(;;)
		if(p.Char2('|', '|'))
			result = Binary<ExeOr>(result, Cond(p));
		else
			return result;
}

One<Oper> Compiler::Exp(CParser& p)
{
	One<Oper> result = Or(p);
	if(p.Char('=')) {
		ExeVar *v = dynamic_cast<ExeVar *>(~result);
		if(!v)
			p.ThrowError("Expected variable");
		One<Oper> rr;
		ExeAssign& a = rr.Create<ExeAssign>();
		a.var = v->var;
		a.val = Exp(p);
		result = rr;
	}
	return result;
}

One<Oper> Compiler::Statement(CParser& p)
{
	One<Oper> result;
	if(p.Char('{')) {
		ExeBlock& blk = result.Create<ExeBlock>();
		while(!p.Char('}')) {
			if(p.IsEof())
				p.ThrowError("unexpected end of file");
			blk.statement.Add(Statement(p).Detach());
		}
	}
	else
	if(p.Id("if")) {
		ExeIf& sif = result.Create<ExeIf>();
		p.PassChar('(');
		sif.cond = Exp(p);
		p.PassChar(')');
		sif.body = Statement(p);
		if(p.Id("else"))
			sif.else_body = Statement(p);
		else
			sif.else_body.Create<Oper>();
	}
	else
	if(p.Id("while")) {
		ExeWhile& swh = result.Create<ExeWhile>();
		p.PassChar('(');
		swh.cond = Exp(p);
		p.PassChar(')');
		swh.body = Statement(p);
	}
	else
	if(p.Id("for")) {
		ExeFor& sf = result.Create<ExeFor>();
		p.PassChar('(');
		sf.init = Exp(p);		
		p.PassChar(';');
		sf.cond = Exp(p);
		p.PassChar(';');
		sf.after = Exp(p);
		p.PassChar(')');
		sf.body = Statement(p);
	}
	else
	if(p.Char(';'))
		result.Create<Oper>();
	else {
		result = Exp(p);
		p.PassChar(';');
	}
	return result;
}
