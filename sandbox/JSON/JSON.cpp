#include "JSON.h"

bool JSONParser::ReadBool()
{
	if(Id("true"))
		return true;
	if(Id("false"))
		return false;
	ThrowError("Excpected 'true' or 'false'");
	return false;
}

bool JSONParser::Bool(bool& b)
{
	if(Id("true")) {
		b = true;
		return true;
	}
	if(Id("false")) {
		b = false;
		return true;
	}
	return false;
}

Value ParseJSON(CParser& p)
{
	if(p.IsNumber())
		return p.ReadDouble();
	if(p.IsString())
		return p.ReadString();
	if(p.Id("null"))
		return Null;
	if(p.Id("true"))
		return true;
	if(p.Id("false"))
		return false;
	if(p.Char('{')) {
		ValueMap m;
		if(!p.IsChar('}'))
			do {
				String key = p.ReadString();
				p.PassChar(':');
				m.Add(key, ParseJSON2(p));
			}
			while(p.Char(','));
		p.PassChar('}');
		return m;
	}
	if(p.Char('[')) {
		ValueArray va;
		if(!p.IsChar(']'))
			do
				va.Add(ParseJSON2(p));
			while(p.Char(','));
		p.PassChar(']');
		return va;		
	}
	p.ThrowError("Unrecognized JSON element");
	return Null;
}

Value ParseJSON(JSONParser& p)
{
	if(p.IsNumber())
		return p.ReadDouble();
	if(p.IsString())
		return p.ReadString();
	if(p.null())
		return Null;
	bool b;
	if(p.Bool(b))
		return b;
	if(p.Object()) {
		ValueMap m;
		while(p.InObject()) {
			String key = p.ReadKey();
			m.Add(key, ParseJSON(p));
		}
		return m;
	}
	if(p.Array()) {
		ValueArray va;
		while(p.InArray())
			va.Add(ParseJSON(p));
		return va;
	}
	p.ThrowError("Unrecognized JSON element");
	return Null;
}

void Json::Set(const String& key, const Value& value)
{
	ValueMap m;
	if(v.GetType() == VALUEMAP_V)
		m = v;
	m.Set(key, value);
	v = m;
}

void Json::Set(int i, const Value& value)
{
	ValueArray va;
	if(v.GetType() == VALUEARRAY_V)
		va = v;
	va.Set(i, value);
	v = va;
}

void Json::Add(const Value& value)
{
	ValueArray va;
	if(v.GetType() == VALUEARRAY_V)
		va = v;
	va.Add(value);
	v = va;
}
