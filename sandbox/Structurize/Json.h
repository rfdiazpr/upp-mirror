#ifndef _Structurize_Json_h_
#define _Structurize_Json_h_

#include <Core/Core.h>

using namespace Upp;

class JsonIO {
	const Value   *src;
	One<ValueMap>  map;
	Value          tgt;

public:
	bool IsLoading() const                       { return src; }
	bool IsStoring() const                       { return !src; }
	
	const Value& Get() const                     { ASSERT(IsLoading()); return *src; }
	void         Set(const Value& v)             { ASSERT(IsStoring() && !map); tgt = v; }
	void         Put(Value& v)                   { ASSERT(IsStoring()); if(map) v = *map; else v = tgt; }
	Value        GetResult() const               { ASSERT(IsStoring()); return map ? Value(*map) : tgt; }

	template <class T>
	JsonIO& operator()(const char *key, T& value);
	
	JsonIO(const Value& src) : src(&src)         {}
	JsonIO()                                     { src = NULL; }
};

template <class T>
void Jsonize(JsonIO& io, T& var)
{
	var.Jsonize(io);
}

template <class T>
JsonIO& JsonIO::operator()(const char *key, T& value)
{
	if(IsLoading()) {
		const Value& v = (*src)[key];
		if(!v.IsVoid()) {
			JsonIO jio(v);
			Jsonize(jio, value);
		}
	}
	else {
		ASSERT(tgt.IsVoid());
		if(!map)
			map.Create();
		JsonIO jio;
		Jsonize(jio, value);
		if(jio.map)
			map->Add(key, *jio.map);
		else
			map->Add(key, jio.tgt);
	}
	return *this;
}

template <class T>
Value StoreAsJsonValue(const T& var)
{
	JsonIO io;
	Jsonize(io, const_cast<T&>(var));
	return io.GetResult();
}

template <class T>
void LoadFromJsonValue(T& var, const Value& x)
{
	JsonIO io(x);
	Jsonize(io, var);
}

template <class T>
String StoreAsJson(const T& var)
{
	return AsJSON(StoreAsJsonValue(var));
}

template <class T>
bool LoadFromJson(T& var, const char *json)
{
	try {
		LoadFromJsonValue(var, ParseJSON(json));
	}
	catch(CParser::Error& e) {
		return false;
	}
	return true;
}

template <class T>
void JsonizeValue(JsonIO& io, T& var)
{ // bad code
	if(io.IsLoading())
		var = io.Get();
	else
		io.Set(var);
}

template<> inline void Jsonize(JsonIO& io, int& var)      { JsonizeValue(io, var); } // Check for correct types
template<> inline void Jsonize(JsonIO& io, double& var)   { JsonizeValue(io, var); }
template<> inline void Jsonize(JsonIO& io, bool& var)     { JsonizeValue(io, var); }
template<> inline void Jsonize(JsonIO& io, String& var)   { JsonizeValue(io, var); }

template <class T, class V>
void JsonizeArray(JsonIO& io, T& array)
{
	if(io.IsLoading()) {
		const Value& va = io.Get();
		array.SetCount(va.GetCount());
		for(int i = 0; i < va.GetCount(); i++) {
			JsonIO jio(va[i]);
			Jsonize(jio, array[i]);
		}
	}
	else {
		Vector<Value> va;
		va.SetCount(array.GetCount());
		for(int i = 0; i < array.GetCount(); i++) {
			JsonIO jio;
			Jsonize(jio, array[i]);
			jio.Put(va[i]);
		}
		io.Set(ValueArray(va));
	}
}

template <class T>
void Jsonize(JsonIO& io, Vector<T>& var)
{
	JsonizeArray<Vector<T>, T>(io, var);
}

template <class T>
void Jsonize(JsonIO& io, Array<T>& var)
{
	JsonizeArray<Array<T>, T>(io, var);
}

template <class T, class K, class V>
void JsonizeMap(JsonIO& io, T& map, const char *keyid, const char *valueid)
{
	if(io.IsLoading()) {
		map.Clear();
		const Value& va = io.Get();
		map.Reserve(va.GetCount());
		for(int i = 0; i < va.GetCount(); i++) {
			K key;
			V value;
			LoadFromJsonValue(key, va[i][keyid]);
			LoadFromJsonValue(value, va[i][valueid]);	
			map.Add(key, value);
		}
	}
	else {
		Vector<Value> va;
		va.SetCount(map.GetCount());
		for(int i = 0; i < map.GetCount(); i++)
			if(!map.IsUnlinked(i)) {
				ValueMap item;
				item.Add(keyid, StoreAsJsonValue(map.GetKey(i)));
				item.Add(valueid, StoreAsJsonValue(map[i]));
				va[i] = item;
			}
		io.Set(ValueArray(va));
	}
}

template <class K, class V, class H>
void Jsonize(JsonIO& io, VectorMap<K, V, H>& map)
{
	JsonizeMap(io, map, "key", "value");
}

template <class K, class V, class H>
void Jsonize(JsonIO& io, ArrayMap<K, V, H>& map)
{
	JsonizeMap<ArrayMap<K, V, H>, K, V>(io, map, "key", "value");
}

template <class T, class V>
void JsonizeIndex(JsonIO& io, T& index)
{
	if(io.IsLoading()) {
		const Value& va = io.Get();
		index.Reserve(va.GetCount());
		for(int i = 0; i < va.GetCount(); i++) {
			V v;
			LoadFromJsonValue(v, va[i]);
			index.Add(v);
		}
	}
	else {
		Vector<Value> va;
		for(int i = 0; i < index.GetCount(); i++)
			if(!index.IsUnlinked(i))
				va.Add(StoreAsJsonValue(index[i]));
		io.Set(ValueArray(va));
	}
}

template <class T>
void Jsonize(JsonIO& io, Index<T>& var)
{
	JsonizeIndex<Index<T>, T>(io, var);
}

template <class T>
void Jsonize(JsonIO& io, ArrayIndex<T>& var)
{
	JsonizeIndex<ArrayIndex<T>, T>(io, var);
}

#endif
