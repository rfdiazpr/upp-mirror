inline
Value::Value(const Value& v)
:	data(String::SPECIAL)
{
	if(v.IsRef() || v.data.IsLarge())
		SetLarge(v);
	else
		data.SetSmall(v.data);
}

template <class T>
struct SvoFn {
	static bool       IsNull(const void *p)                      { return UPP::IsNull(*(T *)p); }
	static void       Serialize(void *p, Stream& s)              { s % *(T*)p; }
	static unsigned   GetHashValue(const void *p)                { return UPP::ValueGetHashValue(*(T*)p); }
	static bool       IsEqual(const void *p1, const void *p2)    { return *(T*)p1 == *(T*)p2; }
	static bool       IsPolyEqual(const void *p, const Value& v) { return UPP::IsPolyEqual(*(T*)p, v); }
	static String     AsString(const void *p)                    { return UPP::AsString(*(T*)p); }
};

#define SVO_FN(id, T) \
	static Value::Sval id[] = { \
		SvoFn<T>::IsNull, SvoFn<T>::Serialize, SvoFn<T>::GetHashValue, SvoFn<T>::IsEqual, \
		SvoFn<T>::IsPolyEqual, SvoFn<T>::AsString \
	};

template <class T>
void Value::InitSmall(const T& init)
{
	ASSERT(sizeof(T) <= 8);
	SVO_FN(sval, T)
	int typeno = GetValueTypeNo<T>();
	ASSERT(typeno >= 0 && typeno < 256);
	svo[typeno] = sval;
	data.SetSpecial(typeno);
	new(&data) T(init);
}

template <class T>
T& Value::GetSmall() const
{
	dword t = GetValueTypeNo<T>();
	ASSERT(t < 255 && (t == STRING_V ? IsString() : Is((byte)t)));
	return *(T*)&data;
}

template <class T>
void Value::SvoRegister()
{
	dword t = GetValueTypeNo<T>();
	ASSERT(t < 255);
	SVO_FN(sval, T)
	svo[t] = sval;
}

template <class T>
const T& Value::To() const
{
	dword t = GetValueTypeNo<T>();
	ASSERT(t != VALUEARRAY_V && t != VALUEMAP_V);
#ifndef _DEBUG
	if(t == VALUEARRAY_V) {
		ASSERT(ptr()->GetType() == VALUEARRAY_V);
		return *(T*)this; // Illegal, but works -> better than crash in release mode
	}
	if(t == VALUEMAP_V) {
		ASSERT(ptr()->GetType() == VALUEMAP_V);
		return *(T*)this; // Illegal, but works -> better than crash in release mode
	}
#endif
	if(t == STRING_V) {
		ASSERT(IsString());
		return *reinterpret_cast<const T*>(&data); // Only active when T is String
	}
	if(t == INT_V || t == INT64_V || t == DOUBLE_V || t == BOOL_V ||
	   t == DATE_V || t == TIME_V || !IsRef()) {
		dword t = GetValueTypeNo<T>();
		ASSERT(t < 255 && (t == STRING_V ? IsString() : Is((byte)t)));
		return *(T*)&data;
	}
	return RichValue<T>::Extract(*this);
}

template <class T>
inline bool Value::Is() const
{
	dword t = GetValueTypeNo<T>();
	if(t > 0x80000000)
		return IsRef() && dynamic_cast<const RawValueRep<T> *>(GetVoidPtr());
	if(t == STRING_V)
		return IsString();
	if(t == VOID_V)
		return IsVoid();
	if(t == INT_V || t == INT64_V || t == DOUBLE_V || t == BOOL_V ||
	   t == DATE_V || t == TIME_V)
	   	return Is((byte)t);
	return t < 255 && Is((byte)t) || IsRef() && ptr()->GetType() == t;
}

template <class T>
Value::Value(const T& x, bool)
{
	InitSmall(x);
}

inline
unsigned Value::GetHashValue() const
{
	return IsString() ? data.GetCount() ? data.GetHashValue() : 0
	                  : GetOtherHashValue();
}

template <class T>
void Value::Register()
{
	RichValue<T>::Register();
}

template <class T> // Deprecated
bool IsTypeRaw(const Value& value, T * = 0) {
	return value.Is<T>();
}

template <class T> // deprecated, use member Is
bool IsType(const Value& x, T* = 0)           { return x.Is<T>(); }

inline
int Value::GetCount() const
{
	if(IsRef()) {
		if(ptr()->GetType() == VALUEARRAY_V)
			return ((ValueArray::Data *)ptr())->data.GetCount();
		if(ptr()->GetType() == VALUEMAP_V)
			return ((ValueMap::Data *)ptr())->value.GetCount();
	}
	return 0;
}

inline
const Value& Value::operator[](int i) const
{
	ASSERT(IsValueArray(*this));
	return ((ValueArray::Data *)ptr())->data[i];
}

inline
const Value& Value::operator[](const String& key) const
{
	return IsValueMap(*this) ? ValueMap(*this)[key] : ErrorValue();
}

inline
const Value& Value::operator[](const char *key) const
{
	return IsValueMap(*this) ? ValueMap(*this)[key] : ErrorValue();
}

inline
const Value& Value::operator[](const Id& key) const
{
	return IsValueMap(*this) ? ValueMap(*this)[key] : ErrorValue();
}
