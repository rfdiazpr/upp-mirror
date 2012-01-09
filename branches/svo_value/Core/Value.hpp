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
	ASSERT(data.IsSpecial((byte)GetValueTypeNo<T>()));
	return *(T*)&data;
}

template <class T>
const T& Value::To() const
{
	if(IsRef())
		return RichValue<T>::Extract(*this);
	return GetSmall<T>();
}

template <class T>
inline bool Value::Is() const
{
	int t = GetValueTypeNo<T>();
	return IsType<T>(*this);
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
