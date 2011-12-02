struct Exe {
	virtual Value Eval(Vector<Value>& stack) const { return Value(); }
	virtual ~Exe() {}
};

struct Compiler {
	static bool IsTrue(const Value& v);
	
	struct Exe1 : Exe {
		One<Exe> a;
	};

	template <class T>
	static One<Exe> Create(One<Exe> a) {
		One<Exe> rr;
		T& m = rr.Create<T>();
		m.a = a;
		return rr;
	}

	struct Exe2 : Exe {
		One<Exe> a;
		One<Exe> b;
	};

	template <class T>
	static One<Exe> Create(One<Exe> a, One<Exe> b) {
		One<Exe> rr;
		T& m = rr.Create<T>();
		m.a = a;
		m.b = b;
		return rr;
	}

	struct ExeVar : Exe {
		int var_index;

		virtual Value Eval(Vector<Value>& stack) const;
	};
	struct ExeConst : Exe {
		Value value;
		
		virtual Value Eval(Vector<Value>& stack) const;
	};

	struct ExeNot : Exe1 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeNeg : Exe1 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeMul : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeDiv : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeMod : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeAdd : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeSub : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeSll : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeSra : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeSrl : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeLt  : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeLte : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeEq  : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeNeq : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeAnd : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeXor : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeOr  : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeAnl : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	struct ExeOrl : Exe2 { virtual Value Eval(Vector<Value>& stack) const; };
	
	struct ExeCond : Exe {
		One<Exe> cond;
		One<Exe> ontrue;
		One<Exe> onfalse;
		
		virtual Value Eval(Vector<Value>& stack) const;
	};

	struct CompiledTemplate {
		String path;
		Index<String> var;

		Exe    program;
		
		Vector<Value> data;
		Value Eval();
	};
	
	CParser p;
	const Index<String>& var;

	One<Exe> Prim();
	One<Exe> Mul();
	One<Exe> Add();
	One<Exe> Shift();
	One<Exe> Rel();
	One<Exe> Eq();
	One<Exe> And();
	One<Exe> Xor();
	One<Exe> Or();
	One<Exe> LogAnd();
	One<Exe> LogOr();
	One<Exe> Conditional();
	
	One<Exe> Exp();
	
	Compiler(const char *code, const Index<String>& var) : p(code), var(var) {}
};

One<Exe> Compile(const char *code, const Index<String>& vars);

Value Run(const One<Exe>& exe, Vector<Value>& vars);