template <class T> // deprecated, use member Is
bool IsType(const Value& x, T* = 0)           { return x.Is<T>(); }

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
void Value::InitSmall0(const T& init)
{
	int typeno = GetValueTypeNo<T>();
	ASSERT(typeno >= 0 && typeno < 256);
	data.SetSpecial(typeno);
	new(&data) T(init);
}

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
T Value::To() const
{
	dword t = GetValueTypeNo<T>();
	if(t == VALUEARRAY_V)
		return *(T*)&ValueArray(*this);
	if(t == VALUEMAP_V)
		return *(T*)&ValueMap(*this);
	if(IsRef())
		return RichValue<T>::Extract(*this);
	return GetSmall<T>();
}

template <class T>
inline bool Value::Is() const
{
	dword t = GetValueTypeNo<T>();
	if(t == STRING_V)
		return IsString();
	if(t == VOID_V)
		return IsVoid();
	if(t == INT_V || t == INT64_V || t == DOUBLE_V || t == BOOL_V ||
	   t == DATE_V || t == TIME_V)
	   	return Is((byte)t);
	return t < 255 && Is((byte)t) || Is(REF) && ptr()->GetType() == t;
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
