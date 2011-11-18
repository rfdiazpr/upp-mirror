#include <Core/Core.h>

using namespace Upp;

struct Oper {
	virtual double Execute() { return 0; }
	virtual ~Oper() {}
};

struct BinOper : Oper {
	One<Oper> a;
	One<Oper> b;
};

struct ExeAdd : BinOper {
	virtual double Execute() { return a->Execute() + b->Execute(); }
};

struct ExeSub : BinOper {
	virtual double Execute() { return a->Execute() - b->Execute(); }
};

struct ExeMul : BinOper {
	virtual double Execute() { return a->Execute() * b->Execute(); }
};

struct ExeDiv : BinOper {
	virtual double Execute() { return a->Execute() / b->Execute(); }
};

struct ExeLt : BinOper {
	virtual double Execute() { return a->Execute() < b->Execute(); }
};

struct ExeLte : BinOper {
	virtual double Execute() { return a->Execute() <= b->Execute(); }
};

struct ExeEq : BinOper {
	virtual double Execute() { return a->Execute() == b->Execute(); }
};

struct ExeNeq : BinOper {
	virtual double Execute() { return a->Execute() != b->Execute(); }
};

struct ExeAnd : BinOper {
	virtual double Execute() { return (int)a->Execute() && (int)b->Execute(); }
};

struct ExeOr : BinOper {
	virtual double Execute() { return (int)a->Execute() || (int)b->Execute(); }
};

struct ExeNot : Oper {
	One<Oper> val;
	virtual double Execute() { return !(int)val->Execute(); }
};

struct ExeNeg : Oper {
	One<Oper> val;
	virtual double Execute() { return -(int)val->Execute(); }
};

struct ExeConst : Oper {
	double value;
	virtual double Execute() { return value; }
};

struct ExeVar : Oper {
	double *var;
	virtual double Execute() { return *var; }
};

struct ExeAssign : Oper {
	double   *var;
	One<Oper> val;
	virtual double Execute() { *var = val->Execute(); return *var; }
};

struct ExeBlock : Oper {
	Array<Oper> statement;
	virtual double Execute() { for(int i = 0; i < statement.GetCount(); i++) statement[i].Execute(); return 0; }
};

struct ExeIf : Oper {
	One<Oper> cond;
	One<Oper> body;
	One<Oper> else_body;
	virtual double Execute() { if(body->Execute()) body->Execute(); else else_body->Execute(); return 0; }
};

struct ExeWhile : Oper {
	One<Oper> cond;
	One<Oper> body;
	virtual double Execute() { while(cond->Execute()) body->Execute(); return 0; }
};

struct ExeFor : Oper {
	One<Oper> init;
	One<Oper> cond;
	One<Oper> body;
	One<Oper> after;
	virtual double Execute() { init->Execute(); while(cond->Execute()) { body->Execute(); after->Execute(); } return 0; }
};

struct Compiler {
	VectorMap<String, double *> var;
	
	template <class T>
	static One<Oper> Binary(One<Oper> first, One<Oper> second) {
		One<Oper> rr;
		T& m = rr.Create<T>();
		m.a = first;
		m.b = second;
		return rr;
	}

	One<Oper> Prim(CParser& p);
	One<Oper> Add(CParser& p);
	One<Oper> Mul(CParser& p);
	One<Oper> Cond(CParser& p);
	One<Oper> And(CParser& p);
	One<Oper> Or(CParser& p);
	One<Oper> Exp(CParser& p);
	One<Oper> Statement(CParser& p);
};
