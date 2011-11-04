#ifndef _JSON_JSON_h_
#define _JSON_JSON_h_

#include <Core/Core.h>

using namespace Upp;

class JSONParser : public CParser {
public:
	bool   Isnull()                { return IsId("null"); }
	bool   null()                  { return Id("null"); }

	bool   IsBool()                { return IsId("true") || IsId("false"); }
	bool   Bool(bool& b);
	bool   ReadBool();

	bool   IsObject()              { return IsChar('{'); }
	bool   Object()                { return Char('{'); }
	bool   InObject()              { Char(','); return !Char('}'); }
	String ReadKey()               { String s = ReadString(); PassChar(':'); return s; }
	
	bool   IsArray()               { return IsChar('['); }
	bool   Array()                 { return Char('['); }
	bool   InArray()               { Char(','); return !Char(']'); }

	JSONParser(const char *ptr) : CParser(ptr) {}
	JSONParser(const char *ptr, const char *fn, int line = 1) : CParser(ptr, fn, line) {}
	JSONParser() {}
};

Value ParseJSON(JSONParser& p);

String JsonValue(const Value& v);

struct JsonArray;

struct JsonObject {
	String text;

	String ToString() const    { return "{" + text + "}"; }
	String operator~() const   { return ToString(); }
	operator String() const    { return ToString(); }
	
	JsonObject& CatRaw(const char *key, const String& val) {
		if(text.GetCount())
			text << ',';
		text << '\"' << key << "\":" << val;
		return *this;		
	}

	JsonObject& operator()(const char *key, const Value& value)       { return CatRaw(key, JsonValue(value)); }
	JsonObject& operator()(const char *key, const JsonObject& object) { return CatRaw(key, ~object); }
	JsonObject& operator()(const char *key, const JsonArray& array);
		
	JsonObject() {}
	JsonObject(const char *key, const Value& value)                   { CatRaw(key, JsonValue(value)); }
};

struct JsonArray {
	String text;

	String ToString() const    { return "[" + text + "]"; }
	String operator ~() const  { return ToString(); }
	operator String() const    { return ToString(); }
	
	JsonArray& CatRaw(const String& val) {
		if(text.GetCount())
			text << ',';
		text << val;
		return *this;		
	}

	JsonArray& operator<<(const Value& value)       { return CatRaw(JsonValue(value)); }
	JsonArray& operator<<(const JsonObject& object) { return CatRaw(~object); }
	JsonArray& operator<<(const JsonArray& array)   { return CatRaw(~array); }
		
	JsonArray() {}
};

String EncodeJson(const Value& v, bool pretty);

struct JsonString : String {
	JsonString& operator()(const String& key, const Value& value);
	JsonString& operator<<(const Value& value);
	JsonString& Object();
	JsonString& EndObject();
	JsonString& Array();
	JsonString& EndArray();
	
	JsonString(bool pretty);
};

inline JsonObject& JsonObject::operator()(const char *key, const JsonArray& array)
{
	return CatRaw(key, array);
}

#endif
