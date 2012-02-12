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

template<> inline void Jsonize(JsonIO& io, int& var)
{
	double v = var;
	Jsonize(io, var);
	if(io.IsLoading()) {
		if(v >= INT_MIN && v <= INT_MAX && (int)v == v)
			var = (int)v;
		else
			throw JsonizeError("number is not integer");
	}
}

template<> inline void Jsonize(JsonIO& io, bool& var)
{
	const Value& v = io.Get();
	if(io.IsLoading())
		if(IsNumber(v) && !IsNull(v))
			var = (bool)v;
		else
			throw JsonizeError("boolean expected");
	else
		io.Set(var);
}

template<> inline void Jsonize(JsonIO& io, String& var)
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

template<> inline void Jsonize(JsonIO& io, WString& var)
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

