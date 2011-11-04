#include "JSON.h"


String EncodeJson0(const Value& v, const String& sep, bool pretty)
{
	String r;
	if(v.GetType() == VALUEMAP_V) {
		r << "{";
		String sep1;
		if(pretty) {
			r << "\r\n";
			sep1 = sep + '\t';
		}
		ValueMap m = v;
		ValueArray va = m.GetValues();
		for(int i = 0; i < m.GetCount(); i++) {
			if(i) {
				r << ",";
				if(pretty)
					r << "\r\n";
			}
			if(pretty)
				r << sep1;
			r << AsCString((String)m.GetKeys()[i]) << (pretty ? ": " : ":")
			  << EncodeJson0(va[i], sep1, pretty);
		}
		if(pretty)
			r << "\r\n" << sep;
		r << "}";
		return r;
	}
	if(v.GetType() == VALUEARRAY_V) {
		r << "[";
		String sep1;
		if(pretty) {
			r << "\r\n";
			sep1 = sep + '\t';
		}
		ValueArray va = v;
		for(int i = 0; i < va.GetCount(); i++) {
			if(i) {
				r << ",";
				if(pretty)
					r << "\r\n";
			}
			if(pretty)
				r << sep1;
			r << EncodeJson0(va[i], sep1, pretty);
		}
		if(pretty)
			r << "\r\n" << sep;
		r << "]";
		return r;
	}
	return JsonValue(v);
}

String EncodeJson(const Value& v, bool pretty)
{
	return EncodeJson0(v, String(), pretty);
}

String JsonValue(const Value& v)
{
	if(IsNumber(v) && IsNull(v))
		return "null";
	if(v.GetType() == INT_V)
		return Format("%d", (int)v);
	if(v.GetType() == BOOL_V)
		return (bool)v ? "true" : "false";
	if(IsNumber(v))
		return Format("%.16g", (double)v);
	if(IsDateTime(v)) {
		Time t = v;
		return Format("\"%04.4d-%02.2d-%02.2d %02.2d:%02.2d:%02.2d\"",
		              t.year, t.month, t.day, t.hour, t.minute, t.second);
	}
	if(IsString(v))
		return AsCString((String)v);
	NEVER();
	return "null";
}
