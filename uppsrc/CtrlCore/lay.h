#pragma BLITZ_APPROVE

#define LAYOUT(name, x, y)       struct name##__layid {};
#define UNTYPED(variable, param)
#define ITEM(classname, var, param)
#define END_LAYOUT

#include LAYOUTFILE

#undef  LAYOUT
#undef  UNTYPED
#undef  ITEM
#undef  END_LAYOUT

#define LAYOUT(name, x, y)       template<class T> \
	                             struct With##name : public T, public name##__layid { \
										static UPP::Size GetLayoutSize() { return UPP::Ctrl::LayoutZoom(x, y); }
#define UNTYPED(variable, param)
#define ITEM(classname, var, param)     classname var;
#define END_LAYOUT               };

#include LAYOUTFILE

#undef  LAYOUT
#undef  UNTYPED
#undef  ITEM
#undef  END_LAYOUT

#define LAYOUT(nm, x, y)       template <class L, class D> \
                               void InitLayout(UPP::Ctrl& parent, L& layout, D& uts, nm##__layid&) {
#define UNTYPED(var, param)       uts.var.param; parent.Add(uts.var);
#define ITEM(clss, var, param)    layout.var.param; parent.Add(layout.var);
#define END_LAYOUT             };

#include LAYOUTFILE

#undef  LAYOUT
#undef  UNTYPED
#undef  ITEM
#undef  END_LAYOUT

#undef  LAYOUTFILE
