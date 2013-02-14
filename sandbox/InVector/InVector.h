#ifndef _InVector_InVector_h_
#define _InVector_InVector_h_

#include <Core/Core.h>
#include <set>

using namespace Upp;

namespace Upp {

template <class K, class T, class Less, class Data>
class SortedAMap : MoveableAndDeepCopyOption< SortedAMap<K, T, Less, Data> >
{
	SortedIndex<K, Less> key;
	
	Data     value;

	T&       At(int i) const { int blki = key.iv.FindBlock(i); return value.Get(blki, i); }

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
	void     Shrink()                               { key.Shrink(); value.Shrink(); }

	void     Remove(int i)                          { key.Remove(i); }
	void     Remove(int i, int count)               { key.Remove(i, count); }
	int      RemoveKey(const K& k)                  { return key.RemoveKey(k); }

	void     Drop(int n = 1)                        { key.Drop(n); }
	T&       Top()                                  { return value.Top(); }
	const T& Top() const                            { return value.Top(); }
	const K& TopKey() const                         { return key.Top(); }
	K        PopKey()                               { K h = TopKey(); Drop(); return h; }
	void     Trim(int n)                            { key.Trim(n); }

	void     Swap(SortedAMap& x)                    { Swap(value, x.value); Swap(key, x.key); }
	
	bool     IsPicked() const                       { return value.IsPicked() || key.IsPicked(); }
	
	const SortedIndex<K>& GetKeys() const           { return key; }
	const InVector<T>& GetValues() const            { return value; }

	SortedAMap()                               { key.iv.slave = &value; }

	SortedAMap(const SortedAMap& s, int);

	typedef K        KeyType;

	typedef typename SortedIndex<K, Less>::ConstIterator KeyConstIterator;

	KeyConstIterator KeyBegin() const                             { return key.Begin(); }
	KeyConstIterator KeyEnd() const                               { return key.End(); }
	KeyConstIterator KeyGetIter(int pos) const                    { return key.GetIter(pos); }

	typedef T                                   ValueType;
	typedef typename Data::Type::ConstIterator  ConstIterator;
	typedef typename Data::Type::Iterator       Iterator;

	Iterator         Begin()                                      { return value.data.Begin(); }
	Iterator         End()                                        { return value.data.End(); }
	Iterator         GetIter(int pos)                             { return value.data.GetIter(pos); }
	ConstIterator    Begin() const                                { return value.data.Begin(); }
	ConstIterator    End() const                                  { return value.data.End(); }
	ConstIterator    GetIter(int pos) const                       { return value.data.GetIter(pos); }
};

template <class T>
struct Slaved_InVector__ : InVectorSlave__ {
	typedef InVector<T> Type;
	InVector<T> data;
	const T *ptr;
      
	virtual void Clear()                          { data.Clear(); }
	virtual void Count(int n)                     { data.count += n; }
	virtual void Insert(int blki, int pos);
	virtual void Split(int blki, int nextsize);
	virtual void AddFirst();
	virtual void RemoveBlk(int blki, int n)       { data.data.Remove(blki, n); }
	virtual void Join(int blki)                   { data.Join(blki); }
	virtual void Remove(int blki, int pos, int n) { data.data[blki].Remove(pos, n); }
	virtual void Index(int blki, int n)           { data.Index(blki, n); }
	virtual void Reindex()                        { data.Reindex(); }

	T& Get(int blki, int i) const                 { return *(T*)&data.data[blki][i]; }
};

template <class K, class T, class Less = StdLess<K> >
class SortedVectorMap : public SortedAMap<K, T, Less, Slaved_InVector__<T> >
{
};

template <class T>
struct Slaved_InArray__ : InVectorSlave__ {
	typedef InArray<T> Type;
	InArray<T> data;
	const T *ptr;
      
	virtual void Clear()                          { data.Clear(); }
	virtual void Count(int n)                     { data.iv.count += n; }
	virtual void Insert(int blki, int pos);
	virtual void Split(int blki, int nextsize);
	virtual void AddFirst();
	virtual void RemoveBlk(int blki, int n)       { data.iv.data.Remove(blki, n); }
	virtual void Join(int blki)                   { data.iv.Join(blki); }
	virtual void Remove(int blki, int pos, int n) { data.iv.data[blki].Remove(pos, n); }
	virtual void Index(int blki, int n)           { data.iv.Index(blki, n); }
	virtual void Reindex()                        { data.iv.Reindex(); }

	T& Get(int blki, int i) const                 { return *(T*)data.iv.data[blki][i]; }
};


template <class K, class T, class Less = StdLess<K> >
class SortedArrayMap : public SortedAMap<K, T, Less, Slaved_InArray__<T> >
{
};

#include "InVector.hpp"

}

#endif
