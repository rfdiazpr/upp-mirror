struct Exe {
	virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const { return Value(); }
	virtual ~Exe() {}
};

struct Compiler {
	static VectorMap<String, Value (*)(const Vector<Value>& arg)> functions;

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

		virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const;
	};

	struct ExeConst : Exe {
		Value value;
		
		virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const;
	};
	
	struct ExeNot : Exe1 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeNeg : Exe1 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeMul : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeDiv : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeMod : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeAdd : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeSub : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeSll : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeSra : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeSrl : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeLt  : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeLte : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeEq  : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeNeq : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeAnd : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeXor : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeOr  : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeAnl : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeOrl : Exe2 { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	
	struct ExeCond : Exe {
		One<Exe> cond;
		One<Exe> ontrue;
		One<Exe> onfalse;
		
		virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const;
	};

	struct ExeField : Exe {
		One<Exe> value;
		String   id;

		virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const;
	};
	
	struct ExeFn : Exe {
		Value (*fn)(const Vector<Value>& arg);
		
		Array<Exe> arg;

		virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const;
	};
	
	struct LoopInfo {
		bool  first;
		bool  last;
		int   index;
		Value key;
	};
	
	struct ExeFirst : ExeVar { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeLast  : ExeVar { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeIndex : ExeVar { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	struct ExeKey   : ExeVar { virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const; };
	
	struct ExeFor : Exe {
		One<Exe> value;
		One<Exe> body;
		One<Exe> onempty;

		virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const;
	};

	struct ExeBlock : Exe {
		Array<Exe> item;
		
		void AddText(const char *b, const char *s);

		virtual Value Eval(Vector<Value>& stack, StringBuffer& out) const;
	};
	
	struct CompiledTemplate {
		String path;
		Index<String> var;

		Exe    program;
		
		Vector<Value> data;
		Value Eval();
	};
	
	CParser       p;
	Index<String> var;
	Vector<bool>  forvar;

	int ForVar(String id, int i);

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
	
	One<Exe> Block();
	
	static void Register(const String& id, Value (*fn)(const Vector<Value>& arg));
	
	Compiler(const char *code, const Index<String>& var) : p(code), var(var, 1) { forvar.SetCount(var.GetCount(), false); }
};

One<Exe> Compile(const char *code, const Index<String>& vars);

String Render(const One<Exe>& exe, Vector<Value>& var);
