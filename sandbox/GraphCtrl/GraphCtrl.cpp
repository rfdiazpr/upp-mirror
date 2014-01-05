#include "GraphCtrl.h"

#define IMAGECLASS GraphCtrlImg
#define IMAGEFILE <GraphCtrl/GraphCtrl.iml>
#include <Draw/iml_source.h>

using namespace Upp;

dword GraphCtrl_Keys::K_AXIS_SELECT = K_SHIFT;
dword GraphCtrl_Keys::K_ZOOM        = K_CTRL;
dword GraphCtrl_Keys::K_SCROLL      = 0;

dword GraphCtrl_Keys::K_AXIS_ZOOM   = GraphCtrl_Keys::K_ZOOM   | GraphCtrl_Keys::K_AXIS_SELECT;
dword GraphCtrl_Keys::K_AXIS_SCROLL = GraphCtrl_Keys::K_SCROLL | GraphCtrl_Keys::K_AXIS_SELECT;

dword GraphCtrl_Keys::K_ELEMENT_FLOAT_MOVERESIZE = K_CTRL;
dword GraphCtrl_Keys::K_ELEMENT_FLOAT_MOVE       = 0;
