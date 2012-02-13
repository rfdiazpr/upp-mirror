#include "Json.h"

template<> void Jsonize(JsonIO& io, double& var)
{
	if(io.IsLoading()) {
		const Value& v = io.Get();
		if(IsNull(v)) {
			var = Null;
			return;
		}
		if(IsNumber(v)) {
			var = io.Get();
			return;
		}
		if(IsString(v)) {
			double h = ScanDouble((String)v);
			if(!IsNull(h)) {
				var = h;
				return;
			}
		}
		throw JsonizeError("number expected");
	}
	else
		io.Set(var);
}

template<> void Jsonize(JsonIO& io, int& var)
{
	double v = IntDbl(var);
	Jsonize(io, v);
	if(io.IsLoading())
		if(IsNull(v))
			var = Null;
		else
		if(v >= INT_MIN && v <= INT_MAX)
			var = (int)v;
		else
			throw JsonizeError("number is not integer");
}

template<> void Jsonize(JsonIO& io, bool& var)
{
	if(io.IsLoading()) {
		const Value& v = io.Get();
		if(IsNumber(v) && !IsNull(v))
			var = (bool)v;
		else
			throw JsonizeError("boolean expected");
	}
	else
		io.Set(var);
}

template<> void Jsonize(JsonIO& io, int64& var)
{
	if(io.IsLoading()) {
		const Value& v = io.Get();
		if(IsNull(v)) {
			var = Null;
			return;
		}
		if(v.Is<int64>() || v.Is<int>()) {
			var = v;
			return;
		}
		if(IsNumber(v)) {
			double d = v;
			if(d >= INT64_MIN && d <= INT64_MAX) {
				var = (int64)d;
				return;
			}
		}
		else
		if(IsString(v)) {
			int64 h = ScanInt64((String)v);
			if(!IsNull(h)) {
				var = h;
				return;
			}
		}
		throw JsonizeError("invalid int64 value");
	}
	else
		if(IsNull(var))
			io.Set(Null);
		else
		if(var >= INT_MIN && var <= INT_MAX)
			io.Set(var);
		else
			io.Set(AsString(var));
}

template<> void Jsonize(JsonIO& io, String& var)
{
	if(io.IsLoading()) {
		const Value& v = io.Get();
		if(IsString(v))
			var = v;
		else
			throw JsonizeError("string expected");
	}
	else
		io.Set(var);
}

template<> void Jsonize(JsonIO& io, WString& var)
{
	if(io.IsLoading()) {
		const Value& v = io.Get();
		if(IsString(v))
			var = v;
		else
			throw JsonizeError("string expected");
	}
	else
		io.Set(var);
}

template<> void Jsonize(JsonIO& io, Date& var)
{
	if(io.IsLoading()) {
		const Value& v = io.Get();
		if(IsNull(v)) {
			var = Null;
			return;
		}
		if(IsString(v)) {
			Date d = ScanDate("ymd", (String)v);
			if(!IsNull(d)) {
				var = d;
				return;
			}
		}
		throw JsonizeError("string expected for Date value");
	}
	else
		if(IsNull(var))
			io.Set(Null);
		else
			io.Set(Format("%04d-%02d-%02d", var.year, var.month, var.day));
}

template<> void Jsonize(JsonIO& io, Time& var)
{
	if(io.IsLoading()) {
		const Value& v = io.Get();
		if(IsNull(v)) {
			var = Null;
			return;
		}
		if(IsString(v)) {
			Time d = ScanTime("ymd", (String)v);
			if(!IsNull(d)) {
				var = d;
				return;
			}
		}
		throw JsonizeError("string expected for Time value");
	}
	else
		if(IsNull(var))
			io.Set(Null);
		else
			io.Set(Format("%04d-%02d-%02d %02d:%02d:%02d",
				          var.year, var.month, var.day, var.hour, var.minute, var.second));
}

template<> void Jsonize(JsonIO& io, Complex& var)
{
	double r = var.real();
	double i = var.imag();
	io("real", r)("imag", i);
	var = Complex(r, i);
}
