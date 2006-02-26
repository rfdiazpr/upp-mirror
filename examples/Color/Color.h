#ifndef _Color_Color_h
#define _Color_Color_h

#include <CtrlLib/CtrlLib.h>

class ColorWindow : public TopWindow {
public:
	typedef Color CLASSNAME;
	ColorWindow();

	virtual void Paint(Draw& draw);
};

#endif
