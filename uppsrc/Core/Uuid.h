struct Uuid : AssignValueTypeNo<Uuid, 50, Moveable<Uuid> > {
	dword a, b, c, d;

	void Serialize(Stream& s);
	bool IsNull() const         { return a == 0 && b == 0 && c == 0 && d == 0; }
	void SetNull()              { a = b = c = d = 0; }

	operator Value() const      { return RichValue<Uuid>(*this); }
	Uuid(const Value& q)        { *this = RichValue<Uuid>::Extract(q); }
	Uuid(const Nuller&)         { SetNull(); }
	Uuid()                      {}

	static Uuid Create();
};

String Format(const Uuid& id);

template<>
inline unsigned GetHashValue(const Uuid& u) { return u.a + u.b + u.c + u.d; }

inline bool  operator==(const Uuid& u, const Uuid& w) {
	return u.a == w.a && u.b == w.b && u.c == w.c && u.d == w.d;
}

inline bool  operator!=(const Uuid& u, const Uuid& w) {
	return !(u == w);
}

template<>
inline bool IsNull(const Uuid& id) { return id.IsNull(); }

template<>
inline String AsString(const Uuid& id) { return Format(id); }

ValueGen& UuidValueGen();
