

#ifdef flagTEST_UNS
#include <UNSCtrl/UNSCtrl.h>

NAMESPACE_UPP

#define LLOG(x) //LOG(x)

//for external tests - not used with upp CtrlCore

Ctrl::Ctrl()
{
	nswnd = NULL;
	ispopup = false;
	cocWndRef = NULL;
}

Ctrl::~Ctrl()
{
	nswnd = NULL;
}

void Ctrl::MouseDrag(Point p, dword)              {}


END_UPP_NAMESPACE

#endif


