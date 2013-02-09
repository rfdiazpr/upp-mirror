#ifndef _InVector_InVector_h_
#define _InVector_InVector_h_

#include <Core/Core.h>
#include <set>

using namespace Upp;

#define LLOG(x)   // DLOG(x)

#define ADAPTIVE

template <class T>
class InVector {
	Vector< Vector<T> > data;
	Vector< Vector<int> > index;
	int   count;
	int   hcount;
	int64 serial;
	int   blk_high;
	int   blk_low;

	int  FindBlock0(int& pos, int& off) const;
	int  FindBlock(int& pos, int& off) const;
	int  FindBlock(int& pos) const;
	void Reindex();

	template <class L>
	int  FindUpperBound(const T& val, const L& less, int& off, int& pos);

	template <class L>
	int  FindLowerBound(const T& val, const L& less, int& off, int& pos);

	bool JoinSmall(int blki);
	T   *Insert0(int ii, int blki, int pos, const T *val);
	T   *Insert(int ii, const T *val);
	
	void Reset();

public:
	InVector();
	const T& operator[](int i) const;
	T& operator[](int i);
	int  GetCount() const      { return count; }

	T&   Insert(int ii)                             { return *Insert(ii, NULL); }
	void Remove(int ii, int count);
	void Clear();
	
	template <class L>
	int FindUpperBound(const T& val, const L& less) { int off, pos; FindUpperBound(val, less, off, pos); return off + pos; }
	int FindUpperBound(const T& val)                { return FindUpperBound(val, StdLess<T>()); }

	template <class L>
	int FindLowerBound(const T& val, const L& less) { int off, pos; FindLowerBound(val, less, off, pos); return off + pos; }
	int FindLowerBound(const T& val)                { return FindLowerBound(val, StdLess<T>()); }

	template <class L> int InsertUpperBound(const T& val, const L& less);
	int InsertUpperBound(const T& val) { return InsertUpperBound(val, StdLess<T>()); }
	
	template <class L> int Find(const T& val, const L& less);
	int Find(const T& val)                          { return Find(val, StdLess<T>()); }

	typedef T        ValueType;

	class ConstIterator  {
		T        *ptr;
		T        *begin;
		T        *end;
		InVector *v;
		int       offset;
		int       blki;

		friend class InVector;

		void NextBlk();
		void PrevBlk();		

	public:
		force_inline int GetIndex() const              { return ptr - begin + offset; }

		force_inline ConstIterator& operator++()       { ASSERT(ptr); if(++ptr == end) NextBlk(); return *this; }
		force_inline ConstIterator& operator--()       { ASSERT(ptr); if(ptr == begin) PrevBlk(); --ptr; return *this; }
		force_inline ConstIterator  operator++(int)    { ConstIterator t = *this; ++*this; return t; }
		force_inline ConstIterator  operator--(int)    { ConstIterator t = *this; --*this; return t; }

		force_inline ConstIterator& operator+=(int d);
		ConstIterator& operator-=(int d)               { return operator+=(-d); }

		ConstIterator operator+(int d) const           { ConstIterator t = *this; t += d; return t; }
		ConstIterator operator-(int d) const           { return operator+(-d); }

		int  operator-(const ConstIterator& x) const   { return GetIndex() - x.GetIndex(); }

		bool operator==(const ConstIterator& b) const  { return ptr == b.ptr; }
		bool operator!=(const ConstIterator& b) const  { return ptr != b.ptr; }
		bool operator<(const ConstIterator& b) const   { return blki == b.blki ? ptr < b.ptr : blki < b.blki; }
		bool operator>(const ConstIterator& b) const   { return blki == b.blki ? ptr > b.ptr : blki > b.blki; }
		bool operator<=(const ConstIterator& b) const  { return blki == b.blki ? ptr <= b.ptr : blki <= b.blki; }
		bool operator>=(const ConstIterator& b) const  { return blki == b.blki ? ptr >= b.ptr : blki >= b.blki; }

		operator bool() const                          { return ptr; }

		const T& operator*() const                     { return *ptr; }
		const T *operator->() const                    { return ptr; }
		const T& operator[](int i) const               { ConstIterator h = *this; h += i; return *h; }
	};

	class Iterator : public ConstIterator {
		typedef ConstIterator B;
	public:
		Iterator& operator++()                         { ConstIterator::operator++(); return *this; }
		Iterator& operator--()                         { ConstIterator::operator--(); return *this; }
		Iterator  operator++(int)                      { Iterator t = *this; ++*this; return t; }
		Iterator  operator--(int)                      { Iterator t = *this; --*this; return t; }

		Iterator& operator+=(int d)                    { ConstIterator::operator+=(d); return *this; }
		Iterator& operator-=(int d)                    { return operator+=(-d); }

		Iterator operator+(int d) const                { Iterator t = *this; t += d; return t; }
		Iterator operator-(int d) const                { return operator+(-d); }

		int  operator-(const Iterator& x) const        { return B::GetIndex() - x.GetIndex(); }

		T& operator*()                                 { return *B::ptr; }
		T *operator->()                                { return B::ptr; }
		T& operator[](int i)                           { Iterator h = *this; h += i; return *h; }

		const T& operator*() const                     { return *B::ptr; }
		const T *operator->() const                    { return B::ptr; }
		const T& operator[](int i) const               { ConstIterator h = *this; h += i; return *h; }
	};

	void SetIter(ConstIterator& it, int ii);
	void SetBegin(ConstIterator& it);
	void SetEnd(ConstIterator& it);

	ConstIterator    Begin() const              { ConstIterator it; SetBegin(it); return it; }
	ConstIterator    End() const                { ConstIterator it; SetEnd(it); return it; }
	ConstIterator    GetIter(int pos) const     { ConstIterator it; SetIter(it, pos); return it; }

	Iterator         Begin()                    { Iterator it; SetBegin(it); return it; }
	Iterator         End()                      { Iterator it; SetEnd(it); return it; }
	Iterator         GetIter(int pos)           { Iterator it; SetIter(it, pos); return it; }

#ifdef _DEBUG
	void DumpIndex();
#endif
	void Info() const { RDUMP(data.GetCount()); RDUMP(blk_high); }
};

#include "InVector.hpp"
#include "Bound.hpp"
#include "Iterator.hpp"

#endif
