#ifndef _InVector_InVector_h_
#define _InVector_InVector_h_

#include <Core/Core.h>
#include <set>

using namespace Upp;

template <class T, class Less = StdLess<T> >
class SortedIndex {
	InVector<T> iv;
	Less less;

public:
	int           Add(const T& x)                  { return iv.InsertUpperBound(x, less); }
	int           FindAdd(const T& key);
	SortedIndex&  operator<<(const T& x)           { Add(x); return *this; }
	
	int           FindLowerBound(const T& x) const { return iv.FindLowerBound(x, less); }
	int           FindUpperBound(const T& x) const { return iv.FindUpperBound(x, less); }

	int           Find(const T& x) const           { return iv.Find(x, less); }
	int           FindNext(int i) const;
	int           FindLast(const T& x) const;
	int           FindPrev(int i) const;

	void          Remove(int i)                    { iv.Remove(i); }
	void          Remove(int i, int count)         { iv.Remove(i, count); }
	int           RemoveKey(const T& x);

	const T&      operator[](int i) const          { return iv[i]; }
	int           GetCount() const                 { return iv.GetCount(); }
	bool          IsEmpty() const                  { return iv.IsEmpty(); }
	void          Clear()                          { iv.Clear(); }

	void          Trim(int n)                      { return iv.Trim(n); }
	void          Drop(int n = 1)                  { iv.Drop(n); }
	const T&      Top() const                      { return iv.Top(); }

	void          Shrink()                         { iv.Shrink(); }
	
	typedef typename InVector<T>::Iterator      Iterator;
	typedef typename InVector<T>::ConstIterator ConstIterator;
	
	typedef T        ValueType;

	ConstIterator    Begin() const                  { return iv.Begin(); }
	ConstIterator    End() const                    { return iv.End(); }
	ConstIterator    GetIter(int pos) const         { return iv.GetIter(pos); }

	Iterator         Begin()                        { return iv.Begin(); }
	Iterator         End()                          { return iv.End(); }
	Iterator         GetIter(int pos)               { return iv.GetIter(pos); }
};

#include "InVector.hpp"

#endif
