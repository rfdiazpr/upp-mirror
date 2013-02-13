#ifndef _InVector_InVector_h_
#define _InVector_InVector_h_

#include <Core/Core.h>
#include <set>

using namespace Upp;

namespace Upp {

template <class K, class T, class Less>
class SortedVectorMap : MoveableAndDeepCopyOption< SortedVectorMap<K, T, Less> >
{
	SortedIndex<K, Less> key;
	
	struct Data : InVectorSlave__ {
		Vector< Vector<T> > data;
		const T *ptr;
	      
		virtual void Clear();
		virtual void Insert(int blki, int pos);
		virtual void Split(int blki, int nextsize);
		virtual void AddFirst();
		virtual void RemoveBlk(int blki, int n);
		virtual void Join(int blki);
		virtual void Remove(int blki, int pos, int n);
	};
	Data  value;
	Less  less;
	
	T&       At(int i) const                        { int blki = key.iv.FindBlock(i); return *(T*)&value.data[blki][i]; }

public:
	T&       Add(const K& k, const T& x)            { value.ptr = &x; key.Add(k); return *(T*)value.ptr; }
	T&       Add(const K& k)                        { value.ptr = NULL; key.Add(k); return *(T*)value.ptr; }

	int      FindLowerBound(const K& k) const       { return key.FindLowerBound(k); }
	int      FindUpperBound(const K& k) const       { return key.FindUpperBound(k); }

	int      Find(const K& k) const                 { return key.Find(k); }
	int      FindNext(int i) const                  { return key.FindNext(i); }
	int      FindLast(const K& k) const             { return key.FindLast(k); }
	int      FindPrev(int i) const                  { return key.FindPrev(i); }

	int      FindAdd(const K& k)                    { value.ptr = NULL; return key.FindAdd(k); }
	int      FindAdd(const K& k, const T& init)     { value.ptr = &init; return key.FindAdd(k); }

	T&       Get(const K& k)                        { return At(Find(k)); }
	const T& Get(const K& k) const                  { return At(Find(k)); }
	const T& Get(const K& k, const T& d) const      { int i = Find(k); return i >= 0 ? value[i] : d; }

	T&       GetAdd(const K& k)                     { return At(FindAdd(k)); }
	T&       GetAdd(const K& k, const T& x)         { return At(FindAdd(k, x)); }

	T       *FindPtr(const K& k)                    { int i = Find(k); return i >= 0 ? &value[i] : NULL; }
	const T *FindPtr(const K& k) const              { int i = Find(k); return i >= 0 ? &value[i] : NULL; }

	const K& GetKey(int i) const                    { return key[i]; }
	const T& operator[](int i) const                { return At(i); }
	T&       operator[](int i)                      { return At(i); }
	int      GetCount() const                       { return key.GetCount(); }
	bool     IsEmpty() const                        { return key.IsEmpty(); }
	void     Clear()                                { key.Clear(); }
	void     Shrink();

	void     Remove(int i)                         { key.Remove(i); }
	void     Remove(int i, int count)              { key.Remove(i, count); }
	int      RemoveKey(const K& k)                 { return key.RemoveKey(k); }

	void     Drop(int n = 1)                       { key.Drop(n); }
//	T&       Top()                                 { return value.Top(); }
//	const T& Top() const                           { return value.Top(); }
	const K& TopKey() const                        { return key.Top(); }
	K        PopKey()                              { K h = TopKey(); Drop(); return h; }
	void     Trim(int n)                           { key.Trim(n); }

	void     Swap(SortedVectorMap& x)              { Swap(value, x.value); Swap(key, x.key); }
	
	bool     IsPicked() const                      { return value.IsPicked() || key.IsPicked(); }
	
	const SortedIndex<K>& GetKeys() const          { return key; }
//	const InVector<T>& GetValues() const           { return value; }

	SortedVectorMap()                              { key.iv.slave = &value; }

/*
	AMap(const AMap& s, int) : key(s.key, 0), value(s.value, 0) {}

	typedef K        KeyType;
	typedef typename Index<K, HashFn>::ConstIterator KeyConstIterator;

	KeyConstIterator KeyBegin() const                             { return key.Begin(); }
	KeyConstIterator KeyEnd() const                               { return key.End(); }
	KeyConstIterator KeyGetIter(int pos) const                    { return key.GetIter(pos); }

	typedef T                          ValueType;
	typedef typename V::ConstIterator  ConstIterator;
	typedef typename V::Iterator       Iterator;

	Iterator         Begin()                                      { return value.Begin(); }
	Iterator         End()                                        { return value.End(); }
	Iterator         GetIter(int pos)                             { return value.GetIter(pos); }
	ConstIterator    Begin() const                                { return value.Begin(); }
	ConstIterator    End() const                                  { return value.End(); }
	ConstIterator    GetIter(int pos) const                       { return value.GetIter(pos); }

	friend int     GetCount(const AMap& v)                        { return v.GetCount(); }
*/
};

#include "InVector.hpp"

}

#endif
