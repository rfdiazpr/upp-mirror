#include <Core/Core.h>

int Pow2Bound(int i);

enum { UNSIGNED_HIBIT = 0x80000000 };

class HashBase2 {
	struct Link {
		int   next;
		int   prev;
	};
	Vector<unsigned>      hash;
	mutable Vector<Link>  link;
	mutable int          *map;
	mutable int           mask;
	mutable int           unlinked;

	void  LinkBefore(int i, Link& l, int bi) const;
	void  LinkTo(int i, Link& l, int& m) const;
	void  Unlink(int i, Link& l, int& mi);
	void  Unlink(int i, Link& l);
	int&  Maph(unsigned _hash) const { return map[int(_hash & mask)]; }
	int&  Mapi(int i) const          { return map[int(hash[i] & mask)]; }
	void  FinishIndex() const;
	void  DoIndex() const;
	void  Free() const;

public:
	void  ClearIndex() const;
	void  Reindex(int n) const;
	void  Reindex() const;

	void  Add(unsigned hash);
	void  Set(int i, unsigned hash);
	void  SetUn(int i, unsigned hash);
	int   Find(unsigned hash) const;
	int   FindNext(int i) const;
	int   FindLast(unsigned hash) const;
	int   FindPrev(int i) const;
	int   Put(unsigned hash);

	bool  IsUnlinked(int i) const           { return hash[i] & UNSIGNED_HIBIT; }
	void  Unlink(int i);
	Vector<int> GetUnlinked() const;

	void  Remove(int i);
	void  Remove(const int *sorted_list, int count);
	void  Insert(int i, unsigned hash);

	int   GetCount() const                  { return hash.GetCount(); }
	void  Trim(int count);
	void  Drop(int n);
	void  Clear()                           { hash.Clear(); ClearIndex(); }

	void  Reserve(int n);
	void  Shrink();

	void  Serialize(Stream& s);

	HashBase2();
	~HashBase2();

	HashBase2(const HashBase2& b, int);
	void operator<<=(const HashBase2& b);

	bool  IsPicked() const                  { return hash.IsPicked(); }

	const unsigned *Begin() const           { return hash.Begin(); }
	const unsigned *End() const             { return hash.End(); }
};

template class Moveable<HashBase2>;
template class Moveable<HashBase2::Link>;

template <class T, class V, class HashFn>
class AIndex2 {
protected:
	HashFn    hashfn;
	V         key;
	HashBase2  hash;

	int      Find0(const T& x, int i) const {
		while(i >= 0 && !(x == key[i])) i = hash.FindNext(i);
		return i;
	}
	int      FindB(const T& x, int i) const {
		while(i >= 0 && !(x == key[i])) i = hash.FindPrev(i);
		return i;
	}
	void     Hash();

public:
	void     Add(const T& x, unsigned _hash);
	void     Add(const T& x);
	int      FindAdd(const T& key, unsigned _hash);
	int      FindAdd(const T& key);

	int      Put(const T& x, unsigned _hash);
	int      Put(const T& x);
	int      FindPut(const T& key, unsigned _hash);
	int      FindPut(const T& key);

	int      Find(const T& x, unsigned _hash) const;
	int      Find(const T& x) const;
	int      FindNext(int i) const;
	int      FindLast(const T& x, unsigned _hash) const;
	int      FindLast(const T& x) const;
	int      FindPrev(int i) const;

	void     Set(int i, const T& x, unsigned _hash);
	void     Set(int i, const T& x);

	void     SetUn(int i, const T& x, unsigned _hash);
	void     SetUn(int i, const T& x);
	
	const T& operator[](int i) const         { return key[i]; }
	int      GetCount() const                { return key.GetCount(); }
	bool     IsEmpty() const                 { return key.IsEmpty(); }

	void     Clear()                         { hash.Clear(); key.Clear(); }

	void     ClearIndex() const              { hash.ClearIndex(); }
	void     Reindex(int n) const            { hash.Reindex(n); }
	void     Reindex() const                 { hash.Reindex(); }

	void     Unlink(int i)                   { hash.Unlink(i); }
	int      UnlinkKey(const T& k, unsigned h);
	int      UnlinkKey(const T& k);
	bool     IsUnlinked(int i) const         { return hash.IsUnlinked(i); }
	void     Sweep();
	
	void     Insert(int i, const T& k, unsigned h);
	void     Insert(int i, const T& k);
	void     Remove(int i);
	void     Remove(const int *sorted_list, int count);
	void     Remove(const Vector<int>& sorted_list);
	int      RemoveKey(const T& k, unsigned h);
	int      RemoveKey(const T& k);

	void     Trim(int n)                     { key.SetCount(n); hash.Trim(n); }
	void     Drop(int n = 1)                 { key.Drop(n); hash.Drop(n); }
	const T& Top() const                     { return key.Top(); }

	void     Reserve(int n)                  { key.Reserve(n); hash.Reserve(n); } 
	void     Shrink()                        { key.Shrink(); hash.Shrink(); } 
	int      GetAlloc() const                { return key.GetAlloc(); }
	
	void     Serialize(Stream& s);
	
	V        PickKeys()                       { return key; }
	const V& GetKeys() const                  { return key; }

// Pick assignment & copy. Picked source can only Clear(), ~AIndex2(), operator=, operator<<=

	AIndex2& operator=(pick_ V& s);
	AIndex2& operator<<=(const V& s);

// Standard container interface
	typedef T                ValueType;
	typedef typename V::ConstIterator ConstIterator;
	ConstIterator  Begin() const                          { return key.Begin(); }
	ConstIterator  End() const                            { return key.End(); }
	ConstIterator  GetIter(int pos) const                 { return key.GetIter(pos); }

// Optimalizations
	friend int  GetCount(const AIndex2& v)                   { return v.GetCount(); }

protected:
	AIndex2(pick_ V& s);
	AIndex2(const V& s, int);
	AIndex2() {}
	AIndex2(const AIndex2& s, int);
};

template <class T, class HashFn = StdHash<T> >
class Index2 : public AIndex2<T, Vector<T>, HashFn> {
	typedef AIndex2< T, Vector<T>, HashFn > B;
public:
	T        Pop()                           { T x = Top(); Drop(); return x; }

	Index2() {}
	Index2(pick_ Index2& s) : B(s)             {}
	Index2(const Index2& s, int) : B(s, 1)     {}
	Index2(pick_ Vector<T>& s) : B(s)         {}
	Index2(const Vector<T>& s, int) : B(s, 1) {}
	
	Index2& operator=(pick_ Vector<T>& x)     { B::operator=(x); return *this; }

	DeepCopyOptionTemplate(Index2)
	MoveableTemplate(Index2)
};

