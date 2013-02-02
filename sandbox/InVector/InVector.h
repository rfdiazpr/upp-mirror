#ifndef _InVector_InVector_h_
#define _InVector_InVector_h_

#include <Core/Core.h>
#include <set>

using namespace Upp;

#define LLOG(x)   // DLOG(x)

#define USECACHE2
#define REINDEX2
//#define IITERATOR

template <class T>
class InVector {
	Vector< Vector<T> > data;
	Vector< Vector<int> > index;
	int   count;
	int   hcount;
#ifdef USECACHE
	int64 serial1, serial2;
#endif

#ifdef USECACHE2
	int64 serial;
#endif
	
#ifdef _DEBUG
	enum { BLKSIZE = 10 };
#else
	enum { BLKSIZE = 12000 / sizeof(T) > 16 ? 12000 / sizeof(T) : 16; };
#endif

#ifdef USECACHE2
	int  FindBlock0(int& pos, int& off) const;
#endif
	int  FindBlock(int& pos, int& off) const;
	int  FindBlock(int& pos) const;
	void Reindex();

	template <class L>
	int  FindUpperBound(const T& val, const L& less, int& off, int& pos);

	template <class L>
	int  FindLowerBound(const T& val, const L& less, int& off, int& pos);

public:
	InVector();
	void Dump();
	T&   Insert(int ii);
	const T& operator[](int i) const;
	T& operator[](int i);
	int  GetCount() const      { return count; }
	
	template <class L>
	int FindUpperBound(const T& val, const L& less) { int off, pos; FindUpperBound(val, less, off, pos); return off + pos; }
	
	int FindUpperBound(const T& val) { return FindUpperBound(val, StdLess<T>()); }

	template <class L>
	int FindLowerBound(const T& val, const L& less) { int off, pos; FindLowerBound(val, less, off, pos); return off + pos; }
	
	int FindLowerBound(const T& val) { return FindLowerBound(val, StdLess<T>()); }
	
	typedef T        ValueType;

#ifdef IITERATOR
	typedef ConstIIterator<InVector> ConstIterator;
	typedef IIterator<InVector>      Iterator;

	ConstIterator    Begin() const              { return ConstIterator(*this, 0); }
	ConstIterator    End() const                { return ConstIterator(*this, GetCount()); }
	ConstIterator    GetIter(int pos) const     { return ConstIterator(*this, pos); }
	Iterator         Begin()                    { return Iterator(*this, 0); }
	Iterator         End()                      { return Iterator(*this, GetCount()); }
	Iterator         GetIter(int pos)           { return Iterator(*this, pos); }
#else
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
#endif
};

#include "InVector.hpp"

#endif
