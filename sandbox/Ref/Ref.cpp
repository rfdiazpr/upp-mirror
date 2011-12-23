#include <Core/Core.h>

using namespace Upp;

#define Ref Ref2

class Ref {
	struct RefType {
		virtual Value Get(void *) const              {}
		virtual void  Set(void *, const Value& v)    {}
	};
	template <class T>
	struct RefValueType : RefType {
		virtual Value Get(void *ptr)                 { return *(T*)ptr; }
		virtual void  Set(void *ptr, const Value& v) { *(T*)ptr = v; }
	}
	void    *ptr;
	RefType& type;

public:
	void SetValue(const Value& v) { type.Set(ptr, v); }
	void GetValue(

	template <class T> Ref(T& var) : type(Single<RefType>()), ptr(&var) {}
};

CONSOLE_APP_MAIN
{
	
}
