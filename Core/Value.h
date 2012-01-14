class Id;
class ValueArray;
class ValueMap;

const dword VOID_V    = 0;

const dword INT_V     = 1;
const dword DOUBLE_V  = 2;
const dword STRING_V  = 3;
const dword DATE_V    = 4;
const dword TIME_V    = 5;

const dword ERROR_V   = 6;

const dword VALUE_V   = 7;

const dword WSTRING_V = 8;

const dword VALUEARRAY_V = 9;

const dword INT64_V  = 10;
const dword BOOL_V   = 11;

const dword VALUEMAP_V   = 12;

const dword UNKNOWN_V = (dword)0xffffffff;

template <class T>
inline dword ValueTypeNo(const T *)       { return StaticTypeNo<T>() + 0x8000000;; }

template<> inline dword ValueTypeNo(const int*)     { return INT_V; }
template<> inline dword ValueTypeNo(const int64*)   { return INT64_V; }
template<> inline dword ValueTypeNo(const double*)  { return DOUBLE_V; }
template<> inline dword ValueTypeNo(const bool*)    { return BOOL_V; }
template<> inline dword ValueTypeNo(const String*)  { return STRING_V; }
template<> inline dword ValueTypeNo(const WString*) { return WSTRING_V; }
template<> inline dword ValueTypeNo(const Date*)    { return DATE_V; }
template<> inline dword ValueTypeNo(const Time*)    { return TIME_V; }

template <class T, dword type, class B = EmptyClass>
class AssignValueTypeNo : public B {
public:
	friend dword ValueTypeNo(const T*)              { return type; }

	void operator=(const AssignValueTypeNo&) {} // MSC 6.0 empty base class bug fix
};

template <class T>
dword GetValueTypeNo() { return ValueTypeNo((T*)NULL); }

class Value : Moveable<Value> {
public:
	class Void {
	protected:
		Atomic  refcount;

	public:
		void               Retain()                    { AtomicInc(refcount); }
		void               Release()                   { if(AtomicDec(refcount) == 0) delete this; }

		virtual dword      GetType() const             { return VOID_V; }
		virtual bool       IsNull() const              { return true; }
		virtual void       Serialize(Stream& s)        {}
		virtual unsigned   GetHashValue() const        { return 0; }
		virtual bool       IsEqual(const Void *p)      { return false; }
		virtual bool       IsPolyEqual(const Value& v) { return false; }
		virtual String     AsString() const            { return ""; }

		Void()                                         { AtomicWrite(refcount, 1); }
		virtual ~Void()                                {}

		friend class Value;
	};

	struct Sval {
		bool       (*IsNull)(const void *p);
		void       (*Serialize)(void *p, Stream& s);
		unsigned   (*GetHashValue)(const void *p);
		bool       (*IsEqual)(const void *p1, const void *p2);
		bool       (*IsPolyEqual)(const void *p, const Value& v);
		String     (*AsString)(const void *p);
	};
	
protected:
	enum { STRING = 0, REF = 255, VOIDV = 3 };

	static VectorMap<dword, Void* (*)(Stream& s) >& Typemap();
	static Sval *svo[256];

	String   data;
	Void    *&ptr()                  { ASSERT(IsRef()); return *(Void **)&data; }
	Void     *ptr() const            { ASSERT(IsRef()); return *(Void **)&data; }
	
	bool     IsString() const        { return data.IsString(); }
	bool     Is(byte v) const        { return data.IsSpecial(v); }
	bool     IsRef() const           { return Is(REF); }
	void     InitRef(Void *p)        { data.SetSpecial(REF); ptr() = p; }
	void     RefRelease();
	void     RefRetain();
	void     Free()                  { if(IsRef()) RefRelease(); }
	void     SetLarge(const Value& v);

	template <class T>
	void     InitSmall(const T& init);
	template <class T>
	T&       GetSmall() const;
	
	int      GetOtherInt() const;
	int64    GetOtherInt64() const;
	double   GetOtherDouble() const;
	bool     GetOtherBool() const;
	Date     GetOtherDate() const;
	Time     GetOtherTime() const;
	String   GetOtherString() const;
	unsigned GetOtherHashValue() const;

	bool     IsPolyEqual(const Value& v) const;

	template <class T>
	Value(const T& value, bool pod);
	
	template <class T> friend Value SvoValue(const T& x);

public:
	static  void Register(dword w, Void* (*c)(Stream& s)) init_;

	template <class T>
	static  void Register();
	template <class T>
	static  void SvoRegister();
	
	dword    GetType() const;
	bool     IsError() const         { return GetType() == ERROR_V; }
	bool     IsVoid() const          { return Is(VOIDV) || IsError(); }
	bool     IsNull() const;

	template <class T>
	bool     Is() const;
	template <class T>
	const T& To() const;

	operator String() const          { return IsString() ? data : GetOtherString(); }
	operator WString() const;
	operator Date() const            { return Is(DATE_V) ? GetSmall<Date>() : GetOtherDate(); }
	operator Time() const            { return Is(TIME_V) ? GetSmall<Time>() : GetOtherTime(); }
	operator double() const          { return Is(DOUBLE_V) ? GetSmall<double>() : GetOtherDouble(); }
	operator int() const             { return Is(INT_V) ? GetSmall<int>() : GetOtherInt(); }
	operator int64() const           { return Is(INT64_V) ? GetSmall<int64>() : GetOtherInt64(); }
	operator bool() const            { return Is(BOOL_V) ? GetSmall<bool>() : GetOtherBool(); }

	Value(const String& s) : data(s) {}
	Value(const WString& s);
	Value(const char *s) : data(s)   {}
	Value(int i)                     : data(i, INT_V, String::SPECIAL) {}
	Value(int64 i)                   : data(i, INT64_V, String::SPECIAL) {}
	Value(double d)                  : data(d, DOUBLE_V, String::SPECIAL) {}
	Value(bool b)                    : data(b, BOOL_V, String::SPECIAL) {}
	Value(Date d)                    : data(d, DATE_V, String::SPECIAL) {}
	Value(Time t)                    : data(t, TIME_V, String::SPECIAL) {}
	Value(const Nuller&)             : data((int)Null, INT_V, String::SPECIAL) {}

	bool operator==(const Value& v) const;
	bool operator!=(const Value& v) const { return !operator==(v); }

	String ToString() const;

	void  Serialize(Stream& s);

	unsigned GetHashValue() const;

	Value& operator=(const Value& v);
	Value(const Value& v);
	
	int   GetCount() const;
	const Value& operator[](int i) const;
	const Value& operator[](const String& key) const;
	const Value& operator[](const char *key) const;
	const Value& operator[](const Id& key) const;

	Value()                               : data((int)Null, VOIDV, String::SPECIAL) {}
	~Value()                              { if(IsRef()) RefRelease(); }

	template <class T>
	static bool FitsSvo()                 { return sizeof(T) <= 8; }

	Value(Void *p)                        { InitRef(p); }
	const Void *GetVoidPtr() const        { ASSERT(IsRef()); return ptr(); }

	friend void Swap(Value& a, Value& b)  { Swap(a.data, b.data); }
};

template <class T>
Value SvoValue(const T& x) {
	return Value(x, false);
}

#define VALUE_COMPARE(T) \
inline bool operator==(const Value& v, T x)   { return (T)v == x; } \
inline bool operator==(T x, const Value& v)   { return (T)v == x; } \
inline bool operator!=(const Value& v, T x)   { return (T)v != x; } \
inline bool operator!=(T x, const Value& v)   { return (T)v != x; } \

VALUE_COMPARE(int)
VALUE_COMPARE(int64)
VALUE_COMPARE(double)
VALUE_COMPARE(bool)
VALUE_COMPARE(Date)
VALUE_COMPARE(Time)
VALUE_COMPARE(String)
VALUE_COMPARE(WString)

inline bool operator==(const Value& v, const char *x)   { return (String)v == x; }
inline bool operator==(const char *x, const Value& v)   { return (String)v == x; }
inline bool operator!=(const Value& v, const char *x)   { return (String)v != x; }
inline bool operator!=(const char *x, const Value& v)   { return (String)v != x; }

inline bool operator==(const Value& v, const wchar *x)  { return (WString)v == x; }
inline bool operator==(const wchar *x, const Value& v)  { return (WString)v == x; }
inline bool operator!=(const Value& v, const wchar *x)  { return (WString)v != x; }
inline bool operator!=(const wchar *x, const Value& v)  { return (WString)v != x; }

inline bool IsVoidValueTypeNo(int q)     { return (dword)q == VOID_V; }
inline bool IsErrorValueTypeNo(int q)    { return (dword)q == ERROR_V; }
inline bool IsStringValueTypeNo(int q)   { return (dword)q == STRING_V || (dword)q == WSTRING_V; }

inline bool IsIntegerValueTypeNo(int q)  { return (dword)q == INT_V || (dword)q == INT64_V || (dword)q == BOOL_V; }
inline bool IsFloatValueTypeNo(int q)    { return (dword)q == DOUBLE_V; }

inline bool IsNumberValueTypeNo(int q)   { return IsIntegerValueTypeNo(q) || IsFloatValueTypeNo(q); }
inline bool IsDateTimeValueTypeNo(int q) { return (dword)q == DATE_V || (dword)q == TIME_V; }

inline bool IsVoid(const Value& v)     { return v.IsVoid(); }
inline bool IsError(const Value& v)    { return v.IsError(); }
inline bool IsString(const Value& v)   { return v.Is<String>() || v.Is<WString>(); }
inline bool IsNumber(const Value& v)   { return v.Is<double>() || v.Is<int>() || v.Is<int64>() || v.Is<bool>(); }
inline bool IsDateTime(const Value& v) { return v.Is<Date>() || v.Is<Time>(); }

template <class T>
class RawValueRep : public Value::Void {
protected:
	T v;

public:
	virtual dword GetType() const             { return GetValueTypeNo<T>(); }
	virtual bool  IsNull() const              { return false; }

	const T& Get() const                      { return v; }
	RawValueRep(const T& v) : v(v)            {}
	RawValueRep()                             {}
	static const RawValueRep *Cast(const Value::Void *p) {
		ASSERT_(dynamic_cast<const RawValueRep *>(p),
		        String().Cat() << "Invalid value conversion: "
		                       << typeid(*p).name() << " -> " << typeid(T).name());
		return (const RawValueRep *) p;
	}
};

template <class T>
class RawValue : public Value {
protected:
	typedef RawValueRep<T> Rep;

	RawValue(Rep *rep) : Value(rep) {}

public:
	RawValue(const T& x) : Value(new Rep(x)) {}
	static const T& Extract(const Value& v) {
		return Rep::Cast(v.GetVoidPtr())->Get();
	}
	static const T& Extract(const Value& v, const T& dflt) {
		return v.Is<T>() ? Rep::Cast(v.GetVoidPtr())->Get() : dflt;
	}
};

template <class T>
class RawPickValueRep : public RawValueRep<T> {
public:
	RawPickValueRep(pick_ T& _v)      { this->v = _v; }
	RawPickValueRep(const T& _v, int) { this->v <<= _v; }
	RawPickValueRep()                 {}
};

template <class T>
class RawPickValue : public RawValue<T> {
protected:
	typedef RawPickValueRep<T> PickRep;

public:
	RawPickValue(pick_ T& x) : RawValue<T>(new PickRep(x)) {}
	RawPickValue(const T& x, int) : RawValue<T>(new PickRep(x, 0)) {}
};

template <class T>
class RawValueCreateRep : public RawValueRep<T> {
public:
	T& Get()                        { return this->v; }
};

template <class T>
class RawValueCreate : public RawValue<T> {
protected:
	typedef RawValueCreateRep<T> Rep;

public:
	RawValueCreate() : RawValue<T>(new Rep()) {}
	T& Get()                                     { return ((Rep *)this->GetVoidPtr())->Get(); }
};

template <class T>
inline Value RawToValue(const T& data)           { return RawValue<T>(data); }

template <class T>
inline Value RawPickToValue(pick_ T& data)       { return RawPickValue<T>(data); }

template <class T>
inline Value RawDeepToValue(const T& data)       { return RawPickValue<T>(data, 0); }

template <class T>
inline T& CreateRawValue(Value& v) {
	RawValueCreate<T> x;
	v = x;
	return x.Get();
}

template <class T>
inline const T& ValueTo(const Value& v, T * = 0)       { return RawValue<T>::Extract(v); }

template <class T>
inline const T& ValueTo(const Value& v, const T& dflt) { return RawValue<T>::Extract(v, dflt); }

template <class T>
inline bool IsPolyEqual(const T& x, const Value& v) {
	return false;
}

template <class T>
inline unsigned ValueGetHashValue(const T& x) {
	return UPP::GetHashValue(x);
}

inline bool IsPolyEqual(const bool& x, const Value& v) {
	return v.Is<double>() && int(x) == double(v)
	    || v.Is<int64>() && int(x) == int64(v)
	    || v.Is<int>() && int(x) == int(v);
}

inline bool IsPolyEqual(const int& x, const Value& v) {
	return v.Is<double>() && x == double(v)
	    || v.Is<int64>() && x == int64(v);
}

inline bool IsPolyEqual(const int64& x, const Value& v) {
	return v.Is<double>() && double(x) == double(v);
}

inline bool IsPolyEqual(const Date& x, const Value& v) {
	return v.Is<Time>() && ToTime(x) == Time(v);
}

inline bool IsPolyEqual(const WString& x, const Value& v) {
	return v.GetType() == STRING_V && WString(v) == x;
}

inline unsigned ValueGetHashValue(const bool& x) {
	return UPP::GetHashValue((int64)x);
}

inline unsigned ValueGetHashValue(const int& x) {
	return UPP::GetHashValue((int64)x);
}

inline unsigned ValueGetHashValue(const double& x) {
	if(x >= INT64_MIN && x <= INT64_MAX && (int64)x == x)
		return UPP::GetHashValue((int64)x);
	return UPP::GetHashValue(x);
}

inline unsigned ValueGetHashValue(const Date& x) {
	return UPP::GetHashValue(ToTime(x));
}

inline unsigned ValueGetHashValue(const WString& x) {
	return UPP::GetHashValue(x.ToString());
}

template <class T>
class RichValueRep : public RawValueRep<T> {
public:
	virtual bool       IsNull() const                { return UPP::IsNull(this->v); }
	virtual void       Serialize(Stream& s)          { s % this->v; }
	virtual unsigned   GetHashValue() const          { return UPP::ValueGetHashValue(this->v); }
	virtual bool       IsEqual(const Value::Void *p) { return Cast(p)->Get() == this->v; }
	virtual bool       IsPolyEqual(const Value& b)   { return UPP::IsPolyEqual(this->v, b); }
	virtual String     AsString() const              { return UPP::AsString(this->v); }

	RichValueRep(const T& v) : RawValueRep<T>(v)     {}
	RichValueRep(Stream& s)                          { Serialize(s); }

	static Value::Void *Create(Stream& s)            { return new RichValueRep(s); }
	static const RichValueRep *Cast(const Value::Void *p) {
		ASSERT_(dynamic_cast<const RichValueRep *>(p),
		        String().Cat() << "Invalid value conversion: "
		                       << typeid(*p).name() << " -> " << typeid(T).name());
		return (const RichValueRep *) p;
	}
};

template <class T>
class RichValue : public Value {
protected:
	typedef RichValueRep<T> Rep;

	static const T& GetNull() {
		static T *q;
		ONCELOCK {
			static T x; SetNull(x);
			q = &x;
		}
		return *q;
	}

public:
	RichValue(const T& x) : Value(new Rep(x)) {}
	static void Register() init_ {
		Value::Register(GetValueTypeNo<T>(), Rep::Create);
	}
	static const T& Extract(const Value& v) {
		if(v.IsNull()) return GetNull();
		return Rep::Cast(v.GetVoidPtr())->Get();
	}
};

template <class T>
inline Value RichToValue(const T& data)                   { return RichValue<T>(data); }

Value        ErrorValue(const char *s);
Value        ErrorValue(const String& s);
const Value& ErrorValue();

String       GetErrorText(const Value& v);

inline bool          IsNull(const Value& v)               { return v.IsNull(); }
inline const Value&  Nvl(const Value& a, const Value& b)  { return IsNull(a) ? b : a; }
