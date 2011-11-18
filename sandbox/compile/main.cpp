#include "compile.h"

VectorMap<String, double> var;

double Exp(CParser& p);

double Term(CParser& p)
{
	if(p.Id("abs")) {
		p.PassChar('(');
		double x = Exp(p);
		p.PassChar(')');
		return fabs(x);
	}
	if(p.IsId())
		return var.Get(p.ReadId(), 0);
	if(p.Char('(')) {
		double x = Exp(p);
		p.PassChar(')');
		return x;
	}
	return p.ReadDouble();
}

double Mul(CParser& p)
{
	double x = Term(p);
	for(;;)
		if(p.Char('*'))
			x = x * Term(p);
		else
		if(p.Char('/')) {
			double y = Term(p);
			if(y == 0)
				p.ThrowError("Divide by zero");
			x = x / y;
		}
		else
			return x;
}

double Exp(CParser& p)
{
	double x = Mul(p);
	for(;;)
		if(p.Char('+'))
			x = x + Mul(p);
		else
		if(p.Char('-'))
			x = x - Mul(p);
		else
			return x;
}

CONSOLE_APP_MAIN
{
	try {
		double x, y;
		double sum = 0;
		double step;

#ifdef _DEBUG
		step = 0.1;
#else
		step = 0.001;
#endif
		
		Compiler c;
		c.var.Add("x", &x);
		c.var.Add("y", &y);
		c.var.Add("sum", &sum);
		c.var.Add("step", &step);
		
		CParser p("1 / (1 - x * y + x - y)");
		One<Oper> fn = c.Exp(p);
		
		x = 5;
		y = 10;
		
		RDUMP(1 / (1 - x * y + x - y));
		RDUMP(fn->Execute());

		if(1) {
			RTIMING("Interpreted");
			sum = 0;
			for(x = 0; x < 1; x += step)
				for(y = 0; y < 1; y += step) {
					var.GetAdd("x") = x;
					var.GetAdd("y") = y;
					sum += Exp(CParser("1 / (1 - x * y + x - y)"));
				}
			RDUMP(sum);
		}
		{
			RTIMING("Compiled loop");
			sum = -1;
			String s = LoadFile(GetDataFile("program.txt"));
			CParser p(s);
			c.Statement(p)->Execute();
			RDUMP(sum);
		}
		if(1) {
			RTIMING("Compiled expression");
			sum = 0;
			for(x = 0; x < 1; x += step)
				for(y = 0; y < 1; y += step)
					sum += fn->Execute();
			RDUMP(sum);
		}
		{
			RTIMING("Direct");
			sum = 0;
			for(x = 0; x < 1; x += step)
				for(y = 0; y < 1; y += step)
					sum += 1 / (1 - x * y + x - y);
			RDUMP(sum);
		}
	}
	catch(CParser::Error& error) {
		Cout() << error << "\n";
	}
}
